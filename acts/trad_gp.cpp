typedef struct {
	keyvalue_vbuffer_t srcp;
	keyvalue_vbuffer_t dstp; // keyvalue_vbuffer_t, uint32_type
	unsigned int srcvid;
	unsigned int dstvid;
	unsigned int status;
	unsigned int tocommit;
} arrow_t;
#define TRADGP_BUFFER_SIZE DOUBLE_BLOCKRAM_SIZE
#define TRADGP_BUFFER_BIGSIZE (DOUBLE_BLOCKRAM_SIZE * VECTOR2_SIZE)
#define TRADGP_LOADCHUNKSZ 64 // 256 // 1024

void TradGPP0(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * mdram){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"TradGP:: running traditional sssp... "<<endl;
	#endif 
	
	globalparams_t globalparamsm = UTILP0_getglobalparams(mdram, 0);
	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP0_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP0_getglobalparams(vdramC, 0);
	
	unsigned int vpmaskbuffer[BLOCKRAM_CUMMTVPMASK_SIZE];

	unsigned int GraphIter = globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID;
	unsigned int actvvcount_currit = UTILP0_GetSecondData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_MAILBOX + globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID); // globalparamsm.SIZE_ACTIVEVERTICES;
	unsigned int actvvcount_nextit = 0;
	unsigned int nextGraphIter = GraphIter + 1;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"TradGP:: running traditional sssp... (iteration "<<GraphIter<<", number of active vertices to process: "<<actvvcount_currit<<")"<<endl;
	#endif 
	
	unsigned int offsetkvs_curractvvs = 0;
	unsigned int offsetkvs_nxtactvvs = 0;
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	offsetkvs_curractvvs = 0; offsetkvs_nxtactvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE;
	#else
	if(GraphIter % 2 == 0){ offsetkvs_curractvvs = 0; offsetkvs_nxtactvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE; } 
	else { offsetkvs_curractvvs = globalparamsm.SIZE_VERTEXPTRS / VECTOR_SIZE; offsetkvs_nxtactvvs = 0; }
	#endif 
	
	unsigned int actvvs_currentit_basekvs = 0;
	unsigned int actvvs_nextit_basekvs = 0;
	#if defined(CONFIG_PRELOADEDVERTICESMASKS)
	actvvs_currentit_basekvs = GraphIter * (CONFIG_HYBRIDGPMODE_MDRAMSECTIONSZ / VECTOR2_SIZE);
	actvvs_nextit_basekvs = (GraphIter + 1) * (CONFIG_HYBRIDGPMODE_MDRAMSECTIONSZ / VECTOR2_SIZE);
	#endif 
	
	unsigned int total_edges_processed = 0;
	long double edgesprocessedinGraphIter[128]; for(unsigned int i=0; i<128; i++){ edgesprocessedinGraphIter[i] = 0; }
	for(unsigned int v=0; v<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; v++){ vpmaskbuffer[v] = 0; }
	
	unsigned int actvvids[TRADGP_BUFFER_SIZE];
	keyvalue_t tempbuffer_vptr[TRADGP_BUFFER_SIZE];
	#pragma HLS DATA_PACK variable = tempbuffer_vptr
	arrow_t independent_edge[TRADGP_BUFFER_BIGSIZE];
	
	TRADGP_LOOP1: for(unsigned int i=0; i<actvvcount_currit; i+=TRADGP_LOADCHUNKSZ){
		unsigned int chunk_size = TRADGP_LOADCHUNKSZ;
		if(i + TRADGP_LOADCHUNKSZ >= actvvcount_currit){ chunk_size = actvvcount_currit - i; }
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		cout<<">>> TRADGP:: processing chunk: i "<<i<<" (of "<<actvvcount_currit<<"), chunk_size: "<<chunk_size<<", TRADGP_LOADCHUNKSZ: "<<TRADGP_LOADCHUNKSZ<<endl;
		#endif 
		
		// read & buffer (actvv ids)
		unsigned int actvvids[VECTOR2_SIZE];
		TRADGP_LOOP2: for(unsigned int i=0; i<chunk_size; i++){
		#pragma HLS PIPELINE
			actvvids[i] = UTILP0_GetData(mdram, globalparamsm.BASEOFFSETKVS_ACTIVEVERTICES + actvvs_currentit_basekvs, i); 
		}
		
		// read & buffer (vertex ptrs)
		TRADGP_LOOP3: for(unsigned int t=0; t<chunk_size; t++){
		// #pragma HLS PIPELINE
			unsigned int srcvid = actvvids[t];
			edge_t vptr_begin = UTILP0_GetData(mdram, globalparamsm.BASEOFFSETKVS_VERTEXPTR, srcvid); 
			edge_t vptr_end = UTILP0_GetData(mdram, globalparamsm.BASEOFFSETKVS_VERTEXPTR, srcvid + 1); 
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MERGE(2): ERROR 20", t, TRADGP_BUFFER_SIZE, vptr_begin, vptr_end, NAp);
			#endif
			tempbuffer_vptr[t].key = vptr_begin;
			tempbuffer_vptr[t].value = vptr_end;
			if(tempbuffer_vptr[t].value < tempbuffer_vptr[t].key){ tempbuffer_vptr[t].key = 0; tempbuffer_vptr[t].value = 0; }
		}
		
		// read & buffer (dstv properties)
		unsigned int index = 0;
		unsigned int sumEs = 0;
		TRADGP_LOOP4: for(unsigned int i=0; i<chunk_size; i++){
			sumEs += tempbuffer_vptr[i].value - tempbuffer_vptr[i].key;
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			cout<<"TradGP:: read & buffer (dstv properties): number of edges to read: "<<tempbuffer_vptr[i].value - tempbuffer_vptr[i].key<<", (tempbuffer_vptr["<<i<<"].key: "<<tempbuffer_vptr[i].key<<", tempbuffer_vptr["<<i<<"].value: "<<tempbuffer_vptr[i].value<<", sumEs: "<<sumEs<<")"<<endl;
			#endif 
			TRADGP_LOOP4B: for(unsigned int t=tempbuffer_vptr[i].key; t<tempbuffer_vptr[i].value; t++){
			#pragma HLS PIPELINE
				unsigned int srcvid = actvvids[i];
				unsigned int dstvid = UTILP0_GetData(mdram, globalparamsm.BASEOFFSETKVS_EDGESDATA, t);
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				cout<<"TradGP:: EDGE: srcvid: "<<srcvid<<", dstvid: "<<dstvid<<endl;
				#endif 
				
				unsigned int s = dstvid % NUM_PEs;
				unsigned int ldstvid = (dstvid - s) / NUM_PEs; 
				unsigned int offset_kvs = s * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
				
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGE(2): ERROR 21", index, TRADGP_BUFFER_BIGSIZE, dstvid, tempbuffer_vptr[i].key, tempbuffer_vptr[i].value);
				actsutilityobj->checkoutofbounds("MERGE(2): ERROR 22", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + (ldstvid / VDATA_SHRINK_RATIO), globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, index, dstvid, offset_kvs);
				#endif
				independent_edge[index].dstp = UTILP0_GetData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO));
				independent_edge[index].srcvid = actvvids[i];
				independent_edge[index].dstvid = dstvid;
				independent_edge[index].status = 0;
				independent_edge[index].tocommit = 0;
				// index += 1;
				#ifdef CONFIG_FORCEDCORRECT_TRADGP
				if(index + 1 < TRADGP_BUFFER_BIGSIZE-2){ index += 1; }
				#endif 
			}
		}
		
		// process 
		TRADGP_LOOP5: for(unsigned int edge=0; edge<index; edge++){
		#pragma HLS PIPELINE
			unsigned int srcvid = independent_edge[edge].srcvid;
			unsigned int dstvid = independent_edge[edge].dstvid;
			
			unsigned int s = dstvid % NUM_PEs;
			unsigned int ldstvid = (dstvid - s) / NUM_PEs; 
			unsigned int offset_kvs = s * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
			
			// read 
			vmdata_t vmdata;
			#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) || defined(CONFIG_GOLDENKERNEL)	
			unsigned int offsetof_vdata = (ldstvid % VDATA_SHRINK_RATIO);
			vmdata.vdata = UTILP0_SWREADBITSFROM_UINTV(independent_edge[edge].dstp, offsetof_vdata, SIZEOF_VDATA);
			vmdata.vmask = UTILP0_SWREADBITSFROM_UINTV(independent_edge[edge].dstp, BEGINOFFSETOF_VMASK + offsetof_vdata, SIZEOF_VMASK);
			#else
			vmdata.vdata = MEMCAP0_READVDATA(independent_edge[edge].dstp); 
			vmdata.vmask = MEMCAP0_READVMASK(independent_edge[edge].dstp); 
			#endif 
			
			// reduce 
			value_t new_vprop = REDUCEP0_reducefunc(vmdata.vdata, NAp, NAp, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, globalparamsvA.ALGORITHMINFO_GRAPHALGORITHMID);
			if(new_vprop != vmdata.vdata){ vmdata.vmask = 1; } else { vmdata.vmask = 0; } // NEWCHANGE.
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(new_vprop != vmdata.vdata){ cout<<">>> TRADGP::REDUCE VECTOR(1):: ACTIVE REDUCE SEEN @ srcvid: "<<srcvid<<", dstvid: "<<dstvid<<endl; }
			#endif
			
			// write 
			if(vmdata.vmask == 1){
				#if defined(ALGORITHMTYPE_REPRESENTVDATASASBITS) || defined(CONFIG_GOLDENKERNEL)
				unsigned int offsetof_vdata = (ldstvid % VDATA_SHRINK_RATIO);
				UTILP0_SWWRITEBITSTO_UINTV(&independent_edge[edge].dstp, BEGINOFFSETOF_VMASK + offsetof_vdata, SIZEOF_VMASK, vmdata.vmask);
				#else 
				MEMCAP0_WRITEVMASK(&independent_edge[edge].dstp, vmdata.vmask); 
				#endif
				independent_edge[edge].tocommit = 1;
			}
			
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsprocessed(0, 1);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(0, 1); 
			#endif 
		}
		
		// commit 
		TRADGP_LOOP6: for(unsigned int edge=0; edge<index; edge++){
			#pragma HLS PIPELINE
			unsigned int srcvid = independent_edge[edge].srcvid;
			unsigned int dstvid = independent_edge[edge].dstvid;
			
			if(independent_edge[edge].tocommit == 1){ // (vtemp != vprop)
				unsigned int s = independent_edge[edge].dstvid % NUM_PEs;
				unsigned int ldstvid = (independent_edge[edge].dstvid - s) / NUM_PEs; 
				unsigned int offset_kvs = s * globalparamsvA.NUM_REDUCEPARTITIONS * globalparamsvA.SIZEKVS2_REDUCEPARTITION;
				
				#ifdef _DEBUGMODE_CHECKS3
				actsutilityobj->checkoutofbounds("MERGE(2): ERROR 28", globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs + (ldstvid / VDATA_SHRINK_RATIO), globalparamsvA.ACTSPARAMS_MAXHBMCAPACITY_KVS, index, dstvid, offset_kvs);
				#endif
				UTILP0_SetData(vdramA, globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				UTILP0_SetData(vdramB, globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				UTILP0_SetData(vdramC, globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + offset_kvs, (ldstvid / VDATA_SHRINK_RATIO), independent_edge[edge].dstp);
				
				// set active vertices mask in vdramA, vdramB and vdramC
				// vpmaskbuffer[independent_edge[edge].dstvid / (1 << globalparamsvA.SIZEKVS2_PROCESSEDGESPARTITION)PROCESSPARTITIONSZ] = 1;
				vpmaskbuffer[independent_edge[edge].dstvid / (1 << globalparamsvA.SIZEKVS2_PROCESSEDGESPARTITION)] = 1;
				
				// collect stats 
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_countkvsreduced(0, 1);
				actsutilityobj->globalstats_reduce_countvalidkvsreduced(0, 1); 
				#endif 
			}
		}
	}
	
	UTILP0_SetFirstData(mdram, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILP0_SetFirstData(vdramA, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILP0_SetFirstData(vdramB, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	UTILP0_SetFirstData(vdramC, BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID, nextGraphIter);
	
	#if not defined(FPGA_IMPL) && defined(CONFIG_COLLECT_DATAS3_DURING_RUN)
	cout<<">>> TRADGP::[A][Iter: "<<GraphIter<<"]:: num edges processed: "<<actsutilityobj->globalstats_getcountnumvalidprocessedges(0)<<"("<<actsutilityobj->globalstats_getcountnumvalidprocessedges(0)<<"), num vertex updates reduced: "<<actsutilityobj->globalstats_getcountvalidkvsreduced(0)<<"("<<actsutilityobj->globalstats_getcountvalidkvsreduced(0)<<")"<<endl;	
	actsutilityobj->clearglobalvars();
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"TradGP: number of active vertices for iteration "<<nextGraphIter<<": "<<actvvcount_nextit<<""<<endl;
	if(actvvcount_nextit == 0){ cout<<"TradGP: no more active vertices to process. breaking out... "<<endl; }
	#endif 
	// exit(EXIT_SUCCESS); //
	return;
}