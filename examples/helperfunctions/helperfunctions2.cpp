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
#include "helperfunctions2.h"
using namespace std;

helperfunctions2::helperfunctions2(graph * _graphobj, stats * _statsobj){
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
helperfunctions2::helperfunctions2(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
	kernelobj = new kernel();
	#ifdef GRAFBOOST_SETUP
	srkernelobj = new sr();
	#endif 
}
helperfunctions2::~helperfunctions2(){} 

// create messages
void helperfunctions2::createmessages(
			uint512_vec_dt * kvstats,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int destvoffset,
			unsigned int beginvid,
			unsigned int beginkey,
			unsigned int beginvalue,
			unsigned int treedepth,
			unsigned int GraphIter,
			unsigned int GraphAlgo,
			unsigned int runsize,
			unsigned int batch_range,
			unsigned int batch_range_pow,
			unsigned int applyvertexbuffersz,
			unsigned int numlastlevelpartitions){
				// cout<<"------------- runsize: "<<runsize<<endl;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key = srcvoffset;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key = srcvsize;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE_KVS].data[0].key = (srcvsize + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_DESTVOFFSET].data[0].key = destvoffset;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINVID].data[0].key = beginvid;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINKEY].data[0].key = beginkey;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINVALUE].data[0].key = beginvalue;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key = treedepth;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key = GraphIter;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0].key = GraphAlgo;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key = 0; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key = runsize; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key = runsize; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE_KVS].data[0].key = (runsize + (VECTOR_SIZE - 1)) / VECTOR_SIZE; 
	
	if(runsize == 0 || runsize >= KVSOURCEDRAMSZ){
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = 0;
	} else {
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 2;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1; // REMOVEME
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 1;
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1; // REMOVEME
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1;
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
	}
	
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE].data[0].key = batch_range; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_KVS].data[0].key = batch_range / VECTOR_SIZE; // batch_range_kvs;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_POW].data[0].key = batch_range_pow;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ].data[0].key = applyvertexbuffersz; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].data[0].key = applyvertexbuffersz / VECTOR_SIZE; // applyvertexbuffersz_kvs;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key = numlastlevelpartitions;
	return;
}

// launch kernel
void helperfunctions2::launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions2::launchkernel:: Print kvdram (before Kernel launch)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), 16); }}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printmessages("helperfunctions2::launchkernel:: messages (before kernel launch)", (uint512_vec_dt *)(&kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS])); }}
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ utilityobj->printkeyvalues("helperfunctions2::launchkernel:: Print vertex datas (before Kernel launch)", (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_VERTICESDATA_KVS]), 16); }}
	#endif
	#ifdef _DEBUGMODE_TIMERS2
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef ACTGRAPH_SETUP
	kernelobj->launchkernel(kvsourcedram, flag);
	#endif 
	#ifdef GRAFBOOST_SETUP
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			srkernelobj->srtopkernel(_sr, (keyvalue_t *)(&kvsourcedram[i][j][BASEOFFSET_KVDRAM_KVS]), kvsourcedram[i][j][BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key);
		}
	}
	#endif 
	
	#ifdef _DEBUGMODE_TIMERS2
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
void helperfunctions2::loadOCLstructures(std::string binaryFile, uint512_vec_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	kernelobj->loadOCLstructures(binaryFile, kvsourcedram);
}
void helperfunctions2::writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	kernelobj->writetokernel(flag, kvsourcedram, hostbeginoffset, beginoffset, size);
}
void helperfunctions2::readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	kernelobj->readfromkernel(flag, kvsourcedram, hostbeginoffset, beginoffset, size);
}
void helperfunctions2::finishOCL(){
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
void helperfunctions2::loadSRstructures(){
	string tmp_dir = graphobj->gettmp_dir();
	string idx_path = graphobj->getidx_path();
	string mat_path = graphobj->getmat_path();
	cout<<"helperfunctions2::loadSRstructures:: tmp_dir: "<<tmp_dir<<endl;
	cout<<"helperfunctions2::loadSRstructures:: idx_path: "<<idx_path<<endl;
	cout<<"helperfunctions2::loadSRstructures:: mat_path: "<<mat_path<<endl;
	cout<<"helperfunctions2::loadSRstructures:: vertex_count: "<<graphobj->get_num_vertices()<<endl;
	
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
void helperfunctions2::finishSR(){
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
	cout<<">>> helperfunctions2::finishSR:: active vertices after iteration: "<<active_cnt<<endl;
	vertex_values->NextIteration();		
	delete _sr;
}
#endif 








