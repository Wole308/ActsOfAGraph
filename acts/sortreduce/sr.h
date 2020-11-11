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
#include "../../src/utility/utility.h"
#include "../../acts/actsutility/actsutility.h"

class sr {
public:
	sr();
	~sr();
	
	#ifdef GRAFBOOST_SETUP
	void srtopkernel(SortReduce<uint64_t,uint32_t>* _sr, uint512_dt * kvdram);
	#endif 
private:
	actsutility * actsutilityobj;
};
#endif








