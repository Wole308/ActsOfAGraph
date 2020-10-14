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
	
	// generate random ranks (test)
	/* for(unsigned int i = 0; i < graphobj->get_num_vertices(); i++){ rank_curr[i] = 8; rank_next[i] = 9; }
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			unsigned int rank_curr_offset = parametersobj[0]->GET_KVDATA_RANGE_PERSSDPARTITION(0) + (i * BATCH_RANGE);
			memcpy((value_t *)(&kvbuffer[0][0][i][j][BASEOFFSET_VERTICESDATA_KVS]), &rank_next[rank_curr_offset], BATCH_RANGE * sizeof(value_t));
		}
	} */
	
	// generate random idxs (test)
	/* for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			keyvalue_t * _kvbuffer = (keyvalue_t *)(&kvbuffer[0][0][i][j][BASEOFFSET_VERTICESDATA_KVS]);
			for(unsigned int k = 0; k < BATCH_RANGE; k++){
				// _kvbuffer[k].key = 0; _kvbuffer[k].value = 0;
				_kvbuffer[k].key = k; _kvbuffer[k].value = NAp;
			}
		}
	} */
	
	// generate random idxs (test)
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			uint512_vec_dt * _kvbuffer = (uint512_vec_dt *)(&kvbuffer[0][0][i][j][BASEOFFSET_VERTICESDATA_KVS]);
			
			for(unsigned int k = 0; k < BATCH_RANGE_KVS / 488; k++){
				for(unsigned int l = 0; l < 488; l++){
					_kvbuffer[k*488 + l].data[0].key = k*488*8 + (0 * 488) + l;
					_kvbuffer[k*488 + l].data[1].key = k*488*8 + (1 * 488) + l;
					_kvbuffer[k*488 + l].data[2].key = k*488*8 + (2 * 488) + l;
					_kvbuffer[k*488 + l].data[3].key = k*488*8 + (3 * 488) + l;
					_kvbuffer[k*488 + l].data[4].key = k*488*8 + (4 * 488) + l;
					_kvbuffer[k*488 + l].data[5].key = k*488*8 + (5 * 488) + l;
					_kvbuffer[k*488 + l].data[6].key = k*488*8 + (6 * 488) + l;
					_kvbuffer[k*488 + l].data[7].key = k*488*8 + (7 * 488) + l;
				}
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
					NAp, // unsigned int vsize,
					TREE_DEPTH, // unsigned int treedepth,
					0, // unsigned int GraphIter,
					PAGERANK, // unsigned int GraphAlgo,
					KVDATA_BATCHSIZE, // unsigned int runsize,
					BATCH_RANGE, // unsigned int batch_range,
					BATCH_RANGE_KVS, // unsigned int batch_range_kvs,
					BATCH_RANGE_POW, // unsigned int batch_range_pow,
					APPLYVERTEXBUFFERSZ, // unsigned int applyvertexbuffersz,
					APPLYVERTEXBUFFERSZ_KVS, // unsigned int applyvertexbuffersz_kvs
					NUMLASTLEVELPARTITIONS); // unsigned int numlastlevelpartitions
		}
	}
	
	// generate keyvalues [key:srcrank, value:dest]
	/* for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ 
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ 
			unsigned int rank_curr_offset = parametersobj[0]->GET_KVDATA_RANGE_PERSSDPARTITION(0) + (i * BATCH_RANGE);
			generatekeyvalues(edgesbuffer[0][0][i][j], &rank_curr[rank_curr_offset], (keyvalue_t *)(kvbuffer[0][0][i][j])); 
		}
	} */
	
	// generate keyvalues [key:srcrank, value:dest]
	/* for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			generatekeyvalues(edgesbuffer[0][0][i][j], (keyvalue_t *)(kvbuffer[0][0][i][j])); 
		}
	} */
	
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

/* void advance_op::generatekeyvalues(edge_type * edgesbuffer, value_t * rank_curr, keyvalue_t * kvbuffer){
	for(unsigned int i=0; i<KVDATA_BATCHSIZE; i++){
		kvbuffer[BASEOFFSET_KVDRAM + i].key = edgesbuffer[i].dstvid;
		kvbuffer[BASEOFFSET_KVDRAM + i].value = rank_curr[edgesbuffer[i].srcvid];
	}
	return;
}

void advance_op::generatekeyvalues(edge_type * edgesbuffer, keyvalue_t * kvbuffer){
	for(unsigned int i=0; i<KVDATA_BATCHSIZE; i++){
		kvbuffer[BASEOFFSET_KVDRAM + i].key = edgesbuffer[i].dstvid;
		kvbuffer[BASEOFFSET_KVDRAM + i].value = edgesbuffer[i].srcvid;
	}
	return;
} */









