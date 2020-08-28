#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include "../../include/common.h"
#include "utility.h"
using namespace std;

#ifdef FPGA_IMPL
#define OCL_CHECK(call)							\
	do {								\
		cl_int err = call;					\
		if (err != CL_SUCCESS) {				\
			cout << "ERR: Error calling " #call		\
				", error code is: " << err << endl;	\
			exit(EXIT_FAILURE);				\
		}							\
	} while (0);
#endif 

utility::utility(){}
utility::~utility(){} 

void utility::print1(string messagea, unsigned int dataa){
	cout<<messagea<<": "<<dataa<<endl;
}
void utility::print2(string messagea, string messageb, unsigned int dataa, unsigned int datab){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<endl;
}
void utility::print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<endl;
}
void utility::print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<endl;
}
void utility::print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<", "<<messagef<<": "<<datef<<endl;
}
void utility::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<endl<<"utility::printkeyvalues:"<<message<<endl;
	for(unsigned int i=0; i<size; i++){ cout<<"keyvalues["<<i<<"].key: "<<keyvalues[i].key<<", keyvalues["<<i<<"].value: "<<keyvalues[i].value<<endl; }
}
void utility::printallparameters(){
	std::cout<<"host:: NUMDRAMBANKS: "<<NUMDRAMBANKS<<std::endl;
	std::cout<<"host:: NUMWORKERS: "<<NUMWORKERS<<std::endl;
	std::cout<<"host:: NUMSUBWORKERS: "<<NUMSUBWORKERS<<std::endl;
	std::cout<<"host:: NUM_PARTITIONS: "<<NUM_PARTITIONS<<std::endl;
	std::cout<<"host:: NUMSUPERCPUTHREADS: "<<NUMSUPERCPUTHREADS<<std::endl;
	std::cout<<"host:: NUMCPUTHREADS: "<<NUMCPUTHREADS<<std::endl;
	std::cout<<"host:: NUMSUBCPUTHREADS: "<<NUMSUBCPUTHREADS<<std::endl;
	std::cout<<"host:: NUMUTILITYTHREADS: "<<NUMUTILITYTHREADS<<std::endl;
	std::cout<<"host:: DATAWIDTH: "<<DATAWIDTH<<std::endl;
	std::cout<<"host:: VECTOR_SIZE: "<<VECTOR_SIZE<<std::endl;
	std::cout<<"host:: KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<std::endl;
	std::cout<<"host:: KVDATA_BATCHSIZE_KVS: "<<KVDATA_BATCHSIZE_KVS<<std::endl;
	std::cout<<"host:: KVDATA_RANGE: "<<KVDATA_RANGE<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_POW: "<<KVDATA_RANGE_POW<<std::endl;
	std::cout<<"host:: BATCH_RANGE: "<<BATCH_RANGE<<std::endl;
	std::cout<<"host:: BATCH_RANGE_POW: "<<BATCH_RANGE_POW<<std::endl;
	std::cout<<"host:: (float)APPROXTREE_DEPTH: "<<(float)APPROXTREE_DEPTH<<std::endl;
	std::cout<<"host:: APPROXTREE_DEPTH: "<<APPROXTREE_DEPTH<<std::endl;
	std::cout<<"host:: TREE_DEPTH: "<<TREE_DEPTH<<std::endl;
	std::cout<<"host:: KVSTATS_SIZE: "<<KVSTATS_SIZE<<std::endl;
	std::cout<<"host:: NUMSSDPARTITIONS: "<<NUMSSDPARTITIONS<<std::endl;	
	std::cout<<"host:: KVDATA_RANGE_PERSSDPARTITION_POW: "<<KVDATA_RANGE_PERSSDPARTITION_POW<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_PERSSDPARTITION: "<<KVDATA_RANGE_PERSSDPARTITION<<std::endl;
	std::cout<<"host:: MESSAGES_SIZE: "<<MESSAGES_SIZE<<std::endl;	
	std::cout<<"host:: NUMSSDPARTITIONS: "<<NUMSSDPARTITIONS<<std::endl;
	std::cout<<"host:: MAXNUMSSDPARTITIONS: "<<MAXNUMSSDPARTITIONS<<std::endl;
	#ifdef LOCKE
	std::cout<<"host:: LOCKE DEFINED"<<std::endl;
	#else 
	std::cout<<"host:: LOCKE NOT DEFINED"<<std::endl;	
	#endif 
	#ifdef STREAMEDGESSETUP
	std::cout<<"host:: STREAMEDGESSETUP DEFINED"<<std::endl;
	#else 
	std::cout<<"host:: STREAMEDGESSETUP NOT DEFINED"<<std::endl;	
	#endif	
	std::cout<<"host:: sizeof(keyvalue_t): "<<sizeof(keyvalue_t)<<" bytes ("<<(sizeof(keyvalue_t) * 8)<<" bits)"<<std::endl;
	std::cout<<"host:: sizeof(vertex_t): "<<sizeof(vertex_t)<<" bytes ("<<(sizeof(vertex_t) * 8)<<" bits)"<<std::endl;
	std::cout<<"host:: sizeof(edge_t): "<<sizeof(edge_t)<<" bytes ("<<(sizeof(edge_t) * 8)<<" bits)"<<std::endl;
	std::cout<<"host:: KVDRAMSZ: "<<KVDRAMSZ<<std::endl;
	std::cout<<"host:: KVDRAMSZ_KVS: "<<KVDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: KVDRAMWORKSPACESZ: "<<KVDRAMWORKSPACESZ<<std::endl;
	std::cout<<"host:: KVDRAMWORKSPACESZ_KVS: "<<KVDRAMWORKSPACESZ_KVS<<std::endl;
	std::cout<<"host:: CAPSULESZ: "<<CAPSULESZ<<std::endl;
	std::cout<<"host:: CAPSULESZ_KVS: "<<CAPSULESZ_KVS<<std::endl;
	std::cout<<"host:: MESSAGESDRAMSZ: "<<MESSAGESDRAMSZ<<std::endl;
	std::cout<<"host:: CAPSULEMETADATADRAMSZ: "<<CAPSULEMETADATADRAMSZ<<std::endl;
	std::cout<<"host:: STATSDRAMSZ: "<<STATSDRAMSZ<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAM: "<<BASEOFFSET_KVDRAM<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAM_KVS: "<<KVDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAMWORKSPACE: "<<KVDRAMWORKSPACESZ<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAMWORKSPACE_KVS: "<<KVDRAMWORKSPACESZ_KVS<<std::endl;	
	std::cout<<"host:: BASEOFFSET_CAPSULES: "<<BASEOFFSET_CAPSULES<<std::endl;
	std::cout<<"host:: BASEOFFSET_CAPSULES_KVS: "<<BASEOFFSET_CAPSULES_KVS<<std::endl;	
	std::cout<<"host:: BASEOFFSET_STATSDRAM: "<<BASEOFFSET_STATSDRAM<<std::endl;
	std::cout<<"host:: KVSOURCEDRAMSZ: "<<KVSOURCEDRAMSZ<<std::endl;	
	std::cout<<"host:: KVSOURCEDRAMSZ_KVS: "<<KVSOURCEDRAMSZ_KVS<<std::endl;	
	std::cout<<"host:: PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<std::endl;	
	std::cout<<"host:: PADDEDKVSOURCEDRAMSZ_KVS: "<<PADDEDKVSOURCEDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: KVSTATSDRAMSZ: "<<KVSTATSDRAMSZ<<std::endl;
	std::cout<<"host:: KVDRAMPADDING: "<<KVDRAMPADDING<<std::endl;
	std::cout<<"host:: APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ<<std::endl;
	std::cout<<"host:: APPLYVERTEXBUFFERSZ_KVS: "<<APPLYVERTEXBUFFERSZ_KVS<<std::endl;
	return;
}

void utility::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data >= upper_bound){ std::cout<<"utility::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }			
}

void utility::stopTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx){
	std::chrono::steady_clock::time_point endtime = std::chrono::steady_clock::now();		
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::nanoseconds> (endtime - begintime).count() << "[nanao seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;			
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::microseconds> (endtime - begintime).count() << "[micro seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds> (endtime - begintime).count() << "[milli seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::seconds> (endtime - begintime).count() << "[seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;
	std::cout << std::endl;
	return;
}
void utility::stopBTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx){
	std::chrono::steady_clock::time_point endtime = std::chrono::steady_clock::now();		
	std::cout << TIMINGRESULTSCOLOR << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::nanoseconds> (endtime - begintime).count() << "[nanao seconds]" << "(Iteration "<<iteration_idx<<")"<< RESET << std::endl;			
	std::cout << TIMINGRESULTSCOLOR << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::microseconds> (endtime - begintime).count() << "[micro seconds]" << "(Iteration "<<iteration_idx<<")"<< RESET << std::endl;
	std::cout << TIMINGRESULTSCOLOR << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds> (endtime - begintime).count() << "[milli seconds]" << "(Iteration "<<iteration_idx<<")"<< RESET << std::endl;
	std::cout << TIMINGRESULTSCOLOR << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::seconds> (endtime - begintime).count() << "[seconds]" << "(Iteration "<<iteration_idx<<")"<< RESET << std::endl;
	std::cout << std::endl;
	return;
}

size_t utility::hsub(size_t val1, size_t val2){
	if(val1 < val2){ return 0; }
	else { return val1 - val2; }
}
size_t utility::hceildiv(size_t val1, size_t val2){
	return (val1 + (val2 - 1)) / val2;
}
int utility::hmin(size_t val1, size_t val2){
	if(val1 < 0){ val1 = 0; }
	if(val2 < 0){ val2 = 0; }
	if(val1 < val2){ return val1; }
	else { return val2; }
}
int utility::hmax(size_t val1, size_t val2){
	if(val1 < 0){ val1 = 0; }
	if(val2 < 0){ val2 = 0; }
	if(val1 > val2){ return val1; }
	else { return val2; }
}
void utility::InsertBit(unsigned int * edgeoffsetbits, size_t index, unsigned int bit){
	unsigned int uinttomodify = index / NUMBITSINUNSIGNEDINT;
	unsigned int position = index % NUMBITSINUNSIGNEDINT;
	
	unsigned int data = edgeoffsetbits[uinttomodify];
	WRITETO(data, position, 1, bit);
	edgeoffsetbits[uinttomodify] = data;
}
unsigned int utility::RetrieveBit(unsigned int * edgeoffsetbits, size_t index){
	unsigned int uinttoread = index / NUMBITSINUNSIGNEDINT;
	unsigned int position = index % NUMBITSINUNSIGNEDINT;
	
	unsigned int data = edgeoffsetbits[uinttoread];
	return READFROM(data, position, 1);
}
int utility::runActs(unsigned int IterCount){
	if((IterCount % DRAMBATCHFACTOR) == (DRAMBATCHFACTOR - 1)){ return 1; }
	else { return 0; }
}
unsigned int utility::GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(unsigned int ssdpartitonid){
	return (ssdpartitonid * KVDATA_RANGE_PERSSDPARTITION);
}
void utility::resetkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<"utility::resetkeyvalues:"<<message<<endl;
	for(unsigned int i=0; i<size; i++){ keyvalues[i].key = 0; keyvalues[i].value = 0; }
}
void utility::allignandappendinvalids(keyvalue_t * buffer, unsigned int size){
	// append INVALID values (edge conditions)
	keyvalue_t NApKV; NApKV.key = INVALIDDATA; NApKV.value = INVALIDDATA;
	
	unsigned int ovsize = allignhigher_KV(size) - size;
	for(unsigned int v=size; v<size + ovsize; v++){
		buffer[v] = NApKV;
	}
	return;
}
unsigned int utility::allignhigher_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
void utility::setarray(unsigned int array[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int _1dimsize, unsigned int _2dimsize, unsigned int value){
	for (int i = 0; i < _1dimsize; i++){ for(unsigned int j = 0; j < _2dimsize; j++){ array[i][j] = value; }}
}
void utility::copy(unsigned int * array1, unsigned int * array2, unsigned int size){
	for(unsigned int i=0; i<size; i++){ array1[i] = array2[i]; }
	return;
}
void utility::countkeyvalueswithvalueequalto(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int value){
	unsigned int count = 0;
	for(unsigned int i=0; i<size; i++){ if(keyvalues[i].value == value){ count += 1; } }
	cout<<"utility::countkeyvalueswithvalueequalto::"<<message<<":: keyvalues with value equal to "<<value<<": "<<count<<endl;
}

#ifdef FPGA_IMPL
void event_cb(cl_event event, cl_int cmd_status, void *data) {
  cl_command_type command;
  clGetEventInfo(event, CL_EVENT_COMMAND_TYPE, sizeof(cl_command_type),
                 &command, nullptr);
  cl_int status;
  clGetEventInfo(event, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(cl_int),
                 &status, nullptr);
  const char *command_str;
  const char *status_str;
	switch (command) {
		case CL_COMMAND_READ_BUFFER:
			command_str = "buffer read";
			break;
		case CL_COMMAND_WRITE_BUFFER:
			command_str = "buffer write";
			break;
		case CL_COMMAND_NDRANGE_KERNEL:
			command_str = "kernel";
			break;
		case CL_COMMAND_MAP_BUFFER:
			command_str = "kernel";
			break;
		case CL_COMMAND_COPY_BUFFER:
			command_str = "kernel";
			break;
		case CL_COMMAND_MIGRATE_MEM_OBJECTS:
			command_str = "buffer migrate";
		  break;
		default:
		command_str = "unknown";
	}
	switch (status) {
		case CL_QUEUED:
			status_str = "Queued";
			break;
		case CL_SUBMITTED:
			status_str = "Submitted";
			break;
		case CL_RUNNING:
			status_str = "Executing";
			break;
		case CL_COMPLETE:
			status_str = "Completed";
			break;
	}
	printf("[%s]: %s %s\n", reinterpret_cast<char *>(data), status_str,
		 command_str);
	fflush(stdout);
}
void utility::set_callback(cl_event event, const char *queue_name) {
  OCL_CHECK(
      clSetEventCallback(event, CL_COMPLETE, event_cb, (void *)queue_name));
}
#endif 








