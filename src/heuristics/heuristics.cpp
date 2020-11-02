#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include "heuristics.h"
using namespace std;

// #define SWITCHTTHRESHOLD (KVDATA_RANGE / 1) // ALL PARTIAL
// #define SWITCHTTHRESHOLD (KVDATA_RANGE / 16)
// #define SWITCHTTHRESHOLD (KVDATA_RANGE / 64) // CORRECTONE
#define SWITCHTTHRESHOLD (KVDATA_RANGE / 256) // USEDONE //
// #define SWITCHTTHRESHOLD 0 // ALL FULL

heuristics::heuristics(){
	heuristicstype.type = 0;
	heuristicstype.numvertexbanks = getdefaultnumvertexbanks();
	heuristicstype.numedgebanks = getdefaultnumedgebanks();
}
heuristics::~heuristics(){} 

heuristicstype_t heuristics::gettype(unsigned int algorithmID, unsigned int graph_iterationidx, size_t num_active_vertices){	
	unsigned int type = 0;
	unsigned int numvertexbanks = 0;
	unsigned int numedgebanks = 0;
	
	#ifdef ACTGRAPH_SETUP
	#ifdef PR_ALGORITHM
	type = STREAMING_ACCESS_TYPE;
	numvertexbanks = NUMSSDPARTITIONS;
	numedgebanks = NUMSSDPARTITIONS;
	#else
	if(num_active_vertices > SWITCHTTHRESHOLD){
		type = STREAMING_ACCESS_TYPE;
		numvertexbanks = NUMSSDPARTITIONS;
		numedgebanks = NUMSSDPARTITIONS;
	} else {
		type = PARTIAL_ACCESS_TYPE;
		numvertexbanks = NUMSSDPARTITIONS;
		numedgebanks = 1;
	}
	#endif	
	#endif
	
	if(heuristicstype.type == type){ heuristicstype.changed = false; } 
	else { heuristicstype.changed = true; }
	
	heuristicstype.type = type;
	heuristicstype.numvertexbanks = numvertexbanks;
	heuristicstype.numedgebanks = numedgebanks;
	return heuristicstype;
}

unsigned int heuristics::getdefaultnumvertexbanks(){
	return 1;
}
unsigned int heuristics::getdefaultnumedgebanks(){	
	return NUMSSDPARTITIONS;
	/* #ifdef ACTGRAPH_SETUP
	#if defined(PR_ALGORITHM)
	return NUMSSDPARTITIONS;
	#else 
	return 1;
	#endif
	#endif 
	#ifdef GRAFBOOST_SETUP
	#if defined(PR_ALGORITHM)
	return 1;
	#else 
	return 1;
	#endif
	#endif */
}



