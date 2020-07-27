#ifndef KERNELPROCESS_H
#define KERNELPROCESS_H
#include <mutex>
#include <iostream>
#include <thread>
#include "../kernels/acts.h"
#include "../src/common.h"

class kernelprocess {
public:
	kernelprocess();
	~kernelprocess();
	
	void topkernel(
uint512_dt * kvsourcedramA
		,uint512_dt * kvdestdramA
		,keyvalue_t * kvstatsA
        );
	void topkernelMW(uint512_dt ** kvsourcedram, uint512_dt ** kvdestdram, keyvalue_t ** kvstats);
	
private:
	#ifdef SW
	#ifdef ACTS
	acts kernelobj;
	#else 
	enigma kernelobj;
	#endif 
	std::thread acts_thread[NUMDRAMBANKS];
	#endif
};
#endif 







