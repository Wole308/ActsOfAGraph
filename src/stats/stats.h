#ifndef STATS_H
#define STATS_H
#include "../../src/graphs/graph.h"
#include "../../include/common.h"

class stats {
public:
	stats(graph * graphobj);
	~stats();
	
	void appendkeyvaluecount(int bank, int col, unsigned int batchsize);
	void appendkerneltimeelapsed(long double timeelapsed_ms);
	unsigned long gettotalkeyvaluesread();
	runsummary_t timingandsummary(unsigned int graph_iterationidx, long double totaltime_ms);
	
private:
	graph * graphobj;
	unsigned long * totalkeyvaluesread[MAXNUMSSDPARTITIONS][MAXNUMSSDPARTITIONS];
	long double totalkerneltime_ms;
};
#endif