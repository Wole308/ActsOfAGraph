#ifndef ADVANCE_OP_H
#define ADVANCE_OP_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../examples/helperfunctions/helperfunctions2.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "advance_op.h"

class advance_op {
public:
	advance_op(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~advance_op();
	void finish();
	
	runsummary_t run();
	void WorkerThread(unsigned int superthreadidx, unsigned int col, hostglobalparams_t globalparams, vector<vertex_t> &nextactivevertices, container_t * container);
	
	void loadgraphdata(unsigned int col, graph * graphobj, value_t * vertexdatabuffer, container_t * container);
	void loadgraphdata(unsigned int col, unsigned int threadid, unsigned int subthreadid, graph * graphobj, value_t * vertexdatabuffer, container_t * container);			
	void loadsourcevertices(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);
	void loaddestvertices(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vertex_t offset, vertex_t size);
	void loadedges(keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);
	void loadmessages(uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);
							
private:
	graph * graphobj;
	parameters * parametersobj[NUMSUPERCPUTHREADS];
	utility * utilityobj[NUMSUPERCPUTHREADS];
	helperfunctions2 * helperfunctionsobj[NUMSUPERCPUTHREADS];
	stats * statsobj;
	hostglobalparams_t globalparams;
	
	vertexprop_t * vertexpropertybuffer;
	value_t * vertexdatabuffer;
	uint512_vec_dt * kvbuffer[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif








