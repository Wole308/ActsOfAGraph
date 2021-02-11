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
#include "../../examples/helperfunctions/postprocess.h"
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../kernels/kernel.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "evalparams.h"
using namespace std;

evalparams::evalparams(){}
evalparams::~evalparams(){} 

void evalparams::loadevalparams(unsigned int testid, uint512_vec_dt * kvstats){
	#if defined(INMEMORYGP) && defined(PR_ALGORITHM)
	// 'NB: first param 1 is standard, second param is: 1=procactvvs+partition, 2=procactvvs+partition+partition...'
	if(testid == 0){ 
		cout<<"loadevalparams pr:: general evaluation."<<endl;
	} else if(testid == 1){
		cout<<"loadevalparams pr:: custom evaluation. evaluating all latency."<<endl;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1 + (TREE_DEPTH);
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
	} else {
		cout<<"loadevalparams pr:: no evaluation (TEST "<<testid<<"). exiting..."<<endl;
		exit(EXIT_FAILURE);
	}
	#endif
	
	#if defined(INMEMORYGP) && defined(BFS_ALGORITHM)
	// 'NB: first param 1 is standard, second param is: 1=procactvvs, 2=procactvvs+partition, 3=procactvvs+partition+partition ...'
	if(testid == 0){
		cout<<"loadevalparams bfs:: general evaluation."<<endl;
	} else if(testid == 1){
		cout<<"loadevalparams bfs:: custom evaluation. evaluating all latency."<<endl;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1 + (1 + TREE_DEPTH);
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = ON;
	
	} else if(testid == 2){
		cout<<"loadevalparams bfs:: custom evaluation. evaluating procactvvs latency only."<<endl;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1 + (1 + TREE_DEPTH);
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key = OFF;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key = OFF;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = OFF;
		
	} else {
		cout<<"loadevalparams bfs:: no evaluation (TEST "<<testid<<"). exiting..."<<endl;
		exit(EXIT_FAILURE);
	}
	#endif
	
	#if defined(INMEMORYGP) && defined(SSSP_ALGORITHM)
	// 'NB: first param 1 is standard, second param is: 1=procactvvs, 2=procactvvs+partition, 3=procactvvs+partition+partition ...'
	if(testid == 0){
		cout<<"loadevalparams sssp:: general evaluation."<<endl;
	} else if(testid == 1){
		cout<<"loadevalparams sssp:: custom evaluation. evaluating all latency."<<endl;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1 + (1 + TREE_DEPTH);
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = ON;
	
	} else if(testid == 2){
		cout<<"loadevalparams sssp:: custom evaluation. evaluating procactvvs latency only."<<endl;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1 + (1 + TREE_DEPTH);
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key = ON;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key = OFF;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key = OFF;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = OFF;
		
	} else {
		cout<<"loadevalparams sssp:: no evaluation (TEST "<<testid<<"). exiting..."<<endl;
		exit(EXIT_FAILURE);
	}
	#endif
	return;
}






