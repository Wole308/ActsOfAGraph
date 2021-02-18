#ifndef SSSP_H
#define SSSP_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../examples/helperfunctions/setupkernel.h"
#include "../../examples/helperfunctions/postprocess.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "sssp.h"

class sssp {
public:
	sssp(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~sssp();
	void finish();
	
	runsummary_t run();
	void experiements(unsigned int evalid, unsigned int start, unsigned int size, unsigned int NumGraphIters, container_t * container, vector<value_t> & activevertices);
	
	void verifyresults(uint512_vec_dt * kvdram);
	
private:
	graph * graphobj;
	parameters * parametersobj;
	utility * utilityobj;
	postprocess * postprocessobj;
	loadgraph * loadgraphobj;
	setupkernel * setupkernelobj;
	stats * statsobj;
	algorithm * algorithmobj;
	hostglobalparams_t globalparams;
	
	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	edge2_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS];
	uint512_vec_dt * vdram;
};
#endif








