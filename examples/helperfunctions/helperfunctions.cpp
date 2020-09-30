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
#include "../../kernels/kernel.h"
#include "../../include/common.h"
#include "helperfunctions.h"
using namespace std;

helperfunctions::helperfunctions(graph * _graphobj){
	parametersobj = new parameters();
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

// replicate vertices data
#ifdef ACTSMODEL
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
	cout<<"... Replicating vertex datas...: "<<size * NUMSUBCPUTHREADS<<" vertex data values replicated. "<<endl;
	return;
}
#endif 
#ifdef ACTSMODEL_LW
void helperfunctions::replicateverticesdata(value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_replicateverticesdata(i, buffer, offset + (i * (size / NUMUTILITYTHREADS)), (size / NUMUTILITYTHREADS)); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i] = std::thread(&helperfunctions::workerthread_replicateverticesdata, this, i, buffer, offset + (i * (size / NUMUTILITYTHREADS)), (size / NUMUTILITYTHREADS)); }
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i].join(); }
	#endif 
	return;
}
void helperfunctions::workerthread_replicateverticesdata(int threadidx, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ /// subthreads have consecutive ranges
		for(unsigned int i=0; i<NUMCPUTHREADS; i++){ /// threads have same range
			memcpy(&buffer[i][j][offset], &buffer[0][0][offset], (size * sizeof(keyvalue_t))); 
		}
	}
	cout<<"... Replicating vertex datas [ithreadidx: "<<threadidx<<"]...: "<<size * NUMSUBCPUTHREADS<<" vertex data values replicated. "<<endl;
	return;
}
#endif 

// cummulate vertices data
#ifdef ACTSMODEL
void helperfunctions::cummulateverticesdata(keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_cummulateverticesdata(i, buffer, offset + (i * (size / NUMUTILITYTHREADS)), (size / NUMUTILITYTHREADS)); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mykernelthread[i][j] = std::thread(&helperfunctions::workerthread_cummulateverticesdata, this, i, buffer, offset + (i * (size / NUMUTILITYTHREADS)), (size / NUMUTILITYTHREADS)); }
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mykernelthread[i][j].join(); }
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
#endif 
#ifdef ACTSMODEL_LW
void helperfunctions::cummulateverticesdata(value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_cummulateverticesdata(i, buffer, offset + (i * (size / NUMUTILITYTHREADS)), (size / NUMUTILITYTHREADS)); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mykernelthread[i][j] = std::thread(&helperfunctions::workerthread_cummulateverticesdata, this, i, buffer, offset + (i * (size / NUMUTILITYTHREADS)), (size / NUMUTILITYTHREADS)); }
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mykernelthread[i][j].join(); }
	#endif 
	return;
}
void helperfunctions::workerthread_cummulateverticesdata(int threadidx, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	value_t min;
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ /// subthreads have consecutive ranges
		for(unsigned int k=offset; k<(offset + size); k++){
			min = buffer[0][j][k];
			for(unsigned int i=1; i<NUMCPUTHREADS; i++){ /// threads have same range
				min = utilityobj->hmin(min, buffer[i][j][k]);
			}
			buffer[0][j][k] = min;
		}
	}
	cout<<"... Cummulating vertex datas [ithreadidx: "<<threadidx<<"]...: "<<size * NUMSUBCPUTHREADS<<" vertex data values cummulated. "<<endl;
	return;
}
#endif 

// apply vertices
#ifdef ACTSMODEL
void helperfunctions::applyvertices(unsigned int bank, unsigned int fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, unsigned int graph_iterationidx){
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_applyvertices(i, bank, fdoffset, buffer, bufferoffset + (i * (datasize / NUMUTILITYTHREADS)), (datasize / NUMUTILITYTHREADS), voffset, graph_iterationidx); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i] = std::thread(&helperfunctions::workerthread_applyvertices, this, i, bank, fdoffset, graphobj->getvertexdatabuffer(bank), graphobj->getvertexisactivebuffer(bank), buffer, bufferoffset + (i * (datasize / NUMUTILITYTHREADS)), (datasize / NUMUTILITYTHREADS), voffset, graph_iterationidx); }		
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i].join(); }
	#endif
	return;
}
void helperfunctions::workerthread_applyvertices(int ithreadidx, unsigned int bank, unsigned int fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize, unsigned int voffset, unsigned int graph_iterationidx){
	value_t * vertexdatabuffer = graphobj->getvertexdatabuffer(bank);
	unsigned int * vertexisactivebuffer = graphobj->getvertexisactivebuffer(bank);
	
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
#endif 
#ifdef ACTSMODEL_LW
void helperfunctions::applyvertices(unsigned int bank, unsigned int fdoffset, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, hostglobalparams_t globalparams){
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_applyvertices(i, bank, fdoffset, buffer, bufferoffset + (i * (datasize / NUMUTILITYTHREADS)), (datasize / NUMUTILITYTHREADS), voffset, globalparams); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i] = std::thread(&helperfunctions::workerthread_applyvertices, this, i, bank, fdoffset, graphobj->getvertexdatabuffer(bank), graphobj->getvertexisactivebuffer(bank), buffer, bufferoffset + (i * (datasize / NUMUTILITYTHREADS)), (datasize / NUMUTILITYTHREADS), voffset, graph_iterationidx); }		
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i].join(); }
	#endif
	return;
}
void helperfunctions::workerthread_applyvertices(int ithreadidx, unsigned int bank, unsigned int fdoffset, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vertex_t bufferoffset, vertex_t datasize, unsigned int voffset, hostglobalparams_t globalparams){
	value_t * vertexdatabuffer = graphobj->getvertexdatabuffer(bank);
	unsigned int * vertexisactivebuffer = graphobj->getvertexisactivebuffer(bank);
	
	vector<keyvalue_t> activeverticesbuffer;
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		for(unsigned int k=bufferoffset; k<bufferoffset + datasize; k++){
			value_t kvtempdata = buffer[0][j][k];
			value_t vdata = vertexdatabuffer[fdoffset + j*parametersobj->GET_BATCH_RANGE(globalparams.groupid) + k];
			value_t temp = algorithmobj->apply(kvtempdata, vdata);
			vertexdatabuffer[fdoffset + j*parametersobj->GET_BATCH_RANGE(globalparams.groupid) + k] = temp;
			#ifndef PR_ALGORITHM // FIXME. use algorithmobj instead
			if(temp != vdata){
				keyvalue_t kv;
				kv.key = voffset + k; 
				kv.value = temp;
				activeverticesbuffer.push_back(kv);
				utilityobj->InsertBit(vertexisactivebuffer, k, 1);
			}
			#endif 
		}
	}
	
	graphobj->saveactiveverticestofile(activeverticesbuffer, (globalparams.graph_iterationidx + 1));
	activeverticesbuffer.clear();
	cout<<"... Applying vertex datas [ithreadidx: "<<ithreadidx<<"]...: "<<datasize * NUMSUBCPUTHREADS<<" vertex data values applied. "<<endl;
	return;
}
#endif 

void helperfunctions::launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions::launchkernel:: Print kvdram (before Kernel launch)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	#ifdef ACTSMODEL_LW
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printmessages("helperfunctions::launchkernel:: messages (before kernel launch)", (uint512_vec_dt *)(&kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS])); }}
	#endif
	#endif
	
	#ifdef ACTSMODEL
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->allignandappendinvalids((keyvalue_t *)kvsourcedram[i][j], kvstats[i][j][BASEOFFSET_STATSDRAM + 0].value); }} // edge conditions
	#endif
	kernelobj->launchkernel(kvsourcedram, kvdestdram, kvstats, flag);
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions::launchkernel:: Print results (after Kernel run)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	for(unsigned int value=0; value<24; value++){ for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->countkeyvalueswithvalueequalto("helperfunctions::launchkernel", (keyvalue_t *)kvdestdram[i][j], KVDATA_RANGE_PERSSDPARTITION, value); }}}			
	#endif
	return;
}

// update messages before & after launch
#ifdef ACTSMODEL
void helperfunctions::updatemessagesbeforelaunch(unsigned int globaliteration_idx, unsigned int voffset, unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int keyvaluecount[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int messagesbaseoffset, unsigned int kvstatsbaseoffset, hostglobalparams_t globalparams){											
	unsigned int totalkeyvalueslaunched = 0;
	for(int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			totalkeyvalueslaunched += keyvaluecount[i][j];
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
			kvstats[i][j][messagesbaseoffset + MESSAGES_VOFFSET].key = globalparams.groupbasevoffset + voffset + (j * parametersobj->GET_BATCH_RANGE(globalparams.groupid)); 
			kvstats[i][j][messagesbaseoffset + MESSAGES_VSIZE].key = NAp;
			kvstats[i][j][messagesbaseoffset + MESSAGES_TREEDEPTH].key = parametersobj->GET_TREE_DEPTH(globalparams.groupid); 
			kvstats[i][j][messagesbaseoffset + MESSAGES_FINALNUMPARTITIONS].key = pow(NUM_PARTITIONS, parametersobj->GET_TREE_DEPTH(globalparams.groupid)); 
			kvstats[i][j][messagesbaseoffset + MESSAGES_GRAPHITERATIONID].key = globalparams.graph_iterationidx;
			kvstats[i][j][messagesbaseoffset + MESSAGES_GRAPHALGORITHMID].key = globalparams.graph_algorithmidx;
			kvstats[i][j][messagesbaseoffset + MESSAGES_GROUPID].key = globalparams.groupid;
			kvstats[i][j][messagesbaseoffset + MESSAGES_BEGINLOP].key = 1;
			kvstats[i][j][messagesbaseoffset + MESSAGES_ENDLOP].key = parametersobj->GET_TREE_DEPTH(globalparams.groupid) + 1;
			kvstats[i][j][messagesbaseoffset + MESSAGES_BATCHRANGE].key = parametersobj->GET_BATCH_RANGE(globalparams.groupid); 
			kvstats[i][j][messagesbaseoffset + MESSAGES_BATCHRANGE_POW].key = parametersobj->GET_BATCH_RANGE_POW(globalparams.groupid);
			kvstats[i][j][messagesbaseoffset + MESSAGES_APPLYVERTEXBUFFERSZ].key = parametersobj->GET_APPLYVERTEXBUFFERSZ(globalparams.groupid); 
			kvstats[i][j][messagesbaseoffset + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].key = parametersobj->GET_APPLYVERTEXBUFFERSZ_KVS(globalparams.groupid); 
		}
	}
	cout<<"...running Acts... total size: "<<totalkeyvalueslaunched<<endl; 
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
void helperfunctions::updatemessagesbeforelaunch(unsigned int globaliteration_idx, unsigned int voffset, unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int keyvaluecount[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_vec_dt * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int messagesbaseoffset_kvs, unsigned int kvstatsbaseoffset_kvs, hostglobalparams_t globalparams){			
	unsigned int totalkeyvalueslaunched = 0;
	for(int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			totalkeyvalueslaunched += keyvaluecount[i][j];
			if((utilityobj->runActs(globaliteration_idx) == 1)){
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_RUNKERNELCOMMANDID].data[0].key = ON;
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_BATCHSIZE].data[0].key = batchsize[i][j];
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_RUNSIZE].data[0].key = keyvaluecount[i][j]; 
				kvstats[i][j][kvstatsbaseoffset_kvs + 0].data[0].value = keyvaluecount[i][j]; 
				cout<<"...running Acts... size["<<i<<"]["<<j<<"]: "<<keyvaluecount[i][j]<<endl; 
				keyvaluecount[i][j] = 0;
			} else { 
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_RUNKERNELCOMMANDID].data[0].key = OFF; 
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_BATCHSIZE].data[0].key = batchsize[i][j]; 
				kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_RUNSIZE].data[0].key = 0;
				kvstats[i][j][kvstatsbaseoffset_kvs + 0].data[0].value = 0; 
				cout<<"...loading KvDRAM... size["<<i<<"]["<<j<<"]: "<<batchsize[i][j]<<endl; 
			}
			
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_PROCESSCOMMANDID].data[0].key = ON;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key = ON;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_PARTITIONCOMMANDID].data[0].key = ON;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = ON;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_VOFFSET].data[0].key = globalparams.groupbasevoffset + voffset + (j * parametersobj->GET_BATCH_RANGE(globalparams.groupid));
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_VSIZE].data[0].key = NAp;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_TREEDEPTH].data[0].key = parametersobj->GET_TREE_DEPTH(globalparams.groupid) + 1;
			// kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_TREEDEPTH].data[0].key = 1; // REMOVEME.
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_FINALNUMPARTITIONS].data[0].key = pow(NUM_PARTITIONS, parametersobj->GET_TREE_DEPTH(globalparams.groupid));
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_GRAPHITERATIONID].data[0].key = globalparams.graph_iterationidx;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_GRAPHALGORITHMID].data[0].key = globalparams.graph_algorithmidx;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_GROUPID].data[0].key = globalparams.groupid;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_BEGINLOP].data[0].key = 0;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_ENDLOP].data[0].key = parametersobj->GET_TREE_DEPTH(globalparams.groupid) + 1;
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_BATCHRANGE].data[0].key = parametersobj->GET_BATCH_RANGE(globalparams.groupid); 
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_BATCHRANGE_POW].data[0].key = parametersobj->GET_BATCH_RANGE_POW(globalparams.groupid);
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_APPLYVERTEXBUFFERSZ].data[0].key = parametersobj->GET_APPLYVERTEXBUFFERSZ(globalparams.groupid); 
			kvstats[i][j][messagesbaseoffset_kvs + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].data[0].key = parametersobj->GET_APPLYVERTEXBUFFERSZ_KVS(globalparams.groupid);
		}
	}
	cout<<"...running Acts... total size: "<<totalkeyvalueslaunched<<endl; 
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
void helperfunctions::writeVstokernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset, unsigned int size){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	if(size >= (MESSAGESDRAMSZ + KVDRAMBUFFERSZ + KVDRAMSZ)){ utilityobj->printstructuresbeforekernelrun("helperfunctions::writeVstokernel", (uint512_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram); }
	#endif
	kernelobj->writeVstokernel(flag, kvsourcedram, beginoffset, size);
}
void helperfunctions::readVsfromkernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset, unsigned int size){
	kernelobj->readVsfromkernel(flag, kvsourcedram, beginoffset, size);
	#ifdef _DEBUGMODE_HOSTPRINTS2
	if(size >= (MESSAGESDRAMSZ + KVDRAMBUFFERSZ + KVDRAMSZ)){ utilityobj->printstructuresafterkernelrun("helperfunctions::readVsfromkernel", (uint512_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram); }
	#endif
}
void helperfunctions::finishOCL(){
	kernelobj->finishOCL();
}
#endif 








