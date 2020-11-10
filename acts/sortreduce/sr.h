#ifndef SRKERNEL_H
#define SRKERNEL_H
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <map>
#include <vector>
#include <iostream>
#include "sortreduce.h"
#include "types.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
// #include "../include/actslw_common.h"
#include "../../src/utility/utility.h"
#include "../../acts/actsutility/actsutility.h"

class sr {
public:
	sr();
	~sr();
	
	unsigned int procedgesfunc(unsigned int Uprop, unsigned int edgeweight);
	void processedges(keyvalue_t * vertexptrs, value_t * vertexdatas, keyvalue_t * edges, keyvalue_t * keyvalues, unsigned int srcvoffset, unsigned int srcvsize, unsigned int edgessize);					
	
	#ifdef GRAFBOOST_SETUP
	void srtopkernel(SortReduce<uint64_t,uint32_t>* _sr, uint512_dt * kvdram);
	#endif 
private:
	actsutility * actsutilityobj;
};
#endif








