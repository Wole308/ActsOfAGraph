#define CONFIG_MERGE_VDATAS
#ifndef ALLVERTEXISACTIVE_ALGORITHM
#define CONFIG_MERGE_VPARTITIONS
#endif 

unsigned int MERGEP0_SPLIT_actvpstatsoffset(globalparams_t globalparams){
	// this returns the stats of the last level of partitioning
	unsigned int _offset = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ _offset += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ _offset += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif
	return _offset;
}

void MERGEP0_mergeVs1(uint512_dt * kvdram0, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs1 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 1:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs1: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 1:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs1: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs1: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active partitions..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask0_next[i].data[0] > 0){ cout<<i<<"(b0), "; }}
 // i<n
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask1_next[i].data[0] > 0){ cout<<i<<"(b1), "; }}
	cout<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs2 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 2:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 2:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 2:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 2:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs2: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
	cout<<"Printing active global stats..."<<endl;
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer0[i].value > 0){ cout<<"MERGE: globalstatsbuffer0["<<i<<"].key : "<<globalstatsbuffer0[i].key<<", globalstatsbuffer0["<<i<<"].value : "<<globalstatsbuffer0[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer1[i].value > 0){ cout<<"MERGE: globalstatsbuffer1["<<i<<"].key : "<<globalstatsbuffer1[i].key<<", globalstatsbuffer1["<<i<<"].value : "<<globalstatsbuffer1[i].value<<endl; }}
 // i<n
	for(unsigned int i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ if(globalstatsbuffer2[i].value > 0){ cout<<"MERGE: globalstatsbuffer2["<<i<<"].key : "<<globalstatsbuffer2[i].key<<", globalstatsbuffer2["<<i<<"].value : "<<globalstatsbuffer2[i].value<<endl; }}
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs3 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 3:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 3:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 3:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 3:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 3:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 3:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs3: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs4 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 4:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 4:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 4:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 4:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs4: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs5 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 5:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 5:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 5:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 5:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 5:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs5: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs6 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 6:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 6:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 6:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 6:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 6:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 6:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs6: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs7 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 7:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 7:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 7:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 7:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 7:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 7:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 7:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs7: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] | pmask7_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs8 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 8:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 8:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 8:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 8:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 8:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 8:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 8:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 8:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs8: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] | pmask7_next[i].data[0] | pmask8_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs9 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 9:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 9:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 9:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 9:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 9:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 9:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 9:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 9:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 9:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs9: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask9_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] | pmask7_next[i].data[0] | pmask8_next[i].data[0] | pmask9_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs10 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 10:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 10:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 10:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 10:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 10:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 10:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 10:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 10:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 10:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 10:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs10: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask9_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask10_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] | pmask7_next[i].data[0] | pmask8_next[i].data[0] | pmask9_next[i].data[0] | pmask10_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs11 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 10, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer10[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 11:: [instance 10, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP10B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs11: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void MERGEP0_mergeVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram, 
keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer11[BLOCKRAM_SIZE], pmask_dt pmask0_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask1_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask2_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask3_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask4_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask5_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask6_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask7_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask8_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask9_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask10_next[BLOCKRAM_PMASK1_SIZE],pmask_dt pmask11_next[BLOCKRAM_PMASK1_SIZE],			globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
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
	
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	pmask_dt pmask_next[BLOCKRAM_PMASK1_SIZE]; 
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ pmask_next[i].data[0] = 0; }
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		pmask_next[i].data[0] = 0  | pmask0_next[i].data[0] | pmask1_next[i].data[0] | pmask2_next[i].data[0] | pmask3_next[i].data[0] | pmask4_next[i].data[0] | pmask5_next[i].data[0] | pmask6_next[i].data[0] | pmask7_next[i].data[0] | pmask8_next[i].data[0] | pmask9_next[i].data[0] | pmask10_next[i].data[0] | pmask11_next[i].data[0] ;
	}
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){
		vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + i]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= pmask_next[i].data[0];
	}
	// #ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	for(unsigned int i=0; i<globalparamsv.NUM_PROCESSEDGESPARTITIONS; i++){ if(pmask_next[i].data[0] > 0){ cout<<"MERGEP0_mergeVs12 (2): PARTITION "<<i<<" IS ACTIVE IN NEXT ITERATION. "<<endl; }}
	// #endif 
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
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 10, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer10[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 10, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP10B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				#endif
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_MERGEVSLOOP11: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 11, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer11[partition].value > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGE Vertices 12:: [instance 11, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<"] "<<endl;
			#endif
			MERGEP0_MERGEVSLOOP11B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram11[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int K = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].key;
					unsigned int V = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k].data[v].value;
					if(MEMCAP0_READVDATA(K)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }
					if(MEMCAP0_READVDATA(V)==0){ cout<<"MERGE:: ZERO SEEN. index: "<<globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k<<endl; }	
				}
				#endif 
				
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 21", lbasevoffset_kvs2 + voffset_kvs2 + k, globalparamsv.SIZE_SRCVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
				actsutilityobj->checkoutofbounds("MERGEP0_mergeVs12: ERROR 22", voffseti_kvs2 + k, globalparams.SIZE_DESTVERTICESDATA / VECTOR2_SIZE, NAp, NAp, NAp);
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
void TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * vdram){
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
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsvA);
	
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
	MERGEP0_SPLIT_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 20", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[0][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGEP0_SPLIT_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
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
	MERGEP0_SPLIT_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		for(unsigned int partition=0; partition<globalparamsvB.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 21", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[1][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGEP0_SPLIT_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
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
	MERGEP0_SPLIT_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){ // NUMCOMPUTEUNITS_SLR1AND2
		for(unsigned int partition=0; partition<globalparamsvC.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 22", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[2][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGEP0_SPLIT_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
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
	MERGEP0_SPLIT_EXCHANGEVPMS_LOOP1: for(unsigned int partition=0; partition<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; partition++){
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


