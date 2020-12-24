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
#include "../../acts/include/actscommon.h"
#include "../../include/common.h"
#include "parameters.h"
using namespace std;


parameters::parameters(){}
parameters::~parameters(){} 

unsigned int parameters::GET_KVDATA_RANGE_PERSSDPARTITION_POW(unsigned int groupid){
	return KVDATA_RANGE_PERSSDPARTITION_POW; 
}
unsigned int parameters::GET_KVDATA_RANGE_PERSSDPARTITION(unsigned int groupid){
	return KVDATA_RANGE_PERSSDPARTITION; 
}
unsigned int parameters::GET_KVDATA_RANGE_PERSSDPARTITION_KVS(unsigned int groupid){
	return KVDATA_RANGE_PERSSDPARTITION_KVS; 
}

unsigned int parameters::GET_BATCH_RANGE_POW(unsigned int groupid){
	return BATCH_RANGE_POW; 
}
unsigned int parameters::GET_BATCH_RANGE(unsigned int groupid){
	return BATCH_RANGE; 
}
unsigned int parameters::GET_BATCH_RANGE_KVS(unsigned int groupid){
	return BATCH_RANGE_KVS; 
}

unsigned int parameters::GET_TREE_DEPTH(unsigned int groupid){
	return TREE_DEPTH; 
}

unsigned int parameters::GET_APPLYVERTEXBUFFERSZ(unsigned int groupid){
	return APPLYVERTEXBUFFERSZ; 
}
unsigned int parameters::GET_APPLYVERTEXBUFFERSZ_KVS(unsigned int groupid){
	return APPLYVERTEXBUFFERSZ_KVS; 
}

unsigned int parameters::GET_NUMLASTLEVELPARTITIONS(unsigned int groupid){
	return NUMLASTLEVELPARTITIONS; 
}



