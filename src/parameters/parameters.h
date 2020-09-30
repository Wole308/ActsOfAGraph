#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <mutex>
#include <thread>
#include "../../acts/include/actscommon.h"
#include "../../include/common.h"
using namespace std;

class parameters {
public:
	parameters();
	~parameters();
	
	unsigned int GET_KVDATA_RANGE_PERSSDPARTITION_POW(unsigned int groupid);
	unsigned int GET_KVDATA_RANGE_PERSSDPARTITION(unsigned int groupid);
	unsigned int GET_KVDATA_RANGE_PERSSDPARTITION_KVS(unsigned int groupid);

	unsigned int GET_BATCH_RANGE_POW(unsigned int groupid);
	unsigned int GET_BATCH_RANGE(unsigned int groupid);
	unsigned int GET_BATCH_RANGE_KVS(unsigned int groupid);
	
	unsigned int GET_MYBATCH_RANGE(unsigned int groupid);

	unsigned int GET_TREE_DEPTH(unsigned int groupid);
	
	unsigned int GET_APPLYVERTEXBUFFERSZ(unsigned int groupid);
	unsigned int GET_APPLYVERTEXBUFFERSZ_KVS(unsigned int groupid);
	
	unsigned int GET_NUMLASTLEVELPARTITIONS(unsigned int groupid);
	
private:
};
#endif







