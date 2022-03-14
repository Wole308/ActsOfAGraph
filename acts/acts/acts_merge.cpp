#define CONFIG_MERGE_VDATAS
#ifndef ALLVERTEXISACTIVE_ALGORITHM
#define CONFIG_MERGE_VPARTITIONS
#endif 

unsigned int acts_all::MERGEP0_actvpstatsoffset(globalparams_t globalparams){
	// this returns the stats of the last level of partitioning
	unsigned int _offset = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ _offset += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ _offset += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif
	return _offset;
}

void acts_all::MERGEP0_mergeVs1(uint512_dt * kvdram0, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs1 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 1:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs1: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 1:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs1---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs1: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs2 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 2:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 2:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 2:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 2:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask2_next[i].data[0] > 0){ cout<<i<<"(b2), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] | pmask2_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] | pmask2_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] | pmask2_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] | pmask2_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] | pmask2_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] | pmask2_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] | pmask2_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] | pmask2_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] | pmask2_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] | pmask2_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] | pmask2_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] | pmask2_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] | pmask2_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] | pmask2_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] | pmask2_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] | pmask2_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] | pmask2_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] | pmask2_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] | pmask2_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] | pmask2_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] | pmask2_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] | pmask2_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] | pmask2_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] | pmask2_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] | pmask2_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] | pmask2_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] | pmask2_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] | pmask2_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] | pmask2_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] | pmask2_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] | pmask2_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs3 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 3:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 3:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 3:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 3:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 3:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 3:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask2_next[i].data[0] > 0){ cout<<i<<"(b2), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask3_next[i].data[0] > 0){ cout<<i<<"(b3), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] | pmask2_next[i].data[1] | pmask3_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] | pmask2_next[i].data[2] | pmask3_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] | pmask2_next[i].data[3] | pmask3_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] | pmask2_next[i].data[4] | pmask3_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] | pmask2_next[i].data[5] | pmask3_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] | pmask2_next[i].data[6] | pmask3_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] | pmask2_next[i].data[7] | pmask3_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] | pmask2_next[i].data[8] | pmask3_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] | pmask2_next[i].data[9] | pmask3_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] | pmask2_next[i].data[10] | pmask3_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] | pmask2_next[i].data[11] | pmask3_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] | pmask2_next[i].data[12] | pmask3_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] | pmask2_next[i].data[13] | pmask3_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] | pmask2_next[i].data[14] | pmask3_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] | pmask2_next[i].data[15] | pmask3_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] | pmask2_next[i].data[16] | pmask3_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] | pmask2_next[i].data[17] | pmask3_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] | pmask2_next[i].data[18] | pmask3_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] | pmask2_next[i].data[19] | pmask3_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] | pmask2_next[i].data[20] | pmask3_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] | pmask2_next[i].data[21] | pmask3_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] | pmask2_next[i].data[22] | pmask3_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] | pmask2_next[i].data[23] | pmask3_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] | pmask2_next[i].data[24] | pmask3_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] | pmask2_next[i].data[25] | pmask3_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] | pmask2_next[i].data[26] | pmask3_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] | pmask2_next[i].data[27] | pmask3_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] | pmask2_next[i].data[28] | pmask3_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] | pmask2_next[i].data[29] | pmask3_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] | pmask2_next[i].data[30] | pmask3_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] | pmask2_next[i].data[31] | pmask3_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs4 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 4:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 4:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 4:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 4:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 4:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 4:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 4:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 4:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask2_next[i].data[0] > 0){ cout<<i<<"(b2), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask3_next[i].data[0] > 0){ cout<<i<<"(b3), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask4_next[i].data[0] > 0){ cout<<i<<"(b4), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] | pmask2_next[i].data[1] | pmask3_next[i].data[1] | pmask4_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] | pmask2_next[i].data[2] | pmask3_next[i].data[2] | pmask4_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] | pmask2_next[i].data[3] | pmask3_next[i].data[3] | pmask4_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] | pmask2_next[i].data[4] | pmask3_next[i].data[4] | pmask4_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] | pmask2_next[i].data[5] | pmask3_next[i].data[5] | pmask4_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] | pmask2_next[i].data[6] | pmask3_next[i].data[6] | pmask4_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] | pmask2_next[i].data[7] | pmask3_next[i].data[7] | pmask4_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] | pmask2_next[i].data[8] | pmask3_next[i].data[8] | pmask4_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] | pmask2_next[i].data[9] | pmask3_next[i].data[9] | pmask4_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] | pmask2_next[i].data[10] | pmask3_next[i].data[10] | pmask4_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] | pmask2_next[i].data[11] | pmask3_next[i].data[11] | pmask4_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] | pmask2_next[i].data[12] | pmask3_next[i].data[12] | pmask4_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] | pmask2_next[i].data[13] | pmask3_next[i].data[13] | pmask4_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] | pmask2_next[i].data[14] | pmask3_next[i].data[14] | pmask4_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] | pmask2_next[i].data[15] | pmask3_next[i].data[15] | pmask4_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] | pmask2_next[i].data[16] | pmask3_next[i].data[16] | pmask4_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] | pmask2_next[i].data[17] | pmask3_next[i].data[17] | pmask4_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] | pmask2_next[i].data[18] | pmask3_next[i].data[18] | pmask4_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] | pmask2_next[i].data[19] | pmask3_next[i].data[19] | pmask4_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] | pmask2_next[i].data[20] | pmask3_next[i].data[20] | pmask4_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] | pmask2_next[i].data[21] | pmask3_next[i].data[21] | pmask4_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] | pmask2_next[i].data[22] | pmask3_next[i].data[22] | pmask4_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] | pmask2_next[i].data[23] | pmask3_next[i].data[23] | pmask4_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] | pmask2_next[i].data[24] | pmask3_next[i].data[24] | pmask4_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] | pmask2_next[i].data[25] | pmask3_next[i].data[25] | pmask4_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] | pmask2_next[i].data[26] | pmask3_next[i].data[26] | pmask4_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] | pmask2_next[i].data[27] | pmask3_next[i].data[27] | pmask4_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] | pmask2_next[i].data[28] | pmask3_next[i].data[28] | pmask4_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] | pmask2_next[i].data[29] | pmask3_next[i].data[29] | pmask4_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] | pmask2_next[i].data[30] | pmask3_next[i].data[30] | pmask4_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] | pmask2_next[i].data[31] | pmask3_next[i].data[31] | pmask4_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs5 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 5:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 5:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 5:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 5:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 5:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 5:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 5:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 5:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 5:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 5:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask2_next[i].data[0] > 0){ cout<<i<<"(b2), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask3_next[i].data[0] > 0){ cout<<i<<"(b3), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask4_next[i].data[0] > 0){ cout<<i<<"(b4), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask5_next[i].data[0] > 0){ cout<<i<<"(b5), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] | pmask2_next[i].data[1] | pmask3_next[i].data[1] | pmask4_next[i].data[1] | pmask5_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] | pmask2_next[i].data[2] | pmask3_next[i].data[2] | pmask4_next[i].data[2] | pmask5_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] | pmask2_next[i].data[3] | pmask3_next[i].data[3] | pmask4_next[i].data[3] | pmask5_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] | pmask2_next[i].data[4] | pmask3_next[i].data[4] | pmask4_next[i].data[4] | pmask5_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] | pmask2_next[i].data[5] | pmask3_next[i].data[5] | pmask4_next[i].data[5] | pmask5_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] | pmask2_next[i].data[6] | pmask3_next[i].data[6] | pmask4_next[i].data[6] | pmask5_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] | pmask2_next[i].data[7] | pmask3_next[i].data[7] | pmask4_next[i].data[7] | pmask5_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] | pmask2_next[i].data[8] | pmask3_next[i].data[8] | pmask4_next[i].data[8] | pmask5_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] | pmask2_next[i].data[9] | pmask3_next[i].data[9] | pmask4_next[i].data[9] | pmask5_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] | pmask2_next[i].data[10] | pmask3_next[i].data[10] | pmask4_next[i].data[10] | pmask5_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] | pmask2_next[i].data[11] | pmask3_next[i].data[11] | pmask4_next[i].data[11] | pmask5_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] | pmask2_next[i].data[12] | pmask3_next[i].data[12] | pmask4_next[i].data[12] | pmask5_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] | pmask2_next[i].data[13] | pmask3_next[i].data[13] | pmask4_next[i].data[13] | pmask5_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] | pmask2_next[i].data[14] | pmask3_next[i].data[14] | pmask4_next[i].data[14] | pmask5_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] | pmask2_next[i].data[15] | pmask3_next[i].data[15] | pmask4_next[i].data[15] | pmask5_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] | pmask2_next[i].data[16] | pmask3_next[i].data[16] | pmask4_next[i].data[16] | pmask5_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] | pmask2_next[i].data[17] | pmask3_next[i].data[17] | pmask4_next[i].data[17] | pmask5_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] | pmask2_next[i].data[18] | pmask3_next[i].data[18] | pmask4_next[i].data[18] | pmask5_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] | pmask2_next[i].data[19] | pmask3_next[i].data[19] | pmask4_next[i].data[19] | pmask5_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] | pmask2_next[i].data[20] | pmask3_next[i].data[20] | pmask4_next[i].data[20] | pmask5_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] | pmask2_next[i].data[21] | pmask3_next[i].data[21] | pmask4_next[i].data[21] | pmask5_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] | pmask2_next[i].data[22] | pmask3_next[i].data[22] | pmask4_next[i].data[22] | pmask5_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] | pmask2_next[i].data[23] | pmask3_next[i].data[23] | pmask4_next[i].data[23] | pmask5_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] | pmask2_next[i].data[24] | pmask3_next[i].data[24] | pmask4_next[i].data[24] | pmask5_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] | pmask2_next[i].data[25] | pmask3_next[i].data[25] | pmask4_next[i].data[25] | pmask5_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] | pmask2_next[i].data[26] | pmask3_next[i].data[26] | pmask4_next[i].data[26] | pmask5_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] | pmask2_next[i].data[27] | pmask3_next[i].data[27] | pmask4_next[i].data[27] | pmask5_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] | pmask2_next[i].data[28] | pmask3_next[i].data[28] | pmask4_next[i].data[28] | pmask5_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] | pmask2_next[i].data[29] | pmask3_next[i].data[29] | pmask4_next[i].data[29] | pmask5_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] | pmask2_next[i].data[30] | pmask3_next[i].data[30] | pmask4_next[i].data[30] | pmask5_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] | pmask2_next[i].data[31] | pmask3_next[i].data[31] | pmask4_next[i].data[31] | pmask5_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs6 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 6:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 6:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask2_next[i].data[0] > 0){ cout<<i<<"(b2), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask3_next[i].data[0] > 0){ cout<<i<<"(b3), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask4_next[i].data[0] > 0){ cout<<i<<"(b4), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask5_next[i].data[0] > 0){ cout<<i<<"(b5), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask6_next[i].data[0] > 0){ cout<<i<<"(b6), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] | pmask2_next[i].data[1] | pmask3_next[i].data[1] | pmask4_next[i].data[1] | pmask5_next[i].data[1] | pmask6_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] | pmask2_next[i].data[2] | pmask3_next[i].data[2] | pmask4_next[i].data[2] | pmask5_next[i].data[2] | pmask6_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] | pmask2_next[i].data[3] | pmask3_next[i].data[3] | pmask4_next[i].data[3] | pmask5_next[i].data[3] | pmask6_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] | pmask2_next[i].data[4] | pmask3_next[i].data[4] | pmask4_next[i].data[4] | pmask5_next[i].data[4] | pmask6_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] | pmask2_next[i].data[5] | pmask3_next[i].data[5] | pmask4_next[i].data[5] | pmask5_next[i].data[5] | pmask6_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] | pmask2_next[i].data[6] | pmask3_next[i].data[6] | pmask4_next[i].data[6] | pmask5_next[i].data[6] | pmask6_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] | pmask2_next[i].data[7] | pmask3_next[i].data[7] | pmask4_next[i].data[7] | pmask5_next[i].data[7] | pmask6_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] | pmask2_next[i].data[8] | pmask3_next[i].data[8] | pmask4_next[i].data[8] | pmask5_next[i].data[8] | pmask6_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] | pmask2_next[i].data[9] | pmask3_next[i].data[9] | pmask4_next[i].data[9] | pmask5_next[i].data[9] | pmask6_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] | pmask2_next[i].data[10] | pmask3_next[i].data[10] | pmask4_next[i].data[10] | pmask5_next[i].data[10] | pmask6_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] | pmask2_next[i].data[11] | pmask3_next[i].data[11] | pmask4_next[i].data[11] | pmask5_next[i].data[11] | pmask6_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] | pmask2_next[i].data[12] | pmask3_next[i].data[12] | pmask4_next[i].data[12] | pmask5_next[i].data[12] | pmask6_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] | pmask2_next[i].data[13] | pmask3_next[i].data[13] | pmask4_next[i].data[13] | pmask5_next[i].data[13] | pmask6_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] | pmask2_next[i].data[14] | pmask3_next[i].data[14] | pmask4_next[i].data[14] | pmask5_next[i].data[14] | pmask6_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] | pmask2_next[i].data[15] | pmask3_next[i].data[15] | pmask4_next[i].data[15] | pmask5_next[i].data[15] | pmask6_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] | pmask2_next[i].data[16] | pmask3_next[i].data[16] | pmask4_next[i].data[16] | pmask5_next[i].data[16] | pmask6_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] | pmask2_next[i].data[17] | pmask3_next[i].data[17] | pmask4_next[i].data[17] | pmask5_next[i].data[17] | pmask6_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] | pmask2_next[i].data[18] | pmask3_next[i].data[18] | pmask4_next[i].data[18] | pmask5_next[i].data[18] | pmask6_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] | pmask2_next[i].data[19] | pmask3_next[i].data[19] | pmask4_next[i].data[19] | pmask5_next[i].data[19] | pmask6_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] | pmask2_next[i].data[20] | pmask3_next[i].data[20] | pmask4_next[i].data[20] | pmask5_next[i].data[20] | pmask6_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] | pmask2_next[i].data[21] | pmask3_next[i].data[21] | pmask4_next[i].data[21] | pmask5_next[i].data[21] | pmask6_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] | pmask2_next[i].data[22] | pmask3_next[i].data[22] | pmask4_next[i].data[22] | pmask5_next[i].data[22] | pmask6_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] | pmask2_next[i].data[23] | pmask3_next[i].data[23] | pmask4_next[i].data[23] | pmask5_next[i].data[23] | pmask6_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] | pmask2_next[i].data[24] | pmask3_next[i].data[24] | pmask4_next[i].data[24] | pmask5_next[i].data[24] | pmask6_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] | pmask2_next[i].data[25] | pmask3_next[i].data[25] | pmask4_next[i].data[25] | pmask5_next[i].data[25] | pmask6_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] | pmask2_next[i].data[26] | pmask3_next[i].data[26] | pmask4_next[i].data[26] | pmask5_next[i].data[26] | pmask6_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] | pmask2_next[i].data[27] | pmask3_next[i].data[27] | pmask4_next[i].data[27] | pmask5_next[i].data[27] | pmask6_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] | pmask2_next[i].data[28] | pmask3_next[i].data[28] | pmask4_next[i].data[28] | pmask5_next[i].data[28] | pmask6_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] | pmask2_next[i].data[29] | pmask3_next[i].data[29] | pmask4_next[i].data[29] | pmask5_next[i].data[29] | pmask6_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] | pmask2_next[i].data[30] | pmask3_next[i].data[30] | pmask4_next[i].data[30] | pmask5_next[i].data[30] | pmask6_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] | pmask2_next[i].data[31] | pmask3_next[i].data[31] | pmask4_next[i].data[31] | pmask5_next[i].data[31] | pmask6_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs7 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 7:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 7:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer7[i].value > 0){ cout<<"MERGE: globalstatsbuffer7["<<i<<"].key : "<<globalstatsbuffer7[i].key<<", globalstatsbuffer7["<<i<<"].value : "<<globalstatsbuffer7[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask2_next[i].data[0] > 0){ cout<<i<<"(b2), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask3_next[i].data[0] > 0){ cout<<i<<"(b3), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask4_next[i].data[0] > 0){ cout<<i<<"(b4), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask5_next[i].data[0] > 0){ cout<<i<<"(b5), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask6_next[i].data[0] > 0){ cout<<i<<"(b6), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask7_next[i].data[0] > 0){ cout<<i<<"(b7), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] | pmask7_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] | pmask2_next[i].data[1] | pmask3_next[i].data[1] | pmask4_next[i].data[1] | pmask5_next[i].data[1] | pmask6_next[i].data[1] | pmask7_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] | pmask2_next[i].data[2] | pmask3_next[i].data[2] | pmask4_next[i].data[2] | pmask5_next[i].data[2] | pmask6_next[i].data[2] | pmask7_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] | pmask2_next[i].data[3] | pmask3_next[i].data[3] | pmask4_next[i].data[3] | pmask5_next[i].data[3] | pmask6_next[i].data[3] | pmask7_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] | pmask2_next[i].data[4] | pmask3_next[i].data[4] | pmask4_next[i].data[4] | pmask5_next[i].data[4] | pmask6_next[i].data[4] | pmask7_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] | pmask2_next[i].data[5] | pmask3_next[i].data[5] | pmask4_next[i].data[5] | pmask5_next[i].data[5] | pmask6_next[i].data[5] | pmask7_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] | pmask2_next[i].data[6] | pmask3_next[i].data[6] | pmask4_next[i].data[6] | pmask5_next[i].data[6] | pmask6_next[i].data[6] | pmask7_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] | pmask2_next[i].data[7] | pmask3_next[i].data[7] | pmask4_next[i].data[7] | pmask5_next[i].data[7] | pmask6_next[i].data[7] | pmask7_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] | pmask2_next[i].data[8] | pmask3_next[i].data[8] | pmask4_next[i].data[8] | pmask5_next[i].data[8] | pmask6_next[i].data[8] | pmask7_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] | pmask2_next[i].data[9] | pmask3_next[i].data[9] | pmask4_next[i].data[9] | pmask5_next[i].data[9] | pmask6_next[i].data[9] | pmask7_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] | pmask2_next[i].data[10] | pmask3_next[i].data[10] | pmask4_next[i].data[10] | pmask5_next[i].data[10] | pmask6_next[i].data[10] | pmask7_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] | pmask2_next[i].data[11] | pmask3_next[i].data[11] | pmask4_next[i].data[11] | pmask5_next[i].data[11] | pmask6_next[i].data[11] | pmask7_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] | pmask2_next[i].data[12] | pmask3_next[i].data[12] | pmask4_next[i].data[12] | pmask5_next[i].data[12] | pmask6_next[i].data[12] | pmask7_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] | pmask2_next[i].data[13] | pmask3_next[i].data[13] | pmask4_next[i].data[13] | pmask5_next[i].data[13] | pmask6_next[i].data[13] | pmask7_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] | pmask2_next[i].data[14] | pmask3_next[i].data[14] | pmask4_next[i].data[14] | pmask5_next[i].data[14] | pmask6_next[i].data[14] | pmask7_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] | pmask2_next[i].data[15] | pmask3_next[i].data[15] | pmask4_next[i].data[15] | pmask5_next[i].data[15] | pmask6_next[i].data[15] | pmask7_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] | pmask2_next[i].data[16] | pmask3_next[i].data[16] | pmask4_next[i].data[16] | pmask5_next[i].data[16] | pmask6_next[i].data[16] | pmask7_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] | pmask2_next[i].data[17] | pmask3_next[i].data[17] | pmask4_next[i].data[17] | pmask5_next[i].data[17] | pmask6_next[i].data[17] | pmask7_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] | pmask2_next[i].data[18] | pmask3_next[i].data[18] | pmask4_next[i].data[18] | pmask5_next[i].data[18] | pmask6_next[i].data[18] | pmask7_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] | pmask2_next[i].data[19] | pmask3_next[i].data[19] | pmask4_next[i].data[19] | pmask5_next[i].data[19] | pmask6_next[i].data[19] | pmask7_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] | pmask2_next[i].data[20] | pmask3_next[i].data[20] | pmask4_next[i].data[20] | pmask5_next[i].data[20] | pmask6_next[i].data[20] | pmask7_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] | pmask2_next[i].data[21] | pmask3_next[i].data[21] | pmask4_next[i].data[21] | pmask5_next[i].data[21] | pmask6_next[i].data[21] | pmask7_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] | pmask2_next[i].data[22] | pmask3_next[i].data[22] | pmask4_next[i].data[22] | pmask5_next[i].data[22] | pmask6_next[i].data[22] | pmask7_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] | pmask2_next[i].data[23] | pmask3_next[i].data[23] | pmask4_next[i].data[23] | pmask5_next[i].data[23] | pmask6_next[i].data[23] | pmask7_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] | pmask2_next[i].data[24] | pmask3_next[i].data[24] | pmask4_next[i].data[24] | pmask5_next[i].data[24] | pmask6_next[i].data[24] | pmask7_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] | pmask2_next[i].data[25] | pmask3_next[i].data[25] | pmask4_next[i].data[25] | pmask5_next[i].data[25] | pmask6_next[i].data[25] | pmask7_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] | pmask2_next[i].data[26] | pmask3_next[i].data[26] | pmask4_next[i].data[26] | pmask5_next[i].data[26] | pmask6_next[i].data[26] | pmask7_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] | pmask2_next[i].data[27] | pmask3_next[i].data[27] | pmask4_next[i].data[27] | pmask5_next[i].data[27] | pmask6_next[i].data[27] | pmask7_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] | pmask2_next[i].data[28] | pmask3_next[i].data[28] | pmask4_next[i].data[28] | pmask5_next[i].data[28] | pmask6_next[i].data[28] | pmask7_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] | pmask2_next[i].data[29] | pmask3_next[i].data[29] | pmask4_next[i].data[29] | pmask5_next[i].data[29] | pmask6_next[i].data[29] | pmask7_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] | pmask2_next[i].data[30] | pmask3_next[i].data[30] | pmask4_next[i].data[30] | pmask5_next[i].data[30] | pmask6_next[i].data[30] | pmask7_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] | pmask2_next[i].data[31] | pmask3_next[i].data[31] | pmask4_next[i].data[31] | pmask5_next[i].data[31] | pmask6_next[i].data[31] | pmask7_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs8 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 8:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 8:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer7[i].value > 0){ cout<<"MERGE: globalstatsbuffer7["<<i<<"].key : "<<globalstatsbuffer7[i].key<<", globalstatsbuffer7["<<i<<"].value : "<<globalstatsbuffer7[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer8[i].value > 0){ cout<<"MERGE: globalstatsbuffer8["<<i<<"].key : "<<globalstatsbuffer8[i].key<<", globalstatsbuffer8["<<i<<"].value : "<<globalstatsbuffer8[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask2_next[i].data[0] > 0){ cout<<i<<"(b2), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask3_next[i].data[0] > 0){ cout<<i<<"(b3), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask4_next[i].data[0] > 0){ cout<<i<<"(b4), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask5_next[i].data[0] > 0){ cout<<i<<"(b5), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask6_next[i].data[0] > 0){ cout<<i<<"(b6), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask7_next[i].data[0] > 0){ cout<<i<<"(b7), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask8_next[i].data[0] > 0){ cout<<i<<"(b8), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] | pmask7_next[i].data[0] | pmask8_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] | pmask2_next[i].data[1] | pmask3_next[i].data[1] | pmask4_next[i].data[1] | pmask5_next[i].data[1] | pmask6_next[i].data[1] | pmask7_next[i].data[1] | pmask8_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] | pmask2_next[i].data[2] | pmask3_next[i].data[2] | pmask4_next[i].data[2] | pmask5_next[i].data[2] | pmask6_next[i].data[2] | pmask7_next[i].data[2] | pmask8_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] | pmask2_next[i].data[3] | pmask3_next[i].data[3] | pmask4_next[i].data[3] | pmask5_next[i].data[3] | pmask6_next[i].data[3] | pmask7_next[i].data[3] | pmask8_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] | pmask2_next[i].data[4] | pmask3_next[i].data[4] | pmask4_next[i].data[4] | pmask5_next[i].data[4] | pmask6_next[i].data[4] | pmask7_next[i].data[4] | pmask8_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] | pmask2_next[i].data[5] | pmask3_next[i].data[5] | pmask4_next[i].data[5] | pmask5_next[i].data[5] | pmask6_next[i].data[5] | pmask7_next[i].data[5] | pmask8_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] | pmask2_next[i].data[6] | pmask3_next[i].data[6] | pmask4_next[i].data[6] | pmask5_next[i].data[6] | pmask6_next[i].data[6] | pmask7_next[i].data[6] | pmask8_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] | pmask2_next[i].data[7] | pmask3_next[i].data[7] | pmask4_next[i].data[7] | pmask5_next[i].data[7] | pmask6_next[i].data[7] | pmask7_next[i].data[7] | pmask8_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] | pmask2_next[i].data[8] | pmask3_next[i].data[8] | pmask4_next[i].data[8] | pmask5_next[i].data[8] | pmask6_next[i].data[8] | pmask7_next[i].data[8] | pmask8_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] | pmask2_next[i].data[9] | pmask3_next[i].data[9] | pmask4_next[i].data[9] | pmask5_next[i].data[9] | pmask6_next[i].data[9] | pmask7_next[i].data[9] | pmask8_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] | pmask2_next[i].data[10] | pmask3_next[i].data[10] | pmask4_next[i].data[10] | pmask5_next[i].data[10] | pmask6_next[i].data[10] | pmask7_next[i].data[10] | pmask8_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] | pmask2_next[i].data[11] | pmask3_next[i].data[11] | pmask4_next[i].data[11] | pmask5_next[i].data[11] | pmask6_next[i].data[11] | pmask7_next[i].data[11] | pmask8_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] | pmask2_next[i].data[12] | pmask3_next[i].data[12] | pmask4_next[i].data[12] | pmask5_next[i].data[12] | pmask6_next[i].data[12] | pmask7_next[i].data[12] | pmask8_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] | pmask2_next[i].data[13] | pmask3_next[i].data[13] | pmask4_next[i].data[13] | pmask5_next[i].data[13] | pmask6_next[i].data[13] | pmask7_next[i].data[13] | pmask8_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] | pmask2_next[i].data[14] | pmask3_next[i].data[14] | pmask4_next[i].data[14] | pmask5_next[i].data[14] | pmask6_next[i].data[14] | pmask7_next[i].data[14] | pmask8_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] | pmask2_next[i].data[15] | pmask3_next[i].data[15] | pmask4_next[i].data[15] | pmask5_next[i].data[15] | pmask6_next[i].data[15] | pmask7_next[i].data[15] | pmask8_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] | pmask2_next[i].data[16] | pmask3_next[i].data[16] | pmask4_next[i].data[16] | pmask5_next[i].data[16] | pmask6_next[i].data[16] | pmask7_next[i].data[16] | pmask8_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] | pmask2_next[i].data[17] | pmask3_next[i].data[17] | pmask4_next[i].data[17] | pmask5_next[i].data[17] | pmask6_next[i].data[17] | pmask7_next[i].data[17] | pmask8_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] | pmask2_next[i].data[18] | pmask3_next[i].data[18] | pmask4_next[i].data[18] | pmask5_next[i].data[18] | pmask6_next[i].data[18] | pmask7_next[i].data[18] | pmask8_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] | pmask2_next[i].data[19] | pmask3_next[i].data[19] | pmask4_next[i].data[19] | pmask5_next[i].data[19] | pmask6_next[i].data[19] | pmask7_next[i].data[19] | pmask8_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] | pmask2_next[i].data[20] | pmask3_next[i].data[20] | pmask4_next[i].data[20] | pmask5_next[i].data[20] | pmask6_next[i].data[20] | pmask7_next[i].data[20] | pmask8_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] | pmask2_next[i].data[21] | pmask3_next[i].data[21] | pmask4_next[i].data[21] | pmask5_next[i].data[21] | pmask6_next[i].data[21] | pmask7_next[i].data[21] | pmask8_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] | pmask2_next[i].data[22] | pmask3_next[i].data[22] | pmask4_next[i].data[22] | pmask5_next[i].data[22] | pmask6_next[i].data[22] | pmask7_next[i].data[22] | pmask8_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] | pmask2_next[i].data[23] | pmask3_next[i].data[23] | pmask4_next[i].data[23] | pmask5_next[i].data[23] | pmask6_next[i].data[23] | pmask7_next[i].data[23] | pmask8_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] | pmask2_next[i].data[24] | pmask3_next[i].data[24] | pmask4_next[i].data[24] | pmask5_next[i].data[24] | pmask6_next[i].data[24] | pmask7_next[i].data[24] | pmask8_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] | pmask2_next[i].data[25] | pmask3_next[i].data[25] | pmask4_next[i].data[25] | pmask5_next[i].data[25] | pmask6_next[i].data[25] | pmask7_next[i].data[25] | pmask8_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] | pmask2_next[i].data[26] | pmask3_next[i].data[26] | pmask4_next[i].data[26] | pmask5_next[i].data[26] | pmask6_next[i].data[26] | pmask7_next[i].data[26] | pmask8_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] | pmask2_next[i].data[27] | pmask3_next[i].data[27] | pmask4_next[i].data[27] | pmask5_next[i].data[27] | pmask6_next[i].data[27] | pmask7_next[i].data[27] | pmask8_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] | pmask2_next[i].data[28] | pmask3_next[i].data[28] | pmask4_next[i].data[28] | pmask5_next[i].data[28] | pmask6_next[i].data[28] | pmask7_next[i].data[28] | pmask8_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] | pmask2_next[i].data[29] | pmask3_next[i].data[29] | pmask4_next[i].data[29] | pmask5_next[i].data[29] | pmask6_next[i].data[29] | pmask7_next[i].data[29] | pmask8_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] | pmask2_next[i].data[30] | pmask3_next[i].data[30] | pmask4_next[i].data[30] | pmask5_next[i].data[30] | pmask6_next[i].data[30] | pmask7_next[i].data[30] | pmask8_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] | pmask2_next[i].data[31] | pmask3_next[i].data[31] | pmask4_next[i].data[31] | pmask5_next[i].data[31] | pmask6_next[i].data[31] | pmask7_next[i].data[31] | pmask8_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs9 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 9:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 9:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask9_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer7[i].value > 0){ cout<<"MERGE: globalstatsbuffer7["<<i<<"].key : "<<globalstatsbuffer7[i].key<<", globalstatsbuffer7["<<i<<"].value : "<<globalstatsbuffer7[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer8[i].value > 0){ cout<<"MERGE: globalstatsbuffer8["<<i<<"].key : "<<globalstatsbuffer8[i].key<<", globalstatsbuffer8["<<i<<"].value : "<<globalstatsbuffer8[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer9[i].value > 0){ cout<<"MERGE: globalstatsbuffer9["<<i<<"].key : "<<globalstatsbuffer9[i].key<<", globalstatsbuffer9["<<i<<"].value : "<<globalstatsbuffer9[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask2_next[i].data[0] > 0){ cout<<i<<"(b2), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask3_next[i].data[0] > 0){ cout<<i<<"(b3), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask4_next[i].data[0] > 0){ cout<<i<<"(b4), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask5_next[i].data[0] > 0){ cout<<i<<"(b5), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask6_next[i].data[0] > 0){ cout<<i<<"(b6), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask7_next[i].data[0] > 0){ cout<<i<<"(b7), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask8_next[i].data[0] > 0){ cout<<i<<"(b8), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask9_next[i].data[0] > 0){ cout<<i<<"(b9), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] | pmask7_next[i].data[0] | pmask8_next[i].data[0] | pmask9_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] | pmask2_next[i].data[1] | pmask3_next[i].data[1] | pmask4_next[i].data[1] | pmask5_next[i].data[1] | pmask6_next[i].data[1] | pmask7_next[i].data[1] | pmask8_next[i].data[1] | pmask9_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] | pmask2_next[i].data[2] | pmask3_next[i].data[2] | pmask4_next[i].data[2] | pmask5_next[i].data[2] | pmask6_next[i].data[2] | pmask7_next[i].data[2] | pmask8_next[i].data[2] | pmask9_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] | pmask2_next[i].data[3] | pmask3_next[i].data[3] | pmask4_next[i].data[3] | pmask5_next[i].data[3] | pmask6_next[i].data[3] | pmask7_next[i].data[3] | pmask8_next[i].data[3] | pmask9_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] | pmask2_next[i].data[4] | pmask3_next[i].data[4] | pmask4_next[i].data[4] | pmask5_next[i].data[4] | pmask6_next[i].data[4] | pmask7_next[i].data[4] | pmask8_next[i].data[4] | pmask9_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] | pmask2_next[i].data[5] | pmask3_next[i].data[5] | pmask4_next[i].data[5] | pmask5_next[i].data[5] | pmask6_next[i].data[5] | pmask7_next[i].data[5] | pmask8_next[i].data[5] | pmask9_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] | pmask2_next[i].data[6] | pmask3_next[i].data[6] | pmask4_next[i].data[6] | pmask5_next[i].data[6] | pmask6_next[i].data[6] | pmask7_next[i].data[6] | pmask8_next[i].data[6] | pmask9_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] | pmask2_next[i].data[7] | pmask3_next[i].data[7] | pmask4_next[i].data[7] | pmask5_next[i].data[7] | pmask6_next[i].data[7] | pmask7_next[i].data[7] | pmask8_next[i].data[7] | pmask9_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] | pmask2_next[i].data[8] | pmask3_next[i].data[8] | pmask4_next[i].data[8] | pmask5_next[i].data[8] | pmask6_next[i].data[8] | pmask7_next[i].data[8] | pmask8_next[i].data[8] | pmask9_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] | pmask2_next[i].data[9] | pmask3_next[i].data[9] | pmask4_next[i].data[9] | pmask5_next[i].data[9] | pmask6_next[i].data[9] | pmask7_next[i].data[9] | pmask8_next[i].data[9] | pmask9_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] | pmask2_next[i].data[10] | pmask3_next[i].data[10] | pmask4_next[i].data[10] | pmask5_next[i].data[10] | pmask6_next[i].data[10] | pmask7_next[i].data[10] | pmask8_next[i].data[10] | pmask9_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] | pmask2_next[i].data[11] | pmask3_next[i].data[11] | pmask4_next[i].data[11] | pmask5_next[i].data[11] | pmask6_next[i].data[11] | pmask7_next[i].data[11] | pmask8_next[i].data[11] | pmask9_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] | pmask2_next[i].data[12] | pmask3_next[i].data[12] | pmask4_next[i].data[12] | pmask5_next[i].data[12] | pmask6_next[i].data[12] | pmask7_next[i].data[12] | pmask8_next[i].data[12] | pmask9_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] | pmask2_next[i].data[13] | pmask3_next[i].data[13] | pmask4_next[i].data[13] | pmask5_next[i].data[13] | pmask6_next[i].data[13] | pmask7_next[i].data[13] | pmask8_next[i].data[13] | pmask9_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] | pmask2_next[i].data[14] | pmask3_next[i].data[14] | pmask4_next[i].data[14] | pmask5_next[i].data[14] | pmask6_next[i].data[14] | pmask7_next[i].data[14] | pmask8_next[i].data[14] | pmask9_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] | pmask2_next[i].data[15] | pmask3_next[i].data[15] | pmask4_next[i].data[15] | pmask5_next[i].data[15] | pmask6_next[i].data[15] | pmask7_next[i].data[15] | pmask8_next[i].data[15] | pmask9_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] | pmask2_next[i].data[16] | pmask3_next[i].data[16] | pmask4_next[i].data[16] | pmask5_next[i].data[16] | pmask6_next[i].data[16] | pmask7_next[i].data[16] | pmask8_next[i].data[16] | pmask9_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] | pmask2_next[i].data[17] | pmask3_next[i].data[17] | pmask4_next[i].data[17] | pmask5_next[i].data[17] | pmask6_next[i].data[17] | pmask7_next[i].data[17] | pmask8_next[i].data[17] | pmask9_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] | pmask2_next[i].data[18] | pmask3_next[i].data[18] | pmask4_next[i].data[18] | pmask5_next[i].data[18] | pmask6_next[i].data[18] | pmask7_next[i].data[18] | pmask8_next[i].data[18] | pmask9_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] | pmask2_next[i].data[19] | pmask3_next[i].data[19] | pmask4_next[i].data[19] | pmask5_next[i].data[19] | pmask6_next[i].data[19] | pmask7_next[i].data[19] | pmask8_next[i].data[19] | pmask9_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] | pmask2_next[i].data[20] | pmask3_next[i].data[20] | pmask4_next[i].data[20] | pmask5_next[i].data[20] | pmask6_next[i].data[20] | pmask7_next[i].data[20] | pmask8_next[i].data[20] | pmask9_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] | pmask2_next[i].data[21] | pmask3_next[i].data[21] | pmask4_next[i].data[21] | pmask5_next[i].data[21] | pmask6_next[i].data[21] | pmask7_next[i].data[21] | pmask8_next[i].data[21] | pmask9_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] | pmask2_next[i].data[22] | pmask3_next[i].data[22] | pmask4_next[i].data[22] | pmask5_next[i].data[22] | pmask6_next[i].data[22] | pmask7_next[i].data[22] | pmask8_next[i].data[22] | pmask9_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] | pmask2_next[i].data[23] | pmask3_next[i].data[23] | pmask4_next[i].data[23] | pmask5_next[i].data[23] | pmask6_next[i].data[23] | pmask7_next[i].data[23] | pmask8_next[i].data[23] | pmask9_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] | pmask2_next[i].data[24] | pmask3_next[i].data[24] | pmask4_next[i].data[24] | pmask5_next[i].data[24] | pmask6_next[i].data[24] | pmask7_next[i].data[24] | pmask8_next[i].data[24] | pmask9_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] | pmask2_next[i].data[25] | pmask3_next[i].data[25] | pmask4_next[i].data[25] | pmask5_next[i].data[25] | pmask6_next[i].data[25] | pmask7_next[i].data[25] | pmask8_next[i].data[25] | pmask9_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] | pmask2_next[i].data[26] | pmask3_next[i].data[26] | pmask4_next[i].data[26] | pmask5_next[i].data[26] | pmask6_next[i].data[26] | pmask7_next[i].data[26] | pmask8_next[i].data[26] | pmask9_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] | pmask2_next[i].data[27] | pmask3_next[i].data[27] | pmask4_next[i].data[27] | pmask5_next[i].data[27] | pmask6_next[i].data[27] | pmask7_next[i].data[27] | pmask8_next[i].data[27] | pmask9_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] | pmask2_next[i].data[28] | pmask3_next[i].data[28] | pmask4_next[i].data[28] | pmask5_next[i].data[28] | pmask6_next[i].data[28] | pmask7_next[i].data[28] | pmask8_next[i].data[28] | pmask9_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] | pmask2_next[i].data[29] | pmask3_next[i].data[29] | pmask4_next[i].data[29] | pmask5_next[i].data[29] | pmask6_next[i].data[29] | pmask7_next[i].data[29] | pmask8_next[i].data[29] | pmask9_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] | pmask2_next[i].data[30] | pmask3_next[i].data[30] | pmask4_next[i].data[30] | pmask5_next[i].data[30] | pmask6_next[i].data[30] | pmask7_next[i].data[30] | pmask8_next[i].data[30] | pmask9_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] | pmask2_next[i].data[31] | pmask3_next[i].data[31] | pmask4_next[i].data[31] | pmask5_next[i].data[31] | pmask6_next[i].data[31] | pmask7_next[i].data[31] | pmask8_next[i].data[31] | pmask9_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs10 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 10:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 10:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask9_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask10_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer7[i].value > 0){ cout<<"MERGE: globalstatsbuffer7["<<i<<"].key : "<<globalstatsbuffer7[i].key<<", globalstatsbuffer7["<<i<<"].value : "<<globalstatsbuffer7[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer8[i].value > 0){ cout<<"MERGE: globalstatsbuffer8["<<i<<"].key : "<<globalstatsbuffer8[i].key<<", globalstatsbuffer8["<<i<<"].value : "<<globalstatsbuffer8[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer9[i].value > 0){ cout<<"MERGE: globalstatsbuffer9["<<i<<"].key : "<<globalstatsbuffer9[i].key<<", globalstatsbuffer9["<<i<<"].value : "<<globalstatsbuffer9[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer10[i].value > 0){ cout<<"MERGE: globalstatsbuffer10["<<i<<"].key : "<<globalstatsbuffer10[i].key<<", globalstatsbuffer10["<<i<<"].value : "<<globalstatsbuffer10[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask2_next[i].data[0] > 0){ cout<<i<<"(b2), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask3_next[i].data[0] > 0){ cout<<i<<"(b3), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask4_next[i].data[0] > 0){ cout<<i<<"(b4), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask5_next[i].data[0] > 0){ cout<<i<<"(b5), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask6_next[i].data[0] > 0){ cout<<i<<"(b6), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask7_next[i].data[0] > 0){ cout<<i<<"(b7), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask8_next[i].data[0] > 0){ cout<<i<<"(b8), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask9_next[i].data[0] > 0){ cout<<i<<"(b9), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask10_next[i].data[0] > 0){ cout<<i<<"(b10), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] | pmask7_next[i].data[0] | pmask8_next[i].data[0] | pmask9_next[i].data[0] | pmask10_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] | pmask2_next[i].data[1] | pmask3_next[i].data[1] | pmask4_next[i].data[1] | pmask5_next[i].data[1] | pmask6_next[i].data[1] | pmask7_next[i].data[1] | pmask8_next[i].data[1] | pmask9_next[i].data[1] | pmask10_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] | pmask2_next[i].data[2] | pmask3_next[i].data[2] | pmask4_next[i].data[2] | pmask5_next[i].data[2] | pmask6_next[i].data[2] | pmask7_next[i].data[2] | pmask8_next[i].data[2] | pmask9_next[i].data[2] | pmask10_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] | pmask2_next[i].data[3] | pmask3_next[i].data[3] | pmask4_next[i].data[3] | pmask5_next[i].data[3] | pmask6_next[i].data[3] | pmask7_next[i].data[3] | pmask8_next[i].data[3] | pmask9_next[i].data[3] | pmask10_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] | pmask2_next[i].data[4] | pmask3_next[i].data[4] | pmask4_next[i].data[4] | pmask5_next[i].data[4] | pmask6_next[i].data[4] | pmask7_next[i].data[4] | pmask8_next[i].data[4] | pmask9_next[i].data[4] | pmask10_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] | pmask2_next[i].data[5] | pmask3_next[i].data[5] | pmask4_next[i].data[5] | pmask5_next[i].data[5] | pmask6_next[i].data[5] | pmask7_next[i].data[5] | pmask8_next[i].data[5] | pmask9_next[i].data[5] | pmask10_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] | pmask2_next[i].data[6] | pmask3_next[i].data[6] | pmask4_next[i].data[6] | pmask5_next[i].data[6] | pmask6_next[i].data[6] | pmask7_next[i].data[6] | pmask8_next[i].data[6] | pmask9_next[i].data[6] | pmask10_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] | pmask2_next[i].data[7] | pmask3_next[i].data[7] | pmask4_next[i].data[7] | pmask5_next[i].data[7] | pmask6_next[i].data[7] | pmask7_next[i].data[7] | pmask8_next[i].data[7] | pmask9_next[i].data[7] | pmask10_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] | pmask2_next[i].data[8] | pmask3_next[i].data[8] | pmask4_next[i].data[8] | pmask5_next[i].data[8] | pmask6_next[i].data[8] | pmask7_next[i].data[8] | pmask8_next[i].data[8] | pmask9_next[i].data[8] | pmask10_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] | pmask2_next[i].data[9] | pmask3_next[i].data[9] | pmask4_next[i].data[9] | pmask5_next[i].data[9] | pmask6_next[i].data[9] | pmask7_next[i].data[9] | pmask8_next[i].data[9] | pmask9_next[i].data[9] | pmask10_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] | pmask2_next[i].data[10] | pmask3_next[i].data[10] | pmask4_next[i].data[10] | pmask5_next[i].data[10] | pmask6_next[i].data[10] | pmask7_next[i].data[10] | pmask8_next[i].data[10] | pmask9_next[i].data[10] | pmask10_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] | pmask2_next[i].data[11] | pmask3_next[i].data[11] | pmask4_next[i].data[11] | pmask5_next[i].data[11] | pmask6_next[i].data[11] | pmask7_next[i].data[11] | pmask8_next[i].data[11] | pmask9_next[i].data[11] | pmask10_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] | pmask2_next[i].data[12] | pmask3_next[i].data[12] | pmask4_next[i].data[12] | pmask5_next[i].data[12] | pmask6_next[i].data[12] | pmask7_next[i].data[12] | pmask8_next[i].data[12] | pmask9_next[i].data[12] | pmask10_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] | pmask2_next[i].data[13] | pmask3_next[i].data[13] | pmask4_next[i].data[13] | pmask5_next[i].data[13] | pmask6_next[i].data[13] | pmask7_next[i].data[13] | pmask8_next[i].data[13] | pmask9_next[i].data[13] | pmask10_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] | pmask2_next[i].data[14] | pmask3_next[i].data[14] | pmask4_next[i].data[14] | pmask5_next[i].data[14] | pmask6_next[i].data[14] | pmask7_next[i].data[14] | pmask8_next[i].data[14] | pmask9_next[i].data[14] | pmask10_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] | pmask2_next[i].data[15] | pmask3_next[i].data[15] | pmask4_next[i].data[15] | pmask5_next[i].data[15] | pmask6_next[i].data[15] | pmask7_next[i].data[15] | pmask8_next[i].data[15] | pmask9_next[i].data[15] | pmask10_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] | pmask2_next[i].data[16] | pmask3_next[i].data[16] | pmask4_next[i].data[16] | pmask5_next[i].data[16] | pmask6_next[i].data[16] | pmask7_next[i].data[16] | pmask8_next[i].data[16] | pmask9_next[i].data[16] | pmask10_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] | pmask2_next[i].data[17] | pmask3_next[i].data[17] | pmask4_next[i].data[17] | pmask5_next[i].data[17] | pmask6_next[i].data[17] | pmask7_next[i].data[17] | pmask8_next[i].data[17] | pmask9_next[i].data[17] | pmask10_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] | pmask2_next[i].data[18] | pmask3_next[i].data[18] | pmask4_next[i].data[18] | pmask5_next[i].data[18] | pmask6_next[i].data[18] | pmask7_next[i].data[18] | pmask8_next[i].data[18] | pmask9_next[i].data[18] | pmask10_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] | pmask2_next[i].data[19] | pmask3_next[i].data[19] | pmask4_next[i].data[19] | pmask5_next[i].data[19] | pmask6_next[i].data[19] | pmask7_next[i].data[19] | pmask8_next[i].data[19] | pmask9_next[i].data[19] | pmask10_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] | pmask2_next[i].data[20] | pmask3_next[i].data[20] | pmask4_next[i].data[20] | pmask5_next[i].data[20] | pmask6_next[i].data[20] | pmask7_next[i].data[20] | pmask8_next[i].data[20] | pmask9_next[i].data[20] | pmask10_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] | pmask2_next[i].data[21] | pmask3_next[i].data[21] | pmask4_next[i].data[21] | pmask5_next[i].data[21] | pmask6_next[i].data[21] | pmask7_next[i].data[21] | pmask8_next[i].data[21] | pmask9_next[i].data[21] | pmask10_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] | pmask2_next[i].data[22] | pmask3_next[i].data[22] | pmask4_next[i].data[22] | pmask5_next[i].data[22] | pmask6_next[i].data[22] | pmask7_next[i].data[22] | pmask8_next[i].data[22] | pmask9_next[i].data[22] | pmask10_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] | pmask2_next[i].data[23] | pmask3_next[i].data[23] | pmask4_next[i].data[23] | pmask5_next[i].data[23] | pmask6_next[i].data[23] | pmask7_next[i].data[23] | pmask8_next[i].data[23] | pmask9_next[i].data[23] | pmask10_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] | pmask2_next[i].data[24] | pmask3_next[i].data[24] | pmask4_next[i].data[24] | pmask5_next[i].data[24] | pmask6_next[i].data[24] | pmask7_next[i].data[24] | pmask8_next[i].data[24] | pmask9_next[i].data[24] | pmask10_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] | pmask2_next[i].data[25] | pmask3_next[i].data[25] | pmask4_next[i].data[25] | pmask5_next[i].data[25] | pmask6_next[i].data[25] | pmask7_next[i].data[25] | pmask8_next[i].data[25] | pmask9_next[i].data[25] | pmask10_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] | pmask2_next[i].data[26] | pmask3_next[i].data[26] | pmask4_next[i].data[26] | pmask5_next[i].data[26] | pmask6_next[i].data[26] | pmask7_next[i].data[26] | pmask8_next[i].data[26] | pmask9_next[i].data[26] | pmask10_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] | pmask2_next[i].data[27] | pmask3_next[i].data[27] | pmask4_next[i].data[27] | pmask5_next[i].data[27] | pmask6_next[i].data[27] | pmask7_next[i].data[27] | pmask8_next[i].data[27] | pmask9_next[i].data[27] | pmask10_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] | pmask2_next[i].data[28] | pmask3_next[i].data[28] | pmask4_next[i].data[28] | pmask5_next[i].data[28] | pmask6_next[i].data[28] | pmask7_next[i].data[28] | pmask8_next[i].data[28] | pmask9_next[i].data[28] | pmask10_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] | pmask2_next[i].data[29] | pmask3_next[i].data[29] | pmask4_next[i].data[29] | pmask5_next[i].data[29] | pmask6_next[i].data[29] | pmask7_next[i].data[29] | pmask8_next[i].data[29] | pmask9_next[i].data[29] | pmask10_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] | pmask2_next[i].data[30] | pmask3_next[i].data[30] | pmask4_next[i].data[30] | pmask5_next[i].data[30] | pmask6_next[i].data[30] | pmask7_next[i].data[30] | pmask8_next[i].data[30] | pmask9_next[i].data[30] | pmask10_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] | pmask2_next[i].data[31] | pmask3_next[i].data[31] | pmask4_next[i].data[31] | pmask5_next[i].data[31] | pmask6_next[i].data[31] | pmask7_next[i].data[31] | pmask8_next[i].data[31] | pmask9_next[i].data[31] | pmask10_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs11 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 11:: [instance 10, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer10[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 11:: [instance 10, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP10B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}
void acts_all::MERGEP0_mergeVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer11[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask9_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask10_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask11_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	#pragma HLS DATA_PACK variable = pmask_next
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsv);
	
	// expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer3[i].value > 0){ cout<<"MERGE: globalstatsbuffer3["<<i<<"].key : "<<globalstatsbuffer3[i].key<<", globalstatsbuffer3["<<i<<"].value : "<<globalstatsbuffer3[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer4[i].value > 0){ cout<<"MERGE: globalstatsbuffer4["<<i<<"].key : "<<globalstatsbuffer4[i].key<<", globalstatsbuffer4["<<i<<"].value : "<<globalstatsbuffer4[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer5[i].value > 0){ cout<<"MERGE: globalstatsbuffer5["<<i<<"].key : "<<globalstatsbuffer5[i].key<<", globalstatsbuffer5["<<i<<"].value : "<<globalstatsbuffer5[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer6[i].value > 0){ cout<<"MERGE: globalstatsbuffer6["<<i<<"].key : "<<globalstatsbuffer6[i].key<<", globalstatsbuffer6["<<i<<"].value : "<<globalstatsbuffer6[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer7[i].value > 0){ cout<<"MERGE: globalstatsbuffer7["<<i<<"].key : "<<globalstatsbuffer7[i].key<<", globalstatsbuffer7["<<i<<"].value : "<<globalstatsbuffer7[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer8[i].value > 0){ cout<<"MERGE: globalstatsbuffer8["<<i<<"].key : "<<globalstatsbuffer8[i].key<<", globalstatsbuffer8["<<i<<"].value : "<<globalstatsbuffer8[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer9[i].value > 0){ cout<<"MERGE: globalstatsbuffer9["<<i<<"].key : "<<globalstatsbuffer9[i].key<<", globalstatsbuffer9["<<i<<"].value : "<<globalstatsbuffer9[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer10[i].value > 0){ cout<<"MERGE: globalstatsbuffer10["<<i<<"].key : "<<globalstatsbuffer10[i].key<<", globalstatsbuffer10["<<i<<"].value : "<<globalstatsbuffer10[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer11[i].value > 0){ cout<<"MERGE: globalstatsbuffer11["<<i<<"].key : "<<globalstatsbuffer11[i].key<<", globalstatsbuffer11["<<i<<"].value : "<<globalstatsbuffer11[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask2_next[i].data[0] > 0){ cout<<i<<"(b2), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask3_next[i].data[0] > 0){ cout<<i<<"(b3), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask4_next[i].data[0] > 0){ cout<<i<<"(b4), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask5_next[i].data[0] > 0){ cout<<i<<"(b5), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask6_next[i].data[0] > 0){ cout<<i<<"(b6), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask7_next[i].data[0] > 0){ cout<<i<<"(b7), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask8_next[i].data[0] > 0){ cout<<i<<"(b8), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask9_next[i].data[0] > 0){ cout<<i<<"(b9), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask10_next[i].data[0] > 0){ cout<<i<<"(b10), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask11_next[i].data[0] > 0){ cout<<i<<"(b11), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){  pmask_next[i].data[0] = 0;  pmask_next[i].data[1] = 0;  pmask_next[i].data[2] = 0;  pmask_next[i].data[3] = 0;  pmask_next[i].data[4] = 0;  pmask_next[i].data[5] = 0;  pmask_next[i].data[6] = 0;  pmask_next[i].data[7] = 0;  pmask_next[i].data[8] = 0;  pmask_next[i].data[9] = 0;  pmask_next[i].data[10] = 0;  pmask_next[i].data[11] = 0;  pmask_next[i].data[12] = 0;  pmask_next[i].data[13] = 0;  pmask_next[i].data[14] = 0;  pmask_next[i].data[15] = 0;  pmask_next[i].data[16] = 0;  pmask_next[i].data[17] = 0;  pmask_next[i].data[18] = 0;  pmask_next[i].data[19] = 0;  pmask_next[i].data[20] = 0;  pmask_next[i].data[21] = 0;  pmask_next[i].data[22] = 0;  pmask_next[i].data[23] = 0;  pmask_next[i].data[24] = 0;  pmask_next[i].data[25] = 0;  pmask_next[i].data[26] = 0;  pmask_next[i].data[27] = 0;  pmask_next[i].data[28] = 0;  pmask_next[i].data[29] = 0;  pmask_next[i].data[30] = 0;  pmask_next[i].data[31] = 0;  } // globalparamsv.NUM_PROCESSEDGESPARTITIONS
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] | pmask7_next[i].data[0] | pmask8_next[i].data[0] | pmask9_next[i].data[0] | pmask10_next[i].data[0] | pmask11_next[i].data[0] ;
		pmask_next[i].data[1] = 0  | pmask0_next[i].data[1] | pmask1_next[i].data[1] | pmask2_next[i].data[1] | pmask3_next[i].data[1] | pmask4_next[i].data[1] | pmask5_next[i].data[1] | pmask6_next[i].data[1] | pmask7_next[i].data[1] | pmask8_next[i].data[1] | pmask9_next[i].data[1] | pmask10_next[i].data[1] | pmask11_next[i].data[1] ;
		pmask_next[i].data[2] = 0  | pmask0_next[i].data[2] | pmask1_next[i].data[2] | pmask2_next[i].data[2] | pmask3_next[i].data[2] | pmask4_next[i].data[2] | pmask5_next[i].data[2] | pmask6_next[i].data[2] | pmask7_next[i].data[2] | pmask8_next[i].data[2] | pmask9_next[i].data[2] | pmask10_next[i].data[2] | pmask11_next[i].data[2] ;
		pmask_next[i].data[3] = 0  | pmask0_next[i].data[3] | pmask1_next[i].data[3] | pmask2_next[i].data[3] | pmask3_next[i].data[3] | pmask4_next[i].data[3] | pmask5_next[i].data[3] | pmask6_next[i].data[3] | pmask7_next[i].data[3] | pmask8_next[i].data[3] | pmask9_next[i].data[3] | pmask10_next[i].data[3] | pmask11_next[i].data[3] ;
		pmask_next[i].data[4] = 0  | pmask0_next[i].data[4] | pmask1_next[i].data[4] | pmask2_next[i].data[4] | pmask3_next[i].data[4] | pmask4_next[i].data[4] | pmask5_next[i].data[4] | pmask6_next[i].data[4] | pmask7_next[i].data[4] | pmask8_next[i].data[4] | pmask9_next[i].data[4] | pmask10_next[i].data[4] | pmask11_next[i].data[4] ;
		pmask_next[i].data[5] = 0  | pmask0_next[i].data[5] | pmask1_next[i].data[5] | pmask2_next[i].data[5] | pmask3_next[i].data[5] | pmask4_next[i].data[5] | pmask5_next[i].data[5] | pmask6_next[i].data[5] | pmask7_next[i].data[5] | pmask8_next[i].data[5] | pmask9_next[i].data[5] | pmask10_next[i].data[5] | pmask11_next[i].data[5] ;
		pmask_next[i].data[6] = 0  | pmask0_next[i].data[6] | pmask1_next[i].data[6] | pmask2_next[i].data[6] | pmask3_next[i].data[6] | pmask4_next[i].data[6] | pmask5_next[i].data[6] | pmask6_next[i].data[6] | pmask7_next[i].data[6] | pmask8_next[i].data[6] | pmask9_next[i].data[6] | pmask10_next[i].data[6] | pmask11_next[i].data[6] ;
		pmask_next[i].data[7] = 0  | pmask0_next[i].data[7] | pmask1_next[i].data[7] | pmask2_next[i].data[7] | pmask3_next[i].data[7] | pmask4_next[i].data[7] | pmask5_next[i].data[7] | pmask6_next[i].data[7] | pmask7_next[i].data[7] | pmask8_next[i].data[7] | pmask9_next[i].data[7] | pmask10_next[i].data[7] | pmask11_next[i].data[7] ;
		pmask_next[i].data[8] = 0  | pmask0_next[i].data[8] | pmask1_next[i].data[8] | pmask2_next[i].data[8] | pmask3_next[i].data[8] | pmask4_next[i].data[8] | pmask5_next[i].data[8] | pmask6_next[i].data[8] | pmask7_next[i].data[8] | pmask8_next[i].data[8] | pmask9_next[i].data[8] | pmask10_next[i].data[8] | pmask11_next[i].data[8] ;
		pmask_next[i].data[9] = 0  | pmask0_next[i].data[9] | pmask1_next[i].data[9] | pmask2_next[i].data[9] | pmask3_next[i].data[9] | pmask4_next[i].data[9] | pmask5_next[i].data[9] | pmask6_next[i].data[9] | pmask7_next[i].data[9] | pmask8_next[i].data[9] | pmask9_next[i].data[9] | pmask10_next[i].data[9] | pmask11_next[i].data[9] ;
		pmask_next[i].data[10] = 0  | pmask0_next[i].data[10] | pmask1_next[i].data[10] | pmask2_next[i].data[10] | pmask3_next[i].data[10] | pmask4_next[i].data[10] | pmask5_next[i].data[10] | pmask6_next[i].data[10] | pmask7_next[i].data[10] | pmask8_next[i].data[10] | pmask9_next[i].data[10] | pmask10_next[i].data[10] | pmask11_next[i].data[10] ;
		pmask_next[i].data[11] = 0  | pmask0_next[i].data[11] | pmask1_next[i].data[11] | pmask2_next[i].data[11] | pmask3_next[i].data[11] | pmask4_next[i].data[11] | pmask5_next[i].data[11] | pmask6_next[i].data[11] | pmask7_next[i].data[11] | pmask8_next[i].data[11] | pmask9_next[i].data[11] | pmask10_next[i].data[11] | pmask11_next[i].data[11] ;
		pmask_next[i].data[12] = 0  | pmask0_next[i].data[12] | pmask1_next[i].data[12] | pmask2_next[i].data[12] | pmask3_next[i].data[12] | pmask4_next[i].data[12] | pmask5_next[i].data[12] | pmask6_next[i].data[12] | pmask7_next[i].data[12] | pmask8_next[i].data[12] | pmask9_next[i].data[12] | pmask10_next[i].data[12] | pmask11_next[i].data[12] ;
		pmask_next[i].data[13] = 0  | pmask0_next[i].data[13] | pmask1_next[i].data[13] | pmask2_next[i].data[13] | pmask3_next[i].data[13] | pmask4_next[i].data[13] | pmask5_next[i].data[13] | pmask6_next[i].data[13] | pmask7_next[i].data[13] | pmask8_next[i].data[13] | pmask9_next[i].data[13] | pmask10_next[i].data[13] | pmask11_next[i].data[13] ;
		pmask_next[i].data[14] = 0  | pmask0_next[i].data[14] | pmask1_next[i].data[14] | pmask2_next[i].data[14] | pmask3_next[i].data[14] | pmask4_next[i].data[14] | pmask5_next[i].data[14] | pmask6_next[i].data[14] | pmask7_next[i].data[14] | pmask8_next[i].data[14] | pmask9_next[i].data[14] | pmask10_next[i].data[14] | pmask11_next[i].data[14] ;
		pmask_next[i].data[15] = 0  | pmask0_next[i].data[15] | pmask1_next[i].data[15] | pmask2_next[i].data[15] | pmask3_next[i].data[15] | pmask4_next[i].data[15] | pmask5_next[i].data[15] | pmask6_next[i].data[15] | pmask7_next[i].data[15] | pmask8_next[i].data[15] | pmask9_next[i].data[15] | pmask10_next[i].data[15] | pmask11_next[i].data[15] ;
		pmask_next[i].data[16] = 0  | pmask0_next[i].data[16] | pmask1_next[i].data[16] | pmask2_next[i].data[16] | pmask3_next[i].data[16] | pmask4_next[i].data[16] | pmask5_next[i].data[16] | pmask6_next[i].data[16] | pmask7_next[i].data[16] | pmask8_next[i].data[16] | pmask9_next[i].data[16] | pmask10_next[i].data[16] | pmask11_next[i].data[16] ;
		pmask_next[i].data[17] = 0  | pmask0_next[i].data[17] | pmask1_next[i].data[17] | pmask2_next[i].data[17] | pmask3_next[i].data[17] | pmask4_next[i].data[17] | pmask5_next[i].data[17] | pmask6_next[i].data[17] | pmask7_next[i].data[17] | pmask8_next[i].data[17] | pmask9_next[i].data[17] | pmask10_next[i].data[17] | pmask11_next[i].data[17] ;
		pmask_next[i].data[18] = 0  | pmask0_next[i].data[18] | pmask1_next[i].data[18] | pmask2_next[i].data[18] | pmask3_next[i].data[18] | pmask4_next[i].data[18] | pmask5_next[i].data[18] | pmask6_next[i].data[18] | pmask7_next[i].data[18] | pmask8_next[i].data[18] | pmask9_next[i].data[18] | pmask10_next[i].data[18] | pmask11_next[i].data[18] ;
		pmask_next[i].data[19] = 0  | pmask0_next[i].data[19] | pmask1_next[i].data[19] | pmask2_next[i].data[19] | pmask3_next[i].data[19] | pmask4_next[i].data[19] | pmask5_next[i].data[19] | pmask6_next[i].data[19] | pmask7_next[i].data[19] | pmask8_next[i].data[19] | pmask9_next[i].data[19] | pmask10_next[i].data[19] | pmask11_next[i].data[19] ;
		pmask_next[i].data[20] = 0  | pmask0_next[i].data[20] | pmask1_next[i].data[20] | pmask2_next[i].data[20] | pmask3_next[i].data[20] | pmask4_next[i].data[20] | pmask5_next[i].data[20] | pmask6_next[i].data[20] | pmask7_next[i].data[20] | pmask8_next[i].data[20] | pmask9_next[i].data[20] | pmask10_next[i].data[20] | pmask11_next[i].data[20] ;
		pmask_next[i].data[21] = 0  | pmask0_next[i].data[21] | pmask1_next[i].data[21] | pmask2_next[i].data[21] | pmask3_next[i].data[21] | pmask4_next[i].data[21] | pmask5_next[i].data[21] | pmask6_next[i].data[21] | pmask7_next[i].data[21] | pmask8_next[i].data[21] | pmask9_next[i].data[21] | pmask10_next[i].data[21] | pmask11_next[i].data[21] ;
		pmask_next[i].data[22] = 0  | pmask0_next[i].data[22] | pmask1_next[i].data[22] | pmask2_next[i].data[22] | pmask3_next[i].data[22] | pmask4_next[i].data[22] | pmask5_next[i].data[22] | pmask6_next[i].data[22] | pmask7_next[i].data[22] | pmask8_next[i].data[22] | pmask9_next[i].data[22] | pmask10_next[i].data[22] | pmask11_next[i].data[22] ;
		pmask_next[i].data[23] = 0  | pmask0_next[i].data[23] | pmask1_next[i].data[23] | pmask2_next[i].data[23] | pmask3_next[i].data[23] | pmask4_next[i].data[23] | pmask5_next[i].data[23] | pmask6_next[i].data[23] | pmask7_next[i].data[23] | pmask8_next[i].data[23] | pmask9_next[i].data[23] | pmask10_next[i].data[23] | pmask11_next[i].data[23] ;
		pmask_next[i].data[24] = 0  | pmask0_next[i].data[24] | pmask1_next[i].data[24] | pmask2_next[i].data[24] | pmask3_next[i].data[24] | pmask4_next[i].data[24] | pmask5_next[i].data[24] | pmask6_next[i].data[24] | pmask7_next[i].data[24] | pmask8_next[i].data[24] | pmask9_next[i].data[24] | pmask10_next[i].data[24] | pmask11_next[i].data[24] ;
		pmask_next[i].data[25] = 0  | pmask0_next[i].data[25] | pmask1_next[i].data[25] | pmask2_next[i].data[25] | pmask3_next[i].data[25] | pmask4_next[i].data[25] | pmask5_next[i].data[25] | pmask6_next[i].data[25] | pmask7_next[i].data[25] | pmask8_next[i].data[25] | pmask9_next[i].data[25] | pmask10_next[i].data[25] | pmask11_next[i].data[25] ;
		pmask_next[i].data[26] = 0  | pmask0_next[i].data[26] | pmask1_next[i].data[26] | pmask2_next[i].data[26] | pmask3_next[i].data[26] | pmask4_next[i].data[26] | pmask5_next[i].data[26] | pmask6_next[i].data[26] | pmask7_next[i].data[26] | pmask8_next[i].data[26] | pmask9_next[i].data[26] | pmask10_next[i].data[26] | pmask11_next[i].data[26] ;
		pmask_next[i].data[27] = 0  | pmask0_next[i].data[27] | pmask1_next[i].data[27] | pmask2_next[i].data[27] | pmask3_next[i].data[27] | pmask4_next[i].data[27] | pmask5_next[i].data[27] | pmask6_next[i].data[27] | pmask7_next[i].data[27] | pmask8_next[i].data[27] | pmask9_next[i].data[27] | pmask10_next[i].data[27] | pmask11_next[i].data[27] ;
		pmask_next[i].data[28] = 0  | pmask0_next[i].data[28] | pmask1_next[i].data[28] | pmask2_next[i].data[28] | pmask3_next[i].data[28] | pmask4_next[i].data[28] | pmask5_next[i].data[28] | pmask6_next[i].data[28] | pmask7_next[i].data[28] | pmask8_next[i].data[28] | pmask9_next[i].data[28] | pmask10_next[i].data[28] | pmask11_next[i].data[28] ;
		pmask_next[i].data[29] = 0  | pmask0_next[i].data[29] | pmask1_next[i].data[29] | pmask2_next[i].data[29] | pmask3_next[i].data[29] | pmask4_next[i].data[29] | pmask5_next[i].data[29] | pmask6_next[i].data[29] | pmask7_next[i].data[29] | pmask8_next[i].data[29] | pmask9_next[i].data[29] | pmask10_next[i].data[29] | pmask11_next[i].data[29] ;
		pmask_next[i].data[30] = 0  | pmask0_next[i].data[30] | pmask1_next[i].data[30] | pmask2_next[i].data[30] | pmask3_next[i].data[30] | pmask4_next[i].data[30] | pmask5_next[i].data[30] | pmask6_next[i].data[30] | pmask7_next[i].data[30] | pmask8_next[i].data[30] | pmask9_next[i].data[30] | pmask10_next[i].data[30] | pmask11_next[i].data[30] ;
		pmask_next[i].data[31] = 0  | pmask0_next[i].data[31] | pmask1_next[i].data[31] | pmask2_next[i].data[31] | pmask3_next[i].data[31] | pmask4_next[i].data[31] | pmask5_next[i].data[31] | pmask6_next[i].data[31] | pmask7_next[i].data[31] | pmask8_next[i].data[31] | pmask9_next[i].data[31] | pmask10_next[i].data[31] | pmask11_next[i].data[31] ;
	}
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		uint32_type temp;
			#ifdef _WIDEWORD
			temp.range(0, 0) = pmask_next[i].data[0];
			temp.range(1, 1) = pmask_next[i].data[1];
			temp.range(2, 2) = pmask_next[i].data[2];
			temp.range(3, 3) = pmask_next[i].data[3];
			temp.range(4, 4) = pmask_next[i].data[4];
			temp.range(5, 5) = pmask_next[i].data[5];
			temp.range(6, 6) = pmask_next[i].data[6];
			temp.range(7, 7) = pmask_next[i].data[7];
			temp.range(8, 8) = pmask_next[i].data[8];
			temp.range(9, 9) = pmask_next[i].data[9];
			temp.range(10, 10) = pmask_next[i].data[10];
			temp.range(11, 11) = pmask_next[i].data[11];
			temp.range(12, 12) = pmask_next[i].data[12];
			temp.range(13, 13) = pmask_next[i].data[13];
			temp.range(14, 14) = pmask_next[i].data[14];
			temp.range(15, 15) = pmask_next[i].data[15];
			temp.range(16, 16) = pmask_next[i].data[16];
			temp.range(17, 17) = pmask_next[i].data[17];
			temp.range(18, 18) = pmask_next[i].data[18];
			temp.range(19, 19) = pmask_next[i].data[19];
			temp.range(20, 20) = pmask_next[i].data[20];
			temp.range(21, 21) = pmask_next[i].data[21];
			temp.range(22, 22) = pmask_next[i].data[22];
			temp.range(23, 23) = pmask_next[i].data[23];
			temp.range(24, 24) = pmask_next[i].data[24];
			temp.range(25, 25) = pmask_next[i].data[25];
			temp.range(26, 26) = pmask_next[i].data[26];
			temp.range(27, 27) = pmask_next[i].data[27];
			temp.range(28, 28) = pmask_next[i].data[28];
			temp.range(29, 29) = pmask_next[i].data[29];
			temp.range(30, 30) = pmask_next[i].data[30];
			temp.range(31, 31) = pmask_next[i].data[31];
			#else 
			UTILP0_WRITEBITSTO_UINTV(&temp, 0, 1, pmask_next[i].data[0]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 1, 1, pmask_next[i].data[1]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 2, 1, pmask_next[i].data[2]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 3, 1, pmask_next[i].data[3]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 4, 1, pmask_next[i].data[4]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 5, 1, pmask_next[i].data[5]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 6, 1, pmask_next[i].data[6]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 7, 1, pmask_next[i].data[7]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 8, 1, pmask_next[i].data[8]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 9, 1, pmask_next[i].data[9]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 10, 1, pmask_next[i].data[10]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 11, 1, pmask_next[i].data[11]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 12, 1, pmask_next[i].data[12]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 13, 1, pmask_next[i].data[13]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 14, 1, pmask_next[i].data[14]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 15, 1, pmask_next[i].data[15]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 16, 1, pmask_next[i].data[16]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 17, 1, pmask_next[i].data[17]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 18, 1, pmask_next[i].data[18]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 19, 1, pmask_next[i].data[19]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 20, 1, pmask_next[i].data[20]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 21, 1, pmask_next[i].data[21]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 22, 1, pmask_next[i].data[22]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 23, 1, pmask_next[i].data[23]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 24, 1, pmask_next[i].data[24]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 25, 1, pmask_next[i].data[25]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 26, 1, pmask_next[i].data[26]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 27, 1, pmask_next[i].data[27]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 28, 1, pmask_next[i].data[28]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 29, 1, pmask_next[i].data[29]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 30, 1, pmask_next[i].data[30]);
			UTILP0_WRITEBITSTO_UINTV(&temp, 31, 1, pmask_next[i].data[31]);
			#endif

		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
			#ifdef _WIDEWORD
			.range(31, 0)
			#else
			.data[0].key
			#endif 
				= temp;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<BLOCKRAM_PMASK1_SIZE; i++){ 
		for(unsigned int v=0; v<32; v++){ 
			if(pmask_next[i].data[v] > 0){ cout<<"MERGEP0_mergeVs12 (2): PARTITION LOCATION [i:"<<i<<", v:"<<v<<"] IS ACTIVE IN NEXT ITERATION. "<<endl; }
		}
	}
	#endif 
	#endif
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 10, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer10[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 10, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP10B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
		cout<<"acts_merge::MERGEP0_merge Vertices 12:: [instance 11, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer11[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE || true){ // REMOVEME 'true'
			#if defined(_DEBUGMODE_KERNELPRINTS) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
			cout<<"acts_merge::MERGE Vertices 12:: [instance 11, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP11B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12---: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, lbasevoffset_kvs2, voffset_kvs2, globalparamsv.SIZE_SRCVERTICESDATA);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram11[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
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
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif
	// exit(EXIT_SUCCESS); // 
	return;
}

extern "C" {
void acts_all::TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * vdram){
#pragma HLS INTERFACE m_axi port = vdramA offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdramB offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdramC offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3

#pragma HLS INTERFACE s_axilite port = vdramA bundle = control
#pragma HLS INTERFACE s_axilite port = vdramB bundle = control
#pragma HLS INTERFACE s_axilite port = vdramC bundle = control
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = vdramA
#pragma HLS DATA_PACK variable = vdramB
#pragma HLS DATA_PACK variable = vdramC
#pragma HLS DATA_PACK variable = vdram

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: ACTS EXCHANGE LAUNCHED. Exchanging vertices across different SLRs..."<<endl; 
	#endif
	
	unsigned int statssizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = statssizes_kvs
	unsigned int partitionoffseti = 0;
	
	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP0_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP0_getglobalparams(vdramC, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_actvpstatsoffset(globalparamsvA);
	
	//>> expensive variables 
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR2 = NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR1 = NUMCOMPUTEUNITS_SLR1 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR0 = NUMCOMPUTEUNITS_SLR0 * globalparamsvA.NUM_REDUCEPARTITIONS;
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS2_REDUCEPARTITION;
	//>>
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: retrieving stats from vdramA, vdramB & vdramC. "<<endl; 
	#endif 
	MEMACCESSP0_retreievekvstats(vdramA, statssizes_kvs[0], globalparamsvA, actvpstats_beginoffset, _TOTALNUMREDUCEPARTITIONS_SLR2);
	MEMACCESSP0_retreievekvstats(vdramB, statssizes_kvs[1], globalparamsvB, actvpstats_beginoffset, _TOTALNUMREDUCEPARTITIONS_SLR1);
	MEMACCESSP0_retreievekvstats(vdramC, statssizes_kvs[2], globalparamsvC, actvpstats_beginoffset, _TOTALNUMREDUCEPARTITIONS_SLR0);

	// vertices
	#ifdef CONFIG_MERGE_VDATAS
	partitionoffseti = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramA to vdramB & vdramC. [begin offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 20", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[0][partitionoffseti] > 0 || true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 21", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 22", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 23", globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetA_kvs += globalparamsvA.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramA to vdramB & vdramC. [end offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif

	partitionoffseti = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramB to vdramA & vdramC. [begin offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		for(unsigned int partition=0; partition<globalparamsvB.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 21", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[1][partitionoffseti] > 0 || true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 24", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 25", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 26", globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetB_kvs += globalparamsvB.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramB to vdramA & vdramC. [end offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	
	partitionoffseti = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramC to vdramA & vdramB. [begin offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	MERGEP0_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){ // NUMCOMPUTEUNITS_SLR1AND2
		for(unsigned int partition=0; partition<globalparamsvC.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 22", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[2][partitionoffseti] > 0 || true){ // CRITICAL REMOVEME. true.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGEP0_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 27", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 28", globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 29", globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
					#endif
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetC_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramC to vdramA & vdramB. [end offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: merging vertices partition masks across vdramA, vdramB & vdramC."<<endl; 
	#endif
	unsigned int pA = 0; unsigned int pB = 0; unsigned int pC = 0; 
	MERGEP0_EXCHANGEVPMS_LOOP1: for(unsigned int partition=0; partition<BLOCKRAM_PMASK1_SIZE; partition++){ // BLOCKRAM_PMASK1_SIZE, globalparamsvA.NUM_PROCESSEDGESPARTITIONS
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 31", globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 32", globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		actsutilityobj->checkoutofbounds("MERGE::EXCHANGE::: ERROR 33", globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition, TOTALDRAMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		
		#ifdef _WIDEWORD
		pA = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		pB = vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		pC = vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		pA = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		pB = vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		pC = vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
		
		unsigned int p = pA | pB | pC;
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(p > 0){ cout<<"TOPP0_topkernelS:: partition mask: "<<partition<<" is active: pA: "<<pA<<", pB: "<<pB<<", pC: "<<pC<<endl; }
		#endif
		
		#ifdef _WIDEWORD
		vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = p;
		vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = p;
		vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = p;
		#else 
		vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = p;
		vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = p;
		vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = p;
		#endif 
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"--- TOPP0_topkernelS: vdramA["<<partition<<"]: "<<p<<endl; 
		#endif
	}
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"TOPP0_topkernelS: Finished. voffsetA: "<<voffsetA_kvs * VECTOR_SIZE<<", voffsetB: "<<voffsetB_kvs * VECTOR_SIZE<<", voffsetC: "<<voffsetC_kvs * VECTOR_SIZE<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}


