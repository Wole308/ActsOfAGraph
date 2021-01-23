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
	
	void verify(vector<vertex_t> &activevertices);
	void verifykvLOP(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * stats[NUMSUBCPUTHREADS], unsigned int CLOP, unsigned int * edges4_count, unsigned int * edgesdstv4_sum);					
	void verifyvertexdata(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS]);
	void verifyactvvsdata(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS]);
	void verifykernelreturnvalues(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS]);
	
private:
	graph * graphobj;
	parameters * parametersobj;
	utility * utilityobj;
	postprocess * postprocessobj;
	loadgraph * loadgraphobj;
	// compactgraph * compactgraphobj;
	setupkernel * setupkernelobj;
	stats * statsobj;
	algorithm * algorithmobj;
	hostglobalparams_t globalparams;
	
	edge_t * vertexptrbuffer;
	value_t * vertexdatabuffer;
	value_t * tempvertexdatabuffer;
	edge2_type * edgedatabuffer;
	uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS];
	
	// edge_t * packedvertexptrbuffer;
	// uuint64_dt * packededgedatabuffer;
};
#endif








