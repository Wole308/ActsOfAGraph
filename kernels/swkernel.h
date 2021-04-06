#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include "../include/config_params.h"
#include "../include/common.h"
#include "../src/stats/stats.h"
#include "../src/graphs/graph.h"
#include "../acts/acts/acts.h"
#include "../acts/acts/actsproc.h"
#include "../acts/acts/actssync.h"
#include "../acts/acts_sw/acts_sw.h"
#include "../src/utility/utility.h"

class swkernel {
public:
	swkernel(graph * _graphobj, stats * _statsobj);
	~swkernel();
	
	#ifdef SW
	long double runapp(edge_type * edges[NUMSUBCPUTHREADS], edge_t * vptrs[NUMSUBCPUTHREADS], unsigned int * vdatas, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[NUMSUBCPUTHREADS][TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int numIters);				
	long double runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	#endif
private:
	utility * utilityobj;
	stats * statsobj;
	graph * graphobj;
	
	#ifdef SW
	acts * kernelobjs[NUMSUBCPUTHREADS];
	
	actsproc * kernelobjs_process[NUMSUBCPUTHREADS];
	actssync * kernelobjs_synchronize;
	
	acts_sw * actssw_obj;
	#endif
};
#endif







