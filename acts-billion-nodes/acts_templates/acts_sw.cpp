#include "acts_sw.h"
using namespace std;
// __NOT__YET__IMPLEMENTED___
// __OBSOLETE___

#ifndef FPGA_IMPL
acts_sw::acts_sw(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
	algorithmobj = new algorithm();
}
acts_sw::~acts_sw(){} 
#endif 

void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	#ifdef _DEBUGMODE_CHECKS3
	if(data >= upper_bound){ std::cout<<"utility::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }			
	#endif 
}

unsigned int get_H(unsigned int vid){
	return (vid % (FOLD_SIZE * EDGE_PACK_SIZE * NUM_PEs)) / (FOLD_SIZE * EDGE_PACK_SIZE);
	// return vid % NUM_PEs;
	// return get_H(vid);
}
unsigned int get_local(unsigned int vid){
	unsigned int W = (FOLD_SIZE * EDGE_PACK_SIZE) * NUM_PEs;
	unsigned int y = vid / W; 
	unsigned int x = vid % (FOLD_SIZE * EDGE_PACK_SIZE);
	unsigned int lvid = (y * (FOLD_SIZE * EDGE_PACK_SIZE)) + x;
	return lvid;
}
unsigned int get_global(unsigned int lvid, unsigned int H){
	unsigned int W = (FOLD_SIZE * EDGE_PACK_SIZE) * NUM_PEs;
	unsigned int y2 = lvid / (FOLD_SIZE * EDGE_PACK_SIZE);
	unsigned int x2 = lvid % (FOLD_SIZE * EDGE_PACK_SIZE);		
	unsigned int vid = (y2 * W) + (H * (FOLD_SIZE * EDGE_PACK_SIZE)) + x2;
	return vid;
}
unsigned int get_local_to_upartition(unsigned int lvid){
	return lvid % MAX_UPARTITION_SIZE;
}
unsigned int get_local_to_vpartition(unsigned int lvid){
	return lvid % MAX_APPLYPARTITION_SIZE;
}

void rearrangeLayoutVx16F(unsigned int s, keyvalue_t in[EDGE_PACK_SIZE], keyvalue_t out[EDGE_PACK_SIZE]){
	if(s==0){ 
		out[0] = in[0]; 
		out[1] = in[1]; 
		out[2] = in[2]; 
		out[3] = in[3]; 
		out[4] = in[4]; 
		out[5] = in[5]; 
		out[6] = in[6]; 
		out[7] = in[7]; 
		out[8] = in[8]; 
		out[9] = in[9]; 
		out[10] = in[10]; 
		out[11] = in[11]; 
		out[12] = in[12]; 
		out[13] = in[13]; 
		out[14] = in[14]; 
		out[15] = in[15]; 
	} else if(s==1){ 
		out[15] = in[0]; 
		out[0] = in[1]; 
		out[1] = in[2]; 
		out[2] = in[3]; 
		out[3] = in[4]; 
		out[4] = in[5]; 
		out[5] = in[6]; 
		out[6] = in[7]; 
		out[7] = in[8]; 
		out[8] = in[9]; 
		out[9] = in[10]; 
		out[10] = in[11]; 
		out[11] = in[12]; 
		out[12] = in[13]; 
		out[13] = in[14]; 
		out[14] = in[15]; 
	} else if(s==2){ 
		out[14] = in[0]; 
		out[15] = in[1]; 
		out[0] = in[2]; 
		out[1] = in[3]; 
		out[2] = in[4]; 
		out[3] = in[5]; 
		out[4] = in[6]; 
		out[5] = in[7]; 
		out[6] = in[8]; 
		out[7] = in[9]; 
		out[8] = in[10]; 
		out[9] = in[11]; 
		out[10] = in[12]; 
		out[11] = in[13]; 
		out[12] = in[14]; 
		out[13] = in[15]; 
	} else if(s==3){ 
		out[13] = in[0]; 
		out[14] = in[1]; 
		out[15] = in[2]; 
		out[0] = in[3]; 
		out[1] = in[4]; 
		out[2] = in[5]; 
		out[3] = in[6]; 
		out[4] = in[7]; 
		out[5] = in[8]; 
		out[6] = in[9]; 
		out[7] = in[10]; 
		out[8] = in[11]; 
		out[9] = in[12]; 
		out[10] = in[13]; 
		out[11] = in[14]; 
		out[12] = in[15]; 
	} else if(s==4){ 
		out[12] = in[0]; 
		out[13] = in[1]; 
		out[14] = in[2]; 
		out[15] = in[3]; 
		out[0] = in[4]; 
		out[1] = in[5]; 
		out[2] = in[6]; 
		out[3] = in[7]; 
		out[4] = in[8]; 
		out[5] = in[9]; 
		out[6] = in[10]; 
		out[7] = in[11]; 
		out[8] = in[12]; 
		out[9] = in[13]; 
		out[10] = in[14]; 
		out[11] = in[15]; 
	} else if(s==5){ 
		out[11] = in[0]; 
		out[12] = in[1]; 
		out[13] = in[2]; 
		out[14] = in[3]; 
		out[15] = in[4]; 
		out[0] = in[5]; 
		out[1] = in[6]; 
		out[2] = in[7]; 
		out[3] = in[8]; 
		out[4] = in[9]; 
		out[5] = in[10]; 
		out[6] = in[11]; 
		out[7] = in[12]; 
		out[8] = in[13]; 
		out[9] = in[14]; 
		out[10] = in[15]; 
	} else if(s==6){ 
		out[10] = in[0]; 
		out[11] = in[1]; 
		out[12] = in[2]; 
		out[13] = in[3]; 
		out[14] = in[4]; 
		out[15] = in[5]; 
		out[0] = in[6]; 
		out[1] = in[7]; 
		out[2] = in[8]; 
		out[3] = in[9]; 
		out[4] = in[10]; 
		out[5] = in[11]; 
		out[6] = in[12]; 
		out[7] = in[13]; 
		out[8] = in[14]; 
		out[9] = in[15]; 
	} else if(s==7){ 
		out[9] = in[0]; 
		out[10] = in[1]; 
		out[11] = in[2]; 
		out[12] = in[3]; 
		out[13] = in[4]; 
		out[14] = in[5]; 
		out[15] = in[6]; 
		out[0] = in[7]; 
		out[1] = in[8]; 
		out[2] = in[9]; 
		out[3] = in[10]; 
		out[4] = in[11]; 
		out[5] = in[12]; 
		out[6] = in[13]; 
		out[7] = in[14]; 
		out[8] = in[15]; 
	} else if(s==8){ 
		out[8] = in[0]; 
		out[9] = in[1]; 
		out[10] = in[2]; 
		out[11] = in[3]; 
		out[12] = in[4]; 
		out[13] = in[5]; 
		out[14] = in[6]; 
		out[15] = in[7]; 
		out[0] = in[8]; 
		out[1] = in[9]; 
		out[2] = in[10]; 
		out[3] = in[11]; 
		out[4] = in[12]; 
		out[5] = in[13]; 
		out[6] = in[14]; 
		out[7] = in[15]; 
	} else if(s==9){ 
		out[7] = in[0]; 
		out[8] = in[1]; 
		out[9] = in[2]; 
		out[10] = in[3]; 
		out[11] = in[4]; 
		out[12] = in[5]; 
		out[13] = in[6]; 
		out[14] = in[7]; 
		out[15] = in[8]; 
		out[0] = in[9]; 
		out[1] = in[10]; 
		out[2] = in[11]; 
		out[3] = in[12]; 
		out[4] = in[13]; 
		out[5] = in[14]; 
		out[6] = in[15]; 
	} else if(s==10){ 
		out[6] = in[0]; 
		out[7] = in[1]; 
		out[8] = in[2]; 
		out[9] = in[3]; 
		out[10] = in[4]; 
		out[11] = in[5]; 
		out[12] = in[6]; 
		out[13] = in[7]; 
		out[14] = in[8]; 
		out[15] = in[9]; 
		out[0] = in[10]; 
		out[1] = in[11]; 
		out[2] = in[12]; 
		out[3] = in[13]; 
		out[4] = in[14]; 
		out[5] = in[15]; 
	} else if(s==11){ 
		out[5] = in[0]; 
		out[6] = in[1]; 
		out[7] = in[2]; 
		out[8] = in[3]; 
		out[9] = in[4]; 
		out[10] = in[5]; 
		out[11] = in[6]; 
		out[12] = in[7]; 
		out[13] = in[8]; 
		out[14] = in[9]; 
		out[15] = in[10]; 
		out[0] = in[11]; 
		out[1] = in[12]; 
		out[2] = in[13]; 
		out[3] = in[14]; 
		out[4] = in[15]; 
	} else if(s==12){ 
		out[4] = in[0]; 
		out[5] = in[1]; 
		out[6] = in[2]; 
		out[7] = in[3]; 
		out[8] = in[4]; 
		out[9] = in[5]; 
		out[10] = in[6]; 
		out[11] = in[7]; 
		out[12] = in[8]; 
		out[13] = in[9]; 
		out[14] = in[10]; 
		out[15] = in[11]; 
		out[0] = in[12]; 
		out[1] = in[13]; 
		out[2] = in[14]; 
		out[3] = in[15]; 
	} else if(s==13){ 
		out[3] = in[0]; 
		out[4] = in[1]; 
		out[5] = in[2]; 
		out[6] = in[3]; 
		out[7] = in[4]; 
		out[8] = in[5]; 
		out[9] = in[6]; 
		out[10] = in[7]; 
		out[11] = in[8]; 
		out[12] = in[9]; 
		out[13] = in[10]; 
		out[14] = in[11]; 
		out[15] = in[12]; 
		out[0] = in[13]; 
		out[1] = in[14]; 
		out[2] = in[15]; 
	} else if(s==14){ 
		out[2] = in[0]; 
		out[3] = in[1]; 
		out[4] = in[2]; 
		out[5] = in[3]; 
		out[6] = in[4]; 
		out[7] = in[5]; 
		out[8] = in[6]; 
		out[9] = in[7]; 
		out[10] = in[8]; 
		out[11] = in[9]; 
		out[12] = in[10]; 
		out[13] = in[11]; 
		out[14] = in[12]; 
		out[15] = in[13]; 
		out[0] = in[14]; 
		out[1] = in[15]; 
	} else { 
		out[1] = in[0]; 
		out[2] = in[1]; 
		out[3] = in[2]; 
		out[4] = in[3]; 
		out[5] = in[4]; 
		out[6] = in[5]; 
		out[7] = in[6]; 
		out[8] = in[7]; 
		out[9] = in[8]; 
		out[10] = in[9]; 
		out[11] = in[10]; 
		out[12] = in[11]; 
		out[13] = in[12]; 
		out[14] = in[13]; 
		out[15] = in[14]; 
		out[0] = in[15]; 
	}
	return;
}		
void rearrangeLayoutVx16B(unsigned int s, keyvalue_t in[EDGE_PACK_SIZE], keyvalue_t out[EDGE_PACK_SIZE]){
	if(s==0){ 
		out[0] = in[0]; 
		out[1] = in[1]; 
		out[2] = in[2]; 
		out[3] = in[3]; 
		out[4] = in[4]; 
		out[5] = in[5]; 
		out[6] = in[6]; 
		out[7] = in[7]; 
		out[8] = in[8]; 
		out[9] = in[9]; 
		out[10] = in[10]; 
		out[11] = in[11]; 
		out[12] = in[12]; 
		out[13] = in[13]; 
		out[14] = in[14]; 
		out[15] = in[15]; 
	}
	else if(s==1){ 
		out[1] = in[0]; 
		out[2] = in[1]; 
		out[3] = in[2]; 
		out[4] = in[3]; 
		out[5] = in[4]; 
		out[6] = in[5]; 
		out[7] = in[6]; 
		out[8] = in[7]; 
		out[9] = in[8]; 
		out[10] = in[9]; 
		out[11] = in[10]; 
		out[12] = in[11]; 
		out[13] = in[12]; 
		out[14] = in[13]; 
		out[15] = in[14]; 
		out[0] = in[15]; 
	}
	else if(s==2){ 
		out[2] = in[0]; 
		out[3] = in[1]; 
		out[4] = in[2]; 
		out[5] = in[3]; 
		out[6] = in[4]; 
		out[7] = in[5]; 
		out[8] = in[6]; 
		out[9] = in[7]; 
		out[10] = in[8]; 
		out[11] = in[9]; 
		out[12] = in[10]; 
		out[13] = in[11]; 
		out[14] = in[12]; 
		out[15] = in[13]; 
		out[0] = in[14]; 
		out[1] = in[15]; 
	}
	else if(s==3){ 
		out[3] = in[0]; 
		out[4] = in[1]; 
		out[5] = in[2]; 
		out[6] = in[3]; 
		out[7] = in[4]; 
		out[8] = in[5]; 
		out[9] = in[6]; 
		out[10] = in[7]; 
		out[11] = in[8]; 
		out[12] = in[9]; 
		out[13] = in[10]; 
		out[14] = in[11]; 
		out[15] = in[12]; 
		out[0] = in[13]; 
		out[1] = in[14]; 
		out[2] = in[15]; 
	}
	else if(s==4){ 
		out[4] = in[0]; 
		out[5] = in[1]; 
		out[6] = in[2]; 
		out[7] = in[3]; 
		out[8] = in[4]; 
		out[9] = in[5]; 
		out[10] = in[6]; 
		out[11] = in[7]; 
		out[12] = in[8]; 
		out[13] = in[9]; 
		out[14] = in[10]; 
		out[15] = in[11]; 
		out[0] = in[12]; 
		out[1] = in[13]; 
		out[2] = in[14]; 
		out[3] = in[15]; 
	}
	else if(s==5){ 
		out[5] = in[0]; 
		out[6] = in[1]; 
		out[7] = in[2]; 
		out[8] = in[3]; 
		out[9] = in[4]; 
		out[10] = in[5]; 
		out[11] = in[6]; 
		out[12] = in[7]; 
		out[13] = in[8]; 
		out[14] = in[9]; 
		out[15] = in[10]; 
		out[0] = in[11]; 
		out[1] = in[12]; 
		out[2] = in[13]; 
		out[3] = in[14]; 
		out[4] = in[15]; 
	}
	else if(s==6){ 
		out[6] = in[0]; 
		out[7] = in[1]; 
		out[8] = in[2]; 
		out[9] = in[3]; 
		out[10] = in[4]; 
		out[11] = in[5]; 
		out[12] = in[6]; 
		out[13] = in[7]; 
		out[14] = in[8]; 
		out[15] = in[9]; 
		out[0] = in[10]; 
		out[1] = in[11]; 
		out[2] = in[12]; 
		out[3] = in[13]; 
		out[4] = in[14]; 
		out[5] = in[15]; 
	}
	else if(s==7){ 
		out[7] = in[0]; 
		out[8] = in[1]; 
		out[9] = in[2]; 
		out[10] = in[3]; 
		out[11] = in[4]; 
		out[12] = in[5]; 
		out[13] = in[6]; 
		out[14] = in[7]; 
		out[15] = in[8]; 
		out[0] = in[9]; 
		out[1] = in[10]; 
		out[2] = in[11]; 
		out[3] = in[12]; 
		out[4] = in[13]; 
		out[5] = in[14]; 
		out[6] = in[15]; 
	}
	else if(s==8){ 
		out[8] = in[0]; 
		out[9] = in[1]; 
		out[10] = in[2]; 
		out[11] = in[3]; 
		out[12] = in[4]; 
		out[13] = in[5]; 
		out[14] = in[6]; 
		out[15] = in[7]; 
		out[0] = in[8]; 
		out[1] = in[9]; 
		out[2] = in[10]; 
		out[3] = in[11]; 
		out[4] = in[12]; 
		out[5] = in[13]; 
		out[6] = in[14]; 
		out[7] = in[15]; 
	}
	else if(s==9){ 
		out[9] = in[0]; 
		out[10] = in[1]; 
		out[11] = in[2]; 
		out[12] = in[3]; 
		out[13] = in[4]; 
		out[14] = in[5]; 
		out[15] = in[6]; 
		out[0] = in[7]; 
		out[1] = in[8]; 
		out[2] = in[9]; 
		out[3] = in[10]; 
		out[4] = in[11]; 
		out[5] = in[12]; 
		out[6] = in[13]; 
		out[7] = in[14]; 
		out[8] = in[15]; 
	}
	else if(s==10){ 
		out[10] = in[0]; 
		out[11] = in[1]; 
		out[12] = in[2]; 
		out[13] = in[3]; 
		out[14] = in[4]; 
		out[15] = in[5]; 
		out[0] = in[6]; 
		out[1] = in[7]; 
		out[2] = in[8]; 
		out[3] = in[9]; 
		out[4] = in[10]; 
		out[5] = in[11]; 
		out[6] = in[12]; 
		out[7] = in[13]; 
		out[8] = in[14]; 
		out[9] = in[15]; 
	}
	else if(s==11){ 
		out[11] = in[0]; 
		out[12] = in[1]; 
		out[13] = in[2]; 
		out[14] = in[3]; 
		out[15] = in[4]; 
		out[0] = in[5]; 
		out[1] = in[6]; 
		out[2] = in[7]; 
		out[3] = in[8]; 
		out[4] = in[9]; 
		out[5] = in[10]; 
		out[6] = in[11]; 
		out[7] = in[12]; 
		out[8] = in[13]; 
		out[9] = in[14]; 
		out[10] = in[15]; 
	}
	else if(s==12){ 
		out[12] = in[0]; 
		out[13] = in[1]; 
		out[14] = in[2]; 
		out[15] = in[3]; 
		out[0] = in[4]; 
		out[1] = in[5]; 
		out[2] = in[6]; 
		out[3] = in[7]; 
		out[4] = in[8]; 
		out[5] = in[9]; 
		out[6] = in[10]; 
		out[7] = in[11]; 
		out[8] = in[12]; 
		out[9] = in[13]; 
		out[10] = in[14]; 
		out[11] = in[15]; 
	}
	else if(s==13){ 
		out[13] = in[0]; 
		out[14] = in[1]; 
		out[15] = in[2]; 
		out[0] = in[3]; 
		out[1] = in[4]; 
		out[2] = in[5]; 
		out[3] = in[6]; 
		out[4] = in[7]; 
		out[5] = in[8]; 
		out[6] = in[9]; 
		out[7] = in[10]; 
		out[8] = in[11]; 
		out[9] = in[12]; 
		out[10] = in[13]; 
		out[11] = in[14]; 
		out[12] = in[15]; 
	}
	else if(s==14){ 
		out[14] = in[0]; 
		out[15] = in[1]; 
		out[0] = in[2]; 
		out[1] = in[3]; 
		out[2] = in[4]; 
		out[3] = in[5]; 
		out[4] = in[6]; 
		out[5] = in[7]; 
		out[6] = in[8]; 
		out[7] = in[9]; 
		out[8] = in[10]; 
		out[9] = in[11]; 
		out[10] = in[12]; 
		out[11] = in[13]; 
		out[12] = in[14]; 
		out[13] = in[15]; 
	}
	else { 
		out[15] = in[0]; 
		out[0] = in[1]; 
		out[1] = in[2]; 
		out[2] = in[3]; 
		out[3] = in[4]; 
		out[4] = in[5]; 
		out[5] = in[6]; 
		out[6] = in[7]; 
		out[7] = in[8]; 
		out[8] = in[9]; 
		out[9] = in[10]; 
		out[10] = in[11]; 
		out[11] = in[12]; 
		out[12] = in[13]; 
		out[13] = in[14]; 
		out[14] = in[15]; 
	}
	return;
}
void check_if_contiguous(keyvalue_t keyvalue[EDGE_PACK_SIZE], keyvalue_t msg1[EDGE_PACK_SIZE], keyvalue_t msg2[EDGE_PACK_SIZE], unsigned int msg1_str, unsigned int msg2_str, unsigned int msg3_str){
	#ifdef _DEBUGMODE_CHECKS3
	for(int v = 0; v < EDGE_PACK_SIZE; v++){ 
		unsigned int local_dstvid = get_local(keyvalue[v].key);
		if(keyvalue[v].key != INVALIDDATA){ 
			if(local_dstvid % EDGE_PACK_SIZE != v){ 
				cout<<"check_if_contiguous(1). ERROR 314. v["<<v<<"].key("<<local_dstvid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<local_dstvid % EDGE_PACK_SIZE<<") != v("<<v<<"). [msg1_str: "<<msg1_str<<", msg2_str: "<<msg2_str<<", msg3_str: "<<msg3_str<<"] EXITING..."<<endl; 
				for(int v = 0; v < EDGE_PACK_SIZE; v++){ cout<<"+++ keyvalue["<<v<<"].key: "<<get_local(keyvalue[v].key)<<"("<<get_local(keyvalue[v].key)%EDGE_PACK_SIZE<<"), keyvalue["<<v<<"].value: "<<keyvalue[v].value<<"("<<keyvalue[v].value%EDGE_PACK_SIZE<<")"<<endl; }
				for(int v = 0; v < EDGE_PACK_SIZE; v++){ cout<<"+++ msg1["<<v<<"].key: "<<msg1[v].key<<"("<<msg1[v].key%EDGE_PACK_SIZE<<"), msg1["<<v<<"].value: "<<msg1[v].value<<"("<<msg1[v].value%EDGE_PACK_SIZE<<")"<<endl; }
				for(int v = 0; v < EDGE_PACK_SIZE; v++){ cout<<"+++ msg2["<<v<<"].key: "<<msg2[v].key<<"("<<msg2[v].key%EDGE_PACK_SIZE<<"), msg2["<<v<<"].value: "<<msg2[v].value<<"("<<msg2[v].value%EDGE_PACK_SIZE<<")"<<endl; }
				exit(EXIT_FAILURE); 
			}
		}
	}
	#endif
}

//////////////////////////////////////////////////////////////////
void insert_cfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_center_t * HBM_center){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		HBM_center->cfrontier_dram[v][p_u][t] = data[v];
	}
}
void retrieve_cfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_center_t * HBM_center){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = HBM_center->cfrontier_dram[v][p_u][t];
	}
	return; 
}

////////////////////////////////////////////////////////////////
unsigned int retrieve_globalparams(unsigned int base_offset__, unsigned int index, HBM_channel_t * HBM_channel){
	unsigned int base_offset = 0;
	return HBM_channel->HBM[base_offset__ + index].data[0];
}

unsigned int retrieve_vptrdram(unsigned int base_offset__, unsigned int index, HBM_channel_t * HBM_channel, unsigned int inst){
	unsigned int base_offset = HBM_channel[inst].globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS].data[0];
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel[inst].globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS].data[0];
	checkoutofbounds("acts_sw::ERROR 701::", (index / HBM_CHANNEL_PACK_SIZE), wwsize, NAp, NAp, NAp);
	#endif 
	return HBM_channel[inst].HBM[base_offset__ + (index / HBM_CHANNEL_PACK_SIZE)].data[index % HBM_CHANNEL_PACK_SIZE];
}	

map_t retrieve_actpackvptrdram(unsigned int base_offset__, unsigned int p_u, unsigned int llp_set, HBM_channel_t * HBM_channel){
	unsigned int base_offset = HBM_channel->globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS].data[0];
	unsigned int offset = ((p_u * MAX_NUM_LLPSETS) + llp_set) * 2;
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS].data[0];
	checkoutofbounds("acts_sw::ERROR 702::", offset / HBM_CHANNEL_PACK_SIZE, wwsize, NAp, NAp, NAp);
	#endif 
	map_t map;
	unsigned int i = offset / HBM_CHANNEL_PACK_SIZE;
	unsigned int j = offset % HBM_CHANNEL_PACK_SIZE;
	map.offset = HBM_channel->HBM[base_offset__ + i].data[j];
	map.size = HBM_channel->HBM[base_offset__ + i].data[j + 1];
	return map;
}

edge3_vec_dt retrieve_csr_pack_edges(unsigned int base_offset__, unsigned int index, HBM_channel_t * HBM_channel){
	edge3_vec_dt edge3_vec;
	unsigned int base_offset = HBM_channel->globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES].data[0];
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES].data[0];
	checkoutofbounds("acts_sw::ERROR 703::", index, wwsize, NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		unsigned int srcvid = HBM_channel->HBM[base_offset__ + index].data[2*v];
		unsigned int dstvid = HBM_channel->HBM[base_offset__ + index].data[2*v + 1];
		edge3_vec.data[v].srcvid = srcvid;
		edge3_vec.data[v].dstvid = (dstvid >> 1) & 0xFFFFFFFF;
		edge3_vec.data[v].weight = 1;
		edge3_vec.data[v].valid = dstvid & 0x1;
	}
	return edge3_vec;
}

edge3_vec_dt retrieve_act_pack_edges(unsigned int base_offset__, unsigned int index, HBM_channel_t * HBM_channel){
	edge3_vec_dt edge3_vec;
	unsigned int base_offset = HBM_channel->globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES].data[0];
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES].data[0];
	checkoutofbounds("acts_sw::ERROR 704::", index, wwsize, NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		unsigned int srcvid = HBM_channel->HBM[base_offset__ + index].data[2*v];
		unsigned int dstvid = HBM_channel->HBM[base_offset__ + index].data[2*v + 1];
		edge3_vec.data[v].srcvid = srcvid;
		edge3_vec.data[v].dstvid = (dstvid >> 1) & 0xFFFFFFFF;
		edge3_vec.data[v].weight = 1;
		edge3_vec.data[v].valid = dstvid & 0x1;
	}
	return edge3_vec;
}
void retrievemany_act_pack_edges(unsigned int base_offset__, unsigned int offsets[NUM_PEs], unsigned int t, edge3_vec_dt edge3_vecs[NUM_PEs], HBM_channel_t * HBM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel[0].globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES].data[0];
	checkoutofbounds("acts_sw::ERROR 704f::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			unsigned int srcvid = HBM_channel[n].HBM[base_offset__ + offsets[n] + t].data[2*v];
			unsigned int dstvid = HBM_channel[n].HBM[base_offset__ + offsets[n] + t].data[2*v + 1];
			edge3_vecs[n].data[v].srcvid = srcvid;
			edge3_vecs[n].data[v].dstvid = (dstvid >> 1) & 0xFFFFFFFF;
			edge3_vecs[n].data[v].weight = 1;
			edge3_vecs[n].data[v].valid = dstvid & 0x1;
		}
	}
	return;
}

void insert_updatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt data, HBM_channel_t * HBM_channel, unsigned int globalparams[32], unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATES].data[0];
	checkoutofbounds("acts_sw::ERROR 708a::", updatesptrs[llp_set] + index, updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 708b::", llp_set, MAX_NUM_APPLYPARTITIONS, index, v, NAp);
		checkoutofbounds("acts_sw::ERROR 708c::", index, HBM_CHANNEL_SIZE, index, v, NAp);
		#endif 
		HBM_channel->HBM[offset__ + index].data[2*v] = data.data[v].key;
		HBM_channel->HBM[offset__ + index].data[2*v + 1] = data.data[v].value;
	}
}
void retrieve_udatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt * data, HBM_channel_t * HBM_channel, unsigned int globalparams[32], unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATES].data[0];
	checkoutofbounds("acts_sw::ERROR 710a::", updatesptrs[llp_set] + index, updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 710b::", llp_set, MAX_NUM_APPLYPARTITIONS, index, v, NAp);
		checkoutofbounds("acts_sw::ERROR 710c::", index, HBM_CHANNEL_SIZE, index, v, NAp);
		#endif 
		data->data[v].key = HBM_channel->HBM[offset__ + index].data[2*v]; 
		data->data[v].value = HBM_channel->HBM[offset__ + index].data[2*v + 1];		
	}
	return; 
}
void insertmany_updatesdram(unsigned int offset__, unsigned int llp_set, unsigned int offsets[NUM_PEs], unsigned int t, uint512_vec_dt data[NUM_PEs], bool ens[NUM_PEs][EDGE_PACK_SIZE], HBM_channel_t * HBM_channel, unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel[0].globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATES].data[0];
	checkoutofbounds("acts_sw::ERROR 708a::", updatesptrs[llp_set] + offsets[0], updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 708b::", llp_set, MAX_NUM_APPLYPARTITIONS, offsets[0], v, NAp);
		checkoutofbounds("acts_sw::ERROR 708c::", offsets[0], HBM_CHANNEL_SIZE, offsets[0], v, NAp);
		#endif 
		for(unsigned int n=0; n<NUM_PEs; n++){
			if(ens[n][v]==true){
				HBM_channel[n].HBM[offset__ + offsets[n] + t].data[2*v] = data[n].data[v].key;
				HBM_channel[n].HBM[offset__ + offsets[n] + t].data[2*v + 1] = data[n].data[v].value;
			}
		}
	}
}
void retrievemany_udatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt data[NUM_PEs], HBM_channel_t * HBM_channel, unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel[0].globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATES].data[0];
	checkoutofbounds("acts_sw::ERROR 710a::", updatesptrs[llp_set] + index, updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 710b::", llp_set, MAX_NUM_APPLYPARTITIONS, index, v, NAp);
		checkoutofbounds("acts_sw::ERROR 710c::", index, HBM_CHANNEL_SIZE, index, v, NAp);
		#endif 
		for(unsigned int n=0; n<NUM_PEs; n++){
			data[n].data[v].key = HBM_channel[n].HBM[offset__ + index].data[2*v]; 
			data[n].data[v].value = HBM_channel[n].HBM[offset__ + index].data[2*v + 1];		
		}
	}
	return; 
}

void insert_vdatadram(unsigned int offset__, unsigned int p_v, unsigned int index, vprop_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS].data[0];
	checkoutofbounds("acts_sw::ERROR 711::", index, wwsize, NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		HBM_channel->HBM[offset__ + index].data[2*v] = data[v].prop;
		HBM_channel->HBM[offset__ + index].data[2*v + 1] = data[v].mask;
	}
	return;
}
void retrieve_vdatadram(unsigned int offset__, unsigned int p_v, unsigned int index, vprop_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS].data[0];
	checkoutofbounds("acts_sw::ERROR 712::", index, wwsize, NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v].prop = HBM_channel->HBM[offset__ + index].data[2*v];
		data[v].mask = HBM_channel->HBM[offset__ + index].data[2*v + 1];
	}
	return;
}

void insert_cfrontierdram_tmp(unsigned int base_offset__, unsigned int index, keyvalue_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP].data[0];
	checkoutofbounds("acts_sw::ERROR 713a::", index, wwsize, NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 713b::", index, MAX_APPLYPARTITION_VECSIZE, index, v, NAp);
		#endif 
		HBM_channel->HBM[base_offset__ + index].data[2*v] = data[v].key;
		HBM_channel->HBM[base_offset__ + index].data[2*v + 1] = data[v].value;
	}
}
void retrieve_cfrontierdram_tmp(unsigned int base_offset__, unsigned int index, keyvalue_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP].data[0];
	checkoutofbounds("acts_sw::ERROR 714a::", index, wwsize, NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 714b::", index, MAX_APPLYPARTITION_VECSIZE, index, v, NAp);
		#endif 
		data[v].key = HBM_channel->HBM[base_offset__ + index].data[2*v];
		data[v].value = HBM_channel->HBM[base_offset__ + index].data[2*v + 1];
	}
	return; 
}

void insert_nfrontierdram(unsigned int offset__, unsigned int p_u, unsigned int index, keyvalue_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS].data[0];
	checkoutofbounds("acts_sw::ERROR 715a::", index, wwsize, NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 715b::", p_u, MAX_NUM_UPARTITIONS, index, v, NAp);
		checkoutofbounds("acts_sw::ERROR 715c::", index, MAX_APPLYPARTITION_VECSIZE, index, v, NAp);
		#endif 
		HBM_channel->HBM[offset__ + index].data[2*v] = data[v].key;
		HBM_channel->HBM[offset__ + index].data[2*v + 1] = data[v].value;
	}
}
void retrieve_nfrontierdram(unsigned int offset__, unsigned int p_u, unsigned int index, keyvalue_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = HBM_channel->globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS].data[0];
	checkoutofbounds("acts_sw::ERROR 716a::", index, wwsize, NAp, NAp, NAp);
	#endif 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 716b::", p_u, MAX_NUM_UPARTITIONS, index, v, NAp);
		checkoutofbounds("acts_sw::ERROR 716c::", index, MAX_APPLYPARTITION_VECSIZE, p_u, v, NAp);
		#endif 
		data[v].key = HBM_channel->HBM[offset__ + index].data[2*v];
		data[v].value = HBM_channel->HBM[offset__ + index].data[2*v + 1];
	}
	return; 
}

//////////////////////////////////////////////////////////////////
void insert_udatabuffer(unsigned int v, unsigned int index, vprop_t data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 100::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->vertex_buffer[v][index] = data; // udata_buffer
}

vprop_t retrieve_udatabuffer(unsigned int v, unsigned int index, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 101::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->vertex_buffer[v][index]; // udata_buffer
}

void insert_vdatabuffer(unsigned int v, unsigned int index, vprop_t data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 102::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->vertex_buffer[v][index] = data; // vdata_buffer
}
vprop_t retrieve_vdatabuffer(unsigned int v, unsigned int index, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 103::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->vertex_buffer[v][index]; // vdata_buffer
}
void insert_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 104::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		BRAM_channel->vertex_buffer[v][index] = data[v]; // vdata_buffer
	}
}
vprop_t retrieve_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 105::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = BRAM_channel->vertex_buffer[v][index]; // vdata_buffer
	}
}
void clear_vdatabuffer(unsigned int v, unsigned int index, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 106::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->vertex_buffer[v][index].mask = INVALIDMASK; // vdata_buffer
}

void insert_updatesbuffer(unsigned int v, unsigned int index, keyvalue_t data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 107::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->updates_buffer[v][index] = data; // vdata_buffer
}
keyvalue_t retrieve_updatesbuffer(unsigned int v, unsigned int index, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 108::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->updates_buffer[v][index]; // vdata_buffer
}

void insert_updatestmpbuffer(unsigned int index, unsigned int t, keyvalue_t data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 109::", index, NUM_PEs, NAp, NAp, NAp);
	checkoutofbounds("acts_sw::ERROR 110::", t, CSRBUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->updates_tmpbuffer[index][t] = data;
}
keyvalue_t retrieve_updatestmpbuffer(unsigned int index, unsigned int t, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 111::", index, NUM_PEs, NAp, NAp, NAp);
	checkoutofbounds("acts_sw::ERROR 112::", t, CSRBUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->updates_tmpbuffer[index][t];
}

// void insert_updates2tmpbuffer(unsigned int index, unsigned int t, keyvalue_t data, BRAM_channel_t * BRAM_channel){
	// #ifdef _DEBUGMODE_CHECKS3
	// checkoutofbounds("acts_sw::ERROR 109::", index, NUM_PEs, NAp, NAp, NAp);
	// checkoutofbounds("acts_sw::ERROR 110::", t, CSRBUFFER_SIZE, NAp, NAp, NAp);
	// #endif 
	// BRAM_channel->updates2_tmpbuffer[index][t] = data;
// }
// keyvalue_t retrieve_updates2tmpbuffer(unsigned int index, unsigned int t, BRAM_channel_t * BRAM_channel){
	// #ifdef _DEBUGMODE_CHECKS3
	// checkoutofbounds("acts_sw::ERROR 111::", index, NUM_PEs, NAp, NAp, NAp);
	// checkoutofbounds("acts_sw::ERROR 112::", t, CSRBUFFER_SIZE, NAp, NAp, NAp);
	// #endif 
	// return BRAM_channel->updates2_tmpbuffer[index][t];
// }

void insert_cfrontierbuffer(unsigned int v, unsigned int t, frontier_t data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 113::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->cfrontier_buffer[v][t] = data;
}
frontier_t retrieve_cfrontierbuffer_i(unsigned int v, unsigned int t, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 114::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->cfrontier_buffer[v][t];
}
void insert_cfrontierbuffer(unsigned int t, frontier_t data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 115::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		BRAM_channel->cfrontier_buffer[v][t] = data[v];
	}
}
void retrieve_cfrontierbuffer_i(unsigned int t, frontier_t data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 116::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = BRAM_channel->cfrontier_buffer[v][t];
	}
	return; 
}

void insert_nfrontierbuffer(unsigned int t, unsigned int v, frontier_t data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 117::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->nfrontier_buffer[v][t] = data;
}
frontier_t retrieve_nfrontierbuffer(unsigned int t, unsigned int v, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 118::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->nfrontier_buffer[v][t];
}
void insert_nfrontierbuffer(unsigned int t, frontier_t data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 119::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		BRAM_channel->nfrontier_buffer[v][t] = data[v];
	}
}
void retrieve_nfrontierbuffer(unsigned int t, frontier_t data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 120::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = BRAM_channel->nfrontier_buffer[v][t];
	}
	return; 
}

void insert_statstmpbuffer(unsigned int t, unsigned int v, unsigned int data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 121::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->stats_tmpbuffer[v][t] = data;
}
unsigned int retrieve_statstmpbuffer(unsigned int t, unsigned int v, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 122::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->stats_tmpbuffer[v][t]; 
}
void insert_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 123::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		BRAM_channel->stats_tmpbuffer[v][t] = data[v];
	}
}
void retrieve_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 124::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = BRAM_channel->stats_tmpbuffer[v][t];
	}
	return; 
}

void insert_statsbuffer(unsigned int t, unsigned int v, unsigned int data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 125::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->stats_buffer[v][t] = data;
}
unsigned int retrieve_statsbuffer(unsigned int t, unsigned int v, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 126::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->stats_buffer[v][t]; 
}

void insert_stats2buffer(unsigned int t, unsigned int v, unsigned int data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 127::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->stats2_buffer[v][t] = data;
}
unsigned int retrieve_stats2buffer(unsigned int t, unsigned int v, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 128::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->stats2_buffer[v][t]; 
}

void insert_vptrbuffer(unsigned int t, vtr_t data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 129::", t, VPTR_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->vptr_buffer[t] = data;
}
vtr_t retrieve_vptrbuffer(unsigned int t, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 130::", t, VPTR_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->vptr_buffer[t];
}
//////////////////////////////////////////////////////////////////

void acts_sw::run(HBM_center_t * HBM_center, HBM_channel_t * HBM_channel){					

	unsigned int __NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	unsigned int __NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs
	cout<<"acts_sw::run:: universalparams.NUM_VERTICES: "<<universalparams.NUM_VERTICES<<", universalparams.NUM_EDGES: "<<universalparams.NUM_EDGES<<", MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<", __NUM_UPARTITIONS: "<<__NUM_UPARTITIONS<<", MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<", __NUM_APPLYPARTITIONS: "<<__NUM_APPLYPARTITIONS<<endl;			
	
	BRAM_channel_t BRAM_channel[NUM_PEs];
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].nfrontier_buffer[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].cfrontier_buffer[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].stats_tmpbuffer[v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ BRAM_channel[i].stats_buffer[v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ BRAM_channel[i].stats2_buffer[v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<NUM_PEs; p++){ BRAM_channel[i].updates_tmpbuffer[p] = new keyvalue_t[CSRBUFFER_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].updates_buffer[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ BRAM_channel[i].vptr_buffer = new vtr_t[VPTR_BUFFER_SIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].edges_buffer[v] = new edge3_type[EDGE_BUFFER_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].vertex_buffer[v] = new vprop_t[MAX_APPLYPARTITION_VECSIZE]; }}
	
	unsigned int * cfrontier_dram___size = new unsigned int[MAX_NUM_UPARTITIONS]; 
	for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ cfrontier_dram___size[p] = 0; }
	
	unsigned int * nfrontier_dram___size[NUM_PEs][EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = nfrontier_dram___size complete dim=1
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_dram___size[i][v] = new unsigned int[MAX_NUM_UPARTITIONS]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ nfrontier_dram___size[i][v][p] = 0; }}}
	
	unsigned int * nfrontier_buffer___size[NUM_PEs][EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = nfrontier_buffer___size complete dim=1
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer___size[i][v] = new unsigned int[NUM_ACTVVPARTITIONS_PER_APPLYPARTITION]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p++){ nfrontier_buffer___size[i][v][p] = 0; }}}
	
	unsigned int updates_tmpbuffer___size[NUM_PEs][NUM_PEs]; 
	#pragma HLS ARRAY_PARTITION variable = updates_tmpbuffer___size complete dim=1
	
	unsigned int * updates_buffer___size[NUM_PEs][EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = updates_buffer___size complete dim=1
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer___size[i][v] = new unsigned int[MAX_NUM_APPLYPARTITIONS]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_APPLYPARTITIONS; p++){ updates_buffer___size[i][v][p] = 0; }}}
	
	unsigned int stats_buffer___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = stats_buffer___size complete dim=1
	unsigned int stats2_buffer___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = stats2_buffer___size complete dim=1
	
	offset_t * upartition_vertices = new offset_t[MAX_NUM_UPARTITIONS]; 
	for(unsigned int p=0; p<__NUM_UPARTITIONS; p++){ upartition_vertices[p].offset = 0; upartition_vertices[p].size = 0; upartition_vertices[p].count = 0; } upartition_vertices[0].count = 1;
	
	offset_t * vpartition_vertices[NUM_PEs]; 
	#pragma HLS ARRAY_PARTITION variable = vpartition_vertices complete dim=1
	for(unsigned int i=0; i<NUM_PEs; i++){ vpartition_vertices[i] = new offset_t[MAX_NUM_APPLYPARTITIONS];  }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ vpartition_vertices[i][p].offset = 0; vpartition_vertices[i][p].size = 0; vpartition_vertices[i][p].count = 0; }}
	
	unsigned int actpackupdates_dram___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = actpackupdates_dram___size complete dim=1
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ actpackupdates_dram___size[i][p] = 0; }}
	
	unsigned int vptrbuffer___size[NUM_PEs]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ vptrbuffer___size[i] = 0; }
	
	unsigned int edges_buffer___size[NUM_PEs]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ edges_buffer___size[i] = 0; }
	
	edge3_type edges[NUM_PEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = edges complete
	unsigned int res[NUM_PEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = res complete
	unsigned int vtemp_in[NUM_PEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vtemp_in complete
	unsigned int vtemp_out[NUM_PEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = vtemp_out complete
	vprop_t uprop[NUM_PEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = uprop complete
	keyvalue_t update_in[NUM_PEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = update_in complete
	keyvalue_t update_out[NUM_PEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = update_out complete
	bool ens[NUM_PEs][EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = ens complete
						
	unsigned int num_LLPs = universalparams.NUM_APPLYPARTITIONS * universalparams.NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (universalparams.NUM_PARTITIONS - 1)) / universalparams.NUM_PARTITIONS;
	unsigned int maxGraphIter = 0;
	
	bool run_vertex_centric = true;
	unsigned int * hybrid_map[MAXNUMGRAPHITERATIONS];
	for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ hybrid_map[i] = new unsigned int[MAX_NUM_UPARTITIONS]; }
	for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ for(unsigned int t=0; t<MAX_NUM_UPARTITIONS; t++){ hybrid_map[i][t] = NAp; }}
	bool enable___vertexcentric = true;
	unsigned int threshold___activefrontiers = 128;
	bool enable___collectactivedstvids = false; // false
	unsigned int threshold___activedstvids = 0xFFFFFFFF;
	unsigned int limits[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable = limits complete
	unsigned int max_limits[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable = max_limits complete
	unsigned int max_limit = 0; 
	
	unsigned int rootvid = 1; keyvalue_t root; root.key = rootvid; root.value = 0; keyvalue_t invalid; invalid.key = INVALIDDATA; invalid.value = INVALIDDATA; 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(v==1){ HBM_center->cfrontier_dram[v][0][0] = root; } else { HBM_center->cfrontier_dram[v][0][0] = invalid; } cfrontier_dram___size[0] = 1; }
	
	if(run_vertex_centric == true){ cout<<"### Vertex-Centric GAS iteration: 0 [1 active vertices]"<<endl; } else { cout<<"### Edge-Centric GAS iteration: 0 [1 active vertices]"<<endl; }
	
	// reset ubuffer 
	vprop_t prop; prop.prop = 0; prop.mask = INVALIDMASK; 
	for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int t=0; t<MAX_UPARTITION_VECSIZE; t++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ insert_udatabuffer(v, t, prop, &BRAM_channel[inst]); }}} // 
	
	// #ifdef __NOT__YET__IMPLEMENTED___
	// retrieve global parameters
	unsigned int globalparams[32];
	unsigned int updatesptrs[MAX_NUM_LLPSETS];
	#pragma HLS ARRAY_PARTITION variable = globalparams complete
	for(unsigned int t=0; t<32; t++){
		globalparams[t] = retrieve_globalparams(0, t, &HBM_channel[0]);
	}
	for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
		updatesptrs[t] = HBM_channel->HBM[globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS] + t].data[0];
		cout<<"--- updatesptrs["<<t<<"]: "<<updatesptrs[t]<<endl;
	}
	
	for(unsigned int GraphIter=0; GraphIter<12; GraphIter++){ // MAXNUMGRAPHITERATIONS
		unsigned int MASK_CODE = 1 + GraphIter;
		unsigned int MASK_CODE_PE = 1 + GraphIter;
		unsigned int MASK_CODE_AU = 4094 + 1 + GraphIter;
		
		// clear counters 
		for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
		#pragma HLS UNROLL
			for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){ 
				actpackupdates_dram___size[inst][p_v] = 0;
				stats_buffer___size[inst][p_v] = 0; stats2_buffer___size[inst][p_v] = 0;
				vpartition_vertices[inst][p_v].offset = 0; vpartition_vertices[inst][p_v].size = 0; vpartition_vertices[inst][p_v].count = 0;
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer___size[inst][v][p_v] = 0; }
			}	
			for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){ 
			#pragma HLS UNROLL
				for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ insert_statsbuffer(t, p_v, 0, &BRAM_channel[inst]); }
			}
		}
		
		// process-edges and partition-updates
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){
			keyvalue_t data[EDGE_PACK_SIZE];
			unsigned int data_u32[EDGE_PACK_SIZE];
			MASK_CODE_PE = ((1 + GraphIter) * MAX_NUM_UPARTITIONS) + p_u;
			bool ___use_vertex_centric___ = false; if(upartition_vertices[p_u].count < threshold___activefrontiers && enable___vertexcentric == true){ ___use_vertex_centric___ = true; }
	
			// broadcast active frontiers
			for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){
			#pragma HLS PIPELINE II=1
				retrieve_cfrontierdram(p_u, t, data, HBM_center);
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
				#pragma HLS UNROLL
					insert_cfrontierdram_tmp(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], t, data, &HBM_channel[inst]);
				}
			}
			
			// parallel-read active frontiers
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){
				#pragma HLS PIPELINE II=1
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
					#pragma HLS UNROLL
						retrieve_cfrontierdram_tmp(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], t, data, &HBM_channel[inst]);
						insert_cfrontierbuffer(t, data, &BRAM_channel[inst]); // 
					}
				}
			}
			
			// clear temp stats buffer
			if(___use_vertex_centric___ == false){
				for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
				#pragma HLS UNROLL
					for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ 
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
						#pragma HLS UNROLL
							insert_statstmpbuffer(t, v, 0, &BRAM_channel[inst]); 
						}
					}
				}
			}
			
			// load active frontiers: convert from vertexID-based to bitmask-based
			for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){
			#pragma HLS PIPELINE II=1
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
					keyvalue_t actvv[EDGE_PACK_SIZE];
					retrieve_cfrontierbuffer_i(t, actvv, &BRAM_channel[inst]); 
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						if(actvv[v].key != INVALIDDATA){
							unsigned int srcvid_lpv = get_local_to_upartition(actvv[v].key) / EDGE_PACK_SIZE; 
							vprop_t prop; prop.prop = actvv[v].value; prop.mask = MASK_CODE_PE; // NEW-MASK*
							insert_udatabuffer(v, srcvid_lpv, prop, &BRAM_channel[inst]); 
						}
					}
				}
			}
			
			for(unsigned int i=0; i<NUM_PEs; i++){ 
			#pragma HLS UNROLL
				for(unsigned int j=0; j<NUM_PEs; j++){ updates_tmpbuffer___size[i][j] = 0; } // reset
			} 
			
			// process-edges and partition-updates
			if(upartition_vertices[p_u].count > 0){
				if(___use_vertex_centric___ == true){
					hybrid_map[GraphIter][p_u] = 0;
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
					#pragma HLS UNROLL
						vptrbuffer___size[inst] = 0; edges_buffer___size[inst] = 0; // reset
					} 
					
					keyvalue_t vptrbuffer_md[NUM_PEs][VPTR_BUFFERMETADATA_SIZE]; for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int t=0; t<VPTR_BUFFERMETADATA_SIZE; t++){ vptrbuffer_md[inst][t].key = 0; vptrbuffer_md[inst][t].value = 0; }}
					unsigned int cummtv[NUM_PEs]; unsigned int cummtv2[NUM_PEs]; for(unsigned int inst=0; inst<NUM_PEs; inst++){ cummtv[inst] = 0; cummtv2[inst] = 0; }
					unsigned int vptrbatch_md_offset___size[NUM_PEs]; for(unsigned int inst=0; inst<NUM_PEs; inst++){ vptrbatch_md_offset___size[inst] = 0; }
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
						// load vptrs
						VC_READVPTRS_LOOP1B: for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){ 
							VC_READVPTRS_LOOP1: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							#pragma HLS PIPELINE II=1
								frontier_t actvv = retrieve_cfrontierbuffer_i(v, t, &BRAM_channel[inst]);
								unsigned int vid = actvv.key; 
								unsigned int hvid = vid / NUM_PEs;
								
								vtr_t data;
								if((actvv.key != INVALIDDATA) && (vid % NUM_PEs == inst)){ 
									// data.begin = retrieve_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], hvid, &HBM_channel[inst], inst);
									// data.end = retrieve_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], hvid+1, &HBM_channel[inst], inst);
									data.begin = retrieve_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], hvid, HBM_channel, inst);
									data.end = retrieve_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], hvid+1, HBM_channel, inst);
									data.size = data.end - data.begin;
									if(data.end < data.begin){ data.size = 0; }
									data.vid = actvv.key; 
									data.prop = actvv.value;
								
									vtr_t chunk = data; unsigned int __BATCH_SIZE = 1024;
									for(unsigned int b=0; b<(data.size + (__BATCH_SIZE - 1)) / __BATCH_SIZE; b++){
										if((b + 1) * __BATCH_SIZE > data.size){ chunk.size = data.size % __BATCH_SIZE; } else { chunk.size = __BATCH_SIZE; }
										chunk.begin = data.begin + b * __BATCH_SIZE;
										chunk.end = chunk.begin + chunk.size;
										checkoutofbounds("acts_sw::ERROR 003::", vptrbuffer___size[inst], VPTR_BUFFER_SIZE, NAp, NAp, NAp);
										insert_vptrbuffer(vptrbuffer___size[inst], chunk, &BRAM_channel[inst]);
										vptrbuffer___size[inst] += 1;
										cummtv2[inst] += chunk.size;
									}
								}
							}
						}
					}
						
					// calculate batches 
					max_limit = 0; for(unsigned int in=0; in<NUM_PEs; in++){ if(max_limit < cummtv2[in]){ max_limit = cummtv2[in]; }} 
					unsigned int num_batches = 1; unsigned int cummtv2_ = max_limit / EDGE_PACK_SIZE;
					if(cummtv2_ >= EDGE_BUFFER_SIZE){
						if(cummtv2_ / 4 < EDGE_BUFFER_SIZE){ num_batches = 4; }
						else if(cummtv2_ / 8 < EDGE_BUFFER_SIZE){ num_batches = 8; } 
						else if(cummtv2_ / 16 < EDGE_BUFFER_SIZE){ num_batches = 16; }
						else { cout<<"acts_sw:: ERROR 2367: suitable n not found. EXITING..."<<endl; exit(EXIT_FAILURE); }
					}
					max_limit = 0; for(unsigned int in=0; in<NUM_PEs; in++){ if(max_limit < vptrbuffer___size[in]){ max_limit = vptrbuffer___size[in]; }}
					unsigned int batch_size = (max_limit + (num_batches - 1)) / num_batches;
				
					// read and process edges 
					VC_READANDPROCESS_EDGES_LOOP1: for(unsigned int n=0; n<num_batches; n++){
						for(unsigned int in=0; in<NUM_PEs; in++){ 
						#pragma HLS UNROLL
							edges_buffer___size[in] = 0; // reset
						} 
					
						// load edges 
						VC_READ_EDGES_LOOP1: for(unsigned int k=0; k<batch_size; k++){ 
						#pragma HLS PIPELINE II=1
							for(unsigned int inst=0; inst<NUM_PEs; inst++){ if((n * batch_size) + k < vptrbuffer___size[inst]){ 
								vtr_t vptr_data = retrieve_vptrbuffer((n * batch_size) + k, &BRAM_channel[inst]); 
								unsigned int vid = vptr_data.vid;
								unsigned int edges_size = vptr_data.end - vptr_data.begin; 
								unsigned int edgelist_size = (edges_size / EDGE_PACK_SIZE) + 2; if((edges_size == 0) || (vptr_data.end < vptr_data.begin)){ edgelist_size = 0; }
								unsigned int uprop = vptr_data.prop;
								
								VC_READ_EDGES_LOOP1B: for(unsigned int t=0; t<edgelist_size; t++){
								#pragma HLS PIPELINE II=1
									edge3_vec_dt edges = retrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, &HBM_channel[inst]);
									edge3_type edge_list[EDGE_PACK_SIZE];
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										if(edges.data[v].srcvid == vid && edges.data[v].srcvid != INVALIDDATA){ edge_list[v] = edges.data[v]; edge_list[v].srcvid = uprop; } else { edge_list[v].srcvid = INVALIDDATA; edge_list[v].dstvid = INVALIDDATA; }
									}
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										checkoutofbounds("acts_sw::ERROR 812::", edges_buffer___size[inst], EDGE_BUFFER_SIZE, vptrbuffer___size[inst], inst, edgelist_size);
										BRAM_channel[inst].edges_buffer[v][edges_buffer___size[inst]] = edge_list[v];
									}
									edges_buffer___size[inst] += 1;
								}
							}} 
						}
							
						// process & partition edges
						max_limit = 0; for(unsigned int in=0; in<NUM_PEs; in++){ if(max_limit < edges_buffer___size[in]){ max_limit = edges_buffer___size[in]; }} 
						VC_PROCESS_EDGES_LOOP1: for(unsigned int t=0; t<max_limit; t++){ 
						#pragma HLS PIPELINE II=1
							for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(t < edges_buffer___size[inst]){ 
							#pragma HLS UNROLL
								for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
								#pragma HLS UNROLL
									edge3_type edge = BRAM_channel[inst].edges_buffer[v][t];
									if(edge.dstvid != INVALIDDATA){
										unsigned int uprop = edge.srcvid; // edge.srcvid has uprop information
										unsigned int res = uprop + 1;
										unsigned int vtemp = min(uprop, res);
										#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
										cout<<"VC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", uprop: "<<uprop<<", res: "<<res<<", vtemp: "<<vtemp<<"]"<<endl;
										cout<<"VC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [local partition: "<<p_u<<", vid: "<<NAp<<"]"<<endl;
										#endif 
										
										unsigned int inst_mirror = get_H(edge.dstvid);
										keyvalue_t update; update.key = edge.dstvid; update.value = vtemp; // res;
										checkoutofbounds("acts_sw::ERROR 813::", updates_tmpbuffer___size[inst][inst_mirror], CSRBUFFER_SIZE, NAp, NAp, NAp);
										insert_updatestmpbuffer(inst_mirror, updates_tmpbuffer___size[inst][inst_mirror], update, &BRAM_channel[inst]); // 
										updates_tmpbuffer___size[inst][inst_mirror] += 1;	
									}
								}
							}} 
						}
					}
					
					// exchange updates between HBMs
					VC_EXCHANGE_UPDATES_LOOP1: for(unsigned int inst_mirror=0; inst_mirror<NUM_PEs; inst_mirror++){
						VC_EXCHANGE_UPDATES_LOOP1B: for(unsigned int inst=0; inst<NUM_PEs; inst++){
							VC_EXCHANGE_UPDATES_LOOP1C: for(unsigned int t=0; t<updates_tmpbuffer___size[inst][inst_mirror]; t++){
								keyvalue_t update = retrieve_updatestmpbuffer(inst_mirror, t, &BRAM_channel[inst]); // 
								unsigned int p_v = get_local(update.key) / MAX_APPLYPARTITION_SIZE; // FIXME.
								keyvalue_t update_local = update; update_local.key = get_local_to_vpartition(get_local(update.key));
								unsigned int v = update_local.key % EDGE_PACK_SIZE;
								#ifdef _DEBUGMODE_KERNELPRINTS//4
								cout<<"VC - EXCHANGE UPDATE SEEN @: [inst: "<<inst<<", inst_mirror: "<<inst_mirror<<"], [update.key: "<<update.key<<", update.value: "<<update.value<<"]"<<endl;
								#endif 
								
								checkoutofbounds("acts_sw::ERROR 123::", p_v, __NUM_APPLYPARTITIONS, update.key, get_local(update.key), MAX_APPLYPARTITION_SIZE);
								checkoutofbounds("acts_sw::ERROR 122::", (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + updates_buffer___size[inst_mirror][v][p_v], MAX_APPLYPARTITION_VECSIZE, updates_buffer___size[inst_mirror][v][p_v], inst_mirror, p_v);
								insert_updatesbuffer(v, (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + updates_buffer___size[inst_mirror][v][p_v], update_local, &BRAM_channel[inst_mirror]);
								updates_buffer___size[inst_mirror][v][p_v] += 1;
								vpartition_vertices[0][p_v].count = 1;
								
								unsigned int dstvid_lpv = update_local.key / EDGE_PACK_SIZE; // FIXME.
								checkoutofbounds("acts_sw::ERROR 171::", dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE, BLOCKRAM_SIZE, NAp, NAp, NAp);
								checkoutofbounds("acts_sw::ERROR 172::", p_v, __NUM_APPLYPARTITIONS, NAp, NAp, NAp);
								if(enable___collectactivedstvids == true){ insert_statsbuffer(dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE, p_v, 1, &BRAM_channel[inst_mirror]); }
							}
						}
					}
				} else { // edge-centric [done]
					hybrid_map[GraphIter][p_u] = 1;
					EC_PROCESS_EDGES_LOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
						map_t maps[NUM_PEs];
						for(unsigned int n=0; n<NUM_PEs; n++){ 
						#pragma HLS UNROLL
							maps[n] = retrieve_actpackvptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS], p_u, llp_set, &HBM_channel[n]); 
						}
						unsigned int max_sz = 0; for(unsigned int n=0; n<NUM_PEs; n++){ limits[n] = maps[n].size; } for(unsigned int n=0; n<NUM_PEs; n++){ if(max_sz < limits[n]){ max_sz = limits[n]; }}
						
						// process edges [done]
						EC_PROCESS_EDGES_LOOP1B: for(unsigned int it=0; it<(max_sz + (BLOCKRAM_SIZE - 1)) / BLOCKRAM_SIZE; it++){
							for(unsigned int n=0; n<NUM_PEs; n++){ updates_tmpbuffer___size[n][0] = 0; } 
							unsigned int sz = 0; if((it + 1) * BLOCKRAM_SIZE >= max_sz){ sz = max_sz - (it * BLOCKRAM_SIZE); } else { sz = BLOCKRAM_SIZE; }
							
							EC_PROCESS_EDGES_LOOP1C: for(unsigned int t_=0; t_<sz; t_++){ 
							#pragma HLS PIPELINE II=1
								unsigned int t = (it * BLOCKRAM_SIZE) + t_;
								unsigned int offsets[NUM_PEs]; edge3_vec_dt edge3_vecs[NUM_PEs]; for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = maps[n].offset; }
								
								retrievemany_act_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs, HBM_channel);
								EC_PROCESS_EDGES_LOOP1D: for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(t < limits[inst]){
								#pragma HLS UNROLL
									edge3_vec_dt edge_vec = edge3_vecs[inst];
									unsigned int sample_key = edge_vec.data[0].dstvid % EDGE_PACK_SIZE; unsigned int sample_u = 0; unsigned int rotateby = sample_key - sample_u; unsigned int rotate_forward = 0; 	
									
									EC_PROCESS_EDGES_LOOP1E: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										if(edge_vec.data[v].valid == 1){
											edges[inst][v] = edge_vec.data[v]; 
											unsigned int srcvid_lp = get_local_to_upartition(edges[inst][v].srcvid);
											unsigned int srcvid_lpv = get_local_to_upartition(edges[inst][v].srcvid) / EDGE_PACK_SIZE;
											checkoutofbounds("acts_sw::ERROR 223::", srcvid_lp, MAX_UPARTITION_SIZE, NAp, NAp, NAp);
											if(srcvid_lp % EDGE_PACK_SIZE != v){ cout<<"acts_sw ERROR 2345:: srcvid_lp("<<srcvid_lp<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
											
											uprop[inst][v] = retrieve_udatabuffer(v, srcvid_lpv, &BRAM_channel[inst]);
											
											res[inst][v] = uprop[inst][v].prop + 1;
											vtemp_in[inst][v] = min(uprop[inst][v].prop, res[inst][v]);
											update_in[inst][v].key = edges[inst][v].dstvid; update_in[inst][v].value = vtemp_in[inst][v];
											if(uprop[inst][v].mask != MASK_CODE_PE){ update_in[inst][v].key = INVALIDDATA; update_in[inst][v].value = INVALIDDATA; } // NEW-MASK*
											#ifdef _DEBUGMODE_KERNELPRINTS//4
											if(uprop[inst][v].mask == MASK_CODE_PE){ cout<<"EC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [srcvid_lp: "<<srcvid_lp<<", dstvid: "<<edges[inst][v].dstvid % MAX_APPLYPARTITION_SIZE<<"], [edge.srcvid: "<<edges[inst][v].srcvid<<", dstvid: "<<edges[inst][v].dstvid<<"]"<<endl; }
											#endif 
										} else {
											uprop[inst][v].mask = INVALIDMASK; update_in[inst][v].key = INVALIDDATA; update_in[inst][v].value = INVALIDDATA;
										}
									}
									
									// circular shift >>>
									checkoutofbounds("acts_sw::ERROR 213::", rotateby, EDGE_PACK_SIZE, NAp, NAp, NAp);
									rearrangeLayoutVx16B(rotateby, update_in[inst], update_out[inst]);
									#ifdef _DEBUGMODE_CHECKS3
									checkoutofbounds("acts_sw::ERROR 59::", rotateby, EDGE_PACK_SIZE, sample_key, NAp, NAp);
									checkoutofbounds("acts_sw::ERROR 60::", rotate_forward, 2, sample_key, NAp, NAp);	
									check_if_contiguous(update_out[inst], update_in[inst], update_in[inst], sample_key, rotate_forward, rotateby);
									#endif 
									
									// partition and save updates
									uint512_vec_dt updates_vec; 
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										updates_vec.data[v] = update_out[inst][v];
										data_u32[v] = update_out[inst][v].key;
									}
									bool isvalid = false; if(data_u32[0]!=INVALIDDATA || data_u32[1]!=INVALIDDATA || data_u32[2]!=INVALIDDATA || data_u32[3]!=INVALIDDATA || data_u32[4]!=INVALIDDATA || data_u32[5]!=INVALIDDATA || data_u32[6]!=INVALIDDATA || data_u32[7]!=INVALIDDATA || data_u32[8]!=INVALIDDATA || data_u32[9]!=INVALIDDATA || data_u32[10]!=INVALIDDATA || data_u32[11]!=INVALIDDATA || data_u32[12]!=INVALIDDATA || data_u32[13]!=INVALIDDATA || data_u32[14]!=INVALIDDATA || data_u32[15]!=INVALIDDATA){ isvalid = true; }			
									// if(isvalid == true){ insert_updatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[llp_set], llp_set, actpackupdates_dram___size[inst][llp_set], updates_vec, &HBM_channel[inst], globalparams, updatesptrs); actpackupdates_dram___size[inst][llp_set] += 1; } // 
									
									if(isvalid == true){ // NEW
										for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
										#pragma HLS UNROLL
											insert_updatestmpbuffer(v, updates_tmpbuffer___size[inst][0], updates_vec.data[v], &BRAM_channel[inst]); //
										}
										updates_tmpbuffer___size[inst][0] += 1;
									}
									
									// collect stats information 
									if(enable___collectactivedstvids == true){
										for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
										#pragma HLS UNROLL
											if(update_out[inst][v].key != INVALIDDATA){
												unsigned int dstvid_lpv = update_out[inst][v].key / EDGE_PACK_SIZE; 
												unsigned int index = dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE;
												checkoutofbounds("acts_sw::ERROR 623::", index, BLOCKRAM_SIZE, update_out[inst][v].key, dstvid_lpv, (MAX_APPLYPARTITION_VECSIZE / BLOCKRAM_SIZE));
												insert_statstmpbuffer(index, v, 1, &BRAM_channel[inst]);
											}
										}
									}
								}}
							}
						
							uint512_vec_dt updates_vec; uint512_vec_dt updates_vecs[NUM_PEs];
							max_limit = 0; for(unsigned int n=0; n<NUM_PEs; n++){ if(max_limit < updates_tmpbuffer___size[n][0]){ max_limit = updates_tmpbuffer___size[n][0]; }}
							unsigned int offsets[NUM_PEs]; for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = actpackupdates_dram___size[n][llp_set]; }
							EC_PROCESS_EDGES_LOOP1F: for(unsigned int t=0; t<max_limit; t++){ 
							#pragma HLS PIPELINE II=1
								EC_PROCESS_EDGES_LOOP1G: for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
								#pragma HLS UNROLL
									EC_PROCESS_EDGES_LOOP1H: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										updates_vecs[inst].data[v] = retrieve_updatestmpbuffer(v, t, &BRAM_channel[inst]);
										if(t < updates_tmpbuffer___size[inst][0]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
									}
								}
								insertmany_updatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[llp_set], llp_set, offsets, t, updates_vecs, ens, HBM_channel, updatesptrs);
								for(unsigned int n=0; n<NUM_PEs; n++){ 
								#pragma HLS UNROLL
									if(t < updates_tmpbuffer___size[n][0]){ actpackupdates_dram___size[n][llp_set] += 1; }
								}	
							}	
						}
						
						// gather stats [done]
						unsigned int data[EDGE_PACK_SIZE];
						EC_PROCESS_EDGES_LOOP1I: for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
						#pragma HLS PIPELINE II=1
							for(unsigned int inst=0; inst<NUM_PEs; inst++){
							#pragma HLS UNROLL
								if(enable___collectactivedstvids == true){ retrieve_statstmpbuffer(t, data, &BRAM_channel[inst]); }
								checkoutofbounds("acts_sw::ERROR 023::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
								if(data[0]==1 || data[1]==1 || data[2]==1 || data[3]==1 || data[4]==1 || data[5]==1 || data[6]==1 || data[7]==1 || data[8]==1 || data[9]==1 || data[10]==1 || data[11]==1 || data[12]==1 || data[13]==1 || data[14]==1 || data[15]==1){ 
									if(enable___collectactivedstvids == true){ insert_statsbuffer(t, llp_set, 1, &BRAM_channel[inst]); }
									vpartition_vertices[inst][llp_set].count = 1;
								}
							}
						}
					}
				}
			}
		}
		
		// save csr updates [done]
		SAVE_CSR_UPDATES_LOOP1: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			unsigned int offsets[NUM_PEs]; for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = actpackupdates_dram___size[n][p_v]; } // NEW
			uint512_vec_dt updates_vecs[NUM_PEs]; 
			max_limit = 0; for(unsigned int inst=0; inst<NUM_PEs; inst++){ max_limits[inst] = 0; }
			SAVE_CSR_UPDATES_LOOP1B: for(unsigned int inst=0; inst<NUM_PEs; inst++){
			#pragma HLS UNROLL
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(max_limits[inst] < updates_buffer___size[inst][v][p_v]){ max_limits[inst] = updates_buffer___size[inst][v][p_v]; }}
			} 
			SAVE_CSR_UPDATES_LOOP1C: for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(max_limit < max_limits[inst]){ max_limit = max_limits[inst]; }}
			
			SAVE_CSR_UPDATES_LOOP1D: for(unsigned int t=0; t<max_limit; t++){
			#pragma HLS PIPELINE II=1
				SAVE_CSR_UPDATES_LOOP1E: for(unsigned int inst=0; inst<NUM_PEs; inst++){
				#pragma HLS UNROLL
					SAVE_CSR_UPDATES_LOOP1F: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						if(t < updates_buffer___size[inst][v][p_v]){ updates_vecs[inst].data[v] = retrieve_updatesbuffer(v, (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + t, &BRAM_channel[inst]); }
						else { updates_vecs[inst].data[v].key = INVALIDDATA; updates_vecs[inst].data[v].value = INVALIDDATA;  }
						if(t < max_limits[inst]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						if(t < updates_buffer___size[inst][v][p_v]){ cout<<"VC - COMMIT CSR UPDATES SEEN @: [inst: "<<inst<<"], [updates_vecs["<<inst<<"].data["<<v<<"].key: "<<updates_vecs[inst].data[v].key<<", updates_vecs["<<inst<<"].data["<<v<<"].value: "<<updates_vecs[inst].data[v].value<<"]"<<endl; }
						#endif
					}
				}
				checkoutofbounds("acts_sw::ERROR 152::", actpackupdates_dram___size[0][p_v], HBM_CHANNEL_SIZE, NAp, NAp, NAp);
				insertmany_updatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v], p_v, offsets, t, updates_vecs, ens, HBM_channel, updatesptrs);
				for(unsigned int n=0; n<NUM_PEs; n++){ 
				#pragma HLS UNROLL
					if(t < max_limits[n]){ actpackupdates_dram___size[n][p_v] += 1; }
				}
			}
		}
		
		// prepare active destID stats [done]
		if(enable___collectactivedstvids == true){
			PREPARE_ACTIVE_DESTIDSTATS_LOOP1C: for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
			#pragma HLS PIPELINE II=1
				PREPARE_ACTIVE_DESTIDSTATS_LOOP1D: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
				#pragma HLS UNROLL
					PREPARE_ACTIVE_DESTIDSTATS_LOOP1E: for(unsigned int inst=0; inst<NUM_PEs; inst++){
					#pragma HLS UNROLL
						if(retrieve_statsbuffer(t, p_v, &BRAM_channel[inst]) == 1){ 
							#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
							cout<<"VC - PREPARE ACTIVE DESTIDS SEEN @: [inst: "<<inst<<"], [p_v: "<<p_v<<", t: "<<t<<"]"<<endl;
							#endif
							insert_stats2buffer(stats2_buffer___size[inst][p_v], p_v, t, &BRAM_channel[inst]); 
							stats2_buffer___size[inst][p_v] += 1;
						}
					}
				}
			}
		}
		
		// apply updates
		for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			bool en = true; if(enable___collectactivedstvids == true){ if(vpartition_vertices[0][p_v].count > 0){ en=true; } else { en=false; }} else { en = true; }
			unsigned int voffset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS] + (p_v * MAX_APPLYPARTITION_VECSIZE);
			
			if(vpartition_vertices[0][p_v].count > 0){
				// read destination properties
				vprop_t data[EDGE_PACK_SIZE];
				if(stats_buffer___size[0][p_v] <= threshold___activedstvids && enable___collectactivedstvids == true){
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ // FIXME.
						for(unsigned int k=0; k<stats2_buffer___size[inst][p_v]; k++){
							unsigned int offset = retrieve_stats2buffer(k, p_v, &BRAM_channel[inst]) * ACTVUPDATESBLOCK_VECSIZE;
							checkoutofbounds("acts_sw::ERROR 117::", offset, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
							for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								if(offset + t < MAX_APPLYPARTITION_VECSIZE){ 
									retrieve_vdatadram(voffset, p_v, offset + t, data, &HBM_channel[inst]);
									insert_vdatabuffer(offset + t, data, &BRAM_channel[inst]);
								}
							}
						}	
					}
				} else {
					for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
					#pragma HLS PIPELINE II=1
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
						#pragma HLS UNROLL
							retrieve_vdatadram(voffset, p_v, t, data, &HBM_channel[inst]);
							insert_vdatabuffer(t, data, &BRAM_channel[inst]);
						}
					}
				}
				
				// apply updates [done]
				uint512_vec_dt updates_vecs[NUM_PEs];
				max_limit = 0; for(unsigned int inst=0; inst<NUM_PEs; inst++){ limits[inst] = actpackupdates_dram___size[inst][p_v]; } for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}
				for(unsigned int t=0; t<max_limit; t++){
				#pragma HLS PIPELINE II=1
					retrievemany_udatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v], p_v, t, updates_vecs, HBM_channel, updatesptrs); // NEW
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
					#pragma HLS UNROLL
						uint512_vec_dt updates_vec = updates_vecs[inst];
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							keyvalue_t update = updates_vec.data[v];
							unsigned int dstvid_lp = update.key; 
							unsigned int dstvid_lpv = update.key / EDGE_PACK_SIZE; 
							if(update.key != INVALIDDATA){
								if(t < limits[inst] && ((dstvid_lp % EDGE_PACK_SIZE) != v)){ cout<<"acts_sw 234:: dstvid_lp("<<dstvid_lp<<") % v("<<v<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }
								if(t < limits[inst]){ checkoutofbounds("acts_sw::ERROR 727::", dstvid_lp, MAX_APPLYPARTITION_SIZE, NAp, inst, update.key); }
								vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, &BRAM_channel[inst]);
								unsigned int new_vprop = 0; if(update.value < vprop.prop){ new_vprop = update.value; } else { new_vprop = vprop.prop; }
								if(new_vprop != vprop.prop){
									#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
									cout<<"APPLY (ACT-PACK) UPDATE SEEN @: inst: ["<<inst<<"]: dstvid_lp: "<<dstvid_lp<<", dstvid_lpv: "<<dstvid_lpv<<", new_vprop: "<<new_vprop<<", vid: "<<update.key<<endl;
									#endif
									vprop_t newprop; newprop.prop = new_vprop; newprop.mask = MASK_CODE_AU;
									if(t < limits[inst]){ insert_vdatabuffer(v, dstvid_lpv, newprop, &BRAM_channel[inst]); } // NEW
								}
							}
						}
					}
				}

				// collect frontier information
				unsigned int vid_first0[NUM_PEs];
				#pragma HLS ARRAY_PARTITION variable = vid_first0 complete
				unsigned int vid_first1[NUM_PEs]; 
				#pragma HLS ARRAY_PARTITION variable = vid_first1 complete
				unsigned int CONST1 = NUM_PEs * EDGE_PACK_SIZE; unsigned int CONST2 = MAX_ACTVV_VECSIZE * NUM_PEs * EDGE_PACK_SIZE;
				for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
				#pragma HLS UNROLL
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
					#pragma HLS UNROLL
						for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){ nfrontier_buffer___size[inst][v][p_actvv] = 0; }
					}
				}
				for(unsigned int inst=0; inst<NUM_PEs; inst++){ vid_first0[inst] = get_global(((p_v * MAX_APPLYPARTITION_SIZE) + ((0 * MAX_ACTVV_VECSIZE + 0) * EDGE_PACK_SIZE + 0)), inst); }
				for(unsigned int n=0; n<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; n++){
					max_limit = 0; for(unsigned int inst=0; inst<NUM_PEs; inst++){ limits[inst] = MAX_ACTVV_VECSIZE; } for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ vid_first1[inst] = vid_first0[inst]; }
					for(unsigned int t=0; t<max_limit; t++){
					#pragma HLS PIPELINE II=1
						unsigned int dstvid_lpv = n * MAX_ACTVV_VECSIZE + t;
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(t < limits[inst]){
						#pragma HLS UNROLL
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							#pragma HLS UNROLL
								checkoutofbounds("acts_sw::ERROR 713::", dstvid_lpv, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
								vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, &BRAM_channel[inst]);
								unsigned int vid = vid_first1[inst] + v;
								#ifdef _DEBUGMODE_CHECKS3
								if(vid % EDGE_PACK_SIZE != v){ cout<<"acts_sw:: ERROR 234. vid("<<vid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<vid % EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
								#endif
								unsigned int p_actvv = dstvid_lpv / MAX_ACTVV_VECSIZE;
								if(vprop.mask == MASK_CODE_AU){ // NEW-MASK*
									#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
									cout<<"COLLECT FRONTIER INFORMATION SEEN @: inst: ["<<inst<<"]: dstvid_lpv: "<<dstvid_lpv<<", v: "<<v<<", p__u__: "<<(vid / MAX_UPARTITION_SIZE)<<", vid: "<<vid<<endl;
									#endif
									frontier_t actvv; actvv.key = vid; actvv.value = vprop.prop; 
									insert_nfrontierbuffer((p_actvv * MAX_ACTVV_VECSIZE) + nfrontier_buffer___size[inst][v][p_actvv], v, actvv, &BRAM_channel[inst]); // 
									nfrontier_buffer___size[inst][v][p_actvv] += 1;
								}
							}
						}}
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
						#pragma HLS UNROLL
							vid_first1[inst] += CONST1; 
						} 
					}
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
					#pragma HLS UNROLL
						vid_first0[inst] += CONST2; 
					}
				}
				
				// commit frontier information
				frontier_t actvv[EDGE_PACK_SIZE];
				for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
					unsigned int max_limit = 0; for(unsigned int inst=0; inst<NUM_PEs; inst++){ max_limits[inst] = 0; }
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
						for(unsigned int inst=0; inst<NUM_PEs; inst++){
						#pragma HLS UNROLL
							if(max_limits[inst] < nfrontier_buffer___size[inst][v][p_actvv]){ max_limits[inst] = nfrontier_buffer___size[inst][v][p_actvv]; }
						}
					}
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(max_limit < max_limits[inst]){ max_limit = max_limits[inst]; }}
					unsigned int p_actvv_ = (p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION) + p_actvv;
					unsigned int offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + (p_actvv_ * MAX_APPLYPARTITION_VECSIZE);
					for(unsigned int t=0; t<max_limit; t++){
					#pragma HLS PIPELINE II=1
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(t < max_limits[inst]){
							retrieve_nfrontierbuffer(p_actvv * MAX_ACTVV_VECSIZE + t, actvv, &BRAM_channel[inst]);
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ // 
							#pragma HLS UNROLL
								if(t >= nfrontier_buffer___size[inst][v][p_actvv]){ actvv[v].key = INVALIDDATA; actvv[v].value = INVALIDDATA; }
							}
							insert_nfrontierdram(offset, p_actvv_, nfrontier_dram___size[inst][0][p_actvv_], actvv, &HBM_channel[inst]);
							nfrontier_dram___size[inst][0][p_actvv_] += 1;
						}}
					}
				}
				
				// commit destination properties
				if(stats_buffer___size[0][p_v] <= threshold___activedstvids && enable___collectactivedstvids == true){
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ // FIXME.
						for(unsigned int k=0; k<stats2_buffer___size[inst][p_v]; k++){	
							unsigned int offset = retrieve_stats2buffer(k, p_v, &BRAM_channel[inst]) * ACTVUPDATESBLOCK_VECSIZE;
							for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								if(offset + t < MAX_APPLYPARTITION_VECSIZE){ 
									checkoutofbounds("acts_sw::ERROR 217::", offset + t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
									retrieve_vdatabuffer(offset + t, data, &BRAM_channel[inst]);
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
									#pragma HLS UNROLL
										data[v].mask = INVALIDMASK; // clear mask. // NEW-MASK*
									} 
									insert_vdatadram(voffset, p_v, offset + t, data, &HBM_channel[inst]);
								}
							}
						}	
					}
				} else {
					for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
					#pragma HLS PIPELINE II=1
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
						#pragma HLS UNROLL
							retrieve_vdatabuffer(t, data, &BRAM_channel[inst]);
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
							#pragma HLS UNROLL
								data[v].mask = INVALIDMASK; // clear mask. NEW-MASK*
							} 
							insert_vdatadram(voffset, p_v, t, data, &HBM_channel[inst]);
						}
					}
				}
			}
		} // p_v
		
		// end of GAS iteration - gathering frontier information
		unsigned int totalactvvs2 = 0; frontier_t actvv[EDGE_PACK_SIZE]; 
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ upartition_vertices[p_u].count = 0; }
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ cfrontier_dram___size[p_u] = 0; } // reset
		for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			for(unsigned int inst=0; inst<NUM_PEs; inst++){
				for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
					unsigned int p_actvv_ = p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION + p_actvv;
					checkoutofbounds("acts_sw::ERROR 225::", p_actvv_, MAX_NUM_UPARTITIONS, NAp, NAp, NAp);
					unsigned int offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + (p_actvv_ * MAX_APPLYPARTITION_VECSIZE);
					for(unsigned int t=0; t<nfrontier_dram___size[inst][0][p_actvv_]; t++){
					#pragma HLS PIPELINE II=1
						upartition_vertices[p_actvv_].count += 1; 
						retrieve_nfrontierdram(offset, p_actvv_, t, actvv, &HBM_channel[inst]);
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						for(unsigned int v=0; v<0; v++){ cout<<"TRANSFER VID-BASED FRONTIERS SEEN @: H: "<<inst<<" => vid: "<<actvv[v].key<<", p_actvv_: "<<p_actvv_<<endl; }
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA && actvv[v].key / MAX_UPARTITION_SIZE != p_actvv_){ cout<<"ERROR 234: actvv["<<v<<"].key("<<actvv[v].key<<") / MAX_UPARTITION_SIZE("<<MAX_UPARTITION_SIZE<<")(="<<actvv[v].key / MAX_UPARTITION_SIZE<<") != p_actvv_("<<p_actvv_<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }}				
						#endif 
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA){ totalactvvs2 += 1; }} // FIXME.
						insert_cfrontierdram(p_actvv_, cfrontier_dram___size[p_actvv_], actvv, HBM_center); cfrontier_dram___size[p_actvv_] += 1;
					}
				}
			}
		}
		
		for(unsigned int t=0; t<__NUM_UPARTITIONS; t++){ if(hybrid_map[GraphIter][t] == 0){ cout<<"V, "; } else if(hybrid_map[GraphIter][t] == 1){ cout<<"E, "; } else { cout<<"-, "; }} cout<<endl;
		if(run_vertex_centric == true){ cout<<"### Vertex-Centric GAS iteration: "<<GraphIter + 1<<" ["<<totalactvvs2<<" active vertices]"<<endl; } else { cout<<"### Edge-Centric GAS iteration: "<<GraphIter + 1<<" ["<<totalactvvs2<<" active vertices]"<<endl;	 }
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ if(upartition_vertices[p_u].count > 0){ cout<<"1, "; } else { cout<<"0, "; }} cout<<endl;
		if(totalactvvs2 == 0 || GraphIter >= MAXNUMGRAPHITERATIONS){ cout<<"no more active vertices to process. breaking out... "<<endl; maxGraphIter = GraphIter; break; }
		for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ nfrontier_dram___size[inst][0][p_u] = 0; }}
	} // NUM_ITs
	return;
}












