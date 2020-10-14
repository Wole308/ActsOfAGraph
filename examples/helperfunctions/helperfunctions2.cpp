#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../kernels/kernel.h"
#include "../../include/common.h"
#include "helperfunctions2.h"
using namespace std;

helperfunctions2::helperfunctions2(graph * _graphobj, stats * _statsobj){
	parametersobj = new parameters();
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	kernelobj = new kernel();
	// statsobj = new stats(graphobj);
	statsobj = _statsobj;
}
helperfunctions2::helperfunctions2(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
	kernelobj = new kernel();
}
helperfunctions2::~helperfunctions2(){} 

// create messages
void helperfunctions2::createmessages(
			uint512_vec_dt * kvstats,
			unsigned int voffset,
			unsigned int vsize,
			unsigned int treedepth,
			unsigned int GraphIter,
			unsigned int GraphAlgo,
			unsigned int runsize,
			unsigned int batch_range,
			unsigned int batch_range_kvs,
			unsigned int batch_range_pow,
			unsigned int applyvertexbuffersz,
			unsigned int applyvertexbuffersz_kvs,
			unsigned int numlastlevelpartitions){
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].data[0].key = voffset;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].data[0].key = vsize;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key = treedepth;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key = GraphIter;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0].key = GraphAlgo;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key = 0; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key = runsize; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key = runsize; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 1;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
	
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0; // REMOVEME
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1;//treedepth + 2;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
	
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE].data[0].key = batch_range; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_KVS].data[0].key = batch_range_kvs;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_POW].data[0].key = batch_range_pow;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ].data[0].key = applyvertexbuffersz; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].data[0].key = applyvertexbuffersz_kvs;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key = numlastlevelpartitions;
	return;
}

// launch kernel
void helperfunctions2::launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions2::launchkernel:: Print kvdram (before Kernel launch)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printmessages("helperfunctions2::launchkernel:: messages (before kernel launch)", (uint512_vec_dt *)(&kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS])); }}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions2::launchkernel:: Print vertex datas (before Kernel launch)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_VERTICESDATA_KVS]), 16); }}
	#endif
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	kernelobj->launchkernel(kvsourcedram, flag);
	
	#ifdef _DEBUGMODE_TIMERS
	long double kerneltimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(kerneltimeelapsed_ms);
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions2::launchkernel:: Print results (after Kernel run)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	for(unsigned int value=0; value<24; value++){ for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->countkeyvalueswithvalueequalto("helperfunctions2::launchkernel", (keyvalue_t *)kvdestdram[i][j], KVDATA_RANGE_PERSSDPARTITION, value); }}}		
	#endif
	return;
}

unsigned int helperfunctions2::getflag(unsigned int globaliteration_idx){
	#ifdef FPGA_IMPL
	int flag = globaliteration_idx % NUMFLAGS;
	#else 
	int flag = 0;
	#endif 
	return flag;
}

#ifdef FPGA_IMPL 
void helperfunctions2::loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	kernelobj->loadOCLstructures(binaryFile, kvsourcedram);
}
void helperfunctions2::writetokernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	kernelobj->writetokernel(flag, kvsourcedram, hostbeginoffset, beginoffset, size);
}
void helperfunctions2::readfromkernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	kernelobj->readfromkernel(flag, kvsourcedram, hostbeginoffset, beginoffset, size);
}
void helperfunctions2::finishOCL(){
	kernelobj->finishOCL();
}
#endif 








