#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/edgeprocess/edge_process.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../examples/helperfunctions/helperfunctions2.h"
#include "../../src/stats/stats.h"
#include "../../src/dataaccess/dataaccess.h"
#include "../../include/common.h"
#include "advance_op.h"
using namespace std;

#define BLOCKH 512 // 488

/** auto advance_op = [rank_curr, rank_next] __host__ __device__(
					const VertexT &src, VertexT &dest,
					const SizeT &edge_id, const VertexT &input_item,
					const SizeT &input_pos, SizeT &output_pos) -> bool {
	ValueT add_value = rank_curr[src];
	if (isfinite(add_value)) {
		atomicAdd(rank_next + dest, add_value);
	}
	return true;
}; */

advance_op::advance_op(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile){
	algorithm * thisalgorithmobj = new algorithm();
	heuristics * heuristicsobj = new heuristics();
	graphobj = new graph(thisalgorithmobj, datasetid, heuristicsobj->getdefaultnumvertexbanks(), heuristicsobj->getdefaultnumedgebanks());
	statsobj = new stats(graphobj);
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ parametersobj[i] = new parameters(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ edgeprocessobj[i] = new edge_process(graphobj, statsobj); }
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i] = new helperfunctions2(graphobj, statsobj); }
	
	rank_curr = new value_t[graphobj->get_num_vertices()];
	rank_next = new value_t[graphobj->get_num_vertices()];
	degrees = new value_t[graphobj->get_num_vertices()];
	
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){
		#ifdef FPGA_IMPL
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[i][flag][j][k] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }}}
		#else
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ kvbuffer[i][flag][j][k] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}}
		#endif
		for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int j=0; j<NUMCPUTHREADS; j++){ for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){ edgesbuffer[i][flag][j][k] = new edge_type[KVDATA_BATCHSIZE]; }}}
	}
	
	#ifdef FPGA_IMPL
	for(unsigned int i=0; i<NUMSUPERCPUTHREADS; i++){ helperfunctionsobj[i]->loadOCLstructures(binaryFile, (uint512_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvbuffer[i]); }
	#endif
}
advance_op::~advance_op(){
	cout<<"advance_op::~advance_op:: finish destroying memory structures... "<<endl;
	delete [] edgesbuffer;
	delete [] kvbuffer;
}

void advance_op::run(){
	cout<<"advance_op::run:: advance_op algorithm started. "<<endl;
	
	// load edges from file
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ 
			graphobj->loadedgesfromfile(0, 0, 0, edgesbuffer[0][0][i][j], 0, KVDATA_BATCHSIZE); 
		}
	}
	
	// generate random idxs (test)
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			keyvalue_t * _kvbuffer = (keyvalue_t *)(&kvbuffer[0][0][i][j][BASEOFFSET_VERTICESDATA_KVS]);
			for(unsigned int k = 0; k < BATCH_RANGE; k++){
					_kvbuffer[k].key = k; 
					_kvbuffer[k].value = 10000000 + k;
			}
		}
	}
	
	// >>>
	// >>> kernel computation of advance
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	// create messages
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ 
			helperfunctionsobj[0]->createmessages(
					kvbuffer[0][0][i][j],
					0, // unsigned int voffset,
					BATCH_RANGE, //NAp, // unsigned int vsize,
					TREE_DEPTH, // unsigned int treedepth,
					0, // unsigned int GraphIter,
					PAGERANK, // unsigned int GraphAlgo,
					KVDATA_BATCHSIZE, // unsigned int runsize,
					BATCH_RANGE, // unsigned int batch_range,
					BATCH_RANGE_POW, // unsigned int batch_range_pow,
					APPLYVERTEXBUFFERSZ, // unsigned int applyvertexbuffersz,
					NUMLASTLEVELPARTITIONS); // unsigned int numlastlevelpartitions
		}
	}
	
	// generate keyvalues [key:src, value:dest]
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			keyvalue_t * _kvbuffer = (keyvalue_t *)(&kvbuffer[0][0][i][j][BASEOFFSET_KVDRAM_KVS]);
			for(unsigned int k=0; k<KVDATA_BATCHSIZE; k++){
				_kvbuffer[k].key = edgesbuffer[0][0][i][j][k].dstvid;
				_kvbuffer[k].value = edgesbuffer[0][0][i][j][k].srcvid;
			}
		}
	}
		
	// acts it!
	helperfunctionsobj[0]->launchkernel((uint512_dt* (*)[NUMSUBCPUTHREADS])kvbuffer[0][0], 0);
	utilityobj[0]->stopTIME("advance_op:: finished. Time Elapsed: ", begintime, NAp);
	// >>> kernel computation of advance
	// >>>
	return;
}
void advance_op::finish(){
	#if (defined(FPGA_IMPL) && defined(PR_ALGORITHM))
	helperfunctionsobj[0]->finishOCL();
	#endif
}









