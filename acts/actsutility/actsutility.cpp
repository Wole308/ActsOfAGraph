#include "../../include/config_params.h"
#ifdef SW
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
#endif 
#ifdef SW
#include "../../src/utility/utility.h"
#endif 
#include "../include/actscommon.h"
#include "../../include/common.h"
#include "actsutility.h"
using namespace std;

actsutility::actsutility(){
	#ifdef SW
	utilityobj = new utility();
	#endif 
	for(unsigned int i=0; i<8; i++){ for(unsigned int k=0; k<MYSTATSYSIZE; k++){ for(unsigned int p=0; p<NUM_PARTITIONS; p++){ mystats[i][k][p] = 0; }}}
	for(unsigned int i=0; i<8; i++){ for(unsigned int p=0; p<NUM_PARTITIONS; p++){ mykeyvalues[i][p].key = 0; mykeyvalues[i][p].value = 0; }}
	mincutoffseen = INFINITI;
	maxcutoffseen = 0;
}
actsutility::~actsutility(){}

void actsutility::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data >= upper_bound){ std::cout<<"acts::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }
}
void actsutility::checkoutofbounds(unsigned int enable, string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(enable == OFF){ return; }
	if(data >= upper_bound){ std::cout<<"acts::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }
}
void actsutility::checkforequal(string message, unsigned int data1, unsigned int data2, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data1 != data2){ cout<<"acts::checkforequal: ERROR. data1 != data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<endl; exit(EXIT_FAILURE); }
}
void actsutility::checkforequal(string message, unsigned int * data1, unsigned int * data2, unsigned int size, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	for(unsigned int i=0; i<size; i++){
		if(data1[i] != data2[i]){ cout<<"acts::checkarraysforequal: ERROR. data1["<<i<<"]("<<data1[i]<<") != data2["<<i<<"]("<<data2[i]<<"). message: "<<message<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<endl; exit(EXIT_FAILURE); }
	}
}
void actsutility::hcheckforequal(unsigned int enable, string message, unsigned int * data1, unsigned int * data2, unsigned int size, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(enable == OFF){ return; } 
	#ifdef _DEBUGMODE_KERNELPRINTS
	printvalues("actsutility::hcheckforequal. data1", data1, NUM_PARTITIONS);
	printvalues("actsutility::hcheckforequal. data2", data2, NUM_PARTITIONS);
	#endif
	if(countvalues("", data1, size) == 0 || countvalues("", data2, size) == 0){ return; } // cout<<"_______________total count=0 seen. returning without counting..."<<endl; return; }
	
	for(unsigned int i=0; i<size; i++){
		if(data1[i] != data2[i]){ cout<<"acts::hcheckforequal: ERROR. data1["<<i<<"]("<<data1[i]<<") != data2["<<i<<"]("<<data2[i]<<"). message: "<<message<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<". printing both arrays and exiting..."<<endl; 
			printvalues("hcheckforequal:data1", data1, size);
			printvalues("hcheckforequal:data2", data2, size);
			exit(EXIT_FAILURE); }
	}
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	// cout<<"actsutility::hcheckforequal: checks for equal successful. "<<endl;
	cout<<"actsutility::hcheckforequal: checks for equal successful. message: "<<message<<endl;
	#endif 
}
void actsutility::checkfornotequal(string message, unsigned int data1, unsigned int data2, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data1 == data2){ cout<<"acts::checkfornotequal: ERROR. data1 == data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<endl; exit(EXIT_FAILURE); }
}
void actsutility::checkfornotequalbyerrorwindow(string message, unsigned int data1, unsigned int data2, unsigned int errorwindow){
	if(data2 - data1 > errorwindow){ cout<<"acts::checkfornotequalbyerrorwindow: ERROR. data1 != data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<", errorwindow: "<<errorwindow<<endl; exit(EXIT_FAILURE); }
}
void actsutility::checkforoverlap(string message, keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int i=0; i<size-1; i++){
		if(keyvalues[i].key + keyvalues[i].value >= keyvalues[i+1].key){ 
			cout<<"aactsutility::checkforoverlap: ERROR. overlap found message: "<<message<<", i: "<<i<<", keyvalues["<<i<<"].key: "<<keyvalues[i].key<<", keyvalues["<<i<<"].value: "<<keyvalues[i].value<<", keyvalues["<<i+1<<"].key: "<<keyvalues[i+1].key<<". printing keyvalues and exiting..."<<endl; 
			printkeyvalues("actsutility::checkforoverlap", keyvalues, size);
			exit(EXIT_FAILURE);
		}								
	}
	// exit(EXIT_SUCCESS);
	return;
}
void actsutility::checkforgreaterthan(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size){
	for(unsigned int i=0; i<size; i++){
		if(keyvalues1[i].value < keyvalues2[i].value){ 
			cout<<"ERROR: "<<message<<". keyvalues1["<<i<<"].value ("<<keyvalues1[i].value<<") < keyvalues2["<<i<<"].value ("<<keyvalues2[i].value<<"). printing both arrays before exiting..."<<endl; 
			printkeyvalues("checkforgreaterthan.keyvalues1", keyvalues1, size);
			printkeyvalues("checkforgreaterthan.keyvalues2", keyvalues2, size);
			exit(EXIT_FAILURE); 
		}
	}
	return;
}
void actsutility::checkforgreaterthan(string message, value_t * values1, value_t * values2, unsigned int size){
	for(unsigned int i=0; i<size; i++){
		if(values1[i] < values2[i]){ 
			cout<<"ERROR: "<<message<<". values1["<<i<<"] ("<<values1[i]<<") < values2["<<i<<"] ("<<values2[i]<<"). printing both arrays before exiting..."<<endl; 
			printvalues("checkforgreaterthan.values1", values1, size);
			printvalues("checkforgreaterthan.values2", values2, size);
			exit(EXIT_FAILURE); 
		}
	}
	return;
}
void actsutility::checkforequal(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size){
	for(unsigned int i=0; i<size; i++){
		if(keyvalues1[i].value != keyvalues2[i].value){ 
			cout<<"ERROR: "<<message<<". keyvalues1["<<i<<"].value ("<<keyvalues1[i].value<<") != keyvalues2["<<i<<"].value ("<<keyvalues2[i].value<<"). EXITING..."<<endl; 
			exit(EXIT_FAILURE); 
		}
	}
	return;
}
void actsutility::checkforlessthanthan(string message, unsigned int data1, unsigned int data2){
	if(data1 < data2){ cout<<"acts::checkforlessthanthan: ERROR. data1 < data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
}
void actsutility::checkforlessthanthan(string message, unsigned int data1, unsigned int data2, unsigned int tolerance){
	if((data1 + tolerance) < data2){ cout<<"acts::checkforlessthanthan: ERROR. (data1 + tolerance) < data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
}
void actsutility::checkfordivisibleby(string message, unsigned int data, unsigned int n){
	if((data % n) != 0){ cout<<"actsutility::checkfordivisibleby:: ERROR. "<<data<<" % "<<n<<" != 0. message:"<<message<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	return;
}
void actsutility::checkfordivisibleby(unsigned int enable, string message, unsigned int data, unsigned int n){
	if(enable == OFF){ return; }
	if((data % n) != 0){ cout<<"actsutility::checkfordivisibleby:: ERROR. "<<data<<" % "<<n<<" != 0. message:"<<message<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	return;
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
void actsutility::print7(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, string messageg, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int dataf, unsigned int datag){				
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<", "<<messagef<<": "<<dataf<<", "<<messageg<<": "<<datag<<endl;
}
void actsutility::print8(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, string messageg, string messageh, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int dataf, unsigned int datag, unsigned int datah){				
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<", "<<messagef<<": "<<dataf<<", "<<messageg<<": "<<datag<<", "<<messageh<<": "<<datah<<endl;
}
void actsutility::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	unsigned int totalkeys = 0;
	unsigned int totalvalues = 0;
	for(unsigned int p=0; p<size; p++){ 
		cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; 
		totalkeys += keyvalues[p].key;
		totalvalues += keyvalues[p].value;
	}
	cout<<"totalkeys: "<<totalkeys<<", totalvalues: "<<totalvalues<<", lastkey + lastvalue: "<<keyvalues[size-1].key + keyvalues[size-1].value<<endl; 
}
void actsutility::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int skipsize){
	if(skipsize == 0){ cout<<endl<<"printkeyvalues:ERROR: skipsize CANNOT be zero. exiting... "<<endl; exit(EXIT_FAILURE); }
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p+=skipsize){ cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; }
}
void actsutility::printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"["<<keyvalues1[p].key<<":"<<keyvalues1[p].value<<"]["<<keyvalues2[p].key<<":"<<keyvalues2[p].value<<"]"<<endl; }
}
void actsutility::printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int i=0; i<size_kvs; i++){ 
		#ifndef _WIDEWORD
		for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"keyvalues["<<i<<"].data["<<v<<"].key: "<<keyvalues[i].data[v].key<<", keyvalues["<<i<<"].data["<<v<<"].value: "<<keyvalues[i].data[v].value<<endl; }
		#endif 
	}
}
void actsutility::printkeyvalues(string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int size){
	cout<<endl<<"actsutility::printkeyvalues:"<<message<<endl;
	for(unsigned int v=0; v<VECTOR_SIZE; v++){
		for(unsigned int i=0; i<size; i++){ cout<<"keyvalues["<<v<<"]["<<i<<"].key: "<<keyvalues[v][i].key<<", keyvalues["<<v<<"]["<<i<<"].value: "<<keyvalues[v][i].value<<endl; }
		cout<<".."<<endl;
	}
}
void actsutility::printvaluecount(string message, keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	cout<<"total values counted: "<<totalnumkeyvalues<<endl;
	return;
}
void actsutility::printvalues(string message, unsigned int * values, unsigned int size){
	cout<<endl<<"printvalues: "<<message<<endl;
	unsigned int count = 0;
	for(unsigned int p=0; p<size; p++){ cout<<"values["<<p<<"]: "<<values[p]<<endl; count += values[p]; }
	cout<<"printvalues. count: "<<count<<endl;
}
void actsutility::printvalueslessthan(string message, value_t * values, unsigned int size, unsigned int data){
	cout<<endl<<"printvalueslessthan: "<<message<<endl;
	for(unsigned int i=0; i<size; i++){ if(values[i] < data){ cout<<"actsutility::printvalueslessthan: values["<<i<<"]: "<<values[i]<<" is less than "<<data<<endl; }}
	return;
}
void actsutility::printparameters(){
	cout<<endl<<"acts::printparameters: test started."<<endl;
	cout<<"acts::printparameters:: KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
	cout<<"acts::printparameters:: KVDATA_BATCHSIZE_KVS: "<<KVDATA_BATCHSIZE_KVS<<endl;
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
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE * NUMSUBWORKERS (total dest_kvs memory): "<<(PADDEDDESTBUFFER_SIZE * NUMSUBWORKERS)<<endl;
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * NUMSUBWORKERS (total dest memory): "<<(PADDEDDESTBUFFER_SIZE * VECTOR_SIZE * NUMSUBWORKERS)<<endl;
	cout<<"acts::printparameters:: APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ<<endl;
	cout<<"acts::printparameters:: APPLYVERTEXBUFFERSZ_KVS: "<<APPLYVERTEXBUFFERSZ_KVS<<endl;
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE: "<<PADDEDDESTBUFFER_SIZE<<endl;
	cout<<"acts::printparameters:: PADDEDDESTBUFFER_SIZE (KV): "<<PADDEDDESTBUFFER_SIZE * VECTOR_SIZE<<endl;
	cout<<"acts::printparameters:: SRCBUFFER_SIZE: "<<SRCBUFFER_SIZE<<endl;
	cout<<"acts::printparameters:: SRCBUFFER_SIZE (KV): "<<SRCBUFFER_SIZE * VECTOR_SIZE<<endl;
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
	cout<<"acts::printglobalvars:: globalstats_reduce_validkvsreduced (valids): "<<globalstats_reduce_validkvsreduced<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvsreducewritten: "<<globalstats_totalkvsreducewritten<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvsprocessed: "<<globalstats_totalkvsprocessed<<endl;
	cout<<"acts::printglobalvars:: globalstats_processedges_validkvsprocessed (valids): "<<globalstats_processedges_validkvsprocessed<<endl;
	cout<<"acts::printglobalvars:: globalvar_errorsingetpartition: "<<globalvar_errorsingetpartition<<endl;
	cout<<"acts::printglobalvars:: globalvar_errorsinreduce: "<<globalvar_errorsinreduce<<endl;
	cout<<"acts::printglobalvars:: globalvar_errorsinprocessedges: "<<globalvar_errorsinprocessedges<<endl;
	cout<<"acts::printglobalvars:: globalstats_totalkvsmerged: "<<globalstats_totalkvsmerged<<endl;
}
void actsutility::printglobalparameters(string message, globalparams_t globalparams){
	cout<<endl<<"actsutility::printglobalparameters: "<<message<<endl;
	std::cout<<"Kernel Started: globalparams.runkernelcommand: "<<globalparams.runkernelcommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.processcommand: "<<globalparams.processcommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.collectstatscommand: "<<globalparams.collectstatscommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.partitioncommand: "<<globalparams.partitioncommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.reducecommand: "<<globalparams.reducecommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.srcvoffset: "<<globalparams.srcvoffset<<std::endl;
	std::cout<<"Kernel Started: globalparams.srcvsize: "<<globalparams.srcvsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.srcvsize_kvs: "<<globalparams.srcvsize_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.edgessize: "<<globalparams.edgessize<<std::endl;
	std::cout<<"Kernel Started: globalparams.edgessize_kvs: "<<globalparams.edgessize_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.destvoffset: "<<globalparams.destvoffset<<std::endl;
	std::cout<<"Kernel Started: globalparams.firstvid: "<<globalparams.firstvid<<std::endl;
	std::cout<<"Kernel Started: globalparams.firstkey: "<<globalparams.firstkey<<std::endl;
	std::cout<<"Kernel Started: globalparams.firstvalue: "<<globalparams.firstvalue<<std::endl;
	std::cout<<"Kernel Started: globalparams.treedepth: "<<globalparams.treedepth<<std::endl;
	std::cout<<"Kernel Started: globalparams.LLOPnumpartitions: "<<globalparams.LLOPnumpartitions<<std::endl;
	std::cout<<"Kernel Started: globalparams.GraphIter: "<<globalparams.GraphIter<<std::endl;
	std::cout<<"Kernel Started: globalparams.batchsize: "<<globalparams.batchsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.runsize: "<<globalparams.runsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.nextbatchoffset: "<<globalparams.nextbatchoffset<<std::endl;
	std::cout<<"Kernel Started: globalparams.groupid: "<<globalparams.groupid<<std::endl;
	std::cout<<"Kernel Started: globalparams.beginLOP: "<<globalparams.beginLOP<<std::endl;
	std::cout<<"Kernel Started: globalparams.endLOP: "<<globalparams.endLOP<<std::endl;
	std::cout<<"Kernel Started: globalparams.numLOPs: "<<globalparams.numLOPs<<std::endl;
	std::cout<<"Kernel Started: globalparams.batch_range: "<<globalparams.batch_range<<std::endl;
	std::cout<<"Kernel Started: globalparams.batch_range_pow: "<<globalparams.batch_range_pow<<std::endl;
	std::cout<<"Kernel Started: globalparams.applyvertexbuffersz: "<<globalparams.applyvertexbuffersz<<std::endl;
	std::cout<<"Kernel Started: globalparams.applyvertexbuffersz_kvs: "<<globalparams.applyvertexbuffersz_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.baseoffset_messagesdram_kvs: "<<globalparams.baseoffset_messagesdram_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.baseoffset_kvdrambuffer_kvs: "<<globalparams.baseoffset_kvdrambuffer_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.baseoffset_kvdram_kvs: "<<globalparams.baseoffset_kvdram_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.baseoffset_kvdramworkspace_kvs: "<<globalparams.baseoffset_kvdramworkspace_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.baseoffset_statsdram_kvs: "<<globalparams.baseoffset_statsdram_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.baseoffset_activevertices_kvs: "<<globalparams.baseoffset_activevertices_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.baseoffset_edgesdata_kvs: "<<globalparams.baseoffset_edgesdata_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.baseoffset_vertexptr_kvs: "<<globalparams.baseoffset_vertexptr_kvs<<std::endl;
	std::cout<<"Kernel Started: globalparams.baseoffset_verticesdata_kvs: "<<globalparams.baseoffset_verticesdata_kvs<<std::endl;
	std::cout<<std::endl;
}
void actsutility::printpartitionresult(unsigned int enable, uint512_dt * kvdram, keyvalue_t * globaldestoffsets, keyvalue_t * globalstatsbuffer, sweepparams_t sweepparams){
	#ifdef _DEBUGMODE_KERNELPRINTS2
	#ifdef ACTSMODEL_LW
	if(enable == OFF){ return; }
	// printglobalvars();
	// clearglobalvars();
	if(sweepparams.currentLOP==0){ printkeyvalues("actslw::topkernel::globalstats", globalstatsbuffer, 16); } // NUMLASTLEVELPARTITIONS, 16
	if(sweepparams.currentLOP==0){  checkforoverlap("actslw::topkernel::actsutility::printpartitionresult::globalstats", globalstatsbuffer, 256); } // REMOVEME // NUMLASTLEVELPARTITIONS
	// if(sweepparams.currentLOP==0){ printkeyvalues("actslw::topkernel::globalstats", (keyvalue_t *)(&kvdram[BASEOFFSET_STATSDRAM_KVS]), 16 * 8, 8); } // NUMLASTLEVELPARTITIONS
	if(sweepparams.currentLOP > 0 && sweepparams.currentLOP <= TREE_DEPTH){ printkeyvalues("actslw::topkernel::globaldestoffsets", (keyvalue_t *)globaldestoffsets, NUM_PARTITIONS); }
	if(sweepparams.currentLOP > 0 && sweepparams.currentLOP <= TREE_DEPTH){ printvaluecount("actslw::topkernel::globaldestoffsets", (keyvalue_t *)globaldestoffsets, NUM_PARTITIONS); }
	if(sweepparams.currentLOP > 0 && sweepparams.currentLOP <= TREE_DEPTH){ checkforoverlap("actslw::topkernel::globaldestoffsets", (keyvalue_t *)globaldestoffsets, NUM_PARTITIONS); }
	if(sweepparams.currentLOP >= 1 && sweepparams.currentLOP <= TREE_DEPTH){ 
		#if not (defined(ACTSMODEL_LW) && defined(ACTSMODEL_LWGROUP2))
		scankeyvalues("actslw::topkernel::", (keyvalue_t *)(&kvdram[sweepparams.workdestbaseaddress_kvs]), globaldestoffsets, NUM_PARTITIONS, BATCH_RANGE / pow(NUM_PARTITIONS, sweepparams.currentLOP), sweepparams.upperlimit);
		#endif 
	}
	#endif
	#endif 
}
void actsutility::printpartitionresult2(unsigned int enable, uint512_dt * kvdram, keyvalue_t * globalstatsbuffer, sweepparams_t sweepparams){
	#ifdef ACTSMODEL_LW
	if(enable == OFF){ return; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	printkeyvalues("actslw::topkernel::globalstats", globalstatsbuffer, 16);
	printvaluecount("actslw::topkernel::globalstats", globalstatsbuffer, 16);
	#endif 
	scankeyvalues("actslw::topkernel::", (keyvalue_t *)(&kvdram[sweepparams.workdestbaseaddress_kvs]), globalstatsbuffer, NUM_PARTITIONS, BATCH_RANGE / pow(NUM_PARTITIONS, sweepparams.currentLOP), sweepparams.upperlimit);
	#endif
}

unsigned int actsutility::ugetvaluecount(keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	return totalnumkeyvalues;
}
unsigned int actsutility::getvaluecountexcept(keyvalue_t * keyvalues, unsigned int size, unsigned int exceptvalue){
	cout<<"getvaluecountexcept: size: "<<size<<", exceptvalue: "<<exceptvalue<<endl;
	unsigned int totalnumkeyvalues = 0; 
	unsigned int totalnumNOTkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ if(keyvalues[p].key != exceptvalue){ totalnumkeyvalues += 1; } else { totalnumNOTkeyvalues += 1; }}
	cout<<"total values counted: "<<totalnumkeyvalues<<", totalnumNOTkeyvalues: "<<totalnumNOTkeyvalues<<endl;
	return totalnumkeyvalues;
}
unsigned int actsutility::getvaluecountexcept(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int exceptvalue){
	cout<<"getvaluecountexcept: "<<message<<". size: "<<size<<", exceptvalue: "<<exceptvalue<<endl;
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
void actsutility::clearglobalvars(){
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"clearglobalvars: "<<endl;
	#endif 
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
	globalstats_reduce_validkvsreduced = 0;
	globalstats_totalkvsreducewritten = 0;
	globalstats_totalkvsprocessed = 0;
	globalstats_processedges_validkvsprocessed = 0;
	globalvar_totalkvsreadV = 0;
	globalvar_errorsingetpartition = 0;
	globalvar_errorsinreduce = 0;
	globalvar_errorsinprocessedges = 0;
	globalstats_totalkvsmerged = 0;
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
void actsutility::scankeyvalues(string message, keyvalue_t * keyvalues, keyvalue_t * stats, unsigned int numberofpartitions, unsigned int rangeperpartition, unsigned int upperlimit){
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"actsutility::scankeyvalues::"<<message<<" numberofpartitions: "<<numberofpartitions<<", rangeperpartition: "<<rangeperpartition<<endl;
	#endif 
	for(unsigned int i=0; i<numberofpartitions; i++){
		unsigned int lowerrangeindex = upperlimit + (i * rangeperpartition);
		unsigned int upperrangeindex = upperlimit + ((i+1) * rangeperpartition);
		unsigned int begin = stats[i].key;
		unsigned int end = stats[i].key + stats[i].value;
		unsigned int numerrorkeys = geterrorkeyvalues(keyvalues, begin, end, lowerrangeindex, upperrangeindex);
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"actsutility::scankeyvalues:: "<<numerrorkeys<<" errors seen for partition "<<i<<". ["<<lowerrangeindex<<" -> "<<upperrangeindex<<"]("<<begin<<" -> "<<end<<")("<<(end-begin)<<" values)"<<endl<<endl;
		#endif 
	}
	return;
}
unsigned int actsutility::geterrorkeyvalues(keyvalue_t * keyvalues, unsigned int begin, unsigned int end, unsigned int lowerrangeindex, unsigned int upperrangeindex){
	unsigned int numerrorkeys = 0;
	for(unsigned int i=begin; i<end; i++){
		if(keyvalues[i].key != INVALIDDATA){
			if(keyvalues[i].key < lowerrangeindex || keyvalues[i].key >= upperrangeindex){
				if(numerrorkeys < 8){ 
					// #ifdef ENABLE_PERFECTACCURACY
					cout<<"actsutility::geterrorkeyvalues::ERROR KEYVALUE. index: "<<i-begin<<", keyvalues["<<i<<"].key: "<<keyvalues[i].key<<", keyvalues["<<i<<"].value: "<<keyvalues[i].value<<endl; 
					// exit(EXIT_FAILURE);
					// #endif 
				}
				// #ifdef ENABLE_PERFECTACCURACY
				// cout<<"actsutility::geterrorkeyvalues::ERROR KEYVALUE keyvalues["<<i<<"].key: "<<keyvalues[i].key<<", keyvalues["<<i<<"].value: "<<keyvalues[i].value<<endl; 
				// exit(EXIT_FAILURE);
				// #endif 
				
				numerrorkeys += 1;
			}
		}
	}
	return numerrorkeys;
}
void actsutility::setstructs(config_t _config, sweepparams_t _sweepparams, travstate_t _travstate){
	config = _config;
	sweepparams = _sweepparams;
	travstate = _travstate;
}
config_t actsutility::getconfig(){ return config; }
sweepparams_t actsutility::getsweepparams(){ return sweepparams; }
travstate_t actsutility::gettravstate(){ return travstate; }
unsigned int actsutility::countvalues(string message, value_t * values, unsigned int size){
	unsigned int totalcount = 0;
	for(unsigned int i=0; i<size; i++){ totalcount += values[i]; }
	return totalcount;
}
void actsutility::countvalueslessthan(string message, value_t * values, unsigned int size, unsigned int data){
	unsigned int totalcount = 0;
	for(unsigned int i=0; i<size; i++){ if(values[i] < data){ totalcount += 1; }}
	cout<<"actsutility::"<<message<<"::countvalueslessthan ("<<data<<"):: total values counted: "<<totalcount<<endl;
	return;
}
void actsutility::checkgraph(keyvalue_t * vertexptrs, keyvalue_t * edges, unsigned int edgessize){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"checking graph... "<<endl;
	#endif 
	
	unsigned int firstsrcvid = edges[0].key;
	unsigned int lastsrcvid = edges[edgessize-1].key;
	
	for(unsigned int i=0; i<edgessize; i++){
		unsigned int currentsrcvid = edges[i].key;
	
		keyy_t beginvptr = vertexptrs[currentsrcvid].key;
		keyy_t endvptr = vertexptrs[currentsrcvid+1].key;
		keyy_t numedges = endvptr - beginvptr;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(i%10000 == 0){ cout<<"checkgraph: currentsrcvid: "<<currentsrcvid<<", numedges: "<<numedges<<", edgessize: "<<edgessize<<endl; }
		#endif 
		
		checkptr(currentsrcvid, currentsrcvid+1, beginvptr, endvptr, edges);
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::checkgraph: check graph operation was SUCCESSFULL. "<<endl;
	#endif
	return;
}
void actsutility::checkptr(unsigned int beginsrcvid, unsigned int endsrcvid, unsigned int beginvptr, unsigned int endvptr, keyvalue_t * edges){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::checkptr(A):: beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<", beginvptr: "<<beginvptr<<", endvptr: "<<endvptr<<endl;
	#endif 
	for(unsigned int i=beginvptr; i<endvptr; i++){
		#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
		if((edges[i].value >= beginsrcvid) && (edges[i].value < endsrcvid))
		#else 
		if((edges[i].key >= beginsrcvid) && (edges[i].key < endsrcvid))
		#endif 
		{
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(((i-beginvptr) % 10000) == 0){ cout<<"checkptr:: edges["<<i<<"].srcvid: "<<edges[i].key<<", beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<endl; }
			#endif 
		} else {
			#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
			cout<<"actsutility::checkptr:: ERROR. out-of-bounds. edges["<<i<<"].srcvid: "<<edges[i].value<<", beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<endl;
			#else 
			cout<<"actsutility::checkptr:: ERROR. out-of-bounds. edges["<<i<<"].srcvid: "<<edges[i].key<<", beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<endl;
			#endif 
			exit(EXIT_FAILURE);
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::checkptr(A): check graph operation was SUCCESSFULL. "<<endl;
	#endif 
	return;
}
void actsutility::checkptr(unsigned int beginsrcvid, unsigned int endsrcvid, unsigned int beginvptr, unsigned int endvptr, keyvalue_t * edges, unsigned int numedges){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::checkptr(B):: beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<", beginvptr: "<<beginvptr<<", endvptr: "<<endvptr<<", numedges: "<<numedges<<endl;
	#endif 
	unsigned int numvalids = 0;
	for(unsigned int i=beginvptr; i<endvptr; i++){
		if((edges[i].key >= beginsrcvid) && (edges[i].key < endsrcvid)){
			numvalids += 1;
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(((i-beginvptr) % 10000) == 0){ cout<<"checkptr:: edges["<<i<<"].srcvid: "<<edges[i].key<<", beginsrcvid: "<<beginsrcvid<<", endsrcvid: "<<endsrcvid<<endl; }
			#endif 
		} else {}
	}
	if(numedges != numvalids){ cout<<"actsutility::checkptr:: ERROR. numedges != numvalids. numedges: "<<numedges<<", numvalids: "<<numvalids<<endl; exit(EXIT_FAILURE); }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::checkptr(B): check graph operation was SUCCESSFULL. numvalids: "<<numvalids<<", numedges: "<<numedges<<endl;
	#endif 
	return;
}
void actsutility::resetkeyvalues(keyvalue_t * keyvalues, unsigned int size){
	for(unsigned int i=0; i<size; i++){ 
		keyvalues[i].key = 0;
		keyvalues[i].value = 0;
	}
	return;
}
void actsutility::resetvalues(keyvalue_t * keyvalues, unsigned int size){
	for(unsigned int i=0; i<size; i++){ 
		keyvalues[i].value = 0;
	}
	return;
}
void actsutility::copykeyvalues(keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size){
	for(unsigned int i=0; i<size; i++){ 
		keyvalues1[i] = keyvalues2[i];
	}
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
void actsutility::globalstats_reduce_countvalidkvsreduced(unsigned int count){
	globalstats_reduce_validkvsreduced += count;
	return;
}
void actsutility::globalstats_countkvsreducewritten(unsigned int count){
	globalstats_totalkvsreducewritten += count;
	return;
}
void actsutility::globalstats_countkvsprocessed(unsigned int count){
	globalstats_totalkvsprocessed += count;
	return;
}
void actsutility::globalstats_processedges_countvalidkvsprocessed(unsigned int count){
	globalstats_processedges_validkvsprocessed += count;
	return;
}
void actsutility::globalstats_countkvsreadV(unsigned int count){
	globalvar_totalkvsreadV += count;
	return;
}
void actsutility::globalstats_counterrorsingetpartition(unsigned int count){
	globalvar_errorsingetpartition += count;
	return;
}
void actsutility::globalstats_counterrorsinreduce(unsigned int count){
	globalvar_errorsinreduce += count;
	return;
}
void actsutility::globalstats_counterrorsinprocessedges(unsigned int count){
	globalvar_errorsinprocessedges += count;
	return;
}
void actsutility::globalstats_countkvsmerged(unsigned int count){
	globalstats_totalkvsmerged += count;
	return;
}
unsigned int actsutility::globalstats_getcounterrorsinreduce(){
	return globalvar_errorsinreduce;
}
unsigned int actsutility::globalstats_getcounterrorsinprocessedges(){
	return globalvar_errorsinprocessedges;
}
unsigned int actsutility::globalstats_getcountnumvalidprocessedges(){
	return globalstats_processedges_validkvsprocessed;
}

partition_type actsutility::getpartition(keyvalue_t keyvalue, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow){
	partition_type partition = ((keyvalue.key - upperlimit) >> (batch_range_pow - (NUM_PARTITIONS_POW * currentLOP)));
	
	#ifdef ENABLE_PERFECTACCURACY
		#ifdef _DEBUGMODE_CHECKS2
		if(partition >= NUM_PARTITIONS){ cout<<"actsutility::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", keyvalue.key: "<<keyvalue.key<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", keyvalue.key: "<<keyvalue.key<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<", batch_range_pow: "<<batch_range_pow<<endl; exit(EXIT_FAILURE); }
		#endif
	#endif 
	#ifndef ENABLE_PERFECTACCURACY
		if(partition >= NUM_PARTITIONS){ partition = (((1 << NUM_PARTITIONS_POW) - 1) & (partition >> (1 - 1))); } // FIXME. REMOVEME. PERFECTIONTEST.
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("actsutility::getpartition 2", partition, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	return partition;
}
void actsutility::checkn(unsigned int enable, string message, keyvalue_t * kv, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int n){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::checkn: "<<message<<endl;
	#endif
	unsigned int commonp = getpartition(kv[0], currentLOP, upperlimit, batch_range_pow);
	unsigned int p = 0;
	
	for(unsigned int i=0; i<n; i++){
		if(kv[i].key == INVALIDDATA || kv[i].value == INVALIDDATA){ continue; }
		// else {
			p = getpartition(kv[i], currentLOP, upperlimit, batch_range_pow);
			// cout<<"------ actsutility::checkn:: commonp: "<<commonp<<", p: "<<p<<", kv[i].key: "<<kv[i].key<<endl;
			if((p != commonp)){
				cout<<"ERROR. checkn. message:"<<message<<". group's partition("<<commonp<<") != element partition("<<p<<"). kv["<<i<<"]: "<<kv[i].key<<". EXITING... ............................................................"<<endl;
				for(unsigned int h=0; h<n; h++){ cout<<"> kv["<<h<<"].key: "<<kv[h].key<<endl; }
				exit(EXIT_FAILURE);
			}
		// }
	}
	return;
}
void actsutility::concatenate2keyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, skeyvalue_t * stats, keyvalue_t * BIGkeyvalues, skeyvalue_t * BIGstats, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::concatenate2keyvalues:: "<<message<<endl;
	#endif 
	unsigned int errcount = 0;
	
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		BIGstats[p].key = stats[p].key * 2;
		BIGstats[p].value = 0;
	}
	
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		unsigned int begin = stats[p].key;
		unsigned int end = stats[p].key + stats[p].value;
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"concatenate2keyvalues:: p: "<<p<<", begin: "<<begin<<", end: "<<end<<", size: "<<(end-begin)<<endl;
		#endif 
		if(end >= PADDEDDESTBUFFER_SIZE * 2){ cout<<"actsutility::concatenate2keyvalues: ERROR. end >= PADDEDDESTBUFFER_SIZE * 2. (stats["<<p<<"].key: "<<stats[p].key<<", stats["<<p<<"].value: "<<stats[p].value<<") EXITING..."<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int k=begin; k<end; k++){
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues1[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues2[k]; 
			BIGstats[p].value+=1;
			
			// if(BIGstats[p].value > PADDEDDESTBUFFER_SIZE * 2){ cout<<"actsutility::concatenate2keyvalues: ERROR. BIGstats["<<p<<"].value >= PADDEDDESTBUFFER_SIZE * 2. (BIGstats["<<p<<"].value: "<<BIGstats[p].value<<") EXITING..."<<endl; errcount += 1; exit(EXIT_FAILURE); }
			if(BIGstats[p].key + BIGstats[p].value > PADDEDDESTBUFFER_SIZE * 2){ cout<<"actsutility::concatenate2keyvalues: ERROR. BIGstats["<<p<<"].value >= PADDEDDESTBUFFER_SIZE * 2. (BIGstats["<<p<<"].value: "<<BIGstats[p].value<<") EXITING..."<<endl; errcount += 1; exit(EXIT_FAILURE); }
		}
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"successful. "<<errcount<<" errors"<<endl;
		#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	printkeyvalues("^^^^^^^^^^^^^^^^^^^^^^^^^ concatenate2keyvalues: printing stats (so2).", (keyvalue_t *)BIGstats, NUM_PARTITIONS);
	#endif 
	return;
}
void actsutility::concatenate4keyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, keyvalue_t * keyvalues3, keyvalue_t * keyvalues4, skeyvalue_t * stats, keyvalue_t * BIGkeyvalues, skeyvalue_t * BIGstats, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::concatenate4keyvalues:: "<<message<<endl;
	#endif 
	unsigned int errcount = 0;
	
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		BIGstats[p].key = stats[p].key * 4;
		BIGstats[p].value = 0;
	}
	
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		unsigned int begin = stats[p].key;
		unsigned int end = stats[p].key + stats[p].value;
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"concatenate4keyvalues:: p: "<<p<<", begin: "<<begin<<", end: "<<end<<", size: "<<(end-begin)<<endl;
		#endif 
		if(end >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ cout<<"actsutility::concatenate4keyvalues: ERROR. end >= PADDEDDESTBUFFER_SIZE * 4. (stats["<<p<<"].key: "<<stats[p].key<<", stats["<<p<<"].value: "<<stats[p].value<<") EXITING..."<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int k=begin; k<end; k++){
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues1[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues2[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues3[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues4[k]; 
			BIGstats[p].value+=1;
			
			if(BIGstats[p].value > PADDEDDESTBUFFER_SIZE * 4){ cout<<"actsutility::concatenate4keyvalues: ERROR. BIGstats["<<p<<"].value >= PADDEDDESTBUFFER_SIZE * 4. (BIGstats["<<p<<"].value: "<<BIGstats[p].value<<") EXITING..."<<endl; errcount += 1; exit(EXIT_FAILURE); }
		}
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"successful. "<<errcount<<" errors"<<endl;
		#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	printkeyvalues("^^^^^^^^^^^^^^^^^^^^^^^^^ concatenate4keyvalues: printing stats (so4).", (keyvalue_t *)BIGstats, NUM_PARTITIONS);
	#endif 
	return;
}
void actsutility::concatenate8keyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, keyvalue_t * keyvalues3, keyvalue_t * keyvalues4, keyvalue_t * keyvalues5, keyvalue_t * keyvalues6, keyvalue_t * keyvalues7, keyvalue_t * keyvalues8, skeyvalue_t * stats, keyvalue_t * BIGkeyvalues, skeyvalue_t * BIGstats, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow){			
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::concatenate8keyvalues:: "<<message<<endl;
	#endif 
	unsigned int errcount = 0;
	
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		BIGstats[p].key = stats[p].key;// * 8; // D's capsules is different from the others (A,B&C) - uses raw values
		BIGstats[p].value = 0;
	}
	
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		unsigned int begin = stats[p].key / VECTOR_SIZE;
		unsigned int end = (stats[p].key + stats[p].value) / VECTOR_SIZE; // D's capsules is different from the others (A,B&C) - uses raw values
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"concatenate8keyvalues:: p: "<<p<<", begin: "<<begin<<", end: "<<end<<", size: "<<(end-begin)<<endl;
		#endif 
		if(end >= PADDEDDESTBUFFER_SIZE * VECTOR_SIZE){ cout<<"actsutility::concatenate8keyvalues: ERROR. end >= PADDEDDESTBUFFER_SIZE * 8. (stats["<<p<<"].key: "<<stats[p].key<<", stats["<<p<<"].value: "<<stats[p].value<<") EXITING..."<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int k=begin; k<end; k++){
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues1[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues2[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues3[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues4[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues5[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues6[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues7[k]; 
			BIGstats[p].value+=1;
			BIGkeyvalues[BIGstats[p].key + BIGstats[p].value] = keyvalues8[k]; 
			BIGstats[p].value+=1;
			
			if(BIGstats[p].value > PADDEDDESTBUFFER_SIZE * 8){ cout<<"actsutility::concatenate8keyvalues: ERROR. BIGstats["<<p<<"].value("<<BIGstats[p].value<<") >= PADDEDDESTBUFFER_SIZE * 8("<<PADDEDDESTBUFFER_SIZE * 8<<"). EXITING..."<<endl; errcount += 1; 
				exit(EXIT_FAILURE); 
				}
		}
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"successful. "<<errcount<<" errors"<<endl;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	printkeyvalues("^^^^^^^^^^^^^^^^^^^^^^^^^ concatenate8keyvalues: printing stats (so4).", (keyvalue_t *)BIGstats, NUM_PARTITIONS);
	#endif
	return;
}

void actsutility::printprofileso1(unsigned int enable, string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[8][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	#ifdef SIMPLEANDFASTPREPAREFUNC
	return; // not applicable.
	#endif 
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	cout<<endl<<"+++ actsutility::printprofileso1 [with stats]:: "<<message<<endl;
	#endif 
	
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ partitioncount[p] = 0; }
	
	for(unsigned int i=0; i<8; i++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"printprofileso1:: concatenating and printing profile for keyvalues["<<i<<"], stats["<<i<<"]"<<endl;
		#endif 
		#ifdef _DEBUGMODE_KERNELPRINTS
		printkeyvalues("printprofileso1:: printing stats:", (keyvalue_t *)stats, NUM_PARTITIONS);
		#endif
		#ifdef SW
		printprofile(ON, "message: " + message + ". i: " + std::to_string(i) + ". printprofileso1:: keyvalues, stats", keyvalues[i], stats[i], PADDEDDESTBUFFER_SIZE, currentLOP, upperlimit, batch_range_pow, partitioncount);
		#else 
		printprofile(ON, "message: " + message + ". printprofileso1:: keyvalues, stats", keyvalues[i], stats[i], PADDEDDESTBUFFER_SIZE, currentLOP, upperlimit, batch_range_pow, partitioncount);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"successful. "<<endl;
	printvalues("actsutility::printprofileso1:: printing collected stats of overall keyvalues.", partitioncount, NUM_PARTITIONS);
	cout<<endl;
	#endif 
	return;
}
void actsutility::printprofileso2(unsigned int enable, string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[4][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	cout<<endl<<"+++ actsutility::printprofileso2 [with stats]:: "<<message<<endl;
	#endif 
	
	keyvalue_t BIGkeyvalues[PADDEDDESTBUFFER_SIZE * 2];
	skeyvalue_t BIGstats[NUM_PARTITIONS];
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ partitioncount[p] = 0; }
	
	for(unsigned int i=0; i<4; i++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"printprofileso2:: concatenating and printing profile for keyvalues["<<2*i<<"], keyvalues["<<2*i+1<<"], stats["<<i<<"]"<<endl;
		#endif 
		concatenate2keyvalues("keyvalues[0], keyvalues[1]", keyvalues[2*i], keyvalues[2*i+1], stats[i], BIGkeyvalues, BIGstats, currentLOP, upperlimit, batch_range_pow);
		#ifdef _DEBUGMODE_KERNELPRINTS
		printkeyvalues("printprofileso2:: printing BIGstats:", (keyvalue_t *)BIGstats, NUM_PARTITIONS);
		#endif
		printprofile(ON, "printprofileso2:: BIGkeyvalues, BIGstats", BIGkeyvalues, BIGstats, 2*PADDEDDESTBUFFER_SIZE, currentLOP, upperlimit, batch_range_pow, partitioncount);
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"successful. "<<endl;
	printvalues("printprofileso2: printing actual stats.", partitioncount, NUM_PARTITIONS);
	cout<<endl;
	#endif 
	return;
}
void actsutility::printprofileso4(unsigned int enable, string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[2][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	cout<<endl<<"+++ actsutility::printprofileso4 [with stats]:: "<<message<<endl;
	#endif 
	
	keyvalue_t BIGkeyvalues[PADDEDDESTBUFFER_SIZE * 4];
	skeyvalue_t BIGstats[NUM_PARTITIONS];
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ partitioncount[p] = 0; }
	
	for(unsigned int i=0; i<2; i++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"printprofileso4:: concatenating and printing profile for keyvalues["<<4*i<<"], keyvalues["<<4*i+1<<"], keyvalues["<<4*i+2<<"], keyvalues["<<4*i+3<<"], stats["<<i<<"]"<<endl;
		#endif 
		concatenate4keyvalues("keyvalues[0], keyvalues[1]", keyvalues[4*i], keyvalues[4*i+1], keyvalues[4*i+2], keyvalues[4*i+3], stats[i], BIGkeyvalues, BIGstats, currentLOP, upperlimit, batch_range_pow);
		#ifdef _DEBUGMODE_KERNELPRINTS
		printkeyvalues("printprofileso4:: printing BIGstats:", (keyvalue_t *)BIGstats, NUM_PARTITIONS);
		#endif
		printprofile(ON, "printprofileso4:: BIGkeyvalues, BIGstats", BIGkeyvalues, BIGstats, 4*PADDEDDESTBUFFER_SIZE, currentLOP, upperlimit, batch_range_pow, partitioncount);
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"successful. "<<endl;
	printvalues("printprofileso4: printing partition counts.", partitioncount, NUM_PARTITIONS);
	// printkeyvalues("printprofileso4: printing stats.", BIGstats, NUM_PARTITIONS);
	cout<<endl;
	#endif 
	return;
}
void actsutility::printprofileso8(unsigned int enable, string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(_DEBUGMODE_RUNKERNELPRINTS)
	cout<<endl<<"+++ actsutility::printprofileso8 [with stats]:: "<<message<<endl;
	#endif 
	
	keyvalue_t BIGkeyvalues[PADDEDDESTBUFFER_SIZE * 8];
	skeyvalue_t BIGstats[NUM_PARTITIONS];
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ partitioncount[p] = 0; }
	
	for(unsigned int i=0; i<1; i++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"printprofileso8:: concatenating and printing profile for keyvalues["<<4*i<<"] ... keyvalues["<<4*i+7<<"], stats["<<i<<"]"<<endl;
		#endif 
		concatenate8keyvalues("keyvalues[0], keyvalues[1]", keyvalues[8*i], keyvalues[8*i+1], keyvalues[8*i+2], keyvalues[8*i+3], keyvalues[8*i+4], keyvalues[8*i+5], keyvalues[8*i+6], keyvalues[8*i+7], stats, BIGkeyvalues, BIGstats, currentLOP, upperlimit, batch_range_pow);
		#ifdef _DEBUGMODE_KERNELPRINTS
		printkeyvalues("printprofileso8:: printing BIGstats:", (keyvalue_t *)BIGstats, NUM_PARTITIONS);
		#endif
		printprofile(ON, "printprofileso8:: BIGkeyvalues, BIGstats", BIGkeyvalues, BIGstats, 8*PADDEDDESTBUFFER_SIZE, currentLOP, upperlimit, batch_range_pow, partitioncount);
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"successful. "<<endl;
	printvalues("printprofileso8: printing partition counts.", partitioncount, NUM_PARTITIONS);
	// printkeyvalues("printprofileso8: printing stats.", BIGstats, NUM_PARTITIONS);
	cout<<endl;
	#endif 
	return;
}
void actsutility::printprofile(unsigned int enable, string message, keyvalue_t * keyvalues, skeyvalue_t * stats, unsigned int size, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"---------- actsutility::printprofile:: "<<message<<endl;
	#endif 
	
	unsigned int errcount = 0;
	unsigned int proccount = 0;
	unsigned int invalidcount = 0;
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		unsigned int begin = stats[p].key;
		unsigned int end = stats[p].key + stats[p].value;
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"printprofile:: p: "<<p<<", begin: "<<begin<<", end: "<<end<<", size: "<<(end-begin)<<endl;
		#endif 
		if(end > size){ cout<<"actsutility::printprofile: ERROR. end > size. (stats["<<p<<"].key: "<<stats[p].key<<", stats["<<p<<"].value: "<<stats[p].value<<") EXITING..."<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int k=begin; k<end; k++){
			if(keyvalues[k].key == INVALIDDATA || keyvalues[k].value == INVALIDDATA){ invalidcount += 1; continue; } 
			
			proccount += 1;
			partitioncount[p] += 1;
			unsigned int thisp = getpartition(keyvalues[k], currentLOP, upperlimit, batch_range_pow);
			if(thisp != p){ 
				errcount += 1;
				cout<<"actsutility::printprofile: ERROR in printprofile: begin: "<<begin<<". end: "<<end<<""<<endl; 
				cout<<"actsutility::printprofile: ERROR in printprofile: message: "<<message<<". thisp("<<thisp<<") != p("<<p<<"). keyvalues["<<k<<"].key: "<<keyvalues[k].key<<". printing some values and EXITING..."<<endl; 
				// printkeyvalues("actsutility::printprofile:keyvalues[begin]", (keyvalue_t *)&keyvalues[begin], 16);
				printkeyvalues("actsutility::printprofile:keyvalues[begin]", (keyvalue_t *)&keyvalues[begin], size);
				printkeyvalues("actsutility::printprofile:stats", (keyvalue_t *)stats, 16);
				exit(EXIT_FAILURE); 
				}
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"printprofile ("<<message<<") successful. "<<proccount <<" processed, "<<errcount<<" errors, "<<invalidcount<<" invalids."<<endl;
	#endif 
	return;
}
void actsutility::printprofile(unsigned int enable, string message, keyvalue_t * keyvalues, unsigned int size, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"actsutility::printprofile [profiling buffer with size provided]: "<<message<<endl;
	#endif 
	unsigned int totalnums = 0;
	unsigned int nums = 0;
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		nums = countkeysbelongingtopartition(p, keyvalues, size, currentLOP, upperlimit, batch_range_pow);
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<"partition("<<p<<"): "<< nums <<endl;
		#endif 
		totalnums += nums;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"printprofile successful. totalnums: "<< totalnums <<endl;
	#endif 
	return;
}

void actsutility::getprofileso1(unsigned int enable, string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[8][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	printprofileso1(ON, message, keyvalues, stats, currentLOP, upperlimit, batch_range_pow, partitioncount);
}
void actsutility::getprofileso2(unsigned int enable, string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[4][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	printprofileso2(ON, message, keyvalues, stats, currentLOP, upperlimit, batch_range_pow, partitioncount);
}
void actsutility::getprofileso4(unsigned int enable, string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[2][NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	printprofileso4(ON, message, keyvalues, stats, currentLOP, upperlimit, batch_range_pow, partitioncount);
}
void actsutility::getprofileso8(unsigned int enable, string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t stats[NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int partitioncount[NUM_PARTITIONS]){
	if(enable == OFF){ return; }
	printprofileso8(ON, message, keyvalues, stats, currentLOP, upperlimit, batch_range_pow, partitioncount);
}
void actsutility::checkprofile(unsigned int enable, string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int size_kvs, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow, unsigned int factor, unsigned int totalnum){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::checkprofile: "<<message<<endl;
	#endif 
	unsigned int mytotalnums = 0;
	unsigned int count = 0;
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		count = 0;
		for(unsigned int i=0; i<size_kvs; i++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				if(getpartition(keyvalues[v][i], currentLOP, upperlimit, batch_range_pow) == p){ count += 1; }
			}
		}
		
		if(count % factor != 0){
			cout<<"actsutility::checkprofile: message("<<message<<"). ERROR in checkprofile: (count("<<count<<") % factor("<<factor<<") != 0). p: "<<p<<", count: "<<count<<", factor: "<<factor<<". EXITING..."<<endl; 
			exit(EXIT_FAILURE); 
		}
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"partition("<<p<<"): "<< count <<endl;
		#endif 
		mytotalnums += count;
	}
	if(mytotalnums != totalnum){
			cout<<"ctsutility::checkprofile: ERROR in checkprofile: mytotalnums("<<mytotalnums<<") != totalnum("<<totalnum<<"). EXITING..."<<endl; 
			exit(EXIT_FAILURE); 
		}
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"checkprofile successful. mytotalnums: "<< mytotalnums <<endl;
	#endif 
	return;
}
void actsutility::checkbufferprofile(unsigned int enable, string message, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t stats[NUM_PARTITIONS], unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::checkbufferprofile::"<<message<<" currentLOP: "<<currentLOP<<", upperlimit: "<<upperlimit<<", batch_range_pow: "<<batch_range_pow<<endl;
	#endif 
	
	unsigned int totalcount = 0;
	unsigned int errcount = 0;
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		unsigned int count = 0;
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"actsutility::checkbufferprofile: stats["<<p<<"].key: "<< stats[p].key<<", stats["<<p<<"].value: "<<stats[p].value <<endl;
		#endif 
		if(p<NUM_PARTITIONS-1){
			if(stats[p].key + stats[p].value > stats[p+1].key){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"actsutility::checkbufferprofile: ERROR in checkbufferprofile: (stats["<<p<<"].key("<<stats[p].key<<") + stats["<<p<<"].value("<<stats[p].value<<") > stats["<<p+1<<"].key("<<stats[p+1].key<<")). EXITING..."<<endl; 
				#endif 
				// exit(EXIT_FAILURE); 
			}
		}
		
		// for(unsigned int i=stats[p].key/VECTOR_SIZE; i<(stats[p].key + stats[p].value)/VECTOR_SIZE; i++){ // FIXME. should be (stats[p].key + stats[p].value + (VECTOR_SIZE-1))/VECTOR_SIZE
		for(unsigned int i=stats[p].key/VECTOR_SIZE; i<(stats[p].key + stats[p].value + (VECTOR_SIZE-1))/VECTOR_SIZE; i++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				unsigned int thisp = getpartition(keyvalues[v][i], currentLOP, upperlimit, batch_range_pow);
				if(thisp != p){
					errcount += 1;
					cout<<"actsutility::checkbufferprofile: ERROR in checkbufferprofile: thisp("<<thisp<<") != p("<<p<<"). keyvalues["<<v<<"]["<<i<<"].key: "<<keyvalues[v][i].key<<". EXITING..."<<endl; 
					exit(EXIT_FAILURE); 
				}
				count += 1;
				totalcount += 1;
			}
		}
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"partition("<<p<<"): "<< count <<endl;
		#endif 
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(errcount == 0){ cout<<"checkbufferprofile successful. "<<totalcount<<" keyvalues processed." <<endl; }
	else{ cout<<"checkbufferprofile failed. "<<errcount<<" errors seen." <<endl; }
	#endif 
	return;
}
void actsutility::compareprofiles(unsigned int enable, string message, keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow){
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"compareprofiles::printprofile: "<<message<<endl;
	#endif 
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		unsigned int nums1 = countkeysbelongingtopartition(p, buffer1, size, currentLOP, upperlimit, batch_range_pow);
		unsigned int nums2 = countkeysbelongingtopartition(p, buffer2, size, currentLOP, upperlimit, batch_range_pow);
		if(nums1 != nums2){ cout<<"actsutility::compareprofiles: partition("<<p<<") not equal: nums1("<<nums1<<") != nums2("<<nums2<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
	}
	return;
}
unsigned int actsutility::countkeysbelongingtopartition(unsigned int p, keyvalue_t * keyvalues, unsigned int size, unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow){
	unsigned int count = 0;
	for(unsigned int i=0; i<size; i++){
		// if(keyvalues[i].key == INVALIDDATA || keyvalues[i].value == INVALIDDATA){ cout<<"countkeysbelongingtopartition 9089"<<endl; exit(EXIT_FAILURE); }
		if(keyvalues[i].key != INVALIDDATA){
			if(getpartition(keyvalues[i], currentLOP, upperlimit, batch_range_pow) == p){ count += 1; }
		}
	}
	return count;
}

void actsutility::collectstats(unsigned int enable, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], unsigned int size_kvs, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int x, unsigned int y){
	for(unsigned int i=0; i<size_kvs; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			if(keyvalues[v][i].key == INVALIDDATA || keyvalues[v][i].value == INVALIDDATA){ continue; }
			
			unsigned int p = getpartition(keyvalues[v][i], currentLOP, upperlimit, batch_range_pow);
			mystats[x][y][p] += 1;
		}
	}
	return;
}
void actsutility::collectstats(unsigned int enable, keyvalue_t keyvalues[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], keyvalue_t localstats[NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int x, unsigned int y){
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		unsigned int begin_kvs = localstats[p].key / VECTOR_SIZE;
		unsigned int size_kvs = localstats[p].value / VECTOR_SIZE;
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"actsutility::collectstats:: p: "<<p<<", begin_kvs: "<<begin_kvs<<", end_kvs: "<<begin_kvs + size_kvs<<", size_kvs: "<<size_kvs<<endl;
		#endif 
		if(begin_kvs + size_kvs > PADDEDDESTBUFFER_SIZE){ cout<<"actsutility::collectstats: ERROR. begin_kvs + size_kvs >= PADDEDDESTBUFFER_SIZE. (localstats["<<p<<"].key: "<<localstats[p].key<<", localstats["<<p<<"].value: "<<localstats[p].value<<") EXITING..."<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int i=begin_kvs; i<begin_kvs + size_kvs; i++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				if(keyvalues[v][i].key == INVALIDDATA || keyvalues[v][i].value == INVALIDDATA){ continue; } 
				
				unsigned int thisp = getpartition(keyvalues[v][i], currentLOP, upperlimit, batch_range_pow);
				mystats[x][y][thisp] += 1;
			}
		}
	}
}
void actsutility::collectstats(unsigned int enable, keyvalue_t * keyvalues, keyvalue_t localstats[NUM_PARTITIONS], step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int x, unsigned int y){
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		unsigned int begin = localstats[p].key;
		unsigned int size = localstats[p].value;
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"actsutility::collectstats:: p: "<<p<<", begin: "<<begin<<", end: "<<begin + size<<", size: "<<size<<endl;
		#endif 
		if(begin + size >= KVSOURCEDRAMSZ){ cout<<"actsutility::collectstats: ERROR. begin + size >= PADDEDDESTBUFFER_SIZE. (localstats["<<p<<"].key: "<<localstats[p].key<<", localstats["<<p<<"].value: "<<localstats[p].value<<") EXITING..."<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int i=begin; i<begin + size; i++){
			if(keyvalues[i].key == INVALIDDATA || keyvalues[i].value == INVALIDDATA){ continue; }
			
			unsigned int thisp = getpartition(keyvalues[i], currentLOP, upperlimit, batch_range_pow);
			mystats[x][y][thisp] += 1;
		}
	}
}
void actsutility::collectstats(unsigned int enable, keyvalue_t localcapsule[NUM_PARTITIONS], unsigned int x, unsigned int y){
	for(buffer_type p=0; p<NUM_PARTITIONS; p++){
		mystats[x][y][p] += localcapsule[p].value;
	}
	return;
}
void actsutility::collectstats(unsigned int enable, keyvalue_t * keyvalues, unsigned int size, step_type currentLOP, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int x, unsigned int y){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"actsutility::collectstats"<<endl;
	#endif 
	for(unsigned int i=0; i<NUM_PARTITIONS; i++){ mystats[x][y][i] = 0; }
	
	for(unsigned int i = 0; i <size; i++){
		unsigned int p = getpartition(keyvalues[i], currentLOP, upperlimit, batch_range_pow);
		mystats[x][y][p] += 1;
	}
	return;
}
unsigned int * actsutility::getstats(unsigned int x, unsigned int y){
	return mystats[x][y];
}
void actsutility::clearallstats(){
	for(unsigned int i=0; i<8; i++){ for(unsigned int k=0; k<MYSTATSYSIZE; k++){ for(unsigned int p=0; p<NUM_PARTITIONS; p++){ mystats[i][k][p] = 0; }}}
}
void actsutility::clearstats(unsigned int x){
	for(unsigned int k=0; k<MYSTATSYSIZE; k++){ for(unsigned int p=0; p<NUM_PARTITIONS; p++){ mystats[x][k][p] = 0; }}
}
keyvalue_t * actsutility::getmykeyvalues(unsigned int x){
	return mykeyvalues[x];
}
unsigned int actsutility::getmincutoffseen(){
	return mincutoffseen;
}
unsigned int actsutility::getmaxcutoffseen(){
	return maxcutoffseen;
}
void actsutility::updatemincutoffseen(unsigned int val){
	if(mincutoffseen > val){ mincutoffseen = val; }
}
unsigned int actsutility::updatemaxcutoffseen(unsigned int val){
	if(maxcutoffseen < val){ maxcutoffseen = val; }
}

void actsutility::intrarunpipelinecheck_shifting(unsigned int enable, string message, 
												keyvalue_t bufferA[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t buffer1capsule[8][NUM_PARTITIONS], 
												keyvalue_t bufferB[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferBcapsule[4][NUM_PARTITIONS],
												keyvalue_t bufferC[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferCcapsule[2][NUM_PARTITIONS],
												keyvalue_t bufferD[VECTOR_SIZE][PADDEDDESTBUFFER_SIZE], skeyvalue_t bufferDcapsule[NUM_PARTITIONS],
												unsigned int partitioncountso1[NUM_PARTITIONS], 
												unsigned int partitioncountso2[NUM_PARTITIONS],
												unsigned int partitioncountso4[NUM_PARTITIONS],
												unsigned int partitioncountso8[NUM_PARTITIONS],
												unsigned int currentLOP, unsigned int upperlimit, unsigned int batch_range_pow){
	if(enable == OFF){ return; }
	printprofileso1(ON, "actsutility::intrarunpipelinecheck_shifting. message:"+message+". bufferA", bufferA, buffer1capsule, currentLOP, upperlimit, batch_range_pow, partitioncountso1);
	
	printprofileso2(ON, "actsutility::intrarunpipelinecheck_shifting. message:"+message+". bufferB", bufferB, bufferBcapsule, currentLOP, upperlimit, batch_range_pow, partitioncountso2);
	
	printprofileso4(ON, "actsutility::intrarunpipelinecheck_shifting. message:"+message+". bufferC", bufferC, bufferCcapsule, currentLOP, upperlimit, batch_range_pow, partitioncountso4);
	
	printprofileso8(ON, "actsutility::intrarunpipelinecheck_shifting. message:"+message+". bufferD", bufferD, bufferDcapsule, currentLOP, upperlimit, batch_range_pow, partitioncountso8);
	// printkeyvalues("actsutility::intrarunpipelinecheck_shifting. bufferDcapsule", (keyvalue_t *)bufferDcapsule, NUM_PARTITIONS);
	return;
}
void actsutility::intrapartitioncheck(){
	for(batch_type k=0; k<MYSTATSYSIZE-4; k+=3){
		#ifdef SW 
		hcheckforequal(ON, "intrapartitioncheck: checking if getstats(0, "+std::to_string(k)+") == getstats(1, "+std::to_string(k+2)+")", getstats(0, k), getstats(1, k+2), NUM_PARTITIONS, NAp, NAp, NAp);
		hcheckforequal(ON, "intrapartitioncheck: checking if getstats(0, "+std::to_string(k+1)+") == getstats(1, "+std::to_string(k+3)+")", getstats(0, k+1), getstats(1, k+3), NUM_PARTITIONS, NAp, NAp, NAp);
		hcheckforequal(ON, "intrapartitioncheck: checking if getstats(0, "+std::to_string(k+2)+") == getstats(1, "+std::to_string(k+4)+")", getstats(0, k+2), getstats(1, k+4), NUM_PARTITIONS, NAp, NAp, NAp); 
		#else 
		hcheckforequal(ON, "intrapartitioncheck: checking if getstats(0, k) == getstats(1, k+2)", getstats(0, k), getstats(1, k+2), NUM_PARTITIONS, NAp, NAp, NAp);
		hcheckforequal(ON, "intrapartitioncheck: checking if getstats(0, k+1) == getstats(1, k+3)", getstats(0, k+1), getstats(1, k+3), NUM_PARTITIONS, NAp, NAp, NAp);
		hcheckforequal(ON, "intrapartitioncheck: checking if getstats(0, k+2) == getstats(1, k+4)", getstats(0, k+2), getstats(1, k+4), NUM_PARTITIONS, NAp, NAp, NAp); 
		#endif 
	}
	clearstats(0);
	clearstats(1);
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"intra-partition check passed. "<<endl;
	#endif
	return;
}
void actsutility::postpartitioncheck(uint512_dt * kvdram, keyvalue_t globalstatsbuffer[GLOBALSTATSBUFFERSZ], travstate_t ptravstate, sweepparams_t sweepparams, globalparams_t globalparams){
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"post-partition check started. "<<endl;
	cout<<"actsutility::postpartitioncheck: currentLOP: "<<sweepparams.currentLOP<<", source_partition: "<<sweepparams.source_partition<<endl;
	#endif 
	#if defined(_DEBUGMODE_CHECKS2) && defined(ENABLE_PERFECTACCURACY)
	checkforoverlap("actsutility::postpartitioncheck: globalstatsbuffer", globalstatsbuffer, NUM_PARTITIONS);
	#endif 
	collectstats(ON, (keyvalue_t *)&kvdram[sweepparams.worksourcebaseaddress_kvs], ptravstate.size_kvs * VECTOR_SIZE, sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 7, 0);
	collectstats(ON, (keyvalue_t *)&kvdram[sweepparams.workdestbaseaddress_kvs], globalstatsbuffer, sweepparams.currentLOP, sweepparams.upperlimit, globalparams.batch_range_pow, 7, 2);
	#ifdef _DEBUGMODE_KERNELPRINTS
	printvalues("actsutility::postpartitioncheck: stats collected online [before partition stage (7,0)]", getstats(7, 0), NUM_PARTITIONS);
	printvalues("actsutility::postpartitioncheck: stats collected online [after partition stage (7,2)]", getstats(7, 2), NUM_PARTITIONS);
	printvalues("actsutility::postpartitioncheck: stats collected online [during partition stage (7,1)]", getstats(7, 1), NUM_PARTITIONS);
	printkeyvalues("actsutility::postpartitioncheck: globalstatsbuffer collected online (globalstatsbuffer)", globalstatsbuffer, NUM_PARTITIONS);
	printkeyvalues("actsutility::postpartitioncheck: stats collected offline (getmykeyvalues(7))", getmykeyvalues(7), NUM_PARTITIONS);
	// checkforgreaterthan("ensuring getstats(7, 0) > getstats(7, 2)", getstats(7, 0), getstats(7, 2), NUM_PARTITIONS);
	cout<<"minimum cutoff seen during partitioning: "<<getmincutoffseen()<<endl;
	cout<<"maximum cutoff seen during partitioning: "<<getmaxcutoffseen()<<endl;
	#endif 
	#if defined(_DEBUGMODE_CHECKS2) && defined(ENABLE_PERFECTACCURACY)
	checkforgreaterthan("ensuring getstats(7, 0) > getstats(7, 2)", getstats(7, 0), getstats(7, 2), NUM_PARTITIONS);
	#endif 
	cout<<"post-partition check passed. "<<endl;
	return;
}




