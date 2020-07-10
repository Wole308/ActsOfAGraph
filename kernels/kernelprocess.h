#ifndef KERNELPROCESS_H
#define KERNELPROCESS_H
#include "../kernels/enigma.h"
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
	
private:
	#ifdef SW
	enigma enigmaobj;
	#endif
};
#endif 







