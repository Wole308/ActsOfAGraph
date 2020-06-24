#ifndef SRKERNELPROCESS_H
#define SRKERNELPROCESS_H
#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <ctime>
#include <map>
#include <vector>

#include "sortreduce.h"
#include "types.h"
#include "../src/common.h"

class srkernelprocess {
public:
	srkernelprocess();
	~srkernelprocess();
	
	void srtopkernel(
		SortReduce<uint64_t,uint32_t>* sr
		,keyvalue_t * kvdramA
		,vertex_t kvbatchszA
        );

private:
	uint512_dt * _kvdramA;
};
#endif








