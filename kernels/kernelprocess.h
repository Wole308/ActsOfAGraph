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
        );
	
private:
	#ifdef SW
	enigma enigmaobj;
	#endif
};
#endif 







