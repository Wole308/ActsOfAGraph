#ifndef ADVANCE_OP_H
#define ADVANCE_OP_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/edgeprocess/edge_process.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../examples/helperfunctions/helperfunctions2.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "advance_op.h"

class advance_op {
public:
	advance_op(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~advance_op();
	void finish();
	
	runsummary_t run();
	void WorkerThread(int superthreadidx, int threadidxoffset, hostglobalparams_t globalparams);
	void WorkerThread2(hostglobalparams_t globalparams);
	
	void loadgraphdata(unsigned int col, edge_t edgeoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t edgesize[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int srcvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int srcvsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]);									
	void loadsourcevertices(value_t * vertexdatabuffer, edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int srcvsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void loaddestvertices(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], size_t offset);
	void loadedges(keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t edgesize[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void loadmessages(uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t edgesize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t beginkeyvalue[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int srcvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int srcvsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int destvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void loadvariables(unsigned int col, edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t edgesize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int srcvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int srcvsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int destvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS],
									edge_t edgeoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t beginkeyvalue[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
									
private:
	graph * graphobj;
	parameters * parametersobj[NUMSUPERCPUTHREADS];
	utility * utilityobj[NUMSUPERCPUTHREADS];
	edge_process * edgeprocessobj[NUMSUPERCPUTHREADS];
	helperfunctions2 * helperfunctionsobj[NUMSUPERCPUTHREADS];
	stats * statsobj;
	hostglobalparams_t globalparams;
	
	// graph structures
	vertexprop_t * vertexpropertybuffer;
	value_t * vertexdatabuffer;
	value_t * rank_curr;
	value_t * rank_next;
	unsigned int * degrees;
	edge_type * edgesbuffer[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	edge_t * vertexptrs[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	
	// acts structures 
	uint512_vec_dt * kvbuffer[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	
	// other structures 
	unsigned int srcvoffset[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int srcvsize[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int destvoffset[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int beginvid[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	keyvalue_t beginkeyvalue[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif








