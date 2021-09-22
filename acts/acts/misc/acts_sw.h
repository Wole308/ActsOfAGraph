#ifndef ACTS_SW_H
#define ACTS_SW_H
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <mutex>
#include <thread>

// #include <chrono>
// #include <stdlib.h>
// #include <ctime>
// #include <map>
// #include <stdio.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <vector>
// #include <mutex>

#include "../../src/graphs/graph.h"
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#include "../../acts/actsutility/actsutility.h"
using namespace std;

#define MULTITHREADEDIMPL
// #define VLOCK
// #define SINGLEVBUFFER

#define NUMTHREADS_SYNC 8

class acts_sw {
public:
	acts_sw();
	~acts_sw();
	
	void processit(graph * graphobj, edge_type * edges, edge_t * vptrs, value_t * vprops, value_t * tempvprops, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int currentLOP, unsigned int GraphAlgo, unsigned int GraphIter, unsigned int * sourcestatsmarker, unsigned int * deststatsmarker);					
	
	void partitionit_reduceit(graph * graphobj, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], value_t * vprops, value_t * tempvprops, vector<vertex_t> &actvvs_nextit, unsigned int currentLOP, unsigned int GraphAlgo, unsigned int * sourcestatsmarker, unsigned int * deststatsmarker);
	
	void topkernelproc(graph * graphobj, edge_type * edges, edge_t * vptrs, value_t * vprops, value_t * tempvprops, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int GraphIter);
	
	#ifdef SINGLEVBUFFER
	void start(graph * graphobj, edge_type * edges[NUMSUBCPUTHREADS], edge_t * vptrs[NUMSUBCPUTHREADS], value_t * vprops, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[NUMSUBCPUTHREADS][TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int numIters);
	void topkernelsync(graph * graphobj, value_t * vprops, value_t * tempvprops, vector<vertex_t> &actvvs_nextit);
	#endif 
	
	#ifndef SINGLEVBUFFER
	void start(graph * graphobj, edge_type * edges[NUMSUBCPUTHREADS], edge_t * vptrs[NUMSUBCPUTHREADS], value_t * vprops, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[NUMSUBCPUTHREADS][TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int numIters);
	void topkernelsync(graph * graphobj, value_t * vprops, value_t * tempvprops[NUMSUBCPUTHREADS], vector<vertex_t> &actvvs_nextit);
	void topkernelsync_thread(unsigned int offset, unsigned int size, graph * graphobj, value_t * vprops, value_t * tempvprops[NUMSUBCPUTHREADS], vector<vertex_t> &actvvs_nextit);
	#endif 
	
private:
	actsutility * actsutilityobj;
	
	std::thread threadobj[NUMSUBCPUTHREADS];
};
#endif 






