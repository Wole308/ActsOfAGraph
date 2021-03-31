#ifndef APP_H
#define APP_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../examples/helperfunctions/loadgraph_sw.h"
#include "../../examples/helperfunctions/setupkernel.h"
#include "../../src/graphs/createundirectedgraph.h" // 
#include "../../kernels/swkernel.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "app.h"

class app {
public:
	app(unsigned int algorithmid, unsigned int datasetid, std::string _binaryFile1, std::string _binaryFile2);
	~app();
	void finish();
	
	runsummary_t run();
	runsummary_t run_sw();
	runsummary_t run_hw();
	
	void verifyresults_sw(value_t * vdatas);
	void verifyresults_hw(uint512_vec_dt * kvdram, globalparams_t globalparams);
	
private:
	graph * graphobj;
	utility * utilityobj;
	loadgraph * loadgraphobj;
	loadgraph_sw * loadgraphswobj;
	setupkernel * setupkernelobj;
	swkernel * swkernelobj;
	stats * statsobj;
	algorithm * algorithmobj;
	
	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	edge2_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS];
	uint512_vec_dt * vdram;
	
	edge_t * vptrs[NUMSUBCPUTHREADS];
	edge_type * edges[NUMSUBCPUTHREADS];
	vector<keyvalue_t> kvdram[NUMSUBCPUTHREADS][TOTALNUMPARTITIONS];
	
	std::string binaryFile[2];
};
#endif








