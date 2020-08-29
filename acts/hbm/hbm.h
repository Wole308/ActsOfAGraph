#ifndef HBM_H
#define HBM_H
#include <iostream>
#include "../../include/common.h"
using namespace std;

class hbm {
public:
	hbm();
	~hbm();
	
	void topkernel(
		v_dt *in1,             // Read-Only Vector 1
		v_dt *in2,             // Read-Only Vector 2
		v_dt *out_add               // Output Result for ADD
	);

private:
};
#endif