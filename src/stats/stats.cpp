#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <mutex>
#include "../../include/common.h"
#include "stats.h"
using namespace std;

stats::stats(graph * _graphobj){
	graphobj = _graphobj;
}
stats::~stats(){} 

void stats::appendkeyvaluecount(int bank, int col, unsigned int batchsize){
	graphobj->appendkeyvaluesread(bank, col, batchsize);
}
unsigned long stats::gettotalkeyvaluesread(){
	unsigned long totalsize = 0;
	for (int i = 0; i < MAXNUMSSDPARTITIONS; i++){ for (int j = 0; j < MAXNUMSSDPARTITIONS; j++){ totalsize += graphobj->gettotalkeyvaluesread(i, j); }}
	return totalsize;
}
runsummary_t stats::timingandsummary(unsigned int graph_iterationidx, long double totaltime_ms){
	cout<<"=== STATS::TIMING AND SUMMARY RESULTS FOR ITERATION: "<<graph_iterationidx<<" === "<<endl;
	long double totaltime_overallexcludingOCLandSSDtransfers_ms = totaltime_ms;
	long double totaltime_SSDtransfers_ms = 0;
	long double totaltime_OCLtransfers_ms = 0;
	
	for (int i = 0; i < NUMSUPERCPUTHREADS; i++){ for (int j = 0; j < NUMCPUTHREADS; j++){ cout<<">>> stats::summary Total number of kvReads for thread ["<<i<<"]["<<j<<"]: "<<graphobj->gettotalkeyvaluesread(i, j)<<endl; }}
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total number of kvReads for all threads: "<<gettotalkeyvaluesread()<< RESET <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (SSD access): "<< totaltime_SSDtransfers_ms << " milliseconds" << RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (SSD access): "<< totaltime_SSDtransfers_ms / 1000 << " seconds" << RESET <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (OCL data transfers): "<< totaltime_OCLtransfers_ms << " milliseconds" << RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (OCL data transfers): "<< totaltime_OCLtransfers_ms / 1000 << " seconds" << RESET <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (Overall processing): "<<totaltime_ms<< " milliseconds" << RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (Overall processing): "<<totaltime_ms / 1000<< " seconds" << RESET <<endl;
	
	totaltime_overallexcludingOCLandSSDtransfers_ms -= (totaltime_SSDtransfers_ms + totaltime_OCLtransfers_ms);
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (Overall processing excluding SSD & OCL data transfers): "<<totaltime_overallexcludingOCLandSSDtransfers_ms<< " milliseconds" << RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (Overall processing excluding SSD & OCL data transfers): "<<totaltime_overallexcludingOCLandSSDtransfers_ms / 1000<< " seconds" << RESET <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Throughput (Overall processing): "<<(unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_ms / 1000)<<" keyvalues per second"<< RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Throughput (Overall processing): "<<((unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_ms / 1000)) / 1000000<<" million keyvalues per second"<< RESET <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Throughput (Overall processing excluding SSD & OCL data transfers): "<<(unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_overallexcludingOCLandSSDtransfers_ms / 1000)<<" keyvalues per second"<< RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Throughput (Overall processing excluding SSD & OCL data transfers): "<<((unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_overallexcludingOCLandSSDtransfers_ms / 1000)) / 1000000<<" million keyvalues per second"<< RESET <<endl;
	cout<<endl;
	
	runsummary_t runsummary; 
	runsummary.totalsize = gettotalkeyvaluesread(); 
	runsummary.totaltime_SSDtransfers_ms = totaltime_SSDtransfers_ms;
	runsummary.totaltime_OCLtransfers_ms = totaltime_OCLtransfers_ms;
	runsummary.totaltime_ms = totaltime_ms;
	runsummary.totaltime_overallexcludingOCLandSSDtransfers_ms = totaltime_overallexcludingOCLandSSDtransfers_ms;
	runsummary.graph_iterationidx = graph_iterationidx;
	return runsummary;
}

