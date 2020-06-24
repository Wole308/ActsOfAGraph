#include "/home/oj2zf/Documents/ActsOfAGraph/src/common.h"
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "/home/oj2zf/Documents/ActsOfAGraph/kernels/enigma.h"
using namespace std;

#define DRAMLIMITSZ (1 << 26)
#define DRAMLIMITSZ_KVS (FORCDFINISH_DRAMSZ / VECTOR_SIZE)

#ifndef FPGA_IMPL
enigma::enigma(){}
enigma::~enigma(){}
#endif
#ifndef HW

unsigned int globalvar_totalkvstatsread;
unsigned int globalvar_totalkvstatscollected;
unsigned int globalvar_totalkvstatswritten;

unsigned int globalvar_totalkvsread;

unsigned int globalstats_totalkvspartitioned;
unsigned int globalstats_totalkvspartitionswritten;

unsigned int globalstats_totalkvsreduced;
unsigned int globalstats_totalkvsreducewritten;
#endif 
#ifdef _LDEBUGMODE_HEADER
void enigma::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound){
	if(data > upper_bound){ std::cout<<"enigma::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<std::endl; exit(EXIT_FAILURE); }
}
void enigma::checkforequal(string message, unsigned int data1, unsigned int data2){
	if(data1 == data2){ cout<<"enigma::checkforequal: ERROR. data1 == data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
}
void enigma::print1(string messagea, unsigned int dataa){
	cout<<messagea<<": "<<dataa<<endl;
}
void enigma::print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<endl;
}
void enigma::print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<endl;
}
void enigma::print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<", "<<messagef<<": "<<datef<<endl;
}
void enigma::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; }
}
void enigma::printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"["<<keyvalues1[p].key<<":"<<keyvalues1[p].value<<"]["<<keyvalues2[p].key<<":"<<keyvalues2[p].value<<"]"<<endl; }
}
void enigma::printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int i=0; i<size_kvs; i++){ 
		#ifdef _WIDEWORD
			cout<<"keyvalues["<<i<<"].data[0].key: "<<(unsigned int)keyvalues[i].range(31, 0)<<", keyvalues["<<i<<"].data[0].value: "<<(unsigned int)keyvalues[i].range(63, 32)<<endl;
			cout<<"keyvalues["<<i<<"].data[1].key: "<<(unsigned int)keyvalues[i].range(95, 64)<<", keyvalues["<<i<<"].data[1].value: "<<(unsigned int)keyvalues[i].range(127, 96)<<endl;
			cout<<"keyvalues["<<i<<"].data[2].key: "<<(unsigned int)keyvalues[i].range(159, 128)<<", keyvalues["<<i<<"].data[2].value: "<<(unsigned int)keyvalues[i].range(191, 160)<<endl;
			cout<<"keyvalues["<<i<<"].data[3].key: "<<(unsigned int)keyvalues[i].range(223, 192)<<", keyvalues["<<i<<"].data[3].value: "<<(unsigned int)keyvalues[i].range(255, 224)<<endl;
			cout<<"keyvalues["<<i<<"].data[4].key: "<<(unsigned int)keyvalues[i].range(287, 256)<<", keyvalues["<<i<<"].data[4].value: "<<(unsigned int)keyvalues[i].range(319, 288)<<endl;
			cout<<"keyvalues["<<i<<"].data[5].key: "<<(unsigned int)keyvalues[i].range(351, 320)<<", keyvalues["<<i<<"].data[5].value: "<<(unsigned int)keyvalues[i].range(383, 352)<<endl;
			cout<<"keyvalues["<<i<<"].data[6].key: "<<(unsigned int)keyvalues[i].range(415, 384)<<", keyvalues["<<i<<"].data[6].value: "<<(unsigned int)keyvalues[i].range(447, 416)<<endl;
			cout<<"keyvalues["<<i<<"].data[7].key: "<<(unsigned int)keyvalues[i].range(479, 448)<<", keyvalues["<<i<<"].data[7].value: "<<(unsigned int)keyvalues[i].range(511, 480)<<endl;
		#else 
		for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"keyvalues["<<i<<"].data["<<v<<"].key: "<<keyvalues[i].data[v].key<<", keyvalues["<<i<<"].data["<<v<<"].value: "<<keyvalues[i].data[v].value<<endl; }
		#endif 
	}
}
void enigma::getvaluecount(string message, keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	cout<<"total values counted: "<<totalnumkeyvalues<<endl;
}
void countkeyvalues(){

}
void enigma::printparameters(){
	cout<<endl<<"enigma::printparameters: test started."<<endl;
	cout<<"enigma::printparameters:: KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
	cout<<"enigma::printparameters:: KVDATA_BATCHSIZE_KVS: "<<KVDATA_BATCHSIZE_KVS<<endl;
	cout<<"enigma::printparameters:: BUFFER_SIZE: "<<BUFFER_SIZE<<endl;
}
#endif
#ifdef _DEBUGMODE_STATS
void enigma::printglobalvars(){
	cout<<"printglobalvars: "<<endl;
	cout<<"enigma::printglobalvars:: COLLECTSTATS & SPREAD & PARTITION:: globalvar_totalkvsread: "<<globalvar_totalkvsread<<endl;
	
	cout<<"enigma::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatsread: "<<globalvar_totalkvstatsread<<endl;
	cout<<"enigma::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatscollected: "<<globalvar_totalkvstatscollected<<endl;
	cout<<"enigma::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatswritten: "<<globalvar_totalkvstatswritten<<endl;	

	cout<<"enigma::printglobalvars:: SPREAD:: globalstats_totalkvspartitioned: "<<globalstats_totalkvspartitioned<<endl;
	cout<<"enigma::printglobalvars:: SPREAD:: globalstats_totalkvspartitionswritten: "<<globalstats_totalkvspartitionswritten<<endl;
	
	cout<<"enigma::printglobalvars:: REDUCE:: globalstats_totalkvsreduced: "<<globalstats_totalkvsreduced<<endl;
	cout<<"enigma::printglobalvars:: REDUCE:: globalstats_totalkvsreducewritten: "<<globalstats_totalkvsreducewritten<<endl;
}
void enigma::clearglobalvars(){
	cout<<"clearglobalvars: "<<endl;
	globalvar_totalkvsread = 0;
	globalstats_totalkvspartitionswritten = 0;
	globalstats_totalkvspartitioned = 0;
	globalvar_totalkvstatsread = 0;
	globalvar_totalkvstatscollected = 0;
	globalvar_totalkvstatswritten = 0;
	globalstats_totalkvsreduced = 0;
	globalstats_totalkvsreducewritten = 0;
}
void enigma::globalstats_countkvstatsread(unsigned int count){
	globalvar_totalkvstatsread += count;
	return;
}
void enigma::globalstats_countkvstatscollected(unsigned int count){
	globalvar_totalkvstatscollected += count;
	return;
}
void enigma::globalstats_countkvstatswritten(unsigned int count){
	globalvar_totalkvstatswritten += count;
	return;
}
void enigma::globalstats_countkvsread(unsigned int count){
	globalvar_totalkvsread += count;
	return;
}
void enigma::globalstats_countkvspartitionswritten(unsigned int count){
	globalstats_totalkvspartitionswritten += count;
	return;
}
void enigma::globalstats_countkvspartitioned(unsigned int count){
	globalstats_totalkvspartitioned += count;
	return;
}
void enigma::globalstats_countkvsreduced(unsigned int count){
	globalstats_totalkvsreduced += count;
	return;
}
void enigma::globalstats_countkvsreducewritten(unsigned int count){
	globalstats_totalkvsreducewritten += count;
	return;
}
#endif 

unsigned int enigma::allignhigher_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
unsigned int enigma::allignlower_KV(unsigned int val){
	unsigned int fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
unsigned int enigma::ceildiv(unsigned int val1, unsigned int val2){
	return (val1 + (val2 - 1)) / val2;
}	
unsigned int enigma::max(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}
unsigned int enigma::min(unsigned int A, unsigned int B){
	#pragma HLS INLINE
	if(A<B){ return A; }
	else { return B; }
}
unsigned int enigma::hsub(unsigned int A, unsigned int B){
	#pragma HLS INLINE
	if(A < B){ return 0; }
	else { return A - B; }
}

unsigned int enigma::getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	// return 0;
	partition_type hashedval = 0;
	hashedval = ((keyvalue.key - upperlimit) >> (KVDATA_RANGE_PERSSDPARTITION_POW - (NUM_PARTITIONS_POW * currentLOP)));
	
	int KKK = (((1 << NUM_PARTITIONS_POW) - 1) & (hashedval >> (1 - 1)));
	// if(KKK >= NUM_PARTITIONS){ cout<<"--- getpartition: KKK: "<<KKK<<endl; exit(EXIT_FAILURE); }
	
	
	// return (hashedval % NUM_PARTITIONS); // REMOVEME.
	return (((1 << NUM_PARTITIONS_POW) - 1) & (hashedval >> (1 - 1))); // REMOVEME.
	// if(hashedval >= NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}
keyvalue_t enigma::getkeyvalue(uint512_dt buffer[BUFFER_SIZE], batch_type addr, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	keyvalue_t keyvalue;
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	if(idy >= maxaddr_kvs){ cout<<"getkeyvalue:: ERROR 45. addr: "<<addr<<", idy: "<<idy<<", idx: "<<idx<<endl; exit(EXIT_FAILURE); }
	checkoutofbounds("enigma::getkeyvalue 35", idy, maxaddr_kvs);
	#endif
	
	#ifdef _WIDEWORD
if(idx==0){	
		keyvalue.key = buffer[idy].range(31, 0);
		keyvalue.value = buffer[idy].range(63, 32);
	}
else if(idx==1){	
		keyvalue.key = buffer[idy].range(95, 64);
		keyvalue.value = buffer[idy].range(127, 96);
	}
else if(idx==2){	
		keyvalue.key = buffer[idy].range(159, 128);
		keyvalue.value = buffer[idy].range(191, 160);
	}
else if(idx==3){	
		keyvalue.key = buffer[idy].range(223, 192);
		keyvalue.value = buffer[idy].range(255, 224);
	}
else if(idx==4){	
		keyvalue.key = buffer[idy].range(287, 256);
		keyvalue.value = buffer[idy].range(319, 288);
	}
else if(idx==5){	
		keyvalue.key = buffer[idy].range(351, 320);
		keyvalue.value = buffer[idy].range(383, 352);
	}
else if(idx==6){	
		keyvalue.key = buffer[idy].range(415, 384);
		keyvalue.value = buffer[idy].range(447, 416);
	}
else if(idx==7){	
		keyvalue.key = buffer[idy].range(479, 448);
		keyvalue.value = buffer[idy].range(511, 480);
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"getkeyvalue:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	#else
	keyvalue = buffer[idy].data[idx];
	#endif
	return keyvalue;
}
void enigma::setkeyvalue(uint512_dt buffer[BUFFER_SIZE], batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::setkeyvalue 35", idy, maxaddr_kvs);
	#endif
	
	#ifdef _WIDEWORD
if(idx==0){	
		buffer[idy].range(31, 0) = keyvalue.key;
		buffer[idy].range(63, 32) = keyvalue.value;
	}
else if(idx==1){	
		buffer[idy].range(95, 64) = keyvalue.key;
		buffer[idy].range(127, 96) = keyvalue.value;
	}
else if(idx==2){	
		buffer[idy].range(159, 128) = keyvalue.key;
		buffer[idy].range(191, 160) = keyvalue.value;
	}
else if(idx==3){	
		buffer[idy].range(223, 192) = keyvalue.key;
		buffer[idy].range(255, 224) = keyvalue.value;
	}
else if(idx==4){	
		buffer[idy].range(287, 256) = keyvalue.key;
		buffer[idy].range(319, 288) = keyvalue.value;
	}
else if(idx==5){	
		buffer[idy].range(351, 320) = keyvalue.key;
		buffer[idy].range(383, 352) = keyvalue.value;
	}
else if(idx==6){	
		buffer[idy].range(415, 384) = keyvalue.key;
		buffer[idy].range(447, 416) = keyvalue.value;
	}
else if(idx==7){	
		buffer[idy].range(479, 448) = keyvalue.key;
		buffer[idy].range(511, 480) = keyvalue.value;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"setkventryII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	#else
	buffer[idy].data[idx] = keyvalue;
	#endif
	return;
}

buffer_type enigma::getactivesize(buffer_type i, batch_type i_end, buffer_type buffersize){	
	#pragma HLS INLINE
	buffer_type activesize = 0;
	buffer_type next_i = i + buffersize;
	if(i >= i_end){ activesize = 0; }
	else if ((i < i_end) && (next_i < i_end)) { activesize = buffersize; }
	else if ((i < i_end) && (next_i >= i_end)) { activesize = i_end - i; }
	else {
		i = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"getactivesize:: should NEVER get here: activesize: "<<activesize<<", i: "<<i<<", next_i: "<<next_i<<", i_end: "<<i_end<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	print4("getactivesize: i", "i_end", "buffersize", "activesize", i, i_end, buffersize, activesize);
	#endif
	return activesize;
}
batch_type enigma::getAddr(batch_type addr){
	#pragma HLS INLINE
	return addr;
}
batch_type enigma::getstatsAddr(batch_type addr){
	#pragma HLS INLINE
	return BASEOFFSET_STATSDRAM + addr;
}
batch_type enigma::getmessagesAddr(batch_type addr){
	#pragma HLS INLINE
	return BASEOFFSET_MESSAGESDRAM + addr;
}

unsigned int enigma::getsourceskipsize(unsigned int currentLOP, unsigned int param){
	#pragma HLS INLINE
	unsigned int treedepth = TREE_DEPTH; // NB: for debugging purposes only
	unsigned int result = param;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		result = result / NUM_PARTITIONS;
	}
	return result;
}
unsigned int enigma::getdestskipsize(unsigned int currentLOP, unsigned int param){
	#pragma HLS INLINE
	unsigned int treedepthloop_tripcount = TREE_DEPTH; // NB: for debugging purposes only
	unsigned int result = param / NUM_PARTITIONS;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepthloop_tripcount avg=treedepthloop_tripcount
		result = result / NUM_PARTITIONS;
	}
	return result;
}

void enigma::loaddeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t LLOPnumpartitions){
	#pragma HLS INLINE
	unsigned int skipsize = getdestskipsize(currentLOP, LLOPnumpartitions);
	unsigned int nxtpos = partitionoffset;
	
	LOADDESTSTATS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE	
		buffer[p] = kvstats[getstatsAddr(nxtpos)]; 
		nxtpos += skipsize;
	}
	LOADDESTSTATS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		buffer[p].value = 0;
	}
	return;
}
void enigma::storedeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t LLOPnumpartitions){
	#pragma HLS INLINE
	unsigned int skipsize = getdestskipsize(currentLOP, LLOPnumpartitions);
	unsigned int nxtpos = partitionoffset;
	
	STOREDESTSTATS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvstats[getstatsAddr(nxtpos)].value = buffer[p].value; 
		nxtpos += skipsize;
	}
	STOREDESTSTATS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		buffer[p].value = 0;
	}
	return;
}

unsigned int enigma::get_num_source_partitions(unsigned int currentLOP){
	#pragma HLS INLINE
	unsigned int treedepth = TREEDEPTH_FORSSDPARTITION0;
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}

void enigma::prepareoffsets(buffer_type sizes[NUM_PARTITIONS], buffer_type offsets[NUM_PARTITIONS]){
	#pragma HLS INLINE	
	// PREPAREOFFSETS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ sizes[p] = 0; }
	offsets[0] = 0;
	for (partition_type p = 1; p<NUM_PARTITIONS; p++){ offsets[p] = offsets[p-1] + sizes[p-1]; }
	return;
}
void enigma::preparecapsule(keyvalue_t capsule[NUM_PARTITIONS]){
	#pragma HLS INLINE
	capsule[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){ capsule[p].key = capsule[p-1].key + capsule[p-1].value; }
	return;
}
void enigma::preparecapsules( keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], keyvalue_t capsule4[NUM_PARTITIONS], keyvalue_t capsule5[NUM_PARTITIONS], keyvalue_t capsule6[NUM_PARTITIONS], keyvalue_t capsule7[NUM_PARTITIONS]){
	#pragma HLS INLINE
	capsule0[0].key = 0;
	capsule1[0].key = 0;
	capsule2[0].key = 0;
	capsule3[0].key = 0;
	capsule4[0].key = 0;
	capsule5[0].key = 0;
	capsule6[0].key = 0;
	capsule7[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){
		capsule0[p].key = capsule0[p-1].key + capsule0[p-1].value;
		capsule1[p].key = capsule1[p-1].key + capsule1[p-1].value;
		capsule2[p].key = capsule2[p-1].key + capsule2[p-1].value;
		capsule3[p].key = capsule3[p-1].key + capsule3[p-1].value;
		capsule4[p].key = capsule4[p-1].key + capsule4[p-1].value;
		capsule5[p].key = capsule5[p-1].key + capsule5[p-1].value;
		capsule6[p].key = capsule6[p-1].key + capsule6[p-1].value;
		capsule7[p].key = capsule7[p-1].key + capsule7[p-1].value;
	}
	return;
}

void enigma::loadclopparams(globalparams_t globalparams, clopparams_t * llopparams, unsigned int currentLOP){
	#pragma HLS INLINE
	if((currentLOP % 2) == 1){ llopparams->baseaddr_worksourcekvs_kvs = BASEOFFSET_KVDRAM_KVS; llopparams->baseaddr_workdestkvs_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; }
	else { llopparams->baseaddr_worksourcekvs_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; llopparams->baseaddr_workdestkvs_kvs = BASEOFFSET_KVDRAM_KVS; }	
	llopparams->baseaddr_capsule_kvs = NAp;
	llopparams->baseaddr_stats = BASEOFFSET_STATSDRAM;
	llopparams->num_source_partitions = get_num_source_partitions(currentLOP);
	llopparams->nextsourceoffset_partition = 0;
	llopparams->nextdestoffset_partition = 0;
	llopparams->nextsourceoffset_kv = 0;
	llopparams->nextdestoffset_kv = 0;
	llopparams->sourceskipsize_partition = getsourceskipsize(currentLOP, globalparams.LLOPnumpartitions);
	llopparams->destskipsize_partition = getdestskipsize(currentLOP, globalparams.LLOPnumpartitions);
	llopparams->sourceskipsize_kv = getsourceskipsize(currentLOP, KVDATA_RANGE_PERSSDPARTITION);
	llopparams->destskipsize_kv = getdestskipsize(currentLOP, KVDATA_RANGE_PERSSDPARTITION);
	llopparams->destpartitionrange = (1 << (KVDATA_RANGE_PERSSDPARTITION_POW - (NUM_PARTITIONS_POW * currentLOP)));
	llopparams->currentLOP = currentLOP;
	llopparams->upperlimit = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	print1("loadclopparams:: llopparams->baseaddr_worksourcekvs_kvs", llopparams->baseaddr_worksourcekvs_kvs);
	print1("loadclopparams:: llopparams->baseaddr_workdestkvs_kvs", llopparams->baseaddr_workdestkvs_kvs);
	print1("loadclopparams:: llopparams->baseaddr_capsule_kvs", llopparams->baseaddr_capsule_kvs);
	print1("loadclopparams:: llopparams->baseaddr_stats", llopparams->baseaddr_stats);
	print1("loadclopparams:: llopparams->num_source_partitions", llopparams->num_source_partitions);
	print1("loadclopparams:: llopparams->nextsourceoffset_partition", llopparams->nextsourceoffset_partition);
	print1("loadclopparams:: llopparams->nextdestoffset_partition", llopparams->nextdestoffset_partition);
	print1("loadclopparams:: llopparams->nextsourceoffset_kv", llopparams->nextsourceoffset_kv);
	print1("loadclopparams:: llopparams->nextdestoffset_kv", llopparams->nextdestoffset_kv);
	print1("loadclopparams:: llopparams->sourceskipsize_partition", llopparams->sourceskipsize_partition);
	print1("loadclopparams:: llopparams->destskipsize_partition", llopparams->destskipsize_partition);
	print1("loadclopparams:: llopparams->sourceskipsize_kv", llopparams->sourceskipsize_kv);
	print1("loadclopparams:: llopparams->destskipsize_kv", llopparams->destskipsize_kv);
	print1("loadclopparams:: llopparams->destpartitionrange", llopparams->destpartitionrange);
	print1("loadclopparams:: llopparams->currentLOP", llopparams->currentLOP);
	print1("loadclopparams:: llopparams->upperlimit", llopparams->upperlimit);
	#endif
	return;
}

value_t enigma::reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount){
	#pragma HLS INLINE
	value_t ret = 0;
	#ifdef PR_ALGORITHM
	ret = value + edgeval;
	#elif defined(BFS_ALGORITHM)
	ret = min(value, IterCount);
	#elif defined(BC_ALGORITHM)
	ret = min(value, IterCount);
	#endif
	return ret;
}

void enigma::generatepartitions0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
	// NB: debugging purposes only variables
	unsigned int analysis_maxtreedepthsz = TREE_DEPTH;
	unsigned int analysis_avgnumsourcepartitions = (1 << (TREE_DEPTH * NUM_PARTITIONS_POW));
	unsigned int analysis_avgsz_kvs = KVDATA_BATCHSIZE_KVS / analysis_avgnumsourcepartitions; 
	unsigned int analysis_avgtripcnt = analysis_avgsz_kvs / (NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE);
	unsigned int analysis_buffersz = BUFFER_SIZE;
	unsigned int analysis_partition = BUFFER_SIZE * VECTOR_SIZE;
	unsigned int analysis_savepartition = BUFFER_SIZE / NUM_PARTITIONS;
	
	uint512_dt v1_local0[PADDEDBUFFER_SIZE];
	uint512_dt result_local0[PADDEDBUFFER_SIZE];
	keyvalue_t capsule0[NUM_PARTITIONS];
	uint512_dt v1_local1[PADDEDBUFFER_SIZE];
	uint512_dt result_local1[PADDEDBUFFER_SIZE];
	keyvalue_t capsule1[NUM_PARTITIONS];
	uint512_dt v1_local2[PADDEDBUFFER_SIZE];
	uint512_dt result_local2[PADDEDBUFFER_SIZE];
	keyvalue_t capsule2[NUM_PARTITIONS];
	uint512_dt v1_local3[PADDEDBUFFER_SIZE];
	uint512_dt result_local3[PADDEDBUFFER_SIZE];
	keyvalue_t capsule3[NUM_PARTITIONS];
	uint512_dt v1_local4[PADDEDBUFFER_SIZE];
	uint512_dt result_local4[PADDEDBUFFER_SIZE];
	keyvalue_t capsule4[NUM_PARTITIONS];
	uint512_dt v1_local5[PADDEDBUFFER_SIZE];
	uint512_dt result_local5[PADDEDBUFFER_SIZE];
	keyvalue_t capsule5[NUM_PARTITIONS];
	uint512_dt v1_local6[PADDEDBUFFER_SIZE];
	uint512_dt result_local6[PADDEDBUFFER_SIZE];
	keyvalue_t capsule6[NUM_PARTITIONS];
	uint512_dt v1_local7[PADDEDBUFFER_SIZE];
	uint512_dt result_local7[PADDEDBUFFER_SIZE];
	keyvalue_t capsule7[NUM_PARTITIONS];

	// Partition Phase
	PARTITIONPHASE_LOOP1: for(unsigned int currentLOP=1; currentLOP<(globalparams.treedepth + 1); currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print4("PartitionPhase0:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		PARTITIONPHASE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partitionparams;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKER_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			#pragma HLS PIPELINE II=1
				kvdeststats_tmp[p].value = 0;
			}
			
			keyvalue_t sourcestat = kvstats[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparams.step_kvs = NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE;
			partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			travstate_t travstate;
			#ifdef _DEBUGMODE_KERNELPRINTS3
			print6("PartitionPhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);
			#endif
			
			int count = 0;
			
			// Partition phase 
			PARTITIONPHASE_LOOP3: for (int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += BUFFER_SIZE * NUMWORKERS * NUMSUBWORKERS){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgtripcnt avg=analysis_avgtripcnt
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("PartitionPhase0:: i", "i_end", "NAp", "NAp", "skip size", i, partitionparams.end_kvs, NAp, NAp, BUFFER_SIZE * NUMWORKERS * NUMSUBWORKERS);
				#endif
				
				buffer_type chunk0_size = BUFFER_SIZE;
				buffer_type chunk1_size = BUFFER_SIZE;
				buffer_type chunk2_size = BUFFER_SIZE;
				buffer_type chunk3_size = BUFFER_SIZE;
				buffer_type chunk4_size = BUFFER_SIZE;
				buffer_type chunk5_size = BUFFER_SIZE;
				buffer_type chunk6_size = BUFFER_SIZE;
				buffer_type chunk7_size = BUFFER_SIZE;
				
				//boundary checks
				batch_type i0 = i + (0 * BUFFER_SIZE);
				if (i0 > partitionparams.end_kvs){ chunk0_size = 0; }
				else if ((i0 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk0_size = partitionparams.end_kvs - i0; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk0_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i0 + chunk0_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i1 = i + (1 * BUFFER_SIZE);
				if (i1 > partitionparams.end_kvs){ chunk1_size = 0; }
				else if ((i1 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk1_size = partitionparams.end_kvs - i1; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk1_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i1 + chunk1_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i2 = i + (2 * BUFFER_SIZE);
				if (i2 > partitionparams.end_kvs){ chunk2_size = 0; }
				else if ((i2 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk2_size = partitionparams.end_kvs - i2; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk2_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i2 + chunk2_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i3 = i + (3 * BUFFER_SIZE);
				if (i3 > partitionparams.end_kvs){ chunk3_size = 0; }
				else if ((i3 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk3_size = partitionparams.end_kvs - i3; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk3_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i3 + chunk3_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i4 = i + (4 * BUFFER_SIZE);
				if (i4 > partitionparams.end_kvs){ chunk4_size = 0; }
				else if ((i4 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk4_size = partitionparams.end_kvs - i4; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk4_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i4 + chunk4_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i5 = i + (5 * BUFFER_SIZE);
				if (i5 > partitionparams.end_kvs){ chunk5_size = 0; }
				else if ((i5 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk5_size = partitionparams.end_kvs - i5; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk5_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i5 + chunk5_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i6 = i + (6 * BUFFER_SIZE);
				if (i6 > partitionparams.end_kvs){ chunk6_size = 0; }
				else if ((i6 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk6_size = partitionparams.end_kvs - i6; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk6_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i6 + chunk6_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i7 = i + (7 * BUFFER_SIZE);
				if (i7 > partitionparams.end_kvs){ chunk7_size = 0; }
				else if ((i7 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk7_size = partitionparams.end_kvs - i7; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk7_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i7 + chunk7_size, KVSOURCEDRAMSZ_KVS);
				#endif

				// burst read first vector from global memory to local memory
				partitionphase_v1_rd:
				
					batch_type readdramoffset0_kvs = llopparams.baseaddr_worksourcekvs_kvs + (0 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd0 dram 35", readdramoffset0_kvs + chunk0_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd0 bram 35", chunk0_size, BUFFER_SIZE);
					#endif
					READBUFFER0_LOOP1: for (buffer_type j=0; j<chunk0_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local0[j] = kvsourcedram[readdramoffset0_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset1_kvs = llopparams.baseaddr_worksourcekvs_kvs + (1 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd1 dram 35", readdramoffset1_kvs + chunk1_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd1 bram 35", chunk1_size, BUFFER_SIZE);
					#endif
					READBUFFER1_LOOP1: for (buffer_type j=0; j<chunk1_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local1[j] = kvsourcedram[readdramoffset1_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset2_kvs = llopparams.baseaddr_worksourcekvs_kvs + (2 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd2 dram 35", readdramoffset2_kvs + chunk2_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd2 bram 35", chunk2_size, BUFFER_SIZE);
					#endif
					READBUFFER2_LOOP1: for (buffer_type j=0; j<chunk2_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local2[j] = kvsourcedram[readdramoffset2_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset3_kvs = llopparams.baseaddr_worksourcekvs_kvs + (3 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd3 dram 35", readdramoffset3_kvs + chunk3_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd3 bram 35", chunk3_size, BUFFER_SIZE);
					#endif
					READBUFFER3_LOOP1: for (buffer_type j=0; j<chunk3_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local3[j] = kvsourcedram[readdramoffset3_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset4_kvs = llopparams.baseaddr_worksourcekvs_kvs + (4 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd4 dram 35", readdramoffset4_kvs + chunk4_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd4 bram 35", chunk4_size, BUFFER_SIZE);
					#endif
					READBUFFER4_LOOP1: for (buffer_type j=0; j<chunk4_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local4[j] = kvsourcedram[readdramoffset4_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset5_kvs = llopparams.baseaddr_worksourcekvs_kvs + (5 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd5 dram 35", readdramoffset5_kvs + chunk5_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd5 bram 35", chunk5_size, BUFFER_SIZE);
					#endif
					READBUFFER5_LOOP1: for (buffer_type j=0; j<chunk5_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local5[j] = kvsourcedram[readdramoffset5_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset6_kvs = llopparams.baseaddr_worksourcekvs_kvs + (6 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd6 dram 35", readdramoffset6_kvs + chunk6_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd6 bram 35", chunk6_size, BUFFER_SIZE);
					#endif
					READBUFFER6_LOOP1: for (buffer_type j=0; j<chunk6_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local6[j] = kvsourcedram[readdramoffset6_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset7_kvs = llopparams.baseaddr_worksourcekvs_kvs + (7 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd7 dram 35", readdramoffset7_kvs + chunk7_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd7 bram 35", chunk7_size, BUFFER_SIZE);
					#endif
					READBUFFER7_LOOP1: for (buffer_type j=0; j<chunk7_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local7[j] = kvsourcedram[readdramoffset7_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
					
				RESETCAPSULEA_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = 0;  capsule1[p].value = 0;  capsule2[p].value = 0;  capsule3[p].value = 0;  capsule4[p].value = 0;  capsule5[p].value = 0;  capsule6[p].value = 0;  capsule7[p].value = 0;  }
				
				// extract capsule
				EXTRACTCAPSULES_LOOP2: for(buffer_type k=0; k<BUFFER_SIZE * VECTOR_SIZE; k++){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partition avg=analysis_partition
				#pragma HLS PIPELINE II=1
					keyvalue_t keyvalue0 = getkeyvalue(v1_local0, k, BUFFER_SIZE);
					keyvalue_t keyvalue1 = getkeyvalue(v1_local1, k, BUFFER_SIZE);
					keyvalue_t keyvalue2 = getkeyvalue(v1_local2, k, BUFFER_SIZE);
					keyvalue_t keyvalue3 = getkeyvalue(v1_local3, k, BUFFER_SIZE);
					keyvalue_t keyvalue4 = getkeyvalue(v1_local4, k, BUFFER_SIZE);
					keyvalue_t keyvalue5 = getkeyvalue(v1_local5, k, BUFFER_SIZE);
					keyvalue_t keyvalue6 = getkeyvalue(v1_local6, k, BUFFER_SIZE);
					keyvalue_t keyvalue7 = getkeyvalue(v1_local7, k, BUFFER_SIZE);
					
					partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p4 = getpartition(keyvalue4, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p5 = getpartition(keyvalue5, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p6 = getpartition(keyvalue6, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p7 = getpartition(keyvalue7, llopparams.currentLOP, llopparams.upperlimit);
					
					if(k < chunk0_size * VECTOR_SIZE){ capsule0[p0].value += 1; }
					if(k < chunk1_size * VECTOR_SIZE){ capsule1[p1].value += 1; }
					if(k < chunk2_size * VECTOR_SIZE){ capsule2[p2].value += 1; }
					if(k < chunk3_size * VECTOR_SIZE){ capsule3[p3].value += 1; }
					if(k < chunk4_size * VECTOR_SIZE){ capsule4[p4].value += 1; }
					if(k < chunk5_size * VECTOR_SIZE){ capsule5[p5].value += 1; }
					if(k < chunk6_size * VECTOR_SIZE){ capsule6[p6].value += 1; }
					if(k < chunk7_size * VECTOR_SIZE){ capsule7[p7].value += 1; }
					
					#ifdef _DEBUGMODE_STATS
					if(k < chunk0_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					if(k < chunk1_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					if(k < chunk2_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					if(k < chunk3_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					if(k < chunk4_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					if(k < chunk5_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					if(k < chunk6_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					if(k < chunk7_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					#endif
				}
				
				preparecapsules( capsule0 , capsule1 , capsule2 , capsule3 , capsule4 , capsule5 , capsule6 , capsule7 );				
				RESETCAPSULEB_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = 0;  capsule1[p].value = 0;  capsule2[p].value = 0;  capsule3[p].value = 0;  capsule4[p].value = 0;  capsule5[p].value = 0;  capsule6[p].value = 0;  capsule7[p].value = 0;  }
				
				// partition
				PARTITION_LOOP1: for(buffer_type k=0; k<BUFFER_SIZE * VECTOR_SIZE; k++){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partition avg=analysis_partition
				#pragma HLS PIPELINE II=1
					keyvalue_t keyvalue0 = getkeyvalue(v1_local0, k, BUFFER_SIZE);
					keyvalue_t keyvalue1 = getkeyvalue(v1_local1, k, BUFFER_SIZE);
					keyvalue_t keyvalue2 = getkeyvalue(v1_local2, k, BUFFER_SIZE);
					keyvalue_t keyvalue3 = getkeyvalue(v1_local3, k, BUFFER_SIZE);
					keyvalue_t keyvalue4 = getkeyvalue(v1_local4, k, BUFFER_SIZE);
					keyvalue_t keyvalue5 = getkeyvalue(v1_local5, k, BUFFER_SIZE);
					keyvalue_t keyvalue6 = getkeyvalue(v1_local6, k, BUFFER_SIZE);
					keyvalue_t keyvalue7 = getkeyvalue(v1_local7, k, BUFFER_SIZE);
					
					partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p4 = getpartition(keyvalue4, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p5 = getpartition(keyvalue5, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p6 = getpartition(keyvalue6, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p7 = getpartition(keyvalue7, llopparams.currentLOP, llopparams.upperlimit);
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule0[p0].key + capsule0[p0].value, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule1[p1].key + capsule1[p1].value, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule2[p2].key + capsule2[p2].value, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule3[p3].key + capsule3[p3].value, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule4[p4].key + capsule4[p4].value, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule5[p5].key + capsule5[p5].value, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule6[p6].key + capsule6[p6].value, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule7[p7].key + capsule7[p7].value, BUFFER_SIZE * VECTOR_SIZE);
					#endif
					
					setkeyvalue(result_local0, capsule0[p0].key + capsule0[p0].value, keyvalue0, BUFFER_SIZE);
					setkeyvalue(result_local1, capsule1[p1].key + capsule1[p1].value, keyvalue1, BUFFER_SIZE);
					setkeyvalue(result_local2, capsule2[p2].key + capsule2[p2].value, keyvalue2, BUFFER_SIZE);
					setkeyvalue(result_local3, capsule3[p3].key + capsule3[p3].value, keyvalue3, BUFFER_SIZE);
					setkeyvalue(result_local4, capsule4[p4].key + capsule4[p4].value, keyvalue4, BUFFER_SIZE);
					setkeyvalue(result_local5, capsule5[p5].key + capsule5[p5].value, keyvalue5, BUFFER_SIZE);
					setkeyvalue(result_local6, capsule6[p6].key + capsule6[p6].value, keyvalue6, BUFFER_SIZE);
					setkeyvalue(result_local7, capsule7[p7].key + capsule7[p7].value, keyvalue7, BUFFER_SIZE);
					
					if(k < chunk0_size * VECTOR_SIZE){ capsule0[p0].value += 1; }
					if(k < chunk1_size * VECTOR_SIZE){ capsule1[p1].value += 1; }
					if(k < chunk2_size * VECTOR_SIZE){ capsule2[p2].value += 1; }
					if(k < chunk3_size * VECTOR_SIZE){ capsule3[p3].value += 1; }
					if(k < chunk4_size * VECTOR_SIZE){ capsule4[p4].value += 1; }
					if(k < chunk5_size * VECTOR_SIZE){ capsule5[p5].value += 1; }
					if(k < chunk6_size * VECTOR_SIZE){ capsule6[p6].value += 1; }
					if(k < chunk7_size * VECTOR_SIZE){ capsule7[p7].value += 1; }
					
					#ifdef _DEBUGMODE_STATS
					if(k < chunk0_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					if(k < chunk1_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					if(k < chunk2_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					if(k < chunk3_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					if(k < chunk4_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					if(k < chunk5_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					if(k < chunk6_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					if(k < chunk7_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					#endif
				}
				
				//burst write the result
				partitionphase_out_write:
					SAVEBUFFER0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule0[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule0[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer0 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer0 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local0[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule0[p].value); }
					SAVEBUFFER1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule1[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule1[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer1 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer1 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local1[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					UPDATEGLOBALSTATS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule1[p].value); }
					SAVEBUFFER2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule2[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule2[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer2 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer2 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local2[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					UPDATEGLOBALSTATS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule2[p].value); }
					SAVEBUFFER3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule3[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule3[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer3 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer3 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local3[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					UPDATEGLOBALSTATS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule3[p].value); }
					SAVEBUFFER4_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule4[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule4[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer4 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer4 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local4[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					UPDATEGLOBALSTATS4_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule4[p].value); }
					SAVEBUFFER5_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule5[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule5[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer5 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer5 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local5[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					UPDATEGLOBALSTATS5_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule5[p].value); }
					SAVEBUFFER6_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule6[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule6[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer6 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer6 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local6[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					UPDATEGLOBALSTATS6_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule6[p].value); }
					SAVEBUFFER7_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule7[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule7[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer7 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer7 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local7[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					UPDATEGLOBALSTATS7_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule7[p].value); }
			}
			
			storedeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			llopparams.nextsourceoffset_partition += llopparams.sourceskipsize_partition;
			llopparams.nextdestoffset_partition += (llopparams.destskipsize_partition * NUM_PARTITIONS);
			llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv;
			llopparams.nextdestoffset_kv += llopparams.destskipsize_kv;
		}
		#ifdef _LDEBUGMODE_HEADER
		printglobalvars();
		printparameters();
		clearglobalvars();
		#endif
	}
	
	// Reduce Phase
	if(globalparams.reducecommand == ON){
		unsigned int currentLOP = globalparams.treedepth + 1;
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);		
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print4("ReducePhase0:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		REDUCEPHASE_LOOP1: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=NUMWORKERS * NUMSUBWORKERS){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partition0params;
			partitionparams_t partition1params;
			partitionparams_t partition2params;
			partitionparams_t partition3params;
			partitionparams_t partition4params;
			partitionparams_t partition5params;
			partitionparams_t partition6params;
			partitionparams_t partition7params;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
		
			keyvalue_t sourcestats[NUMSUBWORKERS];
			unsigned int maxsz = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats[i] = kvstats[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS_APPLYPH; sw++){ maxsz = max(sourcestats[sw].value, maxsz); }
			unsigned int maxsz_kvs = (maxsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			
			keyvalue_t sourcestat;
			sourcestat = sourcestats[0];
			partition0params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition0params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition0params.step_kvs = NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE;
			partition0params.end_kvs = partition0params.begin_kvs + partition0params.size_kvs;
			sourcestat = sourcestats[1];
			partition1params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition1params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition1params.step_kvs = NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE;
			partition1params.end_kvs = partition1params.begin_kvs + partition1params.size_kvs;
			sourcestat = sourcestats[2];
			partition2params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition2params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition2params.step_kvs = NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE;
			partition2params.end_kvs = partition2params.begin_kvs + partition2params.size_kvs;
			sourcestat = sourcestats[3];
			partition3params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition3params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition3params.step_kvs = NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE;
			partition3params.end_kvs = partition3params.begin_kvs + partition3params.size_kvs;
			sourcestat = sourcestats[4];
			partition4params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition4params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition4params.step_kvs = NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE;
			partition4params.end_kvs = partition4params.begin_kvs + partition4params.size_kvs;
			sourcestat = sourcestats[5];
			partition5params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition5params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition5params.step_kvs = NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE;
			partition5params.end_kvs = partition5params.begin_kvs + partition5params.size_kvs;
			sourcestat = sourcestats[6];
			partition6params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition6params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition6params.step_kvs = NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE;
			partition6params.end_kvs = partition6params.begin_kvs + partition6params.size_kvs;
			sourcestat = sourcestats[7];
			partition7params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition7params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition7params.step_kvs = NUMWORKERS * NUMSUBWORKERS * BUFFER_SIZE;
			partition7params.end_kvs = partition7params.begin_kvs + partition7params.size_kvs;
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[0].key, sourcestats[0].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[1].key, sourcestats[1].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[2].key, sourcestats[2].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[3].key, sourcestats[3].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[4].key, sourcestats[4].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[5].key, sourcestats[5].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[6].key, sourcestats[6].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[7].key, sourcestats[7].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			#endif
			// exit(EXIT_SUCCESS);
			
			// Reduce phase
			REDUCEPHASE_REDUCEPARTITION_LOOP: for (int i = 0; i < maxsz_kvs; i += BUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgtripcnt avg=analysis_avgtripcnt
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase0:: i", "i_end", "NAp", "NAp", "skip size", i, maxsz_kvs, NAp, NAp, BUFFER_SIZE);
				#endif
				
				buffer_type chunk0_size = BUFFER_SIZE;
				buffer_type chunk1_size = BUFFER_SIZE;
				buffer_type chunk2_size = BUFFER_SIZE;
				buffer_type chunk3_size = BUFFER_SIZE;
				buffer_type chunk4_size = BUFFER_SIZE;
				buffer_type chunk5_size = BUFFER_SIZE;
				buffer_type chunk6_size = BUFFER_SIZE;
				buffer_type chunk7_size = BUFFER_SIZE;
				
				//boundary checks
				batch_type i0 = i;
				if (i0 > partition0params.end_kvs){ chunk0_size = 0; }
				else if ((i0 + BUFFER_SIZE) > partition0params.end_kvs){ chunk0_size = partition0params.end_kvs - i0; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk0_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i0 + chunk0_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i1 = i;
				if (i1 > partition1params.end_kvs){ chunk1_size = 0; }
				else if ((i1 + BUFFER_SIZE) > partition1params.end_kvs){ chunk1_size = partition1params.end_kvs - i1; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk1_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i1 + chunk1_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i2 = i;
				if (i2 > partition2params.end_kvs){ chunk2_size = 0; }
				else if ((i2 + BUFFER_SIZE) > partition2params.end_kvs){ chunk2_size = partition2params.end_kvs - i2; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk2_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i2 + chunk2_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i3 = i;
				if (i3 > partition3params.end_kvs){ chunk3_size = 0; }
				else if ((i3 + BUFFER_SIZE) > partition3params.end_kvs){ chunk3_size = partition3params.end_kvs - i3; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk3_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i3 + chunk3_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i4 = i;
				if (i4 > partition4params.end_kvs){ chunk4_size = 0; }
				else if ((i4 + BUFFER_SIZE) > partition4params.end_kvs){ chunk4_size = partition4params.end_kvs - i4; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk4_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i4 + chunk4_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i5 = i;
				if (i5 > partition5params.end_kvs){ chunk5_size = 0; }
				else if ((i5 + BUFFER_SIZE) > partition5params.end_kvs){ chunk5_size = partition5params.end_kvs - i5; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk5_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i5 + chunk5_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i6 = i;
				if (i6 > partition6params.end_kvs){ chunk6_size = 0; }
				else if ((i6 + BUFFER_SIZE) > partition6params.end_kvs){ chunk6_size = partition6params.end_kvs - i6; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk6_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i6 + chunk6_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i7 = i;
				if (i7 > partition7params.end_kvs){ chunk7_size = 0; }
				else if ((i7 + BUFFER_SIZE) > partition7params.end_kvs){ chunk7_size = partition7params.end_kvs - i7; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk7_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i7 + chunk7_size, KVSOURCEDRAMSZ_KVS);
				#endif

				// burst read
				reducephase_v1_rd:
				
					batch_type readdramoffset0_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd0 dram 35", readdramoffset0_kvs + chunk0_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd0 bram 35", chunk0_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER0_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE * VECTOR_SIZE; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local0[j] = kvsourcedram[readdramoffset0_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset1_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd1 dram 35", readdramoffset1_kvs + chunk1_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd1 bram 35", chunk1_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER1_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE * VECTOR_SIZE; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local1[j] = kvsourcedram[readdramoffset1_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset2_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd2 dram 35", readdramoffset2_kvs + chunk2_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd2 bram 35", chunk2_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER2_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE * VECTOR_SIZE; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local2[j] = kvsourcedram[readdramoffset2_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset3_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd3 dram 35", readdramoffset3_kvs + chunk3_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd3 bram 35", chunk3_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER3_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE * VECTOR_SIZE; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local3[j] = kvsourcedram[readdramoffset3_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset4_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd4 dram 35", readdramoffset4_kvs + chunk4_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd4 bram 35", chunk4_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER4_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE * VECTOR_SIZE; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local4[j] = kvsourcedram[readdramoffset4_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset5_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd5 dram 35", readdramoffset5_kvs + chunk5_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd5 bram 35", chunk5_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER5_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE * VECTOR_SIZE; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local5[j] = kvsourcedram[readdramoffset5_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset6_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd6 dram 35", readdramoffset6_kvs + chunk6_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd6 bram 35", chunk6_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER6_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE * VECTOR_SIZE; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local6[j] = kvsourcedram[readdramoffset6_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset7_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd7 dram 35", readdramoffset7_kvs + chunk7_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd7 bram 35", chunk7_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER7_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE * VECTOR_SIZE; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local7[j] = kvsourcedram[readdramoffset7_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
				// reduce 
				/* REDUCEPHASE_REDUCE_LOOP2: for(buffer_type k=0; k<BUFFER_SIZE * VECTOR_SIZE; k++){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
				#pragma HLS PIPELINE II=1					
					keyvalue_t keyvalue0 = getkeyvalue(v1_local0, k, BUFFER_SIZE);
					keyvalue_t keyvalue1 = getkeyvalue(v1_local1, k, BUFFER_SIZE);
					keyvalue_t keyvalue2 = getkeyvalue(v1_local2, k, BUFFER_SIZE);
					keyvalue_t keyvalue3 = getkeyvalue(v1_local3, k, BUFFER_SIZE);
					keyvalue_t keyvalue4 = getkeyvalue(v1_local4, k, BUFFER_SIZE);
					keyvalue_t keyvalue5 = getkeyvalue(v1_local5, k, BUFFER_SIZE);
					keyvalue_t keyvalue6 = getkeyvalue(v1_local6, k, BUFFER_SIZE);
					keyvalue_t keyvalue7 = getkeyvalue(v1_local7, k, BUFFER_SIZE);
					
					vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
					vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
					vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
					vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
					vertex_t loc4 = keyvalue4.key - globalparams.vbegin;
					vertex_t loc5 = keyvalue5.key - globalparams.vbegin;
					vertex_t loc6 = keyvalue6.key - globalparams.vbegin;
					vertex_t loc7 = keyvalue7.key - globalparams.vbegin;
					
					if(loc0 >= BUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
					if(loc1 >= BUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
					if(loc2 >= BUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
					if(loc3 >= BUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
					if(loc4 >= BUFFER_SIZE * VECTOR_SIZE){ loc4 = 0; } // REMOVEME.
					if(loc5 >= BUFFER_SIZE * VECTOR_SIZE){ loc5 = 0; } // REMOVEME.
					if(loc6 >= BUFFER_SIZE * VECTOR_SIZE){ loc6 = 0; } // REMOVEME.
					if(loc7 >= BUFFER_SIZE * VECTOR_SIZE){ loc7 = 0; } // REMOVEME.
					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("legion::reduce_ddr0_v 34", loc0, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("legion::reduce_ddr0_v 34", loc1, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("legion::reduce_ddr0_v 34", loc2, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("legion::reduce_ddr0_v 34", loc3, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("legion::reduce_ddr0_v 34", loc4, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("legion::reduce_ddr0_v 34", loc5, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("legion::reduce_ddr0_v 34", loc6, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("legion::reduce_ddr0_v 34", loc7, BUFFER_SIZE * VECTOR_SIZE);
					#endif
					
					keyvalue_t vprop0 = getkeyvalue(result_local0, loc0, BUFFER_SIZE);
					keyvalue_t vprop1 = getkeyvalue(result_local1, loc1, BUFFER_SIZE);
					keyvalue_t vprop2 = getkeyvalue(result_local2, loc2, BUFFER_SIZE);
					keyvalue_t vprop3 = getkeyvalue(result_local3, loc3, BUFFER_SIZE);
					keyvalue_t vprop4 = getkeyvalue(result_local4, loc4, BUFFER_SIZE);
					keyvalue_t vprop5 = getkeyvalue(result_local5, loc5, BUFFER_SIZE);
					keyvalue_t vprop6 = getkeyvalue(result_local6, loc6, BUFFER_SIZE);
					keyvalue_t vprop7 = getkeyvalue(result_local7, loc7, BUFFER_SIZE);
					
					value_t temp0 = reducefunc(vprop0.key, vprop0.value, keyvalue0.value, globalparams.IterCount);
					value_t temp1 = reducefunc(vprop1.key, vprop1.value, keyvalue1.value, globalparams.IterCount);
					value_t temp2 = reducefunc(vprop2.key, vprop2.value, keyvalue2.value, globalparams.IterCount);
					value_t temp3 = reducefunc(vprop3.key, vprop3.value, keyvalue3.value, globalparams.IterCount);
					value_t temp4 = reducefunc(vprop4.key, vprop4.value, keyvalue4.value, globalparams.IterCount);
					value_t temp5 = reducefunc(vprop5.key, vprop5.value, keyvalue5.value, globalparams.IterCount);
					value_t temp6 = reducefunc(vprop6.key, vprop6.value, keyvalue6.value, globalparams.IterCount);
					value_t temp7 = reducefunc(vprop7.key, vprop7.value, keyvalue7.value, globalparams.IterCount);
					
					vprop0.value = temp0;
					vprop1.value = temp1;
					vprop2.value = temp2;
					vprop3.value = temp3;
					vprop4.value = temp4;
					vprop5.value = temp5;
					vprop6.value = temp6;
					vprop7.value = temp7;
					
					setkeyvalue(result_local0, loc0, vprop0, BUFFER_SIZE);	
					setkeyvalue(result_local1, loc1, vprop1, BUFFER_SIZE);	
					setkeyvalue(result_local2, loc2, vprop2, BUFFER_SIZE);	
					setkeyvalue(result_local3, loc3, vprop3, BUFFER_SIZE);	
					setkeyvalue(result_local4, loc4, vprop4, BUFFER_SIZE);	
					setkeyvalue(result_local5, loc5, vprop5, BUFFER_SIZE);	
					setkeyvalue(result_local6, loc6, vprop6, BUFFER_SIZE);	
					setkeyvalue(result_local7, loc7, vprop7, BUFFER_SIZE);	
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreduced(1 * NUMSUBWORKERS);
					#endif
				} */
			}
			//burst write the result
			reducephase_out_write:
				batch_type dramsaveoffset0_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write0 dram 35", dramsaveoffset0_kvs + BUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_SAVEBUFFER0_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset0_kvs + j] = result_local0[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}
				batch_type dramsaveoffset1_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write1 dram 35", dramsaveoffset1_kvs + BUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_SAVEBUFFER1_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset1_kvs + j] = result_local1[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}
				batch_type dramsaveoffset2_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write2 dram 35", dramsaveoffset2_kvs + BUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_SAVEBUFFER2_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset2_kvs + j] = result_local2[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}
				batch_type dramsaveoffset3_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write3 dram 35", dramsaveoffset3_kvs + BUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_SAVEBUFFER3_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset3_kvs + j] = result_local3[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}
				batch_type dramsaveoffset4_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write4 dram 35", dramsaveoffset4_kvs + BUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_SAVEBUFFER4_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset4_kvs + j] = result_local4[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}
				batch_type dramsaveoffset5_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write5 dram 35", dramsaveoffset5_kvs + BUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_SAVEBUFFER5_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset5_kvs + j] = result_local5[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}
				batch_type dramsaveoffset6_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write6 dram 35", dramsaveoffset6_kvs + BUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_SAVEBUFFER6_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset6_kvs + j] = result_local6[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}
				batch_type dramsaveoffset7_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write7 dram 35", dramsaveoffset7_kvs + BUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_SAVEBUFFER7_LOOP1: for (buffer_type j=0; j<BUFFER_SIZE; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset7_kvs + j] = result_local7[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}

			llopparams.nextsourceoffset_partition += NUMWORKERS * NUMSUBWORKERS;
			llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv;
			break;
		}
		#ifdef _LDEBUGMODE_HEADER
		printglobalvars();
		printparameters();
		clearglobalvars();
		#endif
	}
	return;
}

extern "C" {
void enigma::topkernel(
uint512_dt * kvsourcedramA
		,uint512_dt * kvdestdramA
		,keyvalue_t * kvstatsA
        ){
#pragma HLS INTERFACE m_axi port=kvsourcedramA  offset=slave bundle=gmem0 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=kvdestdramA  offset=slave bundle=gmem0 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=kvstatsA  offset=slave bundle=gmem1

#pragma HLS INTERFACE s_axilite port=kvsourcedramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvdestdramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvstatsA  bundle=control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable=kvstatsA
	
	globalparams_t globalparams;
	globalparams.processcommand = kvstatsA[getmessagesAddr(MESSAGES_PROCESSCOMMANDID)].key;
	globalparams.partitioncommand = kvstatsA[getmessagesAddr(MESSAGES_PARTITIONCOMMANDID)].key;
	globalparams.reducecommand = kvstatsA[getmessagesAddr(MESSAGES_APPLYUPDATESCOMMANDID)].key;
	globalparams.vbegin = kvstatsA[getmessagesAddr(DRAM_VOFFSET)].key; 
	globalparams.vsize = kvstatsA[getmessagesAddr(DRAM_VSIZE)].key; 
	globalparams.treedepth = kvstatsA[getmessagesAddr(DRAM_TREEDEPTH)].key; 
	globalparams.LLOPnumpartitions = kvstatsA[getmessagesAddr(DRAM_FINALNUMPARTITIONS)].key;
	globalparams.IterCount = kvstatsA[getmessagesAddr(GRAPH_ITERATIONID)].key;
	globalparams.baseaddr_destkvs_kvs = 0;
	
	#ifdef _LDEBUGMODE_HEADER
	std::cout<<std::endl;
	std::cout<<"Kernel Started: globalparams.processcommand: "<<globalparams.processcommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.partitioncommand: "<<globalparams.partitioncommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.reducecommand: "<<globalparams.reducecommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.vbegin: "<<globalparams.vbegin<<std::endl;
	std::cout<<"Kernel Started: globalparams.vsize: "<<globalparams.vsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.treedepth: "<<globalparams.treedepth<<std::endl;
	std::cout<<"Kernel Started: globalparams.LLOPnumpartitions: "<<globalparams.LLOPnumpartitions<<std::endl;
	std::cout<<"Kernel Started: globalparams.IterCount: "<<globalparams.IterCount<<std::endl;
	#endif
	
	generatepartitions0(kvsourcedramA, kvdestdramA, kvstatsA, globalparams);
	
	/** 	reducepartitions0(kvsourcedramA, kvstatsA, globalparams);
 */
	return;
}
}





