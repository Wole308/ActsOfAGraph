#ifndef APP_H
#define APP_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../examples/helperfunctions/setupkernel.h"
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
	void experiements(unsigned int evalid, unsigned int start, unsigned int size, unsigned int NumGraphIters, container_t * container, vector<value_t> & activevertices, globalparams_t globalparams);				
	
	void verifyresults(uint512_vec_dt * kvdram, globalparams_t globalparams);
	
private:
	graph * graphobj;
	utility * utilityobj;
	loadgraph * loadgraphobj;
	setupkernel * setupkernelobj;
	stats * statsobj;
	algorithm * algorithmobj;
	
	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	edge2_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS];
	uint512_vec_dt * vdram;
	
	std::string binaryFile[2];
};
#endif








