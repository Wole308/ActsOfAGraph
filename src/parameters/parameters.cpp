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
	if(groupid == LOWINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION_POW; }
	else if(groupid == HIGHINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION2_POW; }
	else{ return KVDATA_RANGE_PERSSDPARTITION_POW; }
}
unsigned int parameters::GET_KVDATA_RANGE_PERSSDPARTITION(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION; }
	else if(groupid == HIGHINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION2; }
	else{ return KVDATA_RANGE_PERSSDPARTITION; }
}
unsigned int parameters::GET_KVDATA_RANGE_PERSSDPARTITION_KVS(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION_KVS; }
	else if(groupid == HIGHINDEGREESGROUPID){ return KVDATA_RANGE_PERSSDPARTITION2_KVS; }
	else{ return KVDATA_RANGE_PERSSDPARTITION_KVS; }
}

unsigned int parameters::GET_BATCH_RANGE_POW(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return BATCH_RANGE_POW; }
	else if(groupid == HIGHINDEGREESGROUPID){ return BATCH_RANGE2_POW; }
	else{ return BATCH_RANGE_POW; }
}
unsigned int parameters::GET_BATCH_RANGE(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return BATCH_RANGE; }
	else if(groupid == HIGHINDEGREESGROUPID){ return BATCH_RANGE2; }
	else{ return BATCH_RANGE; }
}
unsigned int parameters::GET_BATCH_RANGE_KVS(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return BATCH_RANGE_KVS; }
	else if(groupid == HIGHINDEGREESGROUPID){ return BATCH_RANGE2_KVS; }
	else{ return BATCH_RANGE_KVS; }
}

unsigned int parameters::GET_MYBATCH_RANGE(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return MYBATCH_RANGE; }
	else if(groupid == HIGHINDEGREESGROUPID){ return MYBATCH_RANGE2; }
	else{ return MYBATCH_RANGE; }
}

unsigned int parameters::GET_TREE_DEPTH(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return TREE_DEPTH; }
	else if(groupid == HIGHINDEGREESGROUPID){ return TREE_DEPTH2; }
	else{ return TREE_DEPTH; }
}

unsigned int parameters::GET_APPLYVERTEXBUFFERSZ(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return APPLYVERTEXBUFFERSZ; }
	else if(groupid == HIGHINDEGREESGROUPID){ return APPLYVERTEXBUFFERSZ2; }
	else{ return APPLYVERTEXBUFFERSZ; }
}
unsigned int parameters::GET_APPLYVERTEXBUFFERSZ_KVS(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return APPLYVERTEXBUFFERSZ_KVS; }
	else if(groupid == HIGHINDEGREESGROUPID){ return APPLYVERTEXBUFFERSZ2_KVS; }
	else{ return APPLYVERTEXBUFFERSZ_KVS; }
}

unsigned int parameters::GET_NUMLASTLEVELPARTITIONS(unsigned int groupid){
	if(groupid == LOWINDEGREESGROUPID){ return NUMLASTLEVELPARTITIONS; }
	else if(groupid == HIGHINDEGREESGROUPID){ return NUMLASTLEVELPARTITIONS2; }
	else{ return NUMLASTLEVELPARTITIONS; }
}
