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
	#ifdef GRAFBOOST_SETUP
	srkernelobj = new sr();
	#endif
	statsobj = _statsobj;
}
postprocess::postprocess(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
	kernelobj = new kernel();
	#ifdef GRAFBOOST_SETUP
	srkernelobj = new sr();
	#endif 
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
		// cumm = buffer[0][0][baseoffset];
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
			
			activeverticesbuffer.push_back((voffset + offset + k));
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
#ifdef FPGA_IMPL 
void postprocess::loadOCLstructures(std::string binaryFile, uint512_vec_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	kernelobj->loadOCLstructures(binaryFile, kvsourcedram);
}
void postprocess::writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	kernelobj->writetokernel(flag, kvsourcedram, hostbeginoffset, beginoffset, size);
}
void postprocess::readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	kernelobj->readfromkernel(flag, kvsourcedram, hostbeginoffset, beginoffset, size);
}
void postprocess::finishOCL(){
	kernelobj->finishOCL();
}
#endif 
#ifdef GRAFBOOST_SETUP 
inline uint32_t vertex_update(uint32_t a, uint32_t b) {
	float af = *(float*)&a;
	float bf = *(float*)&b;
	float tf = af+bf;
	
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}
inline uint32_t edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	float af = *(float*)&value;
	float tf = af/(float)fanout;

	uint32_t ret = *(uint32_t*)&tf;

	return ret;
}
size_t g_vertex_count = 1;
inline uint32_t finalize_program(uint32_t oldval, uint32_t val) {
	float damp = 0.15/(float)g_vertex_count;
	float af = *(float*)&val;
	float tf = damp + af*0.85;
	uint32_t ret = *(uint32_t*)&tf;
	return ret;
}
inline bool is_active(uint32_t old, uint32_t newv, bool marked) {
	if ( !marked ) return false;

	float af = *(float*)&old;
	float bf = *(float*)&newv;

	if ( af > bf ) {
		if (af-bf < 0.0001 ) return false;
		else return true;
	} else {
		if (bf-af < 0.0001 ) return false;
		else return true;
	}
}
void postprocess::loadSRstructures(){
	string tmp_dir = graphobj->gettmp_dir();
	string idx_path = graphobj->getidx_path();
	string mat_path = graphobj->getmat_path();
	cout<<"postprocess::loadSRstructures:: tmp_dir: "<<tmp_dir<<endl;
	cout<<"postprocess::loadSRstructures:: idx_path: "<<idx_path<<endl;
	cout<<"postprocess::loadSRstructures:: mat_path: "<<mat_path<<endl;
	cout<<"postprocess::loadSRstructures:: vertex_count: "<<graphobj->get_num_vertices()<<endl;
	
	int max_thread_count = 12;
	int max_sr_thread_count = 20;
	int max_vertexval_thread_count = 8;
	int max_edgeproc_thread_count = 8;

	// int max_vertexval_thread_count = NUMCPUTHREADS;
	size_t vertex_count = graphobj->get_num_vertices();

	SortReduceTypes::Config<uint64_t,uint32_t>* conf = new SortReduceTypes::Config<uint64_t,uint32_t>(tmp_dir.c_str(), "output.dat", max_sr_thread_count); // (NUMCPUTHREADS + 2)
	conf->SetUpdateFunction(&vertex_update);
	_sr = new SortReduce<uint64_t,uint32_t>(conf);

	float init_val2 = 1.0/(float)vertex_count;
	vertex_values = new VertexValues<uint32_t,uint32_t>(tmp_dir, vertex_count, *(uint32_t*)&init_val2, &is_active, &finalize_program, max_vertexval_thread_count);	
}
void postprocess::finishSR(){
	#ifdef _DEBUGMODE_TIMERS2
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	_sr->Finish();
	SortReduceTypes::Status status = _sr->CheckStatus();
	while ( status.done_external == false ) {
		sleep(1);
		status = _sr->CheckStatus();
		printf( "%s %s:%d-%d %s:%d-%d\n", status.done_input?"yes":"no", status.done_inmem?"yes":"no",status.internal_count, status.sorted_count, status.done_external?"yes":"no", status.external_count, status.file_count );		
		fflush(stdout);
	}
	
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::tuple<uint32_t,uint32_t,bool> res2 = _sr->Next();
	vertex_values->Start();
	while ( std::get<2>(res2) ) {
		uint32_t key = std::get<0>(res2);
		uint32_t val = std::get<1>(res2);

		// printf( "\t\t++ SRR %i %i\n", key, val );
		while ( !vertex_values->Update(key,val) ) ;

		res2 = _sr->Next();
	}
	vertex_values->Finish();
	#ifdef _DEBUGMODE_TIMERS2
	long double kerneltimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(kerneltimeelapsed_ms);
	#endif
	
	size_t active_cnt = vertex_values->GetActiveCount();
	cout<<">>> postprocess::finishSR:: active vertices after iteration: "<<active_cnt<<endl;
	vertex_values->NextIteration();		
	delete _sr;
}
#endif 







