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
,uint512_dt * kvsourcedramB
,uint512_dt * kvsourcedramC
,uint512_dt * kvsourcedramD
		,uint512_dt * kvdestdramA
		,uint512_dt * kvdestdramB
		,uint512_dt * kvdestdramC
		,uint512_dt * kvdestdramD
		,keyvalue_t * kvstatsA
		,keyvalue_t * kvstatsB
		,keyvalue_t * kvstatsC
		,keyvalue_t * kvstatsD
        ){
	#ifdef SW
	return enigmaobj.topkernel(
kvsourcedramA
,kvsourcedramB
,kvsourcedramC
,kvsourcedramD
		,kvdestdramA
		,kvdestdramB
		,kvdestdramC
		,kvdestdramD
		,kvstatsA
		,kvstatsB
		,kvstatsC
		,kvstatsD
        );
	#endif
	return;
}