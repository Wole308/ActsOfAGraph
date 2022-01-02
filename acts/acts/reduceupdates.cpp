#include "reduceupdates.h"
using namespace std;

#ifdef SW
reduceupdates::reduceupdates(){ actsutilityobj = new actsutility(); acts_utilobj = new acts_util(); }
reduceupdates::~reduceupdates(){}
#endif

value_t 
	#ifdef SW 
	reduceupdates::
	#endif 
REDUCE_reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	if(GraphAlgo == PAGERANK){
		temp = vtemp + res;
	} else if(GraphAlgo == BFS){
		temp = acts_utilobj->UTIL_amin(vtemp, GraphIter);
	} else if(GraphAlgo == SSSP){
		temp = acts_utilobj->UTIL_amin(vtemp, res);
	} else {
		temp = NAp;
	}
	return temp;
}

/** void 
	#ifdef SW 
	reduceupdates::
	#endif
REDUCE_reducevector(keyvalue_buffer_t kvdata, keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS PIPELINE II=3 // CRITICAL NEWCHANGE.	
	// #pragma HLS PIPELINE II=2 // CRITICAL NEWCHANGE.	
	analysis_type analysis_loop1 = VECTOR_SIZE;
	
	keyvalue_t mykeyvalue = acts_utilobj->UTIL_GETKV(kvdata);
	
	vertex_t loc = mykeyvalue.key - upperlimit;
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(mykeyvalue.key != acts_utilobj->UTIL_GETK(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ cout<<"REDUCE SEEN @ REDUCE_reducevector:: vid: "<<upperlimit + loc<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<", upperlimit: "<<upperlimit<<", reduce size: "<<globalparams.SIZE_REDUCE<<endl; }
	#endif 
	
	if(loc >= globalparams.SIZE_REDUCE && mykeyvalue.key != acts_utilobj->UTIL_GETK(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->reducehelper_checkreduceloc(0, loc, mykeyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }

	vertex_t rowindex = loc / 2;
	vertex_t colindex = loc % 2;
	
	keyvalue_t curr_vprop;
	if(mykeyvalue.key != acts_utilobj->UTIL_GETV(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ curr_vprop = acts_utilobj->UTIL_GETKV2(destbuffer[destoffset + rowindex]); }
	
	value_t temp;
	if(colindex == 0){ temp = curr_vprop.key; }
	else { temp = curr_vprop.value; }
	
	value_t new_vprop = REDUCE_reducefunc(temp, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(mykeyvalue.key != acts_utilobj->UTIL_GETV(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ cout<<"REDUCEFUNC RESULT @ REDUCE_reducevector:: new_vprop: "<<new_vprop<<", temp: "<<temp<<", mykeyvalue.value: "<<mykeyvalue.value<<", NAp: "<<NAp<<endl; }
	#endif 
	
	if(colindex == 0){ curr_vprop.key = new_vprop; }
	else { curr_vprop.value = new_vprop; }
	
	if(mykeyvalue.key != acts_utilobj->UTIL_GETV(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ destbuffer[destoffset + rowindex] = acts_utilobj->UTIL_GETKV2(curr_vprop); }
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(1);
	if(mykeyvalue.key != acts_utilobj->UTIL_GETV(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
	#endif
	return;
} */
/* void 
	#ifdef SW 
	reduceupdates::
	#endif
REDUCE_reducevector(keyvalue_buffer_t kvdata, keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS PIPELINE II=3 // CRITICAL NEWCHANGE.	
	// #pragma HLS PIPELINE II=2 // CRITICAL NEWCHANGE.	
	analysis_type analysis_loop1 = VECTOR_SIZE;
	
	keyvalue_t mykeyvalue = acts_utilobj->UTIL_GETKV(kvdata);
	
	vertex_t loc = mykeyvalue.key - upperlimit;
	
	bool en = true; if(mykeyvalue.key != acts_utilobj->UTIL_GETV(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ en = true; } else { en = false; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(mykeyvalue.key != acts_utilobj->UTIL_GETK(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ cout<<"REDUCE SEEN @ REDUCE_reducevector:: vid: "<<upperlimit + loc<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<", upperlimit: "<<upperlimit<<", reduce size: "<<globalparams.SIZE_REDUCE<<endl; }
	#endif 
	
	if(loc >= globalparams.SIZE_REDUCE && mykeyvalue.key != acts_utilobj->UTIL_GETK(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->reducehelper_checkreduceloc(0, loc, mykeyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }

	vertex_t rowindex = loc / 2;
	vertex_t colindex = loc % 2;
	
	keyvalue_t curr_vprop;
	if(en == true){ curr_vprop = acts_utilobj->UTIL_GETKV2(destbuffer[destoffset + rowindex]); }
	
	value_t temp;
	if(colindex == 0){ temp = curr_vprop.key; }
	else { temp = curr_vprop.value; }
	
	value_t new_vprop = REDUCE_reducefunc(temp, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(en == true){ cout<<"REDUCEFUNC RESULT @ REDUCE_reducevector:: new_vprop: "<<new_vprop<<", temp: "<<temp<<", mykeyvalue.value: "<<mykeyvalue.value<<", NAp: "<<NAp<<endl; }
	#endif 
	
	if(colindex == 0){ curr_vprop.key = new_vprop; }
	else { curr_vprop.value = new_vprop; }
	
	if(en == true){ destbuffer[destoffset + rowindex] = acts_utilobj->UTIL_GETKV2(curr_vprop); }
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(1);
	if(en == true){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
	#endif
	return;
} */

void 
	#ifdef SW 
	reduceupdates::
	#endif
REDUCE_reducevector(unsigned int col, keyvalue_buffer_t kvdata, keyvalue_vbuffer_t destbuffer[BLOCKRAM_SIZE], buffer_type destoffset, unsigned int upperlimit, sweepparams_t sweepparams, globalparams_t globalparams){
	#pragma HLS PIPELINE II=3
	analysis_type analysis_loop1 = VECTOR_SIZE;
	
	keyvalue_t mykeyvalue = acts_utilobj->UTIL_GETKV(kvdata);
	
	#ifdef CONFIG_SPLIT_DESTVTXS
	// vertex_t loc = ((mykeyvalue.key - upperlimit) - col) / NUM_PARTITIONS;
	vertex_t loc = ((mykeyvalue.key - upperlimit) - col) >> NUM_PARTITIONS_POW;
	#else 
	vertex_t loc = mykeyvalue.key - upperlimit;
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"REDUCE_reducevector:: col: "<<col<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<endl;
	#endif 
	
	bool en = true; if(mykeyvalue.key != acts_utilobj->UTIL_GETV(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ en = true; } else { en = false; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(mykeyvalue.key != acts_utilobj->UTIL_GETK(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ cout<<"REDUCE SEEN @ REDUCE_reducevector:: vid: "<<upperlimit + loc<<", loc: "<<loc<<", mykeyvalue.key: "<<mykeyvalue.key<<", mykeyvalue.value: "<<mykeyvalue.value<<", upperlimit: "<<upperlimit<<", reduce size: "<<globalparams.SIZE_REDUCE<<endl; }
	#endif 
	
	if(loc >= globalparams.SIZE_REDUCE && mykeyvalue.key != acts_utilobj->UTIL_GETK(INVALIDDATA) && mykeyvalue.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){ 
		if(true){ cout<<"REDUCE_reducevector::ERROR SEEN: loc("<<loc<<") >= globalparams.SIZE_REDUCE("<<globalparams.SIZE_REDUCE<<"). mykeyvalue.key: "<<mykeyvalue.key<<", upperlimit: "<<upperlimit<<", col: "<<col<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->reducehelper_checkreduceloc(0, loc, mykeyvalue, sweepparams, globalparams); 
		#endif 
		loc = 0; }

	vertex_t rowindex = loc / 2;
	vertex_t colindex = loc % 2;
	
	keyvalue_t curr_vprop;
	if(en == true){ curr_vprop = acts_utilobj->UTIL_GETKV2(destbuffer[destoffset + rowindex]); }
	
	value_t temp;
	if(colindex == 0){ temp = curr_vprop.key; }
	else { temp = curr_vprop.value; }
	
	value_t new_vprop = REDUCE_reducefunc(temp, mykeyvalue.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(en == true){ cout<<"REDUCEFUNC RESULT @ REDUCE_reducevector:: new_vprop: "<<new_vprop<<", temp: "<<temp<<", mykeyvalue.value: "<<mykeyvalue.value<<", NAp: "<<NAp<<endl; }
	#endif 
	
	if(colindex == 0){ curr_vprop.key = new_vprop; }
	else { curr_vprop.value = new_vprop; }
	
	if(en == true){ destbuffer[destoffset + rowindex] = acts_utilobj->UTIL_GETKV2(curr_vprop); }
	
	#ifdef _DEBUGMODE_STATS
	actsutilityobj->globalstats_countkvsreduced(1);
	if(en == true){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
	#endif
	return;
}

void 
	#ifdef SW 
	reduceupdates::
	#endif 
REDUCE_reduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][DESTBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], sweepparams_t sweepparams, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	#if REDUCEBUFFERFACTOR==8
	analysis_type analysis_loopcount = (DESTBLOCKRAM_SIZE / (NUM_PARTITIONS / 2)); // =46: '2' is safety padding.
	#endif 
	#if REDUCEBUFFERFACTOR==16
	analysis_type analysis_loopcount = (DESTBLOCKRAM_SIZE / NUM_PARTITIONS);
	#endif 
	
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

	unsigned int tmplloprange = 0;
	#if REDUCEBUFFERFACTOR==8
	REDUCEBUFFERPARTITIONS_LOOP1: for(unsigned int it=0; it<NUM_PARTITIONS; it+=NUM_PARTITIONS/2){
		buffer_type maxsize_kvs = 0;
		REDUCEBUFFERPARTITIONS_LOOP1B: for(partition_type p=0; p<NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=1
			bramoffset_kvs[p] = localcapsule[it+p].key / VECTOR_SIZE;
			size_kvs[p] = localcapsule[it+p].value / VECTOR_SIZE;
			if(maxsize_kvs < size_kvs[p]){ maxsize_kvs = size_kvs[p]; }
		}
		
		unsigned int lloprange = acts_utilobj->UTIL_getrangeforeachllop(globalparams);
		REDUCEBUFFERPARTITIONS_LOOP1C: for(unsigned int p=0; p<NUM_PARTITIONS/2; p++){
		#pragma HLS PIPELINE II=1
			upperlimits[p] = sweepparams.upperlimit + tmplloprange;
			tmplloprange += lloprange;
		}
		
		REDUCEBUFFERPARTITIONS_LOOP1D: for(buffer_type i=0; i<maxsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		#pragma HLS PIPELINE II=16
			for(vector_type v=0; v<VECTOR_SIZE; v++){
				kvdata0 = buffer[v][bramoffset_kvs[0] + i]; 	
				kvdata1 = buffer[v][bramoffset_kvs[1] + i]; 	
				kvdata2 = buffer[v][bramoffset_kvs[2] + i]; 	
				kvdata3 = buffer[v][bramoffset_kvs[3] + i]; 	
				kvdata4 = buffer[v][bramoffset_kvs[4] + i]; 	
				kvdata5 = buffer[v][bramoffset_kvs[5] + i]; 	
				kvdata6 = buffer[v][bramoffset_kvs[6] + i]; 	
				kvdata7 = buffer[v][bramoffset_kvs[7] + i]; 	
	

				#ifdef CONFIG_SPLIT_DESTVTXS
				if(i< size_kvs[0]){ REDUCE_reducevector(it+0, kvdata0, vbuffer[it+0], 0, sweepparams.upperlimit, sweepparams, globalparams); }
				if(i< size_kvs[1]){ REDUCE_reducevector(it+1, kvdata1, vbuffer[it+1], 0, sweepparams.upperlimit, sweepparams, globalparams); }
				if(i< size_kvs[2]){ REDUCE_reducevector(it+2, kvdata2, vbuffer[it+2], 0, sweepparams.upperlimit, sweepparams, globalparams); }
				if(i< size_kvs[3]){ REDUCE_reducevector(it+3, kvdata3, vbuffer[it+3], 0, sweepparams.upperlimit, sweepparams, globalparams); }
				if(i< size_kvs[4]){ REDUCE_reducevector(it+4, kvdata4, vbuffer[it+4], 0, sweepparams.upperlimit, sweepparams, globalparams); }
				if(i< size_kvs[5]){ REDUCE_reducevector(it+5, kvdata5, vbuffer[it+5], 0, sweepparams.upperlimit, sweepparams, globalparams); }
				if(i< size_kvs[6]){ REDUCE_reducevector(it+6, kvdata6, vbuffer[it+6], 0, sweepparams.upperlimit, sweepparams, globalparams); }
				if(i< size_kvs[7]){ REDUCE_reducevector(it+7, kvdata7, vbuffer[it+7], 0, sweepparams.upperlimit, sweepparams, globalparams); }
				#else 
				if(i< size_kvs[0]){ REDUCE_reducevector(it+0, kvdata0, vbuffer[it+0], 0, upperlimits[0], sweepparams, globalparams); }
				if(i< size_kvs[1]){ REDUCE_reducevector(it+1, kvdata1, vbuffer[it+1], 0, upperlimits[1], sweepparams, globalparams); }
				if(i< size_kvs[2]){ REDUCE_reducevector(it+2, kvdata2, vbuffer[it+2], 0, upperlimits[2], sweepparams, globalparams); }
				if(i< size_kvs[3]){ REDUCE_reducevector(it+3, kvdata3, vbuffer[it+3], 0, upperlimits[3], sweepparams, globalparams); }
				if(i< size_kvs[4]){ REDUCE_reducevector(it+4, kvdata4, vbuffer[it+4], 0, upperlimits[4], sweepparams, globalparams); }
				if(i< size_kvs[5]){ REDUCE_reducevector(it+5, kvdata5, vbuffer[it+5], 0, upperlimits[5], sweepparams, globalparams); }
				if(i< size_kvs[6]){ REDUCE_reducevector(it+6, kvdata6, vbuffer[it+6], 0, upperlimits[6], sweepparams, globalparams); }
				if(i< size_kvs[7]){ REDUCE_reducevector(it+7, kvdata7, vbuffer[it+7], 0, upperlimits[7], sweepparams, globalparams); }
				#endif 
			}
		}
	}
	#endif 
	#if REDUCEBUFFERFACTOR==16
	buffer_type maxsize_kvs = 0;
	REDUCEBUFFERPARTITIONS_LOOP1B: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		bramoffset_kvs[p] = localcapsule[p].key / VECTOR_SIZE;
		size_kvs[p] = localcapsule[p].value / VECTOR_SIZE;
		if(maxsize_kvs < size_kvs[p]){ maxsize_kvs = size_kvs[p]; }
	}
	
	unsigned int lloprange = acts_utilobj->UTIL_getrangeforeachllop(globalparams);
	REDUCEBUFFERPARTITIONS_LOOP1C: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		upperlimits[p] = sweepparams.upperlimit + tmplloprange;
		tmplloprange += lloprange;
	}
	
	REDUCEBUFFERPARTITIONS_LOOP1D: for(buffer_type i=0; i<maxsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
	#pragma HLS PIPELINE II=16
		for(vector_type v=0; v<VECTOR_SIZE; v++){
			kvdata0 = buffer[v][bramoffset_kvs[0] + i]; 	
			kvdata1 = buffer[v][bramoffset_kvs[1] + i]; 	
			kvdata2 = buffer[v][bramoffset_kvs[2] + i]; 	
			kvdata3 = buffer[v][bramoffset_kvs[3] + i]; 	
			kvdata4 = buffer[v][bramoffset_kvs[4] + i]; 	
			kvdata5 = buffer[v][bramoffset_kvs[5] + i]; 	
			kvdata6 = buffer[v][bramoffset_kvs[6] + i]; 	
			kvdata7 = buffer[v][bramoffset_kvs[7] + i]; 	
			kvdata8 = buffer[v][bramoffset_kvs[8] + i]; 	
			kvdata9 = buffer[v][bramoffset_kvs[9] + i]; 	
			kvdata10 = buffer[v][bramoffset_kvs[10] + i]; 	
			kvdata11 = buffer[v][bramoffset_kvs[11] + i]; 	
			kvdata12 = buffer[v][bramoffset_kvs[12] + i]; 	
			kvdata13 = buffer[v][bramoffset_kvs[13] + i]; 	
			kvdata14 = buffer[v][bramoffset_kvs[14] + i]; 	
			kvdata15 = buffer[v][bramoffset_kvs[15] + i]; 	
	

			if(i< size_kvs[0]){ REDUCE_reducevector(it+0, kvdata0, vbuffer[0], 0, upperlimits[0], sweepparams, globalparams); }
			if(i< size_kvs[1]){ REDUCE_reducevector(it+1, kvdata1, vbuffer[1], 0, upperlimits[1], sweepparams, globalparams); }
			if(i< size_kvs[2]){ REDUCE_reducevector(it+2, kvdata2, vbuffer[2], 0, upperlimits[2], sweepparams, globalparams); }
			if(i< size_kvs[3]){ REDUCE_reducevector(it+3, kvdata3, vbuffer[3], 0, upperlimits[3], sweepparams, globalparams); }
			if(i< size_kvs[4]){ REDUCE_reducevector(it+4, kvdata4, vbuffer[4], 0, upperlimits[4], sweepparams, globalparams); }
			if(i< size_kvs[5]){ REDUCE_reducevector(it+5, kvdata5, vbuffer[5], 0, upperlimits[5], sweepparams, globalparams); }
			if(i< size_kvs[6]){ REDUCE_reducevector(it+6, kvdata6, vbuffer[6], 0, upperlimits[6], sweepparams, globalparams); }
			if(i< size_kvs[7]){ REDUCE_reducevector(it+7, kvdata7, vbuffer[7], 0, upperlimits[7], sweepparams, globalparams); }
			if(i< size_kvs[8]){ REDUCE_reducevector(it+8, kvdata8, vbuffer[8], 0, upperlimits[8], sweepparams, globalparams); }
			if(i< size_kvs[9]){ REDUCE_reducevector(it+9, kvdata9, vbuffer[9], 0, upperlimits[9], sweepparams, globalparams); }
			if(i< size_kvs[10]){ REDUCE_reducevector(it+10, kvdata10, vbuffer[10], 0, upperlimits[10], sweepparams, globalparams); }
			if(i< size_kvs[11]){ REDUCE_reducevector(it+11, kvdata11, vbuffer[11], 0, upperlimits[11], sweepparams, globalparams); }
			if(i< size_kvs[12]){ REDUCE_reducevector(it+12, kvdata12, vbuffer[12], 0, upperlimits[12], sweepparams, globalparams); }
			if(i< size_kvs[13]){ REDUCE_reducevector(it+13, kvdata13, vbuffer[13], 0, upperlimits[13], sweepparams, globalparams); }
			if(i< size_kvs[14]){ REDUCE_reducevector(it+14, kvdata14, vbuffer[14], 0, upperlimits[14], sweepparams, globalparams); }
			if(i< size_kvs[15]){ REDUCE_reducevector(it+15, kvdata15, vbuffer[15], 0, upperlimits[15], sweepparams, globalparams); }
		}
	}
	#endif 
	// exit(EXIT_SUCCESS); ////
	return;
}

void 
	#ifdef SW 
	reduceupdates::
	#endif 
REDUCE_priorreduceandbuffer(bool_type enable, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], keyvalue_capsule_t localcapsule[MAX_NUM_PARTITIONS], keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], buffer_type chunk_size, sweepparams_t sweepparams, globalparams_t globalparams){				
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
			keyvalue_t kv2 = acts_utilobj->UTIL_GETKV(kv);
		
			partition_type p = ((kv2.key - sweepparams.upperlimit) >> (REDUCESZ_POW));
			
			unsigned int upperlimit = sweepparams.upperlimit + p*REDUCESZ;
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			cout<<"priorreduceandbuffer: kv2.key: "<<kv2.key<<", kv2.value: "<<kv2.value<<", p: "<<p<<", upperlimit: "<<upperlimit<<", sweepparams.upperlimit: "<<sweepparams.upperlimit<<", currentLOP: "<<sweepparams.currentLOP<<endl;
			#endif 
			
			if(p < VDATA_PACKINGSIZE){ REDUCE_reducevector(p, kv, vbuffer[p], 0, upperlimit, sweepparams, globalparams); } // REMOVEME.
		}
	}
	return;
}

void
	#ifdef SW
	reduceupdates::
	#endif 
REDUCE_tradreduceandbuffer(bool_type enable, uint512_dt * kvdram, keyvalue_buffer_t buffer[VECTOR_SIZE][SOURCEBLOCKRAM_SIZE], buffer_type chunk_size, keyvalue_t globalstatsbuffer[MAX_NUM_PARTITIONS], sweepparams_t sweepparams, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	analysis_type analysis_loopcount = SOURCEBLOCKRAM_SIZE;
	
	TRADREDUCEANDBUFFER_LOOP1: for(buffer_type i=0; i<chunk_size; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loopcount avg=analysis_loopcount
		TRADREDUCEANDBUFFER_LOOP1B: for(unsigned int v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE II=1 // NEWCHANGE.
			keyvalue_buffer_t kv = buffer[v][i];
			keyvalue_t kv2 = acts_utilobj->UTIL_GETKV(kv);
			
			if(kv2.key != acts_utilobj->UTIL_GETV(INVALIDDATA) && kv2.value != acts_utilobj->UTIL_GETV(INVALIDDATA)){
			
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
				
				value_t new_vprop = REDUCE_reducefunc(temp, kv2.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
				
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
				
				partition_type p = acts_utilobj->UTIL_getpartition(ON, REDUCEMODE, kv, sweepparams.currentLOP, sweepparams.upperlimit, sweepparams.upperpartition, globalparams.POW_BATCHRANGE);
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


