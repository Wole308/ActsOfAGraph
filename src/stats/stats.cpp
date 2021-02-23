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
#include <string> 
#include "../../include/common.h"
#include "stats.h"
using namespace std;

stats::stats(graph * _graphobj){
	graphobj = _graphobj;
	kerneltime_ms = 0;
	totalprocessedgestime_ms = 0;
	totalcummvstime_ms = 0;
	totalapplyvstime_ms = 0;
}
stats::~stats(){} 

void stats::appendkeyvaluecount(int col, unsigned int batchsize){
	graphobj->appendkeyvaluesread(col, batchsize);
}
void stats::appendkerneltimeelapsed(long double timeelapsed_ms){
	kerneltime_ms += timeelapsed_ms;
}
void stats::appendprocessedgestimeelapsed(long double timeelapsed_ms){
	totalprocessedgestime_ms += timeelapsed_ms;
}
void stats::appendcummvstimeelapsed(long double timeelapsed_ms){
	totalcummvstime_ms += timeelapsed_ms;
}
void stats::appendapplyvstimeelapsed(long double timeelapsed_ms){
	totalapplyvstime_ms += timeelapsed_ms;
}
unsigned long stats::gettotalkeyvaluesread(){
	unsigned long totalsize = 0;
	totalsize += graphobj->gettotalkeyvaluesread(0);
	return totalsize;
}
runsummary_t stats::timingandsummary(unsigned int graph_iterationidx, long double totaltime_ms){
	cout<<"=== STATS::TIMING AND SUMMARY RESULTS FOR ITERATION: "<<graph_iterationidx<<" === "<<endl;
	long double totaltime_overallexcludingOCLandSSDtransfers_ms = totaltime_ms;
	long double totaltime_SSDtransfers_ms = 0;
	long double totaltime_OCLtransfers_ms = 0;
	long double time_KernelPre_ms = totalprocessedgestime_ms;
	long double time_Kernel_ms = kerneltime_ms;
	long double time_KernelPost_ms = totalcummvstime_ms + totalapplyvstime_ms;
	long double totaltime_Kernel_ms = time_KernelPre_ms + time_Kernel_ms + time_KernelPost_ms;
	long double totaltime_OverallProcessing_ms = totaltime_SSDtransfers_ms + totaltime_OCLtransfers_ms + totaltime_Kernel_ms;

	cout<<">>> stats::summary Total number of kvReads in bank [0]: "<<graphobj->gettotalkeyvaluesread(0)<<endl; 
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total number of kvReads for all threads: "<<gettotalkeyvaluesread()<< RESET <<endl;
	
	cout <<">>> stats::summary Total time spent (SSD access): "<< totaltime_SSDtransfers_ms << " milliseconds"  <<endl;
	cout <<">>> stats::summary Total time spent (SSD access): "<< totaltime_SSDtransfers_ms / 1000 << " seconds"  <<endl;
	
	cout <<">>> stats::summary Total time spent (OCL data transfers): "<< totaltime_OCLtransfers_ms << " milliseconds"  <<endl;
	cout <<">>> stats::summary Total time spent (OCL data transfers): "<< totaltime_OCLtransfers_ms / 1000 << " seconds"  <<endl;
	
	cout <<">>> stats::summary Total time spent (Kernel Pre Process execution): "<< time_KernelPre_ms << " milliseconds"  <<endl;
	cout <<">>> stats::summary Total time spent (Kernel Pre Process execution): "<< time_KernelPre_ms / 1000 << " seconds"  <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (Kernel execution): "<< time_Kernel_ms << " milliseconds" << RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (Kernel execution): "<< time_Kernel_ms / 1000 << " seconds" << RESET <<endl;
	
	cout <<">>> stats::summary Total time spent (Kernel Post Process execution): "<< time_KernelPost_ms << " milliseconds"  <<endl;
	cout <<">>> stats::summary Total time spent (Kernel Post Process execution): "<< time_KernelPost_ms / 1000 << " seconds"  <<endl;
	
	cout <<">>> stats::summary Total time spent (Overall Kernel execution): "<< totaltime_Kernel_ms << " milliseconds"  <<endl;
	cout <<">>> stats::summary Total time spent (Overall Kernel execution): "<< totaltime_Kernel_ms / 1000 << " seconds"  <<endl;
	
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (Overall processing): "<<totaltime_ms<< " milliseconds" << RESET <<endl;
	cout<< TIMINGRESULTSCOLOR <<">>> stats::summary Total time spent (Overall processing): "<<totaltime_ms / 1000<< " seconds" << RESET <<endl;
	
	totaltime_overallexcludingOCLandSSDtransfers_ms -= (totaltime_SSDtransfers_ms + totaltime_OCLtransfers_ms);
	cout <<">>> stats::summary Total time spent (Overall processing excluding SSD & OCL data transfers): "<<totaltime_overallexcludingOCLandSSDtransfers_ms<< " milliseconds"  <<endl;
	cout <<">>> stats::summary Total time spent (Overall processing excluding SSD & OCL data transfers): "<<totaltime_overallexcludingOCLandSSDtransfers_ms / 1000<< " seconds"  <<endl;
	
	cout <<">>> stats::summary Throughput (Kernel processing): "<<(unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_Kernel_ms / 1000)<<" keyvalues per second" <<endl;
	cout <<">>> stats::summary Throughput (Kernel processing): "<<((unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_Kernel_ms / 1000)) / 1000000<<" million keyvalues per second" <<endl;
	cout <<">>> stats::summary Throughput (Kernel processing): "<<(((unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_Kernel_ms / 1000)) / 1000000) * sizeof(keyvalue_t)<<" MB/s" <<endl;
	
	cout <<">>> stats::summary Throughput (Overall processing): "<<(unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_OverallProcessing_ms / 1000)<<" keyvalues per second" <<endl;
	cout <<">>> stats::summary Throughput (Overall processing): "<<((unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_OverallProcessing_ms / 1000)) / 1000000<<" million keyvalues per second" <<endl;
	cout <<">>> stats::summary Throughput (Overall processing): "<<(((unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_OverallProcessing_ms / 1000)) / 1000000) * sizeof(keyvalue_t)<<" MB/s" <<endl;
	
	cout <<">>> stats::summary Throughput (Overall processing excluding SSD & OCL data transfers): "<<(unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_overallexcludingOCLandSSDtransfers_ms / 1000)<<" keyvalues per second" <<endl;
	cout <<">>> stats::summary Throughput (Overall processing excluding SSD & OCL data transfers): "<<((unsigned long)gettotalkeyvaluesread() / (long double)(totaltime_overallexcludingOCLandSSDtransfers_ms / 1000)) / 1000000<<" million keyvalues per second" <<endl;
	cout<<endl;
	
	graphobj->printdataset();
	
	runsummary_t runsummary; 
	runsummary.totalsize = gettotalkeyvaluesread(); 
	runsummary.totaltime_SSDtransfers_ms = totaltime_SSDtransfers_ms;
	runsummary.totaltime_OCLtransfers_ms = totaltime_OCLtransfers_ms;
	runsummary.totaltime_ms = totaltime_ms;
	runsummary.totaltime_overallexcludingOCLandSSDtransfers_ms = totaltime_overallexcludingOCLandSSDtransfers_ms;
	runsummary.graph_iterationidx = graph_iterationidx;
	return runsummary;
}

