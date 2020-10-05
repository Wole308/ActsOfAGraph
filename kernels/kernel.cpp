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
	// kernelobj = new oclkernel();
	kernelobj = new goclkernel();
	#else 
	kernelobj = new swkernel();
	#endif
}
kernel::~kernel(){} 

void kernel::launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){			
	#ifdef FPGA_IMPL
	writetokernel(flag, kvsourcedram, beginoffset, size);
	#endif 
	
	kernelobj->launchkernel(kvsourcedram, kvdestdram, kvstats, flag);
	
	#ifdef FPGA_IMPL
	readfromkernel(flag, kvsourcedram, beginoffset, size);
	#endif 
	return;
}

#ifdef FPGA_IMPL 
void kernel::loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	kernelobj->loadOCLstructures(binaryFile, kvsourcedram, kvdestdram, kvstats);
}

void kernel::writetokernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset, unsigned int size){
	kernelobj->writetokernel(flag, kvsourcedram, beginoffset, size);
}
void kernel::writetokernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printstructuresbeforekernelrun("helperfunctions::writetokernel", (uint512_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram);
	#endif
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){
			beginoffset[i][j] = 0;
			size[i][j] = MESSAGESDRAMSZ + KVDRAMBUFFERSZ + kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
		}
	}
	kernelobj->writetokernel(flag, kvsourcedram, beginoffset, size);
}

void kernel::readfromkernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset, unsigned int size){
	kernelobj->readfromkernel(flag, kvsourcedram, beginoffset, size);
}
void kernel::readfromkernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){
			beginoffset[i][j] = 0;
			size[i][j] = MESSAGESDRAMSZ + KVDRAMBUFFERSZ + kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key; // PADDEDKVSOURCEDRAMSZ;//INPUTDATASZ; // PADDEDKVSOURCEDRAMSZ;
		}
	}
	kernelobj->readfromkernel(flag, kvsourcedram, beginoffset, size);
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printstructuresafterkernelrun("helperfunctions::readfromkernel", (uint512_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram);
	#endif
}

void kernel::finishOCL(){
	kernelobj->finishOCL();
}
#endif 











