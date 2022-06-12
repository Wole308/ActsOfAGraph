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

utility::utility(universalparams_t _universalparams){
	universalparams = _universalparams;
}
utility::utility(){}
utility::~utility(){} 

void utility::printallparameters(){
	#ifdef USEHBMMEMORY
	std::cout<<"utility:: USEHBMMEMORY DEFINED"<<std::endl;
	#else 
	std::cout<<"utility:: USEDDRAMMEMORY DEFINED"<<std::endl;	
	#endif
	std::cout<<"utility:: universalparams.NUM_PARTITIONS: "<<universalparams.NUM_PARTITIONS<<std::endl;
	std::cout<<"utility:: NUM_PEs: "<<NUM_PEs<<std::endl;
	std::cout<<"utility:: NUM_PEs: "<<NUM_PEs<<std::endl;
	std::cout<<"utility:: DATAWIDTH: "<<DATAWIDTH<<std::endl;
	std::cout<<"utility:: VECTOR_SIZE: "<<VECTOR_SIZE<<std::endl;
	std::cout<<"utility:: universalparams.KVDATA_RANGE: "<<universalparams.KVDATA_RANGE<<std::endl;
	std::cout<<"utility:: universalparams.KVDATA_RANGE_POW: "<<universalparams.KVDATA_RANGE_POW<<std::endl;
	std::cout<<"utility:: universalparams.BATCH_RANGE: "<<universalparams.BATCH_RANGE<<std::endl;
	std::cout<<"utility:: universalparams.BATCH_RANGE_POW: "<<universalparams.BATCH_RANGE_POW<<std::endl;
	std::cout<<"utility:: universalparams.TREE_DEPTH: "<<universalparams.TREE_DEPTH<<std::endl;
	std::cout<<"utility:: universalparams.KVDATA_RANGE: "<<universalparams.KVDATA_RANGE<<std::endl;
	std::cout<<"utility:: MESSAGES_SIZE: "<<MESSAGES_SIZE<<std::endl;
	#ifdef LOCKE
	std::cout<<"utility:: LOCKE DEFINED"<<std::endl;
	#else 
	std::cout<<"utility:: LOCKE NOT DEFINED"<<std::endl;	
	#endif
	std::cout<<"utility:: sizeof(keyvalue_t): "<<sizeof(keyvalue_t)<<" bytes ("<<(sizeof(keyvalue_t) * 8)<<" bits)"<<std::endl;
	std::cout<<"utility:: sizeof(vertex_t): "<<sizeof(vertex_t)<<" bytes ("<<(sizeof(vertex_t) * 8)<<" bits)"<<std::endl;
	std::cout<<"utility:: sizeof(edge_t): "<<sizeof(edge_t)<<" bytes ("<<(sizeof(edge_t) * 8)<<" bits)"<<std::endl;

	// >>>
	std::cout<<">> host[baseoffsets]:: universalparams.KVSOURCEDRAMSZ: "<<universalparams.KVSOURCEDRAMSZ<<std::endl;	
	std::cout<<">> host[baseoffsets]:: universalparams.KVSOURCEDRAMSZ_KVS: "<<universalparams.KVSOURCEDRAMSZ_KVS<<std::endl;	
	std::cout<<">> host[baseoffsets]:: universalparams.PADDEDKVSOURCEDRAMSZ: "<<universalparams.PADDEDKVSOURCEDRAMSZ<<std::endl;	
	std::cout<<">> host[baseoffsets]:: universalparams.PADDEDKVSOURCEDRAMSZ_KVS: "<<universalparams.PADDEDKVSOURCEDRAMSZ_KVS<<std::endl;
	
	std::cout<<">> host[baseoffsets]:: universalparams.VDRAMSZ: "<<universalparams.VDRAMSZ<<std::endl;	
	std::cout<<">> host[baseoffsets]:: universalparams.PADDEDVDRAMSZ: "<<universalparams.PADDEDVDRAMSZ<<std::endl;	
	std::cout<<">> host[baseoffsets]:: universalparams.PADDEDVDRAMSZ_KVS: "<<universalparams.PADDEDVDRAMSZ_KVS<<std::endl;
	// std::cout<<">> host[bytes]:: universalparams.MAXHBMCAPACITY_KVS2: "<<universalparams.MAXHBMCAPACITY_KVS2<<" key-value set"<<std::endl;
	std::cout<<">> host[bytes]:: universalparams.MAXHBMCAPACITY_UINT32: "<<universalparams.MAXHBMCAPACITY_UINT32<<" values"<<std::endl;
	// std::cout<<">> host[baseoffsets]:: universalparams.KVDRAMWORKCAPACITY_BYTES: "<<universalparams.KVDRAMWORKCAPACITY_BYTES<<std::endl;
	std::cout<<">> host[baseoffsets]:: universalparams.HBMWORKCAPACITY_UINT32: "<<universalparams.HBMWORKCAPACITY_UINT32<<std::endl;
	std::cout<<">> host[bytes]:: universalparams.MAXHBMCAPACITY: "<<universalparams.MAXHBMCAPACITY_KVS2 * VECTOR_SIZE * sizeof(keyvalue_t)<<""<<std::endl;
	std::cout<<">> host[baseoffsets]:: universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA: "<<universalparams.VOLUMEOFFSETKVS_WORKSPACEDATA<<std::endl;
	std::cout<<">> host[baseoffsets]:: universalparams.VOLUMEOFFSETKVS_WORKDATA: "<<universalparams.VOLUMEOFFSETKVS_WORKDATA<<std::endl;
	std::cout<<">> host[size]:: universalparams.VOLUMESIZEU32_WORKSPACEDATA: "<<universalparams.VOLUMESIZEU32_WORKSPACEDATA<<std::endl;
	std::cout<<">> host[size]:: universalparams.VOLUMESIZEU32_WORKDATA: "<<universalparams.VOLUMESIZEU32_WORKDATA<<std::endl;
	
	// >>>
	std::cout<<">> host[bytes]:: MESSAGESDRAMSZ (bytes): "<<MESSAGESDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host[bytes]:: universalparams.PADDEDVDRAMSZ (bytes): "<<universalparams.PADDEDVDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host[bytes]:: universalparams.PADDEDKVSOURCEDRAMSZ (bytes): "<<universalparams.PADDEDKVSOURCEDRAMSZ * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	std::cout<<">> host[bytes]:: universalparams.MAXHBMCAPACITY (bytes): "<<universalparams.MAXHBMCAPACITY_KVS2 * VECTOR_SIZE * sizeof(keyvalue_t)<<" bytes"<<std::endl;
	
	std::cout<<"utility:: universalparams.PROC_SRAMSZ_POW: "<<universalparams.PROC_SRAMSZ_POW<<std::endl;
	std::cout<<"utility:: universalparams.PROC_SRAMSZ: "<<universalparams.PROC_SRAMSZ<<std::endl;
	
	std::cout<<"utility:: universalparams.RED_SRAMSZ_POW: "<<universalparams.RED_SRAMSZ_POW<<std::endl;
	std::cout<<"utility:: universalparams.RED_SRAMSZ: "<<universalparams.RED_SRAMSZ<<std::endl;
	
	std::cout<<"utility:: universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM: "<<universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM<<std::endl;
	
	std::cout<<"utility:: universalparams.REDUCEPARTITIONSZ: "<<universalparams.REDUCEPARTITIONSZ<<std::endl;
	std::cout<<"utility:: universalparams.REDUCEPARTITIONSZ_KVS2: "<<universalparams.REDUCEPARTITIONSZ_KVS2<<std::endl;
	std::cout<<"utility:: universalparams.PROCESSPARTITIONSZ: "<<universalparams.PROCESSPARTITIONSZ<<std::endl;
	std::cout<<"utility:: universalparams.PROCESSPARTITIONSZ_KVS2: "<<universalparams.PROCESSPARTITIONSZ_KVS2<<std::endl;
	std::cout<<"utility:: MAX_BLOCKRAM_VSRCDATA_SIZE: "<<MAX_BLOCKRAM_VSRCDATA_SIZE<<std::endl;
	std::cout<<"utility:: MAX_BLOCKRAM_VDESTDATA_SIZE: "<<MAX_BLOCKRAM_VDESTDATA_SIZE<<std::endl;
	std::cout<<"utility:: universalparams.NUMREDUCEPARTITIONS: "<<universalparams.NUMREDUCEPARTITIONS<<std::endl;
	std::cout<<"utility:: universalparams.NUMPROCESSEDGESPARTITIONS: "<<universalparams.NUMPROCESSEDGESPARTITIONS<<std::endl;
	std::cout<<"utility:: universalparams.VPTR_SHRINK_RATIO: "<<universalparams.VPTR_SHRINK_RATIO<<std::endl;
	
	std::cout<<"utility:: PROCESSPARTITIONSZ / NUM_PEs: "<<universalparams.PROCESSPARTITIONSZ / NUM_PEs<<" (remainder "<<(1 << universalparams.PROCESSPARTITIONSZ_KVS2) % NUM_PEs<<")"<<std::endl;
	std::cout<<"utility:: PROCESSPARTITIONSZ_KVS2 / NUM_PEs: "<<universalparams.PROCESSPARTITIONSZ_KVS2 / NUM_PEs<<" (remainder "<<universalparams.PROCESSPARTITIONSZ_KVS2 % NUM_PEs<<")"<<std::endl;
	std::cout<<"utility:: SUB_CHUNK_HEIGHT: "<<universalparams.SUB_CHUNK_HEIGHT<<std::endl;
	
	std::cout<<"utility:: VDATA_SHRINK_RATIO_POW: "<<VDATA_SHRINK_RATIO_POW<<std::endl;
	std::cout<<"utility:: VDATA_SHRINK_RATIO: "<<VDATA_SHRINK_RATIO<<std::endl;
	
	#ifdef CONFIG_INCLUDE_IMPL_WITH_TREEDEPTH_OF_1
	std::cout<<"utility:: *** INCLUDES IMPLEMENTATION WITH universalparams.TREE_DEPTH OF ONE: "<<universalparams.TREE_DEPTH<<std::endl;
	#else 
	std::cout<<"utility:: *** DOES NOT INCLUDE IMPLEMENTATION WITH universalparams.TREE_DEPTH OF ONE: "<<universalparams.TREE_DEPTH<<std::endl;	
	#endif 
	
	std::cout<<"utility:: MAX_SRCBUFFER_SIZE: "<<MAX_SRCBUFFER_SIZE<<std::endl;
	std::cout<<"utility:: universalparams.WORKBUFFER_SIZE: "<<universalparams.WORKBUFFER_SIZE<<std::endl;
	std::cout<<"utility:: MAX_SOURCEBLOCKRAM_SIZE: "<<MAX_SOURCEBLOCKRAM_SIZE<<std::endl;
	std::cout<<"utility:: MAX_DESTBLOCKRAM_SIZE: "<<MAX_DESTBLOCKRAM_SIZE<<std::endl;
	std::cout<<"utility:: MAX_BLOCKRAM_VSRCDATA_SIZE: "<<MAX_BLOCKRAM_VSRCDATA_SIZE<<std::endl;
	std::cout<<"utility:: MAX_BLOCKRAM_VDESTDATA_SIZE: "<<MAX_BLOCKRAM_VDESTDATA_SIZE<<std::endl;
	std::cout<<"utility:: BLOCKRAM_CURRPMASK_SIZE: "<<BLOCKRAM_CURRPMASK_SIZE<<std::endl;
	std::cout<<"utility:: BLOCKRAM_NEXTPMASK_SIZE: "<<BLOCKRAM_NEXTPMASK_SIZE<<std::endl;
	std::cout<<"utility:: BLOCKRAM_CUMMTVPMASK_SIZE: "<<BLOCKRAM_CUMMTVPMASK_SIZE<<std::endl;
	
	#ifdef ENABLE_KERNEL_PROFILING
	std::cout<<"utility:: ENABLE_KERNEL_PROFILING DEFINED: "<<std::endl;
	#else 
	std::cout<<"utility:: ENABLE_KERNEL_PROFILING NOT DEFINED: "<<std::endl;	
	#endif 
	
	#ifdef CONFIG_ACTS_MEMORYLAYOUT
	std::cout<<"utility:: CONFIG_ACTS_MEMORYLAYOUT DEFINED: "<<std::endl;	
	#endif 
	#ifdef CONFIG_ACTS_PARTITIONINGLOGIC
	std::cout<<"utility:: CONFIG_ACTS_PARTITIONINGLOGIC DEFINED: "<<std::endl;	
	#endif
	#ifdef CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
	std::cout<<"utility:: CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC DEFINED: "<<std::endl;	
	#endif
	#ifdef CONFIG_ACTS_HYBRIDLOGIC
	std::cout<<"utility:: CONFIG_ACTS_HYBRIDLOGIC DEFINED: "<<std::endl;	
	#endif
	#ifdef CONFIG_ACTS_VERYBARE
	std::cout<<"utility:: CONFIG_ACTS_VERYBARE DEFINED: "<<std::endl;	
	#endif
	
	#ifndef CONFIG_ACTS_MEMORYLAYOUT
	std::cout<<"utility:: CONFIG_TRADITIONAL_MEMORYLAYOUT DEFINED: "<<std::endl;	
	#endif 
	#ifndef CONFIG_ACTS_PARTITIONINGLOGIC
	std::cout<<"utility:: CONFIG_TRADITIONAL_PARTITIONINGLOGIC DEFINED: "<<std::endl;	
	#endif
	#ifndef CONFIG_ACTS_RECURSIVEPARTITIONINGLOGIC
	std::cout<<"utility:: CONFIG_TRADITIONAL_RECURSIVEPARTITIONINGLOGIC DEFINED: "<<std::endl;	
	#endif
	#ifndef CONFIG_ACTS_HYBRIDLOGIC
	std::cout<<"utility:: CONFIG_TRADITIONAL_HYBRIDLOGIC DEFINED: "<<std::endl;	
	#endif
	#ifndef CONFIG_ACTS_VERYBARE
	std::cout<<"utility:: CONFIG_ACTS_VERYBARE NOT DEFINED: "<<std::endl;	
	#endif
	
	std::cout<<"utility:: universalparams.REDUCESZ_POW: "<<universalparams.REDUCESZ_POW<<std::endl;
	std::cout<<"utility:: universalparams.REDUCESZ: "<<universalparams.REDUCESZ<<std::endl;
	
	std::cout<<"utility:: universalparams.PROC_SRAMSZ: "<<universalparams.PROC_SRAMSZ<<std::endl;
	std::cout<<"utility:: universalparams.RED_SRAMSZ: "<<universalparams.RED_SRAMSZ<<std::endl;
	
	std::cout<< TIMINGRESULTSCOLOR <<"utility:: universalparams.TREE_DEPTH: "<<universalparams.TREE_DEPTH<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR <<"utility:: universalparams.PROCESSPARTITIONSZ_KVS2: "<<universalparams.PROCESSPARTITIONSZ_KVS2<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR <<"utility:: MAX_BLOCKRAM_VSRCDATA_SIZE: "<<MAX_BLOCKRAM_VSRCDATA_SIZE<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR <<"utility:: universalparams.REDUCEPARTITIONSZ_KVS2: "<<universalparams.REDUCEPARTITIONSZ_KVS2<< RESET <<std::endl;
	std::cout<< TIMINGRESULTSCOLOR <<"utility:: MAX_BLOCKRAM_VDESTDATA_SIZE: "<<MAX_BLOCKRAM_VDESTDATA_SIZE<< RESET <<std::endl;
	
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
void utility::printkeyvalues(string message, keyvalue_vec_bittype keyvalues[MAX_NUM_PARTITIONS][BLOCKRAM_SIZE], unsigned int numcols, unsigned int size){
	cout<<endl<<"utility::printkeyvalues:"<<message<<endl;
	for(unsigned int v=0; v<numcols; v++){
		for(unsigned int i=0; i<size; i++){ cout<<"keyvalues["<<v<<"]["<<i<<"].key: "<<keyvalues[v][i].key<<", keyvalues["<<v<<"]["<<i<<"].value: "<<keyvalues[v][i].value<<endl; }
		cout<<".."<<endl;
	}
}
void utility::printtriples(string message, triple_t * triples, unsigned int size){
	cout<<"utility::printtriples:"<<message<<endl;
	unsigned int totalAs = 0;
	unsigned int totalBs = 0;
	unsigned int totalCs = 0;
	unsigned int totalDs = 0;
	for(unsigned int i=0; i<size; i++){ cout<<"triples["<<i<<"].A: "<<triples[i].A<<", triples["<<i<<"].B: "<<triples[i].B<<", triples["<<i<<"].C: "<<triples[i].C<<", triples["<<i<<"].D: "<<triples[i].D<<endl; totalAs+=triples[i].A; totalBs+=triples[i].B; totalCs+=triples[i].C; totalDs+=triples[i].D; }					
	cout<<"total As: "<<totalAs<<", total Bs: "<<totalBs<<", total Cs: "<<totalCs<<", total Ds: "<<totalDs<<endl;
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
void utility::printstructuresbeforekernelrun(string message, uint512_vec_dt * kvsourcedram[NUM_PEs], unsigned int size){
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
void utility::printstructuresafterkernelrun(string message, uint512_vec_dt * kvsourcedram[NUM_PEs], unsigned int size){
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
	for(unsigned int i=0; i<NUM_PEs; i++){
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

bool utility::isbufferused(unsigned int id){
	if(id==0 || id==NUMCOMPUTEUNITS_SLR2 || id==NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1){ return true; } else { return false; }
	return true;
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
unsigned int utility::allignhigher_FACTOR(unsigned int val, unsigned int _FACTOR){
	unsigned int fac = (val + (_FACTOR - 1)) / _FACTOR;
	return (fac * _FACTOR);
}
void utility::setarray(unsigned int array[NUM_PEs], unsigned int size, unsigned int value){
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
void utility::clearkeyvalues(uint512_vec_dt * kvbuffer[NUM_PEs]){
	for(unsigned int j=0; j<NUM_PEs; j++){
		for(unsigned int k=0; k<universalparams.PADDEDKVSOURCEDRAMSZ_KVS; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				kvbuffer[j][k].data[v].key = 0;
				kvbuffer[j][k].data[v].value = 0;
			}
		}
	}
	return;
}
unsigned int utility::getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth){
	// unsigned int partition = (keyvalue.key - upperlimit) / (BATCH_RANGE / pow(universalparams.NUM_PARTITIONS, treedepth));
	unsigned int partition = ((keyvalue.key - upperlimit) >> (universalparams.BATCH_RANGE_POW - (universalparams.NUM_PARTITIONS_POW * treedepth)));
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadgraph::getglobalpartition", partition, (1 << (universalparams.NUM_PARTITIONS_POW * treedepth)), keyvalue.key, upperlimit, NAp);
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
		unsigned int B = (A + (MAX_SRCBUFFER_SIZE-1)) / MAX_SRCBUFFER_SIZE; 
		if(B < 80){ B = B * 2; }
		unsigned int C = ((4 * 4 * 2) * universalparams.NUM_PARTITIONS) + VECTOR_SIZE; 
		skipspacing[p] = (B * C) + 128; 
		
		// #ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
		// skipspacing[p] = skipspacing[p] * 2;
		// #else 
		// skipspacing[p] = 0;	
		// #endif 
		
		// skipspacing[p] = skipspacing[p] * 2;
		skipspacing[p] = 0;	
			
			
		/* #ifdef TESTKERNEL
			skipspacing[p] = skipspacing[p] * 2;
		#else 
			#ifdef CONFIG_PREPROCESS_LOADEDGES_SEQUENTIALSRCVIDS
			skipspacing[p] = skipspacing[p] * 2;
			#else 
			skipspacing[p] = 0;	
			#endif 
			
			// #ifdef ALLVERTEXISACTIVE_ALGORITHM
			// skipspacing[p] = skipspacing[p] * 2; // FIXME.
			// #else 
			// skipspacing[p] = 0; // CRITICAL FIXME
			// #endif 
		#endif  */
		
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
unsigned int utility::READBITSFROM_UINTV(unsigned int data, unsigned int index, unsigned int size){
	unsigned int res = 0;
	res = READFROM_UINT(data, index, size);
	return res;
}
void utility::WRITEBITSTO_UINTV(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
	WRITETO_UINT(data, index, size, value);
	return; 
}
unsigned int utility::UTIL_GETLOCALVID(unsigned int vid, unsigned int instid){
	#pragma HLS INLINE
	return (vid - instid) / NUM_PEs;
}
unsigned int utility::UTIL_GETREALVID(unsigned int lvid, unsigned int instid){
	#pragma HLS INLINE
	return (lvid * NUM_PEs) + instid;
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

#define UTILITY_BRIEFPRINTONLY
void utility::printallfeedback(string message, string graphpath, uint512_vec_dt * vdram, uint512_vec_dt * vdramtemp0, uint512_vec_dt * vdramtemp1, uint512_vec_dt * vdramtemp2, uint512_vec_dt * kvbuffer[NUM_PEs]){
	
	// unsigned int F0 = 0;
	// unsigned int F1 = NUMCOMPUTEUNITS_SLR2;
	// unsigned int F2 = NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1;
	
	unsigned int F0 = 0;
	unsigned int F1 = 1;
	unsigned int F2 = 2;
	unsigned int totalnum_edgesprocessed = 0;
	
	for(unsigned int i=0; i<1; i++){
		for(unsigned int GraphIter=0; GraphIter<10; GraphIter++){ 
			unsigned int num_edgesprocessed = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key;	
			unsigned int num_vertexupdatesreduced = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key;	
			
			unsigned int num_validedgesprocessed = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].value;	
			unsigned int num_validvertexupdatesreduced = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].value;	
			
			cout<<">>> utility::[A]["<<message<<"][PE:"<<i<<"][Iter: "<<GraphIter<<"]:: num edges processed: "<<num_edgesprocessed<<"("<<num_validedgesprocessed<<"), num vertex updates reduced: "<<num_vertexupdatesreduced<<"("<<num_validvertexupdatesreduced<<")"<<endl;	
		}
	}

	for(unsigned int GraphIter=0; GraphIter<10; GraphIter++){ 
		unsigned int num_edgesprocessed = 0;
		unsigned int num_vertexupdatesreduced = 0;
		unsigned int num_validedgesprocessed = 0;
		unsigned int num_validvertexupdatesreduced = 0;
		for(unsigned int i=0; i<NUM_PEs; i++){
			num_edgesprocessed += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key;	
			num_vertexupdatesreduced += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key;	
			
			num_validedgesprocessed += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].value;	
			num_validvertexupdatesreduced += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].value;
			
			totalnum_edgesprocessed += num_edgesprocessed;
		}
		cout<<">>> utility::[A]["<<message<<"][PE:ALL][Iter: "<<GraphIter<<"]:: num edges processed: "<<num_edgesprocessed<<"("<<num_validedgesprocessed<<"), num vertex updates reduced: "<<num_vertexupdatesreduced<<"("<<num_validvertexupdatesreduced<<")"<<endl;	
	}
	
	#ifndef UTILITY_BRIEFPRINTONLY
	for(unsigned int GraphIter=0; GraphIter<10; GraphIter++){
		unsigned int sum_edgesprocessed = 0;
		unsigned int sum_vertexupdatesreduced = 0;
		for(unsigned int i=0; i<NUM_PEs; i++){
			unsigned int num_edgesprocessed = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key;	
			unsigned int num_vertexupdatesreduced = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key;	
			sum_edgesprocessed += num_edgesprocessed;	
			sum_vertexupdatesreduced += num_vertexupdatesreduced;	
			cout<<">>> utility::[B]["<<message<<"][PE:"<<i<<"]:: num edges processed: "<<num_edgesprocessed<<", num vertex updates reduced: "<<num_vertexupdatesreduced<<""<<endl;	
		}
		cout<<">>> utility::[B]["<<message<<"]:: num edges processed: "<<sum_edgesprocessed<<", num vertex updates reduced: "<<sum_vertexupdatesreduced<<""<<endl;	
	}
	
	cout<<">>> utility::verifyresults:: Printing results from app.cpp [vdram @ 0][num processed edges stats @ processedges.cpp] & [@ 32][num reduced vertex-update stats @ processedges.cpp]"<<endl;
	for(unsigned int GraphIter=0; GraphIter<4; GraphIter++){
		cout<<">>> utility::["<<message<<"]::[vdram[MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + "<<GraphIter<<"]]: ("<<vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key<<", "<<vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key<<")"<<endl;
	}
	
	cout<<">>> utility::verifyresults:: Printing results from app.cpp [vdram @ 64][num processed edges stats @ processedges.cpp->actsutility.cpp]."<<endl;
	for(unsigned int GraphIter=0; GraphIter<4; GraphIter++){
		cout<<">>> utility::["<<message<<"]::[vdram[MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + "<<GraphIter<<"]]: "<<vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED2 + GraphIter].data[0].key<<endl;
	}
	
	cout<<">>> utility::verifyresults:: Printing results from app.cpp [vdram @ 96][num active vertices for next iteration @ mem_access.cpp]."<<endl;
	for(unsigned int GraphIter=0; GraphIter<4; GraphIter++){
		cout<<">>> utility::["<<message<<"]::[vdram[MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + "<<GraphIter<<"]]: "<<vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMACTIVEVERTICESFORNEXTITERATION + GraphIter].data[0].key<<endl;
	}
	
	cout<<">>> utility::verifyresults:: Printing results from app.cpp [@ 0][num processed edges stats @ processedges.cpp] & [@ 32][num reduced vertex-update stats @ processedges.cpp]"<<endl;
	for(unsigned int GraphIter=0; GraphIter<4; GraphIter++){
		cout<<">>> utility::["<<message<<"]::[vdramtemp0[MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + "<<GraphIter<<"]]: ("<<vdramtemp0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key<<", "<<vdramtemp0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key<<")"<<endl;
	}
	
	for(unsigned int GraphIter=0; GraphIter<4; GraphIter++){
		cout<<">>> utility::["<<message<<"]::[vdramtemp1[MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + "<<GraphIter<<"]]: ("<<vdramtemp1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key<<", "<<vdramtemp1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key<<")"<<endl;
	}
	
	for(unsigned int GraphIter=0; GraphIter<4; GraphIter++){
		cout<<">>> utility::["<<message<<"]::[vdramtemp2[MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + "<<GraphIter<<"]]: ("<<vdramtemp2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key<<", "<<vdramtemp2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key<<")"<<endl;
	}
	
	for(unsigned int GraphIter=0; GraphIter<4; GraphIter++){
		cout<<">>> utility::["<<message<<"]::[vdram[MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + "<<GraphIter<<"]]: ("<<vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key<<", "<<vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key<<")"<<endl;
	}
	
	for(unsigned int GraphIter=0; GraphIter<4; GraphIter++){
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F0<<"][MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + "<<GraphIter<<"]]: ("<<kvbuffer[F0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key<<", "<<kvbuffer[F0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key<<")"<<endl;
	}
	for(unsigned int GraphIter=0; GraphIter<4; GraphIter++){
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F1<<"][MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + "<<GraphIter<<"]]: ("<<kvbuffer[F1][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key<<", "<<kvbuffer[F1][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key<<")"<<endl;
	}
	for(unsigned int GraphIter=0; GraphIter<4; GraphIter++){
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F2<<"][MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + "<<GraphIter<<"]]: ("<<kvbuffer[F2][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key<<", "<<kvbuffer[F2][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key<<")"<<endl;
	}
	
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F0<<"][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + "<<t<<"]]: "<<kvbuffer[F0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + t].data[0].key<<""<<endl;
	}
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F1<<"][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + "<<t<<"]]: "<<kvbuffer[F1][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + t].data[0].key<<""<<endl;
	}
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F2<<"][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + "<<t<<"]]: "<<kvbuffer[F2][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + t].data[0].key<<""<<endl;
	}
	
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F0<<"][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + "<<t<<"]]: "<<kvbuffer[F0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + t].data[0].key<<""<<endl;
	}
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F1<<"][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + "<<t<<"]]: "<<kvbuffer[F1][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + t].data[0].key<<""<<endl;
	}
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F2<<"][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + "<<t<<"]]: "<<kvbuffer[F2][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + t].data[0].key<<""<<endl;
	}
	
	// for(unsigned int t=0; t<4; t++){ // 128
		// cout<<">>> utility::["<<message<<"]::[kvbuffer[5][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + "<<t<<"]]: "<<kvbuffer[5][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + t].data[0].key<<""<<endl;
	// }
	// for(unsigned int t=0; t<4; t++){ // 128
		// cout<<">>> utility::["<<message<<"]::[kvbuffer[6][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + "<<t<<"]]: "<<kvbuffer[6][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + t].data[0].key<<""<<endl;
	// }
	// for(unsigned int t=0; t<4; t++){ // 128
		// cout<<">>> utility::["<<message<<"]::[kvbuffer[7][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + "<<t<<"]]: "<<kvbuffer[7][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_LOCALBEGINPTR + t].data[0].key<<""<<endl;
	// }
	
	// for(unsigned int t=0; t<4; t++){ // 128
		// cout<<">>> utility::["<<message<<"]::[kvbuffer[5][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + "<<t<<"]]: "<<kvbuffer[5][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + t].data[0].key<<""<<endl;
	// }
	// for(unsigned int t=0; t<4; t++){ // 128
		// cout<<">>> utility::["<<message<<"]::[kvbuffer[6][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + "<<t<<"]]: "<<kvbuffer[6][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + t].data[0].key<<""<<endl;
	// }
	// for(unsigned int t=0; t<4; t++){ // 128
		// cout<<">>> utility::["<<message<<"]::[kvbuffer[7][MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + "<<t<<"]]: "<<kvbuffer[7][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_PROCESSIT_NUMEDGES + t].data[0].key<<""<<endl;
	// }
	
	unsigned int offset = kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].data[0].key;
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F0<<"]["<<offset + t<<"]]: "<<kvbuffer[F0][offset + t].data[0].key<<""<<endl;
	}
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F1<<"]["<<offset + t<<"]]: "<<kvbuffer[F1][offset + t].data[0].key<<""<<endl;
	}
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F2<<"]["<<offset + t<<"]]: "<<kvbuffer[F2][offset + t].data[0].key<<""<<endl;
	}
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer[5]["<<offset + t<<"]]: "<<kvbuffer[5][offset + t].data[0].key<<""<<endl;
	}
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer[6]["<<offset + t<<"]]: "<<kvbuffer[6][offset + t].data[0].key<<""<<endl;
	}
	for(unsigned int t=0; t<4; t++){ // 128
		cout<<">>> utility::["<<message<<"]::[kvbuffer[7]["<<offset + t<<"]]: "<<kvbuffer[7][offset + t].data[0].key<<""<<endl;
	}
	
	unsigned int F = 0;
	for(unsigned int t=0; t<3; t++){
		if(t==0){ F = F0; } if(t==1){ F = F1; } if(t==2){ F = F2; }
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F<<"][MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP]]: "<<kvbuffer[F][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_EDGEBANKLOOP].data[0].key<<""<<endl;
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F<<"][MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP]]: "<<kvbuffer[F1][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_VCHUNKLOOP].data[0].key<<""<<endl;
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F<<"][MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP]]: "<<kvbuffer[F][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_STAGELOOP].data[0].key<<""<<endl;
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F<<"][MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP]]: "<<kvbuffer[F][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_LOPLOOP].data[0].key<<""<<endl;
		cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F<<"][MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP]]: "<<kvbuffer[F][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_SOURCEPLOOP].data[0].key<<""<<endl;
	}
	
	for(unsigned int t2=0; t2<64; t2++){
		cout<<">>> utility::["<<message<<"]::[vdramtemp0[BASEOFFSET_MESSAGESDATA_KVS + "<<BASEOFFSET_MESSAGESDATA_KVS + t2<<"]]: "<<vdramtemp0[BASEOFFSET_MESSAGESDATA_KVS + t2].data[0].key<<""<<endl;
	}
	for(unsigned int t=0; t<4; t++){
		for(unsigned int t2=0; t2<64; t2++){
			if(t==0){ F = F0; } if(t==1){ F = F1; } if(t==2){ F = F2; } if(t==2){ F = 3; }
			cout<<">>> utility::["<<message<<"]::[kvbuffer["<<F<<"][BASEOFFSET_MESSAGESDATA_KVS + "<<BASEOFFSET_MESSAGESDATA_KVS + t2<<"]]: "<<kvbuffer[F][BASEOFFSET_MESSAGESDATA_KVS + t2].data[0].key<<""<<endl;
		}
	}
	#endif 
	
	return;
}












