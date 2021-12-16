#ifndef SWKERNEL_H
#define SWKERNEL_H
#include <mutex>
#include <thread>
#include "../include/config_params.h"
#include "../include/common.h"
#include "../src/stats/stats.h"
#include "../src/graphs/graph.h"
// #include "../acts/acts/acts.h"
#include "../acts/acts/actsproc.h"
#include "../acts/acts/actssync.h"
#include "../acts/acts/actsmerge.h"
#include "../src/utility/utility.h"
#include "../src/algorithm/algorithm.h"

class swkernel {
public:
	swkernel(graph * _graphobj, algorithm * _algorithmobj, stats * _statsobj);
	~swkernel();
	
	#ifdef SW
	long double runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], long double timeelapsed_totals[128][8], unsigned int numValidIters);						
	
	void run3(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	void run12(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	void run22(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	void run24(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	void run25(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	void run32(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	
	void run_sync(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	#endif
	
	void verifyresults_hw(uint512_vec_dt * kvdram, unsigned int _BASEOFFSETKVS_VERTICESDATA);
private:
	utility * utilityobj;
	stats * statsobj;
	graph * graphobj;
	algorithm * algorithmobj;
	
	#ifdef SW
	actsproc * kernelobjs_process[NUMSUBCPUTHREADS];
	actssync * kernelobjs_synchronize;
	// actsmerge * kernelobjs_synchronize;
	#endif
};
#endif







