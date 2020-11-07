#ifndef SRKERNEL_H
#define SRKERNEL_H
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <map>
#include <vector>
#include <iostream>
// #include "sortreduce.h"
// #include "types.h"
#include "../../include/common.h"

class sr {
public:
	sr();
	~sr();
	
	#ifdef GRAFBOOST_SETUP
	void srtopkernel(SortReduce<uint64_t,uint32_t>* _sr, keyvalue_t * kvdram, vertex_t size);
	#endif 
private:
	uint512_dt * _kvdramA;
};
#endif








