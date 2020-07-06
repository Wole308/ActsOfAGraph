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
		,keyvalue_t * kvdramB
		,keyvalue_t * kvdramC
		,keyvalue_t * kvdramD
		,vertex_t kvbatchszA
		,vertex_t kvbatchszB
		,vertex_t kvbatchszC
		,vertex_t kvbatchszD
        );

private:
	uint512_dt * _kvdramA;
	uint512_dt * _kvdramB;
	uint512_dt * _kvdramC;
	uint512_dt * _kvdramD;
};
#endif








