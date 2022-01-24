// #include "../../include/config_params.h"
// #include "../../include/common.h"
// #include "../include/actscommon.h"

#include <string.h>
#include <cmath>
#include <ap_int.h>

typedef ap_uint<512> uint512_dt;

extern "C" {
void headproject(
	uint512_dt * kvdram0,
	uint512_dt * vdram
	){

#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem1

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = vdram
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter; // REMOVEME.
	for(unsigned int t=0; t<123456; t++){
		kvdram0[567 + t] = vdram[123 + t];
		// kvdram1[GraphIter + t] = vdram[GraphIter + t];
		// kvdram2[GraphIter + t] = vdram[GraphIter + t];
		// kvdram3[GraphIter + t] = vdram[GraphIter + t];
		// kvdram4[GraphIter + t] = vdram[GraphIter + t];
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// if(GraphIter > 0){ MERGE_SPLIT_broadcastVs1(kvdram0, vdram); }
	
	// 	// 	// #ifdef _DEBUGMODE_KERNELPRINTS3
	// cout<<">>> topkernelP1: processing instance 0"<<endl;
	// #endif 
	// topkernelproc_embedded(kvdram0);	
	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	// 	
	// MERGE_SPLIT_mergeVs1(kvdram0, vdram);
	return;
}
}


