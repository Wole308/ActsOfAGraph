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

void helperfunctions::applyvertices(unsigned int bank, unsigned int fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, unsigned int graph_iterationidx){
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_applyvertices(i, bank, fdoffset, graphobj->getvertexdatabuffer(bank), graphobj->getvertexisactivebuffer(bank), buffer, bufferoffset + (i * (datasize / NUMUTILITYTHREADS)), (datasize / NUMUTILITYTHREADS), voffset, graph_iterationidx); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i] = std::thread(&helperfunctions::workerthread_applyvertices, this, i, bank, fdoffset, graphobj->getvertexdatabuffer(bank), graphobj->getvertexisactivebuffer(bank), buffer, bufferoffset + (i * (datasize / NUMUTILITYTHREADS)), (datasize / NUMUTILITYTHREADS), voffset, graph_iterationidx); }		
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i].join(); }
	#endif
	return;
}
void helperfunctions::workerthread_applyvertices(int ithreadidx, unsigned int bank, unsigned int fdoffset, value_t * vertexdatabuffer, unsigned int * vertexisactivebuffer, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize, unsigned int voffset, unsigned int graph_iterationidx){
	graphobj->loadvertexdatafromfile(bank, 0, vertexdatabuffer, 0, KVDATA_RANGE_PERSSDPARTITION);
	
	vector<keyvalue_t> activeverticesbuffer;
	for(unsigned int i=bufferoffset; i<bufferoffset + datasize; i++){
		keyvalue_t kvtempdata = buffer[i];
		value_t vdata = vertexdatabuffer[fdoffset + i];
		value_t temp = algorithmobj->apply(kvtempdata.value, vdata);
		vertexdatabuffer[fdoffset + i] = temp;
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
	#ifdef _DEBUGMODE_HOSTPRINTS2
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printmessages("helperfunctions::launchkernel:: messages (after kernel launch)", (uint512_vec_dt *)(&kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS])); }}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions::launchkernel:: Print kvdram before Kernel run", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	#endif

	#ifdef ACTSMODEL
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->allignandappendinvalids((keyvalue_t *)kvsourcedram[i][j], kvstats[i][j][BASEOFFSET_STATSDRAM + 0].value); }} // edge conditions
	#endif
	kernelobj->launchkernel(kvsourcedram, kvdestdram, kvstats, flag);
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions::launchkernel:: Print results after Kernel run", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	for(unsigned int value=0; value<24; value++){ for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->countkeyvalueswithvalueequalto("helperfunctions::launchkernel", (keyvalue_t *)kvdestdram[i][j], KVDATA_RANGE_PERSSDPARTITION, value); }}}			
	#endif
	return;
}

#ifdef ACTSMODEL
void helperfunctions::updatemessagesbeforelaunch(unsigned int globaliteration_idx, unsigned int graph_iterationidx, unsigned int graph_algorithmidx, unsigned int voffset, unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int keyvaluecount[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int messagesbaseoffset, unsigned int kvstatsbaseoffset){			
	for(int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			if((utilityobj->runActs(globaliteration_idx) == 1)){
				kvstats[i][j][messagesbaseoffset + MESSAGES_RUNKERNELCOMMANDID].key = ON;
				kvstats[i][j][messagesbaseoffset + MESSAGES_BATCHSIZE].key = batchsize[i][j];
				kvstats[i][j][messagesbaseoffset + MESSAGES_RUNSIZE].key = keyvaluecount[i][j]; 
				kvstats[i][j][kvstatsbaseoffset + 0].value = keyvaluecount[i][j]; 
				cout<<"...running Acts... size: "<<keyvaluecount[i][j]<<endl; 
				keyvaluecount[i][j] = 0;
			} else { 
				kvstats[i][j][messagesbaseoffset + MESSAGES_RUNKERNELCOMMANDID].key = OFF; 
				kvstats[i][j][messagesbaseoffset + MESSAGES_BATCHSIZE].key = batchsize[i][j]; 
				kvstats[i][j][messagesbaseoffset + MESSAGES_RUNSIZE].key = 0;
				kvstats[i][j][kvstatsbaseoffset + 0].value = 0; 
				cout<<"...loading KvDRAM... size: "<<batchsize[i][j]<<endl; 
			}
			
			kvstats[i][j][messagesbaseoffset + MESSAGES_PROCESSCOMMANDID].key = ON;
			kvstats[i][j][messagesbaseoffset + MESSAGES_COLLECTSTATSCOMMANDID].key = ON;
			kvstats[i][j][messagesbaseoffset + MESSAGES_PARTITIONCOMMANDID].key = ON;
			kvstats[i][j][messagesbaseoffset + MESSAGES_APPLYUPDATESCOMMANDID].key = ON;
			kvstats[i][j][messagesbaseoffset + MESSAGES_APPLYUPDATESCOMMANDID].key = ON;
			kvstats[i][j][messagesbaseoffset + MESSAGES_VOFFSET].key = voffset + (j * BATCH_RANGE);
			kvstats[i][j][messagesbaseoffset + MESSAGES_VSIZE].key = NAp;
			#ifdef ACTSMODEL
			kvstats[i][j][messagesbaseoffset + MESSAGES_TREEDEPTH].key = TREE_DEPTH;
			#else 
			kvstats[i][j][messagesbaseoffset + MESSAGES_TREEDEPTH].key = TREE_DEPTH + 1;
			// kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_TREEDEPTH].key = 2; // REMOVEME.
			#endif
			kvstats[i][j][messagesbaseoffset + MESSAGES_FINALNUMPARTITIONS].key = pow(NUM_PARTITIONS, TREE_DEPTH);
			kvstats[i][j][messagesbaseoffset + MESSAGES_GRAPHITERATIONID].key = graph_iterationidx;
			kvstats[i][j][messagesbaseoffset + MESSAGES_GRAPHALGORITHMID].key = graph_algorithmidx;
		}
	}
	return;
}
void helperfunctions::updatemessagesafterlaunch(unsigned int globaliteration_idx, keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int messagesbaseoffset, unsigned int kvstatsbaseoffset){
	if((utilityobj->runActs(globaliteration_idx) == 1)){
		for(int i = 0; i < NUMCPUTHREADS; i++){
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				kvstats[i][j][messagesbaseoffset + MESSAGES_NEXTBATCHOFFSET].key = 0; 
			}
		}
	} else {
		for(int i = 0; i < NUMCPUTHREADS; i++){
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				kvstats[i][j][messagesbaseoffset + MESSAGES_NEXTBATCHOFFSET].key += kvstats[i][j][messagesbaseoffset + MESSAGES_BATCHSIZE].key; 
			}
		}
	}
	return;			
}
#endif 
#ifdef ACTSMODEL_LW
void helperfunctions::updatemessagesbeforelaunch(unsigned int globaliteration_idx, unsigned int graph_iterationidx, unsigned int graph_algorithmidx, unsigned int voffset, unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int keyvaluecount[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_vec_dt * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int messagesbaseoffset_kvs, unsigned int kvstatsbaseoffset_kvs){			
	for(int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			if((utilityobj->runActs(globaliteration_idx) == 1)){
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_RUNKERNELCOMMANDID].data[0].key = ON;
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_BATCHSIZE].data[0].key = batchsize[i][j];
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_RUNSIZE].data[0].key = keyvaluecount[i][j]; 
				kvstats[i][j][kvstatsbaseoffset_kvs + 0].data[0].value = keyvaluecount[i][j]; 
				cout<<"...running Acts... size: "<<keyvaluecount[i][j]<<endl; 
				keyvaluecount[i][j] = 0;
			} else { 
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_RUNKERNELCOMMANDID].data[0].key = OFF; 
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_BATCHSIZE].data[0].key = batchsize[i][j]; 
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_RUNSIZE].data[0].key = 0;
				kvstats[i][j][kvstatsbaseoffset_kvs + 0].data[0].value = 0; 
				cout<<"...loading KvDRAM... size: "<<batchsize[i][j]<<endl; 
			}
			
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_PROCESSCOMMANDID].data[0].key = ON;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key = ON;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_PARTITIONCOMMANDID].data[0].key = ON;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = ON;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = ON;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_VOFFSET].data[0].key = voffset + (j * BATCH_RANGE);
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_VSIZE].data[0].key = NAp;
			#ifdef ACTSMODEL
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_TREEDEPTH].data[0].key = TREE_DEPTH;
			#else 
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_TREEDEPTH].data[0].key = TREE_DEPTH + 1;
			// kvstats[i][j][HOSTBASEOFFSET_MESSAGESDRAM + MESSAGES_TREEDEPTH].data[0].key = 2; // REMOVEME.
			#endif
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_FINALNUMPARTITIONS].data[0].key = pow(NUM_PARTITIONS, TREE_DEPTH);
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_GRAPHITERATIONID].data[0].key = graph_iterationidx;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_GRAPHALGORITHMID].data[0].key = graph_algorithmidx;
		}
	}
	return;
}
void helperfunctions::updatemessagesafterlaunch(unsigned int globaliteration_idx, uint512_vec_dt * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int messagesbaseoffset_kvs, unsigned int kvstatsbaseoffset_kvs){
	if((utilityobj->runActs(globaliteration_idx) == 1)){
		for(int i = 0; i < NUMCPUTHREADS; i++){
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_NEXTBATCHOFFSET].data[0].key = 0; 
			}
		}
	} else {
		for(int i = 0; i < NUMCPUTHREADS; i++){
			for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_NEXTBATCHOFFSET].data[0].key += kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_BATCHSIZE].data[0].key; 
			}
		}
	}
	return;			
}
#endif 

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








