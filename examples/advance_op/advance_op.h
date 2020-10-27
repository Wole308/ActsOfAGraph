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

typedef struct {
	unsigned int srcvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int srcvsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int destvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int beginvid[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	keyvalue_t beginkeyvalue[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int edgeoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int edgesize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int runsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]; 
} batchparams_t;

typedef struct {
	unsigned int numedgesretrieved[NUMSSDPARTITIONS];
	unsigned int totalnumedgesinfile[NUMSSDPARTITIONS];
} totalsparams_t;

class advance_op {
public:
	advance_op(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~advance_op();
	void finish();
	
	runsummary_t run();
	void WorkerThread(unsigned int superthreadidx, unsigned int col, hostglobalparams_t globalparams);
	
	void loadgraphdata(unsigned int col, value_t * vertexdatabuffer, edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], batchparams_t * batchparams, totalsparams_t * totalsparams);
	void loadgraphdata(unsigned int col, unsigned int threadid, unsigned int subthreadid, value_t * vertexdatabuffer, edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], batchparams_t * batchparams, totalsparams_t * totalsparams);			
	void loadsourcevertices(unsigned int col, value_t * vertexdatabuffer, edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], batchparams_t * batchparams);
	void loaddestvertices(unsigned int col, value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], size_t offset, batchparams_t * batchparams);
	void loadedges(unsigned int col, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], batchparams_t * batchparams);
	void loadmessages(unsigned int col, uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], batchparams_t * batchparams);
							
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
	#ifdef GRAFBOOST_SETUP 
	SortReduce<uint64_t,uint32_t>* sr;
	VertexValues<uint32_t,uint32_t>* vertex_values;
	#endif 
	
	// acts structures 
	uint512_vec_dt * kvbuffer[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif








