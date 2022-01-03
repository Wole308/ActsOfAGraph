#include "processedges_unslicedgraph.h"
using namespace std;

#ifdef SW
processedges_unslicedgraph::processedges_unslicedgraph(){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util();
	mem_accessobj = new mem_access();
}
processedges_unslicedgraph::~processedges_unslicedgraph(){}
#endif

value_t 
	#ifdef SW 
	processedges_unslicedgraph::
	#endif 
PROCESSU_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
	value_t res = 0;
	if(GraphAlgo == PAGERANK){
		res = udata;
	} else if(GraphAlgo == BFS){
		res = NAp;
	} else if(GraphAlgo == SSSP){
		res = udata + edgew;
	} else {
		res = NAp;
	}
	return res;
}

fetchmessage_t 
	#ifdef SW 
	processedges_unslicedgraph::
	#endif 
PROCESSU_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loop = BLOCKRAM_SIZE / 2;
	analysis_type analysis_loop1 = SUBPMASKFACTOR;
	analysis_type analysis_loop2 = 16384 / SUBPMASKFACTOR;
	
	value_t E[2][VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	bool_type en = ON;
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	travstate.i_kvs = travstate.i_kvs / 2;
	travstate.end_kvs = travstate.end_kvs / 2;
	loffset_kvs = loffset_kvs / 2;
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	vertex_t srcvid;
	vertex_t lvid;
	vertex_t lvid_sob = INVALIDDATA;
	vertex_t lvid_eob = INVALIDDATA; 
	vertex_t firstvalidlvid = INVALIDDATA;
	vertex_t lastvalidlvid = INVALIDDATA; 
	
	batch_type vptrbaseoffset_kvs = globalparams.BASEOFFSETKVS_VERTEXPTR + (globalparams.ACTSPARAMS_SRCVOFFSET / VECTOR_SIZE);
	batch_type voffset_kvs = sweepparams.source_partition * reducebuffersz * FETFACTOR;
	keyy_t nextbeginvptr;
	keyy_t nextbeginvptr_kvs;
	
	fetchmessage.chunksize_kvs = edgessize_kvs;
	fetchmessage.nextoffset_kvs = -1;
	buffer_type loadcount = 0;
	
	unsigned int found = 0;
	unsigned int found1 = 0;
	int nextactivei = -1;
	int subpidx = -1;
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	
	buffer_type chunk_size = acts_utilobj->UTIL_getchunksize_kvs(edgessize_kvs, travstate, 0);
	READANDPROCESS_LOOP1: for (buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop avg=analysis_loop	
	#pragma HLS PIPELINE II=1

		#ifdef _WIDEWORD
		E[0][0] = edges[offset_kvs + i].range(31, 0); 
		E[0][1] = edges[offset_kvs + i].range(63, 32); 
		E[0][2] = edges[offset_kvs + i].range(95, 64); 
		E[0][3] = edges[offset_kvs + i].range(127, 96); 
		E[0][4] = edges[offset_kvs + i].range(159, 128); 
		E[0][5] = edges[offset_kvs + i].range(191, 160); 
		E[0][6] = edges[offset_kvs + i].range(223, 192); 
		E[0][7] = edges[offset_kvs + i].range(255, 224); 
		E[1][0] = edges[offset_kvs + i].range(287, 256); 
		E[1][1] = edges[offset_kvs + i].range(319, 288); 
		E[1][2] = edges[offset_kvs + i].range(351, 320); 
		E[1][3] = edges[offset_kvs + i].range(383, 352); 
		E[1][4] = edges[offset_kvs + i].range(415, 384); 
		E[1][5] = edges[offset_kvs + i].range(447, 416); 
		E[1][6] = edges[offset_kvs + i].range(479, 448); 
		E[1][7] = edges[offset_kvs + i].range(511, 480); 
		#else 
		E[0][0] = edges[offset_kvs + i].data[0].key; 
		E[0][1] = edges[offset_kvs + i].data[0].value; 
		E[0][2] = edges[offset_kvs + i].data[1].key; 
		E[0][3] = edges[offset_kvs + i].data[1].value; 
		E[0][4] = edges[offset_kvs + i].data[2].key; 
		E[0][5] = edges[offset_kvs + i].data[2].value; 
		E[0][6] = edges[offset_kvs + i].data[3].key; 
		E[0][7] = edges[offset_kvs + i].data[3].value; 
		E[1][0] = edges[offset_kvs + i].data[4+0].key; 
		E[1][1] = edges[offset_kvs + i].data[4+0].value; 
		E[1][2] = edges[offset_kvs + i].data[4+1].key; 
		E[1][3] = edges[offset_kvs + i].data[4+1].value; 
		E[1][4] = edges[offset_kvs + i].data[4+2].key; 
		E[1][5] = edges[offset_kvs + i].data[4+2].value; 
		E[1][6] = edges[offset_kvs + i].data[4+3].key; 
		E[1][7] = edges[offset_kvs + i].data[4+3].value; 
		#endif
		
		en = ON;
		
		srcvid = E[0][0];
		lvid = srcvid - travstate.i2;
		
		if(i == 0){ lvid_sob = lvid; } 
		if(i == chunk_size-1){ lvid_eob = lvid; } 
		
		if(lvid >= validbound || srcvid == UNUSEDDATA){ en = OFF; lvid = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		if(srcvid < travstate.i2){ cout<<"readandprocess: INVALID srcvid. this is an error. i: "<<i<<", srcvid: "<<srcvid<<", travstate.i2: "<<travstate.i2<<" offset_kvs: "<<offset_kvs<<". exiting..."<<endl; 
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: E[0]["<<v<<"]: "<<E[0][v]<<", E[1]["<<v<<"]: "<<E[1][v]<<endl; }
			exit(EXIT_FAILURE); }
		actsutilityobj->checkoutofbounds("readandprocess.1", lvid, reducebuffersz * FETFACTOR * VECTOR2_SIZE, srcvid, travstate.i2, NAp);
		#endif
		
		value_t udata = acts_utilobj->UTIL_GETVTXDATA(vbuffer, lvid, globalparams);
		unsigned int mask;
		if(GraphAlgo == PAGERANK){ mask = 1; } 
		else { mask = acts_utilobj->UTIL_GETVTXMASK(vmask, lvid, globalparams); }
		value_t res = PROCESSU_processfunc(udata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		
		if(en == ON && mask == 1 && firstvalidlvid == INVALIDDATA){ firstvalidlvid = lvid; } // NEWCHANGE.
		if(en == ON && mask == 1){ lastvalidlvid = lvid; }
		
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("readandprocess.1", mask, 2, NAp, NAp, NAp);
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		if(en == ON && mask == 1){
			if(true){ cout<<"readandprocess: i: "<<i<<", mask: "<<mask<<", srcvid: "<<srcvid<<", travstate.i2: "<<travstate.i2<<", lvid: "<<lvid<<", udata: "<<udata<<endl; }
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: udata: "<<udata<<", E[0]["<<v<<"]: "<<E[0][v]<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: udata: "<<udata<<", E[1]["<<v<<"]: "<<E[1][v]<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
		}
		#endif
		
		keyvalue_t mykeyvalue00;
		keyvalue_t mykeyvalue01;
		keyvalue_t mykeyvalue02;
		keyvalue_t mykeyvalue03;
		keyvalue_t mykeyvalue04;
		keyvalue_t mykeyvalue05;
		keyvalue_t mykeyvalue06;
		keyvalue_t mykeyvalue07;
		keyvalue_t mykeyvalue10;
		keyvalue_t mykeyvalue11;
		keyvalue_t mykeyvalue12;
		keyvalue_t mykeyvalue13;
		keyvalue_t mykeyvalue14;
		keyvalue_t mykeyvalue15;
		keyvalue_t mykeyvalue16;
		keyvalue_t mykeyvalue17;
		
		if(en == ON && mask == 1){
			mykeyvalue00.key = E[0][0]; 
			mykeyvalue00.value = res; 
			mykeyvalue01.key = E[0][1]; 
			mykeyvalue01.value = res; 
			mykeyvalue02.key = E[0][2]; 
			mykeyvalue02.value = res; 
			mykeyvalue03.key = E[0][3]; 
			mykeyvalue03.value = res; 
			mykeyvalue04.key = E[0][4]; 
			mykeyvalue04.value = res; 
			mykeyvalue05.key = E[0][5]; 
			mykeyvalue05.value = res; 
			mykeyvalue06.key = E[0][6]; 
			mykeyvalue06.value = res; 
			mykeyvalue07.key = E[0][7]; 
			mykeyvalue07.value = res; 
			mykeyvalue10.key = E[1][0]; 
			mykeyvalue10.value = res; 
			mykeyvalue11.key = E[1][1]; 
			mykeyvalue11.value = res; 
			mykeyvalue12.key = E[1][2]; 
			mykeyvalue12.value = res; 
			mykeyvalue13.key = E[1][3]; 
			mykeyvalue13.value = res; 
			mykeyvalue14.key = E[1][4]; 
			mykeyvalue14.value = res; 
			mykeyvalue15.key = E[1][5]; 
			mykeyvalue15.value = res; 
			mykeyvalue16.key = E[1][6]; 
			mykeyvalue16.value = res; 
			mykeyvalue17.key = E[1][7]; 
			mykeyvalue17.value = res; 
		} else {
			mykeyvalue00.key = INVALIDDATA; 
			mykeyvalue00.value = INVALIDDATA; 
			mykeyvalue01.key = INVALIDDATA; 
			mykeyvalue01.value = INVALIDDATA; 
			mykeyvalue02.key = INVALIDDATA; 
			mykeyvalue02.value = INVALIDDATA; 
			mykeyvalue03.key = INVALIDDATA; 
			mykeyvalue03.value = INVALIDDATA; 
			mykeyvalue04.key = INVALIDDATA; 
			mykeyvalue04.value = INVALIDDATA; 
			mykeyvalue05.key = INVALIDDATA; 
			mykeyvalue05.value = INVALIDDATA; 
			mykeyvalue06.key = INVALIDDATA; 
			mykeyvalue06.value = INVALIDDATA; 
			mykeyvalue07.key = INVALIDDATA; 
			mykeyvalue07.value = INVALIDDATA; 
			mykeyvalue10.key = INVALIDDATA; 
			mykeyvalue10.value = INVALIDDATA; 
			mykeyvalue11.key = INVALIDDATA; 
			mykeyvalue11.value = INVALIDDATA; 
			mykeyvalue12.key = INVALIDDATA; 
			mykeyvalue12.value = INVALIDDATA; 
			mykeyvalue13.key = INVALIDDATA; 
			mykeyvalue13.value = INVALIDDATA; 
			mykeyvalue14.key = INVALIDDATA; 
			mykeyvalue14.value = INVALIDDATA; 
			mykeyvalue15.key = INVALIDDATA; 
			mykeyvalue15.value = INVALIDDATA; 
			mykeyvalue16.key = INVALIDDATA; 
			mykeyvalue16.value = INVALIDDATA; 
			mykeyvalue17.key = INVALIDDATA; 
			mykeyvalue17.value = INVALIDDATA; 
		}
		
		mykeyvalue00.key = INVALIDDATA;
		mykeyvalue00.value = INVALIDDATA;
		
		buffer[0][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue00);
		buffer[0][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue10);
		buffer[1][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue01);
		buffer[1][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue11);
		buffer[2][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue02);
		buffer[2][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue12);
		buffer[3][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue03);
		buffer[3][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue13);
		buffer[4][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue04);
		buffer[4][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue14);
		buffer[5][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue05);
		buffer[5][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue15);
		buffer[6][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue06);
		buffer[6][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue16);
		buffer[7][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue07);
		buffer[7][loadcount + 1] = acts_utilobj->UTIL_GETKV(mykeyvalue17);
		if(en == ON && mask == 1){ loadcount += 2; }
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsprocessed(VECTOR_SIZE*2);
		if(en == ON && mask == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(VECTOR_SIZE*2); }
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"readandprocess:: lvid_sob: "<<lvid_sob<<", lvid_eob: "<<lvid_eob<<""<<endl;
	#endif
	
	fetchmessage.chunksize_kvs = loadcount;
	return fetchmessage;
}



