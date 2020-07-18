#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <iostream>
#include <thread>
#include "../src/common.h"
// #include "../kernels/enigma.h"
#include "../kernels/acts.h"
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
	#ifdef SW
	return kernelobj.topkernel(
kvsourcedramA
		,kvdestdramA
		,kvstatsA
        );
	#endif
}
void kernelprocess::topkernelMW(uint512_dt ** kvsourcedram, uint512_dt ** kvdestdram, keyvalue_t ** kvstats){
	#ifdef SW
	#ifdef TESTKERNEL
	topkernel(kvsourcedram[0], kvdestdram[0], kvstats[0]);
	#else 
	for (int i = 0; i < NUMDRAMBANKS; i++){ acts_thread[i] = std::thread(&kernelprocess::topkernel, this, kvsourcedram[i], kvdestdram[i], kvstats[i]); }
	for (int i = 0; i < NUMDRAMBANKS; i++){ acts_thread[i].join(); }
	// for (int i = 0; i < NUMDRAMBANKS; i++){ topkernel(kvsourcedram[i], kvdestdram[i], kvstats[i]); }
	#endif
	#endif 
	return;
}



