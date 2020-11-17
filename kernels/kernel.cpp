#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "swkernel.h"
// #include "oclkernel.h"
#include "goclkernel.h"
#include "../src/utility/utility.h"
#include "../acts/include/actscommon.h" //
#include "../include/common.h"
#include "kernel.h"
using namespace std;

kernel::kernel(){
	utilityobj = new utility();
	#ifdef FPGA_IMPL
	kernelobj = new goclkernel();
	#else 
	kernelobj = new swkernel();
	#endif
}
kernel::~kernel(){} 

void kernel::launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){			
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printstructuresbeforekernelrun("kernel::launchkernel", (uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram, 1);
	#endif
	
	#ifdef FPGA_IMPL
	writetokernel(flag, kvsourcedram);
	#endif
	for (int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			utilityobj->paddkeyvalues((keyvalue_t *)&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS], kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key, INVALIDDATA);
		}
	}
	kernelobj->launchkernel(kvsourcedram, flag);
	#if (defined(FPGA_IMPL) && defined(_DEBUGMODE_HOSTPRINTS2))
	readfromkernel(flag, kvsourcedram);
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printstructuresafterkernelrun("kernel::launchkernel", (uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram, 1);
	#endif
	return;
}

#ifdef FPGA_IMPL 
void kernel::loadOCLstructures(std::string binaryFile, uint512_vec_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	kernelobj->loadOCLstructures(binaryFile, kvsourcedram);
}

void kernel::writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	kernelobj->writetokernel(flag, kvsourcedram, hostbeginoffset, beginoffset, size);
}
void kernel::writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	#ifdef ACCESSFPGABY_ENQUEUEWRITEBUFFER
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){
			beginoffset[i][j] = BASEOFFSET_KVDRAM + kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key;
			size[i][j] = kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key;
		}
	}
	kernelobj->writetokernel(flag, kvsourcedram, BASEOFFSET_MESSAGESDRAM, BASEOFFSET_MESSAGESDRAM, MESSAGESDRAMSZ); // messages
	kernelobj->writetokernel(flag, kvsourcedram, beginoffset, beginoffset, size);
	#else
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){
			beginoffset[i][j] = 0;
			size[i][j] = PADDEDKVSOURCEDRAMSZ;
		}
	}
	kernelobj->writetokernel(flag, kvsourcedram, beginoffset, beginoffset, size);
	#endif 
	return;
}

void kernel::readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	kernelobj->readfromkernel(flag, kvsourcedram, hostbeginoffset, beginoffset, size);
}
void kernel::readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){
			beginoffset[i][j] = 0;
			size[i][j] = PADDEDKVSOURCEDRAMSZ;
		}
	}
	kernelobj->readfromkernel(flag, kvsourcedram, beginoffset, beginoffset, size);
}

void kernel::finishOCL(){
	kernelobj->finishOCL();
}
#endif











