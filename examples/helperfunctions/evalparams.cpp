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

void evalparams::loadevalparams(unsigned int testid, uint512_vec_dt * kvbuffer){
	if(testid == 0){
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"loadevalparams bfs:: general evaluation."<<endl;
		#endif 
	} else if(testid == 1){
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"loadevalparams bfs:: custom evaluation. evaluating all latency."<<endl;
		#endif 
		kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key = ON; 
		kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key = ON;  
		kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key = ON;  
	
	} else if(testid == 2){
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"loadevalparams bfs:: custom evaluation. evaluating procactvvs latency only."<<endl;
		#endif 
		kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key = ON; 
		kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key = OFF;  
		kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key = OFF;  
		
	} else {
		cout<<"loadevalparams bfs:: no evaluation (TEST "<<testid<<"). exiting..."<<endl;
		exit(EXIT_FAILURE);
	}
	return;
}






