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
#include "../src/common.h"
#include "utility.h"
using namespace std;

utility::utility(){}
utility::~utility(){} 

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
void utility::stopTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx){
	// #ifdef _DEBUGMODE_HOSTPRINTS
	std::chrono::steady_clock::time_point endtime = std::chrono::steady_clock::now();		
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::nanoseconds> (endtime - begintime).count() << "[nanao seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;			
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::microseconds> (endtime - begintime).count() << "[micro seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds> (endtime - begintime).count() << "[milli seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;
	std::cout << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::seconds> (endtime - begintime).count() << "[seconds]" << "(Iteration "<<iteration_idx<<")"<< std::endl;
	std::cout << std::endl;
	// #endif
	return;
}
vertex_t utility::GETKVDATA_RANGE_FORSSDPARTITION_(unsigned int ssdpartitonid){
	return KVDATA_RANGE_PERSSDPARTITION;
}	
vertex_t utility::GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(unsigned int ssdpartitonid){
	vertex_t rangeoffset = 0;
	
if(ssdpartitonid == 0){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION0;
	}
else if(ssdpartitonid == 1){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION1;
	}
else if(ssdpartitonid == 2){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION2;
	}
else if(ssdpartitonid == 3){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION3;
	}
else if(ssdpartitonid == 4){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION4;
	}
else if(ssdpartitonid == 5){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION5;
	}
else if(ssdpartitonid == 6){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION6;
	}
else if(ssdpartitonid == 7){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION7;
	}
else if(ssdpartitonid == 8){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION8;
	}
else if(ssdpartitonid == 9){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION9;
	}
else if(ssdpartitonid == 10){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION10;
	}
else if(ssdpartitonid == 11){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION11;
	}
else if(ssdpartitonid == 12){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION12;
	}
else if(ssdpartitonid == 13){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION13;
	}
else if(ssdpartitonid == 14){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION14;
	}
else if(ssdpartitonid == 15){
		rangeoffset = KVDATA_RANGEOFFSET_FORSSDPARTITION15;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"utility::GETRANGE should never get here. GETRANGE 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif
	}
	return rangeoffset;
}
vertex_t utility::GETAPPLYVERTEXBUFFERSZ_FORSSDPARTITION_(unsigned int ssdpartitonid){
	return APPLYVERTEXBUFFERSZ_PERSSDPARTITION;
}
unsigned int utility::GETTREEDEPTH_(unsigned int ssdpartitonid){
	return TREE_DEPTH;
}
int utility::hallignup_KV(int val){
	int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
int utility::halligndown_KV(int val){
	int fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
void utility::printkvdrams(uint512_vec_dt * kvdram){
	// Print results after Kernel run
	for(unsigned int i = 0 ; i<(16/VECTOR_SIZE) ; i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			std::cout<<"kvdram["<<i<<"].data["<<j<<"].key: "<<kvdram[i].data[j].key<<""<<std::endl;
		}
    }
	for(unsigned int i = 0 ; i<(16/VECTOR_SIZE) ; i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			std::cout<<"kvdram["<<(PADDEDKVDATA_BATCHSIZE_KVS + i)<<"].data["<<j<<"].key: "<<kvdram[(PADDEDKVDATA_BATCHSIZE_KVS + i)].data[j].key<<""<<std::endl;
		}
    }
	for(unsigned int i = 0 ; i<(16/VECTOR_SIZE) ; i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			std::cout<<"kvdram["<<((2 * PADDEDKVDATA_BATCHSIZE_KVS) + i)<<"].data["<<j<<"].value: "<<kvdram[((2 * PADDEDKVDATA_BATCHSIZE_KVS) + i)].data[j].value<<""<<std::endl;
		}
    }
	std::cout<<std::endl;
	return;
}
void utility::printstats(unsigned int size, metadata_t * kvstats){
	unsigned int totalsize = 0;
	for(unsigned int p = 0 ; p<size; p++){ std::cout<<"kvstats["<<p<<"]: [offset: "<<kvstats[p].offset<<", size:"<<kvstats[p].size<<"]"<<std::endl; }
	// for(unsigned int p = 0 ; p<size; p++){ if(kvstats[p].size > 0){ std::cout<<"kvstats["<<p<<"]: [offset: "<<kvstats[p].offset<<", size:"<<kvstats[p].size<<"]"<<std::endl; }}
	for(unsigned int p = 0 ; p<KVSTATS_SIZE; p++){ totalsize += kvstats[p].size; } 
	cout<<"utility::printstats: total number of kvs in kvstats: "<<totalsize<<endl;
	std::cout<<std::endl;
	return;
}
void utility::printkvdramsII(uint512_vec_dt * kvdram, metadata_t * kvstats){
	#ifdef _DEBUGMODE_CHECKS
	for(unsigned int i=0; i<NUMSSDPARTITIONS; i++){
	cout<<">>> partition: "<<i<<endl;
		for(unsigned int k = ((PADDEDKVDATA_BATCHSIZE + kvstats[i].offset) / VECTOR_SIZE) ; k<(((PADDEDKVDATA_BATCHSIZE + kvstats[i].offset) / VECTOR_SIZE) + 2) ; k++){
			for(unsigned int j=0; j<VECTOR_SIZE; j++){
				std::cout<<"kvdram["<<k<<"].data["<<j<<"].key: "<<kvdram[k].data[j].key<<std::endl;
			}
		}
	}
	#endif 
	return;
}
void utility::checkkvdrams(keyvalue_t * kvdram, metadata_t * kvstats){
	cout<<"utility::checkkvdrams: checking results in kvdram for errors..."<<endl;
	unsigned int partitionrange = KVDATA_RANGE_PERSSDPARTITION / KVSTATS_SIZE;
	unsigned int baseaddress = PADDEDKVDATA_BATCHSIZE;
	for(unsigned int i=0; i<KVSTATS_SIZE; i++){
		for(unsigned int k=kvstats[i].offset; k<(kvstats[i].offset + kvstats[i].size); k++){
			if((kvdram[baseaddress + k].key < (i * partitionrange)) || (kvdram[baseaddress + k].key > ((i+1) * partitionrange))){ cout<<"utility::checkkvdrams. out of range 45. i: "<<i<<", kvdram["<<(baseaddress + k)<<"].key: "<<kvdram[baseaddress + k].key<<", partitionrange: "<<partitionrange<<", kvstats["<<i<<"].offset: "<<kvstats[i].offset<<", kvstats["<<i<<"].size: "<<kvstats[i].size<<endl; exit(EXIT_FAILURE); }				
		}
	}
	unsigned int totalnumkeyvalues;
	for(unsigned int p = 0 ; p<KVSTATS_SIZE; p++){ totalnumkeyvalues += kvstats[p].size; }
	cout<<"utility::checkkvdrams: successful."<<endl;
	return;
}
void utility::checkkvstats(metadata_t * kvstats, unsigned int _totalnumkeyvalues){
	cout<<"utility::checkkvstats: checking results in kvstats for errors. _totalnumkeyvalues: "<<_totalnumkeyvalues<<endl;
	unsigned int totalnumkeyvalues;
	for(unsigned int p = 0 ; p<KVSTATS_SIZE; p++){ totalnumkeyvalues += kvstats[p].size; }
	if(totalnumkeyvalues != _totalnumkeyvalues){ cout<<"utility::checkkvstats. ERROR: total number of kvstats does not match. totalnumkeyvalues: "<<totalnumkeyvalues<<", _totalnumkeyvalues: "<<_totalnumkeyvalues<<endl; }
	cout<<"utility::checkkvstats: successful. totalnumkeyvalues: "<<totalnumkeyvalues<<endl;
	return;
}
void utility::printtempverticesdata(uint512_vec_dt * kvdram){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"utility::printtempverticesdata: printing reduced vertices in temp vertices data..."<<endl;
	#endif 
	unsigned int numreducedvertexproperties = 0;
	for(unsigned int i=(2*PADDEDKVDATA_BATCHSIZE_KVS); i<((2*PADDEDKVDATA_BATCHSIZE_KVS) + (KVDATA_RANGE_PERSSDPARTITION / VECTOR_SIZE)); i++){
		for(unsigned int k=0; k<VECTOR_SIZE; k++){
			#ifdef PR_ALGORITHM
			if(kvdram[i].data[k].value != 0){ numreducedvertexproperties+=1; 
				// cout<<"PR --- utility::printtempverticesdata: reduced vertex found: kvdram["<<i<<"].data["<<k<<"].value: "<<kvdram[i].data[k].value<<endl; 
			}
			#else
			if(kvdram[i].data[k].value != INFINITI){ numreducedvertexproperties+=1; 
				// cout<<"BFS --- utility::printtempverticesdata: reduced vertex found: kvdram["<<i<<"].data["<<k<<"].value: "<<kvdram[i].data[k].value<<endl; 
				// if(numreducedvertexproperties > 828){ cout<<"too many. break"<<endl; exit(EXIT_FAILURE); }
			}
			#endif
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"utility::printtempverticesdata: total number of reduced vertices: "<<numreducedvertexproperties<<endl;
	#endif 
	return;
}
/** void utility::checkkvdrams(uint512_vec_dt * kvdram, metadata_t * kvstats){
	cout<<"utility::checkkvdrams: checking results in kvdram for errors..."<<endl;
	// #ifdef _DEBUGMODE_CHECKS
	unsigned int partitionrange = KVDATA_RANGE_FORSSDPARTITION0 / KVSTATS_SIZE;
	unsigned int baseaddress = PADDEDKVDATA_BATCHSIZE_KVS;
	for(unsigned int i=0; i<KVSTATS_SIZE; i++){
		for(unsigned int k=(kvstats[i].offset / VECTOR_SIZE); k<((kvstats[i].offset + kvstats[i].size) / VECTOR_SIZE); k++){
			for(unsigned int t=0; t<VECTOR_SIZE; t++){
				if((kvdram[baseaddress + k].data[t].key < (i * partitionrange)) || (kvdram[baseaddress + k].data[t].key > ((i+1) * partitionrange))){ 
					cout<<"utility::printkvdramsIII. out of range 45. i: "<<i<<", kvdram["<<(baseaddress + k)<<"].data["<<t<<"].key: "<<kvdram[baseaddress + k].data[t].key<<", partitionrange: "<<partitionrange<<", kvstats["<<i<<"].offset: "<<kvstats[i].offset<<", kvstats["<<i<<"].size: "<<kvstats[i].size<<endl; 
					exit(EXIT_FAILURE); 
				}		
			}			
		}
	}
	// #endif 
	return;
} */
void utility::recordstats(metadata_t * kvstats){
	for(unsigned int p=0; p<KVSTATS_SIZE; p++){
		globaldebugger_totalkeyvaluesstransferredfromkernel += kvstats[p].size;
		globaldebugger_totalkvstats[p] += kvstats[p].size; 
	}
	return;
}
void utility::printbits(int fd, size_t fileoffset, unsigned int size){
	cout<<"utility::printbits. "<<endl;
	char * buffer = new char[size];
	if(pread(fd, buffer, (size * sizeof(char)), fileoffset) <= 0){ cout<<"utility::printbits::ERROR 35"<<endl; exit(EXIT_FAILURE); }
	for(unsigned int i=0; i<size; i++){ for(unsigned int k=0; k<NUMBITSINCHAR; k++){ cout<<READFROM(buffer[i], k, 1)<<", "; } cout<<endl; }
	return;
}
void utility::printbits(char * buffer, unsigned int size){
	cout<<"utility::printbits2. "<<endl;
	for(unsigned int i=0; i<size; i++){ for(unsigned int k=0; k<NUMBITSINCHAR; k++){ cout<<READFROM(buffer[i], k, 1)<<", "; } cout<<endl; }
	return;
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










