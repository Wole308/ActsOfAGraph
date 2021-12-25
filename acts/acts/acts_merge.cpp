#include "acts_merge.h"
using namespace std;

#define NUMWS 16//NUM_PARTITIONS // CRITICAL FIXME.

#ifdef SW
actssync::actssync(){ 
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
actssync::~actssync(){}
#endif

value_t 
	#ifdef SW 
	actssync::
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
	actssync::
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
	actssync::
	#endif 
MERGE_writevdata(bool_type enable1, uint512_dt * kvdram, batch_type dramoffset_kvs, keyvalue_vbuffer_t buffer[VECTOR_SIZE][BLOCKRAM_SIZE], batch_type bufferoffset_kvs, buffer_type size_kvs){
	#pragma HLS function_instantiate variable=kvdram
	if(enable1 == OFF){ return; }
	analysis_type analysis_loopcount = BLOCKRAM_SIZE;

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
			kvdram[dramoffset_kvs + i].data[0].key = keyvalue0.key; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[0].value = keyvalue0.value; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[1].key = keyvalue1.key; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[1].value = keyvalue1.value; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[2].key = keyvalue2.key; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[2].value = keyvalue2.value; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[3].key = keyvalue3.key; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[3].value = keyvalue3.value; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[4].key = keyvalue4.key; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[4].value = keyvalue4.value; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[5].key = keyvalue5.key; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[5].value = keyvalue5.value; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[6].key = keyvalue6.key; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[6].value = keyvalue6.value; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[7].key = keyvalue7.key; // dramoffset_kvs
			kvdram[dramoffset_kvs + i].data[7].value = keyvalue7.value; // dramoffset_kvs
			#endif
	}
	return;
}

extern "C" {
void
	#if defined(SW)
	actssync::
	#endif
topkernelsync(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,uint512_dt * kvdram16,uint512_dt * kvdram17,uint512_dt * kvdram18,uint512_dt * kvdram19,uint512_dt * kvdram20,uint512_dt * kvdram21,uint512_dt * kvdram22,uint512_dt * kvdram23,uint512_dt * kvdram24,uint512_dt * kvdram25,uint512_dt * kvdram26,uint512_dt * kvdram27,uint512_dt * kvdram28,uint512_dt * kvdram29,uint512_dt * kvdram30,uint512_dt * kvdram31, uint512_dt * vdram){
#ifndef ACTS_1by1
 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
 
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
 
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
 
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
 
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
 
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
 
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
 
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
 
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
 
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
 
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
 
#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem11
 
#pragma HLS INTERFACE m_axi port = kvdram12 offset = slave bundle = gmem12
 
#pragma HLS INTERFACE m_axi port = kvdram13 offset = slave bundle = gmem13
 
#pragma HLS INTERFACE m_axi port = kvdram14 offset = slave bundle = gmem14
 
#pragma HLS INTERFACE m_axi port = kvdram15 offset = slave bundle = gmem15
 
#pragma HLS INTERFACE m_axi port = kvdram16 offset = slave bundle = gmem16
 
#pragma HLS INTERFACE m_axi port = kvdram17 offset = slave bundle = gmem17
 
#pragma HLS INTERFACE m_axi port = kvdram18 offset = slave bundle = gmem18
 
#pragma HLS INTERFACE m_axi port = kvdram19 offset = slave bundle = gmem19
 
#pragma HLS INTERFACE m_axi port = kvdram20 offset = slave bundle = gmem20
 
#pragma HLS INTERFACE m_axi port = kvdram21 offset = slave bundle = gmem21
 
#pragma HLS INTERFACE m_axi port = kvdram22 offset = slave bundle = gmem22
 
#pragma HLS INTERFACE m_axi port = kvdram23 offset = slave bundle = gmem23
 
#pragma HLS INTERFACE m_axi port = kvdram24 offset = slave bundle = gmem24
 
#pragma HLS INTERFACE m_axi port = kvdram25 offset = slave bundle = gmem25
 
#pragma HLS INTERFACE m_axi port = kvdram26 offset = slave bundle = gmem26
 
#pragma HLS INTERFACE m_axi port = kvdram27 offset = slave bundle = gmem27
 
#pragma HLS INTERFACE m_axi port = kvdram28 offset = slave bundle = gmem28
 
#pragma HLS INTERFACE m_axi port = kvdram29 offset = slave bundle = gmem29
 
#pragma HLS INTERFACE m_axi port = kvdram30 offset = slave bundle = gmem30
 
#pragma HLS INTERFACE m_axi port = kvdram31 offset = slave bundle = gmem31
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem32

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram11 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram12 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram13 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram14 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram15 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram16 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram17 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram18 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram19 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram21 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram22 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram23 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram24 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram25 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram26 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram27 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram28 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram29 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram31 bundle = control
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = kvdram10
#pragma HLS DATA_PACK variable = kvdram11
#pragma HLS DATA_PACK variable = kvdram12
#pragma HLS DATA_PACK variable = kvdram13
#pragma HLS DATA_PACK variable = kvdram14
#pragma HLS DATA_PACK variable = kvdram15
#pragma HLS DATA_PACK variable = kvdram16
#pragma HLS DATA_PACK variable = kvdram17
#pragma HLS DATA_PACK variable = kvdram18
#pragma HLS DATA_PACK variable = kvdram19
#pragma HLS DATA_PACK variable = kvdram20
#pragma HLS DATA_PACK variable = kvdram21
#pragma HLS DATA_PACK variable = kvdram22
#pragma HLS DATA_PACK variable = kvdram23
#pragma HLS DATA_PACK variable = kvdram24
#pragma HLS DATA_PACK variable = kvdram25
#pragma HLS DATA_PACK variable = kvdram26
#pragma HLS DATA_PACK variable = kvdram27
#pragma HLS DATA_PACK variable = kvdram28
#pragma HLS DATA_PACK variable = kvdram29
#pragma HLS DATA_PACK variable = kvdram30
#pragma HLS DATA_PACK variable = kvdram31
#pragma HLS DATA_PACK variable = vdram
#endif 
	
	#if defined(_DEBUGMODE_KERNELPRINTS2) //& defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== ACTS MERGE Launched... size: "<<SYNC_GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN], 0)<<endl; 
	#endif
	
	globalparams_t globalparams = acts_utilobj->UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = acts_utilobj->UTIL_getglobalparams(vdram);
	batch_type num_source_partitions = acts_utilobj->UTIL_get_num_source_partitions(globalparams.ACTSPARAMS_TREEDEPTH); //??????????
	
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
	keyvalue_vbuffer_t vbuffer12[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer12
	keyvalue_vbuffer_t vbuffer13[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer13
	keyvalue_vbuffer_t vbuffer14[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer14
	keyvalue_vbuffer_t vbuffer15[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer15
	keyvalue_vbuffer_t vbuffer16[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer16
	keyvalue_vbuffer_t vbuffer17[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer17
	keyvalue_vbuffer_t vbuffer18[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer18
	keyvalue_vbuffer_t vbuffer19[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer19
	keyvalue_vbuffer_t vbuffer20[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer20
	keyvalue_vbuffer_t vbuffer21[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer21
	keyvalue_vbuffer_t vbuffer22[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer22
	keyvalue_vbuffer_t vbuffer23[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer23
	keyvalue_vbuffer_t vbuffer24[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer24
	keyvalue_vbuffer_t vbuffer25[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer25
	keyvalue_vbuffer_t vbuffer26[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer26
	keyvalue_vbuffer_t vbuffer27[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer27
	keyvalue_vbuffer_t vbuffer28[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer28
	keyvalue_vbuffer_t vbuffer29[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer29
	keyvalue_vbuffer_t vbuffer30[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer30
	keyvalue_vbuffer_t vbuffer31[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = vbuffer31
	keyvalue_vbuffer_t refbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = refbuffer
	keyvalue_vbuffer_t merge_buffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = merge_buffer
	
	unsigned int vreadoffsetpp0_kvs = 0;
	unsigned int vwriteoffsetpp0_kvs = 0;
	
	unsigned int total_num_blocks = (globalparams.SIZE_BATCHRANGE / NUMWS) / BLOCKRAM_SIZE;
	cout<<">>> ACTS MERGE: globalparams.SIZE_BATCHRANGE: "<<globalparams.SIZE_BATCHRANGE<<endl; 
	cout<<">>> ACTS MERGE: NUMWS: "<<NUMWS<<endl; 
	cout<<">>> ACTS MERGE: BLOCKRAM_SIZE: "<<BLOCKRAM_SIZE<<endl; 
	cout<<">>> ACTS MERGE: total_num_blocks: "<<total_num_blocks<<endl; 
	
	TOPKERNELSYNC_MAINLOOP: for(batch_type iterationidx=0; iterationidx<total_num_blocks; iterationidx+=1){
		#if defined(_DEBUGMODE_KERNELPRINTS3)
		cout<<">>> ACTS MERGE: merging block: "<<iterationidx<<" (of "<<total_num_blocks<<" total blocks)"<<endl; 
		#endif
	
		MERGE_readvdata(ON, kvdram0, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer0, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram1, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer1, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram2, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer2, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram3, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer3, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram4, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer4, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram5, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer5, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram6, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer6, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram7, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer7, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram8, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer8, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram9, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer9, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram10, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer10, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram11, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer11, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram12, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer12, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram13, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer13, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram14, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer14, 0, reducebuffersz);
		MERGE_readvdata(ON, kvdram15, globalparams.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, vbuffer15, 0, reducebuffersz);
		MERGE_readvdata(ON, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vreadoffsetpp0_kvs, refbuffer, 0, reducebuffersz);
		
		unsigned int merge_buffer_index = 0;
		unsigned int v_buffer_index = 0;
		for(unsigned int k=0; k<reducebuffersz / NUMWS; k++){
			
 
				keyvalue_t udata00 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata00 = acts_utilobj->UTIL_GETKV2(vbuffer0[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer0[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata00);
				
				keyvalue_t res00; 
				res00.key = MERGE_applyfunc(udata00.key, vdata00.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res00.value = MERGE_applyfunc(udata00.value, vdata00.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer0[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[0]["<<v_buffer_index<<"].key: "<<vbuffer0[0][v_buffer_index].key<<endl; }
				if(vbuffer0[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[0]["<<v_buffer_index<<"].value: "<<vbuffer0[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[0]["<<v_buffer_index<<"].key: "<<vbuffer0[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[0]["<<v_buffer_index<<"].value: "<<vbuffer0[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res00.key != udata00.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer0[0]["<<v_buffer_index<<"].key: "<<vbuffer0[0][v_buffer_index].key<<endl; }
				if(res00.value != udata00.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer0[0]["<<v_buffer_index<<"].value: "<<vbuffer0[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata01 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata01 = acts_utilobj->UTIL_GETKV2(vbuffer0[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer0[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata01);
				
				keyvalue_t res01; 
				res01.key = MERGE_applyfunc(udata01.key, vdata01.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res01.value = MERGE_applyfunc(udata01.value, vdata01.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer0[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[1]["<<v_buffer_index<<"].key: "<<vbuffer0[1][v_buffer_index].key<<endl; }
				if(vbuffer0[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[1]["<<v_buffer_index<<"].value: "<<vbuffer0[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[1]["<<v_buffer_index<<"].key: "<<vbuffer0[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[1]["<<v_buffer_index<<"].value: "<<vbuffer0[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res01.key != udata01.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer0[1]["<<v_buffer_index<<"].key: "<<vbuffer0[1][v_buffer_index].key<<endl; }
				if(res01.value != udata01.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer0[1]["<<v_buffer_index<<"].value: "<<vbuffer0[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata02 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata02 = acts_utilobj->UTIL_GETKV2(vbuffer0[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer0[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata02);
				
				keyvalue_t res02; 
				res02.key = MERGE_applyfunc(udata02.key, vdata02.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res02.value = MERGE_applyfunc(udata02.value, vdata02.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer0[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[2]["<<v_buffer_index<<"].key: "<<vbuffer0[2][v_buffer_index].key<<endl; }
				if(vbuffer0[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[2]["<<v_buffer_index<<"].value: "<<vbuffer0[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[2]["<<v_buffer_index<<"].key: "<<vbuffer0[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[2]["<<v_buffer_index<<"].value: "<<vbuffer0[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res02.key != udata02.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer0[2]["<<v_buffer_index<<"].key: "<<vbuffer0[2][v_buffer_index].key<<endl; }
				if(res02.value != udata02.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer0[2]["<<v_buffer_index<<"].value: "<<vbuffer0[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata03 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata03 = acts_utilobj->UTIL_GETKV2(vbuffer0[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer0[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata03);
				
				keyvalue_t res03; 
				res03.key = MERGE_applyfunc(udata03.key, vdata03.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res03.value = MERGE_applyfunc(udata03.value, vdata03.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer0[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[3]["<<v_buffer_index<<"].key: "<<vbuffer0[3][v_buffer_index].key<<endl; }
				if(vbuffer0[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[3]["<<v_buffer_index<<"].value: "<<vbuffer0[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[3]["<<v_buffer_index<<"].key: "<<vbuffer0[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[3]["<<v_buffer_index<<"].value: "<<vbuffer0[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res03.key != udata03.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer0[3]["<<v_buffer_index<<"].key: "<<vbuffer0[3][v_buffer_index].key<<endl; }
				if(res03.value != udata03.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer0[3]["<<v_buffer_index<<"].value: "<<vbuffer0[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata04 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata04 = acts_utilobj->UTIL_GETKV2(vbuffer0[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer0[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata04);
				
				keyvalue_t res04; 
				res04.key = MERGE_applyfunc(udata04.key, vdata04.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res04.value = MERGE_applyfunc(udata04.value, vdata04.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer0[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[4]["<<v_buffer_index<<"].key: "<<vbuffer0[4][v_buffer_index].key<<endl; }
				if(vbuffer0[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[4]["<<v_buffer_index<<"].value: "<<vbuffer0[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[4]["<<v_buffer_index<<"].key: "<<vbuffer0[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[4]["<<v_buffer_index<<"].value: "<<vbuffer0[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res04.key != udata04.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer0[4]["<<v_buffer_index<<"].key: "<<vbuffer0[4][v_buffer_index].key<<endl; }
				if(res04.value != udata04.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer0[4]["<<v_buffer_index<<"].value: "<<vbuffer0[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata05 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata05 = acts_utilobj->UTIL_GETKV2(vbuffer0[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer0[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata05);
				
				keyvalue_t res05; 
				res05.key = MERGE_applyfunc(udata05.key, vdata05.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res05.value = MERGE_applyfunc(udata05.value, vdata05.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer0[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[5]["<<v_buffer_index<<"].key: "<<vbuffer0[5][v_buffer_index].key<<endl; }
				if(vbuffer0[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[5]["<<v_buffer_index<<"].value: "<<vbuffer0[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[5]["<<v_buffer_index<<"].key: "<<vbuffer0[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[5]["<<v_buffer_index<<"].value: "<<vbuffer0[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res05.key != udata05.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer0[5]["<<v_buffer_index<<"].key: "<<vbuffer0[5][v_buffer_index].key<<endl; }
				if(res05.value != udata05.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer0[5]["<<v_buffer_index<<"].value: "<<vbuffer0[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata06 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata06 = acts_utilobj->UTIL_GETKV2(vbuffer0[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer0[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata06);
				
				keyvalue_t res06; 
				res06.key = MERGE_applyfunc(udata06.key, vdata06.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res06.value = MERGE_applyfunc(udata06.value, vdata06.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer0[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[6]["<<v_buffer_index<<"].key: "<<vbuffer0[6][v_buffer_index].key<<endl; }
				if(vbuffer0[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[6]["<<v_buffer_index<<"].value: "<<vbuffer0[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[6]["<<v_buffer_index<<"].key: "<<vbuffer0[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[6]["<<v_buffer_index<<"].value: "<<vbuffer0[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res06.key != udata06.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer0[6]["<<v_buffer_index<<"].key: "<<vbuffer0[6][v_buffer_index].key<<endl; }
				if(res06.value != udata06.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer0[6]["<<v_buffer_index<<"].value: "<<vbuffer0[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata07 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata07 = acts_utilobj->UTIL_GETKV2(vbuffer0[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer0[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata07);
				
				keyvalue_t res07; 
				res07.key = MERGE_applyfunc(udata07.key, vdata07.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res07.value = MERGE_applyfunc(udata07.value, vdata07.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer0[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[7]["<<v_buffer_index<<"].key: "<<vbuffer0[7][v_buffer_index].key<<endl; }
				if(vbuffer0[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[7]["<<v_buffer_index<<"].value: "<<vbuffer0[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[7]["<<v_buffer_index<<"].key: "<<vbuffer0[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer0[7]["<<v_buffer_index<<"].value: "<<vbuffer0[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res07.key != udata07.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer0[7]["<<v_buffer_index<<"].key: "<<vbuffer0[7][v_buffer_index].key<<endl; }
				if(res07.value != udata07.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer0[7]["<<v_buffer_index<<"].value: "<<vbuffer0[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata10 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata10 = acts_utilobj->UTIL_GETKV2(vbuffer1[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer1[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata10);
				
				keyvalue_t res10; 
				res10.key = MERGE_applyfunc(udata10.key, vdata10.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res10.value = MERGE_applyfunc(udata10.value, vdata10.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer1[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[0]["<<v_buffer_index<<"].key: "<<vbuffer1[0][v_buffer_index].key<<endl; }
				if(vbuffer1[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[0]["<<v_buffer_index<<"].value: "<<vbuffer1[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[0]["<<v_buffer_index<<"].key: "<<vbuffer1[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[0]["<<v_buffer_index<<"].value: "<<vbuffer1[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res10.key != udata10.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer1[0]["<<v_buffer_index<<"].key: "<<vbuffer1[0][v_buffer_index].key<<endl; }
				if(res10.value != udata10.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer1[0]["<<v_buffer_index<<"].value: "<<vbuffer1[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata11 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata11 = acts_utilobj->UTIL_GETKV2(vbuffer1[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer1[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata11);
				
				keyvalue_t res11; 
				res11.key = MERGE_applyfunc(udata11.key, vdata11.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res11.value = MERGE_applyfunc(udata11.value, vdata11.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer1[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[1]["<<v_buffer_index<<"].key: "<<vbuffer1[1][v_buffer_index].key<<endl; }
				if(vbuffer1[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[1]["<<v_buffer_index<<"].value: "<<vbuffer1[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[1]["<<v_buffer_index<<"].key: "<<vbuffer1[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[1]["<<v_buffer_index<<"].value: "<<vbuffer1[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res11.key != udata11.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer1[1]["<<v_buffer_index<<"].key: "<<vbuffer1[1][v_buffer_index].key<<endl; }
				if(res11.value != udata11.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer1[1]["<<v_buffer_index<<"].value: "<<vbuffer1[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata12 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata12 = acts_utilobj->UTIL_GETKV2(vbuffer1[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer1[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata12);
				
				keyvalue_t res12; 
				res12.key = MERGE_applyfunc(udata12.key, vdata12.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res12.value = MERGE_applyfunc(udata12.value, vdata12.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer1[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[2]["<<v_buffer_index<<"].key: "<<vbuffer1[2][v_buffer_index].key<<endl; }
				if(vbuffer1[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[2]["<<v_buffer_index<<"].value: "<<vbuffer1[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[2]["<<v_buffer_index<<"].key: "<<vbuffer1[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[2]["<<v_buffer_index<<"].value: "<<vbuffer1[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res12.key != udata12.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer1[2]["<<v_buffer_index<<"].key: "<<vbuffer1[2][v_buffer_index].key<<endl; }
				if(res12.value != udata12.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer1[2]["<<v_buffer_index<<"].value: "<<vbuffer1[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata13 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata13 = acts_utilobj->UTIL_GETKV2(vbuffer1[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer1[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata13);
				
				keyvalue_t res13; 
				res13.key = MERGE_applyfunc(udata13.key, vdata13.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res13.value = MERGE_applyfunc(udata13.value, vdata13.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer1[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[3]["<<v_buffer_index<<"].key: "<<vbuffer1[3][v_buffer_index].key<<endl; }
				if(vbuffer1[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[3]["<<v_buffer_index<<"].value: "<<vbuffer1[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[3]["<<v_buffer_index<<"].key: "<<vbuffer1[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[3]["<<v_buffer_index<<"].value: "<<vbuffer1[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res13.key != udata13.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer1[3]["<<v_buffer_index<<"].key: "<<vbuffer1[3][v_buffer_index].key<<endl; }
				if(res13.value != udata13.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer1[3]["<<v_buffer_index<<"].value: "<<vbuffer1[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata14 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata14 = acts_utilobj->UTIL_GETKV2(vbuffer1[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer1[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata14);
				
				keyvalue_t res14; 
				res14.key = MERGE_applyfunc(udata14.key, vdata14.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res14.value = MERGE_applyfunc(udata14.value, vdata14.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer1[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[4]["<<v_buffer_index<<"].key: "<<vbuffer1[4][v_buffer_index].key<<endl; }
				if(vbuffer1[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[4]["<<v_buffer_index<<"].value: "<<vbuffer1[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[4]["<<v_buffer_index<<"].key: "<<vbuffer1[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[4]["<<v_buffer_index<<"].value: "<<vbuffer1[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res14.key != udata14.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer1[4]["<<v_buffer_index<<"].key: "<<vbuffer1[4][v_buffer_index].key<<endl; }
				if(res14.value != udata14.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer1[4]["<<v_buffer_index<<"].value: "<<vbuffer1[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata15 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata15 = acts_utilobj->UTIL_GETKV2(vbuffer1[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer1[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata15);
				
				keyvalue_t res15; 
				res15.key = MERGE_applyfunc(udata15.key, vdata15.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res15.value = MERGE_applyfunc(udata15.value, vdata15.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer1[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[5]["<<v_buffer_index<<"].key: "<<vbuffer1[5][v_buffer_index].key<<endl; }
				if(vbuffer1[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[5]["<<v_buffer_index<<"].value: "<<vbuffer1[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[5]["<<v_buffer_index<<"].key: "<<vbuffer1[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[5]["<<v_buffer_index<<"].value: "<<vbuffer1[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res15.key != udata15.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer1[5]["<<v_buffer_index<<"].key: "<<vbuffer1[5][v_buffer_index].key<<endl; }
				if(res15.value != udata15.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer1[5]["<<v_buffer_index<<"].value: "<<vbuffer1[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata16 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata16 = acts_utilobj->UTIL_GETKV2(vbuffer1[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer1[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata16);
				
				keyvalue_t res16; 
				res16.key = MERGE_applyfunc(udata16.key, vdata16.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res16.value = MERGE_applyfunc(udata16.value, vdata16.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer1[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[6]["<<v_buffer_index<<"].key: "<<vbuffer1[6][v_buffer_index].key<<endl; }
				if(vbuffer1[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[6]["<<v_buffer_index<<"].value: "<<vbuffer1[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[6]["<<v_buffer_index<<"].key: "<<vbuffer1[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[6]["<<v_buffer_index<<"].value: "<<vbuffer1[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res16.key != udata16.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer1[6]["<<v_buffer_index<<"].key: "<<vbuffer1[6][v_buffer_index].key<<endl; }
				if(res16.value != udata16.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer1[6]["<<v_buffer_index<<"].value: "<<vbuffer1[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata17 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata17 = acts_utilobj->UTIL_GETKV2(vbuffer1[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer1[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata17);
				
				keyvalue_t res17; 
				res17.key = MERGE_applyfunc(udata17.key, vdata17.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res17.value = MERGE_applyfunc(udata17.value, vdata17.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer1[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[7]["<<v_buffer_index<<"].key: "<<vbuffer1[7][v_buffer_index].key<<endl; }
				if(vbuffer1[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[7]["<<v_buffer_index<<"].value: "<<vbuffer1[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[7]["<<v_buffer_index<<"].key: "<<vbuffer1[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer1[7]["<<v_buffer_index<<"].value: "<<vbuffer1[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res17.key != udata17.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer1[7]["<<v_buffer_index<<"].key: "<<vbuffer1[7][v_buffer_index].key<<endl; }
				if(res17.value != udata17.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer1[7]["<<v_buffer_index<<"].value: "<<vbuffer1[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata20 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata20 = acts_utilobj->UTIL_GETKV2(vbuffer2[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer2[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata20);
				
				keyvalue_t res20; 
				res20.key = MERGE_applyfunc(udata20.key, vdata20.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res20.value = MERGE_applyfunc(udata20.value, vdata20.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer2[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[0]["<<v_buffer_index<<"].key: "<<vbuffer2[0][v_buffer_index].key<<endl; }
				if(vbuffer2[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[0]["<<v_buffer_index<<"].value: "<<vbuffer2[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[0]["<<v_buffer_index<<"].key: "<<vbuffer2[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[0]["<<v_buffer_index<<"].value: "<<vbuffer2[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res20.key != udata20.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer2[0]["<<v_buffer_index<<"].key: "<<vbuffer2[0][v_buffer_index].key<<endl; }
				if(res20.value != udata20.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer2[0]["<<v_buffer_index<<"].value: "<<vbuffer2[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata21 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata21 = acts_utilobj->UTIL_GETKV2(vbuffer2[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer2[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata21);
				
				keyvalue_t res21; 
				res21.key = MERGE_applyfunc(udata21.key, vdata21.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res21.value = MERGE_applyfunc(udata21.value, vdata21.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer2[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[1]["<<v_buffer_index<<"].key: "<<vbuffer2[1][v_buffer_index].key<<endl; }
				if(vbuffer2[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[1]["<<v_buffer_index<<"].value: "<<vbuffer2[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[1]["<<v_buffer_index<<"].key: "<<vbuffer2[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[1]["<<v_buffer_index<<"].value: "<<vbuffer2[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res21.key != udata21.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer2[1]["<<v_buffer_index<<"].key: "<<vbuffer2[1][v_buffer_index].key<<endl; }
				if(res21.value != udata21.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer2[1]["<<v_buffer_index<<"].value: "<<vbuffer2[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata22 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata22 = acts_utilobj->UTIL_GETKV2(vbuffer2[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer2[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata22);
				
				keyvalue_t res22; 
				res22.key = MERGE_applyfunc(udata22.key, vdata22.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res22.value = MERGE_applyfunc(udata22.value, vdata22.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer2[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[2]["<<v_buffer_index<<"].key: "<<vbuffer2[2][v_buffer_index].key<<endl; }
				if(vbuffer2[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[2]["<<v_buffer_index<<"].value: "<<vbuffer2[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[2]["<<v_buffer_index<<"].key: "<<vbuffer2[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[2]["<<v_buffer_index<<"].value: "<<vbuffer2[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res22.key != udata22.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer2[2]["<<v_buffer_index<<"].key: "<<vbuffer2[2][v_buffer_index].key<<endl; }
				if(res22.value != udata22.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer2[2]["<<v_buffer_index<<"].value: "<<vbuffer2[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata23 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata23 = acts_utilobj->UTIL_GETKV2(vbuffer2[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer2[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata23);
				
				keyvalue_t res23; 
				res23.key = MERGE_applyfunc(udata23.key, vdata23.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res23.value = MERGE_applyfunc(udata23.value, vdata23.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer2[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[3]["<<v_buffer_index<<"].key: "<<vbuffer2[3][v_buffer_index].key<<endl; }
				if(vbuffer2[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[3]["<<v_buffer_index<<"].value: "<<vbuffer2[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[3]["<<v_buffer_index<<"].key: "<<vbuffer2[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[3]["<<v_buffer_index<<"].value: "<<vbuffer2[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res23.key != udata23.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer2[3]["<<v_buffer_index<<"].key: "<<vbuffer2[3][v_buffer_index].key<<endl; }
				if(res23.value != udata23.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer2[3]["<<v_buffer_index<<"].value: "<<vbuffer2[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata24 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata24 = acts_utilobj->UTIL_GETKV2(vbuffer2[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer2[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata24);
				
				keyvalue_t res24; 
				res24.key = MERGE_applyfunc(udata24.key, vdata24.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res24.value = MERGE_applyfunc(udata24.value, vdata24.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer2[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[4]["<<v_buffer_index<<"].key: "<<vbuffer2[4][v_buffer_index].key<<endl; }
				if(vbuffer2[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[4]["<<v_buffer_index<<"].value: "<<vbuffer2[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[4]["<<v_buffer_index<<"].key: "<<vbuffer2[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[4]["<<v_buffer_index<<"].value: "<<vbuffer2[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res24.key != udata24.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer2[4]["<<v_buffer_index<<"].key: "<<vbuffer2[4][v_buffer_index].key<<endl; }
				if(res24.value != udata24.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer2[4]["<<v_buffer_index<<"].value: "<<vbuffer2[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata25 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata25 = acts_utilobj->UTIL_GETKV2(vbuffer2[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer2[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata25);
				
				keyvalue_t res25; 
				res25.key = MERGE_applyfunc(udata25.key, vdata25.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res25.value = MERGE_applyfunc(udata25.value, vdata25.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer2[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[5]["<<v_buffer_index<<"].key: "<<vbuffer2[5][v_buffer_index].key<<endl; }
				if(vbuffer2[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[5]["<<v_buffer_index<<"].value: "<<vbuffer2[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[5]["<<v_buffer_index<<"].key: "<<vbuffer2[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[5]["<<v_buffer_index<<"].value: "<<vbuffer2[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res25.key != udata25.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer2[5]["<<v_buffer_index<<"].key: "<<vbuffer2[5][v_buffer_index].key<<endl; }
				if(res25.value != udata25.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer2[5]["<<v_buffer_index<<"].value: "<<vbuffer2[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata26 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata26 = acts_utilobj->UTIL_GETKV2(vbuffer2[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer2[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata26);
				
				keyvalue_t res26; 
				res26.key = MERGE_applyfunc(udata26.key, vdata26.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res26.value = MERGE_applyfunc(udata26.value, vdata26.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer2[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[6]["<<v_buffer_index<<"].key: "<<vbuffer2[6][v_buffer_index].key<<endl; }
				if(vbuffer2[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[6]["<<v_buffer_index<<"].value: "<<vbuffer2[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[6]["<<v_buffer_index<<"].key: "<<vbuffer2[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[6]["<<v_buffer_index<<"].value: "<<vbuffer2[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res26.key != udata26.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer2[6]["<<v_buffer_index<<"].key: "<<vbuffer2[6][v_buffer_index].key<<endl; }
				if(res26.value != udata26.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer2[6]["<<v_buffer_index<<"].value: "<<vbuffer2[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata27 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata27 = acts_utilobj->UTIL_GETKV2(vbuffer2[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer2[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata27);
				
				keyvalue_t res27; 
				res27.key = MERGE_applyfunc(udata27.key, vdata27.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res27.value = MERGE_applyfunc(udata27.value, vdata27.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer2[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[7]["<<v_buffer_index<<"].key: "<<vbuffer2[7][v_buffer_index].key<<endl; }
				if(vbuffer2[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[7]["<<v_buffer_index<<"].value: "<<vbuffer2[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[7]["<<v_buffer_index<<"].key: "<<vbuffer2[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer2[7]["<<v_buffer_index<<"].value: "<<vbuffer2[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res27.key != udata27.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer2[7]["<<v_buffer_index<<"].key: "<<vbuffer2[7][v_buffer_index].key<<endl; }
				if(res27.value != udata27.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer2[7]["<<v_buffer_index<<"].value: "<<vbuffer2[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata30 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata30 = acts_utilobj->UTIL_GETKV2(vbuffer3[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer3[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata30);
				
				keyvalue_t res30; 
				res30.key = MERGE_applyfunc(udata30.key, vdata30.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res30.value = MERGE_applyfunc(udata30.value, vdata30.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer3[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[0]["<<v_buffer_index<<"].key: "<<vbuffer3[0][v_buffer_index].key<<endl; }
				if(vbuffer3[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[0]["<<v_buffer_index<<"].value: "<<vbuffer3[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[0]["<<v_buffer_index<<"].key: "<<vbuffer3[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[0]["<<v_buffer_index<<"].value: "<<vbuffer3[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res30.key != udata30.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer3[0]["<<v_buffer_index<<"].key: "<<vbuffer3[0][v_buffer_index].key<<endl; }
				if(res30.value != udata30.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer3[0]["<<v_buffer_index<<"].value: "<<vbuffer3[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata31 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata31 = acts_utilobj->UTIL_GETKV2(vbuffer3[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer3[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata31);
				
				keyvalue_t res31; 
				res31.key = MERGE_applyfunc(udata31.key, vdata31.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res31.value = MERGE_applyfunc(udata31.value, vdata31.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer3[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[1]["<<v_buffer_index<<"].key: "<<vbuffer3[1][v_buffer_index].key<<endl; }
				if(vbuffer3[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[1]["<<v_buffer_index<<"].value: "<<vbuffer3[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[1]["<<v_buffer_index<<"].key: "<<vbuffer3[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[1]["<<v_buffer_index<<"].value: "<<vbuffer3[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res31.key != udata31.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer3[1]["<<v_buffer_index<<"].key: "<<vbuffer3[1][v_buffer_index].key<<endl; }
				if(res31.value != udata31.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer3[1]["<<v_buffer_index<<"].value: "<<vbuffer3[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata32 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata32 = acts_utilobj->UTIL_GETKV2(vbuffer3[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer3[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata32);
				
				keyvalue_t res32; 
				res32.key = MERGE_applyfunc(udata32.key, vdata32.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res32.value = MERGE_applyfunc(udata32.value, vdata32.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer3[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[2]["<<v_buffer_index<<"].key: "<<vbuffer3[2][v_buffer_index].key<<endl; }
				if(vbuffer3[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[2]["<<v_buffer_index<<"].value: "<<vbuffer3[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[2]["<<v_buffer_index<<"].key: "<<vbuffer3[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[2]["<<v_buffer_index<<"].value: "<<vbuffer3[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res32.key != udata32.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer3[2]["<<v_buffer_index<<"].key: "<<vbuffer3[2][v_buffer_index].key<<endl; }
				if(res32.value != udata32.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer3[2]["<<v_buffer_index<<"].value: "<<vbuffer3[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata33 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata33 = acts_utilobj->UTIL_GETKV2(vbuffer3[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer3[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata33);
				
				keyvalue_t res33; 
				res33.key = MERGE_applyfunc(udata33.key, vdata33.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res33.value = MERGE_applyfunc(udata33.value, vdata33.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer3[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[3]["<<v_buffer_index<<"].key: "<<vbuffer3[3][v_buffer_index].key<<endl; }
				if(vbuffer3[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[3]["<<v_buffer_index<<"].value: "<<vbuffer3[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[3]["<<v_buffer_index<<"].key: "<<vbuffer3[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[3]["<<v_buffer_index<<"].value: "<<vbuffer3[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res33.key != udata33.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer3[3]["<<v_buffer_index<<"].key: "<<vbuffer3[3][v_buffer_index].key<<endl; }
				if(res33.value != udata33.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer3[3]["<<v_buffer_index<<"].value: "<<vbuffer3[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata34 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata34 = acts_utilobj->UTIL_GETKV2(vbuffer3[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer3[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata34);
				
				keyvalue_t res34; 
				res34.key = MERGE_applyfunc(udata34.key, vdata34.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res34.value = MERGE_applyfunc(udata34.value, vdata34.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer3[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[4]["<<v_buffer_index<<"].key: "<<vbuffer3[4][v_buffer_index].key<<endl; }
				if(vbuffer3[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[4]["<<v_buffer_index<<"].value: "<<vbuffer3[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[4]["<<v_buffer_index<<"].key: "<<vbuffer3[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[4]["<<v_buffer_index<<"].value: "<<vbuffer3[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res34.key != udata34.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer3[4]["<<v_buffer_index<<"].key: "<<vbuffer3[4][v_buffer_index].key<<endl; }
				if(res34.value != udata34.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer3[4]["<<v_buffer_index<<"].value: "<<vbuffer3[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata35 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata35 = acts_utilobj->UTIL_GETKV2(vbuffer3[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer3[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata35);
				
				keyvalue_t res35; 
				res35.key = MERGE_applyfunc(udata35.key, vdata35.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res35.value = MERGE_applyfunc(udata35.value, vdata35.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer3[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[5]["<<v_buffer_index<<"].key: "<<vbuffer3[5][v_buffer_index].key<<endl; }
				if(vbuffer3[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[5]["<<v_buffer_index<<"].value: "<<vbuffer3[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[5]["<<v_buffer_index<<"].key: "<<vbuffer3[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[5]["<<v_buffer_index<<"].value: "<<vbuffer3[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res35.key != udata35.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer3[5]["<<v_buffer_index<<"].key: "<<vbuffer3[5][v_buffer_index].key<<endl; }
				if(res35.value != udata35.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer3[5]["<<v_buffer_index<<"].value: "<<vbuffer3[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata36 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata36 = acts_utilobj->UTIL_GETKV2(vbuffer3[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer3[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata36);
				
				keyvalue_t res36; 
				res36.key = MERGE_applyfunc(udata36.key, vdata36.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res36.value = MERGE_applyfunc(udata36.value, vdata36.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer3[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[6]["<<v_buffer_index<<"].key: "<<vbuffer3[6][v_buffer_index].key<<endl; }
				if(vbuffer3[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[6]["<<v_buffer_index<<"].value: "<<vbuffer3[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[6]["<<v_buffer_index<<"].key: "<<vbuffer3[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[6]["<<v_buffer_index<<"].value: "<<vbuffer3[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res36.key != udata36.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer3[6]["<<v_buffer_index<<"].key: "<<vbuffer3[6][v_buffer_index].key<<endl; }
				if(res36.value != udata36.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer3[6]["<<v_buffer_index<<"].value: "<<vbuffer3[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata37 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata37 = acts_utilobj->UTIL_GETKV2(vbuffer3[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer3[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata37);
				
				keyvalue_t res37; 
				res37.key = MERGE_applyfunc(udata37.key, vdata37.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res37.value = MERGE_applyfunc(udata37.value, vdata37.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer3[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[7]["<<v_buffer_index<<"].key: "<<vbuffer3[7][v_buffer_index].key<<endl; }
				if(vbuffer3[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[7]["<<v_buffer_index<<"].value: "<<vbuffer3[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[7]["<<v_buffer_index<<"].key: "<<vbuffer3[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer3[7]["<<v_buffer_index<<"].value: "<<vbuffer3[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res37.key != udata37.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer3[7]["<<v_buffer_index<<"].key: "<<vbuffer3[7][v_buffer_index].key<<endl; }
				if(res37.value != udata37.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer3[7]["<<v_buffer_index<<"].value: "<<vbuffer3[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata40 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata40 = acts_utilobj->UTIL_GETKV2(vbuffer4[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer4[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata40);
				
				keyvalue_t res40; 
				res40.key = MERGE_applyfunc(udata40.key, vdata40.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res40.value = MERGE_applyfunc(udata40.value, vdata40.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer4[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[0]["<<v_buffer_index<<"].key: "<<vbuffer4[0][v_buffer_index].key<<endl; }
				if(vbuffer4[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[0]["<<v_buffer_index<<"].value: "<<vbuffer4[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[0]["<<v_buffer_index<<"].key: "<<vbuffer4[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[0]["<<v_buffer_index<<"].value: "<<vbuffer4[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res40.key != udata40.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer4[0]["<<v_buffer_index<<"].key: "<<vbuffer4[0][v_buffer_index].key<<endl; }
				if(res40.value != udata40.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer4[0]["<<v_buffer_index<<"].value: "<<vbuffer4[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata41 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata41 = acts_utilobj->UTIL_GETKV2(vbuffer4[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer4[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata41);
				
				keyvalue_t res41; 
				res41.key = MERGE_applyfunc(udata41.key, vdata41.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res41.value = MERGE_applyfunc(udata41.value, vdata41.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer4[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[1]["<<v_buffer_index<<"].key: "<<vbuffer4[1][v_buffer_index].key<<endl; }
				if(vbuffer4[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[1]["<<v_buffer_index<<"].value: "<<vbuffer4[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[1]["<<v_buffer_index<<"].key: "<<vbuffer4[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[1]["<<v_buffer_index<<"].value: "<<vbuffer4[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res41.key != udata41.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer4[1]["<<v_buffer_index<<"].key: "<<vbuffer4[1][v_buffer_index].key<<endl; }
				if(res41.value != udata41.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer4[1]["<<v_buffer_index<<"].value: "<<vbuffer4[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata42 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata42 = acts_utilobj->UTIL_GETKV2(vbuffer4[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer4[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata42);
				
				keyvalue_t res42; 
				res42.key = MERGE_applyfunc(udata42.key, vdata42.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res42.value = MERGE_applyfunc(udata42.value, vdata42.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer4[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[2]["<<v_buffer_index<<"].key: "<<vbuffer4[2][v_buffer_index].key<<endl; }
				if(vbuffer4[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[2]["<<v_buffer_index<<"].value: "<<vbuffer4[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[2]["<<v_buffer_index<<"].key: "<<vbuffer4[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[2]["<<v_buffer_index<<"].value: "<<vbuffer4[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res42.key != udata42.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer4[2]["<<v_buffer_index<<"].key: "<<vbuffer4[2][v_buffer_index].key<<endl; }
				if(res42.value != udata42.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer4[2]["<<v_buffer_index<<"].value: "<<vbuffer4[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata43 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata43 = acts_utilobj->UTIL_GETKV2(vbuffer4[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer4[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata43);
				
				keyvalue_t res43; 
				res43.key = MERGE_applyfunc(udata43.key, vdata43.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res43.value = MERGE_applyfunc(udata43.value, vdata43.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer4[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[3]["<<v_buffer_index<<"].key: "<<vbuffer4[3][v_buffer_index].key<<endl; }
				if(vbuffer4[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[3]["<<v_buffer_index<<"].value: "<<vbuffer4[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[3]["<<v_buffer_index<<"].key: "<<vbuffer4[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[3]["<<v_buffer_index<<"].value: "<<vbuffer4[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res43.key != udata43.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer4[3]["<<v_buffer_index<<"].key: "<<vbuffer4[3][v_buffer_index].key<<endl; }
				if(res43.value != udata43.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer4[3]["<<v_buffer_index<<"].value: "<<vbuffer4[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata44 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata44 = acts_utilobj->UTIL_GETKV2(vbuffer4[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer4[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata44);
				
				keyvalue_t res44; 
				res44.key = MERGE_applyfunc(udata44.key, vdata44.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res44.value = MERGE_applyfunc(udata44.value, vdata44.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer4[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[4]["<<v_buffer_index<<"].key: "<<vbuffer4[4][v_buffer_index].key<<endl; }
				if(vbuffer4[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[4]["<<v_buffer_index<<"].value: "<<vbuffer4[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[4]["<<v_buffer_index<<"].key: "<<vbuffer4[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[4]["<<v_buffer_index<<"].value: "<<vbuffer4[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res44.key != udata44.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer4[4]["<<v_buffer_index<<"].key: "<<vbuffer4[4][v_buffer_index].key<<endl; }
				if(res44.value != udata44.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer4[4]["<<v_buffer_index<<"].value: "<<vbuffer4[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata45 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata45 = acts_utilobj->UTIL_GETKV2(vbuffer4[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer4[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata45);
				
				keyvalue_t res45; 
				res45.key = MERGE_applyfunc(udata45.key, vdata45.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res45.value = MERGE_applyfunc(udata45.value, vdata45.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer4[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[5]["<<v_buffer_index<<"].key: "<<vbuffer4[5][v_buffer_index].key<<endl; }
				if(vbuffer4[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[5]["<<v_buffer_index<<"].value: "<<vbuffer4[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[5]["<<v_buffer_index<<"].key: "<<vbuffer4[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[5]["<<v_buffer_index<<"].value: "<<vbuffer4[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res45.key != udata45.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer4[5]["<<v_buffer_index<<"].key: "<<vbuffer4[5][v_buffer_index].key<<endl; }
				if(res45.value != udata45.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer4[5]["<<v_buffer_index<<"].value: "<<vbuffer4[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata46 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata46 = acts_utilobj->UTIL_GETKV2(vbuffer4[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer4[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata46);
				
				keyvalue_t res46; 
				res46.key = MERGE_applyfunc(udata46.key, vdata46.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res46.value = MERGE_applyfunc(udata46.value, vdata46.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer4[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[6]["<<v_buffer_index<<"].key: "<<vbuffer4[6][v_buffer_index].key<<endl; }
				if(vbuffer4[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[6]["<<v_buffer_index<<"].value: "<<vbuffer4[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[6]["<<v_buffer_index<<"].key: "<<vbuffer4[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[6]["<<v_buffer_index<<"].value: "<<vbuffer4[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res46.key != udata46.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer4[6]["<<v_buffer_index<<"].key: "<<vbuffer4[6][v_buffer_index].key<<endl; }
				if(res46.value != udata46.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer4[6]["<<v_buffer_index<<"].value: "<<vbuffer4[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata47 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata47 = acts_utilobj->UTIL_GETKV2(vbuffer4[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer4[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata47);
				
				keyvalue_t res47; 
				res47.key = MERGE_applyfunc(udata47.key, vdata47.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res47.value = MERGE_applyfunc(udata47.value, vdata47.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer4[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[7]["<<v_buffer_index<<"].key: "<<vbuffer4[7][v_buffer_index].key<<endl; }
				if(vbuffer4[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[7]["<<v_buffer_index<<"].value: "<<vbuffer4[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[7]["<<v_buffer_index<<"].key: "<<vbuffer4[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer4[7]["<<v_buffer_index<<"].value: "<<vbuffer4[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res47.key != udata47.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer4[7]["<<v_buffer_index<<"].key: "<<vbuffer4[7][v_buffer_index].key<<endl; }
				if(res47.value != udata47.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer4[7]["<<v_buffer_index<<"].value: "<<vbuffer4[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata50 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata50 = acts_utilobj->UTIL_GETKV2(vbuffer5[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer5[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata50);
				
				keyvalue_t res50; 
				res50.key = MERGE_applyfunc(udata50.key, vdata50.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res50.value = MERGE_applyfunc(udata50.value, vdata50.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer5[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[0]["<<v_buffer_index<<"].key: "<<vbuffer5[0][v_buffer_index].key<<endl; }
				if(vbuffer5[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[0]["<<v_buffer_index<<"].value: "<<vbuffer5[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[0]["<<v_buffer_index<<"].key: "<<vbuffer5[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[0]["<<v_buffer_index<<"].value: "<<vbuffer5[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res50.key != udata50.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer5[0]["<<v_buffer_index<<"].key: "<<vbuffer5[0][v_buffer_index].key<<endl; }
				if(res50.value != udata50.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer5[0]["<<v_buffer_index<<"].value: "<<vbuffer5[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata51 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata51 = acts_utilobj->UTIL_GETKV2(vbuffer5[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer5[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata51);
				
				keyvalue_t res51; 
				res51.key = MERGE_applyfunc(udata51.key, vdata51.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res51.value = MERGE_applyfunc(udata51.value, vdata51.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer5[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[1]["<<v_buffer_index<<"].key: "<<vbuffer5[1][v_buffer_index].key<<endl; }
				if(vbuffer5[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[1]["<<v_buffer_index<<"].value: "<<vbuffer5[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[1]["<<v_buffer_index<<"].key: "<<vbuffer5[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[1]["<<v_buffer_index<<"].value: "<<vbuffer5[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res51.key != udata51.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer5[1]["<<v_buffer_index<<"].key: "<<vbuffer5[1][v_buffer_index].key<<endl; }
				if(res51.value != udata51.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer5[1]["<<v_buffer_index<<"].value: "<<vbuffer5[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata52 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata52 = acts_utilobj->UTIL_GETKV2(vbuffer5[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer5[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata52);
				
				keyvalue_t res52; 
				res52.key = MERGE_applyfunc(udata52.key, vdata52.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res52.value = MERGE_applyfunc(udata52.value, vdata52.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer5[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[2]["<<v_buffer_index<<"].key: "<<vbuffer5[2][v_buffer_index].key<<endl; }
				if(vbuffer5[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[2]["<<v_buffer_index<<"].value: "<<vbuffer5[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[2]["<<v_buffer_index<<"].key: "<<vbuffer5[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[2]["<<v_buffer_index<<"].value: "<<vbuffer5[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res52.key != udata52.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer5[2]["<<v_buffer_index<<"].key: "<<vbuffer5[2][v_buffer_index].key<<endl; }
				if(res52.value != udata52.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer5[2]["<<v_buffer_index<<"].value: "<<vbuffer5[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata53 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata53 = acts_utilobj->UTIL_GETKV2(vbuffer5[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer5[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata53);
				
				keyvalue_t res53; 
				res53.key = MERGE_applyfunc(udata53.key, vdata53.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res53.value = MERGE_applyfunc(udata53.value, vdata53.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer5[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[3]["<<v_buffer_index<<"].key: "<<vbuffer5[3][v_buffer_index].key<<endl; }
				if(vbuffer5[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[3]["<<v_buffer_index<<"].value: "<<vbuffer5[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[3]["<<v_buffer_index<<"].key: "<<vbuffer5[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[3]["<<v_buffer_index<<"].value: "<<vbuffer5[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res53.key != udata53.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer5[3]["<<v_buffer_index<<"].key: "<<vbuffer5[3][v_buffer_index].key<<endl; }
				if(res53.value != udata53.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer5[3]["<<v_buffer_index<<"].value: "<<vbuffer5[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata54 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata54 = acts_utilobj->UTIL_GETKV2(vbuffer5[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer5[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata54);
				
				keyvalue_t res54; 
				res54.key = MERGE_applyfunc(udata54.key, vdata54.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res54.value = MERGE_applyfunc(udata54.value, vdata54.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer5[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[4]["<<v_buffer_index<<"].key: "<<vbuffer5[4][v_buffer_index].key<<endl; }
				if(vbuffer5[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[4]["<<v_buffer_index<<"].value: "<<vbuffer5[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[4]["<<v_buffer_index<<"].key: "<<vbuffer5[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[4]["<<v_buffer_index<<"].value: "<<vbuffer5[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res54.key != udata54.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer5[4]["<<v_buffer_index<<"].key: "<<vbuffer5[4][v_buffer_index].key<<endl; }
				if(res54.value != udata54.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer5[4]["<<v_buffer_index<<"].value: "<<vbuffer5[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata55 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata55 = acts_utilobj->UTIL_GETKV2(vbuffer5[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer5[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata55);
				
				keyvalue_t res55; 
				res55.key = MERGE_applyfunc(udata55.key, vdata55.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res55.value = MERGE_applyfunc(udata55.value, vdata55.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer5[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[5]["<<v_buffer_index<<"].key: "<<vbuffer5[5][v_buffer_index].key<<endl; }
				if(vbuffer5[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[5]["<<v_buffer_index<<"].value: "<<vbuffer5[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[5]["<<v_buffer_index<<"].key: "<<vbuffer5[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[5]["<<v_buffer_index<<"].value: "<<vbuffer5[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res55.key != udata55.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer5[5]["<<v_buffer_index<<"].key: "<<vbuffer5[5][v_buffer_index].key<<endl; }
				if(res55.value != udata55.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer5[5]["<<v_buffer_index<<"].value: "<<vbuffer5[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata56 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata56 = acts_utilobj->UTIL_GETKV2(vbuffer5[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer5[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata56);
				
				keyvalue_t res56; 
				res56.key = MERGE_applyfunc(udata56.key, vdata56.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res56.value = MERGE_applyfunc(udata56.value, vdata56.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer5[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[6]["<<v_buffer_index<<"].key: "<<vbuffer5[6][v_buffer_index].key<<endl; }
				if(vbuffer5[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[6]["<<v_buffer_index<<"].value: "<<vbuffer5[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[6]["<<v_buffer_index<<"].key: "<<vbuffer5[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[6]["<<v_buffer_index<<"].value: "<<vbuffer5[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res56.key != udata56.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer5[6]["<<v_buffer_index<<"].key: "<<vbuffer5[6][v_buffer_index].key<<endl; }
				if(res56.value != udata56.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer5[6]["<<v_buffer_index<<"].value: "<<vbuffer5[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata57 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata57 = acts_utilobj->UTIL_GETKV2(vbuffer5[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer5[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata57);
				
				keyvalue_t res57; 
				res57.key = MERGE_applyfunc(udata57.key, vdata57.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res57.value = MERGE_applyfunc(udata57.value, vdata57.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer5[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[7]["<<v_buffer_index<<"].key: "<<vbuffer5[7][v_buffer_index].key<<endl; }
				if(vbuffer5[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[7]["<<v_buffer_index<<"].value: "<<vbuffer5[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[7]["<<v_buffer_index<<"].key: "<<vbuffer5[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer5[7]["<<v_buffer_index<<"].value: "<<vbuffer5[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res57.key != udata57.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer5[7]["<<v_buffer_index<<"].key: "<<vbuffer5[7][v_buffer_index].key<<endl; }
				if(res57.value != udata57.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer5[7]["<<v_buffer_index<<"].value: "<<vbuffer5[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata60 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata60 = acts_utilobj->UTIL_GETKV2(vbuffer6[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer6[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata60);
				
				keyvalue_t res60; 
				res60.key = MERGE_applyfunc(udata60.key, vdata60.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res60.value = MERGE_applyfunc(udata60.value, vdata60.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer6[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[0]["<<v_buffer_index<<"].key: "<<vbuffer6[0][v_buffer_index].key<<endl; }
				if(vbuffer6[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[0]["<<v_buffer_index<<"].value: "<<vbuffer6[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[0]["<<v_buffer_index<<"].key: "<<vbuffer6[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[0]["<<v_buffer_index<<"].value: "<<vbuffer6[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res60.key != udata60.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer6[0]["<<v_buffer_index<<"].key: "<<vbuffer6[0][v_buffer_index].key<<endl; }
				if(res60.value != udata60.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer6[0]["<<v_buffer_index<<"].value: "<<vbuffer6[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata61 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata61 = acts_utilobj->UTIL_GETKV2(vbuffer6[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer6[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata61);
				
				keyvalue_t res61; 
				res61.key = MERGE_applyfunc(udata61.key, vdata61.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res61.value = MERGE_applyfunc(udata61.value, vdata61.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer6[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[1]["<<v_buffer_index<<"].key: "<<vbuffer6[1][v_buffer_index].key<<endl; }
				if(vbuffer6[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[1]["<<v_buffer_index<<"].value: "<<vbuffer6[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[1]["<<v_buffer_index<<"].key: "<<vbuffer6[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[1]["<<v_buffer_index<<"].value: "<<vbuffer6[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res61.key != udata61.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer6[1]["<<v_buffer_index<<"].key: "<<vbuffer6[1][v_buffer_index].key<<endl; }
				if(res61.value != udata61.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer6[1]["<<v_buffer_index<<"].value: "<<vbuffer6[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata62 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata62 = acts_utilobj->UTIL_GETKV2(vbuffer6[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer6[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata62);
				
				keyvalue_t res62; 
				res62.key = MERGE_applyfunc(udata62.key, vdata62.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res62.value = MERGE_applyfunc(udata62.value, vdata62.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer6[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[2]["<<v_buffer_index<<"].key: "<<vbuffer6[2][v_buffer_index].key<<endl; }
				if(vbuffer6[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[2]["<<v_buffer_index<<"].value: "<<vbuffer6[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[2]["<<v_buffer_index<<"].key: "<<vbuffer6[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[2]["<<v_buffer_index<<"].value: "<<vbuffer6[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res62.key != udata62.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer6[2]["<<v_buffer_index<<"].key: "<<vbuffer6[2][v_buffer_index].key<<endl; }
				if(res62.value != udata62.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer6[2]["<<v_buffer_index<<"].value: "<<vbuffer6[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata63 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata63 = acts_utilobj->UTIL_GETKV2(vbuffer6[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer6[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata63);
				
				keyvalue_t res63; 
				res63.key = MERGE_applyfunc(udata63.key, vdata63.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res63.value = MERGE_applyfunc(udata63.value, vdata63.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer6[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[3]["<<v_buffer_index<<"].key: "<<vbuffer6[3][v_buffer_index].key<<endl; }
				if(vbuffer6[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[3]["<<v_buffer_index<<"].value: "<<vbuffer6[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[3]["<<v_buffer_index<<"].key: "<<vbuffer6[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[3]["<<v_buffer_index<<"].value: "<<vbuffer6[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res63.key != udata63.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer6[3]["<<v_buffer_index<<"].key: "<<vbuffer6[3][v_buffer_index].key<<endl; }
				if(res63.value != udata63.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer6[3]["<<v_buffer_index<<"].value: "<<vbuffer6[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata64 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata64 = acts_utilobj->UTIL_GETKV2(vbuffer6[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer6[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata64);
				
				keyvalue_t res64; 
				res64.key = MERGE_applyfunc(udata64.key, vdata64.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res64.value = MERGE_applyfunc(udata64.value, vdata64.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer6[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[4]["<<v_buffer_index<<"].key: "<<vbuffer6[4][v_buffer_index].key<<endl; }
				if(vbuffer6[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[4]["<<v_buffer_index<<"].value: "<<vbuffer6[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[4]["<<v_buffer_index<<"].key: "<<vbuffer6[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[4]["<<v_buffer_index<<"].value: "<<vbuffer6[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res64.key != udata64.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer6[4]["<<v_buffer_index<<"].key: "<<vbuffer6[4][v_buffer_index].key<<endl; }
				if(res64.value != udata64.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer6[4]["<<v_buffer_index<<"].value: "<<vbuffer6[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata65 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata65 = acts_utilobj->UTIL_GETKV2(vbuffer6[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer6[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata65);
				
				keyvalue_t res65; 
				res65.key = MERGE_applyfunc(udata65.key, vdata65.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res65.value = MERGE_applyfunc(udata65.value, vdata65.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer6[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[5]["<<v_buffer_index<<"].key: "<<vbuffer6[5][v_buffer_index].key<<endl; }
				if(vbuffer6[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[5]["<<v_buffer_index<<"].value: "<<vbuffer6[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[5]["<<v_buffer_index<<"].key: "<<vbuffer6[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[5]["<<v_buffer_index<<"].value: "<<vbuffer6[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res65.key != udata65.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer6[5]["<<v_buffer_index<<"].key: "<<vbuffer6[5][v_buffer_index].key<<endl; }
				if(res65.value != udata65.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer6[5]["<<v_buffer_index<<"].value: "<<vbuffer6[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata66 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata66 = acts_utilobj->UTIL_GETKV2(vbuffer6[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer6[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata66);
				
				keyvalue_t res66; 
				res66.key = MERGE_applyfunc(udata66.key, vdata66.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res66.value = MERGE_applyfunc(udata66.value, vdata66.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer6[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[6]["<<v_buffer_index<<"].key: "<<vbuffer6[6][v_buffer_index].key<<endl; }
				if(vbuffer6[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[6]["<<v_buffer_index<<"].value: "<<vbuffer6[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[6]["<<v_buffer_index<<"].key: "<<vbuffer6[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[6]["<<v_buffer_index<<"].value: "<<vbuffer6[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res66.key != udata66.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer6[6]["<<v_buffer_index<<"].key: "<<vbuffer6[6][v_buffer_index].key<<endl; }
				if(res66.value != udata66.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer6[6]["<<v_buffer_index<<"].value: "<<vbuffer6[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata67 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata67 = acts_utilobj->UTIL_GETKV2(vbuffer6[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer6[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata67);
				
				keyvalue_t res67; 
				res67.key = MERGE_applyfunc(udata67.key, vdata67.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res67.value = MERGE_applyfunc(udata67.value, vdata67.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer6[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[7]["<<v_buffer_index<<"].key: "<<vbuffer6[7][v_buffer_index].key<<endl; }
				if(vbuffer6[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[7]["<<v_buffer_index<<"].value: "<<vbuffer6[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[7]["<<v_buffer_index<<"].key: "<<vbuffer6[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer6[7]["<<v_buffer_index<<"].value: "<<vbuffer6[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res67.key != udata67.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer6[7]["<<v_buffer_index<<"].key: "<<vbuffer6[7][v_buffer_index].key<<endl; }
				if(res67.value != udata67.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer6[7]["<<v_buffer_index<<"].value: "<<vbuffer6[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata70 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata70 = acts_utilobj->UTIL_GETKV2(vbuffer7[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer7[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata70);
				
				keyvalue_t res70; 
				res70.key = MERGE_applyfunc(udata70.key, vdata70.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res70.value = MERGE_applyfunc(udata70.value, vdata70.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer7[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[0]["<<v_buffer_index<<"].key: "<<vbuffer7[0][v_buffer_index].key<<endl; }
				if(vbuffer7[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[0]["<<v_buffer_index<<"].value: "<<vbuffer7[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[0]["<<v_buffer_index<<"].key: "<<vbuffer7[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[0]["<<v_buffer_index<<"].value: "<<vbuffer7[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res70.key != udata70.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer7[0]["<<v_buffer_index<<"].key: "<<vbuffer7[0][v_buffer_index].key<<endl; }
				if(res70.value != udata70.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer7[0]["<<v_buffer_index<<"].value: "<<vbuffer7[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata71 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata71 = acts_utilobj->UTIL_GETKV2(vbuffer7[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer7[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata71);
				
				keyvalue_t res71; 
				res71.key = MERGE_applyfunc(udata71.key, vdata71.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res71.value = MERGE_applyfunc(udata71.value, vdata71.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer7[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[1]["<<v_buffer_index<<"].key: "<<vbuffer7[1][v_buffer_index].key<<endl; }
				if(vbuffer7[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[1]["<<v_buffer_index<<"].value: "<<vbuffer7[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[1]["<<v_buffer_index<<"].key: "<<vbuffer7[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[1]["<<v_buffer_index<<"].value: "<<vbuffer7[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res71.key != udata71.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer7[1]["<<v_buffer_index<<"].key: "<<vbuffer7[1][v_buffer_index].key<<endl; }
				if(res71.value != udata71.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer7[1]["<<v_buffer_index<<"].value: "<<vbuffer7[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata72 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata72 = acts_utilobj->UTIL_GETKV2(vbuffer7[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer7[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata72);
				
				keyvalue_t res72; 
				res72.key = MERGE_applyfunc(udata72.key, vdata72.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res72.value = MERGE_applyfunc(udata72.value, vdata72.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer7[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[2]["<<v_buffer_index<<"].key: "<<vbuffer7[2][v_buffer_index].key<<endl; }
				if(vbuffer7[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[2]["<<v_buffer_index<<"].value: "<<vbuffer7[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[2]["<<v_buffer_index<<"].key: "<<vbuffer7[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[2]["<<v_buffer_index<<"].value: "<<vbuffer7[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res72.key != udata72.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer7[2]["<<v_buffer_index<<"].key: "<<vbuffer7[2][v_buffer_index].key<<endl; }
				if(res72.value != udata72.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer7[2]["<<v_buffer_index<<"].value: "<<vbuffer7[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata73 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata73 = acts_utilobj->UTIL_GETKV2(vbuffer7[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer7[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata73);
				
				keyvalue_t res73; 
				res73.key = MERGE_applyfunc(udata73.key, vdata73.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res73.value = MERGE_applyfunc(udata73.value, vdata73.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer7[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[3]["<<v_buffer_index<<"].key: "<<vbuffer7[3][v_buffer_index].key<<endl; }
				if(vbuffer7[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[3]["<<v_buffer_index<<"].value: "<<vbuffer7[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[3]["<<v_buffer_index<<"].key: "<<vbuffer7[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[3]["<<v_buffer_index<<"].value: "<<vbuffer7[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res73.key != udata73.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer7[3]["<<v_buffer_index<<"].key: "<<vbuffer7[3][v_buffer_index].key<<endl; }
				if(res73.value != udata73.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer7[3]["<<v_buffer_index<<"].value: "<<vbuffer7[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata74 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata74 = acts_utilobj->UTIL_GETKV2(vbuffer7[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer7[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata74);
				
				keyvalue_t res74; 
				res74.key = MERGE_applyfunc(udata74.key, vdata74.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res74.value = MERGE_applyfunc(udata74.value, vdata74.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer7[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[4]["<<v_buffer_index<<"].key: "<<vbuffer7[4][v_buffer_index].key<<endl; }
				if(vbuffer7[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[4]["<<v_buffer_index<<"].value: "<<vbuffer7[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[4]["<<v_buffer_index<<"].key: "<<vbuffer7[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[4]["<<v_buffer_index<<"].value: "<<vbuffer7[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res74.key != udata74.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer7[4]["<<v_buffer_index<<"].key: "<<vbuffer7[4][v_buffer_index].key<<endl; }
				if(res74.value != udata74.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer7[4]["<<v_buffer_index<<"].value: "<<vbuffer7[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata75 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata75 = acts_utilobj->UTIL_GETKV2(vbuffer7[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer7[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata75);
				
				keyvalue_t res75; 
				res75.key = MERGE_applyfunc(udata75.key, vdata75.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res75.value = MERGE_applyfunc(udata75.value, vdata75.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer7[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[5]["<<v_buffer_index<<"].key: "<<vbuffer7[5][v_buffer_index].key<<endl; }
				if(vbuffer7[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[5]["<<v_buffer_index<<"].value: "<<vbuffer7[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[5]["<<v_buffer_index<<"].key: "<<vbuffer7[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[5]["<<v_buffer_index<<"].value: "<<vbuffer7[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res75.key != udata75.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer7[5]["<<v_buffer_index<<"].key: "<<vbuffer7[5][v_buffer_index].key<<endl; }
				if(res75.value != udata75.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer7[5]["<<v_buffer_index<<"].value: "<<vbuffer7[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata76 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata76 = acts_utilobj->UTIL_GETKV2(vbuffer7[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer7[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata76);
				
				keyvalue_t res76; 
				res76.key = MERGE_applyfunc(udata76.key, vdata76.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res76.value = MERGE_applyfunc(udata76.value, vdata76.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer7[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[6]["<<v_buffer_index<<"].key: "<<vbuffer7[6][v_buffer_index].key<<endl; }
				if(vbuffer7[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[6]["<<v_buffer_index<<"].value: "<<vbuffer7[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[6]["<<v_buffer_index<<"].key: "<<vbuffer7[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[6]["<<v_buffer_index<<"].value: "<<vbuffer7[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res76.key != udata76.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer7[6]["<<v_buffer_index<<"].key: "<<vbuffer7[6][v_buffer_index].key<<endl; }
				if(res76.value != udata76.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer7[6]["<<v_buffer_index<<"].value: "<<vbuffer7[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata77 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata77 = acts_utilobj->UTIL_GETKV2(vbuffer7[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer7[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata77);
				
				keyvalue_t res77; 
				res77.key = MERGE_applyfunc(udata77.key, vdata77.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res77.value = MERGE_applyfunc(udata77.value, vdata77.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer7[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[7]["<<v_buffer_index<<"].key: "<<vbuffer7[7][v_buffer_index].key<<endl; }
				if(vbuffer7[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[7]["<<v_buffer_index<<"].value: "<<vbuffer7[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[7]["<<v_buffer_index<<"].key: "<<vbuffer7[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer7[7]["<<v_buffer_index<<"].value: "<<vbuffer7[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res77.key != udata77.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer7[7]["<<v_buffer_index<<"].key: "<<vbuffer7[7][v_buffer_index].key<<endl; }
				if(res77.value != udata77.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer7[7]["<<v_buffer_index<<"].value: "<<vbuffer7[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata80 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata80 = acts_utilobj->UTIL_GETKV2(vbuffer8[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer8[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata80);
				
				keyvalue_t res80; 
				res80.key = MERGE_applyfunc(udata80.key, vdata80.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res80.value = MERGE_applyfunc(udata80.value, vdata80.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer8[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[0]["<<v_buffer_index<<"].key: "<<vbuffer8[0][v_buffer_index].key<<endl; }
				if(vbuffer8[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[0]["<<v_buffer_index<<"].value: "<<vbuffer8[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[0]["<<v_buffer_index<<"].key: "<<vbuffer8[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[0]["<<v_buffer_index<<"].value: "<<vbuffer8[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res80.key != udata80.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer8[0]["<<v_buffer_index<<"].key: "<<vbuffer8[0][v_buffer_index].key<<endl; }
				if(res80.value != udata80.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer8[0]["<<v_buffer_index<<"].value: "<<vbuffer8[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata81 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata81 = acts_utilobj->UTIL_GETKV2(vbuffer8[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer8[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata81);
				
				keyvalue_t res81; 
				res81.key = MERGE_applyfunc(udata81.key, vdata81.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res81.value = MERGE_applyfunc(udata81.value, vdata81.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer8[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[1]["<<v_buffer_index<<"].key: "<<vbuffer8[1][v_buffer_index].key<<endl; }
				if(vbuffer8[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[1]["<<v_buffer_index<<"].value: "<<vbuffer8[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[1]["<<v_buffer_index<<"].key: "<<vbuffer8[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[1]["<<v_buffer_index<<"].value: "<<vbuffer8[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res81.key != udata81.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer8[1]["<<v_buffer_index<<"].key: "<<vbuffer8[1][v_buffer_index].key<<endl; }
				if(res81.value != udata81.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer8[1]["<<v_buffer_index<<"].value: "<<vbuffer8[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata82 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata82 = acts_utilobj->UTIL_GETKV2(vbuffer8[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer8[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata82);
				
				keyvalue_t res82; 
				res82.key = MERGE_applyfunc(udata82.key, vdata82.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res82.value = MERGE_applyfunc(udata82.value, vdata82.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer8[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[2]["<<v_buffer_index<<"].key: "<<vbuffer8[2][v_buffer_index].key<<endl; }
				if(vbuffer8[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[2]["<<v_buffer_index<<"].value: "<<vbuffer8[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[2]["<<v_buffer_index<<"].key: "<<vbuffer8[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[2]["<<v_buffer_index<<"].value: "<<vbuffer8[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res82.key != udata82.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer8[2]["<<v_buffer_index<<"].key: "<<vbuffer8[2][v_buffer_index].key<<endl; }
				if(res82.value != udata82.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer8[2]["<<v_buffer_index<<"].value: "<<vbuffer8[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata83 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata83 = acts_utilobj->UTIL_GETKV2(vbuffer8[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer8[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata83);
				
				keyvalue_t res83; 
				res83.key = MERGE_applyfunc(udata83.key, vdata83.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res83.value = MERGE_applyfunc(udata83.value, vdata83.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer8[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[3]["<<v_buffer_index<<"].key: "<<vbuffer8[3][v_buffer_index].key<<endl; }
				if(vbuffer8[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[3]["<<v_buffer_index<<"].value: "<<vbuffer8[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[3]["<<v_buffer_index<<"].key: "<<vbuffer8[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[3]["<<v_buffer_index<<"].value: "<<vbuffer8[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res83.key != udata83.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer8[3]["<<v_buffer_index<<"].key: "<<vbuffer8[3][v_buffer_index].key<<endl; }
				if(res83.value != udata83.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer8[3]["<<v_buffer_index<<"].value: "<<vbuffer8[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata84 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata84 = acts_utilobj->UTIL_GETKV2(vbuffer8[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer8[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata84);
				
				keyvalue_t res84; 
				res84.key = MERGE_applyfunc(udata84.key, vdata84.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res84.value = MERGE_applyfunc(udata84.value, vdata84.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer8[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[4]["<<v_buffer_index<<"].key: "<<vbuffer8[4][v_buffer_index].key<<endl; }
				if(vbuffer8[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[4]["<<v_buffer_index<<"].value: "<<vbuffer8[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[4]["<<v_buffer_index<<"].key: "<<vbuffer8[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[4]["<<v_buffer_index<<"].value: "<<vbuffer8[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res84.key != udata84.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer8[4]["<<v_buffer_index<<"].key: "<<vbuffer8[4][v_buffer_index].key<<endl; }
				if(res84.value != udata84.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer8[4]["<<v_buffer_index<<"].value: "<<vbuffer8[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata85 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata85 = acts_utilobj->UTIL_GETKV2(vbuffer8[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer8[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata85);
				
				keyvalue_t res85; 
				res85.key = MERGE_applyfunc(udata85.key, vdata85.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res85.value = MERGE_applyfunc(udata85.value, vdata85.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer8[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[5]["<<v_buffer_index<<"].key: "<<vbuffer8[5][v_buffer_index].key<<endl; }
				if(vbuffer8[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[5]["<<v_buffer_index<<"].value: "<<vbuffer8[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[5]["<<v_buffer_index<<"].key: "<<vbuffer8[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[5]["<<v_buffer_index<<"].value: "<<vbuffer8[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res85.key != udata85.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer8[5]["<<v_buffer_index<<"].key: "<<vbuffer8[5][v_buffer_index].key<<endl; }
				if(res85.value != udata85.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer8[5]["<<v_buffer_index<<"].value: "<<vbuffer8[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata86 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata86 = acts_utilobj->UTIL_GETKV2(vbuffer8[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer8[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata86);
				
				keyvalue_t res86; 
				res86.key = MERGE_applyfunc(udata86.key, vdata86.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res86.value = MERGE_applyfunc(udata86.value, vdata86.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer8[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[6]["<<v_buffer_index<<"].key: "<<vbuffer8[6][v_buffer_index].key<<endl; }
				if(vbuffer8[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[6]["<<v_buffer_index<<"].value: "<<vbuffer8[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[6]["<<v_buffer_index<<"].key: "<<vbuffer8[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[6]["<<v_buffer_index<<"].value: "<<vbuffer8[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res86.key != udata86.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer8[6]["<<v_buffer_index<<"].key: "<<vbuffer8[6][v_buffer_index].key<<endl; }
				if(res86.value != udata86.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer8[6]["<<v_buffer_index<<"].value: "<<vbuffer8[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata87 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata87 = acts_utilobj->UTIL_GETKV2(vbuffer8[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer8[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata87);
				
				keyvalue_t res87; 
				res87.key = MERGE_applyfunc(udata87.key, vdata87.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res87.value = MERGE_applyfunc(udata87.value, vdata87.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer8[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[7]["<<v_buffer_index<<"].key: "<<vbuffer8[7][v_buffer_index].key<<endl; }
				if(vbuffer8[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[7]["<<v_buffer_index<<"].value: "<<vbuffer8[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[7]["<<v_buffer_index<<"].key: "<<vbuffer8[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer8[7]["<<v_buffer_index<<"].value: "<<vbuffer8[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res87.key != udata87.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer8[7]["<<v_buffer_index<<"].key: "<<vbuffer8[7][v_buffer_index].key<<endl; }
				if(res87.value != udata87.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer8[7]["<<v_buffer_index<<"].value: "<<vbuffer8[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata90 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata90 = acts_utilobj->UTIL_GETKV2(vbuffer9[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer9[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata90);
				
				keyvalue_t res90; 
				res90.key = MERGE_applyfunc(udata90.key, vdata90.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res90.value = MERGE_applyfunc(udata90.value, vdata90.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer9[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[0]["<<v_buffer_index<<"].key: "<<vbuffer9[0][v_buffer_index].key<<endl; }
				if(vbuffer9[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[0]["<<v_buffer_index<<"].value: "<<vbuffer9[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[0]["<<v_buffer_index<<"].key: "<<vbuffer9[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[0]["<<v_buffer_index<<"].value: "<<vbuffer9[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res90.key != udata90.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer9[0]["<<v_buffer_index<<"].key: "<<vbuffer9[0][v_buffer_index].key<<endl; }
				if(res90.value != udata90.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer9[0]["<<v_buffer_index<<"].value: "<<vbuffer9[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata91 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata91 = acts_utilobj->UTIL_GETKV2(vbuffer9[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer9[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata91);
				
				keyvalue_t res91; 
				res91.key = MERGE_applyfunc(udata91.key, vdata91.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res91.value = MERGE_applyfunc(udata91.value, vdata91.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer9[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[1]["<<v_buffer_index<<"].key: "<<vbuffer9[1][v_buffer_index].key<<endl; }
				if(vbuffer9[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[1]["<<v_buffer_index<<"].value: "<<vbuffer9[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[1]["<<v_buffer_index<<"].key: "<<vbuffer9[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[1]["<<v_buffer_index<<"].value: "<<vbuffer9[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res91.key != udata91.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer9[1]["<<v_buffer_index<<"].key: "<<vbuffer9[1][v_buffer_index].key<<endl; }
				if(res91.value != udata91.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer9[1]["<<v_buffer_index<<"].value: "<<vbuffer9[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata92 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata92 = acts_utilobj->UTIL_GETKV2(vbuffer9[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer9[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata92);
				
				keyvalue_t res92; 
				res92.key = MERGE_applyfunc(udata92.key, vdata92.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res92.value = MERGE_applyfunc(udata92.value, vdata92.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer9[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[2]["<<v_buffer_index<<"].key: "<<vbuffer9[2][v_buffer_index].key<<endl; }
				if(vbuffer9[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[2]["<<v_buffer_index<<"].value: "<<vbuffer9[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[2]["<<v_buffer_index<<"].key: "<<vbuffer9[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[2]["<<v_buffer_index<<"].value: "<<vbuffer9[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res92.key != udata92.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer9[2]["<<v_buffer_index<<"].key: "<<vbuffer9[2][v_buffer_index].key<<endl; }
				if(res92.value != udata92.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer9[2]["<<v_buffer_index<<"].value: "<<vbuffer9[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata93 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata93 = acts_utilobj->UTIL_GETKV2(vbuffer9[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer9[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata93);
				
				keyvalue_t res93; 
				res93.key = MERGE_applyfunc(udata93.key, vdata93.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res93.value = MERGE_applyfunc(udata93.value, vdata93.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer9[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[3]["<<v_buffer_index<<"].key: "<<vbuffer9[3][v_buffer_index].key<<endl; }
				if(vbuffer9[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[3]["<<v_buffer_index<<"].value: "<<vbuffer9[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[3]["<<v_buffer_index<<"].key: "<<vbuffer9[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[3]["<<v_buffer_index<<"].value: "<<vbuffer9[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res93.key != udata93.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer9[3]["<<v_buffer_index<<"].key: "<<vbuffer9[3][v_buffer_index].key<<endl; }
				if(res93.value != udata93.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer9[3]["<<v_buffer_index<<"].value: "<<vbuffer9[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata94 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata94 = acts_utilobj->UTIL_GETKV2(vbuffer9[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer9[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata94);
				
				keyvalue_t res94; 
				res94.key = MERGE_applyfunc(udata94.key, vdata94.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res94.value = MERGE_applyfunc(udata94.value, vdata94.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer9[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[4]["<<v_buffer_index<<"].key: "<<vbuffer9[4][v_buffer_index].key<<endl; }
				if(vbuffer9[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[4]["<<v_buffer_index<<"].value: "<<vbuffer9[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[4]["<<v_buffer_index<<"].key: "<<vbuffer9[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[4]["<<v_buffer_index<<"].value: "<<vbuffer9[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res94.key != udata94.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer9[4]["<<v_buffer_index<<"].key: "<<vbuffer9[4][v_buffer_index].key<<endl; }
				if(res94.value != udata94.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer9[4]["<<v_buffer_index<<"].value: "<<vbuffer9[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata95 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata95 = acts_utilobj->UTIL_GETKV2(vbuffer9[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer9[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata95);
				
				keyvalue_t res95; 
				res95.key = MERGE_applyfunc(udata95.key, vdata95.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res95.value = MERGE_applyfunc(udata95.value, vdata95.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer9[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[5]["<<v_buffer_index<<"].key: "<<vbuffer9[5][v_buffer_index].key<<endl; }
				if(vbuffer9[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[5]["<<v_buffer_index<<"].value: "<<vbuffer9[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[5]["<<v_buffer_index<<"].key: "<<vbuffer9[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[5]["<<v_buffer_index<<"].value: "<<vbuffer9[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res95.key != udata95.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer9[5]["<<v_buffer_index<<"].key: "<<vbuffer9[5][v_buffer_index].key<<endl; }
				if(res95.value != udata95.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer9[5]["<<v_buffer_index<<"].value: "<<vbuffer9[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata96 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata96 = acts_utilobj->UTIL_GETKV2(vbuffer9[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer9[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata96);
				
				keyvalue_t res96; 
				res96.key = MERGE_applyfunc(udata96.key, vdata96.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res96.value = MERGE_applyfunc(udata96.value, vdata96.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer9[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[6]["<<v_buffer_index<<"].key: "<<vbuffer9[6][v_buffer_index].key<<endl; }
				if(vbuffer9[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[6]["<<v_buffer_index<<"].value: "<<vbuffer9[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[6]["<<v_buffer_index<<"].key: "<<vbuffer9[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[6]["<<v_buffer_index<<"].value: "<<vbuffer9[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res96.key != udata96.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer9[6]["<<v_buffer_index<<"].key: "<<vbuffer9[6][v_buffer_index].key<<endl; }
				if(res96.value != udata96.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer9[6]["<<v_buffer_index<<"].value: "<<vbuffer9[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata97 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata97 = acts_utilobj->UTIL_GETKV2(vbuffer9[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer9[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata97);
				
				keyvalue_t res97; 
				res97.key = MERGE_applyfunc(udata97.key, vdata97.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res97.value = MERGE_applyfunc(udata97.value, vdata97.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer9[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[7]["<<v_buffer_index<<"].key: "<<vbuffer9[7][v_buffer_index].key<<endl; }
				if(vbuffer9[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[7]["<<v_buffer_index<<"].value: "<<vbuffer9[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[7]["<<v_buffer_index<<"].key: "<<vbuffer9[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer9[7]["<<v_buffer_index<<"].value: "<<vbuffer9[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res97.key != udata97.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer9[7]["<<v_buffer_index<<"].key: "<<vbuffer9[7][v_buffer_index].key<<endl; }
				if(res97.value != udata97.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer9[7]["<<v_buffer_index<<"].value: "<<vbuffer9[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata100 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata100 = acts_utilobj->UTIL_GETKV2(vbuffer10[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer10[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata100);
				
				keyvalue_t res100; 
				res100.key = MERGE_applyfunc(udata100.key, vdata100.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res100.value = MERGE_applyfunc(udata100.value, vdata100.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer10[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[0]["<<v_buffer_index<<"].key: "<<vbuffer10[0][v_buffer_index].key<<endl; }
				if(vbuffer10[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[0]["<<v_buffer_index<<"].value: "<<vbuffer10[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[0]["<<v_buffer_index<<"].key: "<<vbuffer10[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[0]["<<v_buffer_index<<"].value: "<<vbuffer10[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res100.key != udata100.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer10[0]["<<v_buffer_index<<"].key: "<<vbuffer10[0][v_buffer_index].key<<endl; }
				if(res100.value != udata100.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer10[0]["<<v_buffer_index<<"].value: "<<vbuffer10[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata101 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata101 = acts_utilobj->UTIL_GETKV2(vbuffer10[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer10[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata101);
				
				keyvalue_t res101; 
				res101.key = MERGE_applyfunc(udata101.key, vdata101.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res101.value = MERGE_applyfunc(udata101.value, vdata101.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer10[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[1]["<<v_buffer_index<<"].key: "<<vbuffer10[1][v_buffer_index].key<<endl; }
				if(vbuffer10[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[1]["<<v_buffer_index<<"].value: "<<vbuffer10[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[1]["<<v_buffer_index<<"].key: "<<vbuffer10[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[1]["<<v_buffer_index<<"].value: "<<vbuffer10[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res101.key != udata101.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer10[1]["<<v_buffer_index<<"].key: "<<vbuffer10[1][v_buffer_index].key<<endl; }
				if(res101.value != udata101.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer10[1]["<<v_buffer_index<<"].value: "<<vbuffer10[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata102 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata102 = acts_utilobj->UTIL_GETKV2(vbuffer10[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer10[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata102);
				
				keyvalue_t res102; 
				res102.key = MERGE_applyfunc(udata102.key, vdata102.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res102.value = MERGE_applyfunc(udata102.value, vdata102.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer10[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[2]["<<v_buffer_index<<"].key: "<<vbuffer10[2][v_buffer_index].key<<endl; }
				if(vbuffer10[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[2]["<<v_buffer_index<<"].value: "<<vbuffer10[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[2]["<<v_buffer_index<<"].key: "<<vbuffer10[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[2]["<<v_buffer_index<<"].value: "<<vbuffer10[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res102.key != udata102.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer10[2]["<<v_buffer_index<<"].key: "<<vbuffer10[2][v_buffer_index].key<<endl; }
				if(res102.value != udata102.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer10[2]["<<v_buffer_index<<"].value: "<<vbuffer10[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata103 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata103 = acts_utilobj->UTIL_GETKV2(vbuffer10[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer10[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata103);
				
				keyvalue_t res103; 
				res103.key = MERGE_applyfunc(udata103.key, vdata103.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res103.value = MERGE_applyfunc(udata103.value, vdata103.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer10[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[3]["<<v_buffer_index<<"].key: "<<vbuffer10[3][v_buffer_index].key<<endl; }
				if(vbuffer10[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[3]["<<v_buffer_index<<"].value: "<<vbuffer10[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[3]["<<v_buffer_index<<"].key: "<<vbuffer10[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[3]["<<v_buffer_index<<"].value: "<<vbuffer10[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res103.key != udata103.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer10[3]["<<v_buffer_index<<"].key: "<<vbuffer10[3][v_buffer_index].key<<endl; }
				if(res103.value != udata103.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer10[3]["<<v_buffer_index<<"].value: "<<vbuffer10[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata104 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata104 = acts_utilobj->UTIL_GETKV2(vbuffer10[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer10[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata104);
				
				keyvalue_t res104; 
				res104.key = MERGE_applyfunc(udata104.key, vdata104.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res104.value = MERGE_applyfunc(udata104.value, vdata104.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer10[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[4]["<<v_buffer_index<<"].key: "<<vbuffer10[4][v_buffer_index].key<<endl; }
				if(vbuffer10[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[4]["<<v_buffer_index<<"].value: "<<vbuffer10[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[4]["<<v_buffer_index<<"].key: "<<vbuffer10[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[4]["<<v_buffer_index<<"].value: "<<vbuffer10[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res104.key != udata104.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer10[4]["<<v_buffer_index<<"].key: "<<vbuffer10[4][v_buffer_index].key<<endl; }
				if(res104.value != udata104.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer10[4]["<<v_buffer_index<<"].value: "<<vbuffer10[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata105 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata105 = acts_utilobj->UTIL_GETKV2(vbuffer10[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer10[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata105);
				
				keyvalue_t res105; 
				res105.key = MERGE_applyfunc(udata105.key, vdata105.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res105.value = MERGE_applyfunc(udata105.value, vdata105.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer10[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[5]["<<v_buffer_index<<"].key: "<<vbuffer10[5][v_buffer_index].key<<endl; }
				if(vbuffer10[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[5]["<<v_buffer_index<<"].value: "<<vbuffer10[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[5]["<<v_buffer_index<<"].key: "<<vbuffer10[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[5]["<<v_buffer_index<<"].value: "<<vbuffer10[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res105.key != udata105.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer10[5]["<<v_buffer_index<<"].key: "<<vbuffer10[5][v_buffer_index].key<<endl; }
				if(res105.value != udata105.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer10[5]["<<v_buffer_index<<"].value: "<<vbuffer10[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata106 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata106 = acts_utilobj->UTIL_GETKV2(vbuffer10[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer10[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata106);
				
				keyvalue_t res106; 
				res106.key = MERGE_applyfunc(udata106.key, vdata106.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res106.value = MERGE_applyfunc(udata106.value, vdata106.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer10[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[6]["<<v_buffer_index<<"].key: "<<vbuffer10[6][v_buffer_index].key<<endl; }
				if(vbuffer10[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[6]["<<v_buffer_index<<"].value: "<<vbuffer10[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[6]["<<v_buffer_index<<"].key: "<<vbuffer10[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[6]["<<v_buffer_index<<"].value: "<<vbuffer10[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res106.key != udata106.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer10[6]["<<v_buffer_index<<"].key: "<<vbuffer10[6][v_buffer_index].key<<endl; }
				if(res106.value != udata106.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer10[6]["<<v_buffer_index<<"].value: "<<vbuffer10[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata107 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata107 = acts_utilobj->UTIL_GETKV2(vbuffer10[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer10[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata107);
				
				keyvalue_t res107; 
				res107.key = MERGE_applyfunc(udata107.key, vdata107.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res107.value = MERGE_applyfunc(udata107.value, vdata107.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer10[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[7]["<<v_buffer_index<<"].key: "<<vbuffer10[7][v_buffer_index].key<<endl; }
				if(vbuffer10[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[7]["<<v_buffer_index<<"].value: "<<vbuffer10[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[7]["<<v_buffer_index<<"].key: "<<vbuffer10[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer10[7]["<<v_buffer_index<<"].value: "<<vbuffer10[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res107.key != udata107.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer10[7]["<<v_buffer_index<<"].key: "<<vbuffer10[7][v_buffer_index].key<<endl; }
				if(res107.value != udata107.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer10[7]["<<v_buffer_index<<"].value: "<<vbuffer10[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata110 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata110 = acts_utilobj->UTIL_GETKV2(vbuffer11[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer11[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata110);
				
				keyvalue_t res110; 
				res110.key = MERGE_applyfunc(udata110.key, vdata110.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res110.value = MERGE_applyfunc(udata110.value, vdata110.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer11[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[0]["<<v_buffer_index<<"].key: "<<vbuffer11[0][v_buffer_index].key<<endl; }
				if(vbuffer11[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[0]["<<v_buffer_index<<"].value: "<<vbuffer11[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[0]["<<v_buffer_index<<"].key: "<<vbuffer11[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[0]["<<v_buffer_index<<"].value: "<<vbuffer11[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res110.key != udata110.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer11[0]["<<v_buffer_index<<"].key: "<<vbuffer11[0][v_buffer_index].key<<endl; }
				if(res110.value != udata110.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer11[0]["<<v_buffer_index<<"].value: "<<vbuffer11[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata111 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata111 = acts_utilobj->UTIL_GETKV2(vbuffer11[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer11[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata111);
				
				keyvalue_t res111; 
				res111.key = MERGE_applyfunc(udata111.key, vdata111.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res111.value = MERGE_applyfunc(udata111.value, vdata111.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer11[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[1]["<<v_buffer_index<<"].key: "<<vbuffer11[1][v_buffer_index].key<<endl; }
				if(vbuffer11[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[1]["<<v_buffer_index<<"].value: "<<vbuffer11[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[1]["<<v_buffer_index<<"].key: "<<vbuffer11[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[1]["<<v_buffer_index<<"].value: "<<vbuffer11[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res111.key != udata111.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer11[1]["<<v_buffer_index<<"].key: "<<vbuffer11[1][v_buffer_index].key<<endl; }
				if(res111.value != udata111.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer11[1]["<<v_buffer_index<<"].value: "<<vbuffer11[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata112 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata112 = acts_utilobj->UTIL_GETKV2(vbuffer11[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer11[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata112);
				
				keyvalue_t res112; 
				res112.key = MERGE_applyfunc(udata112.key, vdata112.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res112.value = MERGE_applyfunc(udata112.value, vdata112.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer11[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[2]["<<v_buffer_index<<"].key: "<<vbuffer11[2][v_buffer_index].key<<endl; }
				if(vbuffer11[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[2]["<<v_buffer_index<<"].value: "<<vbuffer11[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[2]["<<v_buffer_index<<"].key: "<<vbuffer11[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[2]["<<v_buffer_index<<"].value: "<<vbuffer11[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res112.key != udata112.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer11[2]["<<v_buffer_index<<"].key: "<<vbuffer11[2][v_buffer_index].key<<endl; }
				if(res112.value != udata112.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer11[2]["<<v_buffer_index<<"].value: "<<vbuffer11[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata113 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata113 = acts_utilobj->UTIL_GETKV2(vbuffer11[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer11[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata113);
				
				keyvalue_t res113; 
				res113.key = MERGE_applyfunc(udata113.key, vdata113.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res113.value = MERGE_applyfunc(udata113.value, vdata113.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer11[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[3]["<<v_buffer_index<<"].key: "<<vbuffer11[3][v_buffer_index].key<<endl; }
				if(vbuffer11[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[3]["<<v_buffer_index<<"].value: "<<vbuffer11[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[3]["<<v_buffer_index<<"].key: "<<vbuffer11[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[3]["<<v_buffer_index<<"].value: "<<vbuffer11[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res113.key != udata113.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer11[3]["<<v_buffer_index<<"].key: "<<vbuffer11[3][v_buffer_index].key<<endl; }
				if(res113.value != udata113.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer11[3]["<<v_buffer_index<<"].value: "<<vbuffer11[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata114 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata114 = acts_utilobj->UTIL_GETKV2(vbuffer11[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer11[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata114);
				
				keyvalue_t res114; 
				res114.key = MERGE_applyfunc(udata114.key, vdata114.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res114.value = MERGE_applyfunc(udata114.value, vdata114.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer11[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[4]["<<v_buffer_index<<"].key: "<<vbuffer11[4][v_buffer_index].key<<endl; }
				if(vbuffer11[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[4]["<<v_buffer_index<<"].value: "<<vbuffer11[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[4]["<<v_buffer_index<<"].key: "<<vbuffer11[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[4]["<<v_buffer_index<<"].value: "<<vbuffer11[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res114.key != udata114.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer11[4]["<<v_buffer_index<<"].key: "<<vbuffer11[4][v_buffer_index].key<<endl; }
				if(res114.value != udata114.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer11[4]["<<v_buffer_index<<"].value: "<<vbuffer11[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata115 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata115 = acts_utilobj->UTIL_GETKV2(vbuffer11[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer11[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata115);
				
				keyvalue_t res115; 
				res115.key = MERGE_applyfunc(udata115.key, vdata115.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res115.value = MERGE_applyfunc(udata115.value, vdata115.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer11[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[5]["<<v_buffer_index<<"].key: "<<vbuffer11[5][v_buffer_index].key<<endl; }
				if(vbuffer11[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[5]["<<v_buffer_index<<"].value: "<<vbuffer11[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[5]["<<v_buffer_index<<"].key: "<<vbuffer11[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[5]["<<v_buffer_index<<"].value: "<<vbuffer11[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res115.key != udata115.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer11[5]["<<v_buffer_index<<"].key: "<<vbuffer11[5][v_buffer_index].key<<endl; }
				if(res115.value != udata115.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer11[5]["<<v_buffer_index<<"].value: "<<vbuffer11[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata116 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata116 = acts_utilobj->UTIL_GETKV2(vbuffer11[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer11[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata116);
				
				keyvalue_t res116; 
				res116.key = MERGE_applyfunc(udata116.key, vdata116.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res116.value = MERGE_applyfunc(udata116.value, vdata116.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer11[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[6]["<<v_buffer_index<<"].key: "<<vbuffer11[6][v_buffer_index].key<<endl; }
				if(vbuffer11[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[6]["<<v_buffer_index<<"].value: "<<vbuffer11[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[6]["<<v_buffer_index<<"].key: "<<vbuffer11[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[6]["<<v_buffer_index<<"].value: "<<vbuffer11[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res116.key != udata116.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer11[6]["<<v_buffer_index<<"].key: "<<vbuffer11[6][v_buffer_index].key<<endl; }
				if(res116.value != udata116.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer11[6]["<<v_buffer_index<<"].value: "<<vbuffer11[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata117 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata117 = acts_utilobj->UTIL_GETKV2(vbuffer11[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer11[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata117);
				
				keyvalue_t res117; 
				res117.key = MERGE_applyfunc(udata117.key, vdata117.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res117.value = MERGE_applyfunc(udata117.value, vdata117.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer11[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[7]["<<v_buffer_index<<"].key: "<<vbuffer11[7][v_buffer_index].key<<endl; }
				if(vbuffer11[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[7]["<<v_buffer_index<<"].value: "<<vbuffer11[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[7]["<<v_buffer_index<<"].key: "<<vbuffer11[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer11[7]["<<v_buffer_index<<"].value: "<<vbuffer11[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res117.key != udata117.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer11[7]["<<v_buffer_index<<"].key: "<<vbuffer11[7][v_buffer_index].key<<endl; }
				if(res117.value != udata117.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer11[7]["<<v_buffer_index<<"].value: "<<vbuffer11[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata120 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata120 = acts_utilobj->UTIL_GETKV2(vbuffer12[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer12[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata120);
				
				keyvalue_t res120; 
				res120.key = MERGE_applyfunc(udata120.key, vdata120.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res120.value = MERGE_applyfunc(udata120.value, vdata120.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer12[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[0]["<<v_buffer_index<<"].key: "<<vbuffer12[0][v_buffer_index].key<<endl; }
				if(vbuffer12[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[0]["<<v_buffer_index<<"].value: "<<vbuffer12[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[0]["<<v_buffer_index<<"].key: "<<vbuffer12[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[0]["<<v_buffer_index<<"].value: "<<vbuffer12[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res120.key != udata120.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer12[0]["<<v_buffer_index<<"].key: "<<vbuffer12[0][v_buffer_index].key<<endl; }
				if(res120.value != udata120.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer12[0]["<<v_buffer_index<<"].value: "<<vbuffer12[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata121 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata121 = acts_utilobj->UTIL_GETKV2(vbuffer12[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer12[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata121);
				
				keyvalue_t res121; 
				res121.key = MERGE_applyfunc(udata121.key, vdata121.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res121.value = MERGE_applyfunc(udata121.value, vdata121.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer12[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[1]["<<v_buffer_index<<"].key: "<<vbuffer12[1][v_buffer_index].key<<endl; }
				if(vbuffer12[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[1]["<<v_buffer_index<<"].value: "<<vbuffer12[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[1]["<<v_buffer_index<<"].key: "<<vbuffer12[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[1]["<<v_buffer_index<<"].value: "<<vbuffer12[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res121.key != udata121.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer12[1]["<<v_buffer_index<<"].key: "<<vbuffer12[1][v_buffer_index].key<<endl; }
				if(res121.value != udata121.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer12[1]["<<v_buffer_index<<"].value: "<<vbuffer12[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata122 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata122 = acts_utilobj->UTIL_GETKV2(vbuffer12[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer12[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata122);
				
				keyvalue_t res122; 
				res122.key = MERGE_applyfunc(udata122.key, vdata122.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res122.value = MERGE_applyfunc(udata122.value, vdata122.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer12[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[2]["<<v_buffer_index<<"].key: "<<vbuffer12[2][v_buffer_index].key<<endl; }
				if(vbuffer12[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[2]["<<v_buffer_index<<"].value: "<<vbuffer12[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[2]["<<v_buffer_index<<"].key: "<<vbuffer12[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[2]["<<v_buffer_index<<"].value: "<<vbuffer12[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res122.key != udata122.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer12[2]["<<v_buffer_index<<"].key: "<<vbuffer12[2][v_buffer_index].key<<endl; }
				if(res122.value != udata122.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer12[2]["<<v_buffer_index<<"].value: "<<vbuffer12[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata123 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata123 = acts_utilobj->UTIL_GETKV2(vbuffer12[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer12[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata123);
				
				keyvalue_t res123; 
				res123.key = MERGE_applyfunc(udata123.key, vdata123.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res123.value = MERGE_applyfunc(udata123.value, vdata123.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer12[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[3]["<<v_buffer_index<<"].key: "<<vbuffer12[3][v_buffer_index].key<<endl; }
				if(vbuffer12[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[3]["<<v_buffer_index<<"].value: "<<vbuffer12[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[3]["<<v_buffer_index<<"].key: "<<vbuffer12[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[3]["<<v_buffer_index<<"].value: "<<vbuffer12[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res123.key != udata123.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer12[3]["<<v_buffer_index<<"].key: "<<vbuffer12[3][v_buffer_index].key<<endl; }
				if(res123.value != udata123.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer12[3]["<<v_buffer_index<<"].value: "<<vbuffer12[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata124 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata124 = acts_utilobj->UTIL_GETKV2(vbuffer12[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer12[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata124);
				
				keyvalue_t res124; 
				res124.key = MERGE_applyfunc(udata124.key, vdata124.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res124.value = MERGE_applyfunc(udata124.value, vdata124.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer12[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[4]["<<v_buffer_index<<"].key: "<<vbuffer12[4][v_buffer_index].key<<endl; }
				if(vbuffer12[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[4]["<<v_buffer_index<<"].value: "<<vbuffer12[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[4]["<<v_buffer_index<<"].key: "<<vbuffer12[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[4]["<<v_buffer_index<<"].value: "<<vbuffer12[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res124.key != udata124.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer12[4]["<<v_buffer_index<<"].key: "<<vbuffer12[4][v_buffer_index].key<<endl; }
				if(res124.value != udata124.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer12[4]["<<v_buffer_index<<"].value: "<<vbuffer12[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata125 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata125 = acts_utilobj->UTIL_GETKV2(vbuffer12[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer12[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata125);
				
				keyvalue_t res125; 
				res125.key = MERGE_applyfunc(udata125.key, vdata125.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res125.value = MERGE_applyfunc(udata125.value, vdata125.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer12[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[5]["<<v_buffer_index<<"].key: "<<vbuffer12[5][v_buffer_index].key<<endl; }
				if(vbuffer12[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[5]["<<v_buffer_index<<"].value: "<<vbuffer12[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[5]["<<v_buffer_index<<"].key: "<<vbuffer12[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[5]["<<v_buffer_index<<"].value: "<<vbuffer12[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res125.key != udata125.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer12[5]["<<v_buffer_index<<"].key: "<<vbuffer12[5][v_buffer_index].key<<endl; }
				if(res125.value != udata125.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer12[5]["<<v_buffer_index<<"].value: "<<vbuffer12[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata126 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata126 = acts_utilobj->UTIL_GETKV2(vbuffer12[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer12[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata126);
				
				keyvalue_t res126; 
				res126.key = MERGE_applyfunc(udata126.key, vdata126.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res126.value = MERGE_applyfunc(udata126.value, vdata126.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer12[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[6]["<<v_buffer_index<<"].key: "<<vbuffer12[6][v_buffer_index].key<<endl; }
				if(vbuffer12[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[6]["<<v_buffer_index<<"].value: "<<vbuffer12[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[6]["<<v_buffer_index<<"].key: "<<vbuffer12[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[6]["<<v_buffer_index<<"].value: "<<vbuffer12[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res126.key != udata126.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer12[6]["<<v_buffer_index<<"].key: "<<vbuffer12[6][v_buffer_index].key<<endl; }
				if(res126.value != udata126.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer12[6]["<<v_buffer_index<<"].value: "<<vbuffer12[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata127 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata127 = acts_utilobj->UTIL_GETKV2(vbuffer12[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer12[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata127);
				
				keyvalue_t res127; 
				res127.key = MERGE_applyfunc(udata127.key, vdata127.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res127.value = MERGE_applyfunc(udata127.value, vdata127.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer12[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[7]["<<v_buffer_index<<"].key: "<<vbuffer12[7][v_buffer_index].key<<endl; }
				if(vbuffer12[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[7]["<<v_buffer_index<<"].value: "<<vbuffer12[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[7]["<<v_buffer_index<<"].key: "<<vbuffer12[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer12[7]["<<v_buffer_index<<"].value: "<<vbuffer12[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res127.key != udata127.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer12[7]["<<v_buffer_index<<"].key: "<<vbuffer12[7][v_buffer_index].key<<endl; }
				if(res127.value != udata127.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer12[7]["<<v_buffer_index<<"].value: "<<vbuffer12[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata130 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata130 = acts_utilobj->UTIL_GETKV2(vbuffer13[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer13[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata130);
				
				keyvalue_t res130; 
				res130.key = MERGE_applyfunc(udata130.key, vdata130.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res130.value = MERGE_applyfunc(udata130.value, vdata130.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer13[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[0]["<<v_buffer_index<<"].key: "<<vbuffer13[0][v_buffer_index].key<<endl; }
				if(vbuffer13[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[0]["<<v_buffer_index<<"].value: "<<vbuffer13[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[0]["<<v_buffer_index<<"].key: "<<vbuffer13[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[0]["<<v_buffer_index<<"].value: "<<vbuffer13[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res130.key != udata130.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer13[0]["<<v_buffer_index<<"].key: "<<vbuffer13[0][v_buffer_index].key<<endl; }
				if(res130.value != udata130.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer13[0]["<<v_buffer_index<<"].value: "<<vbuffer13[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata131 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata131 = acts_utilobj->UTIL_GETKV2(vbuffer13[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer13[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata131);
				
				keyvalue_t res131; 
				res131.key = MERGE_applyfunc(udata131.key, vdata131.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res131.value = MERGE_applyfunc(udata131.value, vdata131.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer13[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[1]["<<v_buffer_index<<"].key: "<<vbuffer13[1][v_buffer_index].key<<endl; }
				if(vbuffer13[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[1]["<<v_buffer_index<<"].value: "<<vbuffer13[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[1]["<<v_buffer_index<<"].key: "<<vbuffer13[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[1]["<<v_buffer_index<<"].value: "<<vbuffer13[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res131.key != udata131.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer13[1]["<<v_buffer_index<<"].key: "<<vbuffer13[1][v_buffer_index].key<<endl; }
				if(res131.value != udata131.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer13[1]["<<v_buffer_index<<"].value: "<<vbuffer13[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata132 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata132 = acts_utilobj->UTIL_GETKV2(vbuffer13[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer13[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata132);
				
				keyvalue_t res132; 
				res132.key = MERGE_applyfunc(udata132.key, vdata132.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res132.value = MERGE_applyfunc(udata132.value, vdata132.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer13[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[2]["<<v_buffer_index<<"].key: "<<vbuffer13[2][v_buffer_index].key<<endl; }
				if(vbuffer13[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[2]["<<v_buffer_index<<"].value: "<<vbuffer13[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[2]["<<v_buffer_index<<"].key: "<<vbuffer13[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[2]["<<v_buffer_index<<"].value: "<<vbuffer13[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res132.key != udata132.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer13[2]["<<v_buffer_index<<"].key: "<<vbuffer13[2][v_buffer_index].key<<endl; }
				if(res132.value != udata132.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer13[2]["<<v_buffer_index<<"].value: "<<vbuffer13[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata133 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata133 = acts_utilobj->UTIL_GETKV2(vbuffer13[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer13[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata133);
				
				keyvalue_t res133; 
				res133.key = MERGE_applyfunc(udata133.key, vdata133.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res133.value = MERGE_applyfunc(udata133.value, vdata133.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer13[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[3]["<<v_buffer_index<<"].key: "<<vbuffer13[3][v_buffer_index].key<<endl; }
				if(vbuffer13[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[3]["<<v_buffer_index<<"].value: "<<vbuffer13[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[3]["<<v_buffer_index<<"].key: "<<vbuffer13[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[3]["<<v_buffer_index<<"].value: "<<vbuffer13[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res133.key != udata133.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer13[3]["<<v_buffer_index<<"].key: "<<vbuffer13[3][v_buffer_index].key<<endl; }
				if(res133.value != udata133.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer13[3]["<<v_buffer_index<<"].value: "<<vbuffer13[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata134 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata134 = acts_utilobj->UTIL_GETKV2(vbuffer13[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer13[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata134);
				
				keyvalue_t res134; 
				res134.key = MERGE_applyfunc(udata134.key, vdata134.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res134.value = MERGE_applyfunc(udata134.value, vdata134.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer13[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[4]["<<v_buffer_index<<"].key: "<<vbuffer13[4][v_buffer_index].key<<endl; }
				if(vbuffer13[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[4]["<<v_buffer_index<<"].value: "<<vbuffer13[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[4]["<<v_buffer_index<<"].key: "<<vbuffer13[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[4]["<<v_buffer_index<<"].value: "<<vbuffer13[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res134.key != udata134.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer13[4]["<<v_buffer_index<<"].key: "<<vbuffer13[4][v_buffer_index].key<<endl; }
				if(res134.value != udata134.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer13[4]["<<v_buffer_index<<"].value: "<<vbuffer13[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata135 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata135 = acts_utilobj->UTIL_GETKV2(vbuffer13[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer13[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata135);
				
				keyvalue_t res135; 
				res135.key = MERGE_applyfunc(udata135.key, vdata135.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res135.value = MERGE_applyfunc(udata135.value, vdata135.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer13[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[5]["<<v_buffer_index<<"].key: "<<vbuffer13[5][v_buffer_index].key<<endl; }
				if(vbuffer13[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[5]["<<v_buffer_index<<"].value: "<<vbuffer13[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[5]["<<v_buffer_index<<"].key: "<<vbuffer13[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[5]["<<v_buffer_index<<"].value: "<<vbuffer13[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res135.key != udata135.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer13[5]["<<v_buffer_index<<"].key: "<<vbuffer13[5][v_buffer_index].key<<endl; }
				if(res135.value != udata135.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer13[5]["<<v_buffer_index<<"].value: "<<vbuffer13[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata136 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata136 = acts_utilobj->UTIL_GETKV2(vbuffer13[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer13[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata136);
				
				keyvalue_t res136; 
				res136.key = MERGE_applyfunc(udata136.key, vdata136.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res136.value = MERGE_applyfunc(udata136.value, vdata136.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer13[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[6]["<<v_buffer_index<<"].key: "<<vbuffer13[6][v_buffer_index].key<<endl; }
				if(vbuffer13[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[6]["<<v_buffer_index<<"].value: "<<vbuffer13[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[6]["<<v_buffer_index<<"].key: "<<vbuffer13[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[6]["<<v_buffer_index<<"].value: "<<vbuffer13[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res136.key != udata136.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer13[6]["<<v_buffer_index<<"].key: "<<vbuffer13[6][v_buffer_index].key<<endl; }
				if(res136.value != udata136.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer13[6]["<<v_buffer_index<<"].value: "<<vbuffer13[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata137 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata137 = acts_utilobj->UTIL_GETKV2(vbuffer13[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer13[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata137);
				
				keyvalue_t res137; 
				res137.key = MERGE_applyfunc(udata137.key, vdata137.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res137.value = MERGE_applyfunc(udata137.value, vdata137.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer13[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[7]["<<v_buffer_index<<"].key: "<<vbuffer13[7][v_buffer_index].key<<endl; }
				if(vbuffer13[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[7]["<<v_buffer_index<<"].value: "<<vbuffer13[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[7]["<<v_buffer_index<<"].key: "<<vbuffer13[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer13[7]["<<v_buffer_index<<"].value: "<<vbuffer13[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res137.key != udata137.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer13[7]["<<v_buffer_index<<"].key: "<<vbuffer13[7][v_buffer_index].key<<endl; }
				if(res137.value != udata137.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer13[7]["<<v_buffer_index<<"].value: "<<vbuffer13[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata140 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata140 = acts_utilobj->UTIL_GETKV2(vbuffer14[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer14[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata140);
				
				keyvalue_t res140; 
				res140.key = MERGE_applyfunc(udata140.key, vdata140.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res140.value = MERGE_applyfunc(udata140.value, vdata140.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer14[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[0]["<<v_buffer_index<<"].key: "<<vbuffer14[0][v_buffer_index].key<<endl; }
				if(vbuffer14[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[0]["<<v_buffer_index<<"].value: "<<vbuffer14[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[0]["<<v_buffer_index<<"].key: "<<vbuffer14[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[0]["<<v_buffer_index<<"].value: "<<vbuffer14[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res140.key != udata140.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer14[0]["<<v_buffer_index<<"].key: "<<vbuffer14[0][v_buffer_index].key<<endl; }
				if(res140.value != udata140.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer14[0]["<<v_buffer_index<<"].value: "<<vbuffer14[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata141 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata141 = acts_utilobj->UTIL_GETKV2(vbuffer14[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer14[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata141);
				
				keyvalue_t res141; 
				res141.key = MERGE_applyfunc(udata141.key, vdata141.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res141.value = MERGE_applyfunc(udata141.value, vdata141.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer14[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[1]["<<v_buffer_index<<"].key: "<<vbuffer14[1][v_buffer_index].key<<endl; }
				if(vbuffer14[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[1]["<<v_buffer_index<<"].value: "<<vbuffer14[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[1]["<<v_buffer_index<<"].key: "<<vbuffer14[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[1]["<<v_buffer_index<<"].value: "<<vbuffer14[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res141.key != udata141.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer14[1]["<<v_buffer_index<<"].key: "<<vbuffer14[1][v_buffer_index].key<<endl; }
				if(res141.value != udata141.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer14[1]["<<v_buffer_index<<"].value: "<<vbuffer14[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata142 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata142 = acts_utilobj->UTIL_GETKV2(vbuffer14[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer14[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata142);
				
				keyvalue_t res142; 
				res142.key = MERGE_applyfunc(udata142.key, vdata142.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res142.value = MERGE_applyfunc(udata142.value, vdata142.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer14[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[2]["<<v_buffer_index<<"].key: "<<vbuffer14[2][v_buffer_index].key<<endl; }
				if(vbuffer14[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[2]["<<v_buffer_index<<"].value: "<<vbuffer14[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[2]["<<v_buffer_index<<"].key: "<<vbuffer14[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[2]["<<v_buffer_index<<"].value: "<<vbuffer14[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res142.key != udata142.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer14[2]["<<v_buffer_index<<"].key: "<<vbuffer14[2][v_buffer_index].key<<endl; }
				if(res142.value != udata142.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer14[2]["<<v_buffer_index<<"].value: "<<vbuffer14[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata143 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata143 = acts_utilobj->UTIL_GETKV2(vbuffer14[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer14[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata143);
				
				keyvalue_t res143; 
				res143.key = MERGE_applyfunc(udata143.key, vdata143.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res143.value = MERGE_applyfunc(udata143.value, vdata143.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer14[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[3]["<<v_buffer_index<<"].key: "<<vbuffer14[3][v_buffer_index].key<<endl; }
				if(vbuffer14[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[3]["<<v_buffer_index<<"].value: "<<vbuffer14[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[3]["<<v_buffer_index<<"].key: "<<vbuffer14[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[3]["<<v_buffer_index<<"].value: "<<vbuffer14[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res143.key != udata143.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer14[3]["<<v_buffer_index<<"].key: "<<vbuffer14[3][v_buffer_index].key<<endl; }
				if(res143.value != udata143.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer14[3]["<<v_buffer_index<<"].value: "<<vbuffer14[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata144 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata144 = acts_utilobj->UTIL_GETKV2(vbuffer14[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer14[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata144);
				
				keyvalue_t res144; 
				res144.key = MERGE_applyfunc(udata144.key, vdata144.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res144.value = MERGE_applyfunc(udata144.value, vdata144.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer14[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[4]["<<v_buffer_index<<"].key: "<<vbuffer14[4][v_buffer_index].key<<endl; }
				if(vbuffer14[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[4]["<<v_buffer_index<<"].value: "<<vbuffer14[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[4]["<<v_buffer_index<<"].key: "<<vbuffer14[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[4]["<<v_buffer_index<<"].value: "<<vbuffer14[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res144.key != udata144.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer14[4]["<<v_buffer_index<<"].key: "<<vbuffer14[4][v_buffer_index].key<<endl; }
				if(res144.value != udata144.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer14[4]["<<v_buffer_index<<"].value: "<<vbuffer14[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata145 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata145 = acts_utilobj->UTIL_GETKV2(vbuffer14[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer14[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata145);
				
				keyvalue_t res145; 
				res145.key = MERGE_applyfunc(udata145.key, vdata145.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res145.value = MERGE_applyfunc(udata145.value, vdata145.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer14[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[5]["<<v_buffer_index<<"].key: "<<vbuffer14[5][v_buffer_index].key<<endl; }
				if(vbuffer14[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[5]["<<v_buffer_index<<"].value: "<<vbuffer14[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[5]["<<v_buffer_index<<"].key: "<<vbuffer14[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[5]["<<v_buffer_index<<"].value: "<<vbuffer14[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res145.key != udata145.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer14[5]["<<v_buffer_index<<"].key: "<<vbuffer14[5][v_buffer_index].key<<endl; }
				if(res145.value != udata145.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer14[5]["<<v_buffer_index<<"].value: "<<vbuffer14[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata146 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata146 = acts_utilobj->UTIL_GETKV2(vbuffer14[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer14[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata146);
				
				keyvalue_t res146; 
				res146.key = MERGE_applyfunc(udata146.key, vdata146.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res146.value = MERGE_applyfunc(udata146.value, vdata146.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer14[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[6]["<<v_buffer_index<<"].key: "<<vbuffer14[6][v_buffer_index].key<<endl; }
				if(vbuffer14[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[6]["<<v_buffer_index<<"].value: "<<vbuffer14[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[6]["<<v_buffer_index<<"].key: "<<vbuffer14[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[6]["<<v_buffer_index<<"].value: "<<vbuffer14[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res146.key != udata146.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer14[6]["<<v_buffer_index<<"].key: "<<vbuffer14[6][v_buffer_index].key<<endl; }
				if(res146.value != udata146.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer14[6]["<<v_buffer_index<<"].value: "<<vbuffer14[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata147 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata147 = acts_utilobj->UTIL_GETKV2(vbuffer14[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer14[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata147);
				
				keyvalue_t res147; 
				res147.key = MERGE_applyfunc(udata147.key, vdata147.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res147.value = MERGE_applyfunc(udata147.value, vdata147.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer14[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[7]["<<v_buffer_index<<"].key: "<<vbuffer14[7][v_buffer_index].key<<endl; }
				if(vbuffer14[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[7]["<<v_buffer_index<<"].value: "<<vbuffer14[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[7]["<<v_buffer_index<<"].key: "<<vbuffer14[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer14[7]["<<v_buffer_index<<"].value: "<<vbuffer14[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res147.key != udata147.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer14[7]["<<v_buffer_index<<"].key: "<<vbuffer14[7][v_buffer_index].key<<endl; }
				if(res147.value != udata147.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer14[7]["<<v_buffer_index<<"].value: "<<vbuffer14[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
 
				keyvalue_t udata150 = acts_utilobj->UTIL_GETKV2(refbuffer[0][merge_buffer_index]);
				keyvalue_t vdata150 = acts_utilobj->UTIL_GETKV2(vbuffer15[0][v_buffer_index]);
				
				// merge_buffer[0][merge_buffer_index] = vbuffer15[0][v_buffer_index];	 
				merge_buffer[0][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata150);
				
				keyvalue_t res150; 
				res150.key = MERGE_applyfunc(udata150.key, vdata150.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res150.value = MERGE_applyfunc(udata150.value, vdata150.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer15[0][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[0]["<<v_buffer_index<<"].key: "<<vbuffer15[0][v_buffer_index].key<<endl; }
				if(vbuffer15[0][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[0]["<<v_buffer_index<<"].value: "<<vbuffer15[0][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res0.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[0]["<<v_buffer_index<<"].key: "<<vbuffer15[0][v_buffer_index].key<<endl; }
				if(res0.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[0]["<<v_buffer_index<<"].value: "<<vbuffer15[0][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res150.key != udata150.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer15[0]["<<v_buffer_index<<"].key: "<<vbuffer15[0][v_buffer_index].key<<endl; }
				if(res150.value != udata150.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((0*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer15[0]["<<v_buffer_index<<"].value: "<<vbuffer15[0][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata151 = acts_utilobj->UTIL_GETKV2(refbuffer[1][merge_buffer_index]);
				keyvalue_t vdata151 = acts_utilobj->UTIL_GETKV2(vbuffer15[1][v_buffer_index]);
				
				// merge_buffer[1][merge_buffer_index] = vbuffer15[1][v_buffer_index];	 
				merge_buffer[1][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata151);
				
				keyvalue_t res151; 
				res151.key = MERGE_applyfunc(udata151.key, vdata151.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res151.value = MERGE_applyfunc(udata151.value, vdata151.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer15[1][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[1]["<<v_buffer_index<<"].key: "<<vbuffer15[1][v_buffer_index].key<<endl; }
				if(vbuffer15[1][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[1]["<<v_buffer_index<<"].value: "<<vbuffer15[1][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res1.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[1]["<<v_buffer_index<<"].key: "<<vbuffer15[1][v_buffer_index].key<<endl; }
				if(res1.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[1]["<<v_buffer_index<<"].value: "<<vbuffer15[1][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res151.key != udata151.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer15[1]["<<v_buffer_index<<"].key: "<<vbuffer15[1][v_buffer_index].key<<endl; }
				if(res151.value != udata151.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((1*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer15[1]["<<v_buffer_index<<"].value: "<<vbuffer15[1][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata152 = acts_utilobj->UTIL_GETKV2(refbuffer[2][merge_buffer_index]);
				keyvalue_t vdata152 = acts_utilobj->UTIL_GETKV2(vbuffer15[2][v_buffer_index]);
				
				// merge_buffer[2][merge_buffer_index] = vbuffer15[2][v_buffer_index];	 
				merge_buffer[2][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata152);
				
				keyvalue_t res152; 
				res152.key = MERGE_applyfunc(udata152.key, vdata152.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res152.value = MERGE_applyfunc(udata152.value, vdata152.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer15[2][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[2]["<<v_buffer_index<<"].key: "<<vbuffer15[2][v_buffer_index].key<<endl; }
				if(vbuffer15[2][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[2]["<<v_buffer_index<<"].value: "<<vbuffer15[2][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res2.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[2]["<<v_buffer_index<<"].key: "<<vbuffer15[2][v_buffer_index].key<<endl; }
				if(res2.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[2]["<<v_buffer_index<<"].value: "<<vbuffer15[2][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res152.key != udata152.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer15[2]["<<v_buffer_index<<"].key: "<<vbuffer15[2][v_buffer_index].key<<endl; }
				if(res152.value != udata152.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((2*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer15[2]["<<v_buffer_index<<"].value: "<<vbuffer15[2][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata153 = acts_utilobj->UTIL_GETKV2(refbuffer[3][merge_buffer_index]);
				keyvalue_t vdata153 = acts_utilobj->UTIL_GETKV2(vbuffer15[3][v_buffer_index]);
				
				// merge_buffer[3][merge_buffer_index] = vbuffer15[3][v_buffer_index];	 
				merge_buffer[3][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata153);
				
				keyvalue_t res153; 
				res153.key = MERGE_applyfunc(udata153.key, vdata153.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res153.value = MERGE_applyfunc(udata153.value, vdata153.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer15[3][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[3]["<<v_buffer_index<<"].key: "<<vbuffer15[3][v_buffer_index].key<<endl; }
				if(vbuffer15[3][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[3]["<<v_buffer_index<<"].value: "<<vbuffer15[3][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res3.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[3]["<<v_buffer_index<<"].key: "<<vbuffer15[3][v_buffer_index].key<<endl; }
				if(res3.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[3]["<<v_buffer_index<<"].value: "<<vbuffer15[3][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res153.key != udata153.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer15[3]["<<v_buffer_index<<"].key: "<<vbuffer15[3][v_buffer_index].key<<endl; }
				if(res153.value != udata153.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((3*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer15[3]["<<v_buffer_index<<"].value: "<<vbuffer15[3][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata154 = acts_utilobj->UTIL_GETKV2(refbuffer[4][merge_buffer_index]);
				keyvalue_t vdata154 = acts_utilobj->UTIL_GETKV2(vbuffer15[4][v_buffer_index]);
				
				// merge_buffer[4][merge_buffer_index] = vbuffer15[4][v_buffer_index];	 
				merge_buffer[4][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata154);
				
				keyvalue_t res154; 
				res154.key = MERGE_applyfunc(udata154.key, vdata154.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res154.value = MERGE_applyfunc(udata154.value, vdata154.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer15[4][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[4]["<<v_buffer_index<<"].key: "<<vbuffer15[4][v_buffer_index].key<<endl; }
				if(vbuffer15[4][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[4]["<<v_buffer_index<<"].value: "<<vbuffer15[4][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res4.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[4]["<<v_buffer_index<<"].key: "<<vbuffer15[4][v_buffer_index].key<<endl; }
				if(res4.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[4]["<<v_buffer_index<<"].value: "<<vbuffer15[4][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res154.key != udata154.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer15[4]["<<v_buffer_index<<"].key: "<<vbuffer15[4][v_buffer_index].key<<endl; }
				if(res154.value != udata154.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((4*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer15[4]["<<v_buffer_index<<"].value: "<<vbuffer15[4][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata155 = acts_utilobj->UTIL_GETKV2(refbuffer[5][merge_buffer_index]);
				keyvalue_t vdata155 = acts_utilobj->UTIL_GETKV2(vbuffer15[5][v_buffer_index]);
				
				// merge_buffer[5][merge_buffer_index] = vbuffer15[5][v_buffer_index];	 
				merge_buffer[5][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata155);
				
				keyvalue_t res155; 
				res155.key = MERGE_applyfunc(udata155.key, vdata155.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res155.value = MERGE_applyfunc(udata155.value, vdata155.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer15[5][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[5]["<<v_buffer_index<<"].key: "<<vbuffer15[5][v_buffer_index].key<<endl; }
				if(vbuffer15[5][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[5]["<<v_buffer_index<<"].value: "<<vbuffer15[5][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res5.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[5]["<<v_buffer_index<<"].key: "<<vbuffer15[5][v_buffer_index].key<<endl; }
				if(res5.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[5]["<<v_buffer_index<<"].value: "<<vbuffer15[5][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res155.key != udata155.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer15[5]["<<v_buffer_index<<"].key: "<<vbuffer15[5][v_buffer_index].key<<endl; }
				if(res155.value != udata155.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((5*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer15[5]["<<v_buffer_index<<"].value: "<<vbuffer15[5][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata156 = acts_utilobj->UTIL_GETKV2(refbuffer[6][merge_buffer_index]);
				keyvalue_t vdata156 = acts_utilobj->UTIL_GETKV2(vbuffer15[6][v_buffer_index]);
				
				// merge_buffer[6][merge_buffer_index] = vbuffer15[6][v_buffer_index];	 
				merge_buffer[6][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata156);
				
				keyvalue_t res156; 
				res156.key = MERGE_applyfunc(udata156.key, vdata156.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res156.value = MERGE_applyfunc(udata156.value, vdata156.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer15[6][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[6]["<<v_buffer_index<<"].key: "<<vbuffer15[6][v_buffer_index].key<<endl; }
				if(vbuffer15[6][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[6]["<<v_buffer_index<<"].value: "<<vbuffer15[6][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res6.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[6]["<<v_buffer_index<<"].key: "<<vbuffer15[6][v_buffer_index].key<<endl; }
				if(res6.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[6]["<<v_buffer_index<<"].value: "<<vbuffer15[6][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res156.key != udata156.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer15[6]["<<v_buffer_index<<"].key: "<<vbuffer15[6][v_buffer_index].key<<endl; }
				if(res156.value != udata156.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((6*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer15[6]["<<v_buffer_index<<"].value: "<<vbuffer15[6][v_buffer_index].value<<endl; }			
				#endif 
 
				keyvalue_t udata157 = acts_utilobj->UTIL_GETKV2(refbuffer[7][merge_buffer_index]);
				keyvalue_t vdata157 = acts_utilobj->UTIL_GETKV2(vbuffer15[7][v_buffer_index]);
				
				// merge_buffer[7][merge_buffer_index] = vbuffer15[7][v_buffer_index];	 
				merge_buffer[7][merge_buffer_index] = acts_utilobj->UTIL_GETKV2(vdata157);
				
				keyvalue_t res157; 
				res157.key = MERGE_applyfunc(udata157.key, vdata157.key, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				res157.value = MERGE_applyfunc(udata157.value, vdata157.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
				#ifdef _DEBUGMODE_KERNELPRINTS	
				if(vbuffer15[7][v_buffer_index].key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[7]["<<v_buffer_index<<"].key: "<<vbuffer15[7][v_buffer_index].key<<endl; }
				if(vbuffer15[7][v_buffer_index].value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[7]["<<v_buffer_index<<"].value: "<<vbuffer15[7][v_buffer_index].value<<endl; }			
				#endif 	
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE
				if(res7.key < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[7]["<<v_buffer_index<<"].key: "<<vbuffer15[7][v_buffer_index].key<<endl; }
				if(res7.value < 9999){ cout<<"SYNC AND APPLY FUNC SEEN @ vbuffer15[7]["<<v_buffer_index<<"].value: "<<vbuffer15[7][v_buffer_index].value<<endl; }			
				#endif 	
				
				#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
				if(res157.key != udata157.key){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2))<<", vbuffer15[7]["<<v_buffer_index<<"].key: "<<vbuffer15[7][v_buffer_index].key<<endl; }
				if(res157.value != udata157.value){ cout<<"acts_merge::ACTIVE VTX SEEN: vid: "<<(vwriteoffsetpp0_kvs*VECTOR_SIZE*2 + ((7*globalparams.SIZE_REDUCE) + k*2 + 1))<<", vbuffer15[7]["<<v_buffer_index<<"].value: "<<vbuffer15[7][v_buffer_index].value<<endl; }			
				#endif 
			merge_buffer_index += 1;
			v_buffer_index += 1;
			// if(v_buffer_index>8){ exit(EXIT_SUCCESS); }////////////////////
		}
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int k=0; k<reducebuffersz; k++){
 
			if(merge_buffer[0][k].key < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(0 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[0]["<<k<<"].key: "<<merge_buffer[0][k].key<<endl; }
			if(merge_buffer[0][k].value < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(1 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[0]["<<k<<"].value: "<<merge_buffer[0][k].value<<endl; }
 
			if(merge_buffer[1][k].key < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(1 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[1]["<<k<<"].key: "<<merge_buffer[1][k].key<<endl; }
			if(merge_buffer[1][k].value < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(2 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[1]["<<k<<"].value: "<<merge_buffer[1][k].value<<endl; }
 
			if(merge_buffer[2][k].key < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(2 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[2]["<<k<<"].key: "<<merge_buffer[2][k].key<<endl; }
			if(merge_buffer[2][k].value < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(3 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[2]["<<k<<"].value: "<<merge_buffer[2][k].value<<endl; }
 
			if(merge_buffer[3][k].key < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(3 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[3]["<<k<<"].key: "<<merge_buffer[3][k].key<<endl; }
			if(merge_buffer[3][k].value < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(4 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[3]["<<k<<"].value: "<<merge_buffer[3][k].value<<endl; }
 
			if(merge_buffer[4][k].key < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(4 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[4]["<<k<<"].key: "<<merge_buffer[4][k].key<<endl; }
			if(merge_buffer[4][k].value < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(5 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[4]["<<k<<"].value: "<<merge_buffer[4][k].value<<endl; }
 
			if(merge_buffer[5][k].key < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(5 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[5]["<<k<<"].key: "<<merge_buffer[5][k].key<<endl; }
			if(merge_buffer[5][k].value < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(6 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[5]["<<k<<"].value: "<<merge_buffer[5][k].value<<endl; }
 
			if(merge_buffer[6][k].key < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(6 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[6]["<<k<<"].key: "<<merge_buffer[6][k].key<<endl; }
			if(merge_buffer[6][k].value < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(7 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[6]["<<k<<"].value: "<<merge_buffer[6][k].value<<endl; }
 
			if(merge_buffer[7][k].key < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(7 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[7]["<<k<<"].key: "<<merge_buffer[7][k].key<<endl; }
			if(merge_buffer[7][k].value < 999){ cout<<"SYNC AND APPLY FUNC SEEN @ (vid1: "<<(8 * globalparams.SIZE_REDUCE) + k<<", merge_buffer[7]["<<k<<"].value: "<<merge_buffer[7][k].value<<endl; }
		}
		#endif
		// exit(EXIT_SUCCESS); ////////////////////
		
		MERGE_writevdata(ON, vdram, globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + vwriteoffsetpp0_kvs, merge_buffer, 0, reducebuffersz);
		vwriteoffsetpp0_kvs += merge_buffer_index;
		vreadoffsetpp0_kvs += v_buffer_index;
		cout<<">>> ACTS MERGE: merging finished: vreadoffsetpp0_kvs: "<<vreadoffsetpp0_kvs<<", v_buffer_index: "<<v_buffer_index<<", vwriteoffsetpp0_kvs: "<<vwriteoffsetpp0_kvs<<", merge_buffer_index: "<<merge_buffer_index<<endl;
		// exit(EXIT_SUCCESS); //////////////////// CRITICAL REMOVEME.
	}
	#if defined(_DEBUGMODE_KERNELPRINTS3)
	cout<<">>> ACTS MERGE: merging finished: vreadoffsetpp0_kvs: "<<vreadoffsetpp0_kvs<<", vwriteoffsetpp0_kvs: "<<vwriteoffsetpp0_kvs<<endl;
	#endif 
	return;
}
}

