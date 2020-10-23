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
	
	void loadgraphdata(unsigned int col, edge_t edgeoffset[NUMCPUTHREADS], edge_t edgesize[NUMCPUTHREADS], edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int vsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]);									
	void loadactsvertices(value_t * vertexdatabuffer, edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int vsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void loadactskeyvalues(keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t edgesize[NUMCPUTHREADS]);
	void loadactsmessages(uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t edgesize[NUMCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t beginkeyvalue[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int vsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void loadvariables(edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_t edgesize[NUMCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int voffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int vsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void loadvariables2(edge_t edgeoffset[NUMCPUTHREADS], edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t beginkeyvalue[NUMCPUTHREADS][NUMSUBCPUTHREADS]);

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
	unsigned int voffset[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int vsize[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int beginvid[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	keyvalue_t beginkeyvalue[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif








