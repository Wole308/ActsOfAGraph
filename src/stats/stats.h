#ifndef STATS_H
#define STATS_H
#include "../../src/graphs/graph.h"
#include "../../include/common.h"

class stats {
public:
	stats(graph * graphobj);
	~stats();
	
	void appendkeyvaluecount(int col, unsigned int batchsize);
	void appendkerneltimeelapsed(long double timeelapsed_ms);
	void appendprocessedgestimeelapsed(long double timeelapsed_ms);
	void appendcummvstimeelapsed(long double timeelapsed_ms);
	void appendapplyvstimeelapsed(long double timeelapsed_ms);
	unsigned long gettotalkeyvaluesread();
	runsummary_t timingandsummary(unsigned int graph_iterationidx, long double totaltime_ms);
	
private:
	graph * graphobj;
	unsigned long * totalkeyvaluesread[1][1];
	long double kerneltime_ms;
	long double totalprocessedgestime_ms;
	long double totalcummvstime_ms;
	long double totalapplyvstime_ms;
};
#endif