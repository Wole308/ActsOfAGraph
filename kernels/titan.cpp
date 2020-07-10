#include "/home/oj2zf/Documents/ActsOfAGraph/src/common.h"
#include "/home/oj2zf/Documents/ActsOfAGraph/kernels/enigma.h"
#include "/home/oj2zf/Documents/ActsOfAGraph/kernels/titan.h"
using namespace std;

#define DRAMLIMITSZ (1 << 26)
#define DRAMLIMITSZ_KVS (FORCDFINISH_DRAMSZ / VECTOR_SIZE)

#ifndef FPGA_IMPL
enigma::enigma(){}
enigma::~enigma(){}
#endif
#ifndef HW

unsigned int globalvar_totalkvstatsread;
unsigned int globalvar_totalkvsreadV;

unsigned int globalvar_totalkvstatscollected;
unsigned int globalvar_totalkvstatswritten;

unsigned int globalvar_totalkvsread;

unsigned int globalstats_totalkvspartitioned;
unsigned int globalstats_totalkvspartitionswritten;

unsigned int globalstats_totalkvsreduced;
unsigned int globalstats_totalkvsreducewritten;
#endif 
#ifdef _LDEBUGMODE_HEADER
void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound){
	if(data > upper_bound){ std::cout<<"enigma::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<std::endl; exit(EXIT_FAILURE); }
}
void checkforequal(string message, unsigned int data1, unsigned int data2){
	if(data1 == data2){ cout<<"enigma::checkforequal: ERROR. data1 == data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
}
void print1(string messagea, unsigned int dataa){
	cout<<messagea<<": "<<dataa<<endl;
}
void print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<endl;
}
void print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<endl;
}
void print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef){
	cout<<messagea<<": "<<dataa<<", "<<messageb<<": "<<datab<<", "<<messagec<<": "<<datac<<", "<<messaged<<": "<<datad<<", "<<messagee<<": "<<datae<<", "<<messagef<<": "<<datef<<endl;
}
void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; }
}
void printkeyvalues(string message, keyvalue_t * keyvalues1, keyvalue_t * keyvalues2, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"["<<keyvalues1[p].key<<":"<<keyvalues1[p].value<<"]["<<keyvalues2[p].key<<":"<<keyvalues2[p].value<<"]"<<endl; }
}
void printkeyvalues(string message, uint512_dt * keyvalues, unsigned int size_kvs){
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
void getvaluecount(string message, keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	cout<<"total values counted: "<<totalnumkeyvalues<<endl;
}
void countkeyvalues(){

}
void printparameters(){
	cout<<endl<<"enigma::printparameters: test started."<<endl;
	cout<<"enigma::printparameters:: KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
	cout<<"enigma::printparameters:: KVDATA_BATCHSIZE_KVS: "<<KVDATA_BATCHSIZE_KVS<<endl;
	
	cout<<"enigma::printparameters:: PADDEDKVDATA_BATCHSIZE: "<<PADDEDKVDATA_BATCHSIZE<<endl;
	cout<<"enigma::printparameters:: PADDEDKVDATA_BATCHSIZE_KVS: "<<PADDEDKVDATA_BATCHSIZE_KVS<<endl;
	
	cout<<"enigma::printparameters:: BUFFER_SIZE: "<<BUFFER_SIZE<<endl;
	cout<<"enigma::printparameters:: PADDEDBUFFER_SIZE: "<<PADDEDBUFFER_SIZE<<endl;
	
	cout<<"enigma::printparameters:: KVDRAMSZ: "<<KVDRAMSZ<<endl;
	cout<<"enigma::printparameters:: KVDRAMSZ_KVS: "<<KVDRAMSZ_KVS<<endl;
	
	cout<<"enigma::printparameters:: KVSOURCEDRAMSZ: "<<KVSOURCEDRAMSZ<<endl;
	cout<<"enigma::printparameters:: KVSOURCEDRAMSZ_KVS: "<<KVSOURCEDRAMSZ_KVS<<endl;
	
	cout<<"enigma::printparameters:: PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;
	cout<<"enigma::printparameters:: PADDEDKVSOURCEDRAMSZ_KVS: "<<PADDEDKVSOURCEDRAMSZ_KVS<<endl;
}
#endif
#ifdef _DEBUGMODE_STATS
void printglobalvars(){
	cout<<"printglobalvars: "<<endl;
	cout<<"enigma::printglobalvars:: COLLECTSTATS & SPREAD & PARTITION:: globalvar_totalkvsread: "<<globalvar_totalkvsread<<endl;
	cout<<"enigma::printglobalvars:: COLLECTSTATS & SPREAD & PARTITION:: globalvar_totalkvsreadV: "<<globalvar_totalkvsreadV<<endl;
	
	cout<<"enigma::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatsread: "<<globalvar_totalkvstatsread<<endl;
	cout<<"enigma::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatscollected: "<<globalvar_totalkvstatscollected<<endl;
	cout<<"enigma::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatswritten: "<<globalvar_totalkvstatswritten<<endl;	

	cout<<"enigma::printglobalvars:: SPREAD:: globalstats_totalkvspartitioned: "<<globalstats_totalkvspartitioned<<endl;
	cout<<"enigma::printglobalvars:: SPREAD:: globalstats_totalkvspartitionswritten: "<<globalstats_totalkvspartitionswritten<<endl;
	
	cout<<"enigma::printglobalvars:: REDUCE:: globalstats_totalkvsreduced: "<<globalstats_totalkvsreduced<<endl;
	cout<<"enigma::printglobalvars:: REDUCE:: globalstats_totalkvsreducewritten: "<<globalstats_totalkvsreducewritten<<endl;
}
void clearglobalvars(){
	cout<<"clearglobalvars: "<<endl;
	globalvar_totalkvsread = 0;
	globalstats_totalkvspartitionswritten = 0;
	globalstats_totalkvspartitioned = 0;
	globalvar_totalkvstatsread = 0;
	globalvar_totalkvstatscollected = 0;
	globalvar_totalkvstatswritten = 0;
	globalstats_totalkvsreduced = 0;
	globalstats_totalkvsreducewritten = 0;
	globalvar_totalkvsreadV = 0;
}
void globalstats_countkvstatsread(unsigned int count){
	globalvar_totalkvstatsread += count;
	return;
}
void globalstats_countkvstatscollected(unsigned int count){
	globalvar_totalkvstatscollected += count;
	return;
}
void globalstats_countkvstatswritten(unsigned int count){
	globalvar_totalkvstatswritten += count;
	return;
}
void globalstats_countkvsread(unsigned int count){
	globalvar_totalkvsread += count;
	return;
}
void globalstats_countkvspartitionswritten(unsigned int count){
	globalstats_totalkvspartitionswritten += count;
	return;
}
void globalstats_countkvspartitioned(unsigned int count){
	globalstats_totalkvspartitioned += count;
	return;
}
void globalstats_countkvsreduced(unsigned int count){
	globalstats_totalkvsreduced += count;
	return;
}
void globalstats_countkvsreducewritten(unsigned int count){
	globalstats_totalkvsreducewritten += count;
	return;
}
void globalstats_countkvsreadV(unsigned int count){
	globalvar_totalkvsreadV += count;
	return;
}
#endif 

unsigned int allignhigher_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
unsigned int allignlower_KV(unsigned int val){
	unsigned int fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
unsigned int ceildiv(unsigned int val1, unsigned int val2){
	return (val1 + (val2 - 1)) / val2;
}	
unsigned int max(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}
unsigned int min(unsigned int A, unsigned int B){
	#pragma HLS INLINE
	if(A<B){ return A; }
	else { return B; }
}
unsigned int hsub(unsigned int A, unsigned int B){
	#pragma HLS INLINE
	if(A < B){ return 0; }
	else { return A - B; }
}

unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	// return 0;
	partition_type hashedval = 0;
	hashedval = ((keyvalue.key - upperlimit) >> (KVDATA_RANGE_PERSSDPARTITION_POW - (NUM_PARTITIONS_POW * currentLOP)));
	
	int KKK = (((1 << NUM_PARTITIONS_POW) - 1) & (hashedval >> (1 - 1)));
	// if(KKK >= NUM_PARTITIONS){ cout<<"--- getpartition: KKK: "<<KKK<<endl; exit(EXIT_FAILURE); }
	
	
	// return (hashedval % NUM_PARTITIONS); // REMOVEME.
	return (((1 << NUM_PARTITIONS_POW) - 1) & (hashedval >> (1 - 1))); // REMOVEME.
	// if(hashedval >= NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}
keyvalue_t getkeyvalue(uint512_dt buffer[BUFFER_SIZE], batch_type addr, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	keyvalue_t keyvalue;
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	if(idy >= maxaddr_kvs){ cout<<"getkeyvalue:: ERROR 45. addr: "<<addr<<", idy: "<<idy<<", idx: "<<idx<<endl; exit(EXIT_FAILURE); }
	checkoutofbounds("enigma::getkeyvalue 35", idy, maxaddr_kvs);
	#endif
	
	#ifdef _WIDEWORD
if(idx==0){	
		keyvalue.key = buffer[idy].range(31, 0);
		keyvalue.value = buffer[idy].range(63, 32);
	}
else if(idx==1){	
		keyvalue.key = buffer[idy].range(95, 64);
		keyvalue.value = buffer[idy].range(127, 96);
	}
else if(idx==2){	
		keyvalue.key = buffer[idy].range(159, 128);
		keyvalue.value = buffer[idy].range(191, 160);
	}
else if(idx==3){	
		keyvalue.key = buffer[idy].range(223, 192);
		keyvalue.value = buffer[idy].range(255, 224);
	}
else if(idx==4){	
		keyvalue.key = buffer[idy].range(287, 256);
		keyvalue.value = buffer[idy].range(319, 288);
	}
else if(idx==5){	
		keyvalue.key = buffer[idy].range(351, 320);
		keyvalue.value = buffer[idy].range(383, 352);
	}
else if(idx==6){	
		keyvalue.key = buffer[idy].range(415, 384);
		keyvalue.value = buffer[idy].range(447, 416);
	}
else if(idx==7){	
		keyvalue.key = buffer[idy].range(479, 448);
		keyvalue.value = buffer[idy].range(511, 480);
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"getkeyvalue:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	#else
	keyvalue = buffer[idy].data[idx];
	#endif
	return keyvalue;
}
void setkeyvalue(uint512_dt buffer[BUFFER_SIZE], batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("enigma::setkeyvalue 35", idy, maxaddr_kvs);
	#endif
	
	#ifdef _WIDEWORD
if(idx==0){	
		buffer[idy].range(31, 0) = keyvalue.key;
		buffer[idy].range(63, 32) = keyvalue.value;
	}
else if(idx==1){	
		buffer[idy].range(95, 64) = keyvalue.key;
		buffer[idy].range(127, 96) = keyvalue.value;
	}
else if(idx==2){	
		buffer[idy].range(159, 128) = keyvalue.key;
		buffer[idy].range(191, 160) = keyvalue.value;
	}
else if(idx==3){	
		buffer[idy].range(223, 192) = keyvalue.key;
		buffer[idy].range(255, 224) = keyvalue.value;
	}
else if(idx==4){	
		buffer[idy].range(287, 256) = keyvalue.key;
		buffer[idy].range(319, 288) = keyvalue.value;
	}
else if(idx==5){	
		buffer[idy].range(351, 320) = keyvalue.key;
		buffer[idy].range(383, 352) = keyvalue.value;
	}
else if(idx==6){	
		buffer[idy].range(415, 384) = keyvalue.key;
		buffer[idy].range(447, 416) = keyvalue.value;
	}
else if(idx==7){	
		buffer[idy].range(479, 448) = keyvalue.key;
		buffer[idy].range(511, 480) = keyvalue.value;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"setkventryII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	#else
	buffer[idy].data[idx] = keyvalue;
	#endif
	return;
}

buffer_type getactivesize(buffer_type i, batch_type i_end, buffer_type buffersize){	
	#pragma HLS INLINE
	buffer_type activesize = 0;
	buffer_type next_i = i + buffersize;
	if(i >= i_end){ activesize = 0; }
	else if ((i < i_end) && (next_i < i_end)) { activesize = buffersize; }
	else if ((i < i_end) && (next_i >= i_end)) { activesize = i_end - i; }
	else {
		i = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"getactivesize:: should NEVER get here: activesize: "<<activesize<<", i: "<<i<<", next_i: "<<next_i<<", i_end: "<<i_end<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	print4("getactivesize: i", "i_end", "buffersize", "activesize", i, i_end, buffersize, activesize);
	#endif
	return activesize;
}
batch_type getAddr(batch_type addr){
	#pragma HLS INLINE
	return addr;
}
batch_type getstatsAddr(batch_type addr){
	#pragma HLS INLINE
	return BASEOFFSET_STATSDRAM + addr;
}
batch_type getmessagesAddr(batch_type addr){
	#pragma HLS INLINE
	return BASEOFFSET_MESSAGESDRAM + addr;
}

unsigned int getsourceskipsize(unsigned int currentLOP, unsigned int param){
	#pragma HLS INLINE
	unsigned int treedepth = TREE_DEPTH; // NB: for debugging purposes only
	unsigned int result = param;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		result = result / NUM_PARTITIONS;
	}
	return result;
}
unsigned int getdestskipsize(unsigned int currentLOP, unsigned int param){
	#pragma HLS INLINE
	unsigned int treedepthloop_tripcount = TREE_DEPTH; // NB: for debugging purposes only
	unsigned int result = param / NUM_PARTITIONS;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepthloop_tripcount avg=treedepthloop_tripcount
		result = result / NUM_PARTITIONS;
	}
	return result;
}

void loaddeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t LLOPnumpartitions){
	#pragma HLS INLINE
	unsigned int skipsize = getdestskipsize(currentLOP, LLOPnumpartitions);
	unsigned int nxtpos = partitionoffset;
	
	LOADDESTSTATS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE	
		buffer[p] = kvstats[getstatsAddr(nxtpos)]; 
		nxtpos += skipsize;
	}
	LOADDESTSTATS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		buffer[p].value = 0;
	}
	return;
}
void storedeststats(keyvalue_t * kvstats, keyvalue_t buffer[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t LLOPnumpartitions){
	#pragma HLS INLINE
	unsigned int skipsize = getdestskipsize(currentLOP, LLOPnumpartitions);
	unsigned int nxtpos = partitionoffset;
	
	STOREDESTSTATS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvstats[getstatsAddr(nxtpos)].value = buffer[p].value; 
		nxtpos += skipsize;
	}
	STOREDESTSTATS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		buffer[p].value = 0;
	}
	return;
}

unsigned int get_num_source_partitions(unsigned int currentLOP){
	#pragma HLS INLINE
	unsigned int treedepth = TREEDEPTH_FORSSDPARTITION0;
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}

void prepareoffsets(buffer_type sizes[NUM_PARTITIONS], buffer_type offsets[NUM_PARTITIONS]){
	#pragma HLS INLINE	
	// PREPAREOFFSETS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ sizes[p] = 0; }
	offsets[0] = 0;
	for (partition_type p = 1; p<NUM_PARTITIONS; p++){ offsets[p] = offsets[p-1] + sizes[p-1]; }
	return;
}
void preparecapsule(keyvalue_t capsule[NUM_PARTITIONS]){
	#pragma HLS INLINE
	capsule[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){ capsule[p].key = capsule[p-1].key + capsule[p-1].value; }
	return;
}
void preparecapsules( keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t capsule1[NUM_PARTITIONS], keyvalue_t capsule2[NUM_PARTITIONS], keyvalue_t capsule3[NUM_PARTITIONS]){
	#pragma HLS INLINE
	capsule0[0].key = 0;
	capsule1[0].key = 0;
	capsule2[0].key = 0;
	capsule3[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){
		capsule0[p].key = capsule0[p-1].key + capsule0[p-1].value;
		capsule1[p].key = capsule1[p-1].key + capsule1[p-1].value;
		capsule2[p].key = capsule2[p-1].key + capsule2[p-1].value;
		capsule3[p].key = capsule3[p-1].key + capsule3[p-1].value;
	}
	return;
}

void loadclopparams(globalparams_t globalparams, clopparams_t * llopparams, unsigned int currentLOP){
	#pragma HLS INLINE
	if((currentLOP % 2) == 1){ llopparams->baseaddr_worksourcekvs_kvs = BASEOFFSET_KVDRAM_KVS; llopparams->baseaddr_workdestkvs_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; }
	else { llopparams->baseaddr_worksourcekvs_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; llopparams->baseaddr_workdestkvs_kvs = BASEOFFSET_KVDRAM_KVS; }	
	llopparams->baseaddr_capsule_kvs = NAp;
	llopparams->baseaddr_stats = BASEOFFSET_STATSDRAM;
	llopparams->num_source_partitions = get_num_source_partitions(currentLOP);
	llopparams->nextsourceoffset_partition = 0;
	llopparams->nextdestoffset_partition = 0;
	llopparams->nextsourceoffset_kv = 0;
	llopparams->nextdestoffset_kv = 0;
	llopparams->sourceskipsize_partition = getsourceskipsize(currentLOP, globalparams.LLOPnumpartitions);
	llopparams->destskipsize_partition = getdestskipsize(currentLOP, globalparams.LLOPnumpartitions);
	llopparams->sourceskipsize_kv = getsourceskipsize(currentLOP, KVDATA_RANGE_PERSSDPARTITION);
	llopparams->destskipsize_kv = getdestskipsize(currentLOP, KVDATA_RANGE_PERSSDPARTITION);
	llopparams->destpartitionrange = (1 << (KVDATA_RANGE_PERSSDPARTITION_POW - (NUM_PARTITIONS_POW * currentLOP)));
	llopparams->currentLOP = currentLOP;
	llopparams->upperlimit = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	print1("loadclopparams:: llopparams->baseaddr_worksourcekvs_kvs", llopparams->baseaddr_worksourcekvs_kvs);
	print1("loadclopparams:: llopparams->baseaddr_workdestkvs_kvs", llopparams->baseaddr_workdestkvs_kvs);
	print1("loadclopparams:: llopparams->baseaddr_capsule_kvs", llopparams->baseaddr_capsule_kvs);
	print1("loadclopparams:: llopparams->baseaddr_stats", llopparams->baseaddr_stats);
	print1("loadclopparams:: llopparams->num_source_partitions", llopparams->num_source_partitions);
	print1("loadclopparams:: llopparams->nextsourceoffset_partition", llopparams->nextsourceoffset_partition);
	print1("loadclopparams:: llopparams->nextdestoffset_partition", llopparams->nextdestoffset_partition);
	print1("loadclopparams:: llopparams->nextsourceoffset_kv", llopparams->nextsourceoffset_kv);
	print1("loadclopparams:: llopparams->nextdestoffset_kv", llopparams->nextdestoffset_kv);
	print1("loadclopparams:: llopparams->sourceskipsize_partition", llopparams->sourceskipsize_partition);
	print1("loadclopparams:: llopparams->destskipsize_partition", llopparams->destskipsize_partition);
	print1("loadclopparams:: llopparams->sourceskipsize_kv", llopparams->sourceskipsize_kv);
	print1("loadclopparams:: llopparams->destskipsize_kv", llopparams->destskipsize_kv);
	print1("loadclopparams:: llopparams->destpartitionrange", llopparams->destpartitionrange);
	print1("loadclopparams:: llopparams->currentLOP", llopparams->currentLOP);
	print1("loadclopparams:: llopparams->upperlimit", llopparams->upperlimit);
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

value_t reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount){
	#pragma HLS INLINE
	value_t ret = 0;
	#ifdef PR_ALGORITHM
	ret = value + edgeval;
	#elif defined(BFS_ALGORITHM)
	ret = min(value, IterCount);
	#elif defined(BC_ALGORITHM)
	ret = min(value, IterCount);
	#endif
	return ret;
}

void generatepartitions0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
	unsigned int analysis_maxtreedepthsz = TREE_DEPTH;
	unsigned int analysis_avgnumsourcepartitions = (1 << (TREE_DEPTH * NUM_PARTITIONS_POW));
	unsigned int analysis_avgsz_kvs = KVDATA_BATCHSIZE_KVS / analysis_avgnumsourcepartitions; 
	unsigned int analysis_avgtripcnt = analysis_avgsz_kvs / (NUMSUBWORKERS * BUFFER_SIZE);
	unsigned int analysis_buffersz = BUFFER_SIZE;
	unsigned int analysis_partition = BUFFER_SIZE * VECTOR_SIZE;
	unsigned int analysis_savepartition = BUFFER_SIZE / NUM_PARTITIONS;
	
	uint512_dt v1_local0[PADDEDBUFFER_SIZE];
	uint512_dt result_local0[PADDEDBUFFER_SIZE];
	keyvalue_t capsule0[NUM_PARTITIONS];
	uint512_dt v1_local1[PADDEDBUFFER_SIZE];
	uint512_dt result_local1[PADDEDBUFFER_SIZE];
	keyvalue_t capsule1[NUM_PARTITIONS];
	uint512_dt v1_local2[PADDEDBUFFER_SIZE];
	uint512_dt result_local2[PADDEDBUFFER_SIZE];
	keyvalue_t capsule2[NUM_PARTITIONS];
	uint512_dt v1_local3[PADDEDBUFFER_SIZE];
	uint512_dt result_local3[PADDEDBUFFER_SIZE];
	keyvalue_t capsule3[NUM_PARTITIONS];

	// Partition Phase
	PARTITIONPHASE_LOOP1: for(unsigned int currentLOP=1; currentLOP<(globalparams.treedepth + 1); currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print4("PartitionPhase0:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		PARTITIONPHASE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partitionparams;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
			
			loaddeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions);			
			GENERATEPARTITIONS_INITKVDRAMREACKER_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			#pragma HLS PIPELINE II=1
				kvdeststats_tmp[p].value = 0;
			}
			
			keyvalue_t sourcestat = kvstats[getstatsAddr(llopparams.nextsourceoffset_partition)];
			partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparams.step_kvs = NUMSUBWORKERS * BUFFER_SIZE;
			partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			travstate_t travstate;
			#ifdef _DEBUGMODE_KERNELPRINTS3
			print6("PartitionPhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);
			#endif
			
			// Partition phase 
			PARTITIONPHASE_LOOP3: for (int i = partitionparams.begin_kvs; i < partitionparams.end_kvs; i += BUFFER_SIZE * NUMSUBWORKERS){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgtripcnt avg=analysis_avgtripcnt
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("PartitionPhase0:: i", "i_end", "NAp", "NAp", "skip size", i, partitionparams.end_kvs, NAp, NAp, BUFFER_SIZE * NUMSUBWORKERS);
				#endif
				
				buffer_type chunk0_size = BUFFER_SIZE;
				buffer_type chunk1_size = BUFFER_SIZE;
				buffer_type chunk2_size = BUFFER_SIZE;
				buffer_type chunk3_size = BUFFER_SIZE;
				
				//boundary checks
				batch_type i0 = i + (0 * BUFFER_SIZE);
				if (i0 > partitionparams.end_kvs){ chunk0_size = 0; }
				else if ((i0 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk0_size = partitionparams.end_kvs - i0; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk0_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i0 + chunk0_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i1 = i + (1 * BUFFER_SIZE);
				if (i1 > partitionparams.end_kvs){ chunk1_size = 0; }
				else if ((i1 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk1_size = partitionparams.end_kvs - i1; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk1_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i1 + chunk1_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i2 = i + (2 * BUFFER_SIZE);
				if (i2 > partitionparams.end_kvs){ chunk2_size = 0; }
				else if ((i2 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk2_size = partitionparams.end_kvs - i2; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk2_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i2 + chunk2_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i3 = i + (3 * BUFFER_SIZE);
				if (i3 > partitionparams.end_kvs){ chunk3_size = 0; }
				else if ((i3 + BUFFER_SIZE) > partitionparams.end_kvs){ chunk3_size = partitionparams.end_kvs - i3; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("generatepartitions0 35", chunk3_size, BUFFER_SIZE);
				checkoutofbounds("generatepartitions0 35", i3 + chunk3_size, KVSOURCEDRAMSZ_KVS);
				#endif

				// burst read first vector from global memory to local memory
				partitionphase_v1_rd:
					batch_type readdramoffset0_kvs = llopparams.baseaddr_worksourcekvs_kvs + (0 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd0 dram 35", readdramoffset0_kvs + chunk0_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd0 bram 35", chunk0_size, BUFFER_SIZE);
					#endif
					PARTITIONPHASE_READBUFFER0_LOOP1: for (buffer_type j=0; j<chunk0_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local0[j] = kvsourcedram[readdramoffset0_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
					batch_type readdramoffset1_kvs = llopparams.baseaddr_worksourcekvs_kvs + (1 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd1 dram 35", readdramoffset1_kvs + chunk1_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd1 bram 35", chunk1_size, BUFFER_SIZE);
					#endif
					PARTITIONPHASE_READBUFFER1_LOOP1: for (buffer_type j=0; j<chunk1_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local1[j] = kvsourcedram[readdramoffset1_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
					batch_type readdramoffset2_kvs = llopparams.baseaddr_worksourcekvs_kvs + (2 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd2 dram 35", readdramoffset2_kvs + chunk2_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd2 bram 35", chunk2_size, BUFFER_SIZE);
					#endif
					PARTITIONPHASE_READBUFFER2_LOOP1: for (buffer_type j=0; j<chunk2_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local2[j] = kvsourcedram[readdramoffset2_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
					batch_type readdramoffset3_kvs = llopparams.baseaddr_worksourcekvs_kvs + (3 * BUFFER_SIZE) + i;					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partitionphase_v1_rd3 dram 35", readdramoffset3_kvs + chunk3_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::partitionphase_v1_rd3 bram 35", chunk3_size, BUFFER_SIZE);
					#endif
					PARTITIONPHASE_READBUFFER3_LOOP1: for (buffer_type j=0; j<chunk3_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local3[j] = kvsourcedram[readdramoffset3_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
					
				PARTITIONPHASE_RESETCAPSULEA_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = 0;  capsule1[p].value = 0;  capsule2[p].value = 0;  capsule3[p].value = 0;  }
				
				// extract capsule
				PARTITIONPHASE_EXTRACTCAPSULES_LOOP2: for(buffer_type k=0; k<BUFFER_SIZE * VECTOR_SIZE; k++){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partition avg=analysis_partition
				#pragma HLS PIPELINE II=1
					keyvalue_t keyvalue0 = getkeyvalue(v1_local0, k, BUFFER_SIZE);
					keyvalue_t keyvalue1 = getkeyvalue(v1_local1, k, BUFFER_SIZE);
					keyvalue_t keyvalue2 = getkeyvalue(v1_local2, k, BUFFER_SIZE);
					keyvalue_t keyvalue3 = getkeyvalue(v1_local3, k, BUFFER_SIZE);
					
					partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
					
					if(k < chunk0_size * VECTOR_SIZE){ capsule0[p0].value += 1; }
					if(k < chunk1_size * VECTOR_SIZE){ capsule1[p1].value += 1; }
					if(k < chunk2_size * VECTOR_SIZE){ capsule2[p2].value += 1; }
					if(k < chunk3_size * VECTOR_SIZE){ capsule3[p3].value += 1; }
					
					#ifdef _DEBUGMODE_STATS
					if(k < chunk0_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					if(k < chunk1_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					if(k < chunk2_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					if(k < chunk3_size * VECTOR_SIZE){ globalstats_countkvstatscollected(1); }
					#endif
				}
				
				preparecapsules( capsule0 , capsule1 , capsule2 , capsule3 );				
				PARTITIONPHASE_RESETCAPSULEB_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = 0;  capsule1[p].value = 0;  capsule2[p].value = 0;  capsule3[p].value = 0;  }
				
				// partition
				PARTITIONPHASE_PARTITION_LOOP1: for(buffer_type k=0; k<BUFFER_SIZE * VECTOR_SIZE; k++){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partition avg=analysis_partition
				#pragma HLS PIPELINE II=1
					keyvalue_t keyvalue0 = getkeyvalue(v1_local0, k, BUFFER_SIZE);
					keyvalue_t keyvalue1 = getkeyvalue(v1_local1, k, BUFFER_SIZE);
					keyvalue_t keyvalue2 = getkeyvalue(v1_local2, k, BUFFER_SIZE);
					keyvalue_t keyvalue3 = getkeyvalue(v1_local3, k, BUFFER_SIZE);
					
					partition_type p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
					partition_type p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule0[p0].key + capsule0[p0].value, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule1[p1].key + capsule1[p1].value, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule2[p2].key + capsule2[p2].value, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("enigma::partition_ddr0_v 34", capsule3[p3].key + capsule3[p3].value, BUFFER_SIZE * VECTOR_SIZE);
					#endif
					
					setkeyvalue(result_local0, capsule0[p0].key + capsule0[p0].value, keyvalue0, BUFFER_SIZE);
					setkeyvalue(result_local1, capsule1[p1].key + capsule1[p1].value, keyvalue1, BUFFER_SIZE);
					setkeyvalue(result_local2, capsule2[p2].key + capsule2[p2].value, keyvalue2, BUFFER_SIZE);
					setkeyvalue(result_local3, capsule3[p3].key + capsule3[p3].value, keyvalue3, BUFFER_SIZE);
					
					if(k < chunk0_size * VECTOR_SIZE){ capsule0[p0].value += 1; }
					if(k < chunk1_size * VECTOR_SIZE){ capsule1[p1].value += 1; }
					if(k < chunk2_size * VECTOR_SIZE){ capsule2[p2].value += 1; }
					if(k < chunk3_size * VECTOR_SIZE){ capsule3[p3].value += 1; }
					
					#ifdef _DEBUGMODE_STATS
					if(k < chunk0_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					if(k < chunk1_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					if(k < chunk2_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					if(k < chunk3_size * VECTOR_SIZE){ globalstats_countkvspartitioned(1); }
					#endif
				}
				
				//burst write the result
				partitionphase_out_write:
					PARTITIONPHASE_SAVEBUFFER0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule0[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule0[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer0 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer0 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local0[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					PARTITIONPHASE_UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule0[p].value); }
					PARTITIONPHASE_SAVEBUFFER1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule1[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule1[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer1 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer1 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local1[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					PARTITIONPHASE_UPDATEGLOBALSTATS1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule1[p].value); }
					PARTITIONPHASE_SAVEBUFFER2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule2[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule2[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer2 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer2 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local2[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					PARTITIONPHASE_UPDATEGLOBALSTATS2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule2[p].value); }
					PARTITIONPHASE_SAVEBUFFER3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
						batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
						buffer_type bramoffset_kvs = (capsule3[p].key + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
						buffer_type size_kvs = (capsule3[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;					
						#ifdef _DEBUGMODE_CHECKS2
						checkoutofbounds("enigma::savebuffer3 dram 35", dramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), KVSOURCEDRAMSZ_KVS);
						checkoutofbounds("enigma::savebuffer3 bram 35", bramoffset_kvs + (BUFFER_SIZE / NUM_PARTITIONS), PADDEDBUFFER_SIZE);
						#endif
						#ifdef _WIDEWORD
						for(buffer_type i=0; i<(BUFFER_SIZE / NUM_PARTITIONS); i++)
						#else 
						for(buffer_type i=0; i<size_kvs; i++)
						#endif
						{
						#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savepartition avg=analysis_savepartition
						#pragma HLS PIPELINE II=1
							kvsourcedram[dramoffset_kvs + i] = result_local3[bramoffset_kvs + i];
							
							#ifdef _DEBUGMODE_STATS
							globalstats_countkvspartitionswritten(VECTOR_SIZE);
							#endif
						}
					}
					PARTITIONPHASE_UPDATEGLOBALSTATS3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule3[p].value); }
			}
			
			storedeststats(kvstats, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.LLOPnumpartitions); 
			llopparams.nextsourceoffset_partition += llopparams.sourceskipsize_partition;
			llopparams.nextdestoffset_partition += (llopparams.destskipsize_partition * NUM_PARTITIONS);
			llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv;
			llopparams.nextdestoffset_kv += llopparams.destskipsize_kv;
		}
		#ifdef _LDEBUGMODE_HEADER
		printglobalvars();
		printparameters();
		clearglobalvars();
		#endif
	}

	// Reduce Phase
	#ifdef ENABLEREDUCEPHASE
	if(globalparams.reducecommand == ON){
		unsigned int currentLOP = globalparams.treedepth + 1;
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);		
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print4("ReducePhase0:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		
		REDUCEPHASE_LOOP1: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=NUMSUBWORKERS){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions		
		
			partitionparams_t partition0params;
			partitionparams_t partition1params;
			partitionparams_t partition2params;
			partitionparams_t partition3params;
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
		
			keyvalue_t sourcestats[NUMSUBWORKERS];
			unsigned int maxsz = 0;
			for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats[i] = kvstats[source_partition + i]; }
			for(unsigned int sw=0; sw<NUMSUBWORKERS_APPLYPH; sw++){ maxsz = max(sourcestats[sw].value, maxsz); }
			unsigned int maxsz_kvs = (maxsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			
			keyvalue_t sourcestat;
			sourcestat = sourcestats[0];
			partition0params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition0params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition0params.step_kvs = NUMSUBWORKERS * BUFFER_SIZE;
			partition0params.end_kvs = partition0params.begin_kvs + partition0params.size_kvs;
			sourcestat = sourcestats[1];
			partition1params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition1params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition1params.step_kvs = NUMSUBWORKERS * BUFFER_SIZE;
			partition1params.end_kvs = partition1params.begin_kvs + partition1params.size_kvs;
			sourcestat = sourcestats[2];
			partition2params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition2params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition2params.step_kvs = NUMSUBWORKERS * BUFFER_SIZE;
			partition2params.end_kvs = partition2params.begin_kvs + partition2params.size_kvs;
			sourcestat = sourcestats[3];
			partition3params.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partition3params.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partition3params.step_kvs = NUMSUBWORKERS * BUFFER_SIZE;
			partition3params.end_kvs = partition3params.begin_kvs + partition3params.size_kvs;
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[0].key, sourcestats[0].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[1].key, sourcestats[1].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[2].key, sourcestats[2].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[3].key, sourcestats[3].value, BUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition);			
			print4("ReducePhase0:: maxsz", "maxsz_kvs", "NAp", "NAp", maxsz, maxsz_kvs, NAp, NAp);
			#endif
			
			// burst read first vector from global memory to local memory
			reducephase_v1_rdv:
				batch_type readdramoffset0_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::readphase_v1_rd0 dram 35", readdramoffset0_kvs + APPLYVERTEXBUFFERSZ_KVS, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_READVBUFFER0_LOOP1: for (buffer_type j=0; j<APPLYVERTEXBUFFERSZ_KVS; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
					v1_local0[j] = kvsourcedram[readdramoffset0_kvs + j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreadV(VECTOR_SIZE);
					#endif
				}
				batch_type readdramoffset1_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::readphase_v1_rd1 dram 35", readdramoffset1_kvs + APPLYVERTEXBUFFERSZ_KVS, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_READVBUFFER1_LOOP1: for (buffer_type j=0; j<APPLYVERTEXBUFFERSZ_KVS; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
					v1_local1[j] = kvsourcedram[readdramoffset1_kvs + j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreadV(VECTOR_SIZE);
					#endif
				}
				batch_type readdramoffset2_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::readphase_v1_rd2 dram 35", readdramoffset2_kvs + APPLYVERTEXBUFFERSZ_KVS, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_READVBUFFER2_LOOP1: for (buffer_type j=0; j<APPLYVERTEXBUFFERSZ_KVS; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
					v1_local2[j] = kvsourcedram[readdramoffset2_kvs + j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreadV(VECTOR_SIZE);
					#endif
				}
				batch_type readdramoffset3_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::readphase_v1_rd3 dram 35", readdramoffset3_kvs + APPLYVERTEXBUFFERSZ_KVS, KVSOURCEDRAMSZ_KVS);
				#endif
				REDUCEPHASE_READVBUFFER3_LOOP1: for (buffer_type j=0; j<APPLYVERTEXBUFFERSZ_KVS; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
					v1_local3[j] = kvsourcedram[readdramoffset3_kvs + j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreadV(VECTOR_SIZE);
					#endif
				}
			
			// Reduce phase
			REDUCEPHASE_REDUCEPARTITION_LOOP: for (int i = 0; i < maxsz_kvs; i += BUFFER_SIZE){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgtripcnt avg=analysis_avgtripcnt
				#ifdef _DEBUGMODE_KERNELPRINTS
				print5("ReducePhase0:: i", "i_end", "NAp", "NAp", "skip size", i, maxsz_kvs, NAp, NAp, BUFFER_SIZE);
				#endif
				
				buffer_type chunk0_size = BUFFER_SIZE;
				buffer_type chunk1_size = BUFFER_SIZE;
				buffer_type chunk2_size = BUFFER_SIZE;
				buffer_type chunk3_size = BUFFER_SIZE;
				
				//boundary checks
				batch_type i0 = i;
				if (i0 > partition0params.size_kvs){ chunk0_size = 0; }
				else if ((i0 + BUFFER_SIZE) > partition0params.size_kvs){ chunk0_size = partition0params.size_kvs - i0; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk0_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i0 + chunk0_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i1 = i;
				if (i1 > partition1params.size_kvs){ chunk1_size = 0; }
				else if ((i1 + BUFFER_SIZE) > partition1params.size_kvs){ chunk1_size = partition1params.size_kvs - i1; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk1_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i1 + chunk1_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i2 = i;
				if (i2 > partition2params.size_kvs){ chunk2_size = 0; }
				else if ((i2 + BUFFER_SIZE) > partition2params.size_kvs){ chunk2_size = partition2params.size_kvs - i2; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk2_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i2 + chunk2_size, KVSOURCEDRAMSZ_KVS);
				#endif
				batch_type i3 = i;
				if (i3 > partition3params.size_kvs){ chunk3_size = 0; }
				else if ((i3 + BUFFER_SIZE) > partition3params.size_kvs){ chunk3_size = partition3params.size_kvs - i3; }
				else {}
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("reducepartitions0 35", chunk3_size, BUFFER_SIZE);
				checkoutofbounds("reducepartitions0 35", i3 + chunk3_size, KVSOURCEDRAMSZ_KVS);
				#endif

				// burst read
				reducephase_v1_rd:
				
					batch_type readdramoffset0_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd0 dram 35", readdramoffset0_kvs + chunk0_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd0 bram 35", chunk0_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER0_LOOP1: for (buffer_type j=0; j<chunk0_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local0[j] = kvsourcedram[readdramoffset0_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset1_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd1 dram 35", readdramoffset1_kvs + chunk1_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd1 bram 35", chunk1_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER1_LOOP1: for (buffer_type j=0; j<chunk1_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local1[j] = kvsourcedram[readdramoffset1_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset2_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd2 dram 35", readdramoffset2_kvs + chunk2_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd2 bram 35", chunk2_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER2_LOOP1: for (buffer_type j=0; j<chunk2_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local2[j] = kvsourcedram[readdramoffset2_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
					batch_type readdramoffset3_kvs = llopparams.baseaddr_worksourcekvs_kvs + i;									
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("enigma::reducephase_v1_rd3 dram 35", readdramoffset3_kvs + chunk3_size, KVSOURCEDRAMSZ_KVS);
					checkoutofbounds("enigma::reducephase_v1_rd3 bram 35", chunk3_size, BUFFER_SIZE + 1);
					#endif
					REDUCEPHASE_READBUFFER3_LOOP1: for (buffer_type j=0; j<chunk3_size; j++){
					   #pragma HLS PIPELINE II=1
					   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz					
						v1_local3[j] = kvsourcedram[readdramoffset3_kvs + j];
						
						#ifdef _DEBUGMODE_STATS
						globalstats_countkvsread(VECTOR_SIZE);
						#endif
					}
				
				// reduce 
				REDUCEPHASE_REDUCE_LOOP2: for(buffer_type k=0; k<BUFFER_SIZE * VECTOR_SIZE; k++){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
				#pragma HLS PIPELINE II=1					
					keyvalue_t keyvalue0 = getkeyvalue(v1_local0, k, BUFFER_SIZE);
					keyvalue_t keyvalue1 = getkeyvalue(v1_local1, k, BUFFER_SIZE);
					keyvalue_t keyvalue2 = getkeyvalue(v1_local2, k, BUFFER_SIZE);
					keyvalue_t keyvalue3 = getkeyvalue(v1_local3, k, BUFFER_SIZE);
					
					vertex_t loc0 = keyvalue0.key - globalparams.vbegin;
					vertex_t loc1 = keyvalue1.key - globalparams.vbegin;
					vertex_t loc2 = keyvalue2.key - globalparams.vbegin;
					vertex_t loc3 = keyvalue3.key - globalparams.vbegin;
					
					if(loc0 >= BUFFER_SIZE * VECTOR_SIZE){ loc0 = 0; } // REMOVEME.
					if(loc1 >= BUFFER_SIZE * VECTOR_SIZE){ loc1 = 0; } // REMOVEME.
					if(loc2 >= BUFFER_SIZE * VECTOR_SIZE){ loc2 = 0; } // REMOVEME.
					if(loc3 >= BUFFER_SIZE * VECTOR_SIZE){ loc3 = 0; } // REMOVEME.
					
					#ifdef _DEBUGMODE_CHECKS2
					checkoutofbounds("legion::reduce_ddr0_v 34", loc0, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("legion::reduce_ddr0_v 34", loc1, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("legion::reduce_ddr0_v 34", loc2, BUFFER_SIZE * VECTOR_SIZE);
					checkoutofbounds("legion::reduce_ddr0_v 34", loc3, BUFFER_SIZE * VECTOR_SIZE);
					#endif
					
					keyvalue_t vprop0 = getkeyvalue(result_local0, loc0, BUFFER_SIZE);
					keyvalue_t vprop1 = getkeyvalue(result_local1, loc1, BUFFER_SIZE);
					keyvalue_t vprop2 = getkeyvalue(result_local2, loc2, BUFFER_SIZE);
					keyvalue_t vprop3 = getkeyvalue(result_local3, loc3, BUFFER_SIZE);
					
					value_t temp0 = reducefunc(vprop0.key, vprop0.value, keyvalue0.value, globalparams.IterCount);
					value_t temp1 = reducefunc(vprop1.key, vprop1.value, keyvalue1.value, globalparams.IterCount);
					value_t temp2 = reducefunc(vprop2.key, vprop2.value, keyvalue2.value, globalparams.IterCount);
					value_t temp3 = reducefunc(vprop3.key, vprop3.value, keyvalue3.value, globalparams.IterCount);
					
					vprop0.value = temp0;
					vprop1.value = temp1;
					vprop2.value = temp2;
					vprop3.value = temp3;
					
					setkeyvalue(result_local0, loc0, vprop0, BUFFER_SIZE);	
					setkeyvalue(result_local1, loc1, vprop1, BUFFER_SIZE);	
					setkeyvalue(result_local2, loc2, vprop2, BUFFER_SIZE);	
					setkeyvalue(result_local3, loc3, vprop3, BUFFER_SIZE);	
					
					#ifdef _DEBUGMODE_STATS
					if(k < chunk0_size * VECTOR_SIZE){ globalstats_countkvsreduced(1); }
					if(k < chunk1_size * VECTOR_SIZE){ globalstats_countkvsreduced(1); }
					if(k < chunk2_size * VECTOR_SIZE){ globalstats_countkvsreduced(1); }
					if(k < chunk3_size * VECTOR_SIZE){ globalstats_countkvsreduced(1); }
					#endif
				}
			}
			
			//burst write the result
			reducephase_out_write:
				batch_type dramsaveoffset0_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write0 dram 35", dramsaveoffset0_kvs + APPLYVERTEXBUFFERSZ_KVS, KVDATA_RANGE_PERSSDPARTITION_KVS);
				#endif
				REDUCEPHASE_SAVEVBUFFER0_LOOP1: for (buffer_type j=0; j<APPLYVERTEXBUFFERSZ_KVS; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset0_kvs + j] = result_local0[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}
				batch_type dramsaveoffset1_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write1 dram 35", dramsaveoffset1_kvs + APPLYVERTEXBUFFERSZ_KVS, KVDATA_RANGE_PERSSDPARTITION_KVS);
				#endif
				REDUCEPHASE_SAVEVBUFFER1_LOOP1: for (buffer_type j=0; j<APPLYVERTEXBUFFERSZ_KVS; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset1_kvs + j] = result_local1[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}
				batch_type dramsaveoffset2_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write2 dram 35", dramsaveoffset2_kvs + APPLYVERTEXBUFFERSZ_KVS, KVDATA_RANGE_PERSSDPARTITION_KVS);
				#endif
				REDUCEPHASE_SAVEVBUFFER2_LOOP1: for (buffer_type j=0; j<APPLYVERTEXBUFFERSZ_KVS; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset2_kvs + j] = result_local2[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}
				batch_type dramsaveoffset3_kvs = globalparams.baseaddr_destkvs_kvs + (llopparams.nextsourceoffset_kv / VECTOR_SIZE);					
				#ifdef _DEBUGMODE_CHECKS2
				checkoutofbounds("enigma::reducephase_out_write3 dram 35", dramsaveoffset3_kvs + APPLYVERTEXBUFFERSZ_KVS, KVDATA_RANGE_PERSSDPARTITION_KVS);
				#endif
				REDUCEPHASE_SAVEVBUFFER3_LOOP1: for (buffer_type j=0; j<APPLYVERTEXBUFFERSZ_KVS; j++){
				   #pragma HLS PIPELINE II=1
				   #pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
					kvdestdram[dramsaveoffset3_kvs + j] = result_local3[j];
					
					#ifdef _DEBUGMODE_STATS
					globalstats_countkvsreducewritten(VECTOR_SIZE);
					#endif
				}

			llopparams.nextsourceoffset_partition += NUMSUBWORKERS;
			llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv;
		}
		#ifdef _LDEBUGMODE_HEADER
		printglobalvars();
		printparameters();
		clearglobalvars();
		#endif
	}
	#endif
	return;
}

void loadKvDRAM0(uint512_dt * kvdram, batch_type sourceoffset_kvs, batch_type destoffset_kvs, batch_type size_kvs){
	int analysis_chunksize = BUFFER_SIZE;
	int analysis_iterationcount = KVDATA_BATCHSIZE_KVS / BUFFER_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	print4("### loadKvDRAM0:: sourceoffset_kvs", "destoffset_kvs", "size_kvs", "NAp", sourceoffset_kvs, destoffset_kvs, size_kvs, NAp);
	#endif
	
	uint512_dt v1_local[BUFFER_SIZE];
	
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("loadKvDRAM0 35", destoffset_kvs + size_kvs, KVSOURCEDRAMSZ_KVS);
	#endif
	int i;
    for ( i = 0; i < size_kvs; i += BUFFER_SIZE) {
	#pragma HLS LOOP_TRIPCOUNT min=analysis_iterationcount max=analysis_iterationcount
		
		//boundary checks
		int chunk_size = BUFFER_SIZE;
        if ((i + BUFFER_SIZE) > size_kvs)
            chunk_size = size_kvs - i;
	
		v1_rd:
			for (int j = 0; j < chunk_size; j++) {
			   #pragma HLS PIPELINE II=1
			   #pragma HLS LOOP_TRIPCOUNT min=analysis_chunksize max=analysis_chunksize
				v1_local[j] = kvdram[sourceoffset_kvs + i + j];
			}
		out_write:
			for (int j = 0; j < chunk_size; j++) {
			   #pragma HLS PIPELINE II=1
			   #pragma HLS LOOP_TRIPCOUNT min=analysis_chunksize max=analysis_chunksize
				kvdram[destoffset_kvs + i + j] = v1_local[j];
			}
    }
	return;
}

extern "C" {
void topkernel(
uint512_dt * kvsourcedramA
		,uint512_dt * kvdestdramA
		,keyvalue_t * kvstatsA
        ){
#pragma HLS INTERFACE m_axi port=kvsourcedramA  offset=slave bundle=gmem0 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=kvdestdramA  offset=slave bundle=gmem1 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=kvstatsA  offset=slave bundle=gmem2

#pragma HLS INTERFACE s_axilite port=kvsourcedramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvdestdramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvstatsA  bundle=control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable=kvstatsA
	
	globalparams_t globalparams;
	globalparams.processcommand = kvstatsA[getmessagesAddr(MESSAGES_PROCESSCOMMANDID)].key;
	globalparams.partitioncommand = kvstatsA[getmessagesAddr(MESSAGES_PARTITIONCOMMANDID)].key;
	globalparams.reducecommand = kvstatsA[getmessagesAddr(MESSAGES_APPLYUPDATESCOMMANDID)].key;
	globalparams.vbegin = kvstatsA[getmessagesAddr(MESSAGES_VOFFSET)].key; 
	globalparams.vsize = kvstatsA[getmessagesAddr(MESSAGES_VSIZE)].key; 
	globalparams.treedepth = kvstatsA[getmessagesAddr(MESSAGES_TREEDEPTH)].key; 
	globalparams.LLOPnumpartitions = kvstatsA[getmessagesAddr(MESSAGES_FINALNUMPARTITIONS)].key;
	globalparams.IterCount = kvstatsA[getmessagesAddr(MESSAGES_GRAPHITERATIONID)].key;
	globalparams.baseaddr_destkvs_kvs = 0;
	
	#ifdef _LDEBUGMODE_HEADER
	std::cout<<std::endl;
	std::cout<<"Kernel Started: globalparams.processcommand: "<<globalparams.processcommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.partitioncommand: "<<globalparams.partitioncommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.reducecommand: "<<globalparams.reducecommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.vbegin: "<<globalparams.vbegin<<std::endl;
	std::cout<<"Kernel Started: globalparams.vsize: "<<globalparams.vsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.treedepth: "<<globalparams.treedepth<<std::endl;
	std::cout<<"Kernel Started: globalparams.LLOPnumpartitions: "<<globalparams.LLOPnumpartitions<<std::endl;
	std::cout<<"Kernel Started: globalparams.IterCount: "<<globalparams.IterCount<<std::endl;
	#endif
	
	unsigned int destIToffset = (DRAMBATCHFACTOR - (globalparams.IterCount % DRAMBATCHFACTOR) - 1);
	if(destIToffset != 0){ 
		loadKvDRAM0(kvsourcedramA, 0, (DRAMBATCHFACTOR - (globalparams.IterCount % DRAMBATCHFACTOR) - 1) * KVDATA_BATCHSIZE_KVS, KVDATA_BATCHSIZE_KVS);
	}
	if((globalparams.IterCount % DRAMBATCHFACTOR) != (DRAMBATCHFACTOR - 1)){ return; }
	
	generatepartitions0(kvsourcedramA, kvdestdramA, kvstatsA, globalparams);
	
	/** 	reducepartitions0(kvsourcedramA, kvstatsA, globalparams);
 */
	return;
}
}





