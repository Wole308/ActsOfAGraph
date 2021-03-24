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
#include <bits/stdc++.h> 
#include <iostream> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include "../../examples/include/examplescommon.h"
#include "../../acts/include/actscommon.h" //
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

void utility::printallparameters(){
	#ifdef USEHBMMEMORY
	std::cout<<"host:: USEHBMMEMORY DEFINED"<<std::endl;
	#else 
	std::cout<<"host:: USEDDRAMMEMORY DEFINED"<<std::endl;	
	#endif
	std::cout<<"host:: NUMDRAMBANKS: "<<NUMDRAMBANKS<<std::endl;
	std::cout<<"host:: NUMWORKERS: "<<NUMWORKERS<<std::endl;
	std::cout<<"host:: NUMSUBWORKERS: "<<NUMSUBWORKERS<<std::endl;
	std::cout<<"host:: NUM_PARTITIONS: "<<NUM_PARTITIONS<<std::endl;
	std::cout<<"host:: NUMSUBCPUTHREADS: "<<NUMSUBCPUTHREADS<<std::endl;
	std::cout<<"host:: DATAWIDTH: "<<DATAWIDTH<<std::endl;
	std::cout<<"host:: VECTOR_SIZE: "<<VECTOR_SIZE<<std::endl;
	std::cout<<"host:: KVDATA_RANGE: "<<KVDATA_RANGE<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_POW: "<<KVDATA_RANGE_POW<<std::endl;
	std::cout<<"host:: BATCH_RANGE: "<<BATCH_RANGE<<std::endl;
	std::cout<<"host:: BATCH_RANGE_POW: "<<BATCH_RANGE_POW<<std::endl;
	std::cout<<"host:: TREE_DEPTH: "<<TREE_DEPTH<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_POW: "<<KVDATA_RANGE_POW<<std::endl;
	std::cout<<"host:: KVDATA_RANGE: "<<KVDATA_RANGE<<std::endl;
	std::cout<<"host:: MESSAGES_SIZE: "<<MESSAGES_SIZE<<std::endl;
	#ifdef LOCKE
	std::cout<<"host:: LOCKE DEFINED"<<std::endl;
	#else 
	std::cout<<"host:: LOCKE NOT DEFINED"<<std::endl;	
	#endif
	std::cout<<"host:: sizeof(keyvalue_t): "<<sizeof(keyvalue_t)<<" bytes ("<<(sizeof(keyvalue_t) * 8)<<" bits)"<<std::endl;
	std::cout<<"host:: sizeof(vertex_t): "<<sizeof(vertex_t)<<" bytes ("<<(sizeof(vertex_t) * 8)<<" bits)"<<std::endl;
	std::cout<<"host:: sizeof(edge_t): "<<sizeof(edge_t)<<" bytes ("<<(sizeof(edge_t) * 8)<<" bits)"<<std::endl;
	
	std::cout<<"host:: MESSAGESDRAMSZ: "<<MESSAGESDRAMSZ<<std::endl;
	std::cout<<"host:: VERTEXPTRSSZ: "<<VERTEXPTRSSZ<<std::endl;
	std::cout<<"host:: VERTEXPTRSSZ_KVS: "<<VERTEXPTRSSZ_KVS<<std::endl;
	std::cout<<"host:: VERTICESDATASZ: "<<VERTICESDATASZ<<std::endl;
	std::cout<<"host:: VERTICESDATASZ_KVS: "<<VERTICESDATASZ_KVS<<std::endl;

	// >>>
	std::cout<<">> host[baseoffsets]:: BASEOFFSET_MESSAGESDATA: "<<BASEOFFSET_MESSAGESDATA<<std::endl;
	std::cout<<">> host[baseoffsets]:: BASEOFFSET_MESSAGESDATA_KVS: "<<BASEOFFSET_MESSAGESDATA_KVS<<std::endl;
	
	std::cout<<">> host[baseoffsets]:: KVSOURCEDRAMSZ: "<<KVSOURCEDRAMSZ<<std::endl;	
	std::cout<<">> host[baseoffsets]:: KVSOURCEDRAMSZ_KVS: "<<KVSOURCEDRAMSZ_KVS<<std::endl;	
	std::cout<<">> host[baseoffsets]:: PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<std::endl;	
	std::cout<<">> host[baseoffsets]:: PADDEDKVSOURCEDRAMSZ_KVS: "<<PADDEDKVSOURCEDRAMSZ_KVS<<std::endl;
	
	std::cout<<">> host[baseoffsets]:: VDRAMSZ: "<<VDRAMSZ<<std::endl;	
	std::cout<<">> host[baseoffsets]:: PADDEDVDRAMSZ: "<<PADDEDVDRAMSZ<<std::endl;	
	std::cout<<">> host[baseoffsets]:: PADDEDVDRAMSZ_KVS: "<<PADDEDVDRAMSZ_KVS<<std::endl;
	
	// >>>
	std::cout<<">> host[sizes]:: MESSAGESDRAMSZ (keyvalues): "<<MESSAGESDRAMSZ<<" keyvalues"<<std::endl;
	std::cout<<">> host[sizes]:: VERTEXPTRSSZ (keyvalues): "<<VERTEXPTRSSZ<<" keyvalues"<<std::endl;
	std::cout<<">> host[sizes]:: VERTICESDATASZ (keyvalues): "<<VERTICESDATASZ<<" keyvalues"<<std::endl;
	std::cout<<">> host[sizes]:: ACTIVEVERTICESSZ (keyvalues): "<<ACTIVEVERTICESSZ<<" keyvalues"<<std::endl;
	std::cout<<">> host[sizes]:: VERTICESDATAMASKSZ (keyvalues): "<<VERTICESDATAMASKSZ<<" keyvalues"<<std::endl;
	std::cout<<">> host[sizes]:: KVSTATSDRAMSZ (keyvalues): "<<KVSTATSDRAMSZ<<" keyvalues"<<std::endl;
	std::cout<<">> host[sizes]:: PADDEDVDRAMSZ (keyvalues): "<<PADDEDVDRAMSZ<<" keyvalues"<<std::endl;
	std::cout<<">> host[sizes]:: PADDEDKVSOURCEDRAMSZ (keyvalues): "<<PADDEDKVSOURCEDRAMSZ<<" keyvalues"<<std::endl;
	
	// >>>
	std::cout<<">> host[bytes]:: MESSAGESDRAMSZ (bytes): "<<MESSAGESDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host[bytes]:: KVSTATSDRAMSZ (bytes): "<<KVSTATSDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host[bytes]:: VERTEXPTRSSZ (bytes): "<<VERTEXPTRSSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host[bytes]:: VERTICESDATASZ (bytes): "<<VERTICESDATASZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host[bytes]:: ACTIVEVERTICESSZ (bytes): "<<ACTIVEVERTICESSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host[bytes]:: VERTICESDATAMASKSZ (bytes): "<<VERTICESDATAMASKSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host[bytes]:: PADDEDVDRAMSZ (bytes): "<<PADDEDVDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host[bytes]:: PADDEDKVSOURCEDRAMSZ (bytes): "<<PADDEDKVSOURCEDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	
	std::cout<<"host:: KVSTATSDRAMSZ: "<<KVSTATSDRAMSZ<<std::endl;
	std::cout<<"host:: NUMLASTLEVELPARTITIONS: "<<NUMLASTLEVELPARTITIONS<<std::endl;
	
	std::cout<<"host:: REDUCESZ_POW: "<<REDUCESZ_POW<<std::endl;
	std::cout<<"host:: REDUCESZ: "<<REDUCESZ<<std::endl;
	std::cout<<"host:: REDUCEBUFFERSZ: "<<REDUCEBUFFERSZ<<std::endl;
	
	std::cout<<"host:: VMASKBUFFERSZ_KVS: "<<VMASKBUFFERSZ_KVS<<std::endl;
	
	std::cout<<"host:: NUMLASTLEVELPARTITIONS: "<<NUMLASTLEVELPARTITIONS<<std::endl;
	
	#ifdef ACTSFAST
	std::cout<<"host::ACTSFAST DEFINED"<<std::endl;
	#else 
	std::cout<<"host::ACTSFAST NOT DEFINED"<<std::endl;	
	#endif 
	#ifdef MAXPERFORMANCE
	std::cout<<"host::MAXPERFORMANCE DEFINED"<<std::endl;
	#else 
	std::cout<<"host::MAXPERFORMANCE NOT DEFINED"<<std::endl;	
	#endif 
	#ifdef MERGEEDGESANDKVDRAMWORKSPACE
	std::cout<<"host::MERGEEDGESANDKVDRAMWORKSPACE DEFINED"<<std::endl;
	#else 
	std::cout<<"host::MERGEEDGESANDKVDRAMWORKSPACE NOT DEFINED"<<std::endl;	
	#endif
	std::cout<<"host::COLLECTSTATSOFFLINE DEFINED"<<std::endl;
	#ifdef EMBEDDEDCOLLECTSTATS
	std::cout<<"host::EMBEDDEDCOLLECTSTATS DEFINED"<<std::endl;
	#else 
	std::cout<<"host::EMBEDDEDCOLLECTSTATS NOT DEFINED"<<std::endl;	
	#endif
	
	// exit(EXIT_SUCCESS);
	return;
}
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
	unsigned int totalkeys = 0;
	unsigned int totalvalues = 0;
	for(unsigned int i=0; i<size; i++){ cout<<"keyvalues["<<i<<"].key: "<<keyvalues[i].key<<", keyvalues["<<i<<"].value: "<<keyvalues[i].value<<endl; totalkeys+=keyvalues[i].key; totalvalues+=keyvalues[i].value; }
	cout<<"total keys: "<<totalkeys<<", totalvalues: "<<totalvalues<<endl;
}
void utility::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int skipsize){
	if(skipsize == 0){ cout<<endl<<"utility::printkeyvalues:ERROR: skipsize CANNOT be zero. exiting... "<<endl; exit(EXIT_FAILURE); }
	unsigned int totalkeys = 0;
	unsigned int totalvalues = 0;
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p+=skipsize){ cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; totalkeys+=keyvalues[p].key; totalvalues+=keyvalues[p].value; }
	cout<<"total keys: "<<totalkeys<<", totalvalues: "<<totalvalues<<endl;
}
void utility::printkeyvalues(string message, keyvalue_t * keyvalues[VECTOR_SIZE], unsigned int size){
	cout<<endl<<"utility::printkeyvalues:"<<message<<endl;
	unsigned int totalkeys = 0;
	unsigned int totalvalues = 0;
	for(unsigned int v=0; v<VECTOR_SIZE; v++){
		for(unsigned int i=0; i<size; i++){ cout<<"keyvalues["<<v<<"]["<<i<<"].key: "<<keyvalues[v][i].key<<", keyvalues["<<v<<"]["<<i<<"].value: "<<keyvalues[v][i].value<<endl; totalkeys+=keyvalues[v][i].key; totalvalues+=keyvalues[v][i].value; }				
		cout<<".."<<endl;
	}
	cout<<"total keys: "<<totalkeys<<", totalvalues: "<<totalvalues<<endl;
}
void utility::printkeyvalues(string message, keyvalue_t keyvalues[VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int numcols, unsigned int size){
	cout<<endl<<"utility::printkeyvalues:"<<message<<endl;
	for(unsigned int v=0; v<numcols; v++){
		for(unsigned int i=0; i<size; i++){ cout<<"keyvalues["<<v<<"]["<<i<<"].key: "<<keyvalues[v][i].key<<", keyvalues["<<v<<"]["<<i<<"].value: "<<keyvalues[v][i].value<<endl; }
		cout<<".."<<endl;
	}
}
void utility::printkeyvalues(string message, keyvalue_vec_bittype keyvalues[NUM_PARTITIONS][BLOCKRAM_SIZE], unsigned int numcols, unsigned int size){
	cout<<endl<<"utility::printkeyvalues:"<<message<<endl;
	for(unsigned int v=0; v<numcols; v++){
		for(unsigned int i=0; i<size; i++){ cout<<"keyvalues["<<v<<"]["<<i<<"].key: "<<keyvalues[v][i].key<<", keyvalues["<<v<<"]["<<i<<"].value: "<<keyvalues[v][i].value<<endl; }
		cout<<".."<<endl;
	}
}
void utility::printedges(string message, edge2_type * edges, unsigned int size){
	cout<<endl<<"utility::printedges:"<<message<<endl;
	for(unsigned int i=0; i<size; i++){ cout<<"edges["<<i<<"].srcvid: "<<edges[i].srcvid<<", edges["<<i<<"].dstvid: "<<edges[i].dstvid<<endl; }
}
void utility::printmessages(string message, uint512_vec_dt * keyvalues){
	cout<<"utility::printmessages::"<<message<<":: printing messages (after kernel launch) "<<endl;
	cout<<"MESSAGES_ENABLE_RUNKERNELCOMMAND: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key<<endl;
	cout<<"MESSAGES_ENABLE_PROCESSCOMMAND: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key<<endl;
	cout<<"MESSAGES_ENABLE_PARTITIONCOMMAND: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key<<endl;
	cout<<"MESSAGES_ENABLE_APPLYUPDATESCOMMAND: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key<<endl;
	cout<<"MESSAGES_ACTSPARAMS_SRCVOFFSET: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].data[0].key<<endl;
	cout<<"MESSAGES_ACTSPARAMS_SRCVSIZE: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].data[0].key<<endl;
	cout<<"MESSAGES_ACTSPARAMS_DESTVOFFSET: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].data[0].key<<endl;
	cout<<"MESSAGES_ACTSPARAMS_TREEDEPTH: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key<<endl;
	cout<<"MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].data[0].key<<endl;
	cout<<"MESSAGES_ALGORITHMINFO_GRAPHITERATIONID: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key<<endl;
	cout<<"MESSAGES_SIZE_RUN: "<<keyvalues[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl;
	return;
}
void utility::printvalues(string message, unsigned int * values, unsigned int size){
	cout<<"utility::printvalues::"<<message<<":: printing first "<<size<< " values" <<endl;
	unsigned int count = 0;
	unsigned int totalvalues = 0;
	for(unsigned int i=0; i<size; i++){
		cout<<"values["<<i<<"]: "<<values[i]<<endl;
		totalvalues += values[i];
	}
	cout<<"printvalues:: total values: totalvalues: "<<totalvalues<<endl;
}
void utility::printvalues(string message, vector<value_t> & values, unsigned int size){
	cout<<"utility::printvalues::"<<message<<":: printing first "<<size<< " values" <<endl;
	unsigned int count = 0;
	unsigned int totalvalues = 0;
	for(unsigned int i=0; i<size; i++){
		cout<<"values["<<i<<"]: "<<values[i]<<endl;
		totalvalues += values[i];
	}
	cout<<"printvalues:: total values: totalvalues: "<<totalvalues<<endl;
}
void utility::printvaluesgreaterthan(string message, unsigned int * values, unsigned int size, unsigned int threshold){
	unsigned int count = 0;
	unsigned int totalsize = 0;
	int firstindex = -1;
	int lastindex = -1;
	for(unsigned int i=0; i<size; i++){
		if(values[i] >= threshold){
			if(firstindex == -1){ firstindex = i; }
			lastindex = i;
			
			count += 1; 
			totalsize += values[i];
			if(count < 16){ cout<<count<<" found:: values["<<i<<"]: "<<values[i]<<endl; }
		}
	}
	cout<<"utility::printvaluesgreaterthan::"<<message<<":: datas with value greater than "<<threshold<<": "<<count<<", (<threshold: "<<(size - count)<<") total size: "<<totalsize<<endl<<endl;
	cout<<"utility::printvaluesgreaterthan::"<<message<<":: first and last indexes: (firstindex: "<<firstindex<<", lastindex: "<<lastindex<<")"<<endl;
}
void utility::printvalueslessthan(string message, unsigned int * values, unsigned int size, unsigned int threshold){
	unsigned int count = 0;
	for(unsigned int i=0; i<size; i++){
		if(values[i] < threshold){ 
			count += 1; 
			if(count < 16){ cout<<count<<" found:: values["<<i<<"]: "<<values[i]<<endl; }
		}
	}
	cout<<"utility::printvalueslessthan::"<<message<<":: datas with value less than "<<threshold<<": "<<count<<endl<<endl;
}
void utility::printstructuresbeforekernelrun(string message, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int size){
	cout<<"utility::printstructuresbeforekernelrun:: printing structures (before kernel launch). "<<message<<endl;
	for(unsigned int i=0; i<size; i++){
		cout<<"utility::printstructuresbeforekernelrun:: printing messages (before kernel launch) for subthread: "<<i<<endl;
		
		uint512_vec_dt * UVEC = (uint512_vec_dt *)kvsourcedram[i];
		cout<<"utility::printstructuresbeforekernelrun:: printing messages (before kernel launch) for subthread: "<<i<<endl;
		cout<<"MESSAGES_ENABLE_RUNKERNELCOMMAND: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key<<endl;
		cout<<"MESSAGES_ENABLE_PROCESSCOMMAND: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key<<endl;
		cout<<"MESSAGES_ENABLE_PARTITIONCOMMAND: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key<<endl;
		cout<<"MESSAGES_ENABLE_APPLYUPDATESCOMMAND: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key<<endl;
		cout<<"MESSAGES_ACTSPARAMS_SRCVOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].data[0].key<<endl;
		cout<<"MESSAGES_ACTSPARAMS_SRCVSIZE: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].data[0].key<<endl;
		cout<<"MESSAGES_ACTSPARAMS_DESTVOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].data[0].key<<endl;
		cout<<"MESSAGES_ACTSPARAMS_TREEDEPTH: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key<<endl;
		cout<<"MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].data[0].key<<endl;
		cout<<"MESSAGES_ALGORITHMINFO_GRAPHITERATIONID: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key<<endl;
		cout<<"MESSAGES_SIZE_RUN: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl;
		
		// printkeyvalues("utility::printstructuresbeforekernelrun:: kvdram", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_KVDRAM_KVS]), 16);
		// printkeyvalues("utility::printstructuresbeforekernelrun:: kvdram workspace", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_KVDRAMWORKSPACE_KVS]), 16);
		// printkeyvalues("utility::printstructuresbeforekernelrun:: edges", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_EDGESDATA_KVS]), 16);
		// printkeyvalues("utility::printstructuresbeforekernelrun:: vertex ptrs", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_VERTEXPTR_KVS]), 16);
		// printkeyvalues("utility::printstructuresbeforekernelrun:: vertex datas", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_VERTICESDATA_KVS]), 16);
		// printkeyvalues("utility::printstructuresbeforekernelrun:: kvstatsdram", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_STATSDRAM_KVS]), 16*8, 8);
	}
}
void utility::printstructuresafterkernelrun(string message, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int size){
	cout<<"utility::printstructuresafterkernelrun:: printing structures (after kernel launch). "<<message<<endl;
	for(unsigned int i=0; i<size; i++){
		uint512_vec_dt * UVEC = (uint512_vec_dt *)kvsourcedram[i];
		cout<<"utility::printstructuresafterkernelrun:: printing messages (after kernel launch) for subthread: "<<i<<endl;
		cout<<"MESSAGES_ENABLE_RUNKERNELCOMMAND: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key<<endl;
		cout<<"MESSAGES_ENABLE_PROCESSCOMMAND: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key<<endl;
		cout<<"MESSAGES_ENABLE_PARTITIONCOMMAND: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key<<endl;
		cout<<"MESSAGES_ENABLE_APPLYUPDATESCOMMAND: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key<<endl;
		cout<<"MESSAGES_ACTSPARAMS_SRCVOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].data[0].key<<endl;
		cout<<"MESSAGES_ACTSPARAMS_SRCVSIZE: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].data[0].key<<endl;
		cout<<"MESSAGES_ACTSPARAMS_DESTVOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].data[0].key<<endl;
		cout<<"MESSAGES_ACTSPARAMS_TREEDEPTH: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key<<endl;
		cout<<"MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].data[0].key<<endl;
		cout<<"MESSAGES_ALGORITHMINFO_GRAPHITERATIONID: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key<<endl;
		cout<<"MESSAGES_SIZE_RUN: "<<UVEC[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key<<endl;
		
		// printkeyvalues("utility::printstructuresafterkernelrun:: kvdram", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_KVDRAM_KVS]), 16);
		// printkeyvalues("utility::printstructuresafterkernelrun:: kvdram workspace", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_KVDRAMWORKSPACE_KVS]), 16);
		// printkeyvalues("utility::printstructuresafterkernelrun:: edges", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_EDGESDATA_KVS]), 16);
		// printkeyvalues("utility::printstructuresafterkernelrun:: vertex ptrs", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_VERTEXPTR_KVS]), 16);
		// printkeyvalues("utility::printstructuresafterkernelrun:: vertex datas", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_VERTICESDATA_KVS]), 16);
		// printkeyvalues("utility::printstructuresafterkernelrun:: kvstatsdram", (keyvalue_t *)(&kvsourcedram[i][BASEOFFSET_STATSDRAM_KVS]), 16*8, 8);
	}
}
void utility::printcontainer(container_t * container){
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		cout<<"utility::printcontainer:: container->edgessize["<<i<<"]: "<<container->edgessize[i]<<endl;
		cout<<"utility::printcontainer:: container->edgeoffset["<<i<<"]: "<<container->edgeoffset[i]<<endl;
		cout<<"utility::printcontainer:: container->srcvoffset["<<i<<"]: "<<container->srcvoffset[i]<<endl;
		cout<<"utility::printcontainer:: container->srcvsize["<<i<<"]: "<<container->srcvsize[i]<<endl;
		cout<<"utility::printcontainer:: container->destvoffset["<<i<<"]: "<<container->destvoffset[i]<<endl;
		cout<<"utility::printcontainer:: container->firstvid["<<i<<"]: "<<container->firstvid[i]<<endl;
	}
}

void utility::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data >= upper_bound){ std::cout<<"utility::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }			
}
void utility::checkforlessthanthan(string message, unsigned int data1, unsigned int data2){
	if(data1 < data2){ cout<<"utility::checkforlessthanthan: ERROR. data1 < data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
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
unsigned int utility::GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(unsigned int ssdpartitonid){
	return (ssdpartitonid * KVDATA_RANGE);
}
void utility::resetkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<"utility::resetkeyvalues:"<<message<<endl;
	for(unsigned int i=0; i<size; i++){ keyvalues[i].key = 0; keyvalues[i].value = 0; }
}
void utility::resetkeyvalues(keyvalue_t * keyvalues, unsigned int size){
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
unsigned int utility::allignlower_KV(unsigned int val){
	unsigned int fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
unsigned int utility::allignhigherto16_KV(unsigned int val){
	unsigned int fac = (val + (16 - 1)) / 16;
	return (fac * 16);
}
unsigned int utility::allignlowerto16_KV(unsigned int val){
	unsigned int fac = val / 16;
	return (fac * 16);
}
void utility::setarray(unsigned int array[NUMSUBCPUTHREADS], unsigned int size, unsigned int value){
	for(unsigned int i = 0; i < size; i++){ array[i] = value; }
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
unsigned int utility::countvalues(string message, keyvalue_t * keyvalue, unsigned int size){
	unsigned int totalcount = 0;
	for(unsigned int i=0; i<size; i++){ totalcount += keyvalue[i].value; }
	cout<<"utility::countvalues::"<<message<<":: total values counted: "<<totalcount<<endl<<endl;
	return totalcount;
}
unsigned int utility::countvaluesgreaterthan(string message, unsigned int * values, unsigned int size, unsigned int threshold){
	unsigned int count = 0;
	unsigned int totalsize = 0;
	for(unsigned int i=0; i<size; i++){
		if(values[i] >= threshold){ 
			count += 1; 
			totalsize += values[i];
			if(count < 16){ cout<<count<<" found:: values["<<i<<"]: "<<values[i]<<endl; }
		}
	}
	cout<<"utility::printvaluesgreaterthan::"<<message<<":: datas with value greater than "<<threshold<<": "<<count<<", (> threshold: "<<(size - count)<<") total size: "<<totalsize<<endl<<endl;
	return count;
}
void utility::scankeyvalues(string message, keyvalue_t * keyvalues, keyvalue_t * stats, unsigned int numberofpartitions, unsigned int rangeperpartition, unsigned int upperlimit){
	cout<<"utility::scankeyvalues::"<<message<<" numberofpartitions: "<<numberofpartitions<<", rangeperpartition: "<<rangeperpartition<<endl;
	for(unsigned int i=0; i<numberofpartitions; i++){
		unsigned int lowerrangeindex = upperlimit + (i * rangeperpartition);
		unsigned int upperrangeindex = upperlimit + ((i+1) * rangeperpartition);
		unsigned int begin = stats[i].key;
		unsigned int end = stats[i].key + stats[i].value;
		unsigned int numerrorkeys = geterrorkeyvalues(keyvalues, begin, end, lowerrangeindex, upperrangeindex);
		cout<<"utility::scankeyvalues:: "<<numerrorkeys<<" errors seen for partition "<<i<<". ["<<lowerrangeindex<<" -> "<<upperrangeindex<<"]("<<begin<<" -> "<<end<<")("<<(end-begin)<<" values)"<<endl<<endl;
	}
	return;
}
unsigned int utility::geterrorkeyvalues(keyvalue_t * keyvalues, unsigned int begin, unsigned int end, unsigned int lowerrangeindex, unsigned int upperrangeindex){
	unsigned int numerrorkeys = 0;
	for(unsigned int i=begin; i<end; i++){
		if(keyvalues[i].key != INVALIDDATA){
			if(keyvalues[i].key < lowerrangeindex || keyvalues[i].key >= upperrangeindex){
				if(numerrorkeys < 8){ cout<<"utility::geterrorkeyvalues::ERROR KEYVALUE keyvalues["<<i<<"].key: "<<keyvalues[i].key<<", keyvalues["<<i<<"].value: "<<keyvalues[i].value<<endl; }
				numerrorkeys += 1;
			}
		}
	}
	return numerrorkeys;
}
void utility::createdirectory(const char* directory){
	if (mkdir(directory, 0777) == -1) {
		cout << "Directory already exists."; 
        // cerr << "Error :  " << strerror(errno) << endl;
	} else{
        cout << "Directory created"; 
	}
	return;
}
void utility::countvalueslessthan(string message, value_t * values, unsigned int size, unsigned int data){
	unsigned int totalcount = 0;
	for(unsigned int i=0; i<size; i++){ if(values[i] < data){ totalcount += 1; }}
	cout<<"utility::"<<message<<"::countvalueslessthan ("<<data<<"):: total values counted: "<<totalcount<<endl;
	return;
}
void utility::paddkeyvalues(keyvalue_t * keyvalues, unsigned int size, unsigned int padddata){
	unsigned int num = VECTOR_SIZE - (size % VECTOR_SIZE);
	for(unsigned v=size; v<(size + num); v++){ keyvalues[v].key = padddata; keyvalues[v].value = padddata; }
	return;
}
void utility::clearkeyvalues(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS]){
	for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){
		for(unsigned int k=0; k<PADDEDKVSOURCEDRAMSZ_KVS; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				kvbuffer[j][k].data[v].key = 0;
				kvbuffer[j][k].data[v].value = 0;
			}
		}
	}
	return;
}
unsigned int utility::getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth){
	// unsigned int partition = (keyvalue.key - upperlimit) / (BATCH_RANGE / pow(NUM_PARTITIONS, treedepth));
	unsigned int partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * treedepth)));
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadgraph::getglobalpartition", partition, (1 << (NUM_PARTITIONS_POW * treedepth)), keyvalue.key, upperlimit, NAp);
	#endif
	return partition;
}

void utility::calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int * skipspacing){
	buffer[0].key += base;
	for(unsigned int i=1; i<size; i++){ buffer[i].key = allignhigher_KV(buffer[i-1].key + buffer[i-1].value + skipspacing[i-1]); }
	return;
}
void utility::getmarkerpositions(keyvalue_t * stats, unsigned int size){
	unsigned int * skipspacing = new unsigned int[size];
	for(unsigned int p=0; p<size; p++){ 
		
		unsigned int A = (stats[p].value + (VECTOR_SIZE-1)) / VECTOR_SIZE; // FIXME. 
		unsigned int B = (A + (SRCBUFFER_SIZE-1)) / SRCBUFFER_SIZE; 
		if(B < 80){ B = B * 2; }
		unsigned int C = ((4 * 4 * 2) * NUM_PARTITIONS) + VECTOR_SIZE; 
		skipspacing[p] = (B * C) + 128; 
		
		#ifdef ALLVERTEXISACTIVE_ALGORITHM
		skipspacing[p] = 0; // skipspacing[p] * 2; // CRITICAL REMOVEME.
		#else 
		skipspacing[p] = 0; // CRITICAL REMOVEME
		#endif 
		
		// cout<<"--- skipspacing["<<p<<"]: "<<skipspacing[p]<<endl;
	}			
	calculateoffsets(stats, size, 0, skipspacing);
	for(unsigned int i=0; i<size-1; i++){ if(stats[i].key + stats[i].value > stats[i+1].key){ cout<<"utility::getmarkerpositions: ERROR: stats["<<i<<"].key("<<stats[i].key<<") + stats["<<i<<"].value("<<stats[i].value<<") >= stats["<<i+1<<"].key("<<stats[i+1].key<<"). exiting..."<<endl; exit(EXIT_FAILURE); }}	
}

void utility::calculateunallignedoffsets(keyvalue_t * keyvalues, unsigned int size){
	for(unsigned int i=1; i<size; i++){
		keyvalues[i].key = keyvalues[i-1].key + keyvalues[i-1].value; 
	}
	return;
}

// compact graph utilities
void utility::DECTOBINARY(int n){ 
    // array to store binary number 
    int binaryNum[32]; 
  
    // counter for binary array 
    int i = 0; 
    while (n > 0) { 
  
        // storing remainder in binary array 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    } 
  
    // printing binary array in reverse order 
    for (int j = i - 1; j >= 0; j--){
        cout << binaryNum[j]; 
	}
	return;
} 
void utility::ULONGTOBINARY(unsigned long n){ 
    // array to store binary number 
    int binaryNum[64]; 
  
    // counter for binary array 
    int i = 0; 
    while (n > 0) { 
  
        // storing remainder in binary array 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    } 
  
    // printing binary array in reverse order 
	cout<<"utility::ULONGTOBINARY: "<<(unsigned long)n<<" in decimal is: ";
    for (int j = i - 1; j >= 0; j--){
        cout << binaryNum[j]; 
	}
	cout<<endl;
	return;
} 
void utility::ULONGTOBINARY(keyvalue_t keyvalue){ 
	unsigned long * longword = (unsigned long *)&keyvalue;
	ULONGTOBINARY(*longword);
	return;
} 
unsigned int utility::GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned long utility::GETMASK_ULONG(unsigned long index, unsigned long size){
	unsigned long A = ((1 << (size)) - 1);
	unsigned long B = A << index;
	return B;
}
unsigned int utility::READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & GETMASK_UINT((index), (size))) >> (index)); 
}
unsigned int utility::READFROM_ULONG(unsigned long data, unsigned long index, unsigned long size){ 
	return (((data) & GETMASK_ULONG((index), (size))) >> (index)); 
}
unsigned int utility::READFROM_ULONG(keyvalue_t keyvalue, unsigned long index, unsigned long size){
	unsigned long * data = (unsigned long *)&keyvalue;
	return READFROM_ULONG(*data, index, size);
}
void utility::WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){ 
	unsigned int tempdata = *data;
	(tempdata) = ((tempdata) & (~GETMASK_UINT((index), (size)))) | ((value) << (index));
	*data = tempdata;
	return; 
}
void utility::WRITETO_ULONG(unsigned long * data, unsigned long index, unsigned long size, unsigned long value){ 
	unsigned long tempdata = *data;
	(tempdata) = ((tempdata) & (~GETMASK_ULONG((index), (size)))) | ((value) << (index));
	*data = tempdata;
	return; 
}
void utility::WRITETO_ULONG(keyvalue_t * keyvalue, unsigned long index, unsigned long size, unsigned long value){ 
	unsigned long * data = (unsigned long *)keyvalue;
	return WRITETO_ULONG(data, index, size, value);
	return; 
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

void utility::collectedgestats(vector<vertex_t> &srcvids, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, unsigned int * edges_count, unsigned int * edgesdstv_sum){
	*edges_count = 0;
	*edgesdstv_sum = 0;
	for(unsigned int i=0; i<srcvids.size(); i++){
		unsigned int vid = srcvids[i];
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		for(unsigned int k=0; k<edges_size; k++){
			*edges_count += 1;
			*edgesdstv_sum += edgedatabuffer[vptr_begin + k].dstvid;
		}
	}
	return;
}
unsigned int utility::runsssp_sw(vector<vertex_t> &srcvids, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, unsigned int NumGraphIters){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"utility::runsssp_sw:: running traditional sssp... "<<endl;
	#endif 
	
	unsigned int * labels = new unsigned int[KVDATA_RANGE];
	unsigned int * visitedlabels = new unsigned int[KVDATA_RANGE];
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ labels[i] = 0xFFFFFFFF; }
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ visitedlabels[i] = UNVISITED; }
	vector<value_t> rootactvvs;
	vector<value_t> activevertices;
	for(unsigned int i=0; i<srcvids.size(); i++){ rootactvvs.push_back(srcvids[i]); }
	unsigned int total_edges_processed = 0;
	unsigned int actvvsdstv1_sum = 0;
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"utility::runsssp_sw: number of active vertices for iteration 0: "<<activevertices.size()<<endl;
	#endif
	for(unsigned int i=0; i<rootactvvs.size(); i++){ labels[rootactvvs[i]] = 0; }
	unsigned int ew = 1;
	
	for(unsigned int GraphIter=0; GraphIter<NumGraphIters; GraphIter++){
		actvvsdstv1_sum = 0;
	
		for(unsigned int i=0; i<rootactvvs.size(); i++){
			unsigned int vid = rootactvvs[i];
			edge_t vptr_begin = vertexptrbuffer[vid];
			edge_t vptr_end = vertexptrbuffer[vid+1];
			edge_t edges_size = vptr_end - vptr_begin;
			if(vptr_end < vptr_begin){ continue; }
			// cout<<"utility::runsssp_sw: edges_size: "<<edges_size<<endl;
			
			for(unsigned int k=0; k<edges_size; k++){
				unsigned int dstvid = edgedatabuffer[vptr_begin + k].dstvid;
				#ifdef _DEBUGMODE_CHECKS
				checkoutofbounds("utility::runsssp_sw.vptr_begin + k", vptr_begin + k, 91042010, vid, vptr_begin, vptr_end);
				#endif
				
				unsigned int res = labels[vid] + ew;
				if(res < labels[dstvid]){
					#ifdef _DEBUGMODE_KERNELPRINTS
					cout<<"utility::runsssp_sw: ACTIVE VERTICES seen for iteration "<<GraphIter + 1<<": dstvid: "<<dstvid<<endl;
					#endif
					#ifdef _DEBUGMODE_CHECKS3
					checkoutofbounds("utility::runsssp_sw.dstvid", dstvid, KVDATA_RANGE, NAp, NAp, NAp);
					#endif
					
					labels[dstvid] = res;
					if(visitedlabels[dstvid] == UNVISITED){ 
						#ifdef _DEBUGMODE_KERNELPRINTS
						cout<<"utility::runsssp_sw: ACTIVE VERTICES seen for iteration "<<GraphIter + 1<<": dstvid: "<<dstvid<<endl;
						#endif
						
						visitedlabels[dstvid] = VISITED_IN_CURRENT_ITERATION; 
						activevertices.push_back(dstvid); 
						actvvsdstv1_sum += dstvid; 
					}
				}
				
				total_edges_processed += 1;
			}
		}
		
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"utility::runsssp_sw: number of active vertices for iteration "<<GraphIter + 1<<": "<<activevertices.size()<<" (actvvsdstv1_sum: "<<actvvsdstv1_sum<<")"<<endl;
		for(unsigned int i=0; i<hmin(activevertices.size(), 0); i++){ cout<<"utility::runsssp_sw: activevertices["<<i<<"]: "<<activevertices[i]<<endl; }
		#endif
		
		if(activevertices.size() == 0){ cout<<"no more activer vertices to process. breaking out... "<<endl; break; }
		
		rootactvvs.clear();
		for(unsigned int i=0; i<activevertices.size(); i++){ rootactvvs.push_back(activevertices[i]); }
		activevertices.clear();
		
		for(unsigned int i=0; i<KVDATA_RANGE; i++){ 
			if(visitedlabels[i] == VISITED_IN_CURRENT_ITERATION){ visitedlabels[i] = UNVISITED;  }
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"utility::runsssp_sw: total edges processed: "<<total_edges_processed<<endl;
	#endif 
	delete labels;
	return total_edges_processed; // actvvsdstv1_sum;
}













