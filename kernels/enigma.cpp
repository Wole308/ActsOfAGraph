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

#ifndef FPGA_IMPL
enigma::enigma(){}
enigma::~enigma(){}
#endif
#ifndef HW

unsigned int globalvar_totalkvstatsread;
unsigned int globalvar_totalkvsreadV;

unsigned int globalvar_totalkvsread4statscollection;
unsigned int globalvar_totalkvstatswritten;

unsigned int globalvar_totalkvsread;
unsigned int globalstats_totalkvswritten;

unsigned int globalstats_totalkvspartitioned;
unsigned int globalstats_totalkvspartitionswritten;

unsigned int globalstats_totalkvsreduced;
unsigned int globalstats_totalkvsreducewritten;
#endif 
#ifdef _LDEBUGMODE_HEADER
void enigma::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data >= upper_bound){ std::cout<<"enigma::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }
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
	cout<<"enigma::printparameters:: PADDEDBUFFER_SIZE: "<<PADDEDBUFFER_SIZE<<endl;
	
	cout<<"enigma::printparameters:: KVDRAMSZ: "<<KVDRAMSZ<<endl;
	cout<<"enigma::printparameters:: KVDRAMSZ_KVS: "<<KVDRAMSZ_KVS<<endl;
	
	cout<<"enigma::printparameters:: KVSOURCEDRAMSZ: "<<KVSOURCEDRAMSZ<<endl;
	cout<<"enigma::printparameters:: KVSOURCEDRAMSZ_KVS: "<<KVSOURCEDRAMSZ_KVS<<endl;
	
	cout<<"enigma::printparameters:: PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;
	cout<<"enigma::printparameters:: PADDEDKVSOURCEDRAMSZ_KVS: "<<PADDEDKVSOURCEDRAMSZ_KVS<<endl;
	
	cout<<"enigma::printparameters:: BUFFER_SIZE: "<<BUFFER_SIZE<<endl;
	cout<<"enigma::printparameters:: PADDEDBUFFER_SIZE: "<<PADDEDBUFFER_SIZE<<endl;
	
	cout<<"enigma::printparameters:: SRCBUFFER_SIZE: "<<SRCBUFFER_SIZE<<endl;
	cout<<"enigma::printparameters:: SRCBUFFER_SIZE * VECTOR_SIZE: "<<SRCBUFFER_SIZE * VECTOR_SIZE<<endl;
	
	cout<<"enigma::printparameters:: DESTBUFFER_SIZE: "<<DESTBUFFER_SIZE<<endl;
	cout<<"enigma::printparameters:: PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;
	cout<<"enigma::printparameters:: PADDEDDESTBUFFER_SIZE * VECTOR_SIZE: "<<PADDEDDESTBUFFER_SIZE * VECTOR_SIZE<<endl;
	
	cout<<"enigma::printparameters:: PADDEDBUFFER_SIZE / NUM_PARTITIONS: "<<PADDEDBUFFER_SIZE / NUM_PARTITIONS<<endl;
	
	cout<<"enigma::printparameters:: NFACTOR: "<<NFACTOR<<endl;
	
	cout<<"enigma::printparameters:: PADDEDDESTBUFFER_SIZE * NUMSUBWORKERS (total dest_kvs memory): "<<(PADDEDDESTBUFFER_SIZE * NUMSUBWORKERS)<<endl;
	cout<<"enigma::printparameters:: PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * NUMSUBWORKERS (total dest memory): "<<(PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * NUMSUBWORKERS)<<endl;
}
#endif
#ifdef _DEBUGMODE_STATS
void enigma::printglobalvars(){
	cout<<"printglobalvars: "<<endl;
	cout<<"enigma::printglobalvars:: globalvar_totalkvsread: "<<globalvar_totalkvsread<<endl;
	cout<<"enigma::printglobalvars:: globalvar_totalkvsreadV: "<<globalvar_totalkvsreadV<<endl;
	
	cout<<"enigma::printglobalvars:: globalstats_totalkvswritten: "<<globalstats_totalkvswritten<<endl;
	
	cout<<"enigma::printglobalvars:: globalvar_totalkvstatsread: "<<globalvar_totalkvstatsread<<endl;
	cout<<"enigma::printglobalvars:: globalvar_totalkvsread4statscollection: "<<globalvar_totalkvsread4statscollection<<endl;
	cout<<"enigma::printglobalvars:: globalvar_totalkvstatswritten: "<<globalvar_totalkvstatswritten<<endl;	

	cout<<"enigma::printglobalvars:: globalstats_totalkvspartitioned: "<<globalstats_totalkvspartitioned<<endl;
	cout<<"enigma::printglobalvars:: globalstats_totalkvspartitionswritten: "<<globalstats_totalkvspartitionswritten<<endl;
	
	cout<<"enigma::printglobalvars:: globalstats_totalkvsreduced: "<<globalstats_totalkvsreduced<<endl;
	cout<<"enigma::printglobalvars:: globalstats_totalkvsreducewritten: "<<globalstats_totalkvsreducewritten<<endl;
}
void enigma::clearglobalvars(){
	cout<<"clearglobalvars: "<<endl;
	globalvar_totalkvsread = 0;
	globalstats_totalkvswritten = 0;
	globalstats_totalkvspartitionswritten = 0;
	globalstats_totalkvspartitioned = 0;
	globalvar_totalkvstatsread = 0;
	globalvar_totalkvsread4statscollection = 0;
	globalvar_totalkvstatswritten = 0;
	globalstats_totalkvsreduced = 0;
	globalstats_totalkvsreducewritten = 0;
	globalvar_totalkvsreadV = 0;
}
void enigma::globalstats_countkvstatsread(unsigned int count){
	globalvar_totalkvstatsread += count;
	return;
}
void enigma::globalvar_countkvsread4statscollection(unsigned int count){
	globalvar_totalkvsread4statscollection += count;
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
void enigma::globalstats_countkvswritten(unsigned int count){
	globalstats_totalkvswritten += count;
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
void enigma::globalstats_countkvsreadV(unsigned int count){
	globalvar_totalkvsreadV += count;
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
	partition_type val = ((keyvalue.key - upperlimit) >> (KVDATA_RANGE_PERSSDPARTITION_POW - (NUM_PARTITIONS_POW * currentLOP)));
	partition_type hashedval = (((1 << NUM_PARTITIONS_POW) - 1) & (val >> (1 - 1)));
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::getpartition 35", hashedval, NUM_PARTITIONS, NAp, NAp, NAp);
	#endif
	return hashedval;
}
keyvalue_t enigma::getkeyvalue(uint512_dt buffer[BUFFER_SIZE], batch_type addr, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	if(addr >= (maxaddr_kvs * VECTOR_SIZE)){ addr = 0; } // FIXME. FORCEDFINISH
	keyvalue_t keyvalue;
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	if(idy >= maxaddr_kvs){ cout<<"getkeyvalue:: ERROR 45. addr: "<<addr<<", idy: "<<idy<<", idx: "<<idx<<endl; exit(EXIT_FAILURE); }
	checkoutofbounds("enigma::getkeyvalue 35", idy, maxaddr_kvs, NAp, NAp, NAp);
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
	else {}
	#else
	keyvalue = buffer[idy].data[idx];
	#endif
	return keyvalue;
}
void enigma::setkeyvalue(uint512_dt buffer[BUFFER_SIZE], batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	if(addr >= (maxaddr_kvs * VECTOR_SIZE)){ addr = 0; } // FIXME. FORCEDFINISH
	batch_type idy = (addr / VECTOR_SIZE);
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::setkeyvalue 35", idy, maxaddr_kvs, addr, NAp, NAp);
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
	else {}
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
	unsigned int treedepth = TREE_DEPTH;
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}

void enigma::calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS]){
	#pragma HLS INLINE
	capsule[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){ capsule[p].key = capsule[p-1].key + capsule[p-1].value; }
	return;
}
void enigma::calculateoffsets(keyvalue_t capsule0[NUM_PARTITIONS],keyvalue_t capsule1[NUM_PARTITIONS],keyvalue_t capsule2[NUM_PARTITIONS],keyvalue_t capsule3[NUM_PARTITIONS], int dummy){
	#pragma HLS INLINE
	capsule0[0].key = 0;
	capsule1[0].key = 0;
	capsule2[0].key = 0;
	capsule3[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){
		capsule0[p].key = capsule0[p-1].key + capsule0[p-1].value;
		capsule1[p].key = capsule1[p-1].key + capsule1[p-1].value;
		capsule2[p].key = capsule2[p-1].key + capsule2[p-1].value;
		capsule3[p].key = capsule3[p-1].key + capsule3[p-1].value;
	}
	return;
}

void enigma::loadclopparams(globalparams_t globalparams, clopparams_t * llopparams, unsigned int currentLOP){
	#pragma HLS INLINE
	if((currentLOP % 2) == 1){ llopparams->baseaddr_worksourcekvs_kvs = BASEOFFSET_KVDRAM_KVS; llopparams->baseaddr_workdestkvs_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; }
	else { llopparams->baseaddr_worksourcekvs_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; llopparams->baseaddr_workdestkvs_kvs = BASEOFFSET_KVDRAM_KVS; }	
	llopparams->baseaddr_capsule_kvs = BASEOFFSET_CAPSULES_KVS;
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
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
value_t enigma::reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int GraphIter){
	#pragma HLS INLINE
	value_t ret = 0;
	#ifdef PR_ALGORITHM
	ret = value + edgeval;
	#elif defined(BFS_ALGORITHM)
	ret = min(value, GraphIter);
	#elif defined(BC_ALGORITHM)
	ret = min(value, GraphIter);
	#endif
	return ret;
}
void enigma::getchunksize(unsigned int workerID, buffer_type * chunk_size, buffer_type buffer_size, travstate_t travstate){
	#pragma HLS INLINE
	*chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + (workerID * buffer_size);
	if (i > travstate.end_kvs){ *chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ *chunk_size = travstate.end_kvs - i; }
	else {}
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("getchunksize 35", *chunk_size, buffer_size + 1, NAp, NAp, NAp);
	checkoutofbounds("getchunksize 36", i + *chunk_size, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	return;
}
void enigma::getchunksizes(unsigned int workerID , buffer_type * chunk0_size, buffer_type * chunk1_size, buffer_type * chunk2_size, buffer_type * chunk3_size, buffer_type buffer_size, travstate_t travstate){
	#pragma HLS INLINE
	*chunk0_size = buffer_size;
	batch_type i0 = travstate.i_kvs + (((workerID * NUMSUBWORKERS) + 0) * buffer_size);
	if (i0 > travstate.end_kvs){ *chunk0_size = 0; }
	else if ((i0 + buffer_size) > travstate.end_kvs){ *chunk0_size = travstate.end_kvs - i0; }
	else {}
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("getchunksizes 34", *chunk0_size, buffer_size + 1, NAp, NAp, NAp);
	checkoutofbounds("getchunksizes 35", i0 + *chunk0_size, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	*chunk1_size = buffer_size;
	batch_type i1 = travstate.i_kvs + (((workerID * NUMSUBWORKERS) + 1) * buffer_size);
	if (i1 > travstate.end_kvs){ *chunk1_size = 0; }
	else if ((i1 + buffer_size) > travstate.end_kvs){ *chunk1_size = travstate.end_kvs - i1; }
	else {}
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("getchunksizes 34", *chunk1_size, buffer_size + 1, NAp, NAp, NAp);
	checkoutofbounds("getchunksizes 35", i1 + *chunk1_size, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	*chunk2_size = buffer_size;
	batch_type i2 = travstate.i_kvs + (((workerID * NUMSUBWORKERS) + 2) * buffer_size);
	if (i2 > travstate.end_kvs){ *chunk2_size = 0; }
	else if ((i2 + buffer_size) > travstate.end_kvs){ *chunk2_size = travstate.end_kvs - i2; }
	else {}
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("getchunksizes 34", *chunk2_size, buffer_size + 1, NAp, NAp, NAp);
	checkoutofbounds("getchunksizes 35", i2 + *chunk2_size, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	*chunk3_size = buffer_size;
	batch_type i3 = travstate.i_kvs + (((workerID * NUMSUBWORKERS) + 3) * buffer_size);
	if (i3 > travstate.end_kvs){ *chunk3_size = 0; }
	else if ((i3 + buffer_size) > travstate.end_kvs){ *chunk3_size = travstate.end_kvs - i3; }
	else {}
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("getchunksizes 34", *chunk3_size, buffer_size + 1, NAp, NAp, NAp);
	checkoutofbounds("getchunksizes 35", i3 + *chunk3_size, PADDEDKVSOURCEDRAMSZ_KVS + 1, NAp, NAp, NAp);
	#endif
	return;
}
int enigma::IsReducePhase(unsigned int currentLOP, unsigned int treedepth, globalparams_t globalparams){
	if((currentLOP == (treedepth + 1)) && (globalparams.reducecommand == ON)){ return 1; }
	return 0;
}

void enigma::readkeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	// #pragma HLS INLINE // FIXME. remove pragma?
	unsigned int analysis_buffersz = SRCBUFFER_SIZE;
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::readkeyvalues dram 36", baseaddress + offset_kvs + size_kvs, maxaddress_kvs, baseaddress, offset_kvs, size_kvs);
	#endif
	READBUFFER_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	   #pragma HLS PIPELINE II=1
	   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz	
		buffer[i] = kvdram[baseaddress + offset_kvs + i];
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}
void enigma::readkeyvalues1(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	// #pragma HLS INLINE // FIXME. remove pragma?
	unsigned int analysis_buffersz = SRCBUFFER_SIZE;
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::readkeyvalues dram 36", baseaddress + offset_kvs + size_kvs, maxaddress_kvs, baseaddress, offset_kvs, size_kvs);
	#endif
	READBUFFER_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	   #pragma HLS PIPELINE II=1
	   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz	
		buffer[i] = kvdram[baseaddress + offset_kvs + i];
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}
void enigma::readkeyvalues2(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	// #pragma HLS INLINE // FIXME. remove pragma?
	unsigned int analysis_buffersz = SRCBUFFER_SIZE;
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::readkeyvalues dram 36", baseaddress + offset_kvs + size_kvs, maxaddress_kvs, baseaddress, offset_kvs, size_kvs);
	#endif
	READBUFFER_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	   #pragma HLS PIPELINE II=1
	   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz	
		buffer[i] = kvdram[baseaddress + offset_kvs + i];
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}
void enigma::readkeyvalues3(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	// #pragma HLS INLINE // FIXME. remove pragma?
	unsigned int analysis_buffersz = SRCBUFFER_SIZE;
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::readkeyvalues dram 36", baseaddress + offset_kvs + size_kvs, maxaddress_kvs, baseaddress, offset_kvs, size_kvs);
	#endif
	READBUFFER_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	   #pragma HLS PIPELINE II=1
	   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz	
		buffer[i] = kvdram[baseaddress + offset_kvs + i];
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}
travoffsets_t enigma::readcapsules0(uint512_dt * kvdram,  uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE], keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  batch_type baseaddress_kvs, batch_type offset_kvs, travoffsets_t travoffsets){
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::readcapsules0 35", (baseaddress_kvs + offset_kvs + NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	if(loffset == 0){
		batch_type loadoffset0_kvs = travoffsets.capsuleoffset_kvs + 0 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset0_kvs: "<<loadoffset0_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules0 34", baseaddress_kvs + loadoffset0_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule0, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset1_kvs = travoffsets.capsuleoffset_kvs + 1 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset1_kvs: "<<loadoffset1_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules0 34", baseaddress_kvs + loadoffset1_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset2_kvs = travoffsets.capsuleoffset_kvs + 2 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset2_kvs: "<<loadoffset2_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules0 34", baseaddress_kvs + loadoffset2_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule2, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset3_kvs = travoffsets.capsuleoffset_kvs + 3 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset3_kvs: "<<loadoffset3_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules0 34", baseaddress_kvs + loadoffset3_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule3, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		travoffsets.capsuleoffset_kvs += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	
	READCAPSULE0_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		capsule0[i] = getkeyvalue(BIGcapsule0, loffset + i, CAPSULEBUFFER_SIZE);
		capsule1[i] = getkeyvalue(BIGcapsule1, loffset + i, CAPSULEBUFFER_SIZE);
		capsule2[i] = getkeyvalue(BIGcapsule2, loffset + i, CAPSULEBUFFER_SIZE);
		capsule3[i] = getkeyvalue(BIGcapsule3, loffset + i, CAPSULEBUFFER_SIZE);
	}
	return travoffsets;
}
travoffsets_t enigma::readcapsules1(uint512_dt * kvdram,  uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE], keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  batch_type baseaddress_kvs, batch_type offset_kvs, travoffsets_t travoffsets){
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::readcapsules1 35", (baseaddress_kvs + offset_kvs + NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	if(loffset == 0){
		batch_type loadoffset0_kvs = travoffsets.capsuleoffset_kvs + 0 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset0_kvs: "<<loadoffset0_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules1 34", baseaddress_kvs + loadoffset0_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues1(kvdram, BIGcapsule0, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset1_kvs = travoffsets.capsuleoffset_kvs + 1 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset1_kvs: "<<loadoffset1_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules1 34", baseaddress_kvs + loadoffset1_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues1(kvdram, BIGcapsule1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset2_kvs = travoffsets.capsuleoffset_kvs + 2 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset2_kvs: "<<loadoffset2_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules1 34", baseaddress_kvs + loadoffset2_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues1(kvdram, BIGcapsule2, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset3_kvs = travoffsets.capsuleoffset_kvs + 3 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset3_kvs: "<<loadoffset3_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules1 34", baseaddress_kvs + loadoffset3_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues1(kvdram, BIGcapsule3, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		travoffsets.capsuleoffset_kvs += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	
	READCAPSULE1_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		capsule0[i] = getkeyvalue(BIGcapsule0, loffset + i, CAPSULEBUFFER_SIZE);
		capsule1[i] = getkeyvalue(BIGcapsule1, loffset + i, CAPSULEBUFFER_SIZE);
		capsule2[i] = getkeyvalue(BIGcapsule2, loffset + i, CAPSULEBUFFER_SIZE);
		capsule3[i] = getkeyvalue(BIGcapsule3, loffset + i, CAPSULEBUFFER_SIZE);
	}
	return travoffsets;
}
travoffsets_t enigma::readcapsules2(uint512_dt * kvdram,  uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE], keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  batch_type baseaddress_kvs, batch_type offset_kvs, travoffsets_t travoffsets){
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::readcapsules2 35", (baseaddress_kvs + offset_kvs + NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	if(loffset == 0){
		batch_type loadoffset0_kvs = travoffsets.capsuleoffset_kvs + 0 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset0_kvs: "<<loadoffset0_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules2 34", baseaddress_kvs + loadoffset0_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues2(kvdram, BIGcapsule0, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset1_kvs = travoffsets.capsuleoffset_kvs + 1 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset1_kvs: "<<loadoffset1_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules2 34", baseaddress_kvs + loadoffset1_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues2(kvdram, BIGcapsule1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset2_kvs = travoffsets.capsuleoffset_kvs + 2 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset2_kvs: "<<loadoffset2_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules2 34", baseaddress_kvs + loadoffset2_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues2(kvdram, BIGcapsule2, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset3_kvs = travoffsets.capsuleoffset_kvs + 3 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset3_kvs: "<<loadoffset3_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules2 34", baseaddress_kvs + loadoffset3_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues2(kvdram, BIGcapsule3, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		travoffsets.capsuleoffset_kvs += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	
	READCAPSULE2_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		capsule0[i] = getkeyvalue(BIGcapsule0, loffset + i, CAPSULEBUFFER_SIZE);
		capsule1[i] = getkeyvalue(BIGcapsule1, loffset + i, CAPSULEBUFFER_SIZE);
		capsule2[i] = getkeyvalue(BIGcapsule2, loffset + i, CAPSULEBUFFER_SIZE);
		capsule3[i] = getkeyvalue(BIGcapsule3, loffset + i, CAPSULEBUFFER_SIZE);
	}
	return travoffsets;
}
travoffsets_t enigma::readcapsules3(uint512_dt * kvdram,  uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE], keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  batch_type baseaddress_kvs, batch_type offset_kvs, travoffsets_t travoffsets){
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::readcapsules3 35", (baseaddress_kvs + offset_kvs + NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	if(loffset == 0){
		batch_type loadoffset0_kvs = travoffsets.capsuleoffset_kvs + 0 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset0_kvs: "<<loadoffset0_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules3 34", baseaddress_kvs + loadoffset0_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues3(kvdram, BIGcapsule0, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset1_kvs = travoffsets.capsuleoffset_kvs + 1 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset1_kvs: "<<loadoffset1_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules3 34", baseaddress_kvs + loadoffset1_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues3(kvdram, BIGcapsule1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset2_kvs = travoffsets.capsuleoffset_kvs + 2 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset2_kvs: "<<loadoffset2_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules3 34", baseaddress_kvs + loadoffset2_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues3(kvdram, BIGcapsule2, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset3_kvs = travoffsets.capsuleoffset_kvs + 3 * CAPSULEBUFFER_SIZE; 
		// cout<<"READ HERE: readcapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lloadbase_kvs: "<<NAp<<", loadoffset3_kvs: "<<loadoffset3_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules3 34", baseaddress_kvs + loadoffset3_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues3(kvdram, BIGcapsule3, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		travoffsets.capsuleoffset_kvs += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	
	READCAPSULE3_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		capsule0[i] = getkeyvalue(BIGcapsule0, loffset + i, CAPSULEBUFFER_SIZE);
		capsule1[i] = getkeyvalue(BIGcapsule1, loffset + i, CAPSULEBUFFER_SIZE);
		capsule2[i] = getkeyvalue(BIGcapsule2, loffset + i, CAPSULEBUFFER_SIZE);
		capsule3[i] = getkeyvalue(BIGcapsule3, loffset + i, CAPSULEBUFFER_SIZE);
	}
	return travoffsets;
}
			
void enigma::extractcapsules00(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	buffer_type sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	buffer_type sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	buffer_type sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	buffer_type sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	EXTRACTCAPSULES0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p] = 0;
		sizes1[p] = 0;
		sizes2[p] = 0;
		sizes3[p] = 0;
	}
	EXTRACTCAPSULES0_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
			partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
			sizes0[p0] += 1;
			keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
			partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
			sizes1[p1] += 1;
			keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
			partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
			sizes2[p2] += 1;
			keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
			partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
			sizes3[p3] += 1;
		#ifdef _DEBUGMODE_STATS
		globalvar_countkvsread4statscollection(NUMSUBWORKERS);
		#endif
	}
	EXTRACTCAPSULES0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule0[p].key = 0;
		capsule0[p].value += sizes0[p];
		capsule1[p].key = 0;
		capsule1[p].value += sizes1[p];
		capsule2[p].key = 0;
		capsule2[p].value += sizes2[p];
		capsule3[p].key = 0;
		capsule3[p].value += sizes3[p];
	}
	return;
}
void enigma::extractcapsules10(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	buffer_type sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	buffer_type sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	buffer_type sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	buffer_type sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	EXTRACTCAPSULES1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p] = 0;
		sizes1[p] = 0;
		sizes2[p] = 0;
		sizes3[p] = 0;
	}
	EXTRACTCAPSULES1_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
			partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
			sizes0[p0] += 1;
			keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
			partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
			sizes1[p1] += 1;
			keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
			partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
			sizes2[p2] += 1;
			keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
			partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
			sizes3[p3] += 1;
		#ifdef _DEBUGMODE_STATS
		globalvar_countkvsread4statscollection(NUMSUBWORKERS);
		#endif
	}
	EXTRACTCAPSULES1_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule0[p].key = 0;
		capsule0[p].value += sizes0[p];
		capsule1[p].key = 0;
		capsule1[p].value += sizes1[p];
		capsule2[p].key = 0;
		capsule2[p].value += sizes2[p];
		capsule3[p].key = 0;
		capsule3[p].value += sizes3[p];
	}
	return;
}
void enigma::extractcapsules20(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	buffer_type sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	buffer_type sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	buffer_type sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	buffer_type sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	EXTRACTCAPSULES2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p] = 0;
		sizes1[p] = 0;
		sizes2[p] = 0;
		sizes3[p] = 0;
	}
	EXTRACTCAPSULES2_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
			partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
			sizes0[p0] += 1;
			keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
			partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
			sizes1[p1] += 1;
			keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
			partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
			sizes2[p2] += 1;
			keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
			partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
			sizes3[p3] += 1;
		#ifdef _DEBUGMODE_STATS
		globalvar_countkvsread4statscollection(NUMSUBWORKERS);
		#endif
	}
	EXTRACTCAPSULES2_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule0[p].key = 0;
		capsule0[p].value += sizes0[p];
		capsule1[p].key = 0;
		capsule1[p].value += sizes1[p];
		capsule2[p].key = 0;
		capsule2[p].value += sizes2[p];
		capsule3[p].key = 0;
		capsule3[p].value += sizes3[p];
	}
	return;
}
void enigma::extractcapsules30(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	buffer_type sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	buffer_type sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	buffer_type sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	buffer_type sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	EXTRACTCAPSULES3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p] = 0;
		sizes1[p] = 0;
		sizes2[p] = 0;
		sizes3[p] = 0;
	}
	EXTRACTCAPSULES3_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
			partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
			sizes0[p0] += 1;
			keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
			partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
			sizes1[p1] += 1;
			keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
			partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
			sizes2[p2] += 1;
			keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
			partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
			sizes3[p3] += 1;
		#ifdef _DEBUGMODE_STATS
		globalvar_countkvsread4statscollection(NUMSUBWORKERS);
		#endif
	}
	EXTRACTCAPSULES3_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule0[p].key = 0;
		capsule0[p].value += sizes0[p];
		capsule1[p].key = 0;
		capsule1[p].value += sizes1[p];
		capsule2[p].key = 0;
		capsule2[p].value += sizes2[p];
		capsule3[p].key = 0;
		capsule3[p].value += sizes3[p];
	}
	return;
}
void enigma::partitionkeyvalues00(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	unsigned int analysis_partition = SRCBUFFER_SIZE;
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete 
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete 
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete 
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete 
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  sizes0[p].key = capsule0[p].key; sizes0[p].value = capsule0[p].value;  sizes1[p].key = capsule1[p].key; sizes1[p].value = capsule1[p].value;  sizes2[p].key = capsule2[p].key; sizes2[p].value = capsule2[p].value;  sizes3[p].key = capsule3[p].key; sizes3[p].value = capsule3[p].value;  }
	PARTITIONPHASE_PARTITION_MAIN: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partition avg=analysis_partition
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);

		partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_CHECKS
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes1[p1].key + sizes1[p1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes1[p1].key, sizes1[p1].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes2[p2].key + sizes2[p2].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes2[p2].key, sizes2[p2].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes3[p3].key + sizes3[p3].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes3[p3].key, sizes3[p3].value, NAp);
		#endif
		
		setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
		
		sizes0[p0].value += 1;
		sizes1[p1].value += 1;
		sizes2[p2].value += 1;
		sizes3[p3].value += 1;
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  capsule1[p].value = sizes1[p].value;  capsule2[p].value = sizes2[p].value;  capsule3[p].value = sizes3[p].value;  }
}
void enigma::partitionkeyvalues10(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	unsigned int analysis_partition = SRCBUFFER_SIZE;
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete 
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete 
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete 
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete 
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  sizes0[p].key = capsule0[p].key; sizes0[p].value = capsule0[p].value;  sizes1[p].key = capsule1[p].key; sizes1[p].value = capsule1[p].value;  sizes2[p].key = capsule2[p].key; sizes2[p].value = capsule2[p].value;  sizes3[p].key = capsule3[p].key; sizes3[p].value = capsule3[p].value;  }
	PARTITIONPHASE_PARTITION_MAIN: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partition avg=analysis_partition
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);

		partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_CHECKS
		checkoutofbounds("enigma::partitionkeyvalues1 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues1 34", sizes1[p1].key + sizes1[p1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes1[p1].key, sizes1[p1].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues1 34", sizes2[p2].key + sizes2[p2].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes2[p2].key, sizes2[p2].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues1 34", sizes3[p3].key + sizes3[p3].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes3[p3].key, sizes3[p3].value, NAp);
		#endif
		
		setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
		
		sizes0[p0].value += 1;
		sizes1[p1].value += 1;
		sizes2[p2].value += 1;
		sizes3[p3].value += 1;
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  capsule1[p].value = sizes1[p].value;  capsule2[p].value = sizes2[p].value;  capsule3[p].value = sizes3[p].value;  }
}
void enigma::partitionkeyvalues20(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	unsigned int analysis_partition = SRCBUFFER_SIZE;
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete 
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete 
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete 
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete 
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  sizes0[p].key = capsule0[p].key; sizes0[p].value = capsule0[p].value;  sizes1[p].key = capsule1[p].key; sizes1[p].value = capsule1[p].value;  sizes2[p].key = capsule2[p].key; sizes2[p].value = capsule2[p].value;  sizes3[p].key = capsule3[p].key; sizes3[p].value = capsule3[p].value;  }
	PARTITIONPHASE_PARTITION_MAIN: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partition avg=analysis_partition
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);

		partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_CHECKS
		checkoutofbounds("enigma::partitionkeyvalues2 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues2 34", sizes1[p1].key + sizes1[p1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes1[p1].key, sizes1[p1].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues2 34", sizes2[p2].key + sizes2[p2].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes2[p2].key, sizes2[p2].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues2 34", sizes3[p3].key + sizes3[p3].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes3[p3].key, sizes3[p3].value, NAp);
		#endif
		
		setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
		
		sizes0[p0].value += 1;
		sizes1[p1].value += 1;
		sizes2[p2].value += 1;
		sizes3[p3].value += 1;
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  capsule1[p].value = sizes1[p].value;  capsule2[p].value = sizes2[p].value;  capsule3[p].value = sizes3[p].value;  }
}
void enigma::partitionkeyvalues30(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	unsigned int analysis_partition = SRCBUFFER_SIZE;
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete 
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete 
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete 
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete 
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  sizes0[p].key = capsule0[p].key; sizes0[p].value = capsule0[p].value;  sizes1[p].key = capsule1[p].key; sizes1[p].value = capsule1[p].value;  sizes2[p].key = capsule2[p].key; sizes2[p].value = capsule2[p].value;  sizes3[p].key = capsule3[p].key; sizes3[p].value = capsule3[p].value;  }
	PARTITIONPHASE_PARTITION_MAIN: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partition avg=analysis_partition
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);

		partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_CHECKS
		checkoutofbounds("enigma::partitionkeyvalues3 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues3 34", sizes1[p1].key + sizes1[p1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes1[p1].key, sizes1[p1].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues3 34", sizes2[p2].key + sizes2[p2].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes2[p2].key, sizes2[p2].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues3 34", sizes3[p3].key + sizes3[p3].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes3[p3].key, sizes3[p3].value, NAp);
		#endif
		
		setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
		
		sizes0[p0].value += 1;
		sizes1[p1].value += 1;
		sizes2[p2].value += 1;
		sizes3[p3].value += 1;
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		globalstats_countkvspartitioned(1); 
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  capsule1[p].value = sizes1[p].value;  capsule2[p].value = sizes2[p].value;  capsule3[p].value = sizes3[p].value;  }
}

void enigma::reducepartitions0( uint512_dt sourcebuffer0[SRCBUFFER_SIZE],  uint512_dt sourcebuffer1[SRCBUFFER_SIZE],  uint512_dt sourcebuffer2[SRCBUFFER_SIZE],  uint512_dt sourcebuffer3[SRCBUFFER_SIZE],   uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE],  travstate_t travstate, globalparams_t globalparams){
	unsigned int analysis_buffersz = SRCBUFFER_SIZE * VECTOR_SIZE;
	
	REDUCEPHASE_REDUCE_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
	#pragma HLS PIPELINE II=1					
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
		
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr0_v 34", loc0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr0_v 34", loc1, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr0_v 34", loc2, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr0_v 34", loc3, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		#endif
		
		keyvalue_t vprop0 = getkeyvalue(destbuffer0, loc0, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop1 = getkeyvalue(destbuffer1, loc1, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop2 = getkeyvalue(destbuffer2, loc2, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop3 = getkeyvalue(destbuffer3, loc3, PADDEDDESTBUFFER_SIZE);
		
		value_t temp0 = reducefunc(vprop0.key, vprop0.value, keyvalue0.value, globalparams.GraphIter);
		value_t temp1 = reducefunc(vprop1.key, vprop1.value, keyvalue1.value, globalparams.GraphIter);
		value_t temp2 = reducefunc(vprop2.key, vprop2.value, keyvalue2.value, globalparams.GraphIter);
		value_t temp3 = reducefunc(vprop3.key, vprop3.value, keyvalue3.value, globalparams.GraphIter);
		
		vprop0.value = temp0;
		vprop1.value = temp1;
		vprop2.value = temp2;
		vprop3.value = temp3;
		
		setkeyvalue(destbuffer0, loc0, vprop0, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer1, loc1, vprop1, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer2, loc2, vprop2, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer3, loc3, vprop3, PADDEDDESTBUFFER_SIZE);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}
void enigma::reducepartitions1( uint512_dt sourcebuffer0[SRCBUFFER_SIZE],  uint512_dt sourcebuffer1[SRCBUFFER_SIZE],  uint512_dt sourcebuffer2[SRCBUFFER_SIZE],  uint512_dt sourcebuffer3[SRCBUFFER_SIZE],   uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE],  travstate_t travstate, globalparams_t globalparams){
	unsigned int analysis_buffersz = SRCBUFFER_SIZE * VECTOR_SIZE;
	
	REDUCEPHASE_REDUCE_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
	#pragma HLS PIPELINE II=1					
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
		
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr1_v 34", loc0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr1_v 34", loc1, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr1_v 34", loc2, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr1_v 34", loc3, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		#endif
		
		keyvalue_t vprop0 = getkeyvalue(destbuffer0, loc0, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop1 = getkeyvalue(destbuffer1, loc1, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop2 = getkeyvalue(destbuffer2, loc2, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop3 = getkeyvalue(destbuffer3, loc3, PADDEDDESTBUFFER_SIZE);
		
		value_t temp0 = reducefunc(vprop0.key, vprop0.value, keyvalue0.value, globalparams.GraphIter);
		value_t temp1 = reducefunc(vprop1.key, vprop1.value, keyvalue1.value, globalparams.GraphIter);
		value_t temp2 = reducefunc(vprop2.key, vprop2.value, keyvalue2.value, globalparams.GraphIter);
		value_t temp3 = reducefunc(vprop3.key, vprop3.value, keyvalue3.value, globalparams.GraphIter);
		
		vprop0.value = temp0;
		vprop1.value = temp1;
		vprop2.value = temp2;
		vprop3.value = temp3;
		
		setkeyvalue(destbuffer0, loc0, vprop0, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer1, loc1, vprop1, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer2, loc2, vprop2, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer3, loc3, vprop3, PADDEDDESTBUFFER_SIZE);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}
void enigma::reducepartitions2( uint512_dt sourcebuffer0[SRCBUFFER_SIZE],  uint512_dt sourcebuffer1[SRCBUFFER_SIZE],  uint512_dt sourcebuffer2[SRCBUFFER_SIZE],  uint512_dt sourcebuffer3[SRCBUFFER_SIZE],   uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE],  travstate_t travstate, globalparams_t globalparams){
	unsigned int analysis_buffersz = SRCBUFFER_SIZE * VECTOR_SIZE;
	
	REDUCEPHASE_REDUCE_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
	#pragma HLS PIPELINE II=1					
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
		
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr2_v 34", loc0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr2_v 34", loc1, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr2_v 34", loc2, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr2_v 34", loc3, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		#endif
		
		keyvalue_t vprop0 = getkeyvalue(destbuffer0, loc0, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop1 = getkeyvalue(destbuffer1, loc1, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop2 = getkeyvalue(destbuffer2, loc2, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop3 = getkeyvalue(destbuffer3, loc3, PADDEDDESTBUFFER_SIZE);
		
		value_t temp0 = reducefunc(vprop0.key, vprop0.value, keyvalue0.value, globalparams.GraphIter);
		value_t temp1 = reducefunc(vprop1.key, vprop1.value, keyvalue1.value, globalparams.GraphIter);
		value_t temp2 = reducefunc(vprop2.key, vprop2.value, keyvalue2.value, globalparams.GraphIter);
		value_t temp3 = reducefunc(vprop3.key, vprop3.value, keyvalue3.value, globalparams.GraphIter);
		
		vprop0.value = temp0;
		vprop1.value = temp1;
		vprop2.value = temp2;
		vprop3.value = temp3;
		
		setkeyvalue(destbuffer0, loc0, vprop0, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer1, loc1, vprop1, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer2, loc2, vprop2, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer3, loc3, vprop3, PADDEDDESTBUFFER_SIZE);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}
void enigma::reducepartitions3( uint512_dt sourcebuffer0[SRCBUFFER_SIZE],  uint512_dt sourcebuffer1[SRCBUFFER_SIZE],  uint512_dt sourcebuffer2[SRCBUFFER_SIZE],  uint512_dt sourcebuffer3[SRCBUFFER_SIZE],   uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE],  uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE],  travstate_t travstate, globalparams_t globalparams){
	unsigned int analysis_buffersz = SRCBUFFER_SIZE * VECTOR_SIZE;
	
	REDUCEPHASE_REDUCE_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
	#pragma HLS PIPELINE II=1					
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
		
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr3_v 34", loc0, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr3_v 34", loc1, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr3_v 34", loc2, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		checkoutofbounds("legion::reduce_ddr3_v 34", loc3, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, NAp, NAp, NAp);
		#endif
		
		keyvalue_t vprop0 = getkeyvalue(destbuffer0, loc0, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop1 = getkeyvalue(destbuffer1, loc1, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop2 = getkeyvalue(destbuffer2, loc2, PADDEDDESTBUFFER_SIZE);
		keyvalue_t vprop3 = getkeyvalue(destbuffer3, loc3, PADDEDDESTBUFFER_SIZE);
		
		value_t temp0 = reducefunc(vprop0.key, vprop0.value, keyvalue0.value, globalparams.GraphIter);
		value_t temp1 = reducefunc(vprop1.key, vprop1.value, keyvalue1.value, globalparams.GraphIter);
		value_t temp2 = reducefunc(vprop2.key, vprop2.value, keyvalue2.value, globalparams.GraphIter);
		value_t temp3 = reducefunc(vprop3.key, vprop3.value, keyvalue3.value, globalparams.GraphIter);
		
		vprop0.value = temp0;
		vprop1.value = temp1;
		vprop2.value = temp2;
		vprop3.value = temp3;
		
		setkeyvalue(destbuffer0, loc0, vprop0, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer1, loc1, vprop1, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer2, loc2, vprop2, PADDEDDESTBUFFER_SIZE);	
		setkeyvalue(destbuffer3, loc3, vprop3, PADDEDDESTBUFFER_SIZE);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}

void enigma::savekeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	// #pragma HLS INLINE // FIXME. remove pragma?
	int analysis_savebuffer = PADDEDDESTBUFFER_SIZE;
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::savekeyvalues dram 35", baseaddress + offset_kvs + PADDEDDESTBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	READBUFFER_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	   #pragma HLS PIPELINE II=1
	   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer	
		kvdram[baseaddress + offset_kvs + i] = buffer[i];
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	return;
}
void enigma::savekeyvalues1(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	// #pragma HLS INLINE // FIXME. remove pragma?
	int analysis_savebuffer = PADDEDDESTBUFFER_SIZE;
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::savekeyvalues dram 35", baseaddress + offset_kvs + PADDEDDESTBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	READBUFFER_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	   #pragma HLS PIPELINE II=1
	   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer	
		kvdram[baseaddress + offset_kvs + i] = buffer[i];
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	return;
}
void enigma::savekeyvalues2(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	// #pragma HLS INLINE // FIXME. remove pragma?
	int analysis_savebuffer = PADDEDDESTBUFFER_SIZE;
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::savekeyvalues dram 35", baseaddress + offset_kvs + PADDEDDESTBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	READBUFFER_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	   #pragma HLS PIPELINE II=1
	   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer	
		kvdram[baseaddress + offset_kvs + i] = buffer[i];
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	return;
}
void enigma::savekeyvalues3(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	// #pragma HLS INLINE // FIXME. remove pragma?
	int analysis_savebuffer = PADDEDDESTBUFFER_SIZE;
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::savekeyvalues dram 35", baseaddress + offset_kvs + PADDEDDESTBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	READBUFFER_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	   #pragma HLS PIPELINE II=1
	   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer	
		kvdram[baseaddress + offset_kvs + i] = buffer[i];
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	return;
}
travoffsets_t enigma::savecapsules0(uint512_dt * kvdram , uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], batch_type baseaddress_kvs, batch_type offset_kvs, travstate_t travstate, travoffsets_t travoffsets){							
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	calculateoffsets(capsule0,capsule1,capsule2,capsule3, NAp);
	SAVECAPSULE0_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		capsule0[i].value = 0;
		setkeyvalue(BIGcapsule0, loffset + i, capsule0[i], CAPSULEBUFFER_SIZE);
		capsule1[i].value = 0;
		setkeyvalue(BIGcapsule1, loffset + i, capsule1[i], CAPSULEBUFFER_SIZE);
		capsule2[i].value = 0;
		setkeyvalue(BIGcapsule2, loffset + i, capsule2[i], CAPSULEBUFFER_SIZE);
		capsule3[i].value = 0;
		setkeyvalue(BIGcapsule3, loffset + i, capsule3[i], CAPSULEBUFFER_SIZE);
	}
	
	if(((loffset + NUM_PARTITIONS) == (CAPSULEBUFFER_SIZE * VECTOR_SIZE)) || ((travstate.i_kvs + travstate.skip_kvs) >= travstate.end_kvs)){
		batch_type saveoffset0_kvs = travoffsets.capsuleoffset_kvs + (0 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset0_kvs: "<<saveoffset0_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules0 34", baseaddress_kvs + saveoffset0_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule0, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset1_kvs = travoffsets.capsuleoffset_kvs + (1 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset1_kvs: "<<saveoffset1_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules0 34", baseaddress_kvs + saveoffset1_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset2_kvs = travoffsets.capsuleoffset_kvs + (2 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset2_kvs: "<<saveoffset2_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules0 34", baseaddress_kvs + saveoffset2_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule2, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset3_kvs = travoffsets.capsuleoffset_kvs + (3 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset3_kvs: "<<saveoffset3_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules0 34", baseaddress_kvs + saveoffset3_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule3, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		travoffsets.capsuleoffset_kvs += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	return travoffsets;
}
travoffsets_t enigma::savecapsules1(uint512_dt * kvdram , uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], batch_type baseaddress_kvs, batch_type offset_kvs, travstate_t travstate, travoffsets_t travoffsets){							
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	calculateoffsets(capsule0,capsule1,capsule2,capsule3, NAp);
	SAVECAPSULE1_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		capsule0[i].value = 0;
		setkeyvalue(BIGcapsule0, loffset + i, capsule0[i], CAPSULEBUFFER_SIZE);
		capsule1[i].value = 0;
		setkeyvalue(BIGcapsule1, loffset + i, capsule1[i], CAPSULEBUFFER_SIZE);
		capsule2[i].value = 0;
		setkeyvalue(BIGcapsule2, loffset + i, capsule2[i], CAPSULEBUFFER_SIZE);
		capsule3[i].value = 0;
		setkeyvalue(BIGcapsule3, loffset + i, capsule3[i], CAPSULEBUFFER_SIZE);
	}
	
	if(((loffset + NUM_PARTITIONS) == (CAPSULEBUFFER_SIZE * VECTOR_SIZE)) || ((travstate.i_kvs + travstate.skip_kvs) >= travstate.end_kvs)){
		batch_type saveoffset0_kvs = travoffsets.capsuleoffset_kvs + (0 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset0_kvs: "<<saveoffset0_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules1 34", baseaddress_kvs + saveoffset0_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues1(kvdram, BIGcapsule0, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset1_kvs = travoffsets.capsuleoffset_kvs + (1 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset1_kvs: "<<saveoffset1_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules1 34", baseaddress_kvs + saveoffset1_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues1(kvdram, BIGcapsule1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset2_kvs = travoffsets.capsuleoffset_kvs + (2 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset2_kvs: "<<saveoffset2_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules1 34", baseaddress_kvs + saveoffset2_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues1(kvdram, BIGcapsule2, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset3_kvs = travoffsets.capsuleoffset_kvs + (3 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset3_kvs: "<<saveoffset3_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules1 34", baseaddress_kvs + saveoffset3_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues1(kvdram, BIGcapsule3, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		travoffsets.capsuleoffset_kvs += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	return travoffsets;
}
travoffsets_t enigma::savecapsules2(uint512_dt * kvdram , uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], batch_type baseaddress_kvs, batch_type offset_kvs, travstate_t travstate, travoffsets_t travoffsets){							
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	calculateoffsets(capsule0,capsule1,capsule2,capsule3, NAp);
	SAVECAPSULE2_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		capsule0[i].value = 0;
		setkeyvalue(BIGcapsule0, loffset + i, capsule0[i], CAPSULEBUFFER_SIZE);
		capsule1[i].value = 0;
		setkeyvalue(BIGcapsule1, loffset + i, capsule1[i], CAPSULEBUFFER_SIZE);
		capsule2[i].value = 0;
		setkeyvalue(BIGcapsule2, loffset + i, capsule2[i], CAPSULEBUFFER_SIZE);
		capsule3[i].value = 0;
		setkeyvalue(BIGcapsule3, loffset + i, capsule3[i], CAPSULEBUFFER_SIZE);
	}
	
	if(((loffset + NUM_PARTITIONS) == (CAPSULEBUFFER_SIZE * VECTOR_SIZE)) || ((travstate.i_kvs + travstate.skip_kvs) >= travstate.end_kvs)){
		batch_type saveoffset0_kvs = travoffsets.capsuleoffset_kvs + (0 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset0_kvs: "<<saveoffset0_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules2 34", baseaddress_kvs + saveoffset0_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues2(kvdram, BIGcapsule0, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset1_kvs = travoffsets.capsuleoffset_kvs + (1 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset1_kvs: "<<saveoffset1_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules2 34", baseaddress_kvs + saveoffset1_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues2(kvdram, BIGcapsule1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset2_kvs = travoffsets.capsuleoffset_kvs + (2 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset2_kvs: "<<saveoffset2_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules2 34", baseaddress_kvs + saveoffset2_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues2(kvdram, BIGcapsule2, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset3_kvs = travoffsets.capsuleoffset_kvs + (3 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset3_kvs: "<<saveoffset3_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules2 34", baseaddress_kvs + saveoffset3_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues2(kvdram, BIGcapsule3, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		travoffsets.capsuleoffset_kvs += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	return travoffsets;
}
travoffsets_t enigma::savecapsules3(uint512_dt * kvdram , uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], batch_type baseaddress_kvs, batch_type offset_kvs, travstate_t travstate, travoffsets_t travoffsets){							
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	calculateoffsets(capsule0,capsule1,capsule2,capsule3, NAp);
	SAVECAPSULE3_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		capsule0[i].value = 0;
		setkeyvalue(BIGcapsule0, loffset + i, capsule0[i], CAPSULEBUFFER_SIZE);
		capsule1[i].value = 0;
		setkeyvalue(BIGcapsule1, loffset + i, capsule1[i], CAPSULEBUFFER_SIZE);
		capsule2[i].value = 0;
		setkeyvalue(BIGcapsule2, loffset + i, capsule2[i], CAPSULEBUFFER_SIZE);
		capsule3[i].value = 0;
		setkeyvalue(BIGcapsule3, loffset + i, capsule3[i], CAPSULEBUFFER_SIZE);
	}
	
	if(((loffset + NUM_PARTITIONS) == (CAPSULEBUFFER_SIZE * VECTOR_SIZE)) || ((travstate.i_kvs + travstate.skip_kvs) >= travstate.end_kvs)){
		batch_type saveoffset0_kvs = travoffsets.capsuleoffset_kvs + (0 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset0_kvs: "<<saveoffset0_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules3 34", baseaddress_kvs + saveoffset0_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues3(kvdram, BIGcapsule0, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset1_kvs = travoffsets.capsuleoffset_kvs + (1 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset1_kvs: "<<saveoffset1_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules3 34", baseaddress_kvs + saveoffset1_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues3(kvdram, BIGcapsule1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset2_kvs = travoffsets.capsuleoffset_kvs + (2 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset2_kvs: "<<saveoffset2_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules3 34", baseaddress_kvs + saveoffset2_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues3(kvdram, BIGcapsule2, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset3_kvs = travoffsets.capsuleoffset_kvs + (3 * CAPSULEBUFFER_SIZE);
		// cout<<"SAVE HERE: savecapsules:: travoffsets.capsuleoffset_kvs: "<<travoffsets.capsuleoffset_kvs<<", offset_kvs: "<<offset_kvs<<", loffset: "<<loffset<<", lsavebase_kvs: "<<NAp<<", saveoffset3_kvs: "<<saveoffset3_kvs<<endl;
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules3 34", baseaddress_kvs + saveoffset3_kvs + CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues3(kvdram, BIGcapsule3, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		travoffsets.capsuleoffset_kvs += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	return travoffsets;
}
void enigma::savepartitions0(uint512_dt * kvdram , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS] ,keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type kvdrambaseaddress){				
	int analysis_savebuffer = PADDEDBUFFER_SIZE / NUM_PARTITIONS;
	
		SAVEPARTITIONS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule0[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule0[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions0 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions0 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule0[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer0[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule0[p].value; }
		SAVEPARTITIONS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule1[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule1[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions1 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions1 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule1[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer1[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule1[p].value; }
		SAVEPARTITIONS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule2[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule2[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions2 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions2 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule2[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer2[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule2[p].value; }
		SAVEPARTITIONS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule3[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule3[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions3 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions3 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule3[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer3[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule3[p].value; }
	return;
}
void enigma::savepartitions1(uint512_dt * kvdram , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS] ,keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type kvdrambaseaddress){				
	int analysis_savebuffer = PADDEDBUFFER_SIZE / NUM_PARTITIONS;
	
		SAVEPARTITIONS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule0[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule0[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions0 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions0 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule0[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer0[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule0[p].value; }
		SAVEPARTITIONS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule1[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule1[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions1 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions1 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule1[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer1[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule1[p].value; }
		SAVEPARTITIONS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule2[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule2[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions2 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions2 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule2[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer2[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule2[p].value; }
		SAVEPARTITIONS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule3[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule3[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions3 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions3 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule3[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer3[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule3[p].value; }
	return;
}
void enigma::savepartitions2(uint512_dt * kvdram , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS] ,keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type kvdrambaseaddress){				
	int analysis_savebuffer = PADDEDBUFFER_SIZE / NUM_PARTITIONS;
	
		SAVEPARTITIONS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule0[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule0[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions0 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions0 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule0[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer0[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule0[p].value; }
		SAVEPARTITIONS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule1[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule1[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions1 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions1 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule1[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer1[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule1[p].value; }
		SAVEPARTITIONS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule2[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule2[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions2 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions2 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule2[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer2[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule2[p].value; }
		SAVEPARTITIONS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule3[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule3[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions3 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions3 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule3[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer3[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule3[p].value; }
	return;
}
void enigma::savepartitions3(uint512_dt * kvdram , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS] ,keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type kvdrambaseaddress){				
	int analysis_savebuffer = PADDEDBUFFER_SIZE / NUM_PARTITIONS;
	
		SAVEPARTITIONS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule0[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule0[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions0 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions0 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule0[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer0[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule0[p].value; }
		SAVEPARTITIONS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule1[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule1[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions1 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions1 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule1[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer1[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule1[p].value; }
		SAVEPARTITIONS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule2[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule2[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions2 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions2 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule2[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer2[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule2[p].value; }
		SAVEPARTITIONS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = (capsule3[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = (capsule3[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			if(bramoffset_kvs + size_kvs >= PADDEDBUFFER_SIZE){ bramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			if(dramoffset_kvs + size_kvs >= PADDEDKVSOURCEDRAMSZ_KVS){ dramoffset_kvs = 0; size_kvs = 0; } // FIXME. FORCEDFINISH
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions3 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions3 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule3[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			for(buffer_type i=0; i<BUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			for(buffer_type i=0; i<size_kvs; i++)
			#endif
			{
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer3[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				globalstats_countkvspartitionswritten(VECTOR_SIZE);
				#endif
			}
		}
		// UPDATEGLOBALSTATS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += (BUFFER_SIZE * NUMSUBWORKERS) / NUM_PARTITIONS; } // FIXME. FORCEDFINISH
		UPDATEGLOBALSTATS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += capsule3[p].value; }
	return;
}

void enigma::resetcapsules0(keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  int dummy){
	#pragma HLS INLINE 
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		capsule0[p].key = 0; capsule0[p].value = 0;
		capsule1[p].key = 0; capsule1[p].value = 0;
		capsule2[p].key = 0; capsule2[p].value = 0;
		capsule3[p].key = 0; capsule3[p].value = 0;
	}
	return;
}
void enigma::resetcapsules1(keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  int dummy){
	#pragma HLS INLINE 
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		capsule0[p].key = 0; capsule0[p].value = 0;
		capsule1[p].key = 0; capsule1[p].value = 0;
		capsule2[p].key = 0; capsule2[p].value = 0;
		capsule3[p].key = 0; capsule3[p].value = 0;
	}
	return;
}
void enigma::resetcapsules2(keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  int dummy){
	#pragma HLS INLINE 
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		capsule0[p].key = 0; capsule0[p].value = 0;
		capsule1[p].key = 0; capsule1[p].value = 0;
		capsule2[p].key = 0; capsule2[p].value = 0;
		capsule3[p].key = 0; capsule3[p].value = 0;
	}
	return;
}
void enigma::resetcapsules3(keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  int dummy){
	#pragma HLS INLINE 
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		capsule0[p].key = 0; capsule0[p].value = 0;
		capsule1[p].key = 0; capsule1[p].value = 0;
		capsule2[p].key = 0; capsule2[p].value = 0;
		capsule3[p].key = 0; capsule3[p].value = 0;
	}
	return;
}

void enigma::loadKvDRAM0(uint512_dt * kvdram, batch_type sourceoffset_kvs, batch_type destoffset_kvs, batch_type size_kvs){
	int analysis_chunksize = BUFFER_SIZE;
	int analysis_iterationcount = KVDATA_BATCHSIZE_KVS / BUFFER_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	print4("### loadKvDRAM0:: sourceoffset_kvs", "destoffset_kvs", "size_kvs", "NAp", sourceoffset_kvs, destoffset_kvs, size_kvs, NAp);
	#endif
	
	uint512_dt sourcebuffer[BUFFER_SIZE];
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadKvDRAM0 35", destoffset_kvs + size_kvs, KVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	int i;
    for ( i = 0; i < size_kvs; i += BUFFER_SIZE) {
	#pragma HLS LOOP_TRIPCOUNT min=analysis_iterationcount max=analysis_iterationcount
		
		//boundary checks
		int chunk_size = BUFFER_SIZE;
        if ((i + BUFFER_SIZE) > size_kvs)
            chunk_size = size_kvs - i;
	
		v1_rd:
			for (int j = 0; j < chunk_size; j++) {
			   #pragma HLS PIPELINE II=1
			   #pragma HLS LOOP_TRIPCOUNT min=analysis_chunksize max=analysis_chunksize
				sourcebuffer[j] = kvdram[sourceoffset_kvs + i + j];
			}
		out_write:
			for (int j = 0; j < chunk_size; j++) {
			   #pragma HLS PIPELINE II=1
			   #pragma HLS LOOP_TRIPCOUNT min=analysis_chunksize max=analysis_chunksize
				kvdram[destoffset_kvs + i + j] = sourcebuffer[j];
			}
    }
	return;
}
void enigma::loadKvDRAM1(uint512_dt * kvdram, batch_type sourceoffset_kvs, batch_type destoffset_kvs, batch_type size_kvs){
	int analysis_chunksize = BUFFER_SIZE;
	int analysis_iterationcount = KVDATA_BATCHSIZE_KVS / BUFFER_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	print4("### loadKvDRAM1:: sourceoffset_kvs", "destoffset_kvs", "size_kvs", "NAp", sourceoffset_kvs, destoffset_kvs, size_kvs, NAp);
	#endif
	
	uint512_dt sourcebuffer[BUFFER_SIZE];
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadKvDRAM1 35", destoffset_kvs + size_kvs, KVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	int i;
    for ( i = 0; i < size_kvs; i += BUFFER_SIZE) {
	#pragma HLS LOOP_TRIPCOUNT min=analysis_iterationcount max=analysis_iterationcount
		
		//boundary checks
		int chunk_size = BUFFER_SIZE;
        if ((i + BUFFER_SIZE) > size_kvs)
            chunk_size = size_kvs - i;
	
		v1_rd:
			for (int j = 0; j < chunk_size; j++) {
			   #pragma HLS PIPELINE II=1
			   #pragma HLS LOOP_TRIPCOUNT min=analysis_chunksize max=analysis_chunksize
				sourcebuffer[j] = kvdram[sourceoffset_kvs + i + j];
			}
		out_write:
			for (int j = 0; j < chunk_size; j++) {
			   #pragma HLS PIPELINE II=1
			   #pragma HLS LOOP_TRIPCOUNT min=analysis_chunksize max=analysis_chunksize
				kvdram[destoffset_kvs + i + j] = sourcebuffer[j];
			}
    }
	return;
}
void enigma::loadKvDRAM2(uint512_dt * kvdram, batch_type sourceoffset_kvs, batch_type destoffset_kvs, batch_type size_kvs){
	int analysis_chunksize = BUFFER_SIZE;
	int analysis_iterationcount = KVDATA_BATCHSIZE_KVS / BUFFER_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	print4("### loadKvDRAM2:: sourceoffset_kvs", "destoffset_kvs", "size_kvs", "NAp", sourceoffset_kvs, destoffset_kvs, size_kvs, NAp);
	#endif
	
	uint512_dt sourcebuffer[BUFFER_SIZE];
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadKvDRAM2 35", destoffset_kvs + size_kvs, KVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	int i;
    for ( i = 0; i < size_kvs; i += BUFFER_SIZE) {
	#pragma HLS LOOP_TRIPCOUNT min=analysis_iterationcount max=analysis_iterationcount
		
		//boundary checks
		int chunk_size = BUFFER_SIZE;
        if ((i + BUFFER_SIZE) > size_kvs)
            chunk_size = size_kvs - i;
	
		v1_rd:
			for (int j = 0; j < chunk_size; j++) {
			   #pragma HLS PIPELINE II=1
			   #pragma HLS LOOP_TRIPCOUNT min=analysis_chunksize max=analysis_chunksize
				sourcebuffer[j] = kvdram[sourceoffset_kvs + i + j];
			}
		out_write:
			for (int j = 0; j < chunk_size; j++) {
			   #pragma HLS PIPELINE II=1
			   #pragma HLS LOOP_TRIPCOUNT min=analysis_chunksize max=analysis_chunksize
				kvdram[destoffset_kvs + i + j] = sourcebuffer[j];
			}
    }
	return;
}
void enigma::loadKvDRAM3(uint512_dt * kvdram, batch_type sourceoffset_kvs, batch_type destoffset_kvs, batch_type size_kvs){
	int analysis_chunksize = BUFFER_SIZE;
	int analysis_iterationcount = KVDATA_BATCHSIZE_KVS / BUFFER_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	print4("### loadKvDRAM3:: sourceoffset_kvs", "destoffset_kvs", "size_kvs", "NAp", sourceoffset_kvs, destoffset_kvs, size_kvs, NAp);
	#endif
	
	uint512_dt sourcebuffer[BUFFER_SIZE];
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadKvDRAM3 35", destoffset_kvs + size_kvs, KVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	int i;
    for ( i = 0; i < size_kvs; i += BUFFER_SIZE) {
	#pragma HLS LOOP_TRIPCOUNT min=analysis_iterationcount max=analysis_iterationcount
		
		//boundary checks
		int chunk_size = BUFFER_SIZE;
        if ((i + BUFFER_SIZE) > size_kvs)
            chunk_size = size_kvs - i;
	
		v1_rd:
			for (int j = 0; j < chunk_size; j++) {
			   #pragma HLS PIPELINE II=1
			   #pragma HLS LOOP_TRIPCOUNT min=analysis_chunksize max=analysis_chunksize
				sourcebuffer[j] = kvdram[sourceoffset_kvs + i + j];
			}
		out_write:
			for (int j = 0; j < chunk_size; j++) {
			   #pragma HLS PIPELINE II=1
			   #pragma HLS LOOP_TRIPCOUNT min=analysis_chunksize max=analysis_chunksize
				kvdram[destoffset_kvs + i + j] = sourcebuffer[j];
			}
    }
	return;
}
void enigma::partitionandreduce0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
	unsigned int analysis_maxtreedepthsz = 1;
	unsigned int analysis_avgnumsourcepartitions = 1;
	unsigned int analysis_partitionphase = (KVDRAMSZ_KVS / (NUMWORKERS * SRCBUFFER_SIZE)) / VECTOR_SIZE;
	unsigned int analysis_reducephase = 1;
	
	uint512_dt sourcebuffer00[SRCBUFFER_SIZE];
	uint512_dt destbuffer00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule00[NUM_PARTITIONS];
	uint512_dt BIGcapsule00[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule00 instance=mergedarray00 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule00 instance=mergedarray00 horizontal
	uint512_dt sourcebuffer01[SRCBUFFER_SIZE];
	uint512_dt destbuffer01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule01[NUM_PARTITIONS];
	uint512_dt BIGcapsule01[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule01 instance=mergedarray01 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule01 instance=mergedarray01 horizontal
	uint512_dt sourcebuffer02[SRCBUFFER_SIZE];
	uint512_dt destbuffer02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule02[NUM_PARTITIONS];
	uint512_dt BIGcapsule02[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule02 instance=mergedarray02 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule02 instance=mergedarray02 horizontal
	uint512_dt sourcebuffer03[SRCBUFFER_SIZE];
	uint512_dt destbuffer03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule03[NUM_PARTITIONS];
	uint512_dt BIGcapsule03[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule03 instance=mergedarray03 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule03 instance=mergedarray03 horizontal
	
	resetcapsules0(capsule00,capsule01,capsule02,capsule03, NAp);
	unsigned int numLOPs = 0;
	if(globalparams.reducecommand == ON){ numLOPs = globalparams.treedepth + 1 + 1; }
	else { numLOPs = globalparams.treedepth + 1; }

	// Partition Phase
	PARTITIONPHASE_LOOP1: for(unsigned int currentLOP=1; currentLOP<numLOPs; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		unsigned int num_source_partition_skipsize = 0;
		if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ num_source_partition_skipsize = NUMSUBWORKERS; }
		else { num_source_partition_skipsize = 1; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print4("PartitionPhase0:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		PARTITIONPHASE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=num_source_partition_skipsize){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partitionparams;
			travstate_t travstate;
			travoffsets_t travoffsets;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKER_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value = 0; }
			
			keyvalue_t sourcestat = kvstats[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparams.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparams.begin_kvs = 0; partitionparams.size_kvs = 0; partitionparams.step_kvs = 0; partitionparams.end_kvs = 0; }
			travstate.begin_kvs = partitionparams.begin_kvs;
			travstate.end_kvs = partitionparams.end_kvs;
			travstate.skip_kvs = partitionparams.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			
			// Collect capsules
			travoffsets.capsuleoffset_kvs = 0;
			COLLECTSTATS_MAINLOOP1: for(int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("collectstats_ddr0:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				COLLECTSTATS_MAINLOOP1B: for(vector_type v=0; v<NFACTOR; v++){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstate.info = 0;
					
	
	
					readkeyvalues0(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					extractcapsules00(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, llopparams, travstate);
				}
				travoffsets = savecapsules0(kvsourcedram , BIGcapsule00, BIGcapsule01, BIGcapsule02, BIGcapsule03 , capsule00, capsule01, capsule02, capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstate, travoffsets);
				resetcapsules0( capsule00, capsule01, capsule02, capsule03, NAp);
			}
			resetcapsules0( capsule00, capsule01, capsule02, capsule03, NAp);
			
			// Partition phase
			travoffsets.capsuleoffset_kvs = 0;
			PARTITIONPHASE_MAINLOOP1: for (int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("PartitionPhase0:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				travoffsets = readcapsules0(kvsourcedram,  BIGcapsule00,  BIGcapsule01,  BIGcapsule02,  BIGcapsule03,  capsule00,capsule01,capsule02,capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsets); // FIXME. include workerID in offset
				PARTITIONPHASE_MAINLOOP1B: for (int v = 0; v < NFACTOR; v += 1){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstate.info = 1;
				
	
	
					readkeyvalues0(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					partitionkeyvalues00(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, travstate, llopparams);
				}
				savepartitions0(kvsourcedram , destbuffer00, destbuffer01, destbuffer02, destbuffer03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp, llopparams.baseaddr_workdestkvs_kvs);
				resetcapsules0(capsule00,capsule01,capsule02,capsule03, NAp);
			}
			
			// Reduce phase
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
			keyvalue_t sourcestats[NUMSUBWORKERS];
			unsigned int maxsz = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats[i] = kvstats[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxsz = max(sourcestats[sw].value, maxsz); }
			unsigned int maxsz_kvs = (maxsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			
			travstate.info = 0;
			readkeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			REDUCEPHASE_MAINLOOP: for (int i = 0; i < maxsz_kvs; i += SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reducephase avg=analysis_reducephase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase0:: i", "i_end", "NAp", "NAp", "skip size", i, maxsz_kvs, NAp, NAp, SRCBUFFER_SIZE);
				#endif
				travstate.info = 1;

		
				readkeyvalues0(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues0(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues0(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues0(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				
				reducepartitions0( sourcebuffer00,  sourcebuffer01,  sourcebuffer02,  sourcebuffer03,   destbuffer00,  destbuffer01,  destbuffer02,  destbuffer03,  travstate, globalparams);
			}
			savekeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			}
			
			storedeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			llopparams.nextsourceoffset_partition += llopparams.sourceskipsize_partition;
			llopparams.nextdestoffset_partition += (llopparams.destskipsize_partition * NUM_PARTITIONS);
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv * NUMSUBWORKERS; } // 
			else { llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv; }
			llopparams.nextdestoffset_kv += llopparams.destskipsize_kv;
		}
		#ifdef _DEBUGMODE_KERNELPRINTS3
		printglobalvars();
		clearglobalvars();
		#endif
	}
	return;
}
void enigma::partitionandreduce1(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
	unsigned int analysis_maxtreedepthsz = 1;
	unsigned int analysis_avgnumsourcepartitions = 1;
	unsigned int analysis_partitionphase = (KVDRAMSZ_KVS / (NUMWORKERS * SRCBUFFER_SIZE)) / VECTOR_SIZE;
	unsigned int analysis_reducephase = 1;
	
	uint512_dt sourcebuffer00[SRCBUFFER_SIZE];
	uint512_dt destbuffer00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule00[NUM_PARTITIONS];
	uint512_dt BIGcapsule00[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule00 instance=mergedarray00 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule00 instance=mergedarray00 horizontal
	uint512_dt sourcebuffer01[SRCBUFFER_SIZE];
	uint512_dt destbuffer01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule01[NUM_PARTITIONS];
	uint512_dt BIGcapsule01[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule01 instance=mergedarray01 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule01 instance=mergedarray01 horizontal
	uint512_dt sourcebuffer02[SRCBUFFER_SIZE];
	uint512_dt destbuffer02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule02[NUM_PARTITIONS];
	uint512_dt BIGcapsule02[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule02 instance=mergedarray02 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule02 instance=mergedarray02 horizontal
	uint512_dt sourcebuffer03[SRCBUFFER_SIZE];
	uint512_dt destbuffer03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule03[NUM_PARTITIONS];
	uint512_dt BIGcapsule03[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule03 instance=mergedarray03 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule03 instance=mergedarray03 horizontal
	
	resetcapsules1(capsule00,capsule01,capsule02,capsule03, NAp);
	unsigned int numLOPs = 0;
	if(globalparams.reducecommand == ON){ numLOPs = globalparams.treedepth + 1 + 1; }
	else { numLOPs = globalparams.treedepth + 1; }

	// Partition Phase
	PARTITIONPHASE_LOOP1: for(unsigned int currentLOP=1; currentLOP<numLOPs; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		unsigned int num_source_partition_skipsize = 0;
		if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ num_source_partition_skipsize = NUMSUBWORKERS; }
		else { num_source_partition_skipsize = 1; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print4("PartitionPhase1:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		PARTITIONPHASE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=num_source_partition_skipsize){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partitionparams;
			travstate_t travstate;
			travoffsets_t travoffsets;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKER_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value = 0; }
			
			keyvalue_t sourcestat = kvstats[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparams.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparams.begin_kvs = 0; partitionparams.size_kvs = 0; partitionparams.step_kvs = 0; partitionparams.end_kvs = 0; }
			travstate.begin_kvs = partitionparams.begin_kvs;
			travstate.end_kvs = partitionparams.end_kvs;
			travstate.skip_kvs = partitionparams.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase1:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase1:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			
			// Collect capsules
			travoffsets.capsuleoffset_kvs = 0;
			COLLECTSTATS_MAINLOOP1: for(int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("collectstats_ddr1:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				COLLECTSTATS_MAINLOOP1B: for(vector_type v=0; v<NFACTOR; v++){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstate.info = 0;
					
	
	
					readkeyvalues1(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					extractcapsules10(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, llopparams, travstate);
				}
				travoffsets = savecapsules1(kvsourcedram , BIGcapsule00, BIGcapsule01, BIGcapsule02, BIGcapsule03 , capsule00, capsule01, capsule02, capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstate, travoffsets);
				resetcapsules1( capsule00, capsule01, capsule02, capsule03, NAp);
			}
			resetcapsules1( capsule00, capsule01, capsule02, capsule03, NAp);
			
			// Partition phase
			travoffsets.capsuleoffset_kvs = 0;
			PARTITIONPHASE_MAINLOOP1: for (int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("PartitionPhase1:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				travoffsets = readcapsules1(kvsourcedram,  BIGcapsule00,  BIGcapsule01,  BIGcapsule02,  BIGcapsule03,  capsule00,capsule01,capsule02,capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsets); // FIXME. include workerID in offset
				PARTITIONPHASE_MAINLOOP1B: for (int v = 0; v < NFACTOR; v += 1){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstate.info = 1;
				
	
	
					readkeyvalues1(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					partitionkeyvalues10(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, travstate, llopparams);
				}
				savepartitions1(kvsourcedram , destbuffer00, destbuffer01, destbuffer02, destbuffer03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp, llopparams.baseaddr_workdestkvs_kvs);
				resetcapsules1(capsule00,capsule01,capsule02,capsule03, NAp);
			}
			
			// Reduce phase
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
			keyvalue_t sourcestats[NUMSUBWORKERS];
			unsigned int maxsz = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats[i] = kvstats[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxsz = max(sourcestats[sw].value, maxsz); }
			unsigned int maxsz_kvs = (maxsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			
			travstate.info = 0;
			readkeyvalues1(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			REDUCEPHASE_MAINLOOP: for (int i = 0; i < maxsz_kvs; i += SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reducephase avg=analysis_reducephase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase1:: i", "i_end", "NAp", "NAp", "skip size", i, maxsz_kvs, NAp, NAp, SRCBUFFER_SIZE);
				#endif
				travstate.info = 1;

		
				readkeyvalues1(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				
				reducepartitions1( sourcebuffer00,  sourcebuffer01,  sourcebuffer02,  sourcebuffer03,   destbuffer00,  destbuffer01,  destbuffer02,  destbuffer03,  travstate, globalparams);
			}
			savekeyvalues1(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			}
			
			storedeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			llopparams.nextsourceoffset_partition += llopparams.sourceskipsize_partition;
			llopparams.nextdestoffset_partition += (llopparams.destskipsize_partition * NUM_PARTITIONS);
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv * NUMSUBWORKERS; } // 
			else { llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv; }
			llopparams.nextdestoffset_kv += llopparams.destskipsize_kv;
		}
		#ifdef _DEBUGMODE_KERNELPRINTS3
		printglobalvars();
		clearglobalvars();
		#endif
	}
	return;
}
void enigma::partitionandreduce2(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
	unsigned int analysis_maxtreedepthsz = 1;
	unsigned int analysis_avgnumsourcepartitions = 1;
	unsigned int analysis_partitionphase = (KVDRAMSZ_KVS / (NUMWORKERS * SRCBUFFER_SIZE)) / VECTOR_SIZE;
	unsigned int analysis_reducephase = 1;
	
	uint512_dt sourcebuffer00[SRCBUFFER_SIZE];
	uint512_dt destbuffer00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule00[NUM_PARTITIONS];
	uint512_dt BIGcapsule00[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule00 instance=mergedarray00 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule00 instance=mergedarray00 horizontal
	uint512_dt sourcebuffer01[SRCBUFFER_SIZE];
	uint512_dt destbuffer01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule01[NUM_PARTITIONS];
	uint512_dt BIGcapsule01[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule01 instance=mergedarray01 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule01 instance=mergedarray01 horizontal
	uint512_dt sourcebuffer02[SRCBUFFER_SIZE];
	uint512_dt destbuffer02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule02[NUM_PARTITIONS];
	uint512_dt BIGcapsule02[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule02 instance=mergedarray02 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule02 instance=mergedarray02 horizontal
	uint512_dt sourcebuffer03[SRCBUFFER_SIZE];
	uint512_dt destbuffer03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule03[NUM_PARTITIONS];
	uint512_dt BIGcapsule03[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule03 instance=mergedarray03 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule03 instance=mergedarray03 horizontal
	
	resetcapsules2(capsule00,capsule01,capsule02,capsule03, NAp);
	unsigned int numLOPs = 0;
	if(globalparams.reducecommand == ON){ numLOPs = globalparams.treedepth + 1 + 1; }
	else { numLOPs = globalparams.treedepth + 1; }

	// Partition Phase
	PARTITIONPHASE_LOOP1: for(unsigned int currentLOP=1; currentLOP<numLOPs; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		unsigned int num_source_partition_skipsize = 0;
		if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ num_source_partition_skipsize = NUMSUBWORKERS; }
		else { num_source_partition_skipsize = 1; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print4("PartitionPhase2:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		PARTITIONPHASE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=num_source_partition_skipsize){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partitionparams;
			travstate_t travstate;
			travoffsets_t travoffsets;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKER_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value = 0; }
			
			keyvalue_t sourcestat = kvstats[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparams.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparams.begin_kvs = 0; partitionparams.size_kvs = 0; partitionparams.step_kvs = 0; partitionparams.end_kvs = 0; }
			travstate.begin_kvs = partitionparams.begin_kvs;
			travstate.end_kvs = partitionparams.end_kvs;
			travstate.skip_kvs = partitionparams.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase2:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase2:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			
			// Collect capsules
			travoffsets.capsuleoffset_kvs = 0;
			COLLECTSTATS_MAINLOOP1: for(int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("collectstats_ddr2:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				COLLECTSTATS_MAINLOOP1B: for(vector_type v=0; v<NFACTOR; v++){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstate.info = 0;
					
	
	
					readkeyvalues2(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					extractcapsules20(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, llopparams, travstate);
				}
				travoffsets = savecapsules2(kvsourcedram , BIGcapsule00, BIGcapsule01, BIGcapsule02, BIGcapsule03 , capsule00, capsule01, capsule02, capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstate, travoffsets);
				resetcapsules2( capsule00, capsule01, capsule02, capsule03, NAp);
			}
			resetcapsules2( capsule00, capsule01, capsule02, capsule03, NAp);
			
			// Partition phase
			travoffsets.capsuleoffset_kvs = 0;
			PARTITIONPHASE_MAINLOOP1: for (int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("PartitionPhase2:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				travoffsets = readcapsules2(kvsourcedram,  BIGcapsule00,  BIGcapsule01,  BIGcapsule02,  BIGcapsule03,  capsule00,capsule01,capsule02,capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsets); // FIXME. include workerID in offset
				PARTITIONPHASE_MAINLOOP1B: for (int v = 0; v < NFACTOR; v += 1){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstate.info = 1;
				
	
	
					readkeyvalues2(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					partitionkeyvalues20(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, travstate, llopparams);
				}
				savepartitions2(kvsourcedram , destbuffer00, destbuffer01, destbuffer02, destbuffer03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp, llopparams.baseaddr_workdestkvs_kvs);
				resetcapsules2(capsule00,capsule01,capsule02,capsule03, NAp);
			}
			
			// Reduce phase
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
			keyvalue_t sourcestats[NUMSUBWORKERS];
			unsigned int maxsz = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats[i] = kvstats[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxsz = max(sourcestats[sw].value, maxsz); }
			unsigned int maxsz_kvs = (maxsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			
			travstate.info = 0;
			readkeyvalues2(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			REDUCEPHASE_MAINLOOP: for (int i = 0; i < maxsz_kvs; i += SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reducephase avg=analysis_reducephase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase2:: i", "i_end", "NAp", "NAp", "skip size", i, maxsz_kvs, NAp, NAp, SRCBUFFER_SIZE);
				#endif
				travstate.info = 1;

		
				readkeyvalues2(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				
				reducepartitions2( sourcebuffer00,  sourcebuffer01,  sourcebuffer02,  sourcebuffer03,   destbuffer00,  destbuffer01,  destbuffer02,  destbuffer03,  travstate, globalparams);
			}
			savekeyvalues2(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			}
			
			storedeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			llopparams.nextsourceoffset_partition += llopparams.sourceskipsize_partition;
			llopparams.nextdestoffset_partition += (llopparams.destskipsize_partition * NUM_PARTITIONS);
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv * NUMSUBWORKERS; } // 
			else { llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv; }
			llopparams.nextdestoffset_kv += llopparams.destskipsize_kv;
		}
		#ifdef _DEBUGMODE_KERNELPRINTS3
		printglobalvars();
		clearglobalvars();
		#endif
	}
	return;
}
void enigma::partitionandreduce3(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
	unsigned int analysis_maxtreedepthsz = 1;
	unsigned int analysis_avgnumsourcepartitions = 1;
	unsigned int analysis_partitionphase = (KVDRAMSZ_KVS / (NUMWORKERS * SRCBUFFER_SIZE)) / VECTOR_SIZE;
	unsigned int analysis_reducephase = 1;
	
	uint512_dt sourcebuffer00[SRCBUFFER_SIZE];
	uint512_dt destbuffer00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule00[NUM_PARTITIONS];
	uint512_dt BIGcapsule00[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule00 instance=mergedarray00 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule00 instance=mergedarray00 horizontal
	uint512_dt sourcebuffer01[SRCBUFFER_SIZE];
	uint512_dt destbuffer01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule01[NUM_PARTITIONS];
	uint512_dt BIGcapsule01[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule01 instance=mergedarray01 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule01 instance=mergedarray01 horizontal
	uint512_dt sourcebuffer02[SRCBUFFER_SIZE];
	uint512_dt destbuffer02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule02[NUM_PARTITIONS];
	uint512_dt BIGcapsule02[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule02 instance=mergedarray02 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule02 instance=mergedarray02 horizontal
	uint512_dt sourcebuffer03[SRCBUFFER_SIZE];
	uint512_dt destbuffer03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule03[NUM_PARTITIONS];
	uint512_dt BIGcapsule03[CAPSULEBUFFER_SIZE];
	// #pragma HLS ARRAY_MAP variable=capsule03 instance=mergedarray03 horizontal // FIXME. remove pragma?
	// #pragma HLS ARRAY_MAP variable=BIGcapsule03 instance=mergedarray03 horizontal
	
	resetcapsules3(capsule00,capsule01,capsule02,capsule03, NAp);
	unsigned int numLOPs = 0;
	if(globalparams.reducecommand == ON){ numLOPs = globalparams.treedepth + 1 + 1; }
	else { numLOPs = globalparams.treedepth + 1; }

	// Partition Phase
	PARTITIONPHASE_LOOP1: for(unsigned int currentLOP=1; currentLOP<numLOPs; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		unsigned int num_source_partition_skipsize = 0;
		if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ num_source_partition_skipsize = NUMSUBWORKERS; }
		else { num_source_partition_skipsize = 1; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print4("PartitionPhase3:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		PARTITIONPHASE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=num_source_partition_skipsize){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partitionparams;
			travstate_t travstate;
			travoffsets_t travoffsets;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKER_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value = 0; }
			
			keyvalue_t sourcestat = kvstats[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparams.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparams.begin_kvs = 0; partitionparams.size_kvs = 0; partitionparams.step_kvs = 0; partitionparams.end_kvs = 0; }
			travstate.begin_kvs = partitionparams.begin_kvs;
			travstate.end_kvs = partitionparams.end_kvs;
			travstate.skip_kvs = partitionparams.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase3:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase3:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			
			// Collect capsules
			travoffsets.capsuleoffset_kvs = 0;
			COLLECTSTATS_MAINLOOP1: for(int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("collectstats_ddr3:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				COLLECTSTATS_MAINLOOP1B: for(vector_type v=0; v<NFACTOR; v++){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstate.info = 0;
					
	
	
					readkeyvalues3(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					extractcapsules30(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, llopparams, travstate);
				}
				travoffsets = savecapsules3(kvsourcedram , BIGcapsule00, BIGcapsule01, BIGcapsule02, BIGcapsule03 , capsule00, capsule01, capsule02, capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstate, travoffsets);
				resetcapsules3( capsule00, capsule01, capsule02, capsule03, NAp);
			}
			resetcapsules3( capsule00, capsule01, capsule02, capsule03, NAp);
			
			// Partition phase
			travoffsets.capsuleoffset_kvs = 0;
			PARTITIONPHASE_MAINLOOP1: for (int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("PartitionPhase3:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				travoffsets = readcapsules3(kvsourcedram,  BIGcapsule00,  BIGcapsule01,  BIGcapsule02,  BIGcapsule03,  capsule00,capsule01,capsule02,capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsets); // FIXME. include workerID in offset
				PARTITIONPHASE_MAINLOOP1B: for (int v = 0; v < NFACTOR; v += 1){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstate.info = 1;
				
	
	
					readkeyvalues3(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					partitionkeyvalues30(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, travstate, llopparams);
				}
				savepartitions3(kvsourcedram , destbuffer00, destbuffer01, destbuffer02, destbuffer03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp, llopparams.baseaddr_workdestkvs_kvs);
				resetcapsules3(capsule00,capsule01,capsule02,capsule03, NAp);
			}
			
			// Reduce phase
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
			keyvalue_t sourcestats[NUMSUBWORKERS];
			unsigned int maxsz = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats[i] = kvstats[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxsz = max(sourcestats[sw].value, maxsz); }
			unsigned int maxsz_kvs = (maxsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			
			travstate.info = 0;
			readkeyvalues3(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			REDUCEPHASE_MAINLOOP: for (int i = 0; i < maxsz_kvs; i += SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reducephase avg=analysis_reducephase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase3:: i", "i_end", "NAp", "NAp", "skip size", i, maxsz_kvs, NAp, NAp, SRCBUFFER_SIZE);
				#endif
				travstate.info = 1;

		
				readkeyvalues3(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				
				reducepartitions3( sourcebuffer00,  sourcebuffer01,  sourcebuffer02,  sourcebuffer03,   destbuffer00,  destbuffer01,  destbuffer02,  destbuffer03,  travstate, globalparams);
			}
			savekeyvalues3(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			}
			
			storedeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			llopparams.nextsourceoffset_partition += llopparams.sourceskipsize_partition;
			llopparams.nextdestoffset_partition += (llopparams.destskipsize_partition * NUM_PARTITIONS);
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv * NUMSUBWORKERS; } // 
			else { llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv; }
			llopparams.nextdestoffset_kv += llopparams.destskipsize_kv;
		}
		#ifdef _DEBUGMODE_KERNELPRINTS3
		printglobalvars();
		clearglobalvars();
		#endif
	}
	return;
}
void enigma::partitionandreduce(
uint512_dt * kvsourcedramA,uint512_dt * kvsourcedramB,uint512_dt * kvsourcedramC,uint512_dt * kvsourcedramD, 
uint512_dt * kvdestdramA,uint512_dt * kvdestdramB,uint512_dt * kvdestdramC,uint512_dt * kvdestdramD, 
keyvalue_t * kvstatsA,keyvalue_t * kvstatsB,keyvalue_t * kvstatsC,keyvalue_t * kvstatsD, 
	globalparams_t globalparams){
	#pragma HLS INLINE
	unsigned int analysis_maxtreedepthsz = 1;
	unsigned int analysis_avgnumsourcepartitions = 1;
	unsigned int analysis_partitionphase = (KVDRAMSZ_KVS / (NUMWORKERS * SRCBUFFER_SIZE)) / VECTOR_SIZE;
	unsigned int analysis_reducephase = 1;
	
	uint512_dt sourcebufferA00[SRCBUFFER_SIZE];
	uint512_dt destbufferA00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleA00[NUM_PARTITIONS];
	uint512_dt BIGcapsuleA00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferA01[SRCBUFFER_SIZE];
	uint512_dt destbufferA01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleA01[NUM_PARTITIONS];
	uint512_dt BIGcapsuleA01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferA02[SRCBUFFER_SIZE];
	uint512_dt destbufferA02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleA02[NUM_PARTITIONS];
	uint512_dt BIGcapsuleA02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferA03[SRCBUFFER_SIZE];
	uint512_dt destbufferA03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleA03[NUM_PARTITIONS];
	uint512_dt BIGcapsuleA03[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferB00[SRCBUFFER_SIZE];
	uint512_dt destbufferB00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleB00[NUM_PARTITIONS];
	uint512_dt BIGcapsuleB00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferB01[SRCBUFFER_SIZE];
	uint512_dt destbufferB01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleB01[NUM_PARTITIONS];
	uint512_dt BIGcapsuleB01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferB02[SRCBUFFER_SIZE];
	uint512_dt destbufferB02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleB02[NUM_PARTITIONS];
	uint512_dt BIGcapsuleB02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferB03[SRCBUFFER_SIZE];
	uint512_dt destbufferB03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleB03[NUM_PARTITIONS];
	uint512_dt BIGcapsuleB03[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferC00[SRCBUFFER_SIZE];
	uint512_dt destbufferC00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleC00[NUM_PARTITIONS];
	uint512_dt BIGcapsuleC00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferC01[SRCBUFFER_SIZE];
	uint512_dt destbufferC01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleC01[NUM_PARTITIONS];
	uint512_dt BIGcapsuleC01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferC02[SRCBUFFER_SIZE];
	uint512_dt destbufferC02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleC02[NUM_PARTITIONS];
	uint512_dt BIGcapsuleC02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferC03[SRCBUFFER_SIZE];
	uint512_dt destbufferC03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleC03[NUM_PARTITIONS];
	uint512_dt BIGcapsuleC03[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferD00[SRCBUFFER_SIZE];
	uint512_dt destbufferD00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleD00[NUM_PARTITIONS];
	uint512_dt BIGcapsuleD00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferD01[SRCBUFFER_SIZE];
	uint512_dt destbufferD01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleD01[NUM_PARTITIONS];
	uint512_dt BIGcapsuleD01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferD02[SRCBUFFER_SIZE];
	uint512_dt destbufferD02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleD02[NUM_PARTITIONS];
	uint512_dt BIGcapsuleD02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferD03[SRCBUFFER_SIZE];
	uint512_dt destbufferD03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleD03[NUM_PARTITIONS];
	uint512_dt BIGcapsuleD03[CAPSULEBUFFER_SIZE];
	
	resetcapsules0(capsuleA00,capsuleA01,capsuleA02,capsuleA03, NAp);
	resetcapsules1(capsuleB00,capsuleB01,capsuleB02,capsuleB03, NAp);
	resetcapsules2(capsuleC00,capsuleC01,capsuleC02,capsuleC03, NAp);
	resetcapsules3(capsuleD00,capsuleD01,capsuleD02,capsuleD03, NAp);
	unsigned int numLOPs = 0;
	if(globalparams.reducecommand == ON){ numLOPs = globalparams.treedepth + 1 + 1; }
	else { numLOPs = globalparams.treedepth + 1; }

	// Partition Phase
	PARTITIONPHASE_LOOP1: for(unsigned int currentLOP=1; currentLOP<numLOPs; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststatsA_tmp[NUM_PARTITIONS];
		keyvalue_t kvdeststatsB_tmp[NUM_PARTITIONS];
		keyvalue_t kvdeststatsC_tmp[NUM_PARTITIONS];
		keyvalue_t kvdeststatsD_tmp[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		unsigned int num_source_partition_skipsize = 0;
		if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ num_source_partition_skipsize = NUMSUBWORKERS; }
		else { num_source_partition_skipsize = 1; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print4("PartitionPhase:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		PARTITIONPHASE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=num_source_partition_skipsize){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partitionparamsA;
			travstate_t travstateA;
			travoffsets_t travoffsetsA;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstatsA, kvdeststatsA_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKERA_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststatsA_tmp[p].value = 0; }
			
			keyvalue_t sourcestatA = kvstatsA[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparamsA.begin_kvs = sourcestatA.key / VECTOR_SIZE;
			partitionparamsA.size_kvs = (sourcestatA.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparamsA.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparamsA.end_kvs = partitionparamsA.begin_kvs + partitionparamsA.size_kvs;
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparamsA.begin_kvs = 0; partitionparamsA.size_kvs = 0; partitionparamsA.step_kvs = 0; partitionparamsA.end_kvs = 0; }
			travstateA.begin_kvs = partitionparamsA.begin_kvs;
			travstateA.end_kvs = partitionparamsA.end_kvs;
			travstateA.skip_kvs = partitionparamsA.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatA.key, sourcestatA.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatA.key, sourcestatA.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			partitionparams_t partitionparamsB;
			travstate_t travstateB;
			travoffsets_t travoffsetsB;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstatsB, kvdeststatsB_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKERB_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststatsB_tmp[p].value = 0; }
			
			keyvalue_t sourcestatB = kvstatsB[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparamsB.begin_kvs = sourcestatB.key / VECTOR_SIZE;
			partitionparamsB.size_kvs = (sourcestatB.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparamsB.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparamsB.end_kvs = partitionparamsB.begin_kvs + partitionparamsB.size_kvs;
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparamsB.begin_kvs = 0; partitionparamsB.size_kvs = 0; partitionparamsB.step_kvs = 0; partitionparamsB.end_kvs = 0; }
			travstateB.begin_kvs = partitionparamsB.begin_kvs;
			travstateB.end_kvs = partitionparamsB.end_kvs;
			travstateB.skip_kvs = partitionparamsB.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase1:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatB.key, sourcestatB.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase1:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatB.key, sourcestatB.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			partitionparams_t partitionparamsC;
			travstate_t travstateC;
			travoffsets_t travoffsetsC;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstatsC, kvdeststatsC_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKERC_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststatsC_tmp[p].value = 0; }
			
			keyvalue_t sourcestatC = kvstatsC[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparamsC.begin_kvs = sourcestatC.key / VECTOR_SIZE;
			partitionparamsC.size_kvs = (sourcestatC.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparamsC.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparamsC.end_kvs = partitionparamsC.begin_kvs + partitionparamsC.size_kvs;
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparamsC.begin_kvs = 0; partitionparamsC.size_kvs = 0; partitionparamsC.step_kvs = 0; partitionparamsC.end_kvs = 0; }
			travstateC.begin_kvs = partitionparamsC.begin_kvs;
			travstateC.end_kvs = partitionparamsC.end_kvs;
			travstateC.skip_kvs = partitionparamsC.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase2:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatC.key, sourcestatC.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase2:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatC.key, sourcestatC.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			partitionparams_t partitionparamsD;
			travstate_t travstateD;
			travoffsets_t travoffsetsD;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstatsD, kvdeststatsD_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKERD_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststatsD_tmp[p].value = 0; }
			
			keyvalue_t sourcestatD = kvstatsD[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparamsD.begin_kvs = sourcestatD.key / VECTOR_SIZE;
			partitionparamsD.size_kvs = (sourcestatD.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparamsD.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparamsD.end_kvs = partitionparamsD.begin_kvs + partitionparamsD.size_kvs;
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparamsD.begin_kvs = 0; partitionparamsD.size_kvs = 0; partitionparamsD.step_kvs = 0; partitionparamsD.end_kvs = 0; }
			travstateD.begin_kvs = partitionparamsD.begin_kvs;
			travstateD.end_kvs = partitionparamsD.end_kvs;
			travstateD.skip_kvs = partitionparamsD.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase3:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatD.key, sourcestatD.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase3:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatD.key, sourcestatD.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			
			// Collect capsules
			travoffsetsA.capsuleoffset_kvs = 0;
			travoffsetsB.capsuleoffset_kvs = 0;
			travoffsetsC.capsuleoffset_kvs = 0;
			travoffsetsD.capsuleoffset_kvs = 0;
			COLLECTSTATS_MAINLOOP1: for(int i = partitionparamsA.begin_kvs; i < partitionparamsA.end_kvs; i += partitionparamsA.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("collectstats_ddr:: i", "i_end", "step size", "NAp", i, partitionparamsA.end_kvs, partitionparamsA.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparamsA.begin_kvs) / partitionparamsA.step_kvs) * NUM_PARTITIONS;
				COLLECTSTATS_MAINLOOP1B: for(vector_type v=0; v<NFACTOR; v++){
					travstateA.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstateA.info = 0;
					
	
	
					readkeyvalues0(kvsourcedramA, sourcebufferA00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedramA, sourcebufferA01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedramA, sourcebufferA02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedramA, sourcebufferA03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
	
					readkeyvalues1(kvsourcedramB, sourcebufferB00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedramB, sourcebufferB01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedramB, sourcebufferB02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedramB, sourcebufferB03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
	
					readkeyvalues2(kvsourcedramC, sourcebufferC00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedramC, sourcebufferC01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedramC, sourcebufferC02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedramC, sourcebufferC03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
	
					readkeyvalues3(kvsourcedramD, sourcebufferD00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedramD, sourcebufferD01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedramD, sourcebufferD02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedramD, sourcebufferD03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					extractcapsules00(0  ,sourcebufferA00 ,sourcebufferA01 ,sourcebufferA02 ,sourcebufferA03  ,capsuleA00 ,capsuleA01 ,capsuleA02 ,capsuleA03, llopparams, travstateA);
					extractcapsules10(0  ,sourcebufferB00 ,sourcebufferB01 ,sourcebufferB02 ,sourcebufferB03  ,capsuleB00 ,capsuleB01 ,capsuleB02 ,capsuleB03, llopparams, travstateA);
					extractcapsules20(0  ,sourcebufferC00 ,sourcebufferC01 ,sourcebufferC02 ,sourcebufferC03  ,capsuleC00 ,capsuleC01 ,capsuleC02 ,capsuleC03, llopparams, travstateA);
					extractcapsules30(0  ,sourcebufferD00 ,sourcebufferD01 ,sourcebufferD02 ,sourcebufferD03  ,capsuleD00 ,capsuleD01 ,capsuleD02 ,capsuleD03, llopparams, travstateA);
				}
				travoffsetsA = savecapsules0(kvsourcedramA , BIGcapsuleA00, BIGcapsuleA01, BIGcapsuleA02, BIGcapsuleA03 , capsuleA00, capsuleA01, capsuleA02, capsuleA03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstateA, travoffsetsA);
				travoffsetsB = savecapsules1(kvsourcedramB , BIGcapsuleB00, BIGcapsuleB01, BIGcapsuleB02, BIGcapsuleB03 , capsuleB00, capsuleB01, capsuleB02, capsuleB03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstateA, travoffsetsB);
				travoffsetsC = savecapsules2(kvsourcedramC , BIGcapsuleC00, BIGcapsuleC01, BIGcapsuleC02, BIGcapsuleC03 , capsuleC00, capsuleC01, capsuleC02, capsuleC03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstateA, travoffsetsC);
				travoffsetsD = savecapsules3(kvsourcedramD , BIGcapsuleD00, BIGcapsuleD01, BIGcapsuleD02, BIGcapsuleD03 , capsuleD00, capsuleD01, capsuleD02, capsuleD03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstateA, travoffsetsD);
				resetcapsules0( capsuleA00, capsuleA01, capsuleA02, capsuleA03, NAp);
				resetcapsules1( capsuleB00, capsuleB01, capsuleB02, capsuleB03, NAp);
				resetcapsules2( capsuleC00, capsuleC01, capsuleC02, capsuleC03, NAp);
				resetcapsules3( capsuleD00, capsuleD01, capsuleD02, capsuleD03, NAp);
			}
			resetcapsules0( capsuleA00, capsuleA01, capsuleA02, capsuleA03, NAp);
			resetcapsules1( capsuleB00, capsuleB01, capsuleB02, capsuleB03, NAp);
			resetcapsules2( capsuleC00, capsuleC01, capsuleC02, capsuleC03, NAp);
			resetcapsules3( capsuleD00, capsuleD01, capsuleD02, capsuleD03, NAp);
			
			// Partition phase
			travoffsetsA.capsuleoffset_kvs = 0;
			travoffsetsB.capsuleoffset_kvs = 0;
			travoffsetsC.capsuleoffset_kvs = 0;
			travoffsetsD.capsuleoffset_kvs = 0;
			PARTITIONPHASE_MAINLOOP1: for (int i = partitionparamsA.begin_kvs; i < partitionparamsA.end_kvs; i += partitionparamsA.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("PartitionPhase:: i", "i_end", "step size", "NAp", i, partitionparamsA.end_kvs, partitionparamsA.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparamsA.begin_kvs) / partitionparamsA.step_kvs) * NUM_PARTITIONS;
				travoffsetsA = readcapsules0(kvsourcedramA,  BIGcapsuleA00,  BIGcapsuleA01,  BIGcapsuleA02,  BIGcapsuleA03,  capsuleA00,capsuleA01,capsuleA02,capsuleA03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsetsA); // FIXME. include workerID in offset
				travoffsetsB = readcapsules1(kvsourcedramB,  BIGcapsuleB00,  BIGcapsuleB01,  BIGcapsuleB02,  BIGcapsuleB03,  capsuleB00,capsuleB01,capsuleB02,capsuleB03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsetsB); // FIXME. include workerID in offset
				travoffsetsC = readcapsules2(kvsourcedramC,  BIGcapsuleC00,  BIGcapsuleC01,  BIGcapsuleC02,  BIGcapsuleC03,  capsuleC00,capsuleC01,capsuleC02,capsuleC03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsetsC); // FIXME. include workerID in offset
				travoffsetsD = readcapsules3(kvsourcedramD,  BIGcapsuleD00,  BIGcapsuleD01,  BIGcapsuleD02,  BIGcapsuleD03,  capsuleD00,capsuleD01,capsuleD02,capsuleD03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsetsD); // FIXME. include workerID in offset
				PARTITIONPHASE_MAINLOOP1B: for (int v = 0; v < NFACTOR; v += 1){
					travstateA.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstateA.info = 1;
				
					readkeyvalues0(kvsourcedramA, sourcebufferA00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues0(kvsourcedramA, sourcebufferA01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues0(kvsourcedramA, sourcebufferA02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues0(kvsourcedramA, sourcebufferA03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues1(kvsourcedramB, sourcebufferB00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues1(kvsourcedramB, sourcebufferB01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues1(kvsourcedramB, sourcebufferB02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues1(kvsourcedramB, sourcebufferB03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues2(kvsourcedramC, sourcebufferC00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues2(kvsourcedramC, sourcebufferC01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues2(kvsourcedramC, sourcebufferC02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues2(kvsourcedramC, sourcebufferC03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues3(kvsourcedramD, sourcebufferD00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues3(kvsourcedramD, sourcebufferD01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues3(kvsourcedramD, sourcebufferD02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues3(kvsourcedramD, sourcebufferD03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					partitionkeyvalues00(0  ,sourcebufferA00 ,sourcebufferA01 ,sourcebufferA02 ,sourcebufferA03  ,destbufferA00 ,destbufferA01 ,destbufferA02 ,destbufferA03  ,capsuleA00 ,capsuleA01 ,capsuleA02 ,capsuleA03, travstateA, llopparams);
					partitionkeyvalues10(0  ,sourcebufferB00 ,sourcebufferB01 ,sourcebufferB02 ,sourcebufferB03  ,destbufferB00 ,destbufferB01 ,destbufferB02 ,destbufferB03  ,capsuleB00 ,capsuleB01 ,capsuleB02 ,capsuleB03, travstateA, llopparams);
					partitionkeyvalues20(0  ,sourcebufferC00 ,sourcebufferC01 ,sourcebufferC02 ,sourcebufferC03  ,destbufferC00 ,destbufferC01 ,destbufferC02 ,destbufferC03  ,capsuleC00 ,capsuleC01 ,capsuleC02 ,capsuleC03, travstateA, llopparams);
					partitionkeyvalues30(0  ,sourcebufferD00 ,sourcebufferD01 ,sourcebufferD02 ,sourcebufferD03  ,destbufferD00 ,destbufferD01 ,destbufferD02 ,destbufferD03  ,capsuleD00 ,capsuleD01 ,capsuleD02 ,capsuleD03, travstateA, llopparams);
				}
				savepartitions0(kvsourcedramA , destbufferA00, destbufferA01, destbufferA02, destbufferA03 , capsuleA00, capsuleA01, capsuleA02, capsuleA03, kvdeststatsA_tmp, llopparams.baseaddr_workdestkvs_kvs);
				savepartitions1(kvsourcedramB , destbufferB00, destbufferB01, destbufferB02, destbufferB03 , capsuleB00, capsuleB01, capsuleB02, capsuleB03, kvdeststatsB_tmp, llopparams.baseaddr_workdestkvs_kvs);
				savepartitions2(kvsourcedramC , destbufferC00, destbufferC01, destbufferC02, destbufferC03 , capsuleC00, capsuleC01, capsuleC02, capsuleC03, kvdeststatsC_tmp, llopparams.baseaddr_workdestkvs_kvs);
				savepartitions3(kvsourcedramD , destbufferD00, destbufferD01, destbufferD02, destbufferD03 , capsuleD00, capsuleD01, capsuleD02, capsuleD03, kvdeststatsD_tmp, llopparams.baseaddr_workdestkvs_kvs);
				resetcapsules0(capsuleA00,capsuleA01,capsuleA02,capsuleA03, NAp);
				resetcapsules1(capsuleB00,capsuleB01,capsuleB02,capsuleB03, NAp);
				resetcapsules2(capsuleC00,capsuleC01,capsuleC02,capsuleC03, NAp);
				resetcapsules3(capsuleD00,capsuleD01,capsuleD02,capsuleD03, NAp);
			}
			
			// Reduce phase
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
			keyvalue_t sourcestatsA[NUMSUBWORKERS];
			unsigned int maxszA = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestatsA[i] = kvstatsA[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxszA = max(sourcestatsA[sw].value, maxszA); }
			unsigned int maxszA_kvs = (maxszA + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			keyvalue_t sourcestatsB[NUMSUBWORKERS];
			unsigned int maxszB = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestatsB[i] = kvstatsB[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxszB = max(sourcestatsB[sw].value, maxszB); }
			unsigned int maxszB_kvs = (maxszB + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			keyvalue_t sourcestatsC[NUMSUBWORKERS];
			unsigned int maxszC = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestatsC[i] = kvstatsC[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxszC = max(sourcestatsC[sw].value, maxszC); }
			unsigned int maxszC_kvs = (maxszC + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			keyvalue_t sourcestatsD[NUMSUBWORKERS];
			unsigned int maxszD = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestatsD[i] = kvstatsD[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxszD = max(sourcestatsD[sw].value, maxszD); }
			unsigned int maxszD_kvs = (maxszD + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			
			travstateA.info = 0;
			readkeyvalues0(kvdestdramA, destbufferA00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdramA, destbufferA01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdramA, destbufferA02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdramA, destbufferA03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdramB, destbufferB00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdramB, destbufferB01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdramB, destbufferB02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdramB, destbufferB03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdramC, destbufferC00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdramC, destbufferC01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdramC, destbufferC02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdramC, destbufferC03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdramD, destbufferD00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdramD, destbufferD01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdramD, destbufferD02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdramD, destbufferD03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			REDUCEPHASE_MAINLOOP: for (int i = 0; i < maxszA_kvs; i += SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reducephase avg=analysis_reducephase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase:: i", "i_end", "NAp", "NAp", "skip size", i, maxszA_kvs, NAp, NAp, SRCBUFFER_SIZE);
				#endif
				travstateA.info = 1;

		
				readkeyvalues0(kvsourcedramA, sourcebufferA00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues0(kvsourcedramA, sourcebufferA01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues0(kvsourcedramA, sourcebufferA02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues0(kvsourcedramA, sourcebufferA03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedramB, sourcebufferB00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedramB, sourcebufferB01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedramB, sourcebufferB02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedramB, sourcebufferB03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedramC, sourcebufferC00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedramC, sourcebufferC01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedramC, sourcebufferC02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedramC, sourcebufferC03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedramD, sourcebufferD00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedramD, sourcebufferD01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedramD, sourcebufferD02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedramD, sourcebufferD03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				
				reducepartitions0( sourcebufferA00,  sourcebufferA01,  sourcebufferA02,  sourcebufferA03,   destbufferA00,  destbufferA01,  destbufferA02,  destbufferA03,  travstateA, globalparams);
				reducepartitions1( sourcebufferB00,  sourcebufferB01,  sourcebufferB02,  sourcebufferB03,   destbufferB00,  destbufferB01,  destbufferB02,  destbufferB03,  travstateA, globalparams);
				reducepartitions2( sourcebufferC00,  sourcebufferC01,  sourcebufferC02,  sourcebufferC03,   destbufferC00,  destbufferC01,  destbufferC02,  destbufferC03,  travstateA, globalparams);
				reducepartitions3( sourcebufferD00,  sourcebufferD01,  sourcebufferD02,  sourcebufferD03,   destbufferD00,  destbufferD01,  destbufferD02,  destbufferD03,  travstateA, globalparams);
			}
			savekeyvalues0(kvdestdramA, destbufferA00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdramA, destbufferA01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdramA, destbufferA02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdramA, destbufferA03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdramB, destbufferB00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdramB, destbufferB01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdramB, destbufferB02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdramB, destbufferB03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdramC, destbufferC00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdramC, destbufferC01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdramC, destbufferC02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdramC, destbufferC03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdramD, destbufferD00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdramD, destbufferD01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdramD, destbufferD02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdramD, destbufferD03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			}
			
			storedeststats(kvstatsA, kvdeststatsA_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			storedeststats(kvstatsB, kvdeststatsB_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			storedeststats(kvstatsC, kvdeststatsC_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			storedeststats(kvstatsD, kvdeststatsD_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			llopparams.nextsourceoffset_partition += llopparams.sourceskipsize_partition;
			llopparams.nextdestoffset_partition += (llopparams.destskipsize_partition * NUM_PARTITIONS);
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv * NUMSUBWORKERS; } // 
			else { llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv; }
			llopparams.nextdestoffset_kv += llopparams.destskipsize_kv;
		}
		#ifdef _DEBUGMODE_KERNELPRINTS3
		printglobalvars();
		clearglobalvars();
		#endif
	}
	return;
}

extern "C" {
void enigma::topkernel(
uint512_dt * kvsourcedramA
,uint512_dt * kvsourcedramB
,uint512_dt * kvsourcedramC
,uint512_dt * kvsourcedramD
		,uint512_dt * kvdestdramA
		,uint512_dt * kvdestdramB
		,uint512_dt * kvdestdramC
		,uint512_dt * kvdestdramD
		,keyvalue_t * kvstatsA
		,keyvalue_t * kvstatsB
		,keyvalue_t * kvstatsC
		,keyvalue_t * kvstatsD
        ){
// #pragma HLS INTERFACE m_axi port=kvsourcedramA offset=slave bundle=gmem0 max_read_burst_length=256 max_write_burst_length=256
// #pragma HLS INTERFACE m_axi port=kvsourcedramA offset=slave bundle=gmem0 max_read_burst_length=128 max_write_burst_length=128
#pragma HLS INTERFACE m_axi port=kvsourcedramA offset=slave bundle=gmem0 max_read_burst_length=64 max_write_burst_length=64
// #pragma HLS INTERFACE m_axi port=kvsourcedramA offset=slave bundle=gmem0
// #pragma HLS INTERFACE m_axi port=kvsourcedramB offset=slave bundle=gmem1 max_read_burst_length=256 max_write_burst_length=256
// #pragma HLS INTERFACE m_axi port=kvsourcedramB offset=slave bundle=gmem1 max_read_burst_length=128 max_write_burst_length=128
#pragma HLS INTERFACE m_axi port=kvsourcedramB offset=slave bundle=gmem1 max_read_burst_length=64 max_write_burst_length=64
// #pragma HLS INTERFACE m_axi port=kvsourcedramB offset=slave bundle=gmem1
// #pragma HLS INTERFACE m_axi port=kvsourcedramC offset=slave bundle=gmem2 max_read_burst_length=256 max_write_burst_length=256
// #pragma HLS INTERFACE m_axi port=kvsourcedramC offset=slave bundle=gmem2 max_read_burst_length=128 max_write_burst_length=128
#pragma HLS INTERFACE m_axi port=kvsourcedramC offset=slave bundle=gmem2 max_read_burst_length=64 max_write_burst_length=64
// #pragma HLS INTERFACE m_axi port=kvsourcedramC offset=slave bundle=gmem2
// #pragma HLS INTERFACE m_axi port=kvsourcedramD offset=slave bundle=gmem3 max_read_burst_length=256 max_write_burst_length=256
// #pragma HLS INTERFACE m_axi port=kvsourcedramD offset=slave bundle=gmem3 max_read_burst_length=128 max_write_burst_length=128
#pragma HLS INTERFACE m_axi port=kvsourcedramD offset=slave bundle=gmem3 max_read_burst_length=64 max_write_burst_length=64
// #pragma HLS INTERFACE m_axi port=kvsourcedramD offset=slave bundle=gmem3
// #pragma HLS INTERFACE m_axi port=kvdestdramA offset=slave bundle=gmem4 max_read_burst_length=128 max_write_burst_length=128
#pragma HLS INTERFACE m_axi port=kvdestdramA offset=slave bundle=gmem4 max_read_burst_length=64 max_write_burst_length=64
// #pragma HLS INTERFACE m_axi port=kvdestdramB offset=slave bundle=gmem5 max_read_burst_length=128 max_write_burst_length=128
#pragma HLS INTERFACE m_axi port=kvdestdramB offset=slave bundle=gmem5 max_read_burst_length=64 max_write_burst_length=64
// #pragma HLS INTERFACE m_axi port=kvdestdramC offset=slave bundle=gmem6 max_read_burst_length=128 max_write_burst_length=128
#pragma HLS INTERFACE m_axi port=kvdestdramC offset=slave bundle=gmem6 max_read_burst_length=64 max_write_burst_length=64
// #pragma HLS INTERFACE m_axi port=kvdestdramD offset=slave bundle=gmem7 max_read_burst_length=128 max_write_burst_length=128
#pragma HLS INTERFACE m_axi port=kvdestdramD offset=slave bundle=gmem7 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=kvstatsA offset=slave bundle=gmem8
#pragma HLS INTERFACE m_axi port=kvstatsB offset=slave bundle=gmem9
#pragma HLS INTERFACE m_axi port=kvstatsC offset=slave bundle=gmem10
#pragma HLS INTERFACE m_axi port=kvstatsD offset=slave bundle=gmem11

#pragma HLS INTERFACE s_axilite port=kvsourcedramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvsourcedramB  bundle=control
#pragma HLS INTERFACE s_axilite port=kvsourcedramC  bundle=control
#pragma HLS INTERFACE s_axilite port=kvsourcedramD  bundle=control
#pragma HLS INTERFACE s_axilite port=kvdestdramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvdestdramB  bundle=control
#pragma HLS INTERFACE s_axilite port=kvdestdramC  bundle=control
#pragma HLS INTERFACE s_axilite port=kvdestdramD  bundle=control
#pragma HLS INTERFACE s_axilite port=kvstatsA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvstatsB  bundle=control
#pragma HLS INTERFACE s_axilite port=kvstatsC  bundle=control
#pragma HLS INTERFACE s_axilite port=kvstatsD  bundle=control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable=kvstatsA
#pragma HLS DATA_PACK variable=kvstatsB
#pragma HLS DATA_PACK variable=kvstatsC
#pragma HLS DATA_PACK variable=kvstatsD
	
	globalparams_t globalparams;
	globalparams.runkernelcommand = kvstatsA[getmessagesAddr(MESSAGES_RUNKERNELCOMMANDID)].key;
	globalparams.processcommand = kvstatsA[getmessagesAddr(MESSAGES_PROCESSCOMMANDID)].key;
	globalparams.partitioncommand = kvstatsA[getmessagesAddr(MESSAGES_PARTITIONCOMMANDID)].key;
	globalparams.reducecommand = kvstatsA[getmessagesAddr(MESSAGES_APPLYUPDATESCOMMANDID)].key;
	globalparams.vbegin = kvstatsA[getmessagesAddr(MESSAGES_VOFFSET)].key; 
	globalparams.vsize = kvstatsA[getmessagesAddr(MESSAGES_VSIZE)].key; 
	globalparams.treedepth = kvstatsA[getmessagesAddr(MESSAGES_TREEDEPTH)].key; 
	globalparams.LLOPnumpartitions = kvstatsA[getmessagesAddr(MESSAGES_FINALNUMPARTITIONS)].key;
	globalparams.GraphIter = kvstatsA[getmessagesAddr(MESSAGES_GRAPHITERATIONID)].key;
	globalparams.IterCount = kvstatsA[getmessagesAddr(MESSAGES_ITERATIONID)].key;
	globalparams.baseaddr_destkvs_kvs = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	std::cout<<std::endl;
	std::cout<<"Kernel Started: globalparams.runkernelcommand: "<<globalparams.runkernelcommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.processcommand: "<<globalparams.processcommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.partitioncommand: "<<globalparams.partitioncommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.reducecommand: "<<globalparams.reducecommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.vbegin: "<<globalparams.vbegin<<std::endl;
	std::cout<<"Kernel Started: globalparams.vsize: "<<globalparams.vsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.treedepth: "<<globalparams.treedepth<<std::endl;
	std::cout<<"Kernel Started: globalparams.LLOPnumpartitions: "<<globalparams.LLOPnumpartitions<<std::endl;
	std::cout<<"Kernel Started: globalparams.GraphIter: "<<globalparams.GraphIter<<std::endl;
	std::cout<<"Kernel Started: globalparams.IterCount: "<<globalparams.IterCount<<std::endl;
	#endif
	
	/** if(globalparams.runkernelcommand == OFF){
		loadKvDRAM0(kvsourcedramA, 0, ((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS, KVDATA_BATCHSIZE_KVS);
		loadKvDRAM1(kvsourcedramB, 0, ((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS, KVDATA_BATCHSIZE_KVS);
		loadKvDRAM2(kvsourcedramC, 0, ((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS, KVDATA_BATCHSIZE_KVS);
		loadKvDRAM3(kvsourcedramD, 0, ((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS, KVDATA_BATCHSIZE_KVS);
		return;
	} */
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"...running Acts..."<<endl;
	printparameters();
	#endif 
	
	/** 	partitionandreduce0(kvsourcedramA, kvdestdramA, kvstatsA, globalparams);
	partitionandreduce1(kvsourcedramB, kvdestdramB, kvstatsB, globalparams);
	partitionandreduce2(kvsourcedramC, kvdestdramC, kvstatsC, globalparams);
	partitionandreduce3(kvsourcedramD, kvdestdramD, kvstatsD, globalparams);
 */
	
	partitionandreduce(
 kvsourcedramA, kvsourcedramB, kvsourcedramC, kvsourcedramD, 
 kvdestdramA, kvdestdramB, kvdestdramC, kvdestdramD, 
 kvstatsA, kvstatsB, kvstatsC, kvstatsD, 
		globalparams);
		
	/** unsigned int analysis_maxtreedepthsz = 1;
	unsigned int analysis_avgnumsourcepartitions = 1;
	unsigned int analysis_partitionphase = (KVDRAMSZ_KVS / (NUMWORKERS * SRCBUFFER_SIZE)) / VECTOR_SIZE;
	unsigned int analysis_reducephase = 1;
	
	uint512_dt sourcebufferA00[SRCBUFFER_SIZE];
	uint512_dt destbufferA00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleA00[NUM_PARTITIONS];
	uint512_dt BIGcapsuleA00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferA01[SRCBUFFER_SIZE];
	uint512_dt destbufferA01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleA01[NUM_PARTITIONS];
	uint512_dt BIGcapsuleA01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferA02[SRCBUFFER_SIZE];
	uint512_dt destbufferA02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleA02[NUM_PARTITIONS];
	uint512_dt BIGcapsuleA02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferA03[SRCBUFFER_SIZE];
	uint512_dt destbufferA03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleA03[NUM_PARTITIONS];
	uint512_dt BIGcapsuleA03[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferB00[SRCBUFFER_SIZE];
	uint512_dt destbufferB00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleB00[NUM_PARTITIONS];
	uint512_dt BIGcapsuleB00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferB01[SRCBUFFER_SIZE];
	uint512_dt destbufferB01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleB01[NUM_PARTITIONS];
	uint512_dt BIGcapsuleB01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferB02[SRCBUFFER_SIZE];
	uint512_dt destbufferB02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleB02[NUM_PARTITIONS];
	uint512_dt BIGcapsuleB02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferB03[SRCBUFFER_SIZE];
	uint512_dt destbufferB03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleB03[NUM_PARTITIONS];
	uint512_dt BIGcapsuleB03[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferC00[SRCBUFFER_SIZE];
	uint512_dt destbufferC00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleC00[NUM_PARTITIONS];
	uint512_dt BIGcapsuleC00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferC01[SRCBUFFER_SIZE];
	uint512_dt destbufferC01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleC01[NUM_PARTITIONS];
	uint512_dt BIGcapsuleC01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferC02[SRCBUFFER_SIZE];
	uint512_dt destbufferC02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleC02[NUM_PARTITIONS];
	uint512_dt BIGcapsuleC02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferC03[SRCBUFFER_SIZE];
	uint512_dt destbufferC03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleC03[NUM_PARTITIONS];
	uint512_dt BIGcapsuleC03[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferD00[SRCBUFFER_SIZE];
	uint512_dt destbufferD00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleD00[NUM_PARTITIONS];
	uint512_dt BIGcapsuleD00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferD01[SRCBUFFER_SIZE];
	uint512_dt destbufferD01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleD01[NUM_PARTITIONS];
	uint512_dt BIGcapsuleD01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferD02[SRCBUFFER_SIZE];
	uint512_dt destbufferD02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleD02[NUM_PARTITIONS];
	uint512_dt BIGcapsuleD02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebufferD03[SRCBUFFER_SIZE];
	uint512_dt destbufferD03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsuleD03[NUM_PARTITIONS];
	uint512_dt BIGcapsuleD03[CAPSULEBUFFER_SIZE];
	
	resetcapsules0(capsuleA00,capsuleA01,capsuleA02,capsuleA03, NAp);
	resetcapsules1(capsuleB00,capsuleB01,capsuleB02,capsuleB03, NAp);
	resetcapsules2(capsuleC00,capsuleC01,capsuleC02,capsuleC03, NAp);
	resetcapsules3(capsuleD00,capsuleD01,capsuleD02,capsuleD03, NAp);
	unsigned int numLOPs = 0;
	if(globalparams.reducecommand == ON){ numLOPs = globalparams.treedepth + 1 + 1; }
	else { numLOPs = globalparams.treedepth + 1; }

	// Partition Phase
	PARTITIONPHASE_LOOP1: for(unsigned int currentLOP=1; currentLOP<numLOPs; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststatsA_tmp[NUM_PARTITIONS];
		keyvalue_t kvdeststatsB_tmp[NUM_PARTITIONS];
		keyvalue_t kvdeststatsC_tmp[NUM_PARTITIONS];
		keyvalue_t kvdeststatsD_tmp[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		unsigned int num_source_partition_skipsize = 0;
		if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ num_source_partition_skipsize = NUMSUBWORKERS; }
		else { num_source_partition_skipsize = 1; }
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print4("PartitionPhase:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		PARTITIONPHASE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=num_source_partition_skipsize){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partitionparamsA;
			travstate_t travstateA;
			travoffsets_t travoffsetsA;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstatsA, kvdeststatsA_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKERA_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststatsA_tmp[p].value = 0; }
			
			keyvalue_t sourcestatA = kvstatsA[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparamsA.begin_kvs = sourcestatA.key / VECTOR_SIZE;
			partitionparamsA.size_kvs = (sourcestatA.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparamsA.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparamsA.end_kvs = partitionparamsA.begin_kvs + partitionparamsA.size_kvs;
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparamsA.begin_kvs = 0; partitionparamsA.size_kvs = 0; partitionparamsA.step_kvs = 0; partitionparamsA.end_kvs = 0; }
			travstateA.begin_kvs = partitionparamsA.begin_kvs;
			travstateA.end_kvs = partitionparamsA.end_kvs;
			travstateA.skip_kvs = partitionparamsA.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatA.key, sourcestatA.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatA.key, sourcestatA.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			partitionparams_t partitionparamsB;
			travstate_t travstateB;
			travoffsets_t travoffsetsB;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstatsB, kvdeststatsB_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKERB_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststatsB_tmp[p].value = 0; }
			
			keyvalue_t sourcestatB = kvstatsB[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparamsB.begin_kvs = sourcestatB.key / VECTOR_SIZE;
			partitionparamsB.size_kvs = (sourcestatB.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparamsB.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparamsB.end_kvs = partitionparamsB.begin_kvs + partitionparamsB.size_kvs;
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparamsB.begin_kvs = 0; partitionparamsB.size_kvs = 0; partitionparamsB.step_kvs = 0; partitionparamsB.end_kvs = 0; }
			travstateB.begin_kvs = partitionparamsB.begin_kvs;
			travstateB.end_kvs = partitionparamsB.end_kvs;
			travstateB.skip_kvs = partitionparamsB.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase1:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatB.key, sourcestatB.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase1:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatB.key, sourcestatB.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			partitionparams_t partitionparamsC;
			travstate_t travstateC;
			travoffsets_t travoffsetsC;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstatsC, kvdeststatsC_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKERC_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststatsC_tmp[p].value = 0; }
			
			keyvalue_t sourcestatC = kvstatsC[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparamsC.begin_kvs = sourcestatC.key / VECTOR_SIZE;
			partitionparamsC.size_kvs = (sourcestatC.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparamsC.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparamsC.end_kvs = partitionparamsC.begin_kvs + partitionparamsC.size_kvs;
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparamsC.begin_kvs = 0; partitionparamsC.size_kvs = 0; partitionparamsC.step_kvs = 0; partitionparamsC.end_kvs = 0; }
			travstateC.begin_kvs = partitionparamsC.begin_kvs;
			travstateC.end_kvs = partitionparamsC.end_kvs;
			travstateC.skip_kvs = partitionparamsC.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase2:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatC.key, sourcestatC.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase2:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatC.key, sourcestatC.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			partitionparams_t partitionparamsD;
			travstate_t travstateD;
			travoffsets_t travoffsetsD;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstatsD, kvdeststatsD_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKERD_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststatsD_tmp[p].value = 0; }
			
			keyvalue_t sourcestatD = kvstatsD[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparamsD.begin_kvs = sourcestatD.key / VECTOR_SIZE;
			partitionparamsD.size_kvs = (sourcestatD.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparamsD.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparamsD.end_kvs = partitionparamsD.begin_kvs + partitionparamsD.size_kvs;
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparamsD.begin_kvs = 0; partitionparamsD.size_kvs = 0; partitionparamsD.step_kvs = 0; partitionparamsD.end_kvs = 0; }
			travstateD.begin_kvs = partitionparamsD.begin_kvs;
			travstateD.end_kvs = partitionparamsD.end_kvs;
			travstateD.skip_kvs = partitionparamsD.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase3:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatD.key, sourcestatD.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase3:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestatD.key, sourcestatD.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			
			// Collect capsules
			travoffsetsA.capsuleoffset_kvs = 0;
			travoffsetsB.capsuleoffset_kvs = 0;
			travoffsetsC.capsuleoffset_kvs = 0;
			travoffsetsD.capsuleoffset_kvs = 0;
			COLLECTSTATS_MAINLOOP1: for(int i = partitionparamsA.begin_kvs; i < partitionparamsA.end_kvs; i += partitionparamsA.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("collectstats_ddr:: i", "i_end", "step size", "NAp", i, partitionparamsA.end_kvs, partitionparamsA.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparamsA.begin_kvs) / partitionparamsA.step_kvs) * NUM_PARTITIONS;
				COLLECTSTATS_MAINLOOP1B: for(vector_type v=0; v<NFACTOR; v++){
					travstateA.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstateA.info = 0;
					
	
	
					readkeyvalues0(kvsourcedramA, sourcebufferA00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedramA, sourcebufferA01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedramA, sourcebufferA02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues0(kvsourcedramA, sourcebufferA03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
	
					readkeyvalues1(kvsourcedramB, sourcebufferB00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedramB, sourcebufferB01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedramB, sourcebufferB02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues1(kvsourcedramB, sourcebufferB03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
	
					readkeyvalues2(kvsourcedramC, sourcebufferC00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedramC, sourcebufferC01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedramC, sourcebufferC02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues2(kvsourcedramC, sourcebufferC03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
	
					readkeyvalues3(kvsourcedramD, sourcebufferD00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedramD, sourcebufferD01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedramD, sourcebufferD02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
	
					readkeyvalues3(kvsourcedramD, sourcebufferD03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					extractcapsules00(0  ,sourcebufferA00 ,sourcebufferA01 ,sourcebufferA02 ,sourcebufferA03  ,capsuleA00 ,capsuleA01 ,capsuleA02 ,capsuleA03, llopparams, travstateA);
					extractcapsules10(0  ,sourcebufferB00 ,sourcebufferB01 ,sourcebufferB02 ,sourcebufferB03  ,capsuleB00 ,capsuleB01 ,capsuleB02 ,capsuleB03, llopparams, travstateA);
					extractcapsules20(0  ,sourcebufferC00 ,sourcebufferC01 ,sourcebufferC02 ,sourcebufferC03  ,capsuleC00 ,capsuleC01 ,capsuleC02 ,capsuleC03, llopparams, travstateA);
					extractcapsules30(0  ,sourcebufferD00 ,sourcebufferD01 ,sourcebufferD02 ,sourcebufferD03  ,capsuleD00 ,capsuleD01 ,capsuleD02 ,capsuleD03, llopparams, travstateA);
				}
				travoffsetsA = savecapsules0(kvsourcedramA , BIGcapsuleA00, BIGcapsuleA01, BIGcapsuleA02, BIGcapsuleA03 , capsuleA00, capsuleA01, capsuleA02, capsuleA03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstateA, travoffsetsA);
				travoffsetsB = savecapsules1(kvsourcedramB , BIGcapsuleB00, BIGcapsuleB01, BIGcapsuleB02, BIGcapsuleB03 , capsuleB00, capsuleB01, capsuleB02, capsuleB03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstateA, travoffsetsB);
				travoffsetsC = savecapsules2(kvsourcedramC , BIGcapsuleC00, BIGcapsuleC01, BIGcapsuleC02, BIGcapsuleC03 , capsuleC00, capsuleC01, capsuleC02, capsuleC03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstateA, travoffsetsC);
				travoffsetsD = savecapsules3(kvsourcedramD , BIGcapsuleD00, BIGcapsuleD01, BIGcapsuleD02, BIGcapsuleD03 , capsuleD00, capsuleD01, capsuleD02, capsuleD03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstateA, travoffsetsD);
				resetcapsules0( capsuleA00, capsuleA01, capsuleA02, capsuleA03, NAp);
				resetcapsules1( capsuleB00, capsuleB01, capsuleB02, capsuleB03, NAp);
				resetcapsules2( capsuleC00, capsuleC01, capsuleC02, capsuleC03, NAp);
				resetcapsules3( capsuleD00, capsuleD01, capsuleD02, capsuleD03, NAp);
			}
			resetcapsules0( capsuleA00, capsuleA01, capsuleA02, capsuleA03, NAp);
			resetcapsules1( capsuleB00, capsuleB01, capsuleB02, capsuleB03, NAp);
			resetcapsules2( capsuleC00, capsuleC01, capsuleC02, capsuleC03, NAp);
			resetcapsules3( capsuleD00, capsuleD01, capsuleD02, capsuleD03, NAp);
			
			// Partition phase
			travoffsetsA.capsuleoffset_kvs = 0;
			travoffsetsB.capsuleoffset_kvs = 0;
			travoffsetsC.capsuleoffset_kvs = 0;
			travoffsetsD.capsuleoffset_kvs = 0;
			PARTITIONPHASE_MAINLOOP1: for (int i = partitionparamsA.begin_kvs; i < partitionparamsA.end_kvs; i += partitionparamsA.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("PartitionPhase:: i", "i_end", "step size", "NAp", i, partitionparamsA.end_kvs, partitionparamsA.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparamsA.begin_kvs) / partitionparamsA.step_kvs) * NUM_PARTITIONS;
				travoffsetsA = readcapsules0(kvsourcedramA,  BIGcapsuleA00,  BIGcapsuleA01,  BIGcapsuleA02,  BIGcapsuleA03,  capsuleA00,capsuleA01,capsuleA02,capsuleA03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsetsA); // FIXME. include workerID in offset
				travoffsetsB = readcapsules1(kvsourcedramB,  BIGcapsuleB00,  BIGcapsuleB01,  BIGcapsuleB02,  BIGcapsuleB03,  capsuleB00,capsuleB01,capsuleB02,capsuleB03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsetsB); // FIXME. include workerID in offset
				travoffsetsC = readcapsules2(kvsourcedramC,  BIGcapsuleC00,  BIGcapsuleC01,  BIGcapsuleC02,  BIGcapsuleC03,  capsuleC00,capsuleC01,capsuleC02,capsuleC03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsetsC); // FIXME. include workerID in offset
				travoffsetsD = readcapsules3(kvsourcedramD,  BIGcapsuleD00,  BIGcapsuleD01,  BIGcapsuleD02,  BIGcapsuleD03,  capsuleD00,capsuleD01,capsuleD02,capsuleD03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travoffsetsD); // FIXME. include workerID in offset
				PARTITIONPHASE_MAINLOOP1B: for (int v = 0; v < NFACTOR; v += 1){
					travstateA.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE); travstateA.info = 1;
				
					readkeyvalues0(kvsourcedramA, sourcebufferA00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues0(kvsourcedramA, sourcebufferA01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues0(kvsourcedramA, sourcebufferA02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues0(kvsourcedramA, sourcebufferA03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues1(kvsourcedramB, sourcebufferB00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues1(kvsourcedramB, sourcebufferB01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues1(kvsourcedramB, sourcebufferB02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues1(kvsourcedramB, sourcebufferB03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues2(kvsourcedramC, sourcebufferC00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues2(kvsourcedramC, sourcebufferC01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues2(kvsourcedramC, sourcebufferC02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues2(kvsourcedramC, sourcebufferC03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues3(kvsourcedramD, sourcebufferD00, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 0 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues3(kvsourcedramD, sourcebufferD01, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 1 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues3(kvsourcedramD, sourcebufferD02, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 2 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					readkeyvalues3(kvsourcedramD, sourcebufferD03, llopparams.baseaddr_worksourcekvs_kvs, travstateA.i_kvs + (0 * 3 * SRCBUFFER_SIZE), SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
					
					partitionkeyvalues00(0  ,sourcebufferA00 ,sourcebufferA01 ,sourcebufferA02 ,sourcebufferA03  ,destbufferA00 ,destbufferA01 ,destbufferA02 ,destbufferA03  ,capsuleA00 ,capsuleA01 ,capsuleA02 ,capsuleA03, travstateA, llopparams);
					partitionkeyvalues10(0  ,sourcebufferB00 ,sourcebufferB01 ,sourcebufferB02 ,sourcebufferB03  ,destbufferB00 ,destbufferB01 ,destbufferB02 ,destbufferB03  ,capsuleB00 ,capsuleB01 ,capsuleB02 ,capsuleB03, travstateA, llopparams);
					partitionkeyvalues20(0  ,sourcebufferC00 ,sourcebufferC01 ,sourcebufferC02 ,sourcebufferC03  ,destbufferC00 ,destbufferC01 ,destbufferC02 ,destbufferC03  ,capsuleC00 ,capsuleC01 ,capsuleC02 ,capsuleC03, travstateA, llopparams);
					partitionkeyvalues30(0  ,sourcebufferD00 ,sourcebufferD01 ,sourcebufferD02 ,sourcebufferD03  ,destbufferD00 ,destbufferD01 ,destbufferD02 ,destbufferD03  ,capsuleD00 ,capsuleD01 ,capsuleD02 ,capsuleD03, travstateA, llopparams);
				}
				savepartitions0(kvsourcedramA , destbufferA00, destbufferA01, destbufferA02, destbufferA03 , capsuleA00, capsuleA01, capsuleA02, capsuleA03, kvdeststatsA_tmp, llopparams.baseaddr_workdestkvs_kvs);
				savepartitions1(kvsourcedramB , destbufferB00, destbufferB01, destbufferB02, destbufferB03 , capsuleB00, capsuleB01, capsuleB02, capsuleB03, kvdeststatsB_tmp, llopparams.baseaddr_workdestkvs_kvs);
				savepartitions2(kvsourcedramC , destbufferC00, destbufferC01, destbufferC02, destbufferC03 , capsuleC00, capsuleC01, capsuleC02, capsuleC03, kvdeststatsC_tmp, llopparams.baseaddr_workdestkvs_kvs);
				savepartitions3(kvsourcedramD , destbufferD00, destbufferD01, destbufferD02, destbufferD03 , capsuleD00, capsuleD01, capsuleD02, capsuleD03, kvdeststatsD_tmp, llopparams.baseaddr_workdestkvs_kvs);
				resetcapsules0(capsuleA00,capsuleA01,capsuleA02,capsuleA03, NAp);
				resetcapsules1(capsuleB00,capsuleB01,capsuleB02,capsuleB03, NAp);
				resetcapsules2(capsuleC00,capsuleC01,capsuleC02,capsuleC03, NAp);
				resetcapsules3(capsuleD00,capsuleD01,capsuleD02,capsuleD03, NAp);
			}
			
			// Reduce phase
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
			keyvalue_t sourcestatsA[NUMSUBWORKERS];
			unsigned int maxszA = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestatsA[i] = kvstatsA[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxszA = max(sourcestatsA[sw].value, maxszA); }
			unsigned int maxszA_kvs = (maxszA + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			keyvalue_t sourcestatsB[NUMSUBWORKERS];
			unsigned int maxszB = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestatsB[i] = kvstatsB[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxszB = max(sourcestatsB[sw].value, maxszB); }
			unsigned int maxszB_kvs = (maxszB + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			keyvalue_t sourcestatsC[NUMSUBWORKERS];
			unsigned int maxszC = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestatsC[i] = kvstatsC[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxszC = max(sourcestatsC[sw].value, maxszC); }
			unsigned int maxszC_kvs = (maxszC + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			keyvalue_t sourcestatsD[NUMSUBWORKERS];
			unsigned int maxszD = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestatsD[i] = kvstatsD[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxszD = max(sourcestatsD[sw].value, maxszD); }
			unsigned int maxszD_kvs = (maxszD + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			
			travstateA.info = 0;
			readkeyvalues0(kvdestdramA, destbufferA00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdramA, destbufferA01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdramA, destbufferA02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdramA, destbufferA03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdramB, destbufferB00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdramB, destbufferB01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdramB, destbufferB02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues1(kvdestdramB, destbufferB03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdramC, destbufferC00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdramC, destbufferC01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdramC, destbufferC02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues2(kvdestdramC, destbufferC03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdramD, destbufferD00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdramD, destbufferD01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdramD, destbufferD02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues3(kvdestdramD, destbufferD03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			REDUCEPHASE_MAINLOOP: for (int i = 0; i < maxszA_kvs; i += SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reducephase avg=analysis_reducephase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase:: i", "i_end", "NAp", "NAp", "skip size", i, maxszA_kvs, NAp, NAp, SRCBUFFER_SIZE);
				#endif
				travstateA.info = 1;

		
				readkeyvalues0(kvsourcedramA, sourcebufferA00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues0(kvsourcedramA, sourcebufferA01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues0(kvsourcedramA, sourcebufferA02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues0(kvsourcedramA, sourcebufferA03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedramB, sourcebufferB00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedramB, sourcebufferB01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedramB, sourcebufferB02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues1(kvsourcedramB, sourcebufferB03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedramC, sourcebufferC00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedramC, sourcebufferC01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedramC, sourcebufferC02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues2(kvsourcedramC, sourcebufferC03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedramD, sourcebufferD00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[0].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedramD, sourcebufferD01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[1].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedramD, sourcebufferD02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[2].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
		
				readkeyvalues3(kvsourcedramD, sourcebufferD03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestatsA[3].key / VECTOR_SIZE) + i, SRCBUFFER_SIZE, KVSOURCEDRAMSZ_KVS);
				
				reducepartitions0( sourcebufferA00,  sourcebufferA01,  sourcebufferA02,  sourcebufferA03,   destbufferA00,  destbufferA01,  destbufferA02,  destbufferA03,  travstateA, globalparams);
				reducepartitions1( sourcebufferB00,  sourcebufferB01,  sourcebufferB02,  sourcebufferB03,   destbufferB00,  destbufferB01,  destbufferB02,  destbufferB03,  travstateA, globalparams);
				reducepartitions2( sourcebufferC00,  sourcebufferC01,  sourcebufferC02,  sourcebufferC03,   destbufferC00,  destbufferC01,  destbufferC02,  destbufferC03,  travstateA, globalparams);
				reducepartitions3( sourcebufferD00,  sourcebufferD01,  sourcebufferD02,  sourcebufferD03,   destbufferD00,  destbufferD01,  destbufferD02,  destbufferD03,  travstateA, globalparams);
			}
			savekeyvalues0(kvdestdramA, destbufferA00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdramA, destbufferA01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdramA, destbufferA02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdramA, destbufferA03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdramB, destbufferB00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdramB, destbufferB01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdramB, destbufferB02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues1(kvdestdramB, destbufferB03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdramC, destbufferC00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdramC, destbufferC01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdramC, destbufferC02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues2(kvdestdramC, destbufferC03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdramD, destbufferD00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdramD, destbufferD01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdramD, destbufferD02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues3(kvdestdramD, destbufferD03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			}
			
			storedeststats(kvstatsA, kvdeststatsA_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			storedeststats(kvstatsB, kvdeststatsB_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			storedeststats(kvstatsC, kvdeststatsC_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			storedeststats(kvstatsD, kvdeststatsD_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			llopparams.nextsourceoffset_partition += llopparams.sourceskipsize_partition;
			llopparams.nextdestoffset_partition += (llopparams.destskipsize_partition * NUM_PARTITIONS);
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv * NUMSUBWORKERS; } // 
			else { llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv; }
			llopparams.nextdestoffset_kv += llopparams.destskipsize_kv;
		}
		#ifdef _DEBUGMODE_KERNELPRINTS3
		printglobalvars();
		clearglobalvars();
		#endif
	} */
	return;
}
}





