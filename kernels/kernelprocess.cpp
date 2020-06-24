#include "/home/oj2zf/Documents/ActsOfAGraph/debugger/kernel_debugger.h"
#ifndef FPGA_IMPL
#include "/home/oj2zf/Documents/ActsOfAGraph/kernels/kernelprocess.h"
#endif 
#ifndef FPGA_IMPL
#include <chrono>
#endif 
using namespace std;
// #define OOO
#define INVALIDDATA 0

#ifdef _DEBUGMODE_STATS
int kvpairsread0;
int kvpairswritten0;
int kvpairsread0_transfers;
int kvpairswritten0_transfers;
#ifdef _BUFFERB4WRITE
int kvpairswritten_bram2tmp0;
int kvpairswritten_tmp2dram0;
int kvpairswritten_bram2tmp0_transfers;
int kvpairswritten_tmp2dram0_transfers;
#endif
int kvpairswritten_interm2dest_parallel0;
int kvpairswritten_interm2dest_seqential0;
int INapplyphase;
edge_t numedgesprocessedPE;
vertex_t numverticesprocessedPE;
#endif
#ifdef _DEBUGMODE_CHECKS2
unsigned int state_i_kvs;
unsigned int state_ikvs_sz;
unsigned int state_moduleid;
unsigned int state_kvbuffer1_kvs_tracker;
#endif 

/// Utility Functions
void kernelprocess::printstate(){
	#ifdef _DEBUGMODE_CHECKS2
	cout<<">>> kernelprocess::printstate: "<<endl;
	cout<<">>> state_i_kvs: "<<state_i_kvs<<endl;
	cout<<">>> state_ikvs_sz: "<<state_ikvs_sz<<endl;
	cout<<">>> state_moduleid: "<<state_moduleid<<endl;
	cout<<">>> state_kvbuffer1_kvs_tracker: "<<state_kvbuffer1_kvs_tracker<<endl;
	cout<<">>> state_moduleid: "<<state_moduleid<<endl;
	cout<<">>> state_moduleid: "<<state_moduleid<<endl;
	cout<<">>> KVBUFFERSZ_KVS: "<<KVBUFFERSZ_KVS<<endl;
	#endif
}

unsigned int kernelprocess::ceildiv(unsigned int val1, unsigned int val2){
	return (val1 + (val2 - 1)) / val2;
}

unsigned int kernelprocess::min8(unsigned int val0, unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4, unsigned int val5, unsigned int val6, unsigned int val7){
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

unsigned int kernelprocess::min(unsigned int val1, unsigned int val2){
	if(val1 < val2){ return val1; }
	else { return val2; }
}
			
unsigned int kernelprocess::max(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}

unsigned int kernelprocess::minorzero(int val1){
	if(val1 > 0){ return val1; }
	else { return 0; }
}

unsigned int kernelprocess::allignup_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}

unsigned int kernelprocess::alligndown_KV(unsigned int val){
	unsigned int fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}

vertex_t kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(unsigned int ssdpartitonid){
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
		std::cout<<"Kernel::GETKVDATA_RANGE_PERSSDPARTITION should never get here. kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
		#endif
	}
	#endif 
	return range;
}

vertex_t kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION_POW(unsigned int ssdpartitonid){
	return KVDATA_RANGE_PERSSDPARTITION_POW;
}

vertex_t kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(unsigned int ssdpartitonid){
	return APPLYVERTEXBUFFERSZ_PERSSDPARTITION;
}

vertex_t kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(unsigned int ssdpartitonid){
	return APPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS;
}

unsigned int kernelprocess::GETTREEDEPTH(unsigned int ssdpartitonid){
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
		std::cout<<"Kernel:: kernelprocess::GETTREEDEPTH should never get here.  kernelprocess::GETTREEDEPTH 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
		#endif
	}
	#endif 
	return depth;
}

unsigned int kernelprocess::hashfunc(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, vertex_t range){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::kernelprocess::hashfunc: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", range: "<<range<<std::endl;
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
		std::cout<<"Kernel::kernelprocess::hashfunc should never get here. kernelprocess::hashfunc 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_CHECKS2
	if(hashedval >= NUM_PARTITIONS){
		std::cout<<"Kernel::kernelprocess::hashfunc:: out-of-bounds 45. hashedval: "<<hashedval<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", range: "<<range<<", currentLOP: "<<currentLOP<<std::endl;
		exit(EXIT_FAILURE);
	}
	#endif
	if(hashedval >= FORCDFINISH_NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}

unsigned int kernelprocess::hashfunc2(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int ssdpartitonid){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::kernelprocess::hashfunc2: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<std::endl;
	#endif

	unsigned int hashedval = 0;
	if(currentLOP == 1){
		hashedval = (kventry.key - upperlimit) / (kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / NUM_PARTITIONS);		
	} else if(currentLOP == 2){
		hashedval = (kventry.key - upperlimit) / ((kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else if(currentLOP == 3){
		hashedval = (kventry.key - upperlimit) / (((kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else if(currentLOP == 4){
		hashedval = (kventry.key - upperlimit) / ((((kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else if(currentLOP == 5){
		hashedval = (kventry.key - upperlimit) / (((((kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS) / NUM_PARTITIONS);
	} else {
		hashedval = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		std::cout<<"Kernel::kernelprocess::hashfunc2 should never get here. kernelprocess::hashfunc2 45 "<<std::endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_CHECKS2
	if(hashedval >= NUM_PARTITIONS){
		std::cout<<"Kernel::kernelprocess::hashfunc2:: out-of-bounds 45. hashedval: "<<hashedval<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<", currentLOP: "<<currentLOP<<std::endl;
		exit(EXIT_FAILURE);
	}
	#endif
	if(hashedval >= FORCDFINISH_NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}

unsigned int kernelprocess::hashfunc3(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int ssdpartitonid){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::kernelprocess::hashfunc3: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<std::endl;
	#endif

	unsigned int hashedval = 0;
	hashedval = (kventry.key - upperlimit) / (kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / (1 << (NUM_PARTITIONS_POW * currentLOP)));
	#ifdef _DEBUGMODE_CHECKS2
	if(hashedval >= NUM_PARTITIONS){
		std::cout<<"Kernel::kernelprocess::hashfunc3:: out-of-bounds 45. hashedval: "<<hashedval<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<", currentLOP: "<<currentLOP<<std::endl;
		exit(EXIT_FAILURE);
	}
	#endif
	if(hashedval >= FORCDFINISH_NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}

unsigned int kernelprocess::hashfunc4(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int ssdpartitonid){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::kernelprocess::hashfunc4: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<std::endl;
	#endif
	#ifdef _DEBUGMODE_CHECKS2
	if(kventry.key < upperlimit){
		if(kventry.key != INVALIDDATA){ std::cout<<"Kernel::kernelprocess::hashfunc4:: out-of-bounds 44. kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<", currentLOP: "<<currentLOP<<endl; }
		if(kventry.key != INVALIDDATA){ exit(EXIT_FAILURE); }
		// exit(EXIT_FAILURE);
	}
	#endif

	unsigned int hashedval = 0;
	hashedval = ((kventry.key - upperlimit) >> (kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION_POW(ssdpartitonid) - (NUM_PARTITIONS_POW * currentLOP)));
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::kernelprocess::hashfunc4: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<", hashedval: "<<hashedval<<", currentLOP: "<<currentLOP<<", kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION_POW("<<ssdpartitonid<<"): "<<kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION_POW(ssdpartitonid)<<std::endl;
	#endif
	
	#ifdef _DEBUGMODE_CHECKS2
	if(hashedval >= NUM_PARTITIONS){
		if(kventry.key != INVALIDDATA){ std::cout<<"Kernel::kernelprocess::hashfunc4:: out-of-bounds 45. hashedval: "<<hashedval<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", ssdpartitonid: "<<ssdpartitonid<<", currentLOP: "<<currentLOP<<", kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION_POW("<<ssdpartitonid<<"): "<<kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION_POW(ssdpartitonid)<<", (NUM_PARTITIONS_POW * currentLOP): "<<(NUM_PARTITIONS_POW * currentLOP)<<", state_i_kvs: "<<state_i_kvs<<", state_ikvs_sz: "<<state_ikvs_sz<<std::endl;	}
		if(kventry.key != INVALIDDATA){ kernelprocess::printstate(); }
		if(kventry.key != INVALIDDATA){ exit(EXIT_FAILURE); }
		// exit(EXIT_FAILURE);
	}
	#endif
	if(hashedval >= FORCDFINISH_NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}

unsigned int kernelprocess::hashfunc5(unsigned int currentLOP, keyvalue_t kventry, vertex_t upperlimit, unsigned int range){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::kernelprocess::hashfunc5: kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<std::endl;
	#endif

	unsigned int hashedval = 0;
	#pragma HLS RESOURCE variable=hashedval core=DivnS
	hashedval = (kventry.key - upperlimit) / range;
	#ifdef _DEBUGMODE_CHECKS2
	if(hashedval >= NUM_PARTITIONS){
		std::cout<<"Kernel::kernelprocess::hashfunc5:: out-of-bounds 45. hashedval: "<<hashedval<<", NUM_PARTITIONS: "<<NUM_PARTITIONS<<", kventry.key: "<<kventry.key<<", upperlimit: "<<upperlimit<<", currentLOP: "<<currentLOP<<std::endl;
		exit(EXIT_FAILURE);
	}
	#endif
	if(hashedval >= FORCDFINISH_NUM_PARTITIONS){ hashedval = 0; }
	return hashedval;
}

keyvalue_t kernelprocess::getkventry(int addr, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
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

keyvalue_t kernelprocess::getkventryII(int addr, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	if(addr >= FORCDFINISH_KVSRCBUFFERSZ){ addr = 0; }
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(idy, KVSRCBUFFERSZ_KVS, "kernelprocess::getkventryII:: out-of-bounds seen at kernelprocess::getkventryII 5", 0, 0, 0);
	checkforoutofbounds_kernel(idx, VECTOR_SIZE, "kernelprocess::getkventryII:: out-of-bounds seen at kernelprocess::getkventryII 6", 0, 0, 0);
	#endif
	#ifdef _WIDEWORD
	keyvalue_t kventry;
	unsigned int ihi = ((VECTOR_SIZE - idx) * 32) - 1;
	unsigned int ilo = ihi - 31;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(ihi, DATAWIDTH, "kernelprocess::getkventryII:: out-of-bounds seen at kernelprocess::getkventryII 7", 0, 0, 0);
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
		std::cout<<"kernelprocess::getkventryII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return kventry;
	#else
	return kvbuffer[idy].data[idx];
	#endif
}

keyvalue_t kernelprocess::getkventryIII(int addr, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#pragma HLS INLINE // "unlike kernelprocess::getkventryIII, this is inlined"
	if(addr >= FORCDFINISH_KVSRCBUFFERSZ){ addr = 0; }
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(idy, KVSRCBUFFERSZ_KVS, "kernelprocess::getkventryIII:: out-of-bounds seen at kernelprocess::getkventryIII 5", 0, 0, 0);
	checkforoutofbounds_kernel(idx, VECTOR_SIZE, "kernelprocess::getkventryIII:: out-of-bounds seen at kernelprocess::getkventryIII 6", 0, 0, 0);
	#endif
	#ifdef _WIDEWORD
	keyvalue_t kventry;
	unsigned int ihi = ((VECTOR_SIZE - idx) * 32) - 1;
	unsigned int ilo = ihi - 31;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(ihi, DATAWIDTH, "kernelprocess::getkventryIII:: out-of-bounds seen at kernelprocess::getkventryIII 7", 0, 0, 0);
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
		std::cout<<"kernelprocess::getkventryIII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return kventry;
	#else
	return kvbuffer[idy].data[idx];
	#endif
}

keyvalue_t kernelprocess::getkventryIV(int addr, uint512_dt kvbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#pragma HLS INLINE
	if(addr >= APPLYVERTEXBUFFERSZ){ addr = 0; }
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(idy, APPLYVERTEXBUFFERSZ_KVS, "kernelprocess::getkventryIV:: out-of-bounds seen at kernelprocess::getkventryIV 5", 0, 0, 0);
	checkforoutofbounds_kernel(idx, VECTOR_SIZE, "kernelprocess::getkventryIV:: out-of-bounds seen at kernelprocess::getkventryIV 6", 0, 0, 0);
	#endif
	#ifdef _WIDEWORD
	keyvalue_t kventry;
	unsigned int ihi = ((VECTOR_SIZE - idx) * 32) - 1;
	unsigned int ilo = ihi - 31;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(ihi, DATAWIDTH, "kernelprocess::getkventryIV:: out-of-bounds seen at kernelprocess::getkventryIV 7", 0, 0, 0);
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
		std::cout<<"kernelprocess::getkventryIV:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return kventry;
	#else
	return kvbuffer[idy].data[idx];
	#endif
}

void kernelprocess::setkventry(uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS], unsigned int addr, keyvalue_t kventry){
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

void kernelprocess::setkventryII(uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS], unsigned int addr, keyvalue_t kventry){
	if(addr >= FORCDFINISH_KVSRCBUFFERSZ){ addr = 0; }
	unsigned int idy = addr / VECTOR_SIZE;
	unsigned int idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(idy, KVSRCBUFFERSZ_KVS, "kernelprocess::setkventryII:: out-of-bounds seen at kernelprocess::setkventryII 5", 0, 0, 0);
	checkforoutofbounds_kernel(idx, VECTOR_SIZE, "kernelprocess::setkventryII:: out-of-bounds seen at kernelprocess::setkventryII 6", 0, 0, 0);
	#endif
	#ifdef _WIDEWORD
	unsigned int ihi = ((VECTOR_SIZE - idx) * 32) - 1;
	unsigned int ilo = ihi - 31;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(ihi, DATAWIDTH, "kernelprocess::setkventryII:: out-of-bounds seen at kernelprocess::setkventryII 7", 0, 0, 0);
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
		std::cout<<"kernelprocess::setkventryII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	#else
	kvbuffer[idy].data[idx] = kventry;
	#endif
	return;
}

void kernelprocess::setkventryIII(
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
		std::cout<<"kernelprocess::setkventryIII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return;
}

void kernelprocess::setkventry4(
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
		std::cout<<"kernelprocess::setkventryIII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return;
}

keyvalue_t kernelprocess::getkventry3(uint512_dt tmpV1, unsigned int idx){
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(idx, VECTOR_SIZE, "kernelprocess::getkventry3:: out-of-bounds seen at kernelprocess::getkventry3 6", 0, 0, 0);
	#endif
	#ifdef _WIDEWORD
	keyvalue_t kventry;
	unsigned int ihi = ((VECTOR_SIZE - idx) * 32) - 1;
	unsigned int ilo = ihi - 31;
	#ifdef _DEBUGMODE_CHECKS
	checkforoutofbounds_kernel(ihi, DATAWIDTH, "kernelprocess::getkventry3:: out-of-bounds seen at kernelprocess::getkventry3 7", 0, 0, 0);
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
		std::cout<<"kernelprocess::getkventry3:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return kventry;
	#else
	return tmpV1.data[idx];
	#endif
}

keyvalue_t kernelprocess::getkventry4(
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
		std::cout<<"kernelprocess::setkventryIII:: out-of-bounds 45. "<<std::endl;
		exit(EXIT_FAILURE);
		#endif 
	}
	return kventry;
}

unsigned int kernelprocess::getsourceskipsize(unsigned int currentLOP, unsigned int param){
	unsigned int treedepth = TREEDEPTH_FORSSDPARTITION0;
	unsigned int _finalnumpartitions = param;
	for(unsigned int i=0; i<(currentLOP-1); i++){ // FIXME?
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		_finalnumpartitions = _finalnumpartitions / NUM_PARTITIONS;
	}
	return _finalnumpartitions;
}

unsigned int kernelprocess::getdestskipsize(unsigned int currentLOP, unsigned int param){
	unsigned int treedepth = TREEDEPTH_FORSSDPARTITION0; // NB: for debugging purposes only
	unsigned int _finalnumpartitions = param / NUM_PARTITIONS;
	for(unsigned int i=0; i<(currentLOP-1); i++){ // FIXME?
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		_finalnumpartitions = _finalnumpartitions / NUM_PARTITIONS;
	}
	return _finalnumpartitions;
}

void kernelprocess::loaddeststats(metadata_t * kvstats, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t finalnumpartitions){
	unsigned int skipsize = kernelprocess::getdestskipsize(currentLOP, finalnumpartitions);
	unsigned int nxtpos = partitionoffset;
	#ifdef _DEBUGMODE_PRINTS
	cout<<"kernelprocess::loaddeststats:: currentLOP: "<<currentLOP<<", skipsize: "<<skipsize<<", partitionoffset: "<<partitionoffset<<", finalnumpartitions: "<<finalnumpartitions<<endl;
	#endif
	LOADDESTSTATS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel(p, NUM_PARTITIONS, "kernelprocess::loaddeststats:: out-of-bounds seen at kernelprocess::loaddeststats 5", 0, 0, 0);
		checkforoutofbounds_kernel((skipsize * p), KVSTATS_SIZE, "kernelprocess::loaddeststats:: out-of-bounds seen at kernelprocess::loaddeststats 6", 0, 0, 0);
		#endif
		
		/* kvdeststats_tmp[p] = kvstats[((partitionoffset + (p * skipsize)) % FORCDFINISH_KVSTATS_SIZE)];  */
		/* kvdeststats_tmp[p] = kvstats[nxtpos]; 
		nxtpos += skipsize; */		
		kvdeststats_tmp[p] = kvstats[(nxtpos % FORCDFINISH_KVSTATS_SIZE)]; 
		nxtpos += skipsize;
		
		#ifdef _DEBUGMODE_PRINTS
		cout<<"kernelprocess::loaddeststats:: kvdeststats_tmp["<<p<<"].offset: "<<kvdeststats_tmp[p].offset<<", kvdeststats_tmp["<<p<<"].size: "<<kvdeststats_tmp[p].size<<", ";
		cout<<"kernelprocess::loaddeststats:: kvstats["<<(partitionoffset + (p * skipsize))<<"].offset: "<<kvstats[(partitionoffset + (p * skipsize))].offset<<", kvstats["<<(partitionoffset + (p * skipsize))<<"].size: "<<kvstats[(partitionoffset + (p * skipsize))].size;
		cout<<endl;
		#endif
	}
	LOADDESTSTATS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		kvdeststats_tmp[p].size = 0;
	}
	return;
}

void kernelprocess::storedeststats(metadata_t * kvstats, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int partitionoffset, unsigned int currentLOP, vertex_t finalnumpartitions){
	unsigned int skipsize = kernelprocess::getdestskipsize(currentLOP, finalnumpartitions);
	unsigned int nxtpos = partitionoffset;
	#ifdef _DEBUGMODE_PRINTS
	cout<<"kernelprocess::storedeststats:: currentLOP: "<<currentLOP<<", skipsize: "<<skipsize<<", partitionoffset: "<<partitionoffset<<", finalnumpartitions: "<<finalnumpartitions<<endl;
	#endif
	STOREDESTSTATS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel(p, NUM_PARTITIONS, "kernelprocess::storedeststats:: out-of-bounds seen at kernelprocess::storedeststats 5", 0, 0, 0);
		checkforoutofbounds_kernel((skipsize * p), KVSTATS_SIZE, "kernelprocess::storedeststats:: out-of-bounds seen at kernelprocess::storedeststats 6", 0, 0, 0);
		#endif
		
		/* kvstats[((partitionoffset + (p * skipsize)) % FORCDFINISH_KVSTATS_SIZE)].size = kvdeststats_tmp[p].size; */
		/* kvstats[nxtpos].size = kvdeststats_tmp[p].size; 
		nxtpos += skipsize; */
		kvstats[(nxtpos % FORCDFINISH_KVSTATS_SIZE)].size = kvdeststats_tmp[p].size; 
		nxtpos += skipsize;
		
		#ifdef _DEBUGMODE_PRINTS
		cout<<"kernelprocess::storedeststats:: kvdeststats_tmp["<<p<<"].offset: "<<kvdeststats_tmp[p].offset<<", kvdeststats_tmp["<<p<<"].size: "<<kvdeststats_tmp[p].size<<", ";
		cout<<"kernelprocess::storedeststats:: kvstats["<<(partitionoffset + (p * skipsize))<<"].offset: "<<kvstats[(partitionoffset + (p * skipsize))].offset<<", kvstats["<<(partitionoffset + (p * skipsize))<<"].size: "<<kvstats[(partitionoffset + (p * skipsize))].size;
		cout<<endl;
		#endif
	}
	STOREDESTSTATS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		kvdeststats_tmp[p].size = 0;
	}
	return;
}

unsigned int kernelprocess::get_num_source_partitions(unsigned int currentLOP){
	unsigned int treedepth = TREEDEPTH_FORSSDPARTITION0;
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}

unsigned int kernelprocess::reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount){
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
void kernelprocess::read_ddr0_w0_sw0(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVBUFFERSZ_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker.weof = KVBUFFERSZ; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read00:: should NEVER get here: kvbuffer1_tracker.weof: "<<kvbuffer1_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = (kvbuffer1_tracker.weof + (VECTOR_SIZE - 1)) / VECTOR_SIZE; /// FIXME. why?? <round up>
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize_kvs = KVBUFFERSZ_KVS; } 
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS00_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read00:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read00:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::read_ddr0_w0_sw1(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVBUFFERSZ_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker.weof = KVBUFFERSZ; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read01:: should NEVER get here: kvbuffer1_tracker.weof: "<<kvbuffer1_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = (kvbuffer1_tracker.weof + (VECTOR_SIZE - 1)) / VECTOR_SIZE; /// FIXME. why?? <round up>
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize_kvs = KVBUFFERSZ_KVS; } 
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS01_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read01:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read01:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::read_ddr0_w0_sw2(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVBUFFERSZ_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker.weof = KVBUFFERSZ; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read02:: should NEVER get here: kvbuffer1_tracker.weof: "<<kvbuffer1_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = (kvbuffer1_tracker.weof + (VECTOR_SIZE - 1)) / VECTOR_SIZE; /// FIXME. why?? <round up>
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize_kvs = KVBUFFERSZ_KVS; } 
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS02_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read02:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read02:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::read_ddr0_w0_sw3(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVBUFFERSZ_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker.weof = KVBUFFERSZ; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read03:: should NEVER get here: kvbuffer1_tracker.weof: "<<kvbuffer1_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = (kvbuffer1_tracker.weof + (VECTOR_SIZE - 1)) / VECTOR_SIZE; /// FIXME. why?? <round up>
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize_kvs = KVBUFFERSZ_KVS; } 
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS03_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read03:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read03:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::read_ddr0_w0_sw4(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVBUFFERSZ_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker.weof = KVBUFFERSZ; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read04:: should NEVER get here: kvbuffer1_tracker.weof: "<<kvbuffer1_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = (kvbuffer1_tracker.weof + (VECTOR_SIZE - 1)) / VECTOR_SIZE; /// FIXME. why?? <round up>
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize_kvs = KVBUFFERSZ_KVS; } 
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS04_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read04:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read04:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::read_ddr0_w0_sw5(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVBUFFERSZ_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker.weof = KVBUFFERSZ; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read05:: should NEVER get here: kvbuffer1_tracker.weof: "<<kvbuffer1_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = (kvbuffer1_tracker.weof + (VECTOR_SIZE - 1)) / VECTOR_SIZE; /// FIXME. why?? <round up>
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize_kvs = KVBUFFERSZ_KVS; } 
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS05_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read05:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read05:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::read_ddr0_w0_sw6(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVBUFFERSZ_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker.weof = KVBUFFERSZ; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read06:: should NEVER get here: kvbuffer1_tracker.weof: "<<kvbuffer1_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = (kvbuffer1_tracker.weof + (VECTOR_SIZE - 1)) / VECTOR_SIZE; /// FIXME. why?? <round up>
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize_kvs = KVBUFFERSZ_KVS; } 
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS06_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read06:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read06:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::read_ddr0_w0_sw7(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_tracker;
	unsigned int curroffset_kvs = offset_kvs;
	unsigned int nextoffset_kvs = offset_kvs + KVBUFFERSZ_KVS; 
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_tracker.weof = KVBUFFERSZ; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_tracker.weof = (kvsize_kvs - curroffset_kvs) * VECTOR_SIZE; }
	else {
		kvbuffer1_tracker.weof = 0;
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"read07:: should NEVER get here: kvbuffer1_tracker.weof: "<<kvbuffer1_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	
	kvbuffersz_dtype rwsize_kvs = (kvbuffer1_tracker.weof + (VECTOR_SIZE - 1)) / VECTOR_SIZE; /// FIXME. why?? <round up>
	dramsz_dtype dramoffset = baseaddr_kvs + offset_kvs;
	if(rwsize_kvs > FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize_kvs = KVBUFFERSZ_KVS; } 
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	READBURSTKVS07_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel((offset_kvs + i), PADDEDKVDATA_BATCHSIZE_KVS, "read07:: out-of-bounds seen at read 5", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel((dramoffset + i), _x3PADDEDKVDATA_SIZE_KVS, "read07:: out-of-bounds seen at read 6", baseaddr_kvs, offset_kvs, dramoffset);		
		checkforoutofbounds_kernel(i, KVSRCBUFFERSZ_KVS, "out-of-bounds seen at read 7", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		kvbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		kvbuffer[i] = kvdram[(dramoffset + i)];
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}

void kernelprocess::readAU_VU_ddr0_w0_sw0(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
		kvpairsread0 += VECTOR_SIZE;
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
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_VU_ddr0_w0_sw1(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
		kvpairsread0 += VECTOR_SIZE;
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
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_VU_ddr0_w0_sw2(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
		kvpairsread0 += VECTOR_SIZE;
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
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_VU_ddr0_w0_sw3(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
		kvpairsread0 += VECTOR_SIZE;
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
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_VU_ddr0_w0_sw4(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
		kvpairsread0 += VECTOR_SIZE;
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
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_VU_ddr0_w0_sw5(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
		kvpairsread0 += VECTOR_SIZE;
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
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_VU_ddr0_w0_sw6(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
		kvpairsread0 += VECTOR_SIZE;
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
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_VU_ddr0_w0_sw7(unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
		kvpairsread0 += VECTOR_SIZE;
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
	kvpairsread0_transfers += 1;
	#endif
	return;
}

void kernelprocess::readAU_V_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V00 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
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
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V00:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_V_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V01 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
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
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V01:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_V_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V02 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
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
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V02:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_V_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V03 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
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
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V03:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_V_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V04 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
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
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V04:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_V_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V05 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
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
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V05:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_V_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V06 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
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
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V06:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}
void kernelprocess::readAU_V_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int offset_kvs, unsigned int kvsize_kvs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::readAU_V07 read. workerid: "<<workerid<<std::endl;
	#endif
	int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;
	
	tracker_t kvbuffer1_tracker_kvs;
	unsigned int vertexreadsz_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
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
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "readAU_V07:: out-of-bounds seen at read 7A", kvbuffer1_tracker_kvs.weof, i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid));
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairsread0 += VECTOR_SIZE;
		#endif
		
		#ifdef FORCDFINISH
		verticesbuffer[i] = kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))];
		#else 
		verticesbuffer[i] = kvdram[(dramoffset + i)]; 
		#endif 
	}
	#ifdef _DEBUGMODE_STATS
	kvpairsread0_transfers += 1;
	#endif
	return;
}

// PARTITION
void kernelprocess::partition_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"kernelprocess:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
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
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_00:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS00_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}	
	
	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 8;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif
	keyvalue_t kventry1;
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = KVBUFFERSZ_KVS; }
	EXTRACTSTATS00_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
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
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
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
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
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
	
	EXTRACTSTATS00_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION00_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION00_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
			
		kvbufferstats_tmp_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION00_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION00_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}

	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 9;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif 
	keyvalue_t kventry2;
	unsigned int rwsize2_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize2_kvs >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2_kvs = KVBUFFERSZ_KVS; }
	PARTITION00_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2_kvs; j++){
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
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
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
	/* #ifdef _DEBUGMODE_PRINTS
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ cout<<"partition_ddr0_w0_sw0: kvbufferstats_tmp_size["<<p<<"]: "<<kvbufferstats_tmp_size[p]<<endl; }
	exit(EXIT_SUCCESS);
	#endif */
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION00_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		ov[p].size = kvbufferstats_tmp_size[p] - kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION00_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP00_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void kernelprocess::partition_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"kernelprocess:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
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
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_01:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS01_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}	
	
	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 8;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif
	keyvalue_t kventry1;
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = KVBUFFERSZ_KVS; }
	EXTRACTSTATS01_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
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
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
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
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
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
	
	EXTRACTSTATS01_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION01_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION01_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
			
		kvbufferstats_tmp_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION01_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION01_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}

	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 9;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif 
	keyvalue_t kventry2;
	unsigned int rwsize2_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize2_kvs >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2_kvs = KVBUFFERSZ_KVS; }
	PARTITION01_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2_kvs; j++){
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
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
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
	/* #ifdef _DEBUGMODE_PRINTS
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ cout<<"partition_ddr0_w0_sw1: kvbufferstats_tmp_size["<<p<<"]: "<<kvbufferstats_tmp_size[p]<<endl; }
	exit(EXIT_SUCCESS);
	#endif */
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION01_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		ov[p].size = kvbufferstats_tmp_size[p] - kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION01_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP01_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void kernelprocess::partition_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"kernelprocess:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
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
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_02:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS02_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}	
	
	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 8;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif
	keyvalue_t kventry1;
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = KVBUFFERSZ_KVS; }
	EXTRACTSTATS02_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
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
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
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
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
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
	
	EXTRACTSTATS02_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION02_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION02_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
			
		kvbufferstats_tmp_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION02_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION02_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}

	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 9;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif 
	keyvalue_t kventry2;
	unsigned int rwsize2_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize2_kvs >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2_kvs = KVBUFFERSZ_KVS; }
	PARTITION02_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2_kvs; j++){
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
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
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
	/* #ifdef _DEBUGMODE_PRINTS
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ cout<<"partition_ddr0_w0_sw2: kvbufferstats_tmp_size["<<p<<"]: "<<kvbufferstats_tmp_size[p]<<endl; }
	exit(EXIT_SUCCESS);
	#endif */
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION02_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		ov[p].size = kvbufferstats_tmp_size[p] - kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION02_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP02_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void kernelprocess::partition_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"kernelprocess:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
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
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_03:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS03_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}	
	
	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 8;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif
	keyvalue_t kventry1;
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = KVBUFFERSZ_KVS; }
	EXTRACTSTATS03_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
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
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
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
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
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
	
	EXTRACTSTATS03_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION03_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION03_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
			
		kvbufferstats_tmp_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION03_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION03_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}

	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 9;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif 
	keyvalue_t kventry2;
	unsigned int rwsize2_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize2_kvs >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2_kvs = KVBUFFERSZ_KVS; }
	PARTITION03_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2_kvs; j++){
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
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
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
	/* #ifdef _DEBUGMODE_PRINTS
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ cout<<"partition_ddr0_w0_sw3: kvbufferstats_tmp_size["<<p<<"]: "<<kvbufferstats_tmp_size[p]<<endl; }
	exit(EXIT_SUCCESS);
	#endif */
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION03_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		ov[p].size = kvbufferstats_tmp_size[p] - kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION03_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP03_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void kernelprocess::partition_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"kernelprocess:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
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
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_04:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS04_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}	
	
	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 8;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif
	keyvalue_t kventry1;
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = KVBUFFERSZ_KVS; }
	EXTRACTSTATS04_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
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
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
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
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
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
	
	EXTRACTSTATS04_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION04_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION04_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
			
		kvbufferstats_tmp_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION04_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION04_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}

	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 9;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif 
	keyvalue_t kventry2;
	unsigned int rwsize2_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize2_kvs >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2_kvs = KVBUFFERSZ_KVS; }
	PARTITION04_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2_kvs; j++){
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
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
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
	/* #ifdef _DEBUGMODE_PRINTS
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ cout<<"partition_ddr0_w0_sw4: kvbufferstats_tmp_size["<<p<<"]: "<<kvbufferstats_tmp_size[p]<<endl; }
	exit(EXIT_SUCCESS);
	#endif */
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION04_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		ov[p].size = kvbufferstats_tmp_size[p] - kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION04_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP04_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void kernelprocess::partition_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"kernelprocess:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
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
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_05:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS05_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}	
	
	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 8;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif
	keyvalue_t kventry1;
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = KVBUFFERSZ_KVS; }
	EXTRACTSTATS05_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
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
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
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
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
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
	
	EXTRACTSTATS05_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION05_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION05_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
			
		kvbufferstats_tmp_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION05_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION05_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}

	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 9;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif 
	keyvalue_t kventry2;
	unsigned int rwsize2_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize2_kvs >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2_kvs = KVBUFFERSZ_KVS; }
	PARTITION05_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2_kvs; j++){
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
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
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
	/* #ifdef _DEBUGMODE_PRINTS
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ cout<<"partition_ddr0_w0_sw5: kvbufferstats_tmp_size["<<p<<"]: "<<kvbufferstats_tmp_size[p]<<endl; }
	exit(EXIT_SUCCESS);
	#endif */
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION05_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		ov[p].size = kvbufferstats_tmp_size[p] - kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION05_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP05_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void kernelprocess::partition_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"kernelprocess:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
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
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_06:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS06_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}	
	
	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 8;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif
	keyvalue_t kventry1;
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = KVBUFFERSZ_KVS; }
	EXTRACTSTATS06_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
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
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
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
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
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
	
	EXTRACTSTATS06_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION06_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION06_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
			
		kvbufferstats_tmp_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION06_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION06_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}

	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 9;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif 
	keyvalue_t kventry2;
	unsigned int rwsize2_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize2_kvs >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2_kvs = KVBUFFERSZ_KVS; }
	PARTITION06_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2_kvs; j++){
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
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
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
	/* #ifdef _DEBUGMODE_PRINTS
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ cout<<"partition_ddr0_w0_sw6: kvbufferstats_tmp_size["<<p<<"]: "<<kvbufferstats_tmp_size[p]<<endl; }
	exit(EXIT_SUCCESS);
	#endif */
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION06_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		ov[p].size = kvbufferstats_tmp_size[p] - kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION06_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP06_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
void kernelprocess::partition_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS]
		#ifdef _PARTITIONED_KVBUFFERSTATS
		,smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		#else 
		,smalluint_t kvbufferstats_tmp_size[NUM_PARTITIONS]
		#endif 
		,smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"kernelprocess:: partition. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", kvsize_kvs: "<<kvsize_kvs<<std::endl;
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
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_07:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS07_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_tmp_size[p] = 0;
	}	
	
	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 8;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif
	keyvalue_t kventry1;
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = KVBUFFERSZ_KVS; }
	EXTRACTSTATS07_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
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
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif			
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
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
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_tmp_size[(p-1)] + ov[(p-1)].size));
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
	
	EXTRACTSTATS07_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION07_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION07_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
			
		kvbufferstats_tmp_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION07_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION07_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}

	#ifdef _DEBUGMODE_CHECKS2
	state_moduleid = 9;
	state_kvbuffer1_kvs_tracker = kvbuffer1_kvs_tracker.weof;
	#endif 
	keyvalue_t kventry2;
	unsigned int rwsize2_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize2_kvs >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2_kvs = KVBUFFERSZ_KVS; }
	PARTITION07_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2_kvs; j++){
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
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#endif
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_tmp_size[p]), kventry2); 
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
	/* #ifdef _DEBUGMODE_PRINTS
	for(unsigned int p=0; p<NUM_PARTITIONS; p++){ cout<<"partition_ddr0_w0_sw7: kvbufferstats_tmp_size["<<p<<"]: "<<kvbufferstats_tmp_size[p]<<endl; }
	exit(EXIT_SUCCESS);
	#endif */
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION07_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		ov[p].size = kvbufferstats_tmp_size[p] - kernelprocess::alligndown_KV(kvbufferstats_tmp_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION07_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}
	
	#ifdef _PARTITIONED_KVBUFFERSTATS // >>> sync
	WRITEBACK_KVSTATSTMP07_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ kvbufferstats_size[p] = kvbufferstats_tmp_size[p]; }
	#endif	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}

#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::extractstats2_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_00:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS00_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_size[p] = 0;
	}	
	
	keyvalue_t kventry1;
	// #pragma HLS DATA_PACK variable=kventry1
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = 0; }	
	EXTRACTSTATS00_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		EXTRACTSTATS00_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_size[p] += 1; 
		}
	}
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS00_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_size[(p-1)] + ov[(p-1)].size));
	}

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS00_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_size[p] = 0;
	}
	
	EXTRACTSTATS00_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION00_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION00_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
				
		kvbufferstats_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION00_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION00_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}
	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::extractstats2_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_01:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS01_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_size[p] = 0;
	}	
	
	keyvalue_t kventry1;
	// #pragma HLS DATA_PACK variable=kventry1
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = 0; }	
	EXTRACTSTATS01_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		EXTRACTSTATS01_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_size[p] += 1; 
		}
	}
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS01_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_size[(p-1)] + ov[(p-1)].size));
	}

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS01_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_size[p] = 0;
	}
	
	EXTRACTSTATS01_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION01_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION01_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
				
		kvbufferstats_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION01_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION01_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}
	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::extractstats2_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_02:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS02_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_size[p] = 0;
	}	
	
	keyvalue_t kventry1;
	// #pragma HLS DATA_PACK variable=kventry1
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = 0; }	
	EXTRACTSTATS02_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		EXTRACTSTATS02_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_size[p] += 1; 
		}
	}
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS02_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_size[(p-1)] + ov[(p-1)].size));
	}

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS02_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_size[p] = 0;
	}
	
	EXTRACTSTATS02_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION02_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION02_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
				
		kvbufferstats_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION02_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION02_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}
	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::extractstats2_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_03:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS03_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_size[p] = 0;
	}	
	
	keyvalue_t kventry1;
	// #pragma HLS DATA_PACK variable=kventry1
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = 0; }	
	EXTRACTSTATS03_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		EXTRACTSTATS03_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_size[p] += 1; 
		}
	}
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS03_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_size[(p-1)] + ov[(p-1)].size));
	}

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS03_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_size[p] = 0;
	}
	
	EXTRACTSTATS03_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION03_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION03_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
				
		kvbufferstats_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION03_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION03_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}
	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::extractstats2_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_04:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS04_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_size[p] = 0;
	}	
	
	keyvalue_t kventry1;
	// #pragma HLS DATA_PACK variable=kventry1
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = 0; }	
	EXTRACTSTATS04_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		EXTRACTSTATS04_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_size[p] += 1; 
		}
	}
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS04_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_size[(p-1)] + ov[(p-1)].size));
	}

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS04_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_size[p] = 0;
	}
	
	EXTRACTSTATS04_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION04_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION04_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
				
		kvbufferstats_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION04_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION04_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}
	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::extractstats2_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_05:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS05_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_size[p] = 0;
	}	
	
	keyvalue_t kventry1;
	// #pragma HLS DATA_PACK variable=kventry1
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = 0; }	
	EXTRACTSTATS05_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		EXTRACTSTATS05_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_size[p] += 1; 
		}
	}
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS05_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_size[(p-1)] + ov[(p-1)].size));
	}

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS05_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_size[p] = 0;
	}
	
	EXTRACTSTATS05_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION05_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION05_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
				
		kvbufferstats_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION05_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION05_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}
	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::extractstats2_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_06:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS06_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_size[p] = 0;
	}	
	
	keyvalue_t kventry1;
	// #pragma HLS DATA_PACK variable=kventry1
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = 0; }	
	EXTRACTSTATS06_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		EXTRACTSTATS06_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_size[p] += 1; 
		}
	}
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS06_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_size[(p-1)] + ov[(p-1)].size));
	}

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS06_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_size[p] = 0;
	}
	
	EXTRACTSTATS06_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION06_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION06_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
				
		kvbufferstats_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION06_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION06_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}
	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::extractstats2_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only
	
	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"extractstats2_07:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_src, KVBUFFERSZ, "extractstats2_.extractstats.kvbuffer_src");
	#endif
	EXTRACTSTATS07_INIT_LOOP0: for (int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = 0;
		kvbufferstats_size[p] = 0;
	}	
	
	keyvalue_t kventry1;
	// #pragma HLS DATA_PACK variable=kventry1
	unsigned int rwsize_kvs = kvbuffer1_kvs_tracker.weof;
	if(rwsize_kvs >= FORCDFINISH_KVSRCBUFFERSZ){ rwsize_kvs = 0; }	
	EXTRACTSTATS07_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize_kvs; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		EXTRACTSTATS07_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
			// #pragma HLS UNROLL
			#ifdef _WIDEWORD
			// kventry1 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry1.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry1.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else 
			kventry1.key = tmpV1.data[k].key;
			kventry1.value = tmpV1.data[k].value;
			#endif 
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry1, upperlimit, ssdpartitonid);
			#ifdef _DEBUGMODE_
			checkforoutofbounds_kernel(p, NUM_PARTITIONS, "extractstats2_.extractstats:: out-of-bounds seen at write 5");
			#endif
			kvbufferstats_size[p] += 1; 
		}
	}
	
	kvbufferstats_offset[0] = 0;
	EXTRACTSTATS07_ALLIGNOFFSETS_LOOP1: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_offset[p] = kernelprocess::allignup_KV((kvbufferstats_offset[(p-1)] + kvbufferstats_size[(p-1)] + ov[(p-1)].size));
	}

	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: extractstats2_.extractstats2_. workerid: "<<workerid<<std::endl;
	#endif
	EXTRACTSTATS07_INIT2_LOOP1: for (unsigned int p = 0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		kvbufferstats_size[p] = 0;
	}
	
	EXTRACTSTATS07_RESOLVEEDGECONDITIONS_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){ // append ovs back to kvbuffer
	#pragma HLS PIPELINE
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("extractstats2_::PARTITION07_LOOP1A", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printovs2_kernel(ov, p, "[kernel::extractstats2_]", kernelprocess::getkventryII(ov[p].begin, kvbuffer_dest).key, 0, 0);
		#endif

		keyvalue_t kventry;
		#pragma HLS DATA_PACK variable=kventry
		unsigned int rwsize = ov[p].size;
		if(rwsize >= FORCDFINISH_VECTOR_SIZE){ rwsize = 0; }
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ov[p].size, VECTOR_SIZE, "kernel::PARTITION07_LOOP2:: out-of-bounds seen 58", p, 0, 0); 
		#endif
		
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "out-of-bounds seen at extractstats2_ 6", p, 0, 0);
		#endif
		kvbuffer_dest[((kvbufferstats_offset[p] + kvbufferstats_size[p]) / VECTOR_SIZE)] = ovkvs_tmp[p];
		#ifdef _DEBUGMODE_CHECKS
		checkentryforoutofbounds_kernel(destpartitionoffset, p, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid), (kernelprocess::getkventryII((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbuffer_dest).key - upperlimit), "", p, i, (ov[p].begin+0));
		#endif
				
		kvbufferstats_size[p] += rwsize;
		#ifdef _DEBUGMODE_CHECKS
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), kvbufferstats_offset[p+1], "kernel::PARTITION07_LOOP1A:: out-of-bounds seen 35", 0, 0, 0); }
		else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION07_LOOP1A:: out-of-bounds seen 36", 0, 0, 0); }
		#endif
		
		ov[p].begin = 0;
		ov[p].size = 0;
	}
	
	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_tracker.weof, "extractstats2_.extractstats2_.kvbuffer_dest");
	#endif
	return;
}
#endif 

#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::partition2_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: partition2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only

	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"partition2_00:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	keyvalue_t kventry2;
	// #pragma HLS DATA_PACK variable=kventry2
	unsigned int rwsize2 = kvbuffer1_kvs_tracker.weof;
	if(rwsize2 >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2 = 0; }
	PARTITION00_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		PARTITION00_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_size[p]), kventry2); 
			kvbufferstats_size[p] += 1; 
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION00_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION00_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
		}
	}
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION00_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		ov[p].size = kvbufferstats_size[p] - kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION00_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}	

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "partition2_.partition2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::partition2_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: partition2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only

	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"partition2_01:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	keyvalue_t kventry2;
	// #pragma HLS DATA_PACK variable=kventry2
	unsigned int rwsize2 = kvbuffer1_kvs_tracker.weof;
	if(rwsize2 >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2 = 0; }
	PARTITION01_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		PARTITION01_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_size[p]), kventry2); 
			kvbufferstats_size[p] += 1; 
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION01_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION01_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
		}
	}
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION01_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		ov[p].size = kvbufferstats_size[p] - kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION01_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}	

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "partition2_.partition2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::partition2_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: partition2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only

	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"partition2_02:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	keyvalue_t kventry2;
	// #pragma HLS DATA_PACK variable=kventry2
	unsigned int rwsize2 = kvbuffer1_kvs_tracker.weof;
	if(rwsize2 >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2 = 0; }
	PARTITION02_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		PARTITION02_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_size[p]), kventry2); 
			kvbufferstats_size[p] += 1; 
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION02_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION02_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
		}
	}
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION02_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		ov[p].size = kvbufferstats_size[p] - kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION02_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}	

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "partition2_.partition2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::partition2_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: partition2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only

	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"partition2_03:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	keyvalue_t kventry2;
	// #pragma HLS DATA_PACK variable=kventry2
	unsigned int rwsize2 = kvbuffer1_kvs_tracker.weof;
	if(rwsize2 >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2 = 0; }
	PARTITION03_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		PARTITION03_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_size[p]), kventry2); 
			kvbufferstats_size[p] += 1; 
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION03_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION03_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
		}
	}
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION03_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		ov[p].size = kvbufferstats_size[p] - kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION03_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}	

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "partition2_.partition2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::partition2_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: partition2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only

	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"partition2_04:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	keyvalue_t kventry2;
	// #pragma HLS DATA_PACK variable=kventry2
	unsigned int rwsize2 = kvbuffer1_kvs_tracker.weof;
	if(rwsize2 >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2 = 0; }
	PARTITION04_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		PARTITION04_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_size[p]), kventry2); 
			kvbufferstats_size[p] += 1; 
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION04_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION04_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
		}
	}
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION04_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		ov[p].size = kvbufferstats_size[p] - kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION04_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}	

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "partition2_.partition2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::partition2_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: partition2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only

	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"partition2_05:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	keyvalue_t kventry2;
	// #pragma HLS DATA_PACK variable=kventry2
	unsigned int rwsize2 = kvbuffer1_kvs_tracker.weof;
	if(rwsize2 >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2 = 0; }
	PARTITION05_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		PARTITION05_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_size[p]), kventry2); 
			kvbufferstats_size[p] += 1; 
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION05_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION05_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
		}
	}
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION05_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		ov[p].size = kvbufferstats_size[p] - kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION05_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}	

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "partition2_.partition2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::partition2_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: partition2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only

	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"partition2_06:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	keyvalue_t kventry2;
	// #pragma HLS DATA_PACK variable=kventry2
	unsigned int rwsize2 = kvbuffer1_kvs_tracker.weof;
	if(rwsize2 >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2 = 0; }
	PARTITION06_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		PARTITION06_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_size[p]), kventry2); 
			kvbufferstats_size[p] += 1; 
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION06_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION06_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
		}
	}
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION06_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		ov[p].size = kvbufferstats_size[p] - kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION06_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}	

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "partition2_.partition2_.kvbuffer_dest");
	#endif
	return;
}
#endif 
#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::partition2_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int subworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange,
		uint512_dt kvbuffer_src[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer_dest[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], uint512_dt ovkvs_tmp[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel:: partition2_. workerid: "<<workerid<<", KVBUFFERSZ: "<<KVBUFFERSZ<<std::endl;
	#endif
	int kvbuffersz = KVBUFFERSZ;  // NB: debugging purposes only
	int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	int vector_size = VECTOR_SIZE;  // NB: debugging purposes only

	tracker_t kvbuffer1_kvs_tracker;
	unsigned int curroffset_kvs = kvreadoffset_kvs + (subworkerid * KVBUFFERSZ_KVS);
	unsigned int nextoffset_kvs = kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS);
	if(curroffset_kvs >= kvsize_kvs){ kvbuffer1_kvs_tracker.weof = 0; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs < kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = KVBUFFERSZ_KVS; }
	else if ((curroffset_kvs < kvsize_kvs) && (nextoffset_kvs >= kvsize_kvs)) { kvbuffer1_kvs_tracker.weof = kvsize_kvs - curroffset_kvs; }
	else {
		kvbuffer1_kvs_tracker.weof = 0; 
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"partition2_07:: should NEVER get here: kvbuffer1_kvs_tracker.weof: "<<kvbuffer1_kvs_tracker.weof<<", kvsize_kvs: "<<kvsize_kvs<<", kvreadoffset_kvs: "<<kvreadoffset_kvs<<", next kvreadoffset_kvs: "<<(kvreadoffset_kvs + ((subworkerid + 1) * KVBUFFERSZ_KVS))<<", KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
		exit(EXIT_FAILURE);
		#endif
	}

	keyvalue_t kventry2;
	// #pragma HLS DATA_PACK variable=kventry2
	unsigned int rwsize2 = kvbuffer1_kvs_tracker.weof;
	if(rwsize2 >= FORCDFINISH_KVSRCBUFFERSZ_KVS){ rwsize2 = 0; }
	PARTITION07_MAIN_LOOP1: for (unsigned int j = 0; j<rwsize2; j++){
	#pragma HLS PIPELINE
	#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_kvs avg=kvbuffersz_kvs
	uint512_dt tmpV1 = kvbuffer_src[j];
		PARTITION07_MAIN_LOOP1B: for(unsigned int k=0; k<VECTOR_SIZE; k++){
		// #pragma HLS PIPELINE
		// #pragma HLS UNROLL
		#pragma HLS LOOP_TRIPCOUNT min=0 max=vector_size avg=vector_size
			#ifdef _WIDEWORD
			// kventry2 = kernelprocess::getkventry3(tmpV1, k);
			unsigned int _k2 = k * 2;
			unsigned int _k2plus1 = _k2 + 1;
			kventry2.key = tmpV1.range(DATATYPE_SIZE * (_k2 + 1) - 1, _k2 * DATATYPE_SIZE);
			kventry2.value = tmpV1.range(DATATYPE_SIZE * (_k2plus1 + 1) - 1, _k2plus1 * DATATYPE_SIZE);
			#else
			kventry2.key = tmpV1.data[k].key;
			kventry2.value = tmpV1.data[k].value;
			#endif			
			
			unsigned int p = kernelprocess::hashfunc4(currentLOP, kventry2, upperlimit, ssdpartitonid);
kernelprocess::setkventryII(kvbuffer_dest, (kvbufferstats_offset[p] + kvbufferstats_size[p]), kventry2); 
			kvbufferstats_size[p] += 1; 
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), (kvbufferstats_offset[p+1] + 1), "kernel::PARTITION07_LOOP2:: out-of-bounds seen at partition2_ 37", p, 0, 0); }
			else { checkforoutofbounds_kernel((kvbufferstats_offset[p] + kvbufferstats_size[p]), KVSRCBUFFERSZ, "kernel::PARTITION07_LOOP2:: out-of-bounds seen at partition2_ 38", p, 0, 0); }
			#endif
		}
	}
	
	// Store overflows
	uint512_dt tmpV1;
	PARTITION07_RESOLVEEDGECONDITIONS_LOOP2: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		ov[p].begin = kvbufferstats_offset[p] + kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		ov[p].size = kvbufferstats_size[p] - kernelprocess::alligndown_KV(kvbufferstats_size[p]);
		unsigned int ovbegin_kvs = ov[p].begin / VECTOR_SIZE;
		unsigned int ovsize = ov[p].size;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(ovsize, VECTOR_SIZE, "kernel::PARTITION07_LOOP2:: out-of-bounds seen at partition2_ 58", p, 0, 0); 
		checkforoutofbounds_kernel(ovbegin_kvs, KVDESTBUFFERSZ_KVS, "out-of-bounds seen at partition2_ 235", p, ovbegin_kvs, KVDESTBUFFERSZ_KVS);
		#endif
		
		uint512_dt tmpV1 = kvbuffer_dest[ovbegin_kvs];
		ovkvs_tmp[p] = tmpV1;
		#ifdef _DEBUGMODE_PRINTS
		print4ints_kernel("partition2_", "p", p, "p", p, "ov[p].begin", ov[p].begin, "ov[p].size", ov[p].size);
		printkvs2_kernel(ovkvs_tmp[p], "printing ovkvs_tmp's");
		#endif
	}	

	#ifdef _DEBUGMODE_PRINTS
	printkvbuffer_kernel(kvbuffer_dest, kvbuffer1_kvs_tracker.weof, "partition2_.partition2_.kvbuffer_dest");
	#endif
	return;
}
#endif 

// APPLY
void kernelprocess::reduce_ddr0_w0_sw0(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
			kventry = kernelprocess::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw0 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
			unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw0_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = kernelprocess::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw0 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
		unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void kernelprocess::reduce_ddr0_w0_sw1(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
			kventry = kernelprocess::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw1 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
			unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw1_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = kernelprocess::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw1 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
		unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void kernelprocess::reduce_ddr0_w0_sw2(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
			kventry = kernelprocess::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw2 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
			unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw2_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = kernelprocess::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw2 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
		unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void kernelprocess::reduce_ddr0_w0_sw3(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
			kventry = kernelprocess::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw3 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
			unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw3_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = kernelprocess::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw3 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
		unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void kernelprocess::reduce_ddr0_w0_sw4(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
			kventry = kernelprocess::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw4 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
			unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw4_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = kernelprocess::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw4 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
		unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void kernelprocess::reduce_ddr0_w0_sw5(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
			kventry = kernelprocess::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw5 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
			unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw5_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = kernelprocess::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw5 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
		unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void kernelprocess::reduce_ddr0_w0_sw6(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
			kventry = kernelprocess::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw6 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
			unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw6_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = kernelprocess::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw6 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
		unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}
void kernelprocess::reduce_ddr0_w0_sw7(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int workerid, unsigned int verticesreadoffset_vs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvsTTT, metadata_t sourcestatbuffer, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS], uint512_dt kvbuffer[KVSRCBUFFERSZ_APPLY_KVS]){
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
			kventry = kernelprocess::getkventry3(tmpV1, k);
			#else 
			kventry.key = tmpV1.data[k].key;
			kventry.value = tmpV1.data[k].value;
			#endif 
			
			#ifdef _DEBUGMODE_CHECKS2
			if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw7 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<i<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
			#endif
			unsigned int loc = kventry.key - voffset; 
			if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
			
			keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
			unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
	
			#if defined(BC_ALGORITHM)
			// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
			// ret = min(value, IterCount);
			if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
			#endif
			vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);					
		}
	}
	
	uint512_dt tmpV1 = kvbuffer[(kvsize_kvs - 1)];
	unsigned int keyvalues_overflow = sourcestatbuffer.size - alligndown_KV(sourcestatbuffer.size);
	REDUCEOV_ddr0_w0_sw7_LOOP1B: for(unsigned int k=0; k<keyvalues_overflow; k++){
		#ifdef _WIDEWORD
		kventry = kernelprocess::getkventry3(tmpV1, k);
		#else 
		kventry.key = tmpV1.data[k].key;
		kventry.value = tmpV1.data[k].value;
		#endif 
	
		#ifdef _DEBUGMODE_CHECKS2
		if((kventry.key - voffset) >= kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)){ cout<<"out-of-bounds seen at reduce_ddr0_w0_sw7 5: kventry.key: "<<kventry.key<<", voffset: "<<voffset<<", APPLYVERTEXBUFFERSZ_PERSSDPARTITION("<<ssdpartitonid<<"): "<<GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid)<<", KVSRCBUFFERSZ_APPLY_KVS: "<<KVSRCBUFFERSZ_APPLY_KVS<<", kvbuffer1_tracker_kvs.weof: "<<kvbuffer1_tracker_kvs.weof<<", i: "<<(kvsize_kvs - 1)<<", k: "<<k<<endl; exit(EXIT_FAILURE); }
		#endif
		
		unsigned int loc = kventry.key - voffset; 
		if(loc >= FORCDFINISH_APPLYVERTEXBUFFERSZ){ loc = 0; }
		
		keyvalue_t vprop = kernelprocess::getkventryIV(loc, verticesbuffer);
		unsigned int temp = kernelprocess::reducefunc(vprop.key, vprop.value, kventry.value, IterCount);
		
		#if defined(BC_ALGORITHM)
		// reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int IterCount)
		// ret = min(value, IterCount);
		if(temp == vprop.value){ vprop.key = kventry.key; } // REMOVEME. DIRECTIVE
		#endif
		vprop.value = temp;
kernelprocess::setkventryII(verticesbuffer, loc, vprop);
	}
	return;
}

// WRITE
void kernelprocess::write_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
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
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write00 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write00 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write00 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write00 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::write_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
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
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write01 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write01 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write01 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write01 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::write_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
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
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write02 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write02 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write02 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write02 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::write_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
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
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write03 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write03 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write03 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write03 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::write_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
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
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write04 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write04 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write04 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write04 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::write_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
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
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write05 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write05 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write05 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write05 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::write_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
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
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write06 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write06 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write06 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write06 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::write_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset,
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
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write07 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write07 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write07 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write07 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += rwsize_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}

void kernelprocess::writeremainingovs_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
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
		kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::writeremainingovs_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
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
		kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::writeremainingovs_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
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
		kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::writeremainingovs_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
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
		kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::writeremainingovs_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
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
		kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::writeremainingovs_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
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
		kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::writeremainingovs_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
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
		kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
void kernelprocess::writeremainingovs_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt ovkvs_tmp_ddr[NUM_PARTITIONS], smallmargin_t ov[NUM_PARTITIONS], unsigned int baseaddr_kvs){
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
		kvpairswritten0 += ov[p].size; // VECTOR_SIZE;
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}

// BRAM 2 TMP TRANSFERS
#ifdef _BUFFERB4WRITE
void kernelprocess::mergeoffsetsandsizes_ddr0_w0(unsigned int ssdpartitonid, unsigned int workerid
		,smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]
		,smalluint_t kvbufferstats_sw0_size[NUM_PARTITIONS]
		,smalluint_t kvbufferstats_sw1_size[NUM_PARTITIONS]
		,smalluint_t kvbufferstats_sw2_size[NUM_PARTITIONS]
		,smalluint_t kvbufferstats_sw3_size[NUM_PARTITIONS]
		,smalluint_t kvbufferstats_sw4_size[NUM_PARTITIONS]
		,smalluint_t kvbufferstats_sw5_size[NUM_PARTITIONS]
		,smalluint_t kvbufferstats_sw6_size[NUM_PARTITIONS]
		,smalluint_t kvbufferstats_sw7_size[NUM_PARTITIONS]
		){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"mergeoffsetsandsizes_ddr0_w0:: workerid: "<<workerid<<std::endl;
	#endif
	
	MERGE_ddr0_w0_LOOP1: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		DESTtmpkvbufferstats_size[p] = 
kvbufferstats_sw0_size[p]
+kvbufferstats_sw1_size[p]
+kvbufferstats_sw2_size[p]
+kvbufferstats_sw3_size[p]
+kvbufferstats_sw4_size[p]
+kvbufferstats_sw5_size[p]
+kvbufferstats_sw6_size[p]
+kvbufferstats_sw7_size[p]
		;
	}
	
	DESTtmpkvbufferstats_offset[0] = 0;
	MERGE_ddr0_w0_LOOP2: for (int p = 1; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		DESTtmpkvbufferstats_offset[p] = kernelprocess::allignup_KV((DESTtmpkvbufferstats_offset[(p-1)] + DESTtmpkvbufferstats_size[(p-1)]));		
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(DESTtmpkvbufferstats_offset[p], TOTALDESTTMPKVBUFFERSZ, "mergeoffsetsandsizes_ddr0_w0:: out-of-bounds seen at mergeoffsetsandsizes_ddr0_w0 36", p, DESTtmpkvbufferstats_offset[p], 0); 
		#endif		
	}
	
	MERGE_ddr0_w0_LOOP3: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE
		DESTtmpkvbufferstats_size[p] = 0;
	}
	return;
}
#endif

#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2tmp_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp00:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2TMP00_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) >= DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpbaseoffset_kvs = (DESTtmpkvbuffer_weof[p].offset + DESTtmpkvbuffer_weof[p].size) / VECTOR_SIZE;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp00:: ERRRRRRRRRRRRRRRR size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		unsigned int desttmpoffset_kvs = (p * DESTTMPKVBUFFERSZ_KVS) + desttmpbaseoffset_kvs;
		WRITE_BRAM2TMP00_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpoffset_kvs + i)] = kvbuffer[(offset + i)]; 
		}
		DESTtmpkvbuffer_weof[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2tmp_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp01:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2TMP01_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) >= DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpbaseoffset_kvs = (DESTtmpkvbuffer_weof[p].offset + DESTtmpkvbuffer_weof[p].size) / VECTOR_SIZE;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp01:: ERRRRRRRRRRRRRRRR size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		unsigned int desttmpoffset_kvs = (p * DESTTMPKVBUFFERSZ_KVS) + desttmpbaseoffset_kvs;
		WRITE_BRAM2TMP01_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpoffset_kvs + i)] = kvbuffer[(offset + i)]; 
		}
		DESTtmpkvbuffer_weof[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2tmp_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp02:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2TMP02_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) >= DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpbaseoffset_kvs = (DESTtmpkvbuffer_weof[p].offset + DESTtmpkvbuffer_weof[p].size) / VECTOR_SIZE;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp02:: ERRRRRRRRRRRRRRRR size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		unsigned int desttmpoffset_kvs = (p * DESTTMPKVBUFFERSZ_KVS) + desttmpbaseoffset_kvs;
		WRITE_BRAM2TMP02_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpoffset_kvs + i)] = kvbuffer[(offset + i)]; 
		}
		DESTtmpkvbuffer_weof[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2tmp_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp03:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2TMP03_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) >= DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpbaseoffset_kvs = (DESTtmpkvbuffer_weof[p].offset + DESTtmpkvbuffer_weof[p].size) / VECTOR_SIZE;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp03:: ERRRRRRRRRRRRRRRR size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		unsigned int desttmpoffset_kvs = (p * DESTTMPKVBUFFERSZ_KVS) + desttmpbaseoffset_kvs;
		WRITE_BRAM2TMP03_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpoffset_kvs + i)] = kvbuffer[(offset + i)]; 
		}
		DESTtmpkvbuffer_weof[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2tmp_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp04:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2TMP04_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) >= DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpbaseoffset_kvs = (DESTtmpkvbuffer_weof[p].offset + DESTtmpkvbuffer_weof[p].size) / VECTOR_SIZE;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp04:: ERRRRRRRRRRRRRRRR size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		unsigned int desttmpoffset_kvs = (p * DESTTMPKVBUFFERSZ_KVS) + desttmpbaseoffset_kvs;
		WRITE_BRAM2TMP04_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpoffset_kvs + i)] = kvbuffer[(offset + i)]; 
		}
		DESTtmpkvbuffer_weof[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2tmp_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp05:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2TMP05_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) >= DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpbaseoffset_kvs = (DESTtmpkvbuffer_weof[p].offset + DESTtmpkvbuffer_weof[p].size) / VECTOR_SIZE;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp05:: ERRRRRRRRRRRRRRRR size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		unsigned int desttmpoffset_kvs = (p * DESTTMPKVBUFFERSZ_KVS) + desttmpbaseoffset_kvs;
		WRITE_BRAM2TMP05_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpoffset_kvs + i)] = kvbuffer[(offset + i)]; 
		}
		DESTtmpkvbuffer_weof[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2tmp_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp06:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2TMP06_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) >= DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpbaseoffset_kvs = (DESTtmpkvbuffer_weof[p].offset + DESTtmpkvbuffer_weof[p].size) / VECTOR_SIZE;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp06:: ERRRRRRRRRRRRRRRR size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		unsigned int desttmpoffset_kvs = (p * DESTTMPKVBUFFERSZ_KVS) + desttmpbaseoffset_kvs;
		WRITE_BRAM2TMP06_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpoffset_kvs + i)] = kvbuffer[(offset + i)]; 
		}
		DESTtmpkvbuffer_weof[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2tmp_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp07:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2TMP07_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) >= DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpbaseoffset_kvs = (DESTtmpkvbuffer_weof[p].offset + DESTtmpkvbuffer_weof[p].size) / VECTOR_SIZE;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp07:: ERRRRRRRRRRRRRRRR size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		unsigned int desttmpoffset_kvs = (p * DESTTMPKVBUFFERSZ_KVS) + desttmpbaseoffset_kvs;
		WRITE_BRAM2TMP07_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpoffset_kvs + i)] = kvbuffer[(offset + i)]; 
		}
		DESTtmpkvbuffer_weof[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif 

#ifdef _BUFFERB4WRITE
void kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid
		,uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		,uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw0:: workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;  // NB: debugging purposes only
	WRITE_BRAM2TMP00_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE		
		unsigned int offset_kvs = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_baseoffset_kvs = DESTtmpkvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_size_kvs = DESTtmpkvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_offset_kvs = desttmpkvbuffer_baseoffset_kvs + desttmpkvbuffer_size_kvs;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(desttmpkvbuffer_size_kvs > TOTALDESTTMPKVBUFFERSZ){ std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw0:: ERROR desttmpkvbuffer_size_kvs: "<<desttmpkvbuffer_size_kvs<<", TOTALDESTTMPKVBUFFERSZ: "<<TOTALDESTTMPKVBUFFERSZ<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		WRITE_BRAM2TMP00_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpkvbuffer_offset_kvs + i)] = kvbuffer[(offset_kvs + i)]; 
		}
		
		DESTtmpkvbufferstats_size[p] += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif
#ifdef _BUFFERB4WRITE
void kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid
		,uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		,uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw1:: workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;  // NB: debugging purposes only
	WRITE_BRAM2TMP01_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE		
		unsigned int offset_kvs = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_baseoffset_kvs = DESTtmpkvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_size_kvs = DESTtmpkvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_offset_kvs = desttmpkvbuffer_baseoffset_kvs + desttmpkvbuffer_size_kvs;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(desttmpkvbuffer_size_kvs > TOTALDESTTMPKVBUFFERSZ){ std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw1:: ERROR desttmpkvbuffer_size_kvs: "<<desttmpkvbuffer_size_kvs<<", TOTALDESTTMPKVBUFFERSZ: "<<TOTALDESTTMPKVBUFFERSZ<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		WRITE_BRAM2TMP01_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpkvbuffer_offset_kvs + i)] = kvbuffer[(offset_kvs + i)]; 
		}
		
		DESTtmpkvbufferstats_size[p] += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif
#ifdef _BUFFERB4WRITE
void kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid
		,uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		,uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw2:: workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;  // NB: debugging purposes only
	WRITE_BRAM2TMP02_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE		
		unsigned int offset_kvs = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_baseoffset_kvs = DESTtmpkvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_size_kvs = DESTtmpkvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_offset_kvs = desttmpkvbuffer_baseoffset_kvs + desttmpkvbuffer_size_kvs;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(desttmpkvbuffer_size_kvs > TOTALDESTTMPKVBUFFERSZ){ std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw2:: ERROR desttmpkvbuffer_size_kvs: "<<desttmpkvbuffer_size_kvs<<", TOTALDESTTMPKVBUFFERSZ: "<<TOTALDESTTMPKVBUFFERSZ<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		WRITE_BRAM2TMP02_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpkvbuffer_offset_kvs + i)] = kvbuffer[(offset_kvs + i)]; 
		}
		
		DESTtmpkvbufferstats_size[p] += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif
#ifdef _BUFFERB4WRITE
void kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid
		,uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		,uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw3:: workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;  // NB: debugging purposes only
	WRITE_BRAM2TMP03_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE		
		unsigned int offset_kvs = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_baseoffset_kvs = DESTtmpkvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_size_kvs = DESTtmpkvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_offset_kvs = desttmpkvbuffer_baseoffset_kvs + desttmpkvbuffer_size_kvs;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(desttmpkvbuffer_size_kvs > TOTALDESTTMPKVBUFFERSZ){ std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw3:: ERROR desttmpkvbuffer_size_kvs: "<<desttmpkvbuffer_size_kvs<<", TOTALDESTTMPKVBUFFERSZ: "<<TOTALDESTTMPKVBUFFERSZ<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		WRITE_BRAM2TMP03_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpkvbuffer_offset_kvs + i)] = kvbuffer[(offset_kvs + i)]; 
		}
		
		DESTtmpkvbufferstats_size[p] += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif
#ifdef _BUFFERB4WRITE
void kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid
		,uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		,uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw4:: workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;  // NB: debugging purposes only
	WRITE_BRAM2TMP04_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE		
		unsigned int offset_kvs = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_baseoffset_kvs = DESTtmpkvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_size_kvs = DESTtmpkvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_offset_kvs = desttmpkvbuffer_baseoffset_kvs + desttmpkvbuffer_size_kvs;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(desttmpkvbuffer_size_kvs > TOTALDESTTMPKVBUFFERSZ){ std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw4:: ERROR desttmpkvbuffer_size_kvs: "<<desttmpkvbuffer_size_kvs<<", TOTALDESTTMPKVBUFFERSZ: "<<TOTALDESTTMPKVBUFFERSZ<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		WRITE_BRAM2TMP04_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpkvbuffer_offset_kvs + i)] = kvbuffer[(offset_kvs + i)]; 
		}
		
		DESTtmpkvbufferstats_size[p] += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif
#ifdef _BUFFERB4WRITE
void kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid
		,uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		,uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw5:: workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;  // NB: debugging purposes only
	WRITE_BRAM2TMP05_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE		
		unsigned int offset_kvs = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_baseoffset_kvs = DESTtmpkvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_size_kvs = DESTtmpkvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_offset_kvs = desttmpkvbuffer_baseoffset_kvs + desttmpkvbuffer_size_kvs;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(desttmpkvbuffer_size_kvs > TOTALDESTTMPKVBUFFERSZ){ std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw5:: ERROR desttmpkvbuffer_size_kvs: "<<desttmpkvbuffer_size_kvs<<", TOTALDESTTMPKVBUFFERSZ: "<<TOTALDESTTMPKVBUFFERSZ<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		WRITE_BRAM2TMP05_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpkvbuffer_offset_kvs + i)] = kvbuffer[(offset_kvs + i)]; 
		}
		
		DESTtmpkvbufferstats_size[p] += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif
#ifdef _BUFFERB4WRITE
void kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid
		,uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		,uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw6:: workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;  // NB: debugging purposes only
	WRITE_BRAM2TMP06_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE		
		unsigned int offset_kvs = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_baseoffset_kvs = DESTtmpkvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_size_kvs = DESTtmpkvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_offset_kvs = desttmpkvbuffer_baseoffset_kvs + desttmpkvbuffer_size_kvs;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(desttmpkvbuffer_size_kvs > TOTALDESTTMPKVBUFFERSZ){ std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw6:: ERROR desttmpkvbuffer_size_kvs: "<<desttmpkvbuffer_size_kvs<<", TOTALDESTTMPKVBUFFERSZ: "<<TOTALDESTTMPKVBUFFERSZ<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		WRITE_BRAM2TMP06_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpkvbuffer_offset_kvs + i)] = kvbuffer[(offset_kvs + i)]; 
		}
		
		DESTtmpkvbufferstats_size[p] += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif
#ifdef _BUFFERB4WRITE
void kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid
		,uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS]
		,uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw7:: workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;  // NB: debugging purposes only
	WRITE_BRAM2TMP07_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE		
		unsigned int offset_kvs = kvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_baseoffset_kvs = DESTtmpkvbufferstats_offset[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_size_kvs = DESTtmpkvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int desttmpkvbuffer_offset_kvs = desttmpkvbuffer_baseoffset_kvs + desttmpkvbuffer_size_kvs;
		
		#ifdef _DEBUGMODE_CHECKS2
		if(desttmpkvbuffer_size_kvs > TOTALDESTTMPKVBUFFERSZ){ std::cout<<"accel3_write_bram2tmp_ddr0_w0_sw7:: ERROR desttmpkvbuffer_size_kvs: "<<desttmpkvbuffer_size_kvs<<", TOTALDESTTMPKVBUFFERSZ: "<<TOTALDESTTMPKVBUFFERSZ<<std::endl; exit(EXIT_FAILURE); }
		#endif 
		
		WRITE_BRAM2TMP07_LOOP1B: for(unsigned int i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_bram2tmp0 += VECTOR_SIZE;
			#endif
			DESTtmpkvbuffer[(desttmpkvbuffer_offset_kvs + i)] = kvbuffer[(offset_kvs + i)]; 
		}
		
		DESTtmpkvbufferstats_size[p] += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_bram2tmp0_transfers += 1;
		#endif
	}
	return;
}
#endif

// BRAM 2 DRAM TRANSFERS
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2dram_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp00:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2DRAM00_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) < DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		kvbuffersz_dtype size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp00:: ERROR 45 size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif
		
		WRITE_BRAM2DRAM00_LOOP1B: for(kvbuffersz_dtype i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=0 avg=0
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write_bram2tmp00 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write_bram2tmp00 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write_bram2tmp00 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write_bram2tmp00 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2dram_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp01:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2DRAM01_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) < DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		kvbuffersz_dtype size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp01:: ERROR 45 size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif
		
		WRITE_BRAM2DRAM01_LOOP1B: for(kvbuffersz_dtype i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=0 avg=0
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write_bram2tmp01 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write_bram2tmp01 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write_bram2tmp01 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write_bram2tmp01 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2dram_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp02:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2DRAM02_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) < DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		kvbuffersz_dtype size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp02:: ERROR 45 size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif
		
		WRITE_BRAM2DRAM02_LOOP1B: for(kvbuffersz_dtype i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=0 avg=0
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write_bram2tmp02 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write_bram2tmp02 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write_bram2tmp02 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write_bram2tmp02 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2dram_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp03:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2DRAM03_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) < DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		kvbuffersz_dtype size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp03:: ERROR 45 size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif
		
		WRITE_BRAM2DRAM03_LOOP1B: for(kvbuffersz_dtype i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=0 avg=0
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write_bram2tmp03 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write_bram2tmp03 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write_bram2tmp03 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write_bram2tmp03 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2dram_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp04:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2DRAM04_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) < DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		kvbuffersz_dtype size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp04:: ERROR 45 size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif
		
		WRITE_BRAM2DRAM04_LOOP1B: for(kvbuffersz_dtype i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=0 avg=0
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write_bram2tmp04 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write_bram2tmp04 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write_bram2tmp04 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write_bram2tmp04 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2dram_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp05:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2DRAM05_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) < DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		kvbuffersz_dtype size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp05:: ERROR 45 size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif
		
		WRITE_BRAM2DRAM05_LOOP1B: for(kvbuffersz_dtype i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=0 avg=0
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write_bram2tmp05 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write_bram2tmp05 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write_bram2tmp05 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write_bram2tmp05 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2dram_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp06:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2DRAM06_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) < DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		kvbuffersz_dtype size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp06:: ERROR 45 size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif
		
		WRITE_BRAM2DRAM06_LOOP1B: for(kvbuffersz_dtype i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=0 avg=0
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write_bram2tmp06 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write_bram2tmp06 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write_bram2tmp06 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write_bram2tmp06 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
#endif 
#ifdef _BUFFERB4WRITE
void kernelprocess::write_bram2dram_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, 
		uint512_dt kvbuffer[KVDESTBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], smalluint_t kvbufferstats_offset[NUM_PARTITIONS], smalluint_t kvbufferstats_size[NUM_PARTITIONS], uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"writetotmp07:: write_bram2tmp. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int kvbuffersz_kvs = KVBUFFERSZ_KVS;  // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = KVBUFFERSZ_KVS / NUM_PARTITIONS;  // NB: debugging purposes only
	unsigned int desttmpkvbuffersz_kvs = DESTTMPKVBUFFERSZ_KVS;
	WRITE_BRAM2DRAM07_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		if((DESTtmpkvbuffer_weof[p].size + kvbufferstats_size[p]) < DESTTMPKVBUFFERSZ){ continue; }
			
		unsigned int offset = kvbufferstats_offset[p] / VECTOR_SIZE;
		kvbuffersz_dtype size_kvs = kvbufferstats_size[p] / VECTOR_SIZE;
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		unsigned int size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		if(size_kvs > KVDESTBUFFERSZ_KVS){ std::cout<<"writetotmp07:: ERROR 45 size_kvs: "<<size_kvs<<", KVDESTBUFFERSZ_KVS: "<<KVDESTBUFFERSZ_KVS<<std::endl; exit(EXIT_FAILURE); }
		#endif
		
		WRITE_BRAM2DRAM07_LOOP1B: for(kvbuffersz_dtype i=0; i<size_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=0 avg=0
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVDESTBUFFERSZ_KVS, "out-of-bounds seen at write_bram2tmp07 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write_bram2tmp07 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write_bram2tmp07 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write_bram2tmp07 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = kvbuffer[(offset + i)];
			#else 
			kvdram[(dramoffset + i)] = kvbuffer[(offset + i)];
			#endif 
		}
		kvdeststats_tmp[p].size += size_kvs * VECTOR_SIZE;
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0_transfers += 1;
		#endif
	}
	return;
}
#endif 

// WRITE VERTICES TO DRAM (APPLY PHASE)
void kernelprocess::writeAU_Vs_ddr0_w0_sw0(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs00 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS00_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs00:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs00:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0 += VECTOR_SIZE;
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
	kvpairswritten0_transfers += 1;
	#endif
	return;
}
void kernelprocess::writeAU_Vs_ddr0_w0_sw1(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs01 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS01_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs01:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs01:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0 += VECTOR_SIZE;
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
	kvpairswritten0_transfers += 1;
	#endif
	return;
}
void kernelprocess::writeAU_Vs_ddr0_w0_sw2(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs02 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS02_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs02:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs02:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0 += VECTOR_SIZE;
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
	kvpairswritten0_transfers += 1;
	#endif
	return;
}
void kernelprocess::writeAU_Vs_ddr0_w0_sw3(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs03 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS03_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs03:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs03:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0 += VECTOR_SIZE;
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
	kvpairswritten0_transfers += 1;
	#endif
	return;
}
void kernelprocess::writeAU_Vs_ddr0_w0_sw4(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs04 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS04_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs04:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs04:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0 += VECTOR_SIZE;
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
	kvpairswritten0_transfers += 1;
	#endif
	return;
}
void kernelprocess::writeAU_Vs_ddr0_w0_sw5(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs05 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS05_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs05:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs05:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0 += VECTOR_SIZE;
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
	kvpairswritten0_transfers += 1;
	#endif
	return;
}
void kernelprocess::writeAU_Vs_ddr0_w0_sw6(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs06 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS06_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs06:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs06:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0 += VECTOR_SIZE;
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
	kvpairswritten0_transfers += 1;
	#endif
	return;
}
void kernelprocess::writeAU_Vs_ddr0_w0_sw7(unsigned int ssdpartitonid, unsigned int workerid, uint512_dt * kvdram, unsigned int offset_vs, unsigned int baseaddr_vs, uint512_dt verticesbuffer[APPLYVERTEXBUFFERSZ_KVS]){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::writeAU_Vs07 read. workerid: "<<workerid<<std::endl;
	#endif
	unsigned int verticesbuffersz_kvs = APPLYVERTEXBUFFERSZ_KVS;  // NB: debugging purposes only
	
	kvbuffersz_dtype rwsize_kvs = kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid);
	if(rwsize_kvs > FORCDFINISH_APPLYVERTEXBUFFERSZ_KVS){ rwsize_kvs = 0; } 
	dramsz_dtype dramoffset = baseaddr_vs + offset_vs;
	if((dramoffset + rwsize_kvs - 1) >= _x3PADDEDKVDATA_SIZE_KVS){ dramoffset = 0; }
	WRITEBURSTKVS07_AU_LOOP: for(kvbuffersz_dtype i=0; i<rwsize_kvs; i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=verticesbuffersz_kvs avg=verticesbuffersz_kvs
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS
		checkforoutofbounds_kernel((offset_vs + i), _x3PADDEDKVDATA_SIZE_KVS, "writeAU_Vs07:: out-of-bounds seen at read 5", 0, 0, 0);
		checkforoutofbounds_kernel(i, kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid), "writeAU_Vs07:: out-of-bounds seen at read 6", 0, 0, 0);
		#endif
		#ifdef _DEBUGMODE_STATS
		kvpairswritten0 += VECTOR_SIZE;
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
	kvpairswritten0_transfers += 1;
	#endif
	return;
}

// READ MANY
void kernelprocess::readmany_ddr0_w0(unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs
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
	
		
kernelprocess::read_ddr0_w0_sw0(worker0id, kvdram, baseaddr0_kvs, offset0_kvs, kvsize0_kvs, kvbuffer0);
	/// read_ddr0_w0_sw0((beginworkerid + 0), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (0 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer0);
kernelprocess::read_ddr0_w0_sw1(worker1id, kvdram, baseaddr1_kvs, offset1_kvs, kvsize1_kvs, kvbuffer1);
	/// read_ddr0_w0_sw1((beginworkerid + 1), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (1 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer1);
kernelprocess::read_ddr0_w0_sw2(worker2id, kvdram, baseaddr2_kvs, offset2_kvs, kvsize2_kvs, kvbuffer2);
	/// read_ddr0_w0_sw2((beginworkerid + 2), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (2 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer2);
kernelprocess::read_ddr0_w0_sw3(worker3id, kvdram, baseaddr3_kvs, offset3_kvs, kvsize3_kvs, kvbuffer3);
	/// read_ddr0_w0_sw3((beginworkerid + 3), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (3 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer3);
kernelprocess::read_ddr0_w0_sw4(worker4id, kvdram, baseaddr4_kvs, offset4_kvs, kvsize4_kvs, kvbuffer4);
	/// read_ddr0_w0_sw4((beginworkerid + 4), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (4 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer4);
kernelprocess::read_ddr0_w0_sw5(worker5id, kvdram, baseaddr5_kvs, offset5_kvs, kvsize5_kvs, kvbuffer5);
	/// read_ddr0_w0_sw5((beginworkerid + 5), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (5 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer5);
kernelprocess::read_ddr0_w0_sw6(worker6id, kvdram, baseaddr6_kvs, offset6_kvs, kvsize6_kvs, kvbuffer6);
	/// read_ddr0_w0_sw6((beginworkerid + 6), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (6 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer6);
kernelprocess::read_ddr0_w0_sw7(worker7id, kvdram, baseaddr7_kvs, offset7_kvs, kvsize7_kvs, kvbuffer7);
	/// read_ddr0_w0_sw7((beginworkerid + 7), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (7 * KVBUFFERSZ_KVS)), kvsize_kvs, kvbuffer7);
	return;
}

void kernelprocess::readmanyAU_Vs_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs
	,uint512_dt verticesbuffer0[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer1[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer2[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer3[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer4[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer5[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer6[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer7[APPLYVERTEXBUFFERSZ_KVS]
	){
kernelprocess::readAU_V_ddr0_w0_sw0(ssdpartitonid, (beginworkerid + 0), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (0 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer0);
kernelprocess::readAU_V_ddr0_w0_sw1(ssdpartitonid, (beginworkerid + 1), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (1 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer1);
kernelprocess::readAU_V_ddr0_w0_sw2(ssdpartitonid, (beginworkerid + 2), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (2 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer2);
kernelprocess::readAU_V_ddr0_w0_sw3(ssdpartitonid, (beginworkerid + 3), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (3 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer3);
kernelprocess::readAU_V_ddr0_w0_sw4(ssdpartitonid, (beginworkerid + 4), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (4 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer4);
kernelprocess::readAU_V_ddr0_w0_sw5(ssdpartitonid, (beginworkerid + 5), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (5 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer5);
kernelprocess::readAU_V_ddr0_w0_sw6(ssdpartitonid, (beginworkerid + 6), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (6 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer6);
kernelprocess::readAU_V_ddr0_w0_sw7(ssdpartitonid, (beginworkerid + 7), kvdram, baseaddr_kvs, (kvreadoffset_kvs + (7 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), kvsize_kvs, verticesbuffer7);
	return;
}

void kernelprocess::readmanyAU_VUs_ddr0_w0(unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_kvs, metadata_t sourcestatbuffer[NUMSUBWORKERS], unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs
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
	
kernelprocess::readAU_VU_ddr0_w0_sw0(worker0id, kvdram, baseaddr0_kvs, offset0_kvs, kvsize0_kvs, kvbuffer0);
kernelprocess::readAU_VU_ddr0_w0_sw0(worker1id, kvdram, baseaddr1_kvs, offset1_kvs, kvsize1_kvs, kvbuffer1);
kernelprocess::readAU_VU_ddr0_w0_sw0(worker2id, kvdram, baseaddr2_kvs, offset2_kvs, kvsize2_kvs, kvbuffer2);
kernelprocess::readAU_VU_ddr0_w0_sw0(worker3id, kvdram, baseaddr3_kvs, offset3_kvs, kvsize3_kvs, kvbuffer3);
kernelprocess::readAU_VU_ddr0_w0_sw0(worker4id, kvdram, baseaddr4_kvs, offset4_kvs, kvsize4_kvs, kvbuffer4);
kernelprocess::readAU_VU_ddr0_w0_sw0(worker5id, kvdram, baseaddr5_kvs, offset5_kvs, kvsize5_kvs, kvbuffer5);
kernelprocess::readAU_VU_ddr0_w0_sw0(worker6id, kvdram, baseaddr6_kvs, offset6_kvs, kvsize6_kvs, kvbuffer6);
kernelprocess::readAU_VU_ddr0_w0_sw0(worker7id, kvdram, baseaddr7_kvs, offset7_kvs, kvsize7_kvs, kvbuffer7);
	return;
}

// PARTITION MANY
void kernelprocess::partitionmany_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange
	,uint512_dt kvbuffer01[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer02[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS], smallmargin_t ov0[NUM_PARTITIONS], uint512_dt ovkvs_tmp0[NUM_PARTITIONS]
	,uint512_dt kvbuffer11[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer12[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS], smallmargin_t ov1[NUM_PARTITIONS], uint512_dt ovkvs_tmp1[NUM_PARTITIONS]
	,uint512_dt kvbuffer21[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer22[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS], smallmargin_t ov2[NUM_PARTITIONS], uint512_dt ovkvs_tmp2[NUM_PARTITIONS]
	,uint512_dt kvbuffer31[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer32[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS], smallmargin_t ov3[NUM_PARTITIONS], uint512_dt ovkvs_tmp3[NUM_PARTITIONS]
	,uint512_dt kvbuffer41[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer42[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS], smallmargin_t ov4[NUM_PARTITIONS], uint512_dt ovkvs_tmp4[NUM_PARTITIONS]
	,uint512_dt kvbuffer51[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer52[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS], smallmargin_t ov5[NUM_PARTITIONS], uint512_dt ovkvs_tmp5[NUM_PARTITIONS]
	,uint512_dt kvbuffer61[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer62[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS], smallmargin_t ov6[NUM_PARTITIONS], uint512_dt ovkvs_tmp6[NUM_PARTITIONS]
	,uint512_dt kvbuffer71[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer72[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS], smallmargin_t ov7[NUM_PARTITIONS], uint512_dt ovkvs_tmp7[NUM_PARTITIONS]
	){
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
	
kernelprocess::partition_ddr0_w0_sw0(ssdpartiton0id, worker0id, subworker0id, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, kvbuffer01, kvbuffer02, kvbufferstats_offset0, kvbufferstats_size0, ov0, ovkvs_tmp0);
kernelprocess::partition_ddr0_w0_sw1(ssdpartiton1id, worker1id, subworker1id, kvreadoffset1_kvs, kvsize1_kvs, currentLOP1, destpartitionoffset1, upperlimit1, destpartitionrange1, kvbuffer11, kvbuffer12, kvbufferstats_offset1, kvbufferstats_size1, ov1, ovkvs_tmp1);
kernelprocess::partition_ddr0_w0_sw2(ssdpartiton2id, worker2id, subworker2id, kvreadoffset2_kvs, kvsize2_kvs, currentLOP2, destpartitionoffset2, upperlimit2, destpartitionrange2, kvbuffer21, kvbuffer22, kvbufferstats_offset2, kvbufferstats_size2, ov2, ovkvs_tmp2);
kernelprocess::partition_ddr0_w0_sw3(ssdpartiton3id, worker3id, subworker3id, kvreadoffset3_kvs, kvsize3_kvs, currentLOP3, destpartitionoffset3, upperlimit3, destpartitionrange3, kvbuffer31, kvbuffer32, kvbufferstats_offset3, kvbufferstats_size3, ov3, ovkvs_tmp3);
kernelprocess::partition_ddr0_w0_sw4(ssdpartiton4id, worker4id, subworker4id, kvreadoffset4_kvs, kvsize4_kvs, currentLOP4, destpartitionoffset4, upperlimit4, destpartitionrange4, kvbuffer41, kvbuffer42, kvbufferstats_offset4, kvbufferstats_size4, ov4, ovkvs_tmp4);
kernelprocess::partition_ddr0_w0_sw5(ssdpartiton5id, worker5id, subworker5id, kvreadoffset5_kvs, kvsize5_kvs, currentLOP5, destpartitionoffset5, upperlimit5, destpartitionrange5, kvbuffer51, kvbuffer52, kvbufferstats_offset5, kvbufferstats_size5, ov5, ovkvs_tmp5);
kernelprocess::partition_ddr0_w0_sw6(ssdpartiton6id, worker6id, subworker6id, kvreadoffset6_kvs, kvsize6_kvs, currentLOP6, destpartitionoffset6, upperlimit6, destpartitionrange6, kvbuffer61, kvbuffer62, kvbufferstats_offset6, kvbufferstats_size6, ov6, ovkvs_tmp6);
kernelprocess::partition_ddr0_w0_sw7(ssdpartiton7id, worker7id, subworker7id, kvreadoffset7_kvs, kvsize7_kvs, currentLOP7, destpartitionoffset7, upperlimit7, destpartitionrange7, kvbuffer71, kvbuffer72, kvbufferstats_offset7, kvbufferstats_size7, ov7, ovkvs_tmp7);
	return;
}

#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::extractmanystats2_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange
	,uint512_dt kvbuffer01[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer02[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS], smallmargin_t ov0[NUM_PARTITIONS], uint512_dt ovkvs_tmp0[NUM_PARTITIONS]
	,uint512_dt kvbuffer11[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer12[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS], smallmargin_t ov1[NUM_PARTITIONS], uint512_dt ovkvs_tmp1[NUM_PARTITIONS]
	,uint512_dt kvbuffer21[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer22[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS], smallmargin_t ov2[NUM_PARTITIONS], uint512_dt ovkvs_tmp2[NUM_PARTITIONS]
	,uint512_dt kvbuffer31[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer32[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS], smallmargin_t ov3[NUM_PARTITIONS], uint512_dt ovkvs_tmp3[NUM_PARTITIONS]
	,uint512_dt kvbuffer41[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer42[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS], smallmargin_t ov4[NUM_PARTITIONS], uint512_dt ovkvs_tmp4[NUM_PARTITIONS]
	,uint512_dt kvbuffer51[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer52[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS], smallmargin_t ov5[NUM_PARTITIONS], uint512_dt ovkvs_tmp5[NUM_PARTITIONS]
	,uint512_dt kvbuffer61[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer62[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS], smallmargin_t ov6[NUM_PARTITIONS], uint512_dt ovkvs_tmp6[NUM_PARTITIONS]
	,uint512_dt kvbuffer71[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer72[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS], smallmargin_t ov7[NUM_PARTITIONS], uint512_dt ovkvs_tmp7[NUM_PARTITIONS]
	){	
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
	
		
kernelprocess::extractstats2_ddr0_w0_sw0(ssdpartiton0id, worker0id, subworker0id, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, kvbuffer01, kvbuffer02, kvbufferstats_offset0, kvbufferstats_size0, ov0, ovkvs_tmp0);
kernelprocess::extractstats2_ddr0_w0_sw1(ssdpartiton1id, worker1id, subworker1id, kvreadoffset1_kvs, kvsize1_kvs, currentLOP1, destpartitionoffset1, upperlimit1, destpartitionrange1, kvbuffer11, kvbuffer12, kvbufferstats_offset1, kvbufferstats_size1, ov1, ovkvs_tmp1);
kernelprocess::extractstats2_ddr0_w0_sw2(ssdpartiton2id, worker2id, subworker2id, kvreadoffset2_kvs, kvsize2_kvs, currentLOP2, destpartitionoffset2, upperlimit2, destpartitionrange2, kvbuffer21, kvbuffer22, kvbufferstats_offset2, kvbufferstats_size2, ov2, ovkvs_tmp2);
kernelprocess::extractstats2_ddr0_w0_sw3(ssdpartiton3id, worker3id, subworker3id, kvreadoffset3_kvs, kvsize3_kvs, currentLOP3, destpartitionoffset3, upperlimit3, destpartitionrange3, kvbuffer31, kvbuffer32, kvbufferstats_offset3, kvbufferstats_size3, ov3, ovkvs_tmp3);
kernelprocess::extractstats2_ddr0_w0_sw4(ssdpartiton4id, worker4id, subworker4id, kvreadoffset4_kvs, kvsize4_kvs, currentLOP4, destpartitionoffset4, upperlimit4, destpartitionrange4, kvbuffer41, kvbuffer42, kvbufferstats_offset4, kvbufferstats_size4, ov4, ovkvs_tmp4);
kernelprocess::extractstats2_ddr0_w0_sw5(ssdpartiton5id, worker5id, subworker5id, kvreadoffset5_kvs, kvsize5_kvs, currentLOP5, destpartitionoffset5, upperlimit5, destpartitionrange5, kvbuffer51, kvbuffer52, kvbufferstats_offset5, kvbufferstats_size5, ov5, ovkvs_tmp5);
kernelprocess::extractstats2_ddr0_w0_sw6(ssdpartiton6id, worker6id, subworker6id, kvreadoffset6_kvs, kvsize6_kvs, currentLOP6, destpartitionoffset6, upperlimit6, destpartitionrange6, kvbuffer61, kvbuffer62, kvbufferstats_offset6, kvbufferstats_size6, ov6, ovkvs_tmp6);
kernelprocess::extractstats2_ddr0_w0_sw7(ssdpartiton7id, worker7id, subworker7id, kvreadoffset7_kvs, kvsize7_kvs, currentLOP7, destpartitionoffset7, upperlimit7, destpartitionrange7, kvbuffer71, kvbuffer72, kvbufferstats_offset7, kvbufferstats_size7, ov7, ovkvs_tmp7);
	return;
}
#endif 

#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
void kernelprocess::partitionmany2_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, unsigned int kvreadoffset_kvs, unsigned int kvsize_kvs, unsigned int currentLOP, unsigned int destpartitionoffset, unsigned int upperlimit, unsigned int destpartitionrange
	,uint512_dt kvbuffer01[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer02[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS], smallmargin_t ov0[NUM_PARTITIONS], uint512_dt ovkvs_tmp0[NUM_PARTITIONS]
	,uint512_dt kvbuffer11[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer12[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS], smallmargin_t ov1[NUM_PARTITIONS], uint512_dt ovkvs_tmp1[NUM_PARTITIONS]
	,uint512_dt kvbuffer21[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer22[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS], smallmargin_t ov2[NUM_PARTITIONS], uint512_dt ovkvs_tmp2[NUM_PARTITIONS]
	,uint512_dt kvbuffer31[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer32[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS], smallmargin_t ov3[NUM_PARTITIONS], uint512_dt ovkvs_tmp3[NUM_PARTITIONS]
	,uint512_dt kvbuffer41[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer42[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS], smallmargin_t ov4[NUM_PARTITIONS], uint512_dt ovkvs_tmp4[NUM_PARTITIONS]
	,uint512_dt kvbuffer51[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer52[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS], smallmargin_t ov5[NUM_PARTITIONS], uint512_dt ovkvs_tmp5[NUM_PARTITIONS]
	,uint512_dt kvbuffer61[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer62[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS], smallmargin_t ov6[NUM_PARTITIONS], uint512_dt ovkvs_tmp6[NUM_PARTITIONS]
	,uint512_dt kvbuffer71[KVSRCBUFFERSZ_KVS], uint512_dt kvbuffer72[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS], smallmargin_t ov7[NUM_PARTITIONS], uint512_dt ovkvs_tmp7[NUM_PARTITIONS]
	){
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
	
kernelprocess::partition2_ddr0_w0_sw0(ssdpartiton0id, worker0id, subworker0id, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0, kvbuffer01, kvbuffer02, kvbufferstats_offset0, kvbufferstats_size0, ov0, ovkvs_tmp0);
kernelprocess::partition2_ddr0_w0_sw1(ssdpartiton1id, worker1id, subworker1id, kvreadoffset1_kvs, kvsize1_kvs, currentLOP1, destpartitionoffset1, upperlimit1, destpartitionrange1, kvbuffer11, kvbuffer12, kvbufferstats_offset1, kvbufferstats_size1, ov1, ovkvs_tmp1);
kernelprocess::partition2_ddr0_w0_sw2(ssdpartiton2id, worker2id, subworker2id, kvreadoffset2_kvs, kvsize2_kvs, currentLOP2, destpartitionoffset2, upperlimit2, destpartitionrange2, kvbuffer21, kvbuffer22, kvbufferstats_offset2, kvbufferstats_size2, ov2, ovkvs_tmp2);
kernelprocess::partition2_ddr0_w0_sw3(ssdpartiton3id, worker3id, subworker3id, kvreadoffset3_kvs, kvsize3_kvs, currentLOP3, destpartitionoffset3, upperlimit3, destpartitionrange3, kvbuffer31, kvbuffer32, kvbufferstats_offset3, kvbufferstats_size3, ov3, ovkvs_tmp3);
kernelprocess::partition2_ddr0_w0_sw4(ssdpartiton4id, worker4id, subworker4id, kvreadoffset4_kvs, kvsize4_kvs, currentLOP4, destpartitionoffset4, upperlimit4, destpartitionrange4, kvbuffer41, kvbuffer42, kvbufferstats_offset4, kvbufferstats_size4, ov4, ovkvs_tmp4);
kernelprocess::partition2_ddr0_w0_sw5(ssdpartiton5id, worker5id, subworker5id, kvreadoffset5_kvs, kvsize5_kvs, currentLOP5, destpartitionoffset5, upperlimit5, destpartitionrange5, kvbuffer51, kvbuffer52, kvbufferstats_offset5, kvbufferstats_size5, ov5, ovkvs_tmp5);
kernelprocess::partition2_ddr0_w0_sw6(ssdpartiton6id, worker6id, subworker6id, kvreadoffset6_kvs, kvsize6_kvs, currentLOP6, destpartitionoffset6, upperlimit6, destpartitionrange6, kvbuffer61, kvbuffer62, kvbufferstats_offset6, kvbufferstats_size6, ov6, ovkvs_tmp6);
kernelprocess::partition2_ddr0_w0_sw7(ssdpartiton7id, worker7id, subworker7id, kvreadoffset7_kvs, kvsize7_kvs, currentLOP7, destpartitionoffset7, upperlimit7, destpartitionrange7, kvbuffer71, kvbuffer72, kvbufferstats_offset7, kvbufferstats_size7, ov7, ovkvs_tmp7);
	return;
}
#endif 

// APPLY MANY
void kernelprocess::reducemany_ddr0_w0(unsigned int IterCount, unsigned int ssdpartitonid, unsigned int beginworkerid, unsigned int verticesreadoffset_vs, metadata_t sourcestatbuffer[NUMSUBWORKERS], unsigned int kvreadoffset_kvs
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
	unsigned int verticesreadoffset0_vs = verticesreadoffset_vs + (0 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset0_kvs = kvreadoffset_kvs; 
	unsigned int kvsize0_kvs = (sourcestatbuffer[0].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton1id = ssdpartitonid; 
	unsigned int worker1id = beginworkerid + 1; 
	unsigned int verticesreadoffset1_vs = verticesreadoffset_vs + (1 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset1_kvs = kvreadoffset_kvs; 
	unsigned int kvsize1_kvs = (sourcestatbuffer[1].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton2id = ssdpartitonid; 
	unsigned int worker2id = beginworkerid + 2; 
	unsigned int verticesreadoffset2_vs = verticesreadoffset_vs + (2 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset2_kvs = kvreadoffset_kvs; 
	unsigned int kvsize2_kvs = (sourcestatbuffer[2].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton3id = ssdpartitonid; 
	unsigned int worker3id = beginworkerid + 3; 
	unsigned int verticesreadoffset3_vs = verticesreadoffset_vs + (3 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset3_kvs = kvreadoffset_kvs; 
	unsigned int kvsize3_kvs = (sourcestatbuffer[3].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton4id = ssdpartitonid; 
	unsigned int worker4id = beginworkerid + 4; 
	unsigned int verticesreadoffset4_vs = verticesreadoffset_vs + (4 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset4_kvs = kvreadoffset_kvs; 
	unsigned int kvsize4_kvs = (sourcestatbuffer[4].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton5id = ssdpartitonid; 
	unsigned int worker5id = beginworkerid + 5; 
	unsigned int verticesreadoffset5_vs = verticesreadoffset_vs + (5 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset5_kvs = kvreadoffset_kvs; 
	unsigned int kvsize5_kvs = (sourcestatbuffer[5].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton6id = ssdpartitonid; 
	unsigned int worker6id = beginworkerid + 6; 
	unsigned int verticesreadoffset6_vs = verticesreadoffset_vs + (6 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset6_kvs = kvreadoffset_kvs; 
	unsigned int kvsize6_kvs = (sourcestatbuffer[6].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	unsigned int ssdpartiton7id = ssdpartitonid; 
	unsigned int worker7id = beginworkerid + 7; 
	unsigned int verticesreadoffset7_vs = verticesreadoffset_vs + (7 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)); 
	unsigned int kvreadoffset7_kvs = kvreadoffset_kvs; 
	unsigned int kvsize7_kvs = (sourcestatbuffer[7].size + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw0: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:0: "<<((verticesreadoffset_vs + (0 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw1: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:1: "<<((verticesreadoffset_vs + (1 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw2: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:2: "<<((verticesreadoffset_vs + (2 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw3: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:3: "<<((verticesreadoffset_vs + (3 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw4: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:4: "<<((verticesreadoffset_vs + (4 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw5: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:5: "<<((verticesreadoffset_vs + (5 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw6: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:6: "<<((verticesreadoffset_vs + (6 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	#ifdef _DEBUGMODE_PRINTS
	cout<<"reduce_ddr0_sw7: verticesreadoffset: "<<(verticesreadoffset_vs * VECTOR_SIZE)<<", verticesreadoffset for subworker:7: "<<((verticesreadoffset_vs + (7 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))) * VECTOR_SIZE)<<endl;
	#endif
	
kernelprocess::reduce_ddr0_w0_sw0(IterCount, ssdpartiton0id, worker0id, verticesreadoffset0_vs, kvreadoffset0_kvs, kvsize0_kvs, sourcestatbuffer[0], verticesbuffer0, kvbuffer0); // FIXME
kernelprocess::reduce_ddr0_w0_sw1(IterCount, ssdpartiton1id, worker1id, verticesreadoffset1_vs, kvreadoffset1_kvs, kvsize1_kvs, sourcestatbuffer[1], verticesbuffer1, kvbuffer1); // FIXME
kernelprocess::reduce_ddr0_w0_sw2(IterCount, ssdpartiton2id, worker2id, verticesreadoffset2_vs, kvreadoffset2_kvs, kvsize2_kvs, sourcestatbuffer[2], verticesbuffer2, kvbuffer2); // FIXME
kernelprocess::reduce_ddr0_w0_sw3(IterCount, ssdpartiton3id, worker3id, verticesreadoffset3_vs, kvreadoffset3_kvs, kvsize3_kvs, sourcestatbuffer[3], verticesbuffer3, kvbuffer3); // FIXME
kernelprocess::reduce_ddr0_w0_sw4(IterCount, ssdpartiton4id, worker4id, verticesreadoffset4_vs, kvreadoffset4_kvs, kvsize4_kvs, sourcestatbuffer[4], verticesbuffer4, kvbuffer4); // FIXME
kernelprocess::reduce_ddr0_w0_sw5(IterCount, ssdpartiton5id, worker5id, verticesreadoffset5_vs, kvreadoffset5_kvs, kvsize5_kvs, sourcestatbuffer[5], verticesbuffer5, kvbuffer5); // FIXME
kernelprocess::reduce_ddr0_w0_sw6(IterCount, ssdpartiton6id, worker6id, verticesreadoffset6_vs, kvreadoffset6_kvs, kvsize6_kvs, sourcestatbuffer[6], verticesbuffer6, kvbuffer6); // FIXME
kernelprocess::reduce_ddr0_w0_sw7(IterCount, ssdpartiton7id, worker7id, verticesreadoffset7_vs, kvreadoffset7_kvs, kvsize7_kvs, sourcestatbuffer[7], verticesbuffer7, kvbuffer7); // FIXME
	return;
}

// WRITE MANY
void kernelprocess::writemany_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], unsigned int baseaddr_kvs, unsigned int currentLOP, unsigned int destpartitionoffset	
	,uint512_dt kvbuffer0[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS]
	,uint512_dt kvbuffer1[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS]
	,uint512_dt kvbuffer2[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS]
	,uint512_dt kvbuffer3[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS]
	,uint512_dt kvbuffer4[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS]
	,uint512_dt kvbuffer5[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS]
	,uint512_dt kvbuffer6[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS]
	,uint512_dt kvbuffer7[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS]
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
	
kernelprocess::write_ddr0_w0_sw0(ssdpartiton0id, worker0id, currentLOP0, destpartitionoffset0, kvbuffer0, kvbufferstats_offset0, kvbufferstats_size0, kvdram, kvdeststats_tmp, baseaddr0_kvs);
kernelprocess::write_ddr0_w0_sw1(ssdpartiton1id, worker1id, currentLOP1, destpartitionoffset1, kvbuffer1, kvbufferstats_offset1, kvbufferstats_size1, kvdram, kvdeststats_tmp, baseaddr1_kvs);
kernelprocess::write_ddr0_w0_sw2(ssdpartiton2id, worker2id, currentLOP2, destpartitionoffset2, kvbuffer2, kvbufferstats_offset2, kvbufferstats_size2, kvdram, kvdeststats_tmp, baseaddr2_kvs);
kernelprocess::write_ddr0_w0_sw3(ssdpartiton3id, worker3id, currentLOP3, destpartitionoffset3, kvbuffer3, kvbufferstats_offset3, kvbufferstats_size3, kvdram, kvdeststats_tmp, baseaddr3_kvs);
kernelprocess::write_ddr0_w0_sw4(ssdpartiton4id, worker4id, currentLOP4, destpartitionoffset4, kvbuffer4, kvbufferstats_offset4, kvbufferstats_size4, kvdram, kvdeststats_tmp, baseaddr4_kvs);
kernelprocess::write_ddr0_w0_sw5(ssdpartiton5id, worker5id, currentLOP5, destpartitionoffset5, kvbuffer5, kvbufferstats_offset5, kvbufferstats_size5, kvdram, kvdeststats_tmp, baseaddr5_kvs);
kernelprocess::write_ddr0_w0_sw6(ssdpartiton6id, worker6id, currentLOP6, destpartitionoffset6, kvbuffer6, kvbufferstats_offset6, kvbufferstats_size6, kvdram, kvdeststats_tmp, baseaddr6_kvs);
kernelprocess::write_ddr0_w0_sw7(ssdpartiton7id, worker7id, currentLOP7, destpartitionoffset7, kvbuffer7, kvbufferstats_offset7, kvbufferstats_size7, kvdram, kvdeststats_tmp, baseaddr7_kvs);
	return;
}

void kernelprocess::writemanyAU_Vs_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, unsigned int baseaddr_vs, unsigned int kvreadoffset_kvs
	,uint512_dt verticesbuffer0[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer1[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer2[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer3[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer4[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer5[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer6[APPLYVERTEXBUFFERSZ_KVS]
	,uint512_dt verticesbuffer7[APPLYVERTEXBUFFERSZ_KVS]
	){
kernelprocess::writeAU_Vs_ddr0_w0_sw0(ssdpartitonid, (beginworkerid + 0), kvdram, (kvreadoffset_kvs + (0 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer0);
kernelprocess::writeAU_Vs_ddr0_w0_sw1(ssdpartitonid, (beginworkerid + 1), kvdram, (kvreadoffset_kvs + (1 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer1);
kernelprocess::writeAU_Vs_ddr0_w0_sw2(ssdpartitonid, (beginworkerid + 2), kvdram, (kvreadoffset_kvs + (2 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer2);
kernelprocess::writeAU_Vs_ddr0_w0_sw3(ssdpartitonid, (beginworkerid + 3), kvdram, (kvreadoffset_kvs + (3 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer3);
kernelprocess::writeAU_Vs_ddr0_w0_sw4(ssdpartitonid, (beginworkerid + 4), kvdram, (kvreadoffset_kvs + (4 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer4);
kernelprocess::writeAU_Vs_ddr0_w0_sw5(ssdpartitonid, (beginworkerid + 5), kvdram, (kvreadoffset_kvs + (5 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer5);
kernelprocess::writeAU_Vs_ddr0_w0_sw6(ssdpartitonid, (beginworkerid + 6), kvdram, (kvreadoffset_kvs + (6 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer6);
kernelprocess::writeAU_Vs_ddr0_w0_sw7(ssdpartitonid, (beginworkerid + 7), kvdram, (kvreadoffset_kvs + (7 * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))), baseaddr_vs, verticesbuffer7);
	return;
}

#ifdef _BUFFERB4WRITE
void kernelprocess::writemany_bram2tmp_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], unsigned int baseaddr_kvs, unsigned int currentLOP, unsigned int destpartitionoffset
	,uint512_dt kvbuffer0[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS]
	,uint512_dt kvbuffer1[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS]
	,uint512_dt kvbuffer2[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS]
	,uint512_dt kvbuffer3[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS]
	,uint512_dt kvbuffer4[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS]
	,uint512_dt kvbuffer5[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS]
	,uint512_dt kvbuffer6[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS]
	,uint512_dt kvbuffer7[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS]
	,unsigned int ibegin_kvs, unsigned int iend_kvs
	){
	#pragma HLS INLINE OFF
	unsigned int ssdpartiton0id = ssdpartitonid;
	unsigned int worker0id = beginworkerid + 0;
	unsigned int baseaddr0_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton1id = ssdpartitonid;
	unsigned int worker1id = beginworkerid + 1;
	unsigned int baseaddr1_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton2id = ssdpartitonid;
	unsigned int worker2id = beginworkerid + 2;
	unsigned int baseaddr2_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton3id = ssdpartitonid;
	unsigned int worker3id = beginworkerid + 3;
	unsigned int baseaddr3_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton4id = ssdpartitonid;
	unsigned int worker4id = beginworkerid + 4;
	unsigned int baseaddr4_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton5id = ssdpartitonid;
	unsigned int worker5id = beginworkerid + 5;
	unsigned int baseaddr5_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton6id = ssdpartitonid;
	unsigned int worker6id = beginworkerid + 6;
	unsigned int baseaddr6_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton7id = ssdpartitonid;
	unsigned int worker7id = beginworkerid + 7;
	unsigned int baseaddr7_kvs = baseaddr_kvs;
	
 // FIXME? use single logic?
kernelprocess::write_bram2tmp_ddr0_w0_sw0(ssdpartiton0id, worker0id, kvbuffer0, DESTtmpkvbuffer, DESTtmpkvbuffer_weof, kvbufferstats_offset0, kvbufferstats_size0, baseaddr0_kvs);	
 // FIXME? use single logic?
kernelprocess::write_bram2tmp_ddr0_w0_sw1(ssdpartiton1id, worker1id, kvbuffer1, DESTtmpkvbuffer, DESTtmpkvbuffer_weof, kvbufferstats_offset1, kvbufferstats_size1, baseaddr1_kvs);	
 // FIXME? use single logic?
kernelprocess::write_bram2tmp_ddr0_w0_sw2(ssdpartiton2id, worker2id, kvbuffer2, DESTtmpkvbuffer, DESTtmpkvbuffer_weof, kvbufferstats_offset2, kvbufferstats_size2, baseaddr2_kvs);	
 // FIXME? use single logic?
kernelprocess::write_bram2tmp_ddr0_w0_sw3(ssdpartiton3id, worker3id, kvbuffer3, DESTtmpkvbuffer, DESTtmpkvbuffer_weof, kvbufferstats_offset3, kvbufferstats_size3, baseaddr3_kvs);	
 // FIXME? use single logic?
kernelprocess::write_bram2tmp_ddr0_w0_sw4(ssdpartiton4id, worker4id, kvbuffer4, DESTtmpkvbuffer, DESTtmpkvbuffer_weof, kvbufferstats_offset4, kvbufferstats_size4, baseaddr4_kvs);	
 // FIXME? use single logic?
kernelprocess::write_bram2tmp_ddr0_w0_sw5(ssdpartiton5id, worker5id, kvbuffer5, DESTtmpkvbuffer, DESTtmpkvbuffer_weof, kvbufferstats_offset5, kvbufferstats_size5, baseaddr5_kvs);	
 // FIXME? use single logic?
kernelprocess::write_bram2tmp_ddr0_w0_sw6(ssdpartiton6id, worker6id, kvbuffer6, DESTtmpkvbuffer, DESTtmpkvbuffer_weof, kvbufferstats_offset6, kvbufferstats_size6, baseaddr6_kvs);	
 // FIXME? use single logic?
kernelprocess::write_bram2tmp_ddr0_w0_sw7(ssdpartiton7id, worker7id, kvbuffer7, DESTtmpkvbuffer, DESTtmpkvbuffer_weof, kvbufferstats_offset7, kvbufferstats_size7, baseaddr7_kvs);	
	return;
}
#endif 

#ifdef _BUFFERB4WRITE
void kernelprocess::accel1_writemany_bram2tmp_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid
	,uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]
	,uint512_dt kvbuffer0[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset_sw0[NUM_PARTITIONS], smalluint_t kvbufferstats_size_sw0[NUM_PARTITIONS]
	,uint512_dt kvbuffer1[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset_sw1[NUM_PARTITIONS], smalluint_t kvbufferstats_size_sw1[NUM_PARTITIONS]
	,uint512_dt kvbuffer2[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset_sw2[NUM_PARTITIONS], smalluint_t kvbufferstats_size_sw2[NUM_PARTITIONS]
	,uint512_dt kvbuffer3[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset_sw3[NUM_PARTITIONS], smalluint_t kvbufferstats_size_sw3[NUM_PARTITIONS]
	,uint512_dt kvbuffer4[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset_sw4[NUM_PARTITIONS], smalluint_t kvbufferstats_size_sw4[NUM_PARTITIONS]
	,uint512_dt kvbuffer5[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset_sw5[NUM_PARTITIONS], smalluint_t kvbufferstats_size_sw5[NUM_PARTITIONS]
	,uint512_dt kvbuffer6[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset_sw6[NUM_PARTITIONS], smalluint_t kvbufferstats_size_sw6[NUM_PARTITIONS]
	,uint512_dt kvbuffer7[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset_sw7[NUM_PARTITIONS], smalluint_t kvbufferstats_size_sw7[NUM_PARTITIONS]
	,unsigned int dummy
	){
	unsigned int ssdpartiton_sw0id = ssdpartitonid;
	unsigned int worker_sw0id = beginworkerid + 0;
	unsigned int ssdpartiton_sw1id = ssdpartitonid;
	unsigned int worker_sw1id = beginworkerid + 1;
	unsigned int ssdpartiton_sw2id = ssdpartitonid;
	unsigned int worker_sw2id = beginworkerid + 2;
	unsigned int ssdpartiton_sw3id = ssdpartitonid;
	unsigned int worker_sw3id = beginworkerid + 3;
	unsigned int ssdpartiton_sw4id = ssdpartitonid;
	unsigned int worker_sw4id = beginworkerid + 4;
	unsigned int ssdpartiton_sw5id = ssdpartitonid;
	unsigned int worker_sw5id = beginworkerid + 5;
	unsigned int ssdpartiton_sw6id = ssdpartitonid;
	unsigned int worker_sw6id = beginworkerid + 6;
	unsigned int ssdpartiton_sw7id = ssdpartitonid;
	unsigned int worker_sw7id = beginworkerid + 7;
	
kernelprocess::mergeoffsetsandsizes_ddr0_w0(ssdpartitonid, beginworkerid 
		,DESTtmpkvbufferstats_offset, DESTtmpkvbufferstats_size
		,kvbufferstats_size_sw0
		,kvbufferstats_size_sw1
		,kvbufferstats_size_sw2
		,kvbufferstats_size_sw3
		,kvbufferstats_size_sw4
		,kvbufferstats_size_sw5
		,kvbufferstats_size_sw6
		,kvbufferstats_size_sw7
		);
	
kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw0(ssdpartiton_sw0id, worker_sw0id
			,kvbuffer0, kvbufferstats_offset_sw0, kvbufferstats_size_sw0
			,DESTtmpkvbuffer, DESTtmpkvbufferstats_offset, DESTtmpkvbufferstats_size);
kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw1(ssdpartiton_sw1id, worker_sw1id
			,kvbuffer1, kvbufferstats_offset_sw1, kvbufferstats_size_sw1
			,DESTtmpkvbuffer, DESTtmpkvbufferstats_offset, DESTtmpkvbufferstats_size);
kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw2(ssdpartiton_sw2id, worker_sw2id
			,kvbuffer2, kvbufferstats_offset_sw2, kvbufferstats_size_sw2
			,DESTtmpkvbuffer, DESTtmpkvbufferstats_offset, DESTtmpkvbufferstats_size);
kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw3(ssdpartiton_sw3id, worker_sw3id
			,kvbuffer3, kvbufferstats_offset_sw3, kvbufferstats_size_sw3
			,DESTtmpkvbuffer, DESTtmpkvbufferstats_offset, DESTtmpkvbufferstats_size);
kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw4(ssdpartiton_sw4id, worker_sw4id
			,kvbuffer4, kvbufferstats_offset_sw4, kvbufferstats_size_sw4
			,DESTtmpkvbuffer, DESTtmpkvbufferstats_offset, DESTtmpkvbufferstats_size);
kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw5(ssdpartiton_sw5id, worker_sw5id
			,kvbuffer5, kvbufferstats_offset_sw5, kvbufferstats_size_sw5
			,DESTtmpkvbuffer, DESTtmpkvbufferstats_offset, DESTtmpkvbufferstats_size);
kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw6(ssdpartiton_sw6id, worker_sw6id
			,kvbuffer6, kvbufferstats_offset_sw6, kvbufferstats_size_sw6
			,DESTtmpkvbuffer, DESTtmpkvbufferstats_offset, DESTtmpkvbufferstats_size);
kernelprocess::accel1_write_bram2tmp_ddr0_w0_sw7(ssdpartiton_sw7id, worker_sw7id
			,kvbuffer7, kvbufferstats_offset_sw7, kvbufferstats_size_sw7
			,DESTtmpkvbuffer, DESTtmpkvbufferstats_offset, DESTtmpkvbufferstats_size);
	return;
}
#endif 

#ifdef _BUFFERB4WRITE
void kernelprocess::writemany_bram2dram_ddr0_w0(unsigned int ssdpartitonid, unsigned int beginworkerid, uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], unsigned int baseaddr_kvs, unsigned int currentLOP, unsigned int destpartitionoffset
	,uint512_dt kvbuffer0[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset0[NUM_PARTITIONS], smalluint_t kvbufferstats_size0[NUM_PARTITIONS]
	,uint512_dt kvbuffer1[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset1[NUM_PARTITIONS], smalluint_t kvbufferstats_size1[NUM_PARTITIONS]
	,uint512_dt kvbuffer2[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset2[NUM_PARTITIONS], smalluint_t kvbufferstats_size2[NUM_PARTITIONS]
	,uint512_dt kvbuffer3[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset3[NUM_PARTITIONS], smalluint_t kvbufferstats_size3[NUM_PARTITIONS]
	,uint512_dt kvbuffer4[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset4[NUM_PARTITIONS], smalluint_t kvbufferstats_size4[NUM_PARTITIONS]
	,uint512_dt kvbuffer5[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset5[NUM_PARTITIONS], smalluint_t kvbufferstats_size5[NUM_PARTITIONS]
	,uint512_dt kvbuffer6[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset6[NUM_PARTITIONS], smalluint_t kvbufferstats_size6[NUM_PARTITIONS]
	,uint512_dt kvbuffer7[KVDESTBUFFERSZ_KVS], smalluint_t kvbufferstats_offset7[NUM_PARTITIONS], smalluint_t kvbufferstats_size7[NUM_PARTITIONS]
	,unsigned int ibegin_kvs, unsigned int iend_kvs
	){
	unsigned int ssdpartiton0id = ssdpartitonid;
	unsigned int worker0id = beginworkerid + 0;
	unsigned int baseaddr0_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton1id = ssdpartitonid;
	unsigned int worker1id = beginworkerid + 1;
	unsigned int baseaddr1_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton2id = ssdpartitonid;
	unsigned int worker2id = beginworkerid + 2;
	unsigned int baseaddr2_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton3id = ssdpartitonid;
	unsigned int worker3id = beginworkerid + 3;
	unsigned int baseaddr3_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton4id = ssdpartitonid;
	unsigned int worker4id = beginworkerid + 4;
	unsigned int baseaddr4_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton5id = ssdpartitonid;
	unsigned int worker5id = beginworkerid + 5;
	unsigned int baseaddr5_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton6id = ssdpartitonid;
	unsigned int worker6id = beginworkerid + 6;
	unsigned int baseaddr6_kvs = baseaddr_kvs;
	
	unsigned int ssdpartiton7id = ssdpartitonid;
	unsigned int worker7id = beginworkerid + 7;
	unsigned int baseaddr7_kvs = baseaddr_kvs;
	
kernelprocess::write_bram2dram_ddr0_w0_sw0(ssdpartiton0id, worker0id, kvbuffer0, DESTtmpkvbuffer_weof, kvbufferstats_offset0, kvbufferstats_size0, kvdram, kvdeststats_tmp, baseaddr0_kvs);	
kernelprocess::write_bram2dram_ddr0_w0_sw1(ssdpartiton1id, worker1id, kvbuffer1, DESTtmpkvbuffer_weof, kvbufferstats_offset1, kvbufferstats_size1, kvdram, kvdeststats_tmp, baseaddr1_kvs);	
kernelprocess::write_bram2dram_ddr0_w0_sw2(ssdpartiton2id, worker2id, kvbuffer2, DESTtmpkvbuffer_weof, kvbufferstats_offset2, kvbufferstats_size2, kvdram, kvdeststats_tmp, baseaddr2_kvs);	
kernelprocess::write_bram2dram_ddr0_w0_sw3(ssdpartiton3id, worker3id, kvbuffer3, DESTtmpkvbuffer_weof, kvbufferstats_offset3, kvbufferstats_size3, kvdram, kvdeststats_tmp, baseaddr3_kvs);	
kernelprocess::write_bram2dram_ddr0_w0_sw4(ssdpartiton4id, worker4id, kvbuffer4, DESTtmpkvbuffer_weof, kvbufferstats_offset4, kvbufferstats_size4, kvdram, kvdeststats_tmp, baseaddr4_kvs);	
kernelprocess::write_bram2dram_ddr0_w0_sw5(ssdpartiton5id, worker5id, kvbuffer5, DESTtmpkvbuffer_weof, kvbufferstats_offset5, kvbufferstats_size5, kvdram, kvdeststats_tmp, baseaddr5_kvs);	
kernelprocess::write_bram2dram_ddr0_w0_sw6(ssdpartiton6id, worker6id, kvbuffer6, DESTtmpkvbuffer_weof, kvbufferstats_offset6, kvbufferstats_size6, kvdram, kvdeststats_tmp, baseaddr6_kvs);	
kernelprocess::write_bram2dram_ddr0_w0_sw7(ssdpartiton7id, worker7id, kvbuffer7, DESTtmpkvbuffer_weof, kvbufferstats_offset7, kvbufferstats_size7, kvdram, kvdeststats_tmp, baseaddr7_kvs);	
	return;
}
#endif 

// WRITE TMP 2 DRAM
#ifdef _BUFFERB4WRITE
void kernelprocess::write_tmp2dram_ddr0_w0(unsigned int ssdpartitonid, unsigned int workerid, unsigned int currentLOP, unsigned int destpartitionoffset, 
		uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS], uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smallmetadata_t DESTtmpkvbuffer_weof[NUM_PARTITIONS], unsigned int baseaddr_kvs, unsigned int ibegin_kvs, unsigned int iend_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::write_tmp2dram0. workerid: "<<workerid<<std::endl;
	#endif
	int kvbuffersz_kvs = KVBUFFERSZ_KVS; // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = (KVBUFFERSZ_KVS  / NUM_PARTITIONS) * NUMSUBWORKERS;  // NB: debugging purposes only

	WRITE_TMP2DRAM_ddr0_w0_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int dramoffset_g = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g + dramoffset_l;
		unsigned int desttmpbaseoffset_kvs = DESTtmpkvbuffer_weof[p].offset / VECTOR_SIZE;
		kvbuffersz_dtype size_desttmp_kvs = DESTtmpkvbuffer_weof[p].size / VECTOR_SIZE;	
		// if((size_desttmp_kvs < ((NM - 1) * KVSRCBUFFERSZ_KVS)) && (ibegin_kvs + (NUMWORKERS * NUMSUBWORKERS * KVBUFFERSZ_KVS) < iend_kvs)){ continue; }
		if((size_desttmp_kvs < ((NM - 1) * DESTTMPSLICESZ_KVS)) && (ibegin_kvs + (NUMWORKERS * NUMSUBWORKERS * KVBUFFERSZ_KVS) < iend_kvs)){ continue; }

		unsigned int desttmpoffset_kvs = (p * DESTTMPKVBUFFERSZ_KVS) + desttmpbaseoffset_kvs;
		WRITE_TMP2DRAM_ddr0_w0_LOOP1B: for(kvbuffersz_dtype i=0; i<size_desttmp_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_CHECKS
			checkforoutofbounds_kernel((offset + i), KVSRCBUFFERSZ_KVS, "out-of-bounds seen at write_tmp2dram0 5", 0, 0, 0);
			checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE_KVS, "out-of-bounds seen at write_tmp2dram0 6", 0, 0, 0);
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), kvdeststats_tmp[p+1].offset, "out-of-bounds seen at write_tmp2dram0 7", 0, 0, 0); }
			else { checkforoutofbounds_kernel((dramoffset_g + dramoffset_l + i), PADDEDKVDATA_BATCHSIZE, "out-of-bounds seen at write_tmp2dram0 8", 0, 0, 0); }
			#endif
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_tmp2dram0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = DESTtmpkvbuffer[(desttmpoffset_kvs + i)]; 
			#else 
			kvdram[(dramoffset + i)] = DESTtmpkvbuffer[(desttmpoffset_kvs + i)]; 
			#endif 
		}
		kvdeststats_tmp[p].size += size_desttmp_kvs * VECTOR_SIZE;
		DESTtmpkvbuffer_weof[p].size = 0;
		#ifdef _DEBUGMODE_CHECKS2
		if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvdeststats_tmp[p].offset + kvdeststats_tmp[p].size), kvdeststats_tmp[(p+1)].offset, "out-of-bounds seen at write_tmp2dram_ddr0_w0 65", (kvdeststats_tmp[p].offset + kvdeststats_tmp[p].size), kvdeststats_tmp[(p+1)].offset, 0); }
		#endif	
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_tmp2dram0_transfers += 1;
		#endif
	}
	return;
}
#endif 

#ifdef _BUFFERB4WRITE
void kernelprocess::accel1_write_tmp2dram_ddr0_w0(unsigned int ssdpartitonid, unsigned int workerid
		,uint512_dt * kvdram, metadata_t kvdeststats_tmp[NUM_PARTITIONS]
		,uint512_dt DESTtmpkvbuffer[TOTALDESTTMPKVBUFFERSZ_KVS], smalluint_t DESTtmpkvbufferstats_offset[NUM_PARTITIONS], smalluint_t DESTtmpkvbufferstats_size[NUM_PARTITIONS]
		,unsigned int baseaddr_kvs){
	#ifdef _DEBUGMODE_PRINTS
	std::cout<<"Kernel::accel1_write_tmp2dram_ddr0_w0. workerid: "<<workerid<<std::endl;
	#endif
	int kvbuffersz_kvs = KVBUFFERSZ_KVS; // NB: debugging purposes only
	unsigned int kvbuffersz_perpartition_kvs = (KVBUFFERSZ_KVS  / NUM_PARTITIONS) * NUMSUBWORKERS;  // NB: debugging purposes only

	WRITE_TMP2DRAM_ddr0_w0_LOOP1A: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
		#pragma HLS PIPELINE
		unsigned int dramoffset_g_kvs = kvdeststats_tmp[p].offset / VECTOR_SIZE;
		unsigned int dramoffset_l_kvs = kvdeststats_tmp[p].size / VECTOR_SIZE;
		dramsz_dtype dramoffset = baseaddr_kvs + dramoffset_g_kvs + dramoffset_l_kvs;
		unsigned int desttmpoffset_kvs = DESTtmpkvbufferstats_offset[p] / VECTOR_SIZE;
		kvbuffersz_dtype desttmpsize_kvs = DESTtmpkvbufferstats_size[p] / VECTOR_SIZE;
		#ifdef _DEBUGMODE_CHECKS2
		checkforoutofbounds_kernel(dramoffset, _x3PADDEDKVDATA_SIZE_KVS, "out-of-bounds seen at accel1_write_tmp2dram_ddr0_w0 5", 0, 0, 0);
		checkforoutofbounds_kernel(desttmpoffset_kvs, TOTALDESTTMPKVBUFFERSZ_KVS, "out-of-bounds seen at accel1_write_tmp2dram_ddr0_w0 6", 0, 0, 0);
		checkforoutofbounds_kernel(desttmpsize_kvs, TOTALDESTTMPKVBUFFERSZ_KVS, "out-of-bounds seen at accel1_write_tmp2dram_ddr0_w0 7", 0, 0, 0);
		#endif		
		
		WRITE_TMP2DRAM_ddr0_w0_LOOP1B: for(kvbuffersz_dtype i=0; i<desttmpsize_kvs; i++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=kvbuffersz_perpartition_kvs avg=kvbuffersz_perpartition_kvs
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_STATS
			kvpairswritten_tmp2dram0 += VECTOR_SIZE;
			#endif
			
			#ifdef FORCDFINISH
			kvdram[((dramoffset + i) & (FORCDFINISH_DRAMSZ_KVS - 1))] = DESTtmpkvbuffer[(desttmpoffset_kvs + i)]; 
			#else 
			kvdram[(dramoffset + i)] = DESTtmpkvbuffer[(desttmpoffset_kvs + i)]; 
			#endif
			
			#ifdef _DEBUGMODE_CHECKS2
			if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel(((dramoffset + i) * VECTOR_SIZE), ((baseaddr_kvs * VECTOR_SIZE) + kvdeststats_tmp[p+1].offset), "out-of-bounds seen at accel1_write_tmp2dram_ddr0_w0 85", p, (p+1), 0); }
			#endif
		}
		
		kvdeststats_tmp[p].size += desttmpsize_kvs * VECTOR_SIZE;
		DESTtmpkvbufferstats_size[p] = 0;
		#ifdef _DEBUGMODE_CHECKS2
		// if(p<(NUM_PARTITIONS-1)){ checkforoutofbounds_kernel((kvdeststats_tmp[p].offset + kvdeststats_tmp[p].size), kvdeststats_tmp[(p+1)].offset, "out-of-bounds seen at accel3_write_tmp2dram_ddr0_w0 15", (kvdeststats_tmp[p].offset + kvdeststats_tmp[p].size), kvdeststats_tmp[(p+1)].offset, 0); }
		#endif	
		#ifdef _DEBUGMODE_STATS
		kvpairswritten_tmp2dram0_transfers += 1;
		#endif
	}
	return;
}
#endif

void kernelprocess::topkernel_instancePU0(
		unsigned int command,
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
	
	unsigned int maxtreedepthsz = TREEDEPTH_FORSSDPARTITION0; // NB: debugging purposes only
	unsigned int steps = treedepth + 1;
	// unsigned int steps = 2 + 1; // FIXME. CHANGEBACK

	// Source kv buffer
	#ifdef _SHARED_SRCBUFFER
	#endif

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
	
	// Destination kv buffer
	#ifdef _SHARED_DESTBUFFER
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
	
	// Overflow kv buffer
	#ifdef _SHARED_DESTBUFFER
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
	
	// Mergers
	#ifdef _MERGED_SRCANDDESTANDOVSTMPBUFFER
	#endif
	
	// ALL 
	// Destination tmp kv Buffer
	#ifdef _BUFFERB4WRITE
	uint512_dt DESTtmpkvbuffer_ddrs0_w0[TOTALDESTTMPKVBUFFERSZ_KVS];
	smallmetadata_t DESTtmpkvbuffer_ddrs0_w0_weof[NUM_PARTITIONS];	
	smalluint_t DESTtmpkvbufferstats_ddrs0_w0_offset[NUM_PARTITIONS];
	smalluint_t DESTtmpkvbufferstats_ddrs0_w0_size[NUM_PARTITIONS];	
	#ifdef _USEURAM_DESTTMPKVBUFFER
	#pragma HLS RESOURCE variable=DESTtmpkvbuffer_ddrs0_w0 core=XPM_MEMORY uram 
	#endif 
	#endif

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
		kvpairsread0 = 0;
		kvpairswritten0 = 0;
		kvpairsread0_transfers = 0;
		kvpairswritten0_transfers = 0;
		#ifdef _BUFFERB4WRITE
		kvpairswritten_bram2tmp0 = 0;
		kvpairswritten_tmp2dram0 = 0;
		kvpairswritten_bram2tmp0_transfers = 0;
		kvpairswritten_tmp2dram0_transfers = 0;
		#endif 
		kvpairswritten_interm2dest_parallel0 = 0;
		kvpairswritten_interm2dest_seqential0 = 0;
		INapplyphase = 0;
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
		
		#ifdef _BUFFERB4WRITE
		INITDESTTMPKVBUFFERWEOF_LOOP10: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
			DESTtmpkvbuffer_ddrs0_w0_weof[p].offset = 0;
			DESTtmpkvbuffer_ddrs0_w0_weof[p].size = 0;
			DESTtmpkvbufferstats_ddrs0_w0_offset[p] = 0;
			DESTtmpkvbufferstats_ddrs0_w0_size[p] = 0;
		}
		#endif 

		unsigned int num_source_partitions = kernelprocess::get_num_source_partitions(currentLOP);
		unsigned int nextsourceoffset_partition = 0;
		unsigned int nextdestoffset_partition = 0;
		unsigned int nextsourceoffset_kv = 0;
		unsigned int nextdestoffset_kv = 0;
		unsigned int sourceskipsize_partition = kernelprocess::getsourceskipsize(currentLOP, finalnumpartitions);
		unsigned int destskipsize_partition = kernelprocess::getdestskipsize(currentLOP, finalnumpartitions);
		unsigned int sourceskipsize_kv = kernelprocess::getsourceskipsize(currentLOP, kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid));
		unsigned int destskipsize_kv = kernelprocess::getdestskipsize(currentLOP, kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid));
		unsigned int destpartitionrange = (1 << (kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION_POW(ssdpartitonid) - (NUM_PARTITIONS_POW * currentLOP)));
		#ifdef _DEBUGMODE_PRINTS_TMP
		print4ints_kernel(">>> kernel::topkernel_instancePU0", "currentLOP", currentLOP, "tree depth", treedepth, "num_source_partitions", num_source_partitions, "number of kvs in each final partition", destskipsize_kv);
		print4ints_kernel(">>> kernel::topkernel_instancePU0", "currentLOP", currentLOP, "tree depth", treedepth, "num_source_partitions", num_source_partitions, "number of destination partitions", (kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) / destskipsize_kv));		
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
			print4ints_kernel("kernel::topkernel_instancePU0", "ssdpartitonid", ssdpartitonid, "source_partition", source_partition, "num_source_partitions", num_source_partitions, "tree depth", kernelprocess::GETTREEDEPTH(ssdpartitonid));
			#endif
			
			#ifdef _BUFFERB4WRITE
			INITDESTTMPKVBUFFERWEOF_LOOP20: for(unsigned int p=0; p<NUM_PARTITIONS; p++){
				DESTtmpkvbuffer_ddrs0_w0_weof[p].offset = 0;
				DESTtmpkvbuffer_ddrs0_w0_weof[p].size = 0;
				DESTtmpkvbufferstats_ddrs0_w0_offset[p] = 0;
				DESTtmpkvbufferstats_ddrs0_w0_size[p] = 0;	
			}
			#endif 

kernelprocess::loaddeststats(kvstats, kvdeststats_tmp, nextdestoffset_partition, currentLOP, finalnumpartitions);
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
			#ifdef SW 
			// for(unsigned int k=0; k<sizek; k++){ kvdram[offset_kvs].data[(topk + k)].key=kvdram[offset_kvs].data[0].key; kvdram[offset_kvs].data[(topk + k)].value=0; }
			for(unsigned int k=0; k<sizek; k++){ kvdram[offset_kvs].data[(topk + k)].key=INVALIDDATA; kvdram[offset_kvs].data[(topk + k)].value=INVALIDDATA; }
			#else
			keyvalue_t INVkv; INVkv.key = INVALIDDATA; INVkv.value = INVALIDDATA; 
			for(unsigned int k=0; k<sizek; k++){ setkventryII(kvdram, ((offset_kvs * VECTOR_SIZE) + topk + k), INVkv); }
			#endif 
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
				#ifdef _DEBUGMODE_CHECKS2
				state_i_kvs = i_kvs;
				state_ikvs_sz = ikvs_sz;
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
				
				#ifdef _DEBUGMODE_PRINTS
				std::cout<<"Worker 0 started"<<std::endl;
				for(unsigned int p=0; p<NUM_PARTITIONS; p++){
					printovs2_kernel(ov_w0_sw0, p, "[kernel::READ_PARTITION_WRITE_LOOP]::_w0_sw0", kernelprocess::getkventryII(ov_w0_sw0[p].begin, destkvbuffer_w0_sw0).key, 0, 0);
				}
				for(unsigned int p=0; p<NUM_PARTITIONS; p++){
					printovs2_kernel(ov_w0_sw1, p, "[kernel::READ_PARTITION_WRITE_LOOP]::_w0_sw1", kernelprocess::getkventryII(ov_w0_sw1[p].begin, destkvbuffer_w0_sw1).key, 0, 0);
				}
				for(unsigned int p=0; p<NUM_PARTITIONS; p++){
					printovs2_kernel(ov_w0_sw2, p, "[kernel::READ_PARTITION_WRITE_LOOP]::_w0_sw2", kernelprocess::getkventryII(ov_w0_sw2[p].begin, destkvbuffer_w0_sw2).key, 0, 0);
				}
				for(unsigned int p=0; p<NUM_PARTITIONS; p++){
					printovs2_kernel(ov_w0_sw3, p, "[kernel::READ_PARTITION_WRITE_LOOP]::_w0_sw3", kernelprocess::getkventryII(ov_w0_sw3[p].begin, destkvbuffer_w0_sw3).key, 0, 0);
				}
				for(unsigned int p=0; p<NUM_PARTITIONS; p++){
					printovs2_kernel(ov_w0_sw4, p, "[kernel::READ_PARTITION_WRITE_LOOP]::_w0_sw4", kernelprocess::getkventryII(ov_w0_sw4[p].begin, destkvbuffer_w0_sw4).key, 0, 0);
				}
				for(unsigned int p=0; p<NUM_PARTITIONS; p++){
					printovs2_kernel(ov_w0_sw5, p, "[kernel::READ_PARTITION_WRITE_LOOP]::_w0_sw5", kernelprocess::getkventryII(ov_w0_sw5[p].begin, destkvbuffer_w0_sw5).key, 0, 0);
				}
				for(unsigned int p=0; p<NUM_PARTITIONS; p++){
					printovs2_kernel(ov_w0_sw6, p, "[kernel::READ_PARTITION_WRITE_LOOP]::_w0_sw6", kernelprocess::getkventryII(ov_w0_sw6[p].begin, destkvbuffer_w0_sw6).key, 0, 0);
				}
				for(unsigned int p=0; p<NUM_PARTITIONS; p++){
					printovs2_kernel(ov_w0_sw7, p, "[kernel::READ_PARTITION_WRITE_LOOP]::_w0_sw7", kernelprocess::getkventryII(ov_w0_sw7[p].begin, destkvbuffer_w0_sw7).key, 0, 0);
				}
				#endif
				
				readmany_ddr0_w0(beginworker0id, kvdram, baseaddr0_kvs, kvreadoffset0_kvs, kvsize0_kvs
					,srckvbuffer_ddr0_w0_sw0
					,srckvbuffer_ddr0_w0_sw1
					,srckvbuffer_ddr0_w0_sw2
					,srckvbuffer_ddr0_w0_sw3
					,srckvbuffer_ddr0_w0_sw4
					,srckvbuffer_ddr0_w0_sw5
					,srckvbuffer_ddr0_w0_sw6
					,srckvbuffer_ddr0_w0_sw7
					);
					
				#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
				extractmanystats2_ddr0_w0(ssdpartiton0id, beginworker0id, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0
					,srckvbuffer_ddr0_w0_sw0, destkvbuffer_ddr0_w0_sw0, kvbufferstats_offset_w0_sw0, kvbufferstats_size_w0_sw0, ov_w0_sw0, ovkvs_tmp_ddr0_w0_sw0
					,srckvbuffer_ddr0_w0_sw1, destkvbuffer_ddr0_w0_sw1, kvbufferstats_offset_w0_sw1, kvbufferstats_size_w0_sw1, ov_w0_sw1, ovkvs_tmp_ddr0_w0_sw1
					,srckvbuffer_ddr0_w0_sw2, destkvbuffer_ddr0_w0_sw2, kvbufferstats_offset_w0_sw2, kvbufferstats_size_w0_sw2, ov_w0_sw2, ovkvs_tmp_ddr0_w0_sw2
					,srckvbuffer_ddr0_w0_sw3, destkvbuffer_ddr0_w0_sw3, kvbufferstats_offset_w0_sw3, kvbufferstats_size_w0_sw3, ov_w0_sw3, ovkvs_tmp_ddr0_w0_sw3
					,srckvbuffer_ddr0_w0_sw4, destkvbuffer_ddr0_w0_sw4, kvbufferstats_offset_w0_sw4, kvbufferstats_size_w0_sw4, ov_w0_sw4, ovkvs_tmp_ddr0_w0_sw4
					,srckvbuffer_ddr0_w0_sw5, destkvbuffer_ddr0_w0_sw5, kvbufferstats_offset_w0_sw5, kvbufferstats_size_w0_sw5, ov_w0_sw5, ovkvs_tmp_ddr0_w0_sw5
					,srckvbuffer_ddr0_w0_sw6, destkvbuffer_ddr0_w0_sw6, kvbufferstats_offset_w0_sw6, kvbufferstats_size_w0_sw6, ov_w0_sw6, ovkvs_tmp_ddr0_w0_sw6
					,srckvbuffer_ddr0_w0_sw7, destkvbuffer_ddr0_w0_sw7, kvbufferstats_offset_w0_sw7, kvbufferstats_size_w0_sw7, ov_w0_sw7, ovkvs_tmp_ddr0_w0_sw7
					);
				#endif 
					
				#ifdef _SEPERATE_EXTRACTANDPARTITIONFUNC
				partitionmany2_ddr0_w0(ssdpartiton0id, beginworker0id, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0
					,srckvbuffer_ddr0_w0_sw0, destkvbuffer_ddr0_w0_sw0, kvbufferstats_offset_w0_sw0, kvbufferstats_size_w0_sw0, ov_w0_sw0, ovkvs_tmp_ddr0_w0_sw0
					,srckvbuffer_ddr0_w0_sw1, destkvbuffer_ddr0_w0_sw1, kvbufferstats_offset_w0_sw1, kvbufferstats_size_w0_sw1, ov_w0_sw1, ovkvs_tmp_ddr0_w0_sw1
					,srckvbuffer_ddr0_w0_sw2, destkvbuffer_ddr0_w0_sw2, kvbufferstats_offset_w0_sw2, kvbufferstats_size_w0_sw2, ov_w0_sw2, ovkvs_tmp_ddr0_w0_sw2
					,srckvbuffer_ddr0_w0_sw3, destkvbuffer_ddr0_w0_sw3, kvbufferstats_offset_w0_sw3, kvbufferstats_size_w0_sw3, ov_w0_sw3, ovkvs_tmp_ddr0_w0_sw3
					,srckvbuffer_ddr0_w0_sw4, destkvbuffer_ddr0_w0_sw4, kvbufferstats_offset_w0_sw4, kvbufferstats_size_w0_sw4, ov_w0_sw4, ovkvs_tmp_ddr0_w0_sw4
					,srckvbuffer_ddr0_w0_sw5, destkvbuffer_ddr0_w0_sw5, kvbufferstats_offset_w0_sw5, kvbufferstats_size_w0_sw5, ov_w0_sw5, ovkvs_tmp_ddr0_w0_sw5
					,srckvbuffer_ddr0_w0_sw6, destkvbuffer_ddr0_w0_sw6, kvbufferstats_offset_w0_sw6, kvbufferstats_size_w0_sw6, ov_w0_sw6, ovkvs_tmp_ddr0_w0_sw6
					,srckvbuffer_ddr0_w0_sw7, destkvbuffer_ddr0_w0_sw7, kvbufferstats_offset_w0_sw7, kvbufferstats_size_w0_sw7, ov_w0_sw7, ovkvs_tmp_ddr0_w0_sw7
					);
				#endif  
					
				#ifndef _SEPERATE_EXTRACTANDPARTITIONFUNC
				partitionmany_ddr0_w0(ssdpartiton0id, beginworker0id, kvreadoffset0_kvs, kvsize0_kvs, currentLOP0, destpartitionoffset0, upperlimit0, destpartitionrange0
					,srckvbuffer_ddr0_w0_sw0, destkvbuffer_ddr0_w0_sw0, kvbufferstats_offset_w0_sw0, kvbufferstats_size_w0_sw0, ov_w0_sw0, ovkvs_tmp_ddr0_w0_sw0
					,srckvbuffer_ddr0_w0_sw1, destkvbuffer_ddr0_w0_sw1, kvbufferstats_offset_w0_sw1, kvbufferstats_size_w0_sw1, ov_w0_sw1, ovkvs_tmp_ddr0_w0_sw1
					,srckvbuffer_ddr0_w0_sw2, destkvbuffer_ddr0_w0_sw2, kvbufferstats_offset_w0_sw2, kvbufferstats_size_w0_sw2, ov_w0_sw2, ovkvs_tmp_ddr0_w0_sw2
					,srckvbuffer_ddr0_w0_sw3, destkvbuffer_ddr0_w0_sw3, kvbufferstats_offset_w0_sw3, kvbufferstats_size_w0_sw3, ov_w0_sw3, ovkvs_tmp_ddr0_w0_sw3
					,srckvbuffer_ddr0_w0_sw4, destkvbuffer_ddr0_w0_sw4, kvbufferstats_offset_w0_sw4, kvbufferstats_size_w0_sw4, ov_w0_sw4, ovkvs_tmp_ddr0_w0_sw4
					,srckvbuffer_ddr0_w0_sw5, destkvbuffer_ddr0_w0_sw5, kvbufferstats_offset_w0_sw5, kvbufferstats_size_w0_sw5, ov_w0_sw5, ovkvs_tmp_ddr0_w0_sw5
					,srckvbuffer_ddr0_w0_sw6, destkvbuffer_ddr0_w0_sw6, kvbufferstats_offset_w0_sw6, kvbufferstats_size_w0_sw6, ov_w0_sw6, ovkvs_tmp_ddr0_w0_sw6
					,srckvbuffer_ddr0_w0_sw7, destkvbuffer_ddr0_w0_sw7, kvbufferstats_offset_w0_sw7, kvbufferstats_size_w0_sw7, ov_w0_sw7, ovkvs_tmp_ddr0_w0_sw7
					);
				#endif
				
				#ifdef _BUFFERB4WRITE
				#ifdef _BUFFERB4WRITE_DEPRECIATED
				writemany_bram2tmp_ddr0_w0(ssdpartiton0id, beginworker0id, DESTtmpkvbuffer_ddrs0_w0, DESTtmpkvbuffer_ddrs0_w0_weof, destbaseaddr0_kvs, currentLOP0, destpartitionoffset0
					,destkvbuffer_ddr0_w0_sw0, kvbufferstats_offset_w0_sw0, kvbufferstats_size_w0_sw0
					,destkvbuffer_ddr0_w0_sw1, kvbufferstats_offset_w0_sw1, kvbufferstats_size_w0_sw1
					,destkvbuffer_ddr0_w0_sw2, kvbufferstats_offset_w0_sw2, kvbufferstats_size_w0_sw2
					,destkvbuffer_ddr0_w0_sw3, kvbufferstats_offset_w0_sw3, kvbufferstats_size_w0_sw3
					,destkvbuffer_ddr0_w0_sw4, kvbufferstats_offset_w0_sw4, kvbufferstats_size_w0_sw4
					,destkvbuffer_ddr0_w0_sw5, kvbufferstats_offset_w0_sw5, kvbufferstats_size_w0_sw5
					,destkvbuffer_ddr0_w0_sw6, kvbufferstats_offset_w0_sw6, kvbufferstats_size_w0_sw6
					,destkvbuffer_ddr0_w0_sw7, kvbufferstats_offset_w0_sw7, kvbufferstats_size_w0_sw7
					,ibegin0_kvs, iend0_kvs
					);
				#else 
				accel1_writemany_bram2tmp_ddr0_w0(ssdpartiton0id, beginworker0id
					,DESTtmpkvbuffer_ddrs0_w0, DESTtmpkvbufferstats_ddrs0_w0_offset, DESTtmpkvbufferstats_ddrs0_w0_size
					,destkvbuffer_ddr0_w0_sw0, kvbufferstats_offset_w0_sw0, kvbufferstats_size_w0_sw0
					,destkvbuffer_ddr0_w0_sw1, kvbufferstats_offset_w0_sw1, kvbufferstats_size_w0_sw1
					,destkvbuffer_ddr0_w0_sw2, kvbufferstats_offset_w0_sw2, kvbufferstats_size_w0_sw2
					,destkvbuffer_ddr0_w0_sw3, kvbufferstats_offset_w0_sw3, kvbufferstats_size_w0_sw3
					,destkvbuffer_ddr0_w0_sw4, kvbufferstats_offset_w0_sw4, kvbufferstats_size_w0_sw4
					,destkvbuffer_ddr0_w0_sw5, kvbufferstats_offset_w0_sw5, kvbufferstats_size_w0_sw5
					,destkvbuffer_ddr0_w0_sw6, kvbufferstats_offset_w0_sw6, kvbufferstats_size_w0_sw6
					,destkvbuffer_ddr0_w0_sw7, kvbufferstats_offset_w0_sw7, kvbufferstats_size_w0_sw7
					,NAp
					);
				#endif
				#endif 
				
				#ifdef _BUFFERB4WRITE
				#ifdef _BUFFERB4WRITE_DEPRECIATED
				write_tmp2dram_ddr0_w0(ssdpartiton0id, beginworker0id, currentLOP0, destpartitionoffset0, kvdram, kvdeststats_tmp, DESTtmpkvbuffer_ddrs0_w0, DESTtmpkvbuffer_ddrs0_w0_weof, destbaseaddr0_kvs, ibegin0_kvs, iend0_kvs);
				#else 
				accel1_write_tmp2dram_ddr0_w0(ssdpartiton0id, beginworker0id
					,kvdram, kvdeststats_tmp, DESTtmpkvbuffer_ddrs0_w0, DESTtmpkvbufferstats_ddrs0_w0_offset, DESTtmpkvbufferstats_ddrs0_w0_size, destbaseaddr0_kvs);
				#endif
				#endif
					
				#ifndef _BUFFERB4WRITE
				writemany_ddr0_w0(ssdpartiton0id, beginworker0id, kvdram, kvdeststats_tmp, destbaseaddr0_kvs, currentLOP0, destpartitionoffset0
					,destkvbuffer_ddr0_w0_sw0, kvbufferstats_offset_w0_sw0, kvbufferstats_size_w0_sw0
					,destkvbuffer_ddr0_w0_sw1, kvbufferstats_offset_w0_sw1, kvbufferstats_size_w0_sw1
					,destkvbuffer_ddr0_w0_sw2, kvbufferstats_offset_w0_sw2, kvbufferstats_size_w0_sw2
					,destkvbuffer_ddr0_w0_sw3, kvbufferstats_offset_w0_sw3, kvbufferstats_size_w0_sw3
					,destkvbuffer_ddr0_w0_sw4, kvbufferstats_offset_w0_sw4, kvbufferstats_size_w0_sw4
					,destkvbuffer_ddr0_w0_sw5, kvbufferstats_offset_w0_sw5, kvbufferstats_size_w0_sw5
					,destkvbuffer_ddr0_w0_sw6, kvbufferstats_offset_w0_sw6, kvbufferstats_size_w0_sw6
					,destkvbuffer_ddr0_w0_sw7, kvbufferstats_offset_w0_sw7, kvbufferstats_size_w0_sw7
					);
				#endif
			}
			
			// edge conditions
			// kvdeststats_tmp[0].size -= sizek;
			writeremainingovs_ddr0_w0_sw0(ssdpartitonid, (0 * NUMSUBWORKERS), kvdram, kvdeststats_tmp, ovkvs_tmp_ddr0_w0_sw0, ov_w0_sw0, destbaseaddr_kvs);
			writeremainingovs_ddr0_w0_sw1(ssdpartitonid, (0 * NUMSUBWORKERS), kvdram, kvdeststats_tmp, ovkvs_tmp_ddr0_w0_sw1, ov_w0_sw1, destbaseaddr_kvs);
			writeremainingovs_ddr0_w0_sw2(ssdpartitonid, (0 * NUMSUBWORKERS), kvdram, kvdeststats_tmp, ovkvs_tmp_ddr0_w0_sw2, ov_w0_sw2, destbaseaddr_kvs);
			writeremainingovs_ddr0_w0_sw3(ssdpartitonid, (0 * NUMSUBWORKERS), kvdram, kvdeststats_tmp, ovkvs_tmp_ddr0_w0_sw3, ov_w0_sw3, destbaseaddr_kvs);
			writeremainingovs_ddr0_w0_sw4(ssdpartitonid, (0 * NUMSUBWORKERS), kvdram, kvdeststats_tmp, ovkvs_tmp_ddr0_w0_sw4, ov_w0_sw4, destbaseaddr_kvs);
			writeremainingovs_ddr0_w0_sw5(ssdpartitonid, (0 * NUMSUBWORKERS), kvdram, kvdeststats_tmp, ovkvs_tmp_ddr0_w0_sw5, ov_w0_sw5, destbaseaddr_kvs);
			writeremainingovs_ddr0_w0_sw6(ssdpartitonid, (0 * NUMSUBWORKERS), kvdram, kvdeststats_tmp, ovkvs_tmp_ddr0_w0_sw6, ov_w0_sw6, destbaseaddr_kvs);
			writeremainingovs_ddr0_w0_sw7(ssdpartitonid, (0 * NUMSUBWORKERS), kvdram, kvdeststats_tmp, ovkvs_tmp_ddr0_w0_sw7, ov_w0_sw7, destbaseaddr_kvs);
			kvdeststats_tmp[0].size -= sizek;
			// if(currentLOP == 1){ cout<<"------------------------------------------------------------------------ kvdeststats_tmp[0].size: "<<kvdeststats_tmp[0].size<<endl; }
			
			#ifdef _DEBUGMODE_CHECKS
			check_kvdram_kernel(kvdram, destbaseaddr_kvs, kvdeststats_tmp, nextdestoffset_partition, currentLOP, kernelprocess::GETTREEDEPTH(ssdpartitonid), finalnumpartitions, kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid));
			#endif
			#ifdef _DEBUGMODE_STATSX
			std::cout<<"### source partition ended: kvpairsread0: "<<kvpairsread0<<", kvpairswritten0: "<<kvpairswritten0<<" ###"<<std::endl;
			#endif
kernelprocess::storedeststats(kvstats, kvdeststats_tmp, nextdestoffset_partition, currentLOP, finalnumpartitions);
			nextsourceoffset_partition += sourceskipsize_partition;
			nextdestoffset_partition += (destskipsize_partition * NUM_PARTITIONS);
			nextsourceoffset_kv += sourceskipsize_kv;
			nextdestoffset_kv += destskipsize_kv;
		}
		
		#ifdef _DEBUGMODE_STATS
		std::cout<<">>>"<<endl;
		std::cout<<"### topkernel_instancePU::Summary:: KV PAIRS READ (DRAM A): "<<kvpairsread0<<" ("<<kvpairsread0_transfers<<" TRANSFERS)"<<endl;		
		std::cout<<"### topkernel_instancePU::Summary:: KV PAIRS WRITTEN (DRAM A): "<<kvpairswritten0<<" ("<<kvpairswritten0_transfers<<" TRANSFERS)"<<endl;
		#ifdef _BUFFERB4WRITE
		std::cout<<">>>"<<endl;
		std::cout<<"### topkernel_instancePU::Summary:: KV PAIRS WRITTEN FROM BRAM TO TMP (DRAM A): "<<kvpairswritten_bram2tmp0<<" ("<<kvpairswritten_bram2tmp0_transfers<<" TRANSFERS)"<<endl;	
		std::cout<<"### topkernel_instancePU::Summary:: KV PAIRS WRITTEN FROM TMP TO DRAM (DRAM A): "<<kvpairswritten_tmp2dram0<<" ("<<kvpairswritten_tmp2dram0_transfers<<" TRANSFERS)"<<endl;
		std::cout<<"### topkernel_instancePU::Summary:: TOTAL KV PAIRS WRITTEN TO DRAM (DRAM A): "<<(kvpairswritten0 + kvpairswritten_tmp2dram0)<<" ("<<(kvpairswritten0 + kvpairswritten_tmp2dram0)<<" TRANSFERS)"<<endl;
		#endif	
		std::cout<<std::endl;
		#endif
	}
	return;
}

void kernelprocess::topkernel_instanceAU0(
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
	kvpairsread0 = 0;
	kvpairswritten0 = 0;
	INapplyphase = 1;
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
	
	unsigned int num_source_partitions = kernelprocess::get_num_source_partitions(currentLOP);
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
		std::cout<<"kernel::topkernel_instanceAU0:: next read offset: "<<(source_partition * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION(ssdpartitonid))<<endl;
		#endif
		/** [NUMSUBWORKERS_APPLYPH] consecutive locations are loaded into each verticesbuffer */
		readmanyAU_Vs_ddr0_w0(ssdpartitonid, (0 * NUMSUBWORKERS_APPLYPH), kvdram, destbaseaddr_vs, (source_partition * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid)), kernelprocess::GETKVDATA_RANGE_PERSSDPARTITION(ssdpartitonid) /// FIXME KVDATA_BATCHSIZE?
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
			unsigned int verticesreadoffset0_vs = ((vbegin / VECTOR_SIZE) + (source_partition * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))); 
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
		std::cout<<"kernel::topkernel_instanceAU0:: next write offset: "<<(source_partition * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))<<endl;
		#endif
		writemanyAU_Vs_ddr0_w0(ssdpartitonid, (0 * NUMSUBWORKERS_APPLYPH), kvdram, destbaseaddr_vs, (source_partition * kernelprocess::GETAPPLYVERTEXBUFFERSZ_PERSSDPARTITION_KVS(ssdpartitonid))
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
	std::cout<<"### topkernel_instanceAU ended::Summary:: kvpairsread0: "<<kvpairsread0<<", kvpairswritten0: "<<kvpairswritten0<<" ########"<<std::endl<<std::endl;
	#endif
	return;
}

extern "C" {
void kernelprocess::topkernel(
uint512_dt * kvdramA
		,metadata_t * kvstatsA
		,int * messagesA
        ){
#pragma HLS INTERFACE m_axi port=kvdramA  offset=slave bundle=gmem0
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
	
	#ifdef _SHARED_SRCBUFFER
	uint512_dt srckvbuffer_ddr0_w0_sw0[KVSRCBUFFERSZ_KVS];
	uint512_dt srckvbuffer_ddr0_w0_sw1[KVSRCBUFFERSZ_KVS];
	uint512_dt srckvbuffer_ddr0_w0_sw2[KVSRCBUFFERSZ_KVS];
	uint512_dt srckvbuffer_ddr0_w0_sw3[KVSRCBUFFERSZ_KVS];
	uint512_dt srckvbuffer_ddr0_w0_sw4[KVSRCBUFFERSZ_KVS];
	uint512_dt srckvbuffer_ddr0_w0_sw5[KVSRCBUFFERSZ_KVS];
	uint512_dt srckvbuffer_ddr0_w0_sw6[KVSRCBUFFERSZ_KVS];
	uint512_dt srckvbuffer_ddr0_w0_sw7[KVSRCBUFFERSZ_KVS];
	#endif
	
	#ifdef _SHARED_DESTBUFFER
	uint512_dt destkvbuffer_ddr0_w0_sw0[KVDESTBUFFERSZ_KVS];
	uint512_dt destkvbuffer_ddr0_w0_sw1[KVDESTBUFFERSZ_KVS];
	uint512_dt destkvbuffer_ddr0_w0_sw2[KVDESTBUFFERSZ_KVS];
	uint512_dt destkvbuffer_ddr0_w0_sw3[KVDESTBUFFERSZ_KVS];
	uint512_dt destkvbuffer_ddr0_w0_sw4[KVDESTBUFFERSZ_KVS];
	uint512_dt destkvbuffer_ddr0_w0_sw5[KVDESTBUFFERSZ_KVS];
	uint512_dt destkvbuffer_ddr0_w0_sw6[KVDESTBUFFERSZ_KVS];
	uint512_dt destkvbuffer_ddr0_w0_sw7[KVDESTBUFFERSZ_KVS];
	#endif
	
	uint512_dt ovkvs_tmp_ddr0_w0_sw0[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw1[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw2[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw3[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw4[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw5[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw6[NUM_PARTITIONS];
	uint512_dt ovkvs_tmp_ddr0_w0_sw7[NUM_PARTITIONS];
	
	
	#ifdef _SHARED_SRCBUFFER
	#ifdef _SHARED_DESTBUFFER
	#ifdef _MERGED_SRCANDDESTANDOVSTMPBUFFER
	#pragma HLS ARRAY_MAP variable=srckvbuffer_ddr0_w0_sw0 instance=array4_ddr0_w0_sw0 horizontal
	#pragma HLS ARRAY_MAP variable=destkvbuffer_ddr0_w0_sw0 instance=array4_ddr0_w0_sw0 horizontal
	#pragma HLS ARRAY_MAP variable=ovkvs_tmp_ddr0_w0_sw0 instance=array4_ddr0_w0_sw0 horizontal
	#pragma HLS ARRAY_MAP variable=srckvbuffer_ddr0_w0_sw1 instance=array4_ddr0_w0_sw1 horizontal
	#pragma HLS ARRAY_MAP variable=destkvbuffer_ddr0_w0_sw1 instance=array4_ddr0_w0_sw1 horizontal
	#pragma HLS ARRAY_MAP variable=ovkvs_tmp_ddr0_w0_sw1 instance=array4_ddr0_w0_sw1 horizontal
	#pragma HLS ARRAY_MAP variable=srckvbuffer_ddr0_w0_sw2 instance=array4_ddr0_w0_sw2 horizontal
	#pragma HLS ARRAY_MAP variable=destkvbuffer_ddr0_w0_sw2 instance=array4_ddr0_w0_sw2 horizontal
	#pragma HLS ARRAY_MAP variable=ovkvs_tmp_ddr0_w0_sw2 instance=array4_ddr0_w0_sw2 horizontal
	#pragma HLS ARRAY_MAP variable=srckvbuffer_ddr0_w0_sw3 instance=array4_ddr0_w0_sw3 horizontal
	#pragma HLS ARRAY_MAP variable=destkvbuffer_ddr0_w0_sw3 instance=array4_ddr0_w0_sw3 horizontal
	#pragma HLS ARRAY_MAP variable=ovkvs_tmp_ddr0_w0_sw3 instance=array4_ddr0_w0_sw3 horizontal
	#pragma HLS ARRAY_MAP variable=srckvbuffer_ddr0_w0_sw4 instance=array4_ddr0_w0_sw4 horizontal
	#pragma HLS ARRAY_MAP variable=destkvbuffer_ddr0_w0_sw4 instance=array4_ddr0_w0_sw4 horizontal
	#pragma HLS ARRAY_MAP variable=ovkvs_tmp_ddr0_w0_sw4 instance=array4_ddr0_w0_sw4 horizontal
	#pragma HLS ARRAY_MAP variable=srckvbuffer_ddr0_w0_sw5 instance=array4_ddr0_w0_sw5 horizontal
	#pragma HLS ARRAY_MAP variable=destkvbuffer_ddr0_w0_sw5 instance=array4_ddr0_w0_sw5 horizontal
	#pragma HLS ARRAY_MAP variable=ovkvs_tmp_ddr0_w0_sw5 instance=array4_ddr0_w0_sw5 horizontal
	#pragma HLS ARRAY_MAP variable=srckvbuffer_ddr0_w0_sw6 instance=array4_ddr0_w0_sw6 horizontal
	#pragma HLS ARRAY_MAP variable=destkvbuffer_ddr0_w0_sw6 instance=array4_ddr0_w0_sw6 horizontal
	#pragma HLS ARRAY_MAP variable=ovkvs_tmp_ddr0_w0_sw6 instance=array4_ddr0_w0_sw6 horizontal
	#pragma HLS ARRAY_MAP variable=srckvbuffer_ddr0_w0_sw7 instance=array4_ddr0_w0_sw7 horizontal
	#pragma HLS ARRAY_MAP variable=destkvbuffer_ddr0_w0_sw7 instance=array4_ddr0_w0_sw7 horizontal
	#pragma HLS ARRAY_MAP variable=ovkvs_tmp_ddr0_w0_sw7 instance=array4_ddr0_w0_sw7 horizontal
	#endif
	#endif
	#endif
	
	
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
		);
	// return;
		
	topkernel_instanceAU0(
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
		);
	// return;
	
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

kernelprocess::kernelprocess(){}
kernelprocess::~kernelprocess(){}
