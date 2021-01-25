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
#include "../src/stats/stats.h"
#include "../src/utility/utility.h"
#include "../acts/include/actscommon.h" //
#include "../include/common.h"
#include "kernel.h"
using namespace std;

kernel::kernel(stats * _statsobj){
	utilityobj = new utility();
	#ifdef FPGA_IMPL
	kernelobj = new goclkernel(_statsobj);
	#else 
	kernelobj = new swkernel(_statsobj);
	#endif
	statsobj = _statsobj;
}
kernel::~kernel(){} 

void kernel::launchkernel(uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int flag){			
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printstructuresbeforekernelrun("kernel::launchkernel", (uint512_vec_dt **)kvsourcedram, NUMSUBCPUTHREADS);
	#endif
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		utilityobj->paddkeyvalues((keyvalue_t *)&kvsourcedram[i][BASEOFFSET_KVDRAM_KVS], kvsourcedram[i][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key, INVALIDDATA);						
	}
	
	#ifdef FPGA_IMPL
	writetokernel(flag, vdram, kvsourcedram);
	#endif
	kernelobj->launchkernel(vdram, kvsourcedram, flag);
	#ifdef FPGA_IMPL
	readfromkernel(flag, vdram, kvsourcedram);
	#endif
	
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printstructuresafterkernelrun("kernel::launchkernel", (uint512_vec_dt **)kvsourcedram, NUMSUBCPUTHREADS);
	#endif
	return;
}

#ifdef FPGA_IMPL 
void kernel::loadOCLstructures(std::string binaryFile, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	kernelobj->loadOCLstructures(binaryFile, vdram, kvsourcedram);
}

void kernel::writetokernel(unsigned int flag, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		beginoffset[i] = 0;
		size[i] = PADDEDKVSOURCEDRAMSZ;
	}
	kernelobj->writetokernel(flag, vdram, kvsourcedram, beginoffset, beginoffset, size);
	return;
}

void kernel::readfromkernel(unsigned int flag, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		beginoffset[i] = 0;
		size[i] = PADDEDKVSOURCEDRAMSZ;
	}
	kernelobj->readfromkernel(flag, vdram, kvsourcedram, beginoffset, beginoffset, size);
}

void kernel::finishOCL(){
	kernelobj->finishOCL();
}
#endif











