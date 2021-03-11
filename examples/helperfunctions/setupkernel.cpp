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
#include "../../kernels/swkernel.h"
#include "../../kernels/goclkernel.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "setupkernel.h"
using namespace std;

setupkernel::setupkernel(graph * _graphobj, stats * _statsobj){
	#ifdef FPGA_IMPL
	kernelobj = new goclkernel(_statsobj);
	#else 
	kernelobj = new swkernel(_statsobj);
	#endif
	
	utilityobj = new utility();
	graphobj = _graphobj;
	#ifdef GRAFBOOST_SETUP
	srkernelobj = new sr();
	#endif
	statsobj = _statsobj;
}
setupkernel::setupkernel(stats * _statsobj){
	utilityobj = new utility();
	#ifdef GRAFBOOST_SETUP
	srkernelobj = new sr();
	#endif 
}
setupkernel::~setupkernel(){} 

void setupkernel::runapp(std::string binaryFile, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	#ifdef GRAFBOOST_SETUP
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	#endif 

	#ifdef ACTGRAPH_SETUP
	kernelobj->runapp(binaryFile, vdram, kvsourcedram);
	#endif 
	#ifdef GRAFBOOST_SETUP
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ srkernelobj->srtopkernel(_sr, (uint512_dt *)kvsourcedram[i]); }
	#endif
	
	#ifdef GRAFBOOST_SETUP
	#ifdef _DEBUGMODE_TIMERS3
	long double kerneltimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(kerneltimeelapsed_ms);
	#endif
	#endif 
	return;
}

#ifdef GRAFBOOST_SETUP 
#ifdef PR_ALGORITHM
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
#elif defined(BFS_ALGORITHM)
inline uint32_t vertex_update(uint32_t a, uint32_t b) {
	uint32_t ret = a;
	return ret;
}
inline uint32_t edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	return vid;
}
inline uint32_t finalize_program(uint32_t oldval, uint32_t val) {
	return val;
}
inline bool is_active(uint32_t old, uint32_t newv, bool marked) {
	//printf( "Comparing %lx %lx %s\n", old, newv, marked?"Y":"N" );
	if ( old == 0xffffffff ) return true;
	//printf( "Comparing %x %x %s\n", old, newv, marked?"Y":"N" );
	return false;
}
#elif defined(SSSP_ALGORITHM)
inline uint32_t vertex_update(uint32_t a, uint32_t b) {
	uint32_t ret = a;
	return ret;
}
inline uint32_t edge_program(uint32_t vid, uint32_t value, uint32_t edgeweight) {
	//printf( "Edge-program source: %x val: %x edgeweight: %x\n", vid, value, edgeweight);
	return value + edgeweight;
}
inline uint32_t finalize_program(uint32_t oldval, uint32_t val) {
	return val;
}
inline bool is_active(uint32_t old, uint32_t newv, bool marked) {
	//printf( "Comparing %lx %lx %s\n", old, newv, marked?"Y":"N" );
	if ( old == 0xffffffff ) return true;
	//printf( "Comparing %x %x %s\n", old, newv, marked?"Y":"N" );
	return false;
}
#elif defined(BC_ALGORITHM)
inline uint32_t vertex_update_add(uint32_t a, uint32_t b) {
	return a+b;
}
inline uint32_t vertex_update(uint32_t a, uint32_t b) {
	uint32_t ret = a;
	return ret;
}
inline uint32_t edge_program(uint32_t vid, uint32_t value, uint32_t fanout) {
	//printf( "Edge-program source: %x val: %x fanout: %x\n", vid, value, fanout);
	return vid;
}
inline uint32_t finalize_program(uint32_t oldval, uint32_t newval) {
	return newval;
}
inline bool is_active(uint32_t old, uint32_t newv, bool marked) {
	//printf( "Comparing %lx %lx %s\n", old, newv, marked?"Y":"N" );
	if ( old == 0xffffffff ) return true;
	//printf( "Comparing %x %x %s\n", old, newv, marked?"Y":"N" );
	return false;
}
#else 
ERROR: SETUPKERNEL: NO ALGORITHM SPECIFIED 
#endif 
void setupkernel::loadSRstructures(){
	string tmp_dir = graphobj->gettmp_dir();
	string idx_path = graphobj->getidx_path();
	string mat_path = graphobj->getmat_path();
	cout<<"setupkernel::loadSRstructures:: tmp_dir: "<<tmp_dir<<endl;
	cout<<"setupkernel::loadSRstructures:: idx_path: "<<idx_path<<endl;
	cout<<"setupkernel::loadSRstructures:: mat_path: "<<mat_path<<endl;
	cout<<"setupkernel::loadSRstructures:: vertex_count: "<<graphobj->get_num_vertices()<<endl;
	
	int max_thread_count = 12;
	int max_sr_thread_count = 20;
	int max_vertexval_thread_count = 8;
	int max_edgeproc_thread_count = 8;

	size_t vertex_count = graphobj->get_num_vertices();

	#ifdef PR_ALGORITHM
	float init_val2 = 1.0/(float)vertex_count;
	vertex_values = new VertexValues<uint32_t,uint32_t>(tmp_dir, vertex_count, *(uint32_t*)&init_val2, &is_active, &finalize_program, max_vertexval_thread_count);	
	#elif defined(BFS_ALGORITHM)
	vertex_values = new VertexValues<uint32_t,uint32_t>(tmp_dir, vertex_count, 0xffffffff, &is_active, &finalize_program, max_vertexval_thread_count);
	#elif defined(SSSP_ALGORITHM)
	vertex_values = new VertexValues<uint32_t,uint32_t>(tmp_dir, vertex_count, 0xffffffff, &is_active, &finalize_program, max_vertexval_thread_count);
	#elif defined(BC_ALGORITHM)
	vertex_values = new VertexValues<uint32_t,uint32_t>(tmp_dir, vertex_count, 0xffffffff, &is_active, &finalize_program, max_vertexval_thread_count);
	#endif 
}

void setupkernel::startSRteration(){
	string tmp_dir = graphobj->gettmp_dir();
	string idx_path = graphobj->getidx_path();
	string mat_path = graphobj->getmat_path();

	int max_thread_count = 12;
	int max_sr_thread_count = 20;
	int max_vertexval_thread_count = 8;
	int max_edgeproc_thread_count = 8;

	size_t vertex_count = graphobj->get_num_vertices();

	SortReduceTypes::Config<uint64_t,uint32_t>* conf = new SortReduceTypes::Config<uint64_t,uint32_t>(tmp_dir.c_str(), "output.dat", max_sr_thread_count); // (NUMCPUTHREADS + 2)
	conf->quiet = true; // 
	conf->SetUpdateFunction(&vertex_update);
	_sr = new SortReduce<uint64_t,uint32_t>(conf);
}

unsigned int setupkernel::finishSRteration(unsigned int iteration, vector<value_t> &activevertices){
	#ifdef _DEBUGMODE_TIMERS3
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

		// printf( "\t\t++ setupkernel::finishSR:SRR %i %i\n", key, val );
		while ( !vertex_values->Update(key,val) ) ;

		res2 = _sr->Next();
	}
	vertex_values->Finish();
	#ifdef _DEBUGMODE_TIMERS3
	long double kerneltimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(kerneltimeelapsed_ms);
	#endif
	
	size_t active_cnt = vertex_values->GetActiveCount();
	cout<<">>> setupkernel::finishSR:: active vertices after iteration: "<<active_cnt<<endl;
	vertex_values->NextIteration();		
	
	int fd = vertex_values->OpenActiveFile(iteration);
	SortReduceUtils::FileKvReader<uint32_t,uint32_t>* reader = new SortReduceUtils::FileKvReader<uint32_t,uint32_t>(fd);
	std::tuple<uint32_t,uint32_t,bool> res = reader->Next();
	while ( std::get<2>(res) ) {
		uint32_t key = std::get<0>(res);
		uint32_t val = std::get<1>(res);
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"setupkernel::finishSRteration: key: "<<key<<", val: "<<val<<endl;
		#endif 
		activevertices.push_back(key);
		res = reader->Next();
	}
	
	delete _sr;
	return active_cnt;
}
#endif 







