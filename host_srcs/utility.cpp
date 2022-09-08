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
	std::cout<<"utility:: VECTOR2_SIZE: "<<VECTOR2_SIZE<<std::endl;
	std::cout<<"utility:: UPDATEDATA_PACKINGSIZE: "<<UPDATEDATA_PACKINGSIZE<<std::endl;
	std::cout<<"utility:: EDGEDATA_PACKINGSIZE: "<<EDGEDATA_PACKINGSIZE<<std::endl;
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
	// std::cout<<"utility:: SUB_CHUNK_HEIGHT: "<<universalparams.SUB_CHUNK_HEIGHT<<std::endl;
	
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
	
	std::cout<<"utility:: NUM_UPROPBLOCKS_PER_VPARTITION: "<<NUM_UPROPBLOCKS_PER_VPARTITION<<std::endl;
	// std::cout<<"utility:: ALLIGNED_NUM_EDGEBLOCKS_PER_VPARTITION: "<<ALLIGNED_NUM_EDGEBLOCKS_PER_VPARTITION<<std::endl;
	
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

void utility::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data >= upper_bound){ std::cout<<"utility::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }			
}
void utility::checkforlessthanthan(string message, unsigned int data1, unsigned int data2){
	if(data1 < data2){ cout<<"utility::checkforlessthanthan: ERROR. data1 < data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
}

void utility::stopTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx){
	std::chrono::steady_clock::time_point endtime = std::chrono::steady_clock::now();		
	std::cout << TIMINGRESULTSCOLOR << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::nanoseconds> (endtime - begintime).count() << "[nanao seconds]" << "(Iteration "<<iteration_idx<<")"<< RESET << std::endl;			
	std::cout << TIMINGRESULTSCOLOR << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::microseconds> (endtime - begintime).count() << "[micro seconds]" << "(Iteration "<<iteration_idx<<")"<< RESET << std::endl;
	std::cout << TIMINGRESULTSCOLOR << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds> (endtime - begintime).count() << "[milli seconds]" << "(Iteration "<<iteration_idx<<")"<< RESET << std::endl;
	std::cout << TIMINGRESULTSCOLOR << ">>> "<<caption<<": Time Elapsed = " << std::chrono::duration_cast<std::chrono::seconds> (endtime - begintime).count() << "[seconds]" << "(Iteration "<<iteration_idx<<")"<< RESET << std::endl;
	std::cout << std::endl;
	return;
}

bool utility::isbufferused(unsigned int id){
	#ifdef TESTKERNEL 
	if(id==0 || id==NUMCOMPUTEUNITS_SLR2 || id==NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1){ return true; } else { return false; } ///////////////
	// if(id==8){ return true; } else { return false; }
	// if(id==0 || id==8 || id==NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1){ return true; } else { return false; }
	// if(id==0 || id==NUMCOMPUTEUNITS_SLR2 || id==23){ return true; } else { return false; }
	#else 
	return true;
	#endif 
	// if(id==0){ return true; } else { return false; }
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
void utility::resetvalues(value_t * values, unsigned int size, unsigned int value){
	for(unsigned int i=0; i<size; i++){ values[i] = value; }
}

unsigned int utility::allignlower_FACTOR(unsigned int val, unsigned int _FACTOR){
	unsigned int fac = val / _FACTOR;
	return (fac * _FACTOR);
}
unsigned int utility::allignhigher_FACTOR(unsigned int val, unsigned int _FACTOR){
	unsigned int fac = (val + (_FACTOR - 1)) / _FACTOR;
	return (fac * _FACTOR);
}

void utility::calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int * skipspacing, unsigned int allign_factor){
	buffer[0].key += base;
	// for(unsigned int i=1; i<size; i++){ buffer[i].key = allignlower_FACTOR(buffer[i-1].key + buffer[i-1].value + skipspacing[i-1], VECTOR_SIZE); }
	for(unsigned int i=1; i<size; i++){ buffer[i].key = allignhigher_FACTOR(buffer[i-1].key + buffer[i-1].value + skipspacing[i-1], allign_factor); }
	return;
}
void utility::calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int allign_factor){
	buffer[0].key = base;
	for(unsigned int i=1; i<size; i++){ buffer[i].key = allignhigher_FACTOR((buffer[i-1].key + buffer[i-1].value), allign_factor); }
	return;
}
void utility::getmarkerpositions(keyvalue_t * stats, unsigned int size, unsigned int allign_factor){
	unsigned int * skipspacing = new unsigned int[size];
	// for(unsigned int p=0; p<size; p++){ skipspacing[p] = 0;	}
	for(unsigned int p=0; p<size; p++){ skipspacing[p] = 16 * EDGEDATA_PACKINGSIZE;	}
	calculateoffsets(stats, size, 0, skipspacing, allign_factor);
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
	// unsigned int W = EDGEDATA_PACKINGSIZE * NUM_PEs;
	// unsigned int y = vid / W; 
	// unsigned int x = vid % EDGEDATA_PACKINGSIZE;
	// unsigned int lvid = (y * EDGEDATA_PACKINGSIZE) + x;
	// return lvid;
	
	unsigned int W = (VDATA_PACKINGNUMSETS_OFFLINE * EDGEDATA_PACKINGSIZE) * NUM_PEs;
	unsigned int y = vid / W; 
	unsigned int x = vid % (VDATA_PACKINGNUMSETS_OFFLINE * EDGEDATA_PACKINGSIZE);
	unsigned int lvid = (y * (VDATA_PACKINGNUMSETS_OFFLINE * EDGEDATA_PACKINGSIZE)) + x;
	
	// cout<<"UTIL_GETLOCALVID:: vid: "<<vid<<endl;
	// cout<<"UTIL_GETLOCALVID:: W: "<<W<<endl;
	// cout<<"UTIL_GETLOCALVID:: y: "<<y<<endl;
	// cout<<"UTIL_GETLOCALVID:: x: "<<x<<endl;
	// cout<<"UTIL_GETLOCALVID:: lvid: "<<lvid<<endl;
	// exit(EXIT_SUCCESS);
	return lvid;
}
unsigned int utility::UTIL_GETREALVID(unsigned int lvid, unsigned int instid){
	#pragma HLS INLINE

	unsigned int W = (VDATA_PACKINGNUMSETS_OFFLINE * EDGEDATA_PACKINGSIZE) * NUM_PEs;
	unsigned int y2 = lvid / (VDATA_PACKINGNUMSETS_OFFLINE * EDGEDATA_PACKINGSIZE);
	unsigned int x2 = lvid % (VDATA_PACKINGNUMSETS_OFFLINE * EDGEDATA_PACKINGSIZE);		
	unsigned int vid = (y2 * W) + (instid * (VDATA_PACKINGNUMSETS_OFFLINE * EDGEDATA_PACKINGSIZE)) + x2;
	return vid;
}

void utility::writedata(uint512_vec_dt * kvdram, unsigned int offset_kvs, unsigned int index, unsigned int data){
	unsigned int loc = index / 2;
	if(index % 2 == 0){ kvdram[offset_kvs + (loc / VECTOR_SIZE)].data[loc % VECTOR_SIZE].key = data; } 
	else { kvdram[offset_kvs + (loc / VECTOR_SIZE)].data[loc % VECTOR_SIZE].value = data; }	
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











