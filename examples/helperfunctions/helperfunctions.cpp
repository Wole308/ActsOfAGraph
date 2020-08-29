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
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../kernels/kernel.h"
#include "../../include/common.h"
#include "helperfunctions.h"
using namespace std;

helperfunctions::helperfunctions(graph * _graphobj){
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	kernelobj = new kernel();
}
helperfunctions::helperfunctions(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
	kernelobj = new kernel();
}
helperfunctions::~helperfunctions(){} 

void helperfunctions::replicateverticesdata(keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_replicateverticesdata(i, buffer, offset + (i * (size / NUMUTILITYTHREADS)), (size / NUMUTILITYTHREADS)); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i] = std::thread(&helperfunctions::workerthread_replicateverticesdata, this, i, buffer, offset + (i * (size / NUMUTILITYTHREADS)), (size / NUMUTILITYTHREADS)); }
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i].join(); }
	#endif 
	return;
}
void helperfunctions::workerthread_replicateverticesdata(int threadidx, keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			memcpy(&buffer[i][j][offset], &buffer[0][0][offset], (size * sizeof(keyvalue_t))); 
		}
	}
	return;
}

void helperfunctions::cummulateverticesdata(keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ workerthread_cummulateverticesdata(i, buffer, offset + (i * (size / NUMUTILITYTHREADS)), (size / NUMUTILITYTHREADS)); }}
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ mykernelthread[i][j] = std::thread(&helperfunctions::workerthread_cummulateverticesdata, this, i, buffer, offset + (i * (size / NUMUTILITYTHREADS)), (size / NUMUTILITYTHREADS)); }}
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ mykernelthread[i][j].join(); }}
	#endif 
	return;
}
void helperfunctions::workerthread_cummulateverticesdata(int threadidx, keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	unsigned int min;
	for(unsigned int k=offset; k<(offset + size); k++){
		min = buffer[0][0][k].value;
		for(unsigned int i=1; i<NUMCPUTHREADS; i++){
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				min = utilityobj->hmin(min, buffer[i][j][k].value);
			}
		}
		buffer[0][0][k].value = min;
	}
	return;
}

void helperfunctions::applyvertices(unsigned int bank, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, unsigned int graph_iterationidx){
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_applyvertices(i, bank, graphobj->getvertexdatabuffer(bank), graphobj->getvertexisactivebuffer(bank), buffer, bufferoffset + (i * (datasize / NUMUTILITYTHREADS)), (datasize / NUMUTILITYTHREADS), voffset, graph_iterationidx); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i] = std::thread(&helperfunctions::workerthread_applyvertices, this, i, bank, graphobj->getvertexdatabuffer(bank), graphobj->getvertexisactivebuffer(bank), buffer, bufferoffset + (i * (datasize / NUMUTILITYTHREADS)), (datasize / NUMUTILITYTHREADS), voffset, graph_iterationidx); }		
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i].join(); }
	#endif
	return;
}
void helperfunctions::workerthread_applyvertices(int ithreadidx, unsigned int bank, value_t * vertexdatabuffer, unsigned int * vertexisactivebuffer, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize, unsigned int voffset, unsigned int graph_iterationidx){
	graphobj->loadvertexdatafromfile(bank, 0, vertexdatabuffer, 0, KVDATA_RANGE_PERSSDPARTITION);
	
	vector<keyvalue_t> activeverticesbuffer;
	for(unsigned int i=bufferoffset; i<bufferoffset + datasize; i++){
		keyvalue_t kvtempdata = buffer[i];
		value_t vdata = vertexdatabuffer[i];
		value_t temp = algorithmobj->apply(kvtempdata.value, vdata);
		vertexdatabuffer[i] = temp;
		#ifndef PR_ALGORITHM // FIXME. use algorithmobj instead
		if(temp != vdata){
			keyvalue_t kv;
			kv.key = voffset + i; 
			kv.value = temp;
			activeverticesbuffer.push_back(kv);
			utilityobj->InsertBit(vertexisactivebuffer, i, 1);
		}
		#endif 
	}
	
	graphobj->saveactiveverticestofile(activeverticesbuffer, (graph_iterationidx + 1));
	activeverticesbuffer.clear();
	return;
}

void helperfunctions::launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions::launchkernel:: Print results before Kernel run", (keyvalue_t *)kvsourcedram[i][j], 16); }}
	#endif
	
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->allignandappendinvalids((keyvalue_t *)kvsourcedram[i][j], kvstats[i][j][BASEOFFSET_STATSDRAM + 0].value); }} // edge conditions
	kernelobj->launchkernel(kvsourcedram, kvdestdram, kvstats, flag);
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions::launchkernel:: Print results after Kernel run", (keyvalue_t *)kvsourcedram[i][j], 16); }}
	for(unsigned int value=0; value<24; value++){ for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->countkeyvalueswithvalueequalto("helperfunctions::launchkernel", (keyvalue_t *)kvdestdram[i][j], KVDATA_RANGE_PERSSDPARTITION, value); }}}			
	#endif
	return;
}

void helperfunctions::updatemessagesbeforelaunch(unsigned int globaliteration_idx, unsigned int graph_iterationidx, unsigned int voffset, unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int keyvaluecount[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			if((utilityobj->runActs(globaliteration_idx) == 1)){
				kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_RUNKERNELCOMMANDID].key = ON;
				kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_BATCHSIZE].key = batchsize[i][j];
				kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_RUNSIZE].key = keyvaluecount[i][j]; 
				kvstats[i][j][BASEOFFSET_STATSDRAM + 0].value = keyvaluecount[i][j]; 
				cout<<"...running Acts... size: "<<keyvaluecount[i][j]<<endl; 
				keyvaluecount[i][j] = 0;
			} else { 
				kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_RUNKERNELCOMMANDID].key = OFF; 
				kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_BATCHSIZE].key = batchsize[i][j]; 
				kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_RUNSIZE].key = 0;
				kvstats[i][j][BASEOFFSET_STATSDRAM + 0].value = 0; 
				cout<<"...loading KvDRAM... size: "<<batchsize[i][j]<<endl; 
			}
			
			kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_PROCESSCOMMANDID].key = ON;
			kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_COLLECTSTATSCOMMANDID].key = ON;
			kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_PARTITIONCOMMANDID].key = ON;
			kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_APPLYUPDATESCOMMANDID].key = ON;
			kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_VOFFSET].key = voffset + (j * BATCH_RANGE);
			kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_VSIZE].key = NAp;
			kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_TREEDEPTH].key = TREE_DEPTH;
			kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_FINALNUMPARTITIONS].key = pow(NUM_PARTITIONS, TREE_DEPTH);
			kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_GRAPHITERATIONID].key = graph_iterationidx;
		}
	}
	return;
}
void helperfunctions::updatemessagesafterlaunch(unsigned int globaliteration_idx, keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	if((utilityobj->runActs(globaliteration_idx) == 1)){
		for(int i = 0; i < NUMCPUTHREADS; i++){
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_NEXTBATCHOFFSET].key = 0; 
			}
		}
	} else {
		for(int i = 0; i < NUMCPUTHREADS; i++){
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_NEXTBATCHOFFSET].key += kvstats[i][j][BASEOFFSET_MESSAGESDRAM + MESSAGES_BATCHSIZE].key; 
			}
		}
	}
	return;			
}

unsigned int helperfunctions::getflag(unsigned int globaliteration_idx){
	#ifdef FPGA_IMPL
	int flag = globaliteration_idx % 2;
	#else 
	int flag = 0;
	#endif 
	return flag;
}

#ifdef FPGA_IMPL 
void helperfunctions::loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	kernelobj->loadOCLstructures(binaryFile, kvsourcedram, kvdestdram, kvstats);
}
void helperfunctions::writeVstokernel(unsigned int flag){
	kernelobj->writeVstokernel(flag);
}
void helperfunctions::readVsfromkernel(unsigned int flag){
	kernelobj->readVsfromkernel(flag);
}
void helperfunctions::finishOCL(){
	kernelobj->finishOCL();
}
#endif 








