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
	kernelobj->launchkernel(kvsourcedram, kvdestdram, kvstats, flag);
	return;
}

#ifdef FPGA_IMPL 
void kernel::loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	kernelobj->loadOCLstructures(binaryFile, kvsourcedram, kvdestdram, kvstats);
}
void kernel::writeVstokernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset, unsigned int size){
	kernelobj->writeVstokernel(flag, kvsourcedram, beginoffset, size);
}
void kernel::readVsfromkernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset, unsigned int size){
	kernelobj->readVsfromkernel(flag, kvsourcedram, beginoffset, size);
}
void kernel::finishOCL(){
	kernelobj->finishOCL();
}
#endif 











