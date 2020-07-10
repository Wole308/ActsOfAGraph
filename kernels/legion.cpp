#include "/home/centos/src/project_data/oj2zf/ActsOfAGraph/src/common.h"
#include "/home/centos/src/project_data/oj2zf/ActsOfAGraph/kernels/legion.h"
using namespace std;

#ifndef FPGA_IMPL
legion::legion(){}
legion::~legion(){}
#endif 
#ifndef HW

unsigned int globalvar_totalkvstatsread;
unsigned int globalvar_totalkvstatscollected;
unsigned int globalvar_totalkvstatswritten;

unsigned int globalvar_totalkvsread;

unsigned int globalstats_totalkvsspread;
unsigned int globalvar_totalkvsspreadwritten;

unsigned int globalstats_totalkvsreduced;
unsigned int globalstats_totalkvsreducewritten;
#endif 
#ifdef _LDEBUGMODE_HEADER
void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound){
	if(data >= upper_bound){ std::cout<<"legion::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<std::endl; exit(EXIT_FAILURE); }
}
void checkforequal(string message, unsigned int data1, unsigned int data2){
	if(data1 == data2){ cout<<"legion::checkforequal: ERROR. data1 == data2. message: "<<message<<", data1: "<<data1<<", data2: "<<data2<<endl; exit(EXIT_FAILURE); }
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
	cout<<endl<<"legion::printparameters: test started."<<endl;
	cout<<"legion::printparameters:: KVBUFFERSZ: "<<KVBUFFERSZ<<endl;
	cout<<"legion::printparameters:: LSRCKVBUFFERSZ: "<<LSRCKVBUFFERSZ<<endl;
	cout<<"legion::printparameters:: LSRCKVBUFFERSZ_KVS: "<<LSRCKVBUFFERSZ_KVS<<endl;
	cout<<"legion::printparameters:: LDESTKVBUFFERSZ: "<<LDESTKVBUFFERSZ<<endl;
	cout<<"legion::printparameters:: LDESTKVBUFFERSZ_KVS: "<<LDESTKVBUFFERSZ_KVS<<endl;
	cout<<"legion::printparameters:: KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
	cout<<"legion::printparameters:: KVDATA_BATCHSIZE_KVS: "<<KVDATA_BATCHSIZE_KVS<<endl;
	cout<<"legion::printparameters:: CAPSULESTATSBATCHSZ: "<<CAPSULESTATSBATCHSZ<<endl;
	cout<<"legion::printparameters:: CAPSULESTATSBATCHSZ_KVS: "<<CAPSULESTATSBATCHSZ_KVS<<endl;
	cout<<"legion::printparameters:: LKVSTATSBIGBUFFERSZ_KVS: "<<LKVSTATSBIGBUFFERSZ_KVS<<endl;
}
#endif
#ifdef _DEBUGMODE_STATS
void printglobalvars(){
	cout<<"printglobalvars: "<<endl;
	cout<<"legion::printglobalvars:: COLLECTSTATS & SPREAD & PARTITION:: globalvar_totalkvsread: "<<globalvar_totalkvsread<<endl;
	
	cout<<"legion::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatsread: "<<globalvar_totalkvstatsread<<endl;
	cout<<"legion::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatscollected: "<<globalvar_totalkvstatscollected<<endl;
	cout<<"legion::printglobalvars:: COLLECTSTATS:: globalvar_totalkvstatswritten: "<<globalvar_totalkvstatswritten<<endl;	

	cout<<"legion::printglobalvars:: SPREAD:: globalstats_totalkvsspread: "<<globalstats_totalkvsspread<<endl;
	cout<<"legion::printglobalvars:: SPREAD:: globalvar_totalkvsspreadwritten: "<<globalvar_totalkvsspreadwritten<<endl;
	
	cout<<"legion::printglobalvars:: REDUCE:: globalstats_totalkvsreduced: "<<globalstats_totalkvsreduced<<endl;
	cout<<"legion::printglobalvars:: REDUCE:: globalstats_totalkvsreducewritten: "<<globalstats_totalkvsreducewritten<<endl;
}
void clearglobalvars(){
	cout<<"clearglobalvars: "<<endl;
	globalvar_totalkvsread = 0;
	globalvar_totalkvsspreadwritten = 0;
	globalstats_totalkvsspread = 0;
	globalvar_totalkvstatsread = 0;
	globalvar_totalkvstatscollected = 0;
	globalvar_totalkvstatswritten = 0;
	globalstats_totalkvsreduced = 0;
	globalstats_totalkvsreducewritten = 0;
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
void globalstats_countkvsspreadwritten(unsigned int count){
	globalvar_totalkvsspreadwritten += count;
	return;
}
void globalstats_countkvsspread(unsigned int count){
	globalstats_totalkvsspread += count;
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
#endif 

vertex_t GETKVDATA_RANGE_PERSSDPARTITION(){ return KVDATA_RANGE_PERSSDPARTITION; }
vertex_t GETKVDATA_RANGE_PERSSDPARTITION_POW(){ return KVDATA_RANGE_PERSSDPARTITION_POW; }
vertex_t GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(){ return APPLYVERTEXBUFFERSZ_PERSSDPARTITION; }
vertex_t GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(){ return APPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS; }
unsigned int GETTREEDEPTH(){ return TREE_DEPTH; }
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
	#ifdef FORCDFINISH
	return (addr & ((1 << 27) - 1)); 
	#else 
	return addr;
	#endif
}
batch_type getcapsuleAddr(batch_type addr){
	#pragma HLS INLINE
	#ifdef FORCDFINISH
	return (addr & ((1 << 27) - 1)); 
	#else 
	return addr;
	#endif
}
unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	#pragma HLS INLINE
	partition_type hashedval = 0;
	hashedval = ((keyvalue.key - upperlimit) >> (KVDATA_RANGE_PERSSDPARTITION_POW - (NUM_PARTITIONS_POW * currentLOP)));
	#ifdef _DEBUGMODE_CHECKS
	if(hashedval >= NUM_PARTITIONS){ print5("keyvalue.key", "upperlimit", "hashedval", "currentLOP", "RangePerCLOP", keyvalue.key, upperlimit, hashedval, currentLOP, (1 << (KVDATA_RANGE_PERSSDPARTITION_POW - (NUM_PARTITIONS_POW * currentLOP)))); exit(EXIT_FAILURE); }
	#else 
	if(hashedval >= NUM_PARTITIONS){ hashedval = 0; } // REMOVEME.
	#endif
	return hashedval;
}
buffer_type getglobalstatoffset_kvs(unsigned int kvoffset_kvs){
	#pragma HLS INLINE
	return ((kvoffset_kvs / (VECTOR_SIZE * LSRCKVBUFFERSZ_KVS)) * NUM_PARTITIONS);
}
unsigned int getsourceskipsize(unsigned int currentLOP, unsigned int param){
	unsigned int treedepth = TREE_DEPTH; // NB: for debugging purposes only
	unsigned int result = param;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		result = result / NUM_PARTITIONS;
	}
	return result;
}
unsigned int getdestskipsize(unsigned int currentLOP, unsigned int param){
	unsigned int treedepthloop_tripcount = TREE_DEPTH; // NB: for debugging purposes only
	unsigned int result = param / NUM_PARTITIONS;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepthloop_tripcount avg=treedepthloop_tripcount
		result = result / NUM_PARTITIONS;
	}
	return result;
}
keyvalue_t getkeyvalue(uint512_dt * buffer, batch_type addr, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	keyvalue_t keyvalue;
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	if(idy >= maxaddr_kvs){ cout<<"getkeyvalue:: ERROR 45. addr: "<<addr<<", idy: "<<idy<<", idx: "<<idx<<endl; exit(EXIT_FAILURE); }
	checkoutofbounds("legion::getkeyvalue 35", idy, maxaddr_kvs);
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
		std::cout<<"setkventryII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	#else
	keyvalue = buffer[idy].data[idx];
	#endif
	return keyvalue;
}
void setkeyvalue(uint512_dt * buffer, batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("legion::setkeyvalue 35", idy, maxaddr_kvs);
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
void setvalue(uint512_dt * buffer, batch_type addr, value_t value, batch_type maxaddr_kvs){
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("legion::setvalue 35", idy, maxaddr_kvs);
	#endif
	
	#ifdef _WIDEWORD
if(idx==0){
		buffer[idy].range(63, 32) = value;
	}
else if(idx==1){
		buffer[idy].range(127, 96) = value;
	}
else if(idx==2){
		buffer[idy].range(191, 160) = value;
	}
else if(idx==3){
		buffer[idy].range(255, 224) = value;
	}
else if(idx==4){
		buffer[idy].range(319, 288) = value;
	}
else if(idx==5){
		buffer[idy].range(383, 352) = value;
	}
else if(idx==6){
		buffer[idy].range(447, 416) = value;
	}
else if(idx==7){
		buffer[idy].range(511, 480) = value;
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"setkventryII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	#else
	buffer[idy].data[idx].value = value;
	#endif
	return;
}
void loaddeststats(uint512_dt kvstats[KVSTATS_SIZE_KVS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t finalnumpartitions){
	unsigned int skipsize = getdestskipsize(currentLOP, finalnumpartitions);
	unsigned int nxtpos = partitionoffset;
	
	LOADDESTSTATS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		kvdeststats_tmp[p] = getkeyvalue(kvstats, nxtpos, KVSTATS_SIZE_KVS);
		nxtpos += skipsize;
	}
	LOADDESTSTATS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1	
		kvdeststats_tmp[p].value = 0;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ cout<<"loaddeststats:: kvdeststats_tmp["<<p<<"].key: "<<kvdeststats_tmp[p].key<<", kvdeststats_tmp["<<p<<"].value: "<<kvdeststats_tmp[p].value<<endl; }
	#endif
	// exit(EXIT_SUCCESS);
	return;
}
void storedeststats(uint512_dt kvstats[KVSTATS_SIZE_KVS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t finalnumpartitions){
	unsigned int skipsize = getdestskipsize(currentLOP, finalnumpartitions);
	unsigned int nxtpos = partitionoffset;
	STOREDESTSTATS_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		setvalue(kvstats, nxtpos, kvdeststats_tmp[p].value, KVSTATS_SIZE_KVS);
		nxtpos += skipsize;
	}
	STOREDESTSTATS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
		kvdeststats_tmp[p].value = 0;
	}
	return;
}
unsigned int get_num_source_partitions(unsigned int currentLOP){
	unsigned int treedepth = TREE_DEPTH;
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}
void addtobuffer(uint512_dt buffer[LDESTKVBUFFERSZ_KVS], buffer_type offset, keyvalue_t kventry){
	#pragma HLS INLINE
	buffer_type idy = offset / VECTOR_SIZE;
	buffer_type idx = offset % VECTOR_SIZE;
	#ifdef _DEBUGMODE_KERNELPRINTS
	checkoutofbounds("legion::addtobuffer 35", idy, LDESTKVBUFFERSZ_KVS);
	checkoutofbounds("legion::addtobuffer 34", idx, VECTOR_SIZE);
	#endif

if(idx==0){
		#ifdef _WIDEWORD
		buffer[idy].range(31, 0) = kventry.key;
		buffer[idy].range(63, 32) = kventry.value;
		#else 
		buffer[idy].data[0] = kventry;
		#endif 
	}
else if(idx==1){
		#ifdef _WIDEWORD
		buffer[idy].range(95, 64) = kventry.key;
		buffer[idy].range(127, 96) = kventry.value;
		#else 
		buffer[idy].data[1] = kventry;
		#endif 
	}
else if(idx==2){
		#ifdef _WIDEWORD
		buffer[idy].range(159, 128) = kventry.key;
		buffer[idy].range(191, 160) = kventry.value;
		#else 
		buffer[idy].data[2] = kventry;
		#endif 
	}
else if(idx==3){
		#ifdef _WIDEWORD
		buffer[idy].range(223, 192) = kventry.key;
		buffer[idy].range(255, 224) = kventry.value;
		#else 
		buffer[idy].data[3] = kventry;
		#endif 
	}
else if(idx==4){
		#ifdef _WIDEWORD
		buffer[idy].range(287, 256) = kventry.key;
		buffer[idy].range(319, 288) = kventry.value;
		#else 
		buffer[idy].data[4] = kventry;
		#endif 
	}
else if(idx==5){
		#ifdef _WIDEWORD
		buffer[idy].range(351, 320) = kventry.key;
		buffer[idy].range(383, 352) = kventry.value;
		#else 
		buffer[idy].data[5] = kventry;
		#endif 
	}
else if(idx==6){
		#ifdef _WIDEWORD
		buffer[idy].range(415, 384) = kventry.key;
		buffer[idy].range(447, 416) = kventry.value;
		#else 
		buffer[idy].data[6] = kventry;
		#endif 
	}
else if(idx==7){
		#ifdef _WIDEWORD
		buffer[idy].range(479, 448) = kventry.key;
		buffer[idy].range(511, 480) = kventry.value;
		#else 
		buffer[idy].data[7] = kventry;
		#endif 
	}
	else {
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"addtobuffer:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return;
}
void addtobuffer2(uint512_dt buffer0[LDESTKVBUFFERSZ_KVS],uint512_dt buffer1[LDESTKVBUFFERSZ_KVS],uint512_dt buffer2[LDESTKVBUFFERSZ_KVS],uint512_dt buffer3[LDESTKVBUFFERSZ_KVS],uint512_dt buffer4[LDESTKVBUFFERSZ_KVS],uint512_dt buffer5[LDESTKVBUFFERSZ_KVS],uint512_dt buffer6[LDESTKVBUFFERSZ_KVS],uint512_dt buffer7[LDESTKVBUFFERSZ_KVS], buffer_type offset, uint512_dt keyvalueset){
	#pragma HLS INLINE
	buffer_type lbufferidx = offset / LDESTKVBUFFERSZ_KVS;
	buffer_type lbufferoffset = offset % LDESTKVBUFFERSZ_KVS;
	#ifdef _DEBUGMODE_KERNELPRINTS
	checkoutofbounds("legion::addtobuffer 34", lbufferidx, VECTOR_SIZE);
	checkoutofbounds("legion::addtobuffer 35", lbufferoffset, LDESTKVBUFFERSZ_KVS);
	#endif
		
 if(lbufferidx == 0){ buffer0[lbufferoffset] = keyvalueset; }
else if(lbufferidx == 1){ buffer1[lbufferoffset] = keyvalueset; }
else if(lbufferidx == 2){ buffer2[lbufferoffset] = keyvalueset; }
else if(lbufferidx == 3){ buffer3[lbufferoffset] = keyvalueset; }
else if(lbufferidx == 4){ buffer4[lbufferoffset] = keyvalueset; }
else if(lbufferidx == 5){ buffer5[lbufferoffset] = keyvalueset; }
else if(lbufferidx == 6){ buffer6[lbufferoffset] = keyvalueset; }
else if(lbufferidx == 7){ buffer7[lbufferoffset] = keyvalueset; }
	return;
}
void calculateoffsets(keyvalue_t buffer[NUM_PARTITIONS]){
	buffer[0].key = 0;
	for(partition_type p=1; p<NUM_PARTITIONS; p++){ buffer[p].key = buffer[p-1].key + buffer[p-1].value; }
}
value_t reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount){
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

void loadstatsbuffer_ddr0(uint512_dt * kvdram, uint512_dt buffer[KVSTATS_SIZE_KVS], batch_type baseaddress){	
	LOADSTATSBUFFER_DDR0_LOOP1: for(batch_type i=0; i<KVSTATS_SIZE_KVS; i++){
	#pragma HLS PIPELINE II=1
		buffer[i] = kvdram[getAddr(baseaddress + i)];
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	printkeyvalues("loadstatsbuffer_ddr0", (keyvalue_t *)buffer, KVSTATS_SIZE);
	#endif 
	return;
}
void loadcapsulestatsS_ddr0(uint512_dt * stats, keyvalue_t buffer0[NUM_PARTITIONS], keyvalue_t buffer1[NUM_PARTITIONS], keyvalue_t buffer2[NUM_PARTITIONS], keyvalue_t buffer3[NUM_PARTITIONS], keyvalue_t buffer4[NUM_PARTITIONS], keyvalue_t buffer5[NUM_PARTITIONS], keyvalue_t buffer6[NUM_PARTITIONS], keyvalue_t buffer7[NUM_PARTITIONS],  batch_type localoffset_kvs, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	batch_type ldramoffset_kvs = llopparams.baseaddr_capsule_kvs + localoffset_kvs;
	LOADCAPSULESTATSV_DDR0_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::loadcapsulestatsS_ddr0 35", (localoffset_kvs + i), CAPSULESTATSBATCHSZ_KVS);
		checkoutofbounds("legion::loadcapsulestatsS_ddr0 35", (ldramoffset_kvs + i), WORKVOLUMEDRAMSZ_KVS);
		#endif
		uint512_dt Vtemp = stats[getcapsuleAddr((ldramoffset_kvs + i))];
		#ifdef _WIDEWORD
			buffer0[i].key = Vtemp.range(31, 0);
			buffer0[i].value = Vtemp.range(63, 32);
			buffer1[i].key = Vtemp.range(95, 64);
			buffer1[i].value = Vtemp.range(127, 96);
			buffer2[i].key = Vtemp.range(159, 128);
			buffer2[i].value = Vtemp.range(191, 160);
			buffer3[i].key = Vtemp.range(223, 192);
			buffer3[i].value = Vtemp.range(255, 224);
			buffer4[i].key = Vtemp.range(287, 256);
			buffer4[i].value = Vtemp.range(319, 288);
			buffer5[i].key = Vtemp.range(351, 320);
			buffer5[i].value = Vtemp.range(383, 352);
			buffer6[i].key = Vtemp.range(415, 384);
			buffer6[i].value = Vtemp.range(447, 416);
			buffer7[i].key = Vtemp.range(479, 448);
			buffer7[i].value = Vtemp.range(511, 480);
		#else 
			buffer0[i] = Vtemp.data[0];
			buffer1[i] = Vtemp.data[1];
			buffer2[i] = Vtemp.data[2];
			buffer3[i] = Vtemp.data[3];
			buffer4[i] = Vtemp.data[4];
			buffer5[i] = Vtemp.data[5];
			buffer6[i] = Vtemp.data[6];
			buffer7[i] = Vtemp.data[7];
	
		#endif 
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatsread((1 * VECTOR_SIZE));
		#endif
	}
	
	calculateoffsets(buffer0);
	calculateoffsets(buffer1);
	calculateoffsets(buffer2);
	calculateoffsets(buffer3);
	calculateoffsets(buffer4);
	calculateoffsets(buffer5);
	calculateoffsets(buffer6);
	calculateoffsets(buffer7);
	return;
}
void loadbufferV_ddr0(uint512_dt * kvdram, keyvalue_t buffer0[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer1[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer2[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer3[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer4[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer5[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer6[LSRCKVBUFFERSZ_KVS],keyvalue_t buffer7[LSRCKVBUFFERSZ_KVS], batch_type localoffset_kvs, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("legion::loadbufferV_ddr 35", (localoffset_kvs + LSRCKVBUFFERSZ_KVS), PADDEDKVDATA_BATCHSIZE_KVS + 1);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	print4("legion::loadbufferV_ddr 35: localoffset_kvs", "llopparams.sourcebaseaddr_kvs", "NAp", "NAp", localoffset_kvs, llopparams.sourcebaseaddr_kvs, NAp, NAp);
	#endif
	
	LOADBUFFERV_DDR0_LOOP1: for(buffer_type i=0; i<LSRCKVBUFFERSZ_KVS; i++){
	#pragma HLS PIPELINE II=1
		uint512_dt Vtemp = kvdram[getAddr((llopparams.sourcebaseaddr_kvs + localoffset_kvs + i))];
		#ifdef _WIDEWORD
			buffer0[i].key = Vtemp.range(31, 0);
			buffer0[i].value = Vtemp.range(63, 32);
			buffer1[i].key = Vtemp.range(95, 64);
			buffer1[i].value = Vtemp.range(127, 96);
			buffer2[i].key = Vtemp.range(159, 128);
			buffer2[i].value = Vtemp.range(191, 160);
			buffer3[i].key = Vtemp.range(223, 192);
			buffer3[i].value = Vtemp.range(255, 224);
			buffer4[i].key = Vtemp.range(287, 256);
			buffer4[i].value = Vtemp.range(319, 288);
			buffer5[i].key = Vtemp.range(351, 320);
			buffer5[i].value = Vtemp.range(383, 352);
			buffer6[i].key = Vtemp.range(415, 384);
			buffer6[i].value = Vtemp.range(447, 416);
			buffer7[i].key = Vtemp.range(479, 448);
			buffer7[i].value = Vtemp.range(511, 480);
		#else 
			buffer0[i] = Vtemp.data[0];
			buffer1[i] = Vtemp.data[1];
			buffer2[i] = Vtemp.data[2];
			buffer3[i] = Vtemp.data[3];
			buffer4[i] = Vtemp.data[4];
			buffer5[i] = Vtemp.data[5];
			buffer6[i] = Vtemp.data[6];
			buffer7[i] = Vtemp.data[7];
		#endif 
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsread((1 * VECTOR_SIZE));
		#endif
	}
	return;
}
void loadbufferH_ddr0(uint512_dt * kvdram, uint512_dt buffer[LSRCKVBUFFERSZ_KVS], batch_type localoffset_kvs, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("legion::loadbufferH_ddr 35", (localoffset_kvs + LSRCKVBUFFERSZ_KVS), PADDEDKVDATA_BATCHSIZE_KVS + 1);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	print4("legion::loadbufferH_ddr 35: localoffset_kvs", "llopparams.sourcebaseaddr_kvs", "NAp", "NAp", localoffset_kvs, llopparams.sourcebaseaddr_kvs, NAp, NAp);
	#endif
	
	LOADBUFFERH_DDR0_LOOP1: for(buffer_type i=0; i<LSRCKVBUFFERSZ_KVS; i++){
	#pragma HLS PIPELINE II=1
		buffer[i] = kvdram[getAddr(llopparams.sourcebaseaddr_kvs + localoffset_kvs + i)];		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsread((1 * VECTOR_SIZE));
		#endif
	}
	return;
}

void extractcapsules_ddr0(uint512_dt sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], vector_type vecid, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	buffer_type sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	buffer_type sizes1[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes1 complete
	buffer_type sizes2[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes2 complete
	buffer_type sizes3[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes3 complete
	buffer_type sizes4[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes4 complete
	buffer_type sizes5[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes5 complete
	buffer_type sizes6[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes6 complete
	buffer_type sizes7[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes7 complete
	
	if(vecid == 0){
		EXTRACTCAPSULES0_LOOP0: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE II=1
			#ifdef _WIDEWORD
			resultbuffer[p].range(31, 0) = 0;
			resultbuffer[p].range(63, 32) = 0;
			#else 
			resultbuffer[p].data[0].key = 0;
			resultbuffer[p].data[0].value = 0;
			#endif
			#ifdef _WIDEWORD
			resultbuffer[p].range(95, 64) = 0;
			resultbuffer[p].range(127, 96) = 0;
			#else 
			resultbuffer[p].data[1].key = 0;
			resultbuffer[p].data[1].value = 0;
			#endif
			#ifdef _WIDEWORD
			resultbuffer[p].range(159, 128) = 0;
			resultbuffer[p].range(191, 160) = 0;
			#else 
			resultbuffer[p].data[2].key = 0;
			resultbuffer[p].data[2].value = 0;
			#endif
			#ifdef _WIDEWORD
			resultbuffer[p].range(223, 192) = 0;
			resultbuffer[p].range(255, 224) = 0;
			#else 
			resultbuffer[p].data[3].key = 0;
			resultbuffer[p].data[3].value = 0;
			#endif
			#ifdef _WIDEWORD
			resultbuffer[p].range(287, 256) = 0;
			resultbuffer[p].range(319, 288) = 0;
			#else 
			resultbuffer[p].data[4].key = 0;
			resultbuffer[p].data[4].value = 0;
			#endif
			#ifdef _WIDEWORD
			resultbuffer[p].range(351, 320) = 0;
			resultbuffer[p].range(383, 352) = 0;
			#else 
			resultbuffer[p].data[5].key = 0;
			resultbuffer[p].data[5].value = 0;
			#endif
			#ifdef _WIDEWORD
			resultbuffer[p].range(415, 384) = 0;
			resultbuffer[p].range(447, 416) = 0;
			#else 
			resultbuffer[p].data[6].key = 0;
			resultbuffer[p].data[6].value = 0;
			#endif
			#ifdef _WIDEWORD
			resultbuffer[p].range(479, 448) = 0;
			resultbuffer[p].range(511, 480) = 0;
			#else 
			resultbuffer[p].data[7].key = 0;
			resultbuffer[p].data[7].value = 0;
			#endif
		}
	}
	
	EXTRACTCAPSULES0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p] = 0;
		sizes1[p] = 0;
		sizes2[p] = 0;
		sizes3[p] = 0;
		sizes4[p] = 0;
		sizes5[p] = 0;
		sizes6[p] = 0;
		sizes7[p] = 0;
	}	
	EXTRACTCAPSULES0_LOOP2: for(buffer_type i=0; i<LSRCKVBUFFERSZ_KVS; i++){
	#pragma HLS PIPELINE II=1
		uint512_dt Vtemp = sourcebuffer[i];
			keyvalue_t keyvalue0;
			partition_type p0;
			#ifdef _WIDEWORD
			keyvalue0.key = Vtemp.range(31, 0);
			keyvalue0.value = Vtemp.range(63, 32);
			#else 
			keyvalue0 = Vtemp.data[0];
			#endif 
			p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit);
			sizes0[p0] += 1;
			keyvalue_t keyvalue1;
			partition_type p1;
			#ifdef _WIDEWORD
			keyvalue1.key = Vtemp.range(95, 64);
			keyvalue1.value = Vtemp.range(127, 96);
			#else 
			keyvalue1 = Vtemp.data[1];
			#endif 
			p1 = getpartition(keyvalue1, llopparams.currentLOP, llopparams.upperlimit);
			sizes1[p1] += 1;
			keyvalue_t keyvalue2;
			partition_type p2;
			#ifdef _WIDEWORD
			keyvalue2.key = Vtemp.range(159, 128);
			keyvalue2.value = Vtemp.range(191, 160);
			#else 
			keyvalue2 = Vtemp.data[2];
			#endif 
			p2 = getpartition(keyvalue2, llopparams.currentLOP, llopparams.upperlimit);
			sizes2[p2] += 1;
			keyvalue_t keyvalue3;
			partition_type p3;
			#ifdef _WIDEWORD
			keyvalue3.key = Vtemp.range(223, 192);
			keyvalue3.value = Vtemp.range(255, 224);
			#else 
			keyvalue3 = Vtemp.data[3];
			#endif 
			p3 = getpartition(keyvalue3, llopparams.currentLOP, llopparams.upperlimit);
			sizes3[p3] += 1;
			keyvalue_t keyvalue4;
			partition_type p4;
			#ifdef _WIDEWORD
			keyvalue4.key = Vtemp.range(287, 256);
			keyvalue4.value = Vtemp.range(319, 288);
			#else 
			keyvalue4 = Vtemp.data[4];
			#endif 
			p4 = getpartition(keyvalue4, llopparams.currentLOP, llopparams.upperlimit);
			sizes4[p4] += 1;
			keyvalue_t keyvalue5;
			partition_type p5;
			#ifdef _WIDEWORD
			keyvalue5.key = Vtemp.range(351, 320);
			keyvalue5.value = Vtemp.range(383, 352);
			#else 
			keyvalue5 = Vtemp.data[5];
			#endif 
			p5 = getpartition(keyvalue5, llopparams.currentLOP, llopparams.upperlimit);
			sizes5[p5] += 1;
			keyvalue_t keyvalue6;
			partition_type p6;
			#ifdef _WIDEWORD
			keyvalue6.key = Vtemp.range(415, 384);
			keyvalue6.value = Vtemp.range(447, 416);
			#else 
			keyvalue6 = Vtemp.data[6];
			#endif 
			p6 = getpartition(keyvalue6, llopparams.currentLOP, llopparams.upperlimit);
			sizes6[p6] += 1;
			keyvalue_t keyvalue7;
			partition_type p7;
			#ifdef _WIDEWORD
			keyvalue7.key = Vtemp.range(479, 448);
			keyvalue7.value = Vtemp.range(511, 480);
			#else 
			keyvalue7 = Vtemp.data[7];
			#endif 
			p7 = getpartition(keyvalue7, llopparams.currentLOP, llopparams.upperlimit);
			sizes7[p7] += 1;
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvstatscollected(1 * VECTOR_SIZE);
		#endif
	}
	EXTRACTCAPSULES0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		
		#ifdef _WIDEWORD
		resultbuffer[p].range(31, 0) = 0;
		resultbuffer[p].range(63, 32) = resultbuffer[p].range(63, 32) + sizes0[p];
		#else 
		resultbuffer[p].data[0].key = 0;
		resultbuffer[p].data[0].value = resultbuffer[p].data[0].value + sizes0[p];
		#endif
		
		#ifdef _WIDEWORD
		resultbuffer[p].range(95, 64) = 0;
		resultbuffer[p].range(127, 96) = resultbuffer[p].range(127, 96) + sizes1[p];
		#else 
		resultbuffer[p].data[1].key = 0;
		resultbuffer[p].data[1].value = resultbuffer[p].data[1].value + sizes1[p];
		#endif
		
		#ifdef _WIDEWORD
		resultbuffer[p].range(159, 128) = 0;
		resultbuffer[p].range(191, 160) = resultbuffer[p].range(191, 160) + sizes2[p];
		#else 
		resultbuffer[p].data[2].key = 0;
		resultbuffer[p].data[2].value = resultbuffer[p].data[2].value + sizes2[p];
		#endif
		
		#ifdef _WIDEWORD
		resultbuffer[p].range(223, 192) = 0;
		resultbuffer[p].range(255, 224) = resultbuffer[p].range(255, 224) + sizes3[p];
		#else 
		resultbuffer[p].data[3].key = 0;
		resultbuffer[p].data[3].value = resultbuffer[p].data[3].value + sizes3[p];
		#endif
		
		#ifdef _WIDEWORD
		resultbuffer[p].range(287, 256) = 0;
		resultbuffer[p].range(319, 288) = resultbuffer[p].range(319, 288) + sizes4[p];
		#else 
		resultbuffer[p].data[4].key = 0;
		resultbuffer[p].data[4].value = resultbuffer[p].data[4].value + sizes4[p];
		#endif
		
		#ifdef _WIDEWORD
		resultbuffer[p].range(351, 320) = 0;
		resultbuffer[p].range(383, 352) = resultbuffer[p].range(383, 352) + sizes5[p];
		#else 
		resultbuffer[p].data[5].key = 0;
		resultbuffer[p].data[5].value = resultbuffer[p].data[5].value + sizes5[p];
		#endif
		
		#ifdef _WIDEWORD
		resultbuffer[p].range(415, 384) = 0;
		resultbuffer[p].range(447, 416) = resultbuffer[p].range(447, 416) + sizes6[p];
		#else 
		resultbuffer[p].data[6].key = 0;
		resultbuffer[p].data[6].value = resultbuffer[p].data[6].value + sizes6[p];
		#endif
		
		#ifdef _WIDEWORD
		resultbuffer[p].range(479, 448) = 0;
		resultbuffer[p].range(511, 480) = resultbuffer[p].range(511, 480) + sizes7[p];
		#else 
		resultbuffer[p].data[7].key = 0;
		resultbuffer[p].data[7].value = resultbuffer[p].data[7].value + sizes7[p];
		#endif
	}
	return;
}
void spreadkvs_ddr0_v0(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int spreadkvsloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sizes[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes complete 
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	SPREAD0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ sizes[p] = 0; }

	SPREAD0_LOOP2: for(buffer_type i=0; i<sourcesize; i++){ /// LSRCKVBUFFERSZ_KVS
	#pragma HLS LOOP_TRIPCOUNT min=0 max=spreadkvsloop_tripcount avg=spreadkvsloop_tripcount
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue = sourcebuffer[i];
		partition_type p = getpartition(keyvalue, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_KERNELPRINTS
		checkoutofbounds("legion::spreadkvs_ddr0_v0 34", stats[p].key + sizes[p], LDESTKVBUFFERSZ_KVS);
		#endif 
		addtobuffer(resultbuffer, stats[p].key + sizes[p], keyvalue);
		sizes[p] += 1;
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsspread(1);
		#endif
	}
	return;
}
void spreadkvs_ddr0_v1(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int spreadkvsloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sizes[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes complete 
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	SPREAD1_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ sizes[p] = 0; }

	SPREAD1_LOOP2: for(buffer_type i=0; i<sourcesize; i++){ /// LSRCKVBUFFERSZ_KVS
	#pragma HLS LOOP_TRIPCOUNT min=0 max=spreadkvsloop_tripcount avg=spreadkvsloop_tripcount
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue = sourcebuffer[i];
		partition_type p = getpartition(keyvalue, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_KERNELPRINTS
		checkoutofbounds("legion::spreadkvs_ddr0_v1 34", stats[p].key + sizes[p], LDESTKVBUFFERSZ_KVS);
		#endif 
		addtobuffer(resultbuffer, stats[p].key + sizes[p], keyvalue);
		sizes[p] += 1;
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsspread(1);
		#endif
	}
	return;
}
void spreadkvs_ddr0_v2(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int spreadkvsloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sizes[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes complete 
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	SPREAD2_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ sizes[p] = 0; }

	SPREAD2_LOOP2: for(buffer_type i=0; i<sourcesize; i++){ /// LSRCKVBUFFERSZ_KVS
	#pragma HLS LOOP_TRIPCOUNT min=0 max=spreadkvsloop_tripcount avg=spreadkvsloop_tripcount
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue = sourcebuffer[i];
		partition_type p = getpartition(keyvalue, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_KERNELPRINTS
		checkoutofbounds("legion::spreadkvs_ddr0_v2 34", stats[p].key + sizes[p], LDESTKVBUFFERSZ_KVS);
		#endif 
		addtobuffer(resultbuffer, stats[p].key + sizes[p], keyvalue);
		sizes[p] += 1;
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsspread(1);
		#endif
	}
	return;
}
void spreadkvs_ddr0_v3(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int spreadkvsloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sizes[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes complete 
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	SPREAD3_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ sizes[p] = 0; }

	SPREAD3_LOOP2: for(buffer_type i=0; i<sourcesize; i++){ /// LSRCKVBUFFERSZ_KVS
	#pragma HLS LOOP_TRIPCOUNT min=0 max=spreadkvsloop_tripcount avg=spreadkvsloop_tripcount
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue = sourcebuffer[i];
		partition_type p = getpartition(keyvalue, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_KERNELPRINTS
		checkoutofbounds("legion::spreadkvs_ddr0_v3 34", stats[p].key + sizes[p], LDESTKVBUFFERSZ_KVS);
		#endif 
		addtobuffer(resultbuffer, stats[p].key + sizes[p], keyvalue);
		sizes[p] += 1;
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsspread(1);
		#endif
	}
	return;
}
void spreadkvs_ddr0_v4(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int spreadkvsloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sizes[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes complete 
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	SPREAD4_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ sizes[p] = 0; }

	SPREAD4_LOOP2: for(buffer_type i=0; i<sourcesize; i++){ /// LSRCKVBUFFERSZ_KVS
	#pragma HLS LOOP_TRIPCOUNT min=0 max=spreadkvsloop_tripcount avg=spreadkvsloop_tripcount
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue = sourcebuffer[i];
		partition_type p = getpartition(keyvalue, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_KERNELPRINTS
		checkoutofbounds("legion::spreadkvs_ddr0_v4 34", stats[p].key + sizes[p], LDESTKVBUFFERSZ_KVS);
		#endif 
		addtobuffer(resultbuffer, stats[p].key + sizes[p], keyvalue);
		sizes[p] += 1;
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsspread(1);
		#endif
	}
	return;
}
void spreadkvs_ddr0_v5(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int spreadkvsloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sizes[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes complete 
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	SPREAD5_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ sizes[p] = 0; }

	SPREAD5_LOOP2: for(buffer_type i=0; i<sourcesize; i++){ /// LSRCKVBUFFERSZ_KVS
	#pragma HLS LOOP_TRIPCOUNT min=0 max=spreadkvsloop_tripcount avg=spreadkvsloop_tripcount
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue = sourcebuffer[i];
		partition_type p = getpartition(keyvalue, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_KERNELPRINTS
		checkoutofbounds("legion::spreadkvs_ddr0_v5 34", stats[p].key + sizes[p], LDESTKVBUFFERSZ_KVS);
		#endif 
		addtobuffer(resultbuffer, stats[p].key + sizes[p], keyvalue);
		sizes[p] += 1;
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsspread(1);
		#endif
	}
	return;
}
void spreadkvs_ddr0_v6(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int spreadkvsloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sizes[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes complete 
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	SPREAD6_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ sizes[p] = 0; }

	SPREAD6_LOOP2: for(buffer_type i=0; i<sourcesize; i++){ /// LSRCKVBUFFERSZ_KVS
	#pragma HLS LOOP_TRIPCOUNT min=0 max=spreadkvsloop_tripcount avg=spreadkvsloop_tripcount
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue = sourcebuffer[i];
		partition_type p = getpartition(keyvalue, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_KERNELPRINTS
		checkoutofbounds("legion::spreadkvs_ddr0_v6 34", stats[p].key + sizes[p], LDESTKVBUFFERSZ_KVS);
		#endif 
		addtobuffer(resultbuffer, stats[p].key + sizes[p], keyvalue);
		sizes[p] += 1;
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsspread(1);
		#endif
	}
	return;
}
void spreadkvs_ddr0_v7(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int spreadkvsloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sizes[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes complete 
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	SPREAD7_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ sizes[p] = 0; }

	SPREAD7_LOOP2: for(buffer_type i=0; i<sourcesize; i++){ /// LSRCKVBUFFERSZ_KVS
	#pragma HLS LOOP_TRIPCOUNT min=0 max=spreadkvsloop_tripcount avg=spreadkvsloop_tripcount
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue = sourcebuffer[i];
		partition_type p = getpartition(keyvalue, llopparams.currentLOP, llopparams.upperlimit);
		#ifdef _DEBUGMODE_KERNELPRINTS
		checkoutofbounds("legion::spreadkvs_ddr0_v7 34", stats[p].key + sizes[p], LDESTKVBUFFERSZ_KVS);
		#endif 
		addtobuffer(resultbuffer, stats[p].key + sizes[p], keyvalue);
		sizes[p] += 1;
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsspread(1);
		#endif
	}
	return;
}
void reduce_ddr0_v0(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int reduceloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	REDUCE0_LOOP2: for(buffer_type i=0; i<sourcesize; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=reduceloop_tripcount avg=reduceloop_tripcount
	#pragma HLS PIPELINE II=1		
		vertex_t voffset = 0;
		unsigned int IterCount = 0;
		
		keyvalue_t keyvalue = sourcebuffer[i];
		vertex_t loc = keyvalue.key - voffset;
		if(loc >= LDESTKVBUFFERSZ){ loc = 0; } // REMOVEME.
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr0_v0 34", loc, LDESTKVBUFFERSZ);
		#endif
		keyvalue_t vprop = getkeyvalue(resultbuffer, loc, LDESTKVBUFFERSZ_KVS);
		value_t temp = reducefunc(vprop.key, vprop.value, keyvalue.value, IterCount);
		vprop.value = temp;
		setkeyvalue(resultbuffer, loc, vprop, LDESTKVBUFFERSZ_KVS);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}
void reduce_ddr0_v1(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int reduceloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	REDUCE1_LOOP2: for(buffer_type i=0; i<sourcesize; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=reduceloop_tripcount avg=reduceloop_tripcount
	#pragma HLS PIPELINE II=1		
		vertex_t voffset = 0;
		unsigned int IterCount = 0;
		
		keyvalue_t keyvalue = sourcebuffer[i];
		vertex_t loc = keyvalue.key - voffset;
		if(loc >= LDESTKVBUFFERSZ){ loc = 0; } // REMOVEME.
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr0_v1 34", loc, LDESTKVBUFFERSZ);
		#endif
		keyvalue_t vprop = getkeyvalue(resultbuffer, loc, LDESTKVBUFFERSZ_KVS);
		value_t temp = reducefunc(vprop.key, vprop.value, keyvalue.value, IterCount);
		vprop.value = temp;
		setkeyvalue(resultbuffer, loc, vprop, LDESTKVBUFFERSZ_KVS);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}
void reduce_ddr0_v2(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int reduceloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	REDUCE2_LOOP2: for(buffer_type i=0; i<sourcesize; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=reduceloop_tripcount avg=reduceloop_tripcount
	#pragma HLS PIPELINE II=1		
		vertex_t voffset = 0;
		unsigned int IterCount = 0;
		
		keyvalue_t keyvalue = sourcebuffer[i];
		vertex_t loc = keyvalue.key - voffset;
		if(loc >= LDESTKVBUFFERSZ){ loc = 0; } // REMOVEME.
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr0_v2 34", loc, LDESTKVBUFFERSZ);
		#endif
		keyvalue_t vprop = getkeyvalue(resultbuffer, loc, LDESTKVBUFFERSZ_KVS);
		value_t temp = reducefunc(vprop.key, vprop.value, keyvalue.value, IterCount);
		vprop.value = temp;
		setkeyvalue(resultbuffer, loc, vprop, LDESTKVBUFFERSZ_KVS);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}
void reduce_ddr0_v3(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int reduceloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	REDUCE3_LOOP2: for(buffer_type i=0; i<sourcesize; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=reduceloop_tripcount avg=reduceloop_tripcount
	#pragma HLS PIPELINE II=1		
		vertex_t voffset = 0;
		unsigned int IterCount = 0;
		
		keyvalue_t keyvalue = sourcebuffer[i];
		vertex_t loc = keyvalue.key - voffset;
		if(loc >= LDESTKVBUFFERSZ){ loc = 0; } // REMOVEME.
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr0_v3 34", loc, LDESTKVBUFFERSZ);
		#endif
		keyvalue_t vprop = getkeyvalue(resultbuffer, loc, LDESTKVBUFFERSZ_KVS);
		value_t temp = reducefunc(vprop.key, vprop.value, keyvalue.value, IterCount);
		vprop.value = temp;
		setkeyvalue(resultbuffer, loc, vprop, LDESTKVBUFFERSZ_KVS);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}
void reduce_ddr0_v4(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int reduceloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	REDUCE4_LOOP2: for(buffer_type i=0; i<sourcesize; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=reduceloop_tripcount avg=reduceloop_tripcount
	#pragma HLS PIPELINE II=1		
		vertex_t voffset = 0;
		unsigned int IterCount = 0;
		
		keyvalue_t keyvalue = sourcebuffer[i];
		vertex_t loc = keyvalue.key - voffset;
		if(loc >= LDESTKVBUFFERSZ){ loc = 0; } // REMOVEME.
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr0_v4 34", loc, LDESTKVBUFFERSZ);
		#endif
		keyvalue_t vprop = getkeyvalue(resultbuffer, loc, LDESTKVBUFFERSZ_KVS);
		value_t temp = reducefunc(vprop.key, vprop.value, keyvalue.value, IterCount);
		vprop.value = temp;
		setkeyvalue(resultbuffer, loc, vprop, LDESTKVBUFFERSZ_KVS);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}
void reduce_ddr0_v5(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int reduceloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	REDUCE5_LOOP2: for(buffer_type i=0; i<sourcesize; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=reduceloop_tripcount avg=reduceloop_tripcount
	#pragma HLS PIPELINE II=1		
		vertex_t voffset = 0;
		unsigned int IterCount = 0;
		
		keyvalue_t keyvalue = sourcebuffer[i];
		vertex_t loc = keyvalue.key - voffset;
		if(loc >= LDESTKVBUFFERSZ){ loc = 0; } // REMOVEME.
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr0_v5 34", loc, LDESTKVBUFFERSZ);
		#endif
		keyvalue_t vprop = getkeyvalue(resultbuffer, loc, LDESTKVBUFFERSZ_KVS);
		value_t temp = reducefunc(vprop.key, vprop.value, keyvalue.value, IterCount);
		vprop.value = temp;
		setkeyvalue(resultbuffer, loc, vprop, LDESTKVBUFFERSZ_KVS);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}
void reduce_ddr0_v6(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int reduceloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	REDUCE6_LOOP2: for(buffer_type i=0; i<sourcesize; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=reduceloop_tripcount avg=reduceloop_tripcount
	#pragma HLS PIPELINE II=1		
		vertex_t voffset = 0;
		unsigned int IterCount = 0;
		
		keyvalue_t keyvalue = sourcebuffer[i];
		vertex_t loc = keyvalue.key - voffset;
		if(loc >= LDESTKVBUFFERSZ){ loc = 0; } // REMOVEME.
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr0_v6 34", loc, LDESTKVBUFFERSZ);
		#endif
		keyvalue_t vprop = getkeyvalue(resultbuffer, loc, LDESTKVBUFFERSZ_KVS);
		value_t temp = reducefunc(vprop.key, vprop.value, keyvalue.value, IterCount);
		vprop.value = temp;
		setkeyvalue(resultbuffer, loc, vprop, LDESTKVBUFFERSZ_KVS);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}
void reduce_ddr0_v7(keyvalue_t sourcebuffer[LSRCKVBUFFERSZ_KVS], uint512_dt resultbuffer[LDESTKVBUFFERSZ_KVS], travstate_t travstate, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int reduceloop_tripcount = LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	buffer_type sourcesize = getactivesize(travstate.i_kvs, travstate.iend_kvs, LSRCKVBUFFERSZ_KVS);
	REDUCE7_LOOP2: for(buffer_type i=0; i<sourcesize; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=reduceloop_tripcount avg=reduceloop_tripcount
	#pragma HLS PIPELINE II=1		
		vertex_t voffset = 0;
		unsigned int IterCount = 0;
		
		keyvalue_t keyvalue = sourcebuffer[i];
		vertex_t loc = keyvalue.key - voffset;
		if(loc >= LDESTKVBUFFERSZ){ loc = 0; } // REMOVEME.
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::reduce_ddr0_v7 34", loc, LDESTKVBUFFERSZ);
		#endif
		keyvalue_t vprop = getkeyvalue(resultbuffer, loc, LDESTKVBUFFERSZ_KVS);
		value_t temp = reducefunc(vprop.key, vprop.value, keyvalue.value, IterCount);
		vprop.value = temp;
		setkeyvalue(resultbuffer, loc, vprop, LDESTKVBUFFERSZ_KVS);	
		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreduced(1);
		#endif
	}
	return;
}

void savestatsbuffer_ddr0(uint512_dt * kvdram, uint512_dt buffer[KVSTATS_SIZE_KVS], batch_type baseaddress){	
	SAVESTATSBUFFER_DDR0_LOOP1: for(batch_type i=0; i<KVSTATS_SIZE_KVS; i++){
	#pragma HLS PIPELINE II=1
		kvdram[getAddr(baseaddress + i)] = buffer[i];
	}
	#ifdef _DEBUGMODE_KERNELPRINTS
	printkeyvalues("savestatsbuffer_ddr0", (keyvalue_t *)buffer, KVSTATS_SIZE);
	#endif 
	return;
}
void savepartialbuffer_ddr0(uint512_dt * kvdram, uint512_dt buffer[LDESTKVBUFFERSZ_KVS], batch_type baseaddress_kvs, batch_type localoffset_kvs, buffer_type size_kvs, code_type callerprocessid, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){							
	#ifdef _DEBUGMODE_KERNELPRINTS2
	print4("legion::savepartialbuffer_ddr0 35:", "NAp", "NAp", "NAp", NAp, NAp, NAp, NAp);
	#endif
	
	unsigned int ldramoffset_kvs = baseaddress_kvs + localoffset_kvs;
	SAVEPARTIALBUFFER_DDR0_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::savepartialbuffer_ddr0 35", (ldramoffset_kvs + i), WORKVOLUMEDRAMSZ_KVS);
		#endif
		kvdram[getAddr((ldramoffset_kvs + i))] = buffer[i];
		#ifdef _DEBUGMODE_STATS
		if(callerprocessid == SPREADPROCESS){ globalstats_countkvsspreadwritten((1 * VECTOR_SIZE)); }
		else if(callerprocessid == COLLECTSTATSPROCESS){ globalstats_countkvstatswritten(1 * VECTOR_SIZE); }
		#endif
	}
	return;
}
void savebuffer_ddr0(uint512_dt * kvdram, uint512_dt buffer[LDESTKVBUFFERSZ_KVS], keyvalue_t stats[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], batch_type baseaddress_kvs, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){							
	#ifdef _DEBUGMODE_KERNELPRINTS2
	print4("legion::savebuffer_ddr0 35:", "NAp", "NAp", "NAp", NAp, NAp, NAp, NAp);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	printkeyvalues("savebuffer_ddr0", stats, NUM_PARTITIONS);
	#endif
	
	unsigned int analysis_looptripcount = LDESTKVBUFFERSZ_KVS / NUM_PARTITIONS; // NB: debugging purposes only
	SAVEBUFFER_DDR0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::savebuffer_ddr0 34", (kvdeststats_tmp[p].key + kvdeststats_tmp[p].value + stats[p].value), PADDEDKVDATA_BATCHSIZE + 1);
		#endif
	
		batch_type kvdramoffset_kvs = baseaddress_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
		buffer_type bufferoffset_kvs = stats[p].key / VECTOR_SIZE;
		buffer_type size_kvs = stats[p].value / VECTOR_SIZE;
		#ifdef _DEBUGMODE_KERNELPRINTS
		print4("savebuffer_ddr0:: bufferoffset_kvs", "size_kvs", "NAp", "NAp", bufferoffset_kvs, size_kvs, NAp, NAp);
		#endif
		#ifdef _DEBUGMODE_CHECKS2
		checkoutofbounds("legion::savebuffer_ddr0 35", (kvdramoffset_kvs + size_kvs), WORKVOLUMEDRAMSZ_KVS);
		#endif
	
		SAVEBUFFER_DDR0_LOOP1B: for(buffer_type i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_looptripcount avg=analysis_looptripcount
		#pragma HLS PIPELINE II=1
			kvdram[getAddr(kvdramoffset_kvs + i)] = buffer[bufferoffset_kvs + i];
			#ifdef _DEBUGMODE_STATS
			globalstats_countkvsspreadwritten(1 * VECTOR_SIZE);
			#endif
		}
	}
	SAVEBUFFER_DDR0_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += stats[p].value; }
	return;
}
void savebuffer_ddr0(uint512_dt * kvdram, uint512_dt buffer[LDESTKVBUFFERSZ_KVS], batch_type baseaddress_kvs, batch_type localoffset_kvs, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){							
	#ifdef _DEBUGMODE_CHECKS2
	checkoutofbounds("legion::savebuffer_ddr0 34", getAddr(llopparams.destbaseaddr_kvs + localoffset_kvs + LDESTKVBUFFERSZ_KVS), KVDATA_RANGE_PERSSDPARTITION_KVS);
	#endif
	
	SAVEBUFFER_DDR0_LOOP1: for(buffer_type i=0; i<LDESTKVBUFFERSZ_KVS; i++){
	#pragma HLS PIPELINE II=1
		buffer[i] = kvdram[getAddr(llopparams.destbaseaddr_kvs + localoffset_kvs + i)];		
		#ifdef _DEBUGMODE_STATS
		globalstats_countkvsreducewritten((1 * VECTOR_SIZE));
		#endif
	}
	return;
}

void processpartition_ddr0(uint512_dt * kvdram, keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int collectstatsloop_tripcount = KVDATA_BATCHSIZE_KVS / (VECTOR_SIZE * LSRCKVBUFFERSZ_KVS); // NB: for debugging purposes only
	unsigned int spreadloop_tripcount = KVDATA_BATCHSIZE_KVS / (VECTOR_SIZE * LSRCKVBUFFERSZ_KVS); // NB: for debugging purposes only
	unsigned int partitionloop_tripcount = KVDATA_BATCHSIZE_KVS / (VECTOR_SIZE * LSRCKVBUFFERSZ_KVS); // NB: for debugging purposes only
	
	uint512_dt sourcebufferH0[LSRCKVBUFFERSZ_KVS];
	
	keyvalue_t sourcebufferV00[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV01[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV02[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV03[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV04[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV05[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV06[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV07[LSRCKVBUFFERSZ_KVS];
	
	uint512_dt resultbuffer00[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer01[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer02[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer03[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer04[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer05[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer06[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer07[LDESTKVBUFFERSZ_KVS];
	
	keyvalue_t bufferstats00[NUM_PARTITIONS];
	keyvalue_t bufferstats01[NUM_PARTITIONS];
	keyvalue_t bufferstats02[NUM_PARTITIONS];
	keyvalue_t bufferstats03[NUM_PARTITIONS];
	keyvalue_t bufferstats04[NUM_PARTITIONS];
	keyvalue_t bufferstats05[NUM_PARTITIONS];
	keyvalue_t bufferstats06[NUM_PARTITIONS];
	keyvalue_t bufferstats07[NUM_PARTITIONS];
	
	uint512_dt resultbufferS0[LDESTKVBUFFERSZ_KVS];
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	print5("processpartition_ddr0:: begin", "size", "step", "currentLOP", "range_per_destpartition", partitionparams.begin_kvs * VECTOR_SIZE, partitionparams.size_kvs * VECTOR_SIZE, partitionparams.step_kvs * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP));
	#endif
	
	COLLECTSTATS_LOOP2: for(batch_type i_kvs=partitionparams.begin_kvs; i_kvs<(partitionparams.begin_kvs + partitionparams.size_kvs); i_kvs+=NUMWORKERS * VECTOR_SIZE * LSRCKVBUFFERSZ_KVS){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=collectstatsloop_tripcount avg=collectstatsloop_tripcount
		#ifdef _DEBUGMODE_KERNELPRINTS
		print5("collectstats_ddr0:: i", "i_end", "begin", "size", "skip size", i_kvs * VECTOR_SIZE, (partitionparams.begin_kvs + partitionparams.size_kvs) * VECTOR_SIZE, partitionparams.begin_kvs * VECTOR_SIZE, partitionparams.size_kvs * VECTOR_SIZE, NUMWORKERS * VECTOR_SIZE * LSRCKVBUFFERSZ);
		#endif
		COLLECTSTATS_LOOP2B: for(vector_type v=0; v<VECTOR_SIZE; v++){
			loadbufferH_ddr0(kvdram, sourcebufferH0, (i_kvs + 0 * VECTOR_SIZE * LSRCKVBUFFERSZ_KVS) + (v * LSRCKVBUFFERSZ_KVS), globalparams, llopparams, partitionparams);
	
			extractcapsules_ddr0(sourcebufferH0, resultbufferS0, v, globalparams, llopparams, partitionparams);
		}
		savepartialbuffer_ddr0(kvdram, resultbufferS0, llopparams.baseaddr_capsule_kvs, ((((i_kvs + 0 * VECTOR_SIZE * LSRCKVBUFFERSZ_KVS) / VECTOR_SIZE) / LSRCKVBUFFERSZ_KVS) * NUM_PARTITIONS), NUM_PARTITIONS, COLLECTSTATSPROCESS, globalparams, llopparams, partitionparams);
	}
	
	SPREAD_LOOP2: for(unsigned int i_kvs=partitionparams.begin_kvs; i_kvs<(partitionparams.begin_kvs + partitionparams.size_kvs); i_kvs+=NUMWORKERS * VECTOR_SIZE * LSRCKVBUFFERSZ_KVS){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=spreadloop_tripcount avg=spreadloop_tripcount
		#ifdef _DEBUGMODE_KERNELPRINTS
		print5("spread_ddr0:: i_kvs", "i_kvs end", "begin_kvs", "size_kvs", "skip size", i_kvs, (partitionparams.begin_kvs + partitionparams.size_kvs), partitionparams.begin_kvs, partitionparams.size_kvs, NUMWORKERS * VECTOR_SIZE * LSRCKVBUFFERSZ_KVS);
		#endif
		
		loadcapsulestatsS_ddr0(kvdram, bufferstats00, bufferstats01, bufferstats02, bufferstats03, bufferstats04, bufferstats05, bufferstats06, bufferstats07,  getglobalstatoffset_kvs((i_kvs + 0 * VECTOR_SIZE * LSRCKVBUFFERSZ_KVS)), globalparams, llopparams, partitionparams);
		SPREAD_LOOP2B: for(vector_type v=0; v<VECTOR_SIZE; v++){
			travstate_t travstate; travstate.i_kvs = i_kvs + (v * LSRCKVBUFFERSZ_KVS); travstate.iend_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			loadbufferV_ddr0(kvdram, sourcebufferV00,sourcebufferV01,sourcebufferV02,sourcebufferV03,sourcebufferV04,sourcebufferV05,sourcebufferV06,sourcebufferV07, (i_kvs + 0 * VECTOR_SIZE * LSRCKVBUFFERSZ_KVS) + (v * LSRCKVBUFFERSZ_KVS), globalparams, llopparams, partitionparams);
			
			spreadkvs_ddr0_v0(sourcebufferV00, resultbuffer00, bufferstats00, travstate, globalparams, llopparams, partitionparams);
			spreadkvs_ddr0_v1(sourcebufferV01, resultbuffer01, bufferstats01, travstate, globalparams, llopparams, partitionparams);
			spreadkvs_ddr0_v2(sourcebufferV02, resultbuffer02, bufferstats02, travstate, globalparams, llopparams, partitionparams);
			spreadkvs_ddr0_v3(sourcebufferV03, resultbuffer03, bufferstats03, travstate, globalparams, llopparams, partitionparams);
			spreadkvs_ddr0_v4(sourcebufferV04, resultbuffer04, bufferstats04, travstate, globalparams, llopparams, partitionparams);
			spreadkvs_ddr0_v5(sourcebufferV05, resultbuffer05, bufferstats05, travstate, globalparams, llopparams, partitionparams);
			spreadkvs_ddr0_v6(sourcebufferV06, resultbuffer06, bufferstats06, travstate, globalparams, llopparams, partitionparams);
			spreadkvs_ddr0_v7(sourcebufferV07, resultbuffer07, bufferstats07, travstate, globalparams, llopparams, partitionparams);
		}
		savebuffer_ddr0(kvdram, resultbuffer00, bufferstats00, kvdeststats_tmp, llopparams.destbaseaddr_kvs, globalparams, llopparams, partitionparams);
		savebuffer_ddr0(kvdram, resultbuffer01, bufferstats01, kvdeststats_tmp, llopparams.destbaseaddr_kvs, globalparams, llopparams, partitionparams);
		savebuffer_ddr0(kvdram, resultbuffer02, bufferstats02, kvdeststats_tmp, llopparams.destbaseaddr_kvs, globalparams, llopparams, partitionparams);
		savebuffer_ddr0(kvdram, resultbuffer03, bufferstats03, kvdeststats_tmp, llopparams.destbaseaddr_kvs, globalparams, llopparams, partitionparams);
		savebuffer_ddr0(kvdram, resultbuffer04, bufferstats04, kvdeststats_tmp, llopparams.destbaseaddr_kvs, globalparams, llopparams, partitionparams);
		savebuffer_ddr0(kvdram, resultbuffer05, bufferstats05, kvdeststats_tmp, llopparams.destbaseaddr_kvs, globalparams, llopparams, partitionparams);
		savebuffer_ddr0(kvdram, resultbuffer06, bufferstats06, kvdeststats_tmp, llopparams.destbaseaddr_kvs, globalparams, llopparams, partitionparams);
		savebuffer_ddr0(kvdram, resultbuffer07, bufferstats07, kvdeststats_tmp, llopparams.destbaseaddr_kvs, globalparams, llopparams, partitionparams);
	}
	return;
}
void processCLOP_ddr0(uint512_dt * kvdram, uint512_dt kvstatsbuffer[KVSTATS_SIZE_KVS], globalparams_t globalparams, clopparams_t llopparams){
	keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
	#ifdef _DEBUGMODE_KERNELPRINTS
	print4("processCLOP_ddr0:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
	#endif
	PROCESSCLOP_LOOP1: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=1 avg=1
		partitionparams_t partitionparams;
		llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
		
		loaddeststats(kvstatsbuffer, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.finalnumpartitions);		
		PROCESSPARTITION_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			#pragma HLS PIPELINE II=1
			kvdeststats_tmp[p].value = 0;
		}
		#ifdef _DEBUGMODE_KERNELPRINTS
		printkeyvalues("processCLOP_ddr0", kvdeststats_tmp, NUM_PARTITIONS);
		#endif
		
		keyvalue_t sourcestat = getkeyvalue(kvstatsbuffer, llopparams.nextsourceoffset_partition, KVSTATS_SIZE_KVS);
		partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
		partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
		partitionparams.step_kvs = LSRCKVBUFFERSZ_KVS;
		
		processpartition_ddr0(kvdram, kvdeststats_tmp, globalparams, llopparams, partitionparams);
	
		storedeststats(kvstatsbuffer, kvdeststats_tmp, llopparams.nextdestoffset_partition, llopparams.currentLOP, globalparams.finalnumpartitions); 
		llopparams.nextsourceoffset_partition += llopparams.sourceskipsize_partition;
		llopparams.nextdestoffset_partition += (llopparams.destskipsize_partition * NUM_PARTITIONS);
		llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv;
	}
}
void partitionphase_ddr0(uint512_dt * kvdram, globalparams_t globalparams){
	uint512_dt kvstatsbuffer[KVSTATS_SIZE_KVS];
	loadstatsbuffer_ddr0(kvdram, kvstatsbuffer, (KVDRAMSZ_KVS + CAPSULEDRAMSZ_KVS));
	unsigned int partitionphase_looptripcount = TREE_DEPTH; // NB: debugging purposes only
	
	PROCESSALL_LOOP1: for(unsigned int currentLOP=1; currentLOP<globalparams.steps + 1; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=partitionphase_looptripcount avg=partitionphase_looptripcount
		#ifdef _DEBUGMODE_KERNELPRINTS3
		cout<<endl;
		print5("partitionphase_ddr0:: currentLOP", "size", "numsourcepartitions", "num_destpartitions", "destskipsize_partition", currentLOP, globalparams.steps + 1, get_num_source_partitions(currentLOP), get_num_source_partitions(currentLOP) * NUM_PARTITIONS, getdestskipsize(currentLOP, globalparams.finalnumpartitions));
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS
		printkeyvalues("[BEFORE] partitionphase_ddr0", (keyvalue_t *)kvstatsbuffer, KVSTATS_SIZE);
		#endif
		clopparams_t llopparams;
		llopparams.sourcebaseaddr_kvs = 0;
		llopparams.destbaseaddr_kvs = 0;
		if((currentLOP % 2) == 1){ llopparams.sourcebaseaddr_kvs = 0; llopparams.destbaseaddr_kvs = WORKLOADDRAMSZ_KVS; } 
		else { llopparams.sourcebaseaddr_kvs = WORKLOADDRAMSZ_KVS; llopparams.destbaseaddr_kvs = 0; }	
		llopparams.baseaddr_capsule_kvs = KVDRAMSZ_KVS;
		llopparams.baseaddr_stats_kvs = KVDRAMSZ_KVS + CAPSULEDRAMSZ_KVS;
		llopparams.num_source_partitions = get_num_source_partitions(currentLOP);
		llopparams.nextsourceoffset_partition = 0;
		llopparams.nextdestoffset_partition = 0;
		llopparams.nextsourceoffset_kv = 0;
		llopparams.nextdestoffset_kv = 0;
		llopparams.sourceskipsize_partition = getsourceskipsize(currentLOP, globalparams.finalnumpartitions);
		llopparams.destskipsize_partition = getdestskipsize(currentLOP, globalparams.finalnumpartitions);
		llopparams.sourceskipsize_kv = getsourceskipsize(currentLOP, GETKVDATA_RANGE_PERSSDPARTITION());
		llopparams.destskipsize_kv = getdestskipsize(currentLOP, GETKVDATA_RANGE_PERSSDPARTITION());
		llopparams.destpartitionrange = (1 << (GETKVDATA_RANGE_PERSSDPARTITION_POW() - (NUM_PARTITIONS_POW * currentLOP)));
		llopparams.currentLOP = currentLOP;
		llopparams.upperlimit = 0;
		
		processCLOP_ddr0(kvdram, kvstatsbuffer, globalparams, llopparams);
		#ifdef _DEBUGMODE_KERNELPRINTS
		printkeyvalues("[AFTER] partitionphase_ddr0", (keyvalue_t *)kvstatsbuffer, KVSTATS_SIZE);
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS3
		getvaluecount("[AFTER] partitionphase_ddr0", (keyvalue_t *)kvstatsbuffer, KVSTATS_SIZE);
		#endif
	}
	savestatsbuffer_ddr0(kvdram, kvstatsbuffer, (KVDRAMSZ_KVS + CAPSULEDRAMSZ_KVS));
	return;
}

void reducepartition_ddr0(uint512_dt * kvdram_source, uint512_dt * kvdram_dest, globalparams_t globalparams, clopparams_t llopparams, partitionparams_t partitionparams){
	unsigned int reduceloop_tripcount = (KVDATA_RANGE_PERSSDPARTITION_KVS / NUM_LLOPS) / LSRCKVBUFFERSZ_KVS; // NB: for debugging purposes only
	
	keyvalue_t sourcebufferV00[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV01[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV02[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV03[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV04[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV05[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV06[LSRCKVBUFFERSZ_KVS];
	keyvalue_t sourcebufferV07[LSRCKVBUFFERSZ_KVS];
	
	uint512_dt resultbuffer00[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer01[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer02[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer03[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer04[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer05[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer06[LDESTKVBUFFERSZ_KVS];
	uint512_dt resultbuffer07[LDESTKVBUFFERSZ_KVS];
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	print5("reducepartition_ddr0:: begin", "size", "step", "currentLOP", "range_per_destpartition", partitionparams.begin_kvs * VECTOR_SIZE, partitionparams.size_kvs * VECTOR_SIZE, partitionparams.step_kvs * VECTOR_SIZE, llopparams.currentLOP, KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, llopparams.currentLOP));
	#endif
	
	REDUCE_LOOP1: for(unsigned int i_kvs=partitionparams.begin_kvs; i_kvs<(partitionparams.begin_kvs + partitionparams.size_kvs); i_kvs+=NUMWORKERS * LSRCKVBUFFERSZ_KVS){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=reduceloop_tripcount avg=reduceloop_tripcount
		#ifdef _DEBUGMODE_KERNELPRINTS
		print5("reduce_ddr0:: i", "i end", "begin", "size", "skip size", i_kvs * VECTOR_SIZE, (partitionparams.begin_kvs + partitionparams.size_kvs) * VECTOR_SIZE, partitionparams.begin_kvs * VECTOR_SIZE, partitionparams.size_kvs * VECTOR_SIZE, NUMWORKERS * LSRCKVBUFFERSZ);
		#endif
		
		travstate_t travstate; travstate.i_kvs = i_kvs; travstate.iend_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
		loadbufferV_ddr0(kvdram_source, sourcebufferV00,sourcebufferV01,sourcebufferV02,sourcebufferV03,sourcebufferV04,sourcebufferV05,sourcebufferV06,sourcebufferV07, (i_kvs + 0 * LSRCKVBUFFERSZ_KVS), globalparams, llopparams, partitionparams);
		
			
		reduce_ddr0_v0(sourcebufferV00, resultbuffer00, travstate, globalparams, llopparams, partitionparams);
			
		reduce_ddr0_v1(sourcebufferV01, resultbuffer01, travstate, globalparams, llopparams, partitionparams);
			
		reduce_ddr0_v2(sourcebufferV02, resultbuffer02, travstate, globalparams, llopparams, partitionparams);
			
		reduce_ddr0_v3(sourcebufferV03, resultbuffer03, travstate, globalparams, llopparams, partitionparams);
			
		reduce_ddr0_v4(sourcebufferV04, resultbuffer04, travstate, globalparams, llopparams, partitionparams);
			
		reduce_ddr0_v5(sourcebufferV05, resultbuffer05, travstate, globalparams, llopparams, partitionparams);
			
		reduce_ddr0_v6(sourcebufferV06, resultbuffer06, travstate, globalparams, llopparams, partitionparams);
			
		reduce_ddr0_v7(sourcebufferV07, resultbuffer07, travstate, globalparams, llopparams, partitionparams);
	}
	savebuffer_ddr0(kvdram_dest, resultbuffer00, llopparams.destbaseaddr_kvs, llopparams.nextsourceoffset_kv / VECTOR_SIZE, globalparams, llopparams, partitionparams);
	savebuffer_ddr0(kvdram_dest, resultbuffer01, llopparams.destbaseaddr_kvs, llopparams.nextsourceoffset_kv / VECTOR_SIZE, globalparams, llopparams, partitionparams);
	savebuffer_ddr0(kvdram_dest, resultbuffer02, llopparams.destbaseaddr_kvs, llopparams.nextsourceoffset_kv / VECTOR_SIZE, globalparams, llopparams, partitionparams);
	savebuffer_ddr0(kvdram_dest, resultbuffer03, llopparams.destbaseaddr_kvs, llopparams.nextsourceoffset_kv / VECTOR_SIZE, globalparams, llopparams, partitionparams);
	savebuffer_ddr0(kvdram_dest, resultbuffer04, llopparams.destbaseaddr_kvs, llopparams.nextsourceoffset_kv / VECTOR_SIZE, globalparams, llopparams, partitionparams);
	savebuffer_ddr0(kvdram_dest, resultbuffer05, llopparams.destbaseaddr_kvs, llopparams.nextsourceoffset_kv / VECTOR_SIZE, globalparams, llopparams, partitionparams);
	savebuffer_ddr0(kvdram_dest, resultbuffer06, llopparams.destbaseaddr_kvs, llopparams.nextsourceoffset_kv / VECTOR_SIZE, globalparams, llopparams, partitionparams);
	savebuffer_ddr0(kvdram_dest, resultbuffer07, llopparams.destbaseaddr_kvs, llopparams.nextsourceoffset_kv / VECTOR_SIZE, globalparams, llopparams, partitionparams);
	return;
}
void reduceLLOP_ddr0(uint512_dt * kvdram_source, uint512_dt * kvdram_dest, uint512_dt kvstatsbuffer[KVSTATS_SIZE_KVS], globalparams_t globalparams, clopparams_t llopparams){
	#ifdef _DEBUGMODE_KERNELPRINTS
	print4("processCLOP_ddr0:: num_source_partitions", "num_destpartitions", "NAp", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, NAp, NAp);
	#endif	
	unsigned int reduceLLOPloop_tripcount = NUM_LLOPS; // NB: for debugging purposes only

	PROCESSCLOP_LOOP1: for(unsigned int source_partition=0; source_partition<(1 << (NUM_PARTITIONS_POW * TREE_DEPTH)); source_partition++){ // FIXME
		#pragma HLS LOOP_TRIPCOUNT min=0 max=reduceLLOPloop_tripcount avg=reduceLLOPloop_tripcount
		partitionparams_t partitionparams;
		llopparams.upperlimit = 0;
		llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv;
		
		keyvalue_t sourcestat = getkeyvalue(kvstatsbuffer, llopparams.nextsourceoffset_partition, KVSTATS_SIZE_KVS);
		partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
		partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE; // FIXME. reason for slight difference
		partitionparams.step_kvs = LSRCKVBUFFERSZ_KVS;
		
		reducepartition_ddr0(kvdram_source, kvdram_dest, globalparams, llopparams, partitionparams);
		
		llopparams.nextsourceoffset_partition += llopparams.sourceskipsize_partition;
		llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv;
	}
}
void reducephase_ddr0(uint512_dt * kvdram_source, uint512_dt * kvdram_dest, globalparams_t globalparams){
	uint512_dt kvstatsbuffer[KVSTATS_SIZE_KVS];
	loadstatsbuffer_ddr0(kvdram_source, kvstatsbuffer, (KVDRAMSZ_KVS + CAPSULEDRAMSZ_KVS));
	#ifdef _DEBUGMODE_KERNELPRINTS
	printkeyvalues("reducephase_ddr0", (keyvalue_t *)kvstatsbuffer, KVSTATS_SIZE);
	#endif
	
	unsigned int currentLOP = TREE_DEPTH + 1;	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<endl;
	print4("reducephase_ddr0:: currentLOP", "size", "numsourcepartitions", "num_destpartitions", currentLOP, globalparams.steps + 1, get_num_source_partitions(currentLOP), get_num_source_partitions(currentLOP) * NUM_PARTITIONS);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS
	printkeyvalues("[BEFORE] reducephase_ddr0", (keyvalue_t *)kvstatsbuffer, 0);
	#endif
	clopparams_t llopparams;
	if((currentLOP % 2) == 1){ llopparams.sourcebaseaddr_kvs = 0; } 
	else { llopparams.sourcebaseaddr_kvs = WORKLOADDRAMSZ_KVS; }
	llopparams.destbaseaddr_kvs = 0;	
	llopparams.baseaddr_capsule_kvs = KVDRAMSZ_KVS;
	llopparams.baseaddr_stats_kvs = KVDRAMSZ_KVS + CAPSULEDRAMSZ_KVS;
	llopparams.num_source_partitions = get_num_source_partitions(currentLOP);
	llopparams.nextsourceoffset_partition = 0;
	llopparams.nextdestoffset_partition = 0;
	llopparams.nextsourceoffset_kv = 0;
	llopparams.nextdestoffset_kv = 0;
	llopparams.sourceskipsize_partition = getsourceskipsize(currentLOP, globalparams.finalnumpartitions);
	llopparams.destskipsize_partition = getdestskipsize(currentLOP, globalparams.finalnumpartitions);
	llopparams.sourceskipsize_kv = getsourceskipsize(currentLOP, GETKVDATA_RANGE_PERSSDPARTITION());
	llopparams.destskipsize_kv = getdestskipsize(currentLOP, GETKVDATA_RANGE_PERSSDPARTITION());
	llopparams.destpartitionrange = (1 << (GETKVDATA_RANGE_PERSSDPARTITION_POW() - (NUM_PARTITIONS_POW * currentLOP)));
	llopparams.currentLOP = currentLOP;
	llopparams.upperlimit = 0;
	
	reduceLLOP_ddr0(kvdram_source, kvdram_dest, kvstatsbuffer, globalparams, llopparams);
	#ifdef _DEBUGMODE_KERNELPRINTS
	printkeyvalues("[AFTER] reducephase_ddr0", (keyvalue_t *)kvstatsbuffer, 0);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS3
	getvaluecount("[AFTER] reducephase_ddr0", (keyvalue_t *)kvstatsbuffer, KVSTATS_SIZE);
	#endif 
	return;
}

extern "C" {
void topkernel(		
uint512_dt * kvdram_sourceA, uint512_dt * kvdram_destA
        ){
#pragma HLS INTERFACE m_axi port=kvdram_sourceA  offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=kvdram_destA  offset=slave bundle=gmem1

#pragma HLS INTERFACE s_axilite port=kvdram_sourceA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvdram_destA  bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
	
	#ifdef _LDEBUGMODE_HEADER
	printparameters();
	#endif
	
	globalparams_t globalparams;
	globalparams.steps = TREE_DEPTH; // 1; // TREE_DEPTH;
	globalparams.finalnumpartitions = (1 << (NUM_PARTITIONS_POW * TREE_DEPTH));	
	globalparams.paddedkvdatabatchsz_kvs = KVDATA_BATCHSIZE_KVS;
	globalparams.capsulebatchsz_kvs = CAPSULESTATSBATCHSZ_KVS;
	globalparams.vbegin = 0;
	
	partitionphase_ddr0(kvdram_sourceA, globalparams);
	reducephase_ddr0(kvdram_sourceA, kvdram_destA, globalparams);
	
	#ifdef _LDEBUGMODE_HEADER
	cout<<endl<<"=== topkernel ENDED. printing summary and out messages === "<<endl;
	printglobalvars();
	printparameters();
	#endif
	return;
}
}

