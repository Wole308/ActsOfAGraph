#include "acts_kernel.h"
using namespace std;
// __NOT__YET__IMPLEMENTED___
// __OBSOLETE___

// #define ___ENABLE___COLLECTACTIVEDSTVIDS___

unsigned int * globalparams_debug;

#ifndef FPGA_IMPL
acts_kernel::acts_kernel(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
	algorithmobj = new algorithm();
}
acts_kernel::~acts_kernel(){} 
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
void M_I(unsigned int offset, unsigned int v, unsigned int data, HBM_channel_t * channel){
	channel[offset].data[v] = data;
	return;
}
unsigned int M_R(unsigned int offset, unsigned int v, HBM_channel_t * channel){
	return channel[offset].data[v];
}
void M_Is(unsigned int offset, unsigned int v, unsigned int data, HBM_channelTHIS_t * channel, unsigned int inst){
	channel[offset].data[v] = data;
	return;
}
unsigned int M_Rs(unsigned int offset, unsigned int v, HBM_channelTHIS_t * channel, unsigned int inst){
	return channel[offset].data[v];
}

#ifdef __NOT__YET__IMPLEMENTED___
//////////////////////////////////////////////////////////////////
void master_centerinsert_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channel_t * HBM_center){
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
		M_I(offset, v, data[v], HBM_center);
	}
	return;
}
void master_centerretrieve_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channel_t * HBM_center){
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v] = M_R(offset, v, HBM_center);
	}
	return;
}

void master_insert(unsigned int offset, unsigned int v, unsigned int data, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef ___USE_AXI_CHANNEL___
		if(v < HBM_AXI_PACK_SIZE){
			M_Is(offset, v, data, HBM_channelA, inst);
		} else {
			M_Is(offset, v - HBM_AXI_PACK_SIZE, data, HBM_channelB, inst);
		}
	#else 
	M_Is(offset, v, data, HBM_channelA, inst);
	#endif 
	return;
}
unsigned int master_retrieve(unsigned int offset, unsigned int v, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	unsigned int data = 0;
	#ifdef ___USE_AXI_CHANNEL___
		if(v < HBM_AXI_PACK_SIZE){
			data = M_Rs(offset, v, HBM_channelA, inst);
		} else {
			data = M_Rs(offset, v - HBM_AXI_PACK_SIZE, HBM_channelB, inst);
		}
	#else 
	data = M_Rs(offset, v, HBM_channelA, inst);
	#endif 
	return data;
}
void master_insertmany(unsigned int offsets[NUM_PEs], unsigned int v, unsigned int ens[NUM_PEs], unsigned int data[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef ___USE_AXI_CHANNEL___
		if(v < HBM_AXI_PACK_SIZE){
				M_Is(offsets[n], v, data[0], HBM_channelA0, 0);
				M_Is(offsets[n], v, data[1], HBM_channelA1, 1);
				M_Is(offsets[n], v, data[2], HBM_channelA2, 2);
				M_Is(offsets[n], v, data[3], HBM_channelA3, 3);
				M_Is(offsets[n], v, data[4], HBM_channelA4, 4);
				M_Is(offsets[n], v, data[5], HBM_channelA5, 5);
				M_Is(offsets[n], v, data[6], HBM_channelA6, 6);
				M_Is(offsets[n], v, data[7], HBM_channelA7, 7);
				M_Is(offsets[n], v, data[8], HBM_channelA8, 8);
				M_Is(offsets[n], v, data[9], HBM_channelA9, 9);
				M_Is(offsets[n], v, data[10], HBM_channelA10, 10);
				M_Is(offsets[n], v, data[11], HBM_channelA11, 11);
				M_Is(offsets[n], v, data[12], HBM_channelA12, 12);
				M_Is(offsets[n], v, data[13], HBM_channelA13, 13);
				M_Is(offsets[n], v, data[14], HBM_channelA14, 14);
				M_Is(offsets[n], v, data[15], HBM_channelA15, 15);
				M_Is(offsets[n], v, data[16], HBM_channelA16, 16);
				M_Is(offsets[n], v, data[17], HBM_channelA17, 17);
				M_Is(offsets[n], v, data[18], HBM_channelA18, 18);
				M_Is(offsets[n], v, data[19], HBM_channelA19, 19);
				M_Is(offsets[n], v, data[20], HBM_channelA20, 20);
				M_Is(offsets[n], v, data[21], HBM_channelA21, 21);
				M_Is(offsets[n], v, data[22], HBM_channelA22, 22);
				M_Is(offsets[n], v, data[23], HBM_channelA23, 23);
		} else {
				M_Is(offsets[0], v - HBM_AXI_PACK_SIZE, data[0], HBM_channelB0, 0);
				M_Is(offsets[1], v - HBM_AXI_PACK_SIZE, data[1], HBM_channelB1, 1);
				M_Is(offsets[2], v - HBM_AXI_PACK_SIZE, data[2], HBM_channelB2, 2);
				M_Is(offsets[3], v - HBM_AXI_PACK_SIZE, data[3], HBM_channelB3, 3);
				M_Is(offsets[4], v - HBM_AXI_PACK_SIZE, data[4], HBM_channelB4, 4);
				M_Is(offsets[5], v - HBM_AXI_PACK_SIZE, data[5], HBM_channelB5, 5);
				M_Is(offsets[6], v - HBM_AXI_PACK_SIZE, data[6], HBM_channelB6, 6);
				M_Is(offsets[7], v - HBM_AXI_PACK_SIZE, data[7], HBM_channelB7, 7);
				M_Is(offsets[8], v - HBM_AXI_PACK_SIZE, data[8], HBM_channelB8, 8);
				M_Is(offsets[9], v - HBM_AXI_PACK_SIZE, data[9], HBM_channelB9, 9);
				M_Is(offsets[10], v - HBM_AXI_PACK_SIZE, data[10], HBM_channelB10, 10);
				M_Is(offsets[11], v - HBM_AXI_PACK_SIZE, data[11], HBM_channelB11, 11);
				M_Is(offsets[12], v - HBM_AXI_PACK_SIZE, data[12], HBM_channelB12, 12);
				M_Is(offsets[13], v - HBM_AXI_PACK_SIZE, data[13], HBM_channelB13, 13);
				M_Is(offsets[14], v - HBM_AXI_PACK_SIZE, data[14], HBM_channelB14, 14);
				M_Is(offsets[15], v - HBM_AXI_PACK_SIZE, data[15], HBM_channelB15, 15);
				M_Is(offsets[16], v - HBM_AXI_PACK_SIZE, data[16], HBM_channelB16, 16);
				M_Is(offsets[17], v - HBM_AXI_PACK_SIZE, data[17], HBM_channelB17, 17);
				M_Is(offsets[18], v - HBM_AXI_PACK_SIZE, data[18], HBM_channelB18, 18);
				M_Is(offsets[19], v - HBM_AXI_PACK_SIZE, data[19], HBM_channelB19, 19);
				M_Is(offsets[20], v - HBM_AXI_PACK_SIZE, data[20], HBM_channelB20, 20);
				M_Is(offsets[21], v - HBM_AXI_PACK_SIZE, data[21], HBM_channelB21, 21);
				M_Is(offsets[22], v - HBM_AXI_PACK_SIZE, data[22], HBM_channelB22, 22);
				M_Is(offsets[23], v - HBM_AXI_PACK_SIZE, data[23], HBM_channelB23, 23);
		}
	#else 
		if(ens[0]==true){
			M_Is(offsets[0], v, data[0], HBM_channelA0, 0);
		}
		if(ens[1]==true){
			M_Is(offsets[1], v, data[1], HBM_channelA1, 1);
		}
		if(ens[2]==true){
			M_Is(offsets[2], v, data[2], HBM_channelA2, 2);
		}
		if(ens[3]==true){
			M_Is(offsets[3], v, data[3], HBM_channelA3, 3);
		}
		if(ens[4]==true){
			M_Is(offsets[4], v, data[4], HBM_channelA4, 4);
		}
		if(ens[5]==true){
			M_Is(offsets[5], v, data[5], HBM_channelA5, 5);
		}
		if(ens[6]==true){
			M_Is(offsets[6], v, data[6], HBM_channelA6, 6);
		}
		if(ens[7]==true){
			M_Is(offsets[7], v, data[7], HBM_channelA7, 7);
		}
		if(ens[8]==true){
			M_Is(offsets[8], v, data[8], HBM_channelA8, 8);
		}
		if(ens[9]==true){
			M_Is(offsets[9], v, data[9], HBM_channelA9, 9);
		}
		if(ens[10]==true){
			M_Is(offsets[10], v, data[10], HBM_channelA10, 10);
		}
		if(ens[11]==true){
			M_Is(offsets[11], v, data[11], HBM_channelA11, 11);
		}
		if(ens[12]==true){
			M_Is(offsets[12], v, data[12], HBM_channelA12, 12);
		}
		if(ens[13]==true){
			M_Is(offsets[13], v, data[13], HBM_channelA13, 13);
		}
		if(ens[14]==true){
			M_Is(offsets[14], v, data[14], HBM_channelA14, 14);
		}
		if(ens[15]==true){
			M_Is(offsets[15], v, data[15], HBM_channelA15, 15);
		}
		if(ens[16]==true){
			M_Is(offsets[16], v, data[16], HBM_channelA16, 16);
		}
		if(ens[17]==true){
			M_Is(offsets[17], v, data[17], HBM_channelA17, 17);
		}
		if(ens[18]==true){
			M_Is(offsets[18], v, data[18], HBM_channelA18, 18);
		}
		if(ens[19]==true){
			M_Is(offsets[19], v, data[19], HBM_channelA19, 19);
		}
		if(ens[20]==true){
			M_Is(offsets[20], v, data[20], HBM_channelA20, 20);
		}
		if(ens[21]==true){
			M_Is(offsets[21], v, data[21], HBM_channelA21, 21);
		}
		if(ens[22]==true){
			M_Is(offsets[22], v, data[22], HBM_channelA22, 22);
		}
		if(ens[23]==true){
			M_Is(offsets[23], v, data[23], HBM_channelA23, 23);
		}
	#endif 
	return;
}
void master_retrievemany(unsigned int offsets[NUM_PEs], unsigned int v, unsigned int data[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef ___USE_AXI_CHANNEL___
		if(v < HBM_AXI_PACK_SIZE){
				data[0] = M_Rs(offsets[0], v, HBM_channelA0, 0);
				data[1] = M_Rs(offsets[1], v, HBM_channelA1, 1);
				data[2] = M_Rs(offsets[2], v, HBM_channelA2, 2);
				data[3] = M_Rs(offsets[3], v, HBM_channelA3, 3);
				data[4] = M_Rs(offsets[4], v, HBM_channelA4, 4);
				data[5] = M_Rs(offsets[5], v, HBM_channelA5, 5);
				data[6] = M_Rs(offsets[6], v, HBM_channelA6, 6);
				data[7] = M_Rs(offsets[7], v, HBM_channelA7, 7);
				data[8] = M_Rs(offsets[8], v, HBM_channelA8, 8);
				data[9] = M_Rs(offsets[9], v, HBM_channelA9, 9);
				data[10] = M_Rs(offsets[10], v, HBM_channelA10, 10);
				data[11] = M_Rs(offsets[11], v, HBM_channelA11, 11);
				data[12] = M_Rs(offsets[12], v, HBM_channelA12, 12);
				data[13] = M_Rs(offsets[13], v, HBM_channelA13, 13);
				data[14] = M_Rs(offsets[14], v, HBM_channelA14, 14);
				data[15] = M_Rs(offsets[15], v, HBM_channelA15, 15);
				data[16] = M_Rs(offsets[16], v, HBM_channelA16, 16);
				data[17] = M_Rs(offsets[17], v, HBM_channelA17, 17);
				data[18] = M_Rs(offsets[18], v, HBM_channelA18, 18);
				data[19] = M_Rs(offsets[19], v, HBM_channelA19, 19);
				data[20] = M_Rs(offsets[20], v, HBM_channelA20, 20);
				data[21] = M_Rs(offsets[21], v, HBM_channelA21, 21);
				data[22] = M_Rs(offsets[22], v, HBM_channelA22, 22);
				data[23] = M_Rs(offsets[23], v, HBM_channelA23, 23);
		} else {
				data[0] = M_Rs(offsets[0], v - HBM_AXI_PACK_SIZE, HBM_channelB0, 0);
				data[1] = M_Rs(offsets[1], v - HBM_AXI_PACK_SIZE, HBM_channelB1, 1);
				data[2] = M_Rs(offsets[2], v - HBM_AXI_PACK_SIZE, HBM_channelB2, 2);
				data[3] = M_Rs(offsets[3], v - HBM_AXI_PACK_SIZE, HBM_channelB3, 3);
				data[4] = M_Rs(offsets[4], v - HBM_AXI_PACK_SIZE, HBM_channelB4, 4);
				data[5] = M_Rs(offsets[5], v - HBM_AXI_PACK_SIZE, HBM_channelB5, 5);
				data[6] = M_Rs(offsets[6], v - HBM_AXI_PACK_SIZE, HBM_channelB6, 6);
				data[7] = M_Rs(offsets[7], v - HBM_AXI_PACK_SIZE, HBM_channelB7, 7);
				data[8] = M_Rs(offsets[8], v - HBM_AXI_PACK_SIZE, HBM_channelB8, 8);
				data[9] = M_Rs(offsets[9], v - HBM_AXI_PACK_SIZE, HBM_channelB9, 9);
				data[10] = M_Rs(offsets[10], v - HBM_AXI_PACK_SIZE, HBM_channelB10, 10);
				data[11] = M_Rs(offsets[11], v - HBM_AXI_PACK_SIZE, HBM_channelB11, 11);
				data[12] = M_Rs(offsets[12], v - HBM_AXI_PACK_SIZE, HBM_channelB12, 12);
				data[13] = M_Rs(offsets[13], v - HBM_AXI_PACK_SIZE, HBM_channelB13, 13);
				data[14] = M_Rs(offsets[14], v - HBM_AXI_PACK_SIZE, HBM_channelB14, 14);
				data[15] = M_Rs(offsets[15], v - HBM_AXI_PACK_SIZE, HBM_channelB15, 15);
				data[16] = M_Rs(offsets[16], v - HBM_AXI_PACK_SIZE, HBM_channelB16, 16);
				data[17] = M_Rs(offsets[17], v - HBM_AXI_PACK_SIZE, HBM_channelB17, 17);
				data[18] = M_Rs(offsets[18], v - HBM_AXI_PACK_SIZE, HBM_channelB18, 18);
				data[19] = M_Rs(offsets[19], v - HBM_AXI_PACK_SIZE, HBM_channelB19, 19);
				data[20] = M_Rs(offsets[20], v - HBM_AXI_PACK_SIZE, HBM_channelB20, 20);
				data[21] = M_Rs(offsets[21], v - HBM_AXI_PACK_SIZE, HBM_channelB21, 21);
				data[22] = M_Rs(offsets[22], v - HBM_AXI_PACK_SIZE, HBM_channelB22, 22);
				data[23] = M_Rs(offsets[23], v - HBM_AXI_PACK_SIZE, HBM_channelB23, 23);
		}
	#else 
		data[0] = M_Rs(offsets[0], v, HBM_channelA0, 0);
		data[1] = M_Rs(offsets[1], v, HBM_channelA1, 1);
		data[2] = M_Rs(offsets[2], v, HBM_channelA2, 2);
		data[3] = M_Rs(offsets[3], v, HBM_channelA3, 3);
		data[4] = M_Rs(offsets[4], v, HBM_channelA4, 4);
		data[5] = M_Rs(offsets[5], v, HBM_channelA5, 5);
		data[6] = M_Rs(offsets[6], v, HBM_channelA6, 6);
		data[7] = M_Rs(offsets[7], v, HBM_channelA7, 7);
		data[8] = M_Rs(offsets[8], v, HBM_channelA8, 8);
		data[9] = M_Rs(offsets[9], v, HBM_channelA9, 9);
		data[10] = M_Rs(offsets[10], v, HBM_channelA10, 10);
		data[11] = M_Rs(offsets[11], v, HBM_channelA11, 11);
		data[12] = M_Rs(offsets[12], v, HBM_channelA12, 12);
		data[13] = M_Rs(offsets[13], v, HBM_channelA13, 13);
		data[14] = M_Rs(offsets[14], v, HBM_channelA14, 14);
		data[15] = M_Rs(offsets[15], v, HBM_channelA15, 15);
		data[16] = M_Rs(offsets[16], v, HBM_channelA16, 16);
		data[17] = M_Rs(offsets[17], v, HBM_channelA17, 17);
		data[18] = M_Rs(offsets[18], v, HBM_channelA18, 18);
		data[19] = M_Rs(offsets[19], v, HBM_channelA19, 19);
		data[20] = M_Rs(offsets[20], v, HBM_channelA20, 20);
		data[21] = M_Rs(offsets[21], v, HBM_channelA21, 21);
		data[22] = M_Rs(offsets[22], v, HBM_channelA22, 22);
		data[23] = M_Rs(offsets[23], v, HBM_channelA23, 23);
	#endif 
	return;
}

void master_insert_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef ___USE_AXI_CHANNEL___
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			M_Is(offset, v, data[v], HBM_channelA, inst);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			M_Is(offset, v, data[HBM_AXI_PACK_SIZE + v], HBM_channelB, inst);
		}
	#else 
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
		M_Is(offset, v, data[v], HBM_channelA, inst);
	}
	#endif 
	return;
}
void master_retrieve_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef ___USE_AXI_CHANNEL___
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[v] = M_Rs(offset, v, HBM_channelA, inst);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[HBM_AXI_PACK_SIZE + v] = M_Rs(offset, v, HBM_channelB, inst);
		}
	#else 
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v] = M_Rs(offset, v, HBM_channelA, inst);
	}
	#endif 
	return;
}
void master_insertmany_vec(unsigned int offsets[NUM_PEs], unsigned int ens[NUM_PEs], unsigned int data[NUM_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef ___USE_AXI_CHANNEL___
			if(ens[0]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[0], v, data[0][v], HBM_channelA0, 0);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[0], v, data[0][HBM_AXI_PACK_SIZE + v], HBM_channelB0, 0);
				}
			}
			if(ens[1]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[1], v, data[1][v], HBM_channelA1, 1);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[1], v, data[1][HBM_AXI_PACK_SIZE + v], HBM_channelB1, 1);
				}
			}
			if(ens[2]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[2], v, data[2][v], HBM_channelA2, 2);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[2], v, data[2][HBM_AXI_PACK_SIZE + v], HBM_channelB2, 2);
				}
			}
			if(ens[3]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[3], v, data[3][v], HBM_channelA3, 3);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[3], v, data[3][HBM_AXI_PACK_SIZE + v], HBM_channelB3, 3);
				}
			}
			if(ens[4]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[4], v, data[4][v], HBM_channelA4, 4);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[4], v, data[4][HBM_AXI_PACK_SIZE + v], HBM_channelB4, 4);
				}
			}
			if(ens[5]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[5], v, data[5][v], HBM_channelA5, 5);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[5], v, data[5][HBM_AXI_PACK_SIZE + v], HBM_channelB5, 5);
				}
			}
			if(ens[6]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[6], v, data[6][v], HBM_channelA6, 6);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[6], v, data[6][HBM_AXI_PACK_SIZE + v], HBM_channelB6, 6);
				}
			}
			if(ens[7]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[7], v, data[7][v], HBM_channelA7, 7);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[7], v, data[7][HBM_AXI_PACK_SIZE + v], HBM_channelB7, 7);
				}
			}
			if(ens[8]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[8], v, data[8][v], HBM_channelA8, 8);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[8], v, data[8][HBM_AXI_PACK_SIZE + v], HBM_channelB8, 8);
				}
			}
			if(ens[9]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[9], v, data[9][v], HBM_channelA9, 9);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[9], v, data[9][HBM_AXI_PACK_SIZE + v], HBM_channelB9, 9);
				}
			}
			if(ens[10]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[10], v, data[10][v], HBM_channelA10, 10);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[10], v, data[10][HBM_AXI_PACK_SIZE + v], HBM_channelB10, 10);
				}
			}
			if(ens[11]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[11], v, data[11][v], HBM_channelA11, 11);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[11], v, data[11][HBM_AXI_PACK_SIZE + v], HBM_channelB11, 11);
				}
			}
			if(ens[12]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[12], v, data[12][v], HBM_channelA12, 12);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[12], v, data[12][HBM_AXI_PACK_SIZE + v], HBM_channelB12, 12);
				}
			}
			if(ens[13]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[13], v, data[13][v], HBM_channelA13, 13);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[13], v, data[13][HBM_AXI_PACK_SIZE + v], HBM_channelB13, 13);
				}
			}
			if(ens[14]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[14], v, data[14][v], HBM_channelA14, 14);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[14], v, data[14][HBM_AXI_PACK_SIZE + v], HBM_channelB14, 14);
				}
			}
			if(ens[15]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[15], v, data[15][v], HBM_channelA15, 15);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[15], v, data[15][HBM_AXI_PACK_SIZE + v], HBM_channelB15, 15);
				}
			}
			if(ens[16]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[16], v, data[16][v], HBM_channelA16, 16);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[16], v, data[16][HBM_AXI_PACK_SIZE + v], HBM_channelB16, 16);
				}
			}
			if(ens[17]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[17], v, data[17][v], HBM_channelA17, 17);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[17], v, data[17][HBM_AXI_PACK_SIZE + v], HBM_channelB17, 17);
				}
			}
			if(ens[18]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[18], v, data[18][v], HBM_channelA18, 18);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[18], v, data[18][HBM_AXI_PACK_SIZE + v], HBM_channelB18, 18);
				}
			}
			if(ens[19]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[19], v, data[19][v], HBM_channelA19, 19);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[19], v, data[19][HBM_AXI_PACK_SIZE + v], HBM_channelB19, 19);
				}
			}
			if(ens[20]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[20], v, data[20][v], HBM_channelA20, 20);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[20], v, data[20][HBM_AXI_PACK_SIZE + v], HBM_channelB20, 20);
				}
			}
			if(ens[21]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[21], v, data[21][v], HBM_channelA21, 21);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[21], v, data[21][HBM_AXI_PACK_SIZE + v], HBM_channelB21, 21);
				}
			}
			if(ens[22]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[22], v, data[22][v], HBM_channelA22, 22);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[22], v, data[22][HBM_AXI_PACK_SIZE + v], HBM_channelB22, 22);
				}
			}
			if(ens[23]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[23], v, data[23][v], HBM_channelA23, 23);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[23], v, data[23][HBM_AXI_PACK_SIZE + v], HBM_channelB23, 23);
				}
			}
	#else 
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
			if(ens[0]==true){
				M_Is(offsets[0], v, data[0][v], HBM_channelA0, 0);
			}
			if(ens[1]==true){
				M_Is(offsets[1], v, data[1][v], HBM_channelA1, 1);
			}
			if(ens[2]==true){
				M_Is(offsets[2], v, data[2][v], HBM_channelA2, 2);
			}
			if(ens[3]==true){
				M_Is(offsets[3], v, data[3][v], HBM_channelA3, 3);
			}
			if(ens[4]==true){
				M_Is(offsets[4], v, data[4][v], HBM_channelA4, 4);
			}
			if(ens[5]==true){
				M_Is(offsets[5], v, data[5][v], HBM_channelA5, 5);
			}
			if(ens[6]==true){
				M_Is(offsets[6], v, data[6][v], HBM_channelA6, 6);
			}
			if(ens[7]==true){
				M_Is(offsets[7], v, data[7][v], HBM_channelA7, 7);
			}
			if(ens[8]==true){
				M_Is(offsets[8], v, data[8][v], HBM_channelA8, 8);
			}
			if(ens[9]==true){
				M_Is(offsets[9], v, data[9][v], HBM_channelA9, 9);
			}
			if(ens[10]==true){
				M_Is(offsets[10], v, data[10][v], HBM_channelA10, 10);
			}
			if(ens[11]==true){
				M_Is(offsets[11], v, data[11][v], HBM_channelA11, 11);
			}
			if(ens[12]==true){
				M_Is(offsets[12], v, data[12][v], HBM_channelA12, 12);
			}
			if(ens[13]==true){
				M_Is(offsets[13], v, data[13][v], HBM_channelA13, 13);
			}
			if(ens[14]==true){
				M_Is(offsets[14], v, data[14][v], HBM_channelA14, 14);
			}
			if(ens[15]==true){
				M_Is(offsets[15], v, data[15][v], HBM_channelA15, 15);
			}
			if(ens[16]==true){
				M_Is(offsets[16], v, data[16][v], HBM_channelA16, 16);
			}
			if(ens[17]==true){
				M_Is(offsets[17], v, data[17][v], HBM_channelA17, 17);
			}
			if(ens[18]==true){
				M_Is(offsets[18], v, data[18][v], HBM_channelA18, 18);
			}
			if(ens[19]==true){
				M_Is(offsets[19], v, data[19][v], HBM_channelA19, 19);
			}
			if(ens[20]==true){
				M_Is(offsets[20], v, data[20][v], HBM_channelA20, 20);
			}
			if(ens[21]==true){
				M_Is(offsets[21], v, data[21][v], HBM_channelA21, 21);
			}
			if(ens[22]==true){
				M_Is(offsets[22], v, data[22][v], HBM_channelA22, 22);
			}
			if(ens[23]==true){
				M_Is(offsets[23], v, data[23][v], HBM_channelA23, 23);
			}
	}
	#endif 
	return;
}
void master_retrievemany_vec(unsigned int offsets[NUM_PEs], unsigned int data[NUM_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef ___USE_AXI_CHANNEL___
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[0][v] = M_Rs(offsets[0], v, HBM_channelA0, 0);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[0][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets0, v, HBM_channelB0, 0);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[1][v] = M_Rs(offsets[1], v, HBM_channelA1, 1);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[1][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets1, v, HBM_channelB1, 1);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[2][v] = M_Rs(offsets[2], v, HBM_channelA2, 2);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[2][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets2, v, HBM_channelB2, 2);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[3][v] = M_Rs(offsets[3], v, HBM_channelA3, 3);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[3][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets3, v, HBM_channelB3, 3);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[4][v] = M_Rs(offsets[4], v, HBM_channelA4, 4);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[4][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets4, v, HBM_channelB4, 4);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[5][v] = M_Rs(offsets[5], v, HBM_channelA5, 5);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[5][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets5, v, HBM_channelB5, 5);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[6][v] = M_Rs(offsets[6], v, HBM_channelA6, 6);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[6][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets6, v, HBM_channelB6, 6);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[7][v] = M_Rs(offsets[7], v, HBM_channelA7, 7);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[7][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets7, v, HBM_channelB7, 7);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[8][v] = M_Rs(offsets[8], v, HBM_channelA8, 8);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[8][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets8, v, HBM_channelB8, 8);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[9][v] = M_Rs(offsets[9], v, HBM_channelA9, 9);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[9][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets9, v, HBM_channelB9, 9);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[10][v] = M_Rs(offsets[10], v, HBM_channelA10, 10);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[10][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets10, v, HBM_channelB10, 10);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[11][v] = M_Rs(offsets[11], v, HBM_channelA11, 11);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[11][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets11, v, HBM_channelB11, 11);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[12][v] = M_Rs(offsets[12], v, HBM_channelA12, 12);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[12][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets12, v, HBM_channelB12, 12);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[13][v] = M_Rs(offsets[13], v, HBM_channelA13, 13);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[13][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets13, v, HBM_channelB13, 13);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[14][v] = M_Rs(offsets[14], v, HBM_channelA14, 14);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[14][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets14, v, HBM_channelB14, 14);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[15][v] = M_Rs(offsets[15], v, HBM_channelA15, 15);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[15][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets15, v, HBM_channelB15, 15);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[16][v] = M_Rs(offsets[16], v, HBM_channelA16, 16);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[16][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets16, v, HBM_channelB16, 16);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[17][v] = M_Rs(offsets[17], v, HBM_channelA17, 17);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[17][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets17, v, HBM_channelB17, 17);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[18][v] = M_Rs(offsets[18], v, HBM_channelA18, 18);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[18][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets18, v, HBM_channelB18, 18);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[19][v] = M_Rs(offsets[19], v, HBM_channelA19, 19);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[19][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets19, v, HBM_channelB19, 19);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[20][v] = M_Rs(offsets[20], v, HBM_channelA20, 20);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[20][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets20, v, HBM_channelB20, 20);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[21][v] = M_Rs(offsets[21], v, HBM_channelA21, 21);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[21][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets21, v, HBM_channelB21, 21);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[22][v] = M_Rs(offsets[22], v, HBM_channelA22, 22);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[22][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets22, v, HBM_channelB22, 22);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[23][v] = M_Rs(offsets[23], v, HBM_channelA23, 23);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[23][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets23, v, HBM_channelB23, 23);
		}
	#else 
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
			data[0][v] = M_Rs(offsets[0], v, HBM_channelA0, 0);
			data[1][v] = M_Rs(offsets[1], v, HBM_channelA1, 1);
			data[2][v] = M_Rs(offsets[2], v, HBM_channelA2, 2);
			data[3][v] = M_Rs(offsets[3], v, HBM_channelA3, 3);
			data[4][v] = M_Rs(offsets[4], v, HBM_channelA4, 4);
			data[5][v] = M_Rs(offsets[5], v, HBM_channelA5, 5);
			data[6][v] = M_Rs(offsets[6], v, HBM_channelA6, 6);
			data[7][v] = M_Rs(offsets[7], v, HBM_channelA7, 7);
			data[8][v] = M_Rs(offsets[8], v, HBM_channelA8, 8);
			data[9][v] = M_Rs(offsets[9], v, HBM_channelA9, 9);
			data[10][v] = M_Rs(offsets[10], v, HBM_channelA10, 10);
			data[11][v] = M_Rs(offsets[11], v, HBM_channelA11, 11);
			data[12][v] = M_Rs(offsets[12], v, HBM_channelA12, 12);
			data[13][v] = M_Rs(offsets[13], v, HBM_channelA13, 13);
			data[14][v] = M_Rs(offsets[14], v, HBM_channelA14, 14);
			data[15][v] = M_Rs(offsets[15], v, HBM_channelA15, 15);
			data[16][v] = M_Rs(offsets[16], v, HBM_channelA16, 16);
			data[17][v] = M_Rs(offsets[17], v, HBM_channelA17, 17);
			data[18][v] = M_Rs(offsets[18], v, HBM_channelA18, 18);
			data[19][v] = M_Rs(offsets[19], v, HBM_channelA19, 19);
			data[20][v] = M_Rs(offsets[20], v, HBM_channelA20, 20);
			data[21][v] = M_Rs(offsets[21], v, HBM_channelA21, 21);
			data[22][v] = M_Rs(offsets[22], v, HBM_channelA22, 22);
			data[23][v] = M_Rs(offsets[23], v, HBM_channelA23, 23);
	}
	#endif 
	return;
}
#endif 

//////////////////////////////////////////////////////////////////
void master_centerinsert_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channel_t * HBM_center){
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
		M_I(offset, v, data[v], HBM_center);
	}
	return;
}
void master_centerretrieve_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channel_t * HBM_center){
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v] = M_R(offset, v, HBM_center);
	}
	return;
}

void master_insert(unsigned int offset, unsigned int v, unsigned int data, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef ___USE_AXI_CHANNEL___
		if(v < HBM_AXI_PACK_SIZE){
			M_Is(offset, v, data, HBM_channelA, inst);
		} else {
			M_Is(offset, v - HBM_AXI_PACK_SIZE, data, HBM_channelB, inst);
		}
	#else 
	M_Is(offset, v, data, HBM_channelA, inst);
	#endif 
	return;
}
unsigned int master_retrieve(unsigned int offset, unsigned int v, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	unsigned int data = 0;
	#ifdef ___USE_AXI_CHANNEL___
		if(v < HBM_AXI_PACK_SIZE){
			data = M_Rs(offset, v, HBM_channelA, inst);
		} else {
			data = M_Rs(offset, v - HBM_AXI_PACK_SIZE, HBM_channelB, inst);
		}
	#else 
	data = M_Rs(offset, v, HBM_channelA, inst);
	#endif 
	return data;
}
void master_insertmany(unsigned int offsets[NUM_PEs], unsigned int v, unsigned int ens[NUM_PEs], unsigned int data[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef ___USE_AXI_CHANNEL___
		if(v < HBM_AXI_PACK_SIZE){
			for(unsigned int n=0; n<NUM_PEs; n++){
				M_Is(offsets[n], v, data[n], HBM_channelA[n], n);
			}
		} else {
			for(unsigned int n=0; n<NUM_PEs; n++){
				M_Is(offsets[n], v - HBM_AXI_PACK_SIZE, data[n], HBM_channelB[n], n);
			}
		}
	#else 
	for(unsigned int n=0; n<NUM_PEs; n++){
		if(ens[n]==true){
			M_Is(offsets[n], v, data[n], HBM_channelA[n], n);
		}
	}
	#endif 
	return;
}
void master_retrievemany(unsigned int offsets[NUM_PEs], unsigned int v, unsigned int data[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef ___USE_AXI_CHANNEL___
		if(v < HBM_AXI_PACK_SIZE){
			for(unsigned int n=0; n<NUM_PEs; n++){
				data[n] = M_Rs(offsets[n], v, HBM_channelA[n], n);
			}
		} else {
			for(unsigned int n=0; n<NUM_PEs; n++){
				data[n] = M_Rs(offsets[n], v - HBM_AXI_PACK_SIZE, HBM_channelB[n], n);
			}
		}
	#else 
	for(unsigned int n=0; n<NUM_PEs; n++){
		data[n] = M_Rs(offsets[n], v, HBM_channelA[n], n);
	}
	#endif 
	return;
}

void master_insert_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef ___USE_AXI_CHANNEL___
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			M_Is(offset, v, data[v], HBM_channelA, inst);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			M_Is(offset, v, data[HBM_AXI_PACK_SIZE + v], HBM_channelB, inst);
		}
	#else 
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
		M_Is(offset, v, data[v], HBM_channelA, inst);
	}
	#endif 
	return;
}
void master_retrieve_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef ___USE_AXI_CHANNEL___
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[v] = M_Rs(offset, v, HBM_channelA, inst);
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
		#pragma HLS UNROLL
			data[HBM_AXI_PACK_SIZE + v] = M_Rs(offset, v, HBM_channelB, inst);
		}
	#else 
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v] = M_Rs(offset, v, HBM_channelA, inst);
	}
	#endif 
	return;
}
void master_insertmany_vec(unsigned int offsets[NUM_PEs], unsigned int ens[NUM_PEs], unsigned int data[NUM_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef ___USE_AXI_CHANNEL___
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS UNROLL
			if(ens[n]==true){
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[n], v, data[n][v], HBM_channelA[n], n);
				}
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
				#pragma HLS UNROLL
					M_Is(offsets[n], v, data[n][HBM_AXI_PACK_SIZE + v], HBM_channelB[n], n);
				}
			}
		}
	#else 
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			if(ens[n]==true){
				M_Is(offsets[n], v, data[n][v], HBM_channelA[n], n);
			}
		}
	}
	#endif 
	return;
}
void master_retrievemany_vec(unsigned int offsets[NUM_PEs], unsigned int data[NUM_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef ___USE_AXI_CHANNEL___
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS UNROLL
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
			#pragma HLS UNROLL
				data[n][v] = M_Rs(offsets[n], v, HBM_channelA[n], n);
			}
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
			#pragma HLS UNROLL
				data[n][HBM_AXI_PACK_SIZE + v] = M_Rs(offsets[n], v, HBM_channelB[n], n);
			}
		}
	#else 
	for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			data[n][v] = M_Rs(offsets[n], v, HBM_channelA[n], n);
		}
	}
	#endif 
	return;
}

//////////////////////////////////////////////////////////////////
void insert_cfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_center){
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data_[2*v] = data[v].key;
		data_[2*v + 1] = data[v].value;
	}
	master_centerinsert_vec((p_u * MAX_UPARTITION_VECSIZE) + t, data_, HBM_center);
}
void retrieve_cfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_center){
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	master_centerretrieve_vec((p_u * MAX_UPARTITION_VECSIZE) + t, data_, HBM_center);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v].key = data_[2*v];
		data[v].value = data_[2*v+1];
	}
	return; 
}

////////////////////////////////////////////////////////////////
unsigned int dretrieve_globalparams(unsigned int base_offset__, unsigned int index, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	return master_retrieve(base_offset__ + index, 0, HBM_channelA, HBM_channelB, inst); // NEW
}

unsigned int dretrieve_vptrdram(unsigned int base_offset__, unsigned int index, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS];
	checkoutofbounds("acts_kernel::ERROR 901::", (index / HBM_CHANNEL_PACK_SIZE), wwsize, NAp, NAp, NAp);
	#endif 
	return master_retrieve(base_offset__ + (index / HBM_CHANNEL_PACK_SIZE), index % HBM_CHANNEL_PACK_SIZE, HBM_channelA, HBM_channelB, inst);
}	
void dretrievemany_vptrdram(unsigned int base_offset__, unsigned int offsets[NUM_PEs], unsigned int data[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	data[0] = dretrieve_vptrdram(base_offset__, offsets[0], HBM_channelA0, HBM_channelB0, 0);
	data[1] = dretrieve_vptrdram(base_offset__, offsets[1], HBM_channelA1, HBM_channelB1, 1);
	data[2] = dretrieve_vptrdram(base_offset__, offsets[2], HBM_channelA2, HBM_channelB2, 2);
	data[3] = dretrieve_vptrdram(base_offset__, offsets[3], HBM_channelA3, HBM_channelB3, 3);
	data[4] = dretrieve_vptrdram(base_offset__, offsets[4], HBM_channelA4, HBM_channelB4, 4);
	data[5] = dretrieve_vptrdram(base_offset__, offsets[5], HBM_channelA5, HBM_channelB5, 5);
	data[6] = dretrieve_vptrdram(base_offset__, offsets[6], HBM_channelA6, HBM_channelB6, 6);
	data[7] = dretrieve_vptrdram(base_offset__, offsets[7], HBM_channelA7, HBM_channelB7, 7);
	data[8] = dretrieve_vptrdram(base_offset__, offsets[8], HBM_channelA8, HBM_channelB8, 8);
	data[9] = dretrieve_vptrdram(base_offset__, offsets[9], HBM_channelA9, HBM_channelB9, 9);
	data[10] = dretrieve_vptrdram(base_offset__, offsets[10], HBM_channelA10, HBM_channelB10, 10);
	data[11] = dretrieve_vptrdram(base_offset__, offsets[11], HBM_channelA11, HBM_channelB11, 11);
	data[12] = dretrieve_vptrdram(base_offset__, offsets[12], HBM_channelA12, HBM_channelB12, 12);
	data[13] = dretrieve_vptrdram(base_offset__, offsets[13], HBM_channelA13, HBM_channelB13, 13);
	data[14] = dretrieve_vptrdram(base_offset__, offsets[14], HBM_channelA14, HBM_channelB14, 14);
	data[15] = dretrieve_vptrdram(base_offset__, offsets[15], HBM_channelA15, HBM_channelB15, 15);
	data[16] = dretrieve_vptrdram(base_offset__, offsets[16], HBM_channelA16, HBM_channelB16, 16);
	data[17] = dretrieve_vptrdram(base_offset__, offsets[17], HBM_channelA17, HBM_channelB17, 17);
	data[18] = dretrieve_vptrdram(base_offset__, offsets[18], HBM_channelA18, HBM_channelB18, 18);
	data[19] = dretrieve_vptrdram(base_offset__, offsets[19], HBM_channelA19, HBM_channelB19, 19);
	data[20] = dretrieve_vptrdram(base_offset__, offsets[20], HBM_channelA20, HBM_channelB20, 20);
	data[21] = dretrieve_vptrdram(base_offset__, offsets[21], HBM_channelA21, HBM_channelB21, 21);
	data[22] = dretrieve_vptrdram(base_offset__, offsets[22], HBM_channelA22, HBM_channelB22, 22);
	data[23] = dretrieve_vptrdram(base_offset__, offsets[23], HBM_channelA23, HBM_channelB23, 23);
		
	// for(unsigned int n=0; n<NUM_PEs; n++){
	// #pragma HLS UNROLL
		// data[n] = dretrieve_vptrdram(base_offset__, offsets[n], HBM_channelA[n], HBM_channelB[n], n);
	// }	
	return;
}

map_t dretrieve_actpackvptrdram(unsigned int base_offset__, unsigned int p_u, unsigned int llp_set, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	unsigned int offset = ((p_u * MAX_NUM_LLPSETS) + llp_set) * 2;
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS];
	checkoutofbounds("acts_kernel::ERROR 702::", offset / HBM_CHANNEL_PACK_SIZE, wwsize, NAp, NAp, NAp);
	#endif 
	map_t map;
	unsigned int i = offset / HBM_CHANNEL_PACK_SIZE;
	unsigned int j = offset % HBM_CHANNEL_PACK_SIZE;
	map.offset = master_retrieve(base_offset__ + i, j, HBM_channelA, HBM_channelB, inst);
	map.size = master_retrieve(base_offset__ + i, j + 1, HBM_channelA, HBM_channelB, inst);
	return map;
}
void dretrievemany_actpackvptrdram(unsigned int base_offset__, unsigned int p_u, unsigned int llp_set, map_t maps[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	unsigned int offset = ((p_u * MAX_NUM_LLPSETS) + llp_set) * 2; // FIXME.
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS];
	checkoutofbounds("acts_kernel::ERROR 702::", offset / HBM_CHANNEL_PACK_SIZE, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int i = offset / HBM_CHANNEL_PACK_SIZE;
	unsigned int j = offset % HBM_CHANNEL_PACK_SIZE;
	
	unsigned int data_[NUM_PEs];
	unsigned int offsets_[NUM_PEs];
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + i;
	}
	master_retrievemany(offsets_, j, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		maps[n].offset = data_[n];
	}
	master_retrievemany(offsets_, j + 1, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
	for(unsigned int n=0; n<NUM_PEs; n++){
		maps[n].size = data_[n];
	}
	return;
}

edge3_vec_dt dretrieve_csr_pack_edges(unsigned int base_offset__, unsigned int index, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	edge3_vec_dt edge3_vec;
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CSREDGES];
	checkoutofbounds("acts_kernel::ERROR 703::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data[HBM_CHANNEL_PACK_SIZE];
	master_retrieve_vec(base_offset__ + index, data, HBM_channelA, HBM_channelB, inst);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		edge3_vec.data[v].srcvid = data[2*v];
		edge3_vec.data[v].dstvid = (data[2*v+1] >> 1) & 0xFFFFFFFF;
		edge3_vec.data[v].weight = 1;
		edge3_vec.data[v].valid = data[2*v+1] & 0x1;
	}
	return edge3_vec;
}

edge3_vec_dt dretrieve_act_pack_edges(unsigned int base_offset__, unsigned int index, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	edge3_vec_dt edge3_vec;
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
	checkoutofbounds("acts_kernel::ERROR 704::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	master_retrieve_vec(base_offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		edge3_vec.data[v].srcvid = data_[2*v];
		edge3_vec.data[v].dstvid = (data_[2*v+1] >> 1) & 0xFFFFFFFF;
		edge3_vec.data[v].weight = 1;
		edge3_vec.data[v].valid = data_[2*v+1] & 0x1;
	}
	return edge3_vec;
}
void dretrievemany_act_pack_edges(unsigned int base_offset__, unsigned int offsets[NUM_PEs], unsigned int t, edge3_vec_dt edge3_vecs[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
	checkoutofbounds("acts_kernel::ERROR 704f::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_PEs][HBM_CHANNEL_PACK_SIZE];
	unsigned int offsets_[NUM_PEs];
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			edge3_vecs[n].data[v].srcvid = data_[n][2*v];
			edge3_vecs[n].data[v].dstvid = (data_[n][2*v+1] >> 1) & 0xFFFFFFFF;
			edge3_vecs[n].data[v].weight = 1;
			edge3_vecs[n].data[v].valid = data_[n][2*v+1] & 0x1;
		}
	}
	return;
}

unsigned int dretrieve_updatesptrs(unsigned int base_offset__, unsigned int index, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS];
	checkoutofbounds("acts_kernel::ERROR 701::", (index / HBM_CHANNEL_PACK_SIZE), wwsize, inst, NAp, NAp);
	#endif 
	return master_retrieve(base_offset__ + index, 0, HBM_channelA, HBM_channelB, inst); // NEW
}

void dinsert_updatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt data, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst, unsigned int globalparams[32], unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
	checkoutofbounds("acts_kernel::ERROR 708a::", updatesptrs[llp_set] + index, updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 708b::", llp_set, MAX_NUM_APPLYPARTITIONS, index, v, NAp);
		checkoutofbounds("acts_kernel::ERROR 708c::", index, HBM_CHANNEL_SIZE, index, v, NAp);
		#endif 
		data_[2*v] = data.data[v].key;
		data_[2*v + 1] = data.data[v].value;
	}
	master_insert_vec(offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
}
void dretrieve_udatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt * data, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst, unsigned int globalparams[32], unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
	checkoutofbounds("acts_kernel::ERROR 710a::", updatesptrs[llp_set] + index, updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	master_retrieve_vec(offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 710b::", llp_set, MAX_NUM_APPLYPARTITIONS, index, v, NAp);
		checkoutofbounds("acts_kernel::ERROR 710c::", index, HBM_CHANNEL_SIZE, index, v, NAp);
		#endif 
		data->data[v].key = data_[2*v]; 
		data->data[v].value = data_[2*v + 1];		
	}
	return; 
}
void dinsertmany_updatesdram(unsigned int offset__, unsigned int llp_set, unsigned int offsets[NUM_PEs], unsigned int t, uint512_vec_dt data[NUM_PEs], bool ens[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs], unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
	checkoutofbounds("acts_kernel::ERROR 708a::", updatesptrs[llp_set] + offsets[0], updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_PEs][HBM_CHANNEL_PACK_SIZE];
	unsigned int offsets_[NUM_PEs];
	unsigned int ens_[NUM_PEs];
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + offsets[n] + t;
	}
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		ens_[n] = ens[n][0];
	}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			data_[n][2*v] = data[n].data[v].key;
			data_[n][2*v+1] = data[n].data[v].value;
		}
	}
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
}
void dretrievemany_udatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt data[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs], unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
	checkoutofbounds("acts_kernel::ERROR 710a::", updatesptrs[llp_set] + index, updatesptrs[llp_set + 1], NAp, NAp, NAp);
	#endif 

	unsigned int data_[NUM_PEs][HBM_CHANNEL_PACK_SIZE]; 
	unsigned int offsets_[NUM_PEs];
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + index;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			data[n].data[v].key = data_[n][2*v];
			data[n].data[v].value = data_[n][2*v+1];
		}
	}
	return; 
}

void dinsert_vdatadram(unsigned int offset__, unsigned int index, vprop_t data[EDGE_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VDATAS];
	checkoutofbounds("acts_kernel::ERROR 711::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data_[2*v] = data[v].prop;
		data_[2*v + 1] = data[v].mask;
	}
	master_insert_vec(offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
	return;
}
void dretrieve_vdatadram(unsigned int offset__, unsigned int index, vprop_t data[EDGE_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VDATAS];
	checkoutofbounds("acts_kernel::ERROR 712::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	master_retrieve_vec(offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v].prop = data_[2*v];
		data[v].mask = data_[2*v + 1];
	}
	return;
}
void dinsertmany_vdatadram(unsigned int offset__, unsigned int offsets[NUM_PEs], unsigned int t, vprop_t datas[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VDATAS];
	checkoutofbounds("acts_kernel::ERROR 711::", offsets[0], wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_PEs][HBM_CHANNEL_PACK_SIZE]; // NEW
	unsigned int offsets_[NUM_PEs];
	unsigned int ens_[NUM_PEs];
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + offsets[n] + t;
	}
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		ens_[n] = true;
	}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			data_[n][2*v] = datas[n][v].prop;
			data_[n][2*v+1] = datas[n][v].mask;
		}
	}
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
	return;
}
void dretrievemany_vdatadram(unsigned int offset__, unsigned int offsets[NUM_PEs], unsigned int t, vprop_t datas[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__VDATAS];
	checkoutofbounds("acts_kernel::ERROR 712::", offsets[0], wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_PEs][HBM_CHANNEL_PACK_SIZE]; // NEW
	unsigned int offsets_[NUM_PEs];
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			datas[n][v].prop = data_[n][2*v];
			datas[n][v].mask = data_[n][2*v+1];
		}
	}
	return;
}

void dinsert_cfrontierdram_tmp(unsigned int base_offset__, unsigned int index, keyvalue_t data[EDGE_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP];
	checkoutofbounds("acts_kernel::ERROR 713a::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data_[2*v] = data[v].key;
		data_[2*v + 1] = data[v].value;
	}
	master_insert_vec(base_offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
}
void dretrieve_cfrontierdram_tmp(unsigned int base_offset__, unsigned int index, keyvalue_t data[EDGE_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP];
	checkoutofbounds("acts_kernel::ERROR 714a::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	master_retrieve_vec(base_offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 714b::", index, MAX_APPLYPARTITION_VECSIZE, index, v, NAp);
		#endif 
		data[v].key = data_[2*v];
		data[v].value = data_[2*v + 1];
	}
	return; 
}
void dinsertmany_cfrontierdram_tmp(unsigned int base_offset__, unsigned int offsets[NUM_PEs], unsigned int t, keyvalue_t datas[NUM_PEs][EDGE_PACK_SIZE], bool ens[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP];
	checkoutofbounds("acts_kernel::ERROR 713a::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_PEs][HBM_CHANNEL_PACK_SIZE];
	unsigned int offsets_[NUM_PEs];
	unsigned int ens_[NUM_PEs];
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		ens_[n] = ens[n][0];
	}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			data_[n][2*v] = datas[n][v].key;
			data_[n][2*v+1] = datas[n][v].value;
		}
	}
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
	return;
}
void dretrievemany_cfrontierdram_tmp(unsigned int base_offset__, unsigned int offsets[NUM_PEs], unsigned int t, keyvalue_t datas[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP];
	checkoutofbounds("acts_kernel::ERROR 714a::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_PEs][HBM_CHANNEL_PACK_SIZE];
	unsigned int offsets_[NUM_PEs];
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = base_offset__ + offsets[n] + t;
	}
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			datas[n][v].key = data_[n][2*v];
			datas[n][v].value = data_[n][2*v+1];
		}
	}
	return; 
}

void dinsert_nfrontierdram(unsigned int offset__, unsigned int p_u, unsigned int index, keyvalue_t data[EDGE_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS];
	checkoutofbounds("acts_kernel::ERROR 715a::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data_[2*v] = data[v].key;
		data_[2*v + 1] = data[v].value;
	}
	master_insert_vec(offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
}
void dretrieve_nfrontierdram(unsigned int offset__, unsigned int p_u, unsigned int index, keyvalue_t data[EDGE_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS];
	checkoutofbounds("acts_kernel::ERROR 716a::", index, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	master_retrieve_vec(offset__ + index, data_, HBM_channelA, HBM_channelB, inst);
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 716b::", p_u, MAX_NUM_UPARTITIONS, index, v, NAp);
		checkoutofbounds("acts_kernel::ERROR 716c::", index, MAX_APPLYPARTITION_VECSIZE, p_u, v, NAp);
		#endif 
		data[v].key = data_[2*v];
		data[v].value = data_[2*v + 1];
	}
	return; 
}
void dinsertmany_nfrontierdram(unsigned int offset__, unsigned int p_u, unsigned int offsets[NUM_PEs], unsigned int t, keyvalue_t datas[NUM_PEs][EDGE_PACK_SIZE], bool ens[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS];
	checkoutofbounds("acts_kernel::ERROR 715a::", offsets[0] + t, wwsize, NAp, NAp, NAp);
	#endif 
	
	unsigned int data_[NUM_PEs][HBM_CHANNEL_PACK_SIZE];
	unsigned int offsets_[NUM_PEs];
	unsigned int ens_[NUM_PEs];
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		offsets_[n] = offset__ + offsets[n] + t;
	}
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		ens_[n] = ens[n][0];
	}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		for(unsigned int n=0; n<NUM_PEs; n++){
			data_[n][2*v] = datas[n][v].key;
			data_[n][2*v+1] = datas[n][v].value;
		}
	}
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
}

//////////////////////////////////////////////////////////////////
void insert_udatabuffer(unsigned int v, unsigned int index, vprop_t data, vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 100::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	vdata_buffer[v][index] = data;
}

vprop_t retrieve_udatabuffer(unsigned int v, unsigned int index, vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 101::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return vdata_buffer[v][index];
}

void insert_vdatabuffer(unsigned int v, unsigned int index, vprop_t data, vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 102::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	vdata_buffer[v][index] = data; // vdata_buffer
}
vprop_t retrieve_vdatabuffer(unsigned int v, unsigned int index, vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 103::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return vdata_buffer[v][index];
}
void insert_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 104::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		vdata_buffer[v][index] = data[v];
	}
}
vprop_t retrieve_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 105::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = vdata_buffer[v][index];
	}
}

void insert_edgesbuffer(unsigned int v, unsigned int index, edge3_type data, edge3_type * edges_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 192::", index, EDGE_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	edges_buffer[v][index] = data; // vdata_buffer
}
edge3_type retrieve_edgesbuffer(unsigned int v, unsigned int index, edge3_type * edges_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 193::", index, EDGE_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return edges_buffer[v][index];
}

void insert_updatesbuffer(unsigned int v, unsigned int index, keyvalue_t data, keyvalue_t * updates_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 107::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	updates_buffer[v][index] = data;
}
keyvalue_t retrieve_updatesbuffer(unsigned int v, unsigned int index, keyvalue_t * updates_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 108::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return updates_buffer[v][index];
}

void insert_updatestmpbuffer(unsigned int index, unsigned int t, keyvalue_t data, keyvalue_t * updates_tmpbuffer[NUM_PEs]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 109::", index, 24, NAp, NAp, NAp);
	checkoutofbounds("acts_kernel::ERROR 110::", t, CSRBUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	updates_tmpbuffer[index][t] = data;
}
keyvalue_t retrieve_updatestmpbuffer(unsigned int index, unsigned int t, keyvalue_t * updates_tmpbuffer[NUM_PEs]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 111::", index, 24, NAp, NAp, NAp);
	checkoutofbounds("acts_kernel::ERROR 112::", t, CSRBUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return updates_tmpbuffer[index][t];
}

void insert_cfrontierbuffer(unsigned int v, unsigned int t, frontier_t data, keyvalue_t * cfrontier_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 113::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	cfrontier_buffer[v][t] = data;
}
frontier_t retrieve_cfrontierbuffer(unsigned int v, unsigned int t, keyvalue_t * cfrontier_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 114::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return cfrontier_buffer[v][t];
}
void insert_cfrontierbuffer(unsigned int t, frontier_t data[EDGE_PACK_SIZE], keyvalue_t * cfrontier_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 115::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		cfrontier_buffer[v][t] = data[v];
	}
}
void retrieve_cfrontierbuffer(unsigned int t, frontier_t data[EDGE_PACK_SIZE], keyvalue_t * cfrontier_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 116::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = cfrontier_buffer[v][t];
	}
	return; 
}

void insert_nfrontierbuffer(unsigned int t, unsigned int v, frontier_t data, keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 117::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	nfrontier_buffer[v][t] = data;
}
frontier_t retrieve_nfrontierbuffer(unsigned int t, unsigned int v, keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 118::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return nfrontier_buffer[v][t];
}
void insert_nfrontierbuffer(unsigned int t, frontier_t data[EDGE_PACK_SIZE], keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 119::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		nfrontier_buffer[v][t] = data[v];
	}
}
void retrieve_nfrontierbuffer(unsigned int t, frontier_t data[EDGE_PACK_SIZE], keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 120::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = nfrontier_buffer[v][t];
	}
	return; 
}

void insert_statstmpbuffer(unsigned int t, unsigned int v, unsigned int data, unsigned int * stats_tmpbuffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 121::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	stats_tmpbuffer[v][t] = data;
}
unsigned int retrieve_statstmpbuffer(unsigned int t, unsigned int v, unsigned int * stats_tmpbuffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 1242::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return stats_tmpbuffer[v][t]; 
}
void insert_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], unsigned int * stats_tmpbuffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 123::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		stats_tmpbuffer[v][t] = data[v];
	}
}
void retrieve_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], unsigned int * stats_tmpbuffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 124::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = stats_tmpbuffer[v][t];
	}
	return; 
}

void insert_statsbuffer(unsigned int t, unsigned int v, unsigned int data, unsigned int * stats_buffer[MAX_NUM_APPLYPARTITIONS]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 125::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	stats_buffer[v][t] = data;
}
unsigned int retrieve_statsbuffer(unsigned int t, unsigned int v, unsigned int * stats_buffer[MAX_NUM_APPLYPARTITIONS]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 126::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	return stats_buffer[v][t]; 
}

void insert_stats2buffer(unsigned int t, unsigned int v, unsigned int data, unsigned int * stats2_buffer[MAX_NUM_APPLYPARTITIONS]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 127::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	stats2_buffer[v][t] = data;
}
unsigned int retrieve_stats2buffer(unsigned int t, unsigned int v, unsigned int * stats2_buffer[MAX_NUM_APPLYPARTITIONS]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 128::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	return stats2_buffer[v][t]; 
}

void insert_vptrbuffer(unsigned int t, vtr_t data, vtr_t * vptr_buffer){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 129::", t, VPTR_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	vptr_buffer[t] = data;
}
vtr_t retrieve_vptrbuffer(unsigned int t, vtr_t * vptr_buffer){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 130::", t, VPTR_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return vptr_buffer[t];
}
//////////////////////////////////////////////////////////////////

void load_edges(unsigned int inst, unsigned int offset_vptrbuffer, unsigned int baseoffset_csrdram, unsigned int vptrbuffer___size, vtr_t * vptr_buffer, unsigned int * edges_buffer___size, edge3_type * edges_buffer[EDGE_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB){
	if(offset_vptrbuffer < vptrbuffer___size){ // NEW
		vtr_t vptr_data = retrieve_vptrbuffer(offset_vptrbuffer, vptr_buffer); 
		unsigned int vid = vptr_data.vid;
		unsigned int edges_size = vptr_data.end - vptr_data.begin; 
		unsigned int edgelist_size = (edges_size / EDGE_PACK_SIZE) + 2; if((edges_size == 0) || (vptr_data.end < vptr_data.begin)){ edgelist_size = 0; }
		unsigned int uprop = vptr_data.prop;
		
		VC_READ_EDGES_LOOP1B: for(unsigned int t=0; t<edgelist_size; t++){
		#pragma HLS PIPELINE II=1
			edge3_vec_dt edges = dretrieve_csr_pack_edges(baseoffset_csrdram, (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA, HBM_channelB, inst);
			edge3_type edge_list[EDGE_PACK_SIZE];
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
			#pragma HLS UNROLL
				if(edges.data[v].srcvid == vid && edges.data[v].srcvid != INVALIDDATA){ edge_list[v] = edges.data[v]; edge_list[v].srcvid = uprop; } else { edge_list[v].srcvid = INVALIDDATA; edge_list[v].dstvid = INVALIDDATA; }
			}
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
			#pragma HLS UNROLL
				checkoutofbounds("acts_kernel::ERROR 812::", *edges_buffer___size, EDGE_BUFFER_SIZE, vptrbuffer___size, inst, edgelist_size);
				insert_edgesbuffer(v, *edges_buffer___size, edge_list[v], edges_buffer);
			}
			*edges_buffer___size += 1;
			// if(*edges_buffer___size + 2 < EDGE_BUFFER_SIZE){ *edges_buffer___size += 1; } // FIXME. 'forces correctness'
		}
	}
	return;
}

void transport_frontier(unsigned int inst, unsigned int p_v, unsigned int baseoffset_nfdram, unsigned int * cfrontier_dram___size, unsigned int * nfrontier_dram___size[EDGE_PACK_SIZE], offset_t * upartition_vertices, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, HBM_channel_t * HBM_center, unsigned int * totalactvvs2){
	frontier_t actvv[EDGE_PACK_SIZE];
	for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
		unsigned int p_actvv_ = p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION + p_actvv;
		checkoutofbounds("acts_kernel::ERROR 225::", p_actvv_, MAX_NUM_UPARTITIONS, NAp, NAp, NAp);
		unsigned int offset = baseoffset_nfdram + (p_actvv_ * MAX_APPLYPARTITION_VECSIZE);
		for(unsigned int t=0; t<nfrontier_dram___size[0][p_actvv_]; t++){
		#pragma HLS PIPELINE II=1
			upartition_vertices[p_actvv_].count += 1; 
			dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA, HBM_channelB, inst);
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<0; v++){ cout<<"TRANSFER VID-BASED FRONTIERS SEEN @: H: "<<inst<<" => vid: "<<actvv[v].key<<", p_actvv_: "<<p_actvv_<<endl; }
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA && actvv[v].key / MAX_UPARTITION_SIZE != p_actvv_){ cout<<"ERROR 234: actvv["<<v<<"].key("<<actvv[v].key<<") / MAX_UPARTITION_SIZE("<<MAX_UPARTITION_SIZE<<")(="<<actvv[v].key / MAX_UPARTITION_SIZE<<") != p_actvv_("<<p_actvv_<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }}				
			#endif 
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA){ *totalactvvs2 += 1; }} // FIXME.
			insert_cfrontierdram(p_actvv_, cfrontier_dram___size[p_actvv_], actvv, HBM_center); cfrontier_dram___size[p_actvv_] += 1;
		}
	}
}

void acts_kernel::run(HBM_channel_t * HBM_center,  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, HBM_channelTHIS_t * HBM_channelA[NUM_PEs], HBM_channelTHIS_t * HBM_channelB[NUM_PEs]){	
	#ifdef _DEBUGMODE_KERNELPRINTS4
	cout<<"acts_kernel::run:: acts started "<<endl;
	#endif 

	// declaration of BRAM variables
	#ifdef FPGA_IMPL
	
	#else 
	keyvalue_t * nfrontier_buffer[NUM_PEs][EDGE_PACK_SIZE]; // [MAX_APPLYPARTITION_VECSIZE]
	keyvalue_t * cfrontier_bufferREAL0[EDGE_PACK_SIZE]; 
	keyvalue_t * cfrontier_bufferREAL1[NUM_PEs]; // [MAX_APPLYPARTITION_VECSIZE]
	unsigned int * stats_tmpbuffer[NUM_PEs][EDGE_PACK_SIZE]; // [BLOCKRAM_SIZE]
	unsigned int * stats_buffer[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; // [BLOCKRAM_SIZE]
	unsigned int * stats2_buffer[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; // [BLOCKRAM_SIZE]
	keyvalue_t * updates_tmpbuffer[24][NUM_PEs]; // [CSRBUFFER_SIZE(//FIXME.)]
	keyvalue_t * updates_buffer[NUM_PEs][EDGE_PACK_SIZE]; // [MAX_APPLYPARTITION_VECSIZE]
	vtr_t * vptr_buffer[NUM_PEs]; // [VPTR_BUFFER_SIZE]
	edge3_type * edges_buffer[NUM_PEs][EDGE_PACK_SIZE]; // [EDGE_BUFFER_SIZE]
	vprop_t * vdata_buffer[NUM_PEs][EDGE_PACK_SIZE]; // [MAX_APPLYPARTITION_VECSIZE]
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer[i][v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cfrontier_bufferREAL0[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ cfrontier_bufferREAL1[i] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ stats_tmpbuffer[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ stats_buffer[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ stats2_buffer[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<24; i++){ for(unsigned int p=0; p<NUM_PEs; p++){ updates_tmpbuffer[i][p] = new keyvalue_t[CSRBUFFER_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer[i][v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ vptr_buffer[i] = new vtr_t[VPTR_BUFFER_SIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ edges_buffer[i][v] = new edge3_type[EDGE_BUFFER_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ vdata_buffer[i][v] = new vprop_t[MAX_APPLYPARTITION_VECSIZE]; }}
	#endif 
	
	unsigned int __NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	unsigned int __NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs
	cout<<"acts_kernel::run:: universalparams.NUM_VERTICES: "<<universalparams.NUM_VERTICES<<", universalparams.NUM_EDGES: "<<universalparams.NUM_EDGES<<", MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<", __NUM_UPARTITIONS: "<<__NUM_UPARTITIONS<<", MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<", __NUM_APPLYPARTITIONS: "<<__NUM_APPLYPARTITIONS<<endl;			
	
	unsigned int * cfrontier_dram___size = new unsigned int[MAX_NUM_UPARTITIONS]; 
	for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ cfrontier_dram___size[p] = 0; }
	
	unsigned int * nfrontier_dram___size[NUM_PEs][EDGE_PACK_SIZE]; // FIXME. remove first dimension - not necessary
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
	vprop_t data[EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = data complete
	vprop_t datas[NUM_PEs][EDGE_PACK_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = datas complete
	keyvalue_t kvdata[EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = kvdata complete
	keyvalue_t kvdatas[NUM_PEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = kvdatas complete
	unsigned int offsets[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable = offsets complete
	keyvalue_t actvvs[NUM_PEs][EDGE_PACK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = actvvs complete
						
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
	unsigned int threshold___activedstvids = 0; // 0xFFFFFFFF;
	unsigned int limits[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable = limits complete
	unsigned int max_limits[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable = max_limits complete
	unsigned int max_limit = 0; 
	
	// activate root vid
	cfrontier_dram___size[0] = 1;
	
	if(run_vertex_centric == true){ cout<<"### Vertex-Centric GAS iteration: 0 [1 active vertices]"<<endl; } else { cout<<"### Edge-Centric GAS iteration: 0 [1 active vertices]"<<endl; }
	
	// reset ubuffer 
	vprop_t prop; prop.prop = 0; prop.mask = INVALIDMASK; 
	for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int t=0; t<MAX_UPARTITION_VECSIZE; t++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ insert_udatabuffer(v, t, prop, vdata_buffer[inst]); }}} // 
	
	// load global parameters
	unsigned int globalparams[32];
	unsigned int updatesptrs[MAX_NUM_LLPSETS];
	#pragma HLS ARRAY_PARTITION variable = globalparams complete
	for(unsigned int t=0; t<32; t++){
		globalparams[t] = dretrieve_globalparams(0, t, HBM_channelA0, HBM_channelB0, 0);
	}
	globalparams_debug = (unsigned int *)&globalparams[0];
	
	// load updates ptrs
	for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
	#pragma HLS PIPELINE II=1
		updatesptrs[t] = dretrieve_updatesptrs(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS], t, HBM_channelA0, HBM_channelB0, 0);
		cout<<"--- updatesptrs["<<t<<"]: "<<updatesptrs[t]<<endl;
	}
	
	// run acts 
	for(unsigned int GraphIter=0; GraphIter<12; GraphIter++){ // MAXNUMGRAPHITERATIONS
		unsigned int MASK_CODE = 1 + GraphIter;
		unsigned int MASK_CODE_PE = 1 + GraphIter;
		unsigned int MASK_CODE_AU = 4094 + 1 + GraphIter;
		
		// clear counters [done]
		CLEAR_COUNTERS_LOOP1: for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
		#pragma HLS UNROLL
			CLEAR_COUNTERS_LOOP1B: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){ 
				actpackupdates_dram___size[inst][p_v] = 0;
				stats_buffer___size[inst][p_v] = 0; stats2_buffer___size[inst][p_v] = 0;
				vpartition_vertices[inst][p_v].offset = 0; vpartition_vertices[inst][p_v].size = 0; vpartition_vertices[inst][p_v].count = 0;
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer___size[inst][v][p_v] = 0; }
			}	
			CLEAR_COUNTERS_LOOP1C: for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){ 
			#pragma HLS UNROLL
				for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ insert_statsbuffer(t, p_v, 0, stats_buffer[inst]); }
			}
		}
		
		// process-edges and partition-updates
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){
			keyvalue_t data[EDGE_PACK_SIZE];
			unsigned int data_u32[EDGE_PACK_SIZE];
			MASK_CODE_PE = ((1 + GraphIter) * MAX_NUM_UPARTITIONS) + p_u;
			bool ___use_vertex_centric___ = false; if(upartition_vertices[p_u].count < threshold___activefrontiers && enable___vertexcentric == true){ ___use_vertex_centric___ = true; }
	
			// broadcast active frontiers [done]
			for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = 0; }
			for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){
			#pragma HLS PIPELINE II=1
				retrieve_cfrontierdram(p_u, t, data, HBM_center);
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
				#pragma HLS UNROLL
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
					#pragma HLS UNROLL
						kvdatas[inst][v] = data[v];
						ens[inst][v] = true;
					}
				}
				dinsertmany_cfrontierdram_tmp(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], offsets, t, kvdatas, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
			}
			
			// parallel-read and map active frontiers [done]
			for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = 0; }
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){
				#pragma HLS PIPELINE II=1
					dretrievemany_cfrontierdram_tmp(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], offsets, t, kvdatas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
					insert_cfrontierbuffer(t, kvdatas[0], cfrontier_bufferREAL0);
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
					#pragma HLS UNROLL
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							if(kvdatas[inst][v].key != INVALIDDATA){
								unsigned int srcvid_lpv = get_local_to_upartition(kvdatas[inst][v].key) / EDGE_PACK_SIZE; // FIXME.
								vprop_t prop; prop.prop = kvdatas[inst][v].value; prop.mask = MASK_CODE_PE; 
								insert_udatabuffer(v, srcvid_lpv, prop, vdata_buffer[inst]); 
							}
						}
					}
				}
			}

			// reset temp stats buffer [done]
			if(___use_vertex_centric___ == false){
				for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ 
				#pragma HLS PIPELINE II=1
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
					#pragma HLS UNROLL
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
						#pragma HLS UNROLL
							insert_statstmpbuffer(t, v, 0, stats_tmpbuffer[inst]); 
						}
					}
				}
			}
			// reset updates_tmpbuffer [done]
			for(unsigned int i=0; i<NUM_PEs; i++){ 
			#pragma HLS UNROLL
				for(unsigned int j=0; j<NUM_PEs; j++){ updates_tmpbuffer___size[i][j] = 0; } // reset
			} 
			
			// process-edges and partition-updates
			if(upartition_vertices[p_u].count > 0){
				if(___use_vertex_centric___ == true){
					hybrid_map[GraphIter][p_u] = 0;
					
					// reset buffers [done]
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
					#pragma HLS UNROLL
						vptrbuffer___size[inst] = 0; edges_buffer___size[inst] = 0; // reset
					} 
					
					// distribute vptrs [done]
					unsigned int cummtv2[NUM_PEs]; for(unsigned int inst=0; inst<NUM_PEs; inst++){ cummtv2[inst] = 0; }
					unsigned int cfrontier_bufferREAL___size[NUM_PEs]; for(unsigned int n=0; n<NUM_PEs; n++){ cfrontier_bufferREAL___size[n] = 0; }
					VC_READVPTRS_LOOP1: for(unsigned int pingpong=0; pingpong<cfrontier_dram___size[p_u] * EDGE_PACK_SIZE; pingpong++){
						frontier_t actvv = retrieve_cfrontierbuffer(pingpong % EDGE_PACK_SIZE, pingpong / EDGE_PACK_SIZE, cfrontier_bufferREAL0);
						if(actvv.key != INVALIDDATA){ // NEW
							unsigned int inst_ = actvv.key % NUM_PEs;
							// cout<<"--------++++++++++++++++++++++++++------------- actvv.key: "<<actvv.key<<endl;
							cfrontier_bufferREAL1[inst_][cfrontier_bufferREAL___size[inst_]] = actvv;
							cfrontier_bufferREAL___size[inst_] += 1;
							checkoutofbounds("acts_kernel::ERROR 073::", cfrontier_bufferREAL___size[inst_], MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
						}
					}
					
					// load vptrs [done]
					max_limit = 0; for(unsigned int n=0; n<NUM_PEs; n++){ if(max_limit < cfrontier_bufferREAL___size[n]){ max_limit = cfrontier_bufferREAL___size[n]; }} 
					VC_READVPTRS_LOOP2: for(unsigned int t=0; t<max_limit; t++){
						#pragma HLS PIPELINE II=1
						unsigned int begins_u32[NUM_PEs]; 
						unsigned int ends_u32[NUM_PEs];
						frontier_t actvvs[NUM_PEs];
						
						VC_READVPTRS_LOOP2B: for(unsigned int n=0; n<NUM_PEs; n++){
						#pragma HLS UNROLL
							actvvs[n] = cfrontier_bufferREAL1[n][t];
							unsigned int hvid = actvvs[n].key / NUM_PEs; // FIXME. (send local instead?)
							offsets[n] = hvid; 
						}
						dretrievemany_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], offsets, begins_u32,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
						
						VC_READVPTRS_LOOP2C: for(unsigned int n=0; n<NUM_PEs; n++){
						#pragma HLS UNROLL
							offsets[n] += 1; 
						}
						dretrievemany_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], offsets, ends_u32,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
						
						VC_READVPTRS_LOOP2D: for(unsigned int inst=0; inst<NUM_PEs; inst++){
						#pragma HLS UNROLL
							vtr_t data;
							data.begin = begins_u32[inst]; 
							data.end = ends_u32[inst]; 
							data.size = data.end - data.begin;
							if(data.end < data.begin){ data.size = 0; }
							data.vid = actvvs[inst].key; 
							data.prop = actvvs[inst].value;
							
							if(t < cfrontier_bufferREAL___size[inst]){ 
								vtr_t chunk = data; unsigned int __BATCH_SIZE = 1024;
								VC_READVPTRS_LOOP2E: for(unsigned int b=0; b<(data.size + (__BATCH_SIZE - 1)) / __BATCH_SIZE; b++){
									if((b + 1) * __BATCH_SIZE > data.size){ chunk.size = data.size % __BATCH_SIZE; } else { chunk.size = __BATCH_SIZE; }
									chunk.begin = data.begin + b * __BATCH_SIZE;
									chunk.end = chunk.begin + chunk.size;
									checkoutofbounds("acts_kernel::ERROR 003::", vptrbuffer___size[inst], VPTR_BUFFER_SIZE, NAp, NAp, NAp);
									insert_vptrbuffer(vptrbuffer___size[inst], chunk, vptr_buffer[inst]);
									vptrbuffer___size[inst] += 1;
									cummtv2[inst] += chunk.size;
								}
							}
						}
					}
					
					// calculate batches [done]
					max_limit = 0; for(unsigned int in=0; in<NUM_PEs; in++){ if(max_limit < cummtv2[in]){ max_limit = cummtv2[in]; }} 
					unsigned int num_batches = 1; unsigned int cummtv2_ = max_limit / EDGE_PACK_SIZE;
					if(cummtv2_ >= EDGE_BUFFER_SIZE){
						if(cummtv2_ / 4 < EDGE_BUFFER_SIZE){ num_batches = 4; }
						else if(cummtv2_ / 8 < EDGE_BUFFER_SIZE){ num_batches = 8; } 
						else if(cummtv2_ / 16 < EDGE_BUFFER_SIZE){ num_batches = 16; }
						else { cout<<"acts_kernel:: ERROR 2367: suitable n not found. EXITING..."<<endl; exit(EXIT_FAILURE); }
					}
					max_limit = 0; for(unsigned int in=0; in<NUM_PEs; in++){ if(max_limit < vptrbuffer___size[in]){ max_limit = vptrbuffer___size[in]; }}
					unsigned int batch_size = (max_limit + (num_batches - 1)) / num_batches;
				
					// read and process edges 
					VC_READANDPROCESS_EDGES_LOOP1: for(unsigned int n=0; n<num_batches; n++){
						for(unsigned int in=0; in<NUM_PEs; in++){ 
						#pragma HLS UNROLL
							edges_buffer___size[in] = 0; // reset
						} 
						
						// load edges // original
						/* VC_READ_EDGES_LOOP1: for(unsigned int k=0; k<batch_size; k++){ 
						#pragma HLS PIPELINE II=1
							for(unsigned int inst=0; inst<NUM_PEs; inst++){ if((n * batch_size) + k < vptrbuffer___size[inst]){ 
								vtr_t vptr_data = retrieve_vptrbuffer((n * batch_size) + k, vptr_buffer[inst]); 
								unsigned int vid = vptr_data.vid;
								unsigned int edges_size = vptr_data.end - vptr_data.begin; 
								unsigned int edgelist_size = (edges_size / EDGE_PACK_SIZE) + 2; if((edges_size == 0) || (vptr_data.end < vptr_data.begin)){ edgelist_size = 0; }
								unsigned int uprop = vptr_data.prop;
								
								VC_READ_EDGES_LOOP1B: for(unsigned int t=0; t<edgelist_size; t++){
								#pragma HLS PIPELINE II=1
									edge3_vec_dt edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA[inst], HBM_channelB[inst], inst);
									edge3_type edge_list[EDGE_PACK_SIZE];
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										if(edges.data[v].srcvid == vid && edges.data[v].srcvid != INVALIDDATA){ edge_list[v] = edges.data[v]; edge_list[v].srcvid = uprop; } else { edge_list[v].srcvid = INVALIDDATA; edge_list[v].dstvid = INVALIDDATA; }
									}
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										checkoutofbounds("acts_kernel::ERROR 812::", edges_buffer___size[inst], EDGE_BUFFER_SIZE, vptrbuffer___size[inst], inst, edgelist_size);
										insert_edgesbuffer(v, edges_buffer___size[inst], edge_list[v], edges_buffer[inst]); // NEW
									}
									edges_buffer___size[inst] += 1;
									// if(edges_buffer___size[inst] + 2 < EDGE_BUFFER_SIZE){ edges_buffer___size[inst] += 1; } // FIXME. 'forces correctness'
								}
							}} 
						} */
						
						// load edges
						VC_READ_EDGES_LOOP1: for(unsigned int k=0; k<batch_size; k++){ 
						#pragma HLS PIPELINE II=1
	
							// if((n * batch_size) + k < vptrbuffer___size[0]){	
								load_edges(0, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[0], vptr_buffer[0], &edges_buffer___size[0], edges_buffer[0], HBM_channelA0, HBM_channelB0);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[1]){	
								load_edges(1, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[1], vptr_buffer[1], &edges_buffer___size[1], edges_buffer[1], HBM_channelA1, HBM_channelB1);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[2]){	
								load_edges(2, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[2], vptr_buffer[2], &edges_buffer___size[2], edges_buffer[2], HBM_channelA2, HBM_channelB2);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[3]){	
								load_edges(3, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[3], vptr_buffer[3], &edges_buffer___size[3], edges_buffer[3], HBM_channelA3, HBM_channelB3);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[4]){	
								load_edges(4, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[4], vptr_buffer[4], &edges_buffer___size[4], edges_buffer[4], HBM_channelA4, HBM_channelB4);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[5]){	
								load_edges(5, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[5], vptr_buffer[5], &edges_buffer___size[5], edges_buffer[5], HBM_channelA5, HBM_channelB5);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[6]){	
								load_edges(6, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[6], vptr_buffer[6], &edges_buffer___size[6], edges_buffer[6], HBM_channelA6, HBM_channelB6);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[7]){	
								load_edges(7, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[7], vptr_buffer[7], &edges_buffer___size[7], edges_buffer[7], HBM_channelA7, HBM_channelB7);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[8]){	
								load_edges(8, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[8], vptr_buffer[8], &edges_buffer___size[8], edges_buffer[8], HBM_channelA8, HBM_channelB8);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[9]){	
								load_edges(9, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[9], vptr_buffer[9], &edges_buffer___size[9], edges_buffer[9], HBM_channelA9, HBM_channelB9);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[10]){	
								load_edges(10, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[10], vptr_buffer[10], &edges_buffer___size[10], edges_buffer[10], HBM_channelA10, HBM_channelB10);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[11]){	
								load_edges(11, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[11], vptr_buffer[11], &edges_buffer___size[11], edges_buffer[11], HBM_channelA11, HBM_channelB11);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[12]){	
								load_edges(12, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[12], vptr_buffer[12], &edges_buffer___size[12], edges_buffer[12], HBM_channelA12, HBM_channelB12);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[13]){	
								load_edges(13, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[13], vptr_buffer[13], &edges_buffer___size[13], edges_buffer[13], HBM_channelA13, HBM_channelB13);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[14]){	
								load_edges(14, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[14], vptr_buffer[14], &edges_buffer___size[14], edges_buffer[14], HBM_channelA14, HBM_channelB14);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[15]){	
								load_edges(15, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[15], vptr_buffer[15], &edges_buffer___size[15], edges_buffer[15], HBM_channelA15, HBM_channelB15);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[16]){	
								load_edges(16, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[16], vptr_buffer[16], &edges_buffer___size[16], edges_buffer[16], HBM_channelA16, HBM_channelB16);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[17]){	
								load_edges(17, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[17], vptr_buffer[17], &edges_buffer___size[17], edges_buffer[17], HBM_channelA17, HBM_channelB17);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[18]){	
								load_edges(18, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[18], vptr_buffer[18], &edges_buffer___size[18], edges_buffer[18], HBM_channelA18, HBM_channelB18);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[19]){	
								load_edges(19, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[19], vptr_buffer[19], &edges_buffer___size[19], edges_buffer[19], HBM_channelA19, HBM_channelB19);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[20]){	
								load_edges(20, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[20], vptr_buffer[20], &edges_buffer___size[20], edges_buffer[20], HBM_channelA20, HBM_channelB20);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[21]){	
								load_edges(21, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[21], vptr_buffer[21], &edges_buffer___size[21], edges_buffer[21], HBM_channelA21, HBM_channelB21);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[22]){	
								load_edges(22, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[22], vptr_buffer[22], &edges_buffer___size[22], edges_buffer[22], HBM_channelA22, HBM_channelB22);
							// }
	
							// if((n * batch_size) + k < vptrbuffer___size[23]){	
								load_edges(23, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[23], vptr_buffer[23], &edges_buffer___size[23], edges_buffer[23], HBM_channelA23, HBM_channelB23);
							// }
		
						}			
						// load edges 
						/* VC_READ_EDGES_LOOP1: for(unsigned int k=0; k<batch_size; k++){ 
						#pragma HLS PIPELINE II=1
							for(unsigned int inst=0; inst<NUM_PEs; inst++){ if((n * batch_size) + k < vptrbuffer___size[inst]){ 
								vtr_t vptr_data = retrieve_vptrbuffer((n * batch_size) + k, vptr_buffer[inst]); 
								unsigned int vid = vptr_data.vid;
								unsigned int edges_size = vptr_data.end - vptr_data.begin; 
								unsigned int edgelist_size = (edges_size / EDGE_PACK_SIZE) + 2; if((edges_size == 0) || (vptr_data.end < vptr_data.begin)){ edgelist_size = 0; }
								unsigned int uprop = vptr_data.prop;
								
								// cout<<"--------++++++++++++++++++++++++++------------- vid: "<<vid<<", edgelist_size: "<<edgelist_size<<endl;
								
								VC_READ_EDGES_LOOP1B: for(unsigned int t=0; t<edgelist_size; t++){
								#pragma HLS PIPELINE II=1
									
									edge3_vec_dt edges;
									if(inst == 0){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA0, HBM_channelB0, inst);
									
										
									}
									if(inst == 1){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA1, HBM_channelB1, inst);
									
										
									}
									if(inst == 2){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA2, HBM_channelB2, inst);
									
										
									}
									if(inst == 3){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA3, HBM_channelB3, inst);
									
										
									}
									if(inst == 4){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA4, HBM_channelB4, inst);
									
										
									}
									if(inst == 5){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA5, HBM_channelB5, inst);
									
										
									}
									if(inst == 6){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA6, HBM_channelB6, inst);
									
										
									}
									if(inst == 7){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA7, HBM_channelB7, inst);
									
										
									}
									if(inst == 8){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA8, HBM_channelB8, inst);
									
										
									}
									if(inst == 9){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA9, HBM_channelB9, inst);
									
										
									}
									if(inst == 10){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA10, HBM_channelB10, inst);
									
										
									}
									if(inst == 11){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA11, HBM_channelB11, inst);
									
										
									}
									if(inst == 12){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA12, HBM_channelB12, inst);
									
										
									}
									if(inst == 13){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA13, HBM_channelB13, inst);
									
										
									}
									if(inst == 14){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA14, HBM_channelB14, inst);
									
										
									}
									if(inst == 15){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA15, HBM_channelB15, inst);
									
										
									}
									if(inst == 16){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA16, HBM_channelB16, inst);
									
										
									}
									if(inst == 17){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA17, HBM_channelB17, inst);
									
										
									}
									if(inst == 18){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA18, HBM_channelB18, inst);
									
										
									}
									if(inst == 19){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA19, HBM_channelB19, inst);
									
										
									}
									if(inst == 20){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA20, HBM_channelB20, inst);
									
										
									}
									if(inst == 21){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA21, HBM_channelB21, inst);
									
										
									}
									if(inst == 22){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA22, HBM_channelB22, inst);
									
										
									}
									if(inst == 23){
										edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA23, HBM_channelB23, inst);
									
										
									}
	
									// edge3_vec_dt edges = dretrieve_csr_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], (vptr_data.begin / EDGE_PACK_SIZE) + t, HBM_channelA, HBM_channelB, inst);
									
									// for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
										// cout<<"--------++++++++++++++++++++++++++------------- inst: "<<inst<<", edges.data["<<v<<"].srcvid: "<<edges.data[v].srcvid<<", edges.data["<<v<<"].dstvid: "<<edges.data[v].dstvid<<", edges.data["<<v<<"].valid: "<<edges.data[v].valid<<endl;
									// }
									
									edge3_type edge_list[EDGE_PACK_SIZE];
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										if(edges.data[v].srcvid == vid && edges.data[v].srcvid != INVALIDDATA){ edge_list[v] = edges.data[v]; edge_list[v].srcvid = uprop; } else { edge_list[v].srcvid = INVALIDDATA; edge_list[v].dstvid = INVALIDDATA; }
									}
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										checkoutofbounds("acts_sw::ERROR 812::", edges_buffer___size[inst], EDGE_BUFFER_SIZE, vptrbuffer___size[inst], inst, edgelist_size);
										insert_edgesbuffer(v, edges_buffer___size[inst], edge_list[v], edges_buffer[inst]); // NEW
									}
									edges_buffer___size[inst] += 1;
									// if(edges_buffer___size[inst] + 2 < EDGE_BUFFER_SIZE){ edges_buffer___size[inst] += 1; } // FIXME. 'forces correctness'
								}
							}} 
						} */
						// exit(EXIT_SUCCESS);
							
						// process & partition edges [done]
						max_limit = 0; for(unsigned int in=0; in<NUM_PEs; in++){ if(max_limit < edges_buffer___size[in]){ max_limit = edges_buffer___size[in]; }} 
						VC_PROCESS_EDGES_LOOP1: for(unsigned int t=0; t<max_limit; t++){ 
						#pragma HLS PIPELINE II=1
							for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
							#pragma HLS UNROLL
								for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
								#pragma HLS UNROLL
									edge3_type edge = retrieve_edgesbuffer(v, t, edges_buffer[inst]); // NEW
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
										checkoutofbounds("acts_kernel::ERROR 813a::", inst_mirror, NUM_PEs,  edge.dstvid, inst_mirror, NAp);
										checkoutofbounds("acts_kernel::ERROR 813b::", updates_tmpbuffer___size[inst][inst_mirror], CSRBUFFER_SIZE, NAp, NAp, NAp);
										if(t < edges_buffer___size[inst]){ // NEW
											insert_updatestmpbuffer(inst_mirror, updates_tmpbuffer___size[inst][inst_mirror], update, updates_tmpbuffer[inst]); // 
											updates_tmpbuffer___size[inst][inst_mirror] += 1;	
										}
									}
								}
							}
						}
					}
					
					// exchange updates between HBMs
					VC_EXCHANGE_UPDATES_LOOP1: for(unsigned int inst_mirror=0; inst_mirror<NUM_PEs; inst_mirror++){
						VC_EXCHANGE_UPDATES_LOOP1B: for(unsigned int inst=0; inst<NUM_PEs; inst++){
							VC_EXCHANGE_UPDATES_LOOP1C: for(unsigned int t=0; t<updates_tmpbuffer___size[inst][inst_mirror]; t++){
								keyvalue_t update = retrieve_updatestmpbuffer(inst_mirror, t, updates_tmpbuffer[inst]); // 
								unsigned int p_v = get_local(update.key) / MAX_APPLYPARTITION_SIZE; // FIXME.
								keyvalue_t update_local = update; update_local.key = get_local_to_vpartition(get_local(update.key));
								unsigned int v = update_local.key % EDGE_PACK_SIZE;
								#ifdef _DEBUGMODE_KERNELPRINTS//4
								cout<<"VC - EXCHANGE UPDATE SEEN @: [inst: "<<inst<<", inst_mirror: "<<inst_mirror<<"], [update.key: "<<update.key<<", update.value: "<<update.value<<"]"<<endl;
								#endif 
								
								checkoutofbounds("acts_kernel::ERROR 123::", p_v, __NUM_APPLYPARTITIONS, update.key, get_local(update.key), MAX_APPLYPARTITION_SIZE);
								checkoutofbounds("acts_kernel::ERROR 122::", (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + updates_buffer___size[inst_mirror][v][p_v], MAX_APPLYPARTITION_VECSIZE, updates_buffer___size[inst_mirror][v][p_v], inst_mirror, p_v);
								insert_updatesbuffer(v, (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + updates_buffer___size[inst_mirror][v][p_v], update_local, updates_buffer[inst_mirror]);
								updates_buffer___size[inst_mirror][v][p_v] += 1;
								vpartition_vertices[0][p_v].count = 1;
								
								unsigned int dstvid_lpv = update_local.key / EDGE_PACK_SIZE; // FIXME.
								checkoutofbounds("acts_kernel::ERROR 171::", dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE, BLOCKRAM_SIZE, NAp, NAp, NAp);
								checkoutofbounds("acts_kernel::ERROR 172::", p_v, __NUM_APPLYPARTITIONS, NAp, NAp, NAp);
								if(enable___collectactivedstvids == true){ insert_statsbuffer(dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE, p_v, 1, stats_buffer[inst_mirror]); }
							}
						}
					}
				} else { // edge-centric [done]
					hybrid_map[GraphIter][p_u] = 1;
					EC_PROCESS_EDGES_LOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
						map_t maps[NUM_PEs];
						dretrievemany_actpackvptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS], p_u, llp_set, maps,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
						unsigned int max_sz = 0; for(unsigned int n=0; n<NUM_PEs; n++){ limits[n] = maps[n].size; } for(unsigned int n=0; n<NUM_PEs; n++){ if(max_sz < limits[n]){ max_sz = limits[n]; }}
						
						// process edges [done]
						EC_PROCESS_EDGES_LOOP1B: for(unsigned int it=0; it<(max_sz + (BLOCKRAM_SIZE - 1)) / BLOCKRAM_SIZE; it++){
							for(unsigned int n=0; n<NUM_PEs; n++){ updates_tmpbuffer___size[n][0] = 0; } 
							unsigned int sz = 0; if((it + 1) * BLOCKRAM_SIZE >= max_sz){ sz = max_sz - (it * BLOCKRAM_SIZE); } else { sz = BLOCKRAM_SIZE; }
							
							// process edge batch [done] 
							EC_PROCESS_EDGES_LOOP1C: for(unsigned int t_=0; t_<sz; t_++){ 
							#pragma HLS PIPELINE II=1
								unsigned int t = (it * BLOCKRAM_SIZE) + t_;
								unsigned int offsets[NUM_PEs]; edge3_vec_dt edge3_vecs[NUM_PEs]; for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = maps[n].offset; }
								
								dretrievemany_act_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
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
											checkoutofbounds("acts_kernel::ERROR 223::", srcvid_lp, MAX_UPARTITION_SIZE, NAp, NAp, NAp);
											if(srcvid_lp % EDGE_PACK_SIZE != v){ cout<<"acts_kernel ERROR 2345:: srcvid_lp("<<srcvid_lp<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
											
											uprop[inst][v] = retrieve_udatabuffer(v, srcvid_lpv, vdata_buffer[inst]);
											
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
									checkoutofbounds("acts_kernel::ERROR 213::", rotateby, EDGE_PACK_SIZE, NAp, NAp, NAp);
									rearrangeLayoutVx16B(rotateby, update_in[inst], update_out[inst]);
									#ifdef _DEBUGMODE_CHECKS3
									checkoutofbounds("acts_kernel::ERROR 59::", rotateby, EDGE_PACK_SIZE, sample_key, NAp, NAp);
									checkoutofbounds("acts_kernel::ERROR 60::", rotate_forward, 2, sample_key, NAp, NAp);	
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
									
									if(isvalid == true){ // NEW
										for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
										#pragma HLS UNROLL
											insert_updatestmpbuffer(v, updates_tmpbuffer___size[inst][0], updates_vec.data[v], updates_tmpbuffer[inst]); //
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
												checkoutofbounds("acts_kernel::ERROR 623::", index, BLOCKRAM_SIZE, update_out[inst][v].key, dstvid_lpv, (MAX_APPLYPARTITION_VECSIZE / BLOCKRAM_SIZE));
												insert_statstmpbuffer(index, v, 1, stats_tmpbuffer[inst]);
											}
										}
									}
								}}
							}
						
							// store updates batch [done]
							uint512_vec_dt updates_vec; uint512_vec_dt updates_vecs[NUM_PEs];
							max_limit = 0; for(unsigned int n=0; n<NUM_PEs; n++){ if(max_limit < updates_tmpbuffer___size[n][0]){ max_limit = updates_tmpbuffer___size[n][0]; }}
							unsigned int offsets[NUM_PEs]; for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = actpackupdates_dram___size[n][llp_set]; }
							EC_PROCESS_EDGES_LOOP1F: for(unsigned int t=0; t<max_limit; t++){ 
							#pragma HLS PIPELINE II=1
								EC_PROCESS_EDGES_LOOP1G: for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
								#pragma HLS UNROLL
									EC_PROCESS_EDGES_LOOP1H: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									#pragma HLS UNROLL
										updates_vecs[inst].data[v] = retrieve_updatestmpbuffer(v, t, updates_tmpbuffer[inst]);
										if(t < updates_tmpbuffer___size[inst][0]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
									}
								}
								dinsertmany_updatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[llp_set], llp_set, offsets, t, updates_vecs, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB, updatesptrs);
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
								if(enable___collectactivedstvids == true){ retrieve_statstmpbuffer(t, data, stats_tmpbuffer[inst]); }
								checkoutofbounds("acts_kernel::ERROR 023::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
								if(data[0]==1 || data[1]==1 || data[2]==1 || data[3]==1 || data[4]==1 || data[5]==1 || data[6]==1 || data[7]==1 || data[8]==1 || data[9]==1 || data[10]==1 || data[11]==1 || data[12]==1 || data[13]==1 || data[14]==1 || data[15]==1){ 
									if(enable___collectactivedstvids == true){ insert_statsbuffer(t, llp_set, 1, stats_buffer[inst]); }
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
						if(t < updates_buffer___size[inst][v][p_v]){ updates_vecs[inst].data[v] = retrieve_updatesbuffer(v, (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + t, updates_buffer[inst]); }
						else { updates_vecs[inst].data[v].key = INVALIDDATA; updates_vecs[inst].data[v].value = INVALIDDATA;  }
						if(t < max_limits[inst]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						if(t < updates_buffer___size[inst][v][p_v]){ cout<<"VC - COMMIT CSR UPDATES SEEN @: [inst: "<<inst<<"], [updates_vecs["<<inst<<"].data["<<v<<"].key: "<<updates_vecs[inst].data[v].key<<", updates_vecs["<<inst<<"].data["<<v<<"].value: "<<updates_vecs[inst].data[v].value<<"]"<<endl; }
						#endif
					}
				}
				checkoutofbounds("acts_kernel::ERROR 152::", actpackupdates_dram___size[0][p_v], HBM_CHANNEL_SIZE, NAp, NAp, NAp);
				dinsertmany_updatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v], p_v, offsets, t, updates_vecs, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB, updatesptrs);
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
						if(retrieve_statsbuffer(t, p_v, stats_buffer[inst]) == 1){ 
							#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
							cout<<"VC - PREPARE ACTIVE DESTIDS SEEN @: [inst: "<<inst<<"], [p_v: "<<p_v<<", t: "<<t<<"]"<<endl;
							#endif
							insert_stats2buffer(stats2_buffer___size[inst][p_v], p_v, t, stats2_buffer[inst]); 
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
				for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = 0; }
				if(stats_buffer___size[0][p_v] < threshold___activedstvids && enable___collectactivedstvids == true){
					#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ // FIXME.
						for(unsigned int k=0; k<stats2_buffer___size[inst][p_v]; k++){
							unsigned int offset = retrieve_stats2buffer(k, p_v, stats2_buffer[inst]) * ACTVUPDATESBLOCK_VECSIZE;
							checkoutofbounds("acts_kernel::ERROR 117::", offset, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
							for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								if(offset + t < MAX_APPLYPARTITION_VECSIZE){ 
									dretrieve_vdatadram(voffset, offset + t, data, HBM_channelA[inst], HBM_channelB[inst], inst);
									insert_vdatabuffer(offset + t, data, vdata_buffer[inst]);
								}
							}
						}	
					}
					#endif 
				} else {
					for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
					#pragma HLS PIPELINE II=1
						dretrievemany_vdatadram(voffset, offsets, t, datas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
						#pragma HLS UNROLL
							insert_vdatabuffer(t, datas[inst], vdata_buffer[inst]);
						}
					}
				}
				
				// apply updates [done]
				uint512_vec_dt updates_vecs[NUM_PEs];
				max_limit = 0; for(unsigned int inst=0; inst<NUM_PEs; inst++){ limits[inst] = actpackupdates_dram___size[inst][p_v]; } for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}
				for(unsigned int t=0; t<max_limit; t++){
				#pragma HLS PIPELINE II=1
					dretrievemany_udatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v], p_v, t, updates_vecs,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB, updatesptrs); // NEW
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
					#pragma HLS UNROLL
						uint512_vec_dt updates_vec = updates_vecs[inst];
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							keyvalue_t update = updates_vec.data[v];
							unsigned int dstvid_lp = update.key; 
							unsigned int dstvid_lpv = update.key / EDGE_PACK_SIZE; 
							if(update.key != INVALIDDATA){
								if(t < limits[inst] && ((dstvid_lp % EDGE_PACK_SIZE) != v)){ cout<<"acts_kernel 234:: dstvid_lp("<<dstvid_lp<<") % v("<<v<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }
								if(t < limits[inst]){ checkoutofbounds("acts_kernel::ERROR 727::", dstvid_lp, MAX_APPLYPARTITION_SIZE, NAp, inst, update.key); }
								vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, vdata_buffer[inst]);
								unsigned int new_vprop = 0; if(update.value < vprop.prop){ new_vprop = update.value; } else { new_vprop = vprop.prop; }
								if(new_vprop != vprop.prop){
									#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
									cout<<"APPLY (ACT-PACK) UPDATE SEEN @: inst: ["<<inst<<"]: dstvid_lp: "<<dstvid_lp<<", dstvid_lpv: "<<dstvid_lpv<<", new_vprop: "<<new_vprop<<", vid: "<<update.key<<endl;
									#endif
									vprop_t newprop; newprop.prop = new_vprop; newprop.mask = MASK_CODE_AU;
									if(t < limits[inst]){ insert_vdatabuffer(v, dstvid_lpv, newprop, vdata_buffer[inst]); } // NEW
								}
							}
						}
					}
				}

				// collect frontier information [done]
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
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
						#pragma HLS UNROLL
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							#pragma HLS UNROLL
								checkoutofbounds("acts_kernel::ERROR 713::", dstvid_lpv, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
								vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, vdata_buffer[inst]);
								unsigned int vid = vid_first1[inst] + v;
								#ifdef _DEBUGMODE_CHECKS3
								if(vid % EDGE_PACK_SIZE != v){ cout<<"acts_kernel:: ERROR 234. vid("<<vid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<vid % EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
								#endif
								unsigned int p_actvv = dstvid_lpv / MAX_ACTVV_VECSIZE;
								if(vprop.mask == MASK_CODE_AU){ 
									#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
									cout<<"COLLECT FRONTIER INFORMATION SEEN @: inst: ["<<inst<<"]: dstvid_lpv: "<<dstvid_lpv<<", v: "<<v<<", p__u__: "<<(vid / MAX_UPARTITION_SIZE)<<", vid: "<<vid<<endl;
									#endif
									frontier_t actvv; actvv.key = vid; actvv.value = vprop.prop; 
									if(t < limits[inst]){ // NEW
										insert_nfrontierbuffer((p_actvv * MAX_ACTVV_VECSIZE) + nfrontier_buffer___size[inst][v][p_actvv], v, actvv, nfrontier_buffer[inst]); // 
										nfrontier_buffer___size[inst][v][p_actvv] += 1;
									}
								}
							}
						}
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
				
				// commit frontier information [done]
				frontier_t actvv[EDGE_PACK_SIZE];
				for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = 0; }
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
						#pragma HLS UNROLL
							retrieve_nfrontierbuffer(p_actvv * MAX_ACTVV_VECSIZE + t, actvvs[inst], nfrontier_buffer[inst]);
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ // 
							#pragma HLS UNROLL
								if(t >= nfrontier_buffer___size[inst][v][p_actvv]){ actvvs[inst][v].key = INVALIDDATA; actvvs[inst][v].value = INVALIDDATA; }
								if(t < max_limits[inst]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
							}
						}}
						dinsertmany_nfrontierdram(offset, p_actvv_, offsets, t, actvvs, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
						for(unsigned int n=0; n<NUM_PEs; n++){ 
						#pragma HLS UNROLL
							if(t < max_limits[n]){ nfrontier_dram___size[n][0][p_actvv_] += 1; }
						}
					}
				}
				
				// commit destination properties
				for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = 0; }
				if(stats_buffer___size[0][p_v] < threshold___activedstvids && enable___collectactivedstvids == true){
					#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ // FIXME.
						for(unsigned int k=0; k<stats2_buffer___size[inst][p_v]; k++){	
							unsigned int offset = retrieve_stats2buffer(k, p_v, stats2_buffer[inst]) * ACTVUPDATESBLOCK_VECSIZE;
							for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								if(offset + t < MAX_APPLYPARTITION_VECSIZE){ 
									checkoutofbounds("acts_kernel::ERROR 217::", offset + t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
									retrieve_vdatabuffer(offset + t, data, vdata_buffer[inst]);
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
									#pragma HLS UNROLL
										data[v].mask = INVALIDMASK; // clear mask
									} 
									dinsert_vdatadram(voffset, offset + t, data, HBM_channelA[inst], HBM_channelB[inst], inst);
								}
							}
						}	
					}
					#endif 
				} else {
					for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
					#pragma HLS PIPELINE II=1
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
						#pragma HLS UNROLL
							retrieve_vdatabuffer(t, datas[inst], vdata_buffer[inst]);
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
							#pragma HLS UNROLL
								datas[inst][v].mask = INVALIDMASK; // clear mask. 
							}
						}
						dinsertmany_vdatadram(voffset, offsets, t, datas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, HBM_channelA, HBM_channelB);
					}
				}
			}
		} // p_v
		
		/* // end of GAS iteration - gathering frontier information - original
		unsigned int totalactvvs2 = 0; frontier_t actvv[EDGE_PACK_SIZE]; 
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ upartition_vertices[p_u].count = 0; }
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ cfrontier_dram___size[p_u] = 0; } // reset
		for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			for(unsigned int inst=0; inst<NUM_PEs; inst++){
				for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
					unsigned int p_actvv_ = p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION + p_actvv;
					checkoutofbounds("acts_kernel::ERROR 225::", p_actvv_, MAX_NUM_UPARTITIONS, NAp, NAp, NAp);
					unsigned int offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + (p_actvv_ * MAX_APPLYPARTITION_VECSIZE);
					for(unsigned int t=0; t<nfrontier_dram___size[inst][0][p_actvv_]; t++){
					#pragma HLS PIPELINE II=1
						upartition_vertices[p_actvv_].count += 1; 
						dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA[inst], HBM_channelB[inst], inst);
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						for(unsigned int v=0; v<0; v++){ cout<<"TRANSFER VID-BASED FRONTIERS SEEN @: H: "<<inst<<" => vid: "<<actvv[v].key<<", p_actvv_: "<<p_actvv_<<endl; }
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA && actvv[v].key / MAX_UPARTITION_SIZE != p_actvv_){ cout<<"ERROR 234: actvv["<<v<<"].key("<<actvv[v].key<<") / MAX_UPARTITION_SIZE("<<MAX_UPARTITION_SIZE<<")(="<<actvv[v].key / MAX_UPARTITION_SIZE<<") != p_actvv_("<<p_actvv_<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }}				
						#endif 
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA){ totalactvvs2 += 1; }} // FIXME.
						insert_cfrontierdram(p_actvv_, cfrontier_dram___size[p_actvv_], actvv, HBM_center); cfrontier_dram___size[p_actvv_] += 1;
					}
				}
			}
		} */
		
		// end of GAS iteration - gathering frontier information
		unsigned int totalactvvs2 = 0; frontier_t actvv[EDGE_PACK_SIZE]; 
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ upartition_vertices[p_u].count = 0; }
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ cfrontier_dram___size[p_u] = 0; } // reset
		for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			transport_frontier(0, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[0], upartition_vertices, HBM_channelA0, HBM_channelB0, HBM_center, &totalactvvs2);
			transport_frontier(1, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[1], upartition_vertices, HBM_channelA1, HBM_channelB1, HBM_center, &totalactvvs2);
			transport_frontier(2, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[2], upartition_vertices, HBM_channelA2, HBM_channelB2, HBM_center, &totalactvvs2);
			transport_frontier(3, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[3], upartition_vertices, HBM_channelA3, HBM_channelB3, HBM_center, &totalactvvs2);
			transport_frontier(4, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[4], upartition_vertices, HBM_channelA4, HBM_channelB4, HBM_center, &totalactvvs2);
			transport_frontier(5, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[5], upartition_vertices, HBM_channelA5, HBM_channelB5, HBM_center, &totalactvvs2);
			transport_frontier(6, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[6], upartition_vertices, HBM_channelA6, HBM_channelB6, HBM_center, &totalactvvs2);
			transport_frontier(7, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[7], upartition_vertices, HBM_channelA7, HBM_channelB7, HBM_center, &totalactvvs2);
			transport_frontier(8, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[8], upartition_vertices, HBM_channelA8, HBM_channelB8, HBM_center, &totalactvvs2);
			transport_frontier(9, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[9], upartition_vertices, HBM_channelA9, HBM_channelB9, HBM_center, &totalactvvs2);
			transport_frontier(10, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[10], upartition_vertices, HBM_channelA10, HBM_channelB10, HBM_center, &totalactvvs2);
			transport_frontier(11, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[11], upartition_vertices, HBM_channelA11, HBM_channelB11, HBM_center, &totalactvvs2);
			transport_frontier(12, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[12], upartition_vertices, HBM_channelA12, HBM_channelB12, HBM_center, &totalactvvs2);
			transport_frontier(13, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[13], upartition_vertices, HBM_channelA13, HBM_channelB13, HBM_center, &totalactvvs2);
			transport_frontier(14, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[14], upartition_vertices, HBM_channelA14, HBM_channelB14, HBM_center, &totalactvvs2);
			transport_frontier(15, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[15], upartition_vertices, HBM_channelA15, HBM_channelB15, HBM_center, &totalactvvs2);
			transport_frontier(16, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[16], upartition_vertices, HBM_channelA16, HBM_channelB16, HBM_center, &totalactvvs2);
			transport_frontier(17, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[17], upartition_vertices, HBM_channelA17, HBM_channelB17, HBM_center, &totalactvvs2);
			transport_frontier(18, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[18], upartition_vertices, HBM_channelA18, HBM_channelB18, HBM_center, &totalactvvs2);
			transport_frontier(19, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[19], upartition_vertices, HBM_channelA19, HBM_channelB19, HBM_center, &totalactvvs2);
			transport_frontier(20, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[20], upartition_vertices, HBM_channelA20, HBM_channelB20, HBM_center, &totalactvvs2);
			transport_frontier(21, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[21], upartition_vertices, HBM_channelA21, HBM_channelB21, HBM_center, &totalactvvs2);
			transport_frontier(22, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[22], upartition_vertices, HBM_channelA22, HBM_channelB22, HBM_center, &totalactvvs2);
			transport_frontier(23, p_v, globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS], cfrontier_dram___size, nfrontier_dram___size[23], upartition_vertices, HBM_channelA23, HBM_channelB23, HBM_center, &totalactvvs2);
		}
		
		/* // end of GAS iteration - gathering frontier information
		unsigned int totalactvvs2 = 0; frontier_t actvv[EDGE_PACK_SIZE]; 
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ upartition_vertices[p_u].count = 0; }
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ cfrontier_dram___size[p_u] = 0; } // reset
		for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			for(unsigned int inst=0; inst<NUM_PEs; inst++){
				for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
					unsigned int p_actvv_ = p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION + p_actvv;
					checkoutofbounds("acts_kernel::ERROR 225::", p_actvv_, MAX_NUM_UPARTITIONS, NAp, NAp, NAp);
					unsigned int offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + (p_actvv_ * MAX_APPLYPARTITION_VECSIZE);
					for(unsigned int t=0; t<nfrontier_dram___size[inst][0][p_actvv_]; t++){
					#pragma HLS PIPELINE II=1
						upartition_vertices[p_actvv_].count += 1; 
						// dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA[inst], HBM_channelB[inst], inst);
						
						if(inst == 0){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA0, HBM_channelB0, inst);
						}
						if(inst == 1){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA1, HBM_channelB1, inst);
						}
						if(inst == 2){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA2, HBM_channelB2, inst);
						}
						if(inst == 3){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA3, HBM_channelB3, inst);
						}
						if(inst == 4){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA4, HBM_channelB4, inst);
						}
						if(inst == 5){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA5, HBM_channelB5, inst);
						}
						if(inst == 6){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA6, HBM_channelB6, inst);
						}
						if(inst == 7){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA7, HBM_channelB7, inst);
						}
						if(inst == 8){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA8, HBM_channelB8, inst);
						}
						if(inst == 9){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA9, HBM_channelB9, inst);
						}
						if(inst == 10){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA10, HBM_channelB10, inst);
						}
						if(inst == 11){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA11, HBM_channelB11, inst);
						}
						if(inst == 12){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA12, HBM_channelB12, inst);
						}
						if(inst == 13){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA13, HBM_channelB13, inst);
						}
						if(inst == 14){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA14, HBM_channelB14, inst);
						}
						if(inst == 15){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA15, HBM_channelB15, inst);
						}
						if(inst == 16){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA16, HBM_channelB16, inst);
						}
						if(inst == 17){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA17, HBM_channelB17, inst);
						}
						if(inst == 18){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA18, HBM_channelB18, inst);
						}
						if(inst == 19){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA19, HBM_channelB19, inst);
						}
						if(inst == 20){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA20, HBM_channelB20, inst);
						}
						if(inst == 21){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA21, HBM_channelB21, inst);
						}
						if(inst == 22){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA22, HBM_channelB22, inst);
						}
						if(inst == 23){
							dretrieve_nfrontierdram(offset, p_actvv_, t, actvv, HBM_channelA23, HBM_channelB23, inst);
						}
	
						
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						for(unsigned int v=0; v<0; v++){ cout<<"TRANSFER VID-BASED FRONTIERS SEEN @: H: "<<inst<<" => vid: "<<actvv[v].key<<", p_actvv_: "<<p_actvv_<<endl; }
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA && actvv[v].key / MAX_UPARTITION_SIZE != p_actvv_){ cout<<"ERROR 234: actvv["<<v<<"].key("<<actvv[v].key<<") / MAX_UPARTITION_SIZE("<<MAX_UPARTITION_SIZE<<")(="<<actvv[v].key / MAX_UPARTITION_SIZE<<") != p_actvv_("<<p_actvv_<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }}				
						#endif 
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA){ totalactvvs2 += 1; }} // FIXME.
						insert_cfrontierdram(p_actvv_, cfrontier_dram___size[p_actvv_], actvv, HBM_center); cfrontier_dram___size[p_actvv_] += 1;
					}
				}
			}
		} */
		
		for(unsigned int t=0; t<__NUM_UPARTITIONS; t++){ if(hybrid_map[GraphIter][t] == 0){ cout<<"V, "; } else if(hybrid_map[GraphIter][t] == 1){ cout<<"E, "; } else { cout<<"-, "; }} cout<<endl;
		if(run_vertex_centric == true){ cout<<"### Vertex-Centric GAS iteration: "<<GraphIter + 1<<" ["<<totalactvvs2<<" active vertices]"<<endl; } else { cout<<"### Edge-Centric GAS iteration: "<<GraphIter + 1<<" ["<<totalactvvs2<<" active vertices]"<<endl;	 }
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ if(upartition_vertices[p_u].count > 0){ cout<<"1, "; } else { cout<<"0, "; }} cout<<endl;
		if(totalactvvs2 == 0 || GraphIter >= MAXNUMGRAPHITERATIONS){ cout<<"no more active vertices to process. breaking out... "<<endl; maxGraphIter = GraphIter; break; }
		for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ nfrontier_dram___size[inst][0][p_u] = 0; }}
	} // NUM_ITs
	return;
}











