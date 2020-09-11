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
	#ifdef ACTSMODEL_LW
	updatemessagesbeforelaunch((uint512_vec_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram, kvstats);
	#endif
	kernelobj->launchkernel(kvsourcedram, kvdestdram, kvstats, flag);
	return;
}

#ifdef ACTSMODEL_LW
void kernel::updatemessagesbeforelaunch(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	cout<<"kernel::updatemessagesbeforelaunch:: updating messages before launch..."<<endl;
	for(int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_RUNKERNELCOMMANDID];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_BATCHSIZE];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_RUNSIZE]; 
			kvsourcedram[i][j][BASEOFFSET_STATSDRAM_KVS + 0].data[0] = kvstats[i][j][HOSTBASEOFFSET_STATSDRAM + 0]; 
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_PROCESSCOMMANDID];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_COLLECTSTATSCOMMANDID];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_PARTITIONCOMMANDID];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_APPLYUPDATESCOMMANDID];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_VOFFSET];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_VSIZE];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_TREEDEPTH];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_FINALNUMPARTITIONS];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_GRAPHITERATIONID];
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_GRAPHALGORITHMID];
			
			kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0] = kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_NEXTBATCHOFFSET]; 
		}
	}
	cout<<"kernel::updatemessagesbeforelaunch:: updating messages before launch finished."<<endl;
	return;
}
#endif 

#ifdef FPGA_IMPL 
void kernel::loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	kernelobj->loadOCLstructures(binaryFile, kvsourcedram, kvdestdram, kvstats);
}
void kernel::writeVstokernel(unsigned int flag){
	kernelobj->writeVstokernel(flag);
}
void kernel::readVsfromkernel(unsigned int flag){
	kernelobj->readVsfromkernel(flag);
}
void kernel::finishOCL(){
	kernelobj->finishOCL();
}
#endif 











