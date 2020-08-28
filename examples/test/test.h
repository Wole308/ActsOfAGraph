#ifndef TEST_H
#define TEST_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/edgeprocess/edge_process.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../src/stats/stats.h"
#include "../../src/dataaccess/dataaccess.h"
#include "../../include/common.h"
#include "test.h"

class test {
public:
	test();
	~test();
	
	void run();
	void finish();
	
	void loadkvdram(keyvalue_t * batch[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	
private:
	utility * utilityobj;
	helperfunctions * helperfunctionsobj;

	uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	uint512_vec_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif








