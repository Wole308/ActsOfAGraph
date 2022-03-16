#ifndef APP_H
#define APP_H
#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <bits/stdc++.h> 
#include <iostream> 
#include <sys/stat.h> 
#include <sys/types.h>
#include <algorithm>
#include <thread>
#include <iostream>
#include <mutex>
#include <vector>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/loadgraph.h"
#include "../../examples/helperfunctions/loadedges_random.h"
#include "../../examples/helperfunctions/setupkernel.h"
#include "../../src/graphs/createundirectedgraph.h" // 
#include "../../kernels/swkernel.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"

class app {
public:
	app(unsigned int algorithmid, unsigned int datasetid, std::string _binaryFile1, std::string _binaryFile2);
	~app();
	void finish();
	
	runsummary_t run();
	runsummary_t run_hw();
	
	void verifyresults(uint512_vec_dt * kvdram, globalparams_t globalparams);
	void verifyresults_splitdstvtxs(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], globalparams_t globalparams);
	void verifyresults_splitdstvtxs(uint512_vec_dt * vbuffer, globalparams_t globalparams);
	
private:
	graph * graphobj;
	utility * utilityobj;
	loadgraph * loadgraphobj;
	loadedges_random * loadedgesobj;
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
	uint512_vec_dt * edges[NUMSUBCPUTHREADS];
	
	std::string binaryFile[2];
};
#endif








