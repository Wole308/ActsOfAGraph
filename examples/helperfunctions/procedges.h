#ifndef PEDGES_H
#define PEDGES_H
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
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../../acts/include/actscommon.h"
#include "../../src/utility/utility.h"
#include "../../acts/actsutility/actsutility.h"
using namespace std;

class procedges {
public:
	procedges(stats * _statsobj);
	~procedges();
	
	unsigned int processedgefunc(value_t Uprop, unsigned int edgeweight, unsigned int voutdegree, unsigned int GraphIter, unsigned int GraphAlgo);
	void processedges(edge_t * vertexptrs, value_t * verticesdata, keyvalue_t * edges, keyvalue_t * keyvalues, unsigned int srcvoffset, unsigned int srcvsize, unsigned int edgessize, unsigned int GraphIter, unsigned int GraphAlgo);					
	
	void start(uint512_vec_dt * kvdram[NUMSUBCPUTHREADS], edge_t * vertexptrs, value_t * verticesdata, keyvalue_t * edges[NUMSUBCPUTHREADS]);
	void start(uint512_vec_dt * kvdram[NUMSUBCPUTHREADS], edge_t * vertexptrs[NUMSUBCPUTHREADS], value_t * verticesdata[NUMSUBCPUTHREADS], keyvalue_t * edges[NUMSUBCPUTHREADS]);
	
	void WorkerThread(uint512_vec_dt * kvdram, edge_t * vertexptrs, value_t * verticesdata, keyvalue_t * edges);
private:
	utility * utilityobj;
	actsutility * actsutilityobj;
	stats * statsobj;
	std::thread mythread[NUMSUBCPUTHREADS];
};
#endif 







