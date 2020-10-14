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
#include "../../src/dataaccess/dataaccess.h"
#include "../../include/common.h"
#include "advance_op.h"

class advance_op {
public:
	advance_op(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~advance_op();
	
	void run();
	void finish();
	
	void generatekeyvalues(edge_type * edgesbuffer, value_t * rank_curr, keyvalue_t * kvbuffer);
	void generatekeyvalues(edge_type * edgesbuffer, keyvalue_t * kvbuffer);
	
private:
	graph * graphobj;
	parameters * parametersobj[NUMSUPERCPUTHREADS];
	utility * utilityobj[NUMSUPERCPUTHREADS];
	edge_process * edgeprocessobj[NUMSUPERCPUTHREADS];
	helperfunctions2 * helperfunctionsobj[NUMSUPERCPUTHREADS];
	stats * statsobj;

	value_t * rank_curr;
	value_t * rank_next;
	unsigned int * degrees;
	edge_type * edgesbuffer[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	uint512_vec_dt * kvbuffer[NUMSUPERCPUTHREADS][NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	
	hostglobalparams_t globalparams;
};
#endif








