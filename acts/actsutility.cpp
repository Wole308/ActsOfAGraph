#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <string>
#include <iostream>
#include <string.h>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "../src/utility/utility.h"
#include "../include/common.h"
#include "actsutility.h"
using namespace std;

actsutility::actsutility(){
	utilityobj = new utility();
}
actsutility::~actsutility(){} 

void actsutility::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data >= upper_bound){ std::cout<<"acts::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }
}
void actsutility::checkforequal(string message, unsigned int data1, unsigned int data2){
	if(data1 == data2){ cout<<"acts::checkforequal: ERROR. data1 == data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
}
void actsutility::print1(string messagea, unsigned int dataa){
	cout<<messagea<<": "<<dataa<<endl;
}
void actsutility::print2(string messagea, string messageb, unsigned int dataa, unsigned int datab){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<endl;
}
void actsutility::print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<endl;
}
void actsutility::print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<endl;
}
void actsutility::print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<", "<<messagef<<": "<<datef<<endl;
}
void actsutility::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; }
}
void actsutility::printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"["<<keyvalues1[p].key<<":"<<keyvalues1[p].value<<"]["<<keyvalues2[p].key<<":"<<keyvalues2[p].value<<"]"<<endl; }
}
void actsutility::printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int i=0; i<size_kvs; i++){ 
		#ifdef _WIDEWORD
		{%for v in context['VECTOR_SIZE_seq']%}
			cout<<"keyvalues["<<i<<"].data[{{v}}].key: "<<(unsigned int)keyvalues[i].range({{32 * ((v * 2) + 1) - 1}}, {{(v * 2) * 32}})<<", keyvalues["<<i<<"].data[{{v}}].value: "<<(unsigned int)keyvalues[i].range({{32 * (((v * 2) + 1) + 1) - 1}}, {{(v * 2 + 1) * 32}})<<endl;
		{%endfor%}
		#else 
		for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"keyvalues["<<i<<"].data["<<v<<"].key: "<<keyvalues[i].data[v].key<<", keyvalues["<<i<<"].data["<<v<<"].value: "<<keyvalues[i].data[v].value<<endl; }
		#endif 
	}
}
void actsutility::printvaluecount(string message, keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	cout<<"total values counted: "<<totalnumkeyvalues<<endl;
	return;
}
unsigned int actsutility::getvaluecountexcept(keyvalue_t * keyvalues, unsigned int size, unsigned int exceptvalue){
	cout<<"getvaluecountexcept: size: "<<size<<", exceptvalue: "<<exceptvalue<<endl;
	unsigned int totalnumkeyvalues = 0; 
	unsigned int totalnumNOTkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ if(keyvalues[p].key != exceptvalue){ totalnumkeyvalues += 1; } else { totalnumNOTkeyvalues += 1; }}
	cout<<"total values counted: "<<totalnumkeyvalues<<", totalnumNOTkeyvalues: "<<totalnumNOTkeyvalues<<endl;
	return totalnumkeyvalues;
}
void actsutility::setkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, keyvalue_t keyvalue){
	cout<<"setting keyvalues... "<<endl;
	for(unsigned int i=0; i<size; i++){ keyvalues[i] = keyvalue; }
}
void actsutility::printparameters(){
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
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE * VECTOR_SIZE: "<<PADDEDDESTBUFFER_SIZE * VECTOR_SIZE<<endl;
	cout<<"acts::printparameters:: PADDEDBUFFER_SIZE / NUM_PARTITIONS: "<<PADDEDBUFFER_SIZE / NUM_PARTITIONS<<endl;
	cout<<"acts::printparameters:: NFACTOR: "<<NFACTOR<<endl;
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE * NUMSUBWORKERS (total dest_kvs memory): "<<(PADDEDDESTBUFFER_SIZE * NUMSUBWORKERS)<<endl;
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * NUMSUBWORKERS (total dest memory): "<<(PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * NUMSUBWORKERS)<<endl;
	cout<<"acts::printparameters:: APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ<<endl;
	cout<<"acts::printparameters:: APPLYVERTEXBUFFERSZ_KVS: "<<APPLYVERTEXBUFFERSZ_KVS<<endl;
	// cout<<"acts::printparameters:: CS_NUM_READ_PIPELINES: "<<CS_NUM_READ_PIPELINES<<endl;
	// cout<<"acts::printparameters:: PVU_NUM_READ_PIPELINES: "<<PVU_NUM_READ_PIPELINES<<endl;
	// cout<<"acts::printparameters:: PADSKIP: "<<PADSKIP<<endl;
}
void actsutility::printglobalvars(){
	cout<<"printglobalvars: "<<endl;
	cout<<"acts::printglobalvars:: globalvar_totalkvsread: "<<globalvar_totalkvsread<<endl;
	cout<<"acts::printglobalvars:: globalvar_totalkvsreadV: "<<globalvar_totalkvsreadV<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvswritten: "<<globalstats_totalkvswritten<<endl;
	cout<<"acts::printglobalvars:: globalvar_totalkvstatsread: "<<globalvar_totalkvstatsread<<endl;
	cout<<"acts::printglobalvars:: globalvar_collectstats_totalkvsread: "<<globalvar_collectstats_totalkvsread<<endl;
	cout<<"acts::printglobalvars:: globalvar_extractcapsules_totalkvsread: "<<globalvar_extractcapsules_totalkvsread<<endl;
	cout<<"acts::printglobalvars:: globalvar_organizekeyvalues_totalkvsread: "<<globalvar_organizekeyvalues_totalkvsread<<endl;
	cout<<"acts::printglobalvars:: globalvar_savestats_totalstatswritten: "<<globalvar_savestats_totalstatswritten<<endl;	
	cout<<"acts::printglobalvars:: globalvar_savepartitions_invalids: "<<globalvar_savepartitions_invalids<<endl;	
	cout<<"acts::printglobalvars:: globalvar_inmemory_totalvalidkeyvalues: "<<globalvar_inmemory_totalvalidkeyvalues<<endl;	
	cout<<"acts::printglobalvars:: globalstats_totalkvspartitioned: "<<globalstats_totalkvspartitioned<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvspartitionswritten: "<<globalstats_totalkvspartitionswritten<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvspartitionswritten_actual: "<<globalstats_totalkvspartitionswritten_actual<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvspartitionswritten (valids): "<<globalstats_totalkvspartitionswritten - globalvar_savepartitions_invalids<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvsreduced: "<<globalstats_totalkvsreduced<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvsreducewritten: "<<globalstats_totalkvsreducewritten<<endl;
	cout<<"acts::printglobalvars:: globalstats_reduce_validkvsreduced (valids): "<<globalstats_reduce_validkvsreduced<<endl;
}
void actsutility::clearglobalvars(){
	cout<<"clearglobalvars: "<<endl;
	globalvar_totalkvsread = 0;
	globalstats_totalkvswritten = 0;
	globalstats_totalkvspartitionswritten = 0;
	globalstats_totalkvspartitionswritten_actual = 0;
	globalstats_totalkvspartitioned = 0;
	globalvar_totalkvstatsread = 0;
	globalvar_collectstats_totalkvsread = 0;
	globalvar_extractcapsules_totalkvsread = 0;
	globalvar_organizekeyvalues_totalkvsread = 0;
	globalvar_savestats_totalstatswritten = 0;
	globalvar_savepartitions_invalids = 0;
	globalvar_inmemory_totalvalidkeyvalues = 0;
	globalstats_totalkvsreduced = 0;
	globalstats_totalkvsreducewritten = 0;
	globalstats_reduce_validkvsreduced = 0;
	globalvar_totalkvsreadV = 0;
}
void actsutility::IsEqual(keyvalue_t ** data1, keyvalue_t ** data2, unsigned int _1stdimsize, unsigned int _2nddimsize){
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

void actsutility::globalstats_countkvstatsread(unsigned int count){
	globalvar_totalkvstatsread += count;
	return;
}
void actsutility::globalvar_collectstats_counttotalkvsread(unsigned int count){
	globalvar_collectstats_totalkvsread += count;
	return;
}
void actsutility::globalvar_extractcapsules_counttotalkvsread(unsigned int count){
	globalvar_extractcapsules_totalkvsread += count;
	return;
}
void actsutility::globalvar_organizekeyvalues_counttotalkvsread(unsigned int count){
	globalvar_organizekeyvalues_totalkvsread += count;
	return;
}
void actsutility::globalvar_savestats_counttotalstatswritten(unsigned int count){
	globalvar_savestats_totalstatswritten += count;
	return;
}
void actsutility::globalvar_savepartitions_countinvalids(unsigned int count){
	globalvar_savepartitions_invalids += count;
	return;
}
void actsutility::globalvar_inmemory_counttotalvalidkeyvalues(unsigned int count){
	globalvar_inmemory_totalvalidkeyvalues += count;
	return;
}
void actsutility::globalstats_countkvsread(unsigned int count){
	globalvar_totalkvsread += count;
	return;
}
void actsutility::globalstats_countkvswritten(unsigned int count){
	globalstats_totalkvswritten += count;
	return;
}
void actsutility::globalstats_countkvspartitionswritten(unsigned int count){
	globalstats_totalkvspartitionswritten += count;
	return;
}
void actsutility::globalstats_countkvspartitionswritten_actual(unsigned int count){
	globalstats_totalkvspartitionswritten_actual += count;
	return;
}
void actsutility::globalstats_countkvspartitioned(unsigned int count){
	globalstats_totalkvspartitioned += count;
	return;
}
void actsutility::globalstats_countkvsreduced(unsigned int count){
	globalstats_totalkvsreduced += count;
	return;
}
void actsutility::globalstats_countkvsreducewritten(unsigned int count){
	globalstats_totalkvsreducewritten += count;
	return;
}
void actsutility::globalstats_reduce_countvalidkvsreduced(unsigned int count){
	globalstats_reduce_validkvsreduced += count;
	return;
}
void actsutility::globalstats_countkvsreadV(unsigned int count){
	globalvar_totalkvsreadV += count;
	return;
}













