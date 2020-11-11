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
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "postprocess.h"
using namespace std;

postprocess::postprocess(graph * _graphobj, stats * _statsobj){
	parametersobj = new parameters();
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	kernelobj = new kernel();
	statsobj = _statsobj;
}
postprocess::postprocess(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
	kernelobj = new kernel();
}
postprocess::~postprocess(){} 

void postprocess::cummulateverticesdata(value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"... cummulating vertex datas... ["<<NUMCPUTHREADS<<" threads, "<<NUMSUBCPUTHREADS<<" subthreads]"<<endl;
	#endif 
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_cummulateverticesdata(i, buffer, (i * (KVDATA_RANGE_PERSSDPARTITION / NUMUTILITYTHREADS)), (KVDATA_RANGE_PERSSDPARTITION / NUMUTILITYTHREADS)); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mykernelthread[i] = std::thread(&postprocess::workerthread_cummulateverticesdata, this, i, buffer, (i * (KVDATA_RANGE_PERSSDPARTITION / NUMUTILITYTHREADS)), (KVDATA_RANGE_PERSSDPARTITION / NUMUTILITYTHREADS)); }
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mykernelthread[i].join(); }
	#endif 
	return;
}
void postprocess::workerthread_cummulateverticesdata(int threadidx, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size){
	unsigned int baseoffset = BASEOFFSET_VERTICESDATA * (sizeof(keyvalue_t) / sizeof(value_t));
	unsigned int onceactivecnt = 0;
	value_t cumm = INFINITI;
	
	for(unsigned int k=offset; k<(offset + size); k++){
		cumm = buffer[0][0][baseoffset + k];
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			cumm = algorithmobj->cummulate(cumm, buffer[0][j][baseoffset + k]);
			
			if(cumm < INFINITI){ onceactivecnt += 1; }
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(cumm < INFINITI){ cout<<"cummulateverticesdata: once active vertex seen @ "<<k<<": cumm: "<<cumm<<", buffer[0]["<<j<<"]["<<baseoffset + k<<"]: "<<buffer[0][j][baseoffset + k]<<endl; }
			#endif
		}
		// if(k-offset >= 20){ exit(EXIT_SUCCESS); } // REMOVEME.
		buffer[0][0][baseoffset + k] = cumm;
	}
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"workerthread_cummulateverticesdata: number of vertex ids once active: "<<onceactivecnt<<endl;
	#endif 
	return;
}

void postprocess::applyvertices(vector<value_t> &activeverticesbuffer, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset, unsigned int vsize){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"... applying vertex datas... ["<<NUMCPUTHREADS<<" threads, "<<NUMSUBCPUTHREADS<<" subthreads]"<<endl;
	#endif 
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_applyvertices(i, activeverticesbuffer, buffer, voffset, i * (vsize / NUMUTILITYTHREADS), (vsize / NUMUTILITYTHREADS)); }
	#else
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i] = std::thread(&postprocess::workerthread_applyvertices, this, i, activeverticesbuffer, buffer, voffset, i * (vsize / NUMUTILITYTHREADS), (vsize / NUMUTILITYTHREADS)); }	
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i].join(); }
	#endif
	return;
}
void postprocess::workerthread_applyvertices(int ithreadidx, vector<value_t> &activeverticesbuffer, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset, vertex_t offset, vertex_t size){		
	value_t * vertexdatabuffer = graphobj->getvertexdatabuffer();
	unsigned int baseoffset = BASEOFFSET_VERTICESDATA * (sizeof(keyvalue_t) / sizeof(value_t));
	unsigned int onceactivecnt = 0;
	
	for(unsigned int k=0; k<size; k++){
		value_t kvtempdata = buffer[0][0][baseoffset + k];
		value_t vdata = vertexdatabuffer[voffset + offset + k];
		value_t temp = algorithmobj->apply(kvtempdata, vdata);
		vertexdatabuffer[voffset + offset + k] = temp;
	
		if(temp != vdata){
			onceactivecnt += 1; 
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"applyvertices: active vertex seen @ "<<k<<": vid: "<<voffset + offset + k<<", temp: "<<temp<<", vdata: "<<vdata<<endl; 
			#endif 
			
			#ifndef PAGERANK
			activeverticesbuffer.push_back((voffset + offset + k));
			#endif
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"workerthread_applyvertices: number of vertex ids once active: "<<onceactivecnt<<endl;
	cout<<"workerthread_applyvertices: number of vertex ids in activeverticesbuffer: "<<activeverticesbuffer.size()<<endl;
	utilityobj->countvalueslessthan("workerthread_applyvertices", vertexdatabuffer, KVDATA_RANGE, INFINITI);
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"applyvertices: number of active vertices for next iteration: "<<activeverticesbuffer.size()<<endl;
	#endif 
	return;
}

unsigned int postprocess::getflag(unsigned int globaliteration_idx){
	#ifdef FPGA_IMPL
	int flag = globaliteration_idx % NUMFLAGS;
	#else 
	int flag = 0;
	#endif 
	return flag;
}






