#include "acts_merge.h"
using namespace std;

#ifdef SW
acts_merge::acts_merge(){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(); 
	processedges_unslicedgraphobj = new processedges_unslicedgraph();
	processedges_slicedgraphobj = new processedges_slicedgraph();
	partitionupdatesobj = new partitionupdates();
	reduceupdatesobj = new reduceupdates();
	mem_accessobj = new mem_access();
	actsobj = new acts();
	mergeobj = new merge_vtxs();
}
acts_merge::~acts_merge(){}
#endif

value_t 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	if(GraphAlgo == PAGERANK){
		// temp = vtemp + res;
		// temp = 0.5 + (1 - 0.5)*res / vdeg; // IDEAL
		temp = res;
	} else if(GraphAlgo == BFS){
		temp = res;
	} else if(GraphAlgo == SSSP){
		temp = acts_utilobj->UTIL_amin(vtemp, res);
	} else {
		temp = NAp;
	}
	return temp;
}

void // 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_readvdata(bool_type enable1, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs){
	#pragma HLS function_instantiate variable=kvdram
	if(enable1 == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
		
	READVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=1
		keyvalue_t mykeyvalue0;
		keyvalue_t mykeyvalue1;
		keyvalue_t mykeyvalue2;
		keyvalue_t mykeyvalue3;
		keyvalue_t mykeyvalue4;
		keyvalue_t mykeyvalue5;
		keyvalue_t mykeyvalue6;
		keyvalue_t mykeyvalue7;
		
		#ifdef _WIDEWORD
		mykeyvalue0.key = kvdram[dramoffset_kvs + i].range(31, 0); 
		mykeyvalue0.value = kvdram[dramoffset_kvs + i].range(63, 32); 
		mykeyvalue1.key = kvdram[dramoffset_kvs + i].range(95, 64); 
		mykeyvalue1.value = kvdram[dramoffset_kvs + i].range(127, 96); 
		mykeyvalue2.key = kvdram[dramoffset_kvs + i].range(159, 128); 
		mykeyvalue2.value = kvdram[dramoffset_kvs + i].range(191, 160); 
		mykeyvalue3.key = kvdram[dramoffset_kvs + i].range(223, 192); 
		mykeyvalue3.value = kvdram[dramoffset_kvs + i].range(255, 224); 
		mykeyvalue4.key = kvdram[dramoffset_kvs + i].range(287, 256); 
		mykeyvalue4.value = kvdram[dramoffset_kvs + i].range(319, 288); 
		mykeyvalue5.key = kvdram[dramoffset_kvs + i].range(351, 320); 
		mykeyvalue5.value = kvdram[dramoffset_kvs + i].range(383, 352); 
		mykeyvalue6.key = kvdram[dramoffset_kvs + i].range(415, 384); 
		mykeyvalue6.value = kvdram[dramoffset_kvs + i].range(447, 416); 
		mykeyvalue7.key = kvdram[dramoffset_kvs + i].range(479, 448); 
		mykeyvalue7.value = kvdram[dramoffset_kvs + i].range(511, 480); 
		#else 
		mykeyvalue0.key = kvdram[dramoffset_kvs + i].data[0].key; 
		mykeyvalue0.value = kvdram[dramoffset_kvs + i].data[0].value; 
		mykeyvalue1.key = kvdram[dramoffset_kvs + i].data[1].key; 
		mykeyvalue1.value = kvdram[dramoffset_kvs + i].data[1].value; 
		mykeyvalue2.key = kvdram[dramoffset_kvs + i].data[2].key; 
		mykeyvalue2.value = kvdram[dramoffset_kvs + i].data[2].value; 
		mykeyvalue3.key = kvdram[dramoffset_kvs + i].data[3].key; 
		mykeyvalue3.value = kvdram[dramoffset_kvs + i].data[3].value; 
		mykeyvalue4.key = kvdram[dramoffset_kvs + i].data[4].key; 
		mykeyvalue4.value = kvdram[dramoffset_kvs + i].data[4].value; 
		mykeyvalue5.key = kvdram[dramoffset_kvs + i].data[5].key; 
		mykeyvalue5.value = kvdram[dramoffset_kvs + i].data[5].value; 
		mykeyvalue6.key = kvdram[dramoffset_kvs + i].data[6].key; 
		mykeyvalue6.value = kvdram[dramoffset_kvs + i].data[6].value; 
		mykeyvalue7.key = kvdram[dramoffset_kvs + i].data[7].key; 
		mykeyvalue7.value = kvdram[dramoffset_kvs + i].data[7].value; 
		#endif
		
		buffer[0][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue0);
		buffer[1][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue1);
		buffer[2][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue2);
		buffer[3][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue3);
		buffer[4][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue4);
		buffer[5][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue5);
		buffer[6][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue6);
		buffer[7][bufferoffset_kvs + i] = acts_utilobj->UTIL_GETKV2(mykeyvalue7);
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}

void // 
	#ifdef SW 
	acts_merge::
	#endif 
MERGE_writevdata(bool_type enable1, uint512_dt * kvdram, batch_type basedramoffset_kvs, batch_type localdramoffset_kvs, keyvalue_vbuffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs){
	#pragma HLS function_instantiate variable=kvdram
	if(enable1 == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;
	unsigned int dramoffset_kvs = basedramoffset_kvs + localdramoffset_kvs;

	WRITEVDATA_LOOP: for (buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = acts_utilobj->UTIL_GETKV2(buffer[0][i]); 
			keyvalue_t keyvalue1 = acts_utilobj->UTIL_GETKV2(buffer[1][i]); 
			keyvalue_t keyvalue2 = acts_utilobj->UTIL_GETKV2(buffer[2][i]); 
			keyvalue_t keyvalue3 = acts_utilobj->UTIL_GETKV2(buffer[3][i]); 
			keyvalue_t keyvalue4 = acts_utilobj->UTIL_GETKV2(buffer[4][i]); 
			keyvalue_t keyvalue5 = acts_utilobj->UTIL_GETKV2(buffer[5][i]); 
			keyvalue_t keyvalue6 = acts_utilobj->UTIL_GETKV2(buffer[6][i]); 
			keyvalue_t keyvalue7 = acts_utilobj->UTIL_GETKV2(buffer[7][i]); 
			
			#ifdef _WIDEWORD
			kvdram[dramoffset_kvs + i].range(31, 0) = keyvalue0.key;
			kvdram[dramoffset_kvs + i].range(63, 32) = keyvalue0.value;
			kvdram[dramoffset_kvs + i].range(95, 64) = keyvalue1.key;
			kvdram[dramoffset_kvs + i].range(127, 96) = keyvalue1.value;
			kvdram[dramoffset_kvs + i].range(159, 128) = keyvalue2.key;
			kvdram[dramoffset_kvs + i].range(191, 160) = keyvalue2.value;
			kvdram[dramoffset_kvs + i].range(223, 192) = keyvalue3.key;
			kvdram[dramoffset_kvs + i].range(255, 224) = keyvalue3.value;
			kvdram[dramoffset_kvs + i].range(287, 256) = keyvalue4.key;
			kvdram[dramoffset_kvs + i].range(319, 288) = keyvalue4.value;
			kvdram[dramoffset_kvs + i].range(351, 320) = keyvalue5.key;
			kvdram[dramoffset_kvs + i].range(383, 352) = keyvalue5.value;
			kvdram[dramoffset_kvs + i].range(415, 384) = keyvalue6.key;
			kvdram[dramoffset_kvs + i].range(447, 416) = keyvalue6.value;
			kvdram[dramoffset_kvs + i].range(479, 448) = keyvalue7.key;
			kvdram[dramoffset_kvs + i].range(511, 480) = keyvalue7.value;
			#else
			kvdram[dramoffset_kvs + i].data[0].key = keyvalue0.key; 
			kvdram[dramoffset_kvs + i].data[0].value = keyvalue0.value;
			kvdram[dramoffset_kvs + i].data[1].key = keyvalue1.key; 
			kvdram[dramoffset_kvs + i].data[1].value = keyvalue1.value;
			kvdram[dramoffset_kvs + i].data[2].key = keyvalue2.key; 
			kvdram[dramoffset_kvs + i].data[2].value = keyvalue2.value;
			kvdram[dramoffset_kvs + i].data[3].key = keyvalue3.key; 
			kvdram[dramoffset_kvs + i].data[3].value = keyvalue3.value;
			kvdram[dramoffset_kvs + i].data[4].key = keyvalue4.key; 
			kvdram[dramoffset_kvs + i].data[4].value = keyvalue4.value;
			kvdram[dramoffset_kvs + i].data[5].key = keyvalue5.key; 
			kvdram[dramoffset_kvs + i].data[5].value = keyvalue5.value;
			kvdram[dramoffset_kvs + i].data[6].key = keyvalue6.key; 
			kvdram[dramoffset_kvs + i].data[6].value = keyvalue6.value;
			kvdram[dramoffset_kvs + i].data[7].key = keyvalue7.key; 
			kvdram[dramoffset_kvs + i].data[7].value = keyvalue7.value;
			#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<">>> ACTS MERGE: MERGE_writevdata: data written at: dramoffset: "<<localdramoffset_kvs*VECTOR2_SIZE<<", dramoffset_kvs: "<<localdramoffset_kvs<<", size_kvs: "<<size_kvs<<endl;
	#endif
	return;
}

void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge1(uint512_dt * kvdram0, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[1];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 1; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 1; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 1;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 1; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 1; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[2];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 2; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 2; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 2;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 2; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 2; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[3];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 3; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 3; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 3;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 3; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 3; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[0]["<<v_index + k<<"].key: "<<vbuffer2[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[0]["<<v_index + k<<"].value: "<<vbuffer2[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[1]["<<v_index + k<<"].key: "<<vbuffer2[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[1]["<<v_index + k<<"].value: "<<vbuffer2[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[2]["<<v_index + k<<"].key: "<<vbuffer2[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[2]["<<v_index + k<<"].value: "<<vbuffer2[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[3]["<<v_index + k<<"].key: "<<vbuffer2[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[3]["<<v_index + k<<"].value: "<<vbuffer2[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[4]["<<v_index + k<<"].key: "<<vbuffer2[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[4]["<<v_index + k<<"].value: "<<vbuffer2[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[5]["<<v_index + k<<"].key: "<<vbuffer2[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[5]["<<v_index + k<<"].value: "<<vbuffer2[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[6]["<<v_index + k<<"].key: "<<vbuffer2[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[6]["<<v_index + k<<"].value: "<<vbuffer2[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[7]["<<v_index + k<<"].key: "<<vbuffer2[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[7]["<<v_index + k<<"].value: "<<vbuffer2[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[4];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 4; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 4; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 4;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 4; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 4; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram3, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[0]["<<v_index + k<<"].key: "<<vbuffer2[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[0]["<<v_index + k<<"].value: "<<vbuffer2[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[1]["<<v_index + k<<"].key: "<<vbuffer2[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[1]["<<v_index + k<<"].value: "<<vbuffer2[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[2]["<<v_index + k<<"].key: "<<vbuffer2[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[2]["<<v_index + k<<"].value: "<<vbuffer2[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[3]["<<v_index + k<<"].key: "<<vbuffer2[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[3]["<<v_index + k<<"].value: "<<vbuffer2[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[4]["<<v_index + k<<"].key: "<<vbuffer2[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[4]["<<v_index + k<<"].value: "<<vbuffer2[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[5]["<<v_index + k<<"].key: "<<vbuffer2[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[5]["<<v_index + k<<"].value: "<<vbuffer2[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[6]["<<v_index + k<<"].key: "<<vbuffer2[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[6]["<<v_index + k<<"].value: "<<vbuffer2[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[7]["<<v_index + k<<"].key: "<<vbuffer2[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[7]["<<v_index + k<<"].value: "<<vbuffer2[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer3[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[0]["<<v_index + k<<"].key: "<<vbuffer3[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[0]["<<v_index + k<<"].value: "<<vbuffer3[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer3[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[1]["<<v_index + k<<"].key: "<<vbuffer3[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[1]["<<v_index + k<<"].value: "<<vbuffer3[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer3[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[2]["<<v_index + k<<"].key: "<<vbuffer3[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[2]["<<v_index + k<<"].value: "<<vbuffer3[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer3[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[3]["<<v_index + k<<"].key: "<<vbuffer3[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[3]["<<v_index + k<<"].value: "<<vbuffer3[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer3[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[4]["<<v_index + k<<"].key: "<<vbuffer3[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[4]["<<v_index + k<<"].value: "<<vbuffer3[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer3[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[5]["<<v_index + k<<"].key: "<<vbuffer3[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[5]["<<v_index + k<<"].value: "<<vbuffer3[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer3[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[6]["<<v_index + k<<"].key: "<<vbuffer3[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[6]["<<v_index + k<<"].value: "<<vbuffer3[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer3[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[7]["<<v_index + k<<"].key: "<<vbuffer3[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[7]["<<v_index + k<<"].value: "<<vbuffer3[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_vbuffer_t vbuffer4[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[5];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 5; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 5; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 5;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 5; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 5; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram3, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram4, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[0]["<<v_index + k<<"].key: "<<vbuffer2[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[0]["<<v_index + k<<"].value: "<<vbuffer2[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[1]["<<v_index + k<<"].key: "<<vbuffer2[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[1]["<<v_index + k<<"].value: "<<vbuffer2[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[2]["<<v_index + k<<"].key: "<<vbuffer2[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[2]["<<v_index + k<<"].value: "<<vbuffer2[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[3]["<<v_index + k<<"].key: "<<vbuffer2[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[3]["<<v_index + k<<"].value: "<<vbuffer2[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[4]["<<v_index + k<<"].key: "<<vbuffer2[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[4]["<<v_index + k<<"].value: "<<vbuffer2[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[5]["<<v_index + k<<"].key: "<<vbuffer2[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[5]["<<v_index + k<<"].value: "<<vbuffer2[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[6]["<<v_index + k<<"].key: "<<vbuffer2[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[6]["<<v_index + k<<"].value: "<<vbuffer2[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[7]["<<v_index + k<<"].key: "<<vbuffer2[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[7]["<<v_index + k<<"].value: "<<vbuffer2[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer3[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[0]["<<v_index + k<<"].key: "<<vbuffer3[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[0]["<<v_index + k<<"].value: "<<vbuffer3[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer3[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[1]["<<v_index + k<<"].key: "<<vbuffer3[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[1]["<<v_index + k<<"].value: "<<vbuffer3[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer3[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[2]["<<v_index + k<<"].key: "<<vbuffer3[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[2]["<<v_index + k<<"].value: "<<vbuffer3[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer3[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[3]["<<v_index + k<<"].key: "<<vbuffer3[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[3]["<<v_index + k<<"].value: "<<vbuffer3[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer3[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[4]["<<v_index + k<<"].key: "<<vbuffer3[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[4]["<<v_index + k<<"].value: "<<vbuffer3[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer3[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[5]["<<v_index + k<<"].key: "<<vbuffer3[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[5]["<<v_index + k<<"].value: "<<vbuffer3[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer3[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[6]["<<v_index + k<<"].key: "<<vbuffer3[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[6]["<<v_index + k<<"].value: "<<vbuffer3[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer3[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[7]["<<v_index + k<<"].key: "<<vbuffer3[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[7]["<<v_index + k<<"].value: "<<vbuffer3[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer4[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[0]["<<v_index + k<<"].key: "<<vbuffer4[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[0]["<<v_index + k<<"].value: "<<vbuffer4[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer4[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[1]["<<v_index + k<<"].key: "<<vbuffer4[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[1]["<<v_index + k<<"].value: "<<vbuffer4[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer4[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[2]["<<v_index + k<<"].key: "<<vbuffer4[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[2]["<<v_index + k<<"].value: "<<vbuffer4[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer4[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[3]["<<v_index + k<<"].key: "<<vbuffer4[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[3]["<<v_index + k<<"].value: "<<vbuffer4[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer4[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[4]["<<v_index + k<<"].key: "<<vbuffer4[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[4]["<<v_index + k<<"].value: "<<vbuffer4[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer4[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[5]["<<v_index + k<<"].key: "<<vbuffer4[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[5]["<<v_index + k<<"].value: "<<vbuffer4[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer4[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[6]["<<v_index + k<<"].key: "<<vbuffer4[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[6]["<<v_index + k<<"].value: "<<vbuffer4[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer4[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[7]["<<v_index + k<<"].key: "<<vbuffer4[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[7]["<<v_index + k<<"].value: "<<vbuffer4[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_vbuffer_t vbuffer4[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_vbuffer_t vbuffer5[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[6];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 6; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 6; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 6;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 6; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 6; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram3, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram4, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram5, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[0]["<<v_index + k<<"].key: "<<vbuffer2[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[0]["<<v_index + k<<"].value: "<<vbuffer2[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[1]["<<v_index + k<<"].key: "<<vbuffer2[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[1]["<<v_index + k<<"].value: "<<vbuffer2[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[2]["<<v_index + k<<"].key: "<<vbuffer2[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[2]["<<v_index + k<<"].value: "<<vbuffer2[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[3]["<<v_index + k<<"].key: "<<vbuffer2[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[3]["<<v_index + k<<"].value: "<<vbuffer2[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[4]["<<v_index + k<<"].key: "<<vbuffer2[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[4]["<<v_index + k<<"].value: "<<vbuffer2[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[5]["<<v_index + k<<"].key: "<<vbuffer2[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[5]["<<v_index + k<<"].value: "<<vbuffer2[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[6]["<<v_index + k<<"].key: "<<vbuffer2[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[6]["<<v_index + k<<"].value: "<<vbuffer2[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[7]["<<v_index + k<<"].key: "<<vbuffer2[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[7]["<<v_index + k<<"].value: "<<vbuffer2[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer3[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[0]["<<v_index + k<<"].key: "<<vbuffer3[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[0]["<<v_index + k<<"].value: "<<vbuffer3[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer3[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[1]["<<v_index + k<<"].key: "<<vbuffer3[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[1]["<<v_index + k<<"].value: "<<vbuffer3[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer3[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[2]["<<v_index + k<<"].key: "<<vbuffer3[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[2]["<<v_index + k<<"].value: "<<vbuffer3[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer3[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[3]["<<v_index + k<<"].key: "<<vbuffer3[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[3]["<<v_index + k<<"].value: "<<vbuffer3[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer3[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[4]["<<v_index + k<<"].key: "<<vbuffer3[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[4]["<<v_index + k<<"].value: "<<vbuffer3[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer3[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[5]["<<v_index + k<<"].key: "<<vbuffer3[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[5]["<<v_index + k<<"].value: "<<vbuffer3[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer3[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[6]["<<v_index + k<<"].key: "<<vbuffer3[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[6]["<<v_index + k<<"].value: "<<vbuffer3[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer3[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[7]["<<v_index + k<<"].key: "<<vbuffer3[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[7]["<<v_index + k<<"].value: "<<vbuffer3[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer4[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[0]["<<v_index + k<<"].key: "<<vbuffer4[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[0]["<<v_index + k<<"].value: "<<vbuffer4[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer4[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[1]["<<v_index + k<<"].key: "<<vbuffer4[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[1]["<<v_index + k<<"].value: "<<vbuffer4[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer4[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[2]["<<v_index + k<<"].key: "<<vbuffer4[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[2]["<<v_index + k<<"].value: "<<vbuffer4[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer4[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[3]["<<v_index + k<<"].key: "<<vbuffer4[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[3]["<<v_index + k<<"].value: "<<vbuffer4[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer4[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[4]["<<v_index + k<<"].key: "<<vbuffer4[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[4]["<<v_index + k<<"].value: "<<vbuffer4[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer4[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[5]["<<v_index + k<<"].key: "<<vbuffer4[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[5]["<<v_index + k<<"].value: "<<vbuffer4[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer4[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[6]["<<v_index + k<<"].key: "<<vbuffer4[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[6]["<<v_index + k<<"].value: "<<vbuffer4[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer4[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[7]["<<v_index + k<<"].key: "<<vbuffer4[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[7]["<<v_index + k<<"].value: "<<vbuffer4[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer5[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[0]["<<v_index + k<<"].key: "<<vbuffer5[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[0]["<<v_index + k<<"].value: "<<vbuffer5[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer5[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[1]["<<v_index + k<<"].key: "<<vbuffer5[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[1]["<<v_index + k<<"].value: "<<vbuffer5[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer5[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[2]["<<v_index + k<<"].key: "<<vbuffer5[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[2]["<<v_index + k<<"].value: "<<vbuffer5[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer5[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[3]["<<v_index + k<<"].key: "<<vbuffer5[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[3]["<<v_index + k<<"].value: "<<vbuffer5[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer5[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[4]["<<v_index + k<<"].key: "<<vbuffer5[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[4]["<<v_index + k<<"].value: "<<vbuffer5[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer5[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[5]["<<v_index + k<<"].key: "<<vbuffer5[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[5]["<<v_index + k<<"].value: "<<vbuffer5[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer5[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[6]["<<v_index + k<<"].key: "<<vbuffer5[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[6]["<<v_index + k<<"].value: "<<vbuffer5[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer5[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[7]["<<v_index + k<<"].key: "<<vbuffer5[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[7]["<<v_index + k<<"].value: "<<vbuffer5[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_vbuffer_t vbuffer4[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_vbuffer_t vbuffer5[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_vbuffer_t vbuffer6[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[7];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 7; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 7; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 7;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 7; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 7; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram3, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram4, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram5, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram6, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[0]["<<v_index + k<<"].key: "<<vbuffer2[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[0]["<<v_index + k<<"].value: "<<vbuffer2[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[1]["<<v_index + k<<"].key: "<<vbuffer2[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[1]["<<v_index + k<<"].value: "<<vbuffer2[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[2]["<<v_index + k<<"].key: "<<vbuffer2[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[2]["<<v_index + k<<"].value: "<<vbuffer2[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[3]["<<v_index + k<<"].key: "<<vbuffer2[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[3]["<<v_index + k<<"].value: "<<vbuffer2[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[4]["<<v_index + k<<"].key: "<<vbuffer2[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[4]["<<v_index + k<<"].value: "<<vbuffer2[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[5]["<<v_index + k<<"].key: "<<vbuffer2[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[5]["<<v_index + k<<"].value: "<<vbuffer2[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[6]["<<v_index + k<<"].key: "<<vbuffer2[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[6]["<<v_index + k<<"].value: "<<vbuffer2[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[7]["<<v_index + k<<"].key: "<<vbuffer2[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[7]["<<v_index + k<<"].value: "<<vbuffer2[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer3[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[0]["<<v_index + k<<"].key: "<<vbuffer3[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[0]["<<v_index + k<<"].value: "<<vbuffer3[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer3[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[1]["<<v_index + k<<"].key: "<<vbuffer3[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[1]["<<v_index + k<<"].value: "<<vbuffer3[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer3[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[2]["<<v_index + k<<"].key: "<<vbuffer3[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[2]["<<v_index + k<<"].value: "<<vbuffer3[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer3[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[3]["<<v_index + k<<"].key: "<<vbuffer3[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[3]["<<v_index + k<<"].value: "<<vbuffer3[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer3[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[4]["<<v_index + k<<"].key: "<<vbuffer3[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[4]["<<v_index + k<<"].value: "<<vbuffer3[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer3[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[5]["<<v_index + k<<"].key: "<<vbuffer3[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[5]["<<v_index + k<<"].value: "<<vbuffer3[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer3[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[6]["<<v_index + k<<"].key: "<<vbuffer3[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[6]["<<v_index + k<<"].value: "<<vbuffer3[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer3[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[7]["<<v_index + k<<"].key: "<<vbuffer3[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[7]["<<v_index + k<<"].value: "<<vbuffer3[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer4[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[0]["<<v_index + k<<"].key: "<<vbuffer4[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[0]["<<v_index + k<<"].value: "<<vbuffer4[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer4[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[1]["<<v_index + k<<"].key: "<<vbuffer4[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[1]["<<v_index + k<<"].value: "<<vbuffer4[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer4[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[2]["<<v_index + k<<"].key: "<<vbuffer4[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[2]["<<v_index + k<<"].value: "<<vbuffer4[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer4[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[3]["<<v_index + k<<"].key: "<<vbuffer4[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[3]["<<v_index + k<<"].value: "<<vbuffer4[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer4[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[4]["<<v_index + k<<"].key: "<<vbuffer4[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[4]["<<v_index + k<<"].value: "<<vbuffer4[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer4[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[5]["<<v_index + k<<"].key: "<<vbuffer4[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[5]["<<v_index + k<<"].value: "<<vbuffer4[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer4[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[6]["<<v_index + k<<"].key: "<<vbuffer4[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[6]["<<v_index + k<<"].value: "<<vbuffer4[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer4[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[7]["<<v_index + k<<"].key: "<<vbuffer4[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[7]["<<v_index + k<<"].value: "<<vbuffer4[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer5[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[0]["<<v_index + k<<"].key: "<<vbuffer5[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[0]["<<v_index + k<<"].value: "<<vbuffer5[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer5[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[1]["<<v_index + k<<"].key: "<<vbuffer5[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[1]["<<v_index + k<<"].value: "<<vbuffer5[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer5[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[2]["<<v_index + k<<"].key: "<<vbuffer5[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[2]["<<v_index + k<<"].value: "<<vbuffer5[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer5[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[3]["<<v_index + k<<"].key: "<<vbuffer5[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[3]["<<v_index + k<<"].value: "<<vbuffer5[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer5[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[4]["<<v_index + k<<"].key: "<<vbuffer5[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[4]["<<v_index + k<<"].value: "<<vbuffer5[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer5[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[5]["<<v_index + k<<"].key: "<<vbuffer5[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[5]["<<v_index + k<<"].value: "<<vbuffer5[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer5[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[6]["<<v_index + k<<"].key: "<<vbuffer5[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[6]["<<v_index + k<<"].value: "<<vbuffer5[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer5[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[7]["<<v_index + k<<"].key: "<<vbuffer5[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[7]["<<v_index + k<<"].value: "<<vbuffer5[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer6[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[0]["<<v_index + k<<"].key: "<<vbuffer6[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[0]["<<v_index + k<<"].value: "<<vbuffer6[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer6[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[1]["<<v_index + k<<"].key: "<<vbuffer6[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[1]["<<v_index + k<<"].value: "<<vbuffer6[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer6[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[2]["<<v_index + k<<"].key: "<<vbuffer6[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[2]["<<v_index + k<<"].value: "<<vbuffer6[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer6[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[3]["<<v_index + k<<"].key: "<<vbuffer6[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[3]["<<v_index + k<<"].value: "<<vbuffer6[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer6[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[4]["<<v_index + k<<"].key: "<<vbuffer6[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[4]["<<v_index + k<<"].value: "<<vbuffer6[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer6[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[5]["<<v_index + k<<"].key: "<<vbuffer6[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[5]["<<v_index + k<<"].value: "<<vbuffer6[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer6[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[6]["<<v_index + k<<"].key: "<<vbuffer6[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[6]["<<v_index + k<<"].value: "<<vbuffer6[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer6[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[7]["<<v_index + k<<"].key: "<<vbuffer6[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[7]["<<v_index + k<<"].value: "<<vbuffer6[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_vbuffer_t vbuffer4[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_vbuffer_t vbuffer5[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_vbuffer_t vbuffer6[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_vbuffer_t vbuffer7[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[8];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 8; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 8; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 8;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[7] = acts_utilobj->UTIL_gettravstate(ON, kvdram7, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 8; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 8; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram3, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram4, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram5, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram6, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram7, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer7, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[0]["<<v_index + k<<"].key: "<<vbuffer2[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[0]["<<v_index + k<<"].value: "<<vbuffer2[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[1]["<<v_index + k<<"].key: "<<vbuffer2[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[1]["<<v_index + k<<"].value: "<<vbuffer2[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[2]["<<v_index + k<<"].key: "<<vbuffer2[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[2]["<<v_index + k<<"].value: "<<vbuffer2[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[3]["<<v_index + k<<"].key: "<<vbuffer2[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[3]["<<v_index + k<<"].value: "<<vbuffer2[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[4]["<<v_index + k<<"].key: "<<vbuffer2[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[4]["<<v_index + k<<"].value: "<<vbuffer2[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[5]["<<v_index + k<<"].key: "<<vbuffer2[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[5]["<<v_index + k<<"].value: "<<vbuffer2[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[6]["<<v_index + k<<"].key: "<<vbuffer2[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[6]["<<v_index + k<<"].value: "<<vbuffer2[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[7]["<<v_index + k<<"].key: "<<vbuffer2[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[7]["<<v_index + k<<"].value: "<<vbuffer2[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer3[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[0]["<<v_index + k<<"].key: "<<vbuffer3[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[0]["<<v_index + k<<"].value: "<<vbuffer3[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer3[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[1]["<<v_index + k<<"].key: "<<vbuffer3[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[1]["<<v_index + k<<"].value: "<<vbuffer3[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer3[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[2]["<<v_index + k<<"].key: "<<vbuffer3[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[2]["<<v_index + k<<"].value: "<<vbuffer3[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer3[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[3]["<<v_index + k<<"].key: "<<vbuffer3[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[3]["<<v_index + k<<"].value: "<<vbuffer3[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer3[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[4]["<<v_index + k<<"].key: "<<vbuffer3[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[4]["<<v_index + k<<"].value: "<<vbuffer3[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer3[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[5]["<<v_index + k<<"].key: "<<vbuffer3[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[5]["<<v_index + k<<"].value: "<<vbuffer3[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer3[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[6]["<<v_index + k<<"].key: "<<vbuffer3[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[6]["<<v_index + k<<"].value: "<<vbuffer3[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer3[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[7]["<<v_index + k<<"].key: "<<vbuffer3[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[7]["<<v_index + k<<"].value: "<<vbuffer3[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer4[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[0]["<<v_index + k<<"].key: "<<vbuffer4[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[0]["<<v_index + k<<"].value: "<<vbuffer4[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer4[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[1]["<<v_index + k<<"].key: "<<vbuffer4[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[1]["<<v_index + k<<"].value: "<<vbuffer4[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer4[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[2]["<<v_index + k<<"].key: "<<vbuffer4[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[2]["<<v_index + k<<"].value: "<<vbuffer4[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer4[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[3]["<<v_index + k<<"].key: "<<vbuffer4[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[3]["<<v_index + k<<"].value: "<<vbuffer4[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer4[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[4]["<<v_index + k<<"].key: "<<vbuffer4[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[4]["<<v_index + k<<"].value: "<<vbuffer4[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer4[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[5]["<<v_index + k<<"].key: "<<vbuffer4[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[5]["<<v_index + k<<"].value: "<<vbuffer4[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer4[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[6]["<<v_index + k<<"].key: "<<vbuffer4[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[6]["<<v_index + k<<"].value: "<<vbuffer4[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer4[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[7]["<<v_index + k<<"].key: "<<vbuffer4[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[7]["<<v_index + k<<"].value: "<<vbuffer4[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer5[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[0]["<<v_index + k<<"].key: "<<vbuffer5[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[0]["<<v_index + k<<"].value: "<<vbuffer5[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer5[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[1]["<<v_index + k<<"].key: "<<vbuffer5[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[1]["<<v_index + k<<"].value: "<<vbuffer5[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer5[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[2]["<<v_index + k<<"].key: "<<vbuffer5[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[2]["<<v_index + k<<"].value: "<<vbuffer5[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer5[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[3]["<<v_index + k<<"].key: "<<vbuffer5[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[3]["<<v_index + k<<"].value: "<<vbuffer5[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer5[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[4]["<<v_index + k<<"].key: "<<vbuffer5[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[4]["<<v_index + k<<"].value: "<<vbuffer5[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer5[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[5]["<<v_index + k<<"].key: "<<vbuffer5[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[5]["<<v_index + k<<"].value: "<<vbuffer5[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer5[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[6]["<<v_index + k<<"].key: "<<vbuffer5[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[6]["<<v_index + k<<"].value: "<<vbuffer5[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer5[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[7]["<<v_index + k<<"].key: "<<vbuffer5[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[7]["<<v_index + k<<"].value: "<<vbuffer5[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer6[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[0]["<<v_index + k<<"].key: "<<vbuffer6[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[0]["<<v_index + k<<"].value: "<<vbuffer6[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer6[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[1]["<<v_index + k<<"].key: "<<vbuffer6[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[1]["<<v_index + k<<"].value: "<<vbuffer6[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer6[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[2]["<<v_index + k<<"].key: "<<vbuffer6[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[2]["<<v_index + k<<"].value: "<<vbuffer6[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer6[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[3]["<<v_index + k<<"].key: "<<vbuffer6[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[3]["<<v_index + k<<"].value: "<<vbuffer6[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer6[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[4]["<<v_index + k<<"].key: "<<vbuffer6[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[4]["<<v_index + k<<"].value: "<<vbuffer6[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer6[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[5]["<<v_index + k<<"].key: "<<vbuffer6[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[5]["<<v_index + k<<"].value: "<<vbuffer6[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer6[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[6]["<<v_index + k<<"].key: "<<vbuffer6[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[6]["<<v_index + k<<"].value: "<<vbuffer6[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer6[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[7]["<<v_index + k<<"].key: "<<vbuffer6[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[7]["<<v_index + k<<"].value: "<<vbuffer6[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer7[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[0]["<<v_index + k<<"].key: "<<vbuffer7[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[0]["<<v_index + k<<"].value: "<<vbuffer7[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer7[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[1]["<<v_index + k<<"].key: "<<vbuffer7[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[1]["<<v_index + k<<"].value: "<<vbuffer7[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer7[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[2]["<<v_index + k<<"].key: "<<vbuffer7[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[2]["<<v_index + k<<"].value: "<<vbuffer7[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer7[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[3]["<<v_index + k<<"].key: "<<vbuffer7[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[3]["<<v_index + k<<"].value: "<<vbuffer7[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer7[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[4]["<<v_index + k<<"].key: "<<vbuffer7[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[4]["<<v_index + k<<"].value: "<<vbuffer7[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer7[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[5]["<<v_index + k<<"].key: "<<vbuffer7[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[5]["<<v_index + k<<"].value: "<<vbuffer7[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer7[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[6]["<<v_index + k<<"].key: "<<vbuffer7[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[6]["<<v_index + k<<"].value: "<<vbuffer7[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer7[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[7]["<<v_index + k<<"].key: "<<vbuffer7[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[7]["<<v_index + k<<"].value: "<<vbuffer7[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_vbuffer_t vbuffer4[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_vbuffer_t vbuffer5[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_vbuffer_t vbuffer6[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_vbuffer_t vbuffer7[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_vbuffer_t vbuffer8[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[9];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 9; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 9; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 9;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[7] = acts_utilobj->UTIL_gettravstate(ON, kvdram7, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[8] = acts_utilobj->UTIL_gettravstate(ON, kvdram8, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 9; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 9; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram3, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram4, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram5, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram6, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram7, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer7, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram8, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer8, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[0]["<<v_index + k<<"].key: "<<vbuffer2[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[0]["<<v_index + k<<"].value: "<<vbuffer2[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[1]["<<v_index + k<<"].key: "<<vbuffer2[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[1]["<<v_index + k<<"].value: "<<vbuffer2[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[2]["<<v_index + k<<"].key: "<<vbuffer2[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[2]["<<v_index + k<<"].value: "<<vbuffer2[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[3]["<<v_index + k<<"].key: "<<vbuffer2[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[3]["<<v_index + k<<"].value: "<<vbuffer2[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[4]["<<v_index + k<<"].key: "<<vbuffer2[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[4]["<<v_index + k<<"].value: "<<vbuffer2[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[5]["<<v_index + k<<"].key: "<<vbuffer2[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[5]["<<v_index + k<<"].value: "<<vbuffer2[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[6]["<<v_index + k<<"].key: "<<vbuffer2[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[6]["<<v_index + k<<"].value: "<<vbuffer2[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[7]["<<v_index + k<<"].key: "<<vbuffer2[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[7]["<<v_index + k<<"].value: "<<vbuffer2[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer3[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[0]["<<v_index + k<<"].key: "<<vbuffer3[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[0]["<<v_index + k<<"].value: "<<vbuffer3[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer3[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[1]["<<v_index + k<<"].key: "<<vbuffer3[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[1]["<<v_index + k<<"].value: "<<vbuffer3[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer3[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[2]["<<v_index + k<<"].key: "<<vbuffer3[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[2]["<<v_index + k<<"].value: "<<vbuffer3[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer3[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[3]["<<v_index + k<<"].key: "<<vbuffer3[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[3]["<<v_index + k<<"].value: "<<vbuffer3[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer3[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[4]["<<v_index + k<<"].key: "<<vbuffer3[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[4]["<<v_index + k<<"].value: "<<vbuffer3[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer3[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[5]["<<v_index + k<<"].key: "<<vbuffer3[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[5]["<<v_index + k<<"].value: "<<vbuffer3[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer3[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[6]["<<v_index + k<<"].key: "<<vbuffer3[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[6]["<<v_index + k<<"].value: "<<vbuffer3[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer3[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[7]["<<v_index + k<<"].key: "<<vbuffer3[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[7]["<<v_index + k<<"].value: "<<vbuffer3[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer4[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[0]["<<v_index + k<<"].key: "<<vbuffer4[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[0]["<<v_index + k<<"].value: "<<vbuffer4[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer4[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[1]["<<v_index + k<<"].key: "<<vbuffer4[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[1]["<<v_index + k<<"].value: "<<vbuffer4[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer4[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[2]["<<v_index + k<<"].key: "<<vbuffer4[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[2]["<<v_index + k<<"].value: "<<vbuffer4[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer4[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[3]["<<v_index + k<<"].key: "<<vbuffer4[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[3]["<<v_index + k<<"].value: "<<vbuffer4[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer4[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[4]["<<v_index + k<<"].key: "<<vbuffer4[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[4]["<<v_index + k<<"].value: "<<vbuffer4[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer4[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[5]["<<v_index + k<<"].key: "<<vbuffer4[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[5]["<<v_index + k<<"].value: "<<vbuffer4[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer4[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[6]["<<v_index + k<<"].key: "<<vbuffer4[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[6]["<<v_index + k<<"].value: "<<vbuffer4[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer4[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[7]["<<v_index + k<<"].key: "<<vbuffer4[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[7]["<<v_index + k<<"].value: "<<vbuffer4[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer5[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[0]["<<v_index + k<<"].key: "<<vbuffer5[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[0]["<<v_index + k<<"].value: "<<vbuffer5[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer5[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[1]["<<v_index + k<<"].key: "<<vbuffer5[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[1]["<<v_index + k<<"].value: "<<vbuffer5[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer5[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[2]["<<v_index + k<<"].key: "<<vbuffer5[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[2]["<<v_index + k<<"].value: "<<vbuffer5[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer5[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[3]["<<v_index + k<<"].key: "<<vbuffer5[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[3]["<<v_index + k<<"].value: "<<vbuffer5[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer5[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[4]["<<v_index + k<<"].key: "<<vbuffer5[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[4]["<<v_index + k<<"].value: "<<vbuffer5[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer5[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[5]["<<v_index + k<<"].key: "<<vbuffer5[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[5]["<<v_index + k<<"].value: "<<vbuffer5[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer5[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[6]["<<v_index + k<<"].key: "<<vbuffer5[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[6]["<<v_index + k<<"].value: "<<vbuffer5[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer5[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[7]["<<v_index + k<<"].key: "<<vbuffer5[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[7]["<<v_index + k<<"].value: "<<vbuffer5[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer6[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[0]["<<v_index + k<<"].key: "<<vbuffer6[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[0]["<<v_index + k<<"].value: "<<vbuffer6[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer6[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[1]["<<v_index + k<<"].key: "<<vbuffer6[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[1]["<<v_index + k<<"].value: "<<vbuffer6[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer6[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[2]["<<v_index + k<<"].key: "<<vbuffer6[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[2]["<<v_index + k<<"].value: "<<vbuffer6[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer6[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[3]["<<v_index + k<<"].key: "<<vbuffer6[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[3]["<<v_index + k<<"].value: "<<vbuffer6[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer6[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[4]["<<v_index + k<<"].key: "<<vbuffer6[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[4]["<<v_index + k<<"].value: "<<vbuffer6[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer6[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[5]["<<v_index + k<<"].key: "<<vbuffer6[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[5]["<<v_index + k<<"].value: "<<vbuffer6[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer6[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[6]["<<v_index + k<<"].key: "<<vbuffer6[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[6]["<<v_index + k<<"].value: "<<vbuffer6[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer6[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[7]["<<v_index + k<<"].key: "<<vbuffer6[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[7]["<<v_index + k<<"].value: "<<vbuffer6[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer7[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[0]["<<v_index + k<<"].key: "<<vbuffer7[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[0]["<<v_index + k<<"].value: "<<vbuffer7[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer7[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[1]["<<v_index + k<<"].key: "<<vbuffer7[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[1]["<<v_index + k<<"].value: "<<vbuffer7[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer7[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[2]["<<v_index + k<<"].key: "<<vbuffer7[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[2]["<<v_index + k<<"].value: "<<vbuffer7[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer7[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[3]["<<v_index + k<<"].key: "<<vbuffer7[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[3]["<<v_index + k<<"].value: "<<vbuffer7[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer7[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[4]["<<v_index + k<<"].key: "<<vbuffer7[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[4]["<<v_index + k<<"].value: "<<vbuffer7[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer7[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[5]["<<v_index + k<<"].key: "<<vbuffer7[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[5]["<<v_index + k<<"].value: "<<vbuffer7[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer7[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[6]["<<v_index + k<<"].key: "<<vbuffer7[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[6]["<<v_index + k<<"].value: "<<vbuffer7[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer7[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[7]["<<v_index + k<<"].key: "<<vbuffer7[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[7]["<<v_index + k<<"].value: "<<vbuffer7[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer8[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[0]["<<v_index + k<<"].key: "<<vbuffer8[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[0]["<<v_index + k<<"].value: "<<vbuffer8[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer8[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[1]["<<v_index + k<<"].key: "<<vbuffer8[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[1]["<<v_index + k<<"].value: "<<vbuffer8[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer8[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[2]["<<v_index + k<<"].key: "<<vbuffer8[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[2]["<<v_index + k<<"].value: "<<vbuffer8[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer8[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[3]["<<v_index + k<<"].key: "<<vbuffer8[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[3]["<<v_index + k<<"].value: "<<vbuffer8[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer8[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[4]["<<v_index + k<<"].key: "<<vbuffer8[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[4]["<<v_index + k<<"].value: "<<vbuffer8[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer8[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[5]["<<v_index + k<<"].key: "<<vbuffer8[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[5]["<<v_index + k<<"].value: "<<vbuffer8[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer8[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[6]["<<v_index + k<<"].key: "<<vbuffer8[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[6]["<<v_index + k<<"].value: "<<vbuffer8[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer8[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[7]["<<v_index + k<<"].key: "<<vbuffer8[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[7]["<<v_index + k<<"].value: "<<vbuffer8[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_vbuffer_t vbuffer4[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_vbuffer_t vbuffer5[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_vbuffer_t vbuffer6[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_vbuffer_t vbuffer7[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_vbuffer_t vbuffer8[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	keyvalue_vbuffer_t vbuffer9[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[10];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 10; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 10; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 10;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[7] = acts_utilobj->UTIL_gettravstate(ON, kvdram7, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[8] = acts_utilobj->UTIL_gettravstate(ON, kvdram8, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[9] = acts_utilobj->UTIL_gettravstate(ON, kvdram9, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 10; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 10; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram3, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram4, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram5, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram6, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram7, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer7, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram8, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer8, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram9, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer9, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[0]["<<v_index + k<<"].key: "<<vbuffer2[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[0]["<<v_index + k<<"].value: "<<vbuffer2[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[1]["<<v_index + k<<"].key: "<<vbuffer2[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[1]["<<v_index + k<<"].value: "<<vbuffer2[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[2]["<<v_index + k<<"].key: "<<vbuffer2[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[2]["<<v_index + k<<"].value: "<<vbuffer2[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[3]["<<v_index + k<<"].key: "<<vbuffer2[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[3]["<<v_index + k<<"].value: "<<vbuffer2[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[4]["<<v_index + k<<"].key: "<<vbuffer2[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[4]["<<v_index + k<<"].value: "<<vbuffer2[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[5]["<<v_index + k<<"].key: "<<vbuffer2[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[5]["<<v_index + k<<"].value: "<<vbuffer2[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[6]["<<v_index + k<<"].key: "<<vbuffer2[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[6]["<<v_index + k<<"].value: "<<vbuffer2[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[7]["<<v_index + k<<"].key: "<<vbuffer2[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[7]["<<v_index + k<<"].value: "<<vbuffer2[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer3[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[0]["<<v_index + k<<"].key: "<<vbuffer3[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[0]["<<v_index + k<<"].value: "<<vbuffer3[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer3[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[1]["<<v_index + k<<"].key: "<<vbuffer3[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[1]["<<v_index + k<<"].value: "<<vbuffer3[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer3[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[2]["<<v_index + k<<"].key: "<<vbuffer3[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[2]["<<v_index + k<<"].value: "<<vbuffer3[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer3[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[3]["<<v_index + k<<"].key: "<<vbuffer3[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[3]["<<v_index + k<<"].value: "<<vbuffer3[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer3[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[4]["<<v_index + k<<"].key: "<<vbuffer3[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[4]["<<v_index + k<<"].value: "<<vbuffer3[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer3[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[5]["<<v_index + k<<"].key: "<<vbuffer3[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[5]["<<v_index + k<<"].value: "<<vbuffer3[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer3[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[6]["<<v_index + k<<"].key: "<<vbuffer3[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[6]["<<v_index + k<<"].value: "<<vbuffer3[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer3[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[7]["<<v_index + k<<"].key: "<<vbuffer3[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[7]["<<v_index + k<<"].value: "<<vbuffer3[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer4[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[0]["<<v_index + k<<"].key: "<<vbuffer4[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[0]["<<v_index + k<<"].value: "<<vbuffer4[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer4[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[1]["<<v_index + k<<"].key: "<<vbuffer4[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[1]["<<v_index + k<<"].value: "<<vbuffer4[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer4[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[2]["<<v_index + k<<"].key: "<<vbuffer4[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[2]["<<v_index + k<<"].value: "<<vbuffer4[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer4[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[3]["<<v_index + k<<"].key: "<<vbuffer4[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[3]["<<v_index + k<<"].value: "<<vbuffer4[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer4[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[4]["<<v_index + k<<"].key: "<<vbuffer4[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[4]["<<v_index + k<<"].value: "<<vbuffer4[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer4[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[5]["<<v_index + k<<"].key: "<<vbuffer4[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[5]["<<v_index + k<<"].value: "<<vbuffer4[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer4[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[6]["<<v_index + k<<"].key: "<<vbuffer4[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[6]["<<v_index + k<<"].value: "<<vbuffer4[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer4[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[7]["<<v_index + k<<"].key: "<<vbuffer4[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[7]["<<v_index + k<<"].value: "<<vbuffer4[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer5[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[0]["<<v_index + k<<"].key: "<<vbuffer5[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[0]["<<v_index + k<<"].value: "<<vbuffer5[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer5[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[1]["<<v_index + k<<"].key: "<<vbuffer5[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[1]["<<v_index + k<<"].value: "<<vbuffer5[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer5[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[2]["<<v_index + k<<"].key: "<<vbuffer5[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[2]["<<v_index + k<<"].value: "<<vbuffer5[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer5[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[3]["<<v_index + k<<"].key: "<<vbuffer5[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[3]["<<v_index + k<<"].value: "<<vbuffer5[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer5[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[4]["<<v_index + k<<"].key: "<<vbuffer5[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[4]["<<v_index + k<<"].value: "<<vbuffer5[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer5[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[5]["<<v_index + k<<"].key: "<<vbuffer5[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[5]["<<v_index + k<<"].value: "<<vbuffer5[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer5[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[6]["<<v_index + k<<"].key: "<<vbuffer5[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[6]["<<v_index + k<<"].value: "<<vbuffer5[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer5[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[7]["<<v_index + k<<"].key: "<<vbuffer5[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[7]["<<v_index + k<<"].value: "<<vbuffer5[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer6[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[0]["<<v_index + k<<"].key: "<<vbuffer6[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[0]["<<v_index + k<<"].value: "<<vbuffer6[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer6[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[1]["<<v_index + k<<"].key: "<<vbuffer6[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[1]["<<v_index + k<<"].value: "<<vbuffer6[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer6[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[2]["<<v_index + k<<"].key: "<<vbuffer6[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[2]["<<v_index + k<<"].value: "<<vbuffer6[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer6[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[3]["<<v_index + k<<"].key: "<<vbuffer6[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[3]["<<v_index + k<<"].value: "<<vbuffer6[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer6[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[4]["<<v_index + k<<"].key: "<<vbuffer6[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[4]["<<v_index + k<<"].value: "<<vbuffer6[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer6[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[5]["<<v_index + k<<"].key: "<<vbuffer6[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[5]["<<v_index + k<<"].value: "<<vbuffer6[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer6[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[6]["<<v_index + k<<"].key: "<<vbuffer6[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[6]["<<v_index + k<<"].value: "<<vbuffer6[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer6[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[7]["<<v_index + k<<"].key: "<<vbuffer6[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[7]["<<v_index + k<<"].value: "<<vbuffer6[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer7[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[0]["<<v_index + k<<"].key: "<<vbuffer7[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[0]["<<v_index + k<<"].value: "<<vbuffer7[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer7[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[1]["<<v_index + k<<"].key: "<<vbuffer7[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[1]["<<v_index + k<<"].value: "<<vbuffer7[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer7[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[2]["<<v_index + k<<"].key: "<<vbuffer7[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[2]["<<v_index + k<<"].value: "<<vbuffer7[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer7[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[3]["<<v_index + k<<"].key: "<<vbuffer7[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[3]["<<v_index + k<<"].value: "<<vbuffer7[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer7[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[4]["<<v_index + k<<"].key: "<<vbuffer7[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[4]["<<v_index + k<<"].value: "<<vbuffer7[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer7[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[5]["<<v_index + k<<"].key: "<<vbuffer7[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[5]["<<v_index + k<<"].value: "<<vbuffer7[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer7[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[6]["<<v_index + k<<"].key: "<<vbuffer7[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[6]["<<v_index + k<<"].value: "<<vbuffer7[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer7[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[7]["<<v_index + k<<"].key: "<<vbuffer7[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[7]["<<v_index + k<<"].value: "<<vbuffer7[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer8[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[0]["<<v_index + k<<"].key: "<<vbuffer8[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[0]["<<v_index + k<<"].value: "<<vbuffer8[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer8[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[1]["<<v_index + k<<"].key: "<<vbuffer8[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[1]["<<v_index + k<<"].value: "<<vbuffer8[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer8[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[2]["<<v_index + k<<"].key: "<<vbuffer8[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[2]["<<v_index + k<<"].value: "<<vbuffer8[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer8[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[3]["<<v_index + k<<"].key: "<<vbuffer8[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[3]["<<v_index + k<<"].value: "<<vbuffer8[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer8[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[4]["<<v_index + k<<"].key: "<<vbuffer8[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[4]["<<v_index + k<<"].value: "<<vbuffer8[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer8[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[5]["<<v_index + k<<"].key: "<<vbuffer8[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[5]["<<v_index + k<<"].value: "<<vbuffer8[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer8[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[6]["<<v_index + k<<"].key: "<<vbuffer8[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[6]["<<v_index + k<<"].value: "<<vbuffer8[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer8[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[7]["<<v_index + k<<"].key: "<<vbuffer8[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[7]["<<v_index + k<<"].value: "<<vbuffer8[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer9[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[0]["<<v_index + k<<"].key: "<<vbuffer9[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[0]["<<v_index + k<<"].value: "<<vbuffer9[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer9[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[1]["<<v_index + k<<"].key: "<<vbuffer9[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[1]["<<v_index + k<<"].value: "<<vbuffer9[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer9[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[2]["<<v_index + k<<"].key: "<<vbuffer9[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[2]["<<v_index + k<<"].value: "<<vbuffer9[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer9[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[3]["<<v_index + k<<"].key: "<<vbuffer9[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[3]["<<v_index + k<<"].value: "<<vbuffer9[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer9[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[4]["<<v_index + k<<"].key: "<<vbuffer9[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[4]["<<v_index + k<<"].value: "<<vbuffer9[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer9[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[5]["<<v_index + k<<"].key: "<<vbuffer9[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[5]["<<v_index + k<<"].value: "<<vbuffer9[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer9[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[6]["<<v_index + k<<"].key: "<<vbuffer9[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[6]["<<v_index + k<<"].value: "<<vbuffer9[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer9[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[7]["<<v_index + k<<"].key: "<<vbuffer9[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[7]["<<v_index + k<<"].value: "<<vbuffer9[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_vbuffer_t vbuffer4[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_vbuffer_t vbuffer5[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_vbuffer_t vbuffer6[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_vbuffer_t vbuffer7[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_vbuffer_t vbuffer8[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	keyvalue_vbuffer_t vbuffer9[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	keyvalue_vbuffer_t vbuffer10[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[11];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 11; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 11; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 11;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[7] = acts_utilobj->UTIL_gettravstate(ON, kvdram7, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[8] = acts_utilobj->UTIL_gettravstate(ON, kvdram8, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[9] = acts_utilobj->UTIL_gettravstate(ON, kvdram9, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[10] = acts_utilobj->UTIL_gettravstate(ON, kvdram10, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 11; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 11; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram3, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram4, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram5, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram6, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram7, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer7, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram8, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer8, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram9, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer9, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram10, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer10, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[0]["<<v_index + k<<"].key: "<<vbuffer2[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[0]["<<v_index + k<<"].value: "<<vbuffer2[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[1]["<<v_index + k<<"].key: "<<vbuffer2[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[1]["<<v_index + k<<"].value: "<<vbuffer2[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[2]["<<v_index + k<<"].key: "<<vbuffer2[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[2]["<<v_index + k<<"].value: "<<vbuffer2[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[3]["<<v_index + k<<"].key: "<<vbuffer2[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[3]["<<v_index + k<<"].value: "<<vbuffer2[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[4]["<<v_index + k<<"].key: "<<vbuffer2[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[4]["<<v_index + k<<"].value: "<<vbuffer2[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[5]["<<v_index + k<<"].key: "<<vbuffer2[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[5]["<<v_index + k<<"].value: "<<vbuffer2[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[6]["<<v_index + k<<"].key: "<<vbuffer2[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[6]["<<v_index + k<<"].value: "<<vbuffer2[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[7]["<<v_index + k<<"].key: "<<vbuffer2[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[7]["<<v_index + k<<"].value: "<<vbuffer2[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer3[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[0]["<<v_index + k<<"].key: "<<vbuffer3[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[0]["<<v_index + k<<"].value: "<<vbuffer3[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer3[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[1]["<<v_index + k<<"].key: "<<vbuffer3[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[1]["<<v_index + k<<"].value: "<<vbuffer3[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer3[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[2]["<<v_index + k<<"].key: "<<vbuffer3[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[2]["<<v_index + k<<"].value: "<<vbuffer3[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer3[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[3]["<<v_index + k<<"].key: "<<vbuffer3[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[3]["<<v_index + k<<"].value: "<<vbuffer3[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer3[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[4]["<<v_index + k<<"].key: "<<vbuffer3[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[4]["<<v_index + k<<"].value: "<<vbuffer3[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer3[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[5]["<<v_index + k<<"].key: "<<vbuffer3[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[5]["<<v_index + k<<"].value: "<<vbuffer3[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer3[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[6]["<<v_index + k<<"].key: "<<vbuffer3[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[6]["<<v_index + k<<"].value: "<<vbuffer3[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer3[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[7]["<<v_index + k<<"].key: "<<vbuffer3[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[7]["<<v_index + k<<"].value: "<<vbuffer3[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer4[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[0]["<<v_index + k<<"].key: "<<vbuffer4[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[0]["<<v_index + k<<"].value: "<<vbuffer4[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer4[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[1]["<<v_index + k<<"].key: "<<vbuffer4[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[1]["<<v_index + k<<"].value: "<<vbuffer4[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer4[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[2]["<<v_index + k<<"].key: "<<vbuffer4[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[2]["<<v_index + k<<"].value: "<<vbuffer4[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer4[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[3]["<<v_index + k<<"].key: "<<vbuffer4[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[3]["<<v_index + k<<"].value: "<<vbuffer4[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer4[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[4]["<<v_index + k<<"].key: "<<vbuffer4[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[4]["<<v_index + k<<"].value: "<<vbuffer4[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer4[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[5]["<<v_index + k<<"].key: "<<vbuffer4[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[5]["<<v_index + k<<"].value: "<<vbuffer4[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer4[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[6]["<<v_index + k<<"].key: "<<vbuffer4[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[6]["<<v_index + k<<"].value: "<<vbuffer4[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer4[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[7]["<<v_index + k<<"].key: "<<vbuffer4[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[7]["<<v_index + k<<"].value: "<<vbuffer4[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer5[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[0]["<<v_index + k<<"].key: "<<vbuffer5[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[0]["<<v_index + k<<"].value: "<<vbuffer5[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer5[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[1]["<<v_index + k<<"].key: "<<vbuffer5[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[1]["<<v_index + k<<"].value: "<<vbuffer5[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer5[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[2]["<<v_index + k<<"].key: "<<vbuffer5[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[2]["<<v_index + k<<"].value: "<<vbuffer5[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer5[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[3]["<<v_index + k<<"].key: "<<vbuffer5[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[3]["<<v_index + k<<"].value: "<<vbuffer5[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer5[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[4]["<<v_index + k<<"].key: "<<vbuffer5[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[4]["<<v_index + k<<"].value: "<<vbuffer5[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer5[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[5]["<<v_index + k<<"].key: "<<vbuffer5[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[5]["<<v_index + k<<"].value: "<<vbuffer5[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer5[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[6]["<<v_index + k<<"].key: "<<vbuffer5[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[6]["<<v_index + k<<"].value: "<<vbuffer5[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer5[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[7]["<<v_index + k<<"].key: "<<vbuffer5[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[7]["<<v_index + k<<"].value: "<<vbuffer5[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer6[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[0]["<<v_index + k<<"].key: "<<vbuffer6[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[0]["<<v_index + k<<"].value: "<<vbuffer6[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer6[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[1]["<<v_index + k<<"].key: "<<vbuffer6[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[1]["<<v_index + k<<"].value: "<<vbuffer6[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer6[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[2]["<<v_index + k<<"].key: "<<vbuffer6[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[2]["<<v_index + k<<"].value: "<<vbuffer6[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer6[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[3]["<<v_index + k<<"].key: "<<vbuffer6[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[3]["<<v_index + k<<"].value: "<<vbuffer6[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer6[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[4]["<<v_index + k<<"].key: "<<vbuffer6[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[4]["<<v_index + k<<"].value: "<<vbuffer6[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer6[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[5]["<<v_index + k<<"].key: "<<vbuffer6[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[5]["<<v_index + k<<"].value: "<<vbuffer6[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer6[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[6]["<<v_index + k<<"].key: "<<vbuffer6[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[6]["<<v_index + k<<"].value: "<<vbuffer6[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer6[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[7]["<<v_index + k<<"].key: "<<vbuffer6[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[7]["<<v_index + k<<"].value: "<<vbuffer6[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer7[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[0]["<<v_index + k<<"].key: "<<vbuffer7[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[0]["<<v_index + k<<"].value: "<<vbuffer7[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer7[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[1]["<<v_index + k<<"].key: "<<vbuffer7[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[1]["<<v_index + k<<"].value: "<<vbuffer7[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer7[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[2]["<<v_index + k<<"].key: "<<vbuffer7[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[2]["<<v_index + k<<"].value: "<<vbuffer7[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer7[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[3]["<<v_index + k<<"].key: "<<vbuffer7[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[3]["<<v_index + k<<"].value: "<<vbuffer7[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer7[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[4]["<<v_index + k<<"].key: "<<vbuffer7[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[4]["<<v_index + k<<"].value: "<<vbuffer7[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer7[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[5]["<<v_index + k<<"].key: "<<vbuffer7[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[5]["<<v_index + k<<"].value: "<<vbuffer7[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer7[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[6]["<<v_index + k<<"].key: "<<vbuffer7[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[6]["<<v_index + k<<"].value: "<<vbuffer7[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer7[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[7]["<<v_index + k<<"].key: "<<vbuffer7[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[7]["<<v_index + k<<"].value: "<<vbuffer7[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer8[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[0]["<<v_index + k<<"].key: "<<vbuffer8[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[0]["<<v_index + k<<"].value: "<<vbuffer8[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer8[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[1]["<<v_index + k<<"].key: "<<vbuffer8[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[1]["<<v_index + k<<"].value: "<<vbuffer8[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer8[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[2]["<<v_index + k<<"].key: "<<vbuffer8[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[2]["<<v_index + k<<"].value: "<<vbuffer8[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer8[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[3]["<<v_index + k<<"].key: "<<vbuffer8[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[3]["<<v_index + k<<"].value: "<<vbuffer8[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer8[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[4]["<<v_index + k<<"].key: "<<vbuffer8[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[4]["<<v_index + k<<"].value: "<<vbuffer8[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer8[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[5]["<<v_index + k<<"].key: "<<vbuffer8[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[5]["<<v_index + k<<"].value: "<<vbuffer8[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer8[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[6]["<<v_index + k<<"].key: "<<vbuffer8[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[6]["<<v_index + k<<"].value: "<<vbuffer8[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer8[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[7]["<<v_index + k<<"].key: "<<vbuffer8[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[7]["<<v_index + k<<"].value: "<<vbuffer8[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer9[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[0]["<<v_index + k<<"].key: "<<vbuffer9[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[0]["<<v_index + k<<"].value: "<<vbuffer9[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer9[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[1]["<<v_index + k<<"].key: "<<vbuffer9[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[1]["<<v_index + k<<"].value: "<<vbuffer9[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer9[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[2]["<<v_index + k<<"].key: "<<vbuffer9[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[2]["<<v_index + k<<"].value: "<<vbuffer9[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer9[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[3]["<<v_index + k<<"].key: "<<vbuffer9[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[3]["<<v_index + k<<"].value: "<<vbuffer9[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer9[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[4]["<<v_index + k<<"].key: "<<vbuffer9[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[4]["<<v_index + k<<"].value: "<<vbuffer9[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer9[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[5]["<<v_index + k<<"].key: "<<vbuffer9[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[5]["<<v_index + k<<"].value: "<<vbuffer9[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer9[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[6]["<<v_index + k<<"].key: "<<vbuffer9[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[6]["<<v_index + k<<"].value: "<<vbuffer9[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer9[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[7]["<<v_index + k<<"].key: "<<vbuffer9[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[7]["<<v_index + k<<"].value: "<<vbuffer9[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer10[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[0]["<<v_index + k<<"].key: "<<vbuffer10[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[0]["<<v_index + k<<"].value: "<<vbuffer10[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer10[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[1]["<<v_index + k<<"].key: "<<vbuffer10[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[1]["<<v_index + k<<"].value: "<<vbuffer10[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer10[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[2]["<<v_index + k<<"].key: "<<vbuffer10[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[2]["<<v_index + k<<"].value: "<<vbuffer10[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer10[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[3]["<<v_index + k<<"].key: "<<vbuffer10[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[3]["<<v_index + k<<"].value: "<<vbuffer10[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer10[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[4]["<<v_index + k<<"].key: "<<vbuffer10[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[4]["<<v_index + k<<"].value: "<<vbuffer10[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer10[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[5]["<<v_index + k<<"].key: "<<vbuffer10[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[5]["<<v_index + k<<"].value: "<<vbuffer10[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer10[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[6]["<<v_index + k<<"].key: "<<vbuffer10[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[6]["<<v_index + k<<"].value: "<<vbuffer10[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer10[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[7]["<<v_index + k<<"].key: "<<vbuffer10[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[7]["<<v_index + k<<"].value: "<<vbuffer10[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
void
	#ifdef SW 
	acts_merge::
	#endif
MERGE_merge12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram){
	#pragma HLS INLINE
	
	#if defined(_DEBUGMODE_KERNELPRINTS2)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH);
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	
	keyvalue_vbuffer_t vbuffer0[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer0
	keyvalue_vbuffer_t vbuffer1[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer1
	keyvalue_vbuffer_t vbuffer2[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer2
	keyvalue_vbuffer_t vbuffer3[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer3
	keyvalue_vbuffer_t vbuffer4[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer4
	keyvalue_vbuffer_t vbuffer5[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer5
	keyvalue_vbuffer_t vbuffer6[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer6
	keyvalue_vbuffer_t vbuffer7[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer7
	keyvalue_vbuffer_t vbuffer8[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer8
	keyvalue_vbuffer_t vbuffer9[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer9
	keyvalue_vbuffer_t vbuffer10[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer10
	keyvalue_vbuffer_t vbuffer11[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer11
	
	keyvalue_vbuffer_t mergedv_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = mergedv_buffer
	
	travstate_t rtravstates[12];
	#pragma HLS ARRAY_PARTITION variable=rtravstates complete
	
	value_t buffer[DOUBLE_BLOCKRAM_SIZE];
	for(unsigned int i=0; i<DOUBLE_BLOCKRAM_SIZE; i++){ buffer[i] = 0; } 
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	#endif 
	
	unsigned int _MERGECHUNKSZ = reducebuffersz / 12; // SIMPLIFYME.
	unsigned int _MERGECHUNKSCOUNT = 12; // FIXME. edge-condition // (reducebuffersz + (_MERGECHUNKSZ-1)) / _MERGECHUNKSZ;
	unsigned int total_num_blocks = 16 * 2; //  (globalparams.SIZE_BATCHRANGE / 16) / reducebuffersz; // SIMPLIFYME. CRITICAL AUTOMATEME.

	TOPKERNELMERGE_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		unsigned int vreadoffset_kvs = iterationidx * reducebuffersz;
		unsigned int vwriteoffset_kvs = iterationidx * reducebuffersz * 12;
		unsigned int vwritebufferoffset_kvs = 0;
		unsigned int partition = iterationidx / 2;
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<">>> ACTS MERGE: merge iterationidx started: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks), [vreadoffset: "<<vreadoffset_kvs * VECTOR2_SIZE<<", vwriteoffset: "<<vwriteoffset_kvs * VECTOR2_SIZE<<"]"<<endl;
		#endif
		
		unsigned int mv_index = 0;
		unsigned int v_index = 0;
		keyvalue_t udata0;
		keyvalue_t vdata0;
		keyvalue_t res0;
		keyvalue_t udata1;
		keyvalue_t vdata1;
		keyvalue_t res1;
		keyvalue_t udata2;
		keyvalue_t vdata2;
		keyvalue_t res2;
		keyvalue_t udata3;
		keyvalue_t vdata3;
		keyvalue_t res3;
		keyvalue_t udata4;
		keyvalue_t vdata4;
		keyvalue_t res4;
		keyvalue_t udata5;
		keyvalue_t vdata5;
		keyvalue_t res5;
		keyvalue_t udata6;
		keyvalue_t vdata6;
		keyvalue_t res6;
		keyvalue_t udata7;
		keyvalue_t vdata7;
		keyvalue_t res7;
		
		// skipping logic 
		bool_type enablemerge = OFF; 
		unsigned int ntravszs = 0;
		rtravstates[0] = acts_utilobj->UTIL_gettravstate(ON, kvdram0, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[1] = acts_utilobj->UTIL_gettravstate(ON, kvdram1, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[2] = acts_utilobj->UTIL_gettravstate(ON, kvdram2, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[3] = acts_utilobj->UTIL_gettravstate(ON, kvdram3, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[4] = acts_utilobj->UTIL_gettravstate(ON, kvdram4, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[5] = acts_utilobj->UTIL_gettravstate(ON, kvdram5, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[6] = acts_utilobj->UTIL_gettravstate(ON, kvdram6, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[7] = acts_utilobj->UTIL_gettravstate(ON, kvdram7, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[8] = acts_utilobj->UTIL_gettravstate(ON, kvdram8, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[9] = acts_utilobj->UTIL_gettravstate(ON, kvdram9, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[10] = acts_utilobj->UTIL_gettravstate(ON, kvdram10, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		rtravstates[11] = acts_utilobj->UTIL_gettravstate(ON, kvdram11, globalparams, globalparams.ACTSPARAMS_TREEDEPTH, iterationidx/2); // CRITICAL NOTEME. POSSIBLE SOURCE OF ROUTING CONSTRAINTS?
		for(unsigned int i = 0; i < 12; i++){ ntravszs += rtravstates[i].size_kvs; }
		if(ntravszs > 0){ enablemerge = ON; } else { enablemerge = OFF; }
		cout<<"---- acts_merge:: printing rtravstates "<<endl; for(unsigned int i = 0; i < 12; i++){ cout<<rtravstates[i].size_kvs<<", "; } cout<<endl;
		if(enablemerge == ON){ cout<<"---- acts_merge:: iterationidx: "<<iterationidx<<", partition: "<<partition<<", enablemerge: "<<enablemerge<<", ntravszs: "<<ntravszs<<endl; }
		continue; /////////////
		
		MERGE_readvdata(enablemerge, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer2, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram3, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer3, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram4, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer4, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram5, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer5, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram6, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer6, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram7, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer7, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram8, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer8, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram9, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer9, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram10, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer10, 0, reducebuffersz);
		MERGE_readvdata(enablemerge, kvdram11, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffset_kvs, vbuffer11, 0, reducebuffersz);
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		MERGE_readvdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffset_kvs, refbuffer, 0, reducebuffersz);
		#endif 
		
		if(enablemerge==ON){
			for(unsigned int i=0; i<_MERGECHUNKSCOUNT; i++){ // _MERGECHUNKSCOUNT
				for(unsigned int k=0; k<_MERGECHUNKSZ; k++){ // _MERGECHUNKSZ. FIXME. edge-condition
				#pragma HLS PIPELINE II=1
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[0]["<<v_index + k<<"].key: "<<vbuffer0[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[0]["<<v_index + k<<"].value: "<<vbuffer0[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[1]["<<v_index + k<<"].key: "<<vbuffer0[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[1]["<<v_index + k<<"].value: "<<vbuffer0[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[2]["<<v_index + k<<"].key: "<<vbuffer0[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[2]["<<v_index + k<<"].value: "<<vbuffer0[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[3]["<<v_index + k<<"].key: "<<vbuffer0[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[3]["<<v_index + k<<"].value: "<<vbuffer0[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[4]["<<v_index + k<<"].key: "<<vbuffer0[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[4]["<<v_index + k<<"].value: "<<vbuffer0[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[5]["<<v_index + k<<"].key: "<<vbuffer0[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[5]["<<v_index + k<<"].value: "<<vbuffer0[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[6]["<<v_index + k<<"].key: "<<vbuffer0[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[6]["<<v_index + k<<"].value: "<<vbuffer0[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer0[7]["<<v_index + k<<"].key: "<<vbuffer0[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer0[7]["<<v_index + k<<"].value: "<<vbuffer0[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[0]["<<v_index + k<<"].key: "<<vbuffer1[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[0]["<<v_index + k<<"].value: "<<vbuffer1[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[1]["<<v_index + k<<"].key: "<<vbuffer1[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[1]["<<v_index + k<<"].value: "<<vbuffer1[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[2]["<<v_index + k<<"].key: "<<vbuffer1[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[2]["<<v_index + k<<"].value: "<<vbuffer1[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[3]["<<v_index + k<<"].key: "<<vbuffer1[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[3]["<<v_index + k<<"].value: "<<vbuffer1[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[4]["<<v_index + k<<"].key: "<<vbuffer1[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[4]["<<v_index + k<<"].value: "<<vbuffer1[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[5]["<<v_index + k<<"].key: "<<vbuffer1[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[5]["<<v_index + k<<"].value: "<<vbuffer1[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[6]["<<v_index + k<<"].key: "<<vbuffer1[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[6]["<<v_index + k<<"].value: "<<vbuffer1[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer1[7]["<<v_index + k<<"].key: "<<vbuffer1[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer1[7]["<<v_index + k<<"].value: "<<vbuffer1[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[0]["<<v_index + k<<"].key: "<<vbuffer2[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[0]["<<v_index + k<<"].value: "<<vbuffer2[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[1]["<<v_index + k<<"].key: "<<vbuffer2[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[1]["<<v_index + k<<"].value: "<<vbuffer2[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[2]["<<v_index + k<<"].key: "<<vbuffer2[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[2]["<<v_index + k<<"].value: "<<vbuffer2[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[3]["<<v_index + k<<"].key: "<<vbuffer2[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[3]["<<v_index + k<<"].value: "<<vbuffer2[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[4]["<<v_index + k<<"].key: "<<vbuffer2[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[4]["<<v_index + k<<"].value: "<<vbuffer2[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[5]["<<v_index + k<<"].key: "<<vbuffer2[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[5]["<<v_index + k<<"].value: "<<vbuffer2[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[6]["<<v_index + k<<"].key: "<<vbuffer2[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[6]["<<v_index + k<<"].value: "<<vbuffer2[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer2[7]["<<v_index + k<<"].key: "<<vbuffer2[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer2[7]["<<v_index + k<<"].value: "<<vbuffer2[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer3[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[0]["<<v_index + k<<"].key: "<<vbuffer3[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[0]["<<v_index + k<<"].value: "<<vbuffer3[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer3[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[1]["<<v_index + k<<"].key: "<<vbuffer3[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[1]["<<v_index + k<<"].value: "<<vbuffer3[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer3[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[2]["<<v_index + k<<"].key: "<<vbuffer3[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[2]["<<v_index + k<<"].value: "<<vbuffer3[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer3[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[3]["<<v_index + k<<"].key: "<<vbuffer3[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[3]["<<v_index + k<<"].value: "<<vbuffer3[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer3[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[4]["<<v_index + k<<"].key: "<<vbuffer3[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[4]["<<v_index + k<<"].value: "<<vbuffer3[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer3[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[5]["<<v_index + k<<"].key: "<<vbuffer3[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[5]["<<v_index + k<<"].value: "<<vbuffer3[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer3[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[6]["<<v_index + k<<"].key: "<<vbuffer3[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[6]["<<v_index + k<<"].value: "<<vbuffer3[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer3[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer3[7]["<<v_index + k<<"].key: "<<vbuffer3[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer3[7]["<<v_index + k<<"].value: "<<vbuffer3[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer4[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[0]["<<v_index + k<<"].key: "<<vbuffer4[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[0]["<<v_index + k<<"].value: "<<vbuffer4[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer4[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[1]["<<v_index + k<<"].key: "<<vbuffer4[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[1]["<<v_index + k<<"].value: "<<vbuffer4[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer4[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[2]["<<v_index + k<<"].key: "<<vbuffer4[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[2]["<<v_index + k<<"].value: "<<vbuffer4[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer4[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[3]["<<v_index + k<<"].key: "<<vbuffer4[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[3]["<<v_index + k<<"].value: "<<vbuffer4[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer4[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[4]["<<v_index + k<<"].key: "<<vbuffer4[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[4]["<<v_index + k<<"].value: "<<vbuffer4[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer4[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[5]["<<v_index + k<<"].key: "<<vbuffer4[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[5]["<<v_index + k<<"].value: "<<vbuffer4[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer4[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[6]["<<v_index + k<<"].key: "<<vbuffer4[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[6]["<<v_index + k<<"].value: "<<vbuffer4[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer4[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer4[7]["<<v_index + k<<"].key: "<<vbuffer4[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer4[7]["<<v_index + k<<"].value: "<<vbuffer4[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer5[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[0]["<<v_index + k<<"].key: "<<vbuffer5[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[0]["<<v_index + k<<"].value: "<<vbuffer5[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer5[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[1]["<<v_index + k<<"].key: "<<vbuffer5[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[1]["<<v_index + k<<"].value: "<<vbuffer5[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer5[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[2]["<<v_index + k<<"].key: "<<vbuffer5[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[2]["<<v_index + k<<"].value: "<<vbuffer5[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer5[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[3]["<<v_index + k<<"].key: "<<vbuffer5[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[3]["<<v_index + k<<"].value: "<<vbuffer5[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer5[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[4]["<<v_index + k<<"].key: "<<vbuffer5[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[4]["<<v_index + k<<"].value: "<<vbuffer5[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer5[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[5]["<<v_index + k<<"].key: "<<vbuffer5[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[5]["<<v_index + k<<"].value: "<<vbuffer5[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer5[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[6]["<<v_index + k<<"].key: "<<vbuffer5[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[6]["<<v_index + k<<"].value: "<<vbuffer5[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer5[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer5[7]["<<v_index + k<<"].key: "<<vbuffer5[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer5[7]["<<v_index + k<<"].value: "<<vbuffer5[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer6[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[0]["<<v_index + k<<"].key: "<<vbuffer6[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[0]["<<v_index + k<<"].value: "<<vbuffer6[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer6[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[1]["<<v_index + k<<"].key: "<<vbuffer6[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[1]["<<v_index + k<<"].value: "<<vbuffer6[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer6[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[2]["<<v_index + k<<"].key: "<<vbuffer6[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[2]["<<v_index + k<<"].value: "<<vbuffer6[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer6[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[3]["<<v_index + k<<"].key: "<<vbuffer6[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[3]["<<v_index + k<<"].value: "<<vbuffer6[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer6[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[4]["<<v_index + k<<"].key: "<<vbuffer6[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[4]["<<v_index + k<<"].value: "<<vbuffer6[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer6[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[5]["<<v_index + k<<"].key: "<<vbuffer6[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[5]["<<v_index + k<<"].value: "<<vbuffer6[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer6[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[6]["<<v_index + k<<"].key: "<<vbuffer6[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[6]["<<v_index + k<<"].value: "<<vbuffer6[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer6[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer6[7]["<<v_index + k<<"].key: "<<vbuffer6[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer6[7]["<<v_index + k<<"].value: "<<vbuffer6[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer7[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[0]["<<v_index + k<<"].key: "<<vbuffer7[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[0]["<<v_index + k<<"].value: "<<vbuffer7[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer7[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[1]["<<v_index + k<<"].key: "<<vbuffer7[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[1]["<<v_index + k<<"].value: "<<vbuffer7[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer7[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[2]["<<v_index + k<<"].key: "<<vbuffer7[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[2]["<<v_index + k<<"].value: "<<vbuffer7[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer7[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[3]["<<v_index + k<<"].key: "<<vbuffer7[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[3]["<<v_index + k<<"].value: "<<vbuffer7[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer7[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[4]["<<v_index + k<<"].key: "<<vbuffer7[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[4]["<<v_index + k<<"].value: "<<vbuffer7[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer7[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[5]["<<v_index + k<<"].key: "<<vbuffer7[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[5]["<<v_index + k<<"].value: "<<vbuffer7[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer7[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[6]["<<v_index + k<<"].key: "<<vbuffer7[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[6]["<<v_index + k<<"].value: "<<vbuffer7[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer7[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer7[7]["<<v_index + k<<"].key: "<<vbuffer7[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer7[7]["<<v_index + k<<"].value: "<<vbuffer7[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer8[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[0]["<<v_index + k<<"].key: "<<vbuffer8[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[0]["<<v_index + k<<"].value: "<<vbuffer8[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer8[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[1]["<<v_index + k<<"].key: "<<vbuffer8[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[1]["<<v_index + k<<"].value: "<<vbuffer8[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer8[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[2]["<<v_index + k<<"].key: "<<vbuffer8[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[2]["<<v_index + k<<"].value: "<<vbuffer8[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer8[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[3]["<<v_index + k<<"].key: "<<vbuffer8[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[3]["<<v_index + k<<"].value: "<<vbuffer8[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer8[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[4]["<<v_index + k<<"].key: "<<vbuffer8[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[4]["<<v_index + k<<"].value: "<<vbuffer8[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer8[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[5]["<<v_index + k<<"].key: "<<vbuffer8[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[5]["<<v_index + k<<"].value: "<<vbuffer8[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer8[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[6]["<<v_index + k<<"].key: "<<vbuffer8[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[6]["<<v_index + k<<"].value: "<<vbuffer8[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer8[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer8[7]["<<v_index + k<<"].key: "<<vbuffer8[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer8[7]["<<v_index + k<<"].value: "<<vbuffer8[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer9[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[0]["<<v_index + k<<"].key: "<<vbuffer9[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[0]["<<v_index + k<<"].value: "<<vbuffer9[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer9[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[1]["<<v_index + k<<"].key: "<<vbuffer9[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[1]["<<v_index + k<<"].value: "<<vbuffer9[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer9[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[2]["<<v_index + k<<"].key: "<<vbuffer9[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[2]["<<v_index + k<<"].value: "<<vbuffer9[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer9[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[3]["<<v_index + k<<"].key: "<<vbuffer9[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[3]["<<v_index + k<<"].value: "<<vbuffer9[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer9[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[4]["<<v_index + k<<"].key: "<<vbuffer9[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[4]["<<v_index + k<<"].value: "<<vbuffer9[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer9[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[5]["<<v_index + k<<"].key: "<<vbuffer9[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[5]["<<v_index + k<<"].value: "<<vbuffer9[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer9[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[6]["<<v_index + k<<"].key: "<<vbuffer9[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[6]["<<v_index + k<<"].value: "<<vbuffer9[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer9[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer9[7]["<<v_index + k<<"].key: "<<vbuffer9[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer9[7]["<<v_index + k<<"].value: "<<vbuffer9[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer10[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[0]["<<v_index + k<<"].key: "<<vbuffer10[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[0]["<<v_index + k<<"].value: "<<vbuffer10[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer10[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[1]["<<v_index + k<<"].key: "<<vbuffer10[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[1]["<<v_index + k<<"].value: "<<vbuffer10[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer10[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[2]["<<v_index + k<<"].key: "<<vbuffer10[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[2]["<<v_index + k<<"].value: "<<vbuffer10[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer10[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[3]["<<v_index + k<<"].key: "<<vbuffer10[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[3]["<<v_index + k<<"].value: "<<vbuffer10[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer10[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[4]["<<v_index + k<<"].key: "<<vbuffer10[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[4]["<<v_index + k<<"].value: "<<vbuffer10[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer10[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[5]["<<v_index + k<<"].key: "<<vbuffer10[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[5]["<<v_index + k<<"].value: "<<vbuffer10[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer10[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[6]["<<v_index + k<<"].key: "<<vbuffer10[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[6]["<<v_index + k<<"].value: "<<vbuffer10[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer10[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer10[7]["<<v_index + k<<"].key: "<<vbuffer10[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer10[7]["<<v_index + k<<"].value: "<<vbuffer10[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
						vdata0 = acts_utilobj->UTIL_GETKV2(vbuffer11[0][v_index + k]);
						mergedv_buffer[0][mv_index] = acts_utilobj->UTIL_GETKV2(vdata0);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata0 = acts_utilobj->UTIL_GETKV2(refbuffer[0][mv_index]);
						res0.key = MERGE_applyfunc(udata0.key, vdata0.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res0.value = MERGE_applyfunc(udata0.value, vdata0.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res0.key != udata0.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer11[0]["<<v_index + k<<"].key: "<<vbuffer11[0][v_index + k].key<<", refbuffer[0]["<<mv_index<<"].key: "<<refbuffer[0][mv_index].key<<endl; }
						if(res0.value != udata0.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (0*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer11[0]["<<v_index + k<<"].value: "<<vbuffer11[0][v_index + k].value<<", refbuffer[0]["<<mv_index<<"].value: "<<refbuffer[0][mv_index].value<<endl; }					
						#endif
						vdata1 = acts_utilobj->UTIL_GETKV2(vbuffer11[1][v_index + k]);
						mergedv_buffer[1][mv_index] = acts_utilobj->UTIL_GETKV2(vdata1);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata1 = acts_utilobj->UTIL_GETKV2(refbuffer[1][mv_index]);
						res1.key = MERGE_applyfunc(udata1.key, vdata1.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res1.value = MERGE_applyfunc(udata1.value, vdata1.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res1.key != udata1.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer11[1]["<<v_index + k<<"].key: "<<vbuffer11[1][v_index + k].key<<", refbuffer[1]["<<mv_index<<"].key: "<<refbuffer[1][mv_index].key<<endl; }
						if(res1.value != udata1.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (1*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer11[1]["<<v_index + k<<"].value: "<<vbuffer11[1][v_index + k].value<<", refbuffer[1]["<<mv_index<<"].value: "<<refbuffer[1][mv_index].value<<endl; }					
						#endif
						vdata2 = acts_utilobj->UTIL_GETKV2(vbuffer11[2][v_index + k]);
						mergedv_buffer[2][mv_index] = acts_utilobj->UTIL_GETKV2(vdata2);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata2 = acts_utilobj->UTIL_GETKV2(refbuffer[2][mv_index]);
						res2.key = MERGE_applyfunc(udata2.key, vdata2.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res2.value = MERGE_applyfunc(udata2.value, vdata2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res2.key != udata2.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer11[2]["<<v_index + k<<"].key: "<<vbuffer11[2][v_index + k].key<<", refbuffer[2]["<<mv_index<<"].key: "<<refbuffer[2][mv_index].key<<endl; }
						if(res2.value != udata2.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (2*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer11[2]["<<v_index + k<<"].value: "<<vbuffer11[2][v_index + k].value<<", refbuffer[2]["<<mv_index<<"].value: "<<refbuffer[2][mv_index].value<<endl; }					
						#endif
						vdata3 = acts_utilobj->UTIL_GETKV2(vbuffer11[3][v_index + k]);
						mergedv_buffer[3][mv_index] = acts_utilobj->UTIL_GETKV2(vdata3);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata3 = acts_utilobj->UTIL_GETKV2(refbuffer[3][mv_index]);
						res3.key = MERGE_applyfunc(udata3.key, vdata3.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res3.value = MERGE_applyfunc(udata3.value, vdata3.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res3.key != udata3.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer11[3]["<<v_index + k<<"].key: "<<vbuffer11[3][v_index + k].key<<", refbuffer[3]["<<mv_index<<"].key: "<<refbuffer[3][mv_index].key<<endl; }
						if(res3.value != udata3.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (3*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer11[3]["<<v_index + k<<"].value: "<<vbuffer11[3][v_index + k].value<<", refbuffer[3]["<<mv_index<<"].value: "<<refbuffer[3][mv_index].value<<endl; }					
						#endif
						vdata4 = acts_utilobj->UTIL_GETKV2(vbuffer11[4][v_index + k]);
						mergedv_buffer[4][mv_index] = acts_utilobj->UTIL_GETKV2(vdata4);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata4 = acts_utilobj->UTIL_GETKV2(refbuffer[4][mv_index]);
						res4.key = MERGE_applyfunc(udata4.key, vdata4.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res4.value = MERGE_applyfunc(udata4.value, vdata4.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res4.key != udata4.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer11[4]["<<v_index + k<<"].key: "<<vbuffer11[4][v_index + k].key<<", refbuffer[4]["<<mv_index<<"].key: "<<refbuffer[4][mv_index].key<<endl; }
						if(res4.value != udata4.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (4*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer11[4]["<<v_index + k<<"].value: "<<vbuffer11[4][v_index + k].value<<", refbuffer[4]["<<mv_index<<"].value: "<<refbuffer[4][mv_index].value<<endl; }					
						#endif
						vdata5 = acts_utilobj->UTIL_GETKV2(vbuffer11[5][v_index + k]);
						mergedv_buffer[5][mv_index] = acts_utilobj->UTIL_GETKV2(vdata5);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata5 = acts_utilobj->UTIL_GETKV2(refbuffer[5][mv_index]);
						res5.key = MERGE_applyfunc(udata5.key, vdata5.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res5.value = MERGE_applyfunc(udata5.value, vdata5.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res5.key != udata5.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer11[5]["<<v_index + k<<"].key: "<<vbuffer11[5][v_index + k].key<<", refbuffer[5]["<<mv_index<<"].key: "<<refbuffer[5][mv_index].key<<endl; }
						if(res5.value != udata5.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (5*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer11[5]["<<v_index + k<<"].value: "<<vbuffer11[5][v_index + k].value<<", refbuffer[5]["<<mv_index<<"].value: "<<refbuffer[5][mv_index].value<<endl; }					
						#endif
						vdata6 = acts_utilobj->UTIL_GETKV2(vbuffer11[6][v_index + k]);
						mergedv_buffer[6][mv_index] = acts_utilobj->UTIL_GETKV2(vdata6);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata6 = acts_utilobj->UTIL_GETKV2(refbuffer[6][mv_index]);
						res6.key = MERGE_applyfunc(udata6.key, vdata6.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res6.value = MERGE_applyfunc(udata6.value, vdata6.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res6.key != udata6.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer11[6]["<<v_index + k<<"].key: "<<vbuffer11[6][v_index + k].key<<", refbuffer[6]["<<mv_index<<"].key: "<<refbuffer[6][mv_index].key<<endl; }
						if(res6.value != udata6.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (6*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer11[6]["<<v_index + k<<"].value: "<<vbuffer11[6][v_index + k].value<<", refbuffer[6]["<<mv_index<<"].value: "<<refbuffer[6][mv_index].value<<endl; }					
						#endif
						vdata7 = acts_utilobj->UTIL_GETKV2(vbuffer11[7][v_index + k]);
						mergedv_buffer[7][mv_index] = acts_utilobj->UTIL_GETKV2(vdata7);
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						udata7 = acts_utilobj->UTIL_GETKV2(refbuffer[7][mv_index]);
						res7.key = MERGE_applyfunc(udata7.key, vdata7.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						res7.value = MERGE_applyfunc(udata7.value, vdata7.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
						if(res7.key != udata7.key){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index<<", vbuffer11[7]["<<v_index + k<<"].key: "<<vbuffer11[7][v_index + k].key<<", refbuffer[7]["<<mv_index<<"].key: "<<refbuffer[7][mv_index].key<<endl; }
						if(res7.value != udata7.value){ cout<<"acts_merge::ACTIVE VTX SEEN(1) @ (vid: "<<(vwriteoffset_kvs+vwritebufferoffset_kvs)*VECTOR_SIZE*2 + (7*globalparams.SIZE_REDUCE) + 2*mv_index + 1<<", vbuffer11[7]["<<v_index + k<<"].value: "<<vbuffer11[7][v_index + k].value<<", refbuffer[7]["<<mv_index<<"].value: "<<refbuffer[7][mv_index].value<<endl; }					
						#endif
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<">>> ACTS MERGE: i: "<<i<<", mv_index: "<<mv_index<<", v_index: "<<v_index + k<<", _MERGECHUNKSCOUNT: "<<_MERGECHUNKSCOUNT<<", _MERGECHUNKSZ: "<<_MERGECHUNKSZ<<endl;
						#endif 
					mv_index += 1;
				}
				
				v_index += _MERGECHUNKSZ;
				MERGE_writevdata(enablemerge, vdram, globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA, vwriteoffset_kvs + vwritebufferoffset_kvs, mergedv_buffer, 0, mv_index);
				vwritebufferoffset_kvs += mv_index;
				mv_index = 0;
			}
		}
		// exit(EXIT_SUCCESS); //
		
		// skipping logic (update stats)
		if(enablemerge == ON){ buffer[partition] += 64; } else { buffer[partition] += 0; }
	}
	exit(EXIT_SUCCESS);
	return;
}
