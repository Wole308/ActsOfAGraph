#include "../include/common.h"

#define MAXVALID_APPLYPARTITION_VECSIZE (MAX_APPLYPARTITION_VECSIZE >> POW_VALID_VDATA)

// unsigned int master_retrieve(unsigned int offset, unsigned int v, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
	// #pragma HLS INLINE // FIXME_HARDWARE
	// unsigned int datas[HBM_CHANNEL_PACK_SIZE];
	// #pragma HLS ARRAY_PARTITION variable=datas complete
	// master_retrieve_vec(offset, datas, HBM_channelA, HBM_channelB, inst);
	// return datas[v];
// }

void master_retrievemany_vec(unsigned int offsets[VALID_NUMPEs], unsigned int data[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
#pragma HLS INLINE // FIXME_HARDWARE
	return;
}

void dretrievemany_cfrontierdram_tmp(unsigned int base_offset__, unsigned int offsets[VALID_NUMPEs], unsigned int t, keyvalue_t datas[VALID_NUMPEs][EDGE_PACK_SIZE],  HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5){
#pragma HLS INLINE	
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP];
	checkoutofbounds("acts_kernel::ERROR 714a::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[VALID_NUMPEs][HBM_CHANNEL_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable=data_ complete dim=0
	unsigned int offsets_[VALID_NUMPEs];
	#pragma HLS ARRAY_PARTITION variable=offsets_ complete
	for(unsigned int n=0; n<VALID_NUMPEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<VALID_NUMPEs; n++){
		#pragma HLS UNROLL
			datas[n][v].key = data_[n][2*v];
			datas[n][v].value = data_[n][2*v+1];
		}
	}
	return; 
}

void insertvec_cfrontierbuffer_tmp(unsigned int t, frontier_t data[EDGE_PACK_SIZE], keyvalue_t cfrontier_buffer_tmp[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]){
#pragma HLS INLINE
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 115::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		cfrontier_buffer_tmp[v][t] = data[v];
	}
}

unsigned int dretrieve_globalparams(unsigned int base_offset__, unsigned int index, HBM_channelAXI_t * HBM_channelA, HBM_channelAXI_t * HBM_channelB, unsigned int inst){
#pragma HLS INLINE // FIXME_HARDWARE
	// return master_retrieve(base_offset__ + index, 0, HBM_channelA, HBM_channelB, inst); // NEW
	return HBM_channelA[base_offset__ + index].data[0];
}

//////////////////////////////////////////////////////////////////
extern "C" {		
void top_function( HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB){
#pragma HLS INTERFACE m_axi port = HBM_channelA0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = HBM_channelB0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = HBM_channelA1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = HBM_channelB1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = HBM_channelA2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = HBM_channelB2 offset = slave bundle = gmem5
#pragma HLS INTERFACE m_axi port = HBM_channelA3 offset = slave bundle = gmem6
#pragma HLS INTERFACE m_axi port = HBM_channelB3 offset = slave bundle = gmem7
#pragma HLS INTERFACE m_axi port = HBM_channelA4 offset = slave bundle = gmem8
#pragma HLS INTERFACE m_axi port = HBM_channelB4 offset = slave bundle = gmem9
#pragma HLS INTERFACE m_axi port = HBM_channelA5 offset = slave bundle = gmem10
#pragma HLS INTERFACE m_axi port = HBM_channelB5 offset = slave bundle = gmem11
#pragma HLS INTERFACE m_axi port = HBM_centerA offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = HBM_centerB offset = slave bundle = gmem13

#pragma HLS INTERFACE s_axilite port = HBM_channelA0 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB0 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA1 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB1 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA2 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB2 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA3 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB3 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA4 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB4 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA5 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB5 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_centerA bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_centerB bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

	// declaration of BRAM variables
	keyvalue_t cfrontier_buffer_tmp[EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE]; 
	#pragma HLS resource variable=cfrontier_buffer_tmp core=XPM_MEMORY uram
	#pragma HLS ARRAY_PARTITION variable=cfrontier_buffer_tmp complete dim=1
	vprop_t vdata_buffer[VALID_NUMPEs][EDGE_PACK_SIZE][MAXVALID_APPLYPARTITION_VECSIZE];
	#pragma HLS resource variable=vdata_buffer core=XPM_MEMORY uram 
	#pragma HLS ARRAY_PARTITION variable=vdata_buffer complete dim=1
    #pragma HLS ARRAY_PARTITION variable=vdata_buffer complete dim=2 
	
	// load global parameters
	unsigned int globalparams[GLOBALBUFFER_SIZE];
	MY_LOOP209: for(unsigned int t=0; t<GLOBALBUFFER_SIZE; t++){
	#pragma HLS PIPELINE II=1
		globalparams[t] = dretrieve_globalparams(0, t, HBM_channelA0, HBM_channelB0, 0);
	}

	// run acts 
	RUN_ACTS_LOOP: for(unsigned int GraphIter=0; GraphIter<globalparams[0]; GraphIter++){ // globalparams[GLOBALPARAMSCODE__PARAM__NUM_ITERATIONS]
		unsigned int MASK_CODE = 1 + GraphIter;
		unsigned int MASK_CODE_PE = 1 + GraphIter;
		unsigned int MASK_CODE_AU = 4094 + 1 + GraphIter;
		
		// process-edges and partition-updates
		PROCESS_EDGES_MODULE_LOOP1: for(unsigned int n=0; n<1; n++){
			// process-edges and partition-updates
			PROCESS_EDGES_MODULE_LOOP1B: for(unsigned int p_u=0; p_u<globalparams[1]; p_u++){ // __NUM_UPARTITIONS
				MASK_CODE_PE = ((1 + GraphIter) * MAX_NUM_UPARTITIONS) + p_u;
			
				// broadcast active frontiers [done]
				unsigned int uoffset_vecsz = p_u * MAX_UPARTITION_VECSIZE; 
				unsigned int data[4];
				#pragma HLS ARRAY_PARTITION variable=data complete
				unsigned int offset_c = uoffset_vecsz + 1234; // globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP];
				
				if(globalparams[2] == 1){
				MY_LOOP179: for(unsigned int t=0; t<MAXVALID_APPLYPARTITION_VECSIZE; t++){ 
				#pragma HLS PIPELINE II=1
					data[0] = HBM_centerA[uoffset_vecsz + t].data[0];
					data[1] = HBM_centerA[uoffset_vecsz + t].data[1];
					data[2] = HBM_centerB[uoffset_vecsz + t].data[0];
					data[3] = HBM_centerB[uoffset_vecsz + t].data[1];
					
					HBM_channelA0[offset_c + t].data[0] = data[0];
					HBM_channelA0[offset_c + t].data[1] = data[1];
					HBM_channelB0[offset_c + t].data[0] = data[2];
					HBM_channelB0[offset_c + t].data[1] = data[3];
					
					HBM_channelA1[offset_c + t].data[0] = data[0];
					HBM_channelA1[offset_c + t].data[1] = data[1];
					HBM_channelB1[offset_c + t].data[0] = data[2];
					HBM_channelB1[offset_c + t].data[1] = data[3];
					
					HBM_channelA2[offset_c + t].data[0] = data[0];
					HBM_channelA2[offset_c + t].data[1] = data[1];
					HBM_channelB2[offset_c + t].data[0] = data[2];
					HBM_channelB2[offset_c + t].data[1] = data[3];
					
					HBM_channelA3[offset_c + t].data[0] = data[0];
					HBM_channelA3[offset_c + t].data[1] = data[1];
					HBM_channelB3[offset_c + t].data[0] = data[2];
					HBM_channelB3[offset_c + t].data[1] = data[3];
					
					HBM_channelA4[offset_c + t].data[0] = data[0];
					HBM_channelA4[offset_c + t].data[1] = data[1];
					HBM_channelB4[offset_c + t].data[0] = data[2];
					HBM_channelB4[offset_c + t].data[1] = data[3];
					
					HBM_channelA5[offset_c + t].data[0] = data[0];
					HBM_channelA5[offset_c + t].data[1] = data[1];
					HBM_channelB5[offset_c + t].data[0] = data[2];
					HBM_channelB5[offset_c + t].data[1] = data[3];
				}
				}
				
				unsigned int data0[2];
				unsigned int data1[2];
				unsigned int data2[2];
				unsigned int data3[2];
				unsigned int data4[2];
				unsigned int data5[2];
				if(globalparams[3] == 1){
				MY_LOOP1793: for(unsigned int t=0; t<MAXVALID_APPLYPARTITION_VECSIZE; t++){ 
				#pragma HLS PIPELINE II=1
					data0[0] = HBM_channelA0[uoffset_vecsz + t].data[0];
					data0[1] = HBM_channelA0[uoffset_vecsz + t].data[1];
					
					data1[0] = HBM_channelA1[uoffset_vecsz + t].data[0];
					data1[1] = HBM_channelA1[uoffset_vecsz + t].data[1];
					
					data2[0] = HBM_channelA2[uoffset_vecsz + t].data[0];
					data2[1] = HBM_channelA2[uoffset_vecsz + t].data[1];
					
					data3[0] = HBM_channelA3[uoffset_vecsz + t].data[0];
					data3[1] = HBM_channelA3[uoffset_vecsz + t].data[1];
					
					data4[0] = HBM_channelA4[uoffset_vecsz + t].data[0];
					data4[1] = HBM_channelA4[uoffset_vecsz + t].data[1];
					
					data5[0] = HBM_channelA5[uoffset_vecsz + t].data[0];
					data5[1] = HBM_channelA5[uoffset_vecsz + t].data[1];
					
					HBM_channelB0[offset_c + t].data[0] = data0[0];
					HBM_channelB0[offset_c + t].data[1] = data0[1];
					
					HBM_channelB1[offset_c + t].data[0] = data1[0];
					HBM_channelB1[offset_c + t].data[1] = data1[1];
					
					HBM_channelB2[offset_c + t].data[0] = data2[0];
					HBM_channelB2[offset_c + t].data[1] = data2[1];
					
					HBM_channelB3[offset_c + t].data[0] = data3[0];
					HBM_channelB3[offset_c + t].data[1] = data3[1];
					
					HBM_channelB4[offset_c + t].data[0] = data4[0];
					HBM_channelB4[offset_c + t].data[1] = data4[1];
					
					HBM_channelB5[offset_c + t].data[0] = data5[0];
					HBM_channelB5[offset_c + t].data[1] = data5[1];
				}
				}
				
				if(globalparams[4] == 1){
				MY_LOOP17933: for(unsigned int t=0; t<MAXVALID_APPLYPARTITION_VECSIZE; t++){ 
				#pragma HLS PIPELINE II=1	
					data[0] = HBM_centerA[uoffset_vecsz + t].data[0];
					data[1] = HBM_centerA[uoffset_vecsz + t].data[1];
					
					HBM_channelA0[offset_c + t].data[0] = data[0];
					HBM_channelA0[offset_c + t].data[1] = data[1];
					
					HBM_channelA1[offset_c + t].data[0] = data[0];
					HBM_channelA1[offset_c + t].data[1] = data[1];
					
					HBM_channelA2[offset_c + t].data[0] = data[0];
					HBM_channelA2[offset_c + t].data[1] = data[1];
					
					HBM_channelA3[offset_c + t].data[0] = data[0];
					HBM_channelA3[offset_c + t].data[1] = data[1];
					
					HBM_channelA4[offset_c + t].data[0] = data[0];
					HBM_channelA4[offset_c + t].data[1] = data[1];
					
					HBM_channelA5[offset_c + t].data[0] = data[0];
					HBM_channelA5[offset_c + t].data[1] = data[1];
				}
				}
			}
		}
	}
}
}