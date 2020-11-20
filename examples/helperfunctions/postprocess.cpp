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

std::mutex mtx; // mutex for critical section

postprocess::postprocess(graph * _graphobj, stats * _statsobj){
	parametersobj = new parameters();
	utilityobj = new utility();
	graphobj = _graphobj;
	for(unsigned int i=0; i<NUMUTILITYTHREADS; i++){ algorithmobj[i] = new algorithm(); }
	kernelobj = new kernel();
	statsobj = _statsobj;
}
postprocess::postprocess(){
	utilityobj = new utility();
	for(unsigned int i=0; i<NUMUTILITYTHREADS; i++){ algorithmobj[i] = new algorithm(); }
	kernelobj = new kernel();
}
postprocess::~postprocess(){} 

void postprocess::cummulateandcommitverticesdata(value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], value_t * tempvertexdatabuffer, vertex_t voffset){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"... cummulating vertex datas... ["<<NUMCPUTHREADS<<" threads, "<<NUMSUBCPUTHREADS<<" subthreads]"<<endl;
	#endif 
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_cummulateandcommitverticesdata(i, buffer, tempvertexdatabuffer, voffset, (i * (KVDATA_RANGE_PERSSDPARTITION / NUMUTILITYTHREADS)), (KVDATA_RANGE_PERSSDPARTITION / NUMUTILITYTHREADS)); }
	#else 
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i] = std::thread(&postprocess::workerthread_cummulateandcommitverticesdata, this, i, buffer, tempvertexdatabuffer, voffset, (i * (KVDATA_RANGE_PERSSDPARTITION / NUMUTILITYTHREADS)), (KVDATA_RANGE_PERSSDPARTITION / NUMUTILITYTHREADS)); }
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i].join(); }
	#endif 
	
	#ifdef _DEBUGMODE_TIMERS2
	utilityobj->stopBTIME("cummulateandcommitverticesdata:: total time elapsed: ", begintime, NAp);
	#endif
	#ifdef _DEBUGMODE_TIMERS3
	long double cummvstimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendcummvstimeelapsed(cummvstimeelapsed_ms);
	#endif
	return;
}
void postprocess::workerthread_cummulateandcommitverticesdata(int threadidx, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], value_t * tempvertexdatabuffer, vertex_t voffset, unsigned int offset, unsigned int size){
	unsigned int baseoffset = BASEOFFSET_VERTICESDATA * (sizeof(keyvalue_t) / sizeof(value_t));
	unsigned int onceactivecnt = 0;
	value_t cumm = INFINITI;
	
	for(unsigned int k=offset; k<(offset + size); k++){
		cumm = INFINITI;
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			cumm = algorithmobj[threadidx]->cummulate(cumm, buffer[0][j][baseoffset + k]);
			
			if(cumm < INFINITI){ onceactivecnt += 1; }
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(cumm < INFINITI){ cout<<"cummulateverticesdata: once active vertex seen @ "<<k<<": cumm: "<<cumm<<", buffer[0]["<<j<<"]["<<baseoffset + k<<"]: "<<buffer[0][j][baseoffset + k]<<endl; }
			#endif
		}
		tempvertexdatabuffer[voffset + k] = cumm;
	}
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"workerthread_cummulateverticesdata: number of vertex ids once active: "<<onceactivecnt<<endl;
	#endif 
	return;
}

void postprocess::applyvertices2(value_t * tempvertexdatabuffer, value_t * vertexdatabuffer, vector<value_t> &activeverticesbuffer, unsigned int GraphAlgo){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"... applying vertex datas... ["<<NUMCPUTHREADS<<" threads, "<<NUMSUBCPUTHREADS<<" subthreads]"<<endl;
	#endif
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef LOCKE
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ workerthread_applyvertices2(i, tempvertexdatabuffer, vertexdatabuffer, activeverticesbuffer, i * (KVDATA_RANGE / NUMUTILITYTHREADS), (KVDATA_RANGE / NUMUTILITYTHREADS), GraphAlgo); }
	#else
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i] = std::thread(&postprocess::workerthread_applyvertices2, this, i, tempvertexdatabuffer, vertexdatabuffer, std::ref(activeverticesbuffer), i * (KVDATA_RANGE / NUMUTILITYTHREADS), (KVDATA_RANGE / NUMUTILITYTHREADS), GraphAlgo); }	
	for (int i = 0; i < NUMUTILITYTHREADS; i++){ mythread[i].join(); }
	#endif

	#ifdef _DEBUGMODE_TIMERS2
	utilityobj->stopBTIME("applyvertices2:: total time elapsed: ", begintime, NAp);
	#endif
	#ifdef _DEBUGMODE_TIMERS3
	long double applyvstimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendapplyvstimeelapsed(applyvstimeelapsed_ms);
	#endif
	return;
}
void postprocess::workerthread_applyvertices2(int ithreadidx, value_t * tempvertexdatabuffer, value_t * vertexdatabuffer, vector<value_t> &activeverticesbuffer, vertex_t offset, vertex_t size, unsigned int GraphAlgo){		
	unsigned int onceactivecnt = 0;
	
	for(unsigned int k=0; k<size; k++){
		value_t kvtempdata = tempvertexdatabuffer[offset + k]; 
		value_t vdata = vertexdatabuffer[offset + k];
		value_t temp = algorithmobj[ithreadidx]->apply(kvtempdata, vdata);
		// mtx.lock(); //
		vertexdatabuffer[offset + k] = temp;
		// mtx.unlock();// 
		
		if(temp != vdata){
			onceactivecnt += 1; 
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"applyvertices2: active vertex seen @ "<<k<<": vid: "<<offset + k<<", *temp: "<<temp<<", vdata: "<<vdata<<endl; 
			#endif 
			
			if(GraphAlgo != PAGERANK){ 
				mtx.lock(); //
				activeverticesbuffer.push_back((offset + k)); 
				mtx.unlock();// 
			}
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS2
	mtx.lock(); //
	cout<<"applyvertices2: number of vertex ids once active: "<<onceactivecnt<<endl;
	cout<<"applyvertices2: number of vertex ids in activeverticesbuffer: "<<activeverticesbuffer.size()<<endl;
	utilityobj->countvalueslessthan("applyvertices2", vertexdatabuffer, KVDATA_RANGE, INFINITI);
	mtx.unlock();// 
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	mtx.lock(); //
	cout<<"applyvertices2: number of active vertices for next iteration: "<<activeverticesbuffer.size()<<endl;
	mtx.unlock();// 
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






