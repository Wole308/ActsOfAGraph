#include "../src/common.h"
#include "../kernels/enigma.h"
#include "../kernels/kernelprocess.h"
#ifndef FPGA_IMPL
#include <chrono>
#endif 
using namespace std;

kernelprocess::kernelprocess(){ }
kernelprocess::~kernelprocess(){}

void kernelprocess::topkernel(
uint512_dt * kvsourcedramA
		,uint512_dt * kvdestdramA
		,keyvalue_t * kvstatsA
        ){
	
	return enigmaobj.topkernel(
kvsourcedramA
		,kvdestdramA
		,kvstatsA
        );
	return;
}