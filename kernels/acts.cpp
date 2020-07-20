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
#include "/home/oj2zf/Documents/ActsOfAGraph/kernels/acts.h"
using namespace std;
#define MMM
#define PARTITIONFUNC1

#ifndef FPGA_IMPL
acts::acts(){}
acts::~acts(){}
#endif
#ifndef HW

unsigned int globalvar_totalkvstatsread;
unsigned int globalvar_totalkvsreadV;

unsigned int globalvar_collectstats_totalkvsread;
unsigned int globalvar_extractcapsules_totalkvsread;
unsigned int globalvar_organizekeyvalues_totalkvsread;
unsigned int globalvar_savestats_totalstatswritten;

unsigned int globalvar_totalkvsread;
unsigned int globalstats_totalkvswritten;

unsigned int globalstats_totalkvspartitioned;
unsigned int globalstats_totalkvspartitionswritten;

unsigned int globalstats_totalkvsreduced;
unsigned int globalstats_totalkvsreducewritten;
#endif 
#ifdef _LDEBUGMODE_HEADER
void acts::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data >= upper_bound){ std::cout<<"acts::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }
}
void acts::checkforequal(string message, unsigned int data1, unsigned int data2){
	if(data1 == data2){ cout<<"acts::checkforequal: ERROR. data1 == data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
}
void acts::print1(string messagea, unsigned int dataa){
	cout<<messagea<<": "<<dataa<<endl;
}
void acts::print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<endl;
}
void acts::print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<endl;
}
void acts::print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<", "<<messagef<<": "<<datef<<endl;
}
void acts::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; }
}
void acts::printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"["<<keyvalues1[p].key<<":"<<keyvalues1[p].value<<"]["<<keyvalues2[p].key<<":"<<keyvalues2[p].value<<"]"<<endl; }
}
void acts::printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs){
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
void acts::getvaluecount(string message, keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	cout<<"total values counted: "<<totalnumkeyvalues<<endl;
}
void acts::printparameters(){
	cout<<endl<<"acts::printparameters: test started."<<endl;
	cout<<"acts::printparameters:: KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
	cout<<"acts::printparameters:: KVDATA_BATCHSIZE_KVS: "<<KVDATA_BATCHSIZE_KVS<<endl;
	
	cout<<"acts::printparameters:: PADDEDBUFFER_SIZE: "<<PADDEDBUFFER_SIZE<<endl;
	
	cout<<"acts::printparameters:: KVDRAMSZ: "<<KVDRAMSZ<<endl;
	cout<<"acts::printparameters:: KVDRAMSZ_KVS: "<<KVDRAMSZ_KVS<<endl;
	
	cout<<"acts::printparameters:: KVSOURCEDRAMSZ: "<<KVSOURCEDRAMSZ<<endl;
	cout<<"acts::printparameters:: KVSOURCEDRAMSZ_KVS: "<<KVSOURCEDRAMSZ_KVS<<endl;
	
	cout<<"acts::printparameters:: PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;
	cout<<"acts::printparameters:: PADDEDKVSOURCEDRAMSZ_KVS: "<<PADDEDKVSOURCEDRAMSZ_KVS<<endl;
	
	cout<<"acts::printparameters:: SRCBUFFER_SIZE: "<<SRCBUFFER_SIZE<<endl;
	cout<<"acts::printparameters:: SRCBUFFER_SIZE * VECTOR_SIZE: "<<SRCBUFFER_SIZE * VECTOR_SIZE<<endl;
	
	// cout<<"acts::printparameters:: DESTBUFFER_SIZE: "<<DESTBUFFER_SIZE<<endl;
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE * VECTOR_SIZE: "<<PADDEDDESTBUFFER_SIZE * VECTOR_SIZE<<endl;
	
	cout<<"acts::printparameters:: PADDEDBUFFER_SIZE / NUM_PARTITIONS: "<<PADDEDBUFFER_SIZE / NUM_PARTITIONS<<endl;
	
	cout<<"acts::printparameters:: NFACTOR: "<<NFACTOR<<endl;
	
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE * NUMSUBWORKERS (total dest_kvs memory): "<<(PADDEDDESTBUFFER_SIZE * NUMSUBWORKERS)<<endl;
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * NUMSUBWORKERS (total dest memory): "<<(PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * NUMSUBWORKERS)<<endl;
}
void acts::printglobalvars(){
	cout<<"printglobalvars: "<<endl;
	cout<<"acts::printglobalvars:: globalvar_totalkvsread: "<<globalvar_totalkvsread<<endl;
	cout<<"acts::printglobalvars:: globalvar_totalkvsreadV: "<<globalvar_totalkvsreadV<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvswritten: "<<globalstats_totalkvswritten<<endl;
	cout<<"acts::printglobalvars:: globalvar_totalkvstatsread: "<<globalvar_totalkvstatsread<<endl;
	cout<<"acts::printglobalvars:: globalvar_collectstats_totalkvsread: "<<globalvar_collectstats_totalkvsread<<endl;
	cout<<"acts::printglobalvars:: globalvar_extractcapsules_totalkvsread: "<<globalvar_extractcapsules_totalkvsread<<endl;
	cout<<"acts::printglobalvars:: globalvar_organizekeyvalues_totalkvsread: "<<globalvar_organizekeyvalues_totalkvsread<<endl;
	cout<<"acts::printglobalvars:: globalvar_savestats_totalstatswritten: "<<globalvar_savestats_totalstatswritten<<endl;	
	cout<<"acts::printglobalvars:: globalstats_totalkvspartitioned: "<<globalstats_totalkvspartitioned<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvspartitionswritten: "<<globalstats_totalkvspartitionswritten<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvsreduced: "<<globalstats_totalkvsreduced<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvsreducewritten: "<<globalstats_totalkvsreducewritten<<endl;
}
void acts::clearglobalvars(){
	cout<<"clearglobalvars: "<<endl;
	globalvar_totalkvsread = 0;
	globalstats_totalkvswritten = 0;
	globalstats_totalkvspartitionswritten = 0;
	globalstats_totalkvspartitioned = 0;
	globalvar_totalkvstatsread = 0;
	globalvar_collectstats_totalkvsread = 0;
	globalvar_extractcapsules_totalkvsread = 0;
	globalvar_organizekeyvalues_totalkvsread = 0;
	globalvar_savestats_totalstatswritten = 0;
	globalstats_totalkvsreduced = 0;
	globalstats_totalkvsreducewritten = 0;
	globalvar_totalkvsreadV = 0;
}
keyvalue_t ** Debugger_Sizes1;
keyvalue_t ** Debugger_Sizes2;
unsigned int debugger_i;
void acts::createDebuggers(){
	Debugger_Sizes1 = new keyvalue_t*[DEBUGGER_SIZE];
	Debugger_Sizes2 = new keyvalue_t*[DEBUGGER_SIZE];
	for(int i = 0; i < DEBUGGER_SIZE; ++i){
		Debugger_Sizes1[i] = new keyvalue_t[NUM_PARTITIONS];
		Debugger_Sizes2[i] = new keyvalue_t[NUM_PARTITIONS];
	}
	initializeDebuggers();
}
void acts::initializeDebuggers(){
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
void acts::debuggerA(keyvalue_t ** Debugger_Sizes, keyvalue_t sizes[NUM_PARTITIONS], unsigned int committype){
	checkoutofbounds("acts::debuggerA 34", debugger_i, DEBUGGER_SIZE, debugger_i, DEBUGGER_SIZE, NAp);
	for(partition_type p=0; p<NUM_PARTITIONS; p++){
		Debugger_Sizes[debugger_i][p].key = 0;
		if(committype == 0){ Debugger_Sizes[debugger_i][p].value += sizes[p].value; }
		else if(committype == 1){ Debugger_Sizes[debugger_i][p].value = sizes[p].value; }
		else {}
	}
}
void acts::IsEqual(keyvalue_t ** data1, keyvalue_t ** data2, unsigned int _1stdimsize, unsigned int _2nddimsize){
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
void acts::globalstats_countkvstatsread(unsigned int count){
	globalvar_totalkvstatsread += count;
	return;
}
void acts::globalvar_collectstats_counttotalkvsread(unsigned int count){
	globalvar_collectstats_totalkvsread += count;
	return;
}
void acts::globalvar_extractcapsules_counttotalkvsread(unsigned int count){
	globalvar_extractcapsules_totalkvsread += count;
	return;
}
void acts::globalvar_organizekeyvalues_counttotalkvsread(unsigned int count){
	globalvar_organizekeyvalues_totalkvsread += count;
	return;
}
void acts::globalvar_savestats_counttotalstatswritten(unsigned int count){
	globalvar_savestats_totalstatswritten += count;
	return;
}
void acts::globalstats_countkvsread(unsigned int count){
	globalvar_totalkvsread += count;
	return;
}
void acts::globalstats_countkvswritten(unsigned int count){
	globalstats_totalkvswritten += count;
	return;
}
void acts::globalstats_countkvspartitionswritten(unsigned int count){
	globalstats_totalkvspartitionswritten += count;
	return;
}
void acts::globalstats_countkvspartitioned(unsigned int count){
	globalstats_totalkvspartitioned += count;
	return;
}
void acts::globalstats_countkvsreduced(unsigned int count){
	globalstats_totalkvsreduced += count;
	return;
}
void acts::globalstats_countkvsreducewritten(unsigned int count){
	globalstats_totalkvsreducewritten += count;
	return;
}
void acts::globalstats_countkvsreadV(unsigned int count){
	globalvar_totalkvsreadV += count;
	return;
}
#endif 

unsigned int acts::allignhigher_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
unsigned int acts::allignlower_KV(unsigned int val){
	unsigned int fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
unsigned int acts::ceildiv(unsigned int val1, unsigned int val2){
	return (val1 + (val2 - 1)) / val2;
}	
unsigned int acts::max(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}
unsigned int acts::min(unsigned int A, unsigned int B){
	#pragma HLS INLINE
	if(A<B){ return A; }
	else { return B; }
}
unsigned int acts::hsub(unsigned int A, unsigned int B){
	#pragma HLS INLINE
	if(A < B){ return 0; }
	else { return A - B; }
}
void acts::checkandforce(unsigned int data, unsigned int upper_bound, unsigned int * datatoforce, unsigned int forceval){
	#pragma HLS INLINE
	return;
	/* if(data >= upper_bound){ *datatoforce = forceval; }
	return; */
}
int acts::WithinValidRange(unsigned int val1, unsigned int val2){
	#pragma HLS INLINE
	if(val1 < val2){ return 1; }
	else { return 0; }
}

unsigned int acts::getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	partition_type val = ((keyvalue.key - upperlimit) >> (KVDATA_RANGE_PERSSDPARTITION_POW - (NUM_PARTITIONS_POW * currentLOP)));
	partition_type hashedval = (((1 << NUM_PARTITIONS_POW) - 1) & (val >> (1 - 1)));
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("acts::getpartition 35", hashedval, NUM_PARTITIONS, NAp, NAp, NAp);
	#endif
	return hashedval;
}
keyvalue_t acts::getkeyvalue(uint512_dt * buffer, batch_type addr, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	if(addr >= (maxaddr_kvs * VECTOR_SIZE)){ addr = 0; } // FIXME. FORCEDFINISH
	keyvalue_t keyvalue;
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	if(idy >= maxaddr_kvs){ cout<<"getkeyvalue:: ERROR 45. addr: "<<addr<<", idy: "<<idy<<", idx: "<<idx<<endl; exit(EXIT_FAILURE); }
	checkoutofbounds("acts::getkeyvalue 35", idy, maxaddr_kvs, NAp, NAp, NAp);
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
void acts::setkeyvalue(uint512_dt * buffer, batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	if(addr >= (maxaddr_kvs * VECTOR_SIZE)){ addr = 0; } // FIXME. FORCEDFINISH
	batch_type idy = (addr / VECTOR_SIZE);
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("acts::setkeyvalue 35", idy, maxaddr_kvs, addr, NAp, NAp);
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
void acts::setkeyvalue(uint512_dt * Vtemp, vector_type idx, keyvalue_t keyvalue){
	// #pragma HLS INLINE OFF
	#ifdef _WIDEWORD
if(idx==0){	
		Vtemp->range(31, 0) = keyvalue.key;
		Vtemp->range(63, 32) = keyvalue.value;
	}
else if(idx==1){	
		Vtemp->range(95, 64) = keyvalue.key;
		Vtemp->range(127, 96) = keyvalue.value;
	}
else if(idx==2){	
		Vtemp->range(159, 128) = keyvalue.key;
		Vtemp->range(191, 160) = keyvalue.value;
	}
else if(idx==3){	
		Vtemp->range(223, 192) = keyvalue.key;
		Vtemp->range(255, 224) = keyvalue.value;
	}
else if(idx==4){	
		Vtemp->range(287, 256) = keyvalue.key;
		Vtemp->range(319, 288) = keyvalue.value;
	}
else if(idx==5){	
		Vtemp->range(351, 320) = keyvalue.key;
		Vtemp->range(383, 352) = keyvalue.value;
	}
else if(idx==6){	
		Vtemp->range(415, 384) = keyvalue.key;
		Vtemp->range(447, 416) = keyvalue.value;
	}
else if(idx==7){	
		Vtemp->range(479, 448) = keyvalue.key;
		Vtemp->range(511, 480) = keyvalue.value;
	}
	else {}
	#else
	Vtemp->data[idx] = keyvalue;
	#endif
	return;
}
void acts::setkeyvalues( uint512_dt * Vtemp00,  uint512_dt * Vtemp01,  uint512_dt * Vtemp02,  uint512_dt * Vtemp03,  uint512_dt * Vtemp04,  uint512_dt * Vtemp05,  uint512_dt * Vtemp06,  uint512_dt * Vtemp07,  uint512_dt * Vtemp10,  uint512_dt * Vtemp11,  uint512_dt * Vtemp12,  uint512_dt * Vtemp13,  uint512_dt * Vtemp14,  uint512_dt * Vtemp15,  uint512_dt * Vtemp16,  uint512_dt * Vtemp17,  uint512_dt * Vtemp20,  uint512_dt * Vtemp21,  uint512_dt * Vtemp22,  uint512_dt * Vtemp23,  uint512_dt * Vtemp24,  uint512_dt * Vtemp25,  uint512_dt * Vtemp26,  uint512_dt * Vtemp27,  uint512_dt * Vtemp30,  uint512_dt * Vtemp31,  uint512_dt * Vtemp32,  uint512_dt * Vtemp33,  uint512_dt * Vtemp34,  uint512_dt * Vtemp35,  uint512_dt * Vtemp36,  uint512_dt * Vtemp37,   keyvalue_t keyvalue00,  keyvalue_t keyvalue01,  keyvalue_t keyvalue02,  keyvalue_t keyvalue03,  keyvalue_t keyvalue04,  keyvalue_t keyvalue05,  keyvalue_t keyvalue06,  keyvalue_t keyvalue07,  keyvalue_t keyvalue10,  keyvalue_t keyvalue11,  keyvalue_t keyvalue12,  keyvalue_t keyvalue13,  keyvalue_t keyvalue14,  keyvalue_t keyvalue15,  keyvalue_t keyvalue16,  keyvalue_t keyvalue17,  keyvalue_t keyvalue20,  keyvalue_t keyvalue21,  keyvalue_t keyvalue22,  keyvalue_t keyvalue23,  keyvalue_t keyvalue24,  keyvalue_t keyvalue25,  keyvalue_t keyvalue26,  keyvalue_t keyvalue27,  keyvalue_t keyvalue30,  keyvalue_t keyvalue31,  keyvalue_t keyvalue32,  keyvalue_t keyvalue33,  keyvalue_t keyvalue34,  keyvalue_t keyvalue35,  keyvalue_t keyvalue36,  keyvalue_t keyvalue37,  vector_type idx){
	#pragma HLS INLINE OFF
	#ifdef _WIDEWORD
if(idx==0){
		Vtemp00->range(31, 0) = keyvalue00.key;
		Vtemp00->range(63, 32) = keyvalue00.value;
		Vtemp01->range(31, 0) = keyvalue01.key;
		Vtemp01->range(63, 32) = keyvalue01.value;
		Vtemp02->range(31, 0) = keyvalue02.key;
		Vtemp02->range(63, 32) = keyvalue02.value;
		Vtemp03->range(31, 0) = keyvalue03.key;
		Vtemp03->range(63, 32) = keyvalue03.value;
		Vtemp04->range(31, 0) = keyvalue04.key;
		Vtemp04->range(63, 32) = keyvalue04.value;
		Vtemp05->range(31, 0) = keyvalue05.key;
		Vtemp05->range(63, 32) = keyvalue05.value;
		Vtemp06->range(31, 0) = keyvalue06.key;
		Vtemp06->range(63, 32) = keyvalue06.value;
		Vtemp07->range(31, 0) = keyvalue07.key;
		Vtemp07->range(63, 32) = keyvalue07.value;
		Vtemp10->range(31, 0) = keyvalue10.key;
		Vtemp10->range(63, 32) = keyvalue10.value;
		Vtemp11->range(31, 0) = keyvalue11.key;
		Vtemp11->range(63, 32) = keyvalue11.value;
		Vtemp12->range(31, 0) = keyvalue12.key;
		Vtemp12->range(63, 32) = keyvalue12.value;
		Vtemp13->range(31, 0) = keyvalue13.key;
		Vtemp13->range(63, 32) = keyvalue13.value;
		Vtemp14->range(31, 0) = keyvalue14.key;
		Vtemp14->range(63, 32) = keyvalue14.value;
		Vtemp15->range(31, 0) = keyvalue15.key;
		Vtemp15->range(63, 32) = keyvalue15.value;
		Vtemp16->range(31, 0) = keyvalue16.key;
		Vtemp16->range(63, 32) = keyvalue16.value;
		Vtemp17->range(31, 0) = keyvalue17.key;
		Vtemp17->range(63, 32) = keyvalue17.value;
		Vtemp20->range(31, 0) = keyvalue20.key;
		Vtemp20->range(63, 32) = keyvalue20.value;
		Vtemp21->range(31, 0) = keyvalue21.key;
		Vtemp21->range(63, 32) = keyvalue21.value;
		Vtemp22->range(31, 0) = keyvalue22.key;
		Vtemp22->range(63, 32) = keyvalue22.value;
		Vtemp23->range(31, 0) = keyvalue23.key;
		Vtemp23->range(63, 32) = keyvalue23.value;
		Vtemp24->range(31, 0) = keyvalue24.key;
		Vtemp24->range(63, 32) = keyvalue24.value;
		Vtemp25->range(31, 0) = keyvalue25.key;
		Vtemp25->range(63, 32) = keyvalue25.value;
		Vtemp26->range(31, 0) = keyvalue26.key;
		Vtemp26->range(63, 32) = keyvalue26.value;
		Vtemp27->range(31, 0) = keyvalue27.key;
		Vtemp27->range(63, 32) = keyvalue27.value;
		Vtemp30->range(31, 0) = keyvalue30.key;
		Vtemp30->range(63, 32) = keyvalue30.value;
		Vtemp31->range(31, 0) = keyvalue31.key;
		Vtemp31->range(63, 32) = keyvalue31.value;
		Vtemp32->range(31, 0) = keyvalue32.key;
		Vtemp32->range(63, 32) = keyvalue32.value;
		Vtemp33->range(31, 0) = keyvalue33.key;
		Vtemp33->range(63, 32) = keyvalue33.value;
		Vtemp34->range(31, 0) = keyvalue34.key;
		Vtemp34->range(63, 32) = keyvalue34.value;
		Vtemp35->range(31, 0) = keyvalue35.key;
		Vtemp35->range(63, 32) = keyvalue35.value;
		Vtemp36->range(31, 0) = keyvalue36.key;
		Vtemp36->range(63, 32) = keyvalue36.value;
		Vtemp37->range(31, 0) = keyvalue37.key;
		Vtemp37->range(63, 32) = keyvalue37.value;
	}
else if(idx==1){
		Vtemp00->range(95, 64) = keyvalue00.key;
		Vtemp00->range(127, 96) = keyvalue00.value;
		Vtemp01->range(95, 64) = keyvalue01.key;
		Vtemp01->range(127, 96) = keyvalue01.value;
		Vtemp02->range(95, 64) = keyvalue02.key;
		Vtemp02->range(127, 96) = keyvalue02.value;
		Vtemp03->range(95, 64) = keyvalue03.key;
		Vtemp03->range(127, 96) = keyvalue03.value;
		Vtemp04->range(95, 64) = keyvalue04.key;
		Vtemp04->range(127, 96) = keyvalue04.value;
		Vtemp05->range(95, 64) = keyvalue05.key;
		Vtemp05->range(127, 96) = keyvalue05.value;
		Vtemp06->range(95, 64) = keyvalue06.key;
		Vtemp06->range(127, 96) = keyvalue06.value;
		Vtemp07->range(95, 64) = keyvalue07.key;
		Vtemp07->range(127, 96) = keyvalue07.value;
		Vtemp10->range(95, 64) = keyvalue10.key;
		Vtemp10->range(127, 96) = keyvalue10.value;
		Vtemp11->range(95, 64) = keyvalue11.key;
		Vtemp11->range(127, 96) = keyvalue11.value;
		Vtemp12->range(95, 64) = keyvalue12.key;
		Vtemp12->range(127, 96) = keyvalue12.value;
		Vtemp13->range(95, 64) = keyvalue13.key;
		Vtemp13->range(127, 96) = keyvalue13.value;
		Vtemp14->range(95, 64) = keyvalue14.key;
		Vtemp14->range(127, 96) = keyvalue14.value;
		Vtemp15->range(95, 64) = keyvalue15.key;
		Vtemp15->range(127, 96) = keyvalue15.value;
		Vtemp16->range(95, 64) = keyvalue16.key;
		Vtemp16->range(127, 96) = keyvalue16.value;
		Vtemp17->range(95, 64) = keyvalue17.key;
		Vtemp17->range(127, 96) = keyvalue17.value;
		Vtemp20->range(95, 64) = keyvalue20.key;
		Vtemp20->range(127, 96) = keyvalue20.value;
		Vtemp21->range(95, 64) = keyvalue21.key;
		Vtemp21->range(127, 96) = keyvalue21.value;
		Vtemp22->range(95, 64) = keyvalue22.key;
		Vtemp22->range(127, 96) = keyvalue22.value;
		Vtemp23->range(95, 64) = keyvalue23.key;
		Vtemp23->range(127, 96) = keyvalue23.value;
		Vtemp24->range(95, 64) = keyvalue24.key;
		Vtemp24->range(127, 96) = keyvalue24.value;
		Vtemp25->range(95, 64) = keyvalue25.key;
		Vtemp25->range(127, 96) = keyvalue25.value;
		Vtemp26->range(95, 64) = keyvalue26.key;
		Vtemp26->range(127, 96) = keyvalue26.value;
		Vtemp27->range(95, 64) = keyvalue27.key;
		Vtemp27->range(127, 96) = keyvalue27.value;
		Vtemp30->range(95, 64) = keyvalue30.key;
		Vtemp30->range(127, 96) = keyvalue30.value;
		Vtemp31->range(95, 64) = keyvalue31.key;
		Vtemp31->range(127, 96) = keyvalue31.value;
		Vtemp32->range(95, 64) = keyvalue32.key;
		Vtemp32->range(127, 96) = keyvalue32.value;
		Vtemp33->range(95, 64) = keyvalue33.key;
		Vtemp33->range(127, 96) = keyvalue33.value;
		Vtemp34->range(95, 64) = keyvalue34.key;
		Vtemp34->range(127, 96) = keyvalue34.value;
		Vtemp35->range(95, 64) = keyvalue35.key;
		Vtemp35->range(127, 96) = keyvalue35.value;
		Vtemp36->range(95, 64) = keyvalue36.key;
		Vtemp36->range(127, 96) = keyvalue36.value;
		Vtemp37->range(95, 64) = keyvalue37.key;
		Vtemp37->range(127, 96) = keyvalue37.value;
	}
else if(idx==2){
		Vtemp00->range(159, 128) = keyvalue00.key;
		Vtemp00->range(191, 160) = keyvalue00.value;
		Vtemp01->range(159, 128) = keyvalue01.key;
		Vtemp01->range(191, 160) = keyvalue01.value;
		Vtemp02->range(159, 128) = keyvalue02.key;
		Vtemp02->range(191, 160) = keyvalue02.value;
		Vtemp03->range(159, 128) = keyvalue03.key;
		Vtemp03->range(191, 160) = keyvalue03.value;
		Vtemp04->range(159, 128) = keyvalue04.key;
		Vtemp04->range(191, 160) = keyvalue04.value;
		Vtemp05->range(159, 128) = keyvalue05.key;
		Vtemp05->range(191, 160) = keyvalue05.value;
		Vtemp06->range(159, 128) = keyvalue06.key;
		Vtemp06->range(191, 160) = keyvalue06.value;
		Vtemp07->range(159, 128) = keyvalue07.key;
		Vtemp07->range(191, 160) = keyvalue07.value;
		Vtemp10->range(159, 128) = keyvalue10.key;
		Vtemp10->range(191, 160) = keyvalue10.value;
		Vtemp11->range(159, 128) = keyvalue11.key;
		Vtemp11->range(191, 160) = keyvalue11.value;
		Vtemp12->range(159, 128) = keyvalue12.key;
		Vtemp12->range(191, 160) = keyvalue12.value;
		Vtemp13->range(159, 128) = keyvalue13.key;
		Vtemp13->range(191, 160) = keyvalue13.value;
		Vtemp14->range(159, 128) = keyvalue14.key;
		Vtemp14->range(191, 160) = keyvalue14.value;
		Vtemp15->range(159, 128) = keyvalue15.key;
		Vtemp15->range(191, 160) = keyvalue15.value;
		Vtemp16->range(159, 128) = keyvalue16.key;
		Vtemp16->range(191, 160) = keyvalue16.value;
		Vtemp17->range(159, 128) = keyvalue17.key;
		Vtemp17->range(191, 160) = keyvalue17.value;
		Vtemp20->range(159, 128) = keyvalue20.key;
		Vtemp20->range(191, 160) = keyvalue20.value;
		Vtemp21->range(159, 128) = keyvalue21.key;
		Vtemp21->range(191, 160) = keyvalue21.value;
		Vtemp22->range(159, 128) = keyvalue22.key;
		Vtemp22->range(191, 160) = keyvalue22.value;
		Vtemp23->range(159, 128) = keyvalue23.key;
		Vtemp23->range(191, 160) = keyvalue23.value;
		Vtemp24->range(159, 128) = keyvalue24.key;
		Vtemp24->range(191, 160) = keyvalue24.value;
		Vtemp25->range(159, 128) = keyvalue25.key;
		Vtemp25->range(191, 160) = keyvalue25.value;
		Vtemp26->range(159, 128) = keyvalue26.key;
		Vtemp26->range(191, 160) = keyvalue26.value;
		Vtemp27->range(159, 128) = keyvalue27.key;
		Vtemp27->range(191, 160) = keyvalue27.value;
		Vtemp30->range(159, 128) = keyvalue30.key;
		Vtemp30->range(191, 160) = keyvalue30.value;
		Vtemp31->range(159, 128) = keyvalue31.key;
		Vtemp31->range(191, 160) = keyvalue31.value;
		Vtemp32->range(159, 128) = keyvalue32.key;
		Vtemp32->range(191, 160) = keyvalue32.value;
		Vtemp33->range(159, 128) = keyvalue33.key;
		Vtemp33->range(191, 160) = keyvalue33.value;
		Vtemp34->range(159, 128) = keyvalue34.key;
		Vtemp34->range(191, 160) = keyvalue34.value;
		Vtemp35->range(159, 128) = keyvalue35.key;
		Vtemp35->range(191, 160) = keyvalue35.value;
		Vtemp36->range(159, 128) = keyvalue36.key;
		Vtemp36->range(191, 160) = keyvalue36.value;
		Vtemp37->range(159, 128) = keyvalue37.key;
		Vtemp37->range(191, 160) = keyvalue37.value;
	}
else if(idx==3){
		Vtemp00->range(223, 192) = keyvalue00.key;
		Vtemp00->range(255, 224) = keyvalue00.value;
		Vtemp01->range(223, 192) = keyvalue01.key;
		Vtemp01->range(255, 224) = keyvalue01.value;
		Vtemp02->range(223, 192) = keyvalue02.key;
		Vtemp02->range(255, 224) = keyvalue02.value;
		Vtemp03->range(223, 192) = keyvalue03.key;
		Vtemp03->range(255, 224) = keyvalue03.value;
		Vtemp04->range(223, 192) = keyvalue04.key;
		Vtemp04->range(255, 224) = keyvalue04.value;
		Vtemp05->range(223, 192) = keyvalue05.key;
		Vtemp05->range(255, 224) = keyvalue05.value;
		Vtemp06->range(223, 192) = keyvalue06.key;
		Vtemp06->range(255, 224) = keyvalue06.value;
		Vtemp07->range(223, 192) = keyvalue07.key;
		Vtemp07->range(255, 224) = keyvalue07.value;
		Vtemp10->range(223, 192) = keyvalue10.key;
		Vtemp10->range(255, 224) = keyvalue10.value;
		Vtemp11->range(223, 192) = keyvalue11.key;
		Vtemp11->range(255, 224) = keyvalue11.value;
		Vtemp12->range(223, 192) = keyvalue12.key;
		Vtemp12->range(255, 224) = keyvalue12.value;
		Vtemp13->range(223, 192) = keyvalue13.key;
		Vtemp13->range(255, 224) = keyvalue13.value;
		Vtemp14->range(223, 192) = keyvalue14.key;
		Vtemp14->range(255, 224) = keyvalue14.value;
		Vtemp15->range(223, 192) = keyvalue15.key;
		Vtemp15->range(255, 224) = keyvalue15.value;
		Vtemp16->range(223, 192) = keyvalue16.key;
		Vtemp16->range(255, 224) = keyvalue16.value;
		Vtemp17->range(223, 192) = keyvalue17.key;
		Vtemp17->range(255, 224) = keyvalue17.value;
		Vtemp20->range(223, 192) = keyvalue20.key;
		Vtemp20->range(255, 224) = keyvalue20.value;
		Vtemp21->range(223, 192) = keyvalue21.key;
		Vtemp21->range(255, 224) = keyvalue21.value;
		Vtemp22->range(223, 192) = keyvalue22.key;
		Vtemp22->range(255, 224) = keyvalue22.value;
		Vtemp23->range(223, 192) = keyvalue23.key;
		Vtemp23->range(255, 224) = keyvalue23.value;
		Vtemp24->range(223, 192) = keyvalue24.key;
		Vtemp24->range(255, 224) = keyvalue24.value;
		Vtemp25->range(223, 192) = keyvalue25.key;
		Vtemp25->range(255, 224) = keyvalue25.value;
		Vtemp26->range(223, 192) = keyvalue26.key;
		Vtemp26->range(255, 224) = keyvalue26.value;
		Vtemp27->range(223, 192) = keyvalue27.key;
		Vtemp27->range(255, 224) = keyvalue27.value;
		Vtemp30->range(223, 192) = keyvalue30.key;
		Vtemp30->range(255, 224) = keyvalue30.value;
		Vtemp31->range(223, 192) = keyvalue31.key;
		Vtemp31->range(255, 224) = keyvalue31.value;
		Vtemp32->range(223, 192) = keyvalue32.key;
		Vtemp32->range(255, 224) = keyvalue32.value;
		Vtemp33->range(223, 192) = keyvalue33.key;
		Vtemp33->range(255, 224) = keyvalue33.value;
		Vtemp34->range(223, 192) = keyvalue34.key;
		Vtemp34->range(255, 224) = keyvalue34.value;
		Vtemp35->range(223, 192) = keyvalue35.key;
		Vtemp35->range(255, 224) = keyvalue35.value;
		Vtemp36->range(223, 192) = keyvalue36.key;
		Vtemp36->range(255, 224) = keyvalue36.value;
		Vtemp37->range(223, 192) = keyvalue37.key;
		Vtemp37->range(255, 224) = keyvalue37.value;
	}
else if(idx==4){
		Vtemp00->range(287, 256) = keyvalue00.key;
		Vtemp00->range(319, 288) = keyvalue00.value;
		Vtemp01->range(287, 256) = keyvalue01.key;
		Vtemp01->range(319, 288) = keyvalue01.value;
		Vtemp02->range(287, 256) = keyvalue02.key;
		Vtemp02->range(319, 288) = keyvalue02.value;
		Vtemp03->range(287, 256) = keyvalue03.key;
		Vtemp03->range(319, 288) = keyvalue03.value;
		Vtemp04->range(287, 256) = keyvalue04.key;
		Vtemp04->range(319, 288) = keyvalue04.value;
		Vtemp05->range(287, 256) = keyvalue05.key;
		Vtemp05->range(319, 288) = keyvalue05.value;
		Vtemp06->range(287, 256) = keyvalue06.key;
		Vtemp06->range(319, 288) = keyvalue06.value;
		Vtemp07->range(287, 256) = keyvalue07.key;
		Vtemp07->range(319, 288) = keyvalue07.value;
		Vtemp10->range(287, 256) = keyvalue10.key;
		Vtemp10->range(319, 288) = keyvalue10.value;
		Vtemp11->range(287, 256) = keyvalue11.key;
		Vtemp11->range(319, 288) = keyvalue11.value;
		Vtemp12->range(287, 256) = keyvalue12.key;
		Vtemp12->range(319, 288) = keyvalue12.value;
		Vtemp13->range(287, 256) = keyvalue13.key;
		Vtemp13->range(319, 288) = keyvalue13.value;
		Vtemp14->range(287, 256) = keyvalue14.key;
		Vtemp14->range(319, 288) = keyvalue14.value;
		Vtemp15->range(287, 256) = keyvalue15.key;
		Vtemp15->range(319, 288) = keyvalue15.value;
		Vtemp16->range(287, 256) = keyvalue16.key;
		Vtemp16->range(319, 288) = keyvalue16.value;
		Vtemp17->range(287, 256) = keyvalue17.key;
		Vtemp17->range(319, 288) = keyvalue17.value;
		Vtemp20->range(287, 256) = keyvalue20.key;
		Vtemp20->range(319, 288) = keyvalue20.value;
		Vtemp21->range(287, 256) = keyvalue21.key;
		Vtemp21->range(319, 288) = keyvalue21.value;
		Vtemp22->range(287, 256) = keyvalue22.key;
		Vtemp22->range(319, 288) = keyvalue22.value;
		Vtemp23->range(287, 256) = keyvalue23.key;
		Vtemp23->range(319, 288) = keyvalue23.value;
		Vtemp24->range(287, 256) = keyvalue24.key;
		Vtemp24->range(319, 288) = keyvalue24.value;
		Vtemp25->range(287, 256) = keyvalue25.key;
		Vtemp25->range(319, 288) = keyvalue25.value;
		Vtemp26->range(287, 256) = keyvalue26.key;
		Vtemp26->range(319, 288) = keyvalue26.value;
		Vtemp27->range(287, 256) = keyvalue27.key;
		Vtemp27->range(319, 288) = keyvalue27.value;
		Vtemp30->range(287, 256) = keyvalue30.key;
		Vtemp30->range(319, 288) = keyvalue30.value;
		Vtemp31->range(287, 256) = keyvalue31.key;
		Vtemp31->range(319, 288) = keyvalue31.value;
		Vtemp32->range(287, 256) = keyvalue32.key;
		Vtemp32->range(319, 288) = keyvalue32.value;
		Vtemp33->range(287, 256) = keyvalue33.key;
		Vtemp33->range(319, 288) = keyvalue33.value;
		Vtemp34->range(287, 256) = keyvalue34.key;
		Vtemp34->range(319, 288) = keyvalue34.value;
		Vtemp35->range(287, 256) = keyvalue35.key;
		Vtemp35->range(319, 288) = keyvalue35.value;
		Vtemp36->range(287, 256) = keyvalue36.key;
		Vtemp36->range(319, 288) = keyvalue36.value;
		Vtemp37->range(287, 256) = keyvalue37.key;
		Vtemp37->range(319, 288) = keyvalue37.value;
	}
else if(idx==5){
		Vtemp00->range(351, 320) = keyvalue00.key;
		Vtemp00->range(383, 352) = keyvalue00.value;
		Vtemp01->range(351, 320) = keyvalue01.key;
		Vtemp01->range(383, 352) = keyvalue01.value;
		Vtemp02->range(351, 320) = keyvalue02.key;
		Vtemp02->range(383, 352) = keyvalue02.value;
		Vtemp03->range(351, 320) = keyvalue03.key;
		Vtemp03->range(383, 352) = keyvalue03.value;
		Vtemp04->range(351, 320) = keyvalue04.key;
		Vtemp04->range(383, 352) = keyvalue04.value;
		Vtemp05->range(351, 320) = keyvalue05.key;
		Vtemp05->range(383, 352) = keyvalue05.value;
		Vtemp06->range(351, 320) = keyvalue06.key;
		Vtemp06->range(383, 352) = keyvalue06.value;
		Vtemp07->range(351, 320) = keyvalue07.key;
		Vtemp07->range(383, 352) = keyvalue07.value;
		Vtemp10->range(351, 320) = keyvalue10.key;
		Vtemp10->range(383, 352) = keyvalue10.value;
		Vtemp11->range(351, 320) = keyvalue11.key;
		Vtemp11->range(383, 352) = keyvalue11.value;
		Vtemp12->range(351, 320) = keyvalue12.key;
		Vtemp12->range(383, 352) = keyvalue12.value;
		Vtemp13->range(351, 320) = keyvalue13.key;
		Vtemp13->range(383, 352) = keyvalue13.value;
		Vtemp14->range(351, 320) = keyvalue14.key;
		Vtemp14->range(383, 352) = keyvalue14.value;
		Vtemp15->range(351, 320) = keyvalue15.key;
		Vtemp15->range(383, 352) = keyvalue15.value;
		Vtemp16->range(351, 320) = keyvalue16.key;
		Vtemp16->range(383, 352) = keyvalue16.value;
		Vtemp17->range(351, 320) = keyvalue17.key;
		Vtemp17->range(383, 352) = keyvalue17.value;
		Vtemp20->range(351, 320) = keyvalue20.key;
		Vtemp20->range(383, 352) = keyvalue20.value;
		Vtemp21->range(351, 320) = keyvalue21.key;
		Vtemp21->range(383, 352) = keyvalue21.value;
		Vtemp22->range(351, 320) = keyvalue22.key;
		Vtemp22->range(383, 352) = keyvalue22.value;
		Vtemp23->range(351, 320) = keyvalue23.key;
		Vtemp23->range(383, 352) = keyvalue23.value;
		Vtemp24->range(351, 320) = keyvalue24.key;
		Vtemp24->range(383, 352) = keyvalue24.value;
		Vtemp25->range(351, 320) = keyvalue25.key;
		Vtemp25->range(383, 352) = keyvalue25.value;
		Vtemp26->range(351, 320) = keyvalue26.key;
		Vtemp26->range(383, 352) = keyvalue26.value;
		Vtemp27->range(351, 320) = keyvalue27.key;
		Vtemp27->range(383, 352) = keyvalue27.value;
		Vtemp30->range(351, 320) = keyvalue30.key;
		Vtemp30->range(383, 352) = keyvalue30.value;
		Vtemp31->range(351, 320) = keyvalue31.key;
		Vtemp31->range(383, 352) = keyvalue31.value;
		Vtemp32->range(351, 320) = keyvalue32.key;
		Vtemp32->range(383, 352) = keyvalue32.value;
		Vtemp33->range(351, 320) = keyvalue33.key;
		Vtemp33->range(383, 352) = keyvalue33.value;
		Vtemp34->range(351, 320) = keyvalue34.key;
		Vtemp34->range(383, 352) = keyvalue34.value;
		Vtemp35->range(351, 320) = keyvalue35.key;
		Vtemp35->range(383, 352) = keyvalue35.value;
		Vtemp36->range(351, 320) = keyvalue36.key;
		Vtemp36->range(383, 352) = keyvalue36.value;
		Vtemp37->range(351, 320) = keyvalue37.key;
		Vtemp37->range(383, 352) = keyvalue37.value;
	}
else if(idx==6){
		Vtemp00->range(415, 384) = keyvalue00.key;
		Vtemp00->range(447, 416) = keyvalue00.value;
		Vtemp01->range(415, 384) = keyvalue01.key;
		Vtemp01->range(447, 416) = keyvalue01.value;
		Vtemp02->range(415, 384) = keyvalue02.key;
		Vtemp02->range(447, 416) = keyvalue02.value;
		Vtemp03->range(415, 384) = keyvalue03.key;
		Vtemp03->range(447, 416) = keyvalue03.value;
		Vtemp04->range(415, 384) = keyvalue04.key;
		Vtemp04->range(447, 416) = keyvalue04.value;
		Vtemp05->range(415, 384) = keyvalue05.key;
		Vtemp05->range(447, 416) = keyvalue05.value;
		Vtemp06->range(415, 384) = keyvalue06.key;
		Vtemp06->range(447, 416) = keyvalue06.value;
		Vtemp07->range(415, 384) = keyvalue07.key;
		Vtemp07->range(447, 416) = keyvalue07.value;
		Vtemp10->range(415, 384) = keyvalue10.key;
		Vtemp10->range(447, 416) = keyvalue10.value;
		Vtemp11->range(415, 384) = keyvalue11.key;
		Vtemp11->range(447, 416) = keyvalue11.value;
		Vtemp12->range(415, 384) = keyvalue12.key;
		Vtemp12->range(447, 416) = keyvalue12.value;
		Vtemp13->range(415, 384) = keyvalue13.key;
		Vtemp13->range(447, 416) = keyvalue13.value;
		Vtemp14->range(415, 384) = keyvalue14.key;
		Vtemp14->range(447, 416) = keyvalue14.value;
		Vtemp15->range(415, 384) = keyvalue15.key;
		Vtemp15->range(447, 416) = keyvalue15.value;
		Vtemp16->range(415, 384) = keyvalue16.key;
		Vtemp16->range(447, 416) = keyvalue16.value;
		Vtemp17->range(415, 384) = keyvalue17.key;
		Vtemp17->range(447, 416) = keyvalue17.value;
		Vtemp20->range(415, 384) = keyvalue20.key;
		Vtemp20->range(447, 416) = keyvalue20.value;
		Vtemp21->range(415, 384) = keyvalue21.key;
		Vtemp21->range(447, 416) = keyvalue21.value;
		Vtemp22->range(415, 384) = keyvalue22.key;
		Vtemp22->range(447, 416) = keyvalue22.value;
		Vtemp23->range(415, 384) = keyvalue23.key;
		Vtemp23->range(447, 416) = keyvalue23.value;
		Vtemp24->range(415, 384) = keyvalue24.key;
		Vtemp24->range(447, 416) = keyvalue24.value;
		Vtemp25->range(415, 384) = keyvalue25.key;
		Vtemp25->range(447, 416) = keyvalue25.value;
		Vtemp26->range(415, 384) = keyvalue26.key;
		Vtemp26->range(447, 416) = keyvalue26.value;
		Vtemp27->range(415, 384) = keyvalue27.key;
		Vtemp27->range(447, 416) = keyvalue27.value;
		Vtemp30->range(415, 384) = keyvalue30.key;
		Vtemp30->range(447, 416) = keyvalue30.value;
		Vtemp31->range(415, 384) = keyvalue31.key;
		Vtemp31->range(447, 416) = keyvalue31.value;
		Vtemp32->range(415, 384) = keyvalue32.key;
		Vtemp32->range(447, 416) = keyvalue32.value;
		Vtemp33->range(415, 384) = keyvalue33.key;
		Vtemp33->range(447, 416) = keyvalue33.value;
		Vtemp34->range(415, 384) = keyvalue34.key;
		Vtemp34->range(447, 416) = keyvalue34.value;
		Vtemp35->range(415, 384) = keyvalue35.key;
		Vtemp35->range(447, 416) = keyvalue35.value;
		Vtemp36->range(415, 384) = keyvalue36.key;
		Vtemp36->range(447, 416) = keyvalue36.value;
		Vtemp37->range(415, 384) = keyvalue37.key;
		Vtemp37->range(447, 416) = keyvalue37.value;
	}
else if(idx==7){
		Vtemp00->range(479, 448) = keyvalue00.key;
		Vtemp00->range(511, 480) = keyvalue00.value;
		Vtemp01->range(479, 448) = keyvalue01.key;
		Vtemp01->range(511, 480) = keyvalue01.value;
		Vtemp02->range(479, 448) = keyvalue02.key;
		Vtemp02->range(511, 480) = keyvalue02.value;
		Vtemp03->range(479, 448) = keyvalue03.key;
		Vtemp03->range(511, 480) = keyvalue03.value;
		Vtemp04->range(479, 448) = keyvalue04.key;
		Vtemp04->range(511, 480) = keyvalue04.value;
		Vtemp05->range(479, 448) = keyvalue05.key;
		Vtemp05->range(511, 480) = keyvalue05.value;
		Vtemp06->range(479, 448) = keyvalue06.key;
		Vtemp06->range(511, 480) = keyvalue06.value;
		Vtemp07->range(479, 448) = keyvalue07.key;
		Vtemp07->range(511, 480) = keyvalue07.value;
		Vtemp10->range(479, 448) = keyvalue10.key;
		Vtemp10->range(511, 480) = keyvalue10.value;
		Vtemp11->range(479, 448) = keyvalue11.key;
		Vtemp11->range(511, 480) = keyvalue11.value;
		Vtemp12->range(479, 448) = keyvalue12.key;
		Vtemp12->range(511, 480) = keyvalue12.value;
		Vtemp13->range(479, 448) = keyvalue13.key;
		Vtemp13->range(511, 480) = keyvalue13.value;
		Vtemp14->range(479, 448) = keyvalue14.key;
		Vtemp14->range(511, 480) = keyvalue14.value;
		Vtemp15->range(479, 448) = keyvalue15.key;
		Vtemp15->range(511, 480) = keyvalue15.value;
		Vtemp16->range(479, 448) = keyvalue16.key;
		Vtemp16->range(511, 480) = keyvalue16.value;
		Vtemp17->range(479, 448) = keyvalue17.key;
		Vtemp17->range(511, 480) = keyvalue17.value;
		Vtemp20->range(479, 448) = keyvalue20.key;
		Vtemp20->range(511, 480) = keyvalue20.value;
		Vtemp21->range(479, 448) = keyvalue21.key;
		Vtemp21->range(511, 480) = keyvalue21.value;
		Vtemp22->range(479, 448) = keyvalue22.key;
		Vtemp22->range(511, 480) = keyvalue22.value;
		Vtemp23->range(479, 448) = keyvalue23.key;
		Vtemp23->range(511, 480) = keyvalue23.value;
		Vtemp24->range(479, 448) = keyvalue24.key;
		Vtemp24->range(511, 480) = keyvalue24.value;
		Vtemp25->range(479, 448) = keyvalue25.key;
		Vtemp25->range(511, 480) = keyvalue25.value;
		Vtemp26->range(479, 448) = keyvalue26.key;
		Vtemp26->range(511, 480) = keyvalue26.value;
		Vtemp27->range(479, 448) = keyvalue27.key;
		Vtemp27->range(511, 480) = keyvalue27.value;
		Vtemp30->range(479, 448) = keyvalue30.key;
		Vtemp30->range(511, 480) = keyvalue30.value;
		Vtemp31->range(479, 448) = keyvalue31.key;
		Vtemp31->range(511, 480) = keyvalue31.value;
		Vtemp32->range(479, 448) = keyvalue32.key;
		Vtemp32->range(511, 480) = keyvalue32.value;
		Vtemp33->range(479, 448) = keyvalue33.key;
		Vtemp33->range(511, 480) = keyvalue33.value;
		Vtemp34->range(479, 448) = keyvalue34.key;
		Vtemp34->range(511, 480) = keyvalue34.value;
		Vtemp35->range(479, 448) = keyvalue35.key;
		Vtemp35->range(511, 480) = keyvalue35.value;
		Vtemp36->range(479, 448) = keyvalue36.key;
		Vtemp36->range(511, 480) = keyvalue36.value;
		Vtemp37->range(479, 448) = keyvalue37.key;
		Vtemp37->range(511, 480) = keyvalue37.value;
	}
	else {}
	#else
	Vtemp00->data[idx] = keyvalue00;
	Vtemp01->data[idx] = keyvalue01;
	Vtemp02->data[idx] = keyvalue02;
	Vtemp03->data[idx] = keyvalue03;
	Vtemp04->data[idx] = keyvalue04;
	Vtemp05->data[idx] = keyvalue05;
	Vtemp06->data[idx] = keyvalue06;
	Vtemp07->data[idx] = keyvalue07;
	Vtemp10->data[idx] = keyvalue10;
	Vtemp11->data[idx] = keyvalue11;
	Vtemp12->data[idx] = keyvalue12;
	Vtemp13->data[idx] = keyvalue13;
	Vtemp14->data[idx] = keyvalue14;
	Vtemp15->data[idx] = keyvalue15;
	Vtemp16->data[idx] = keyvalue16;
	Vtemp17->data[idx] = keyvalue17;
	Vtemp20->data[idx] = keyvalue20;
	Vtemp21->data[idx] = keyvalue21;
	Vtemp22->data[idx] = keyvalue22;
	Vtemp23->data[idx] = keyvalue23;
	Vtemp24->data[idx] = keyvalue24;
	Vtemp25->data[idx] = keyvalue25;
	Vtemp26->data[idx] = keyvalue26;
	Vtemp27->data[idx] = keyvalue27;
	Vtemp30->data[idx] = keyvalue30;
	Vtemp31->data[idx] = keyvalue31;
	Vtemp32->data[idx] = keyvalue32;
	Vtemp33->data[idx] = keyvalue33;
	Vtemp34->data[idx] = keyvalue34;
	Vtemp35->data[idx] = keyvalue35;
	Vtemp36->data[idx] = keyvalue36;
	Vtemp37->data[idx] = keyvalue37;
	#endif
	return;
}

void acts::resetkeyandvalues(keyvalue_t * buffer, unsigned int size){
	#pragma HLS INLINE
	RESETKEYVANDVALUES_LOOP: for(partition_type p=0; p<size; p++){ buffer[p].key = 0; buffer[p].value = 0; }
}
void acts::resetvalues(keyvalue_t * buffer, unsigned int size){
	#pragma HLS INLINE
	RESETVALUES_LOOP: for(partition_type p=0; p<size; p++){ buffer[p].value = 0; }
}
void acts::copykeys(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	#pragma HLS INLINE
	COPYKEYS_LOOP: for(partition_type p=0; p<size; p++){ buffer2[p].key = buffer1[p].key; }
}
void acts::copy(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	#pragma HLS INLINE
	COPYKEYS_LOOP: for(partition_type p=0; p<size; p++){ buffer2[p] = buffer1[p]; }
}

buffer_type acts::getactivesize(buffer_type i, batch_type i_end, buffer_type buffersize){	
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
batch_type acts::getAddr(batch_type addr){
	#pragma HLS INLINE
	return addr;
}
batch_type acts::getstatsAddr(batch_type addr){
	#pragma HLS INLINE
	return BASEOFFSET_STATSDRAM + addr;
}
batch_type acts::getmessagesAddr(batch_type addr){
	#pragma HLS INLINE
	return BASEOFFSET_MESSAGESDRAM + addr;
}

unsigned int acts::getsourceskipsize(unsigned int currentLOP, unsigned int param){
	#pragma HLS INLINE
	unsigned int treedepth = TREE_DEPTH; // NB: for debugging purposes only
	unsigned int result = param;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		result = result / NUM_PARTITIONS;
	}
	return result;
}
unsigned int acts::getdestskipsize(unsigned int currentLOP, unsigned int param){
	#pragma HLS INLINE
	unsigned int treedepthloop_tripcount = TREE_DEPTH; // NB: for debugging purposes only
	unsigned int result = param / NUM_PARTITIONS;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepthloop_tripcount avg=treedepthloop_tripcount
		result = result / NUM_PARTITIONS;
	}
	return result;
}
void acts::storedeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int drambaseoffset, unsigned int dramloffset){
	#pragma HLS INLINE
	batch_type offset = drambaseoffset + dramloffset;
	
	STOREDESTSTATS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvstats[getstatsAddr(offset + p)] = buffer[p];
	}
	return;
}
unsigned int acts::get_num_source_partitions(unsigned int currentLOP){
	#pragma HLS INLINE
	unsigned int treedepth = TREE_DEPTH;
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}

void acts::calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS]){
	#pragma HLS INLINE
	capsule[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){ capsule[p].key = capsule[p-1].key + capsule[p-1].value; }
	return;
}
void acts::calculateoffsets(keyvalue_t capsule0[NUM_PARTITIONS],keyvalue_t capsule1[NUM_PARTITIONS],keyvalue_t capsule2[NUM_PARTITIONS],keyvalue_t capsule3[NUM_PARTITIONS], int dummy){
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
	checkoutofbounds("acts::calculateoffsets 34", capsule0[NUM_PARTITIONS-1].key + capsule0[NUM_PARTITIONS-1].value, (SRCBUFFER_SIZE * NFACTOR * VECTOR_SIZE) + 1, capsule0[NUM_PARTITIONS-1].key, capsule0[NUM_PARTITIONS-1].value, PADDEDDESTBUFFER_SIZE);
	checkoutofbounds("acts::calculateoffsets 34", capsule1[NUM_PARTITIONS-1].key + capsule1[NUM_PARTITIONS-1].value, (SRCBUFFER_SIZE * NFACTOR * VECTOR_SIZE) + 1, capsule1[NUM_PARTITIONS-1].key, capsule1[NUM_PARTITIONS-1].value, PADDEDDESTBUFFER_SIZE);
	checkoutofbounds("acts::calculateoffsets 34", capsule2[NUM_PARTITIONS-1].key + capsule2[NUM_PARTITIONS-1].value, (SRCBUFFER_SIZE * NFACTOR * VECTOR_SIZE) + 1, capsule2[NUM_PARTITIONS-1].key, capsule2[NUM_PARTITIONS-1].value, PADDEDDESTBUFFER_SIZE);
	checkoutofbounds("acts::calculateoffsets 34", capsule3[NUM_PARTITIONS-1].key + capsule3[NUM_PARTITIONS-1].value, (SRCBUFFER_SIZE * NFACTOR * VECTOR_SIZE) + 1, capsule3[NUM_PARTITIONS-1].key, capsule3[NUM_PARTITIONS-1].value, PADDEDDESTBUFFER_SIZE);
	#endif
	return;
}
void acts::calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int offset){
	#pragma HLS INLINE
	capsule[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){ capsule[p].key = capsule[p-1].key + capsule[p-1].value; }
	PREPARECAPSULE_LOOP2: for (partition_type p = 0; p<NUM_PARTITIONS; p++){ capsule[p].key += offset; }
	return;
}

void acts::loadclopparams(globalparams_t globalparams, clopparams_t * llopparams, unsigned int currentLOP){
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
value_t acts::reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int GraphIter){
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
void acts::getchunksize(buffer_type * chunk_size, buffer_type buffer_size, travstate_t travstate, unsigned int localoffset_kvs){
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
int acts::IsReducePhase(unsigned int currentLOP, unsigned int treedepth, globalparams_t globalparams){
	if((currentLOP == (treedepth + 1)) && (globalparams.reducecommand == ON)){ return 1; }
	return 0;
}

void acts::readkeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	unsigned int analysis_buffersz = SRCBUFFER_SIZE;
	
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &size_kvs, 0);
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &offset_kvs, 0);
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("acts::readkeyvalues dram 36", baseaddress + offset_kvs + size_kvs, maxaddress_kvs, baseaddress, offset_kvs, size_kvs);
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
void acts::readkeyvalues0(unsigned int workerID, uint512_dt * kvdram  ,uint512_dt * buffer0  ,uint512_dt * buffer1  ,uint512_dt * buffer2  ,uint512_dt * buffer3  ,batch_type baseaddress ,batch_type offset_kvs, batch_type maxaddress_kvs, travstate_t travstate){			
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	readkeyvalues0(kvdram, buffer0, baseaddress, offset_kvs + (workerID * 0 * SRCBUFFER_SIZE), chunk0_size, maxaddress_kvs);
	readkeyvalues0(kvdram, buffer1, baseaddress, offset_kvs + (workerID * 1 * SRCBUFFER_SIZE), chunk1_size, maxaddress_kvs);
	readkeyvalues0(kvdram, buffer2, baseaddress, offset_kvs + (workerID * 2 * SRCBUFFER_SIZE), chunk2_size, maxaddress_kvs);
	readkeyvalues0(kvdram, buffer3, baseaddress, offset_kvs + (workerID * 3 * SRCBUFFER_SIZE), chunk3_size, maxaddress_kvs);
	return;
}
void acts::readkeyvalues0(unsigned int workerID, uint512_dt * kvdram  ,uint512_dt * buffer0  ,uint512_dt * buffer1  ,uint512_dt * buffer2  ,uint512_dt * buffer3  ,batch_type baseaddress, keyvalue_t sourcestats[NUMSUBWORKERS], batch_type maxaddress_kvs, travstate_t travstates[NUMSUBWORKERS]){			
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstates[0], 0);
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstates[1], 0);
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstates[2], 0);
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstates[3], 0);
	
	readkeyvalues0(kvdram, buffer0, baseaddress, ((sourcestats[0].key / VECTOR_SIZE) + travstates[0].i_kvs) + (workerID * 0 * SRCBUFFER_SIZE), chunk0_size, maxaddress_kvs);
	readkeyvalues0(kvdram, buffer1, baseaddress, ((sourcestats[1].key / VECTOR_SIZE) + travstates[1].i_kvs) + (workerID * 1 * SRCBUFFER_SIZE), chunk1_size, maxaddress_kvs);
	readkeyvalues0(kvdram, buffer2, baseaddress, ((sourcestats[2].key / VECTOR_SIZE) + travstates[2].i_kvs) + (workerID * 2 * SRCBUFFER_SIZE), chunk2_size, maxaddress_kvs);
	readkeyvalues0(kvdram, buffer3, baseaddress, ((sourcestats[3].key / VECTOR_SIZE) + travstates[3].i_kvs) + (workerID * 3 * SRCBUFFER_SIZE), chunk3_size, maxaddress_kvs);
	return;
}
travoffsets_t acts::readcapsules0(unsigned int workerID, uint512_dt * kvdram,  uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE], keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  batch_type baseaddress_kvs, batch_type offset_kvs, int enable, travoffsets_t travoffsets){
	if(enable == 0){ return travoffsets; }
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("acts::readcapsules0 35", (baseaddress_kvs + offset_kvs + NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
	#endif
	
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	if(loffset == 0){
		batch_type loadoffset0_kvs = travoffsets.capsuleoffset_kvs + 0 * CAPSULEBUFFER_SIZE; 
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("acts::readcapsules0 34", baseaddress_kvs + loadoffset0_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule0, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset1_kvs = travoffsets.capsuleoffset_kvs + 1 * CAPSULEBUFFER_SIZE; 
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("acts::readcapsules0 34", baseaddress_kvs + loadoffset1_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule1, baseaddress_kvs, loadoffset1_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset2_kvs = travoffsets.capsuleoffset_kvs + 2 * CAPSULEBUFFER_SIZE; 
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("acts::readcapsules0 34", baseaddress_kvs + loadoffset2_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule2, baseaddress_kvs, loadoffset2_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type loadoffset3_kvs = travoffsets.capsuleoffset_kvs + 3 * CAPSULEBUFFER_SIZE; 
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("acts::readcapsules0 34", baseaddress_kvs + loadoffset3_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule3, baseaddress_kvs, loadoffset3_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS);
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

/** void acts::collectstats00(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
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
			keyvalue_t keyvalue1 = getkeyvalue(sourcebuffer1, k, SRCBUFFER_SIZE);
			partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
			keyvalue_t keyvalue2 = getkeyvalue(sourcebuffer2, k, SRCBUFFER_SIZE);
			partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
			keyvalue_t keyvalue3 = getkeyvalue(sourcebuffer3, k, SRCBUFFER_SIZE);
			partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
			if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		#ifdef _DEBUGMODE_STATS
		globalvar_collectstats_counttotalkvsread(NUMSUBWORKERS);
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
	printkeyvalues("collect capsules", capsule0, NUM_PARTITIONS);
	// exit(EXIT_SUCCESS);
	return;
}
 */
void acts::collectstats00(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	if(enable == 0){ return; }
	unsigned int analysis_collectstats = SRCBUFFER_SIZE;
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type chunk1_size = SRCBUFFER_SIZE;
	getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (1 * SRCBUFFER_SIZE)));
	
	buffer_type chunk2_size = SRCBUFFER_SIZE;
	getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (2 * SRCBUFFER_SIZE)));
	
	buffer_type chunk3_size = SRCBUFFER_SIZE;
	getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (3 * SRCBUFFER_SIZE)));
	
	buffer_type sizes00[NUM_PARTITIONS];
	buffer_type sizes01[NUM_PARTITIONS];
	buffer_type sizes02[NUM_PARTITIONS];
	buffer_type sizes03[NUM_PARTITIONS];
	buffer_type sizes04[NUM_PARTITIONS];
	buffer_type sizes05[NUM_PARTITIONS];
	buffer_type sizes06[NUM_PARTITIONS];
	buffer_type sizes07[NUM_PARTITIONS];
	buffer_type sizes10[NUM_PARTITIONS];
	buffer_type sizes11[NUM_PARTITIONS];
	buffer_type sizes12[NUM_PARTITIONS];
	buffer_type sizes13[NUM_PARTITIONS];
	buffer_type sizes14[NUM_PARTITIONS];
	buffer_type sizes15[NUM_PARTITIONS];
	buffer_type sizes16[NUM_PARTITIONS];
	buffer_type sizes17[NUM_PARTITIONS];
	buffer_type sizes20[NUM_PARTITIONS];
	buffer_type sizes21[NUM_PARTITIONS];
	buffer_type sizes22[NUM_PARTITIONS];
	buffer_type sizes23[NUM_PARTITIONS];
	buffer_type sizes24[NUM_PARTITIONS];
	buffer_type sizes25[NUM_PARTITIONS];
	buffer_type sizes26[NUM_PARTITIONS];
	buffer_type sizes27[NUM_PARTITIONS];
	buffer_type sizes30[NUM_PARTITIONS];
	buffer_type sizes31[NUM_PARTITIONS];
	buffer_type sizes32[NUM_PARTITIONS];
	buffer_type sizes33[NUM_PARTITIONS];
	buffer_type sizes34[NUM_PARTITIONS];
	buffer_type sizes35[NUM_PARTITIONS];
	buffer_type sizes36[NUM_PARTITIONS];
	buffer_type sizes37[NUM_PARTITIONS];
	
	COLLECTSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
 
		sizes00[p] = 0; 
 
		sizes01[p] = 0; 
 
		sizes02[p] = 0; 
 
		sizes03[p] = 0; 
 
		sizes04[p] = 0; 
 
		sizes05[p] = 0; 
 
		sizes06[p] = 0; 
 
		sizes07[p] = 0; 
 
		sizes10[p] = 0; 
 
		sizes11[p] = 0; 
 
		sizes12[p] = 0; 
 
		sizes13[p] = 0; 
 
		sizes14[p] = 0; 
 
		sizes15[p] = 0; 
 
		sizes16[p] = 0; 
 
		sizes17[p] = 0; 
 
		sizes20[p] = 0; 
 
		sizes21[p] = 0; 
 
		sizes22[p] = 0; 
 
		sizes23[p] = 0; 
 
		sizes24[p] = 0; 
 
		sizes25[p] = 0; 
 
		sizes26[p] = 0; 
 
		sizes27[p] = 0; 
 
		sizes30[p] = 0; 
 
		sizes31[p] = 0; 
 
		sizes32[p] = 0; 
 
		sizes33[p] = 0; 
 
		sizes34[p] = 0; 
 
		sizes35[p] = 0; 
 
		sizes36[p] = 0; 
 
		sizes37[p] = 0; 
 
	}
	COLLECTSTATS0_LOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=2
		keyvalue_t keyvalue00;
		keyvalue_t keyvalue01;
		keyvalue_t keyvalue02;
		keyvalue_t keyvalue03;
		keyvalue_t keyvalue04;
		keyvalue_t keyvalue05;
		keyvalue_t keyvalue06;
		keyvalue_t keyvalue07;
		keyvalue_t keyvalue10;
		keyvalue_t keyvalue11;
		keyvalue_t keyvalue12;
		keyvalue_t keyvalue13;
		keyvalue_t keyvalue14;
		keyvalue_t keyvalue15;
		keyvalue_t keyvalue16;
		keyvalue_t keyvalue17;
		keyvalue_t keyvalue20;
		keyvalue_t keyvalue21;
		keyvalue_t keyvalue22;
		keyvalue_t keyvalue23;
		keyvalue_t keyvalue24;
		keyvalue_t keyvalue25;
		keyvalue_t keyvalue26;
		keyvalue_t keyvalue27;
		keyvalue_t keyvalue30;
		keyvalue_t keyvalue31;
		keyvalue_t keyvalue32;
		keyvalue_t keyvalue33;
		keyvalue_t keyvalue34;
		keyvalue_t keyvalue35;
		keyvalue_t keyvalue36;
		keyvalue_t keyvalue37;
		
		#ifdef _WIDEWORD
		keyvalue00.key = sourcebuffer0[i].range(32 * ((0 * 2) + 1) - 1, (0 * 2) * 32);
		keyvalue00.value = sourcebuffer0[i].range(32 * (((0 * 2) + 1) + 1) - 1, (0 * 2 + 1) * 32);
		keyvalue01.key = sourcebuffer0[i].range(32 * ((1 * 2) + 1) - 1, (1 * 2) * 32);
		keyvalue01.value = sourcebuffer0[i].range(32 * (((1 * 2) + 1) + 1) - 1, (1 * 2 + 1) * 32);
		keyvalue02.key = sourcebuffer0[i].range(32 * ((2 * 2) + 1) - 1, (2 * 2) * 32);
		keyvalue02.value = sourcebuffer0[i].range(32 * (((2 * 2) + 1) + 1) - 1, (2 * 2 + 1) * 32);
		keyvalue03.key = sourcebuffer0[i].range(32 * ((3 * 2) + 1) - 1, (3 * 2) * 32);
		keyvalue03.value = sourcebuffer0[i].range(32 * (((3 * 2) + 1) + 1) - 1, (3 * 2 + 1) * 32);
		keyvalue04.key = sourcebuffer0[i].range(32 * ((4 * 2) + 1) - 1, (4 * 2) * 32);
		keyvalue04.value = sourcebuffer0[i].range(32 * (((4 * 2) + 1) + 1) - 1, (4 * 2 + 1) * 32);
		keyvalue05.key = sourcebuffer0[i].range(32 * ((5 * 2) + 1) - 1, (5 * 2) * 32);
		keyvalue05.value = sourcebuffer0[i].range(32 * (((5 * 2) + 1) + 1) - 1, (5 * 2 + 1) * 32);
		keyvalue06.key = sourcebuffer0[i].range(32 * ((6 * 2) + 1) - 1, (6 * 2) * 32);
		keyvalue06.value = sourcebuffer0[i].range(32 * (((6 * 2) + 1) + 1) - 1, (6 * 2 + 1) * 32);
		keyvalue07.key = sourcebuffer0[i].range(32 * ((7 * 2) + 1) - 1, (7 * 2) * 32);
		keyvalue07.value = sourcebuffer0[i].range(32 * (((7 * 2) + 1) + 1) - 1, (7 * 2 + 1) * 32);
		keyvalue10.key = sourcebuffer1[i].range(32 * ((0 * 2) + 1) - 1, (0 * 2) * 32);
		keyvalue10.value = sourcebuffer1[i].range(32 * (((0 * 2) + 1) + 1) - 1, (0 * 2 + 1) * 32);
		keyvalue11.key = sourcebuffer1[i].range(32 * ((1 * 2) + 1) - 1, (1 * 2) * 32);
		keyvalue11.value = sourcebuffer1[i].range(32 * (((1 * 2) + 1) + 1) - 1, (1 * 2 + 1) * 32);
		keyvalue12.key = sourcebuffer1[i].range(32 * ((2 * 2) + 1) - 1, (2 * 2) * 32);
		keyvalue12.value = sourcebuffer1[i].range(32 * (((2 * 2) + 1) + 1) - 1, (2 * 2 + 1) * 32);
		keyvalue13.key = sourcebuffer1[i].range(32 * ((3 * 2) + 1) - 1, (3 * 2) * 32);
		keyvalue13.value = sourcebuffer1[i].range(32 * (((3 * 2) + 1) + 1) - 1, (3 * 2 + 1) * 32);
		keyvalue14.key = sourcebuffer1[i].range(32 * ((4 * 2) + 1) - 1, (4 * 2) * 32);
		keyvalue14.value = sourcebuffer1[i].range(32 * (((4 * 2) + 1) + 1) - 1, (4 * 2 + 1) * 32);
		keyvalue15.key = sourcebuffer1[i].range(32 * ((5 * 2) + 1) - 1, (5 * 2) * 32);
		keyvalue15.value = sourcebuffer1[i].range(32 * (((5 * 2) + 1) + 1) - 1, (5 * 2 + 1) * 32);
		keyvalue16.key = sourcebuffer1[i].range(32 * ((6 * 2) + 1) - 1, (6 * 2) * 32);
		keyvalue16.value = sourcebuffer1[i].range(32 * (((6 * 2) + 1) + 1) - 1, (6 * 2 + 1) * 32);
		keyvalue17.key = sourcebuffer1[i].range(32 * ((7 * 2) + 1) - 1, (7 * 2) * 32);
		keyvalue17.value = sourcebuffer1[i].range(32 * (((7 * 2) + 1) + 1) - 1, (7 * 2 + 1) * 32);
		keyvalue20.key = sourcebuffer2[i].range(32 * ((0 * 2) + 1) - 1, (0 * 2) * 32);
		keyvalue20.value = sourcebuffer2[i].range(32 * (((0 * 2) + 1) + 1) - 1, (0 * 2 + 1) * 32);
		keyvalue21.key = sourcebuffer2[i].range(32 * ((1 * 2) + 1) - 1, (1 * 2) * 32);
		keyvalue21.value = sourcebuffer2[i].range(32 * (((1 * 2) + 1) + 1) - 1, (1 * 2 + 1) * 32);
		keyvalue22.key = sourcebuffer2[i].range(32 * ((2 * 2) + 1) - 1, (2 * 2) * 32);
		keyvalue22.value = sourcebuffer2[i].range(32 * (((2 * 2) + 1) + 1) - 1, (2 * 2 + 1) * 32);
		keyvalue23.key = sourcebuffer2[i].range(32 * ((3 * 2) + 1) - 1, (3 * 2) * 32);
		keyvalue23.value = sourcebuffer2[i].range(32 * (((3 * 2) + 1) + 1) - 1, (3 * 2 + 1) * 32);
		keyvalue24.key = sourcebuffer2[i].range(32 * ((4 * 2) + 1) - 1, (4 * 2) * 32);
		keyvalue24.value = sourcebuffer2[i].range(32 * (((4 * 2) + 1) + 1) - 1, (4 * 2 + 1) * 32);
		keyvalue25.key = sourcebuffer2[i].range(32 * ((5 * 2) + 1) - 1, (5 * 2) * 32);
		keyvalue25.value = sourcebuffer2[i].range(32 * (((5 * 2) + 1) + 1) - 1, (5 * 2 + 1) * 32);
		keyvalue26.key = sourcebuffer2[i].range(32 * ((6 * 2) + 1) - 1, (6 * 2) * 32);
		keyvalue26.value = sourcebuffer2[i].range(32 * (((6 * 2) + 1) + 1) - 1, (6 * 2 + 1) * 32);
		keyvalue27.key = sourcebuffer2[i].range(32 * ((7 * 2) + 1) - 1, (7 * 2) * 32);
		keyvalue27.value = sourcebuffer2[i].range(32 * (((7 * 2) + 1) + 1) - 1, (7 * 2 + 1) * 32);
		keyvalue30.key = sourcebuffer3[i].range(32 * ((0 * 2) + 1) - 1, (0 * 2) * 32);
		keyvalue30.value = sourcebuffer3[i].range(32 * (((0 * 2) + 1) + 1) - 1, (0 * 2 + 1) * 32);
		keyvalue31.key = sourcebuffer3[i].range(32 * ((1 * 2) + 1) - 1, (1 * 2) * 32);
		keyvalue31.value = sourcebuffer3[i].range(32 * (((1 * 2) + 1) + 1) - 1, (1 * 2 + 1) * 32);
		keyvalue32.key = sourcebuffer3[i].range(32 * ((2 * 2) + 1) - 1, (2 * 2) * 32);
		keyvalue32.value = sourcebuffer3[i].range(32 * (((2 * 2) + 1) + 1) - 1, (2 * 2 + 1) * 32);
		keyvalue33.key = sourcebuffer3[i].range(32 * ((3 * 2) + 1) - 1, (3 * 2) * 32);
		keyvalue33.value = sourcebuffer3[i].range(32 * (((3 * 2) + 1) + 1) - 1, (3 * 2 + 1) * 32);
		keyvalue34.key = sourcebuffer3[i].range(32 * ((4 * 2) + 1) - 1, (4 * 2) * 32);
		keyvalue34.value = sourcebuffer3[i].range(32 * (((4 * 2) + 1) + 1) - 1, (4 * 2 + 1) * 32);
		keyvalue35.key = sourcebuffer3[i].range(32 * ((5 * 2) + 1) - 1, (5 * 2) * 32);
		keyvalue35.value = sourcebuffer3[i].range(32 * (((5 * 2) + 1) + 1) - 1, (5 * 2 + 1) * 32);
		keyvalue36.key = sourcebuffer3[i].range(32 * ((6 * 2) + 1) - 1, (6 * 2) * 32);
		keyvalue36.value = sourcebuffer3[i].range(32 * (((6 * 2) + 1) + 1) - 1, (6 * 2 + 1) * 32);
		keyvalue37.key = sourcebuffer3[i].range(32 * ((7 * 2) + 1) - 1, (7 * 2) * 32);
		keyvalue37.value = sourcebuffer3[i].range(32 * (((7 * 2) + 1) + 1) - 1, (7 * 2 + 1) * 32);
		#else
		keyvalue00 = sourcebuffer0[i].data[0];
		keyvalue01 = sourcebuffer0[i].data[1];
		keyvalue02 = sourcebuffer0[i].data[2];
		keyvalue03 = sourcebuffer0[i].data[3];
		keyvalue04 = sourcebuffer0[i].data[4];
		keyvalue05 = sourcebuffer0[i].data[5];
		keyvalue06 = sourcebuffer0[i].data[6];
		keyvalue07 = sourcebuffer0[i].data[7];
		keyvalue10 = sourcebuffer1[i].data[0];
		keyvalue11 = sourcebuffer1[i].data[1];
		keyvalue12 = sourcebuffer1[i].data[2];
		keyvalue13 = sourcebuffer1[i].data[3];
		keyvalue14 = sourcebuffer1[i].data[4];
		keyvalue15 = sourcebuffer1[i].data[5];
		keyvalue16 = sourcebuffer1[i].data[6];
		keyvalue17 = sourcebuffer1[i].data[7];
		keyvalue20 = sourcebuffer2[i].data[0];
		keyvalue21 = sourcebuffer2[i].data[1];
		keyvalue22 = sourcebuffer2[i].data[2];
		keyvalue23 = sourcebuffer2[i].data[3];
		keyvalue24 = sourcebuffer2[i].data[4];
		keyvalue25 = sourcebuffer2[i].data[5];
		keyvalue26 = sourcebuffer2[i].data[6];
		keyvalue27 = sourcebuffer2[i].data[7];
		keyvalue30 = sourcebuffer3[i].data[0];
		keyvalue31 = sourcebuffer3[i].data[1];
		keyvalue32 = sourcebuffer3[i].data[2];
		keyvalue33 = sourcebuffer3[i].data[3];
		keyvalue34 = sourcebuffer3[i].data[4];
		keyvalue35 = sourcebuffer3[i].data[5];
		keyvalue36 = sourcebuffer3[i].data[6];
		keyvalue37 = sourcebuffer3[i].data[7];
		#endif
		
		partition_type p00 = getpartition(keyvalue00, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p01 = getpartition(keyvalue01, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p02 = getpartition(keyvalue02, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p03 = getpartition(keyvalue03, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p04 = getpartition(keyvalue04, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p05 = getpartition(keyvalue05, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p06 = getpartition(keyvalue06, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p07 = getpartition(keyvalue07, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p10 = getpartition(keyvalue10, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p11 = getpartition(keyvalue11, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p12 = getpartition(keyvalue12, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p13 = getpartition(keyvalue13, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p14 = getpartition(keyvalue14, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p15 = getpartition(keyvalue15, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p16 = getpartition(keyvalue16, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p17 = getpartition(keyvalue17, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p20 = getpartition(keyvalue20, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p21 = getpartition(keyvalue21, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p22 = getpartition(keyvalue22, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p23 = getpartition(keyvalue23, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p24 = getpartition(keyvalue24, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p25 = getpartition(keyvalue25, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p26 = getpartition(keyvalue26, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p27 = getpartition(keyvalue27, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p30 = getpartition(keyvalue30, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p31 = getpartition(keyvalue31, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p32 = getpartition(keyvalue32, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p33 = getpartition(keyvalue33, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p34 = getpartition(keyvalue34, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p35 = getpartition(keyvalue35, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p36 = getpartition(keyvalue36, llopparams.currentLOP, llopparams.upperlimit);
		partition_type p37 = getpartition(keyvalue37, llopparams.currentLOP, llopparams.upperlimit);
		
		if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes00[p00] += 1; }
		if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes01[p01] += 1; }
		if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes02[p02] += 1; }
		if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes03[p03] += 1; }
		if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes04[p04] += 1; }
		if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes05[p05] += 1; }
		if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes06[p06] += 1; }
		if(WithinValidRange(i, chunk0_size * VECTOR_SIZE) == 1){ sizes07[p07] += 1; }
		if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes10[p10] += 1; }
		if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes11[p11] += 1; }
		if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes12[p12] += 1; }
		if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes13[p13] += 1; }
		if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes14[p14] += 1; }
		if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes15[p15] += 1; }
		if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes16[p16] += 1; }
		if(WithinValidRange(i, chunk1_size * VECTOR_SIZE) == 1){ sizes17[p17] += 1; }
		if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes20[p20] += 1; }
		if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes21[p21] += 1; }
		if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes22[p22] += 1; }
		if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes23[p23] += 1; }
		if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes24[p24] += 1; }
		if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes25[p25] += 1; }
		if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes26[p26] += 1; }
		if(WithinValidRange(i, chunk2_size * VECTOR_SIZE) == 1){ sizes27[p27] += 1; }
		if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes30[p30] += 1; }
		if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes31[p31] += 1; }
		if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes32[p32] += 1; }
		if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes33[p33] += 1; }
		if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes34[p34] += 1; }
		if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes35[p35] += 1; }
		if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes36[p36] += 1; }
		if(WithinValidRange(i, chunk3_size * VECTOR_SIZE) == 1){ sizes37[p37] += 1; }
		
		#ifdef _DEBUGMODE_STATS
		globalvar_collectstats_counttotalkvsread(NUMSUBWORKERS * VECTOR_SIZE);
		#endif
	}
	COLLECTSTATS0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		capsule0[p].key = 0;
		capsule0[p].value += ( sizes00[p] + sizes01[p] + sizes02[p] + sizes03[p] + sizes04[p] + sizes05[p] + sizes06[p] + sizes07[p] );
		capsule1[p].key = 0;
		capsule1[p].value += ( sizes10[p] + sizes11[p] + sizes12[p] + sizes13[p] + sizes14[p] + sizes15[p] + sizes16[p] + sizes17[p] );
		capsule2[p].key = 0;
		capsule2[p].value += ( sizes20[p] + sizes21[p] + sizes22[p] + sizes23[p] + sizes24[p] + sizes25[p] + sizes26[p] + sizes27[p] );
		capsule3[p].key = 0;
		capsule3[p].value += ( sizes30[p] + sizes31[p] + sizes32[p] + sizes33[p] + sizes34[p] + sizes35[p] + sizes36[p] + sizes37[p] );
	}
	#ifdef _DEBUGMODE_CHECKSM
	debuggerA(Debugger_Sizes1, sizes0, 0);
	#endif 
	return;
}

void acts::partitionkeyvalues00(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE], uint512_dt sourcebuffer1[SRCBUFFER_SIZE], uint512_dt sourcebuffer2[SRCBUFFER_SIZE], uint512_dt sourcebuffer3[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	if(enable == 0){ return; }
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
		checkoutofbounds("acts::partitionkeyvalues0 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, NAp);
		checkoutofbounds("acts::partitionkeyvalues0 34", sizes1[p1].key + sizes1[p1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes1[p1].key, sizes1[p1].value, NAp);
		checkoutofbounds("acts::partitionkeyvalues0 34", sizes2[p2].key + sizes2[p2].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes2[p2].key, sizes2[p2].value, NAp);
		checkoutofbounds("acts::partitionkeyvalues0 34", sizes3[p3].key + sizes3[p3].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes3[p3].key, sizes3[p3].value, NAp);
		#endif
		
		setkeyvalue(destbuffer0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer1, sizes1[p1].key + sizes1[p1].value, keyvalue1, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer2, sizes2[p2].key + sizes2[p2].value, keyvalue2, PADDEDDESTBUFFER_SIZE);
		setkeyvalue(destbuffer3, sizes3[p3].key + sizes3[p3].value, keyvalue3, PADDEDDESTBUFFER_SIZE);
		
		if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ sizes0[p0].value += 1; }
		if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ sizes1[p1].value += 1; }
		if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ sizes2[p2].value += 1; }
		if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ sizes3[p3].value += 1; }
		
		#ifdef _DEBUGMODE_STATS
		if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
		if(WithinValidRange(k, chunk1_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
		if(WithinValidRange(k, chunk2_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
		if(WithinValidRange(k, chunk3_size * VECTOR_SIZE) == 1){ globalstats_countkvspartitioned(1); }
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  capsule1[p].value = sizes1[p].value;  capsule2[p].value = sizes2[p].value;  capsule3[p].value = sizes3[p].value;  }
	#ifdef _DEBUGMODE_CHECKSM
	debuggerA(Debugger_Sizes2, sizes0, 1);
	#endif 
}
void acts::reducepartitions0(unsigned int enable, unsigned int workerID  ,uint512_dt sourcebuffer0[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer1[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer2[SRCBUFFER_SIZE]  ,uint512_dt sourcebuffer3[SRCBUFFER_SIZE]   ,uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE]  ,uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE]  ,globalparams_t globalparams){
	if(enable == 0){ return; }
	unsigned int analysis_buffersz = SRCBUFFER_SIZE * VECTOR_SIZE;
	
	REDUCE_MAINLOOP: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
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

void acts::savekeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	int analysis_savebuffer = PADDEDDESTBUFFER_SIZE;
	
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &size_kvs, 0);
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &offset_kvs, 0);
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("acts::savekeyvalues dram 35", baseaddress + offset_kvs + size_kvs, maxaddress_kvs, NAp, NAp, NAp);
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
travoffsets_t acts::savecapsules0(unsigned int workerID, uint512_dt * kvdram , uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule1[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule2[CAPSULEBUFFER_SIZE], uint512_dt BIGcapsule3[CAPSULEBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type baseaddress_kvs, batch_type offset_kvs, travstate_t travstate, travoffsets_t travoffsets){							
	buffer_type loffset = offset_kvs % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	// printkeyvalues("savecapsules", capsule0, NUM_PARTITIONS);
	// getvaluecount("savecapsules", capsule0, NUM_PARTITIONS);
	// exit(EXIT_SUCCESS);
	
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
		checkoutofbounds("acts::savecapsules0 34", baseaddress_kvs + saveoffset0_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule0, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalvar_savestats_counttotalstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset1_kvs = travoffsets.capsuleoffset_kvs + (1 * CAPSULEBUFFER_SIZE);
		
		checkandforce(baseaddress_kvs + saveoffset1_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, &saveoffset1_kvs, 0);
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("acts::savecapsules0 34", baseaddress_kvs + saveoffset1_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule1, baseaddress_kvs, saveoffset1_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalvar_savestats_counttotalstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset2_kvs = travoffsets.capsuleoffset_kvs + (2 * CAPSULEBUFFER_SIZE);
		
		checkandforce(baseaddress_kvs + saveoffset2_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, &saveoffset2_kvs, 0);
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("acts::savecapsules0 34", baseaddress_kvs + saveoffset2_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule2, baseaddress_kvs, saveoffset2_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalvar_savestats_counttotalstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		batch_type saveoffset3_kvs = travoffsets.capsuleoffset_kvs + (3 * CAPSULEBUFFER_SIZE);
		
		checkandforce(baseaddress_kvs + saveoffset3_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, &saveoffset3_kvs, 0);
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("acts::savecapsules0 34", baseaddress_kvs + saveoffset3_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule3, baseaddress_kvs, saveoffset3_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		globalvar_savestats_counttotalstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		travoffsets.capsuleoffset_kvs += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	return travoffsets;
}
void acts::savepartitions0(unsigned int workerID, uint512_dt * kvdram , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer1[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer2[PADDEDDESTBUFFER_SIZE], uint512_dt destbuffer3[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS] ,keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type kvdrambaseaddress, int enable){				
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
			checkoutofbounds("acts::savepartitions0 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("acts::savepartitions0 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule0[p].key, size_kvs);
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
			checkoutofbounds("acts::savepartitions1 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("acts::savepartitions1 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule1[p].key, size_kvs);
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
			checkoutofbounds("acts::savepartitions2 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("acts::savepartitions2 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule2[p].key, size_kvs);
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
			checkoutofbounds("acts::savepartitions3 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, size_kvs);
			checkoutofbounds("acts::savepartitions3 bram 35", bramoffset_kvs + size_kvs, PADDEDBUFFER_SIZE, bramoffset_kvs, capsule3[p].key, size_kvs);
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

void acts::resetcapsules0(keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS],  int enableresetdebugger){
	#pragma HLS INLINE 
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		capsule0[p].key = 0; capsule0[p].value = 0;
		capsule1[p].key = 0; capsule1[p].value = 0;
		capsule2[p].key = 0; capsule2[p].value = 0;
		capsule3[p].key = 0; capsule3[p].value = 0;
	}
	return;
}

void acts::partitionandreduce0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
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
	
	unsigned int enable_collectstats;
	unsigned int enable_partitionkeyvalues;
	unsigned int enable_reducekeyvalues;
	
	resetcapsules0(capsule00,capsule01,capsule02,capsule03, NAp);
	
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
		readkeyvalues0(kvsourcedram, sourcebuffer00[0], 0, 0 + i, chunk_size, PADDEDKVSOURCEDRAMSZ_KVS);
		savekeyvalues0(kvsourcedram, sourcebuffer00[0], 0, (((globalparams.IterCount % DRAMBATCHFACTOR) + 1) * KVDATA_BATCHSIZE_KVS) + i, chunk_size, PADDEDKVSOURCEDRAMSZ_KVS);
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
			checkoutofbounds("acts::partitionandreduce0 22", getstatsAddr(sourcestatsmarker), KVSTATSDRAMSZ, NAp, NAp, NAp);
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
				COLLECTSTATS_MAINLOOP1B: for(vector_type v=0; v<NFACTOR; v+=2){
					if(v==0){ enable_collectstats = 0; } else { enable_collectstats = 1; }
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[0]  ,sourcebuffer01[0]  ,sourcebuffer02[0]  ,sourcebuffer03[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs, PADDEDKVSOURCEDRAMSZ_KVS, travstate);
					collectstats00(enable_collectstats, 0  ,sourcebuffer00[1] ,sourcebuffer01[1] ,sourcebuffer02[1] ,sourcebuffer03[1]  ,capsule00 ,capsule01 ,capsule02 ,capsule03, llopparams, travstate);
					
					travstate.i_kvs = i + ((v + 1) * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					collectstats00(1, 0  ,sourcebuffer00[0] ,sourcebuffer01[0] ,sourcebuffer02[0] ,sourcebuffer03[0]  ,capsule00 ,capsule01 ,capsule02 ,capsule03, llopparams, travstate);
					readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[1]  ,sourcebuffer01[1]  ,sourcebuffer02[1]  ,sourcebuffer03[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs, PADDEDKVSOURCEDRAMSZ_KVS, travstate);
				}
				collectstats00(1, 0  ,sourcebuffer00[1] ,sourcebuffer01[1] ,sourcebuffer02[1] ,sourcebuffer03[1]  ,capsule00 ,capsule01 ,capsule02 ,capsule03, llopparams, travstate);
				travoffsets = savecapsules0(0, kvsourcedram , BIGcapsule00, BIGcapsule01, BIGcapsule02, BIGcapsule03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp, llopparams.baseaddr_capsule_kvs, capsuleoffset_kvs, travstate, travoffsets);
				resetcapsules0( capsule00, capsule01, capsule02, capsule03, NAp);
				#ifdef _DEBUGMODE_CHECKSM
				debugger_i += 1;
				#endif
			}
			resetcapsules0( capsule00, capsule01, capsule02, capsule03, NAp);
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
				PARTITIONPHASE_MAINLOOP1B: for(vector_type v = 0; v<NFACTOR; v+=2){
					if(v==0){ enable_partitionkeyvalues = 0; } else { enable_partitionkeyvalues = 1; }
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[0]  ,sourcebuffer01[0]  ,sourcebuffer02[0]  ,sourcebuffer03[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs, PADDEDKVSOURCEDRAMSZ_KVS, travstate);
					partitionkeyvalues00(enable_partitionkeyvalues, 0  ,sourcebuffer00[1] ,sourcebuffer01[1] ,sourcebuffer02[1] ,sourcebuffer03[1]  ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, travstate, llopparams);
					
					travstate.i_kvs = i + ((v + 1) * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					partitionkeyvalues00(1, 0  ,sourcebuffer00[0] ,sourcebuffer01[0] ,sourcebuffer02[0] ,sourcebuffer03[0]  ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, travstate, llopparams);
					readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[1]  ,sourcebuffer01[1]  ,sourcebuffer02[1]  ,sourcebuffer03[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate.i_kvs, PADDEDKVSOURCEDRAMSZ_KVS, travstate);
				}
				partitionkeyvalues00(1, 0  ,sourcebuffer00[1] ,sourcebuffer01[1] ,sourcebuffer02[1] ,sourcebuffer03[1]  ,destbuffer00 ,destbuffer01 ,destbuffer02 ,destbuffer03  ,capsule00 ,capsule01 ,capsule02 ,capsule03, travstate, llopparams);
				savepartitions0(0, kvsourcedram , destbuffer00, destbuffer01, destbuffer02, destbuffer03 , capsule00, capsule01, capsule02, capsule03, kvdeststats_tmp2, llopparams.baseaddr_workdestkvs_kvs, 1);
				resetcapsules0(capsule00,capsule01,capsule02,capsule03, NAp);
				#ifdef _DEBUGMODE_CHECKSM
				debugger_i += 1;
				#endif
			}
			#ifdef _DEBUGMODE_CHECKSM
			IsEqual(Debugger_Sizes1, Debugger_Sizes2, debugger_i, NUM_PARTITIONS);
			debugger_i = 0;
			#endif
			
			// Reduce phase
			// #ifdef XXX
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
			#ifdef _DEBUGMODE_CHECKS2
			checkoutofbounds("acts::partitionandreduce0 23", getstatsAddr(sourcestatsmarker + NUMSUBWORKERS), KVSTATSDRAMSZ, NAp, NAp, NAp);
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
			
			readkeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			readkeyvalues0(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			REDUCEPHASE_MAINLOOP: for (int i = 0; i < maxsz_kvs; i += 2 * SRCBUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reducephase avg=analysis_reducephase
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase0:: i", "i_end", "NAp", "NAp", "skip size", i, maxsz_kvs, NAp, NAp, SRCBUFFER_SIZE);
				#endif
				/** for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ travstates[sw].i_kvs = i; }
				
				buffer_type chunk0_size = SRCBUFFER_SIZE;
				getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstates[0], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer00[0], llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[0].key / VECTOR_SIZE) + i, chunk0_size, PADDEDKVSOURCEDRAMSZ_KVS);
				buffer_type chunk1_size = SRCBUFFER_SIZE;
				getchunksize(&chunk1_size, SRCBUFFER_SIZE, travstates[1], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer01[0], llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[1].key / VECTOR_SIZE) + i, chunk1_size, PADDEDKVSOURCEDRAMSZ_KVS);
				buffer_type chunk2_size = SRCBUFFER_SIZE;
				getchunksize(&chunk2_size, SRCBUFFER_SIZE, travstates[2], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer02[0], llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[2].key / VECTOR_SIZE) + i, chunk2_size, PADDEDKVSOURCEDRAMSZ_KVS);
				buffer_type chunk3_size = SRCBUFFER_SIZE;
				getchunksize(&chunk3_size, SRCBUFFER_SIZE, travstates[3], 0);
				readkeyvalues0(kvsourcedram, sourcebuffer03[0], llopparams.baseaddr_worksourcekvs_kvs, (sourcestats[3].key / VECTOR_SIZE) + i, chunk3_size, PADDEDKVSOURCEDRAMSZ_KVS);
				
				reducepartitions0(0,  sourcebuffer00[0],  sourcebuffer01[0],  sourcebuffer02[0],  sourcebuffer03[0],   destbuffer00,  destbuffer01,  destbuffer02,  destbuffer03,  travstate, globalparams); */
			
				if(i==0){ enable_reducekeyvalues = 0; } else { enable_reducekeyvalues = 1; }
				for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ travstates[sw].i_kvs = i; }
				readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[0]  ,sourcebuffer01[0]  ,sourcebuffer02[0]  ,sourcebuffer03[0]  ,llopparams.baseaddr_worksourcekvs_kvs, sourcestats, PADDEDKVSOURCEDRAMSZ_KVS, travstates);
				reducepartitions0(enable_reducekeyvalues, 0  ,sourcebuffer00[1]  ,sourcebuffer01[1]  ,sourcebuffer02[1]  ,sourcebuffer03[1]   ,destbuffer00  ,destbuffer01  ,destbuffer02  ,destbuffer03  ,globalparams);
			
				for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ travstates[sw].i_kvs = i + SRCBUFFER_SIZE; }
				reducepartitions0(1, 0  ,sourcebuffer00[0]  ,sourcebuffer01[0]  ,sourcebuffer02[0]  ,sourcebuffer03[0]   ,destbuffer00  ,destbuffer01  ,destbuffer02  ,destbuffer03  ,globalparams);
				readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[1]  ,sourcebuffer01[1]  ,sourcebuffer02[1]  ,sourcebuffer03[1]  ,llopparams.baseaddr_worksourcekvs_kvs, sourcestats, PADDEDKVSOURCEDRAMSZ_KVS, travstates);
				
			}
			/// reducepartitions0(1, 0  ,sourcebuffer00[1]  ,sourcebuffer01[1]  ,sourcebuffer02[1]  ,sourcebuffer03[1]   ,destbuffer00  ,destbuffer01  ,destbuffer02  ,destbuffer03  ,globalparams);
			savekeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdram, destbuffer01, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (1 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdram, destbuffer02, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (2 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			savekeyvalues0(kvdestdram, destbuffer03, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (3 * PADDEDDESTBUFFER_SIZE)), PADDEDDESTBUFFER_SIZE, KVDATA_RANGE_PERSSDPARTITION);
			}
			// #endif 
			
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
void acts::topkernel(
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





