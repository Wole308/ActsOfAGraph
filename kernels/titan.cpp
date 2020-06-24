#include "/home/oj2zf/Documents/ActsOfAGraph/debugger/kernel_debugger.h"
#ifndef FPGA_IMPL
#include "/home/oj2zf/Documents/ActsOfAGraph/kernels/titan.h"
#endif 
#ifndef FPGA_IMPL
#include <chrono>
#endif 
using namespace std;
// #define OOO
#define INVALIDDATA 0

#ifdef _DEBUGMODE_STATS
int titan_kvpairsread0;
int titan_kvpairswritten0;
int titan_kvpairsread0_transfers;
int titan_kvpairswritten0_transfers;
#ifdef _BUFFERB4WRITE
int titan_kvpairswritten_bram2tmp0;
int titan_kvpairswritten_tmp2dram0;
int titan_kvpairswritten_bram2tmp0_transfers;
int titan_kvpairswritten_tmp2dram0_transfers;
#endif
int titan_kvpairswritten_interm2dest_parallel0;
int titan_kvpairswritten_interm2dest_seqential0;
int titan_INapplyphase;
edge_t titan_numedgesprocessedPE;
vertex_t titan_numverticesprocessedPE;
#endif


#ifndef FPGA_IMPL
titan::titan(){}
titan::~titan(){}
#endif
#ifndef HW

unsigned int globalvar_totalkvstatsread;
unsigned int globalvar_totalkvstatscollected;
unsigned int globalvar_totalkvstatswritten;

unsigned int globalvar_totalkvsread;

unsigned int globalstats_totalkvspartitioned;
unsigned int globalstats_totalkvspartitionswritten;

unsigned int globalstats_totalkvsreduced;
unsigned int globalstats_totalkvsreducewritten;
#endif 
#ifdef _LDEBUGMODE_HEADER
void titan::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound){
	if(data >= upper_bound){ std::cout<<"enigma::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<std::endl; exit(EXIT_FAILURE); }
}
void titan::checkforequal(string message, unsigned int data1, unsigned int data2){
	if(data1 == data2){ cout<<"enigma::checkforequal: ERROR. data1 == data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
}
void titan::print1(string messagea, unsigned int dataa){
	cout<<messagea<<": "<<dataa<<endl;
}
void titan::print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<endl;
}
void titan::print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<endl;
}
void titan::print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<", "<<messagef<<": "<<datef<<endl;
}
void titan::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; }
}
void titan::printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"["<<keyvalues1[p].key<<":"<<keyvalues1[p].value<<"]["<<keyvalues2[p].key<<":"<<keyvalues2[p].value<<"]"<<endl; }
}
void titan::printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs){
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
void titan::getvaluecount(string message, keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	cout<<"total values counted: "<<totalnumkeyvalues<<endl;
}
void countkeyvalues(){

}
void titan::printparameters(){
	cout<<endl<<"enigma::printparameters: test started."<<endl;
	cout<<"enigma::printparameters:: KVBUFFERSZ: "<<KVBUFFERSZ<<endl;
	cout<<"enigma::printparameters:: LSRCKVBUFFERSZ: "<<LSRCKVBUFFERSZ<<endl;
	cout<<"enigma::printparameters:: LSRCKVBUFFERSZ_KVS: "<<LSRCKVBUFFERSZ_KVS<<endl;
	cout<<"enigma::printparameters:: LDESTKVBUFFERSZ: "<<LDESTKVBUFFERSZ<<endl;
	cout<<"enigma::printparameters:: LDESTKVBUFFERSZ_KVS: "<<LDESTKVBUFFERSZ_KVS<<endl;
	cout<<"enigma::printparameters:: KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
	cout<<"enigma::printparameters:: KVDATA_BATCHSIZE_KVS: "<<KVDATA_BATCHSIZE_KVS<<endl;
	cout<<"enigma::printparameters:: LKVSTATSBIGBUFFERSZ_KVS: "<<LKVSTATSBIGBUFFERSZ_KVS<<endl;
}
#endif
#ifdef _DEBUGMODE_STATS
void titan::printglobalvars(){
	cout<<"printglobalvars: "<<endl;
	cout<<"enigma::printglobalvars:: COLLECTSTATS & SPREAD & PARTITION:: globalvar_totalkvsread: "<<globalvar_totalkvsread<<endl;
	
	cout<<"enigma::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatsread: "<<globalvar_totalkvstatsread<<endl;
	cout<<"enigma::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatscollected: "<<globalvar_totalkvstatscollected<<endl;
	cout<<"enigma::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatswritten: "<<globalvar_totalkvstatswritten<<endl;	

	cout<<"enigma::printglobalvars:: SPREAD:: globalstats_totalkvspartitioned: "<<globalstats_totalkvspartitioned<<endl;
	cout<<"enigma::printglobalvars:: SPREAD:: globalstats_totalkvspartitionswritten: "<<globalstats_totalkvspartitionswritten<<endl;
	
	cout<<"enigma::printglobalvars:: REDUCE:: globalstats_totalkvsreduced: "<<globalstats_totalkvsreduced<<endl;
	// cout<<"enigma::printglobalvars:: REDUCE:: globalstats_totalkvsreducewritten: "<<globalstats_totalkvsreducewritten<<endl;
}
void titan::clearglobalvars(){
	cout<<"clearglobalvars: "<<endl;
	globalvar_totalkvsread = 0;
	globalstats_totalkvspartitionswritten = 0;
	globalstats_totalkvspartitioned = 0;
	globalvar_totalkvstatsread = 0;
	globalvar_totalkvstatscollected = 0;
	globalvar_totalkvstatswritten = 0;
	globalstats_totalkvsreduced = 0;
	globalstats_totalkvsreducewritten = 0;
}
void titan::globalstats_countkvstatsread(unsigned int count){
	globalvar_totalkvstatsread += count;
	return;
}
void titan::globalstats_countkvstatscollected(unsigned int count){
	globalvar_totalkvstatscollected += count;
	return;
}
void titan::globalstats_countkvstatswritten(unsigned int count){
	globalvar_totalkvstatswritten += count;
	return;
}
void titan::globalstats_countkvsread(unsigned int count){
	globalvar_totalkvsread += count;
	return;
}
void titan::globalstats_countkvspartitionswritten(unsigned int count){
	globalstats_totalkvspartitionswritten += count;
	return;
}
void titan::globalstats_countkvspartitioned(unsigned int count){
	globalstats_totalkvspartitioned += count;
	return;
}
void titan::globalstats_countkvsreduced(unsigned int count){
	globalstats_totalkvsreduced += count;
	return;
}
void titan::globalstats_countkvsreducewritten(unsigned int count){
	globalstats_totalkvsreducewritten += count;
	return;
}
#endif 

/// Utility Functions
void titan::printstate(){
}

unsigned int titan::ceildiv(unsigned int val1, unsigned int val2){
	return (val1 + (val2 - 1)) / val2;
}

unsigned int titan::min8(unsigned int val0, unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4, unsigned int val5, unsigned int val6, unsigned int val7){
	unsigned int min = 7777777;	
	#ifdef _DEBUGMODE_CHECKS
	cout<<"min8: val0: "<<val0<<endl;
	cout<<"min8: val1: "<<val1<<endl;
	cout<<"min8: val2: "<<val2<<endl;
	cout<<"min8: val3: "<<val3<<endl;
	cout<<"min8: val4: "<<val4<<endl;
	cout<<"min8: val5: "<<val5<<endl;
	cout<<"min8: val6: "<<val6<<endl;
	cout<<"min8: val7: "<<val7<<endl;
	#endif	
	if(min > val0){ min = val0; }
	if(min > val1){ min = val1; }
	if(min > val2){ min = val2; }
	if(min > val3){ min = val3; }
	if(min > val4){ min = val4; }
	if(min > val5){ min = val5; }
	if(min > val6){ min = val6; }
	if(min > val7){ min = val7; }
	return min;
}

unsigned int titan::min(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
			
unsigned int titan::max(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}

unsigned int titan::minorzero(int val1){
	if(val1 > 0){ return val1; }
	else { return 0; }
}

unsigned int titan::allignup_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}

unsigned int titan::alligndown_KV(unsigned int val){
	unsigned int fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}

vertex_t titan::GETKVDATA_RANGE_PERSSDPARTITION(unsigned int ssdpartitonid){
	vertex_t range = 0;
	#ifdef _EVENWORLOADDISTRIBUTION
	if(ssdpartitonid < MAXNUMSSDPARTITIONS){
		range = KVDATA_RANGE_PERSSDPARTITION;
	} else { range = KVDATA_RANGE; } // NB: partitioning phase takes the DEFAULT
	#else
	range = KVDATA_RANGE_PERSSDPARTITION;
	else {
		range = KVDATA_RANGE; // NB: partitioning phase takes the DEFAULT
		#ifdef _DEBUGMODE_CHECKS2
		#ifndef PARTITION_PHASE
		std::cout<<"Kernel::GETKVDATA_RANGE_PERSSDPARTITION should never get here. titan::GETKVDATA_RANGE_PERSSDPARTITION 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
		#endif
	}
	#endif 
	return range;
}

vertex_t titan::GETKVDATA_RANGE_PERSSDPARTITION_POW(unsigned int ssdpartitonid){
	return KVDATA_RANGE_PERSSDPARTITION_POW;
}

vertex_t titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(unsigned int ssdpartitonid){
	return APPLYVERTEXBUFFERSZ_PERSSDPARTITION;
}

vertex_t titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(unsigned int ssdpartitonid){
	return APPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS;
}

unsigned int titan::GETTREEDEPTH(unsigned int ssdpartitonid){
	unsigned int depth = 0;
	#ifdef _EVENWORLOADDISTRIBUTION
	if(ssdpartitonid < MAXNUMSSDPARTITIONS){
		depth = TREEDEPTH_FORSSDPARTITION0;
	} else { depth = 1; } // NB: partitioning phase takes the DEFAULT
	#else 
if(ssdpartitonid == 0){
		depth = TREEDEPTH_FORSSDPARTITION0;
	}
else if(ssdpartitonid == 1){
		depth = TREEDEPTH_FORSSDPARTITION1;
	}
else if(ssdpartitonid == 2){
		depth = TREEDEPTH_FORSSDPARTITION2;
	}
else if(ssdpartitonid == 3){
		depth = TREEDEPTH_FORSSDPARTITION3;
	}
else if(ssdpartitonid == 4){
		depth = TREEDEPTH_FORSSDPARTITION4;
	}
else if(ssdpartitonid == 5){
		depth = TREEDEPTH_FORSSDPARTITION5;
	}
else if(ssdpartitonid == 6){
		depth = TREEDEPTH_FORSSDPARTITION6;
	}
else if(ssdpartitonid == 7){
		depth = TREEDEPTH_FORSSDPARTITION7;
	}
else if(ssdpartitonid == 8){
		depth = TREEDEPTH_FORSSDPARTITION8;
	}
else if(ssdpartitonid == 9){
		depth = TREEDEPTH_FORSSDPARTITION9;
	}
else if(ssdpartitonid == 10){
		depth = TREEDEPTH_FORSSDPARTITION10;
	}
else if(ssdpartitonid == 11){
		depth = TREEDEPTH_FORSSDPARTITION11;
	}
else if(ssdpartitonid == 12){
		depth = TREEDEPTH_FORSSDPARTITION12;
	}
else if(ssdpartitonid == 13){
		depth = TREEDEPTH_FORSSDPARTITION13;
	}
else if(ssdpartitonid == 14){
		depth = TREEDEPTH_FORSSDPARTITION14;
	}
else if(ssdpartitonid == 15){
		depth = TREEDEPTH_FORSSDPARTITION15;
	}
	else {
		// depth = TREEDEPTH_FORSSDPARTITION0;
		depth = 1; // NB: partitioning phase takes the DEFAULT
		#ifdef _DEBUGMODE_CHECKS2
		#ifndef PARTITION_PHASE
		std::cout<<"Kernel:: titan::GETTREEDEPTH should never get here.  titan::GETTREEDEPTH 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
		#endif
	}
	#endif 
	return depth;
}

unsigned int titan::hashfunc(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, vertex_t range){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::titan::hashfunc: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", range: "<<range<<std::endl;
	#endif

	unsigned int hashedval = 0;
	if(currentLOP == 1){
		hashedval = (kventry.key - upperlimit) / (range / NUM_PARTITIONS);
	} else if(currentLOP == 2){
		hashedval = (kventry.key - upperlimit) / ((range / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else if(currentLOP == 3){
		hashedval = (kventry.key - upperlimit) / (((range / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else if(currentLOP == 4){
		hashedval = (kventry.key - upperlimit) / ((((range / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else if(currentLOP == 5){
		hashedval = (kventry.key - upperlimit) / (((((range / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else {
		hashedval = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"Kernel::titan::hashfunc should never get here. titan::hashfunc 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_CHECKS2
	if(hashedval >= NUM_PARTITIONS){
		std::cout<<"Kernel::titan::hashfunc:: out-of-bounds 45. hashedval: "<<hashedval<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", range: "<<range<<", currentLOP: "<<currentLOP<<std::endl;
		exit(EXIT_FAILURE);
	}
	#endif
	if(hashedval >= FORCDFINISH_NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}

unsigned int titan::hashfunc2(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int ssdpartitonid){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::titan::hashfunc2: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<std::endl;
	#endif

	unsigned int hashedval = 0;
	if(currentLOP == 1){
		hashedval = (kventry.key - upperlimit) / (titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / NUM_PARTITIONS);		
	} else if(currentLOP == 2){
		hashedval = (kventry.key - upperlimit) / ((titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else if(currentLOP == 3){
		hashedval = (kventry.key - upperlimit) / (((titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else if(currentLOP == 4){
		hashedval = (kventry.key - upperlimit) / ((((titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else if(currentLOP == 5){
		hashedval = (kventry.key - upperlimit) / (((((titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else {
		hashedval = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"Kernel::titan::hashfunc2 should never get here. titan::hashfunc2 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_CHECKS2
	if(hashedval >= NUM_PARTITIONS){
		std::cout<<"Kernel::titan::hashfunc2:: out-of-bounds 45. hashedval: "<<hashedval<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<", currentLOP: "<<currentLOP<<std::endl;
		exit(EXIT_FAILURE);
	}
	#endif
	if(hashedval >= FORCDFINISH_NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}

unsigned int titan::hashfunc3(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int ssdpartitonid){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::titan::hashfunc3: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<std::endl;
	#endif

	unsigned int hashedval = 0;
	hashedval = (kventry.key - upperlimit) / (titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / (1 << (NUM_PARTITIONS_POW * currentLOP)));
	#ifdef _DEBUGMODE_CHECKS2
	if(hashedval >= NUM_PARTITIONS){
		std::cout<<"Kernel::titan::hashfunc3:: out-of-bounds 45. hashedval: "<<hashedval<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<", currentLOP: "<<currentLOP<<std::endl;
		exit(EXIT_FAILURE);
	}
	#endif
	if(hashedval >= FORCDFINISH_NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}

unsigned int titan::hashfunc4(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int ssdpartitonid){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::titan::hashfunc4: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<std::endl;
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	if(kventry.key < upperlimit){
		if(kventry.key != INVALIDDATA){ std::cout<<"Kernel::titan::hashfunc4:: out-of-bounds 44. kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<", currentLOP: "<<currentLOP<<endl; }
		if(kventry.key != INVALIDDATA){ exit(EXIT_FAILURE); }
		// exit(EXIT_FAILURE);
	}
	#endif

	unsigned int hashedval = 0;
	hashedval = ((kventry.key - upperlimit) >> (titan::GETKVDATA_RANGE_PERSSDPARTITION_POW(ssdpartitonid) - (NUM_PARTITIONS_POW * currentLOP)));
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::titan::hashfunc4: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<", hashedval: "<<hashedval<<", currentLOP: "<<currentLOP<<", titan::GETKVDATA_RANGE_PERSSDPARTITION_POW("<<ssdpartitonid<<"): "<<titan::GETKVDATA_RANGE_PERSSDPARTITION_POW(ssdpartitonid)<<std::endl;
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	if(hashedval >= NUM_PARTITIONS){
		if(kventry.key != INVALIDDATA){ std::cout<<"Kernel::titan::hashfunc4:: out-of-bounds 45. hashedval: "<<hashedval<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<", currentLOP: "<<currentLOP<<", titan::GETKVDATA_RANGE_PERSSDPARTITION_POW("<<ssdpartitonid<<"): "<<titan::GETKVDATA_RANGE_PERSSDPARTITION_POW(ssdpartitonid)<<", (NUM_PARTITIONS_POW * currentLOP): "<<(NUM_PARTITIONS_POW * currentLOP)<<std::endl;	}
		if(kventry.key != INVALIDDATA){ titan::printstate(); }
		if(kventry.key != INVALIDDATA){ exit(EXIT_FAILURE); }
		// exit(EXIT_FAILURE);
	}
	#endif
	if(hashedval >= FORCDFINISH_NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}

unsigned int titan::hashfunc5(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int range){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::titan::hashfunc5: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<std::endl;
	#endif

	unsigned int hashedval = 0;
	#pragma HLS RESOURCE variable=hashedval core=DivnS
	hashedval = (kventry.key - upperlimit) / range;
	#ifdef _DEBUGMODE_CHECKS2
	if(hashedval >= NUM_PARTITIONS){
		std::cout<<"Kernel::titan::hashfunc5:: out-of-bounds 45. hashedval: "<<hashedval<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<std::endl;
		exit(EXIT_FAILURE);
	}
	#endif
	if(hashedval >= FORCDFINISH_NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}

keyvalue_t titan::getkventry(int addr, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	unsigned int idy = (addr / VECTOR_SIZE);
	unsigned int idx = ((addr * 2) % (VECTOR_SIZE * 2));
	unsigned int idx_plus1 = idx + 1;
	#ifdef _WIDEWORD
	keyvalue_t kventry;
	kventry.key = kvbuffer[(idy % FORCDFINISH_KVSRCBUFFERSZ_KVS)].range(DATATYPE_SIZE * (idx + 1) - 1, idx * DATATYPE_SIZE); 
	kventry.value = kvbuffer[(idy % FORCDFINISH_KVSRCBUFFERSZ_KVS)].range(DATATYPE_SIZE * (idx_plus1 + 1) - 1, idx_plus1 * DATATYPE_SIZE); 
	return kventry;
	#else 
	return kvbuffer[(idy % FORCDFINISH_KVSRCBUFFERSZ_KVS)].data[idx]; 
	#endif 
}

keyvalue_t titan::getkventryII(int addr, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	if(addr >= FORCDFINISH_KVSRCBUFFERSZ){ addr = 0; }
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(idy, KVSRCBUFFERSZ_KVS, "titan::getkventryII:: out-of-bounds seen at titan::getkventryII 5", 0, 0, 0);
	checkforoutofbounds_kernel(idx, VECTOR_SIZE, "titan::getkventryII:: out-of-bounds seen at titan::getkventryII 6", 0, 0, 0);
	#endif
	#ifdef _WIDEWORD
	keyvalue_t kventry;
	unsigned int ihi = ((VECTOR_SIZE - idx) * 32) - 1;
	unsigned int ilo = ihi - 31;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(ihi, DATAWIDTH, "titan::getkventryII:: out-of-bounds seen at titan::getkventryII 7", 0, 0, 0);
	#endif
if(idx==0){
		kventry.key = kvbuffer[idy].range(31, 0);
		kventry.value = kvbuffer[idy].range(63, 32);
	}
else if(idx==1){
		kventry.key = kvbuffer[idy].range(95, 64);
		kventry.value = kvbuffer[idy].range(127, 96);
	}
else if(idx==2){
		kventry.key = kvbuffer[idy].range(159, 128);
		kventry.value = kvbuffer[idy].range(191, 160);
	}
else if(idx==3){
		kventry.key = kvbuffer[idy].range(223, 192);
		kventry.value = kvbuffer[idy].range(255, 224);
	}
else if(idx==4){
		kventry.key = kvbuffer[idy].range(287, 256);
		kventry.value = kvbuffer[idy].range(319, 288);
	}
else if(idx==5){
		kventry.key = kvbuffer[idy].range(351, 320);
		kventry.value = kvbuffer[idy].range(383, 352);
	}
else if(idx==6){
		kventry.key = kvbuffer[idy].range(415, 384);
		kventry.value = kvbuffer[idy].range(447, 416);
	}
else if(idx==7){
		kventry.key = kvbuffer[idy].range(479, 448);
		kventry.value = kvbuffer[idy].range(511, 480);
	}
	else {
		kventry.key = 0; 
		kventry.value = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"titan::getkventryII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return kventry;
	#else
	return kvbuffer[idy].data[idx];
	#endif
}

keyvalue_t titan::getkventryIII(int addr, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#pragma HLS INLINE // "unlike titan::getkventryIII, this is inlined"
	if(addr >= FORCDFINISH_KVSRCBUFFERSZ){ addr = 0; }
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(idy, KVSRCBUFFERSZ_KVS, "titan::getkventryIII:: out-of-bounds seen at titan::getkventryIII 5", 0, 0, 0);
	checkforoutofbounds_kernel(idx, VECTOR_SIZE, "titan::getkventryIII:: out-of-bounds seen at titan::getkventryIII 6", 0, 0, 0);
	#endif
	#ifdef _WIDEWORD
	keyvalue_t kventry;
	unsigned int ihi = ((VECTOR_SIZE - idx) * 32) - 1;
	unsigned int ilo = ihi - 31;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(ihi, DATAWIDTH, "titan::getkventryIII:: out-of-bounds seen at titan::getkventryIII 7", 0, 0, 0);
	#endif
if(idx==0){
		kventry.key = kvbuffer[idy].range(31, 0);
		kventry.value = kvbuffer[idy].range(63, 32);
	}
else if(idx==1){
		kventry.key = kvbuffer[idy].range(95, 64);
		kventry.value = kvbuffer[idy].range(127, 96);
	}
else if(idx==2){
		kventry.key = kvbuffer[idy].range(159, 128);
		kventry.value = kvbuffer[idy].range(191, 160);
	}
else if(idx==3){
		kventry.key = kvbuffer[idy].range(223, 192);
		kventry.value = kvbuffer[idy].range(255, 224);
	}
else if(idx==4){
		kventry.key = kvbuffer[idy].range(287, 256);
		kventry.value = kvbuffer[idy].range(319, 288);
	}
else if(idx==5){
		kventry.key = kvbuffer[idy].range(351, 320);
		kventry.value = kvbuffer[idy].range(383, 352);
	}
else if(idx==6){
		kventry.key = kvbuffer[idy].range(415, 384);
		kventry.value = kvbuffer[idy].range(447, 416);
	}
else if(idx==7){
		kventry.key = kvbuffer[idy].range(479, 448);
		kventry.value = kvbuffer[idy].range(511, 480);
	}
	else {
		kventry.key = 0; 
		kventry.value = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"titan::getkventryIII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return kventry;
	#else
	return kvbuffer[idy].data[idx];
	#endif
}

keyvalue_t titan::getkventryIV(int addr, uint512_dt kvbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#pragma HLS INLINE
	if(addr >= APPLYVERTEXBUFFERSZ){ addr = 0; }
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(idy, APPLYVERTEXBUFFERSZ_KVS, "titan::getkventryIV:: out-of-bounds seen at titan::getkventryIV 5", 0, 0, 0);
	checkforoutofbounds_kernel(idx, VECTOR_SIZE, "titan::getkventryIV:: out-of-bounds seen at titan::getkventryIV 6", 0, 0, 0);
	#endif
	#ifdef _WIDEWORD
	keyvalue_t kventry;
	unsigned int ihi = ((VECTOR_SIZE - idx) * 32) - 1;
	unsigned int ilo = ihi - 31;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(ihi, DATAWIDTH, "titan::getkventryIV:: out-of-bounds seen at titan::getkventryIV 7", 0, 0, 0);
	#endif
if(idx==0){
		kventry.key = kvbuffer[idy].range(31, 0);
		kventry.value = kvbuffer[idy].range(63, 32);
	}
else if(idx==1){
		kventry.key = kvbuffer[idy].range(95, 64);
		kventry.value = kvbuffer[idy].range(127, 96);
	}
else if(idx==2){
		kventry.key = kvbuffer[idy].range(159, 128);
		kventry.value = kvbuffer[idy].range(191, 160);
	}
else if(idx==3){
		kventry.key = kvbuffer[idy].range(223, 192);
		kventry.value = kvbuffer[idy].range(255, 224);
	}
else if(idx==4){
		kventry.key = kvbuffer[idy].range(287, 256);
		kventry.value = kvbuffer[idy].range(319, 288);
	}
else if(idx==5){
		kventry.key = kvbuffer[idy].range(351, 320);
		kventry.value = kvbuffer[idy].range(383, 352);
	}
else if(idx==6){
		kventry.key = kvbuffer[idy].range(415, 384);
		kventry.value = kvbuffer[idy].range(447, 416);
	}
else if(idx==7){
		kventry.key = kvbuffer[idy].range(479, 448);
		kventry.value = kvbuffer[idy].range(511, 480);
	}
	else {
		kventry.key = 0; 
		kventry.value = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"titan::getkventryIV:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return kventry;
	#else
	return kvbuffer[idy].data[idx];
	#endif
}

void titan::setkventry(uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS], unsigned int addr, keyvalue_t kventry){
	unsigned int idy = (addr / VECTOR_SIZE);
	unsigned int idx = ((addr * 2) % (VECTOR_SIZE * 2));
	unsigned int idx_plus1 = idx + 1;
	#ifdef _WIDEWORD
	kvbuffer[(idy % FORCDFINISH_KVSRCBUFFERSZ_KVS)].range(DATATYPE_SIZE * (idx + 1) - 1, idx * DATATYPE_SIZE) = kventry.key; 
	kvbuffer[(idy % FORCDFINISH_KVSRCBUFFERSZ_KVS)].range(DATATYPE_SIZE * (idx_plus1 + 1) - 1, idx_plus1 * DATATYPE_SIZE) = kventry.value; 
	#else 
	kvbuffer[(idy % FORCDFINISH_KVSRCBUFFERSZ_KVS)].data[idx] = kventry; 
	#endif 
	return;
}

void titan::setkventryII(uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS], unsigned int addr, keyvalue_t kventry){
	if(addr >= FORCDFINISH_KVSRCBUFFERSZ){ addr = 0; }
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(idy, KVSRCBUFFERSZ_KVS, "titan::setkventryII:: out-of-bounds seen at titan::setkventryII 5", 0, 0, 0);
	checkforoutofbounds_kernel(idx, VECTOR_SIZE, "titan::setkventryII:: out-of-bounds seen at titan::setkventryII 6", 0, 0, 0);
	#endif
	#ifdef _WIDEWORD
	unsigned int ihi = ((VECTOR_SIZE - idx) * 32) - 1;
	unsigned int ilo = ihi - 31;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(ihi, DATAWIDTH, "titan::setkventryII:: out-of-bounds seen at titan::setkventryII 7", 0, 0, 0);
	#endif
if(idx==0){	
		kvbuffer[idy].range(31, 0) = kventry.key;
		kvbuffer[idy].range(63, 32) = kventry.value;
	}
else if(idx==1){	
		kvbuffer[idy].range(95, 64) = kventry.key;
		kvbuffer[idy].range(127, 96) = kventry.value;
	}
else if(idx==2){	
		kvbuffer[idy].range(159, 128) = kventry.key;
		kvbuffer[idy].range(191, 160) = kventry.value;
	}
else if(idx==3){	
		kvbuffer[idy].range(223, 192) = kventry.key;
		kvbuffer[idy].range(255, 224) = kventry.value;
	}
else if(idx==4){	
		kvbuffer[idy].range(287, 256) = kventry.key;
		kvbuffer[idy].range(319, 288) = kventry.value;
	}
else if(idx==5){	
		kvbuffer[idy].range(351, 320) = kventry.key;
		kvbuffer[idy].range(383, 352) = kventry.value;
	}
else if(idx==6){	
		kvbuffer[idy].range(415, 384) = kventry.key;
		kvbuffer[idy].range(447, 416) = kventry.value;
	}
else if(idx==7){	
		kvbuffer[idy].range(479, 448) = kventry.key;
		kvbuffer[idy].range(511, 480) = kventry.value;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"titan::setkventryII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	#else
	kvbuffer[idy].data[idx] = kventry;
	#endif
	return;
}

void titan::setkventryIII(
		keyvalue_t kvbuffer_vec0[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec1[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec2[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec3[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec4[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec5[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec6[KVDESTBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec7[KVDESTBUFFERSZ_KVS], 
		unsigned int addr, keyvalue_t kventry){
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
if(idx==0){	
		kvbuffer_vec0[idy].key = kventry.key;
		kvbuffer_vec0[idy].value = kventry.value;
	}
else if(idx==1){	
		kvbuffer_vec1[idy].key = kventry.key;
		kvbuffer_vec1[idy].value = kventry.value;
	}
else if(idx==2){	
		kvbuffer_vec2[idy].key = kventry.key;
		kvbuffer_vec2[idy].value = kventry.value;
	}
else if(idx==3){	
		kvbuffer_vec3[idy].key = kventry.key;
		kvbuffer_vec3[idy].value = kventry.value;
	}
else if(idx==4){	
		kvbuffer_vec4[idy].key = kventry.key;
		kvbuffer_vec4[idy].value = kventry.value;
	}
else if(idx==5){	
		kvbuffer_vec5[idy].key = kventry.key;
		kvbuffer_vec5[idy].value = kventry.value;
	}
else if(idx==6){	
		kvbuffer_vec6[idy].key = kventry.key;
		kvbuffer_vec6[idy].value = kventry.value;
	}
else if(idx==7){	
		kvbuffer_vec7[idy].key = kventry.key;
		kvbuffer_vec7[idy].value = kventry.value;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"titan::setkventryIII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return;
}

void titan::setkventry4(
		keyvalue_t kvbuffer_vec0[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec1[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec2[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec3[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec4[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec5[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec6[APPLYVERTEXBUFFERSZ_KVS], 
		keyvalue_t kvbuffer_vec7[APPLYVERTEXBUFFERSZ_KVS], 
		unsigned int addr, keyvalue_t kventry){
	#pragma HLS INLINE
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
if(idx==0){	
		kvbuffer_vec0[idy].key = kventry.key;
		kvbuffer_vec0[idy].value = kventry.value;
	}
else if(idx==1){	
		kvbuffer_vec1[idy].key = kventry.key;
		kvbuffer_vec1[idy].value = kventry.value;
	}
else if(idx==2){	
		kvbuffer_vec2[idy].key = kventry.key;
		kvbuffer_vec2[idy].value = kventry.value;
	}
else if(idx==3){	
		kvbuffer_vec3[idy].key = kventry.key;
		kvbuffer_vec3[idy].value = kventry.value;
	}
else if(idx==4){	
		kvbuffer_vec4[idy].key = kventry.key;
		kvbuffer_vec4[idy].value = kventry.value;
	}
else if(idx==5){	
		kvbuffer_vec5[idy].key = kventry.key;
		kvbuffer_vec5[idy].value = kventry.value;
	}
else if(idx==6){	
		kvbuffer_vec6[idy].key = kventry.key;
		kvbuffer_vec6[idy].value = kventry.value;
	}
else if(idx==7){	
		kvbuffer_vec7[idy].key = kventry.key;
		kvbuffer_vec7[idy].value = kventry.value;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"titan::setkventryIII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return;
}

keyvalue_t titan::getkventry3(uint512_dt tmpV1, unsigned int idx){
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(idx, VECTOR_SIZE, "titan::getkventry3:: out-of-bounds seen at titan::getkventry3 6", 0, 0, 0);
	#endif
	#ifdef _WIDEWORD
	keyvalue_t kventry;
	unsigned int ihi = ((VECTOR_SIZE - idx) * 32) - 1;
	unsigned int ilo = ihi - 31;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(ihi, DATAWIDTH, "titan::getkventry3:: out-of-bounds seen at titan::getkventry3 7", 0, 0, 0);
	#endif
if(idx==0){
		kventry.key = tmpV1.range(31, 0);
		kventry.value = tmpV1.range(63, 32);
	}
else if(idx==1){
		kventry.key = tmpV1.range(95, 64);
		kventry.value = tmpV1.range(127, 96);
	}
else if(idx==2){
		kventry.key = tmpV1.range(159, 128);
		kventry.value = tmpV1.range(191, 160);
	}
else if(idx==3){
		kventry.key = tmpV1.range(223, 192);
		kventry.value = tmpV1.range(255, 224);
	}
else if(idx==4){
		kventry.key = tmpV1.range(287, 256);
		kventry.value = tmpV1.range(319, 288);
	}
else if(idx==5){
		kventry.key = tmpV1.range(351, 320);
		kventry.value = tmpV1.range(383, 352);
	}
else if(idx==6){
		kventry.key = tmpV1.range(415, 384);
		kventry.value = tmpV1.range(447, 416);
	}
else if(idx==7){
		kventry.key = tmpV1.range(479, 448);
		kventry.value = tmpV1.range(511, 480);
	}
	else {
		kventry.key = 0; 
		kventry.value = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"titan::getkventry3:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return kventry;
	#else
	return tmpV1.data[idx];
	#endif
}

keyvalue_t titan::getkventry4(
		keyvalue_t kvbuffer_vec0[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec1[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec2[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec3[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec4[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec5[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec6[KVSRCBUFFERSZ_APPLY_KVS], 
		keyvalue_t kvbuffer_vec7[KVSRCBUFFERSZ_APPLY_KVS], 
		unsigned int addr){
	#pragma HLS INLINE
	keyvalue_t kventry;
	#pragma HLS DATA_PACK variable kventry
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
if(idx==0){	
		kventry.key = kvbuffer_vec0[idy].key;
		kventry.value = kvbuffer_vec0[idy].value;
	}
else if(idx==1){	
		kventry.key = kvbuffer_vec1[idy].key;
		kventry.value = kvbuffer_vec1[idy].value;
	}
else if(idx==2){	
		kventry.key = kvbuffer_vec2[idy].key;
		kventry.value = kvbuffer_vec2[idy].value;
	}
else if(idx==3){	
		kventry.key = kvbuffer_vec3[idy].key;
		kventry.value = kvbuffer_vec3[idy].value;
	}
else if(idx==4){	
		kventry.key = kvbuffer_vec4[idy].key;
		kventry.value = kvbuffer_vec4[idy].value;
	}
else if(idx==5){	
		kventry.key = kvbuffer_vec5[idy].key;
		kventry.value = kvbuffer_vec5[idy].value;
	}
else if(idx==6){	
		kventry.key = kvbuffer_vec6[idy].key;
		kventry.value = kvbuffer_vec6[idy].value;
	}
else if(idx==7){	
		kventry.key = kvbuffer_vec7[idy].key;
		kventry.value = kvbuffer_vec7[idy].value;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"titan::setkventryIII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return kventry;
}

unsigned int titan::getsourceskipsize(unsigned int currentLOP, unsigned int param){
	unsigned int treedepth = TREEDEPTH_FORSSDPARTITION0;
	unsigned int _finalnumpartitions = param;
	for(unsigned int i=0; i<(currentLOP-1); i++){ // FIXME?
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		_finalnumpartitions = _finalnumpartitions / NUM_PARTITIONS;
	}
	return _finalnumpartitions;
}

unsigned int titan::getdestskipsize(unsigned int currentLOP, unsigned int param){
	unsigned int treedepth = TREEDEPTH_FORSSDPARTITION0; // NB: for debugging purposes only
	unsigned int _finalnumpartitions = param / NUM_PARTITIONS;
	for(unsigned int i=0; i<(currentLOP-1); i++){ // FIXME?
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		_finalnumpartitions = _finalnumpartitions / NUM_PARTITIONS;
	}
	return _finalnumpartitions;
}

void titan::loaddeststats(metadata_t * kvstats, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t finalnumpartitions){
	unsigned int skipsize = titan::getdestskipsize(currentLOP, finalnumpartitions);
	unsigned int nxtpos = partitionoffset;
	#ifdef _DEBUGMODE_PRINTS
	cout<<"titan::loaddeststats:: currentLOP: "<<currentLOP<<", skipsize: "<<skipsize<<", partitionoffset: "<<partitionoffset<<", finalnumpartitions: "<<finalnumpartitions<<endl;
	#endif
	LOADDESTSTATS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel(p, NUM_PARTITIONS, "titan::loaddeststats:: out-of-bounds seen at titan::loaddeststats 5", 0, 0, 0);
		checkforoutofbounds_kernel((skipsize * p), KVSTATS_SIZE, "titan::loaddeststats:: out-of-bounds seen at titan::loaddeststats 6", 0, 0, 0);
		#endif
		
		/* kvdeststats_tmp[p] = kvstats[((partitionoffset + (p * skipsize)) % FORCDFINISH_KVSTATS_SIZE)];  */
		/* kvdeststats_tmp[p] = kvstats[nxtpos]; 
		nxtpos += skipsize; */		
		kvdeststats_tmp[p] = kvstats[(nxtpos % FORCDFINISH_KVSTATS_SIZE)]; 
		nxtpos += skipsize;
		
		#ifdef _DEBUGMODE_PRINTS
		cout<<"titan::loaddeststats:: kvdeststats_tmp["<<p<<"].offset: "<<kvdeststats_tmp[p].offset<<", kvdeststats_tmp["<<p<<"].size: "<<kvdeststats_tmp[p].size<<", ";
		cout<<"titan::loaddeststats:: kvstats["<<(partitionoffset + (p * skipsize))<<"].offset: "<<kvstats[(partitionoffset + (p * skipsize))].offset<<", kvstats["<<(partitionoffset + (p * skipsize))<<"].size: "<<kvstats[(partitionoffset + (p * skipsize))].size;
		cout<<endl;
		#endif
	}
	LOADDESTSTATS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		kvdeststats_tmp[p].size = 0;
	}
	return;
}

void titan::storedeststats(metadata_t * kvstats, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t finalnumpartitions){
	unsigned int skipsize = titan::getdestskipsize(currentLOP, finalnumpartitions);
	unsigned int nxtpos = partitionoffset;
	#ifdef _DEBUGMODE_PRINTS
	cout<<"titan::storedeststats:: currentLOP: "<<currentLOP<<", skipsize: "<<skipsize<<", partitionoffset: "<<partitionoffset<<", finalnumpartitions: "<<finalnumpartitions<<endl;
	#endif
	STOREDESTSTATS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel(p, NUM_PARTITIONS, "titan::storedeststats:: out-of-bounds seen at titan::storedeststats 5", 0, 0, 0);
		checkforoutofbounds_kernel((skipsize * p), KVSTATS_SIZE, "titan::storedeststats:: out-of-bounds seen at titan::storedeststats 6", 0, 0, 0);
		#endif
		
		/* kvstats[((partitionoffset + (p * skipsize)) % FORCDFINISH_KVSTATS_SIZE)].size = kvdeststats_tmp[p].size; */
		/* kvstats[nxtpos].size = kvdeststats_tmp[p].size; 
		nxtpos += skipsize; */
		kvstats[(nxtpos % FORCDFINISH_KVSTATS_SIZE)].size = kvdeststats_tmp[p].size; 
		nxtpos += skipsize;
		
		#ifdef _DEBUGMODE_PRINTS
		cout<<"titan::storedeststats:: kvdeststats_tmp["<<p<<"].offset: "<<kvdeststats_tmp[p].offset<<", kvdeststats_tmp["<<p<<"].size: "<<kvdeststats_tmp[p].size<<", ";
		cout<<"titan::storedeststats:: kvstats["<<(partitionoffset + (p * skipsize))<<"].offset: "<<kvstats[(partitionoffset + (p * skipsize))].offset<<", kvstats["<<(partitionoffset + (p * skipsize))<<"].size: "<<kvstats[(partitionoffset + (p * skipsize))].size;
		cout<<endl;
		#endif
	}
	STOREDESTSTATS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		kvdeststats_tmp[p].size = 0;
	}
	return;
}

unsigned int titan::get_num_source_partitions(unsigned int currentLOP){
	unsigned int treedepth = TREEDEPTH_FORSSDPARTITION0;
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}

unsigned int titan::reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount){
	unsigned int ret = 0;
	#ifdef PR_ALGORITHM
	ret = value + edgeval;
	#elif defined(BFS_ALGORITHM)
	// ret = 777;
	// cout<<"************** reducefunc: "<<endl; exit(EXIT_SUCCESS);
	ret = min(value, IterCount);
	// cout<<"************** reducefunc: ret: "<<ret<<", value: "<<value<<", IterCount: "<<IterCount<<endl; exit(EXIT_SUCCESS);
	#elif defined(BC_ALGORITHM)
	// ret = min(value, edgeval);
	ret = min(value, IterCount);
	#endif
	return ret;
}

/// Main Functions
// READ
void titan::read_ddr0_w0_sw0(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only

	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	READBURSTKVS00_LOOP: for(kvbuffersz_dtype i=0; i<KVBUFFERSZ_KVS; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read00:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read00:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::read_ddr0_w0_sw1(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only

	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	READBURSTKVS01_LOOP: for(kvbuffersz_dtype i=0; i<KVBUFFERSZ_KVS; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read01:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read01:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::read_ddr0_w0_sw2(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only

	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	READBURSTKVS02_LOOP: for(kvbuffersz_dtype i=0; i<KVBUFFERSZ_KVS; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read02:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read02:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::read_ddr0_w0_sw3(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only

	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	READBURSTKVS03_LOOP: for(kvbuffersz_dtype i=0; i<KVBUFFERSZ_KVS; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read03:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read03:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::read_ddr0_w0_sw4(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only

	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	READBURSTKVS04_LOOP: for(kvbuffersz_dtype i=0; i<KVBUFFERSZ_KVS; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read04:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read04:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::read_ddr0_w0_sw5(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only

	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	READBURSTKVS05_LOOP: for(kvbuffersz_dtype i=0; i<KVBUFFERSZ_KVS; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read05:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read05:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::read_ddr0_w0_sw6(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only

	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	READBURSTKVS06_LOOP: for(kvbuffersz_dtype i=0; i<KVBUFFERSZ_KVS; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read06:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read06:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::read_ddr0_w0_sw7(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only

	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	READBURSTKVS07_LOOP: for(kvbuffersz_dtype i=0; i<KVBUFFERSZ_KVS; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read07:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read07:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}

void titan::readAU_VU_ddr0_w0_sw0(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVSRCBUFFERSZ_APPLY_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_tracker_kvs.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read00:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY_KVS){ rwsize_kvs = KVSRCBUFFERSZ_APPLY_KVS; } 
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS00_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read00:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read00:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read00:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read00:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_APPLY_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		// for(unsigned int k=0; k<VECTOR_SIZE; k++){ cout<<"readAU_VU_ddr: kvdram["<<(dramoffset + i)<<"].data["<<k<<"].key: "<<kvdram[(dramoffset + i)].data[k].key<<", kvdram["<<(dramoffset + i)<<"].data["<<k<<"].value: "<<kvdram[(dramoffset + i)].data[k].value<<endl; } // FIXME. REMOVEME
		// exit(EXIT_SUCCESS);
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_VU_ddr0_w0_sw1(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVSRCBUFFERSZ_APPLY_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_tracker_kvs.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read01:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY_KVS){ rwsize_kvs = KVSRCBUFFERSZ_APPLY_KVS; } 
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS01_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read01:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read01:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read01:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read01:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_APPLY_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		// for(unsigned int k=0; k<VECTOR_SIZE; k++){ cout<<"readAU_VU_ddr: kvdram["<<(dramoffset + i)<<"].data["<<k<<"].key: "<<kvdram[(dramoffset + i)].data[k].key<<", kvdram["<<(dramoffset + i)<<"].data["<<k<<"].value: "<<kvdram[(dramoffset + i)].data[k].value<<endl; } // FIXME. REMOVEME
		// exit(EXIT_SUCCESS);
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_VU_ddr0_w0_sw2(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVSRCBUFFERSZ_APPLY_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_tracker_kvs.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read02:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY_KVS){ rwsize_kvs = KVSRCBUFFERSZ_APPLY_KVS; } 
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS02_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read02:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read02:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read02:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read02:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_APPLY_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		// for(unsigned int k=0; k<VECTOR_SIZE; k++){ cout<<"readAU_VU_ddr: kvdram["<<(dramoffset + i)<<"].data["<<k<<"].key: "<<kvdram[(dramoffset + i)].data[k].key<<", kvdram["<<(dramoffset + i)<<"].data["<<k<<"].value: "<<kvdram[(dramoffset + i)].data[k].value<<endl; } // FIXME. REMOVEME
		// exit(EXIT_SUCCESS);
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_VU_ddr0_w0_sw3(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVSRCBUFFERSZ_APPLY_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_tracker_kvs.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read03:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY_KVS){ rwsize_kvs = KVSRCBUFFERSZ_APPLY_KVS; } 
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS03_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read03:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read03:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read03:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read03:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_APPLY_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		// for(unsigned int k=0; k<VECTOR_SIZE; k++){ cout<<"readAU_VU_ddr: kvdram["<<(dramoffset + i)<<"].data["<<k<<"].key: "<<kvdram[(dramoffset + i)].data[k].key<<", kvdram["<<(dramoffset + i)<<"].data["<<k<<"].value: "<<kvdram[(dramoffset + i)].data[k].value<<endl; } // FIXME. REMOVEME
		// exit(EXIT_SUCCESS);
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_VU_ddr0_w0_sw4(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVSRCBUFFERSZ_APPLY_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_tracker_kvs.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read04:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY_KVS){ rwsize_kvs = KVSRCBUFFERSZ_APPLY_KVS; } 
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS04_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read04:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read04:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read04:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read04:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_APPLY_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		// for(unsigned int k=0; k<VECTOR_SIZE; k++){ cout<<"readAU_VU_ddr: kvdram["<<(dramoffset + i)<<"].data["<<k<<"].key: "<<kvdram[(dramoffset + i)].data[k].key<<", kvdram["<<(dramoffset + i)<<"].data["<<k<<"].value: "<<kvdram[(dramoffset + i)].data[k].value<<endl; } // FIXME. REMOVEME
		// exit(EXIT_SUCCESS);
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_VU_ddr0_w0_sw5(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVSRCBUFFERSZ_APPLY_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_tracker_kvs.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read05:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY_KVS){ rwsize_kvs = KVSRCBUFFERSZ_APPLY_KVS; } 
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS05_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read05:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read05:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read05:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read05:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_APPLY_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		// for(unsigned int k=0; k<VECTOR_SIZE; k++){ cout<<"readAU_VU_ddr: kvdram["<<(dramoffset + i)<<"].data["<<k<<"].key: "<<kvdram[(dramoffset + i)].data[k].key<<", kvdram["<<(dramoffset + i)<<"].data["<<k<<"].value: "<<kvdram[(dramoffset + i)].data[k].value<<endl; } // FIXME. REMOVEME
		// exit(EXIT_SUCCESS);
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_VU_ddr0_w0_sw6(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVSRCBUFFERSZ_APPLY_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_tracker_kvs.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read06:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY_KVS){ rwsize_kvs = KVSRCBUFFERSZ_APPLY_KVS; } 
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS06_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read06:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read06:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read06:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read06:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_APPLY_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		// for(unsigned int k=0; k<VECTOR_SIZE; k++){ cout<<"readAU_VU_ddr: kvdram["<<(dramoffset + i)<<"].data["<<k<<"].key: "<<kvdram[(dramoffset + i)].data[k].key<<", kvdram["<<(dramoffset + i)<<"].data["<<k<<"].value: "<<kvdram[(dramoffset + i)].data[k].value<<endl; } // FIXME. REMOVEME
		// exit(EXIT_SUCCESS);
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_VU_ddr0_w0_sw7(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVSRCBUFFERSZ_APPLY_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_tracker_kvs.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read07:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY_KVS){ rwsize_kvs = KVSRCBUFFERSZ_APPLY_KVS; } 
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS07_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read07:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read07:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read07:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read07:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_APPLY_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		// for(unsigned int k=0; k<VECTOR_SIZE; k++){ cout<<"readAU_VU_ddr: kvdram["<<(dramoffset + i)<<"].data["<<k<<"].key: "<<kvdram[(dramoffset + i)].data[k].key<<", kvdram["<<(dramoffset + i)<<"].data["<<k<<"].value: "<<kvdram[(dramoffset + i)].data[k].value<<endl; } // FIXME. REMOVEME
		// exit(EXIT_SUCCESS);
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}

void titan::readAU_V_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V00 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + vertexreadsz_kvs; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = vertexreadsz_kvs; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read00:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<(kvbuffer1_tracker_kvs.weof * VECTOR_SIZE)<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = vertexreadsz_kvs; }
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS00_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), (VERTICESDATASZ_FORDRAMX / VECTOR_SIZE), "readAU_V00:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);	
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V00:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V00:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V00:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V00:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_V_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V01 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + vertexreadsz_kvs; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = vertexreadsz_kvs; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read01:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<(kvbuffer1_tracker_kvs.weof * VECTOR_SIZE)<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = vertexreadsz_kvs; }
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS01_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), (VERTICESDATASZ_FORDRAMX / VECTOR_SIZE), "readAU_V01:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);	
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V01:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V01:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V01:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V01:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_V_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V02 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + vertexreadsz_kvs; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = vertexreadsz_kvs; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read02:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<(kvbuffer1_tracker_kvs.weof * VECTOR_SIZE)<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = vertexreadsz_kvs; }
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS02_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), (VERTICESDATASZ_FORDRAMX / VECTOR_SIZE), "readAU_V02:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);	
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V02:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V02:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V02:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V02:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_V_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V03 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + vertexreadsz_kvs; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = vertexreadsz_kvs; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read03:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<(kvbuffer1_tracker_kvs.weof * VECTOR_SIZE)<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = vertexreadsz_kvs; }
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS03_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), (VERTICESDATASZ_FORDRAMX / VECTOR_SIZE), "readAU_V03:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);	
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V03:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V03:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V03:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V03:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_V_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V04 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + vertexreadsz_kvs; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = vertexreadsz_kvs; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read04:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<(kvbuffer1_tracker_kvs.weof * VECTOR_SIZE)<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = vertexreadsz_kvs; }
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS04_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), (VERTICESDATASZ_FORDRAMX / VECTOR_SIZE), "readAU_V04:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);	
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V04:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V04:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V04:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V04:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_V_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V05 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + vertexreadsz_kvs; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = vertexreadsz_kvs; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read05:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<(kvbuffer1_tracker_kvs.weof * VECTOR_SIZE)<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = vertexreadsz_kvs; }
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS05_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), (VERTICESDATASZ_FORDRAMX / VECTOR_SIZE), "readAU_V05:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);	
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V05:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V05:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V05:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V05:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_V_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V06 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + vertexreadsz_kvs; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = vertexreadsz_kvs; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read06:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<(kvbuffer1_tracker_kvs.weof * VECTOR_SIZE)<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = vertexreadsz_kvs; }
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS06_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), (VERTICESDATASZ_FORDRAMX / VECTOR_SIZE), "readAU_V06:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);	
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V06:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V06:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V06:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V06:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}
void titan::readAU_V_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V07 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + vertexreadsz_kvs; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = vertexreadsz_kvs; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read07:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<(kvbuffer1_tracker_kvs.weof * VECTOR_SIZE)<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = vertexreadsz_kvs; }
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS07_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), (VERTICESDATASZ_FORDRAMX / VECTOR_SIZE), "readAU_V07:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);	
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V07:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V07:: out-of-bounds seen at read 6", (curroffset_kvs * 1), kvsize_kvs, kvbuffer1_tracker_kvs.weof);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "readAU_V07:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V07:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0_transfers += 1;
	#endif
	return;
}

// PARTITION
void titan::partition_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"titan:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	#ifdef _PARTITIONED_KVBUFFERSTATS
	smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=kvbufferstats_tmp_size complete
	#endif	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	READ_KVSTATSTMP00_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_tmp_size[p] = kvbufferstats_size[p]; }
	#endif 
	
	tracker_t kvbuffer1_kvs_tracker;
	kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS;

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS00_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}
	
	keyvalue_t kventry1;
	EXTRACTSTATS00_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry1.key = tmpV1.range(31, 0);
	kventry1.value = tmpV1.range(63, 32);
	#endif 
		EXTRACTSTATS00_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry1 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry1.key = tmpV1.range(31, 0);
			kventry1.value = tmpV1.range(63, 32);
			#endif 
			#endif 			
		}
	}
	// kvbufferstats_tmp_size[0] -=  // NB: this eliminates any edge conditions associated with partially alligned kvbuffer_src. NB: ensure extras are set to zero in host!
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS00_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = titan::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
	}
	#ifdef _DEBUGMODE_PRINTS
	printstats2_kernel(kvbufferstats_tmp_size, NUM_PARTITIONS, "", 0, 0, 0);
	#endif

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS00_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_tmp_size[p] = 0;
	}

	keyvalue_t kventry2;
	PARTITION00_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry2.key = tmpV1.range(31, 0);
	kventry2.value = tmpV1.range(63, 32);
	#endif 
		PARTITION00_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry2 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
titan::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION00_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION00_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry2.key = tmpV1.range(31, 0);
			kventry2.value = tmpV1.range(63, 32);
			#endif	
			#endif 
		}
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP00_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void titan::partition_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"titan:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	#ifdef _PARTITIONED_KVBUFFERSTATS
	smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=kvbufferstats_tmp_size complete
	#endif	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	READ_KVSTATSTMP01_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_tmp_size[p] = kvbufferstats_size[p]; }
	#endif 
	
	tracker_t kvbuffer1_kvs_tracker;
	kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS;

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS01_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}
	
	keyvalue_t kventry1;
	EXTRACTSTATS01_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry1.key = tmpV1.range(31, 0);
	kventry1.value = tmpV1.range(63, 32);
	#endif 
		EXTRACTSTATS01_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry1 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry1.key = tmpV1.range(31, 0);
			kventry1.value = tmpV1.range(63, 32);
			#endif 
			#endif 			
		}
	}
	// kvbufferstats_tmp_size[0] -=  // NB: this eliminates any edge conditions associated with partially alligned kvbuffer_src. NB: ensure extras are set to zero in host!
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS01_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = titan::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
	}
	#ifdef _DEBUGMODE_PRINTS
	printstats2_kernel(kvbufferstats_tmp_size, NUM_PARTITIONS, "", 0, 0, 0);
	#endif

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS01_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_tmp_size[p] = 0;
	}

	keyvalue_t kventry2;
	PARTITION01_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry2.key = tmpV1.range(31, 0);
	kventry2.value = tmpV1.range(63, 32);
	#endif 
		PARTITION01_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry2 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
titan::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION01_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION01_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry2.key = tmpV1.range(31, 0);
			kventry2.value = tmpV1.range(63, 32);
			#endif	
			#endif 
		}
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP01_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void titan::partition_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"titan:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	#ifdef _PARTITIONED_KVBUFFERSTATS
	smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=kvbufferstats_tmp_size complete
	#endif	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	READ_KVSTATSTMP02_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_tmp_size[p] = kvbufferstats_size[p]; }
	#endif 
	
	tracker_t kvbuffer1_kvs_tracker;
	kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS;

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS02_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}
	
	keyvalue_t kventry1;
	EXTRACTSTATS02_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry1.key = tmpV1.range(31, 0);
	kventry1.value = tmpV1.range(63, 32);
	#endif 
		EXTRACTSTATS02_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry1 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry1.key = tmpV1.range(31, 0);
			kventry1.value = tmpV1.range(63, 32);
			#endif 
			#endif 			
		}
	}
	// kvbufferstats_tmp_size[0] -=  // NB: this eliminates any edge conditions associated with partially alligned kvbuffer_src. NB: ensure extras are set to zero in host!
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS02_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = titan::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
	}
	#ifdef _DEBUGMODE_PRINTS
	printstats2_kernel(kvbufferstats_tmp_size, NUM_PARTITIONS, "", 0, 0, 0);
	#endif

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS02_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_tmp_size[p] = 0;
	}

	keyvalue_t kventry2;
	PARTITION02_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry2.key = tmpV1.range(31, 0);
	kventry2.value = tmpV1.range(63, 32);
	#endif 
		PARTITION02_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry2 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
titan::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION02_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION02_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry2.key = tmpV1.range(31, 0);
			kventry2.value = tmpV1.range(63, 32);
			#endif	
			#endif 
		}
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP02_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void titan::partition_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"titan:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	#ifdef _PARTITIONED_KVBUFFERSTATS
	smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=kvbufferstats_tmp_size complete
	#endif	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	READ_KVSTATSTMP03_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_tmp_size[p] = kvbufferstats_size[p]; }
	#endif 
	
	tracker_t kvbuffer1_kvs_tracker;
	kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS;

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS03_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}
	
	keyvalue_t kventry1;
	EXTRACTSTATS03_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry1.key = tmpV1.range(31, 0);
	kventry1.value = tmpV1.range(63, 32);
	#endif 
		EXTRACTSTATS03_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry1 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry1.key = tmpV1.range(31, 0);
			kventry1.value = tmpV1.range(63, 32);
			#endif 
			#endif 			
		}
	}
	// kvbufferstats_tmp_size[0] -=  // NB: this eliminates any edge conditions associated with partially alligned kvbuffer_src. NB: ensure extras are set to zero in host!
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS03_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = titan::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
	}
	#ifdef _DEBUGMODE_PRINTS
	printstats2_kernel(kvbufferstats_tmp_size, NUM_PARTITIONS, "", 0, 0, 0);
	#endif

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS03_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_tmp_size[p] = 0;
	}

	keyvalue_t kventry2;
	PARTITION03_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry2.key = tmpV1.range(31, 0);
	kventry2.value = tmpV1.range(63, 32);
	#endif 
		PARTITION03_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry2 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
titan::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION03_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION03_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry2.key = tmpV1.range(31, 0);
			kventry2.value = tmpV1.range(63, 32);
			#endif	
			#endif 
		}
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP03_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void titan::partition_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"titan:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	#ifdef _PARTITIONED_KVBUFFERSTATS
	smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=kvbufferstats_tmp_size complete
	#endif	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	READ_KVSTATSTMP04_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_tmp_size[p] = kvbufferstats_size[p]; }
	#endif 
	
	tracker_t kvbuffer1_kvs_tracker;
	kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS;

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS04_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}
	
	keyvalue_t kventry1;
	EXTRACTSTATS04_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry1.key = tmpV1.range(31, 0);
	kventry1.value = tmpV1.range(63, 32);
	#endif 
		EXTRACTSTATS04_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry1 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry1.key = tmpV1.range(31, 0);
			kventry1.value = tmpV1.range(63, 32);
			#endif 
			#endif 			
		}
	}
	// kvbufferstats_tmp_size[0] -=  // NB: this eliminates any edge conditions associated with partially alligned kvbuffer_src. NB: ensure extras are set to zero in host!
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS04_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = titan::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
	}
	#ifdef _DEBUGMODE_PRINTS
	printstats2_kernel(kvbufferstats_tmp_size, NUM_PARTITIONS, "", 0, 0, 0);
	#endif

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS04_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_tmp_size[p] = 0;
	}

	keyvalue_t kventry2;
	PARTITION04_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry2.key = tmpV1.range(31, 0);
	kventry2.value = tmpV1.range(63, 32);
	#endif 
		PARTITION04_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry2 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
titan::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION04_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION04_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry2.key = tmpV1.range(31, 0);
			kventry2.value = tmpV1.range(63, 32);
			#endif	
			#endif 
		}
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP04_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void titan::partition_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"titan:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	#ifdef _PARTITIONED_KVBUFFERSTATS
	smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=kvbufferstats_tmp_size complete
	#endif	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	READ_KVSTATSTMP05_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_tmp_size[p] = kvbufferstats_size[p]; }
	#endif 
	
	tracker_t kvbuffer1_kvs_tracker;
	kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS;

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS05_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}
	
	keyvalue_t kventry1;
	EXTRACTSTATS05_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry1.key = tmpV1.range(31, 0);
	kventry1.value = tmpV1.range(63, 32);
	#endif 
		EXTRACTSTATS05_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry1 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry1.key = tmpV1.range(31, 0);
			kventry1.value = tmpV1.range(63, 32);
			#endif 
			#endif 			
		}
	}
	// kvbufferstats_tmp_size[0] -=  // NB: this eliminates any edge conditions associated with partially alligned kvbuffer_src. NB: ensure extras are set to zero in host!
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS05_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = titan::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
	}
	#ifdef _DEBUGMODE_PRINTS
	printstats2_kernel(kvbufferstats_tmp_size, NUM_PARTITIONS, "", 0, 0, 0);
	#endif

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS05_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_tmp_size[p] = 0;
	}

	keyvalue_t kventry2;
	PARTITION05_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry2.key = tmpV1.range(31, 0);
	kventry2.value = tmpV1.range(63, 32);
	#endif 
		PARTITION05_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry2 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
titan::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION05_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION05_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry2.key = tmpV1.range(31, 0);
			kventry2.value = tmpV1.range(63, 32);
			#endif	
			#endif 
		}
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP05_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void titan::partition_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"titan:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	#ifdef _PARTITIONED_KVBUFFERSTATS
	smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=kvbufferstats_tmp_size complete
	#endif	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	READ_KVSTATSTMP06_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_tmp_size[p] = kvbufferstats_size[p]; }
	#endif 
	
	tracker_t kvbuffer1_kvs_tracker;
	kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS;

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS06_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}
	
	keyvalue_t kventry1;
	EXTRACTSTATS06_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry1.key = tmpV1.range(31, 0);
	kventry1.value = tmpV1.range(63, 32);
	#endif 
		EXTRACTSTATS06_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry1 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry1.key = tmpV1.range(31, 0);
			kventry1.value = tmpV1.range(63, 32);
			#endif 
			#endif 			
		}
	}
	// kvbufferstats_tmp_size[0] -=  // NB: this eliminates any edge conditions associated with partially alligned kvbuffer_src. NB: ensure extras are set to zero in host!
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS06_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = titan::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
	}
	#ifdef _DEBUGMODE_PRINTS
	printstats2_kernel(kvbufferstats_tmp_size, NUM_PARTITIONS, "", 0, 0, 0);
	#endif

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS06_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_tmp_size[p] = 0;
	}

	keyvalue_t kventry2;
	PARTITION06_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry2.key = tmpV1.range(31, 0);
	kventry2.value = tmpV1.range(63, 32);
	#endif 
		PARTITION06_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry2 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
titan::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION06_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION06_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry2.key = tmpV1.range(31, 0);
			kventry2.value = tmpV1.range(63, 32);
			#endif	
			#endif 
		}
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP06_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void titan::partition_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"titan:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	#ifdef _PARTITIONED_KVBUFFERSTATS
	smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=kvbufferstats_tmp_size complete
	#endif	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	READ_KVSTATSTMP07_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_tmp_size[p] = kvbufferstats_size[p]; }
	#endif 
	
	tracker_t kvbuffer1_kvs_tracker;
	kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS;

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS07_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}
	
	keyvalue_t kventry1;
	EXTRACTSTATS07_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry1.key = tmpV1.range(31, 0);
	kventry1.value = tmpV1.range(63, 32);
	#endif 
		EXTRACTSTATS07_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry1 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry1.key = tmpV1.range(31, 0);
			kventry1.value = tmpV1.range(63, 32);
			#endif 
			#endif 			
		}
	}
	// kvbufferstats_tmp_size[0] -=  // NB: this eliminates any edge conditions associated with partially alligned kvbuffer_src. NB: ensure extras are set to zero in host!
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS07_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = titan::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
	}
	#ifdef _DEBUGMODE_PRINTS
	printstats2_kernel(kvbufferstats_tmp_size, NUM_PARTITIONS, "", 0, 0, 0);
	#endif

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS07_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_tmp_size[p] = 0;
	}

	keyvalue_t kventry2;
	PARTITION07_MAIN_LOOP1: for (unsigned int j = 0; j<KVBUFFERSZ_KVS; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
	#ifdef OOO 
	kventry2.key = tmpV1.range(31, 0);
	kventry2.value = tmpV1.range(63, 32);
	#endif 
		PARTITION07_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD			
			#ifndef OOO
			// kventry2 = titan::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = titan::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
titan::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
			kvbufferstats_tmp_size[p] += 1;
			
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION07_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION07_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
			
			#ifdef _WIDEWORD
			#ifdef OOO
			tmpV1 = tmpV1 >> 64;
			kventry2.key = tmpV1.range(31, 0);
			kventry2.value = tmpV1.range(63, 32);
			#endif	
			#endif 
		}
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP07_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}

// APPLY
void titan::reduce_ddr0_w0_sw0(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: reduce. workerid: "<<workerid<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	#endif
	
	int kvbuffersz = KVSRCBUFFERSZ_APPLY; // NB: debugging purposes only
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only

	tracker_t kvbuffer1_tracker_kvs;
	unsigned int kvsize_kvs = (sourcestatbuffer.size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int kvsize_lower_kvs = sourcestatbuffer.size / VECTOR_SIZE;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (0 * KVSRCBUFFERSZ_APPLY_KVS); // subworkerid
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((0 + 1) * KVSRCBUFFERSZ_APPLY_KVS);
	if(curroffset_kvs >= kvsize_lower_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs < kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs >= kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_lower_kvs - curroffset_kvs); }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"reduce_ddr0_sw0:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_lower_kvs: "<<kvsize_lower_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((0 + 1) * KVSRCBUFFERSZ_APPLY_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"reduce_ddr0_sw0:: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	exit(EXIT_SUCCESS);
	#endif
	vertex_t voffset = verticesreadoffset_vs * VECTOR_SIZE;
	keyvalue_t kventry;
	keyvalue_t kv; 
	
	unsigned int rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY){ rwsize_kvs = 0; }
	REDUCE_ddr0_w0_sw0_LOOP1: for (unsigned int i = 0; i<rwsize_kvs; i++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer[i];
		REDUCE_ddr0_w0_sw0_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			#ifdef _WIDEWORD
			kventry = titan::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw0 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
			unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw0_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = titan::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw0 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
		unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void titan::reduce_ddr0_w0_sw1(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: reduce. workerid: "<<workerid<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	#endif
	
	int kvbuffersz = KVSRCBUFFERSZ_APPLY; // NB: debugging purposes only
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only

	tracker_t kvbuffer1_tracker_kvs;
	unsigned int kvsize_kvs = (sourcestatbuffer.size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int kvsize_lower_kvs = sourcestatbuffer.size / VECTOR_SIZE;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (0 * KVSRCBUFFERSZ_APPLY_KVS); // subworkerid
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((0 + 1) * KVSRCBUFFERSZ_APPLY_KVS);
	if(curroffset_kvs >= kvsize_lower_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs < kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs >= kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_lower_kvs - curroffset_kvs); }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"reduce_ddr0_sw1:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_lower_kvs: "<<kvsize_lower_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((1 + 1) * KVSRCBUFFERSZ_APPLY_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"reduce_ddr0_sw1:: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	exit(EXIT_SUCCESS);
	#endif
	vertex_t voffset = verticesreadoffset_vs * VECTOR_SIZE;
	keyvalue_t kventry;
	keyvalue_t kv; 
	
	unsigned int rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY){ rwsize_kvs = 0; }
	REDUCE_ddr0_w0_sw1_LOOP1: for (unsigned int i = 0; i<rwsize_kvs; i++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer[i];
		REDUCE_ddr0_w0_sw1_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			#ifdef _WIDEWORD
			kventry = titan::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw1 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
			unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw1_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = titan::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw1 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
		unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void titan::reduce_ddr0_w0_sw2(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: reduce. workerid: "<<workerid<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	#endif
	
	int kvbuffersz = KVSRCBUFFERSZ_APPLY; // NB: debugging purposes only
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only

	tracker_t kvbuffer1_tracker_kvs;
	unsigned int kvsize_kvs = (sourcestatbuffer.size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int kvsize_lower_kvs = sourcestatbuffer.size / VECTOR_SIZE;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (0 * KVSRCBUFFERSZ_APPLY_KVS); // subworkerid
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((0 + 1) * KVSRCBUFFERSZ_APPLY_KVS);
	if(curroffset_kvs >= kvsize_lower_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs < kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs >= kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_lower_kvs - curroffset_kvs); }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"reduce_ddr0_sw2:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_lower_kvs: "<<kvsize_lower_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((2 + 1) * KVSRCBUFFERSZ_APPLY_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"reduce_ddr0_sw2:: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	exit(EXIT_SUCCESS);
	#endif
	vertex_t voffset = verticesreadoffset_vs * VECTOR_SIZE;
	keyvalue_t kventry;
	keyvalue_t kv; 
	
	unsigned int rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY){ rwsize_kvs = 0; }
	REDUCE_ddr0_w0_sw2_LOOP1: for (unsigned int i = 0; i<rwsize_kvs; i++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer[i];
		REDUCE_ddr0_w0_sw2_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			#ifdef _WIDEWORD
			kventry = titan::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw2 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
			unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw2_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = titan::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw2 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
		unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void titan::reduce_ddr0_w0_sw3(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: reduce. workerid: "<<workerid<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	#endif
	
	int kvbuffersz = KVSRCBUFFERSZ_APPLY; // NB: debugging purposes only
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only

	tracker_t kvbuffer1_tracker_kvs;
	unsigned int kvsize_kvs = (sourcestatbuffer.size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int kvsize_lower_kvs = sourcestatbuffer.size / VECTOR_SIZE;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (0 * KVSRCBUFFERSZ_APPLY_KVS); // subworkerid
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((0 + 1) * KVSRCBUFFERSZ_APPLY_KVS);
	if(curroffset_kvs >= kvsize_lower_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs < kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs >= kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_lower_kvs - curroffset_kvs); }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"reduce_ddr0_sw3:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_lower_kvs: "<<kvsize_lower_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((3 + 1) * KVSRCBUFFERSZ_APPLY_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"reduce_ddr0_sw3:: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	exit(EXIT_SUCCESS);
	#endif
	vertex_t voffset = verticesreadoffset_vs * VECTOR_SIZE;
	keyvalue_t kventry;
	keyvalue_t kv; 
	
	unsigned int rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY){ rwsize_kvs = 0; }
	REDUCE_ddr0_w0_sw3_LOOP1: for (unsigned int i = 0; i<rwsize_kvs; i++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer[i];
		REDUCE_ddr0_w0_sw3_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			#ifdef _WIDEWORD
			kventry = titan::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw3 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
			unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw3_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = titan::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw3 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
		unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void titan::reduce_ddr0_w0_sw4(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: reduce. workerid: "<<workerid<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	#endif
	
	int kvbuffersz = KVSRCBUFFERSZ_APPLY; // NB: debugging purposes only
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only

	tracker_t kvbuffer1_tracker_kvs;
	unsigned int kvsize_kvs = (sourcestatbuffer.size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int kvsize_lower_kvs = sourcestatbuffer.size / VECTOR_SIZE;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (0 * KVSRCBUFFERSZ_APPLY_KVS); // subworkerid
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((0 + 1) * KVSRCBUFFERSZ_APPLY_KVS);
	if(curroffset_kvs >= kvsize_lower_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs < kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs >= kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_lower_kvs - curroffset_kvs); }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"reduce_ddr0_sw4:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_lower_kvs: "<<kvsize_lower_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((4 + 1) * KVSRCBUFFERSZ_APPLY_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"reduce_ddr0_sw4:: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	exit(EXIT_SUCCESS);
	#endif
	vertex_t voffset = verticesreadoffset_vs * VECTOR_SIZE;
	keyvalue_t kventry;
	keyvalue_t kv; 
	
	unsigned int rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY){ rwsize_kvs = 0; }
	REDUCE_ddr0_w0_sw4_LOOP1: for (unsigned int i = 0; i<rwsize_kvs; i++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer[i];
		REDUCE_ddr0_w0_sw4_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			#ifdef _WIDEWORD
			kventry = titan::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw4 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
			unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw4_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = titan::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw4 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
		unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void titan::reduce_ddr0_w0_sw5(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: reduce. workerid: "<<workerid<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	#endif
	
	int kvbuffersz = KVSRCBUFFERSZ_APPLY; // NB: debugging purposes only
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only

	tracker_t kvbuffer1_tracker_kvs;
	unsigned int kvsize_kvs = (sourcestatbuffer.size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int kvsize_lower_kvs = sourcestatbuffer.size / VECTOR_SIZE;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (0 * KVSRCBUFFERSZ_APPLY_KVS); // subworkerid
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((0 + 1) * KVSRCBUFFERSZ_APPLY_KVS);
	if(curroffset_kvs >= kvsize_lower_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs < kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs >= kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_lower_kvs - curroffset_kvs); }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"reduce_ddr0_sw5:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_lower_kvs: "<<kvsize_lower_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((5 + 1) * KVSRCBUFFERSZ_APPLY_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"reduce_ddr0_sw5:: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	exit(EXIT_SUCCESS);
	#endif
	vertex_t voffset = verticesreadoffset_vs * VECTOR_SIZE;
	keyvalue_t kventry;
	keyvalue_t kv; 
	
	unsigned int rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY){ rwsize_kvs = 0; }
	REDUCE_ddr0_w0_sw5_LOOP1: for (unsigned int i = 0; i<rwsize_kvs; i++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer[i];
		REDUCE_ddr0_w0_sw5_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			#ifdef _WIDEWORD
			kventry = titan::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw5 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
			unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw5_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = titan::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw5 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
		unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void titan::reduce_ddr0_w0_sw6(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: reduce. workerid: "<<workerid<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	#endif
	
	int kvbuffersz = KVSRCBUFFERSZ_APPLY; // NB: debugging purposes only
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only

	tracker_t kvbuffer1_tracker_kvs;
	unsigned int kvsize_kvs = (sourcestatbuffer.size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int kvsize_lower_kvs = sourcestatbuffer.size / VECTOR_SIZE;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (0 * KVSRCBUFFERSZ_APPLY_KVS); // subworkerid
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((0 + 1) * KVSRCBUFFERSZ_APPLY_KVS);
	if(curroffset_kvs >= kvsize_lower_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs < kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs >= kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_lower_kvs - curroffset_kvs); }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"reduce_ddr0_sw6:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_lower_kvs: "<<kvsize_lower_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((6 + 1) * KVSRCBUFFERSZ_APPLY_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"reduce_ddr0_sw6:: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	exit(EXIT_SUCCESS);
	#endif
	vertex_t voffset = verticesreadoffset_vs * VECTOR_SIZE;
	keyvalue_t kventry;
	keyvalue_t kv; 
	
	unsigned int rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY){ rwsize_kvs = 0; }
	REDUCE_ddr0_w0_sw6_LOOP1: for (unsigned int i = 0; i<rwsize_kvs; i++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer[i];
		REDUCE_ddr0_w0_sw6_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			#ifdef _WIDEWORD
			kventry = titan::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw6 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
			unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw6_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = titan::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw6 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
		unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void titan::reduce_ddr0_w0_sw7(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: reduce. workerid: "<<workerid<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	#endif
	
	int kvbuffersz = KVSRCBUFFERSZ_APPLY; // NB: debugging purposes only
	unsigned int kvbuffersz_kvs = KVSRCBUFFERSZ_APPLY_KVS;  // NB: debugging purposes only

	tracker_t kvbuffer1_tracker_kvs;
	unsigned int kvsize_kvs = (sourcestatbuffer.size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int kvsize_lower_kvs = sourcestatbuffer.size / VECTOR_SIZE;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (0 * KVSRCBUFFERSZ_APPLY_KVS); // subworkerid
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((0 + 1) * KVSRCBUFFERSZ_APPLY_KVS);
	if(curroffset_kvs >= kvsize_lower_kvs){ kvbuffer1_tracker_kvs.weof = 0; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs < kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = KVSRCBUFFERSZ_APPLY_KVS; }
	else if ((curroffset_kvs < kvsize_lower_kvs) && (nextoffset_kvs >= kvsize_lower_kvs)) { kvbuffer1_tracker_kvs.weof = (kvsize_lower_kvs - curroffset_kvs); }
	else {
		kvbuffer1_tracker_kvs.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"reduce_ddr0_sw7:: should NEVER get here: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", kvsize_lower_kvs: "<<kvsize_lower_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((7 + 1) * KVSRCBUFFERSZ_APPLY_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"reduce_ddr0_sw7:: kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", KVSRCBUFFERSZ_APPLY: "<<KVSRCBUFFERSZ_APPLY<<std::endl;
	exit(EXIT_SUCCESS);
	#endif
	vertex_t voffset = verticesreadoffset_vs * VECTOR_SIZE;
	keyvalue_t kventry;
	keyvalue_t kv; 
	
	unsigned int rwsize_kvs = kvbuffer1_tracker_kvs.weof;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_APPLY){ rwsize_kvs = 0; }
	REDUCE_ddr0_w0_sw7_LOOP1: for (unsigned int i = 0; i<rwsize_kvs; i++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer[i];
		REDUCE_ddr0_w0_sw7_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			#ifdef _WIDEWORD
			kventry = titan::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw7 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
			unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw7_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = titan::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw7 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = titan::getkventryIV(loc, verticesbuffer);
		unsigned int temp = titan::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
titan::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}

// WRITE
void titan::write_fake_ddr0_w0_sw0(uint512_dt * kvdram, uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], unsigned int dramoffset_kvs, unsigned int bramoffset_kvs, unsigned int size_kvs){
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
	FAKEWRITEBURSTKVS00_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		kvdram[((dramoffset_kvs + i) & 0xFFFFFFF)] = kvbuffer[(bramoffset_kvs + i)];
	}
	return;
}
void titan::write_fake_ddr0_w0_sw1(uint512_dt * kvdram, uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], unsigned int dramoffset_kvs, unsigned int bramoffset_kvs, unsigned int size_kvs){
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
	FAKEWRITEBURSTKVS01_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		kvdram[((dramoffset_kvs + i) & 0xFFFFFFF)] = kvbuffer[(bramoffset_kvs + i)];
	}
	return;
}
void titan::write_fake_ddr0_w0_sw2(uint512_dt * kvdram, uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], unsigned int dramoffset_kvs, unsigned int bramoffset_kvs, unsigned int size_kvs){
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
	FAKEWRITEBURSTKVS02_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		kvdram[((dramoffset_kvs + i) & 0xFFFFFFF)] = kvbuffer[(bramoffset_kvs + i)];
	}
	return;
}
void titan::write_fake_ddr0_w0_sw3(uint512_dt * kvdram, uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], unsigned int dramoffset_kvs, unsigned int bramoffset_kvs, unsigned int size_kvs){
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
	FAKEWRITEBURSTKVS03_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		kvdram[((dramoffset_kvs + i) & 0xFFFFFFF)] = kvbuffer[(bramoffset_kvs + i)];
	}
	return;
}
void titan::write_fake_ddr0_w0_sw4(uint512_dt * kvdram, uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], unsigned int dramoffset_kvs, unsigned int bramoffset_kvs, unsigned int size_kvs){
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
	FAKEWRITEBURSTKVS04_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		kvdram[((dramoffset_kvs + i) & 0xFFFFFFF)] = kvbuffer[(bramoffset_kvs + i)];
	}
	return;
}
void titan::write_fake_ddr0_w0_sw5(uint512_dt * kvdram, uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], unsigned int dramoffset_kvs, unsigned int bramoffset_kvs, unsigned int size_kvs){
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
	FAKEWRITEBURSTKVS05_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		kvdram[((dramoffset_kvs + i) & 0xFFFFFFF)] = kvbuffer[(bramoffset_kvs + i)];
	}
	return;
}
void titan::write_fake_ddr0_w0_sw6(uint512_dt * kvdram, uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], unsigned int dramoffset_kvs, unsigned int bramoffset_kvs, unsigned int size_kvs){
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
	FAKEWRITEBURSTKVS06_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		kvdram[((dramoffset_kvs + i) & 0xFFFFFFF)] = kvbuffer[(bramoffset_kvs + i)];
	}
	return;
}
void titan::write_fake_ddr0_w0_sw7(uint512_dt * kvdram, uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], unsigned int dramoffset_kvs, unsigned int bramoffset_kvs, unsigned int size_kvs){
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
	FAKEWRITEBURSTKVS07_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){ 
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		kvdram[((dramoffset_kvs + i) & 0xFFFFFFF)] = kvbuffer[(bramoffset_kvs + i)];
	}
	return;
}
void titan::writeburst_ddr0_w0_sw0(uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int dramoffset_kvs, unsigned int offset_kvs, unsigned int size_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#pragma HLS INLINE OFF
	unsigned int chunk_size = KVDESTBUFFERSZ_KVS;
	
	if (size_kvs < chunk_size){ chunk_size = size_kvs; }
	
	dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_kvs;
	MYWRITEBURSTKVS00_LOOP: for(kvbuffersz_dtype i=0; i<chunk_size; i++){ // size_kvs
	#pragma HLS PIPELINE II=1	
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset_kvs + i)];
		#else 
		kvdram[(dramoffset + i)] = kvbuffer[(offset_kvs + i)];
		#endif
	}
	return;
}
void titan::writeburst_ddr0_w0_sw1(uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int dramoffset_kvs, unsigned int offset_kvs, unsigned int size_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#pragma HLS INLINE OFF
	unsigned int chunk_size = KVDESTBUFFERSZ_KVS;
	
	if (size_kvs < chunk_size){ chunk_size = size_kvs; }
	
	dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_kvs;
	MYWRITEBURSTKVS01_LOOP: for(kvbuffersz_dtype i=0; i<chunk_size; i++){ // size_kvs
	#pragma HLS PIPELINE II=1	
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset_kvs + i)];
		#else 
		kvdram[(dramoffset + i)] = kvbuffer[(offset_kvs + i)];
		#endif
	}
	return;
}
void titan::writeburst_ddr0_w0_sw2(uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int dramoffset_kvs, unsigned int offset_kvs, unsigned int size_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#pragma HLS INLINE OFF
	unsigned int chunk_size = KVDESTBUFFERSZ_KVS;
	
	if (size_kvs < chunk_size){ chunk_size = size_kvs; }
	
	dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_kvs;
	MYWRITEBURSTKVS02_LOOP: for(kvbuffersz_dtype i=0; i<chunk_size; i++){ // size_kvs
	#pragma HLS PIPELINE II=1	
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset_kvs + i)];
		#else 
		kvdram[(dramoffset + i)] = kvbuffer[(offset_kvs + i)];
		#endif
	}
	return;
}
void titan::writeburst_ddr0_w0_sw3(uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int dramoffset_kvs, unsigned int offset_kvs, unsigned int size_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#pragma HLS INLINE OFF
	unsigned int chunk_size = KVDESTBUFFERSZ_KVS;
	
	if (size_kvs < chunk_size){ chunk_size = size_kvs; }
	
	dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_kvs;
	MYWRITEBURSTKVS03_LOOP: for(kvbuffersz_dtype i=0; i<chunk_size; i++){ // size_kvs
	#pragma HLS PIPELINE II=1	
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset_kvs + i)];
		#else 
		kvdram[(dramoffset + i)] = kvbuffer[(offset_kvs + i)];
		#endif
	}
	return;
}
void titan::writeburst_ddr0_w0_sw4(uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int dramoffset_kvs, unsigned int offset_kvs, unsigned int size_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#pragma HLS INLINE OFF
	unsigned int chunk_size = KVDESTBUFFERSZ_KVS;
	
	if (size_kvs < chunk_size){ chunk_size = size_kvs; }
	
	dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_kvs;
	MYWRITEBURSTKVS04_LOOP: for(kvbuffersz_dtype i=0; i<chunk_size; i++){ // size_kvs
	#pragma HLS PIPELINE II=1	
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset_kvs + i)];
		#else 
		kvdram[(dramoffset + i)] = kvbuffer[(offset_kvs + i)];
		#endif
	}
	return;
}
void titan::writeburst_ddr0_w0_sw5(uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int dramoffset_kvs, unsigned int offset_kvs, unsigned int size_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#pragma HLS INLINE OFF
	unsigned int chunk_size = KVDESTBUFFERSZ_KVS;
	
	if (size_kvs < chunk_size){ chunk_size = size_kvs; }
	
	dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_kvs;
	MYWRITEBURSTKVS05_LOOP: for(kvbuffersz_dtype i=0; i<chunk_size; i++){ // size_kvs
	#pragma HLS PIPELINE II=1	
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset_kvs + i)];
		#else 
		kvdram[(dramoffset + i)] = kvbuffer[(offset_kvs + i)];
		#endif
	}
	return;
}
void titan::writeburst_ddr0_w0_sw6(uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int dramoffset_kvs, unsigned int offset_kvs, unsigned int size_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#pragma HLS INLINE OFF
	unsigned int chunk_size = KVDESTBUFFERSZ_KVS;
	
	if (size_kvs < chunk_size){ chunk_size = size_kvs; }
	
	dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_kvs;
	MYWRITEBURSTKVS06_LOOP: for(kvbuffersz_dtype i=0; i<chunk_size; i++){ // size_kvs
	#pragma HLS PIPELINE II=1	
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset_kvs + i)];
		#else 
		kvdram[(dramoffset + i)] = kvbuffer[(offset_kvs + i)];
		#endif
	}
	return;
}
void titan::writeburst_ddr0_w0_sw7(uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int dramoffset_kvs, unsigned int offset_kvs, unsigned int size_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#pragma HLS INLINE OFF
	unsigned int chunk_size = KVDESTBUFFERSZ_KVS;
	
	if (size_kvs < chunk_size){ chunk_size = size_kvs; }
	
	dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_kvs;
	MYWRITEBURSTKVS07_LOOP: for(kvbuffersz_dtype i=0; i<chunk_size; i++){ // size_kvs
	#pragma HLS PIPELINE II=1	
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset_kvs + i)];
		#else 
		kvdram[(dramoffset + i)] = kvbuffer[(offset_kvs + i)];
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt * kvdram, unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
		
		#ifdef _WIDEWORD
		unsigned int offset0 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#else 
		unsigned int offset0 = kvbuffer[0].data[0].value / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].data[0].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].data[0].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset0_g + dramoffset0_l), offset0, size0_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset1 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#else 
		unsigned int offset1 = kvbuffer[0].data[1].value / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].data[1].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].data[1].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset1_g + dramoffset1_l), offset1, size1_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset2 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#else 
		unsigned int offset2 = kvbuffer[0].data[2].value / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].data[2].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].data[2].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset2_g + dramoffset2_l), offset2, size2_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset3 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#else 
		unsigned int offset3 = kvbuffer[0].data[3].value / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].data[3].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].data[3].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset3_g + dramoffset3_l), offset3, size3_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset4 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#else 
		unsigned int offset4 = kvbuffer[0].data[4].value / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].data[4].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].data[4].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset4_g + dramoffset4_l), offset4, size4_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset5 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#else 
		unsigned int offset5 = kvbuffer[0].data[5].value / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].data[5].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].data[5].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset5_g + dramoffset5_l), offset5, size5_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset6 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#else 
		unsigned int offset6 = kvbuffer[0].data[6].value / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].data[6].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].data[6].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset6_g + dramoffset6_l), offset6, size6_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset7 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#else 
		unsigned int offset7 = kvbuffer[0].data[7].value / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].data[7].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].data[7].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset7_g + dramoffset7_l), offset7, size7_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset8 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#else 
		unsigned int offset8 = kvbuffer[0].data[8].value / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].data[8].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].data[8].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset8_g + dramoffset8_l), offset8, size8_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset9 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#else 
		unsigned int offset9 = kvbuffer[0].data[9].value / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].data[9].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].data[9].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset9_g + dramoffset9_l), offset9, size9_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset10 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#else 
		unsigned int offset10 = kvbuffer[0].data[10].value / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].data[10].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].data[10].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset10_g + dramoffset10_l), offset10, size10_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset11 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#else 
		unsigned int offset11 = kvbuffer[0].data[11].value / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].data[11].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].data[11].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset11_g + dramoffset11_l), offset11, size11_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset12 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#else 
		unsigned int offset12 = kvbuffer[0].data[12].value / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].data[12].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].data[12].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset12_g + dramoffset12_l), offset12, size12_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset13 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#else 
		unsigned int offset13 = kvbuffer[0].data[13].value / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].data[13].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].data[13].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset13_g + dramoffset13_l), offset13, size13_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset14 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#else 
		unsigned int offset14 = kvbuffer[0].data[14].value / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].data[14].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].data[14].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset14_g + dramoffset14_l), offset14, size14_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset15 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#else 
		unsigned int offset15 = kvbuffer[0].data[15].value / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].data[15].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].data[15].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset15_g + dramoffset15_l), offset15, size15_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset16 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#else 
		unsigned int offset16 = kvbuffer[0].data[16].value / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].data[16].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].data[16].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset16_g + dramoffset16_l), offset16, size16_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset17 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#else 
		unsigned int offset17 = kvbuffer[0].data[17].value / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].data[17].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].data[17].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset17_g + dramoffset17_l), offset17, size17_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset18 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#else 
		unsigned int offset18 = kvbuffer[0].data[18].value / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].data[18].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].data[18].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset18_g + dramoffset18_l), offset18, size18_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset19 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#else 
		unsigned int offset19 = kvbuffer[0].data[19].value / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].data[19].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].data[19].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset19_g + dramoffset19_l), offset19, size19_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset20 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#else 
		unsigned int offset20 = kvbuffer[0].data[20].value / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].data[20].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].data[20].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset20_g + dramoffset20_l), offset20, size20_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset21 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#else 
		unsigned int offset21 = kvbuffer[0].data[21].value / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].data[21].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].data[21].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset21_g + dramoffset21_l), offset21, size21_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset22 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#else 
		unsigned int offset22 = kvbuffer[0].data[22].value / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].data[22].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].data[22].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset22_g + dramoffset22_l), offset22, size22_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset23 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#else 
		unsigned int offset23 = kvbuffer[0].data[23].value / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].data[23].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].data[23].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset23_g + dramoffset23_l), offset23, size23_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset24 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#else 
		unsigned int offset24 = kvbuffer[0].data[24].value / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].data[24].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].data[24].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset24_g + dramoffset24_l), offset24, size24_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset25 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#else 
		unsigned int offset25 = kvbuffer[0].data[25].value / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].data[25].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].data[25].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset25_g + dramoffset25_l), offset25, size25_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset26 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#else 
		unsigned int offset26 = kvbuffer[0].data[26].value / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].data[26].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].data[26].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset26_g + dramoffset26_l), offset26, size26_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset27 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#else 
		unsigned int offset27 = kvbuffer[0].data[27].value / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].data[27].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].data[27].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset27_g + dramoffset27_l), offset27, size27_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset28 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#else 
		unsigned int offset28 = kvbuffer[0].data[28].value / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].data[28].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].data[28].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset28_g + dramoffset28_l), offset28, size28_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset29 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#else 
		unsigned int offset29 = kvbuffer[0].data[29].value / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].data[29].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].data[29].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset29_g + dramoffset29_l), offset29, size29_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset30 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#else 
		unsigned int offset30 = kvbuffer[0].data[30].value / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].data[30].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].data[30].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset30_g + dramoffset30_l), offset30, size30_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset31 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#else 
		unsigned int offset31 = kvbuffer[0].data[31].value / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].data[31].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].data[31].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#endif 
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset31_g + dramoffset31_l), offset31, size31_kvs, kvbuffer);
		
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	return;
}
void titan::write_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt * kvdram, unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
		
		#ifdef _WIDEWORD
		unsigned int offset0 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#else 
		unsigned int offset0 = kvbuffer[0].data[0].value / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].data[0].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].data[0].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset0_g + dramoffset0_l), offset0, size0_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset1 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#else 
		unsigned int offset1 = kvbuffer[0].data[1].value / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].data[1].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].data[1].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset1_g + dramoffset1_l), offset1, size1_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset2 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#else 
		unsigned int offset2 = kvbuffer[0].data[2].value / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].data[2].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].data[2].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset2_g + dramoffset2_l), offset2, size2_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset3 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#else 
		unsigned int offset3 = kvbuffer[0].data[3].value / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].data[3].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].data[3].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset3_g + dramoffset3_l), offset3, size3_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset4 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#else 
		unsigned int offset4 = kvbuffer[0].data[4].value / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].data[4].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].data[4].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset4_g + dramoffset4_l), offset4, size4_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset5 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#else 
		unsigned int offset5 = kvbuffer[0].data[5].value / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].data[5].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].data[5].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset5_g + dramoffset5_l), offset5, size5_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset6 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#else 
		unsigned int offset6 = kvbuffer[0].data[6].value / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].data[6].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].data[6].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset6_g + dramoffset6_l), offset6, size6_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset7 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#else 
		unsigned int offset7 = kvbuffer[0].data[7].value / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].data[7].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].data[7].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset7_g + dramoffset7_l), offset7, size7_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset8 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#else 
		unsigned int offset8 = kvbuffer[0].data[8].value / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].data[8].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].data[8].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset8_g + dramoffset8_l), offset8, size8_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset9 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#else 
		unsigned int offset9 = kvbuffer[0].data[9].value / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].data[9].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].data[9].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset9_g + dramoffset9_l), offset9, size9_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset10 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#else 
		unsigned int offset10 = kvbuffer[0].data[10].value / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].data[10].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].data[10].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset10_g + dramoffset10_l), offset10, size10_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset11 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#else 
		unsigned int offset11 = kvbuffer[0].data[11].value / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].data[11].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].data[11].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset11_g + dramoffset11_l), offset11, size11_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset12 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#else 
		unsigned int offset12 = kvbuffer[0].data[12].value / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].data[12].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].data[12].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset12_g + dramoffset12_l), offset12, size12_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset13 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#else 
		unsigned int offset13 = kvbuffer[0].data[13].value / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].data[13].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].data[13].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset13_g + dramoffset13_l), offset13, size13_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset14 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#else 
		unsigned int offset14 = kvbuffer[0].data[14].value / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].data[14].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].data[14].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset14_g + dramoffset14_l), offset14, size14_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset15 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#else 
		unsigned int offset15 = kvbuffer[0].data[15].value / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].data[15].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].data[15].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset15_g + dramoffset15_l), offset15, size15_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset16 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#else 
		unsigned int offset16 = kvbuffer[0].data[16].value / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].data[16].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].data[16].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset16_g + dramoffset16_l), offset16, size16_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset17 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#else 
		unsigned int offset17 = kvbuffer[0].data[17].value / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].data[17].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].data[17].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset17_g + dramoffset17_l), offset17, size17_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset18 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#else 
		unsigned int offset18 = kvbuffer[0].data[18].value / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].data[18].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].data[18].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset18_g + dramoffset18_l), offset18, size18_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset19 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#else 
		unsigned int offset19 = kvbuffer[0].data[19].value / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].data[19].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].data[19].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset19_g + dramoffset19_l), offset19, size19_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset20 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#else 
		unsigned int offset20 = kvbuffer[0].data[20].value / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].data[20].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].data[20].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset20_g + dramoffset20_l), offset20, size20_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset21 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#else 
		unsigned int offset21 = kvbuffer[0].data[21].value / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].data[21].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].data[21].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset21_g + dramoffset21_l), offset21, size21_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset22 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#else 
		unsigned int offset22 = kvbuffer[0].data[22].value / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].data[22].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].data[22].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset22_g + dramoffset22_l), offset22, size22_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset23 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#else 
		unsigned int offset23 = kvbuffer[0].data[23].value / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].data[23].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].data[23].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset23_g + dramoffset23_l), offset23, size23_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset24 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#else 
		unsigned int offset24 = kvbuffer[0].data[24].value / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].data[24].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].data[24].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset24_g + dramoffset24_l), offset24, size24_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset25 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#else 
		unsigned int offset25 = kvbuffer[0].data[25].value / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].data[25].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].data[25].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset25_g + dramoffset25_l), offset25, size25_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset26 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#else 
		unsigned int offset26 = kvbuffer[0].data[26].value / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].data[26].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].data[26].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset26_g + dramoffset26_l), offset26, size26_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset27 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#else 
		unsigned int offset27 = kvbuffer[0].data[27].value / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].data[27].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].data[27].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset27_g + dramoffset27_l), offset27, size27_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset28 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#else 
		unsigned int offset28 = kvbuffer[0].data[28].value / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].data[28].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].data[28].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset28_g + dramoffset28_l), offset28, size28_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset29 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#else 
		unsigned int offset29 = kvbuffer[0].data[29].value / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].data[29].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].data[29].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset29_g + dramoffset29_l), offset29, size29_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset30 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#else 
		unsigned int offset30 = kvbuffer[0].data[30].value / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].data[30].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].data[30].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset30_g + dramoffset30_l), offset30, size30_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset31 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#else 
		unsigned int offset31 = kvbuffer[0].data[31].value / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].data[31].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].data[31].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#endif 
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset31_g + dramoffset31_l), offset31, size31_kvs, kvbuffer);
		
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	return;
}
void titan::write_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt * kvdram, unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
		
		#ifdef _WIDEWORD
		unsigned int offset0 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#else 
		unsigned int offset0 = kvbuffer[0].data[0].value / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].data[0].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].data[0].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset0_g + dramoffset0_l), offset0, size0_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset1 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#else 
		unsigned int offset1 = kvbuffer[0].data[1].value / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].data[1].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].data[1].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset1_g + dramoffset1_l), offset1, size1_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset2 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#else 
		unsigned int offset2 = kvbuffer[0].data[2].value / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].data[2].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].data[2].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset2_g + dramoffset2_l), offset2, size2_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset3 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#else 
		unsigned int offset3 = kvbuffer[0].data[3].value / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].data[3].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].data[3].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset3_g + dramoffset3_l), offset3, size3_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset4 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#else 
		unsigned int offset4 = kvbuffer[0].data[4].value / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].data[4].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].data[4].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset4_g + dramoffset4_l), offset4, size4_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset5 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#else 
		unsigned int offset5 = kvbuffer[0].data[5].value / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].data[5].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].data[5].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset5_g + dramoffset5_l), offset5, size5_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset6 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#else 
		unsigned int offset6 = kvbuffer[0].data[6].value / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].data[6].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].data[6].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset6_g + dramoffset6_l), offset6, size6_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset7 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#else 
		unsigned int offset7 = kvbuffer[0].data[7].value / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].data[7].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].data[7].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset7_g + dramoffset7_l), offset7, size7_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset8 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#else 
		unsigned int offset8 = kvbuffer[0].data[8].value / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].data[8].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].data[8].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset8_g + dramoffset8_l), offset8, size8_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset9 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#else 
		unsigned int offset9 = kvbuffer[0].data[9].value / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].data[9].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].data[9].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset9_g + dramoffset9_l), offset9, size9_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset10 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#else 
		unsigned int offset10 = kvbuffer[0].data[10].value / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].data[10].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].data[10].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset10_g + dramoffset10_l), offset10, size10_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset11 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#else 
		unsigned int offset11 = kvbuffer[0].data[11].value / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].data[11].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].data[11].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset11_g + dramoffset11_l), offset11, size11_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset12 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#else 
		unsigned int offset12 = kvbuffer[0].data[12].value / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].data[12].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].data[12].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset12_g + dramoffset12_l), offset12, size12_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset13 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#else 
		unsigned int offset13 = kvbuffer[0].data[13].value / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].data[13].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].data[13].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset13_g + dramoffset13_l), offset13, size13_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset14 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#else 
		unsigned int offset14 = kvbuffer[0].data[14].value / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].data[14].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].data[14].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset14_g + dramoffset14_l), offset14, size14_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset15 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#else 
		unsigned int offset15 = kvbuffer[0].data[15].value / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].data[15].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].data[15].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset15_g + dramoffset15_l), offset15, size15_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset16 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#else 
		unsigned int offset16 = kvbuffer[0].data[16].value / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].data[16].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].data[16].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset16_g + dramoffset16_l), offset16, size16_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset17 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#else 
		unsigned int offset17 = kvbuffer[0].data[17].value / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].data[17].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].data[17].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset17_g + dramoffset17_l), offset17, size17_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset18 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#else 
		unsigned int offset18 = kvbuffer[0].data[18].value / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].data[18].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].data[18].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset18_g + dramoffset18_l), offset18, size18_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset19 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#else 
		unsigned int offset19 = kvbuffer[0].data[19].value / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].data[19].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].data[19].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset19_g + dramoffset19_l), offset19, size19_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset20 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#else 
		unsigned int offset20 = kvbuffer[0].data[20].value / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].data[20].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].data[20].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset20_g + dramoffset20_l), offset20, size20_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset21 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#else 
		unsigned int offset21 = kvbuffer[0].data[21].value / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].data[21].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].data[21].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset21_g + dramoffset21_l), offset21, size21_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset22 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#else 
		unsigned int offset22 = kvbuffer[0].data[22].value / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].data[22].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].data[22].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset22_g + dramoffset22_l), offset22, size22_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset23 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#else 
		unsigned int offset23 = kvbuffer[0].data[23].value / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].data[23].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].data[23].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset23_g + dramoffset23_l), offset23, size23_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset24 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#else 
		unsigned int offset24 = kvbuffer[0].data[24].value / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].data[24].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].data[24].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset24_g + dramoffset24_l), offset24, size24_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset25 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#else 
		unsigned int offset25 = kvbuffer[0].data[25].value / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].data[25].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].data[25].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset25_g + dramoffset25_l), offset25, size25_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset26 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#else 
		unsigned int offset26 = kvbuffer[0].data[26].value / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].data[26].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].data[26].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset26_g + dramoffset26_l), offset26, size26_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset27 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#else 
		unsigned int offset27 = kvbuffer[0].data[27].value / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].data[27].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].data[27].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset27_g + dramoffset27_l), offset27, size27_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset28 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#else 
		unsigned int offset28 = kvbuffer[0].data[28].value / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].data[28].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].data[28].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset28_g + dramoffset28_l), offset28, size28_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset29 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#else 
		unsigned int offset29 = kvbuffer[0].data[29].value / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].data[29].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].data[29].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset29_g + dramoffset29_l), offset29, size29_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset30 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#else 
		unsigned int offset30 = kvbuffer[0].data[30].value / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].data[30].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].data[30].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset30_g + dramoffset30_l), offset30, size30_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset31 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#else 
		unsigned int offset31 = kvbuffer[0].data[31].value / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].data[31].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].data[31].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#endif 
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset31_g + dramoffset31_l), offset31, size31_kvs, kvbuffer);
		
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	return;
}
void titan::write_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt * kvdram, unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
		
		#ifdef _WIDEWORD
		unsigned int offset0 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#else 
		unsigned int offset0 = kvbuffer[0].data[0].value / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].data[0].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].data[0].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset0_g + dramoffset0_l), offset0, size0_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset1 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#else 
		unsigned int offset1 = kvbuffer[0].data[1].value / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].data[1].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].data[1].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset1_g + dramoffset1_l), offset1, size1_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset2 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#else 
		unsigned int offset2 = kvbuffer[0].data[2].value / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].data[2].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].data[2].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset2_g + dramoffset2_l), offset2, size2_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset3 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#else 
		unsigned int offset3 = kvbuffer[0].data[3].value / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].data[3].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].data[3].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset3_g + dramoffset3_l), offset3, size3_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset4 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#else 
		unsigned int offset4 = kvbuffer[0].data[4].value / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].data[4].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].data[4].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset4_g + dramoffset4_l), offset4, size4_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset5 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#else 
		unsigned int offset5 = kvbuffer[0].data[5].value / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].data[5].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].data[5].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset5_g + dramoffset5_l), offset5, size5_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset6 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#else 
		unsigned int offset6 = kvbuffer[0].data[6].value / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].data[6].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].data[6].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset6_g + dramoffset6_l), offset6, size6_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset7 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#else 
		unsigned int offset7 = kvbuffer[0].data[7].value / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].data[7].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].data[7].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset7_g + dramoffset7_l), offset7, size7_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset8 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#else 
		unsigned int offset8 = kvbuffer[0].data[8].value / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].data[8].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].data[8].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset8_g + dramoffset8_l), offset8, size8_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset9 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#else 
		unsigned int offset9 = kvbuffer[0].data[9].value / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].data[9].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].data[9].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset9_g + dramoffset9_l), offset9, size9_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset10 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#else 
		unsigned int offset10 = kvbuffer[0].data[10].value / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].data[10].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].data[10].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset10_g + dramoffset10_l), offset10, size10_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset11 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#else 
		unsigned int offset11 = kvbuffer[0].data[11].value / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].data[11].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].data[11].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset11_g + dramoffset11_l), offset11, size11_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset12 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#else 
		unsigned int offset12 = kvbuffer[0].data[12].value / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].data[12].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].data[12].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset12_g + dramoffset12_l), offset12, size12_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset13 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#else 
		unsigned int offset13 = kvbuffer[0].data[13].value / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].data[13].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].data[13].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset13_g + dramoffset13_l), offset13, size13_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset14 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#else 
		unsigned int offset14 = kvbuffer[0].data[14].value / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].data[14].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].data[14].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset14_g + dramoffset14_l), offset14, size14_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset15 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#else 
		unsigned int offset15 = kvbuffer[0].data[15].value / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].data[15].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].data[15].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset15_g + dramoffset15_l), offset15, size15_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset16 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#else 
		unsigned int offset16 = kvbuffer[0].data[16].value / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].data[16].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].data[16].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset16_g + dramoffset16_l), offset16, size16_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset17 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#else 
		unsigned int offset17 = kvbuffer[0].data[17].value / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].data[17].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].data[17].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset17_g + dramoffset17_l), offset17, size17_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset18 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#else 
		unsigned int offset18 = kvbuffer[0].data[18].value / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].data[18].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].data[18].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset18_g + dramoffset18_l), offset18, size18_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset19 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#else 
		unsigned int offset19 = kvbuffer[0].data[19].value / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].data[19].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].data[19].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset19_g + dramoffset19_l), offset19, size19_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset20 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#else 
		unsigned int offset20 = kvbuffer[0].data[20].value / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].data[20].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].data[20].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset20_g + dramoffset20_l), offset20, size20_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset21 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#else 
		unsigned int offset21 = kvbuffer[0].data[21].value / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].data[21].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].data[21].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset21_g + dramoffset21_l), offset21, size21_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset22 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#else 
		unsigned int offset22 = kvbuffer[0].data[22].value / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].data[22].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].data[22].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset22_g + dramoffset22_l), offset22, size22_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset23 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#else 
		unsigned int offset23 = kvbuffer[0].data[23].value / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].data[23].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].data[23].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset23_g + dramoffset23_l), offset23, size23_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset24 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#else 
		unsigned int offset24 = kvbuffer[0].data[24].value / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].data[24].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].data[24].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset24_g + dramoffset24_l), offset24, size24_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset25 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#else 
		unsigned int offset25 = kvbuffer[0].data[25].value / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].data[25].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].data[25].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset25_g + dramoffset25_l), offset25, size25_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset26 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#else 
		unsigned int offset26 = kvbuffer[0].data[26].value / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].data[26].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].data[26].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset26_g + dramoffset26_l), offset26, size26_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset27 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#else 
		unsigned int offset27 = kvbuffer[0].data[27].value / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].data[27].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].data[27].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset27_g + dramoffset27_l), offset27, size27_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset28 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#else 
		unsigned int offset28 = kvbuffer[0].data[28].value / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].data[28].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].data[28].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset28_g + dramoffset28_l), offset28, size28_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset29 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#else 
		unsigned int offset29 = kvbuffer[0].data[29].value / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].data[29].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].data[29].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset29_g + dramoffset29_l), offset29, size29_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset30 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#else 
		unsigned int offset30 = kvbuffer[0].data[30].value / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].data[30].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].data[30].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset30_g + dramoffset30_l), offset30, size30_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset31 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#else 
		unsigned int offset31 = kvbuffer[0].data[31].value / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].data[31].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].data[31].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#endif 
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset31_g + dramoffset31_l), offset31, size31_kvs, kvbuffer);
		
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	return;
}
void titan::write_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt * kvdram, unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
		
		#ifdef _WIDEWORD
		unsigned int offset0 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#else 
		unsigned int offset0 = kvbuffer[0].data[0].value / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].data[0].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].data[0].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset0_g + dramoffset0_l), offset0, size0_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset1 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#else 
		unsigned int offset1 = kvbuffer[0].data[1].value / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].data[1].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].data[1].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset1_g + dramoffset1_l), offset1, size1_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset2 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#else 
		unsigned int offset2 = kvbuffer[0].data[2].value / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].data[2].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].data[2].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset2_g + dramoffset2_l), offset2, size2_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset3 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#else 
		unsigned int offset3 = kvbuffer[0].data[3].value / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].data[3].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].data[3].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset3_g + dramoffset3_l), offset3, size3_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset4 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#else 
		unsigned int offset4 = kvbuffer[0].data[4].value / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].data[4].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].data[4].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset4_g + dramoffset4_l), offset4, size4_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset5 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#else 
		unsigned int offset5 = kvbuffer[0].data[5].value / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].data[5].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].data[5].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset5_g + dramoffset5_l), offset5, size5_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset6 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#else 
		unsigned int offset6 = kvbuffer[0].data[6].value / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].data[6].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].data[6].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset6_g + dramoffset6_l), offset6, size6_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset7 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#else 
		unsigned int offset7 = kvbuffer[0].data[7].value / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].data[7].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].data[7].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset7_g + dramoffset7_l), offset7, size7_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset8 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#else 
		unsigned int offset8 = kvbuffer[0].data[8].value / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].data[8].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].data[8].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset8_g + dramoffset8_l), offset8, size8_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset9 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#else 
		unsigned int offset9 = kvbuffer[0].data[9].value / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].data[9].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].data[9].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset9_g + dramoffset9_l), offset9, size9_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset10 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#else 
		unsigned int offset10 = kvbuffer[0].data[10].value / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].data[10].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].data[10].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset10_g + dramoffset10_l), offset10, size10_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset11 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#else 
		unsigned int offset11 = kvbuffer[0].data[11].value / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].data[11].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].data[11].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset11_g + dramoffset11_l), offset11, size11_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset12 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#else 
		unsigned int offset12 = kvbuffer[0].data[12].value / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].data[12].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].data[12].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset12_g + dramoffset12_l), offset12, size12_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset13 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#else 
		unsigned int offset13 = kvbuffer[0].data[13].value / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].data[13].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].data[13].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset13_g + dramoffset13_l), offset13, size13_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset14 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#else 
		unsigned int offset14 = kvbuffer[0].data[14].value / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].data[14].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].data[14].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset14_g + dramoffset14_l), offset14, size14_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset15 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#else 
		unsigned int offset15 = kvbuffer[0].data[15].value / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].data[15].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].data[15].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset15_g + dramoffset15_l), offset15, size15_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset16 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#else 
		unsigned int offset16 = kvbuffer[0].data[16].value / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].data[16].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].data[16].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset16_g + dramoffset16_l), offset16, size16_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset17 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#else 
		unsigned int offset17 = kvbuffer[0].data[17].value / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].data[17].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].data[17].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset17_g + dramoffset17_l), offset17, size17_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset18 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#else 
		unsigned int offset18 = kvbuffer[0].data[18].value / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].data[18].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].data[18].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset18_g + dramoffset18_l), offset18, size18_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset19 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#else 
		unsigned int offset19 = kvbuffer[0].data[19].value / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].data[19].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].data[19].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset19_g + dramoffset19_l), offset19, size19_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset20 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#else 
		unsigned int offset20 = kvbuffer[0].data[20].value / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].data[20].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].data[20].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset20_g + dramoffset20_l), offset20, size20_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset21 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#else 
		unsigned int offset21 = kvbuffer[0].data[21].value / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].data[21].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].data[21].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset21_g + dramoffset21_l), offset21, size21_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset22 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#else 
		unsigned int offset22 = kvbuffer[0].data[22].value / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].data[22].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].data[22].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset22_g + dramoffset22_l), offset22, size22_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset23 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#else 
		unsigned int offset23 = kvbuffer[0].data[23].value / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].data[23].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].data[23].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset23_g + dramoffset23_l), offset23, size23_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset24 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#else 
		unsigned int offset24 = kvbuffer[0].data[24].value / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].data[24].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].data[24].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset24_g + dramoffset24_l), offset24, size24_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset25 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#else 
		unsigned int offset25 = kvbuffer[0].data[25].value / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].data[25].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].data[25].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset25_g + dramoffset25_l), offset25, size25_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset26 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#else 
		unsigned int offset26 = kvbuffer[0].data[26].value / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].data[26].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].data[26].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset26_g + dramoffset26_l), offset26, size26_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset27 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#else 
		unsigned int offset27 = kvbuffer[0].data[27].value / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].data[27].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].data[27].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset27_g + dramoffset27_l), offset27, size27_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset28 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#else 
		unsigned int offset28 = kvbuffer[0].data[28].value / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].data[28].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].data[28].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset28_g + dramoffset28_l), offset28, size28_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset29 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#else 
		unsigned int offset29 = kvbuffer[0].data[29].value / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].data[29].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].data[29].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset29_g + dramoffset29_l), offset29, size29_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset30 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#else 
		unsigned int offset30 = kvbuffer[0].data[30].value / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].data[30].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].data[30].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset30_g + dramoffset30_l), offset30, size30_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset31 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#else 
		unsigned int offset31 = kvbuffer[0].data[31].value / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].data[31].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].data[31].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#endif 
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset31_g + dramoffset31_l), offset31, size31_kvs, kvbuffer);
		
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	return;
}
void titan::write_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt * kvdram, unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
		
		#ifdef _WIDEWORD
		unsigned int offset0 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#else 
		unsigned int offset0 = kvbuffer[0].data[0].value / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].data[0].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].data[0].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset0_g + dramoffset0_l), offset0, size0_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset1 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#else 
		unsigned int offset1 = kvbuffer[0].data[1].value / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].data[1].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].data[1].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset1_g + dramoffset1_l), offset1, size1_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset2 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#else 
		unsigned int offset2 = kvbuffer[0].data[2].value / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].data[2].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].data[2].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset2_g + dramoffset2_l), offset2, size2_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset3 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#else 
		unsigned int offset3 = kvbuffer[0].data[3].value / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].data[3].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].data[3].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset3_g + dramoffset3_l), offset3, size3_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset4 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#else 
		unsigned int offset4 = kvbuffer[0].data[4].value / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].data[4].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].data[4].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset4_g + dramoffset4_l), offset4, size4_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset5 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#else 
		unsigned int offset5 = kvbuffer[0].data[5].value / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].data[5].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].data[5].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset5_g + dramoffset5_l), offset5, size5_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset6 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#else 
		unsigned int offset6 = kvbuffer[0].data[6].value / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].data[6].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].data[6].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset6_g + dramoffset6_l), offset6, size6_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset7 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#else 
		unsigned int offset7 = kvbuffer[0].data[7].value / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].data[7].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].data[7].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset7_g + dramoffset7_l), offset7, size7_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset8 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#else 
		unsigned int offset8 = kvbuffer[0].data[8].value / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].data[8].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].data[8].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset8_g + dramoffset8_l), offset8, size8_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset9 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#else 
		unsigned int offset9 = kvbuffer[0].data[9].value / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].data[9].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].data[9].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset9_g + dramoffset9_l), offset9, size9_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset10 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#else 
		unsigned int offset10 = kvbuffer[0].data[10].value / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].data[10].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].data[10].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset10_g + dramoffset10_l), offset10, size10_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset11 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#else 
		unsigned int offset11 = kvbuffer[0].data[11].value / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].data[11].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].data[11].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset11_g + dramoffset11_l), offset11, size11_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset12 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#else 
		unsigned int offset12 = kvbuffer[0].data[12].value / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].data[12].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].data[12].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset12_g + dramoffset12_l), offset12, size12_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset13 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#else 
		unsigned int offset13 = kvbuffer[0].data[13].value / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].data[13].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].data[13].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset13_g + dramoffset13_l), offset13, size13_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset14 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#else 
		unsigned int offset14 = kvbuffer[0].data[14].value / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].data[14].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].data[14].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset14_g + dramoffset14_l), offset14, size14_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset15 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#else 
		unsigned int offset15 = kvbuffer[0].data[15].value / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].data[15].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].data[15].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset15_g + dramoffset15_l), offset15, size15_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset16 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#else 
		unsigned int offset16 = kvbuffer[0].data[16].value / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].data[16].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].data[16].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset16_g + dramoffset16_l), offset16, size16_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset17 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#else 
		unsigned int offset17 = kvbuffer[0].data[17].value / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].data[17].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].data[17].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset17_g + dramoffset17_l), offset17, size17_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset18 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#else 
		unsigned int offset18 = kvbuffer[0].data[18].value / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].data[18].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].data[18].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset18_g + dramoffset18_l), offset18, size18_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset19 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#else 
		unsigned int offset19 = kvbuffer[0].data[19].value / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].data[19].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].data[19].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset19_g + dramoffset19_l), offset19, size19_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset20 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#else 
		unsigned int offset20 = kvbuffer[0].data[20].value / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].data[20].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].data[20].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset20_g + dramoffset20_l), offset20, size20_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset21 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#else 
		unsigned int offset21 = kvbuffer[0].data[21].value / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].data[21].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].data[21].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset21_g + dramoffset21_l), offset21, size21_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset22 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#else 
		unsigned int offset22 = kvbuffer[0].data[22].value / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].data[22].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].data[22].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset22_g + dramoffset22_l), offset22, size22_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset23 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#else 
		unsigned int offset23 = kvbuffer[0].data[23].value / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].data[23].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].data[23].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset23_g + dramoffset23_l), offset23, size23_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset24 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#else 
		unsigned int offset24 = kvbuffer[0].data[24].value / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].data[24].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].data[24].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset24_g + dramoffset24_l), offset24, size24_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset25 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#else 
		unsigned int offset25 = kvbuffer[0].data[25].value / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].data[25].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].data[25].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset25_g + dramoffset25_l), offset25, size25_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset26 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#else 
		unsigned int offset26 = kvbuffer[0].data[26].value / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].data[26].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].data[26].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset26_g + dramoffset26_l), offset26, size26_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset27 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#else 
		unsigned int offset27 = kvbuffer[0].data[27].value / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].data[27].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].data[27].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset27_g + dramoffset27_l), offset27, size27_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset28 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#else 
		unsigned int offset28 = kvbuffer[0].data[28].value / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].data[28].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].data[28].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset28_g + dramoffset28_l), offset28, size28_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset29 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#else 
		unsigned int offset29 = kvbuffer[0].data[29].value / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].data[29].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].data[29].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset29_g + dramoffset29_l), offset29, size29_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset30 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#else 
		unsigned int offset30 = kvbuffer[0].data[30].value / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].data[30].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].data[30].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset30_g + dramoffset30_l), offset30, size30_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset31 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#else 
		unsigned int offset31 = kvbuffer[0].data[31].value / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].data[31].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].data[31].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#endif 
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset31_g + dramoffset31_l), offset31, size31_kvs, kvbuffer);
		
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	return;
}
void titan::write_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt * kvdram, unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
		
		#ifdef _WIDEWORD
		unsigned int offset0 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#else 
		unsigned int offset0 = kvbuffer[0].data[0].value / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].data[0].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].data[0].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset0_g + dramoffset0_l), offset0, size0_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset1 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#else 
		unsigned int offset1 = kvbuffer[0].data[1].value / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].data[1].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].data[1].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset1_g + dramoffset1_l), offset1, size1_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset2 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#else 
		unsigned int offset2 = kvbuffer[0].data[2].value / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].data[2].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].data[2].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset2_g + dramoffset2_l), offset2, size2_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset3 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#else 
		unsigned int offset3 = kvbuffer[0].data[3].value / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].data[3].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].data[3].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset3_g + dramoffset3_l), offset3, size3_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset4 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#else 
		unsigned int offset4 = kvbuffer[0].data[4].value / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].data[4].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].data[4].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset4_g + dramoffset4_l), offset4, size4_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset5 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#else 
		unsigned int offset5 = kvbuffer[0].data[5].value / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].data[5].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].data[5].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset5_g + dramoffset5_l), offset5, size5_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset6 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#else 
		unsigned int offset6 = kvbuffer[0].data[6].value / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].data[6].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].data[6].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset6_g + dramoffset6_l), offset6, size6_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset7 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#else 
		unsigned int offset7 = kvbuffer[0].data[7].value / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].data[7].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].data[7].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset7_g + dramoffset7_l), offset7, size7_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset8 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#else 
		unsigned int offset8 = kvbuffer[0].data[8].value / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].data[8].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].data[8].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset8_g + dramoffset8_l), offset8, size8_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset9 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#else 
		unsigned int offset9 = kvbuffer[0].data[9].value / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].data[9].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].data[9].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset9_g + dramoffset9_l), offset9, size9_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset10 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#else 
		unsigned int offset10 = kvbuffer[0].data[10].value / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].data[10].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].data[10].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset10_g + dramoffset10_l), offset10, size10_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset11 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#else 
		unsigned int offset11 = kvbuffer[0].data[11].value / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].data[11].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].data[11].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset11_g + dramoffset11_l), offset11, size11_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset12 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#else 
		unsigned int offset12 = kvbuffer[0].data[12].value / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].data[12].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].data[12].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset12_g + dramoffset12_l), offset12, size12_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset13 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#else 
		unsigned int offset13 = kvbuffer[0].data[13].value / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].data[13].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].data[13].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset13_g + dramoffset13_l), offset13, size13_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset14 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#else 
		unsigned int offset14 = kvbuffer[0].data[14].value / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].data[14].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].data[14].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset14_g + dramoffset14_l), offset14, size14_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset15 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#else 
		unsigned int offset15 = kvbuffer[0].data[15].value / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].data[15].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].data[15].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset15_g + dramoffset15_l), offset15, size15_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset16 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#else 
		unsigned int offset16 = kvbuffer[0].data[16].value / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].data[16].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].data[16].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset16_g + dramoffset16_l), offset16, size16_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset17 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#else 
		unsigned int offset17 = kvbuffer[0].data[17].value / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].data[17].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].data[17].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset17_g + dramoffset17_l), offset17, size17_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset18 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#else 
		unsigned int offset18 = kvbuffer[0].data[18].value / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].data[18].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].data[18].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset18_g + dramoffset18_l), offset18, size18_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset19 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#else 
		unsigned int offset19 = kvbuffer[0].data[19].value / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].data[19].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].data[19].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset19_g + dramoffset19_l), offset19, size19_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset20 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#else 
		unsigned int offset20 = kvbuffer[0].data[20].value / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].data[20].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].data[20].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset20_g + dramoffset20_l), offset20, size20_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset21 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#else 
		unsigned int offset21 = kvbuffer[0].data[21].value / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].data[21].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].data[21].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset21_g + dramoffset21_l), offset21, size21_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset22 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#else 
		unsigned int offset22 = kvbuffer[0].data[22].value / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].data[22].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].data[22].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset22_g + dramoffset22_l), offset22, size22_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset23 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#else 
		unsigned int offset23 = kvbuffer[0].data[23].value / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].data[23].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].data[23].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset23_g + dramoffset23_l), offset23, size23_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset24 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#else 
		unsigned int offset24 = kvbuffer[0].data[24].value / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].data[24].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].data[24].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset24_g + dramoffset24_l), offset24, size24_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset25 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#else 
		unsigned int offset25 = kvbuffer[0].data[25].value / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].data[25].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].data[25].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset25_g + dramoffset25_l), offset25, size25_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset26 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#else 
		unsigned int offset26 = kvbuffer[0].data[26].value / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].data[26].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].data[26].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset26_g + dramoffset26_l), offset26, size26_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset27 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#else 
		unsigned int offset27 = kvbuffer[0].data[27].value / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].data[27].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].data[27].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset27_g + dramoffset27_l), offset27, size27_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset28 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#else 
		unsigned int offset28 = kvbuffer[0].data[28].value / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].data[28].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].data[28].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset28_g + dramoffset28_l), offset28, size28_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset29 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#else 
		unsigned int offset29 = kvbuffer[0].data[29].value / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].data[29].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].data[29].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset29_g + dramoffset29_l), offset29, size29_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset30 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#else 
		unsigned int offset30 = kvbuffer[0].data[30].value / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].data[30].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].data[30].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset30_g + dramoffset30_l), offset30, size30_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset31 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#else 
		unsigned int offset31 = kvbuffer[0].data[31].value / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].data[31].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].data[31].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#endif 
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset31_g + dramoffset31_l), offset31, size31_kvs, kvbuffer);
		
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	return;
}
void titan::write_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt * kvdram, unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	
		
		#ifdef _WIDEWORD
		unsigned int offset0 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#else 
		unsigned int offset0 = kvbuffer[0].data[0].value / VECTOR_SIZE;
		unsigned int size0_kvs = (kvbuffer[0].data[0].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset0_g = 0;
		unsigned int dramoffset0_l = (kvbuffer[1].data[0].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset0 = baseaddr_kvs + dramoffset0_g + dramoffset0_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset0_g + dramoffset0_l), offset0, size0_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset1 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#else 
		unsigned int offset1 = kvbuffer[0].data[1].value / VECTOR_SIZE;
		unsigned int size1_kvs = (kvbuffer[0].data[1].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset1_g = 0;
		unsigned int dramoffset1_l = (kvbuffer[1].data[1].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset1 = baseaddr_kvs + dramoffset1_g + dramoffset1_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset1_g + dramoffset1_l), offset1, size1_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset2 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#else 
		unsigned int offset2 = kvbuffer[0].data[2].value / VECTOR_SIZE;
		unsigned int size2_kvs = (kvbuffer[0].data[2].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset2_g = 0;
		unsigned int dramoffset2_l = (kvbuffer[1].data[2].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset2 = baseaddr_kvs + dramoffset2_g + dramoffset2_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset2_g + dramoffset2_l), offset2, size2_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset3 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#else 
		unsigned int offset3 = kvbuffer[0].data[3].value / VECTOR_SIZE;
		unsigned int size3_kvs = (kvbuffer[0].data[3].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset3_g = 0;
		unsigned int dramoffset3_l = (kvbuffer[1].data[3].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset3 = baseaddr_kvs + dramoffset3_g + dramoffset3_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset3_g + dramoffset3_l), offset3, size3_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset4 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#else 
		unsigned int offset4 = kvbuffer[0].data[4].value / VECTOR_SIZE;
		unsigned int size4_kvs = (kvbuffer[0].data[4].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset4_g = 0;
		unsigned int dramoffset4_l = (kvbuffer[1].data[4].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset4 = baseaddr_kvs + dramoffset4_g + dramoffset4_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset4_g + dramoffset4_l), offset4, size4_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset5 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#else 
		unsigned int offset5 = kvbuffer[0].data[5].value / VECTOR_SIZE;
		unsigned int size5_kvs = (kvbuffer[0].data[5].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset5_g = 0;
		unsigned int dramoffset5_l = (kvbuffer[1].data[5].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset5 = baseaddr_kvs + dramoffset5_g + dramoffset5_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset5_g + dramoffset5_l), offset5, size5_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset6 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#else 
		unsigned int offset6 = kvbuffer[0].data[6].value / VECTOR_SIZE;
		unsigned int size6_kvs = (kvbuffer[0].data[6].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset6_g = 0;
		unsigned int dramoffset6_l = (kvbuffer[1].data[6].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset6 = baseaddr_kvs + dramoffset6_g + dramoffset6_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset6_g + dramoffset6_l), offset6, size6_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset7 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#else 
		unsigned int offset7 = kvbuffer[0].data[7].value / VECTOR_SIZE;
		unsigned int size7_kvs = (kvbuffer[0].data[7].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset7_g = 0;
		unsigned int dramoffset7_l = (kvbuffer[1].data[7].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset7 = baseaddr_kvs + dramoffset7_g + dramoffset7_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset7_g + dramoffset7_l), offset7, size7_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset8 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#else 
		unsigned int offset8 = kvbuffer[0].data[8].value / VECTOR_SIZE;
		unsigned int size8_kvs = (kvbuffer[0].data[8].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset8_g = 0;
		unsigned int dramoffset8_l = (kvbuffer[1].data[8].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset8 = baseaddr_kvs + dramoffset8_g + dramoffset8_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset8_g + dramoffset8_l), offset8, size8_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset9 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#else 
		unsigned int offset9 = kvbuffer[0].data[9].value / VECTOR_SIZE;
		unsigned int size9_kvs = (kvbuffer[0].data[9].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset9_g = 0;
		unsigned int dramoffset9_l = (kvbuffer[1].data[9].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset9 = baseaddr_kvs + dramoffset9_g + dramoffset9_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset9_g + dramoffset9_l), offset9, size9_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset10 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#else 
		unsigned int offset10 = kvbuffer[0].data[10].value / VECTOR_SIZE;
		unsigned int size10_kvs = (kvbuffer[0].data[10].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset10_g = 0;
		unsigned int dramoffset10_l = (kvbuffer[1].data[10].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset10 = baseaddr_kvs + dramoffset10_g + dramoffset10_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset10_g + dramoffset10_l), offset10, size10_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset11 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#else 
		unsigned int offset11 = kvbuffer[0].data[11].value / VECTOR_SIZE;
		unsigned int size11_kvs = (kvbuffer[0].data[11].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset11_g = 0;
		unsigned int dramoffset11_l = (kvbuffer[1].data[11].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset11 = baseaddr_kvs + dramoffset11_g + dramoffset11_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset11_g + dramoffset11_l), offset11, size11_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset12 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#else 
		unsigned int offset12 = kvbuffer[0].data[12].value / VECTOR_SIZE;
		unsigned int size12_kvs = (kvbuffer[0].data[12].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset12_g = 0;
		unsigned int dramoffset12_l = (kvbuffer[1].data[12].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset12 = baseaddr_kvs + dramoffset12_g + dramoffset12_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset12_g + dramoffset12_l), offset12, size12_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset13 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#else 
		unsigned int offset13 = kvbuffer[0].data[13].value / VECTOR_SIZE;
		unsigned int size13_kvs = (kvbuffer[0].data[13].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset13_g = 0;
		unsigned int dramoffset13_l = (kvbuffer[1].data[13].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset13 = baseaddr_kvs + dramoffset13_g + dramoffset13_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset13_g + dramoffset13_l), offset13, size13_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset14 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#else 
		unsigned int offset14 = kvbuffer[0].data[14].value / VECTOR_SIZE;
		unsigned int size14_kvs = (kvbuffer[0].data[14].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset14_g = 0;
		unsigned int dramoffset14_l = (kvbuffer[1].data[14].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset14 = baseaddr_kvs + dramoffset14_g + dramoffset14_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset14_g + dramoffset14_l), offset14, size14_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset15 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#else 
		unsigned int offset15 = kvbuffer[0].data[15].value / VECTOR_SIZE;
		unsigned int size15_kvs = (kvbuffer[0].data[15].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset15_g = 0;
		unsigned int dramoffset15_l = (kvbuffer[1].data[15].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset15 = baseaddr_kvs + dramoffset15_g + dramoffset15_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset15_g + dramoffset15_l), offset15, size15_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset16 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#else 
		unsigned int offset16 = kvbuffer[0].data[16].value / VECTOR_SIZE;
		unsigned int size16_kvs = (kvbuffer[0].data[16].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset16_g = 0;
		unsigned int dramoffset16_l = (kvbuffer[1].data[16].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset16 = baseaddr_kvs + dramoffset16_g + dramoffset16_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset16_g + dramoffset16_l), offset16, size16_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset17 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#else 
		unsigned int offset17 = kvbuffer[0].data[17].value / VECTOR_SIZE;
		unsigned int size17_kvs = (kvbuffer[0].data[17].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset17_g = 0;
		unsigned int dramoffset17_l = (kvbuffer[1].data[17].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset17 = baseaddr_kvs + dramoffset17_g + dramoffset17_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset17_g + dramoffset17_l), offset17, size17_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset18 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#else 
		unsigned int offset18 = kvbuffer[0].data[18].value / VECTOR_SIZE;
		unsigned int size18_kvs = (kvbuffer[0].data[18].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset18_g = 0;
		unsigned int dramoffset18_l = (kvbuffer[1].data[18].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset18 = baseaddr_kvs + dramoffset18_g + dramoffset18_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset18_g + dramoffset18_l), offset18, size18_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset19 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#else 
		unsigned int offset19 = kvbuffer[0].data[19].value / VECTOR_SIZE;
		unsigned int size19_kvs = (kvbuffer[0].data[19].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset19_g = 0;
		unsigned int dramoffset19_l = (kvbuffer[1].data[19].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset19 = baseaddr_kvs + dramoffset19_g + dramoffset19_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset19_g + dramoffset19_l), offset19, size19_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset20 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#else 
		unsigned int offset20 = kvbuffer[0].data[20].value / VECTOR_SIZE;
		unsigned int size20_kvs = (kvbuffer[0].data[20].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset20_g = 0;
		unsigned int dramoffset20_l = (kvbuffer[1].data[20].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset20 = baseaddr_kvs + dramoffset20_g + dramoffset20_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset20_g + dramoffset20_l), offset20, size20_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset21 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#else 
		unsigned int offset21 = kvbuffer[0].data[21].value / VECTOR_SIZE;
		unsigned int size21_kvs = (kvbuffer[0].data[21].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset21_g = 0;
		unsigned int dramoffset21_l = (kvbuffer[1].data[21].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset21 = baseaddr_kvs + dramoffset21_g + dramoffset21_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset21_g + dramoffset21_l), offset21, size21_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset22 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#else 
		unsigned int offset22 = kvbuffer[0].data[22].value / VECTOR_SIZE;
		unsigned int size22_kvs = (kvbuffer[0].data[22].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset22_g = 0;
		unsigned int dramoffset22_l = (kvbuffer[1].data[22].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset22 = baseaddr_kvs + dramoffset22_g + dramoffset22_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset22_g + dramoffset22_l), offset22, size22_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset23 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#else 
		unsigned int offset23 = kvbuffer[0].data[23].value / VECTOR_SIZE;
		unsigned int size23_kvs = (kvbuffer[0].data[23].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset23_g = 0;
		unsigned int dramoffset23_l = (kvbuffer[1].data[23].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset23 = baseaddr_kvs + dramoffset23_g + dramoffset23_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset23_g + dramoffset23_l), offset23, size23_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset24 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#else 
		unsigned int offset24 = kvbuffer[0].data[24].value / VECTOR_SIZE;
		unsigned int size24_kvs = (kvbuffer[0].data[24].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset24_g = 0;
		unsigned int dramoffset24_l = (kvbuffer[1].data[24].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset24 = baseaddr_kvs + dramoffset24_g + dramoffset24_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset24_g + dramoffset24_l), offset24, size24_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset25 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#else 
		unsigned int offset25 = kvbuffer[0].data[25].value / VECTOR_SIZE;
		unsigned int size25_kvs = (kvbuffer[0].data[25].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset25_g = 0;
		unsigned int dramoffset25_l = (kvbuffer[1].data[25].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset25 = baseaddr_kvs + dramoffset25_g + dramoffset25_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset25_g + dramoffset25_l), offset25, size25_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset26 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#else 
		unsigned int offset26 = kvbuffer[0].data[26].value / VECTOR_SIZE;
		unsigned int size26_kvs = (kvbuffer[0].data[26].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset26_g = 0;
		unsigned int dramoffset26_l = (kvbuffer[1].data[26].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset26 = baseaddr_kvs + dramoffset26_g + dramoffset26_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset26_g + dramoffset26_l), offset26, size26_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset27 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#else 
		unsigned int offset27 = kvbuffer[0].data[27].value / VECTOR_SIZE;
		unsigned int size27_kvs = (kvbuffer[0].data[27].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset27_g = 0;
		unsigned int dramoffset27_l = (kvbuffer[1].data[27].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset27 = baseaddr_kvs + dramoffset27_g + dramoffset27_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset27_g + dramoffset27_l), offset27, size27_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset28 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#else 
		unsigned int offset28 = kvbuffer[0].data[28].value / VECTOR_SIZE;
		unsigned int size28_kvs = (kvbuffer[0].data[28].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset28_g = 0;
		unsigned int dramoffset28_l = (kvbuffer[1].data[28].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset28 = baseaddr_kvs + dramoffset28_g + dramoffset28_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset28_g + dramoffset28_l), offset28, size28_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset29 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#else 
		unsigned int offset29 = kvbuffer[0].data[29].value / VECTOR_SIZE;
		unsigned int size29_kvs = (kvbuffer[0].data[29].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset29_g = 0;
		unsigned int dramoffset29_l = (kvbuffer[1].data[29].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset29 = baseaddr_kvs + dramoffset29_g + dramoffset29_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset29_g + dramoffset29_l), offset29, size29_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset30 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#else 
		unsigned int offset30 = kvbuffer[0].data[30].value / VECTOR_SIZE;
		unsigned int size30_kvs = (kvbuffer[0].data[30].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset30_g = 0;
		unsigned int dramoffset30_l = (kvbuffer[1].data[30].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset30 = baseaddr_kvs + dramoffset30_g + dramoffset30_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset30_g + dramoffset30_l), offset30, size30_kvs, kvbuffer);
		
		
		#ifdef _WIDEWORD
		unsigned int offset31 = kvbuffer[0].range(63, 32) / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].range(31, 0) / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].range(63, 32) / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#else 
		unsigned int offset31 = kvbuffer[0].data[31].value / VECTOR_SIZE;
		unsigned int size31_kvs = (kvbuffer[0].data[31].key / VECTOR_SIZE) % (KVDESTBUFFERSZ_KVS / NUM_PARTITIONS);
		unsigned int dramoffset31_g = 0;
		unsigned int dramoffset31_l = (kvbuffer[1].data[31].value / VECTOR_SIZE) % 1024;
		dramsz_dtype dramoffset31 = baseaddr_kvs + dramoffset31_g + dramoffset31_l;
		#endif 
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset31_g + dramoffset31_l), offset31, size31_kvs, kvbuffer);
		
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	return;
}
/** void titan::write_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEKVS00_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		// #pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		
		writeburst_ddr0_w0_sw0(kvdram, baseaddr_kvs, (dramoffset_g + dramoffset_l), offset, rwsize_kvs, kvbuffer);
		
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEKVS01_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		// #pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		
		writeburst_ddr0_w0_sw1(kvdram, baseaddr_kvs, (dramoffset_g + dramoffset_l), offset, rwsize_kvs, kvbuffer);
		
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEKVS02_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		// #pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		
		writeburst_ddr0_w0_sw2(kvdram, baseaddr_kvs, (dramoffset_g + dramoffset_l), offset, rwsize_kvs, kvbuffer);
		
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEKVS03_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		// #pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		
		writeburst_ddr0_w0_sw3(kvdram, baseaddr_kvs, (dramoffset_g + dramoffset_l), offset, rwsize_kvs, kvbuffer);
		
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEKVS04_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		// #pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		
		writeburst_ddr0_w0_sw4(kvdram, baseaddr_kvs, (dramoffset_g + dramoffset_l), offset, rwsize_kvs, kvbuffer);
		
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEKVS05_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		// #pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		
		writeburst_ddr0_w0_sw5(kvdram, baseaddr_kvs, (dramoffset_g + dramoffset_l), offset, rwsize_kvs, kvbuffer);
		
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEKVS06_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		// #pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		
		writeburst_ddr0_w0_sw6(kvdram, baseaddr_kvs, (dramoffset_g + dramoffset_l), offset, rwsize_kvs, kvbuffer);
		
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	// #pragma HLS INLINE OFF
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEKVS07_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		// #pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		
		writeburst_ddr0_w0_sw7(kvdram, baseaddr_kvs, (dramoffset_g + dramoffset_l), offset, rwsize_kvs, kvbuffer);
		
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
 */
/** void titan::write_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write00. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS00_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		WRITEBURSTKVS00_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
			// cout<<"------------- partition: "<<p<<", rwsize_kvs: "<<rwsize_kvs<<endl;
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write00 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write00 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write00 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write00 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write01. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS01_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		WRITEBURSTKVS01_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
			// cout<<"------------- partition: "<<p<<", rwsize_kvs: "<<rwsize_kvs<<endl;
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write01 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write01 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write01 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write01 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write02. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS02_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		WRITEBURSTKVS02_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
			// cout<<"------------- partition: "<<p<<", rwsize_kvs: "<<rwsize_kvs<<endl;
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write02 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write02 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write02 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write02 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write03. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS03_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		WRITEBURSTKVS03_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
			// cout<<"------------- partition: "<<p<<", rwsize_kvs: "<<rwsize_kvs<<endl;
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write03 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write03 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write03 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write03 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write04. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS04_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		WRITEBURSTKVS04_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
			// cout<<"------------- partition: "<<p<<", rwsize_kvs: "<<rwsize_kvs<<endl;
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write04 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write04 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write04 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write04 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write05. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS05_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		WRITEBURSTKVS05_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
			// cout<<"------------- partition: "<<p<<", rwsize_kvs: "<<rwsize_kvs<<endl;
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write05 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write05 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write05 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write05 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write06. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS06_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		WRITEBURSTKVS06_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
			// cout<<"------------- partition: "<<p<<", rwsize_kvs: "<<rwsize_kvs<<endl;
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write06 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write06 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write06 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write06 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::write_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write07. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS07_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;

		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = 0; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }
		WRITEBURSTKVS07_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
			// cout<<"------------- partition: "<<p<<", rwsize_kvs: "<<rwsize_kvs<<endl;
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write07 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write07 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write07 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write07 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
 */
/** void titan::write_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write00. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS00_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = KVDESTBUFFERSZ_KVS; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }		
		
		// uint9_dt offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		// uint9_dt size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		// uint24_dt dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		// uint24_dt dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		// uint24_dt dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		// uint9_dt rwsize_kvs = size_kvs;
		
		/// WRITEBURSTKVS00_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
		WRITEBURSTKVS00_LOOP1B: for(uint9_dt i=0; i<rwsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=kvbuffersz_perpartition_kvs max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write00 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write00 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write00 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write00 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISHXXX // CHANGED.
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
			// kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
		}
		// kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].size += kvbufferstats_size[p]; }
	return;
}
void titan::write_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write01. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS01_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = KVDESTBUFFERSZ_KVS; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }		
		
		// uint9_dt offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		// uint9_dt size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		// uint24_dt dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		// uint24_dt dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		// uint24_dt dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		// uint9_dt rwsize_kvs = size_kvs;
		
		/// WRITEBURSTKVS01_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
		WRITEBURSTKVS01_LOOP1B: for(uint9_dt i=0; i<rwsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=kvbuffersz_perpartition_kvs max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write01 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write01 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write01 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write01 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISHXXX // CHANGED.
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
			// kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
		}
		// kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].size += kvbufferstats_size[p]; }
	return;
}
void titan::write_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write02. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS02_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = KVDESTBUFFERSZ_KVS; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }		
		
		// uint9_dt offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		// uint9_dt size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		// uint24_dt dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		// uint24_dt dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		// uint24_dt dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		// uint9_dt rwsize_kvs = size_kvs;
		
		/// WRITEBURSTKVS02_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
		WRITEBURSTKVS02_LOOP1B: for(uint9_dt i=0; i<rwsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=kvbuffersz_perpartition_kvs max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write02 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write02 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write02 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write02 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISHXXX // CHANGED.
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
			// kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
		}
		// kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].size += kvbufferstats_size[p]; }
	return;
}
void titan::write_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write03. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS03_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = KVDESTBUFFERSZ_KVS; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }		
		
		// uint9_dt offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		// uint9_dt size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		// uint24_dt dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		// uint24_dt dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		// uint24_dt dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		// uint9_dt rwsize_kvs = size_kvs;
		
		/// WRITEBURSTKVS03_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
		WRITEBURSTKVS03_LOOP1B: for(uint9_dt i=0; i<rwsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=kvbuffersz_perpartition_kvs max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write03 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write03 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write03 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write03 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISHXXX // CHANGED.
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
			// kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
		}
		// kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].size += kvbufferstats_size[p]; }
	return;
}
void titan::write_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write04. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS04_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = KVDESTBUFFERSZ_KVS; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }		
		
		// uint9_dt offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		// uint9_dt size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		// uint24_dt dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		// uint24_dt dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		// uint24_dt dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		// uint9_dt rwsize_kvs = size_kvs;
		
		/// WRITEBURSTKVS04_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
		WRITEBURSTKVS04_LOOP1B: for(uint9_dt i=0; i<rwsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=kvbuffersz_perpartition_kvs max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write04 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write04 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write04 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write04 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISHXXX // CHANGED.
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
			// kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
		}
		// kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].size += kvbufferstats_size[p]; }
	return;
}
void titan::write_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write05. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS05_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = KVDESTBUFFERSZ_KVS; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }		
		
		// uint9_dt offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		// uint9_dt size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		// uint24_dt dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		// uint24_dt dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		// uint24_dt dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		// uint9_dt rwsize_kvs = size_kvs;
		
		/// WRITEBURSTKVS05_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
		WRITEBURSTKVS05_LOOP1B: for(uint9_dt i=0; i<rwsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=kvbuffersz_perpartition_kvs max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write05 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write05 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write05 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write05 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISHXXX // CHANGED.
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
			// kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
		}
		// kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].size += kvbufferstats_size[p]; }
	return;
}
void titan::write_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write06. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS06_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = KVDESTBUFFERSZ_KVS; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }		
		
		// uint9_dt offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		// uint9_dt size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		// uint24_dt dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		// uint24_dt dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		// uint24_dt dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		// uint9_dt rwsize_kvs = size_kvs;
		
		/// WRITEBURSTKVS06_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
		WRITEBURSTKVS06_LOOP1B: for(uint9_dt i=0; i<rwsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=kvbuffersz_perpartition_kvs max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write06 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write06 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write06 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write06 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISHXXX // CHANGED.
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
			// kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
		}
		// kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].size += kvbufferstats_size[p]; }
	return;
}
void titan::write_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write07. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	WRITEBURSTKVS07_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		kvbuffersz_dtype rwsize_kvs = size_kvs;
		if(rwsize_kvs > FORCDFINISH_KVDESTBUFFERSZ_KVS){ rwsize_kvs = KVDESTBUFFERSZ_KVS; } 
		if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
		if((offset + rwsize_kvs) >= FORCDFINISH_KVDESTBUFFERSZ_KVS){ offset = 0; }		
		
		// uint9_dt offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		// uint9_dt size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		// uint24_dt dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		// uint24_dt dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		// uint24_dt dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		// uint9_dt rwsize_kvs = size_kvs;
		
		/// WRITEBURSTKVS07_LOOP1B: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
		WRITEBURSTKVS07_LOOP1B: for(uint9_dt i=0; i<rwsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=kvbuffersz_perpartition_kvs max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write07 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write07 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write07 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write07 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			titan_kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISHXXX // CHANGED.
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
			// kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
		}
		// kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].size += kvbufferstats_size[p]; }
	return;
}
 */

void titan::writeremainingovs_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeremainingovs_ddr00. workerid: "<<workerid<<std::endl;
	#endif
	
	WRITEBURSTKVS00_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		#ifdef FORCDFINISH
		kvdram[(dramoffset & (FORCDFINISH_DRAMSZ_KVS - 1))] = ovkvs_tmp_ddr[p];
		#else 
		kvdram[dramoffset] = ovkvs_tmp_ddr[p];
		#endif 
		
		kvdeststats_tmp[p].size += ov[p].size; // VECTOR_SIZE;
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::writeremainingovs_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeremainingovs_ddr01. workerid: "<<workerid<<std::endl;
	#endif
	
	WRITEBURSTKVS01_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		#ifdef FORCDFINISH
		kvdram[(dramoffset & (FORCDFINISH_DRAMSZ_KVS - 1))] = ovkvs_tmp_ddr[p];
		#else 
		kvdram[dramoffset] = ovkvs_tmp_ddr[p];
		#endif 
		
		kvdeststats_tmp[p].size += ov[p].size; // VECTOR_SIZE;
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::writeremainingovs_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeremainingovs_ddr02. workerid: "<<workerid<<std::endl;
	#endif
	
	WRITEBURSTKVS02_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		#ifdef FORCDFINISH
		kvdram[(dramoffset & (FORCDFINISH_DRAMSZ_KVS - 1))] = ovkvs_tmp_ddr[p];
		#else 
		kvdram[dramoffset] = ovkvs_tmp_ddr[p];
		#endif 
		
		kvdeststats_tmp[p].size += ov[p].size; // VECTOR_SIZE;
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::writeremainingovs_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeremainingovs_ddr03. workerid: "<<workerid<<std::endl;
	#endif
	
	WRITEBURSTKVS03_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		#ifdef FORCDFINISH
		kvdram[(dramoffset & (FORCDFINISH_DRAMSZ_KVS - 1))] = ovkvs_tmp_ddr[p];
		#else 
		kvdram[dramoffset] = ovkvs_tmp_ddr[p];
		#endif 
		
		kvdeststats_tmp[p].size += ov[p].size; // VECTOR_SIZE;
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::writeremainingovs_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeremainingovs_ddr04. workerid: "<<workerid<<std::endl;
	#endif
	
	WRITEBURSTKVS04_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		#ifdef FORCDFINISH
		kvdram[(dramoffset & (FORCDFINISH_DRAMSZ_KVS - 1))] = ovkvs_tmp_ddr[p];
		#else 
		kvdram[dramoffset] = ovkvs_tmp_ddr[p];
		#endif 
		
		kvdeststats_tmp[p].size += ov[p].size; // VECTOR_SIZE;
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::writeremainingovs_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeremainingovs_ddr05. workerid: "<<workerid<<std::endl;
	#endif
	
	WRITEBURSTKVS05_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		#ifdef FORCDFINISH
		kvdram[(dramoffset & (FORCDFINISH_DRAMSZ_KVS - 1))] = ovkvs_tmp_ddr[p];
		#else 
		kvdram[dramoffset] = ovkvs_tmp_ddr[p];
		#endif 
		
		kvdeststats_tmp[p].size += ov[p].size; // VECTOR_SIZE;
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::writeremainingovs_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeremainingovs_ddr06. workerid: "<<workerid<<std::endl;
	#endif
	
	WRITEBURSTKVS06_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		#ifdef FORCDFINISH
		kvdram[(dramoffset & (FORCDFINISH_DRAMSZ_KVS - 1))] = ovkvs_tmp_ddr[p];
		#else 
		kvdram[dramoffset] = ovkvs_tmp_ddr[p];
		#endif 
		
		kvdeststats_tmp[p].size += ov[p].size; // VECTOR_SIZE;
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void titan::writeremainingovs_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeremainingovs_ddr07. workerid: "<<workerid<<std::endl;
	#endif
	
	WRITEBURSTKVS07_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		
		#ifdef FORCDFINISH
		kvdram[(dramoffset & (FORCDFINISH_DRAMSZ_KVS - 1))] = ovkvs_tmp_ddr[p];
		#else 
		kvdram[dramoffset] = ovkvs_tmp_ddr[p];
		#endif 
		
		kvdeststats_tmp[p].size += ov[p].size; // VECTOR_SIZE;
		
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}

// WRITE VERTICES TO DRAM (APPLY PHASE)
void titan::writeAU_Vs_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs00 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS00_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs00:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs00:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		// kvdram[(dramoffset + i)] = verticesbuffer[i];
		// kvdram[((dramoffset + i) % _x3PADDEDKVDATA_SIZE_KVS)] = verticesbuffer[i]; 
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = verticesbuffer[i];
		#else 
		kvdram[(dramoffset + i)] = verticesbuffer[i];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairswritten0_transfers += 1;
	#endif
	return;
}
void titan::writeAU_Vs_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs01 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS01_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs01:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs01:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		// kvdram[(dramoffset + i)] = verticesbuffer[i];
		// kvdram[((dramoffset + i) % _x3PADDEDKVDATA_SIZE_KVS)] = verticesbuffer[i]; 
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = verticesbuffer[i];
		#else 
		kvdram[(dramoffset + i)] = verticesbuffer[i];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairswritten0_transfers += 1;
	#endif
	return;
}
void titan::writeAU_Vs_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs02 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS02_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs02:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs02:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		// kvdram[(dramoffset + i)] = verticesbuffer[i];
		// kvdram[((dramoffset + i) % _x3PADDEDKVDATA_SIZE_KVS)] = verticesbuffer[i]; 
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = verticesbuffer[i];
		#else 
		kvdram[(dramoffset + i)] = verticesbuffer[i];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairswritten0_transfers += 1;
	#endif
	return;
}
void titan::writeAU_Vs_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs03 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS03_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs03:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs03:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		// kvdram[(dramoffset + i)] = verticesbuffer[i];
		// kvdram[((dramoffset + i) % _x3PADDEDKVDATA_SIZE_KVS)] = verticesbuffer[i]; 
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = verticesbuffer[i];
		#else 
		kvdram[(dramoffset + i)] = verticesbuffer[i];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairswritten0_transfers += 1;
	#endif
	return;
}
void titan::writeAU_Vs_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs04 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS04_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs04:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs04:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		// kvdram[(dramoffset + i)] = verticesbuffer[i];
		// kvdram[((dramoffset + i) % _x3PADDEDKVDATA_SIZE_KVS)] = verticesbuffer[i]; 
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = verticesbuffer[i];
		#else 
		kvdram[(dramoffset + i)] = verticesbuffer[i];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairswritten0_transfers += 1;
	#endif
	return;
}
void titan::writeAU_Vs_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs05 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS05_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs05:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs05:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		// kvdram[(dramoffset + i)] = verticesbuffer[i];
		// kvdram[((dramoffset + i) % _x3PADDEDKVDATA_SIZE_KVS)] = verticesbuffer[i]; 
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = verticesbuffer[i];
		#else 
		kvdram[(dramoffset + i)] = verticesbuffer[i];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairswritten0_transfers += 1;
	#endif
	return;
}
void titan::writeAU_Vs_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs06 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS06_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs06:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs06:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		// kvdram[(dramoffset + i)] = verticesbuffer[i];
		// kvdram[((dramoffset + i) % _x3PADDEDKVDATA_SIZE_KVS)] = verticesbuffer[i]; 
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = verticesbuffer[i];
		#else 
		kvdram[(dramoffset + i)] = verticesbuffer[i];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairswritten0_transfers += 1;
	#endif
	return;
}
void titan::writeAU_Vs_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs07 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS07_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs07:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs07:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairswritten0 += VECTOR_SIZE;
		#endif
		// kvdram[(dramoffset + i)] = verticesbuffer[i];
		// kvdram[((dramoffset + i) % _x3PADDEDKVDATA_SIZE_KVS)] = verticesbuffer[i]; 
		#ifdef FORCDFINISH
		kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = verticesbuffer[i];
		#else 
		kvdram[(dramoffset + i)] = verticesbuffer[i];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	titan_kvpairswritten0_transfers += 1;
	#endif
	return;
}

// READ MANY
void titan::readmany_ddr0_w0(unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs
	,uint512_dt kvbuffer0[KVSRCBUFFERSZ_KVS]
	,uint512_dt kvbuffer1[KVSRCBUFFERSZ_KVS]
	,uint512_dt kvbuffer2[KVSRCBUFFERSZ_KVS]
	,uint512_dt kvbuffer3[KVSRCBUFFERSZ_KVS]
	,uint512_dt kvbuffer4[KVSRCBUFFERSZ_KVS]
	,uint512_dt kvbuffer5[KVSRCBUFFERSZ_KVS]
	,uint512_dt kvbuffer6[KVSRCBUFFERSZ_KVS]
	,uint512_dt kvbuffer7[KVSRCBUFFERSZ_KVS]
	){
	unsigned int worker0id = beginworkerid + 0;
	unsigned int baseaddr0_kvs = baseaddr_kvs;
	unsigned int offset0_kvs = kvreadoffset_kvs + (0 * KVBUFFERSZ_KVS);
	unsigned int kvsize0_kvs = kvsize_kvs;
	
	unsigned int worker1id = beginworkerid + 1;
	unsigned int baseaddr1_kvs = baseaddr_kvs;
	unsigned int offset1_kvs = kvreadoffset_kvs + (1 * KVBUFFERSZ_KVS);
	unsigned int kvsize1_kvs = kvsize_kvs;
	
	unsigned int worker2id = beginworkerid + 2;
	unsigned int baseaddr2_kvs = baseaddr_kvs;
	unsigned int offset2_kvs = kvreadoffset_kvs + (2 * KVBUFFERSZ_KVS);
	unsigned int kvsize2_kvs = kvsize_kvs;
	
	unsigned int worker3id = beginworkerid + 3;
	unsigned int baseaddr3_kvs = baseaddr_kvs;
	unsigned int offset3_kvs = kvreadoffset_kvs + (3 * KVBUFFERSZ_KVS);
	unsigned int kvsize3_kvs = kvsize_kvs;
	
	unsigned int worker4id = beginworkerid + 4;
	unsigned int baseaddr4_kvs = baseaddr_kvs;
	unsigned int offset4_kvs = kvreadoffset_kvs + (4 * KVBUFFERSZ_KVS);
	unsigned int kvsize4_kvs = kvsize_kvs;
	
	unsigned int worker5id = beginworkerid + 5;
	unsigned int baseaddr5_kvs = baseaddr_kvs;
	unsigned int offset5_kvs = kvreadoffset_kvs + (5 * KVBUFFERSZ_KVS);
	unsigned int kvsize5_kvs = kvsize_kvs;
	
	unsigned int worker6id = beginworkerid + 6;
	unsigned int baseaddr6_kvs = baseaddr_kvs;
	unsigned int offset6_kvs = kvreadoffset_kvs + (6 * KVBUFFERSZ_KVS);
	unsigned int kvsize6_kvs = kvsize_kvs;
	
	unsigned int worker7id = beginworkerid + 7;
	unsigned int baseaddr7_kvs = baseaddr_kvs;
	unsigned int offset7_kvs = kvreadoffset_kvs + (7 * KVBUFFERSZ_KVS);
	unsigned int kvsize7_kvs = kvsize_kvs;
	
		
titan::read_ddr0_w0_sw0(worker0id, kvdram, baseaddr0_kvs, offset0_kvs, kvsize0_kvs, kvbuffer0);
	/// read_ddr0_w0_sw0((beginworkerid + 0), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (0 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer0);
titan::read_ddr0_w0_sw1(worker1id, kvdram, baseaddr1_kvs, offset1_kvs, kvsize1_kvs, kvbuffer1);
	/// read_ddr0_w0_sw1((beginworkerid + 1), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (1 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer1);
titan::read_ddr0_w0_sw2(worker2id, kvdram, baseaddr2_kvs, offset2_kvs, kvsize2_kvs, kvbuffer2);
	/// read_ddr0_w0_sw2((beginworkerid + 2), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (2 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer2);
titan::read_ddr0_w0_sw3(worker3id, kvdram, baseaddr3_kvs, offset3_kvs, kvsize3_kvs, kvbuffer3);
	/// read_ddr0_w0_sw3((beginworkerid + 3), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (3 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer3);
titan::read_ddr0_w0_sw4(worker4id, kvdram, baseaddr4_kvs, offset4_kvs, kvsize4_kvs, kvbuffer4);
	/// read_ddr0_w0_sw4((beginworkerid + 4), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (4 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer4);
titan::read_ddr0_w0_sw5(worker5id, kvdram, baseaddr5_kvs, offset5_kvs, kvsize5_kvs, kvbuffer5);
	/// read_ddr0_w0_sw5((beginworkerid + 5), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (5 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer5);
titan::read_ddr0_w0_sw6(worker6id, kvdram, baseaddr6_kvs, offset6_kvs, kvsize6_kvs, kvbuffer6);
	/// read_ddr0_w0_sw6((beginworkerid + 6), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (6 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer6);
titan::read_ddr0_w0_sw7(worker7id, kvdram, baseaddr7_kvs, offset7_kvs, kvsize7_kvs, kvbuffer7);
	/// read_ddr0_w0_sw7((beginworkerid + 7), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (7 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer7);
	return;
}

void titan::readmanyAU_Vs_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs
	,uint512_dt verticesbuffer0[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer1[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer2[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer3[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer4[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer5[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer6[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer7[APPLYVERTEXBUFFERSZ_KVS]
	){
titan::readAU_V_ddr0_w0_sw0(ssdpartitonid, (beginworkerid + 0), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (0 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer0);
titan::readAU_V_ddr0_w0_sw1(ssdpartitonid, (beginworkerid + 1), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (1 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer1);
titan::readAU_V_ddr0_w0_sw2(ssdpartitonid, (beginworkerid + 2), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (2 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer2);
titan::readAU_V_ddr0_w0_sw3(ssdpartitonid, (beginworkerid + 3), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (3 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer3);
titan::readAU_V_ddr0_w0_sw4(ssdpartitonid, (beginworkerid + 4), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (4 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer4);
titan::readAU_V_ddr0_w0_sw5(ssdpartitonid, (beginworkerid + 5), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (5 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer5);
titan::readAU_V_ddr0_w0_sw6(ssdpartitonid, (beginworkerid + 6), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (6 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer6);
titan::readAU_V_ddr0_w0_sw7(ssdpartitonid, (beginworkerid + 7), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (7 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer7);
	return;
}

void titan::readmanyAU_VUs_ddr0_w0(unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, metadata_t sourcestatbuffer[NUMSUBWORKERS], unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs
	,uint512_dt kvbuffer0[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer1[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer2[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer3[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer4[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer5[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer6[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer7[KVSRCBUFFERSZ_APPLY_KVS]
	){
	unsigned int worker0id = beginworkerid + 0; 
	unsigned int baseaddr0_kvs = baseaddr_kvs;
	unsigned int offset0_kvs = (sourcestatbuffer[0].offset / VECTOR_SIZE) + kvreadoffset_kvs;
	unsigned int kvsize0_kvs = (sourcestatbuffer[0].offset + sourcestatbuffer[0].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// cout<<"readmanyAU_VUs_ddr:: kvsize0_kvs: "<<kvsize0_kvs<<", sourcestatbuffer[0].offset: "<<sourcestatbuffer[0].offset<<", sourcestatbuffer[0].size: "<<sourcestatbuffer[0].size<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<endl;
	unsigned int worker1id = beginworkerid + 1; 
	unsigned int baseaddr1_kvs = baseaddr_kvs;
	unsigned int offset1_kvs = (sourcestatbuffer[1].offset / VECTOR_SIZE) + kvreadoffset_kvs;
	unsigned int kvsize1_kvs = (sourcestatbuffer[1].offset + sourcestatbuffer[1].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// cout<<"readmanyAU_VUs_ddr:: kvsize1_kvs: "<<kvsize1_kvs<<", sourcestatbuffer[1].offset: "<<sourcestatbuffer[1].offset<<", sourcestatbuffer[1].size: "<<sourcestatbuffer[1].size<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<endl;
	unsigned int worker2id = beginworkerid + 2; 
	unsigned int baseaddr2_kvs = baseaddr_kvs;
	unsigned int offset2_kvs = (sourcestatbuffer[2].offset / VECTOR_SIZE) + kvreadoffset_kvs;
	unsigned int kvsize2_kvs = (sourcestatbuffer[2].offset + sourcestatbuffer[2].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// cout<<"readmanyAU_VUs_ddr:: kvsize2_kvs: "<<kvsize2_kvs<<", sourcestatbuffer[2].offset: "<<sourcestatbuffer[2].offset<<", sourcestatbuffer[2].size: "<<sourcestatbuffer[2].size<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<endl;
	unsigned int worker3id = beginworkerid + 3; 
	unsigned int baseaddr3_kvs = baseaddr_kvs;
	unsigned int offset3_kvs = (sourcestatbuffer[3].offset / VECTOR_SIZE) + kvreadoffset_kvs;
	unsigned int kvsize3_kvs = (sourcestatbuffer[3].offset + sourcestatbuffer[3].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// cout<<"readmanyAU_VUs_ddr:: kvsize3_kvs: "<<kvsize3_kvs<<", sourcestatbuffer[3].offset: "<<sourcestatbuffer[3].offset<<", sourcestatbuffer[3].size: "<<sourcestatbuffer[3].size<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<endl;
	unsigned int worker4id = beginworkerid + 4; 
	unsigned int baseaddr4_kvs = baseaddr_kvs;
	unsigned int offset4_kvs = (sourcestatbuffer[4].offset / VECTOR_SIZE) + kvreadoffset_kvs;
	unsigned int kvsize4_kvs = (sourcestatbuffer[4].offset + sourcestatbuffer[4].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// cout<<"readmanyAU_VUs_ddr:: kvsize4_kvs: "<<kvsize4_kvs<<", sourcestatbuffer[4].offset: "<<sourcestatbuffer[4].offset<<", sourcestatbuffer[4].size: "<<sourcestatbuffer[4].size<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<endl;
	unsigned int worker5id = beginworkerid + 5; 
	unsigned int baseaddr5_kvs = baseaddr_kvs;
	unsigned int offset5_kvs = (sourcestatbuffer[5].offset / VECTOR_SIZE) + kvreadoffset_kvs;
	unsigned int kvsize5_kvs = (sourcestatbuffer[5].offset + sourcestatbuffer[5].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// cout<<"readmanyAU_VUs_ddr:: kvsize5_kvs: "<<kvsize5_kvs<<", sourcestatbuffer[5].offset: "<<sourcestatbuffer[5].offset<<", sourcestatbuffer[5].size: "<<sourcestatbuffer[5].size<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<endl;
	unsigned int worker6id = beginworkerid + 6; 
	unsigned int baseaddr6_kvs = baseaddr_kvs;
	unsigned int offset6_kvs = (sourcestatbuffer[6].offset / VECTOR_SIZE) + kvreadoffset_kvs;
	unsigned int kvsize6_kvs = (sourcestatbuffer[6].offset + sourcestatbuffer[6].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// cout<<"readmanyAU_VUs_ddr:: kvsize6_kvs: "<<kvsize6_kvs<<", sourcestatbuffer[6].offset: "<<sourcestatbuffer[6].offset<<", sourcestatbuffer[6].size: "<<sourcestatbuffer[6].size<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<endl;
	unsigned int worker7id = beginworkerid + 7; 
	unsigned int baseaddr7_kvs = baseaddr_kvs;
	unsigned int offset7_kvs = (sourcestatbuffer[7].offset / VECTOR_SIZE) + kvreadoffset_kvs;
	unsigned int kvsize7_kvs = (sourcestatbuffer[7].offset + sourcestatbuffer[7].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	// cout<<"readmanyAU_VUs_ddr:: kvsize7_kvs: "<<kvsize7_kvs<<", sourcestatbuffer[7].offset: "<<sourcestatbuffer[7].offset<<", sourcestatbuffer[7].size: "<<sourcestatbuffer[7].size<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<endl;
	
titan::readAU_VU_ddr0_w0_sw0(worker0id, kvdram, baseaddr0_kvs, offset0_kvs, kvsize0_kvs, kvbuffer0);
titan::readAU_VU_ddr0_w0_sw0(worker1id, kvdram, baseaddr1_kvs, offset1_kvs, kvsize1_kvs, kvbuffer1);
titan::readAU_VU_ddr0_w0_sw0(worker2id, kvdram, baseaddr2_kvs, offset2_kvs, kvsize2_kvs, kvbuffer2);
titan::readAU_VU_ddr0_w0_sw0(worker3id, kvdram, baseaddr3_kvs, offset3_kvs, kvsize3_kvs, kvbuffer3);
titan::readAU_VU_ddr0_w0_sw0(worker4id, kvdram, baseaddr4_kvs, offset4_kvs, kvsize4_kvs, kvbuffer4);
titan::readAU_VU_ddr0_w0_sw0(worker5id, kvdram, baseaddr5_kvs, offset5_kvs, kvsize5_kvs, kvbuffer5);
titan::readAU_VU_ddr0_w0_sw0(worker6id, kvdram, baseaddr6_kvs, offset6_kvs, kvsize6_kvs, kvbuffer6);
titan::readAU_VU_ddr0_w0_sw0(worker7id, kvdram, baseaddr7_kvs, offset7_kvs, kvsize7_kvs, kvbuffer7);
	return;
}

// PARTITION MANY
void titan::partitionmany_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange
	,uint512_dt kvbuffer01[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer02[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS], smallmargin_t ov0[NUM_PARTITIONS], uint512_dt ovkvs_tmp0[NUM_PARTITIONS]
	,uint512_dt kvbuffer11[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer12[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS], smallmargin_t ov1[NUM_PARTITIONS], uint512_dt ovkvs_tmp1[NUM_PARTITIONS]
	,uint512_dt kvbuffer21[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer22[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS], smallmargin_t ov2[NUM_PARTITIONS], uint512_dt ovkvs_tmp2[NUM_PARTITIONS]
	,uint512_dt kvbuffer31[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer32[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS], smallmargin_t ov3[NUM_PARTITIONS], uint512_dt ovkvs_tmp3[NUM_PARTITIONS]
	,uint512_dt kvbuffer41[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer42[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS], smallmargin_t ov4[NUM_PARTITIONS], uint512_dt ovkvs_tmp4[NUM_PARTITIONS]
	,uint512_dt kvbuffer51[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer52[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS], smallmargin_t ov5[NUM_PARTITIONS], uint512_dt ovkvs_tmp5[NUM_PARTITIONS]
	,uint512_dt kvbuffer61[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer62[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS], smallmargin_t ov6[NUM_PARTITIONS], uint512_dt ovkvs_tmp6[NUM_PARTITIONS]
	,uint512_dt kvbuffer71[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer72[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS], smallmargin_t ov7[NUM_PARTITIONS], uint512_dt ovkvs_tmp7[NUM_PARTITIONS]
	){
	#pragma HLS INLINE OFF
	unsigned int ssdpartiton0id = ssdpartitonid;
	unsigned int worker0id =  beginworkerid + 0;
	unsigned int subworker0id = 0;
	unsigned int kvreadoffset0_kvs = kvreadoffset_kvs;
	unsigned int kvsize0_kvs = kvsize_kvs;
	unsigned int currentLOP0 = currentLOP;
	unsigned int destpartitionoffset0 = destpartitionoffset;
	unsigned int upperlimit0 = upperlimit;
	unsigned int destpartitionrange0 = destpartitionrange;	
	unsigned int ssdpartiton1id = ssdpartitonid;
	unsigned int worker1id =  beginworkerid + 1;
	unsigned int subworker1id = 1;
	unsigned int kvreadoffset1_kvs = kvreadoffset_kvs;
	unsigned int kvsize1_kvs = kvsize_kvs;
	unsigned int currentLOP1 = currentLOP;
	unsigned int destpartitionoffset1 = destpartitionoffset;
	unsigned int upperlimit1 = upperlimit;
	unsigned int destpartitionrange1 = destpartitionrange;	
	unsigned int ssdpartiton2id = ssdpartitonid;
	unsigned int worker2id =  beginworkerid + 2;
	unsigned int subworker2id = 2;
	unsigned int kvreadoffset2_kvs = kvreadoffset_kvs;
	unsigned int kvsize2_kvs = kvsize_kvs;
	unsigned int currentLOP2 = currentLOP;
	unsigned int destpartitionoffset2 = destpartitionoffset;
	unsigned int upperlimit2 = upperlimit;
	unsigned int destpartitionrange2 = destpartitionrange;	
	unsigned int ssdpartiton3id = ssdpartitonid;
	unsigned int worker3id =  beginworkerid + 3;
	unsigned int subworker3id = 3;
	unsigned int kvreadoffset3_kvs = kvreadoffset_kvs;
	unsigned int kvsize3_kvs = kvsize_kvs;
	unsigned int currentLOP3 = currentLOP;
	unsigned int destpartitionoffset3 = destpartitionoffset;
	unsigned int upperlimit3 = upperlimit;
	unsigned int destpartitionrange3 = destpartitionrange;	
	unsigned int ssdpartiton4id = ssdpartitonid;
	unsigned int worker4id =  beginworkerid + 4;
	unsigned int subworker4id = 4;
	unsigned int kvreadoffset4_kvs = kvreadoffset_kvs;
	unsigned int kvsize4_kvs = kvsize_kvs;
	unsigned int currentLOP4 = currentLOP;
	unsigned int destpartitionoffset4 = destpartitionoffset;
	unsigned int upperlimit4 = upperlimit;
	unsigned int destpartitionrange4 = destpartitionrange;	
	unsigned int ssdpartiton5id = ssdpartitonid;
	unsigned int worker5id =  beginworkerid + 5;
	unsigned int subworker5id = 5;
	unsigned int kvreadoffset5_kvs = kvreadoffset_kvs;
	unsigned int kvsize5_kvs = kvsize_kvs;
	unsigned int currentLOP5 = currentLOP;
	unsigned int destpartitionoffset5 = destpartitionoffset;
	unsigned int upperlimit5 = upperlimit;
	unsigned int destpartitionrange5 = destpartitionrange;	
	unsigned int ssdpartiton6id = ssdpartitonid;
	unsigned int worker6id =  beginworkerid + 6;
	unsigned int subworker6id = 6;
	unsigned int kvreadoffset6_kvs = kvreadoffset_kvs;
	unsigned int kvsize6_kvs = kvsize_kvs;
	unsigned int currentLOP6 = currentLOP;
	unsigned int destpartitionoffset6 = destpartitionoffset;
	unsigned int upperlimit6 = upperlimit;
	unsigned int destpartitionrange6 = destpartitionrange;	
	unsigned int ssdpartiton7id = ssdpartitonid;
	unsigned int worker7id =  beginworkerid + 7;
	unsigned int subworker7id = 7;
	unsigned int kvreadoffset7_kvs = kvreadoffset_kvs;
	unsigned int kvsize7_kvs = kvsize_kvs;
	unsigned int currentLOP7 = currentLOP;
	unsigned int destpartitionoffset7 = destpartitionoffset;
	unsigned int upperlimit7 = upperlimit;
	unsigned int destpartitionrange7 = destpartitionrange;	
	
titan::partition_ddr0_w0_sw0(ssdpartiton0id, worker0id, subworker0id, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, kvbuffer01, kvbuffer02, kvbufferstats_offset0, kvbufferstats_size0, ov0, ovkvs_tmp0);
titan::partition_ddr0_w0_sw1(ssdpartiton1id, worker1id, subworker1id, kvreadoffset1_kvs, kvsize1_kvs, currentLOP1, destpartitionoffset1, upperlimit1, destpartitionrange1, kvbuffer11, kvbuffer12, kvbufferstats_offset1, kvbufferstats_size1, ov1, ovkvs_tmp1);
titan::partition_ddr0_w0_sw2(ssdpartiton2id, worker2id, subworker2id, kvreadoffset2_kvs, kvsize2_kvs, currentLOP2, destpartitionoffset2, upperlimit2, destpartitionrange2, kvbuffer21, kvbuffer22, kvbufferstats_offset2, kvbufferstats_size2, ov2, ovkvs_tmp2);
titan::partition_ddr0_w0_sw3(ssdpartiton3id, worker3id, subworker3id, kvreadoffset3_kvs, kvsize3_kvs, currentLOP3, destpartitionoffset3, upperlimit3, destpartitionrange3, kvbuffer31, kvbuffer32, kvbufferstats_offset3, kvbufferstats_size3, ov3, ovkvs_tmp3);
titan::partition_ddr0_w0_sw4(ssdpartiton4id, worker4id, subworker4id, kvreadoffset4_kvs, kvsize4_kvs, currentLOP4, destpartitionoffset4, upperlimit4, destpartitionrange4, kvbuffer41, kvbuffer42, kvbufferstats_offset4, kvbufferstats_size4, ov4, ovkvs_tmp4);
titan::partition_ddr0_w0_sw5(ssdpartiton5id, worker5id, subworker5id, kvreadoffset5_kvs, kvsize5_kvs, currentLOP5, destpartitionoffset5, upperlimit5, destpartitionrange5, kvbuffer51, kvbuffer52, kvbufferstats_offset5, kvbufferstats_size5, ov5, ovkvs_tmp5);
titan::partition_ddr0_w0_sw6(ssdpartiton6id, worker6id, subworker6id, kvreadoffset6_kvs, kvsize6_kvs, currentLOP6, destpartitionoffset6, upperlimit6, destpartitionrange6, kvbuffer61, kvbuffer62, kvbufferstats_offset6, kvbufferstats_size6, ov6, ovkvs_tmp6);
titan::partition_ddr0_w0_sw7(ssdpartiton7id, worker7id, subworker7id, kvreadoffset7_kvs, kvsize7_kvs, currentLOP7, destpartitionoffset7, upperlimit7, destpartitionrange7, kvbuffer71, kvbuffer72, kvbufferstats_offset7, kvbufferstats_size7, ov7, ovkvs_tmp7);
	return;
}

// APPLY MANY
void titan::reducemany_ddr0_w0(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int beginworkerid, unsigned int verticesreadoffset_vs, metadata_t sourcestatbuffer[NUMSUBWORKERS], unsigned int kvreadoffset_kvs
	,uint512_dt verticesbuffer0[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer1[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer2[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer3[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer4[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer5[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer6[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer7[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt kvbuffer0[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer1[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer2[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer3[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer4[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer5[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer6[KVSRCBUFFERSZ_APPLY_KVS]
	,uint512_dt kvbuffer7[KVSRCBUFFERSZ_APPLY_KVS]
	
	){		
	unsigned int ssdpartiton0id = ssdpartitonid; 
	unsigned int worker0id = beginworkerid + 0; 
	unsigned int verticesreadoffset0_vs = verticesreadoffset_vs + (0 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset0_kvs = kvreadoffset_kvs; 
	unsigned int kvsize0_kvs = (sourcestatbuffer[0].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton1id = ssdpartitonid; 
	unsigned int worker1id = beginworkerid + 1; 
	unsigned int verticesreadoffset1_vs = verticesreadoffset_vs + (1 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset1_kvs = kvreadoffset_kvs; 
	unsigned int kvsize1_kvs = (sourcestatbuffer[1].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton2id = ssdpartitonid; 
	unsigned int worker2id = beginworkerid + 2; 
	unsigned int verticesreadoffset2_vs = verticesreadoffset_vs + (2 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset2_kvs = kvreadoffset_kvs; 
	unsigned int kvsize2_kvs = (sourcestatbuffer[2].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton3id = ssdpartitonid; 
	unsigned int worker3id = beginworkerid + 3; 
	unsigned int verticesreadoffset3_vs = verticesreadoffset_vs + (3 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset3_kvs = kvreadoffset_kvs; 
	unsigned int kvsize3_kvs = (sourcestatbuffer[3].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton4id = ssdpartitonid; 
	unsigned int worker4id = beginworkerid + 4; 
	unsigned int verticesreadoffset4_vs = verticesreadoffset_vs + (4 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset4_kvs = kvreadoffset_kvs; 
	unsigned int kvsize4_kvs = (sourcestatbuffer[4].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton5id = ssdpartitonid; 
	unsigned int worker5id = beginworkerid + 5; 
	unsigned int verticesreadoffset5_vs = verticesreadoffset_vs + (5 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset5_kvs = kvreadoffset_kvs; 
	unsigned int kvsize5_kvs = (sourcestatbuffer[5].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton6id = ssdpartitonid; 
	unsigned int worker6id = beginworkerid + 6; 
	unsigned int verticesreadoffset6_vs = verticesreadoffset_vs + (6 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset6_kvs = kvreadoffset_kvs; 
	unsigned int kvsize6_kvs = (sourcestatbuffer[6].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton7id = ssdpartitonid; 
	unsigned int worker7id = beginworkerid + 7; 
	unsigned int verticesreadoffset7_vs = verticesreadoffset_vs + (7 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset7_kvs = kvreadoffset_kvs; 
	unsigned int kvsize7_kvs = (sourcestatbuffer[7].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw0: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:0: "<<((verticesreadoffset_vs + (0 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw1: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:1: "<<((verticesreadoffset_vs + (1 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw2: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:2: "<<((verticesreadoffset_vs + (2 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw3: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:3: "<<((verticesreadoffset_vs + (3 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw4: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:4: "<<((verticesreadoffset_vs + (4 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw5: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:5: "<<((verticesreadoffset_vs + (5 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw6: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:6: "<<((verticesreadoffset_vs + (6 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw7: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:7: "<<((verticesreadoffset_vs + (7 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	
titan::reduce_ddr0_w0_sw0(IterCount, ssdpartiton0id, worker0id, verticesreadoffset0_vs, kvreadoffset0_kvs, kvsize0_kvs, sourcestatbuffer[0], verticesbuffer0, kvbuffer0); // FIXME
titan::reduce_ddr0_w0_sw1(IterCount, ssdpartiton1id, worker1id, verticesreadoffset1_vs, kvreadoffset1_kvs, kvsize1_kvs, sourcestatbuffer[1], verticesbuffer1, kvbuffer1); // FIXME
titan::reduce_ddr0_w0_sw2(IterCount, ssdpartiton2id, worker2id, verticesreadoffset2_vs, kvreadoffset2_kvs, kvsize2_kvs, sourcestatbuffer[2], verticesbuffer2, kvbuffer2); // FIXME
titan::reduce_ddr0_w0_sw3(IterCount, ssdpartiton3id, worker3id, verticesreadoffset3_vs, kvreadoffset3_kvs, kvsize3_kvs, sourcestatbuffer[3], verticesbuffer3, kvbuffer3); // FIXME
titan::reduce_ddr0_w0_sw4(IterCount, ssdpartiton4id, worker4id, verticesreadoffset4_vs, kvreadoffset4_kvs, kvsize4_kvs, sourcestatbuffer[4], verticesbuffer4, kvbuffer4); // FIXME
titan::reduce_ddr0_w0_sw5(IterCount, ssdpartiton5id, worker5id, verticesreadoffset5_vs, kvreadoffset5_kvs, kvsize5_kvs, sourcestatbuffer[5], verticesbuffer5, kvbuffer5); // FIXME
titan::reduce_ddr0_w0_sw6(IterCount, ssdpartiton6id, worker6id, verticesreadoffset6_vs, kvreadoffset6_kvs, kvsize6_kvs, sourcestatbuffer[6], verticesbuffer6, kvbuffer6); // FIXME
titan::reduce_ddr0_w0_sw7(IterCount, ssdpartiton7id, worker7id, verticesreadoffset7_vs, kvreadoffset7_kvs, kvsize7_kvs, sourcestatbuffer[7], verticesbuffer7, kvbuffer7); // FIXME
	return;
}

// WRITE MANY
void titan::writemany_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int currentLOP, unsigned int destpartitionoffset	
	,uint512_dt kvbuffer0[KVDESTBUFFERSZ_KVS]
	,uint512_dt kvbuffer1[KVDESTBUFFERSZ_KVS]
	,uint512_dt kvbuffer2[KVDESTBUFFERSZ_KVS]
	,uint512_dt kvbuffer3[KVDESTBUFFERSZ_KVS]
	,uint512_dt kvbuffer4[KVDESTBUFFERSZ_KVS]
	,uint512_dt kvbuffer5[KVDESTBUFFERSZ_KVS]
	,uint512_dt kvbuffer6[KVDESTBUFFERSZ_KVS]
	,uint512_dt kvbuffer7[KVDESTBUFFERSZ_KVS]
	){
	unsigned int ssdpartiton0id = ssdpartitonid;
	unsigned int worker0id = beginworkerid + 0;
	unsigned int currentLOP0 = currentLOP;
	unsigned int destpartitionoffset0 = destpartitionoffset;
	unsigned int baseaddr0_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton1id = ssdpartitonid;
	unsigned int worker1id = beginworkerid + 1;
	unsigned int currentLOP1 = currentLOP;
	unsigned int destpartitionoffset1 = destpartitionoffset;
	unsigned int baseaddr1_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton2id = ssdpartitonid;
	unsigned int worker2id = beginworkerid + 2;
	unsigned int currentLOP2 = currentLOP;
	unsigned int destpartitionoffset2 = destpartitionoffset;
	unsigned int baseaddr2_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton3id = ssdpartitonid;
	unsigned int worker3id = beginworkerid + 3;
	unsigned int currentLOP3 = currentLOP;
	unsigned int destpartitionoffset3 = destpartitionoffset;
	unsigned int baseaddr3_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton4id = ssdpartitonid;
	unsigned int worker4id = beginworkerid + 4;
	unsigned int currentLOP4 = currentLOP;
	unsigned int destpartitionoffset4 = destpartitionoffset;
	unsigned int baseaddr4_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton5id = ssdpartitonid;
	unsigned int worker5id = beginworkerid + 5;
	unsigned int currentLOP5 = currentLOP;
	unsigned int destpartitionoffset5 = destpartitionoffset;
	unsigned int baseaddr5_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton6id = ssdpartitonid;
	unsigned int worker6id = beginworkerid + 6;
	unsigned int currentLOP6 = currentLOP;
	unsigned int destpartitionoffset6 = destpartitionoffset;
	unsigned int baseaddr6_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton7id = ssdpartitonid;
	unsigned int worker7id = beginworkerid + 7;
	unsigned int currentLOP7 = currentLOP;
	unsigned int destpartitionoffset7 = destpartitionoffset;
	unsigned int baseaddr7_kvs = baseaddr_kvs;
	
	
titan::write_ddr0_w0_sw0(ssdpartiton0id, worker0id, currentLOP0, destpartitionoffset0, kvbuffer0, kvdram, baseaddr0_kvs);
	
	// titan::write_ddr0_w0_sw0(ssdpartiton0id, worker0id, currentLOP0, destpartitionoffset0, kvbuffer0, kvbufferstats_offset0, kvbufferstats_size0, kvdram, kvdeststats_tmp, baseaddr0_kvs);
	
	
titan::write_ddr0_w0_sw1(ssdpartiton1id, worker1id, currentLOP1, destpartitionoffset1, kvbuffer1, kvdram, baseaddr1_kvs);
	
	// titan::write_ddr0_w0_sw1(ssdpartiton1id, worker1id, currentLOP1, destpartitionoffset1, kvbuffer1, kvbufferstats_offset1, kvbufferstats_size1, kvdram, kvdeststats_tmp, baseaddr1_kvs);
	
	
titan::write_ddr0_w0_sw2(ssdpartiton2id, worker2id, currentLOP2, destpartitionoffset2, kvbuffer2, kvdram, baseaddr2_kvs);
	
	// titan::write_ddr0_w0_sw2(ssdpartiton2id, worker2id, currentLOP2, destpartitionoffset2, kvbuffer2, kvbufferstats_offset2, kvbufferstats_size2, kvdram, kvdeststats_tmp, baseaddr2_kvs);
	
	
titan::write_ddr0_w0_sw3(ssdpartiton3id, worker3id, currentLOP3, destpartitionoffset3, kvbuffer3, kvdram, baseaddr3_kvs);
	
	// titan::write_ddr0_w0_sw3(ssdpartiton3id, worker3id, currentLOP3, destpartitionoffset3, kvbuffer3, kvbufferstats_offset3, kvbufferstats_size3, kvdram, kvdeststats_tmp, baseaddr3_kvs);
	
	
titan::write_ddr0_w0_sw4(ssdpartiton4id, worker4id, currentLOP4, destpartitionoffset4, kvbuffer4, kvdram, baseaddr4_kvs);
	
	// titan::write_ddr0_w0_sw4(ssdpartiton4id, worker4id, currentLOP4, destpartitionoffset4, kvbuffer4, kvbufferstats_offset4, kvbufferstats_size4, kvdram, kvdeststats_tmp, baseaddr4_kvs);
	
	
titan::write_ddr0_w0_sw5(ssdpartiton5id, worker5id, currentLOP5, destpartitionoffset5, kvbuffer5, kvdram, baseaddr5_kvs);
	
	// titan::write_ddr0_w0_sw5(ssdpartiton5id, worker5id, currentLOP5, destpartitionoffset5, kvbuffer5, kvbufferstats_offset5, kvbufferstats_size5, kvdram, kvdeststats_tmp, baseaddr5_kvs);
	
	
titan::write_ddr0_w0_sw6(ssdpartiton6id, worker6id, currentLOP6, destpartitionoffset6, kvbuffer6, kvdram, baseaddr6_kvs);
	
	// titan::write_ddr0_w0_sw6(ssdpartiton6id, worker6id, currentLOP6, destpartitionoffset6, kvbuffer6, kvbufferstats_offset6, kvbufferstats_size6, kvdram, kvdeststats_tmp, baseaddr6_kvs);
	
	
titan::write_ddr0_w0_sw7(ssdpartiton7id, worker7id, currentLOP7, destpartitionoffset7, kvbuffer7, kvdram, baseaddr7_kvs);
	
	// titan::write_ddr0_w0_sw7(ssdpartiton7id, worker7id, currentLOP7, destpartitionoffset7, kvbuffer7, kvbufferstats_offset7, kvbufferstats_size7, kvdram, kvdeststats_tmp, baseaddr7_kvs);
	
	return;
}

void titan::writemanyAU_Vs_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_vs, unsigned int kvreadoffset_kvs
	,uint512_dt verticesbuffer0[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer1[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer2[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer3[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer4[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer5[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer6[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer7[APPLYVERTEXBUFFERSZ_KVS]
	){
titan::writeAU_Vs_ddr0_w0_sw0(ssdpartitonid, (beginworkerid + 0), kvdram, (kvreadoffset_kvs + (0 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer0);
titan::writeAU_Vs_ddr0_w0_sw1(ssdpartitonid, (beginworkerid + 1), kvdram, (kvreadoffset_kvs + (1 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer1);
titan::writeAU_Vs_ddr0_w0_sw2(ssdpartitonid, (beginworkerid + 2), kvdram, (kvreadoffset_kvs + (2 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer2);
titan::writeAU_Vs_ddr0_w0_sw3(ssdpartitonid, (beginworkerid + 3), kvdram, (kvreadoffset_kvs + (3 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer3);
titan::writeAU_Vs_ddr0_w0_sw4(ssdpartitonid, (beginworkerid + 4), kvdram, (kvreadoffset_kvs + (4 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer4);
titan::writeAU_Vs_ddr0_w0_sw5(ssdpartitonid, (beginworkerid + 5), kvdram, (kvreadoffset_kvs + (5 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer5);
titan::writeAU_Vs_ddr0_w0_sw6(ssdpartitonid, (beginworkerid + 6), kvdram, (kvreadoffset_kvs + (6 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer6);
titan::writeAU_Vs_ddr0_w0_sw7(ssdpartitonid, (beginworkerid + 7), kvdram, (kvreadoffset_kvs + (7 * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer7);
	return;
}

void titan::topkernel_instancePU0(
		unsigned int command,
		uint512_dt * kvdram,
		metadata_t * kvstats,
		unsigned int treedepth,
		unsigned int finalnumpartitions,
		unsigned int vbegin,
		unsigned int ssdpartitonid,
		unsigned int paddedkvdatabatchsz_kvs
        ){
	if(command != ON){ return; }
	
	unsigned int maxtreedepthsz = TREEDEPTH_FORSSDPARTITION0; // NB: debugging purposes only
	unsigned int steps = treedepth + 1;
	// unsigned int steps = 2 + 1; // FIXME. CHANGEBACK

	#ifndef _SHARED_SRCBUFFER
	uint512_dt srckvbuffer_ddr0_w0_sw0[KVSRCBUFFERSZ_KVS];
	#endif
	#ifdef _USEURAM_SRCKVBUFFER
	#pragma HLS RESOURCE variable=srckvbuffer_ddr0_w0_sw0 core=XPM_MEMORY uram 
	#endif 
	#ifndef _SHARED_SRCBUFFER
	uint512_dt srckvbuffer_ddr0_w0_sw1[KVSRCBUFFERSZ_KVS];
	#endif
	#ifdef _USEURAM_SRCKVBUFFER
	#pragma HLS RESOURCE variable=srckvbuffer_ddr0_w0_sw1 core=XPM_MEMORY uram 
	#endif 
	#ifndef _SHARED_SRCBUFFER
	uint512_dt srckvbuffer_ddr0_w0_sw2[KVSRCBUFFERSZ_KVS];
	#endif
	#ifdef _USEURAM_SRCKVBUFFER
	#pragma HLS RESOURCE variable=srckvbuffer_ddr0_w0_sw2 core=XPM_MEMORY uram 
	#endif 
	#ifndef _SHARED_SRCBUFFER
	uint512_dt srckvbuffer_ddr0_w0_sw3[KVSRCBUFFERSZ_KVS];
	#endif
	#ifdef _USEURAM_SRCKVBUFFER
	#pragma HLS RESOURCE variable=srckvbuffer_ddr0_w0_sw3 core=XPM_MEMORY uram 
	#endif 
	#ifndef _SHARED_SRCBUFFER
	uint512_dt srckvbuffer_ddr0_w0_sw4[KVSRCBUFFERSZ_KVS];
	#endif
	#ifdef _USEURAM_SRCKVBUFFER
	#pragma HLS RESOURCE variable=srckvbuffer_ddr0_w0_sw4 core=XPM_MEMORY uram 
	#endif 
	#ifndef _SHARED_SRCBUFFER
	uint512_dt srckvbuffer_ddr0_w0_sw5[KVSRCBUFFERSZ_KVS];
	#endif
	#ifdef _USEURAM_SRCKVBUFFER
	#pragma HLS RESOURCE variable=srckvbuffer_ddr0_w0_sw5 core=XPM_MEMORY uram 
	#endif 
	#ifndef _SHARED_SRCBUFFER
	uint512_dt srckvbuffer_ddr0_w0_sw6[KVSRCBUFFERSZ_KVS];
	#endif
	#ifdef _USEURAM_SRCKVBUFFER
	#pragma HLS RESOURCE variable=srckvbuffer_ddr0_w0_sw6 core=XPM_MEMORY uram 
	#endif 
	#ifndef _SHARED_SRCBUFFER
	uint512_dt srckvbuffer_ddr0_w0_sw7[KVSRCBUFFERSZ_KVS];
	#endif
	#ifdef _USEURAM_SRCKVBUFFER
	#pragma HLS RESOURCE variable=srckvbuffer_ddr0_w0_sw7 core=XPM_MEMORY uram 
	#endif 
	
	#ifndef _SHARED_DESTBUFFER
	uint512_dt destkvbuffer_ddr0_w0_sw0[KVDESTBUFFERSZ_KVS]; 
	#endif
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw0
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw0
	#endif
	#ifndef _SHARED_DESTBUFFER
	uint512_dt destkvbuffer_ddr0_w0_sw1[KVDESTBUFFERSZ_KVS]; 
	#endif
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw1
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw1
	#endif
	#ifndef _SHARED_DESTBUFFER
	uint512_dt destkvbuffer_ddr0_w0_sw2[KVDESTBUFFERSZ_KVS]; 
	#endif
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw2
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw2
	#endif
	#ifndef _SHARED_DESTBUFFER
	uint512_dt destkvbuffer_ddr0_w0_sw3[KVDESTBUFFERSZ_KVS]; 
	#endif
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw3
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw3
	#endif
	#ifndef _SHARED_DESTBUFFER
	uint512_dt destkvbuffer_ddr0_w0_sw4[KVDESTBUFFERSZ_KVS]; 
	#endif
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw4
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw4
	#endif
	#ifndef _SHARED_DESTBUFFER
	uint512_dt destkvbuffer_ddr0_w0_sw5[KVDESTBUFFERSZ_KVS]; 
	#endif
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw5
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw5
	#endif
	#ifndef _SHARED_DESTBUFFER
	uint512_dt destkvbuffer_ddr0_w0_sw6[KVDESTBUFFERSZ_KVS]; 
	#endif
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw6
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw6
	#endif
	#ifndef _SHARED_DESTBUFFER
	uint512_dt destkvbuffer_ddr0_w0_sw7[KVDESTBUFFERSZ_KVS]; 
	#endif
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw7
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw7
	#endif
	
	#ifndef _SHARED_DESTBUFFER
	uint512_dt ovkvs_tmp_ddr0_w0_sw0[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw1[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw2[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw3[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw4[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw5[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw6[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw7[NUM_PARTITIONS];
	#endif
	
	// Other structures
	smalluint_t kvbufferstats_offset_w0_sw0[NUM_PARTITIONS];
	smalluint_t kvbufferstats_size_w0_sw0[NUM_PARTITIONS];	
	smallmargin_t ov_w0_sw0[NUM_PARTITIONS];
	smalluint_t kvbufferstats_offset_w0_sw1[NUM_PARTITIONS];
	smalluint_t kvbufferstats_size_w0_sw1[NUM_PARTITIONS];	
	smallmargin_t ov_w0_sw1[NUM_PARTITIONS];
	smalluint_t kvbufferstats_offset_w0_sw2[NUM_PARTITIONS];
	smalluint_t kvbufferstats_size_w0_sw2[NUM_PARTITIONS];	
	smallmargin_t ov_w0_sw2[NUM_PARTITIONS];
	smalluint_t kvbufferstats_offset_w0_sw3[NUM_PARTITIONS];
	smalluint_t kvbufferstats_size_w0_sw3[NUM_PARTITIONS];	
	smallmargin_t ov_w0_sw3[NUM_PARTITIONS];
	smalluint_t kvbufferstats_offset_w0_sw4[NUM_PARTITIONS];
	smalluint_t kvbufferstats_size_w0_sw4[NUM_PARTITIONS];	
	smallmargin_t ov_w0_sw4[NUM_PARTITIONS];
	smalluint_t kvbufferstats_offset_w0_sw5[NUM_PARTITIONS];
	smalluint_t kvbufferstats_size_w0_sw5[NUM_PARTITIONS];	
	smallmargin_t ov_w0_sw5[NUM_PARTITIONS];
	smalluint_t kvbufferstats_offset_w0_sw6[NUM_PARTITIONS];
	smalluint_t kvbufferstats_size_w0_sw6[NUM_PARTITIONS];	
	smallmargin_t ov_w0_sw6[NUM_PARTITIONS];
	smalluint_t kvbufferstats_offset_w0_sw7[NUM_PARTITIONS];
	smalluint_t kvbufferstats_size_w0_sw7[NUM_PARTITIONS];	
	smallmargin_t ov_w0_sw7[NUM_PARTITIONS];

	READ_PARTITION_WRITE_LOOP1: for(unsigned int currentLOP=1; currentLOP<(steps % 8); currentLOP++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=maxtreedepthsz avg=maxtreedepthsz
		#ifdef _DEBUGMODE_PRINTS_TMP
		std::cout<<std::endl;
		std::cout<<"### kernel::topkernel_instancePU0 Started: currentLOP: "<<currentLOP<<" ###"<<std::endl;
		#endif 
		#ifdef _DEBUGMODE_PRINTS
		printkvbuffer_kernel(kvdram, 32, "kernel::topkernel_instancePU0:: printing kvdram");
		#endif
		#ifdef _DEBUGMODE_STATS
		titan_kvpairsread0 = 0;
		titan_kvpairswritten0 = 0;
		titan_kvpairsread0_transfers = 0;
		titan_kvpairswritten0_transfers = 0;
		titan_kvpairswritten_interm2dest_parallel0 = 0;
		titan_kvpairswritten_interm2dest_seqential0 = 0;
		titan_INapplyphase = 0;
		#endif
		
		metadata_t kvdeststats_tmp[NUM_PARTITIONS];
		unsigned int sourcebaseaddr_kvs = 0;
		unsigned int destbaseaddr_kvs = 0;

		INITKVDRAMREACKER_LOOP: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
			#pragma HLS PIPELINE II=1
			kvdeststats_tmp[p].size = 0;
		}
		
		if((currentLOP % 2) == 1){
			sourcebaseaddr_kvs = 0;
			destbaseaddr_kvs = paddedkvdatabatchsz_kvs;
		} else {
			sourcebaseaddr_kvs = paddedkvdatabatchsz_kvs;
			destbaseaddr_kvs = 0;
		}

		unsigned int num_source_partitions = titan::get_num_source_partitions(currentLOP);
		unsigned int nextsourceoffset_partition = 0;
		unsigned int nextdestoffset_partition = 0;
		unsigned int nextsourceoffset_kv = 0;
		unsigned int nextdestoffset_kv = 0;
		unsigned int sourceskipsize_partition = titan::getsourceskipsize(currentLOP, finalnumpartitions);
		unsigned int destskipsize_partition = titan::getdestskipsize(currentLOP, finalnumpartitions);
		unsigned int sourceskipsize_kv = titan::getsourceskipsize(currentLOP, titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid));
		unsigned int destskipsize_kv = titan::getdestskipsize(currentLOP, titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid));
		unsigned int destpartitionrange = (1 << (titan::GETKVDATA_RANGE_PERSSDPARTITION_POW(ssdpartitonid) - (NUM_PARTITIONS_POW * currentLOP)));
		#ifdef _DEBUGMODE_PRINTS_TMP
		print4ints_kernel(">>> kernel::topkernel_instancePU0", "currentLOP", currentLOP, "tree depth", treedepth, "num_source_partitions", num_source_partitions, "number of kvs in each final partition", destskipsize_kv);
		print4ints_kernel(">>> kernel::topkernel_instancePU0", "currentLOP", currentLOP, "tree depth", treedepth, "num_source_partitions", num_source_partitions, "number of destination partitions", (titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / destskipsize_kv));		
		#endif
		#ifdef _DEBUGMODE_PRINTS_TMP
		print4ints_kernel(">>> kernel::topkernel_instancePU0", "currentLOP", currentLOP, "num_source_partitions", num_source_partitions, "tree depth", treedepth, "", 0);
		print4ints_kernel(">>> kernel::topkernel_instancePU0", "sourceskipsize_partition", sourceskipsize_partition, "destskipsize_partition", destskipsize_partition, "sourceskipsize_kv", sourceskipsize_kv, "destskipsize_kv", destskipsize_kv);
		#endif
		
		unsigned int avgnum_source_partitions = (1 << (TREE_DEPTH * NUM_PARTITIONS_POW)); // NB: debugging purposes only
		if(num_source_partitions > finalnumpartitions){ num_source_partitions = 0; } 
		READ_PARTITION_WRITE_LOOP2: for(unsigned int source_partition=0; source_partition<num_source_partitions; source_partition++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=avgnum_source_partitions avg=avgnum_source_partitions
			#ifdef _DEBUGMODE_PRINTS_TMP
			print4ints_kernel("kernel::topkernel_instancePU0", "ssdpartitonid", ssdpartitonid, "source_partition", source_partition, "num_source_partitions", num_source_partitions, "tree depth", titan::GETTREEDEPTH(ssdpartitonid));
			#endif

titan::loaddeststats(kvstats, kvdeststats_tmp, nextdestoffset_partition, currentLOP, finalnumpartitions);
			#ifdef _DEBUGMODE_PRINTS
			printstats_kernel(kvdeststats_tmp, NUM_PARTITIONS, "kernel::kvdeststats_tmp", source_partition, 0, 0);
			print4ints_kernel("kernel::topkernel_instancePU0", "source_partition", source_partition, "num_source_partitions", num_source_partitions, "destpartitionoffset", nextdestoffset_partition, "nextsourceoffset_partition", nextsourceoffset_partition);
			#endif

			INITOV_LOOP_w0_sw01: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
				ov_w0_sw0[p].begin = 0; ov_w0_sw0[p].size = 0;
			}
			INITOV_LOOP_w0_sw11: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
				ov_w0_sw1[p].begin = 0; ov_w0_sw1[p].size = 0;
			}
			INITOV_LOOP_w0_sw21: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
				ov_w0_sw2[p].begin = 0; ov_w0_sw2[p].size = 0;
			}
			INITOV_LOOP_w0_sw31: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
				ov_w0_sw3[p].begin = 0; ov_w0_sw3[p].size = 0;
			}
			INITOV_LOOP_w0_sw41: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
				ov_w0_sw4[p].begin = 0; ov_w0_sw4[p].size = 0;
			}
			INITOV_LOOP_w0_sw51: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
				ov_w0_sw5[p].begin = 0; ov_w0_sw5[p].size = 0;
			}
			INITOV_LOOP_w0_sw61: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
				ov_w0_sw6[p].begin = 0; ov_w0_sw6[p].size = 0;
			}
			INITOV_LOOP_w0_sw71: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
				ov_w0_sw7[p].begin = 0; ov_w0_sw7[p].size = 0;
			}
			
			INITKVDRAMREACKER_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
				#pragma HLS PIPELINE II=1
				kvdeststats_tmp[p].size = 0;
			}
			
			if(nextsourceoffset_partition >= FORCDFINISH_KVSTATS_SIZE){ nextsourceoffset_partition = 0; } 
			metadata_t sourcestat = kvstats[(nextsourceoffset_partition % FORCDFINISH_KVSTATS_SIZE)]; 
			unsigned int begin_kvs = sourcestat.offset / VECTOR_SIZE;
			unsigned int size_kvs = (sourcestat.size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			
			unsigned int alligndownsize_kvs = sourcestat.size / VECTOR_SIZE;
			unsigned int offset_kvs = sourcebaseaddr_kvs + begin_kvs + alligndownsize_kvs;
			unsigned int topk = sourcestat.size % VECTOR_SIZE;
			unsigned int sizek = allignup_KV(sourcestat.size) - sourcestat.size;
			#ifdef _DEBUGMODE_PRINTS
			std::cout<<"kernel::topkernel_instancePU0:: begin_kvs "<<begin_kvs<<", size_kvs: "<<size_kvs<<", begin: "<<(begin_kvs * VECTOR_SIZE)<<", size: "<<(size_kvs * VECTOR_SIZE)<<", (sourcestat.size % VECTOR_SIZE): "<<(sourcestat.size % VECTOR_SIZE)<<endl;
			#endif
			
			unsigned int avgikvs_sz = PADDEDKVDATA_BATCHSIZE_KVS / avgnum_source_partitions; // NB: debugging purposes only
			unsigned int avgtripcnt = avgikvs_sz / (NUMWORKERS * NUMSUBWORKERS * KVBUFFERSZ_KVS); // NB: debugging purposes only			
			unsigned int ikvs_sz = begin_kvs + size_kvs;
			if(ikvs_sz >= paddedkvdatabatchsz_kvs){ begin_kvs = 0; ikvs_sz = 0; } 
			if(begin_kvs > ikvs_sz){ begin_kvs = 0; ikvs_sz = 0; } 
			READ_PARTITION_WRITE_LOOP3: for(unsigned int i_kvs=begin_kvs; i_kvs<ikvs_sz; i_kvs+=(NUMWORKERS * NUMSUBWORKERS * KVBUFFERSZ_KVS)){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=avgtripcnt avg=avgtripcnt
				#ifdef _DEBUGMODE_PRINTS // 
				std::cout<<"kernel::topkernel_instancePU0::Current Iteration:: offset_kvs: "<<i_kvs<<", last offset: "<<ikvs_sz<<", skip size (kvs): "<<(NUMWORKERS * NUMSUBWORKERS * KVBUFFERSZ_KVS)<<", source_partition: "<<source_partition<<", currentLOP: "<<currentLOP<<endl;
				#endif
				
				unsigned int ssdpartiton0id = ssdpartitonid;
				unsigned int beginworker0id = 0 * NUMSUBWORKERS;
				unsigned int baseaddr0_kvs = sourcebaseaddr_kvs;
				unsigned int kvreadoffset0_kvs = i_kvs + (0 * NUMSUBWORKERS * KVBUFFERSZ_KVS);
				unsigned int kvsize0_kvs = begin_kvs + size_kvs;
				unsigned int currentLOP0 = currentLOP;
				unsigned int destpartitionoffset0 = nextdestoffset_partition;
				unsigned int upperlimit0 = vbegin + nextsourceoffset_kv;
				unsigned int destpartitionrange0 = destpartitionrange;
				unsigned int ibegin0_kvs = i_kvs;
				unsigned int iend0_kvs = ikvs_sz;
				unsigned int destbaseaddr0_kvs = destbaseaddr_kvs;
				unsigned int finalnumpartitions0 = finalnumpartitions;
				
					
				read_ddr0_w0_sw0(NAp, kvdram, baseaddr0_kvs, kvreadoffset0_kvs + (0 * KVBUFFERSZ_KVS), kvsize0_kvs, srckvbuffer_ddr0_w0_sw0);
				read_ddr0_w0_sw1(NAp, kvdram, baseaddr0_kvs, kvreadoffset0_kvs + (1 * KVBUFFERSZ_KVS), kvsize0_kvs, srckvbuffer_ddr0_w0_sw1);
				read_ddr0_w0_sw2(NAp, kvdram, baseaddr0_kvs, kvreadoffset0_kvs + (2 * KVBUFFERSZ_KVS), kvsize0_kvs, srckvbuffer_ddr0_w0_sw2);
				read_ddr0_w0_sw3(NAp, kvdram, baseaddr0_kvs, kvreadoffset0_kvs + (3 * KVBUFFERSZ_KVS), kvsize0_kvs, srckvbuffer_ddr0_w0_sw3);
				read_ddr0_w0_sw4(NAp, kvdram, baseaddr0_kvs, kvreadoffset0_kvs + (4 * KVBUFFERSZ_KVS), kvsize0_kvs, srckvbuffer_ddr0_w0_sw4);
				read_ddr0_w0_sw5(NAp, kvdram, baseaddr0_kvs, kvreadoffset0_kvs + (5 * KVBUFFERSZ_KVS), kvsize0_kvs, srckvbuffer_ddr0_w0_sw5);
				read_ddr0_w0_sw6(NAp, kvdram, baseaddr0_kvs, kvreadoffset0_kvs + (6 * KVBUFFERSZ_KVS), kvsize0_kvs, srckvbuffer_ddr0_w0_sw6);
				read_ddr0_w0_sw7(NAp, kvdram, baseaddr0_kvs, kvreadoffset0_kvs + (7 * KVBUFFERSZ_KVS), kvsize0_kvs, srckvbuffer_ddr0_w0_sw7);
				
				#ifdef _WIDEWORD
				upperlimit0 = srckvbuffer_ddr0_w0_sw0[0].range(31, 0) + srckvbuffer_ddr0_w0_sw1[0].range(31, 0) + srckvbuffer_ddr0_w0_sw2[0].range(31, 0) + srckvbuffer_ddr0_w0_sw3[0].range(31, 0);
				#else 
				upperlimit0 = srckvbuffer_ddr0_w0_sw0[0].data[0].key + srckvbuffer_ddr0_w0_sw1[0].data[0].key + srckvbuffer_ddr0_w0_sw2[0].data[0].key + srckvbuffer_ddr0_w0_sw3[0].data[0].key;
				#endif 
				
				/* partitionmany_ddr0_w0(ssdpartiton0id, beginworker0id, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0
					,srckvbuffer_ddr0_w0_sw0, destkvbuffer_ddr0_w0_sw0, kvbufferstats_offset_w0_sw0, kvbufferstats_size_w0_sw0, ov_w0_sw0, ovkvs_tmp_ddr0_w0_sw0
					,srckvbuffer_ddr0_w0_sw1, destkvbuffer_ddr0_w0_sw1, kvbufferstats_offset_w0_sw1, kvbufferstats_size_w0_sw1, ov_w0_sw1, ovkvs_tmp_ddr0_w0_sw1
					,srckvbuffer_ddr0_w0_sw2, destkvbuffer_ddr0_w0_sw2, kvbufferstats_offset_w0_sw2, kvbufferstats_size_w0_sw2, ov_w0_sw2, ovkvs_tmp_ddr0_w0_sw2
					,srckvbuffer_ddr0_w0_sw3, destkvbuffer_ddr0_w0_sw3, kvbufferstats_offset_w0_sw3, kvbufferstats_size_w0_sw3, ov_w0_sw3, ovkvs_tmp_ddr0_w0_sw3
					,srckvbuffer_ddr0_w0_sw4, destkvbuffer_ddr0_w0_sw4, kvbufferstats_offset_w0_sw4, kvbufferstats_size_w0_sw4, ov_w0_sw4, ovkvs_tmp_ddr0_w0_sw4
					,srckvbuffer_ddr0_w0_sw5, destkvbuffer_ddr0_w0_sw5, kvbufferstats_offset_w0_sw5, kvbufferstats_size_w0_sw5, ov_w0_sw5, ovkvs_tmp_ddr0_w0_sw5
					,srckvbuffer_ddr0_w0_sw6, destkvbuffer_ddr0_w0_sw6, kvbufferstats_offset_w0_sw6, kvbufferstats_size_w0_sw6, ov_w0_sw6, ovkvs_tmp_ddr0_w0_sw6
					,srckvbuffer_ddr0_w0_sw7, destkvbuffer_ddr0_w0_sw7, kvbufferstats_offset_w0_sw7, kvbufferstats_size_w0_sw7, ov_w0_sw7, ovkvs_tmp_ddr0_w0_sw7
					); */
					
				partition_ddr0_w0_sw0(ssdpartiton0id, beginworker0id, NAp, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, srckvbuffer_ddr0_w0_sw0, destkvbuffer_ddr0_w0_sw0, kvbufferstats_offset_w0_sw0, kvbufferstats_size_w0_sw0, ov_w0_sw0, ovkvs_tmp_ddr0_w0_sw0);
				partition_ddr0_w0_sw1(ssdpartiton0id, beginworker0id, NAp, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, srckvbuffer_ddr0_w0_sw1, destkvbuffer_ddr0_w0_sw1, kvbufferstats_offset_w0_sw1, kvbufferstats_size_w0_sw1, ov_w0_sw1, ovkvs_tmp_ddr0_w0_sw1);
				partition_ddr0_w0_sw2(ssdpartiton0id, beginworker0id, NAp, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, srckvbuffer_ddr0_w0_sw2, destkvbuffer_ddr0_w0_sw2, kvbufferstats_offset_w0_sw2, kvbufferstats_size_w0_sw2, ov_w0_sw2, ovkvs_tmp_ddr0_w0_sw2);
				partition_ddr0_w0_sw3(ssdpartiton0id, beginworker0id, NAp, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, srckvbuffer_ddr0_w0_sw3, destkvbuffer_ddr0_w0_sw3, kvbufferstats_offset_w0_sw3, kvbufferstats_size_w0_sw3, ov_w0_sw3, ovkvs_tmp_ddr0_w0_sw3);
				partition_ddr0_w0_sw4(ssdpartiton0id, beginworker0id, NAp, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, srckvbuffer_ddr0_w0_sw4, destkvbuffer_ddr0_w0_sw4, kvbufferstats_offset_w0_sw4, kvbufferstats_size_w0_sw4, ov_w0_sw4, ovkvs_tmp_ddr0_w0_sw4);
				partition_ddr0_w0_sw5(ssdpartiton0id, beginworker0id, NAp, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, srckvbuffer_ddr0_w0_sw5, destkvbuffer_ddr0_w0_sw5, kvbufferstats_offset_w0_sw5, kvbufferstats_size_w0_sw5, ov_w0_sw5, ovkvs_tmp_ddr0_w0_sw5);
				partition_ddr0_w0_sw6(ssdpartiton0id, beginworker0id, NAp, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, srckvbuffer_ddr0_w0_sw6, destkvbuffer_ddr0_w0_sw6, kvbufferstats_offset_w0_sw6, kvbufferstats_size_w0_sw6, ov_w0_sw6, ovkvs_tmp_ddr0_w0_sw6);
				partition_ddr0_w0_sw7(ssdpartiton0id, beginworker0id, NAp, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, srckvbuffer_ddr0_w0_sw7, destkvbuffer_ddr0_w0_sw7, kvbufferstats_offset_w0_sw7, kvbufferstats_size_w0_sw7, ov_w0_sw7, ovkvs_tmp_ddr0_w0_sw7);
					
				destbaseaddr0_kvs = destbaseaddr0_kvs + kvbufferstats_offset_w0_sw0[0] + kvbufferstats_offset_w0_sw1[0] + kvbufferstats_offset_w0_sw2[0] + kvbufferstats_offset_w0_sw3[0];
				
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[0].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[0] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[0] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[1].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[1] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[1] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[2].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[2] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[2] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[3].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[3] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[3] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[4].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[4] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[4] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[5].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[5] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[5] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[6].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[6] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[6] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[7].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[7] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[7] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[8].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[8] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[8] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[9].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[9] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[9] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[10].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[10] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[10] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[11].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[11] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[11] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[12].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[12] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[12] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[13].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[13] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[13] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[14].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[14] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[14] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[15].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[15] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[15] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[16].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[16] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[16] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[17].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[17] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[17] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[18].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[18] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[18] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[19].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[19] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[19] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[20].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[20] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[20] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[21].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[21] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[21] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[22].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[22] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[22] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[23].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[23] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[23] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[24].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[24] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[24] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[25].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[25] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[25] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[26].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[26] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[26] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[27].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[27] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[27] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[28].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[28] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[28] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[29].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[29] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[29] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[30].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[30] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[30] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw0(kvdram, destkvbuffer_ddr0_w0_sw0, (destbaseaddr0_kvs + (kvdeststats_tmp[31].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw0[31] / VECTOR_SIZE), (kvbufferstats_size_w0_sw0[31] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[0].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[0] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[0] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[1].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[1] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[1] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[2].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[2] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[2] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[3].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[3] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[3] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[4].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[4] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[4] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[5].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[5] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[5] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[6].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[6] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[6] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[7].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[7] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[7] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[8].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[8] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[8] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[9].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[9] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[9] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[10].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[10] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[10] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[11].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[11] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[11] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[12].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[12] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[12] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[13].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[13] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[13] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[14].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[14] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[14] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[15].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[15] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[15] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[16].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[16] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[16] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[17].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[17] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[17] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[18].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[18] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[18] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[19].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[19] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[19] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[20].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[20] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[20] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[21].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[21] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[21] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[22].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[22] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[22] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[23].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[23] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[23] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[24].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[24] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[24] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[25].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[25] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[25] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[26].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[26] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[26] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[27].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[27] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[27] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[28].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[28] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[28] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[29].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[29] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[29] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[30].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[30] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[30] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw1(kvdram, destkvbuffer_ddr0_w0_sw1, (destbaseaddr0_kvs + (kvdeststats_tmp[31].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw1[31] / VECTOR_SIZE), (kvbufferstats_size_w0_sw1[31] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[0].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[0] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[0] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[1].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[1] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[1] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[2].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[2] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[2] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[3].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[3] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[3] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[4].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[4] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[4] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[5].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[5] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[5] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[6].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[6] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[6] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[7].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[7] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[7] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[8].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[8] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[8] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[9].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[9] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[9] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[10].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[10] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[10] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[11].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[11] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[11] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[12].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[12] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[12] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[13].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[13] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[13] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[14].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[14] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[14] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[15].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[15] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[15] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[16].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[16] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[16] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[17].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[17] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[17] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[18].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[18] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[18] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[19].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[19] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[19] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[20].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[20] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[20] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[21].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[21] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[21] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[22].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[22] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[22] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[23].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[23] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[23] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[24].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[24] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[24] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[25].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[25] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[25] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[26].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[26] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[26] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[27].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[27] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[27] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[28].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[28] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[28] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[29].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[29] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[29] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[30].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[30] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[30] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw2(kvdram, destkvbuffer_ddr0_w0_sw2, (destbaseaddr0_kvs + (kvdeststats_tmp[31].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw2[31] / VECTOR_SIZE), (kvbufferstats_size_w0_sw2[31] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[0].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[0] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[0] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[1].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[1] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[1] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[2].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[2] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[2] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[3].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[3] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[3] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[4].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[4] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[4] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[5].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[5] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[5] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[6].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[6] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[6] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[7].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[7] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[7] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[8].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[8] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[8] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[9].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[9] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[9] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[10].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[10] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[10] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[11].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[11] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[11] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[12].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[12] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[12] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[13].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[13] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[13] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[14].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[14] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[14] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[15].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[15] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[15] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[16].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[16] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[16] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[17].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[17] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[17] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[18].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[18] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[18] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[19].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[19] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[19] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[20].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[20] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[20] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[21].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[21] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[21] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[22].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[22] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[22] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[23].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[23] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[23] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[24].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[24] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[24] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[25].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[25] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[25] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[26].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[26] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[26] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[27].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[27] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[27] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[28].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[28] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[28] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[29].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[29] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[29] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[30].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[30] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[30] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw3(kvdram, destkvbuffer_ddr0_w0_sw3, (destbaseaddr0_kvs + (kvdeststats_tmp[31].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw3[31] / VECTOR_SIZE), (kvbufferstats_size_w0_sw3[31] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[0].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[0] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[0] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[1].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[1] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[1] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[2].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[2] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[2] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[3].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[3] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[3] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[4].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[4] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[4] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[5].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[5] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[5] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[6].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[6] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[6] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[7].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[7] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[7] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[8].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[8] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[8] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[9].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[9] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[9] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[10].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[10] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[10] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[11].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[11] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[11] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[12].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[12] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[12] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[13].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[13] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[13] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[14].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[14] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[14] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[15].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[15] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[15] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[16].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[16] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[16] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[17].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[17] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[17] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[18].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[18] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[18] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[19].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[19] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[19] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[20].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[20] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[20] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[21].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[21] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[21] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[22].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[22] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[22] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[23].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[23] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[23] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[24].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[24] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[24] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[25].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[25] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[25] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[26].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[26] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[26] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[27].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[27] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[27] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[28].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[28] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[28] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[29].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[29] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[29] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[30].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[30] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[30] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw4(kvdram, destkvbuffer_ddr0_w0_sw4, (destbaseaddr0_kvs + (kvdeststats_tmp[31].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw4[31] / VECTOR_SIZE), (kvbufferstats_size_w0_sw4[31] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[0].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[0] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[0] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[1].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[1] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[1] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[2].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[2] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[2] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[3].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[3] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[3] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[4].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[4] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[4] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[5].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[5] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[5] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[6].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[6] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[6] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[7].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[7] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[7] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[8].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[8] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[8] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[9].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[9] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[9] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[10].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[10] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[10] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[11].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[11] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[11] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[12].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[12] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[12] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[13].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[13] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[13] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[14].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[14] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[14] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[15].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[15] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[15] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[16].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[16] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[16] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[17].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[17] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[17] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[18].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[18] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[18] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[19].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[19] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[19] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[20].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[20] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[20] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[21].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[21] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[21] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[22].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[22] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[22] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[23].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[23] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[23] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[24].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[24] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[24] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[25].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[25] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[25] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[26].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[26] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[26] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[27].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[27] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[27] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[28].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[28] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[28] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[29].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[29] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[29] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[30].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[30] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[30] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw5(kvdram, destkvbuffer_ddr0_w0_sw5, (destbaseaddr0_kvs + (kvdeststats_tmp[31].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw5[31] / VECTOR_SIZE), (kvbufferstats_size_w0_sw5[31] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[0].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[0] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[0] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[1].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[1] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[1] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[2].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[2] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[2] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[3].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[3] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[3] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[4].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[4] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[4] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[5].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[5] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[5] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[6].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[6] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[6] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[7].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[7] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[7] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[8].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[8] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[8] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[9].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[9] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[9] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[10].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[10] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[10] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[11].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[11] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[11] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[12].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[12] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[12] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[13].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[13] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[13] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[14].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[14] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[14] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[15].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[15] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[15] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[16].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[16] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[16] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[17].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[17] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[17] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[18].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[18] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[18] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[19].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[19] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[19] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[20].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[20] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[20] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[21].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[21] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[21] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[22].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[22] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[22] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[23].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[23] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[23] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[24].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[24] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[24] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[25].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[25] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[25] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[26].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[26] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[26] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[27].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[27] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[27] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[28].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[28] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[28] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[29].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[29] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[29] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[30].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[30] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[30] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw6(kvdram, destkvbuffer_ddr0_w0_sw6, (destbaseaddr0_kvs + (kvdeststats_tmp[31].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw6[31] / VECTOR_SIZE), (kvbufferstats_size_w0_sw6[31] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[0].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[0] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[0] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[1].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[1] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[1] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[2].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[2] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[2] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[3].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[3] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[3] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[4].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[4] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[4] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[5].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[5] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[5] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[6].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[6] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[6] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[7].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[7] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[7] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[8].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[8] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[8] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[9].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[9] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[9] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[10].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[10] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[10] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[11].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[11] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[11] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[12].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[12] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[12] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[13].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[13] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[13] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[14].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[14] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[14] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[15].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[15] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[15] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[16].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[16] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[16] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[17].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[17] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[17] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[18].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[18] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[18] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[19].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[19] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[19] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[20].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[20] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[20] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[21].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[21] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[21] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[22].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[22] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[22] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[23].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[23] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[23] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[24].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[24] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[24] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[25].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[25] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[25] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[26].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[26] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[26] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[27].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[27] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[27] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[28].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[28] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[28] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[29].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[29] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[29] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[30].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[30] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[30] / VECTOR_SIZE));
				write_fake_ddr0_w0_sw7(kvdram, destkvbuffer_ddr0_w0_sw7, (destbaseaddr0_kvs + (kvdeststats_tmp[31].offset / VECTOR_SIZE)), (kvbufferstats_offset_w0_sw7[31] / VECTOR_SIZE), (kvbufferstats_size_w0_sw7[31] / VECTOR_SIZE));
					
			}
			
			#ifdef _DEBUGMODE_CHECKS
			check_kvdram_kernel(kvdram, destbaseaddr_kvs, kvdeststats_tmp, nextdestoffset_partition, currentLOP, titan::GETTREEDEPTH(ssdpartitonid), finalnumpartitions, titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid));
			#endif
			#ifdef _DEBUGMODE_STATSX
			std::cout<<"### source partition ended: titan_kvpairsread0: "<<titan_kvpairsread0<<", titan_kvpairswritten0: "<<titan_kvpairswritten0<<" ###"<<std::endl;
			#endif
titan::storedeststats(kvstats, kvdeststats_tmp, nextdestoffset_partition, currentLOP, finalnumpartitions);
			nextsourceoffset_partition += sourceskipsize_partition;
			nextdestoffset_partition += (destskipsize_partition * NUM_PARTITIONS);
			nextsourceoffset_kv += sourceskipsize_kv;
			nextdestoffset_kv += destskipsize_kv;
		}
		
		#ifdef _DEBUGMODE_STATS
		std::cout<<">>>"<<endl;
		std::cout<<"### topkernel_instancePU::Summary:: KV PAIRS READ (DRAM A): "<<titan_kvpairsread0<<" ("<<titan_kvpairsread0_transfers<<" TRANSFERS)"<<endl;		
		std::cout<<"### topkernel_instancePU::Summary:: KV PAIRS WRITTEN (DRAM A): "<<titan_kvpairswritten0<<" ("<<titan_kvpairswritten0_transfers<<" TRANSFERS)"<<endl;
		std::cout<<std::endl;
		#endif
	}
	return;
}

void titan::topkernel_instanceAU0(
		unsigned int command,
		unsigned int IterCount,
		uint512_dt * kvdram,
		metadata_t * kvstats,
		unsigned int treedepth,
		unsigned int finalnumpartitions,
		unsigned int vbegin,
		unsigned int ssdpartitonid,
		unsigned int paddedkvdatabatchsz_kvs
		
		#ifdef _SHARED_SRCBUFFER
		,uint512_dt srckvbuffer_ddr0_w0_sw0[KVSRCBUFFERSZ_KVS]
		,uint512_dt srckvbuffer_ddr0_w0_sw1[KVSRCBUFFERSZ_KVS]
		,uint512_dt srckvbuffer_ddr0_w0_sw2[KVSRCBUFFERSZ_KVS]
		,uint512_dt srckvbuffer_ddr0_w0_sw3[KVSRCBUFFERSZ_KVS]
		,uint512_dt srckvbuffer_ddr0_w0_sw4[KVSRCBUFFERSZ_KVS]
		,uint512_dt srckvbuffer_ddr0_w0_sw5[KVSRCBUFFERSZ_KVS]
		,uint512_dt srckvbuffer_ddr0_w0_sw6[KVSRCBUFFERSZ_KVS]
		,uint512_dt srckvbuffer_ddr0_w0_sw7[KVSRCBUFFERSZ_KVS]
		#endif
		#ifdef _SHARED_DESTBUFFER
		,uint512_dt destkvbuffer_ddr0_w0_sw0[KVDESTBUFFERSZ_KVS]
		,uint512_dt destkvbuffer_ddr0_w0_sw1[KVDESTBUFFERSZ_KVS]
		,uint512_dt destkvbuffer_ddr0_w0_sw2[KVDESTBUFFERSZ_KVS]
		,uint512_dt destkvbuffer_ddr0_w0_sw3[KVDESTBUFFERSZ_KVS]
		,uint512_dt destkvbuffer_ddr0_w0_sw4[KVDESTBUFFERSZ_KVS]
		,uint512_dt destkvbuffer_ddr0_w0_sw5[KVDESTBUFFERSZ_KVS]
		,uint512_dt destkvbuffer_ddr0_w0_sw6[KVDESTBUFFERSZ_KVS]
		,uint512_dt destkvbuffer_ddr0_w0_sw7[KVDESTBUFFERSZ_KVS]
		#endif
		#ifdef _SHARED_DESTBUFFER
		,uint512_dt ovkvs_tmp_ddr0_w0_sw0[NUM_PARTITIONS]
		,uint512_dt ovkvs_tmp_ddr0_w0_sw1[NUM_PARTITIONS]
		,uint512_dt ovkvs_tmp_ddr0_w0_sw2[NUM_PARTITIONS]
		,uint512_dt ovkvs_tmp_ddr0_w0_sw3[NUM_PARTITIONS]
		,uint512_dt ovkvs_tmp_ddr0_w0_sw4[NUM_PARTITIONS]
		,uint512_dt ovkvs_tmp_ddr0_w0_sw5[NUM_PARTITIONS]
		,uint512_dt ovkvs_tmp_ddr0_w0_sw6[NUM_PARTITIONS]
		,uint512_dt ovkvs_tmp_ddr0_w0_sw7[NUM_PARTITIONS]
		#endif
        ){
	if(command != ON){ return; }

	unsigned int currentLOP = treedepth + 1;
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<std::endl;
	std::cout<<"### kernel::topkernel_instanceAU0 Started: currentLOP: "<<currentLOP<<" ###"<<std::endl;
	printkvbuffer_kernel(kvdram, 32, "kernel::topkernel_instanceAU0:: printing kvdram");
	#endif
	#ifdef _DEBUGMODE_STATS
	titan_kvpairsread0 = 0;
	titan_kvpairswritten0 = 0;
	titan_INapplyphase = 1;
	#endif

	unsigned int sourcebaseaddr_kvs = 0;
	unsigned int destbaseaddr_vs = 0;
	if((treedepth % 2) == 1){
		sourcebaseaddr_kvs = paddedkvdatabatchsz_kvs;
	} else {
		sourcebaseaddr_kvs = 0;
	}
	destbaseaddr_vs = 2 * paddedkvdatabatchsz_kvs; // third part houses the vertices 
	
	// src kvbuffer
	#ifndef _SHARED_SRCBUFFER
	uint512_dt srckvbuffer_ddr0_w0_sw0[KVSRCBUFFERSZ_APPLY_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw0
	#endif
	uint512_dt srckvbuffer_ddr0_w0_sw1[KVSRCBUFFERSZ_APPLY_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw1
	#endif
	uint512_dt srckvbuffer_ddr0_w0_sw2[KVSRCBUFFERSZ_APPLY_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw2
	#endif
	uint512_dt srckvbuffer_ddr0_w0_sw3[KVSRCBUFFERSZ_APPLY_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw3
	#endif
	uint512_dt srckvbuffer_ddr0_w0_sw4[KVSRCBUFFERSZ_APPLY_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw4
	#endif
	uint512_dt srckvbuffer_ddr0_w0_sw5[KVSRCBUFFERSZ_APPLY_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw5
	#endif
	uint512_dt srckvbuffer_ddr0_w0_sw6[KVSRCBUFFERSZ_APPLY_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw6
	#endif
	uint512_dt srckvbuffer_ddr0_w0_sw7[KVSRCBUFFERSZ_APPLY_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=srckvbuffer_ddr0_w0_sw7
	#endif
	#endif
	
	#ifndef _SHARED_DESTBUFFER
	uint512_dt destkvbuffer_ddr0_w0_sw0[APPLYVERTEXBUFFERSZ_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw0
	/// #pragma HLS RESOURCE variable=destkvbuffer_ddr0_w0_sw0 core=XPM_MEMORY uram // utilizes URAM
	#endif
	uint512_dt destkvbuffer_ddr0_w0_sw1[APPLYVERTEXBUFFERSZ_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw1
	/// #pragma HLS RESOURCE variable=destkvbuffer_ddr0_w0_sw1 core=XPM_MEMORY uram // utilizes URAM
	#endif
	uint512_dt destkvbuffer_ddr0_w0_sw2[APPLYVERTEXBUFFERSZ_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw2
	/// #pragma HLS RESOURCE variable=destkvbuffer_ddr0_w0_sw2 core=XPM_MEMORY uram // utilizes URAM
	#endif
	uint512_dt destkvbuffer_ddr0_w0_sw3[APPLYVERTEXBUFFERSZ_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw3
	/// #pragma HLS RESOURCE variable=destkvbuffer_ddr0_w0_sw3 core=XPM_MEMORY uram // utilizes URAM
	#endif
	uint512_dt destkvbuffer_ddr0_w0_sw4[APPLYVERTEXBUFFERSZ_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw4
	/// #pragma HLS RESOURCE variable=destkvbuffer_ddr0_w0_sw4 core=XPM_MEMORY uram // utilizes URAM
	#endif
	uint512_dt destkvbuffer_ddr0_w0_sw5[APPLYVERTEXBUFFERSZ_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw5
	/// #pragma HLS RESOURCE variable=destkvbuffer_ddr0_w0_sw5 core=XPM_MEMORY uram // utilizes URAM
	#endif
	uint512_dt destkvbuffer_ddr0_w0_sw6[APPLYVERTEXBUFFERSZ_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw6
	/// #pragma HLS RESOURCE variable=destkvbuffer_ddr0_w0_sw6 core=XPM_MEMORY uram // utilizes URAM
	#endif
	uint512_dt destkvbuffer_ddr0_w0_sw7[APPLYVERTEXBUFFERSZ_KVS];
	#ifndef _WIDEWORD
	#pragma HLS DATA_PACK variable=destkvbuffer_ddr0_w0_sw7
	/// #pragma HLS RESOURCE variable=destkvbuffer_ddr0_w0_sw7 core=XPM_MEMORY uram // utilizes URAM
	#endif
	#endif
	
	unsigned int num_source_partitions = titan::get_num_source_partitions(currentLOP);
	#ifdef _DEBUGMODE_PRINTS_TMP
	print4ints_kernel("kernel::kernel", "currentLOP", currentLOP, "num_source_partitions", num_source_partitions, "tree depth", treedepth, "vbegin", vbegin);
	#endif

	READ_PARTITION_WRITE_LOOP1: for(unsigned int source_partition=0; source_partition<num_source_partitions; source_partition+=NUMSUBWORKERS_APPLYPH){
		#ifdef _DEBUGMODE_PRINTS_TMP
		print4ints_kernel("kernel::topkernel_instanceAU0", "ssdpartitonid", ssdpartitonid, "source_partition", source_partition, "num_source_partitions", num_source_partitions, "skip size", NUMSUBWORKERS_APPLYPH);
		#endif

		metadata_t sourcestatbuffer[NUMSUBWORKERS_APPLYPH];
		for(unsigned int i=0; i<NUMSUBWORKERS_APPLYPH; i++){
			sourcestatbuffer[i] = kvstats[((source_partition + i) % FORCDFINISH_KVSTATS_SIZE)]; 
			#ifdef _DEBUGMODE_PRINTS
			cout<<">>> sourcestatbuffer["<<i<<"].offset: "<<sourcestatbuffer[i].offset<<", kvstats["<<(source_partition + i)<<"].offset: "<<kvstats[(source_partition + i)].offset<<", kvstats["<<(source_partition + i)<<"].size: "<<kvstats[(source_partition + i)].size<<endl;
			#endif
		}
		#ifdef _DEBUGMODE_PRINTS
		for(unsigned int i=0; i<NUMSUBWORKERS_APPLYPH; i++){ std::cout<<"kernel::topkernel_instanceAU0:: begin_kvs[i] "<<(sourcestatbuffer[i].offset / VECTOR_SIZE)<<", size_kvs[i]: "<<(sourcestatbuffer[i].size / VECTOR_SIZE)<<endl; }
		#endif

		#ifdef _DEBUGMODE_PRINTS
		std::cout<<"kernel::topkernel_instanceAU0:: next read offset: "<<(source_partition * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid))<<endl;
		#endif
		/** [NUMSUBWORKERS_APPLYPH] consecutive locations are loaded into each verticesbuffer */
		readmanyAU_Vs_ddr0_w0(ssdpartitonid, (0 * NUMSUBWORKERS_APPLYPH), kvdram, destbaseaddr_vs, (source_partition * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)), titan::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) /// FIXME KVDATA_BATCHSIZE?
				,destkvbuffer_ddr0_w0_sw0
				,destkvbuffer_ddr0_w0_sw1
				,destkvbuffer_ddr0_w0_sw2
				,destkvbuffer_ddr0_w0_sw3
				,destkvbuffer_ddr0_w0_sw4
				,destkvbuffer_ddr0_w0_sw5
				,destkvbuffer_ddr0_w0_sw6
				,destkvbuffer_ddr0_w0_sw7
				);
		
		unsigned int maxsz = 0;
		for(unsigned int sw=0; sw<NUMSUBWORKERS_APPLYPH; sw++){ maxsz = max(sourcestatbuffer[sw].size, maxsz); } // get max number of iterations
		unsigned int max_ikvs = ceildiv(maxsz, (NUMWORKERS_APPLYPH * KVSRCBUFFERSZ_APPLY));

		READ_PARTITION_WRITE_LOOP2: for(unsigned int i=0; i<max_ikvs; i++){
			unsigned int i_kvs = i * NUMWORKERS_APPLYPH * KVSRCBUFFERSZ_APPLY_KVS;
			#ifdef _DEBUGMODE_PRINTS
			std::cout<<"kernel::topkernel_instanceAU0::Current Iteration:: i: "<<i<<", i_kvs: "<<i_kvs<<", max_ikvs: "<<max_ikvs<<", skip size (kvs): "<<(NUMWORKERS_APPLYPH * NUMSUBWORKERS_APPLYPH * KVBUFFERSZ_KVS)<<endl;
			#endif
		
			/** Each subworker is assigned to a last-level partition. */
			unsigned int ssdpartiton0id = ssdpartitonid; 
			unsigned int beginworker0id = 0 * NUMSUBWORKERS_APPLYPH; 
			unsigned int verticesreadoffset0_vs = ((vbegin / VECTOR_SIZE) + (source_partition * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))); 
			unsigned int kvreadoffset0_kvs = i_kvs + (0 * KVSRCBUFFERSZ_APPLY_KVS);
			unsigned int sourcebaseaddr0_kvs = sourcebaseaddr_kvs;			
			
			readmanyAU_VUs_ddr0_w0(beginworker0id, kvdram, sourcebaseaddr0_kvs, sourcestatbuffer, kvreadoffset0_kvs, PADDEDKVDATA_BATCHSIZE /// FIXME KVDATA_BATCHSIZE or PADDEDKVDATA_BATCHSIZE?
				,srckvbuffer_ddr0_w0_sw0
				,srckvbuffer_ddr0_w0_sw1
				,srckvbuffer_ddr0_w0_sw2
				,srckvbuffer_ddr0_w0_sw3
				,srckvbuffer_ddr0_w0_sw4
				,srckvbuffer_ddr0_w0_sw5
				,srckvbuffer_ddr0_w0_sw6
				,srckvbuffer_ddr0_w0_sw7
				);		
			
			/** Each subworker's key-values' is committed to its vertices' */
			reducemany_ddr0_w0(IterCount, ssdpartitonid, beginworker0id, verticesreadoffset0_vs, sourcestatbuffer, kvreadoffset0_kvs 
				,destkvbuffer_ddr0_w0_sw0
				,destkvbuffer_ddr0_w0_sw1
				,destkvbuffer_ddr0_w0_sw2
				,destkvbuffer_ddr0_w0_sw3
				,destkvbuffer_ddr0_w0_sw4
				,destkvbuffer_ddr0_w0_sw5
				,destkvbuffer_ddr0_w0_sw6
				,destkvbuffer_ddr0_w0_sw7
				,srckvbuffer_ddr0_w0_sw0
				,srckvbuffer_ddr0_w0_sw1
				,srckvbuffer_ddr0_w0_sw2
				,srckvbuffer_ddr0_w0_sw3
				,srckvbuffer_ddr0_w0_sw4
				,srckvbuffer_ddr0_w0_sw5
				,srckvbuffer_ddr0_w0_sw6
				,srckvbuffer_ddr0_w0_sw7
				);	
		}

		#ifdef _DEBUGMODE_PRINTS
		std::cout<<"kernel::topkernel_instanceAU0:: next write offset: "<<(source_partition * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))<<endl;
		#endif
		writemanyAU_Vs_ddr0_w0(ssdpartitonid, (0 * NUMSUBWORKERS_APPLYPH), kvdram, destbaseaddr_vs, (source_partition * titan::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))
				,destkvbuffer_ddr0_w0_sw0
				,destkvbuffer_ddr0_w0_sw1
				,destkvbuffer_ddr0_w0_sw2
				,destkvbuffer_ddr0_w0_sw3
				,destkvbuffer_ddr0_w0_sw4
				,destkvbuffer_ddr0_w0_sw5
				,destkvbuffer_ddr0_w0_sw6
				,destkvbuffer_ddr0_w0_sw7
				);
	}
	#ifdef _DEBUGMODE_STATS
	std::cout<<"### topkernel_instanceAU ended::Summary:: titan_kvpairsread0: "<<titan_kvpairsread0<<", titan_kvpairswritten0: "<<titan_kvpairswritten0<<" ########"<<std::endl<<std::endl;
	#endif
	return;
}

extern "C" {
void titan::topkernel(
uint512_dt * kvdramA
		,metadata_t * kvstatsA
		,int * messagesA
        ){
#pragma HLS INTERFACE m_axi port=kvdramA  offset=slave bundle=gmem0 max_read_burst_length=128 max_write_burst_length=128
#pragma HLS INTERFACE m_axi port=kvstatsA  offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=messagesA  offset=slave bundle=gmem2

#pragma HLS INTERFACE s_axilite port=kvdramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvstatsA  bundle=control
#pragma HLS INTERFACE s_axilite port=messagesA bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifndef _WIDEWORD
#pragma HLS DATA_PACK variable=kvdramA
#endif 
#pragma HLS DATA_PACK variable=kvstatsA

	#if defined(_DEBUGMODE_PRINTS_TMP) || defined(_DEBUGMODE_STATS)
	std::cout<<std::endl;
	std::cout<<"Kernel Started: messagesA[MESSAGES_KVDATASIZEID]: "<<messagesA[MESSAGES_KVDATASIZEID]<<std::endl;
	std::cout<<"Kernel Started: final number of partitions: "<<messagesA[MESSAGES_FINALNUMPARTITIONSID]<<std::endl;
	std::cout<<"Kernel Started: messagesA[MESSAGES_COMMANDID]: "<<messagesA[MESSAGES_COMMANDID]<<std::endl;
	std::cout<<"Kernel Started: messagesA[MESSAGES_ITERATIONID]: "<<messagesA[MESSAGES_ITERATIONID]<<std::endl;
	std::cout<<"Kernel Started: messagesA[MESSAGES_SSDPARTITIONID]: "<<messagesA[MESSAGES_SSDPARTITIONID]<<std::endl;
	std::cout<<"Kernel Started: messagesA[DRAM_VOFFSET]: "<<messagesA[DRAM_VOFFSET]<<std::endl;
	std::cout<<"Kernel Started: messagesA[DRAM_VSIZE]: "<<messagesA[DRAM_VSIZE]<<std::endl;
	std::cout<<"Kernel Started: messagesA[DRAM_TREEDEPTH]: "<<messagesA[DRAM_TREEDEPTH]<<std::endl;
	std::cout<<"Kernel Started: messagesA[DRAM_FINALNUMPARTITIONS]: "<<messagesA[DRAM_FINALNUMPARTITIONS]<<std::endl;
	std::cout<<"Kernel Started: messagesA[MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID]: "<<messagesA[MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID]<<std::endl;
	
	#endif
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer(kvdram0, 32, "kernel.kvdramA");
	#endif
	
	uint512_dt ovkvs_tmp_ddr0_w0_sw0[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw1[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw2[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw3[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw4[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw5[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw6[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw7[NUM_PARTITIONS];
	
	
	unsigned int dramA_processcommand = messagesA[(MESSAGES_PROCESSCOMMANDID )];
	unsigned int dramA_partitioncommand = messagesA[(MESSAGES_PARTITIONCOMMANDID )];
	unsigned int dramA_reducecommand = messagesA[(MESSAGES_APPLYUPDATESCOMMANDID )];
	unsigned int dramA_iteration_idx = messagesA[(MESSAGES_ITERATIONID)]; 
	unsigned int dramA_ssdpartitonid = messagesA[(MESSAGES_SSDPARTITIONID)];
	unsigned int dramA_vbegin = messagesA[(DRAM_VOFFSET)]; 
	unsigned int dramA_vsize = messagesA[(DRAM_VSIZE)]; 
	unsigned int dramA_treedepth = messagesA[(DRAM_TREEDEPTH)]; 
	unsigned int dramA_finalnumpartitions = messagesA[(DRAM_FINALNUMPARTITIONS)]; 
	unsigned int dramA_paddedkvdatabatchszkvs = messagesA[MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID];
	unsigned int dramA_IterCount = messagesA[GRAPH_ITERATIONID];
	
	#ifndef FPGA_IMPL
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#endif
	
	topkernel_instancePU0(
		dramA_partitioncommand,
		kvdramA,
		kvstatsA,
		dramA_treedepth,
		dramA_finalnumpartitions,
		dramA_vbegin,
		dramA_ssdpartitonid,
		dramA_paddedkvdatabatchszkvs
		);
	// return;
		
	/** topkernel_instanceAU0(
		dramA_reducecommand,
		dramA_IterCount,
		kvdramA,
		kvstatsA,
		dramA_treedepth,
		dramA_finalnumpartitions,
		dramA_vbegin,
		dramA_ssdpartitonid,
		dramA_paddedkvdatabatchszkvs
		#ifdef _SHARED_SRCBUFFER
		,srckvbuffer_ddr0_w0_sw0
		,srckvbuffer_ddr0_w0_sw1
		,srckvbuffer_ddr0_w0_sw2
		,srckvbuffer_ddr0_w0_sw3
		,srckvbuffer_ddr0_w0_sw4
		,srckvbuffer_ddr0_w0_sw5
		,srckvbuffer_ddr0_w0_sw6
		,srckvbuffer_ddr0_w0_sw7
		#endif 
		#ifdef _SHARED_DESTBUFFER
		,destkvbuffer_ddr0_w0_sw0
		,destkvbuffer_ddr0_w0_sw1
		,destkvbuffer_ddr0_w0_sw2
		,destkvbuffer_ddr0_w0_sw3
		,destkvbuffer_ddr0_w0_sw4
		,destkvbuffer_ddr0_w0_sw5
		,destkvbuffer_ddr0_w0_sw6
		,destkvbuffer_ddr0_w0_sw7
		#endif 
		#ifdef _SHARED_DESTBUFFER
		,ovkvs_tmp_ddr0_w0_sw0
		,ovkvs_tmp_ddr0_w0_sw1
		,ovkvs_tmp_ddr0_w0_sw2
		,ovkvs_tmp_ddr0_w0_sw3
		,ovkvs_tmp_ddr0_w0_sw4
		,ovkvs_tmp_ddr0_w0_sw5
		,ovkvs_tmp_ddr0_w0_sw6
		,ovkvs_tmp_ddr0_w0_sw7
		#endif
		); */
	// return;
	
	// cout<<"FINISHED kernel run..."<<endl;
	#ifndef FPGA_IMPL
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Kernel Finished: Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[nanao seconds]" << std::endl;
	std::cout << "Kernel Finished: Time difference = " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << "[micro seconds]" << std::endl;
	std::cout << "Kernel Finished: Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[milli seconds]" << std::endl;
	std::cout << "Kernel Finished: Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[seconds]" << std::endl;
	std::cout << std::endl;
	#endif 
	#endif 
	return;
}
}
