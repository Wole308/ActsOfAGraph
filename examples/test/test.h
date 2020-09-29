#ifndef TEST_H
#define TEST_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/edgeprocess/edge_process.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../src/stats/stats.h"
#include "../../src/dataaccess/dataaccess.h"
#include "../../include/common.h"
#include "test.h"

class test {
public:
	test(std::string binaryFile);
	~test();
	
	void run();
	void finish();
	
	void loadkvdram(keyvalue_t * batch[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	
private:
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	dataaccess * dataaccessobj;
	stats * statsobj;
	helperfunctions * helperfunctionsobj;

	uint512_vec_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	uint512_vec_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS];
	
	hostglobalparams_t globalparams;
};
#endif








