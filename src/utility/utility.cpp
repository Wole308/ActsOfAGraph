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
	std::cout<<"host:: EDGES_BATCHSIZE: "<<EDGES_BATCHSIZE<<std::endl;
	std::cout<<"host:: EDGES_BATCHSIZE_KVS: "<<EDGES_BATCHSIZE_KVS<<std::endl;
	std::cout<<"host:: PADDEDEDGES_BATCHSIZE: "<<PADDEDEDGES_BATCHSIZE<<std::endl;
	std::cout<<"host:: KVDATA_RANGE: "<<KVDATA_RANGE<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_POW: "<<KVDATA_RANGE_POW<<std::endl;
	std::cout<<"host:: BATCH_RANGE: "<<BATCH_RANGE<<std::endl;
	std::cout<<"host:: BATCH_RANGE_POW: "<<BATCH_RANGE_POW<<std::endl;
	std::cout<<"host:: EDGESSZ: "<<EDGESSZ<<std::endl; 
	
	std::cout<<"host:: TREE_DEPTH: "<<TREE_DEPTH<<std::endl;
	std::cout<<"host:: NUMSSDPARTITIONS: "<<NUMSSDPARTITIONS<<std::endl;	
	std::cout<<"host:: KVDATA_RANGE_PERSSDPARTITION_POW: "<<KVDATA_RANGE_PERSSDPARTITION_POW<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_PERSSDPARTITION: "<<KVDATA_RANGE_PERSSDPARTITION<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_PERSSDPARTITION2_POW: "<<KVDATA_RANGE_PERSSDPARTITION2_POW<<std::endl;
	std::cout<<"host:: KVDATA_RANGE_PERSSDPARTITION2: "<<KVDATA_RANGE_PERSSDPARTITION2<<std::endl;
	std::cout<<"host:: MESSAGES_SIZE: "<<MESSAGES_SIZE<<std::endl;	
	std::cout<<"host:: NUMSSDPARTITIONS: "<<NUMSSDPARTITIONS<<std::endl;
	std::cout<<"host:: NUMSSDPARTITIONS_POW: "<<NUMSSDPARTITIONS_POW<<std::endl;
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
	
	std::cout<<"host:: MESSAGESDRAMSZ: "<<MESSAGESDRAMSZ<<std::endl;
	std::cout<<"host:: KVDRAMSZ: "<<KVDRAMSZ<<std::endl;
	std::cout<<"host:: KVDRAMSZ_KVS: "<<KVDRAMSZ_KVS<<std::endl;
	std::cout<<"host:: KVDRAMWORKSPACESZ: "<<KVDRAMWORKSPACESZ<<std::endl;
	std::cout<<"host:: KVDRAMWORKSPACESZ_KVS: "<<KVDRAMWORKSPACESZ_KVS<<std::endl;
	std::cout<<"host:: EDGESSZ: "<<EDGESSZ<<std::endl;
	std::cout<<"host:: EDGESSZ_KVS: "<<EDGESSZ_KVS<<std::endl;
	std::cout<<"host:: VERTEXPTRSSZ: "<<VERTEXPTRSSZ<<std::endl;
	std::cout<<"host:: VERTEXPTRSSZ_KVS: "<<VERTEXPTRSSZ_KVS<<std::endl;
	std::cout<<"host:: VERTICESDATASZ: "<<VERTICESDATASZ<<std::endl;
	std::cout<<"host:: VERTICESDATASZ_KVS: "<<VERTICESDATASZ_KVS<<std::endl;

	std::cout<<"host:: BASEOFFSET_KVDRAM: "<<BASEOFFSET_KVDRAM<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAM_KVS: "<<BASEOFFSET_KVDRAM_KVS<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAMWORKSPACE: "<<BASEOFFSET_KVDRAMWORKSPACE<<std::endl;
	std::cout<<"host:: BASEOFFSET_KVDRAMWORKSPACE_KVS: "<<BASEOFFSET_KVDRAMWORKSPACE_KVS<<std::endl;	
	std::cout<<"host:: BASEOFFSET_STATSDRAM: "<<BASEOFFSET_STATSDRAM<<std::endl;
	std::cout<<"host:: BASEOFFSET_STATSDRAM_KVS: "<<BASEOFFSET_STATSDRAM_KVS<<std::endl;
	std::cout<<"host:: BASEOFFSET_ACTIVEVERTICES: "<<BASEOFFSET_ACTIVEVERTICES<<std::endl;
	std::cout<<"host:: BASEOFFSET_ACTIVEVERTICES_KVS: "<<BASEOFFSET_ACTIVEVERTICES_KVS<<std::endl;
	std::cout<<"host:: BASEOFFSET_EDGESDATA: "<<BASEOFFSET_EDGESDATA<<std::endl; 
	std::cout<<"host:: BASEOFFSET_EDGESDATA_KVS: "<<BASEOFFSET_EDGESDATA_KVS<<std::endl; 
	std::cout<<"host:: BASEOFFSET_VERTEXPTR: "<<BASEOFFSET_VERTEXPTR<<std::endl;
	std::cout<<"host:: BASEOFFSET_VERTEXPTR_KVS: "<<BASEOFFSET_VERTEXPTR_KVS<<std::endl;
	std::cout<<"host:: BASEOFFSET_VERTICESDATA: "<<BASEOFFSET_VERTICESDATA<<std::endl;
	std::cout<<"host:: BASEOFFSET_VERTICESDATA_KVS: "<<BASEOFFSET_VERTICESDATA_KVS<<std::endl;
	std::cout<<"host:: KVSOURCEDRAMSZ: "<<KVSOURCEDRAMSZ<<std::endl;	
	std::cout<<"host:: KVSOURCEDRAMSZ_KVS: "<<KVSOURCEDRAMSZ_KVS<<std::endl;	
	std::cout<<"host:: PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<std::endl;	
	std::cout<<"host:: PADDEDKVSOURCEDRAMSZ_KVS: "<<PADDEDKVSOURCEDRAMSZ_KVS<<std::endl;
	
	std::cout<<">> host:: MESSAGESDRAMSZ (bytes): "<<MESSAGESDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host:: KVDRAMBUFFERSZ (bytes): "<<KVDRAMBUFFERSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host:: KVDRAMSZ (bytes): "<<KVDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host:: KVDRAMWORKSPACESZ (bytes): "<<KVDRAMWORKSPACESZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host:: KVSTATSDRAMSZ (bytes): "<<KVSTATSDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host:: ACTIVEVERTICESSZ (bytes): "<<ACTIVEVERTICESSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host:: EDGESSZ (bytes): "<<EDGESSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host:: VERTEXPTRSSZ (bytes): "<<VERTEXPTRSSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host:: VERTICESDATASZ (bytes): "<<VERTICESDATASZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	
	std::cout<<">> host:: PADDEDKVSOURCEDRAMSZ (bytes): "<<PADDEDKVSOURCEDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	#if defined(ACTGRAPH_SETUP) && not defined(_GENERATE2DGRAPH)
	// if((PADDEDKVSOURCEDRAMSZ * sizeof(keyvalue_t)) >= (256 * 1024 * 1024)){ cout<<"WARNING: greater than max HBM size (256MB). EXITING..."<<endl; exit(EXIT_FAILURE); }			
	if((PADDEDKVSOURCEDRAMSZ * sizeof(keyvalue_t)) >= (256 * 1024 * 1024)){ cout<<"WARNING: greater than max HBM size (256MB). EXITING..."<<endl; }
	#endif 
	
	std::cout<<"host:: KVSTATSDRAMSZ: "<<KVSTATSDRAMSZ<<std::endl;
	std::cout<<"host:: KVDRAMPADDING: "<<KVDRAMPADDING<<std::endl;
	std::cout<<"host:: APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ<<std::endl;
	std::cout<<"host:: APPLYVERTEXBUFFERSZ_KVS: "<<APPLYVERTEXBUFFERSZ_KVS<<std::endl;
	std::cout<<"host:: NUMLASTLEVELPARTITIONS: "<<NUMLASTLEVELPARTITIONS<<std::endl;
 
	#ifdef INMEMORYGP
	std::cout<<"host:: INMEMORYGP enabled "<<std::endl;
	#endif 
	#ifndef INMEMORYGP
	std::cout<<"host:: INMEMORYGP disabled "<<std::endl;
	#endif
	
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
	#ifdef MERGEPROCESSEDGESANDPARTITIONSTAGE
	std::cout<<"host::MERGEPROCESSEDGESANDPARTITIONSTAGE DEFINED"<<std::endl;
	#else 
	std::cout<<"host::MERGEPROCESSEDGESANDPARTITIONSTAGE NOT DEFINED"<<std::endl;	
	#endif 
	#ifdef COLLECTSTATSOFFLINE
	std::cout<<"host::COLLECTSTATSOFFLINE DEFINED"<<std::endl;
	#else 
	std::cout<<"host::COLLECTSTATSOFFLINE NOT DEFINED"<<std::endl;	
	#endif
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
void utility::printedges(string message, edge2_type * edges, unsigned int size){
	cout<<endl<<"utility::printedges:"<<message<<endl;
	for(unsigned int i=0; i<size; i++){ cout<<"edges["<<i<<"].srcvid: "<<edges[i].srcvid<<", edges["<<i<<"].dstvid: "<<edges[i].dstvid<<endl; }
}
void utility::printpackededges(string message, uuint64_dt * edges, unsigned int size){
	cout<<endl<<"utility::printpackededges:"<<message<<endl;
	for(unsigned int i=0; i<size; i++){
		// printcodedkeyvalue("printpackededges.edges["+std::to_string(i)+"].data", edges[i].data);
	}
}
void utility::printmessages(string message, uint512_vec_dt * keyvalues){
	cout<<"utility::printmessages::"<<message<<":: printing messages (after kernel launch) "<<endl;
	cout<<"MESSAGES_RUNKERNELCOMMANDID: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key<<endl;
	cout<<"MESSAGES_PROCESSCOMMANDID: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key<<endl;
	cout<<"MESSAGES_COLLECTSTATSCOMMANDID: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key<<endl;
	cout<<"MESSAGES_PARTITIONCOMMANDID: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key<<endl;
	cout<<"MESSAGES_APPLYUPDATESCOMMANDID: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key<<endl;
	cout<<"MESSAGES_SRCVOFFSET: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key<<endl;
	cout<<"MESSAGES_SRCVSIZE: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key<<endl;
	cout<<"MESSAGES_DESTVOFFSET: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_DESTVOFFSET].data[0].key<<endl;
	cout<<"MESSAGES_TREEDEPTH: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key<<endl;
	cout<<"MESSAGES_FINALNUMPARTITIONS: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key<<endl;
	cout<<"MESSAGES_GRAPHITERATIONID: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key<<endl;
	cout<<"MESSAGES_BATCHSIZE: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key<<endl;
	cout<<"MESSAGES_RUNSIZE: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl;
	cout<<"MESSAGES_NEXTBATCHOFFSET: "<<keyvalues[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key<<endl;
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
void utility::printstructuresbeforekernelrun(string message, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size){
	cout<<"utility::printstructuresbeforekernelrun:: printing structures (before kernel launch). "<<message<<endl;
	for(unsigned int i=0; i<size; i++){ // NUMSUBCPUTHREADS
		cout<<"utility::printstructuresbeforekernelrun:: printing messages (before kernel launch) for subthread: "<<i<<endl;
		
		uint512_vec_dt * UVEC = (uint512_vec_dt *)kvsourcedram[0][i];
		cout<<"utility::printstructuresafterkernelrun:: printing messages (before kernel launch) for subthread: "<<i<<endl;
		cout<<"MESSAGES_RUNKERNELCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key<<endl;
		cout<<"MESSAGES_PROCESSCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key<<endl;
		cout<<"MESSAGES_COLLECTSTATSCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key<<endl;
		cout<<"MESSAGES_PARTITIONCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key<<endl;
		cout<<"MESSAGES_APPLYUPDATESCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key<<endl;
		cout<<"MESSAGES_SRCVOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key<<endl;
		cout<<"MESSAGES_SRCVSIZE: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key<<endl;
		cout<<"MESSAGES_DESTVOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_DESTVOFFSET].data[0].key<<endl;
		cout<<"MESSAGES_TREEDEPTH: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key<<endl;
		cout<<"MESSAGES_FINALNUMPARTITIONS: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key<<endl;
		cout<<"MESSAGES_GRAPHITERATIONID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key<<endl;
		cout<<"MESSAGES_NEXTBATCHOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key<<endl;
		cout<<"MESSAGES_BATCHSIZE: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key<<endl;
		cout<<"MESSAGES_RUNSIZE: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl;
		cout<<"MESSAGES_NEXTBATCHOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key<<endl;
		
		printkeyvalues("utility::printstructuresbeforekernelrun:: kvdram workspace (before kernel launch)::kvdram.first16", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_KVDRAM_KVS]), 16);
		// printkeyvalues("utility::printstructuresbeforekernelrun:: kvdram workspace (before kernel launch)::kvdram.last16", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_KVDRAM_KVS+KVDATA_BATCHSIZE_KVS-2]), 16);
		printkeyvalues("utility::printstructuresbeforekernelrun:: kvdram workspace (before kernel launch)::kvdram workspace", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_KVDRAMWORKSPACE_KVS]), 16);
		printkeyvalues("utility::printstructuresbeforekernelrun:: kvdram workspace (before kernel launch)::vertex ptrs", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_VERTEXPTR_KVS]), 16);
		printkeyvalues("utility::printstructuresbeforekernelrun:: kvdram workspace (before kernel launch)::vertex datas", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_VERTICESDATA_KVS]), 16);
		printkeyvalues("utility::printstructuresbeforekernelrun:: global capsule (before kernel launch)::kvstatsdram", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_STATSDRAM_KVS]), 16*8, 8);
	}
}
void utility::printstructuresafterkernelrun(string message, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size){
	cout<<"utility::printstructuresafterkernelrun:: printing structures (after kernel launch). "<<message<<endl;
	for(unsigned int i=0; i<size; i++){ // NUMSUBCPUTHREADS
		uint512_vec_dt * UVEC = (uint512_vec_dt *)kvsourcedram[0][i];
		cout<<"utility::printstructuresafterkernelrun:: printing messages (after kernel launch) for subthread: "<<i<<endl;
		cout<<"MESSAGES_RUNKERNELCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key<<endl;
		cout<<"MESSAGES_PROCESSCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key<<endl;
		cout<<"MESSAGES_COLLECTSTATSCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key<<endl;
		cout<<"MESSAGES_PARTITIONCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key<<endl;
		cout<<"MESSAGES_APPLYUPDATESCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key<<endl;
		cout<<"MESSAGES_SRCVOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key<<endl;
		cout<<"MESSAGES_SRCVSIZE: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key<<endl;
		cout<<"MESSAGES_DESTVOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_DESTVOFFSET].data[0].key<<endl;
		cout<<"MESSAGES_TREEDEPTH: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key<<endl;
		cout<<"MESSAGES_FINALNUMPARTITIONS: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key<<endl;
		cout<<"MESSAGES_GRAPHITERATIONID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key<<endl;
		cout<<"MESSAGES_NEXTBATCHOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key<<endl;
		cout<<"MESSAGES_BATCHSIZE: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key<<endl;
		cout<<"MESSAGES_RUNSIZE: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl;
		cout<<"MESSAGES_NEXTBATCHOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key<<endl;
		
		printkeyvalues("utility::printstructuresafterkernelrun:: kvdram workspace (after kernel launch)::kvdram.first16", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_KVDRAM_KVS]), 16);
		// printkeyvalues("utility::printstructuresafterkernelrun:: kvdram workspace (after kernel launch)::kvdram.last16", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_KVDRAM_KVS+KVDATA_BATCHSIZE_KVS-2]), 16);
		// printkeyvalues("utility::printstructuresafterkernelrun:: kvdram workspace (after kernel launch)::kvdram.middle16", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_KVDRAM_KVS+(KVDATA_BATCHSIZE_KVS/16)-2]), 16);
		printkeyvalues("utility::printstructuresafterkernelrun:: kvdram workspace (after kernel launch)::kvdram workspace", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_KVDRAMWORKSPACE_KVS]), 16);
		printkeyvalues("utility::printstructuresafterkernelrun:: kvdram workspace (after kernel launch)::vertex ptrs", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_VERTEXPTR_KVS]), 16);
		printkeyvalues("utility::printstructuresafterkernelrun:: kvdram workspace (after kernel launch)::vertex datas", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_VERTICESDATA_KVS]), 16);
		printkeyvalues("utility::printstructuresafterkernelrun:: global capsule (after kernel launch)::kvstatsdram", (keyvalue_t *)(&kvsourcedram[0][i][BASEOFFSET_STATSDRAM_KVS]), 16*8, 8);
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
int utility::runActs(unsigned int IterCount){
	if((IterCount % DRAMBATCHFACTOR) == (DRAMBATCHFACTOR - 1)){ return 1; }
	else { return 0; }
}
int utility::runActs(unsigned int IterCount, bool forcetrue){
	if(((IterCount % DRAMBATCHFACTOR) == (DRAMBATCHFACTOR - 1)) || (forcetrue == true)){ return 1; }
	else { return 0; }
}
unsigned int utility::GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(unsigned int ssdpartitonid){
	return (ssdpartitonid * KVDATA_RANGE_PERSSDPARTITION);
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
void utility::clearkeyvalues(uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){
			for(unsigned int k=0; k<PADDEDKVSOURCEDRAMSZ_KVS; k++){
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					kvbuffer[i][j][k].data[v].key = 0;
					kvbuffer[i][j][k].data[v].value = 0;
				}
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
		
		// skipspacing[p] = 0;
	}			
	calculateoffsets(stats, size, 0, skipspacing);
	for(unsigned int i=0; i<size-1; i++){ if(stats[i].key + stats[i].value > stats[i+1].key){ cout<<"utility::getmarkerpositions: ERROR: stats["<<i<<"].key("<<stats[i].key<<") + stats["<<i<<"].value("<<stats[i].value<<") >= stats["<<i+1<<"].key("<<stats[i+1].key<<"). exiting..."<<endl; exit(EXIT_FAILURE); }}					
}

void utility::calculateunallignedoffsets(keyvalue_t * keyvalues, unsigned int size){
	cout<<"--- utility::calculateunallignedoffsets..."<<". size: "<<size<<endl;
	for(unsigned int i=1; i<size; i++){ 
		if(i%100000==0){ cout<<"--- utility::calculateunallignedoffsets... i: "<<i<<", size: "<<size<<endl; }
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
void utility::PUSH(uuint64_dt * longword, unsigned int data, unsigned int databitsz){
	longword->data = (longword->data << databitsz) | data;
	return;
}
void utility::PARSE(string message, unsigned long longword){ 
	cout<<"utility::PARSE::"<<message<<" message"<<endl;
	unsigned int streetaddr = READFROM_ULONG(longword, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
	unsigned int numitems = READFROM_ULONG(longword, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
	unsigned int item = 0;
	cout<<"PARSE: streetaddr: "<<streetaddr<<", numitems: "<<numitems<<endl;
	for(unsigned int i=0; i<numitems; i++){
		item = READFROM_ULONG(longword, COMPACTPARAM_STARTOFFSET_DATA + i*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
		cout<<"PARSE: item "<<i<<": "<<((streetaddr * (1 << SRAMSZ_POW)) + item)<<endl;
	}
	return;
}
unsigned int utility::PARSE(unsigned long longword, unsigned int * _items){ 
	unsigned int streetaddr = READFROM_ULONG(longword, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
	unsigned int numitems = READFROM_ULONG(longword, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
	if(numitems > 3){
		cout<<"utility::PARSE. numitems > 3. exiting..."<<endl;
		ULONGTOBINARY(longword);
		PARSE("compactgraph::verify actual committing...", longword);
		exit(EXIT_FAILURE);
	}
	unsigned int item = 0;
	for(unsigned int i=0; i<numitems; i++){
		item = READFROM_ULONG(longword, COMPACTPARAM_STARTOFFSET_DATA + i*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
		_items[i] = ((streetaddr * (1 << SRAMSZ_POW)) + item);
	}
	return numitems;
}
unsigned int utility::PARSE(keyvalue_t keyvalue, unsigned int * _items){
	unsigned long * longword = (unsigned long *)&keyvalue;
	return PARSE(*longword, _items);
}
unsigned int utility::GETKEY(unsigned long longword){ 
	unsigned int streetaddr = READFROM_ULONG(longword, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
	unsigned int numitems = READFROM_ULONG(longword, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
	unsigned int item = READFROM_ULONG(longword, COMPACTPARAM_STARTOFFSET_DATA + 0*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
	return ((streetaddr * (1 << SRAMSZ_POW)) + item);
}
keyy_t utility::GETKEY(keyvalue_t keyvalue){ 
	#ifdef COMPACTEDGES
	if(keyvalue.value == INVALIDDATA){
		return keyvalue.key;
	} else {
		unsigned long * longword = (unsigned long *)&keyvalue;
		unsigned int streetaddr = READFROM_ULONG(*longword, COMPACTPARAM_STARTOFFSET_STREETADDR, COMPACTPARAM_BITSIZE_STREETADDR);
		unsigned int numitems = READFROM_ULONG(*longword, COMPACTPARAM_STARTOFFSET_NUMITEMS, COMPACTPARAM_BITSIZE_NUMITEMS);
		unsigned int item = READFROM_ULONG(*longword, COMPACTPARAM_STARTOFFSET_DATA + 0*COMPACTPARAM_BITSIZE_EACHDATA, COMPACTPARAM_BITSIZE_EACHDATA);
		// cout<<"--- utility::GETKEY:: streetaddr: "<<streetaddr<<", item: "<<item<<endl;
		return ((streetaddr * (1 << SRAMSZ_POW)) + item);
	}
	#else 
	return keyvalue.key;
	#endif
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
unsigned int utility::runbfs_sw(vector<vertex_t> &srcvids, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, unsigned int NumGraphIters){
	unsigned int * labels = new unsigned int[KVDATA_RANGE];
	for(unsigned int i=0; i<KVDATA_RANGE; i++){ labels[i] = UNVISITED; }
	vector<value_t> rootactvvs;
	vector<value_t> activevertices;
	for(unsigned int i=0; i<srcvids.size(); i++){ rootactvvs.push_back(srcvids[i]); }
	unsigned int edges1_count = 0;
	unsigned int edgesdstv1_sum = 0;
	
	for(unsigned int GraphIter=0; GraphIter<NumGraphIters; GraphIter++){
		edges1_count = 0;
		edgesdstv1_sum = 0;
	
		for(unsigned int i=0; i<rootactvvs.size(); i++){
			unsigned int vid = rootactvvs[i];
			edge_t vptr_begin = vertexptrbuffer[vid];
			edge_t vptr_end = vertexptrbuffer[vid+1];
			edge_t edges_size = vptr_end - vptr_begin;
			
			for(unsigned int k=0; k<edges_size; k++){
				unsigned int dstvid = edgedatabuffer[vptr_begin + k].dstvid;
				if(labels[dstvid] == UNVISITED){ 
					#ifdef _DEBUGMODE_KERNELPRINTS
					cout<<"utility::runbfs_sw: ACTIVE VERTICES seen for next iteration. dstvid: "<<dstvid<<endl;
					#endif
					
					labels[dstvid] = VISITED_IN_CURRENT_ITERATION; 
					activevertices.push_back(dstvid); 
					edgesdstv1_sum += dstvid; 
				}
				else if(labels[dstvid] == VISITED_IN_CURRENT_ITERATION){ } 
				else if(labels[dstvid] == VISITED_IN_PAST_ITERATION){ } 
				else{ cout<<"utility::runbfs_sw: should never get here. exiting..."<<endl; exit(EXIT_FAILURE); }
			}
		}
		
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"+++++++++++++++++++++++++++++ utility::runbfs_sw (nextit acvvs) edges1_count: NAp, edgesdstv1_sum: "<<edgesdstv1_sum<<endl;
		cout<<"utility::runbfs_sw: number of active vertices for next iteration: "<<activevertices.size()<<endl;
		// for(unsigned int i=0; i<activevertices.size(); i++){ cout<<"utility::runbfs_sw: activevertices["<<i<<"]: "<<activevertices[i]<<endl; }
		for(unsigned int i=0; i<hmin(activevertices.size(), 16); i++){ cout<<"utility::runbfs_sw: activevertices["<<i<<"]: "<<activevertices[i]<<endl; }
		#endif
		
		rootactvvs.clear();
		for(unsigned int i=0; i<activevertices.size(); i++){ rootactvvs.push_back(activevertices[i]); }
		activevertices.clear();
		
		for(unsigned int i=0; i<KVDATA_RANGE; i++){ 
			if(labels[i] == VISITED_IN_CURRENT_ITERATION){ labels[i] = VISITED_IN_PAST_ITERATION;  }
		}
	}
	delete labels;
	return edgesdstv1_sum;
}







