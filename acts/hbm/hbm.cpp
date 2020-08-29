#include <iostream>
#include "hbm.h"
using namespace std;

#ifdef SW
hbm::hbm(){}
hbm::~hbm(){}
#endif

extern "C" {
#ifdef SW 
void hbm::topkernel
#else 
void topkernel
#endif 
(
    v_dt *in1,             // Read-Only Vector 1
    v_dt *in2,             // Read-Only Vector 2
    v_dt *out_add               // Output Result for ADD
){
#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = out_add offset = slave bundle = gmem2

#pragma HLS INTERFACE s_axilite port = in1 bundle = control
#pragma HLS INTERFACE s_axilite port = in2 bundle = control
#pragma HLS INTERFACE s_axilite port = out_add bundle = control

#pragma HLS INTERFACE s_axilite port = return bundle = control

#pragma HLS DATA_PACK variable = in1
#pragma HLS DATA_PACK variable = in2
#pragma HLS DATA_PACK variable = out_add

	#ifdef _DEBUGMODE_KERNELPRINTS2
	cout<<"hbm::topkernel:: HBM kernel example started"<<endl;
	#endif 
	unsigned int size = 1024;
    unsigned int num_times = 1024;

    unsigned int vSize = ((size - 1) / VDATA_SIZE) + 1;

    v_dt tmpIn1, tmpIn2;
    v_dt tmpOutAdd, tmpOutMul;

    for (int count = 0; count < num_times; count++) {
    vops1:
        for (int i = 0; i < vSize; i++) {
		#pragma HLS PIPELINE II=1
            tmpIn1 = in1[i];
            tmpIn2 = in2[i];
	
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(count == 0 && i < 2){ for(unsigned int k=0; k<VDATA_SIZE; k++){ cout<<"hbm::topkernel:: i: "<<i<<", tmpIn1.data["<<k<<"]: "<<tmpIn1.data[k]<<", tmpIn2.data["<<k<<"]: "<<tmpIn2.data[k]<<endl; }}				
			#endif 
			
			vops2:
				for (int k = 0; k < VDATA_SIZE; k++) {
					tmpOutAdd.data[k] = tmpIn1.data[k] + tmpIn2.data[k];
				}

            out_add[i] = tmpOutAdd;
        }
    }
}
}