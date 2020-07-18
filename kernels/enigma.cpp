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
#define MMM

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
void enigma::printparameters(){
	cout<<endl<<"enigma::printparameters: test started."<<endl;
	cout<<"enigma::printparameters:: KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
	cout<<"enigma::printparameters:: KVDATA_BATCHSIZE_KVS: "<<KVDATA_BATCHSIZE_KVS<<endl;
	
	// cout<<"enigma::printparameters:: BUFFER_SIZE: "<<BUFFER_SIZE<<endl;
	cout<<"enigma::printparameters:: PADDEDBUFFER_SIZE: "<<PADDEDBUFFER_SIZE<<endl;
	
	cout<<"enigma::printparameters:: KVDRAMSZ: "<<KVDRAMSZ<<endl;
	cout<<"enigma::printparameters:: KVDRAMSZ_KVS: "<<KVDRAMSZ_KVS<<endl;
	
	cout<<"enigma::printparameters:: KVSOURCEDRAMSZ: "<<KVSOURCEDRAMSZ<<endl;
	cout<<"enigma::printparameters:: KVSOURCEDRAMSZ_KVS: "<<KVSOURCEDRAMSZ_KVS<<endl;
	
	cout<<"enigma::printparameters:: PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;
	cout<<"enigma::printparameters:: PADDEDKVSOURCEDRAMSZ_KVS: "<<PADDEDKVSOURCEDRAMSZ_KVS<<endl;
	
	cout<<"enigma::printparameters:: SRCBUFFER_SIZE: "<<SRCBUFFER_SIZE<<endl;
	cout<<"enigma::printparameters:: SRCBUFFER_SIZE * VECTOR_SIZE: "<<SRCBUFFER_SIZE * VECTOR_SIZE<<endl;
	
	// cout<<"enigma::printparameters:: DESTBUFFER_SIZE: "<<DESTBUFFER_SIZE<<endl;
	cout<<"enigma::printparameters:: PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;
	cout<<"enigma::printparameters:: PADDEDDESTBUFFER_SIZE * VECTOR_SIZE: "<<PADDEDDESTBUFFER_SIZE * VECTOR_SIZE<<endl;
	
	cout<<"enigma::printparameters:: PADDEDBUFFER_SIZE / NUM_PARTITIONS: "<<PADDEDBUFFER_SIZE / NUM_PARTITIONS<<endl;
	
	cout<<"enigma::printparameters:: NFACTOR: "<<NFACTOR<<endl;
	
	cout<<"enigma::printparameters:: PADDEDDESTBUFFER_SIZE * NUMSUBWORKERS (total dest_kvs memory): "<<(PADDEDDESTBUFFER_SIZE * NUMSUBWORKERS)<<endl;
	cout<<"enigma::printparameters:: PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * NUMSUBWORKERS (total dest memory): "<<(PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * NUMSUBWORKERS)<<endl;
}
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
keyvalue_t ** Debugger_Sizes1;
keyvalue_t ** Debugger_Sizes2;
unsigned int debugger_i;
void enigma::createDebuggers(){
	Debugger_Sizes1 = new keyvalue_t*[DEBUGGER_SIZE];
	Debugger_Sizes2 = new keyvalue_t*[DEBUGGER_SIZE];
	for(int i = 0; i < DEBUGGER_SIZE; ++i){
		Debugger_Sizes1[i] = new keyvalue_t[NUM_PARTITIONS];
		Debugger_Sizes2[i] = new keyvalue_t[NUM_PARTITIONS];
	}
	initializeDebuggers();
}
void enigma::initializeDebuggers(){
	for(unsigned int i=0; i<DEBUGGER_SIZE; i++){ // reset
		for(unsigned int p=0; p<NUM_PARTITIONS; p++){
			Debugger_Sizes1[i][p].key = 0;
			Debugger_Sizes1[i][p].value = 0;
			Debugger_Sizes2[i][p].key = 0;
			Debugger_Sizes2[i][p].value = 0;
		}
	}
	debugger_i = 0;
}
void enigma::debuggerA(keyvalue_t ** Debugger_Sizes, keyvalue_t sizes[NUM_PARTITIONS], unsigned int committype){
	checkoutofbounds("enigma::debuggerA 34", debugger_i, DEBUGGER_SIZE, debugger_i, DEBUGGER_SIZE, NAp);
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		Debugger_Sizes[debugger_i][p].key = 0;
		if(committype == 0){ Debugger_Sizes[debugger_i][p].value += sizes[p].value; }
		else if(committype == 1){ Debugger_Sizes[debugger_i][p].value = sizes[p].value; }
		else {}
		// cout<<"Data placed in sizes... Debugger_Sizes["<<debugger_i<<"]["<<p<<"].value: "<<Debugger_Sizes[debugger_i][p].value<<endl;
	}
	// exit(EXIT_SUCCESS);
}
void enigma::IsEqual(keyvalue_t ** data1, keyvalue_t ** data2, unsigned int _1stdimsize, unsigned int _2nddimsize){
	for(unsigned int i=0; i<_1stdimsize; i++){
		for(unsigned int p=0; p<_2nddimsize; p++){
			if((data1[i][p].key == data2[i][p].key) && (data1[i][p].value == data2[i][p].value)){} 
			else {
				cout<<"ERROR:IsEqual: unequal values seen: i: "<<i<<", p: "<<p<<endl;
				cout<<"data1["<<i<<"]["<<p<<"].key: "<<data1[i][p].key<<", data1["<<i<<"]["<<p<<"].value: "<<data1[i][p].value<<endl;
				cout<<"data2["<<i<<"]["<<p<<"].key: "<<data2[i][p].key<<", data2["<<i<<"]["<<p<<"].value: "<<data2[i][p].value<<endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	cout<<"SUCCESS:IsEqual: test passed. _1stdimsize: "<<_1stdimsize<<", _2nddimsize: "<<_2nddimsize<<endl;
	return;
}
#endif
#ifdef _DEBUGMODE_STATS
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
void enigma::checkandforce(unsigned int data, unsigned int upper_bound, unsigned int * datatoforce, unsigned int forceval){
	#pragma HLS INLINE
	return;
	/* if(data >= upper_bound){ *datatoforce = forceval; }
	return; */
}
int enigma::WithinValidRange(unsigned int val1, unsigned int val2){
	#pragma HLS INLINE
	if(val1 < val2){ return 1; }
	else { return 0; }
}

unsigned int enigma::getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	partition_type val = ((keyvalue.key - upperlimit) >> (KVDATA_RANGE_PERSSDPARTITION_POW - (NUM_PARTITIONS_POW * currentLOP)));
	partition_type hashedval = (((1 << NUM_PARTITIONS_POW) - 1) & (val >> (1 - 1)));
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::getpartition 35", hashedval, NUM_PARTITIONS, NAp, NAp, NAp);
	#endif
	return hashedval;
}
keyvalue_t enigma::getkeyvalue(uint512_dt * buffer, batch_type addr, batch_type maxaddr_kvs){
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
void enigma::setkeyvalue(uint512_dt * buffer, batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs){
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

void enigma::resetkeyandvalues(keyvalue_t * buffer, unsigned int size){
	#pragma HLS INLINE
	RESETKEYVANDVALUES_LOOP: for(partition_type p=0; p<size; p++){ buffer[p].key = 0; buffer[p].value = 0; }
}
void enigma::resetvalues(keyvalue_t * buffer, unsigned int size){
	#pragma HLS INLINE
	RESETVALUES_LOOP: for(partition_type p=0; p<size; p++){ buffer[p].value = 0; }
}
void enigma::copykeys(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	#pragma HLS INLINE
	COPYKEYS_LOOP: for(partition_type p=0; p<size; p++){ buffer2[p].key = buffer1[p].key; }
}
void enigma::copy(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	#pragma HLS INLINE
	COPYKEYS_LOOP: for(partition_type p=0; p<size; p++){ buffer2[p] = buffer1[p]; }
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
	#ifdef _DEBUGMODE_KERNELPRINTSX
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
void enigma::storedeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int drambaseoffset, unsigned int dramloffset){
	#pragma HLS INLINE
	batch_type offset = drambaseoffset + dramloffset;
	
	STOREDESTSTATS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvstats[getstatsAddr(offset + p)] = buffer[p];
		// cout<<"storedeststats:: kvstats["<<getstatsAddr(offset + p)<<"].key: "<<kvstats[getstatsAddr(offset + p)].key<<", kvstats["<<getstatsAddr(offset + p)<<"].value: "<<kvstats[getstatsAddr(offset + p)].value<<endl;
	}
	// exit(EXIT_SUCCESS);
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
		// capsule0[p].key = allignhigher_KV(capsule0[p-1].key + capsule0[p-1].value);
		capsule0[p].key = capsule0[p-1].key + capsule0[p-1].value;
		// capsule1[p].key = allignhigher_KV(capsule1[p-1].key + capsule1[p-1].value);
		capsule1[p].key = capsule1[p-1].key + capsule1[p-1].value;
		// capsule2[p].key = allignhigher_KV(capsule2[p-1].key + capsule2[p-1].value);
		capsule2[p].key = capsule2[p-1].key + capsule2[p-1].value;
		// capsule3[p].key = allignhigher_KV(capsule3[p-1].key + capsule3[p-1].value);
		capsule3[p].key = capsule3[p-1].key + capsule3[p-1].value;
	}
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::calculateoffsets 34", capsule0[NUM_PARTITIONS-1].key + capsule0[NUM_PARTITIONS-1].value, (SRCBUFFER_SIZE * NFACTOR * VECTOR_SIZE) + 1, capsule0[NUM_PARTITIONS-1].key, capsule0[NUM_PARTITIONS-1].value, PADDEDDESTBUFFER_SIZE);
	checkoutofbounds("enigma::calculateoffsets 34", capsule1[NUM_PARTITIONS-1].key + capsule1[NUM_PARTITIONS-1].value, (SRCBUFFER_SIZE * NFACTOR * VECTOR_SIZE) + 1, capsule1[NUM_PARTITIONS-1].key, capsule1[NUM_PARTITIONS-1].value, PADDEDDESTBUFFER_SIZE);
	checkoutofbounds("enigma::calculateoffsets 34", capsule2[NUM_PARTITIONS-1].key + capsule2[NUM_PARTITIONS-1].value, (SRCBUFFER_SIZE * NFACTOR * VECTOR_SIZE) + 1, capsule2[NUM_PARTITIONS-1].key, capsule2[NUM_PARTITIONS-1].value, PADDEDDESTBUFFER_SIZE);
	checkoutofbounds("enigma::calculateoffsets 34", capsule3[NUM_PARTITIONS-1].key + capsule3[NUM_PARTITIONS-1].value, (SRCBUFFER_SIZE * NFACTOR * VECTOR_SIZE) + 1, capsule3[NUM_PARTITIONS-1].key, capsule3[NUM_PARTITIONS-1].value, PADDEDDESTBUFFER_SIZE);
	#endif
	return;
}
void enigma::calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int offset){
	#pragma HLS INLINE
	capsule[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){ capsule[p].key = capsule[p-1].key + capsule[p-1].value; }
	PREPARECAPSULE_LOOP2: for (partition_type p = 0; p<NUM_PARTITIONS; p++){ capsule[p].key += offset; }
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
void enigma::getchunksize(buffer_type * chunk_size, buffer_type buffer_size, travstate_t travstate, unsigned int localoffset_kvs){
	#pragma HLS INLINE
	*chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset_kvs;
	if (i > travstate.end_kvs){ *chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ *chunk_size = travstate.end_kvs - i; }
	else {}
	checkandforce(*chunk_size, buffer_size + 1, chunk_size, 0);
	checkandforce(i + *chunk_size, PADDEDKVSOURCEDRAMSZ_KVS, chunk_size, 0);
	checkandforce(i + *chunk_size, PADDEDKVSOURCEDRAMSZ_KVS, &i, 0);
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("getchunksize 35", *chunk_size, buffer_size + 1, *chunk_size, buffer_size, NAp);
	checkoutofbounds("getchunksize 36", i + *chunk_size, PADDEDKVSOURCEDRAMSZ_KVS + 1, i, *chunk_size, PADDEDKVSOURCEDRAMSZ_KVS);
	#endif
	return;
}
int enigma::IsReducePhase(unsigned int currentLOP, unsigned int treedepth, globalparams_t globalparams){
	if((currentLOP == (treedepth + 1)) && (globalparams.reducecommand == ON)){ return 1; }
	return 0;
}

void enigma::readkeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs, int enable, int collectstats){			
	if(enable == 0){ return; }
	unsigned int analysis_buffersz = SRCBUFFER_SIZE;
	
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &size_kvs, 0);
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &offset_kvs, 0);
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::readkeyvalues dram 36", baseaddress + offset_kvs + size_kvs, maxaddress_kvs, baseaddress, offset_kvs, size_kvs);
	#endif
	READBUFFER_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	   #pragma HLS PIPELINE II=1
	   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz	
		buffer[i] = kvdram[baseaddress + offset_kvs + i];
		#ifdef _DEBUGMODE_STATS
		if(collectstats==1){ globalstats_countkvsread(VECTOR_SIZE); }
		/// globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}
void enigma::readkeyvalues0(unsigned int workerID, uint512_dt * kvdram  ,uint512_dt * buffer0  ,uint512_dt * buffer1  ,uint512_dt * buffer2  ,uint512_dt * buffer3  ,batch_type baseaddress ,batch_type offset_kvs  ,buffer_type size0_kvs  ,buffer_type size1_kvs  ,buffer_type size2_kvs  ,buffer_type size3_kvs  ,batch_type maxaddress_kvs, int enable, int collectstats){			
	if(enable == 0){ return; }
	
	readkeyvalues0(kvdram, buffer0, baseaddress, offset_kvs + (workerID * 0 * SRCBUFFER_SIZE), size0_kvs, maxaddress_kvs, enable, collectstats);
	readkeyvalues0(kvdram, buffer1, baseaddress, offset_kvs + (workerID * 1 * SRCBUFFER_SIZE), size1_kvs, maxaddress_kvs, enable, collectstats);
	readkeyvalues0(kvdram, buffer2, baseaddress, offset_kvs + (workerID * 2 * SRCBUFFER_SIZE), size2_kvs, maxaddress_kvs, enable, collectstats);
	readkeyvalues0(kvdram, buffer3, baseaddress, offset_kvs + (workerID * 3 * SRCBUFFER_SIZE), size3_kvs, maxaddress_kvs, enable, collectstats);
	return;
}
travoffsets_t enigma::readcapsules0(unsigned int workerID, uint512_dt * kvdram,  uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE], keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  batch_type baseaddress_kvs, batch_type offset_kvs, int enable, travoffsets_t travoffsets){
	if(enable == 0){ return travoffsets; }
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::readcapsules0 35", (baseaddress_kvs + offset_kvs + NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	if(loffset == 0){
		batch_type loadoffset0_kvs = travoffsets.capsuleoffset_kvs + 0 * CAPSULEBUFFER_SIZE; 
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules0 34", baseaddress_kvs + loadoffset0_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule0, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADCAPSULES_PARTITION);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset1_kvs = travoffsets.capsuleoffset_kvs + 1 * CAPSULEBUFFER_SIZE; 
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules0 34", baseaddress_kvs + loadoffset1_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADCAPSULES_PARTITION);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset2_kvs = travoffsets.capsuleoffset_kvs + 2 * CAPSULEBUFFER_SIZE; 
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules0 34", baseaddress_kvs + loadoffset2_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule2, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADCAPSULES_PARTITION);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset3_kvs = travoffsets.capsuleoffset_kvs + 3 * CAPSULEBUFFER_SIZE; 
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::readcapsules0 34", baseaddress_kvs + loadoffset3_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule3, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADCAPSULES_PARTITION);
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

void enigma::King00(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS] ,int enable, clopparams_t llopparams, travstate_t travstate, globalparams_t globalparams, operation_t operation){						
	if(enable == 0){ return; }
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	KING0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		if(operation.type == EXTRACTCAPSULES){
			sizes0[p].value = 0;
			sizes1[p].value = 0;
			sizes2[p].value = 0;
			sizes3[p].value = 0;
		} else if(operation.type == PARTITIONKEYVALUES){
 
			sizes0[p].key = capsule0[p].key; 
			sizes0[p].value = capsule0[p].value; 
 
			sizes1[p].key = capsule1[p].key; 
			sizes1[p].value = capsule1[p].value; 
 
			sizes2[p].key = capsule2[p].key; 
			sizes2[p].value = capsule2[p].value; 
 
			sizes3[p].key = capsule3[p].key; 
			sizes3[p].value = capsule3[p].value; 
 
		} else {}
	}
	
	KING0_MAINLOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE * VECTOR_SIZE; i++){
	#pragma HLS PIPELINE II=1
		// load 
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, i, SRCBUFFER_SIZE);
		
		// operate
		partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
		
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		// operate
		if(operation.type == EXTRACTCAPSULES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		} else if(operation.type == PARTITIONKEYVALUES){
			setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		} else {}
		
		// stats
		#ifdef _DEBUGMODE_STATS
		if(operation.type == EXTRACTCAPSULES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
		} else if(operation.type == PARTITIONKEYVALUES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
		} else {}
		#endif
	}
	
	#ifdef _DEBUGMODE_CHECKSM
	if(operation.type == EXTRACTCAPSULES){ debuggerA(Debugger_Sizes1, sizes0, 0); } // Debugger
	if(operation.type == PARTITIONKEYVALUES){ debuggerA(Debugger_Sizes2, sizes0, 1); } // Debugger
	#endif 
	KING0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		if(operation.type == EXTRACTCAPSULES){
			capsule0[p].key = 0;
			capsule0[p].value += sizes0[p].value;
			capsule1[p].key = 0;
			capsule1[p].value += sizes1[p].value;
			capsule2[p].key = 0;
			capsule2[p].value += sizes2[p].value;
			capsule3[p].key = 0;
			capsule3[p].value += sizes3[p].value;
		} else if(operation.type == PARTITIONKEYVALUES){
			capsule0[p].value = sizes0[p].value;
			capsule1[p].value = sizes1[p].value;
			capsule2[p].value = sizes2[p].value;
			capsule3[p].value = sizes3[p].value;
		} else { }
	}
	return;
}
void enigma::King01(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS] ,int enable, clopparams_t llopparams, travstate_t travstate, globalparams_t globalparams, operation_t operation){						
	if(enable == 0){ return; }
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	KING0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		if(operation.type == EXTRACTCAPSULES){
			sizes0[p].value = 0;
			sizes1[p].value = 0;
			sizes2[p].value = 0;
			sizes3[p].value = 0;
		} else if(operation.type == PARTITIONKEYVALUES){
 
			sizes0[p].key = capsule0[p].key; 
			sizes0[p].value = capsule0[p].value; 
 
			sizes1[p].key = capsule1[p].key; 
			sizes1[p].value = capsule1[p].value; 
 
			sizes2[p].key = capsule2[p].key; 
			sizes2[p].value = capsule2[p].value; 
 
			sizes3[p].key = capsule3[p].key; 
			sizes3[p].value = capsule3[p].value; 
 
		} else {}
	}
	
	KING0_MAINLOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE * VECTOR_SIZE; i++){
	#pragma HLS PIPELINE II=1
		// load 
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, i, SRCBUFFER_SIZE);
		
		// operate
		partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
		
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		// operate
		if(operation.type == EXTRACTCAPSULES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		} else if(operation.type == PARTITIONKEYVALUES){
			setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		} else {}
		
		// stats
		#ifdef _DEBUGMODE_STATS
		if(operation.type == EXTRACTCAPSULES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
		} else if(operation.type == PARTITIONKEYVALUES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
		} else {}
		#endif
	}
	
	#ifdef _DEBUGMODE_CHECKSM
	if(operation.type == EXTRACTCAPSULES){ debuggerA(Debugger_Sizes1, sizes0, 0); } // Debugger
	if(operation.type == PARTITIONKEYVALUES){ debuggerA(Debugger_Sizes2, sizes0, 1); } // Debugger
	#endif 
	KING0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		if(operation.type == EXTRACTCAPSULES){
			capsule0[p].key = 0;
			capsule0[p].value += sizes0[p].value;
			capsule1[p].key = 0;
			capsule1[p].value += sizes1[p].value;
			capsule2[p].key = 0;
			capsule2[p].value += sizes2[p].value;
			capsule3[p].key = 0;
			capsule3[p].value += sizes3[p].value;
		} else if(operation.type == PARTITIONKEYVALUES){
			capsule0[p].value = sizes0[p].value;
			capsule1[p].value = sizes1[p].value;
			capsule2[p].value = sizes2[p].value;
			capsule3[p].value = sizes3[p].value;
		} else { }
	}
	return;
}
void enigma::King02(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS] ,int enable, clopparams_t llopparams, travstate_t travstate, globalparams_t globalparams, operation_t operation){						
	if(enable == 0){ return; }
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	KING0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		if(operation.type == EXTRACTCAPSULES){
			sizes0[p].value = 0;
			sizes1[p].value = 0;
			sizes2[p].value = 0;
			sizes3[p].value = 0;
		} else if(operation.type == PARTITIONKEYVALUES){
 
			sizes0[p].key = capsule0[p].key; 
			sizes0[p].value = capsule0[p].value; 
 
			sizes1[p].key = capsule1[p].key; 
			sizes1[p].value = capsule1[p].value; 
 
			sizes2[p].key = capsule2[p].key; 
			sizes2[p].value = capsule2[p].value; 
 
			sizes3[p].key = capsule3[p].key; 
			sizes3[p].value = capsule3[p].value; 
 
		} else {}
	}
	
	KING0_MAINLOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE * VECTOR_SIZE; i++){
	#pragma HLS PIPELINE II=1
		// load 
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, i, SRCBUFFER_SIZE);
		
		// operate
		partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
		
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		// operate
		if(operation.type == EXTRACTCAPSULES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		} else if(operation.type == PARTITIONKEYVALUES){
			setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		} else {}
		
		// stats
		#ifdef _DEBUGMODE_STATS
		if(operation.type == EXTRACTCAPSULES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
		} else if(operation.type == PARTITIONKEYVALUES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
		} else {}
		#endif
	}
	
	#ifdef _DEBUGMODE_CHECKSM
	if(operation.type == EXTRACTCAPSULES){ debuggerA(Debugger_Sizes1, sizes0, 0); } // Debugger
	if(operation.type == PARTITIONKEYVALUES){ debuggerA(Debugger_Sizes2, sizes0, 1); } // Debugger
	#endif 
	KING0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		if(operation.type == EXTRACTCAPSULES){
			capsule0[p].key = 0;
			capsule0[p].value += sizes0[p].value;
			capsule1[p].key = 0;
			capsule1[p].value += sizes1[p].value;
			capsule2[p].key = 0;
			capsule2[p].value += sizes2[p].value;
			capsule3[p].key = 0;
			capsule3[p].value += sizes3[p].value;
		} else if(operation.type == PARTITIONKEYVALUES){
			capsule0[p].value = sizes0[p].value;
			capsule1[p].value = sizes1[p].value;
			capsule2[p].value = sizes2[p].value;
			capsule3[p].value = sizes3[p].value;
		} else { }
	}
	return;
}
void enigma::King03(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS] ,int enable, clopparams_t llopparams, travstate_t travstate, globalparams_t globalparams, operation_t operation){						
	if(enable == 0){ return; }
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	KING0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		if(operation.type == EXTRACTCAPSULES){
			sizes0[p].value = 0;
			sizes1[p].value = 0;
			sizes2[p].value = 0;
			sizes3[p].value = 0;
		} else if(operation.type == PARTITIONKEYVALUES){
 
			sizes0[p].key = capsule0[p].key; 
			sizes0[p].value = capsule0[p].value; 
 
			sizes1[p].key = capsule1[p].key; 
			sizes1[p].value = capsule1[p].value; 
 
			sizes2[p].key = capsule2[p].key; 
			sizes2[p].value = capsule2[p].value; 
 
			sizes3[p].key = capsule3[p].key; 
			sizes3[p].value = capsule3[p].value; 
 
		} else {}
	}
	
	KING0_MAINLOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE * VECTOR_SIZE; i++){
	#pragma HLS PIPELINE II=1
		// load 
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, i, SRCBUFFER_SIZE);
		
		// operate
		partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
		
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		// operate
		if(operation.type == EXTRACTCAPSULES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		} else if(operation.type == PARTITIONKEYVALUES){
			setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
			setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		} else {}
		
		// stats
		#ifdef _DEBUGMODE_STATS
		if(operation.type == EXTRACTCAPSULES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ globalvar_countkvsread4statscollection(1); }
		} else if(operation.type == PARTITIONKEYVALUES){
			if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
			if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
		} else {}
		#endif
	}
	
	#ifdef _DEBUGMODE_CHECKSM
	if(operation.type == EXTRACTCAPSULES){ debuggerA(Debugger_Sizes1, sizes0, 0); } // Debugger
	if(operation.type == PARTITIONKEYVALUES){ debuggerA(Debugger_Sizes2, sizes0, 1); } // Debugger
	#endif 
	KING0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		if(operation.type == EXTRACTCAPSULES){
			capsule0[p].key = 0;
			capsule0[p].value += sizes0[p].value;
			capsule1[p].key = 0;
			capsule1[p].value += sizes1[p].value;
			capsule2[p].key = 0;
			capsule2[p].value += sizes2[p].value;
			capsule3[p].key = 0;
			capsule3[p].value += sizes3[p].value;
		} else if(operation.type == PARTITIONKEYVALUES){
			capsule0[p].value = sizes0[p].value;
			capsule1[p].value = sizes1[p].value;
			capsule2[p].value = sizes2[p].value;
			capsule3[p].value = sizes3[p].value;
		} else { }
	}
	return;
}
void enigma::Queen00(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] ,int enable, travstate_t travstate, globalparams_t globalparams){						
	if(enable == 0){ return; }
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	QUEEN0_MAINLOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE * VECTOR_SIZE; i++){
	// #pragma HLS PIPELINE II=1
		// load 
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, i, SRCBUFFER_SIZE);
		
		// operate
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		// operate
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
		
		// stats
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(NUMSUBWORKERS);
		#endif
	}
	return;
}
void enigma::Queen01(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] ,int enable, travstate_t travstate, globalparams_t globalparams){						
	if(enable == 0){ return; }
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	QUEEN0_MAINLOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE * VECTOR_SIZE; i++){
	// #pragma HLS PIPELINE II=1
		// load 
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, i, SRCBUFFER_SIZE);
		
		// operate
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		// operate
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
		
		// stats
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(NUMSUBWORKERS);
		#endif
	}
	return;
}
void enigma::Queen02(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] ,int enable, travstate_t travstate, globalparams_t globalparams){						
	if(enable == 0){ return; }
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	QUEEN0_MAINLOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE * VECTOR_SIZE; i++){
	// #pragma HLS PIPELINE II=1
		// load 
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, i, SRCBUFFER_SIZE);
		
		// operate
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		// operate
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
		
		// stats
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(NUMSUBWORKERS);
		#endif
	}
	return;
}
void enigma::Queen03(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] ,int enable, travstate_t travstate, globalparams_t globalparams){						
	if(enable == 0){ return; }
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	QUEEN0_MAINLOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE * VECTOR_SIZE; i++){
	// #pragma HLS PIPELINE II=1
		// load 
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, i, SRCBUFFER_SIZE);
		keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, i, SRCBUFFER_SIZE);
		
		// operate
		vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
		vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
		vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
		vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
		if(loc0 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
		if(loc1 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
		if(loc2 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
		if(loc3 >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
		
		// operate
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
		
		// stats
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(NUMSUBWORKERS);
		#endif
	}
	return;
}

void enigma::extractcapsules00(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	EXTRACTCAPSULES0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p].value = 0;
		sizes1[p].value = 0;
		sizes2[p].value = 0;
		sizes3[p].value = 0;
	}
	EXTRACTCAPSULES0_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
			partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			// if(k < chunk0_size * VECTOR_SIZE){ sizes0[p0].value += 1; }
			/// sizes0[p0].value += 1;
			keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
			partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			// if(k < chunk1_size * VECTOR_SIZE){ sizes1[p1].value += 1; }
			/// sizes1[p1].value += 1;
			keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
			partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			// if(k < chunk2_size * VECTOR_SIZE){ sizes2[p2].value += 1; }
			/// sizes2[p2].value += 1;
			keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
			partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
			// if(k < chunk3_size * VECTOR_SIZE){ sizes3[p3].value += 1; }
			/// sizes3[p3].value += 1;
		#ifdef _DEBUGMODE_STATS
		globalvar_countkvsread4statscollection(NUMSUBWORKERS);
		#endif
	}
	EXTRACTCAPSULES0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule0[p].key = 0;
		capsule0[p].value += sizes0[p].value;
		capsule1[p].key = 0;
		capsule1[p].value += sizes1[p].value;
		capsule2[p].key = 0;
		capsule2[p].value += sizes2[p].value;
		capsule3[p].key = 0;
		capsule3[p].value += sizes3[p].value;
	}
	#ifdef _DEBUGMODE_CHECKSM
	debuggerA(Debugger_Sizes1, sizes0, 0); // Debugger
	#endif 
	return;
}
void enigma::extractcapsules01(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	EXTRACTCAPSULES0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p].value = 0;
		sizes1[p].value = 0;
		sizes2[p].value = 0;
		sizes3[p].value = 0;
	}
	EXTRACTCAPSULES0_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
			partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			// if(k < chunk0_size * VECTOR_SIZE){ sizes0[p0].value += 1; }
			/// sizes0[p0].value += 1;
			keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
			partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			// if(k < chunk1_size * VECTOR_SIZE){ sizes1[p1].value += 1; }
			/// sizes1[p1].value += 1;
			keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
			partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			// if(k < chunk2_size * VECTOR_SIZE){ sizes2[p2].value += 1; }
			/// sizes2[p2].value += 1;
			keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
			partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
			// if(k < chunk3_size * VECTOR_SIZE){ sizes3[p3].value += 1; }
			/// sizes3[p3].value += 1;
		#ifdef _DEBUGMODE_STATS
		globalvar_countkvsread4statscollection(NUMSUBWORKERS);
		#endif
	}
	EXTRACTCAPSULES0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule0[p].key = 0;
		capsule0[p].value += sizes0[p].value;
		capsule1[p].key = 0;
		capsule1[p].value += sizes1[p].value;
		capsule2[p].key = 0;
		capsule2[p].value += sizes2[p].value;
		capsule3[p].key = 0;
		capsule3[p].value += sizes3[p].value;
	}
	#ifdef _DEBUGMODE_CHECKSM
	debuggerA(Debugger_Sizes1, sizes0, 0); // Debugger
	#endif 
	return;
}
void enigma::extractcapsules02(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	EXTRACTCAPSULES0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p].value = 0;
		sizes1[p].value = 0;
		sizes2[p].value = 0;
		sizes3[p].value = 0;
	}
	EXTRACTCAPSULES0_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
			partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			// if(k < chunk0_size * VECTOR_SIZE){ sizes0[p0].value += 1; }
			/// sizes0[p0].value += 1;
			keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
			partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			// if(k < chunk1_size * VECTOR_SIZE){ sizes1[p1].value += 1; }
			/// sizes1[p1].value += 1;
			keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
			partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			// if(k < chunk2_size * VECTOR_SIZE){ sizes2[p2].value += 1; }
			/// sizes2[p2].value += 1;
			keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
			partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
			// if(k < chunk3_size * VECTOR_SIZE){ sizes3[p3].value += 1; }
			/// sizes3[p3].value += 1;
		#ifdef _DEBUGMODE_STATS
		globalvar_countkvsread4statscollection(NUMSUBWORKERS);
		#endif
	}
	EXTRACTCAPSULES0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule0[p].key = 0;
		capsule0[p].value += sizes0[p].value;
		capsule1[p].key = 0;
		capsule1[p].value += sizes1[p].value;
		capsule2[p].key = 0;
		capsule2[p].value += sizes2[p].value;
		capsule3[p].key = 0;
		capsule3[p].value += sizes3[p].value;
	}
	#ifdef _DEBUGMODE_CHECKSM
	debuggerA(Debugger_Sizes1, sizes0, 0); // Debugger
	#endif 
	return;
}
void enigma::extractcapsules03(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	keyvalue_t sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	keyvalue_t sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	keyvalue_t sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	
	EXTRACTCAPSULES0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p].value = 0;
		sizes1[p].value = 0;
		sizes2[p].value = 0;
		sizes3[p].value = 0;
	}
	EXTRACTCAPSULES0_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
			partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
			// if(k < chunk0_size * VECTOR_SIZE){ sizes0[p0].value += 1; }
			/// sizes0[p0].value += 1;
			keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
			partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			// if(k < chunk1_size * VECTOR_SIZE){ sizes1[p1].value += 1; }
			/// sizes1[p1].value += 1;
			keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
			partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			// if(k < chunk2_size * VECTOR_SIZE){ sizes2[p2].value += 1; }
			/// sizes2[p2].value += 1;
			keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
			partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
			// if(k < chunk3_size * VECTOR_SIZE){ sizes3[p3].value += 1; }
			/// sizes3[p3].value += 1;
		#ifdef _DEBUGMODE_STATS
		globalvar_countkvsread4statscollection(NUMSUBWORKERS);
		#endif
	}
	EXTRACTCAPSULES0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule0[p].key = 0;
		capsule0[p].value += sizes0[p].value;
		capsule1[p].key = 0;
		capsule1[p].value += sizes1[p].value;
		capsule2[p].key = 0;
		capsule2[p].value += sizes2[p].value;
		capsule3[p].key = 0;
		capsule3[p].value += sizes3[p].value;
	}
	#ifdef _DEBUGMODE_CHECKSM
	debuggerA(Debugger_Sizes1, sizes0, 0); // Debugger
	#endif 
	return;
}
void enigma::partitionkeyvalues00(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	unsigned int analysis_partition = SRCBUFFER_SIZE;
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
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
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes1[p1].key + sizes1[p1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes1[p1].key, sizes1[p1].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes2[p2].key + sizes2[p2].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes2[p2].key, sizes2[p2].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes3[p3].key + sizes3[p3].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes3[p3].key, sizes3[p3].value, NAp);
		#endif
		
		setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
		
		if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
		// if(k < chunk0_size * VECTOR_SIZE){ sizes0[p0].value += 1; }
		/// sizes0[p0].value += 1;
		if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
		// if(k < chunk1_size * VECTOR_SIZE){ sizes1[p1].value += 1; }
		/// sizes1[p1].value += 1;
		if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
		// if(k < chunk2_size * VECTOR_SIZE){ sizes2[p2].value += 1; }
		/// sizes2[p2].value += 1;
		if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		// if(k < chunk3_size * VECTOR_SIZE){ sizes3[p3].value += 1; }
		/// sizes3[p3].value += 1;
		
		#ifdef _DEBUGMODE_STATS
		if(k < chunk0_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk1_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk2_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk3_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  capsule1[p].value = sizes1[p].value;  capsule2[p].value = sizes2[p].value;  capsule3[p].value = sizes3[p].value;  }
	#ifdef _DEBUGMODE_CHECKSM
	debuggerA(Debugger_Sizes2, sizes0, 1); // Debugger
	#endif 
}
void enigma::partitionkeyvalues01(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	unsigned int analysis_partition = SRCBUFFER_SIZE;
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
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
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes1[p1].key + sizes1[p1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes1[p1].key, sizes1[p1].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes2[p2].key + sizes2[p2].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes2[p2].key, sizes2[p2].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes3[p3].key + sizes3[p3].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes3[p3].key, sizes3[p3].value, NAp);
		#endif
		
		setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
		
		if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
		// if(k < chunk0_size * VECTOR_SIZE){ sizes0[p0].value += 1; }
		/// sizes0[p0].value += 1;
		if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
		// if(k < chunk1_size * VECTOR_SIZE){ sizes1[p1].value += 1; }
		/// sizes1[p1].value += 1;
		if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
		// if(k < chunk2_size * VECTOR_SIZE){ sizes2[p2].value += 1; }
		/// sizes2[p2].value += 1;
		if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		// if(k < chunk3_size * VECTOR_SIZE){ sizes3[p3].value += 1; }
		/// sizes3[p3].value += 1;
		
		#ifdef _DEBUGMODE_STATS
		if(k < chunk0_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk1_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk2_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk3_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  capsule1[p].value = sizes1[p].value;  capsule2[p].value = sizes2[p].value;  capsule3[p].value = sizes3[p].value;  }
	#ifdef _DEBUGMODE_CHECKSM
	debuggerA(Debugger_Sizes2, sizes0, 1); // Debugger
	#endif 
}
void enigma::partitionkeyvalues02(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	unsigned int analysis_partition = SRCBUFFER_SIZE;
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
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
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes1[p1].key + sizes1[p1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes1[p1].key, sizes1[p1].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes2[p2].key + sizes2[p2].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes2[p2].key, sizes2[p2].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes3[p3].key + sizes3[p3].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes3[p3].key, sizes3[p3].value, NAp);
		#endif
		
		setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
		
		if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
		// if(k < chunk0_size * VECTOR_SIZE){ sizes0[p0].value += 1; }
		/// sizes0[p0].value += 1;
		if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
		// if(k < chunk1_size * VECTOR_SIZE){ sizes1[p1].value += 1; }
		/// sizes1[p1].value += 1;
		if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
		// if(k < chunk2_size * VECTOR_SIZE){ sizes2[p2].value += 1; }
		/// sizes2[p2].value += 1;
		if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		// if(k < chunk3_size * VECTOR_SIZE){ sizes3[p3].value += 1; }
		/// sizes3[p3].value += 1;
		
		#ifdef _DEBUGMODE_STATS
		if(k < chunk0_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk1_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk2_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk3_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  capsule1[p].value = sizes1[p].value;  capsule2[p].value = sizes2[p].value;  capsule3[p].value = sizes3[p].value;  }
	#ifdef _DEBUGMODE_CHECKSM
	debuggerA(Debugger_Sizes2, sizes0, 1); // Debugger
	#endif 
}
void enigma::partitionkeyvalues03(unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	unsigned int analysis_partition = SRCBUFFER_SIZE;
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
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
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes1[p1].key + sizes1[p1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes1[p1].key, sizes1[p1].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes2[p2].key + sizes2[p2].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes2[p2].key, sizes2[p2].value, NAp);
		checkoutofbounds("enigma::partitionkeyvalues0 34", sizes3[p3].key + sizes3[p3].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes3[p3].key, sizes3[p3].value, NAp);
		#endif
		
		setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
		
		if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
		// if(k < chunk0_size * VECTOR_SIZE){ sizes0[p0].value += 1; }
		/// sizes0[p0].value += 1;
		if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
		// if(k < chunk1_size * VECTOR_SIZE){ sizes1[p1].value += 1; }
		/// sizes1[p1].value += 1;
		if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
		// if(k < chunk2_size * VECTOR_SIZE){ sizes2[p2].value += 1; }
		/// sizes2[p2].value += 1;
		if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		// if(k < chunk3_size * VECTOR_SIZE){ sizes3[p3].value += 1; }
		/// sizes3[p3].value += 1;
		
		#ifdef _DEBUGMODE_STATS
		if(k < chunk0_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk1_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk2_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		if(k < chunk3_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
		/// globalstats_countkvspartitioned(1); 
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  capsule1[p].value = sizes1[p].value;  capsule2[p].value = sizes2[p].value;  capsule3[p].value = sizes3[p].value;  }
	#ifdef _DEBUGMODE_CHECKSM
	debuggerA(Debugger_Sizes2, sizes0, 1); // Debugger
	#endif 
}
void enigma::reducepartitions0(unsigned int workerID  ,uint512_dt sourcebuffer0[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer1[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer2[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer3[SRCBUFFER_SIZE]   ,uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE]  ,travstate_t travstate, globalparams_t globalparams){
	unsigned int analysis_buffersz = SRCBUFFER_SIZE * VECTOR_SIZE;
	
	REDUCE_MAINLOOP: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
	// #pragma HLS PIPELINE II=1
	// #pragma HLS PIPELINE II=2
	// #pragma HLS PIPELINE II=3
 // 1_seq, NUMSUBWORKERS_seq
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
 // 1_seq, NUMSUBWORKERS_seq
		keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
 // 1_seq, NUMSUBWORKERS_seq
		keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
 // 1_seq, NUMSUBWORKERS_seq
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

void enigma::savekeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs, int enable, int collectstats){			
	if(enable == 0){ return; }
	int analysis_savebuffer = PADDEDDESTBUFFER_SIZE;
	
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &size_kvs, 0);
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &offset_kvs, 0);
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::savekeyvalues dram 35", baseaddress + offset_kvs + size_kvs, maxaddress_kvs, NAp, NAp, NAp);
	#endif
	READBUFFER_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	   #pragma HLS PIPELINE II=1
	   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer	
		kvdram[baseaddress + offset_kvs + i] = buffer[i];
		#ifdef _DEBUGMODE_STATS
		if(collectstats==1){ globalstats_countkvswritten(VECTOR_SIZE); }
		/// globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	return;
}
travoffsets_t enigma::savecapsules0(unsigned int workerID, uint512_dt * kvdram , uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type baseaddress_kvs, batch_type offset_kvs, int enable, travstate_t travstate, travoffsets_t travoffsets){							
	if(enable == 0){ return travoffsets; }
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	// gathering global information
	APPENDCAPSULE0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		kvdeststats_tmp[p].value += capsule0[p].value;
	}
	APPENDCAPSULE1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		kvdeststats_tmp[p].value += capsule1[p].value;
	}
	APPENDCAPSULE2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		kvdeststats_tmp[p].value += capsule2[p].value;
	}
	APPENDCAPSULE3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		kvdeststats_tmp[p].value += capsule3[p].value;
	}
	
	calculateoffsets(capsule0,capsule1,capsule2,capsule3, NAp);
	SAVECAPSULE0_LOOP2: for(partition_type i=0; i<NUM_PARTITIONS; i++){
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
		
		checkandforce(baseaddress_kvs + saveoffset0_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, &saveoffset0_kvs, 0);
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules0 34", baseaddress_kvs + saveoffset0_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule0, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENSAVECAPSULES_COLLECTSTATS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset1_kvs = travoffsets.capsuleoffset_kvs + (1 * CAPSULEBUFFER_SIZE);
		
		checkandforce(baseaddress_kvs + saveoffset1_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, &saveoffset1_kvs, 0);
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules0 34", baseaddress_kvs + saveoffset1_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENSAVECAPSULES_COLLECTSTATS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset2_kvs = travoffsets.capsuleoffset_kvs + (2 * CAPSULEBUFFER_SIZE);
		
		checkandforce(baseaddress_kvs + saveoffset2_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, &saveoffset2_kvs, 0);
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules0 34", baseaddress_kvs + saveoffset2_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule2, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENSAVECAPSULES_COLLECTSTATS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset3_kvs = travoffsets.capsuleoffset_kvs + (3 * CAPSULEBUFFER_SIZE);
		
		checkandforce(baseaddress_kvs + saveoffset3_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, &saveoffset3_kvs, 0);
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("enigma::savecapsules0 34", baseaddress_kvs + saveoffset3_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule3, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENSAVECAPSULES_COLLECTSTATS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		travoffsets.capsuleoffset_kvs += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	return travoffsets;
}
void enigma::savepartitions0(unsigned int workerID, uint512_dt * kvdram , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS] ,keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type kvdrambaseaddress, int enable){				
	if(enable == 0){ return; }
	int analysis_savebuffer = PADDEDBUFFER_SIZE / NUM_PARTITIONS;
	
		SAVEPARTITIONS0_MAINLOOP1A: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = capsule0[p].key / VECTOR_SIZE; // FIXME. TRUNCATED.
			buffer_type size_kvs = capsule0[p].value / VECTOR_SIZE;
			// buffer_type bramoffset_kvs = (capsule0[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			// buffer_type size_kvs = (capsule0[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			checkandforce(bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, &size_kvs, 0);
			checkandforce(bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, &bramoffset_kvs, 0);
			checkandforce(dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, &size_kvs, 0);
			checkandforce(dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, &dramoffset_kvs, 0);
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions0 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions0 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule0[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			SAVEPARTITIONS0_MAINLOOP1B: for(buffer_type i=0; i<PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			SAVEPARTITIONS0_MAINLOOP1B: for(buffer_type i=0; i<size_kvs; i++)
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
		SAVEPARTITIONS1_MAINLOOP1A: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = capsule1[p].key / VECTOR_SIZE; // FIXME. TRUNCATED.
			buffer_type size_kvs = capsule1[p].value / VECTOR_SIZE;
			// buffer_type bramoffset_kvs = (capsule1[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			// buffer_type size_kvs = (capsule1[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			checkandforce(bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, &size_kvs, 0);
			checkandforce(bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, &bramoffset_kvs, 0);
			checkandforce(dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, &size_kvs, 0);
			checkandforce(dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, &dramoffset_kvs, 0);
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions1 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions1 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule1[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			SAVEPARTITIONS1_MAINLOOP1B: for(buffer_type i=0; i<PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			SAVEPARTITIONS1_MAINLOOP1B: for(buffer_type i=0; i<size_kvs; i++)
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
		SAVEPARTITIONS2_MAINLOOP1A: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = capsule2[p].key / VECTOR_SIZE; // FIXME. TRUNCATED.
			buffer_type size_kvs = capsule2[p].value / VECTOR_SIZE;
			// buffer_type bramoffset_kvs = (capsule2[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			// buffer_type size_kvs = (capsule2[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			checkandforce(bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, &size_kvs, 0);
			checkandforce(bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, &bramoffset_kvs, 0);
			checkandforce(dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, &size_kvs, 0);
			checkandforce(dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, &dramoffset_kvs, 0);
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions2 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions2 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule2[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			SAVEPARTITIONS2_MAINLOOP1B: for(buffer_type i=0; i<PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			SAVEPARTITIONS2_MAINLOOP1B: for(buffer_type i=0; i<size_kvs; i++)
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
		SAVEPARTITIONS3_MAINLOOP1A: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = kvdrambaseaddress + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = capsule3[p].key / VECTOR_SIZE; // FIXME. TRUNCATED.
			buffer_type size_kvs = capsule3[p].value / VECTOR_SIZE;
			// buffer_type bramoffset_kvs = (capsule3[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			// buffer_type size_kvs = (capsule3[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			checkandforce(bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, &size_kvs, 0);
			checkandforce(bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, &bramoffset_kvs, 0);
			checkandforce(dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, &size_kvs, 0);
			checkandforce(dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, &dramoffset_kvs, 0);
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::savepartitions3 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("enigma::savepartitions3 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule3[p].key, size_kvs);
			#endif

			#ifdef FPGA_IMPL
			SAVEPARTITIONS3_MAINLOOP1B: for(buffer_type i=0; i<PADDEDDESTBUFFER_SIZE / NUM_PARTITIONS; i++) // FIXME. FORCEDFINISH
			#else 
			SAVEPARTITIONS3_MAINLOOP1B: for(buffer_type i=0; i<size_kvs; i++)
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

void enigma::resetcapsules0(keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  int enableresetdebugger){
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
	int analysis_chunksize = SRCBUFFER_SIZE;
	int analysis_iterationcount = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS2
	print4("### loadKvDRAM0:: sourceoffset_kvs", "destoffset_kvs", "size_kvs", "NAp", sourceoffset_kvs, destoffset_kvs, size_kvs, NAp);
	#endif
	
	uint512_dt sourcebuffer[SRCBUFFER_SIZE];
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadKvDRAM0 35", destoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	int i;
    for ( i = 0; i < size_kvs; i += SRCBUFFER_SIZE) {
	#pragma HLS LOOP_TRIPCOUNT min=analysis_iterationcount max=analysis_iterationcount
		
		//boundary checks
		int chunk_size = SRCBUFFER_SIZE;
        if ((i + SRCBUFFER_SIZE) > size_kvs)
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
void enigma::Throne0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
	int analysis_maxtreedepthsz = 1;
	int analysis_avgnumsourcepartitions = 1;
	int analysis_partitionphase = (KVDRAMSZ_KVS / (NUMWORKERS * SRCBUFFER_SIZE)) / VECTOR_SIZE;
	int analysis_reducephase = 1;
	int analysis_chunksize = SRCBUFFER_SIZE;
	int analysis_iterationcount = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	
	uint512_dt sourcebuffer00[SRCBUFFER_SIZE];
	uint512_dt destbuffer00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule00[NUM_PARTITIONS];
	uint512_dt BIGcapsule00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer01[SRCBUFFER_SIZE];
	uint512_dt destbuffer01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule01[NUM_PARTITIONS];
	uint512_dt BIGcapsule01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer02[SRCBUFFER_SIZE];
	uint512_dt destbuffer02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule02[NUM_PARTITIONS];
	uint512_dt BIGcapsule02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer03[SRCBUFFER_SIZE];
	uint512_dt destbuffer03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule03[NUM_PARTITIONS];
	uint512_dt BIGcapsule03[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer10[SRCBUFFER_SIZE];
	uint512_dt destbuffer10[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule10[NUM_PARTITIONS];
	uint512_dt BIGcapsule10[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer11[SRCBUFFER_SIZE];
	uint512_dt destbuffer11[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule11[NUM_PARTITIONS];
	uint512_dt BIGcapsule11[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer12[SRCBUFFER_SIZE];
	uint512_dt destbuffer12[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule12[NUM_PARTITIONS];
	uint512_dt BIGcapsule12[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer13[SRCBUFFER_SIZE];
	uint512_dt destbuffer13[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule13[NUM_PARTITIONS];
	uint512_dt BIGcapsule13[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer20[SRCBUFFER_SIZE];
	uint512_dt destbuffer20[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule20[NUM_PARTITIONS];
	uint512_dt BIGcapsule20[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer21[SRCBUFFER_SIZE];
	uint512_dt destbuffer21[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule21[NUM_PARTITIONS];
	uint512_dt BIGcapsule21[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer22[SRCBUFFER_SIZE];
	uint512_dt destbuffer22[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule22[NUM_PARTITIONS];
	uint512_dt BIGcapsule22[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer23[SRCBUFFER_SIZE];
	uint512_dt destbuffer23[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule23[NUM_PARTITIONS];
	uint512_dt BIGcapsule23[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer30[SRCBUFFER_SIZE];
	uint512_dt destbuffer30[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule30[NUM_PARTITIONS];
	uint512_dt BIGcapsule30[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer31[SRCBUFFER_SIZE];
	uint512_dt destbuffer31[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule31[NUM_PARTITIONS];
	uint512_dt BIGcapsule31[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer32[SRCBUFFER_SIZE];
	uint512_dt destbuffer32[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule32[NUM_PARTITIONS];
	uint512_dt BIGcapsule32[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer33[SRCBUFFER_SIZE];
	uint512_dt destbuffer33[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule33[NUM_PARTITIONS];
	uint512_dt BIGcapsule33[CAPSULEBUFFER_SIZE];
	
	keyvalue_t sourcestats[NUMSUBWORKERS];
	int enable_readcapsules = 0; 
	int enable_savecapsules = 0; 
	int enable_readkeyvalues = 0;
	int enable_savekeyvalues = 0;
	int enable_savepartitions = 0;
	int enable_extractandpartition = 0;
	int enable_reduce = 0;
	operation_t operation;
	
	resetcapsules0(capsule00,capsule01,capsule02,capsule03, 0);
	resetcapsules0(capsule10,capsule11,capsule12,capsule13, 0);
	resetcapsules0(capsule20,capsule21,capsule22,capsule23, 0);
	resetcapsules0(capsule30,capsule31,capsule32,capsule33, 0);
	unsigned int numLOPs = 0;
	if(globalparams.runkernelcommand == ON){
		if(globalparams.reducecommand == ON){ numLOPs = globalparams.treedepth + 1 + 1; }
		else { numLOPs = globalparams.treedepth + 1; }
	} else { numLOPs = 1; }
	unsigned int loadkvdramsize_kvs = 0;
	if(globalparams.runkernelcommand == ON){ loadkvdramsize_kvs = 0; }
	else { loadkvdramsize_kvs = KVDATA_BATCHSIZE_KVS; }
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 1;
	
	// Load KvDrams
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadKvDRAM0 35", ((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS + KVDATA_BATCHSIZE_KVS, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	if(globalparams.runkernelcommand == ON){ cout<<"...running Acts..."<<endl; } // printparameters();
	else { cout<<"...loading KvDRAM..."<<endl; }
	#endif
	int i;
	LOADKVDRAMPHASE_LOOP1: for (i = 0; i < loadkvdramsize_kvs; i += SRCBUFFER_SIZE){
	#pragma HLS LOOP_TRIPCOUNT min=analysis_iterationcount max=analysis_iterationcount
		int chunk_size = SRCBUFFER_SIZE;
		if ((i + SRCBUFFER_SIZE) > loadkvdramsize_kvs){ chunk_size = loadkvdramsize_kvs - i; }
		readkeyvalues0(kvsourcedram, sourcebuffer00, 0, 0 + i, chunk_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, 0);
		savekeyvalues0(kvsourcedram, sourcebuffer00, 0, (((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS) + i, chunk_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, 0);
	}

	// Partition Phase
	THRONE_MAINLOOP1: for(unsigned int currentLOP=1; currentLOP<numLOPs; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		keyvalue_t kvdeststats_tmp2[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		unsigned int num_source_partition_skipsize = 0;
		if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ num_source_partition_skipsize = NUMSUBWORKERS; }
		else { num_source_partition_skipsize = 1; }
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<endl;
		print4("PartitionPhase0:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		THRONE_MAINLOOP1B: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=num_source_partition_skipsize){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partitionparams;
			travstate_t travstate;
			travstate_t travstates[NUMSUBWORKERS];
			travoffsets_t travoffsets;
			unsigned int nfactor;
			
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			keyvalue_t sourcestat = kvstats[getstatsAddr(sourcestatsmarker)];
			partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparams.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			nfactor = NFACTOR;
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparams.begin_kvs = 0; partitionparams.size_kvs = 0; partitionparams.step_kvs = 1; partitionparams.end_kvs = 0; } // to prevent divide-by-zero error
			travstate.begin_kvs = partitionparams.begin_kvs;
			travstate.end_kvs = partitionparams.end_kvs;
			travstate.skip_kvs = partitionparams.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * SRCBUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * SRCBUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			resetkeyandvalues(kvdeststats_tmp, NUM_PARTITIONS);
			resetkeyandvalues(kvdeststats_tmp2, NUM_PARTITIONS);
			
			THRONE_MAINLOOP1C: for(unsigned int op=0; op<3; op++){
				if(op == 0){ operation.type = EXTRACTCAPSULES; }
				else if(op == 1) { operation.type = PARTITIONKEYVALUES; }
				else if(op == 2) { operation.type = REDUCEKEYVALUES; }
				else {}
				travoffsets.capsuleoffset_kvs = 0;
				
				enable_readcapsules = 0; 
				enable_savecapsules = 0; 
				enable_readkeyvalues = 0;
				enable_savekeyvalues = 0;
				enable_savepartitions = 0;
				enable_extractandpartition = 0;
				enable_reduce = 0;
				if(operation.type == EXTRACTCAPSULES){ enable_extractandpartition = 1; enable_savecapsules = 1; }
				else if(operation.type == PARTITIONKEYVALUES){ enable_readcapsules = 1; enable_extractandpartition = 1; enable_savepartitions = 1; }
				else if(operation.type == REDUCEKEYVALUES){ enable_readkeyvalues = 1; enable_reduce = 1; enable_savekeyvalues = 1; }
				else {}
				
				#ifdef MMM
				if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){
					// leave as is
				} else if(operation.type == REDUCEKEYVALUES){
					if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
						unsigned int maxsz = 0;
						for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats[i] = kvstats[getstatsAddr(sourcestatsmarker + i)]; }
						for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxsz = max(sourcestats[sw].value, maxsz); }
						unsigned int maxsz_kvs = (maxsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
						travstates[sw].begin_kvs = 0;
						travstates[sw].end_kvs = (sourcestats[sw].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						travstates[sw].skip_kvs = SRCBUFFER_SIZE; }
						
						partitionparams.begin_kvs = 0;
						partitionparams.size_kvs = maxsz_kvs;
						partitionparams.step_kvs = SRCBUFFER_SIZE;
						partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
						nfactor = 1;
					} else {
						enable_readkeyvalues = 0; 
						enable_savekeyvalues = 0; 
						partitionparams.begin_kvs = 0;
						partitionparams.size_kvs = 0;
						partitionparams.step_kvs = 1; // to prevent divide-by-zero error
						partitionparams.end_kvs = 0;
					}
				}
				#endif 
				
				readkeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, enable_readkeyvalues, ENREADVPROPS_REDUCE); // reduce
				readkeyvalues0(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, enable_readkeyvalues, ENREADVPROPS_REDUCE); // reduce
				readkeyvalues0(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, enable_readkeyvalues, ENREADVPROPS_REDUCE); // reduce
				readkeyvalues0(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, enable_readkeyvalues, ENREADVPROPS_REDUCE); // reduce
					
				THRONE_MAINLOOP1D: for(int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
					#ifdef _DEBUGMODE_KERNELPRINTS
					if(operation.type == EXTRACTCAPSULES){ print4("Collectstats0:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp); }
					if(operation.type == PARTITIONKEYVALUES){ print4("PartitionKVs 0:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp); }
					if(operation.type == REDUCEKEYVALUES){ print4("ReduceKVs 0:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp); }
					#endif
				
					batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / (NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE)) * NUM_PARTITIONS;
					// batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
					travoffsets = readcapsules0(0, kvsourcedram,  BIGcapsule00,  BIGcapsule01,  BIGcapsule02,  BIGcapsule03,  capsule00,capsule01,capsule02,capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, enable_readcapsules, travoffsets); // partition // FIXME. include workerID in offset
					travoffsets = readcapsules0(1, kvsourcedram,  BIGcapsule10,  BIGcapsule11,  BIGcapsule12,  BIGcapsule13,  capsule10,capsule11,capsule12,capsule13, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, enable_readcapsules, travoffsets); // partition // FIXME. include workerID in offset
					travoffsets = readcapsules0(2, kvsourcedram,  BIGcapsule20,  BIGcapsule21,  BIGcapsule22,  BIGcapsule23,  capsule20,capsule21,capsule22,capsule23, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, enable_readcapsules, travoffsets); // partition // FIXME. include workerID in offset
					travoffsets = readcapsules0(3, kvsourcedram,  BIGcapsule30,  BIGcapsule31,  BIGcapsule32,  BIGcapsule33,  capsule30,capsule31,capsule32,capsule33, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, enable_readcapsules, travoffsets); // partition // FIXME. include workerID in offset
				
					THRONE_MAINLOOP1E: for(vector_type v=0; v<nfactor; v++){
						travstate.topi_kvs = i;
						travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
						
						// #ifdef MMM
	
						buffer_type chunk00_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk00_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk00_size, SRCBUFFER_SIZE, travstates[0], 0); }
						else {}
						buffer_type readoffset00_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset00_kvs = travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE); }
						else { readoffset00_kvs = (sourcestats[0].key / VECTOR_SIZE) + i; }
						buffer_type chunk01_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk01_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk01_size, SRCBUFFER_SIZE, travstates[1], 0); }
						else {}
						buffer_type readoffset01_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset01_kvs = travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE); }
						else { readoffset01_kvs = (sourcestats[1].key / VECTOR_SIZE) + i; }
						buffer_type chunk02_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk02_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk02_size, SRCBUFFER_SIZE, travstates[2], 0); }
						else {}
						buffer_type readoffset02_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset02_kvs = travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE); }
						else { readoffset02_kvs = (sourcestats[2].key / VECTOR_SIZE) + i; }
						buffer_type chunk03_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk03_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk03_size, SRCBUFFER_SIZE, travstates[3], 0); }
						else {}
						buffer_type readoffset03_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset03_kvs = travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE); }
						else { readoffset03_kvs = (sourcestats[3].key / VECTOR_SIZE) + i; }
	
						buffer_type chunk10_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk10_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk10_size, SRCBUFFER_SIZE, travstates[0], 0); }
						else {}
						buffer_type readoffset10_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset10_kvs = travstate.i_kvs + (1 * 0 * SRCBUFFER_SIZE); }
						else { readoffset10_kvs = (sourcestats[0].key / VECTOR_SIZE) + i; }
						buffer_type chunk11_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk11_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk11_size, SRCBUFFER_SIZE, travstates[1], 0); }
						else {}
						buffer_type readoffset11_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset11_kvs = travstate.i_kvs + (1 * 1 * SRCBUFFER_SIZE); }
						else { readoffset11_kvs = (sourcestats[1].key / VECTOR_SIZE) + i; }
						buffer_type chunk12_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk12_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk12_size, SRCBUFFER_SIZE, travstates[2], 0); }
						else {}
						buffer_type readoffset12_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset12_kvs = travstate.i_kvs + (1 * 2 * SRCBUFFER_SIZE); }
						else { readoffset12_kvs = (sourcestats[2].key / VECTOR_SIZE) + i; }
						buffer_type chunk13_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk13_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk13_size, SRCBUFFER_SIZE, travstates[3], 0); }
						else {}
						buffer_type readoffset13_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset13_kvs = travstate.i_kvs + (1 * 3 * SRCBUFFER_SIZE); }
						else { readoffset13_kvs = (sourcestats[3].key / VECTOR_SIZE) + i; }
	
						buffer_type chunk20_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk20_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk20_size, SRCBUFFER_SIZE, travstates[0], 0); }
						else {}
						buffer_type readoffset20_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset20_kvs = travstate.i_kvs + (2 * 0 * SRCBUFFER_SIZE); }
						else { readoffset20_kvs = (sourcestats[0].key / VECTOR_SIZE) + i; }
						buffer_type chunk21_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk21_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk21_size, SRCBUFFER_SIZE, travstates[1], 0); }
						else {}
						buffer_type readoffset21_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset21_kvs = travstate.i_kvs + (2 * 1 * SRCBUFFER_SIZE); }
						else { readoffset21_kvs = (sourcestats[1].key / VECTOR_SIZE) + i; }
						buffer_type chunk22_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk22_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk22_size, SRCBUFFER_SIZE, travstates[2], 0); }
						else {}
						buffer_type readoffset22_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset22_kvs = travstate.i_kvs + (2 * 2 * SRCBUFFER_SIZE); }
						else { readoffset22_kvs = (sourcestats[2].key / VECTOR_SIZE) + i; }
						buffer_type chunk23_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk23_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk23_size, SRCBUFFER_SIZE, travstates[3], 0); }
						else {}
						buffer_type readoffset23_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset23_kvs = travstate.i_kvs + (2 * 3 * SRCBUFFER_SIZE); }
						else { readoffset23_kvs = (sourcestats[3].key / VECTOR_SIZE) + i; }
	
						buffer_type chunk30_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk30_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk30_size, SRCBUFFER_SIZE, travstates[0], 0); }
						else {}
						buffer_type readoffset30_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset30_kvs = travstate.i_kvs + (3 * 0 * SRCBUFFER_SIZE); }
						else { readoffset30_kvs = (sourcestats[0].key / VECTOR_SIZE) + i; }
						buffer_type chunk31_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk31_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk31_size, SRCBUFFER_SIZE, travstates[1], 0); }
						else {}
						buffer_type readoffset31_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset31_kvs = travstate.i_kvs + (3 * 1 * SRCBUFFER_SIZE); }
						else { readoffset31_kvs = (sourcestats[1].key / VECTOR_SIZE) + i; }
						buffer_type chunk32_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk32_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk32_size, SRCBUFFER_SIZE, travstates[2], 0); }
						else {}
						buffer_type readoffset32_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset32_kvs = travstate.i_kvs + (3 * 2 * SRCBUFFER_SIZE); }
						else { readoffset32_kvs = (sourcestats[2].key / VECTOR_SIZE) + i; }
						buffer_type chunk33_size = SRCBUFFER_SIZE;
						if(operation.type == EXTRACTCAPSULES || operation.type == PARTITIONKEYVALUES){ getchunksize(&chunk33_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE))); }
						else if(operation.type == REDUCEKEYVALUES){ getchunksize(&chunk33_size, SRCBUFFER_SIZE, travstates[3], 0); }
						else {}
						buffer_type readoffset33_kvs;
						if((operation.type == EXTRACTCAPSULES) || (operation.type == PARTITIONKEYVALUES)){ readoffset33_kvs = travstate.i_kvs + (3 * 3 * SRCBUFFER_SIZE); }
						else { readoffset33_kvs = (sourcestats[3].key / VECTOR_SIZE) + i; }
						// #endif
						
	
						readkeyvalues0(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, readoffset00_kvs, chunk00_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, readoffset01_kvs, chunk01_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, readoffset02_kvs, chunk02_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, readoffset03_kvs, chunk03_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
	
						readkeyvalues0(kvsourcedram, sourcebuffer10, llopparams.baseaddr_worksourcekvs_kvs, readoffset10_kvs, chunk10_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer11, llopparams.baseaddr_worksourcekvs_kvs, readoffset11_kvs, chunk11_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer12, llopparams.baseaddr_worksourcekvs_kvs, readoffset12_kvs, chunk12_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer13, llopparams.baseaddr_worksourcekvs_kvs, readoffset13_kvs, chunk13_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
	
						readkeyvalues0(kvsourcedram, sourcebuffer20, llopparams.baseaddr_worksourcekvs_kvs, readoffset20_kvs, chunk20_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer21, llopparams.baseaddr_worksourcekvs_kvs, readoffset21_kvs, chunk21_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer22, llopparams.baseaddr_worksourcekvs_kvs, readoffset22_kvs, chunk22_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer23, llopparams.baseaddr_worksourcekvs_kvs, readoffset23_kvs, chunk23_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
	
						readkeyvalues0(kvsourcedram, sourcebuffer30, llopparams.baseaddr_worksourcekvs_kvs, readoffset30_kvs, chunk30_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer31, llopparams.baseaddr_worksourcekvs_kvs, readoffset31_kvs, chunk31_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer32, llopparams.baseaddr_worksourcekvs_kvs, readoffset32_kvs, chunk32_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						readkeyvalues0(kvsourcedram, sourcebuffer33, llopparams.baseaddr_worksourcekvs_kvs, readoffset33_kvs, chunk33_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES);
						
						King00(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, enable_extractandpartition, llopparams, travstate, globalparams, operation);
						King01(1  ,sourcebuffer10 ,sourcebuffer11 ,sourcebuffer12 ,sourcebuffer13  ,destbuffer10 ,destbuffer11 ,destbuffer12 ,destbuffer13  ,capsule10 ,capsule11 ,capsule12 ,capsule13, enable_extractandpartition, llopparams, travstate, globalparams, operation);
						King02(2  ,sourcebuffer20 ,sourcebuffer21 ,sourcebuffer22 ,sourcebuffer23  ,destbuffer20 ,destbuffer21 ,destbuffer22 ,destbuffer23  ,capsule20 ,capsule21 ,capsule22 ,capsule23, enable_extractandpartition, llopparams, travstate, globalparams, operation);
						King03(3  ,sourcebuffer30 ,sourcebuffer31 ,sourcebuffer32 ,sourcebuffer33  ,destbuffer30 ,destbuffer31 ,destbuffer32 ,destbuffer33  ,capsule30 ,capsule31 ,capsule32 ,capsule33, enable_extractandpartition, llopparams, travstate, globalparams, operation);
						
						#ifdef MMM
						
						Queen00(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03 ,enable_reduce, travstate, globalparams);
						Queen01(1  ,sourcebuffer10 ,sourcebuffer11 ,sourcebuffer12 ,sourcebuffer13  ,destbuffer10 ,destbuffer11 ,destbuffer12 ,destbuffer13 ,enable_reduce, travstate, globalparams);
						Queen02(2  ,sourcebuffer20 ,sourcebuffer21 ,sourcebuffer22 ,sourcebuffer23  ,destbuffer20 ,destbuffer21 ,destbuffer22 ,destbuffer23 ,enable_reduce, travstate, globalparams);
						Queen03(3  ,sourcebuffer30 ,sourcebuffer31 ,sourcebuffer32 ,sourcebuffer33  ,destbuffer30 ,destbuffer31 ,destbuffer32 ,destbuffer33 ,enable_reduce, travstate, globalparams);
						
						#endif 
					}
					
					#ifdef MMM
					travoffsets = savecapsules0(0, kvsourcedram , BIGcapsule00, BIGcapsule01, BIGcapsule02, BIGcapsule03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, enable_savecapsules, travstate, travoffsets); // extract capsules
					travoffsets = savecapsules0(1, kvsourcedram , BIGcapsule10, BIGcapsule11, BIGcapsule12, BIGcapsule13 , capsule10, capsule11, capsule12, capsule13, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, enable_savecapsules, travstate, travoffsets); // extract capsules
					travoffsets = savecapsules0(2, kvsourcedram , BIGcapsule20, BIGcapsule21, BIGcapsule22, BIGcapsule23 , capsule20, capsule21, capsule22, capsule23, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, enable_savecapsules, travstate, travoffsets); // extract capsules
					travoffsets = savecapsules0(3, kvsourcedram , BIGcapsule30, BIGcapsule31, BIGcapsule32, BIGcapsule33 , capsule30, capsule31, capsule32, capsule33, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, enable_savecapsules, travstate, travoffsets); // extract capsules
					savepartitions0(0, kvsourcedram , destbuffer00, destbuffer01, destbuffer02, destbuffer03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, enable_savepartitions); // partition 
					savepartitions0(1, kvsourcedram , destbuffer10, destbuffer11, destbuffer12, destbuffer13 , capsule10, capsule11, capsule12, capsule13, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, enable_savepartitions); // partition 
					savepartitions0(2, kvsourcedram , destbuffer20, destbuffer21, destbuffer22, destbuffer23 , capsule20, capsule21, capsule22, capsule23, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, enable_savepartitions); // partition 
					savepartitions0(3, kvsourcedram , destbuffer30, destbuffer31, destbuffer32, destbuffer33 , capsule30, capsule31, capsule32, capsule33, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, enable_savepartitions); // partition 
					resetcapsules0( capsule00, capsule01, capsule02, capsule03, 1);
					resetcapsules0( capsule10, capsule11, capsule12, capsule13, 1);
					resetcapsules0( capsule20, capsule21, capsule22, capsule23, 1);
					resetcapsules0( capsule30, capsule31, capsule32, capsule33, 1);
					#ifdef _DEBUGMODE_CHECKSM
					debugger_i += 1;
					#endif
					#endif
				}
				
				#ifdef MMM
				resetcapsules0( capsule00, capsule01, capsule02, capsule03, 0);
				resetcapsules0( capsule10, capsule11, capsule12, capsule13, 0);
				resetcapsules0( capsule20, capsule21, capsule22, capsule23, 0);
				resetcapsules0( capsule30, capsule31, capsule32, capsule33, 0);
				#endif 
 // reduce
				savekeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, enable_savekeyvalues, ENSAVEVERTICES_REDUCE);
 // reduce
				savekeyvalues0(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, enable_savekeyvalues, ENSAVEVERTICES_REDUCE);
 // reduce
				savekeyvalues0(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, enable_savekeyvalues, ENSAVEVERTICES_REDUCE);
 // reduce
				savekeyvalues0(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, enable_savekeyvalues, ENSAVEVERTICES_REDUCE);
				
				#ifdef MMM
				
				if(operation.type == EXTRACTCAPSULES){
				copy(kvdeststats_tmp, kvdeststats_tmp2, NUM_PARTITIONS); // should happen before calculateoffsets
				calculateoffsets(kvdeststats_tmp, sourcestat.key);
				calculateoffsets(kvdeststats_tmp2);
				resetvalues(kvdeststats_tmp2, NUM_PARTITIONS); }
				#ifdef _DEBUGMODE_CHECKSM
				debugger_i = 0;
				#endif
				
				#endif 
			}
			
			#ifdef _DEBUGMODE_CHECKSM
			printkeyvalues("TEST PRINTER", (keyvalue_t*)Debugger_Sizes1[0], NUM_PARTITIONS);
			printkeyvalues("TEST PRINTER", (keyvalue_t*)Debugger_Sizes2[0], NUM_PARTITIONS);
			printkeyvalues("TEST PRINTER", (keyvalue_t*)Debugger_Sizes1[1], NUM_PARTITIONS);
			printkeyvalues("TEST PRINTER", (keyvalue_t*)Debugger_Sizes2[1], NUM_PARTITIONS);
			exit(EXIT_SUCCESS);
			#endif 
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 0){ storedeststats(kvstats, kvdeststats_tmp, 0, deststatsmarker); deststatsmarker += NUM_PARTITIONS; }
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ sourcestatsmarker += NUMSUBWORKERS; }
			else { sourcestatsmarker += 1; }
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv * NUMSUBWORKERS; } 
			else { llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv; }
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		printglobalvars();
		clearglobalvars();
		#endif
	}
	return;
}
void enigma::Rule0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
	int analysis_maxtreedepthsz = 1;
	int analysis_avgnumsourcepartitions = 1;
	int analysis_partitionphase = (KVDRAMSZ_KVS / (NUMWORKERS * SRCBUFFER_SIZE)) / VECTOR_SIZE;
	int analysis_reducephase = 1;
	int analysis_chunksize = SRCBUFFER_SIZE;
	int analysis_iterationcount = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	
	uint512_dt sourcebuffer00[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer00 dim=1
	uint512_dt destbuffer00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule00[NUM_PARTITIONS];
	uint512_dt BIGcapsule00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer01[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer01 dim=1
	uint512_dt destbuffer01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule01[NUM_PARTITIONS];
	uint512_dt BIGcapsule01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer02[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer02 dim=1
	uint512_dt destbuffer02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule02[NUM_PARTITIONS];
	uint512_dt BIGcapsule02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer03[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer03 dim=1
	uint512_dt destbuffer03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule03[NUM_PARTITIONS];
	uint512_dt BIGcapsule03[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer10[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer10 dim=1
	uint512_dt destbuffer10[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule10[NUM_PARTITIONS];
	uint512_dt BIGcapsule10[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer11[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer11 dim=1
	uint512_dt destbuffer11[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule11[NUM_PARTITIONS];
	uint512_dt BIGcapsule11[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer12[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer12 dim=1
	uint512_dt destbuffer12[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule12[NUM_PARTITIONS];
	uint512_dt BIGcapsule12[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer13[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer13 dim=1
	uint512_dt destbuffer13[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule13[NUM_PARTITIONS];
	uint512_dt BIGcapsule13[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer20[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer20 dim=1
	uint512_dt destbuffer20[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule20[NUM_PARTITIONS];
	uint512_dt BIGcapsule20[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer21[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer21 dim=1
	uint512_dt destbuffer21[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule21[NUM_PARTITIONS];
	uint512_dt BIGcapsule21[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer22[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer22 dim=1
	uint512_dt destbuffer22[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule22[NUM_PARTITIONS];
	uint512_dt BIGcapsule22[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer23[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer23 dim=1
	uint512_dt destbuffer23[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule23[NUM_PARTITIONS];
	uint512_dt BIGcapsule23[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer30[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer30 dim=1
	uint512_dt destbuffer30[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule30[NUM_PARTITIONS];
	uint512_dt BIGcapsule30[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer31[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer31 dim=1
	uint512_dt destbuffer31[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule31[NUM_PARTITIONS];
	uint512_dt BIGcapsule31[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer32[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer32 dim=1
	uint512_dt destbuffer32[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule32[NUM_PARTITIONS];
	uint512_dt BIGcapsule32[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer33[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer33 dim=1
	uint512_dt destbuffer33[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule33[NUM_PARTITIONS];
	uint512_dt BIGcapsule33[CAPSULEBUFFER_SIZE];
	
	resetcapsules0(capsule00,capsule01,capsule02,capsule03, NAp);
	resetcapsules0(capsule10,capsule11,capsule12,capsule13, NAp);
	resetcapsules0(capsule20,capsule21,capsule22,capsule23, NAp);
	resetcapsules0(capsule30,capsule31,capsule32,capsule33, NAp);
	
	unsigned int numLOPs = 0;
	if(globalparams.runkernelcommand == ON){
		if(globalparams.reducecommand == ON){ numLOPs = globalparams.treedepth + 1 + 1; }
		else { numLOPs = globalparams.treedepth + 1; }
	} else { numLOPs = 1; }
	unsigned int loadkvdramsize_kvs = 0;
	if(globalparams.runkernelcommand == ON){ loadkvdramsize_kvs = 0; }
	else { loadkvdramsize_kvs = KVDATA_BATCHSIZE_KVS; }
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 1;
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadKvDRAM0 35", ((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS + KVDATA_BATCHSIZE_KVS, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparams.runkernelcommand == ON){ cout<<"...running Acts..."<<endl; } // printparameters();
	else { cout<<"...loading KvDRAM..."<<endl; }
	#endif
	int i;
	LOADKVDRAMPHASE_LOOP1: for (i = 0; i < loadkvdramsize_kvs; i += SRCBUFFER_SIZE){
	#pragma HLS LOOP_TRIPCOUNT min=analysis_iterationcount max=analysis_iterationcount
		//boundary checks
		int chunk_size = SRCBUFFER_SIZE;
		if ((i + SRCBUFFER_SIZE) > loadkvdramsize_kvs){ chunk_size = loadkvdramsize_kvs - i; }
		readkeyvalues0(kvsourcedram, sourcebuffer00[0], 0, 0 + i, chunk_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, 0);
		savekeyvalues0(kvsourcedram, sourcebuffer00[0], 0, (((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS) + i, chunk_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, 0);
	}

	// Partition Phase
	PARTITIONANDREDUCE_LOOP1: for(unsigned int currentLOP=1; currentLOP<numLOPs; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		keyvalue_t kvdeststats_tmp2[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		unsigned int num_source_partition_skipsize = 0;
		if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ num_source_partition_skipsize = NUMSUBWORKERS; }
		else { num_source_partition_skipsize = 1; }
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<endl;
		print4("PartitionPhase0:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		PARTITIONANDREDUCE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=num_source_partition_skipsize){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions
		
			partitionparams_t partitionparams;
			travstate_t travstate;
			travstate_t travstates[NUMSUBWORKERS];
			travoffsets_t travoffsets;
			#ifdef _DEBUGMODE_CHECKSM
			initializeDebuggers();
			#endif
			
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			keyvalue_t sourcestat = kvstats[getstatsAddr(sourcestatsmarker)];
			partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparams.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparams.begin_kvs = 0; partitionparams.size_kvs = 0; partitionparams.step_kvs = 0; partitionparams.end_kvs = 0; }
			travstate.begin_kvs = partitionparams.begin_kvs;
			travstate.end_kvs = partitionparams.end_kvs;
			travstate.skip_kvs = partitionparams.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * SRCBUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * SRCBUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			resetkeyandvalues(kvdeststats_tmp, NUM_PARTITIONS);
			resetkeyandvalues(kvdeststats_tmp2, NUM_PARTITIONS);
			
			// Collect capsules
			travoffsets.capsuleoffset_kvs = 0;
			COLLECTSTATS_MAINLOOP1: for(int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("collectstats_ddr0:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				COLLECTSTATS_MAINLOOP1B: for(vector_type v=0; v<NFACTOR / 2; v++){
					travstate.i_kvs = i + (v * 2 * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					
	
					buffer_type chunk00_size = SRCBUFFER_SIZE;
					buffer_type chunk01_size = SRCBUFFER_SIZE;
					buffer_type chunk02_size = SRCBUFFER_SIZE;
					buffer_type chunk03_size = SRCBUFFER_SIZE;
	
					buffer_type chunk10_size = SRCBUFFER_SIZE;
					buffer_type chunk11_size = SRCBUFFER_SIZE;
					buffer_type chunk12_size = SRCBUFFER_SIZE;
					buffer_type chunk13_size = SRCBUFFER_SIZE;
	
					buffer_type chunk20_size = SRCBUFFER_SIZE;
					buffer_type chunk21_size = SRCBUFFER_SIZE;
					buffer_type chunk22_size = SRCBUFFER_SIZE;
					buffer_type chunk23_size = SRCBUFFER_SIZE;
	
					buffer_type chunk30_size = SRCBUFFER_SIZE;
					buffer_type chunk31_size = SRCBUFFER_SIZE;
					buffer_type chunk32_size = SRCBUFFER_SIZE;
					buffer_type chunk33_size = SRCBUFFER_SIZE;
					
					readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[0]  ,sourcebuffer01[0]  ,sourcebuffer02[0]  ,sourcebuffer03[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs  ,chunk00_size  ,chunk01_size  ,chunk02_size  ,chunk03_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					readkeyvalues0(1, kvsourcedram  ,sourcebuffer10[0]  ,sourcebuffer11[0]  ,sourcebuffer12[0]  ,sourcebuffer13[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs  ,chunk10_size  ,chunk11_size  ,chunk12_size  ,chunk13_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					readkeyvalues0(2, kvsourcedram  ,sourcebuffer20[0]  ,sourcebuffer21[0]  ,sourcebuffer22[0]  ,sourcebuffer23[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs  ,chunk20_size  ,chunk21_size  ,chunk22_size  ,chunk23_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					readkeyvalues0(3, kvsourcedram  ,sourcebuffer30[0]  ,sourcebuffer31[0]  ,sourcebuffer32[0]  ,sourcebuffer33[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs  ,chunk30_size  ,chunk31_size  ,chunk32_size  ,chunk33_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					
					extractcapsules00(0  ,sourcebuffer00[1] ,sourcebuffer01[1] ,sourcebuffer02[1] ,sourcebuffer03[1]  ,capsule00 ,capsule01 ,capsule02 ,capsule03, llopparams, travstate);
					extractcapsules01(1  ,sourcebuffer10[1] ,sourcebuffer11[1] ,sourcebuffer12[1] ,sourcebuffer13[1]  ,capsule10 ,capsule11 ,capsule12 ,capsule13, llopparams, travstate);
					extractcapsules02(2  ,sourcebuffer20[1] ,sourcebuffer21[1] ,sourcebuffer22[1] ,sourcebuffer23[1]  ,capsule20 ,capsule21 ,capsule22 ,capsule23, llopparams, travstate);
					extractcapsules03(3  ,sourcebuffer30[1] ,sourcebuffer31[1] ,sourcebuffer32[1] ,sourcebuffer33[1]  ,capsule30 ,capsule31 ,capsule32 ,capsule33, llopparams, travstate);
					
					readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[1]  ,sourcebuffer01[1]  ,sourcebuffer02[1]  ,sourcebuffer03[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs + (NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE)  ,chunk00_size  ,chunk01_size  ,chunk02_size  ,chunk03_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					readkeyvalues0(1, kvsourcedram  ,sourcebuffer10[1]  ,sourcebuffer11[1]  ,sourcebuffer12[1]  ,sourcebuffer13[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs + (NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE)  ,chunk10_size  ,chunk11_size  ,chunk12_size  ,chunk13_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					readkeyvalues0(2, kvsourcedram  ,sourcebuffer20[1]  ,sourcebuffer21[1]  ,sourcebuffer22[1]  ,sourcebuffer23[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs + (NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE)  ,chunk20_size  ,chunk21_size  ,chunk22_size  ,chunk23_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					readkeyvalues0(3, kvsourcedram  ,sourcebuffer30[1]  ,sourcebuffer31[1]  ,sourcebuffer32[1]  ,sourcebuffer33[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs + (NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE)  ,chunk30_size  ,chunk31_size  ,chunk32_size  ,chunk33_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					
					extractcapsules00(0  ,sourcebuffer00[0] ,sourcebuffer01[0] ,sourcebuffer02[0] ,sourcebuffer03[0]  ,capsule00 ,capsule01 ,capsule02 ,capsule03, llopparams, travstate);
					extractcapsules01(1  ,sourcebuffer10[0] ,sourcebuffer11[0] ,sourcebuffer12[0] ,sourcebuffer13[0]  ,capsule10 ,capsule11 ,capsule12 ,capsule13, llopparams, travstate);
					extractcapsules02(2  ,sourcebuffer20[0] ,sourcebuffer21[0] ,sourcebuffer22[0] ,sourcebuffer23[0]  ,capsule20 ,capsule21 ,capsule22 ,capsule23, llopparams, travstate);
					extractcapsules03(3  ,sourcebuffer30[0] ,sourcebuffer31[0] ,sourcebuffer32[0] ,sourcebuffer33[0]  ,capsule30 ,capsule31 ,capsule32 ,capsule33, llopparams, travstate);
				}
				travoffsets = savecapsules0(0, kvsourcedram , BIGcapsule00, BIGcapsule01, BIGcapsule02, BIGcapsule03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travstate, travoffsets);
				travoffsets = savecapsules0(1, kvsourcedram , BIGcapsule10, BIGcapsule11, BIGcapsule12, BIGcapsule13 , capsule10, capsule11, capsule12, capsule13, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travstate, travoffsets);
				travoffsets = savecapsules0(2, kvsourcedram , BIGcapsule20, BIGcapsule21, BIGcapsule22, BIGcapsule23 , capsule20, capsule21, capsule22, capsule23, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travstate, travoffsets);
				travoffsets = savecapsules0(3, kvsourcedram , BIGcapsule30, BIGcapsule31, BIGcapsule32, BIGcapsule33 , capsule30, capsule31, capsule32, capsule33, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travstate, travoffsets);
				resetcapsules0( capsule00, capsule01, capsule02, capsule03, NAp);
				resetcapsules0( capsule10, capsule11, capsule12, capsule13, NAp);
				resetcapsules0( capsule20, capsule21, capsule22, capsule23, NAp);
				resetcapsules0( capsule30, capsule31, capsule32, capsule33, NAp);
				#ifdef _DEBUGMODE_CHECKSM
				debugger_i += 1;
				#endif
			}
			resetcapsules0( capsule00, capsule01, capsule02, capsule03, NAp);
			resetcapsules0( capsule10, capsule11, capsule12, capsule13, NAp);
			resetcapsules0( capsule20, capsule21, capsule22, capsule23, NAp);
			resetcapsules0( capsule30, capsule31, capsule32, capsule33, NAp);
			copy(kvdeststats_tmp, kvdeststats_tmp2, NUM_PARTITIONS); // should happen before calculateoffsets
			calculateoffsets(kvdeststats_tmp, sourcestat.key);
			calculateoffsets(kvdeststats_tmp2);
			resetvalues(kvdeststats_tmp2, NUM_PARTITIONS);
			#ifdef _DEBUGMODE_CHECKSM
			debugger_i = 0;
			#endif
			
			// Partition phase
			travoffsets.capsuleoffset_kvs = 0;
			PARTITIONPHASE_MAINLOOP1: for (int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("PartitionPhase0:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				travoffsets = readcapsules0(0, kvsourcedram,  BIGcapsule00,  BIGcapsule01,  BIGcapsule02,  BIGcapsule03,  capsule00,capsule01,capsule02,capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travoffsets); // FIXME. include workerID in offset
				travoffsets = readcapsules0(1, kvsourcedram,  BIGcapsule10,  BIGcapsule11,  BIGcapsule12,  BIGcapsule13,  capsule10,capsule11,capsule12,capsule13, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travoffsets); // FIXME. include workerID in offset
				travoffsets = readcapsules0(2, kvsourcedram,  BIGcapsule20,  BIGcapsule21,  BIGcapsule22,  BIGcapsule23,  capsule20,capsule21,capsule22,capsule23, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travoffsets); // FIXME. include workerID in offset
				travoffsets = readcapsules0(3, kvsourcedram,  BIGcapsule30,  BIGcapsule31,  BIGcapsule32,  BIGcapsule33,  capsule30,capsule31,capsule32,capsule33, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travoffsets); // FIXME. include workerID in offset
				PARTITIONPHASE_MAINLOOP1B: for (int v = 0; v < NFACTOR / 2; v += 1){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					
	
					buffer_type chunk00_size = SRCBUFFER_SIZE;
					buffer_type chunk01_size = SRCBUFFER_SIZE;
					buffer_type chunk02_size = SRCBUFFER_SIZE;
					buffer_type chunk03_size = SRCBUFFER_SIZE;
	
					buffer_type chunk10_size = SRCBUFFER_SIZE;
					buffer_type chunk11_size = SRCBUFFER_SIZE;
					buffer_type chunk12_size = SRCBUFFER_SIZE;
					buffer_type chunk13_size = SRCBUFFER_SIZE;
	
					buffer_type chunk20_size = SRCBUFFER_SIZE;
					buffer_type chunk21_size = SRCBUFFER_SIZE;
					buffer_type chunk22_size = SRCBUFFER_SIZE;
					buffer_type chunk23_size = SRCBUFFER_SIZE;
	
					buffer_type chunk30_size = SRCBUFFER_SIZE;
					buffer_type chunk31_size = SRCBUFFER_SIZE;
					buffer_type chunk32_size = SRCBUFFER_SIZE;
					buffer_type chunk33_size = SRCBUFFER_SIZE;
					
					readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[0]  ,sourcebuffer01[0]  ,sourcebuffer02[0]  ,sourcebuffer03[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs  ,chunk00_size  ,chunk01_size  ,chunk02_size  ,chunk03_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					readkeyvalues0(1, kvsourcedram  ,sourcebuffer10[0]  ,sourcebuffer11[0]  ,sourcebuffer12[0]  ,sourcebuffer13[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs  ,chunk10_size  ,chunk11_size  ,chunk12_size  ,chunk13_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					readkeyvalues0(2, kvsourcedram  ,sourcebuffer20[0]  ,sourcebuffer21[0]  ,sourcebuffer22[0]  ,sourcebuffer23[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs  ,chunk20_size  ,chunk21_size  ,chunk22_size  ,chunk23_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					readkeyvalues0(3, kvsourcedram  ,sourcebuffer30[0]  ,sourcebuffer31[0]  ,sourcebuffer32[0]  ,sourcebuffer33[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs  ,chunk30_size  ,chunk31_size  ,chunk32_size  ,chunk33_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					
					partitionkeyvalues00(0  ,sourcebuffer00[1]  ,sourcebuffer01[1]  ,sourcebuffer02[1]  ,sourcebuffer03[1]   ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, travstate, llopparams);
					partitionkeyvalues01(1  ,sourcebuffer10[1]  ,sourcebuffer11[1]  ,sourcebuffer12[1]  ,sourcebuffer13[1]   ,destbuffer10 ,destbuffer11 ,destbuffer12 ,destbuffer13  ,capsule10 ,capsule11 ,capsule12 ,capsule13, travstate, llopparams);
					partitionkeyvalues02(2  ,sourcebuffer20[1]  ,sourcebuffer21[1]  ,sourcebuffer22[1]  ,sourcebuffer23[1]   ,destbuffer20 ,destbuffer21 ,destbuffer22 ,destbuffer23  ,capsule20 ,capsule21 ,capsule22 ,capsule23, travstate, llopparams);
					partitionkeyvalues03(3  ,sourcebuffer30[1]  ,sourcebuffer31[1]  ,sourcebuffer32[1]  ,sourcebuffer33[1]   ,destbuffer30 ,destbuffer31 ,destbuffer32 ,destbuffer33  ,capsule30 ,capsule31 ,capsule32 ,capsule33, travstate, llopparams);
					
					readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[1]  ,sourcebuffer01[1]  ,sourcebuffer02[1]  ,sourcebuffer03[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs + (NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE)  ,chunk00_size  ,chunk01_size  ,chunk02_size  ,chunk03_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					readkeyvalues0(1, kvsourcedram  ,sourcebuffer10[1]  ,sourcebuffer11[1]  ,sourcebuffer12[1]  ,sourcebuffer13[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs + (NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE)  ,chunk10_size  ,chunk11_size  ,chunk12_size  ,chunk13_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					readkeyvalues0(2, kvsourcedram  ,sourcebuffer20[1]  ,sourcebuffer21[1]  ,sourcebuffer22[1]  ,sourcebuffer23[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs + (NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE)  ,chunk20_size  ,chunk21_size  ,chunk22_size  ,chunk23_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					readkeyvalues0(3, kvsourcedram  ,sourcebuffer30[1]  ,sourcebuffer31[1]  ,sourcebuffer32[1]  ,sourcebuffer33[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs + (NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE)  ,chunk30_size  ,chunk31_size  ,chunk32_size  ,chunk33_size  ,PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					
					partitionkeyvalues00(0  ,sourcebuffer00[0]  ,sourcebuffer01[0]  ,sourcebuffer02[0]  ,sourcebuffer03[0]   ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, travstate, llopparams);
					partitionkeyvalues01(1  ,sourcebuffer10[0]  ,sourcebuffer11[0]  ,sourcebuffer12[0]  ,sourcebuffer13[0]   ,destbuffer10 ,destbuffer11 ,destbuffer12 ,destbuffer13  ,capsule10 ,capsule11 ,capsule12 ,capsule13, travstate, llopparams);
					partitionkeyvalues02(2  ,sourcebuffer20[0]  ,sourcebuffer21[0]  ,sourcebuffer22[0]  ,sourcebuffer23[0]   ,destbuffer20 ,destbuffer21 ,destbuffer22 ,destbuffer23  ,capsule20 ,capsule21 ,capsule22 ,capsule23, travstate, llopparams);
					partitionkeyvalues03(3  ,sourcebuffer30[0]  ,sourcebuffer31[0]  ,sourcebuffer32[0]  ,sourcebuffer33[0]   ,destbuffer30 ,destbuffer31 ,destbuffer32 ,destbuffer33  ,capsule30 ,capsule31 ,capsule32 ,capsule33, travstate, llopparams);
				}
				savepartitions0(0, kvsourcedram , destbuffer00, destbuffer01, destbuffer02, destbuffer03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, 1);
				savepartitions0(1, kvsourcedram , destbuffer10, destbuffer11, destbuffer12, destbuffer13 , capsule10, capsule11, capsule12, capsule13, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, 1);
				savepartitions0(2, kvsourcedram , destbuffer20, destbuffer21, destbuffer22, destbuffer23 , capsule20, capsule21, capsule22, capsule23, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, 1);
				savepartitions0(3, kvsourcedram , destbuffer30, destbuffer31, destbuffer32, destbuffer33 , capsule30, capsule31, capsule32, capsule33, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, 1);
				resetcapsules0(capsule00,capsule01,capsule02,capsule03, NAp);
				resetcapsules0(capsule10,capsule11,capsule12,capsule13, NAp);
				resetcapsules0(capsule20,capsule21,capsule22,capsule23, NAp);
				resetcapsules0(capsule30,capsule31,capsule32,capsule33, NAp);
				#ifdef _DEBUGMODE_CHECKSM
				debugger_i += 1;
				#endif
			}
			#ifdef _DEBUGMODE_CHECKSM
			IsEqual(Debugger_Sizes1, Debugger_Sizes2, debugger_i, NUM_PARTITIONS);
			debugger_i = 0;
			#endif
			
			// Reduce phase
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
			keyvalue_t sourcestats[NUMSUBWORKERS];
			unsigned int maxsz = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats[i] = kvstats[getstatsAddr(sourcestatsmarker + i)]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxsz = max(sourcestats[sw].value, maxsz); }
			unsigned int maxsz_kvs = (maxsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
			travstates[sw].begin_kvs = 0;
			travstates[sw].end_kvs = (sourcestats[sw].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			travstates[sw].skip_kvs = SRCBUFFER_SIZE; }
			
			readkeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENREADVPROPS_REDUCE);
			readkeyvalues0(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENREADVPROPS_REDUCE);
			readkeyvalues0(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENREADVPROPS_REDUCE);
			readkeyvalues0(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENREADVPROPS_REDUCE);
			REDUCEPHASE_MAINLOOP: for (int i = 0; i < maxsz_kvs; i += SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reducephase avg=analysis_reducephase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase0:: i", "i_end", "NAp", "NAp", "skip size", i, maxsz_kvs, NAp, NAp, SRCBUFFER_SIZE);
				#endif
				for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ travstates[sw].i_kvs = i; }
				
				buffer_type chunk0_size = SRCBUFFER_SIZE;
				getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstates[0], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer00[0], llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[0].key / VECTOR_SIZE) + i, chunk0_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_REDUCE);
				buffer_type chunk1_size = SRCBUFFER_SIZE;
				getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstates[1], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer01[0], llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[1].key / VECTOR_SIZE) + i, chunk1_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_REDUCE);
				buffer_type chunk2_size = SRCBUFFER_SIZE;
				getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstates[2], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer02[0], llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[2].key / VECTOR_SIZE) + i, chunk2_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_REDUCE);
				buffer_type chunk3_size = SRCBUFFER_SIZE;
				getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstates[3], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer03[0], llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[3].key / VECTOR_SIZE) + i, chunk3_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_REDUCE);
				
				reducepartitions0(0,  sourcebuffer00[0],  sourcebuffer01[0],  sourcebuffer02[0],  sourcebuffer03[0],   destbuffer00,  destbuffer01,  destbuffer02,  destbuffer03,  travstate, globalparams);
			}
			savekeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENSAVEVERTICES_REDUCE);
			savekeyvalues0(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENSAVEVERTICES_REDUCE);
			savekeyvalues0(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENSAVEVERTICES_REDUCE);
			savekeyvalues0(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENSAVEVERTICES_REDUCE);
			}
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 0){ storedeststats(kvstats, kvdeststats_tmp, 0, deststatsmarker); deststatsmarker += NUM_PARTITIONS; }
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ sourcestatsmarker += NUMSUBWORKERS; }
			else { sourcestatsmarker += 1; }
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv * NUMSUBWORKERS; } 
			else { llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv; }
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		printglobalvars();
		clearglobalvars();
		#endif
	}
	return;
}
void enigma::partitionandreduce0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
	int analysis_maxtreedepthsz = 1;
	int analysis_avgnumsourcepartitions = 1;
	int analysis_partitionphase = (KVDRAMSZ_KVS / (NUMWORKERS * SRCBUFFER_SIZE)) / VECTOR_SIZE;
	int analysis_reducephase = 1;
	int analysis_chunksize = SRCBUFFER_SIZE;
	int analysis_iterationcount = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	
	uint512_dt sourcebuffer00[SRCBUFFER_SIZE];
	uint512_dt destbuffer00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule00[NUM_PARTITIONS];
	uint512_dt BIGcapsule00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer01[SRCBUFFER_SIZE];
	uint512_dt destbuffer01[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule01[NUM_PARTITIONS];
	uint512_dt BIGcapsule01[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer02[SRCBUFFER_SIZE];
	uint512_dt destbuffer02[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule02[NUM_PARTITIONS];
	uint512_dt BIGcapsule02[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer03[SRCBUFFER_SIZE];
	uint512_dt destbuffer03[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule03[NUM_PARTITIONS];
	uint512_dt BIGcapsule03[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer10[SRCBUFFER_SIZE];
	uint512_dt destbuffer10[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule10[NUM_PARTITIONS];
	uint512_dt BIGcapsule10[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer11[SRCBUFFER_SIZE];
	uint512_dt destbuffer11[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule11[NUM_PARTITIONS];
	uint512_dt BIGcapsule11[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer12[SRCBUFFER_SIZE];
	uint512_dt destbuffer12[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule12[NUM_PARTITIONS];
	uint512_dt BIGcapsule12[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer13[SRCBUFFER_SIZE];
	uint512_dt destbuffer13[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule13[NUM_PARTITIONS];
	uint512_dt BIGcapsule13[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer20[SRCBUFFER_SIZE];
	uint512_dt destbuffer20[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule20[NUM_PARTITIONS];
	uint512_dt BIGcapsule20[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer21[SRCBUFFER_SIZE];
	uint512_dt destbuffer21[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule21[NUM_PARTITIONS];
	uint512_dt BIGcapsule21[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer22[SRCBUFFER_SIZE];
	uint512_dt destbuffer22[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule22[NUM_PARTITIONS];
	uint512_dt BIGcapsule22[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer23[SRCBUFFER_SIZE];
	uint512_dt destbuffer23[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule23[NUM_PARTITIONS];
	uint512_dt BIGcapsule23[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer30[SRCBUFFER_SIZE];
	uint512_dt destbuffer30[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule30[NUM_PARTITIONS];
	uint512_dt BIGcapsule30[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer31[SRCBUFFER_SIZE];
	uint512_dt destbuffer31[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule31[NUM_PARTITIONS];
	uint512_dt BIGcapsule31[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer32[SRCBUFFER_SIZE];
	uint512_dt destbuffer32[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule32[NUM_PARTITIONS];
	uint512_dt BIGcapsule32[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer33[SRCBUFFER_SIZE];
	uint512_dt destbuffer33[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule33[NUM_PARTITIONS];
	uint512_dt BIGcapsule33[CAPSULEBUFFER_SIZE];
	
	resetcapsules0(capsule00,capsule01,capsule02,capsule03, NAp);
	resetcapsules0(capsule10,capsule11,capsule12,capsule13, NAp);
	resetcapsules0(capsule20,capsule21,capsule22,capsule23, NAp);
	resetcapsules0(capsule30,capsule31,capsule32,capsule33, NAp);
	
	unsigned int numLOPs = 0;
	if(globalparams.runkernelcommand == ON){
		if(globalparams.reducecommand == ON){ numLOPs = globalparams.treedepth + 1 + 1; }
		else { numLOPs = globalparams.treedepth + 1; }
	} else { numLOPs = 1; }
	unsigned int loadkvdramsize_kvs = 0;
	if(globalparams.runkernelcommand == ON){ loadkvdramsize_kvs = 0; }
	else { loadkvdramsize_kvs = KVDATA_BATCHSIZE_KVS; }
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 1;
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadKvDRAM0 35", ((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS + KVDATA_BATCHSIZE_KVS, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparams.runkernelcommand == ON){ cout<<"...running Acts... size: "<<kvstats[BASEOFFSET_STATSDRAM + 0].value<<endl; } // printparameters();
	else { cout<<"...loading KvDRAM..."<<endl; }
	
	#endif
	int i;
	LOADKVDRAMPHASE_LOOP1: for (i = 0; i < loadkvdramsize_kvs; i += SRCBUFFER_SIZE){
	#pragma HLS LOOP_TRIPCOUNT min=analysis_iterationcount max=analysis_iterationcount
		//boundary checks
		int chunk_size = SRCBUFFER_SIZE;
		if ((i + SRCBUFFER_SIZE) > loadkvdramsize_kvs){ chunk_size = loadkvdramsize_kvs - i; }
		readkeyvalues0(kvsourcedram, sourcebuffer00, 0, 0 + i, chunk_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, 0);
		savekeyvalues0(kvsourcedram, sourcebuffer00, 0, (((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS) + i, chunk_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, 0);
	}

	// Partition Phase
	PARTITIONANDREDUCE_LOOP1: for(unsigned int currentLOP=1; currentLOP<numLOPs; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		keyvalue_t kvdeststats_tmp2[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		unsigned int num_source_partition_skipsize = 0;
		if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ num_source_partition_skipsize = NUMSUBWORKERS; }
		else { num_source_partition_skipsize = 1; }
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<endl;
		print4("PartitionPhase0:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		PARTITIONANDREDUCE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=num_source_partition_skipsize){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions
		
			partitionparams_t partitionparams;
			travstate_t travstate;
			travstate_t travstates[NUMSUBWORKERS];
			travoffsets_t travoffsets;
			#ifdef _DEBUGMODE_CHECKSM
			initializeDebuggers();
			#endif
			
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::partitionandreduce0 22", getstatsAddr(sourcestatsmarker), KVSTATSDRAMSZ, NAp, NAp, NAp);
			#endif
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			keyvalue_t sourcestat = kvstats[getstatsAddr(sourcestatsmarker)];
			partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparams.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparams.begin_kvs = 0; partitionparams.size_kvs = 0; partitionparams.step_kvs = 0; partitionparams.end_kvs = 0; }
			travstate.begin_kvs = partitionparams.begin_kvs;
			travstate.end_kvs = partitionparams.end_kvs;
			travstate.skip_kvs = partitionparams.step_kvs;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * SRCBUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { print6("PartitionPhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * SRCBUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			resetkeyandvalues(kvdeststats_tmp, NUM_PARTITIONS);
			resetkeyandvalues(kvdeststats_tmp2, NUM_PARTITIONS);
			
			// Collect capsules
			travoffsets.capsuleoffset_kvs = 0;
			COLLECTSTATS_MAINLOOP1: for(int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("collectstats_ddr0:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				COLLECTSTATS_MAINLOOP1B: for(vector_type v=0; v<NFACTOR; v++){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					
	
					buffer_type chunk00_size = SRCBUFFER_SIZE;
					getchunksize(&chunk00_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE), chunk00_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk01_size = SRCBUFFER_SIZE;
					getchunksize(&chunk01_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE), chunk01_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk02_size = SRCBUFFER_SIZE;
					getchunksize(&chunk02_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE), chunk02_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk03_size = SRCBUFFER_SIZE;
					getchunksize(&chunk03_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE), chunk03_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
	
					buffer_type chunk10_size = SRCBUFFER_SIZE;
					getchunksize(&chunk10_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer10, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (1 * 0 * SRCBUFFER_SIZE), chunk10_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk11_size = SRCBUFFER_SIZE;
					getchunksize(&chunk11_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer11, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (1 * 1 * SRCBUFFER_SIZE), chunk11_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk12_size = SRCBUFFER_SIZE;
					getchunksize(&chunk12_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer12, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (1 * 2 * SRCBUFFER_SIZE), chunk12_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk13_size = SRCBUFFER_SIZE;
					getchunksize(&chunk13_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer13, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (1 * 3 * SRCBUFFER_SIZE), chunk13_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
	
					buffer_type chunk20_size = SRCBUFFER_SIZE;
					getchunksize(&chunk20_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer20, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (2 * 0 * SRCBUFFER_SIZE), chunk20_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk21_size = SRCBUFFER_SIZE;
					getchunksize(&chunk21_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer21, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (2 * 1 * SRCBUFFER_SIZE), chunk21_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk22_size = SRCBUFFER_SIZE;
					getchunksize(&chunk22_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer22, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (2 * 2 * SRCBUFFER_SIZE), chunk22_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk23_size = SRCBUFFER_SIZE;
					getchunksize(&chunk23_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer23, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (2 * 3 * SRCBUFFER_SIZE), chunk23_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
	
					buffer_type chunk30_size = SRCBUFFER_SIZE;
					getchunksize(&chunk30_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer30, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (3 * 0 * SRCBUFFER_SIZE), chunk30_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk31_size = SRCBUFFER_SIZE;
					getchunksize(&chunk31_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer31, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (3 * 1 * SRCBUFFER_SIZE), chunk31_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk32_size = SRCBUFFER_SIZE;
					getchunksize(&chunk32_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer32, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (3 * 2 * SRCBUFFER_SIZE), chunk32_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					buffer_type chunk33_size = SRCBUFFER_SIZE;
					getchunksize(&chunk33_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer33, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (3 * 3 * SRCBUFFER_SIZE), chunk33_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKVS_COLLECTSTATS);
					
					extractcapsules00(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, llopparams, travstate);
					extractcapsules01(1  ,sourcebuffer10 ,sourcebuffer11 ,sourcebuffer12 ,sourcebuffer13  ,capsule10 ,capsule11 ,capsule12 ,capsule13, llopparams, travstate);
					extractcapsules02(2  ,sourcebuffer20 ,sourcebuffer21 ,sourcebuffer22 ,sourcebuffer23  ,capsule20 ,capsule21 ,capsule22 ,capsule23, llopparams, travstate);
					extractcapsules03(3  ,sourcebuffer30 ,sourcebuffer31 ,sourcebuffer32 ,sourcebuffer33  ,capsule30 ,capsule31 ,capsule32 ,capsule33, llopparams, travstate);
				}
				travoffsets = savecapsules0(0, kvsourcedram , BIGcapsule00, BIGcapsule01, BIGcapsule02, BIGcapsule03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travstate, travoffsets);
				travoffsets = savecapsules0(1, kvsourcedram , BIGcapsule10, BIGcapsule11, BIGcapsule12, BIGcapsule13 , capsule10, capsule11, capsule12, capsule13, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travstate, travoffsets);
				travoffsets = savecapsules0(2, kvsourcedram , BIGcapsule20, BIGcapsule21, BIGcapsule22, BIGcapsule23 , capsule20, capsule21, capsule22, capsule23, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travstate, travoffsets);
				travoffsets = savecapsules0(3, kvsourcedram , BIGcapsule30, BIGcapsule31, BIGcapsule32, BIGcapsule33 , capsule30, capsule31, capsule32, capsule33, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travstate, travoffsets);
				resetcapsules0( capsule00, capsule01, capsule02, capsule03, NAp);
				resetcapsules0( capsule10, capsule11, capsule12, capsule13, NAp);
				resetcapsules0( capsule20, capsule21, capsule22, capsule23, NAp);
				resetcapsules0( capsule30, capsule31, capsule32, capsule33, NAp);
				#ifdef _DEBUGMODE_CHECKSM
				debugger_i += 1;
				#endif
			}
			resetcapsules0( capsule00, capsule01, capsule02, capsule03, NAp);
			resetcapsules0( capsule10, capsule11, capsule12, capsule13, NAp);
			resetcapsules0( capsule20, capsule21, capsule22, capsule23, NAp);
			resetcapsules0( capsule30, capsule31, capsule32, capsule33, NAp);
			copy(kvdeststats_tmp, kvdeststats_tmp2, NUM_PARTITIONS); // should happen before calculateoffsets
			calculateoffsets(kvdeststats_tmp, sourcestat.key);
			calculateoffsets(kvdeststats_tmp2);
			resetvalues(kvdeststats_tmp2, NUM_PARTITIONS);
			#ifdef _DEBUGMODE_CHECKSM
			debugger_i = 0;
			#endif
			
			// Partition phase
			travoffsets.capsuleoffset_kvs = 0;
			PARTITIONPHASE_MAINLOOP1: for (int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += partitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print4("PartitionPhase0:: i", "i_end", "step size", "NAp", i, partitionparams.end_kvs, partitionparams.step_kvs, NAp);
				#endif
				
				batch_type capsuleoffset_kvs = ((i - partitionparams.begin_kvs) / partitionparams.step_kvs) * NUM_PARTITIONS;
				travoffsets = readcapsules0(0, kvsourcedram,  BIGcapsule00,  BIGcapsule01,  BIGcapsule02,  BIGcapsule03,  capsule00,capsule01,capsule02,capsule03, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travoffsets); // FIXME. include workerID in offset
				travoffsets = readcapsules0(1, kvsourcedram,  BIGcapsule10,  BIGcapsule11,  BIGcapsule12,  BIGcapsule13,  capsule10,capsule11,capsule12,capsule13, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travoffsets); // FIXME. include workerID in offset
				travoffsets = readcapsules0(2, kvsourcedram,  BIGcapsule20,  BIGcapsule21,  BIGcapsule22,  BIGcapsule23,  capsule20,capsule21,capsule22,capsule23, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travoffsets); // FIXME. include workerID in offset
				travoffsets = readcapsules0(3, kvsourcedram,  BIGcapsule30,  BIGcapsule31,  BIGcapsule32,  BIGcapsule33,  capsule30,capsule31,capsule32,capsule33, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, 1, travoffsets); // FIXME. include workerID in offset
				PARTITIONPHASE_MAINLOOP1B: for (int v = 0; v < NFACTOR; v += 1){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					
					buffer_type chunk00_size = SRCBUFFER_SIZE;
					getchunksize(&chunk00_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 0 * SRCBUFFER_SIZE), chunk00_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk01_size = SRCBUFFER_SIZE;
					getchunksize(&chunk01_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 1 * SRCBUFFER_SIZE), chunk01_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk02_size = SRCBUFFER_SIZE;
					getchunksize(&chunk02_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 2 * SRCBUFFER_SIZE), chunk02_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk03_size = SRCBUFFER_SIZE;
					getchunksize(&chunk03_size, SRCBUFFER_SIZE, travstate, ((0 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (0 * 3 * SRCBUFFER_SIZE), chunk03_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk10_size = SRCBUFFER_SIZE;
					getchunksize(&chunk10_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer10, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (1 * 0 * SRCBUFFER_SIZE), chunk10_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk11_size = SRCBUFFER_SIZE;
					getchunksize(&chunk11_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer11, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (1 * 1 * SRCBUFFER_SIZE), chunk11_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk12_size = SRCBUFFER_SIZE;
					getchunksize(&chunk12_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer12, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (1 * 2 * SRCBUFFER_SIZE), chunk12_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk13_size = SRCBUFFER_SIZE;
					getchunksize(&chunk13_size, SRCBUFFER_SIZE, travstate, ((1 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer13, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (1 * 3 * SRCBUFFER_SIZE), chunk13_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk20_size = SRCBUFFER_SIZE;
					getchunksize(&chunk20_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer20, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (2 * 0 * SRCBUFFER_SIZE), chunk20_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk21_size = SRCBUFFER_SIZE;
					getchunksize(&chunk21_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer21, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (2 * 1 * SRCBUFFER_SIZE), chunk21_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk22_size = SRCBUFFER_SIZE;
					getchunksize(&chunk22_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer22, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (2 * 2 * SRCBUFFER_SIZE), chunk22_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk23_size = SRCBUFFER_SIZE;
					getchunksize(&chunk23_size, SRCBUFFER_SIZE, travstate, ((2 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer23, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (2 * 3 * SRCBUFFER_SIZE), chunk23_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk30_size = SRCBUFFER_SIZE;
					getchunksize(&chunk30_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer30, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (3 * 0 * SRCBUFFER_SIZE), chunk30_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk31_size = SRCBUFFER_SIZE;
					getchunksize(&chunk31_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer31, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (3 * 1 * SRCBUFFER_SIZE), chunk31_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk32_size = SRCBUFFER_SIZE;
					getchunksize(&chunk32_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer32, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (3 * 2 * SRCBUFFER_SIZE), chunk32_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					buffer_type chunk33_size = SRCBUFFER_SIZE;
					getchunksize(&chunk33_size, SRCBUFFER_SIZE, travstate, ((3 * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
					readkeyvalues0(kvsourcedram, sourcebuffer33, llopparams.baseaddr_worksourcekvs_kvs, travstate.i_kvs + (3 * 3 * SRCBUFFER_SIZE), chunk33_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_PARTITION);
					
					partitionkeyvalues00(0  ,sourcebuffer00 ,sourcebuffer01 ,sourcebuffer02 ,sourcebuffer03  ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, travstate, llopparams);
					partitionkeyvalues01(1  ,sourcebuffer10 ,sourcebuffer11 ,sourcebuffer12 ,sourcebuffer13  ,destbuffer10 ,destbuffer11 ,destbuffer12 ,destbuffer13  ,capsule10 ,capsule11 ,capsule12 ,capsule13, travstate, llopparams);
					partitionkeyvalues02(2  ,sourcebuffer20 ,sourcebuffer21 ,sourcebuffer22 ,sourcebuffer23  ,destbuffer20 ,destbuffer21 ,destbuffer22 ,destbuffer23  ,capsule20 ,capsule21 ,capsule22 ,capsule23, travstate, llopparams);
					partitionkeyvalues03(3  ,sourcebuffer30 ,sourcebuffer31 ,sourcebuffer32 ,sourcebuffer33  ,destbuffer30 ,destbuffer31 ,destbuffer32 ,destbuffer33  ,capsule30 ,capsule31 ,capsule32 ,capsule33, travstate, llopparams);
				}
				savepartitions0(0, kvsourcedram , destbuffer00, destbuffer01, destbuffer02, destbuffer03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, 1);
				savepartitions0(1, kvsourcedram , destbuffer10, destbuffer11, destbuffer12, destbuffer13 , capsule10, capsule11, capsule12, capsule13, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, 1);
				savepartitions0(2, kvsourcedram , destbuffer20, destbuffer21, destbuffer22, destbuffer23 , capsule20, capsule21, capsule22, capsule23, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, 1);
				savepartitions0(3, kvsourcedram , destbuffer30, destbuffer31, destbuffer32, destbuffer33 , capsule30, capsule31, capsule32, capsule33, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, 1);
				resetcapsules0(capsule00,capsule01,capsule02,capsule03, NAp);
				resetcapsules0(capsule10,capsule11,capsule12,capsule13, NAp);
				resetcapsules0(capsule20,capsule21,capsule22,capsule23, NAp);
				resetcapsules0(capsule30,capsule31,capsule32,capsule33, NAp);
				#ifdef _DEBUGMODE_CHECKSM
				debugger_i += 1;
				#endif
			}
			#ifdef _DEBUGMODE_CHECKSM
			IsEqual(Debugger_Sizes1, Debugger_Sizes2, debugger_i, NUM_PARTITIONS);
			debugger_i = 0;
			#endif
			
			// Reduce phase
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("enigma::partitionandreduce0 23", getstatsAddr(sourcestatsmarker + NUMSUBWORKERS), KVSTATSDRAMSZ, NAp, NAp, NAp);
			#endif
			keyvalue_t sourcestats[NUMSUBWORKERS];
			unsigned int maxsz = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats[i] = kvstats[getstatsAddr(sourcestatsmarker + i)]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxsz = max(sourcestats[sw].value, maxsz); }
			unsigned int maxsz_kvs = (maxsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){
			travstates[sw].begin_kvs = 0;
			travstates[sw].end_kvs = (sourcestats[sw].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			travstates[sw].skip_kvs = SRCBUFFER_SIZE; }
			
			readkeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENREADVPROPS_REDUCE);
			readkeyvalues0(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENREADVPROPS_REDUCE);
			readkeyvalues0(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENREADVPROPS_REDUCE);
			readkeyvalues0(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENREADVPROPS_REDUCE);
			REDUCEPHASE_MAINLOOP: for (int i = 0; i < maxsz_kvs; i += SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reducephase avg=analysis_reducephase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase0:: i", "i_end", "NAp", "NAp", "skip size", i, maxsz_kvs, NAp, NAp, SRCBUFFER_SIZE);
				#endif
				for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ travstates[sw].i_kvs = i; }
				
				buffer_type chunk0_size = SRCBUFFER_SIZE;
				getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstates[0], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer00, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[0].key / VECTOR_SIZE) + i, chunk0_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_REDUCE);
				buffer_type chunk1_size = SRCBUFFER_SIZE;
				getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstates[1], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer01, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[1].key / VECTOR_SIZE) + i, chunk1_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_REDUCE);
				buffer_type chunk2_size = SRCBUFFER_SIZE;
				getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstates[2], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer02, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[2].key / VECTOR_SIZE) + i, chunk2_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_REDUCE);
				buffer_type chunk3_size = SRCBUFFER_SIZE;
				getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstates[3], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer03, llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[3].key / VECTOR_SIZE) + i, chunk3_size, PADDEDKVSOURCEDRAMSZ_KVS, 1, ENREADKEYVALUES_REDUCE);
				
				reducepartitions0(0,  sourcebuffer00,  sourcebuffer01,  sourcebuffer02,  sourcebuffer03,   destbuffer00,  destbuffer01,  destbuffer02,  destbuffer03,  travstate, globalparams);
			}
			savekeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENSAVEVERTICES_REDUCE);
			savekeyvalues0(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENSAVEVERTICES_REDUCE);
			savekeyvalues0(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENSAVEVERTICES_REDUCE);
			savekeyvalues0(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION, 1, ENSAVEVERTICES_REDUCE);
			}
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 0){ storedeststats(kvstats, kvdeststats_tmp, 0, deststatsmarker); deststatsmarker += NUM_PARTITIONS; }
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ sourcestatsmarker += NUMSUBWORKERS; }
			else { sourcestatsmarker += 1; }
			
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv * NUMSUBWORKERS; } 
			else { llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv; }
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		printglobalvars();
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
#pragma HLS INTERFACE m_axi port=kvsourcedramA offset=slave bundle=gmem0 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=kvdestdramA offset=slave bundle=gmem1 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=kvstatsA offset=slave bundle=gmem2

#pragma HLS INTERFACE s_axilite port=kvsourcedramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvdestdramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvstatsA  bundle=control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable=kvstatsA
	
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
	// return;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
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
	std::cout<<"Kernel Started: kvstatsA[BASEOFFSET_STATSDRAM + 0].value: "<<kvstatsA[BASEOFFSET_STATSDRAM + 0].value<<std::endl;
	#endif
	#ifdef _DEBUGMODE_CHECKSM
	createDebuggers();
	initializeDebuggers();
	#endif
	// return;
	
	// 	// Throne0(kvsourcedramA, kvdestdramA, kvstatsA, globalparams);
	// 	
	// 	// Rule0(kvsourcedramA, kvdestdramA, kvstatsA, globalparams);
	// 	
	partitionandreduce0(kvsourcedramA, kvdestdramA, kvstatsA, globalparams);
	return;
}
}





