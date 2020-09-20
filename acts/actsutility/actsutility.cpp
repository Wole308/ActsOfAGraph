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
}
actsutility::~actsutility(){} 

void actsutility::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data >= upper_bound){ std::cout<<"acts::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }
}
void actsutility::checkforequal(string message, unsigned int data1, unsigned int data2){
	if(data1 == data2){ cout<<"acts::checkforequal: ERROR. data1 == data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
}
void actsutility::checkforoverlap(string message, keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int i=0; i<size-1; i++){
		if(keyvalues[i].key + keyvalues[i].value >= keyvalues[i+1].key){ 
			cout<<"aactsutility::checkforoverlap: ERROR. overlap found message: "<<message<<", i: "<<i<<", keyvalues[i].key: "<<keyvalues[i].key<<", keyvalues[i].value: "<<keyvalues[i].value<<", keyvalues[i+1].key: "<<keyvalues[i+1].key<<endl; exit(EXIT_FAILURE); 
		}								
	}
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
void actsutility::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; }
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
void actsutility::printvaluecount(string message, keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	cout<<"total values counted: "<<totalnumkeyvalues<<endl;
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
	cout<<"acts::printglobalvars:: globalstats_totalkvsreducewritten: "<<globalstats_totalkvsreducewritten<<endl;
	cout<<"acts::printglobalvars:: globalstats_reduce_validkvsreduced (valids): "<<globalstats_reduce_validkvsreduced<<endl;
	cout<<"acts::printglobalvars:: globalvar_errorsingetpartition: "<<globalvar_errorsingetpartition<<endl;
	cout<<"acts::printglobalvars:: globalvar_errorsinreduce: "<<globalvar_errorsinreduce<<endl;
}
void actsutility::printglobalparameters(string message, globalparams_t globalparams){
	cout<<endl<<"actsutility::printglobalparameters: "<<message<<endl;
	std::cout<<"Kernel Started: globalparams.runkernelcommand: "<<globalparams.runkernelcommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.processcommand: "<<globalparams.processcommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.collectstatscommand: "<<globalparams.collectstatscommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.partitioncommand: "<<globalparams.partitioncommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.reducecommand: "<<globalparams.reducecommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.vbegin: "<<globalparams.vbegin<<std::endl;
	std::cout<<"Kernel Started: globalparams.vsize: "<<globalparams.vsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.treedepth: "<<globalparams.treedepth<<std::endl;
	std::cout<<"Kernel Started: globalparams.LLOPnumpartitions: "<<globalparams.LLOPnumpartitions<<std::endl;
	std::cout<<"Kernel Started: globalparams.GraphIter: "<<globalparams.GraphIter<<std::endl;
	std::cout<<"Kernel Started: globalparams.batchsize: "<<globalparams.batchsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.runsize: "<<globalparams.runsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.nextbatchoffset: "<<globalparams.nextbatchoffset<<std::endl;
	std::cout<<"Kernel Started: globalparams.statsalreadycollected: "<<globalparams.statsalreadycollected<<std::endl;
	std::cout<<std::endl;
}
void actsutility::printpartitionstep(config_t config, sweepparams_t sweepparams, travstate_t travstate, unsigned int instanceid){
	#ifdef _DEBUGMODE_KERNELPRINTS2
	string message; string message2;
	if(config.enablecollectglobalstats == ON) { message = "collectglobalstats"; } else if (config.enablepartition == ON){ message = "partition"; } else if (config.enablereduce == ON){ message = "reduce"; }
	message2 = "### dispatch"+to_string(instanceid)+"::" + message + ":: source partition";
	print6(message2, "upperlimit", "travstate.begin", "travstate.end", "destination range", "currentLOP", sweepparams.source_partition, sweepparams.upperlimit, travstate.begin_kvs * VECTOR_SIZE, travstate.end_kvs * VECTOR_SIZE, BATCH_RANGE / (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), sweepparams.currentLOP);				
	#endif
	return;
}
void actsutility::printpartitionresult(unsigned int enable, uint512_dt * kvdram, keyvalue_t * globaldestoffsets, keyvalue_t * globalstatsbuffer, sweepparams_t sweepparams){
	#ifdef _DEBUGMODE_KERNELPRINTS2
	#ifdef ACTSMODEL_LW
	if(enable == OFF){ return; }
	// printglobalvars();
	// clearglobalvars();
	if(sweepparams.currentLOP==0){ printkeyvalues("actslw::topkernel::globalstats", globalstatsbuffer, 16); } // NUMLASTLEVELPARTITIONS, 16
	if(sweepparams.currentLOP==0){  checkforoverlap("actslw::topkernel::actsutility::printpartitionresult::globalstats", globalstatsbuffer, NUMLASTLEVELPARTITIONS); }
	// if(sweepparams.currentLOP==0){ printkeyvalues("actslw::topkernel::globalstats", (keyvalue_t *)(&kvdram[BASEOFFSET_STATSDRAM_KVS]), 16 * 8, 8); } // NUMLASTLEVELPARTITIONS
	if(sweepparams.currentLOP > 0 && sweepparams.currentLOP <= TREE_DEPTH){ printkeyvalues("actslw::topkernel::globaldestoffsets", (keyvalue_t *)globaldestoffsets, NUM_PARTITIONS); }
	if(sweepparams.currentLOP > 0 && sweepparams.currentLOP <= TREE_DEPTH){ printvaluecount("actslw::topkernel::globaldestoffsets", (keyvalue_t *)globaldestoffsets, NUM_PARTITIONS); }
	if(sweepparams.currentLOP > 0 && sweepparams.currentLOP <= TREE_DEPTH){ checkforoverlap("actslw::topkernel::globaldestoffsets", (keyvalue_t *)globaldestoffsets, NUM_PARTITIONS); }
	if(sweepparams.currentLOP >= 1 && sweepparams.currentLOP <= TREE_DEPTH){ 
		#if not (defined(ACTSMODEL_LW) && defined(ACTSMODEL_LWTYPE2))
		// scankeyvalues("actslw::topkernel::", (keyvalue_t *)(&kvdram[sweepparams.workdestbaseaddress_kvs]), globaldestoffsets, (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP)), BATCH_RANGE / pow(NUM_PARTITIONS, sweepparams.currentLOP));// (1 << (NUM_PARTITIONS_POW * sweepparams.currentLOP))); 
		scankeyvalues("actslw::topkernel::", (keyvalue_t *)(&kvdram[sweepparams.workdestbaseaddress_kvs]), globaldestoffsets, NUM_PARTITIONS, BATCH_RANGE / pow(NUM_PARTITIONS, sweepparams.currentLOP), sweepparams.upperlimit);
		#endif 
	}
	#endif
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
void actsutility::setkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, keyvalue_t keyvalue){
	cout<<"setting keyvalues... "<<endl;
	for(unsigned int i=0; i<size; i++){ keyvalues[i] = keyvalue; }
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
	globalvar_errorsingetpartition = 0;
	globalvar_errorsinreduce = 0;
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
	cout<<"actsutility::scankeyvalues::"<<message<<" numberofpartitions: "<<numberofpartitions<<", rangeperpartition: "<<rangeperpartition<<endl;
	for(unsigned int i=0; i<numberofpartitions; i++){
		unsigned int lowerrangeindex = upperlimit + (i * rangeperpartition);
		unsigned int upperrangeindex = upperlimit + ((i+1) * rangeperpartition);
		unsigned int begin = stats[i].key;
		unsigned int end = stats[i].key + stats[i].value;
		unsigned int numerrorkeys = geterrorkeyvalues(keyvalues, begin, end, lowerrangeindex, upperrangeindex);
		cout<<"actsutility::scankeyvalues:: "<<numerrorkeys<<" errors seen for partition "<<i<<". ["<<lowerrangeindex<<" -> "<<upperrangeindex<<"]("<<begin<<" -> "<<end<<")("<<(end-begin)<<" values)"<<endl<<endl;
	}
	return;
}
unsigned int actsutility::geterrorkeyvalues(keyvalue_t * keyvalues, unsigned int begin, unsigned int end, unsigned int lowerrangeindex, unsigned int upperrangeindex){
	unsigned int numerrorkeys = 0;
	for(unsigned int i=begin; i<end; i++){
		if(keyvalues[i].key != INVALIDDATA){
			if(keyvalues[i].key < lowerrangeindex || keyvalues[i].key >= upperrangeindex){
				if(numerrorkeys < 8){ cout<<"actsutility::geterrorkeyvalues::ERROR KEYVALUE keyvalues["<<i<<"].key: "<<keyvalues[i].key<<", keyvalues["<<i<<"].value: "<<keyvalues[i].value<<endl; }
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
void actsutility::globalstats_counterrorsingetpartition(unsigned int count){
	globalvar_errorsingetpartition += count;
	return;
}
void actsutility::globalstats_counterrorsinreduce(unsigned int count){
	globalvar_errorsinreduce += count;
	return;
}













