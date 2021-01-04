#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include "../acts/acts/acts.h"
#include "../src/stats/stats.h"
#include "../src/utility/utility.h"
#include "../include/common.h"
#include "swkernel.h"
using namespace std;

swkernel::swkernel(stats * _statsobj){
	utilityobj = new utility();
	statsobj = _statsobj;
	#ifdef SW 
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs[i] = new acts(); }
	#endif 
}
swkernel::~swkernel(){} 

#ifdef SW
void swkernel::launchkernel(uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int flag){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef MULTIACTSINSTANCES
	kernelobjs[0]->topkernel((uint512_dt **)kvsourcedram);
	#else 
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		kernelobjs[i]->topkernel((uint512_dt *)kvsourcedram[i]);
		// exit(EXIT_SUCCESS); 
		// break;
	}
	#endif 
	
	#ifdef _DEBUGMODE_TIMERS3
	long double kerneltimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(kerneltimeelapsed_ms);
	#endif
	return;
}

void swkernel::finishOCL(){
	return;
}
#endif 












