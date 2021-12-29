#include "processedges_slicedgraph.h"
using namespace std;

#ifdef SW
processedges_slicedgraph::processedges_slicedgraph(){ actsutilityobj = new actsutility(); acts_utilobj = new acts_util(); }
processedges_slicedgraph::~processedges_slicedgraph(){}
#endif

value_t 
	#ifdef SW 
	processedges_slicedgraph::
	#endif 
PROCESS_processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo){
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
	processedges_slicedgraph::
	#endif 
PROCESS_readandprocess(bool_type enable, uint512_dt * edges, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unitBRAMwidth_type vmask_subp[BLOCKRAM_SIZE], keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], 
		batch_type goffset_kvs, batch_type loffset_kvs, batch_type size_kvs, travstate_t travstate, sweepparams_t sweepparams, globalparams_t globalparams){
	fetchmessage_t fetchmessage;
	fetchmessage.chunksize_kvs = -1;
	fetchmessage.nextoffset_kvs = -1;
	if(enable == OFF){ return fetchmessage; }
	
	analysis_type analysis_loop = BLOCKRAM_SIZE / 2;
	analysis_type analysis_loop1 = SUBPMASKFACTOR;
	analysis_type analysis_loop2 = 16384 / SUBPMASKFACTOR;
	
	value_t E[2][VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=E complete
	bool_type en = ON;
	bool_type ens[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=ens complete
	
	buffer_type reducebuffersz = globalparams.SIZE_REDUCE / 2;
	unsigned int validbound = reducebuffersz * FETFACTOR * VECTOR2_SIZE;
	
	travstate.i_kvs = travstate.i_kvs / 2;
	travstate.end_kvs = travstate.end_kvs / 2;
	loffset_kvs = loffset_kvs / 2;
	buffer_type edgessize_kvs = size_kvs / 2;
	
	batch_type offset_kvs = goffset_kvs + loffset_kvs;
	
	value_t udatas[MAX_NUM_UNIQ_EDGES_PER_VEC]; // AUTOMATEME.
	#pragma HLS ARRAY_PARTITION variable=udatas complete
	value_t masks[MAX_NUM_UNIQ_EDGES_PER_VEC]; // AUTOMATEME.
	#pragma HLS ARRAY_PARTITION variable=masks complete
	vertex_t lvids[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=lvids complete
	
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
		E[0][8] = edges[offset_kvs + i].range(287, 256); 
		E[0][9] = edges[offset_kvs + i].range(319, 288); 
		E[0][10] = edges[offset_kvs + i].range(351, 320); 
		E[0][11] = edges[offset_kvs + i].range(383, 352); 
		E[0][12] = edges[offset_kvs + i].range(415, 384); 
		E[0][13] = edges[offset_kvs + i].range(447, 416); 
		E[0][14] = edges[offset_kvs + i].range(479, 448); 
		E[0][15] = edges[offset_kvs + i].range(511, 480); 
		#else 
		E[0][0] = edges[offset_kvs + i].data[0].key; 
		E[0][1] = edges[offset_kvs + i].data[0].value; 
		E[0][2] = edges[offset_kvs + i].data[1].key; 
		E[0][3] = edges[offset_kvs + i].data[1].value; 
		E[0][4] = edges[offset_kvs + i].data[2].key; 
		E[0][5] = edges[offset_kvs + i].data[2].value; 
		E[0][6] = edges[offset_kvs + i].data[3].key; 
		E[0][7] = edges[offset_kvs + i].data[3].value; 
		E[0][8] = edges[offset_kvs + i].data[4].key; 
		E[0][9] = edges[offset_kvs + i].data[4].value; 
		E[0][10] = edges[offset_kvs + i].data[5].key; 
		E[0][11] = edges[offset_kvs + i].data[5].value; 
		E[0][12] = edges[offset_kvs + i].data[6].key; 
		E[0][13] = edges[offset_kvs + i].data[6].value; 
		E[0][14] = edges[offset_kvs + i].data[7].key; 
		E[0][15] = edges[offset_kvs + i].data[7].value; 
		#endif
		
		vertex_t srcvid_head = E[0][0];
		vertex_t lvid_head = srcvid_head - travstate.i2;
		
		bool_type en = ON;
		bool_type ens0 = ON; if(E[0][0] == INVALIDDATA || E[0][0] >= validbound){ ens0 = OFF; }
		bool_type ens1 = ON; if(E[0][1] == INVALIDDATA || E[0][1] >= validbound){ ens1 = OFF; }
		bool_type ens2 = ON; if(E[0][2] == INVALIDDATA || E[0][2] >= validbound){ ens2 = OFF; }
		bool_type ens3 = ON; if(E[0][3] == INVALIDDATA || E[0][3] >= validbound){ ens3 = OFF; }
		bool_type ens4 = ON; if(E[0][4] == INVALIDDATA || E[0][4] >= validbound){ ens4 = OFF; }
		bool_type ens5 = ON; if(E[0][5] == INVALIDDATA || E[0][5] >= validbound){ ens5 = OFF; }
		bool_type ens6 = ON; if(E[0][6] == INVALIDDATA || E[0][6] >= validbound){ ens6 = OFF; }
		bool_type ens7 = ON; if(E[0][7] == INVALIDDATA || E[0][7] >= validbound){ ens7 = OFF; }
		bool_type ens8 = ON; if(E[0][8] == INVALIDDATA || E[0][8] >= validbound){ ens8 = OFF; }
		bool_type ens9 = ON; if(E[0][9] == INVALIDDATA || E[0][9] >= validbound){ ens9 = OFF; }
		bool_type ens10 = ON; if(E[0][10] == INVALIDDATA || E[0][10] >= validbound){ ens10 = OFF; }
		bool_type ens11 = ON; if(E[0][11] == INVALIDDATA || E[0][11] >= validbound){ ens11 = OFF; }
		bool_type ens12 = ON; if(E[0][12] == INVALIDDATA || E[0][12] >= validbound){ ens12 = OFF; }
		bool_type ens13 = ON; if(E[0][13] == INVALIDDATA || E[0][13] >= validbound){ ens13 = OFF; }
		bool_type ens14 = ON; if(E[0][14] == INVALIDDATA || E[0][14] >= validbound){ ens14 = OFF; }
		bool_type ens15 = ON; if(E[0][15] == INVALIDDATA || E[0][15] >= validbound){ ens15 = OFF; }
		if(lvid_head >= validbound){  ens0 = OFF;  ens1 = OFF;  ens2 = OFF;  ens3 = OFF;  ens4 = OFF;  ens5 = OFF;  ens6 = OFF;  ens7 = OFF;  ens8 = OFF;  ens9 = OFF;  ens10 = OFF;  ens11 = OFF;  ens12 = OFF;  ens13 = OFF;  ens14 = OFF;  ens15 = OFF;  lvid_head = 0; } // CRITICAL NEWCHANGE WITH SLICED.
		
		// vertex_t srcvid_head = E[0][0];
		// vertex_t lvid_head = srcvid_head - travstate.i2;
		// if(lvid_head >= validbound){ lvid_head = 0; } // CRITICAL REMOVEME.
			#ifdef _DEBUGMODE_CHECKS2
			if(srcvid_head < travstate.i2){ cout<<"readandprocess: INVALID srcvid_head. this is an error. i: "<<i<<"(of "<<chunk_size<<"), srcvid_head: "<<srcvid_head<<", travstate.i2: "<<travstate.i2<<" offset_kvs: "<<offset_kvs<<". exiting..."<<endl;					 
				for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: E[0]["<<v<<"]: "<<E[0][v]<<endl; }
				exit(EXIT_FAILURE); }
			if(lvid_head >= (reducebuffersz*FETFACTOR*VECTOR2_SIZE)){ cout<<"readandprocess: INVALID srcvid_head. this is an error. i: "<<i<<"(of "<<chunk_size<<"), lvid_head: "<<lvid_head<<", reducebuffersz*FETFACTOR*VECTOR2_SIZE: "<<reducebuffersz*FETFACTOR*VECTOR2_SIZE<<". exiting..."<<endl;					 
				for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"readandprocess: E[0]["<<v<<"]: "<<E[0][v]<<endl; }
				exit(EXIT_FAILURE); }
			actsutilityobj->checkoutofbounds("readandprocess.1", lvid_head, reducebuffersz * FETFACTOR * VECTOR2_SIZE, srcvid_head, travstate.i2, i);
			#endif
		
 // AUTOMATEME. OPTIMIZEME. FIXME.BOTTLENECK
		udatas[0] = acts_utilobj->UTIL_GETVTXDATA(vbuffer, lvid_head + 0, globalparams);
		masks[0] = acts_utilobj->UTIL_GETVTXMASK(vmask, lvid_head + 0, globalparams); 
 // AUTOMATEME. OPTIMIZEME. FIXME.BOTTLENECK
		udatas[1] = acts_utilobj->UTIL_GETVTXDATA(vbuffer, lvid_head + 1, globalparams);
		masks[1] = acts_utilobj->UTIL_GETVTXMASK(vmask, lvid_head + 1, globalparams); 
 // AUTOMATEME. OPTIMIZEME. FIXME.BOTTLENECK
		udatas[2] = acts_utilobj->UTIL_GETVTXDATA(vbuffer, lvid_head + 2, globalparams);
		masks[2] = acts_utilobj->UTIL_GETVTXMASK(vmask, lvid_head + 2, globalparams); 
 // AUTOMATEME. OPTIMIZEME. FIXME.BOTTLENECK
		udatas[3] = acts_utilobj->UTIL_GETVTXDATA(vbuffer, lvid_head + 3, globalparams);
		masks[3] = acts_utilobj->UTIL_GETVTXMASK(vmask, lvid_head + 3, globalparams); 
 // AUTOMATEME. OPTIMIZEME. FIXME.BOTTLENECK
		udatas[4] = acts_utilobj->UTIL_GETVTXDATA(vbuffer, lvid_head + 4, globalparams);
		masks[4] = acts_utilobj->UTIL_GETVTXMASK(vmask, lvid_head + 4, globalparams); 
 // AUTOMATEME. OPTIMIZEME. FIXME.BOTTLENECK
		udatas[5] = acts_utilobj->UTIL_GETVTXDATA(vbuffer, lvid_head + 5, globalparams);
		masks[5] = acts_utilobj->UTIL_GETVTXMASK(vmask, lvid_head + 5, globalparams); 
 // AUTOMATEME. OPTIMIZEME. FIXME.BOTTLENECK
		udatas[6] = acts_utilobj->UTIL_GETVTXDATA(vbuffer, lvid_head + 6, globalparams);
		masks[6] = acts_utilobj->UTIL_GETVTXMASK(vmask, lvid_head + 6, globalparams); 
 // AUTOMATEME. OPTIMIZEME. FIXME.BOTTLENECK
		udatas[7] = acts_utilobj->UTIL_GETVTXDATA(vbuffer, lvid_head + 7, globalparams);
		masks[7] = acts_utilobj->UTIL_GETVTXMASK(vmask, lvid_head + 7, globalparams); 
			#ifdef _DEBUGMODE_CHECKS
			if(masks[0] > 2){ cout<<"ERROR @ readandprocess.masks[0].1. masks[0]: "<<masks[0]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<", lvid + 0: "<<lvid + 0<<endl; } exit(EXIT_FAILURE); }
			if(masks[1] > 2){ cout<<"ERROR @ readandprocess.masks[1].1. masks[1]: "<<masks[1]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<", lvid + 1: "<<lvid + 1<<endl; } exit(EXIT_FAILURE); }
			if(masks[2] > 2){ cout<<"ERROR @ readandprocess.masks[2].1. masks[2]: "<<masks[2]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<", lvid + 2: "<<lvid + 2<<endl; } exit(EXIT_FAILURE); }
			if(masks[3] > 2){ cout<<"ERROR @ readandprocess.masks[3].1. masks[3]: "<<masks[3]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<", lvid + 3: "<<lvid + 3<<endl; } exit(EXIT_FAILURE); }
			if(masks[4] > 2){ cout<<"ERROR @ readandprocess.masks[4].1. masks[4]: "<<masks[4]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<", lvid + 4: "<<lvid + 4<<endl; } exit(EXIT_FAILURE); }
			if(masks[5] > 2){ cout<<"ERROR @ readandprocess.masks[5].1. masks[5]: "<<masks[5]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<", lvid + 5: "<<lvid + 5<<endl; } exit(EXIT_FAILURE); }
			if(masks[6] > 2){ cout<<"ERROR @ readandprocess.masks[6].1. masks[6]: "<<masks[6]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<", lvid + 6: "<<lvid + 6<<endl; } exit(EXIT_FAILURE); }
			if(masks[7] > 2){ cout<<"ERROR @ readandprocess.masks[7].1. masks[7]: "<<masks[7]<<endl; for(unsigned int n=0; n<8; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<", lvid + 7: "<<lvid + 7<<endl; } exit(EXIT_FAILURE); }
			#endif
		
		unsigned int incr0 = acts_utilobj->UTIL_READFROM_UINT(E[0][0], 28, 4);
		unsigned int lsrcvids0 = lvid_head + incr0;
		unsigned int ldstvids0 = acts_utilobj->UTIL_READFROM_UINT(E[0][0], 0, 28);
		unsigned int incr1 = acts_utilobj->UTIL_READFROM_UINT(E[0][1], 28, 4);
		unsigned int lsrcvids1 = lvid_head + incr1;
		unsigned int ldstvids1 = acts_utilobj->UTIL_READFROM_UINT(E[0][1], 0, 28);
		unsigned int incr2 = acts_utilobj->UTIL_READFROM_UINT(E[0][2], 28, 4);
		unsigned int lsrcvids2 = lvid_head + incr2;
		unsigned int ldstvids2 = acts_utilobj->UTIL_READFROM_UINT(E[0][2], 0, 28);
		unsigned int incr3 = acts_utilobj->UTIL_READFROM_UINT(E[0][3], 28, 4);
		unsigned int lsrcvids3 = lvid_head + incr3;
		unsigned int ldstvids3 = acts_utilobj->UTIL_READFROM_UINT(E[0][3], 0, 28);
		unsigned int incr4 = acts_utilobj->UTIL_READFROM_UINT(E[0][4], 28, 4);
		unsigned int lsrcvids4 = lvid_head + incr4;
		unsigned int ldstvids4 = acts_utilobj->UTIL_READFROM_UINT(E[0][4], 0, 28);
		unsigned int incr5 = acts_utilobj->UTIL_READFROM_UINT(E[0][5], 28, 4);
		unsigned int lsrcvids5 = lvid_head + incr5;
		unsigned int ldstvids5 = acts_utilobj->UTIL_READFROM_UINT(E[0][5], 0, 28);
		unsigned int incr6 = acts_utilobj->UTIL_READFROM_UINT(E[0][6], 28, 4);
		unsigned int lsrcvids6 = lvid_head + incr6;
		unsigned int ldstvids6 = acts_utilobj->UTIL_READFROM_UINT(E[0][6], 0, 28);
		unsigned int incr7 = acts_utilobj->UTIL_READFROM_UINT(E[0][7], 28, 4);
		unsigned int lsrcvids7 = lvid_head + incr7;
		unsigned int ldstvids7 = acts_utilobj->UTIL_READFROM_UINT(E[0][7], 0, 28);
		unsigned int incr8 = acts_utilobj->UTIL_READFROM_UINT(E[0][8], 28, 4);
		unsigned int lsrcvids8 = lvid_head + incr8;
		unsigned int ldstvids8 = acts_utilobj->UTIL_READFROM_UINT(E[0][8], 0, 28);
		unsigned int incr9 = acts_utilobj->UTIL_READFROM_UINT(E[0][9], 28, 4);
		unsigned int lsrcvids9 = lvid_head + incr9;
		unsigned int ldstvids9 = acts_utilobj->UTIL_READFROM_UINT(E[0][9], 0, 28);
		unsigned int incr10 = acts_utilobj->UTIL_READFROM_UINT(E[0][10], 28, 4);
		unsigned int lsrcvids10 = lvid_head + incr10;
		unsigned int ldstvids10 = acts_utilobj->UTIL_READFROM_UINT(E[0][10], 0, 28);
		unsigned int incr11 = acts_utilobj->UTIL_READFROM_UINT(E[0][11], 28, 4);
		unsigned int lsrcvids11 = lvid_head + incr11;
		unsigned int ldstvids11 = acts_utilobj->UTIL_READFROM_UINT(E[0][11], 0, 28);
		unsigned int incr12 = acts_utilobj->UTIL_READFROM_UINT(E[0][12], 28, 4);
		unsigned int lsrcvids12 = lvid_head + incr12;
		unsigned int ldstvids12 = acts_utilobj->UTIL_READFROM_UINT(E[0][12], 0, 28);
		unsigned int incr13 = acts_utilobj->UTIL_READFROM_UINT(E[0][13], 28, 4);
		unsigned int lsrcvids13 = lvid_head + incr13;
		unsigned int ldstvids13 = acts_utilobj->UTIL_READFROM_UINT(E[0][13], 0, 28);
		unsigned int incr14 = acts_utilobj->UTIL_READFROM_UINT(E[0][14], 28, 4);
		unsigned int lsrcvids14 = lvid_head + incr14;
		unsigned int ldstvids14 = acts_utilobj->UTIL_READFROM_UINT(E[0][14], 0, 28);
		unsigned int incr15 = acts_utilobj->UTIL_READFROM_UINT(E[0][15], 28, 4);
		unsigned int lsrcvids15 = lvid_head + incr15;
		unsigned int ldstvids15 = acts_utilobj->UTIL_READFROM_UINT(E[0][15], 0, 28);
			#ifdef _DEBUGMODE_CHECKS2
			if(ens0 == ON && incr0 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr0("<<incr0<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids0: "<<lsrcvids0<<", ldstvids0: "<<ldstvids0<<endl; exit(EXIT_FAILURE); }
			if(ens1 == ON && incr1 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr1("<<incr1<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids1: "<<lsrcvids1<<", ldstvids1: "<<ldstvids1<<endl; exit(EXIT_FAILURE); }
			if(ens2 == ON && incr2 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr2("<<incr2<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids2: "<<lsrcvids2<<", ldstvids2: "<<ldstvids2<<endl; exit(EXIT_FAILURE); }
			if(ens3 == ON && incr3 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr3("<<incr3<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids3: "<<lsrcvids3<<", ldstvids3: "<<ldstvids3<<endl; exit(EXIT_FAILURE); }
			if(ens4 == ON && incr4 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr4("<<incr4<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids4: "<<lsrcvids4<<", ldstvids4: "<<ldstvids4<<endl; exit(EXIT_FAILURE); }
			if(ens5 == ON && incr5 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr5("<<incr5<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids5: "<<lsrcvids5<<", ldstvids5: "<<ldstvids5<<endl; exit(EXIT_FAILURE); }
			if(ens6 == ON && incr6 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr6("<<incr6<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids6: "<<lsrcvids6<<", ldstvids6: "<<ldstvids6<<endl; exit(EXIT_FAILURE); }
			if(ens7 == ON && incr7 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr7("<<incr7<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids7: "<<lsrcvids7<<", ldstvids7: "<<ldstvids7<<endl; exit(EXIT_FAILURE); }
			if(ens8 == ON && incr8 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr8("<<incr8<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids8: "<<lsrcvids8<<", ldstvids8: "<<ldstvids8<<endl; exit(EXIT_FAILURE); }
			if(ens9 == ON && incr9 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr9("<<incr9<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids9: "<<lsrcvids9<<", ldstvids9: "<<ldstvids9<<endl; exit(EXIT_FAILURE); }
			if(ens10 == ON && incr10 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr10("<<incr10<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids10: "<<lsrcvids10<<", ldstvids10: "<<ldstvids10<<endl; exit(EXIT_FAILURE); }
			if(ens11 == ON && incr11 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr11("<<incr11<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids11: "<<lsrcvids11<<", ldstvids11: "<<ldstvids11<<endl; exit(EXIT_FAILURE); }
			if(ens12 == ON && incr12 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr12("<<incr12<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids12: "<<lsrcvids12<<", ldstvids12: "<<ldstvids12<<endl; exit(EXIT_FAILURE); }
			if(ens13 == ON && incr13 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr13("<<incr13<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids13: "<<lsrcvids13<<", ldstvids13: "<<ldstvids13<<endl; exit(EXIT_FAILURE); }
			if(ens14 == ON && incr14 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr14("<<incr14<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids14: "<<lsrcvids14<<", ldstvids14: "<<ldstvids14<<endl; exit(EXIT_FAILURE); }
			if(ens15 == ON && incr15 > MAX_NUM_UNIQ_EDGES_PER_VEC){ cout<<"readandprocess: incr15("<<incr15<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). lsrcvids15: "<<lsrcvids15<<", ldstvids15: "<<ldstvids15<<endl; exit(EXIT_FAILURE); }
			#endif
		
		unsigned int mask0; // OPTIMIZEME. bittype_t
		unsigned int mask1; // OPTIMIZEME. bittype_t
		unsigned int mask2; // OPTIMIZEME. bittype_t
		unsigned int mask3; // OPTIMIZEME. bittype_t
		unsigned int mask4; // OPTIMIZEME. bittype_t
		unsigned int mask5; // OPTIMIZEME. bittype_t
		unsigned int mask6; // OPTIMIZEME. bittype_t
		unsigned int mask7; // OPTIMIZEME. bittype_t
		unsigned int mask8; // OPTIMIZEME. bittype_t
		unsigned int mask9; // OPTIMIZEME. bittype_t
		unsigned int mask10; // OPTIMIZEME. bittype_t
		unsigned int mask11; // OPTIMIZEME. bittype_t
		unsigned int mask12; // OPTIMIZEME. bittype_t
		unsigned int mask13; // OPTIMIZEME. bittype_t
		unsigned int mask14; // OPTIMIZEME. bittype_t
		unsigned int mask15; // OPTIMIZEME. bittype_t
		if(GraphAlgo == PAGERANK){  mask0 = 1;  mask1 = 1;  mask2 = 1;  mask3 = 1;  mask4 = 1;  mask5 = 1;  mask6 = 1;  mask7 = 1;  mask8 = 1;  mask9 = 1;  mask10 = 1;  mask11 = 1;  mask12 = 1;  mask13 = 1;  mask14 = 1;  mask15 = 1;  mask0 = 0; } 
		else {  mask0 = masks[incr0];  mask1 = masks[incr1];  mask2 = masks[incr2];  mask3 = masks[incr3];  mask4 = masks[incr4];  mask5 = masks[incr5];  mask6 = masks[incr6];  mask7 = masks[incr7];  mask8 = masks[incr8];  mask9 = masks[incr9];  mask10 = masks[incr10];  mask11 = masks[incr11];  mask12 = masks[incr12];  mask13 = masks[incr13];  mask14 = masks[incr14];  mask15 = masks[incr15];  mask0 = 0; }
			#ifdef _DEBUGMODE_CHECKS2
			if(ens0 == ON && mask0 > 2){ 
				cout<<"ERROR @ readandprocess.mask0.2. mask0: "<<mask0<<", incr0: "<<incr0<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][0], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens1 == ON && mask1 > 2){ 
				cout<<"ERROR @ readandprocess.mask1.2. mask1: "<<mask1<<", incr1: "<<incr1<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][1], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens2 == ON && mask2 > 2){ 
				cout<<"ERROR @ readandprocess.mask2.2. mask2: "<<mask2<<", incr2: "<<incr2<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][2], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens3 == ON && mask3 > 2){ 
				cout<<"ERROR @ readandprocess.mask3.2. mask3: "<<mask3<<", incr3: "<<incr3<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][3], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens4 == ON && mask4 > 2){ 
				cout<<"ERROR @ readandprocess.mask4.2. mask4: "<<mask4<<", incr4: "<<incr4<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][4], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens5 == ON && mask5 > 2){ 
				cout<<"ERROR @ readandprocess.mask5.2. mask5: "<<mask5<<", incr5: "<<incr5<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][5], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens6 == ON && mask6 > 2){ 
				cout<<"ERROR @ readandprocess.mask6.2. mask6: "<<mask6<<", incr6: "<<incr6<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][6], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens7 == ON && mask7 > 2){ 
				cout<<"ERROR @ readandprocess.mask7.2. mask7: "<<mask7<<", incr7: "<<incr7<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][7], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens8 == ON && mask8 > 2){ 
				cout<<"ERROR @ readandprocess.mask8.2. mask8: "<<mask8<<", incr8: "<<incr8<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][8], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens9 == ON && mask9 > 2){ 
				cout<<"ERROR @ readandprocess.mask9.2. mask9: "<<mask9<<", incr9: "<<incr9<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][9], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens10 == ON && mask10 > 2){ 
				cout<<"ERROR @ readandprocess.mask10.2. mask10: "<<mask10<<", incr10: "<<incr10<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][10], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens11 == ON && mask11 > 2){ 
				cout<<"ERROR @ readandprocess.mask11.2. mask11: "<<mask11<<", incr11: "<<incr11<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][11], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens12 == ON && mask12 > 2){ 
				cout<<"ERROR @ readandprocess.mask12.2. mask12: "<<mask12<<", incr12: "<<incr12<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][12], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens13 == ON && mask13 > 2){ 
				cout<<"ERROR @ readandprocess.mask13.2. mask13: "<<mask13<<", incr13: "<<incr13<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][13], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens14 == ON && mask14 > 2){ 
				cout<<"ERROR @ readandprocess.mask14.2. mask14: "<<mask14<<", incr14: "<<incr14<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][14], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			if(ens15 == ON && mask15 > 2){ 
				cout<<"ERROR @ readandprocess.mask15.2. mask15: "<<mask15<<", incr15: "<<incr15<<endl; 
				for(unsigned int n=0; n<MAX_NUM_UNIQ_EDGES_PER_VEC; n++){ cout<<">>> readandprocess.mask: masks["<<n<<"]: "<<masks[n]<<endl; } 
				for(unsigned int n=0; n<16; n++){ cout<<">>> readandprocess.mask: E[0]["<<n<<"].dstvid: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][n], 0, 28)<<", E[0]["<<n<<"].incr: "<<acts_utilobj->UTIL_READFROM_UINT(E[0][15], 28, 4)<<endl; } 
				exit(EXIT_FAILURE); 
			}
			#endif
		
		if(mask0==1){cout<<"--- i: "<<i<<", mask0: "<<mask0<<endl;}
		if(mask1==1){cout<<"--- i: "<<i<<", mask1: "<<mask1<<endl;}
		if(mask2==1){cout<<"--- i: "<<i<<", mask2: "<<mask2<<endl;}
		if(mask3==1){cout<<"--- i: "<<i<<", mask3: "<<mask3<<endl;}
		if(mask4==1){cout<<"--- i: "<<i<<", mask4: "<<mask4<<endl;}
		if(mask5==1){cout<<"--- i: "<<i<<", mask5: "<<mask5<<endl;}
		if(mask6==1){cout<<"--- i: "<<i<<", mask6: "<<mask6<<endl;}
		if(mask7==1){cout<<"--- i: "<<i<<", mask7: "<<mask7<<endl;}
		if(mask8==1){cout<<"--- i: "<<i<<", mask8: "<<mask8<<endl;}
		if(mask9==1){cout<<"--- i: "<<i<<", mask9: "<<mask9<<endl;}
		if(mask10==1){cout<<"--- i: "<<i<<", mask10: "<<mask10<<endl;}
		if(mask11==1){cout<<"--- i: "<<i<<", mask11: "<<mask11<<endl;}
		if(mask12==1){cout<<"--- i: "<<i<<", mask12: "<<mask12<<endl;}
		if(mask13==1){cout<<"--- i: "<<i<<", mask13: "<<mask13<<endl;}
		if(mask14==1){cout<<"--- i: "<<i<<", mask14: "<<mask14<<endl;}
		if(mask15==1){cout<<"--- i: "<<i<<", mask15: "<<mask15<<endl;}
		
		value_t res0 = PROCESS_processfunc(udatas[incr0], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res1 = PROCESS_processfunc(udatas[incr1], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res2 = PROCESS_processfunc(udatas[incr2], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res3 = PROCESS_processfunc(udatas[incr3], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res4 = PROCESS_processfunc(udatas[incr4], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res5 = PROCESS_processfunc(udatas[incr5], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res6 = PROCESS_processfunc(udatas[incr6], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res7 = PROCESS_processfunc(udatas[incr7], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res8 = PROCESS_processfunc(udatas[incr8], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res9 = PROCESS_processfunc(udatas[incr9], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res10 = PROCESS_processfunc(udatas[incr10], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res11 = PROCESS_processfunc(udatas[incr11], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res12 = PROCESS_processfunc(udatas[incr12], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res13 = PROCESS_processfunc(udatas[incr13], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res14 = PROCESS_processfunc(udatas[incr14], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
		value_t res15 = PROCESS_processfunc(udatas[incr15], 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID); 
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			if(ens0 == ON && mask0 == 1){ cout<<"readandprocess: [lsrcvids0: "<<lsrcvids0<<", ldstvids0: "<<ldstvids0<<", udata: "<<udatas[incr0]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens1 == ON && mask1 == 1){ cout<<"readandprocess: [lsrcvids1: "<<lsrcvids1<<", ldstvids1: "<<ldstvids1<<", udata: "<<udatas[incr1]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens2 == ON && mask2 == 1){ cout<<"readandprocess: [lsrcvids2: "<<lsrcvids2<<", ldstvids2: "<<ldstvids2<<", udata: "<<udatas[incr2]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens3 == ON && mask3 == 1){ cout<<"readandprocess: [lsrcvids3: "<<lsrcvids3<<", ldstvids3: "<<ldstvids3<<", udata: "<<udatas[incr3]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens4 == ON && mask4 == 1){ cout<<"readandprocess: [lsrcvids4: "<<lsrcvids4<<", ldstvids4: "<<ldstvids4<<", udata: "<<udatas[incr4]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens5 == ON && mask5 == 1){ cout<<"readandprocess: [lsrcvids5: "<<lsrcvids5<<", ldstvids5: "<<ldstvids5<<", udata: "<<udatas[incr5]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens6 == ON && mask6 == 1){ cout<<"readandprocess: [lsrcvids6: "<<lsrcvids6<<", ldstvids6: "<<ldstvids6<<", udata: "<<udatas[incr6]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens7 == ON && mask7 == 1){ cout<<"readandprocess: [lsrcvids7: "<<lsrcvids7<<", ldstvids7: "<<ldstvids7<<", udata: "<<udatas[incr7]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens8 == ON && mask8 == 1){ cout<<"readandprocess: [lsrcvids8: "<<lsrcvids8<<", ldstvids8: "<<ldstvids8<<", udata: "<<udatas[incr8]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens9 == ON && mask9 == 1){ cout<<"readandprocess: [lsrcvids9: "<<lsrcvids9<<", ldstvids9: "<<ldstvids9<<", udata: "<<udatas[incr9]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens10 == ON && mask10 == 1){ cout<<"readandprocess: [lsrcvids10: "<<lsrcvids10<<", ldstvids10: "<<ldstvids10<<", udata: "<<udatas[incr10]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens11 == ON && mask11 == 1){ cout<<"readandprocess: [lsrcvids11: "<<lsrcvids11<<", ldstvids11: "<<ldstvids11<<", udata: "<<udatas[incr11]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens12 == ON && mask12 == 1){ cout<<"readandprocess: [lsrcvids12: "<<lsrcvids12<<", ldstvids12: "<<ldstvids12<<", udata: "<<udatas[incr12]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens13 == ON && mask13 == 1){ cout<<"readandprocess: [lsrcvids13: "<<lsrcvids13<<", ldstvids13: "<<ldstvids13<<", udata: "<<udatas[incr13]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens14 == ON && mask14 == 1){ cout<<"readandprocess: [lsrcvids14: "<<lsrcvids14<<", ldstvids14: "<<ldstvids14<<", udata: "<<udatas[incr14]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			if(ens15 == ON && mask15 == 1){ cout<<"readandprocess: [lsrcvids15: "<<lsrcvids15<<", ldstvids15: "<<ldstvids15<<", udata: "<<udatas[incr15]<<"]. sweepparams.source_partition: "<<sweepparams.source_partition<<endl; }
			#endif
		
		keyvalue_t mykeyvalue0; if(ens0 == ON && mask0 == 1){ mykeyvalue0.key = ldstvids0; mykeyvalue0.value = res0; } else { mykeyvalue0.key = INVALIDDATA; mykeyvalue0.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue1; if(ens1 == ON && mask1 == 1){ mykeyvalue1.key = ldstvids1; mykeyvalue1.value = res1; } else { mykeyvalue1.key = INVALIDDATA; mykeyvalue1.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue2; if(ens2 == ON && mask2 == 1){ mykeyvalue2.key = ldstvids2; mykeyvalue2.value = res2; } else { mykeyvalue2.key = INVALIDDATA; mykeyvalue2.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue3; if(ens3 == ON && mask3 == 1){ mykeyvalue3.key = ldstvids3; mykeyvalue3.value = res3; } else { mykeyvalue3.key = INVALIDDATA; mykeyvalue3.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue4; if(ens4 == ON && mask4 == 1){ mykeyvalue4.key = ldstvids4; mykeyvalue4.value = res4; } else { mykeyvalue4.key = INVALIDDATA; mykeyvalue4.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue5; if(ens5 == ON && mask5 == 1){ mykeyvalue5.key = ldstvids5; mykeyvalue5.value = res5; } else { mykeyvalue5.key = INVALIDDATA; mykeyvalue5.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue6; if(ens6 == ON && mask6 == 1){ mykeyvalue6.key = ldstvids6; mykeyvalue6.value = res6; } else { mykeyvalue6.key = INVALIDDATA; mykeyvalue6.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue7; if(ens7 == ON && mask7 == 1){ mykeyvalue7.key = ldstvids7; mykeyvalue7.value = res7; } else { mykeyvalue7.key = INVALIDDATA; mykeyvalue7.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue8; if(ens8 == ON && mask8 == 1){ mykeyvalue8.key = ldstvids8; mykeyvalue8.value = res8; } else { mykeyvalue8.key = INVALIDDATA; mykeyvalue8.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue9; if(ens9 == ON && mask9 == 1){ mykeyvalue9.key = ldstvids9; mykeyvalue9.value = res9; } else { mykeyvalue9.key = INVALIDDATA; mykeyvalue9.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue10; if(ens10 == ON && mask10 == 1){ mykeyvalue10.key = ldstvids10; mykeyvalue10.value = res10; } else { mykeyvalue10.key = INVALIDDATA; mykeyvalue10.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue11; if(ens11 == ON && mask11 == 1){ mykeyvalue11.key = ldstvids11; mykeyvalue11.value = res11; } else { mykeyvalue11.key = INVALIDDATA; mykeyvalue11.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue12; if(ens12 == ON && mask12 == 1){ mykeyvalue12.key = ldstvids12; mykeyvalue12.value = res12; } else { mykeyvalue12.key = INVALIDDATA; mykeyvalue12.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue13; if(ens13 == ON && mask13 == 1){ mykeyvalue13.key = ldstvids13; mykeyvalue13.value = res13; } else { mykeyvalue13.key = INVALIDDATA; mykeyvalue13.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue14; if(ens14 == ON && mask14 == 1){ mykeyvalue14.key = ldstvids14; mykeyvalue14.value = res14; } else { mykeyvalue14.key = INVALIDDATA; mykeyvalue14.value = INVALIDDATA;  }
		keyvalue_t mykeyvalue15; if(ens15 == ON && mask15 == 1){ mykeyvalue15.key = ldstvids15; mykeyvalue15.value = res15; } else { mykeyvalue15.key = INVALIDDATA; mykeyvalue15.value = INVALIDDATA;  }
		mykeyvalue0.key = INVALIDDATA;
		mykeyvalue0.value = INVALIDDATA;
		
		buffer[0][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue0);
		buffer[1][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue1);
		buffer[2][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue2);
		buffer[3][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue3);
		buffer[4][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue4);
		buffer[5][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue5);
		buffer[6][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue6);
		buffer[7][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue7);
		buffer[8][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue8);
		buffer[9][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue9);
		buffer[10][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue10);
		buffer[11][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue11);
		buffer[12][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue12);
		buffer[13][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue13);
		buffer[14][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue14);
		buffer[15][loadcount] = acts_utilobj->UTIL_GETKV(mykeyvalue15);
		if(
(ens0 == ON && mask0 == 1)
||(ens1 == ON && mask1 == 1)
||(ens2 == ON && mask2 == 1)
||(ens3 == ON && mask3 == 1)
||(ens4 == ON && mask4 == 1)
||(ens5 == ON && mask5 == 1)
||(ens6 == ON && mask6 == 1)
||(ens7 == ON && mask7 == 1)
||(ens8 == ON && mask8 == 1)
||(ens9 == ON && mask9 == 1)
||(ens10 == ON && mask10 == 1)
||(ens11 == ON && mask11 == 1)
||(ens12 == ON && mask12 == 1)
||(ens13 == ON && mask13 == 1)
||(ens14 == ON && mask14 == 1)
||(ens15 == ON && mask15 == 1)
 )
		{ loadcount += 2; } // mask0? FIXME. CRITICAL OPTIMIZEME
		
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens0 == ON && mask0 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens1 == ON && mask1 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens2 == ON && mask2 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens3 == ON && mask3 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens4 == ON && mask4 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens5 == ON && mask5 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens6 == ON && mask6 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens7 == ON && mask7 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens8 == ON && mask8 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens9 == ON && mask9 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens10 == ON && mask10 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens11 == ON && mask11 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens12 == ON && mask12 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens13 == ON && mask13 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens14 == ON && mask14 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			actsutilityobj->globalstats_countkvsprocessed(1);
			if(ens15 == ON && mask15 == 1){ actsutilityobj->globalstats_processedges_countvalidkvsprocessed(1); } // mask0? FIXME.
			#endif 
	}
	fetchmessage.chunksize_kvs = loadcount;
	return fetchmessage;
}



