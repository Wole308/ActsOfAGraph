#include "acts_kernel.h"
using namespace std;
// __NOT__YET__IMPLEMENTED___
// __OBSOLETE___

// #define ___ENABLE___COLLECTACTIVEDSTVIDS___
#define ___FORCE_SUCCESS___

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
void master_centerinsert_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelX_t * HBM_center){
	unsigned int data_tmp[HBM_AXI_PACK_SIZE];
	
	if(data[0] == INVALIDDATA){ data_tmp[0] = INVALIDDATA; } else { data_tmp[0] = (data[1] << 28) | data[0];  }		
	
	if(data[2] == INVALIDDATA){ data_tmp[1] = INVALIDDATA; } else { data_tmp[1] = (data[3] << 28) | data[2];  }		
	
	if(data[4] == INVALIDDATA){ data_tmp[2] = INVALIDDATA; } else { data_tmp[2] = (data[5] << 28) | data[4];  }		
	
	if(data[6] == INVALIDDATA){ data_tmp[3] = INVALIDDATA; } else { data_tmp[3] = (data[7] << 28) | data[6];  }		
	
	if(data[8] == INVALIDDATA){ data_tmp[4] = INVALIDDATA; } else { data_tmp[4] = (data[9] << 28) | data[8];  }		
	
	if(data[10] == INVALIDDATA){ data_tmp[5] = INVALIDDATA; } else { data_tmp[5] = (data[11] << 28) | data[10];  }		
	
	if(data[12] == INVALIDDATA){ data_tmp[6] = INVALIDDATA; } else { data_tmp[6] = (data[13] << 28) | data[12];  }		
	
	if(data[14] == INVALIDDATA){ data_tmp[7] = INVALIDDATA; } else { data_tmp[7] = (data[15] << 28) | data[14];  }		
	
	if(data[16] == INVALIDDATA){ data_tmp[8] = INVALIDDATA; } else { data_tmp[8] = (data[17] << 28) | data[16];  }		
	
	if(data[18] == INVALIDDATA){ data_tmp[9] = INVALIDDATA; } else { data_tmp[9] = (data[19] << 28) | data[18];  }		
	
	if(data[20] == INVALIDDATA){ data_tmp[10] = INVALIDDATA; } else { data_tmp[10] = (data[21] << 28) | data[20];  }		
	
	if(data[22] == INVALIDDATA){ data_tmp[11] = INVALIDDATA; } else { data_tmp[11] = (data[23] << 28) | data[22];  }		
	
	if(data[24] == INVALIDDATA){ data_tmp[12] = INVALIDDATA; } else { data_tmp[12] = (data[25] << 28) | data[24];  }		
	
	if(data[26] == INVALIDDATA){ data_tmp[13] = INVALIDDATA; } else { data_tmp[13] = (data[27] << 28) | data[26];  }		
	
	if(data[28] == INVALIDDATA){ data_tmp[14] = INVALIDDATA; } else { data_tmp[14] = (data[29] << 28) | data[28];  }		
	
	if(data[30] == INVALIDDATA){ data_tmp[15] = INVALIDDATA; } else { data_tmp[15] = (data[31] << 28) | data[30];  }		
	
	#ifdef FPGA_IMPL 
	HBM_center[offset].range(31, 0) = data_tmp[0];
	HBM_center[offset].range(63, 32) = data_tmp[1];
	HBM_center[offset].range(95, 64) = data_tmp[2];
	HBM_center[offset].range(127, 96) = data_tmp[3];
	HBM_center[offset].range(159, 128) = data_tmp[4];
	HBM_center[offset].range(191, 160) = data_tmp[5];
	HBM_center[offset].range(223, 192) = data_tmp[6];
	HBM_center[offset].range(255, 224) = data_tmp[7];
	HBM_center[offset].range(287, 256) = data_tmp[8];
	HBM_center[offset].range(319, 288) = data_tmp[9];
	HBM_center[offset].range(351, 320) = data_tmp[10];
	HBM_center[offset].range(383, 352) = data_tmp[11];
	HBM_center[offset].range(415, 384) = data_tmp[12];
	HBM_center[offset].range(447, 416) = data_tmp[13];
	HBM_center[offset].range(479, 448) = data_tmp[14];
	HBM_center[offset].range(511, 480) = data_tmp[15];
	#else 
	HBM_center[offset].data[0] = data_tmp[0];
	HBM_center[offset].data[1] = data_tmp[1];
	HBM_center[offset].data[2] = data_tmp[2];
	HBM_center[offset].data[3] = data_tmp[3];
	HBM_center[offset].data[4] = data_tmp[4];
	HBM_center[offset].data[5] = data_tmp[5];
	HBM_center[offset].data[6] = data_tmp[6];
	HBM_center[offset].data[7] = data_tmp[7];
	HBM_center[offset].data[8] = data_tmp[8];
	HBM_center[offset].data[9] = data_tmp[9];
	HBM_center[offset].data[10] = data_tmp[10];
	HBM_center[offset].data[11] = data_tmp[11];
	HBM_center[offset].data[12] = data_tmp[12];
	HBM_center[offset].data[13] = data_tmp[13];
	HBM_center[offset].data[14] = data_tmp[14];
	HBM_center[offset].data[15] = data_tmp[15];
	#endif 
	return;
}
void master_centerretrieve_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelX_t * HBM_center){
	unsigned int data_tmp[HBM_AXI_PACK_SIZE];
	#ifdef FPGA_IMPL 
	data_tmp[0] = HBM_center[offset].range(31, 0);
	data_tmp[1] = HBM_center[offset].range(63, 32);
	data_tmp[2] = HBM_center[offset].range(95, 64);
	data_tmp[3] = HBM_center[offset].range(127, 96);
	data_tmp[4] = HBM_center[offset].range(159, 128);
	data_tmp[5] = HBM_center[offset].range(191, 160);
	data_tmp[6] = HBM_center[offset].range(223, 192);
	data_tmp[7] = HBM_center[offset].range(255, 224);
	data_tmp[8] = HBM_center[offset].range(287, 256);
	data_tmp[9] = HBM_center[offset].range(319, 288);
	data_tmp[10] = HBM_center[offset].range(351, 320);
	data_tmp[11] = HBM_center[offset].range(383, 352);
	data_tmp[12] = HBM_center[offset].range(415, 384);
	data_tmp[13] = HBM_center[offset].range(447, 416);
	data_tmp[14] = HBM_center[offset].range(479, 448);
	data_tmp[15] = HBM_center[offset].range(511, 480);
	#else 
	data_tmp[0] = HBM_center[offset].data[0];
	data_tmp[1] = HBM_center[offset].data[1];
	data_tmp[2] = HBM_center[offset].data[2];
	data_tmp[3] = HBM_center[offset].data[3];
	data_tmp[4] = HBM_center[offset].data[4];
	data_tmp[5] = HBM_center[offset].data[5];
	data_tmp[6] = HBM_center[offset].data[6];
	data_tmp[7] = HBM_center[offset].data[7];
	data_tmp[8] = HBM_center[offset].data[8];
	data_tmp[9] = HBM_center[offset].data[9];
	data_tmp[10] = HBM_center[offset].data[10];
	data_tmp[11] = HBM_center[offset].data[11];
	data_tmp[12] = HBM_center[offset].data[12];
	data_tmp[13] = HBM_center[offset].data[13];
	data_tmp[14] = HBM_center[offset].data[14];
	data_tmp[15] = HBM_center[offset].data[15];
	#endif 
	
	if(data_tmp[0] == INVALIDDATA){ data[0] = INVALIDDATA; data[1] = INVALIDDATA; } 
	else { data[0] = data_tmp[0] & 0x0FFFFFFF; data[1] = (data_tmp[0] >> 28) & 0x0000000F; }
	if(data_tmp[1] == INVALIDDATA){ data[2] = INVALIDDATA; data[3] = INVALIDDATA; } 
	else { data[2] = data_tmp[1] & 0x0FFFFFFF; data[3] = (data_tmp[1] >> 28) & 0x0000000F; }
	if(data_tmp[2] == INVALIDDATA){ data[4] = INVALIDDATA; data[5] = INVALIDDATA; } 
	else { data[4] = data_tmp[2] & 0x0FFFFFFF; data[5] = (data_tmp[2] >> 28) & 0x0000000F; }
	if(data_tmp[3] == INVALIDDATA){ data[6] = INVALIDDATA; data[7] = INVALIDDATA; } 
	else { data[6] = data_tmp[3] & 0x0FFFFFFF; data[7] = (data_tmp[3] >> 28) & 0x0000000F; }
	if(data_tmp[4] == INVALIDDATA){ data[8] = INVALIDDATA; data[9] = INVALIDDATA; } 
	else { data[8] = data_tmp[4] & 0x0FFFFFFF; data[9] = (data_tmp[4] >> 28) & 0x0000000F; }
	if(data_tmp[5] == INVALIDDATA){ data[10] = INVALIDDATA; data[11] = INVALIDDATA; } 
	else { data[10] = data_tmp[5] & 0x0FFFFFFF; data[11] = (data_tmp[5] >> 28) & 0x0000000F; }
	if(data_tmp[6] == INVALIDDATA){ data[12] = INVALIDDATA; data[13] = INVALIDDATA; } 
	else { data[12] = data_tmp[6] & 0x0FFFFFFF; data[13] = (data_tmp[6] >> 28) & 0x0000000F; }
	if(data_tmp[7] == INVALIDDATA){ data[14] = INVALIDDATA; data[15] = INVALIDDATA; } 
	else { data[14] = data_tmp[7] & 0x0FFFFFFF; data[15] = (data_tmp[7] >> 28) & 0x0000000F; }
	if(data_tmp[8] == INVALIDDATA){ data[16] = INVALIDDATA; data[17] = INVALIDDATA; } 
	else { data[16] = data_tmp[8] & 0x0FFFFFFF; data[17] = (data_tmp[8] >> 28) & 0x0000000F; }
	if(data_tmp[9] == INVALIDDATA){ data[18] = INVALIDDATA; data[19] = INVALIDDATA; } 
	else { data[18] = data_tmp[9] & 0x0FFFFFFF; data[19] = (data_tmp[9] >> 28) & 0x0000000F; }
	if(data_tmp[10] == INVALIDDATA){ data[20] = INVALIDDATA; data[21] = INVALIDDATA; } 
	else { data[20] = data_tmp[10] & 0x0FFFFFFF; data[21] = (data_tmp[10] >> 28) & 0x0000000F; }
	if(data_tmp[11] == INVALIDDATA){ data[22] = INVALIDDATA; data[23] = INVALIDDATA; } 
	else { data[22] = data_tmp[11] & 0x0FFFFFFF; data[23] = (data_tmp[11] >> 28) & 0x0000000F; }
	if(data_tmp[12] == INVALIDDATA){ data[24] = INVALIDDATA; data[25] = INVALIDDATA; } 
	else { data[24] = data_tmp[12] & 0x0FFFFFFF; data[25] = (data_tmp[12] >> 28) & 0x0000000F; }
	if(data_tmp[13] == INVALIDDATA){ data[26] = INVALIDDATA; data[27] = INVALIDDATA; } 
	else { data[26] = data_tmp[13] & 0x0FFFFFFF; data[27] = (data_tmp[13] >> 28) & 0x0000000F; }
	if(data_tmp[14] == INVALIDDATA){ data[28] = INVALIDDATA; data[29] = INVALIDDATA; } 
	else { data[28] = data_tmp[14] & 0x0FFFFFFF; data[29] = (data_tmp[14] >> 28) & 0x0000000F; }
	if(data_tmp[15] == INVALIDDATA){ data[30] = INVALIDDATA; data[31] = INVALIDDATA; } 
	else { data[30] = data_tmp[15] & 0x0FFFFFFF; data[31] = (data_tmp[15] >> 28) & 0x0000000F; }
	return;
}

void master_insert_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef FPGA_IMPL 
		HBM_channelA[offset].range(31, 0) = data[0];
		HBM_channelA[offset].range(63, 32) = data[1];
		HBM_channelA[offset].range(95, 64) = data[2];
		HBM_channelA[offset].range(127, 96) = data[3];
		HBM_channelA[offset].range(159, 128) = data[4];
		HBM_channelA[offset].range(191, 160) = data[5];
		HBM_channelA[offset].range(223, 192) = data[6];
		HBM_channelA[offset].range(255, 224) = data[7];
		HBM_channelA[offset].range(287, 256) = data[8];
		HBM_channelA[offset].range(319, 288) = data[9];
		HBM_channelA[offset].range(351, 320) = data[10];
		HBM_channelA[offset].range(383, 352) = data[11];
		HBM_channelA[offset].range(415, 384) = data[12];
		HBM_channelA[offset].range(447, 416) = data[13];
		HBM_channelA[offset].range(479, 448) = data[14];
		HBM_channelA[offset].range(511, 480) = data[15];
		HBM_channelB[offset].range(31, 0) = data[16];
		HBM_channelB[offset].range(63, 32) = data[17];
		HBM_channelB[offset].range(95, 64) = data[18];
		HBM_channelB[offset].range(127, 96) = data[19];
		HBM_channelB[offset].range(159, 128) = data[20];
		HBM_channelB[offset].range(191, 160) = data[21];
		HBM_channelB[offset].range(223, 192) = data[22];
		HBM_channelB[offset].range(255, 224) = data[23];
		HBM_channelB[offset].range(287, 256) = data[24];
		HBM_channelB[offset].range(319, 288) = data[25];
		HBM_channelB[offset].range(351, 320) = data[26];
		HBM_channelB[offset].range(383, 352) = data[27];
		HBM_channelB[offset].range(415, 384) = data[28];
		HBM_channelB[offset].range(447, 416) = data[29];
		HBM_channelB[offset].range(479, 448) = data[30];
		HBM_channelB[offset].range(511, 480) = data[31];
	#else 
		#ifdef ___USE_AXI_CHANNEL___
		HBM_channelA[offset].data[0] = data[0];
		HBM_channelA[offset].data[1] = data[1];
		HBM_channelA[offset].data[2] = data[2];
		HBM_channelA[offset].data[3] = data[3];
		HBM_channelA[offset].data[4] = data[4];
		HBM_channelA[offset].data[5] = data[5];
		HBM_channelA[offset].data[6] = data[6];
		HBM_channelA[offset].data[7] = data[7];
		HBM_channelA[offset].data[8] = data[8];
		HBM_channelA[offset].data[9] = data[9];
		HBM_channelA[offset].data[10] = data[10];
		HBM_channelA[offset].data[11] = data[11];
		HBM_channelA[offset].data[12] = data[12];
		HBM_channelA[offset].data[13] = data[13];
		HBM_channelA[offset].data[14] = data[14];
		HBM_channelA[offset].data[15] = data[15];
		HBM_channelB[offset].data[0] = data[16];
		HBM_channelB[offset].data[1] = data[17];
		HBM_channelB[offset].data[2] = data[18];
		HBM_channelB[offset].data[3] = data[19];
		HBM_channelB[offset].data[4] = data[20];
		HBM_channelB[offset].data[5] = data[21];
		HBM_channelB[offset].data[6] = data[22];
		HBM_channelB[offset].data[7] = data[23];
		HBM_channelB[offset].data[8] = data[24];
		HBM_channelB[offset].data[9] = data[25];
		HBM_channelB[offset].data[10] = data[26];
		HBM_channelB[offset].data[11] = data[27];
		HBM_channelB[offset].data[12] = data[28];
		HBM_channelB[offset].data[13] = data[29];
		HBM_channelB[offset].data[14] = data[30];
		HBM_channelB[offset].data[15] = data[31];
		#else
		HBM_channelA[offset].data[0] = data[0];
		HBM_channelA[offset].data[1] = data[1];
		HBM_channelA[offset].data[2] = data[2];
		HBM_channelA[offset].data[3] = data[3];
		HBM_channelA[offset].data[4] = data[4];
		HBM_channelA[offset].data[5] = data[5];
		HBM_channelA[offset].data[6] = data[6];
		HBM_channelA[offset].data[7] = data[7];
		HBM_channelA[offset].data[8] = data[8];
		HBM_channelA[offset].data[9] = data[9];
		HBM_channelA[offset].data[10] = data[10];
		HBM_channelA[offset].data[11] = data[11];
		HBM_channelA[offset].data[12] = data[12];
		HBM_channelA[offset].data[13] = data[13];
		HBM_channelA[offset].data[14] = data[14];
		HBM_channelA[offset].data[15] = data[15];
		HBM_channelA[offset].data[16] = data[16];
		HBM_channelA[offset].data[17] = data[17];
		HBM_channelA[offset].data[18] = data[18];
		HBM_channelA[offset].data[19] = data[19];
		HBM_channelA[offset].data[20] = data[20];
		HBM_channelA[offset].data[21] = data[21];
		HBM_channelA[offset].data[22] = data[22];
		HBM_channelA[offset].data[23] = data[23];
		HBM_channelA[offset].data[24] = data[24];
		HBM_channelA[offset].data[25] = data[25];
		HBM_channelA[offset].data[26] = data[26];
		HBM_channelA[offset].data[27] = data[27];
		HBM_channelA[offset].data[28] = data[28];
		HBM_channelA[offset].data[29] = data[29];
		HBM_channelA[offset].data[30] = data[30];
		HBM_channelA[offset].data[31] = data[31];
		#endif 
	#endif
	return;
}
void master_retrieve_vec(unsigned int offset, unsigned int data[HBM_CHANNEL_PACK_SIZE], HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	#ifdef FPGA_IMPL 
		data[0] = HBM_channelA[offset].range(31, 0);
		data[1] = HBM_channelA[offset].range(63, 32);
		data[2] = HBM_channelA[offset].range(95, 64);
		data[3] = HBM_channelA[offset].range(127, 96);
		data[4] = HBM_channelA[offset].range(159, 128);
		data[5] = HBM_channelA[offset].range(191, 160);
		data[6] = HBM_channelA[offset].range(223, 192);
		data[7] = HBM_channelA[offset].range(255, 224);
		data[8] = HBM_channelA[offset].range(287, 256);
		data[9] = HBM_channelA[offset].range(319, 288);
		data[10] = HBM_channelA[offset].range(351, 320);
		data[11] = HBM_channelA[offset].range(383, 352);
		data[12] = HBM_channelA[offset].range(415, 384);
		data[13] = HBM_channelA[offset].range(447, 416);
		data[14] = HBM_channelA[offset].range(479, 448);
		data[15] = HBM_channelA[offset].range(511, 480);
		data[16] = HBM_channelB[offset].range(31, 0);
		data[17] = HBM_channelB[offset].range(63, 32);
		data[18] = HBM_channelB[offset].range(95, 64);
		data[19] = HBM_channelB[offset].range(127, 96);
		data[20] = HBM_channelB[offset].range(159, 128);
		data[21] = HBM_channelB[offset].range(191, 160);
		data[22] = HBM_channelB[offset].range(223, 192);
		data[23] = HBM_channelB[offset].range(255, 224);
		data[24] = HBM_channelB[offset].range(287, 256);
		data[25] = HBM_channelB[offset].range(319, 288);
		data[26] = HBM_channelB[offset].range(351, 320);
		data[27] = HBM_channelB[offset].range(383, 352);
		data[28] = HBM_channelB[offset].range(415, 384);
		data[29] = HBM_channelB[offset].range(447, 416);
		data[30] = HBM_channelB[offset].range(479, 448);
		data[31] = HBM_channelB[offset].range(511, 480);
	#else 
		#ifdef ___USE_AXI_CHANNEL___
		data[0] = HBM_channelA[offset].data[0];
		data[1] = HBM_channelA[offset].data[1];
		data[2] = HBM_channelA[offset].data[2];
		data[3] = HBM_channelA[offset].data[3];
		data[4] = HBM_channelA[offset].data[4];
		data[5] = HBM_channelA[offset].data[5];
		data[6] = HBM_channelA[offset].data[6];
		data[7] = HBM_channelA[offset].data[7];
		data[8] = HBM_channelA[offset].data[8];
		data[9] = HBM_channelA[offset].data[9];
		data[10] = HBM_channelA[offset].data[10];
		data[11] = HBM_channelA[offset].data[11];
		data[12] = HBM_channelA[offset].data[12];
		data[13] = HBM_channelA[offset].data[13];
		data[14] = HBM_channelA[offset].data[14];
		data[15] = HBM_channelA[offset].data[15];
		data[16] = HBM_channelB[offset].data[0];
		data[17] = HBM_channelB[offset].data[1];
		data[18] = HBM_channelB[offset].data[2];
		data[19] = HBM_channelB[offset].data[3];
		data[20] = HBM_channelB[offset].data[4];
		data[21] = HBM_channelB[offset].data[5];
		data[22] = HBM_channelB[offset].data[6];
		data[23] = HBM_channelB[offset].data[7];
		data[24] = HBM_channelB[offset].data[8];
		data[25] = HBM_channelB[offset].data[9];
		data[26] = HBM_channelB[offset].data[10];
		data[27] = HBM_channelB[offset].data[11];
		data[28] = HBM_channelB[offset].data[12];
		data[29] = HBM_channelB[offset].data[13];
		data[30] = HBM_channelB[offset].data[14];
		data[31] = HBM_channelB[offset].data[15];
		#else
		data[0] = HBM_channelA[offset].data[0];
		data[1] = HBM_channelA[offset].data[1];
		data[2] = HBM_channelA[offset].data[2];
		data[3] = HBM_channelA[offset].data[3];
		data[4] = HBM_channelA[offset].data[4];
		data[5] = HBM_channelA[offset].data[5];
		data[6] = HBM_channelA[offset].data[6];
		data[7] = HBM_channelA[offset].data[7];
		data[8] = HBM_channelA[offset].data[8];
		data[9] = HBM_channelA[offset].data[9];
		data[10] = HBM_channelA[offset].data[10];
		data[11] = HBM_channelA[offset].data[11];
		data[12] = HBM_channelA[offset].data[12];
		data[13] = HBM_channelA[offset].data[13];
		data[14] = HBM_channelA[offset].data[14];
		data[15] = HBM_channelA[offset].data[15];
		data[16] = HBM_channelA[offset].data[16];
		data[17] = HBM_channelA[offset].data[17];
		data[18] = HBM_channelA[offset].data[18];
		data[19] = HBM_channelA[offset].data[19];
		data[20] = HBM_channelA[offset].data[20];
		data[21] = HBM_channelA[offset].data[21];
		data[22] = HBM_channelA[offset].data[22];
		data[23] = HBM_channelA[offset].data[23];
		data[24] = HBM_channelA[offset].data[24];
		data[25] = HBM_channelA[offset].data[25];
		data[26] = HBM_channelA[offset].data[26];
		data[27] = HBM_channelA[offset].data[27];
		data[28] = HBM_channelA[offset].data[28];
		data[29] = HBM_channelA[offset].data[29];
		data[30] = HBM_channelA[offset].data[30];
		data[31] = HBM_channelA[offset].data[31];
		#endif
	#endif 
	return;
}
void master_insertmany_vec(unsigned int offsets[NUM_PEs], unsigned int ens[NUM_PEs], unsigned int data[NUM_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
	#ifdef FPGA_IMPL 
		if(ens[0]==true){
			HBM_channelA0[offsets[0]].range(31, 0) = data[0][0];
			HBM_channelA0[offsets[0]].range(63, 32) = data[0][1];
			HBM_channelA0[offsets[0]].range(95, 64) = data[0][2];
			HBM_channelA0[offsets[0]].range(127, 96) = data[0][3];
			HBM_channelA0[offsets[0]].range(159, 128) = data[0][4];
			HBM_channelA0[offsets[0]].range(191, 160) = data[0][5];
			HBM_channelA0[offsets[0]].range(223, 192) = data[0][6];
			HBM_channelA0[offsets[0]].range(255, 224) = data[0][7];
			HBM_channelA0[offsets[0]].range(287, 256) = data[0][8];
			HBM_channelA0[offsets[0]].range(319, 288) = data[0][9];
			HBM_channelA0[offsets[0]].range(351, 320) = data[0][10];
			HBM_channelA0[offsets[0]].range(383, 352) = data[0][11];
			HBM_channelA0[offsets[0]].range(415, 384) = data[0][12];
			HBM_channelA0[offsets[0]].range(447, 416) = data[0][13];
			HBM_channelA0[offsets[0]].range(479, 448) = data[0][14];
			HBM_channelA0[offsets[0]].range(511, 480) = data[0][15];
			HBM_channelB0[offsets[0]].range(31, 0) = data[0][16];
			HBM_channelB0[offsets[0]].range(63, 32) = data[0][17];
			HBM_channelB0[offsets[0]].range(95, 64) = data[0][18];
			HBM_channelB0[offsets[0]].range(127, 96) = data[0][19];
			HBM_channelB0[offsets[0]].range(159, 128) = data[0][20];
			HBM_channelB0[offsets[0]].range(191, 160) = data[0][21];
			HBM_channelB0[offsets[0]].range(223, 192) = data[0][22];
			HBM_channelB0[offsets[0]].range(255, 224) = data[0][23];
			HBM_channelB0[offsets[0]].range(287, 256) = data[0][24];
			HBM_channelB0[offsets[0]].range(319, 288) = data[0][25];
			HBM_channelB0[offsets[0]].range(351, 320) = data[0][26];
			HBM_channelB0[offsets[0]].range(383, 352) = data[0][27];
			HBM_channelB0[offsets[0]].range(415, 384) = data[0][28];
			HBM_channelB0[offsets[0]].range(447, 416) = data[0][29];
			HBM_channelB0[offsets[0]].range(479, 448) = data[0][30];
			HBM_channelB0[offsets[0]].range(511, 480) = data[0][31];
		}
		if(ens[1]==true){
			HBM_channelA1[offsets[1]].range(31, 0) = data[1][0];
			HBM_channelA1[offsets[1]].range(63, 32) = data[1][1];
			HBM_channelA1[offsets[1]].range(95, 64) = data[1][2];
			HBM_channelA1[offsets[1]].range(127, 96) = data[1][3];
			HBM_channelA1[offsets[1]].range(159, 128) = data[1][4];
			HBM_channelA1[offsets[1]].range(191, 160) = data[1][5];
			HBM_channelA1[offsets[1]].range(223, 192) = data[1][6];
			HBM_channelA1[offsets[1]].range(255, 224) = data[1][7];
			HBM_channelA1[offsets[1]].range(287, 256) = data[1][8];
			HBM_channelA1[offsets[1]].range(319, 288) = data[1][9];
			HBM_channelA1[offsets[1]].range(351, 320) = data[1][10];
			HBM_channelA1[offsets[1]].range(383, 352) = data[1][11];
			HBM_channelA1[offsets[1]].range(415, 384) = data[1][12];
			HBM_channelA1[offsets[1]].range(447, 416) = data[1][13];
			HBM_channelA1[offsets[1]].range(479, 448) = data[1][14];
			HBM_channelA1[offsets[1]].range(511, 480) = data[1][15];
			HBM_channelB1[offsets[1]].range(31, 0) = data[1][16];
			HBM_channelB1[offsets[1]].range(63, 32) = data[1][17];
			HBM_channelB1[offsets[1]].range(95, 64) = data[1][18];
			HBM_channelB1[offsets[1]].range(127, 96) = data[1][19];
			HBM_channelB1[offsets[1]].range(159, 128) = data[1][20];
			HBM_channelB1[offsets[1]].range(191, 160) = data[1][21];
			HBM_channelB1[offsets[1]].range(223, 192) = data[1][22];
			HBM_channelB1[offsets[1]].range(255, 224) = data[1][23];
			HBM_channelB1[offsets[1]].range(287, 256) = data[1][24];
			HBM_channelB1[offsets[1]].range(319, 288) = data[1][25];
			HBM_channelB1[offsets[1]].range(351, 320) = data[1][26];
			HBM_channelB1[offsets[1]].range(383, 352) = data[1][27];
			HBM_channelB1[offsets[1]].range(415, 384) = data[1][28];
			HBM_channelB1[offsets[1]].range(447, 416) = data[1][29];
			HBM_channelB1[offsets[1]].range(479, 448) = data[1][30];
			HBM_channelB1[offsets[1]].range(511, 480) = data[1][31];
		}
		if(ens[2]==true){
			HBM_channelA2[offsets[2]].range(31, 0) = data[2][0];
			HBM_channelA2[offsets[2]].range(63, 32) = data[2][1];
			HBM_channelA2[offsets[2]].range(95, 64) = data[2][2];
			HBM_channelA2[offsets[2]].range(127, 96) = data[2][3];
			HBM_channelA2[offsets[2]].range(159, 128) = data[2][4];
			HBM_channelA2[offsets[2]].range(191, 160) = data[2][5];
			HBM_channelA2[offsets[2]].range(223, 192) = data[2][6];
			HBM_channelA2[offsets[2]].range(255, 224) = data[2][7];
			HBM_channelA2[offsets[2]].range(287, 256) = data[2][8];
			HBM_channelA2[offsets[2]].range(319, 288) = data[2][9];
			HBM_channelA2[offsets[2]].range(351, 320) = data[2][10];
			HBM_channelA2[offsets[2]].range(383, 352) = data[2][11];
			HBM_channelA2[offsets[2]].range(415, 384) = data[2][12];
			HBM_channelA2[offsets[2]].range(447, 416) = data[2][13];
			HBM_channelA2[offsets[2]].range(479, 448) = data[2][14];
			HBM_channelA2[offsets[2]].range(511, 480) = data[2][15];
			HBM_channelB2[offsets[2]].range(31, 0) = data[2][16];
			HBM_channelB2[offsets[2]].range(63, 32) = data[2][17];
			HBM_channelB2[offsets[2]].range(95, 64) = data[2][18];
			HBM_channelB2[offsets[2]].range(127, 96) = data[2][19];
			HBM_channelB2[offsets[2]].range(159, 128) = data[2][20];
			HBM_channelB2[offsets[2]].range(191, 160) = data[2][21];
			HBM_channelB2[offsets[2]].range(223, 192) = data[2][22];
			HBM_channelB2[offsets[2]].range(255, 224) = data[2][23];
			HBM_channelB2[offsets[2]].range(287, 256) = data[2][24];
			HBM_channelB2[offsets[2]].range(319, 288) = data[2][25];
			HBM_channelB2[offsets[2]].range(351, 320) = data[2][26];
			HBM_channelB2[offsets[2]].range(383, 352) = data[2][27];
			HBM_channelB2[offsets[2]].range(415, 384) = data[2][28];
			HBM_channelB2[offsets[2]].range(447, 416) = data[2][29];
			HBM_channelB2[offsets[2]].range(479, 448) = data[2][30];
			HBM_channelB2[offsets[2]].range(511, 480) = data[2][31];
		}
		if(ens[3]==true){
			HBM_channelA3[offsets[3]].range(31, 0) = data[3][0];
			HBM_channelA3[offsets[3]].range(63, 32) = data[3][1];
			HBM_channelA3[offsets[3]].range(95, 64) = data[3][2];
			HBM_channelA3[offsets[3]].range(127, 96) = data[3][3];
			HBM_channelA3[offsets[3]].range(159, 128) = data[3][4];
			HBM_channelA3[offsets[3]].range(191, 160) = data[3][5];
			HBM_channelA3[offsets[3]].range(223, 192) = data[3][6];
			HBM_channelA3[offsets[3]].range(255, 224) = data[3][7];
			HBM_channelA3[offsets[3]].range(287, 256) = data[3][8];
			HBM_channelA3[offsets[3]].range(319, 288) = data[3][9];
			HBM_channelA3[offsets[3]].range(351, 320) = data[3][10];
			HBM_channelA3[offsets[3]].range(383, 352) = data[3][11];
			HBM_channelA3[offsets[3]].range(415, 384) = data[3][12];
			HBM_channelA3[offsets[3]].range(447, 416) = data[3][13];
			HBM_channelA3[offsets[3]].range(479, 448) = data[3][14];
			HBM_channelA3[offsets[3]].range(511, 480) = data[3][15];
			HBM_channelB3[offsets[3]].range(31, 0) = data[3][16];
			HBM_channelB3[offsets[3]].range(63, 32) = data[3][17];
			HBM_channelB3[offsets[3]].range(95, 64) = data[3][18];
			HBM_channelB3[offsets[3]].range(127, 96) = data[3][19];
			HBM_channelB3[offsets[3]].range(159, 128) = data[3][20];
			HBM_channelB3[offsets[3]].range(191, 160) = data[3][21];
			HBM_channelB3[offsets[3]].range(223, 192) = data[3][22];
			HBM_channelB3[offsets[3]].range(255, 224) = data[3][23];
			HBM_channelB3[offsets[3]].range(287, 256) = data[3][24];
			HBM_channelB3[offsets[3]].range(319, 288) = data[3][25];
			HBM_channelB3[offsets[3]].range(351, 320) = data[3][26];
			HBM_channelB3[offsets[3]].range(383, 352) = data[3][27];
			HBM_channelB3[offsets[3]].range(415, 384) = data[3][28];
			HBM_channelB3[offsets[3]].range(447, 416) = data[3][29];
			HBM_channelB3[offsets[3]].range(479, 448) = data[3][30];
			HBM_channelB3[offsets[3]].range(511, 480) = data[3][31];
		}
		if(ens[4]==true){
			HBM_channelA4[offsets[4]].range(31, 0) = data[4][0];
			HBM_channelA4[offsets[4]].range(63, 32) = data[4][1];
			HBM_channelA4[offsets[4]].range(95, 64) = data[4][2];
			HBM_channelA4[offsets[4]].range(127, 96) = data[4][3];
			HBM_channelA4[offsets[4]].range(159, 128) = data[4][4];
			HBM_channelA4[offsets[4]].range(191, 160) = data[4][5];
			HBM_channelA4[offsets[4]].range(223, 192) = data[4][6];
			HBM_channelA4[offsets[4]].range(255, 224) = data[4][7];
			HBM_channelA4[offsets[4]].range(287, 256) = data[4][8];
			HBM_channelA4[offsets[4]].range(319, 288) = data[4][9];
			HBM_channelA4[offsets[4]].range(351, 320) = data[4][10];
			HBM_channelA4[offsets[4]].range(383, 352) = data[4][11];
			HBM_channelA4[offsets[4]].range(415, 384) = data[4][12];
			HBM_channelA4[offsets[4]].range(447, 416) = data[4][13];
			HBM_channelA4[offsets[4]].range(479, 448) = data[4][14];
			HBM_channelA4[offsets[4]].range(511, 480) = data[4][15];
			HBM_channelB4[offsets[4]].range(31, 0) = data[4][16];
			HBM_channelB4[offsets[4]].range(63, 32) = data[4][17];
			HBM_channelB4[offsets[4]].range(95, 64) = data[4][18];
			HBM_channelB4[offsets[4]].range(127, 96) = data[4][19];
			HBM_channelB4[offsets[4]].range(159, 128) = data[4][20];
			HBM_channelB4[offsets[4]].range(191, 160) = data[4][21];
			HBM_channelB4[offsets[4]].range(223, 192) = data[4][22];
			HBM_channelB4[offsets[4]].range(255, 224) = data[4][23];
			HBM_channelB4[offsets[4]].range(287, 256) = data[4][24];
			HBM_channelB4[offsets[4]].range(319, 288) = data[4][25];
			HBM_channelB4[offsets[4]].range(351, 320) = data[4][26];
			HBM_channelB4[offsets[4]].range(383, 352) = data[4][27];
			HBM_channelB4[offsets[4]].range(415, 384) = data[4][28];
			HBM_channelB4[offsets[4]].range(447, 416) = data[4][29];
			HBM_channelB4[offsets[4]].range(479, 448) = data[4][30];
			HBM_channelB4[offsets[4]].range(511, 480) = data[4][31];
		}
		if(ens[5]==true){
			HBM_channelA5[offsets[5]].range(31, 0) = data[5][0];
			HBM_channelA5[offsets[5]].range(63, 32) = data[5][1];
			HBM_channelA5[offsets[5]].range(95, 64) = data[5][2];
			HBM_channelA5[offsets[5]].range(127, 96) = data[5][3];
			HBM_channelA5[offsets[5]].range(159, 128) = data[5][4];
			HBM_channelA5[offsets[5]].range(191, 160) = data[5][5];
			HBM_channelA5[offsets[5]].range(223, 192) = data[5][6];
			HBM_channelA5[offsets[5]].range(255, 224) = data[5][7];
			HBM_channelA5[offsets[5]].range(287, 256) = data[5][8];
			HBM_channelA5[offsets[5]].range(319, 288) = data[5][9];
			HBM_channelA5[offsets[5]].range(351, 320) = data[5][10];
			HBM_channelA5[offsets[5]].range(383, 352) = data[5][11];
			HBM_channelA5[offsets[5]].range(415, 384) = data[5][12];
			HBM_channelA5[offsets[5]].range(447, 416) = data[5][13];
			HBM_channelA5[offsets[5]].range(479, 448) = data[5][14];
			HBM_channelA5[offsets[5]].range(511, 480) = data[5][15];
			HBM_channelB5[offsets[5]].range(31, 0) = data[5][16];
			HBM_channelB5[offsets[5]].range(63, 32) = data[5][17];
			HBM_channelB5[offsets[5]].range(95, 64) = data[5][18];
			HBM_channelB5[offsets[5]].range(127, 96) = data[5][19];
			HBM_channelB5[offsets[5]].range(159, 128) = data[5][20];
			HBM_channelB5[offsets[5]].range(191, 160) = data[5][21];
			HBM_channelB5[offsets[5]].range(223, 192) = data[5][22];
			HBM_channelB5[offsets[5]].range(255, 224) = data[5][23];
			HBM_channelB5[offsets[5]].range(287, 256) = data[5][24];
			HBM_channelB5[offsets[5]].range(319, 288) = data[5][25];
			HBM_channelB5[offsets[5]].range(351, 320) = data[5][26];
			HBM_channelB5[offsets[5]].range(383, 352) = data[5][27];
			HBM_channelB5[offsets[5]].range(415, 384) = data[5][28];
			HBM_channelB5[offsets[5]].range(447, 416) = data[5][29];
			HBM_channelB5[offsets[5]].range(479, 448) = data[5][30];
			HBM_channelB5[offsets[5]].range(511, 480) = data[5][31];
		}
		if(ens[6]==true){
			HBM_channelA6[offsets[6]].range(31, 0) = data[6][0];
			HBM_channelA6[offsets[6]].range(63, 32) = data[6][1];
			HBM_channelA6[offsets[6]].range(95, 64) = data[6][2];
			HBM_channelA6[offsets[6]].range(127, 96) = data[6][3];
			HBM_channelA6[offsets[6]].range(159, 128) = data[6][4];
			HBM_channelA6[offsets[6]].range(191, 160) = data[6][5];
			HBM_channelA6[offsets[6]].range(223, 192) = data[6][6];
			HBM_channelA6[offsets[6]].range(255, 224) = data[6][7];
			HBM_channelA6[offsets[6]].range(287, 256) = data[6][8];
			HBM_channelA6[offsets[6]].range(319, 288) = data[6][9];
			HBM_channelA6[offsets[6]].range(351, 320) = data[6][10];
			HBM_channelA6[offsets[6]].range(383, 352) = data[6][11];
			HBM_channelA6[offsets[6]].range(415, 384) = data[6][12];
			HBM_channelA6[offsets[6]].range(447, 416) = data[6][13];
			HBM_channelA6[offsets[6]].range(479, 448) = data[6][14];
			HBM_channelA6[offsets[6]].range(511, 480) = data[6][15];
			HBM_channelB6[offsets[6]].range(31, 0) = data[6][16];
			HBM_channelB6[offsets[6]].range(63, 32) = data[6][17];
			HBM_channelB6[offsets[6]].range(95, 64) = data[6][18];
			HBM_channelB6[offsets[6]].range(127, 96) = data[6][19];
			HBM_channelB6[offsets[6]].range(159, 128) = data[6][20];
			HBM_channelB6[offsets[6]].range(191, 160) = data[6][21];
			HBM_channelB6[offsets[6]].range(223, 192) = data[6][22];
			HBM_channelB6[offsets[6]].range(255, 224) = data[6][23];
			HBM_channelB6[offsets[6]].range(287, 256) = data[6][24];
			HBM_channelB6[offsets[6]].range(319, 288) = data[6][25];
			HBM_channelB6[offsets[6]].range(351, 320) = data[6][26];
			HBM_channelB6[offsets[6]].range(383, 352) = data[6][27];
			HBM_channelB6[offsets[6]].range(415, 384) = data[6][28];
			HBM_channelB6[offsets[6]].range(447, 416) = data[6][29];
			HBM_channelB6[offsets[6]].range(479, 448) = data[6][30];
			HBM_channelB6[offsets[6]].range(511, 480) = data[6][31];
		}
		if(ens[7]==true){
			HBM_channelA7[offsets[7]].range(31, 0) = data[7][0];
			HBM_channelA7[offsets[7]].range(63, 32) = data[7][1];
			HBM_channelA7[offsets[7]].range(95, 64) = data[7][2];
			HBM_channelA7[offsets[7]].range(127, 96) = data[7][3];
			HBM_channelA7[offsets[7]].range(159, 128) = data[7][4];
			HBM_channelA7[offsets[7]].range(191, 160) = data[7][5];
			HBM_channelA7[offsets[7]].range(223, 192) = data[7][6];
			HBM_channelA7[offsets[7]].range(255, 224) = data[7][7];
			HBM_channelA7[offsets[7]].range(287, 256) = data[7][8];
			HBM_channelA7[offsets[7]].range(319, 288) = data[7][9];
			HBM_channelA7[offsets[7]].range(351, 320) = data[7][10];
			HBM_channelA7[offsets[7]].range(383, 352) = data[7][11];
			HBM_channelA7[offsets[7]].range(415, 384) = data[7][12];
			HBM_channelA7[offsets[7]].range(447, 416) = data[7][13];
			HBM_channelA7[offsets[7]].range(479, 448) = data[7][14];
			HBM_channelA7[offsets[7]].range(511, 480) = data[7][15];
			HBM_channelB7[offsets[7]].range(31, 0) = data[7][16];
			HBM_channelB7[offsets[7]].range(63, 32) = data[7][17];
			HBM_channelB7[offsets[7]].range(95, 64) = data[7][18];
			HBM_channelB7[offsets[7]].range(127, 96) = data[7][19];
			HBM_channelB7[offsets[7]].range(159, 128) = data[7][20];
			HBM_channelB7[offsets[7]].range(191, 160) = data[7][21];
			HBM_channelB7[offsets[7]].range(223, 192) = data[7][22];
			HBM_channelB7[offsets[7]].range(255, 224) = data[7][23];
			HBM_channelB7[offsets[7]].range(287, 256) = data[7][24];
			HBM_channelB7[offsets[7]].range(319, 288) = data[7][25];
			HBM_channelB7[offsets[7]].range(351, 320) = data[7][26];
			HBM_channelB7[offsets[7]].range(383, 352) = data[7][27];
			HBM_channelB7[offsets[7]].range(415, 384) = data[7][28];
			HBM_channelB7[offsets[7]].range(447, 416) = data[7][29];
			HBM_channelB7[offsets[7]].range(479, 448) = data[7][30];
			HBM_channelB7[offsets[7]].range(511, 480) = data[7][31];
		}
		if(ens[8]==true){
			HBM_channelA8[offsets[8]].range(31, 0) = data[8][0];
			HBM_channelA8[offsets[8]].range(63, 32) = data[8][1];
			HBM_channelA8[offsets[8]].range(95, 64) = data[8][2];
			HBM_channelA8[offsets[8]].range(127, 96) = data[8][3];
			HBM_channelA8[offsets[8]].range(159, 128) = data[8][4];
			HBM_channelA8[offsets[8]].range(191, 160) = data[8][5];
			HBM_channelA8[offsets[8]].range(223, 192) = data[8][6];
			HBM_channelA8[offsets[8]].range(255, 224) = data[8][7];
			HBM_channelA8[offsets[8]].range(287, 256) = data[8][8];
			HBM_channelA8[offsets[8]].range(319, 288) = data[8][9];
			HBM_channelA8[offsets[8]].range(351, 320) = data[8][10];
			HBM_channelA8[offsets[8]].range(383, 352) = data[8][11];
			HBM_channelA8[offsets[8]].range(415, 384) = data[8][12];
			HBM_channelA8[offsets[8]].range(447, 416) = data[8][13];
			HBM_channelA8[offsets[8]].range(479, 448) = data[8][14];
			HBM_channelA8[offsets[8]].range(511, 480) = data[8][15];
			HBM_channelB8[offsets[8]].range(31, 0) = data[8][16];
			HBM_channelB8[offsets[8]].range(63, 32) = data[8][17];
			HBM_channelB8[offsets[8]].range(95, 64) = data[8][18];
			HBM_channelB8[offsets[8]].range(127, 96) = data[8][19];
			HBM_channelB8[offsets[8]].range(159, 128) = data[8][20];
			HBM_channelB8[offsets[8]].range(191, 160) = data[8][21];
			HBM_channelB8[offsets[8]].range(223, 192) = data[8][22];
			HBM_channelB8[offsets[8]].range(255, 224) = data[8][23];
			HBM_channelB8[offsets[8]].range(287, 256) = data[8][24];
			HBM_channelB8[offsets[8]].range(319, 288) = data[8][25];
			HBM_channelB8[offsets[8]].range(351, 320) = data[8][26];
			HBM_channelB8[offsets[8]].range(383, 352) = data[8][27];
			HBM_channelB8[offsets[8]].range(415, 384) = data[8][28];
			HBM_channelB8[offsets[8]].range(447, 416) = data[8][29];
			HBM_channelB8[offsets[8]].range(479, 448) = data[8][30];
			HBM_channelB8[offsets[8]].range(511, 480) = data[8][31];
		}
		if(ens[9]==true){
			HBM_channelA9[offsets[9]].range(31, 0) = data[9][0];
			HBM_channelA9[offsets[9]].range(63, 32) = data[9][1];
			HBM_channelA9[offsets[9]].range(95, 64) = data[9][2];
			HBM_channelA9[offsets[9]].range(127, 96) = data[9][3];
			HBM_channelA9[offsets[9]].range(159, 128) = data[9][4];
			HBM_channelA9[offsets[9]].range(191, 160) = data[9][5];
			HBM_channelA9[offsets[9]].range(223, 192) = data[9][6];
			HBM_channelA9[offsets[9]].range(255, 224) = data[9][7];
			HBM_channelA9[offsets[9]].range(287, 256) = data[9][8];
			HBM_channelA9[offsets[9]].range(319, 288) = data[9][9];
			HBM_channelA9[offsets[9]].range(351, 320) = data[9][10];
			HBM_channelA9[offsets[9]].range(383, 352) = data[9][11];
			HBM_channelA9[offsets[9]].range(415, 384) = data[9][12];
			HBM_channelA9[offsets[9]].range(447, 416) = data[9][13];
			HBM_channelA9[offsets[9]].range(479, 448) = data[9][14];
			HBM_channelA9[offsets[9]].range(511, 480) = data[9][15];
			HBM_channelB9[offsets[9]].range(31, 0) = data[9][16];
			HBM_channelB9[offsets[9]].range(63, 32) = data[9][17];
			HBM_channelB9[offsets[9]].range(95, 64) = data[9][18];
			HBM_channelB9[offsets[9]].range(127, 96) = data[9][19];
			HBM_channelB9[offsets[9]].range(159, 128) = data[9][20];
			HBM_channelB9[offsets[9]].range(191, 160) = data[9][21];
			HBM_channelB9[offsets[9]].range(223, 192) = data[9][22];
			HBM_channelB9[offsets[9]].range(255, 224) = data[9][23];
			HBM_channelB9[offsets[9]].range(287, 256) = data[9][24];
			HBM_channelB9[offsets[9]].range(319, 288) = data[9][25];
			HBM_channelB9[offsets[9]].range(351, 320) = data[9][26];
			HBM_channelB9[offsets[9]].range(383, 352) = data[9][27];
			HBM_channelB9[offsets[9]].range(415, 384) = data[9][28];
			HBM_channelB9[offsets[9]].range(447, 416) = data[9][29];
			HBM_channelB9[offsets[9]].range(479, 448) = data[9][30];
			HBM_channelB9[offsets[9]].range(511, 480) = data[9][31];
		}
		if(ens[10]==true){
			HBM_channelA10[offsets[10]].range(31, 0) = data[10][0];
			HBM_channelA10[offsets[10]].range(63, 32) = data[10][1];
			HBM_channelA10[offsets[10]].range(95, 64) = data[10][2];
			HBM_channelA10[offsets[10]].range(127, 96) = data[10][3];
			HBM_channelA10[offsets[10]].range(159, 128) = data[10][4];
			HBM_channelA10[offsets[10]].range(191, 160) = data[10][5];
			HBM_channelA10[offsets[10]].range(223, 192) = data[10][6];
			HBM_channelA10[offsets[10]].range(255, 224) = data[10][7];
			HBM_channelA10[offsets[10]].range(287, 256) = data[10][8];
			HBM_channelA10[offsets[10]].range(319, 288) = data[10][9];
			HBM_channelA10[offsets[10]].range(351, 320) = data[10][10];
			HBM_channelA10[offsets[10]].range(383, 352) = data[10][11];
			HBM_channelA10[offsets[10]].range(415, 384) = data[10][12];
			HBM_channelA10[offsets[10]].range(447, 416) = data[10][13];
			HBM_channelA10[offsets[10]].range(479, 448) = data[10][14];
			HBM_channelA10[offsets[10]].range(511, 480) = data[10][15];
			HBM_channelB10[offsets[10]].range(31, 0) = data[10][16];
			HBM_channelB10[offsets[10]].range(63, 32) = data[10][17];
			HBM_channelB10[offsets[10]].range(95, 64) = data[10][18];
			HBM_channelB10[offsets[10]].range(127, 96) = data[10][19];
			HBM_channelB10[offsets[10]].range(159, 128) = data[10][20];
			HBM_channelB10[offsets[10]].range(191, 160) = data[10][21];
			HBM_channelB10[offsets[10]].range(223, 192) = data[10][22];
			HBM_channelB10[offsets[10]].range(255, 224) = data[10][23];
			HBM_channelB10[offsets[10]].range(287, 256) = data[10][24];
			HBM_channelB10[offsets[10]].range(319, 288) = data[10][25];
			HBM_channelB10[offsets[10]].range(351, 320) = data[10][26];
			HBM_channelB10[offsets[10]].range(383, 352) = data[10][27];
			HBM_channelB10[offsets[10]].range(415, 384) = data[10][28];
			HBM_channelB10[offsets[10]].range(447, 416) = data[10][29];
			HBM_channelB10[offsets[10]].range(479, 448) = data[10][30];
			HBM_channelB10[offsets[10]].range(511, 480) = data[10][31];
		}
		if(ens[11]==true){
			HBM_channelA11[offsets[11]].range(31, 0) = data[11][0];
			HBM_channelA11[offsets[11]].range(63, 32) = data[11][1];
			HBM_channelA11[offsets[11]].range(95, 64) = data[11][2];
			HBM_channelA11[offsets[11]].range(127, 96) = data[11][3];
			HBM_channelA11[offsets[11]].range(159, 128) = data[11][4];
			HBM_channelA11[offsets[11]].range(191, 160) = data[11][5];
			HBM_channelA11[offsets[11]].range(223, 192) = data[11][6];
			HBM_channelA11[offsets[11]].range(255, 224) = data[11][7];
			HBM_channelA11[offsets[11]].range(287, 256) = data[11][8];
			HBM_channelA11[offsets[11]].range(319, 288) = data[11][9];
			HBM_channelA11[offsets[11]].range(351, 320) = data[11][10];
			HBM_channelA11[offsets[11]].range(383, 352) = data[11][11];
			HBM_channelA11[offsets[11]].range(415, 384) = data[11][12];
			HBM_channelA11[offsets[11]].range(447, 416) = data[11][13];
			HBM_channelA11[offsets[11]].range(479, 448) = data[11][14];
			HBM_channelA11[offsets[11]].range(511, 480) = data[11][15];
			HBM_channelB11[offsets[11]].range(31, 0) = data[11][16];
			HBM_channelB11[offsets[11]].range(63, 32) = data[11][17];
			HBM_channelB11[offsets[11]].range(95, 64) = data[11][18];
			HBM_channelB11[offsets[11]].range(127, 96) = data[11][19];
			HBM_channelB11[offsets[11]].range(159, 128) = data[11][20];
			HBM_channelB11[offsets[11]].range(191, 160) = data[11][21];
			HBM_channelB11[offsets[11]].range(223, 192) = data[11][22];
			HBM_channelB11[offsets[11]].range(255, 224) = data[11][23];
			HBM_channelB11[offsets[11]].range(287, 256) = data[11][24];
			HBM_channelB11[offsets[11]].range(319, 288) = data[11][25];
			HBM_channelB11[offsets[11]].range(351, 320) = data[11][26];
			HBM_channelB11[offsets[11]].range(383, 352) = data[11][27];
			HBM_channelB11[offsets[11]].range(415, 384) = data[11][28];
			HBM_channelB11[offsets[11]].range(447, 416) = data[11][29];
			HBM_channelB11[offsets[11]].range(479, 448) = data[11][30];
			HBM_channelB11[offsets[11]].range(511, 480) = data[11][31];
		}
		if(ens[12]==true){
			HBM_channelA12[offsets[12]].range(31, 0) = data[12][0];
			HBM_channelA12[offsets[12]].range(63, 32) = data[12][1];
			HBM_channelA12[offsets[12]].range(95, 64) = data[12][2];
			HBM_channelA12[offsets[12]].range(127, 96) = data[12][3];
			HBM_channelA12[offsets[12]].range(159, 128) = data[12][4];
			HBM_channelA12[offsets[12]].range(191, 160) = data[12][5];
			HBM_channelA12[offsets[12]].range(223, 192) = data[12][6];
			HBM_channelA12[offsets[12]].range(255, 224) = data[12][7];
			HBM_channelA12[offsets[12]].range(287, 256) = data[12][8];
			HBM_channelA12[offsets[12]].range(319, 288) = data[12][9];
			HBM_channelA12[offsets[12]].range(351, 320) = data[12][10];
			HBM_channelA12[offsets[12]].range(383, 352) = data[12][11];
			HBM_channelA12[offsets[12]].range(415, 384) = data[12][12];
			HBM_channelA12[offsets[12]].range(447, 416) = data[12][13];
			HBM_channelA12[offsets[12]].range(479, 448) = data[12][14];
			HBM_channelA12[offsets[12]].range(511, 480) = data[12][15];
			HBM_channelB12[offsets[12]].range(31, 0) = data[12][16];
			HBM_channelB12[offsets[12]].range(63, 32) = data[12][17];
			HBM_channelB12[offsets[12]].range(95, 64) = data[12][18];
			HBM_channelB12[offsets[12]].range(127, 96) = data[12][19];
			HBM_channelB12[offsets[12]].range(159, 128) = data[12][20];
			HBM_channelB12[offsets[12]].range(191, 160) = data[12][21];
			HBM_channelB12[offsets[12]].range(223, 192) = data[12][22];
			HBM_channelB12[offsets[12]].range(255, 224) = data[12][23];
			HBM_channelB12[offsets[12]].range(287, 256) = data[12][24];
			HBM_channelB12[offsets[12]].range(319, 288) = data[12][25];
			HBM_channelB12[offsets[12]].range(351, 320) = data[12][26];
			HBM_channelB12[offsets[12]].range(383, 352) = data[12][27];
			HBM_channelB12[offsets[12]].range(415, 384) = data[12][28];
			HBM_channelB12[offsets[12]].range(447, 416) = data[12][29];
			HBM_channelB12[offsets[12]].range(479, 448) = data[12][30];
			HBM_channelB12[offsets[12]].range(511, 480) = data[12][31];
		}
		if(ens[13]==true){
			HBM_channelA13[offsets[13]].range(31, 0) = data[13][0];
			HBM_channelA13[offsets[13]].range(63, 32) = data[13][1];
			HBM_channelA13[offsets[13]].range(95, 64) = data[13][2];
			HBM_channelA13[offsets[13]].range(127, 96) = data[13][3];
			HBM_channelA13[offsets[13]].range(159, 128) = data[13][4];
			HBM_channelA13[offsets[13]].range(191, 160) = data[13][5];
			HBM_channelA13[offsets[13]].range(223, 192) = data[13][6];
			HBM_channelA13[offsets[13]].range(255, 224) = data[13][7];
			HBM_channelA13[offsets[13]].range(287, 256) = data[13][8];
			HBM_channelA13[offsets[13]].range(319, 288) = data[13][9];
			HBM_channelA13[offsets[13]].range(351, 320) = data[13][10];
			HBM_channelA13[offsets[13]].range(383, 352) = data[13][11];
			HBM_channelA13[offsets[13]].range(415, 384) = data[13][12];
			HBM_channelA13[offsets[13]].range(447, 416) = data[13][13];
			HBM_channelA13[offsets[13]].range(479, 448) = data[13][14];
			HBM_channelA13[offsets[13]].range(511, 480) = data[13][15];
			HBM_channelB13[offsets[13]].range(31, 0) = data[13][16];
			HBM_channelB13[offsets[13]].range(63, 32) = data[13][17];
			HBM_channelB13[offsets[13]].range(95, 64) = data[13][18];
			HBM_channelB13[offsets[13]].range(127, 96) = data[13][19];
			HBM_channelB13[offsets[13]].range(159, 128) = data[13][20];
			HBM_channelB13[offsets[13]].range(191, 160) = data[13][21];
			HBM_channelB13[offsets[13]].range(223, 192) = data[13][22];
			HBM_channelB13[offsets[13]].range(255, 224) = data[13][23];
			HBM_channelB13[offsets[13]].range(287, 256) = data[13][24];
			HBM_channelB13[offsets[13]].range(319, 288) = data[13][25];
			HBM_channelB13[offsets[13]].range(351, 320) = data[13][26];
			HBM_channelB13[offsets[13]].range(383, 352) = data[13][27];
			HBM_channelB13[offsets[13]].range(415, 384) = data[13][28];
			HBM_channelB13[offsets[13]].range(447, 416) = data[13][29];
			HBM_channelB13[offsets[13]].range(479, 448) = data[13][30];
			HBM_channelB13[offsets[13]].range(511, 480) = data[13][31];
		}
		if(ens[14]==true){
			HBM_channelA14[offsets[14]].range(31, 0) = data[14][0];
			HBM_channelA14[offsets[14]].range(63, 32) = data[14][1];
			HBM_channelA14[offsets[14]].range(95, 64) = data[14][2];
			HBM_channelA14[offsets[14]].range(127, 96) = data[14][3];
			HBM_channelA14[offsets[14]].range(159, 128) = data[14][4];
			HBM_channelA14[offsets[14]].range(191, 160) = data[14][5];
			HBM_channelA14[offsets[14]].range(223, 192) = data[14][6];
			HBM_channelA14[offsets[14]].range(255, 224) = data[14][7];
			HBM_channelA14[offsets[14]].range(287, 256) = data[14][8];
			HBM_channelA14[offsets[14]].range(319, 288) = data[14][9];
			HBM_channelA14[offsets[14]].range(351, 320) = data[14][10];
			HBM_channelA14[offsets[14]].range(383, 352) = data[14][11];
			HBM_channelA14[offsets[14]].range(415, 384) = data[14][12];
			HBM_channelA14[offsets[14]].range(447, 416) = data[14][13];
			HBM_channelA14[offsets[14]].range(479, 448) = data[14][14];
			HBM_channelA14[offsets[14]].range(511, 480) = data[14][15];
			HBM_channelB14[offsets[14]].range(31, 0) = data[14][16];
			HBM_channelB14[offsets[14]].range(63, 32) = data[14][17];
			HBM_channelB14[offsets[14]].range(95, 64) = data[14][18];
			HBM_channelB14[offsets[14]].range(127, 96) = data[14][19];
			HBM_channelB14[offsets[14]].range(159, 128) = data[14][20];
			HBM_channelB14[offsets[14]].range(191, 160) = data[14][21];
			HBM_channelB14[offsets[14]].range(223, 192) = data[14][22];
			HBM_channelB14[offsets[14]].range(255, 224) = data[14][23];
			HBM_channelB14[offsets[14]].range(287, 256) = data[14][24];
			HBM_channelB14[offsets[14]].range(319, 288) = data[14][25];
			HBM_channelB14[offsets[14]].range(351, 320) = data[14][26];
			HBM_channelB14[offsets[14]].range(383, 352) = data[14][27];
			HBM_channelB14[offsets[14]].range(415, 384) = data[14][28];
			HBM_channelB14[offsets[14]].range(447, 416) = data[14][29];
			HBM_channelB14[offsets[14]].range(479, 448) = data[14][30];
			HBM_channelB14[offsets[14]].range(511, 480) = data[14][31];
		}
		if(ens[15]==true){
			HBM_channelA15[offsets[15]].range(31, 0) = data[15][0];
			HBM_channelA15[offsets[15]].range(63, 32) = data[15][1];
			HBM_channelA15[offsets[15]].range(95, 64) = data[15][2];
			HBM_channelA15[offsets[15]].range(127, 96) = data[15][3];
			HBM_channelA15[offsets[15]].range(159, 128) = data[15][4];
			HBM_channelA15[offsets[15]].range(191, 160) = data[15][5];
			HBM_channelA15[offsets[15]].range(223, 192) = data[15][6];
			HBM_channelA15[offsets[15]].range(255, 224) = data[15][7];
			HBM_channelA15[offsets[15]].range(287, 256) = data[15][8];
			HBM_channelA15[offsets[15]].range(319, 288) = data[15][9];
			HBM_channelA15[offsets[15]].range(351, 320) = data[15][10];
			HBM_channelA15[offsets[15]].range(383, 352) = data[15][11];
			HBM_channelA15[offsets[15]].range(415, 384) = data[15][12];
			HBM_channelA15[offsets[15]].range(447, 416) = data[15][13];
			HBM_channelA15[offsets[15]].range(479, 448) = data[15][14];
			HBM_channelA15[offsets[15]].range(511, 480) = data[15][15];
			HBM_channelB15[offsets[15]].range(31, 0) = data[15][16];
			HBM_channelB15[offsets[15]].range(63, 32) = data[15][17];
			HBM_channelB15[offsets[15]].range(95, 64) = data[15][18];
			HBM_channelB15[offsets[15]].range(127, 96) = data[15][19];
			HBM_channelB15[offsets[15]].range(159, 128) = data[15][20];
			HBM_channelB15[offsets[15]].range(191, 160) = data[15][21];
			HBM_channelB15[offsets[15]].range(223, 192) = data[15][22];
			HBM_channelB15[offsets[15]].range(255, 224) = data[15][23];
			HBM_channelB15[offsets[15]].range(287, 256) = data[15][24];
			HBM_channelB15[offsets[15]].range(319, 288) = data[15][25];
			HBM_channelB15[offsets[15]].range(351, 320) = data[15][26];
			HBM_channelB15[offsets[15]].range(383, 352) = data[15][27];
			HBM_channelB15[offsets[15]].range(415, 384) = data[15][28];
			HBM_channelB15[offsets[15]].range(447, 416) = data[15][29];
			HBM_channelB15[offsets[15]].range(479, 448) = data[15][30];
			HBM_channelB15[offsets[15]].range(511, 480) = data[15][31];
		}
		if(ens[16]==true){
			HBM_channelA16[offsets[16]].range(31, 0) = data[16][0];
			HBM_channelA16[offsets[16]].range(63, 32) = data[16][1];
			HBM_channelA16[offsets[16]].range(95, 64) = data[16][2];
			HBM_channelA16[offsets[16]].range(127, 96) = data[16][3];
			HBM_channelA16[offsets[16]].range(159, 128) = data[16][4];
			HBM_channelA16[offsets[16]].range(191, 160) = data[16][5];
			HBM_channelA16[offsets[16]].range(223, 192) = data[16][6];
			HBM_channelA16[offsets[16]].range(255, 224) = data[16][7];
			HBM_channelA16[offsets[16]].range(287, 256) = data[16][8];
			HBM_channelA16[offsets[16]].range(319, 288) = data[16][9];
			HBM_channelA16[offsets[16]].range(351, 320) = data[16][10];
			HBM_channelA16[offsets[16]].range(383, 352) = data[16][11];
			HBM_channelA16[offsets[16]].range(415, 384) = data[16][12];
			HBM_channelA16[offsets[16]].range(447, 416) = data[16][13];
			HBM_channelA16[offsets[16]].range(479, 448) = data[16][14];
			HBM_channelA16[offsets[16]].range(511, 480) = data[16][15];
			HBM_channelB16[offsets[16]].range(31, 0) = data[16][16];
			HBM_channelB16[offsets[16]].range(63, 32) = data[16][17];
			HBM_channelB16[offsets[16]].range(95, 64) = data[16][18];
			HBM_channelB16[offsets[16]].range(127, 96) = data[16][19];
			HBM_channelB16[offsets[16]].range(159, 128) = data[16][20];
			HBM_channelB16[offsets[16]].range(191, 160) = data[16][21];
			HBM_channelB16[offsets[16]].range(223, 192) = data[16][22];
			HBM_channelB16[offsets[16]].range(255, 224) = data[16][23];
			HBM_channelB16[offsets[16]].range(287, 256) = data[16][24];
			HBM_channelB16[offsets[16]].range(319, 288) = data[16][25];
			HBM_channelB16[offsets[16]].range(351, 320) = data[16][26];
			HBM_channelB16[offsets[16]].range(383, 352) = data[16][27];
			HBM_channelB16[offsets[16]].range(415, 384) = data[16][28];
			HBM_channelB16[offsets[16]].range(447, 416) = data[16][29];
			HBM_channelB16[offsets[16]].range(479, 448) = data[16][30];
			HBM_channelB16[offsets[16]].range(511, 480) = data[16][31];
		}
		if(ens[17]==true){
			HBM_channelA17[offsets[17]].range(31, 0) = data[17][0];
			HBM_channelA17[offsets[17]].range(63, 32) = data[17][1];
			HBM_channelA17[offsets[17]].range(95, 64) = data[17][2];
			HBM_channelA17[offsets[17]].range(127, 96) = data[17][3];
			HBM_channelA17[offsets[17]].range(159, 128) = data[17][4];
			HBM_channelA17[offsets[17]].range(191, 160) = data[17][5];
			HBM_channelA17[offsets[17]].range(223, 192) = data[17][6];
			HBM_channelA17[offsets[17]].range(255, 224) = data[17][7];
			HBM_channelA17[offsets[17]].range(287, 256) = data[17][8];
			HBM_channelA17[offsets[17]].range(319, 288) = data[17][9];
			HBM_channelA17[offsets[17]].range(351, 320) = data[17][10];
			HBM_channelA17[offsets[17]].range(383, 352) = data[17][11];
			HBM_channelA17[offsets[17]].range(415, 384) = data[17][12];
			HBM_channelA17[offsets[17]].range(447, 416) = data[17][13];
			HBM_channelA17[offsets[17]].range(479, 448) = data[17][14];
			HBM_channelA17[offsets[17]].range(511, 480) = data[17][15];
			HBM_channelB17[offsets[17]].range(31, 0) = data[17][16];
			HBM_channelB17[offsets[17]].range(63, 32) = data[17][17];
			HBM_channelB17[offsets[17]].range(95, 64) = data[17][18];
			HBM_channelB17[offsets[17]].range(127, 96) = data[17][19];
			HBM_channelB17[offsets[17]].range(159, 128) = data[17][20];
			HBM_channelB17[offsets[17]].range(191, 160) = data[17][21];
			HBM_channelB17[offsets[17]].range(223, 192) = data[17][22];
			HBM_channelB17[offsets[17]].range(255, 224) = data[17][23];
			HBM_channelB17[offsets[17]].range(287, 256) = data[17][24];
			HBM_channelB17[offsets[17]].range(319, 288) = data[17][25];
			HBM_channelB17[offsets[17]].range(351, 320) = data[17][26];
			HBM_channelB17[offsets[17]].range(383, 352) = data[17][27];
			HBM_channelB17[offsets[17]].range(415, 384) = data[17][28];
			HBM_channelB17[offsets[17]].range(447, 416) = data[17][29];
			HBM_channelB17[offsets[17]].range(479, 448) = data[17][30];
			HBM_channelB17[offsets[17]].range(511, 480) = data[17][31];
		}
		if(ens[18]==true){
			HBM_channelA18[offsets[18]].range(31, 0) = data[18][0];
			HBM_channelA18[offsets[18]].range(63, 32) = data[18][1];
			HBM_channelA18[offsets[18]].range(95, 64) = data[18][2];
			HBM_channelA18[offsets[18]].range(127, 96) = data[18][3];
			HBM_channelA18[offsets[18]].range(159, 128) = data[18][4];
			HBM_channelA18[offsets[18]].range(191, 160) = data[18][5];
			HBM_channelA18[offsets[18]].range(223, 192) = data[18][6];
			HBM_channelA18[offsets[18]].range(255, 224) = data[18][7];
			HBM_channelA18[offsets[18]].range(287, 256) = data[18][8];
			HBM_channelA18[offsets[18]].range(319, 288) = data[18][9];
			HBM_channelA18[offsets[18]].range(351, 320) = data[18][10];
			HBM_channelA18[offsets[18]].range(383, 352) = data[18][11];
			HBM_channelA18[offsets[18]].range(415, 384) = data[18][12];
			HBM_channelA18[offsets[18]].range(447, 416) = data[18][13];
			HBM_channelA18[offsets[18]].range(479, 448) = data[18][14];
			HBM_channelA18[offsets[18]].range(511, 480) = data[18][15];
			HBM_channelB18[offsets[18]].range(31, 0) = data[18][16];
			HBM_channelB18[offsets[18]].range(63, 32) = data[18][17];
			HBM_channelB18[offsets[18]].range(95, 64) = data[18][18];
			HBM_channelB18[offsets[18]].range(127, 96) = data[18][19];
			HBM_channelB18[offsets[18]].range(159, 128) = data[18][20];
			HBM_channelB18[offsets[18]].range(191, 160) = data[18][21];
			HBM_channelB18[offsets[18]].range(223, 192) = data[18][22];
			HBM_channelB18[offsets[18]].range(255, 224) = data[18][23];
			HBM_channelB18[offsets[18]].range(287, 256) = data[18][24];
			HBM_channelB18[offsets[18]].range(319, 288) = data[18][25];
			HBM_channelB18[offsets[18]].range(351, 320) = data[18][26];
			HBM_channelB18[offsets[18]].range(383, 352) = data[18][27];
			HBM_channelB18[offsets[18]].range(415, 384) = data[18][28];
			HBM_channelB18[offsets[18]].range(447, 416) = data[18][29];
			HBM_channelB18[offsets[18]].range(479, 448) = data[18][30];
			HBM_channelB18[offsets[18]].range(511, 480) = data[18][31];
		}
		if(ens[19]==true){
			HBM_channelA19[offsets[19]].range(31, 0) = data[19][0];
			HBM_channelA19[offsets[19]].range(63, 32) = data[19][1];
			HBM_channelA19[offsets[19]].range(95, 64) = data[19][2];
			HBM_channelA19[offsets[19]].range(127, 96) = data[19][3];
			HBM_channelA19[offsets[19]].range(159, 128) = data[19][4];
			HBM_channelA19[offsets[19]].range(191, 160) = data[19][5];
			HBM_channelA19[offsets[19]].range(223, 192) = data[19][6];
			HBM_channelA19[offsets[19]].range(255, 224) = data[19][7];
			HBM_channelA19[offsets[19]].range(287, 256) = data[19][8];
			HBM_channelA19[offsets[19]].range(319, 288) = data[19][9];
			HBM_channelA19[offsets[19]].range(351, 320) = data[19][10];
			HBM_channelA19[offsets[19]].range(383, 352) = data[19][11];
			HBM_channelA19[offsets[19]].range(415, 384) = data[19][12];
			HBM_channelA19[offsets[19]].range(447, 416) = data[19][13];
			HBM_channelA19[offsets[19]].range(479, 448) = data[19][14];
			HBM_channelA19[offsets[19]].range(511, 480) = data[19][15];
			HBM_channelB19[offsets[19]].range(31, 0) = data[19][16];
			HBM_channelB19[offsets[19]].range(63, 32) = data[19][17];
			HBM_channelB19[offsets[19]].range(95, 64) = data[19][18];
			HBM_channelB19[offsets[19]].range(127, 96) = data[19][19];
			HBM_channelB19[offsets[19]].range(159, 128) = data[19][20];
			HBM_channelB19[offsets[19]].range(191, 160) = data[19][21];
			HBM_channelB19[offsets[19]].range(223, 192) = data[19][22];
			HBM_channelB19[offsets[19]].range(255, 224) = data[19][23];
			HBM_channelB19[offsets[19]].range(287, 256) = data[19][24];
			HBM_channelB19[offsets[19]].range(319, 288) = data[19][25];
			HBM_channelB19[offsets[19]].range(351, 320) = data[19][26];
			HBM_channelB19[offsets[19]].range(383, 352) = data[19][27];
			HBM_channelB19[offsets[19]].range(415, 384) = data[19][28];
			HBM_channelB19[offsets[19]].range(447, 416) = data[19][29];
			HBM_channelB19[offsets[19]].range(479, 448) = data[19][30];
			HBM_channelB19[offsets[19]].range(511, 480) = data[19][31];
		}
		if(ens[20]==true){
			HBM_channelA20[offsets[20]].range(31, 0) = data[20][0];
			HBM_channelA20[offsets[20]].range(63, 32) = data[20][1];
			HBM_channelA20[offsets[20]].range(95, 64) = data[20][2];
			HBM_channelA20[offsets[20]].range(127, 96) = data[20][3];
			HBM_channelA20[offsets[20]].range(159, 128) = data[20][4];
			HBM_channelA20[offsets[20]].range(191, 160) = data[20][5];
			HBM_channelA20[offsets[20]].range(223, 192) = data[20][6];
			HBM_channelA20[offsets[20]].range(255, 224) = data[20][7];
			HBM_channelA20[offsets[20]].range(287, 256) = data[20][8];
			HBM_channelA20[offsets[20]].range(319, 288) = data[20][9];
			HBM_channelA20[offsets[20]].range(351, 320) = data[20][10];
			HBM_channelA20[offsets[20]].range(383, 352) = data[20][11];
			HBM_channelA20[offsets[20]].range(415, 384) = data[20][12];
			HBM_channelA20[offsets[20]].range(447, 416) = data[20][13];
			HBM_channelA20[offsets[20]].range(479, 448) = data[20][14];
			HBM_channelA20[offsets[20]].range(511, 480) = data[20][15];
			HBM_channelB20[offsets[20]].range(31, 0) = data[20][16];
			HBM_channelB20[offsets[20]].range(63, 32) = data[20][17];
			HBM_channelB20[offsets[20]].range(95, 64) = data[20][18];
			HBM_channelB20[offsets[20]].range(127, 96) = data[20][19];
			HBM_channelB20[offsets[20]].range(159, 128) = data[20][20];
			HBM_channelB20[offsets[20]].range(191, 160) = data[20][21];
			HBM_channelB20[offsets[20]].range(223, 192) = data[20][22];
			HBM_channelB20[offsets[20]].range(255, 224) = data[20][23];
			HBM_channelB20[offsets[20]].range(287, 256) = data[20][24];
			HBM_channelB20[offsets[20]].range(319, 288) = data[20][25];
			HBM_channelB20[offsets[20]].range(351, 320) = data[20][26];
			HBM_channelB20[offsets[20]].range(383, 352) = data[20][27];
			HBM_channelB20[offsets[20]].range(415, 384) = data[20][28];
			HBM_channelB20[offsets[20]].range(447, 416) = data[20][29];
			HBM_channelB20[offsets[20]].range(479, 448) = data[20][30];
			HBM_channelB20[offsets[20]].range(511, 480) = data[20][31];
		}
		if(ens[21]==true){
			HBM_channelA21[offsets[21]].range(31, 0) = data[21][0];
			HBM_channelA21[offsets[21]].range(63, 32) = data[21][1];
			HBM_channelA21[offsets[21]].range(95, 64) = data[21][2];
			HBM_channelA21[offsets[21]].range(127, 96) = data[21][3];
			HBM_channelA21[offsets[21]].range(159, 128) = data[21][4];
			HBM_channelA21[offsets[21]].range(191, 160) = data[21][5];
			HBM_channelA21[offsets[21]].range(223, 192) = data[21][6];
			HBM_channelA21[offsets[21]].range(255, 224) = data[21][7];
			HBM_channelA21[offsets[21]].range(287, 256) = data[21][8];
			HBM_channelA21[offsets[21]].range(319, 288) = data[21][9];
			HBM_channelA21[offsets[21]].range(351, 320) = data[21][10];
			HBM_channelA21[offsets[21]].range(383, 352) = data[21][11];
			HBM_channelA21[offsets[21]].range(415, 384) = data[21][12];
			HBM_channelA21[offsets[21]].range(447, 416) = data[21][13];
			HBM_channelA21[offsets[21]].range(479, 448) = data[21][14];
			HBM_channelA21[offsets[21]].range(511, 480) = data[21][15];
			HBM_channelB21[offsets[21]].range(31, 0) = data[21][16];
			HBM_channelB21[offsets[21]].range(63, 32) = data[21][17];
			HBM_channelB21[offsets[21]].range(95, 64) = data[21][18];
			HBM_channelB21[offsets[21]].range(127, 96) = data[21][19];
			HBM_channelB21[offsets[21]].range(159, 128) = data[21][20];
			HBM_channelB21[offsets[21]].range(191, 160) = data[21][21];
			HBM_channelB21[offsets[21]].range(223, 192) = data[21][22];
			HBM_channelB21[offsets[21]].range(255, 224) = data[21][23];
			HBM_channelB21[offsets[21]].range(287, 256) = data[21][24];
			HBM_channelB21[offsets[21]].range(319, 288) = data[21][25];
			HBM_channelB21[offsets[21]].range(351, 320) = data[21][26];
			HBM_channelB21[offsets[21]].range(383, 352) = data[21][27];
			HBM_channelB21[offsets[21]].range(415, 384) = data[21][28];
			HBM_channelB21[offsets[21]].range(447, 416) = data[21][29];
			HBM_channelB21[offsets[21]].range(479, 448) = data[21][30];
			HBM_channelB21[offsets[21]].range(511, 480) = data[21][31];
		}
		if(ens[22]==true){
			HBM_channelA22[offsets[22]].range(31, 0) = data[22][0];
			HBM_channelA22[offsets[22]].range(63, 32) = data[22][1];
			HBM_channelA22[offsets[22]].range(95, 64) = data[22][2];
			HBM_channelA22[offsets[22]].range(127, 96) = data[22][3];
			HBM_channelA22[offsets[22]].range(159, 128) = data[22][4];
			HBM_channelA22[offsets[22]].range(191, 160) = data[22][5];
			HBM_channelA22[offsets[22]].range(223, 192) = data[22][6];
			HBM_channelA22[offsets[22]].range(255, 224) = data[22][7];
			HBM_channelA22[offsets[22]].range(287, 256) = data[22][8];
			HBM_channelA22[offsets[22]].range(319, 288) = data[22][9];
			HBM_channelA22[offsets[22]].range(351, 320) = data[22][10];
			HBM_channelA22[offsets[22]].range(383, 352) = data[22][11];
			HBM_channelA22[offsets[22]].range(415, 384) = data[22][12];
			HBM_channelA22[offsets[22]].range(447, 416) = data[22][13];
			HBM_channelA22[offsets[22]].range(479, 448) = data[22][14];
			HBM_channelA22[offsets[22]].range(511, 480) = data[22][15];
			HBM_channelB22[offsets[22]].range(31, 0) = data[22][16];
			HBM_channelB22[offsets[22]].range(63, 32) = data[22][17];
			HBM_channelB22[offsets[22]].range(95, 64) = data[22][18];
			HBM_channelB22[offsets[22]].range(127, 96) = data[22][19];
			HBM_channelB22[offsets[22]].range(159, 128) = data[22][20];
			HBM_channelB22[offsets[22]].range(191, 160) = data[22][21];
			HBM_channelB22[offsets[22]].range(223, 192) = data[22][22];
			HBM_channelB22[offsets[22]].range(255, 224) = data[22][23];
			HBM_channelB22[offsets[22]].range(287, 256) = data[22][24];
			HBM_channelB22[offsets[22]].range(319, 288) = data[22][25];
			HBM_channelB22[offsets[22]].range(351, 320) = data[22][26];
			HBM_channelB22[offsets[22]].range(383, 352) = data[22][27];
			HBM_channelB22[offsets[22]].range(415, 384) = data[22][28];
			HBM_channelB22[offsets[22]].range(447, 416) = data[22][29];
			HBM_channelB22[offsets[22]].range(479, 448) = data[22][30];
			HBM_channelB22[offsets[22]].range(511, 480) = data[22][31];
		}
		if(ens[23]==true){
			HBM_channelA23[offsets[23]].range(31, 0) = data[23][0];
			HBM_channelA23[offsets[23]].range(63, 32) = data[23][1];
			HBM_channelA23[offsets[23]].range(95, 64) = data[23][2];
			HBM_channelA23[offsets[23]].range(127, 96) = data[23][3];
			HBM_channelA23[offsets[23]].range(159, 128) = data[23][4];
			HBM_channelA23[offsets[23]].range(191, 160) = data[23][5];
			HBM_channelA23[offsets[23]].range(223, 192) = data[23][6];
			HBM_channelA23[offsets[23]].range(255, 224) = data[23][7];
			HBM_channelA23[offsets[23]].range(287, 256) = data[23][8];
			HBM_channelA23[offsets[23]].range(319, 288) = data[23][9];
			HBM_channelA23[offsets[23]].range(351, 320) = data[23][10];
			HBM_channelA23[offsets[23]].range(383, 352) = data[23][11];
			HBM_channelA23[offsets[23]].range(415, 384) = data[23][12];
			HBM_channelA23[offsets[23]].range(447, 416) = data[23][13];
			HBM_channelA23[offsets[23]].range(479, 448) = data[23][14];
			HBM_channelA23[offsets[23]].range(511, 480) = data[23][15];
			HBM_channelB23[offsets[23]].range(31, 0) = data[23][16];
			HBM_channelB23[offsets[23]].range(63, 32) = data[23][17];
			HBM_channelB23[offsets[23]].range(95, 64) = data[23][18];
			HBM_channelB23[offsets[23]].range(127, 96) = data[23][19];
			HBM_channelB23[offsets[23]].range(159, 128) = data[23][20];
			HBM_channelB23[offsets[23]].range(191, 160) = data[23][21];
			HBM_channelB23[offsets[23]].range(223, 192) = data[23][22];
			HBM_channelB23[offsets[23]].range(255, 224) = data[23][23];
			HBM_channelB23[offsets[23]].range(287, 256) = data[23][24];
			HBM_channelB23[offsets[23]].range(319, 288) = data[23][25];
			HBM_channelB23[offsets[23]].range(351, 320) = data[23][26];
			HBM_channelB23[offsets[23]].range(383, 352) = data[23][27];
			HBM_channelB23[offsets[23]].range(415, 384) = data[23][28];
			HBM_channelB23[offsets[23]].range(447, 416) = data[23][29];
			HBM_channelB23[offsets[23]].range(479, 448) = data[23][30];
			HBM_channelB23[offsets[23]].range(511, 480) = data[23][31];
		}
	#else 
		#ifdef ___USE_AXI_CHANNEL___
		if(ens[0]==true){
			HBM_channelA0[offsets[0]].data[0] = data[0][0];
			HBM_channelA0[offsets[0]].data[1] = data[0][1];
			HBM_channelA0[offsets[0]].data[2] = data[0][2];
			HBM_channelA0[offsets[0]].data[3] = data[0][3];
			HBM_channelA0[offsets[0]].data[4] = data[0][4];
			HBM_channelA0[offsets[0]].data[5] = data[0][5];
			HBM_channelA0[offsets[0]].data[6] = data[0][6];
			HBM_channelA0[offsets[0]].data[7] = data[0][7];
			HBM_channelA0[offsets[0]].data[8] = data[0][8];
			HBM_channelA0[offsets[0]].data[9] = data[0][9];
			HBM_channelA0[offsets[0]].data[10] = data[0][10];
			HBM_channelA0[offsets[0]].data[11] = data[0][11];
			HBM_channelA0[offsets[0]].data[12] = data[0][12];
			HBM_channelA0[offsets[0]].data[13] = data[0][13];
			HBM_channelA0[offsets[0]].data[14] = data[0][14];
			HBM_channelA0[offsets[0]].data[15] = data[0][15];
			HBM_channelB0[offsets[0]].data[0] = data[0][16];
			HBM_channelB0[offsets[0]].data[1] = data[0][17];
			HBM_channelB0[offsets[0]].data[2] = data[0][18];
			HBM_channelB0[offsets[0]].data[3] = data[0][19];
			HBM_channelB0[offsets[0]].data[4] = data[0][20];
			HBM_channelB0[offsets[0]].data[5] = data[0][21];
			HBM_channelB0[offsets[0]].data[6] = data[0][22];
			HBM_channelB0[offsets[0]].data[7] = data[0][23];
			HBM_channelB0[offsets[0]].data[8] = data[0][24];
			HBM_channelB0[offsets[0]].data[9] = data[0][25];
			HBM_channelB0[offsets[0]].data[10] = data[0][26];
			HBM_channelB0[offsets[0]].data[11] = data[0][27];
			HBM_channelB0[offsets[0]].data[12] = data[0][28];
			HBM_channelB0[offsets[0]].data[13] = data[0][29];
			HBM_channelB0[offsets[0]].data[14] = data[0][30];
			HBM_channelB0[offsets[0]].data[15] = data[0][31];
		}
		if(ens[1]==true){
			HBM_channelA1[offsets[1]].data[0] = data[1][0];
			HBM_channelA1[offsets[1]].data[1] = data[1][1];
			HBM_channelA1[offsets[1]].data[2] = data[1][2];
			HBM_channelA1[offsets[1]].data[3] = data[1][3];
			HBM_channelA1[offsets[1]].data[4] = data[1][4];
			HBM_channelA1[offsets[1]].data[5] = data[1][5];
			HBM_channelA1[offsets[1]].data[6] = data[1][6];
			HBM_channelA1[offsets[1]].data[7] = data[1][7];
			HBM_channelA1[offsets[1]].data[8] = data[1][8];
			HBM_channelA1[offsets[1]].data[9] = data[1][9];
			HBM_channelA1[offsets[1]].data[10] = data[1][10];
			HBM_channelA1[offsets[1]].data[11] = data[1][11];
			HBM_channelA1[offsets[1]].data[12] = data[1][12];
			HBM_channelA1[offsets[1]].data[13] = data[1][13];
			HBM_channelA1[offsets[1]].data[14] = data[1][14];
			HBM_channelA1[offsets[1]].data[15] = data[1][15];
			HBM_channelB1[offsets[1]].data[0] = data[1][16];
			HBM_channelB1[offsets[1]].data[1] = data[1][17];
			HBM_channelB1[offsets[1]].data[2] = data[1][18];
			HBM_channelB1[offsets[1]].data[3] = data[1][19];
			HBM_channelB1[offsets[1]].data[4] = data[1][20];
			HBM_channelB1[offsets[1]].data[5] = data[1][21];
			HBM_channelB1[offsets[1]].data[6] = data[1][22];
			HBM_channelB1[offsets[1]].data[7] = data[1][23];
			HBM_channelB1[offsets[1]].data[8] = data[1][24];
			HBM_channelB1[offsets[1]].data[9] = data[1][25];
			HBM_channelB1[offsets[1]].data[10] = data[1][26];
			HBM_channelB1[offsets[1]].data[11] = data[1][27];
			HBM_channelB1[offsets[1]].data[12] = data[1][28];
			HBM_channelB1[offsets[1]].data[13] = data[1][29];
			HBM_channelB1[offsets[1]].data[14] = data[1][30];
			HBM_channelB1[offsets[1]].data[15] = data[1][31];
		}
		if(ens[2]==true){
			HBM_channelA2[offsets[2]].data[0] = data[2][0];
			HBM_channelA2[offsets[2]].data[1] = data[2][1];
			HBM_channelA2[offsets[2]].data[2] = data[2][2];
			HBM_channelA2[offsets[2]].data[3] = data[2][3];
			HBM_channelA2[offsets[2]].data[4] = data[2][4];
			HBM_channelA2[offsets[2]].data[5] = data[2][5];
			HBM_channelA2[offsets[2]].data[6] = data[2][6];
			HBM_channelA2[offsets[2]].data[7] = data[2][7];
			HBM_channelA2[offsets[2]].data[8] = data[2][8];
			HBM_channelA2[offsets[2]].data[9] = data[2][9];
			HBM_channelA2[offsets[2]].data[10] = data[2][10];
			HBM_channelA2[offsets[2]].data[11] = data[2][11];
			HBM_channelA2[offsets[2]].data[12] = data[2][12];
			HBM_channelA2[offsets[2]].data[13] = data[2][13];
			HBM_channelA2[offsets[2]].data[14] = data[2][14];
			HBM_channelA2[offsets[2]].data[15] = data[2][15];
			HBM_channelB2[offsets[2]].data[0] = data[2][16];
			HBM_channelB2[offsets[2]].data[1] = data[2][17];
			HBM_channelB2[offsets[2]].data[2] = data[2][18];
			HBM_channelB2[offsets[2]].data[3] = data[2][19];
			HBM_channelB2[offsets[2]].data[4] = data[2][20];
			HBM_channelB2[offsets[2]].data[5] = data[2][21];
			HBM_channelB2[offsets[2]].data[6] = data[2][22];
			HBM_channelB2[offsets[2]].data[7] = data[2][23];
			HBM_channelB2[offsets[2]].data[8] = data[2][24];
			HBM_channelB2[offsets[2]].data[9] = data[2][25];
			HBM_channelB2[offsets[2]].data[10] = data[2][26];
			HBM_channelB2[offsets[2]].data[11] = data[2][27];
			HBM_channelB2[offsets[2]].data[12] = data[2][28];
			HBM_channelB2[offsets[2]].data[13] = data[2][29];
			HBM_channelB2[offsets[2]].data[14] = data[2][30];
			HBM_channelB2[offsets[2]].data[15] = data[2][31];
		}
		if(ens[3]==true){
			HBM_channelA3[offsets[3]].data[0] = data[3][0];
			HBM_channelA3[offsets[3]].data[1] = data[3][1];
			HBM_channelA3[offsets[3]].data[2] = data[3][2];
			HBM_channelA3[offsets[3]].data[3] = data[3][3];
			HBM_channelA3[offsets[3]].data[4] = data[3][4];
			HBM_channelA3[offsets[3]].data[5] = data[3][5];
			HBM_channelA3[offsets[3]].data[6] = data[3][6];
			HBM_channelA3[offsets[3]].data[7] = data[3][7];
			HBM_channelA3[offsets[3]].data[8] = data[3][8];
			HBM_channelA3[offsets[3]].data[9] = data[3][9];
			HBM_channelA3[offsets[3]].data[10] = data[3][10];
			HBM_channelA3[offsets[3]].data[11] = data[3][11];
			HBM_channelA3[offsets[3]].data[12] = data[3][12];
			HBM_channelA3[offsets[3]].data[13] = data[3][13];
			HBM_channelA3[offsets[3]].data[14] = data[3][14];
			HBM_channelA3[offsets[3]].data[15] = data[3][15];
			HBM_channelB3[offsets[3]].data[0] = data[3][16];
			HBM_channelB3[offsets[3]].data[1] = data[3][17];
			HBM_channelB3[offsets[3]].data[2] = data[3][18];
			HBM_channelB3[offsets[3]].data[3] = data[3][19];
			HBM_channelB3[offsets[3]].data[4] = data[3][20];
			HBM_channelB3[offsets[3]].data[5] = data[3][21];
			HBM_channelB3[offsets[3]].data[6] = data[3][22];
			HBM_channelB3[offsets[3]].data[7] = data[3][23];
			HBM_channelB3[offsets[3]].data[8] = data[3][24];
			HBM_channelB3[offsets[3]].data[9] = data[3][25];
			HBM_channelB3[offsets[3]].data[10] = data[3][26];
			HBM_channelB3[offsets[3]].data[11] = data[3][27];
			HBM_channelB3[offsets[3]].data[12] = data[3][28];
			HBM_channelB3[offsets[3]].data[13] = data[3][29];
			HBM_channelB3[offsets[3]].data[14] = data[3][30];
			HBM_channelB3[offsets[3]].data[15] = data[3][31];
		}
		if(ens[4]==true){
			HBM_channelA4[offsets[4]].data[0] = data[4][0];
			HBM_channelA4[offsets[4]].data[1] = data[4][1];
			HBM_channelA4[offsets[4]].data[2] = data[4][2];
			HBM_channelA4[offsets[4]].data[3] = data[4][3];
			HBM_channelA4[offsets[4]].data[4] = data[4][4];
			HBM_channelA4[offsets[4]].data[5] = data[4][5];
			HBM_channelA4[offsets[4]].data[6] = data[4][6];
			HBM_channelA4[offsets[4]].data[7] = data[4][7];
			HBM_channelA4[offsets[4]].data[8] = data[4][8];
			HBM_channelA4[offsets[4]].data[9] = data[4][9];
			HBM_channelA4[offsets[4]].data[10] = data[4][10];
			HBM_channelA4[offsets[4]].data[11] = data[4][11];
			HBM_channelA4[offsets[4]].data[12] = data[4][12];
			HBM_channelA4[offsets[4]].data[13] = data[4][13];
			HBM_channelA4[offsets[4]].data[14] = data[4][14];
			HBM_channelA4[offsets[4]].data[15] = data[4][15];
			HBM_channelB4[offsets[4]].data[0] = data[4][16];
			HBM_channelB4[offsets[4]].data[1] = data[4][17];
			HBM_channelB4[offsets[4]].data[2] = data[4][18];
			HBM_channelB4[offsets[4]].data[3] = data[4][19];
			HBM_channelB4[offsets[4]].data[4] = data[4][20];
			HBM_channelB4[offsets[4]].data[5] = data[4][21];
			HBM_channelB4[offsets[4]].data[6] = data[4][22];
			HBM_channelB4[offsets[4]].data[7] = data[4][23];
			HBM_channelB4[offsets[4]].data[8] = data[4][24];
			HBM_channelB4[offsets[4]].data[9] = data[4][25];
			HBM_channelB4[offsets[4]].data[10] = data[4][26];
			HBM_channelB4[offsets[4]].data[11] = data[4][27];
			HBM_channelB4[offsets[4]].data[12] = data[4][28];
			HBM_channelB4[offsets[4]].data[13] = data[4][29];
			HBM_channelB4[offsets[4]].data[14] = data[4][30];
			HBM_channelB4[offsets[4]].data[15] = data[4][31];
		}
		if(ens[5]==true){
			HBM_channelA5[offsets[5]].data[0] = data[5][0];
			HBM_channelA5[offsets[5]].data[1] = data[5][1];
			HBM_channelA5[offsets[5]].data[2] = data[5][2];
			HBM_channelA5[offsets[5]].data[3] = data[5][3];
			HBM_channelA5[offsets[5]].data[4] = data[5][4];
			HBM_channelA5[offsets[5]].data[5] = data[5][5];
			HBM_channelA5[offsets[5]].data[6] = data[5][6];
			HBM_channelA5[offsets[5]].data[7] = data[5][7];
			HBM_channelA5[offsets[5]].data[8] = data[5][8];
			HBM_channelA5[offsets[5]].data[9] = data[5][9];
			HBM_channelA5[offsets[5]].data[10] = data[5][10];
			HBM_channelA5[offsets[5]].data[11] = data[5][11];
			HBM_channelA5[offsets[5]].data[12] = data[5][12];
			HBM_channelA5[offsets[5]].data[13] = data[5][13];
			HBM_channelA5[offsets[5]].data[14] = data[5][14];
			HBM_channelA5[offsets[5]].data[15] = data[5][15];
			HBM_channelB5[offsets[5]].data[0] = data[5][16];
			HBM_channelB5[offsets[5]].data[1] = data[5][17];
			HBM_channelB5[offsets[5]].data[2] = data[5][18];
			HBM_channelB5[offsets[5]].data[3] = data[5][19];
			HBM_channelB5[offsets[5]].data[4] = data[5][20];
			HBM_channelB5[offsets[5]].data[5] = data[5][21];
			HBM_channelB5[offsets[5]].data[6] = data[5][22];
			HBM_channelB5[offsets[5]].data[7] = data[5][23];
			HBM_channelB5[offsets[5]].data[8] = data[5][24];
			HBM_channelB5[offsets[5]].data[9] = data[5][25];
			HBM_channelB5[offsets[5]].data[10] = data[5][26];
			HBM_channelB5[offsets[5]].data[11] = data[5][27];
			HBM_channelB5[offsets[5]].data[12] = data[5][28];
			HBM_channelB5[offsets[5]].data[13] = data[5][29];
			HBM_channelB5[offsets[5]].data[14] = data[5][30];
			HBM_channelB5[offsets[5]].data[15] = data[5][31];
		}
		if(ens[6]==true){
			HBM_channelA6[offsets[6]].data[0] = data[6][0];
			HBM_channelA6[offsets[6]].data[1] = data[6][1];
			HBM_channelA6[offsets[6]].data[2] = data[6][2];
			HBM_channelA6[offsets[6]].data[3] = data[6][3];
			HBM_channelA6[offsets[6]].data[4] = data[6][4];
			HBM_channelA6[offsets[6]].data[5] = data[6][5];
			HBM_channelA6[offsets[6]].data[6] = data[6][6];
			HBM_channelA6[offsets[6]].data[7] = data[6][7];
			HBM_channelA6[offsets[6]].data[8] = data[6][8];
			HBM_channelA6[offsets[6]].data[9] = data[6][9];
			HBM_channelA6[offsets[6]].data[10] = data[6][10];
			HBM_channelA6[offsets[6]].data[11] = data[6][11];
			HBM_channelA6[offsets[6]].data[12] = data[6][12];
			HBM_channelA6[offsets[6]].data[13] = data[6][13];
			HBM_channelA6[offsets[6]].data[14] = data[6][14];
			HBM_channelA6[offsets[6]].data[15] = data[6][15];
			HBM_channelB6[offsets[6]].data[0] = data[6][16];
			HBM_channelB6[offsets[6]].data[1] = data[6][17];
			HBM_channelB6[offsets[6]].data[2] = data[6][18];
			HBM_channelB6[offsets[6]].data[3] = data[6][19];
			HBM_channelB6[offsets[6]].data[4] = data[6][20];
			HBM_channelB6[offsets[6]].data[5] = data[6][21];
			HBM_channelB6[offsets[6]].data[6] = data[6][22];
			HBM_channelB6[offsets[6]].data[7] = data[6][23];
			HBM_channelB6[offsets[6]].data[8] = data[6][24];
			HBM_channelB6[offsets[6]].data[9] = data[6][25];
			HBM_channelB6[offsets[6]].data[10] = data[6][26];
			HBM_channelB6[offsets[6]].data[11] = data[6][27];
			HBM_channelB6[offsets[6]].data[12] = data[6][28];
			HBM_channelB6[offsets[6]].data[13] = data[6][29];
			HBM_channelB6[offsets[6]].data[14] = data[6][30];
			HBM_channelB6[offsets[6]].data[15] = data[6][31];
		}
		if(ens[7]==true){
			HBM_channelA7[offsets[7]].data[0] = data[7][0];
			HBM_channelA7[offsets[7]].data[1] = data[7][1];
			HBM_channelA7[offsets[7]].data[2] = data[7][2];
			HBM_channelA7[offsets[7]].data[3] = data[7][3];
			HBM_channelA7[offsets[7]].data[4] = data[7][4];
			HBM_channelA7[offsets[7]].data[5] = data[7][5];
			HBM_channelA7[offsets[7]].data[6] = data[7][6];
			HBM_channelA7[offsets[7]].data[7] = data[7][7];
			HBM_channelA7[offsets[7]].data[8] = data[7][8];
			HBM_channelA7[offsets[7]].data[9] = data[7][9];
			HBM_channelA7[offsets[7]].data[10] = data[7][10];
			HBM_channelA7[offsets[7]].data[11] = data[7][11];
			HBM_channelA7[offsets[7]].data[12] = data[7][12];
			HBM_channelA7[offsets[7]].data[13] = data[7][13];
			HBM_channelA7[offsets[7]].data[14] = data[7][14];
			HBM_channelA7[offsets[7]].data[15] = data[7][15];
			HBM_channelB7[offsets[7]].data[0] = data[7][16];
			HBM_channelB7[offsets[7]].data[1] = data[7][17];
			HBM_channelB7[offsets[7]].data[2] = data[7][18];
			HBM_channelB7[offsets[7]].data[3] = data[7][19];
			HBM_channelB7[offsets[7]].data[4] = data[7][20];
			HBM_channelB7[offsets[7]].data[5] = data[7][21];
			HBM_channelB7[offsets[7]].data[6] = data[7][22];
			HBM_channelB7[offsets[7]].data[7] = data[7][23];
			HBM_channelB7[offsets[7]].data[8] = data[7][24];
			HBM_channelB7[offsets[7]].data[9] = data[7][25];
			HBM_channelB7[offsets[7]].data[10] = data[7][26];
			HBM_channelB7[offsets[7]].data[11] = data[7][27];
			HBM_channelB7[offsets[7]].data[12] = data[7][28];
			HBM_channelB7[offsets[7]].data[13] = data[7][29];
			HBM_channelB7[offsets[7]].data[14] = data[7][30];
			HBM_channelB7[offsets[7]].data[15] = data[7][31];
		}
		if(ens[8]==true){
			HBM_channelA8[offsets[8]].data[0] = data[8][0];
			HBM_channelA8[offsets[8]].data[1] = data[8][1];
			HBM_channelA8[offsets[8]].data[2] = data[8][2];
			HBM_channelA8[offsets[8]].data[3] = data[8][3];
			HBM_channelA8[offsets[8]].data[4] = data[8][4];
			HBM_channelA8[offsets[8]].data[5] = data[8][5];
			HBM_channelA8[offsets[8]].data[6] = data[8][6];
			HBM_channelA8[offsets[8]].data[7] = data[8][7];
			HBM_channelA8[offsets[8]].data[8] = data[8][8];
			HBM_channelA8[offsets[8]].data[9] = data[8][9];
			HBM_channelA8[offsets[8]].data[10] = data[8][10];
			HBM_channelA8[offsets[8]].data[11] = data[8][11];
			HBM_channelA8[offsets[8]].data[12] = data[8][12];
			HBM_channelA8[offsets[8]].data[13] = data[8][13];
			HBM_channelA8[offsets[8]].data[14] = data[8][14];
			HBM_channelA8[offsets[8]].data[15] = data[8][15];
			HBM_channelB8[offsets[8]].data[0] = data[8][16];
			HBM_channelB8[offsets[8]].data[1] = data[8][17];
			HBM_channelB8[offsets[8]].data[2] = data[8][18];
			HBM_channelB8[offsets[8]].data[3] = data[8][19];
			HBM_channelB8[offsets[8]].data[4] = data[8][20];
			HBM_channelB8[offsets[8]].data[5] = data[8][21];
			HBM_channelB8[offsets[8]].data[6] = data[8][22];
			HBM_channelB8[offsets[8]].data[7] = data[8][23];
			HBM_channelB8[offsets[8]].data[8] = data[8][24];
			HBM_channelB8[offsets[8]].data[9] = data[8][25];
			HBM_channelB8[offsets[8]].data[10] = data[8][26];
			HBM_channelB8[offsets[8]].data[11] = data[8][27];
			HBM_channelB8[offsets[8]].data[12] = data[8][28];
			HBM_channelB8[offsets[8]].data[13] = data[8][29];
			HBM_channelB8[offsets[8]].data[14] = data[8][30];
			HBM_channelB8[offsets[8]].data[15] = data[8][31];
		}
		if(ens[9]==true){
			HBM_channelA9[offsets[9]].data[0] = data[9][0];
			HBM_channelA9[offsets[9]].data[1] = data[9][1];
			HBM_channelA9[offsets[9]].data[2] = data[9][2];
			HBM_channelA9[offsets[9]].data[3] = data[9][3];
			HBM_channelA9[offsets[9]].data[4] = data[9][4];
			HBM_channelA9[offsets[9]].data[5] = data[9][5];
			HBM_channelA9[offsets[9]].data[6] = data[9][6];
			HBM_channelA9[offsets[9]].data[7] = data[9][7];
			HBM_channelA9[offsets[9]].data[8] = data[9][8];
			HBM_channelA9[offsets[9]].data[9] = data[9][9];
			HBM_channelA9[offsets[9]].data[10] = data[9][10];
			HBM_channelA9[offsets[9]].data[11] = data[9][11];
			HBM_channelA9[offsets[9]].data[12] = data[9][12];
			HBM_channelA9[offsets[9]].data[13] = data[9][13];
			HBM_channelA9[offsets[9]].data[14] = data[9][14];
			HBM_channelA9[offsets[9]].data[15] = data[9][15];
			HBM_channelB9[offsets[9]].data[0] = data[9][16];
			HBM_channelB9[offsets[9]].data[1] = data[9][17];
			HBM_channelB9[offsets[9]].data[2] = data[9][18];
			HBM_channelB9[offsets[9]].data[3] = data[9][19];
			HBM_channelB9[offsets[9]].data[4] = data[9][20];
			HBM_channelB9[offsets[9]].data[5] = data[9][21];
			HBM_channelB9[offsets[9]].data[6] = data[9][22];
			HBM_channelB9[offsets[9]].data[7] = data[9][23];
			HBM_channelB9[offsets[9]].data[8] = data[9][24];
			HBM_channelB9[offsets[9]].data[9] = data[9][25];
			HBM_channelB9[offsets[9]].data[10] = data[9][26];
			HBM_channelB9[offsets[9]].data[11] = data[9][27];
			HBM_channelB9[offsets[9]].data[12] = data[9][28];
			HBM_channelB9[offsets[9]].data[13] = data[9][29];
			HBM_channelB9[offsets[9]].data[14] = data[9][30];
			HBM_channelB9[offsets[9]].data[15] = data[9][31];
		}
		if(ens[10]==true){
			HBM_channelA10[offsets[10]].data[0] = data[10][0];
			HBM_channelA10[offsets[10]].data[1] = data[10][1];
			HBM_channelA10[offsets[10]].data[2] = data[10][2];
			HBM_channelA10[offsets[10]].data[3] = data[10][3];
			HBM_channelA10[offsets[10]].data[4] = data[10][4];
			HBM_channelA10[offsets[10]].data[5] = data[10][5];
			HBM_channelA10[offsets[10]].data[6] = data[10][6];
			HBM_channelA10[offsets[10]].data[7] = data[10][7];
			HBM_channelA10[offsets[10]].data[8] = data[10][8];
			HBM_channelA10[offsets[10]].data[9] = data[10][9];
			HBM_channelA10[offsets[10]].data[10] = data[10][10];
			HBM_channelA10[offsets[10]].data[11] = data[10][11];
			HBM_channelA10[offsets[10]].data[12] = data[10][12];
			HBM_channelA10[offsets[10]].data[13] = data[10][13];
			HBM_channelA10[offsets[10]].data[14] = data[10][14];
			HBM_channelA10[offsets[10]].data[15] = data[10][15];
			HBM_channelB10[offsets[10]].data[0] = data[10][16];
			HBM_channelB10[offsets[10]].data[1] = data[10][17];
			HBM_channelB10[offsets[10]].data[2] = data[10][18];
			HBM_channelB10[offsets[10]].data[3] = data[10][19];
			HBM_channelB10[offsets[10]].data[4] = data[10][20];
			HBM_channelB10[offsets[10]].data[5] = data[10][21];
			HBM_channelB10[offsets[10]].data[6] = data[10][22];
			HBM_channelB10[offsets[10]].data[7] = data[10][23];
			HBM_channelB10[offsets[10]].data[8] = data[10][24];
			HBM_channelB10[offsets[10]].data[9] = data[10][25];
			HBM_channelB10[offsets[10]].data[10] = data[10][26];
			HBM_channelB10[offsets[10]].data[11] = data[10][27];
			HBM_channelB10[offsets[10]].data[12] = data[10][28];
			HBM_channelB10[offsets[10]].data[13] = data[10][29];
			HBM_channelB10[offsets[10]].data[14] = data[10][30];
			HBM_channelB10[offsets[10]].data[15] = data[10][31];
		}
		if(ens[11]==true){
			HBM_channelA11[offsets[11]].data[0] = data[11][0];
			HBM_channelA11[offsets[11]].data[1] = data[11][1];
			HBM_channelA11[offsets[11]].data[2] = data[11][2];
			HBM_channelA11[offsets[11]].data[3] = data[11][3];
			HBM_channelA11[offsets[11]].data[4] = data[11][4];
			HBM_channelA11[offsets[11]].data[5] = data[11][5];
			HBM_channelA11[offsets[11]].data[6] = data[11][6];
			HBM_channelA11[offsets[11]].data[7] = data[11][7];
			HBM_channelA11[offsets[11]].data[8] = data[11][8];
			HBM_channelA11[offsets[11]].data[9] = data[11][9];
			HBM_channelA11[offsets[11]].data[10] = data[11][10];
			HBM_channelA11[offsets[11]].data[11] = data[11][11];
			HBM_channelA11[offsets[11]].data[12] = data[11][12];
			HBM_channelA11[offsets[11]].data[13] = data[11][13];
			HBM_channelA11[offsets[11]].data[14] = data[11][14];
			HBM_channelA11[offsets[11]].data[15] = data[11][15];
			HBM_channelB11[offsets[11]].data[0] = data[11][16];
			HBM_channelB11[offsets[11]].data[1] = data[11][17];
			HBM_channelB11[offsets[11]].data[2] = data[11][18];
			HBM_channelB11[offsets[11]].data[3] = data[11][19];
			HBM_channelB11[offsets[11]].data[4] = data[11][20];
			HBM_channelB11[offsets[11]].data[5] = data[11][21];
			HBM_channelB11[offsets[11]].data[6] = data[11][22];
			HBM_channelB11[offsets[11]].data[7] = data[11][23];
			HBM_channelB11[offsets[11]].data[8] = data[11][24];
			HBM_channelB11[offsets[11]].data[9] = data[11][25];
			HBM_channelB11[offsets[11]].data[10] = data[11][26];
			HBM_channelB11[offsets[11]].data[11] = data[11][27];
			HBM_channelB11[offsets[11]].data[12] = data[11][28];
			HBM_channelB11[offsets[11]].data[13] = data[11][29];
			HBM_channelB11[offsets[11]].data[14] = data[11][30];
			HBM_channelB11[offsets[11]].data[15] = data[11][31];
		}
		if(ens[12]==true){
			HBM_channelA12[offsets[12]].data[0] = data[12][0];
			HBM_channelA12[offsets[12]].data[1] = data[12][1];
			HBM_channelA12[offsets[12]].data[2] = data[12][2];
			HBM_channelA12[offsets[12]].data[3] = data[12][3];
			HBM_channelA12[offsets[12]].data[4] = data[12][4];
			HBM_channelA12[offsets[12]].data[5] = data[12][5];
			HBM_channelA12[offsets[12]].data[6] = data[12][6];
			HBM_channelA12[offsets[12]].data[7] = data[12][7];
			HBM_channelA12[offsets[12]].data[8] = data[12][8];
			HBM_channelA12[offsets[12]].data[9] = data[12][9];
			HBM_channelA12[offsets[12]].data[10] = data[12][10];
			HBM_channelA12[offsets[12]].data[11] = data[12][11];
			HBM_channelA12[offsets[12]].data[12] = data[12][12];
			HBM_channelA12[offsets[12]].data[13] = data[12][13];
			HBM_channelA12[offsets[12]].data[14] = data[12][14];
			HBM_channelA12[offsets[12]].data[15] = data[12][15];
			HBM_channelB12[offsets[12]].data[0] = data[12][16];
			HBM_channelB12[offsets[12]].data[1] = data[12][17];
			HBM_channelB12[offsets[12]].data[2] = data[12][18];
			HBM_channelB12[offsets[12]].data[3] = data[12][19];
			HBM_channelB12[offsets[12]].data[4] = data[12][20];
			HBM_channelB12[offsets[12]].data[5] = data[12][21];
			HBM_channelB12[offsets[12]].data[6] = data[12][22];
			HBM_channelB12[offsets[12]].data[7] = data[12][23];
			HBM_channelB12[offsets[12]].data[8] = data[12][24];
			HBM_channelB12[offsets[12]].data[9] = data[12][25];
			HBM_channelB12[offsets[12]].data[10] = data[12][26];
			HBM_channelB12[offsets[12]].data[11] = data[12][27];
			HBM_channelB12[offsets[12]].data[12] = data[12][28];
			HBM_channelB12[offsets[12]].data[13] = data[12][29];
			HBM_channelB12[offsets[12]].data[14] = data[12][30];
			HBM_channelB12[offsets[12]].data[15] = data[12][31];
		}
		if(ens[13]==true){
			HBM_channelA13[offsets[13]].data[0] = data[13][0];
			HBM_channelA13[offsets[13]].data[1] = data[13][1];
			HBM_channelA13[offsets[13]].data[2] = data[13][2];
			HBM_channelA13[offsets[13]].data[3] = data[13][3];
			HBM_channelA13[offsets[13]].data[4] = data[13][4];
			HBM_channelA13[offsets[13]].data[5] = data[13][5];
			HBM_channelA13[offsets[13]].data[6] = data[13][6];
			HBM_channelA13[offsets[13]].data[7] = data[13][7];
			HBM_channelA13[offsets[13]].data[8] = data[13][8];
			HBM_channelA13[offsets[13]].data[9] = data[13][9];
			HBM_channelA13[offsets[13]].data[10] = data[13][10];
			HBM_channelA13[offsets[13]].data[11] = data[13][11];
			HBM_channelA13[offsets[13]].data[12] = data[13][12];
			HBM_channelA13[offsets[13]].data[13] = data[13][13];
			HBM_channelA13[offsets[13]].data[14] = data[13][14];
			HBM_channelA13[offsets[13]].data[15] = data[13][15];
			HBM_channelB13[offsets[13]].data[0] = data[13][16];
			HBM_channelB13[offsets[13]].data[1] = data[13][17];
			HBM_channelB13[offsets[13]].data[2] = data[13][18];
			HBM_channelB13[offsets[13]].data[3] = data[13][19];
			HBM_channelB13[offsets[13]].data[4] = data[13][20];
			HBM_channelB13[offsets[13]].data[5] = data[13][21];
			HBM_channelB13[offsets[13]].data[6] = data[13][22];
			HBM_channelB13[offsets[13]].data[7] = data[13][23];
			HBM_channelB13[offsets[13]].data[8] = data[13][24];
			HBM_channelB13[offsets[13]].data[9] = data[13][25];
			HBM_channelB13[offsets[13]].data[10] = data[13][26];
			HBM_channelB13[offsets[13]].data[11] = data[13][27];
			HBM_channelB13[offsets[13]].data[12] = data[13][28];
			HBM_channelB13[offsets[13]].data[13] = data[13][29];
			HBM_channelB13[offsets[13]].data[14] = data[13][30];
			HBM_channelB13[offsets[13]].data[15] = data[13][31];
		}
		if(ens[14]==true){
			HBM_channelA14[offsets[14]].data[0] = data[14][0];
			HBM_channelA14[offsets[14]].data[1] = data[14][1];
			HBM_channelA14[offsets[14]].data[2] = data[14][2];
			HBM_channelA14[offsets[14]].data[3] = data[14][3];
			HBM_channelA14[offsets[14]].data[4] = data[14][4];
			HBM_channelA14[offsets[14]].data[5] = data[14][5];
			HBM_channelA14[offsets[14]].data[6] = data[14][6];
			HBM_channelA14[offsets[14]].data[7] = data[14][7];
			HBM_channelA14[offsets[14]].data[8] = data[14][8];
			HBM_channelA14[offsets[14]].data[9] = data[14][9];
			HBM_channelA14[offsets[14]].data[10] = data[14][10];
			HBM_channelA14[offsets[14]].data[11] = data[14][11];
			HBM_channelA14[offsets[14]].data[12] = data[14][12];
			HBM_channelA14[offsets[14]].data[13] = data[14][13];
			HBM_channelA14[offsets[14]].data[14] = data[14][14];
			HBM_channelA14[offsets[14]].data[15] = data[14][15];
			HBM_channelB14[offsets[14]].data[0] = data[14][16];
			HBM_channelB14[offsets[14]].data[1] = data[14][17];
			HBM_channelB14[offsets[14]].data[2] = data[14][18];
			HBM_channelB14[offsets[14]].data[3] = data[14][19];
			HBM_channelB14[offsets[14]].data[4] = data[14][20];
			HBM_channelB14[offsets[14]].data[5] = data[14][21];
			HBM_channelB14[offsets[14]].data[6] = data[14][22];
			HBM_channelB14[offsets[14]].data[7] = data[14][23];
			HBM_channelB14[offsets[14]].data[8] = data[14][24];
			HBM_channelB14[offsets[14]].data[9] = data[14][25];
			HBM_channelB14[offsets[14]].data[10] = data[14][26];
			HBM_channelB14[offsets[14]].data[11] = data[14][27];
			HBM_channelB14[offsets[14]].data[12] = data[14][28];
			HBM_channelB14[offsets[14]].data[13] = data[14][29];
			HBM_channelB14[offsets[14]].data[14] = data[14][30];
			HBM_channelB14[offsets[14]].data[15] = data[14][31];
		}
		if(ens[15]==true){
			HBM_channelA15[offsets[15]].data[0] = data[15][0];
			HBM_channelA15[offsets[15]].data[1] = data[15][1];
			HBM_channelA15[offsets[15]].data[2] = data[15][2];
			HBM_channelA15[offsets[15]].data[3] = data[15][3];
			HBM_channelA15[offsets[15]].data[4] = data[15][4];
			HBM_channelA15[offsets[15]].data[5] = data[15][5];
			HBM_channelA15[offsets[15]].data[6] = data[15][6];
			HBM_channelA15[offsets[15]].data[7] = data[15][7];
			HBM_channelA15[offsets[15]].data[8] = data[15][8];
			HBM_channelA15[offsets[15]].data[9] = data[15][9];
			HBM_channelA15[offsets[15]].data[10] = data[15][10];
			HBM_channelA15[offsets[15]].data[11] = data[15][11];
			HBM_channelA15[offsets[15]].data[12] = data[15][12];
			HBM_channelA15[offsets[15]].data[13] = data[15][13];
			HBM_channelA15[offsets[15]].data[14] = data[15][14];
			HBM_channelA15[offsets[15]].data[15] = data[15][15];
			HBM_channelB15[offsets[15]].data[0] = data[15][16];
			HBM_channelB15[offsets[15]].data[1] = data[15][17];
			HBM_channelB15[offsets[15]].data[2] = data[15][18];
			HBM_channelB15[offsets[15]].data[3] = data[15][19];
			HBM_channelB15[offsets[15]].data[4] = data[15][20];
			HBM_channelB15[offsets[15]].data[5] = data[15][21];
			HBM_channelB15[offsets[15]].data[6] = data[15][22];
			HBM_channelB15[offsets[15]].data[7] = data[15][23];
			HBM_channelB15[offsets[15]].data[8] = data[15][24];
			HBM_channelB15[offsets[15]].data[9] = data[15][25];
			HBM_channelB15[offsets[15]].data[10] = data[15][26];
			HBM_channelB15[offsets[15]].data[11] = data[15][27];
			HBM_channelB15[offsets[15]].data[12] = data[15][28];
			HBM_channelB15[offsets[15]].data[13] = data[15][29];
			HBM_channelB15[offsets[15]].data[14] = data[15][30];
			HBM_channelB15[offsets[15]].data[15] = data[15][31];
		}
		if(ens[16]==true){
			HBM_channelA16[offsets[16]].data[0] = data[16][0];
			HBM_channelA16[offsets[16]].data[1] = data[16][1];
			HBM_channelA16[offsets[16]].data[2] = data[16][2];
			HBM_channelA16[offsets[16]].data[3] = data[16][3];
			HBM_channelA16[offsets[16]].data[4] = data[16][4];
			HBM_channelA16[offsets[16]].data[5] = data[16][5];
			HBM_channelA16[offsets[16]].data[6] = data[16][6];
			HBM_channelA16[offsets[16]].data[7] = data[16][7];
			HBM_channelA16[offsets[16]].data[8] = data[16][8];
			HBM_channelA16[offsets[16]].data[9] = data[16][9];
			HBM_channelA16[offsets[16]].data[10] = data[16][10];
			HBM_channelA16[offsets[16]].data[11] = data[16][11];
			HBM_channelA16[offsets[16]].data[12] = data[16][12];
			HBM_channelA16[offsets[16]].data[13] = data[16][13];
			HBM_channelA16[offsets[16]].data[14] = data[16][14];
			HBM_channelA16[offsets[16]].data[15] = data[16][15];
			HBM_channelB16[offsets[16]].data[0] = data[16][16];
			HBM_channelB16[offsets[16]].data[1] = data[16][17];
			HBM_channelB16[offsets[16]].data[2] = data[16][18];
			HBM_channelB16[offsets[16]].data[3] = data[16][19];
			HBM_channelB16[offsets[16]].data[4] = data[16][20];
			HBM_channelB16[offsets[16]].data[5] = data[16][21];
			HBM_channelB16[offsets[16]].data[6] = data[16][22];
			HBM_channelB16[offsets[16]].data[7] = data[16][23];
			HBM_channelB16[offsets[16]].data[8] = data[16][24];
			HBM_channelB16[offsets[16]].data[9] = data[16][25];
			HBM_channelB16[offsets[16]].data[10] = data[16][26];
			HBM_channelB16[offsets[16]].data[11] = data[16][27];
			HBM_channelB16[offsets[16]].data[12] = data[16][28];
			HBM_channelB16[offsets[16]].data[13] = data[16][29];
			HBM_channelB16[offsets[16]].data[14] = data[16][30];
			HBM_channelB16[offsets[16]].data[15] = data[16][31];
		}
		if(ens[17]==true){
			HBM_channelA17[offsets[17]].data[0] = data[17][0];
			HBM_channelA17[offsets[17]].data[1] = data[17][1];
			HBM_channelA17[offsets[17]].data[2] = data[17][2];
			HBM_channelA17[offsets[17]].data[3] = data[17][3];
			HBM_channelA17[offsets[17]].data[4] = data[17][4];
			HBM_channelA17[offsets[17]].data[5] = data[17][5];
			HBM_channelA17[offsets[17]].data[6] = data[17][6];
			HBM_channelA17[offsets[17]].data[7] = data[17][7];
			HBM_channelA17[offsets[17]].data[8] = data[17][8];
			HBM_channelA17[offsets[17]].data[9] = data[17][9];
			HBM_channelA17[offsets[17]].data[10] = data[17][10];
			HBM_channelA17[offsets[17]].data[11] = data[17][11];
			HBM_channelA17[offsets[17]].data[12] = data[17][12];
			HBM_channelA17[offsets[17]].data[13] = data[17][13];
			HBM_channelA17[offsets[17]].data[14] = data[17][14];
			HBM_channelA17[offsets[17]].data[15] = data[17][15];
			HBM_channelB17[offsets[17]].data[0] = data[17][16];
			HBM_channelB17[offsets[17]].data[1] = data[17][17];
			HBM_channelB17[offsets[17]].data[2] = data[17][18];
			HBM_channelB17[offsets[17]].data[3] = data[17][19];
			HBM_channelB17[offsets[17]].data[4] = data[17][20];
			HBM_channelB17[offsets[17]].data[5] = data[17][21];
			HBM_channelB17[offsets[17]].data[6] = data[17][22];
			HBM_channelB17[offsets[17]].data[7] = data[17][23];
			HBM_channelB17[offsets[17]].data[8] = data[17][24];
			HBM_channelB17[offsets[17]].data[9] = data[17][25];
			HBM_channelB17[offsets[17]].data[10] = data[17][26];
			HBM_channelB17[offsets[17]].data[11] = data[17][27];
			HBM_channelB17[offsets[17]].data[12] = data[17][28];
			HBM_channelB17[offsets[17]].data[13] = data[17][29];
			HBM_channelB17[offsets[17]].data[14] = data[17][30];
			HBM_channelB17[offsets[17]].data[15] = data[17][31];
		}
		if(ens[18]==true){
			HBM_channelA18[offsets[18]].data[0] = data[18][0];
			HBM_channelA18[offsets[18]].data[1] = data[18][1];
			HBM_channelA18[offsets[18]].data[2] = data[18][2];
			HBM_channelA18[offsets[18]].data[3] = data[18][3];
			HBM_channelA18[offsets[18]].data[4] = data[18][4];
			HBM_channelA18[offsets[18]].data[5] = data[18][5];
			HBM_channelA18[offsets[18]].data[6] = data[18][6];
			HBM_channelA18[offsets[18]].data[7] = data[18][7];
			HBM_channelA18[offsets[18]].data[8] = data[18][8];
			HBM_channelA18[offsets[18]].data[9] = data[18][9];
			HBM_channelA18[offsets[18]].data[10] = data[18][10];
			HBM_channelA18[offsets[18]].data[11] = data[18][11];
			HBM_channelA18[offsets[18]].data[12] = data[18][12];
			HBM_channelA18[offsets[18]].data[13] = data[18][13];
			HBM_channelA18[offsets[18]].data[14] = data[18][14];
			HBM_channelA18[offsets[18]].data[15] = data[18][15];
			HBM_channelB18[offsets[18]].data[0] = data[18][16];
			HBM_channelB18[offsets[18]].data[1] = data[18][17];
			HBM_channelB18[offsets[18]].data[2] = data[18][18];
			HBM_channelB18[offsets[18]].data[3] = data[18][19];
			HBM_channelB18[offsets[18]].data[4] = data[18][20];
			HBM_channelB18[offsets[18]].data[5] = data[18][21];
			HBM_channelB18[offsets[18]].data[6] = data[18][22];
			HBM_channelB18[offsets[18]].data[7] = data[18][23];
			HBM_channelB18[offsets[18]].data[8] = data[18][24];
			HBM_channelB18[offsets[18]].data[9] = data[18][25];
			HBM_channelB18[offsets[18]].data[10] = data[18][26];
			HBM_channelB18[offsets[18]].data[11] = data[18][27];
			HBM_channelB18[offsets[18]].data[12] = data[18][28];
			HBM_channelB18[offsets[18]].data[13] = data[18][29];
			HBM_channelB18[offsets[18]].data[14] = data[18][30];
			HBM_channelB18[offsets[18]].data[15] = data[18][31];
		}
		if(ens[19]==true){
			HBM_channelA19[offsets[19]].data[0] = data[19][0];
			HBM_channelA19[offsets[19]].data[1] = data[19][1];
			HBM_channelA19[offsets[19]].data[2] = data[19][2];
			HBM_channelA19[offsets[19]].data[3] = data[19][3];
			HBM_channelA19[offsets[19]].data[4] = data[19][4];
			HBM_channelA19[offsets[19]].data[5] = data[19][5];
			HBM_channelA19[offsets[19]].data[6] = data[19][6];
			HBM_channelA19[offsets[19]].data[7] = data[19][7];
			HBM_channelA19[offsets[19]].data[8] = data[19][8];
			HBM_channelA19[offsets[19]].data[9] = data[19][9];
			HBM_channelA19[offsets[19]].data[10] = data[19][10];
			HBM_channelA19[offsets[19]].data[11] = data[19][11];
			HBM_channelA19[offsets[19]].data[12] = data[19][12];
			HBM_channelA19[offsets[19]].data[13] = data[19][13];
			HBM_channelA19[offsets[19]].data[14] = data[19][14];
			HBM_channelA19[offsets[19]].data[15] = data[19][15];
			HBM_channelB19[offsets[19]].data[0] = data[19][16];
			HBM_channelB19[offsets[19]].data[1] = data[19][17];
			HBM_channelB19[offsets[19]].data[2] = data[19][18];
			HBM_channelB19[offsets[19]].data[3] = data[19][19];
			HBM_channelB19[offsets[19]].data[4] = data[19][20];
			HBM_channelB19[offsets[19]].data[5] = data[19][21];
			HBM_channelB19[offsets[19]].data[6] = data[19][22];
			HBM_channelB19[offsets[19]].data[7] = data[19][23];
			HBM_channelB19[offsets[19]].data[8] = data[19][24];
			HBM_channelB19[offsets[19]].data[9] = data[19][25];
			HBM_channelB19[offsets[19]].data[10] = data[19][26];
			HBM_channelB19[offsets[19]].data[11] = data[19][27];
			HBM_channelB19[offsets[19]].data[12] = data[19][28];
			HBM_channelB19[offsets[19]].data[13] = data[19][29];
			HBM_channelB19[offsets[19]].data[14] = data[19][30];
			HBM_channelB19[offsets[19]].data[15] = data[19][31];
		}
		if(ens[20]==true){
			HBM_channelA20[offsets[20]].data[0] = data[20][0];
			HBM_channelA20[offsets[20]].data[1] = data[20][1];
			HBM_channelA20[offsets[20]].data[2] = data[20][2];
			HBM_channelA20[offsets[20]].data[3] = data[20][3];
			HBM_channelA20[offsets[20]].data[4] = data[20][4];
			HBM_channelA20[offsets[20]].data[5] = data[20][5];
			HBM_channelA20[offsets[20]].data[6] = data[20][6];
			HBM_channelA20[offsets[20]].data[7] = data[20][7];
			HBM_channelA20[offsets[20]].data[8] = data[20][8];
			HBM_channelA20[offsets[20]].data[9] = data[20][9];
			HBM_channelA20[offsets[20]].data[10] = data[20][10];
			HBM_channelA20[offsets[20]].data[11] = data[20][11];
			HBM_channelA20[offsets[20]].data[12] = data[20][12];
			HBM_channelA20[offsets[20]].data[13] = data[20][13];
			HBM_channelA20[offsets[20]].data[14] = data[20][14];
			HBM_channelA20[offsets[20]].data[15] = data[20][15];
			HBM_channelB20[offsets[20]].data[0] = data[20][16];
			HBM_channelB20[offsets[20]].data[1] = data[20][17];
			HBM_channelB20[offsets[20]].data[2] = data[20][18];
			HBM_channelB20[offsets[20]].data[3] = data[20][19];
			HBM_channelB20[offsets[20]].data[4] = data[20][20];
			HBM_channelB20[offsets[20]].data[5] = data[20][21];
			HBM_channelB20[offsets[20]].data[6] = data[20][22];
			HBM_channelB20[offsets[20]].data[7] = data[20][23];
			HBM_channelB20[offsets[20]].data[8] = data[20][24];
			HBM_channelB20[offsets[20]].data[9] = data[20][25];
			HBM_channelB20[offsets[20]].data[10] = data[20][26];
			HBM_channelB20[offsets[20]].data[11] = data[20][27];
			HBM_channelB20[offsets[20]].data[12] = data[20][28];
			HBM_channelB20[offsets[20]].data[13] = data[20][29];
			HBM_channelB20[offsets[20]].data[14] = data[20][30];
			HBM_channelB20[offsets[20]].data[15] = data[20][31];
		}
		if(ens[21]==true){
			HBM_channelA21[offsets[21]].data[0] = data[21][0];
			HBM_channelA21[offsets[21]].data[1] = data[21][1];
			HBM_channelA21[offsets[21]].data[2] = data[21][2];
			HBM_channelA21[offsets[21]].data[3] = data[21][3];
			HBM_channelA21[offsets[21]].data[4] = data[21][4];
			HBM_channelA21[offsets[21]].data[5] = data[21][5];
			HBM_channelA21[offsets[21]].data[6] = data[21][6];
			HBM_channelA21[offsets[21]].data[7] = data[21][7];
			HBM_channelA21[offsets[21]].data[8] = data[21][8];
			HBM_channelA21[offsets[21]].data[9] = data[21][9];
			HBM_channelA21[offsets[21]].data[10] = data[21][10];
			HBM_channelA21[offsets[21]].data[11] = data[21][11];
			HBM_channelA21[offsets[21]].data[12] = data[21][12];
			HBM_channelA21[offsets[21]].data[13] = data[21][13];
			HBM_channelA21[offsets[21]].data[14] = data[21][14];
			HBM_channelA21[offsets[21]].data[15] = data[21][15];
			HBM_channelB21[offsets[21]].data[0] = data[21][16];
			HBM_channelB21[offsets[21]].data[1] = data[21][17];
			HBM_channelB21[offsets[21]].data[2] = data[21][18];
			HBM_channelB21[offsets[21]].data[3] = data[21][19];
			HBM_channelB21[offsets[21]].data[4] = data[21][20];
			HBM_channelB21[offsets[21]].data[5] = data[21][21];
			HBM_channelB21[offsets[21]].data[6] = data[21][22];
			HBM_channelB21[offsets[21]].data[7] = data[21][23];
			HBM_channelB21[offsets[21]].data[8] = data[21][24];
			HBM_channelB21[offsets[21]].data[9] = data[21][25];
			HBM_channelB21[offsets[21]].data[10] = data[21][26];
			HBM_channelB21[offsets[21]].data[11] = data[21][27];
			HBM_channelB21[offsets[21]].data[12] = data[21][28];
			HBM_channelB21[offsets[21]].data[13] = data[21][29];
			HBM_channelB21[offsets[21]].data[14] = data[21][30];
			HBM_channelB21[offsets[21]].data[15] = data[21][31];
		}
		if(ens[22]==true){
			HBM_channelA22[offsets[22]].data[0] = data[22][0];
			HBM_channelA22[offsets[22]].data[1] = data[22][1];
			HBM_channelA22[offsets[22]].data[2] = data[22][2];
			HBM_channelA22[offsets[22]].data[3] = data[22][3];
			HBM_channelA22[offsets[22]].data[4] = data[22][4];
			HBM_channelA22[offsets[22]].data[5] = data[22][5];
			HBM_channelA22[offsets[22]].data[6] = data[22][6];
			HBM_channelA22[offsets[22]].data[7] = data[22][7];
			HBM_channelA22[offsets[22]].data[8] = data[22][8];
			HBM_channelA22[offsets[22]].data[9] = data[22][9];
			HBM_channelA22[offsets[22]].data[10] = data[22][10];
			HBM_channelA22[offsets[22]].data[11] = data[22][11];
			HBM_channelA22[offsets[22]].data[12] = data[22][12];
			HBM_channelA22[offsets[22]].data[13] = data[22][13];
			HBM_channelA22[offsets[22]].data[14] = data[22][14];
			HBM_channelA22[offsets[22]].data[15] = data[22][15];
			HBM_channelB22[offsets[22]].data[0] = data[22][16];
			HBM_channelB22[offsets[22]].data[1] = data[22][17];
			HBM_channelB22[offsets[22]].data[2] = data[22][18];
			HBM_channelB22[offsets[22]].data[3] = data[22][19];
			HBM_channelB22[offsets[22]].data[4] = data[22][20];
			HBM_channelB22[offsets[22]].data[5] = data[22][21];
			HBM_channelB22[offsets[22]].data[6] = data[22][22];
			HBM_channelB22[offsets[22]].data[7] = data[22][23];
			HBM_channelB22[offsets[22]].data[8] = data[22][24];
			HBM_channelB22[offsets[22]].data[9] = data[22][25];
			HBM_channelB22[offsets[22]].data[10] = data[22][26];
			HBM_channelB22[offsets[22]].data[11] = data[22][27];
			HBM_channelB22[offsets[22]].data[12] = data[22][28];
			HBM_channelB22[offsets[22]].data[13] = data[22][29];
			HBM_channelB22[offsets[22]].data[14] = data[22][30];
			HBM_channelB22[offsets[22]].data[15] = data[22][31];
		}
		if(ens[23]==true){
			HBM_channelA23[offsets[23]].data[0] = data[23][0];
			HBM_channelA23[offsets[23]].data[1] = data[23][1];
			HBM_channelA23[offsets[23]].data[2] = data[23][2];
			HBM_channelA23[offsets[23]].data[3] = data[23][3];
			HBM_channelA23[offsets[23]].data[4] = data[23][4];
			HBM_channelA23[offsets[23]].data[5] = data[23][5];
			HBM_channelA23[offsets[23]].data[6] = data[23][6];
			HBM_channelA23[offsets[23]].data[7] = data[23][7];
			HBM_channelA23[offsets[23]].data[8] = data[23][8];
			HBM_channelA23[offsets[23]].data[9] = data[23][9];
			HBM_channelA23[offsets[23]].data[10] = data[23][10];
			HBM_channelA23[offsets[23]].data[11] = data[23][11];
			HBM_channelA23[offsets[23]].data[12] = data[23][12];
			HBM_channelA23[offsets[23]].data[13] = data[23][13];
			HBM_channelA23[offsets[23]].data[14] = data[23][14];
			HBM_channelA23[offsets[23]].data[15] = data[23][15];
			HBM_channelB23[offsets[23]].data[0] = data[23][16];
			HBM_channelB23[offsets[23]].data[1] = data[23][17];
			HBM_channelB23[offsets[23]].data[2] = data[23][18];
			HBM_channelB23[offsets[23]].data[3] = data[23][19];
			HBM_channelB23[offsets[23]].data[4] = data[23][20];
			HBM_channelB23[offsets[23]].data[5] = data[23][21];
			HBM_channelB23[offsets[23]].data[6] = data[23][22];
			HBM_channelB23[offsets[23]].data[7] = data[23][23];
			HBM_channelB23[offsets[23]].data[8] = data[23][24];
			HBM_channelB23[offsets[23]].data[9] = data[23][25];
			HBM_channelB23[offsets[23]].data[10] = data[23][26];
			HBM_channelB23[offsets[23]].data[11] = data[23][27];
			HBM_channelB23[offsets[23]].data[12] = data[23][28];
			HBM_channelB23[offsets[23]].data[13] = data[23][29];
			HBM_channelB23[offsets[23]].data[14] = data[23][30];
			HBM_channelB23[offsets[23]].data[15] = data[23][31];
		}
		#else 
		if(ens[0]==true){
			HBM_channelA0[offsets[0]].data[0] = data[0][0];
			HBM_channelA0[offsets[0]].data[1] = data[0][1];
			HBM_channelA0[offsets[0]].data[2] = data[0][2];
			HBM_channelA0[offsets[0]].data[3] = data[0][3];
			HBM_channelA0[offsets[0]].data[4] = data[0][4];
			HBM_channelA0[offsets[0]].data[5] = data[0][5];
			HBM_channelA0[offsets[0]].data[6] = data[0][6];
			HBM_channelA0[offsets[0]].data[7] = data[0][7];
			HBM_channelA0[offsets[0]].data[8] = data[0][8];
			HBM_channelA0[offsets[0]].data[9] = data[0][9];
			HBM_channelA0[offsets[0]].data[10] = data[0][10];
			HBM_channelA0[offsets[0]].data[11] = data[0][11];
			HBM_channelA0[offsets[0]].data[12] = data[0][12];
			HBM_channelA0[offsets[0]].data[13] = data[0][13];
			HBM_channelA0[offsets[0]].data[14] = data[0][14];
			HBM_channelA0[offsets[0]].data[15] = data[0][15];
			HBM_channelA0[offsets[0]].data[16] = data[0][16];
			HBM_channelA0[offsets[0]].data[17] = data[0][17];
			HBM_channelA0[offsets[0]].data[18] = data[0][18];
			HBM_channelA0[offsets[0]].data[19] = data[0][19];
			HBM_channelA0[offsets[0]].data[20] = data[0][20];
			HBM_channelA0[offsets[0]].data[21] = data[0][21];
			HBM_channelA0[offsets[0]].data[22] = data[0][22];
			HBM_channelA0[offsets[0]].data[23] = data[0][23];
			HBM_channelA0[offsets[0]].data[24] = data[0][24];
			HBM_channelA0[offsets[0]].data[25] = data[0][25];
			HBM_channelA0[offsets[0]].data[26] = data[0][26];
			HBM_channelA0[offsets[0]].data[27] = data[0][27];
			HBM_channelA0[offsets[0]].data[28] = data[0][28];
			HBM_channelA0[offsets[0]].data[29] = data[0][29];
			HBM_channelA0[offsets[0]].data[30] = data[0][30];
			HBM_channelA0[offsets[0]].data[31] = data[0][31];
		}
		if(ens[1]==true){
			HBM_channelA1[offsets[1]].data[0] = data[1][0];
			HBM_channelA1[offsets[1]].data[1] = data[1][1];
			HBM_channelA1[offsets[1]].data[2] = data[1][2];
			HBM_channelA1[offsets[1]].data[3] = data[1][3];
			HBM_channelA1[offsets[1]].data[4] = data[1][4];
			HBM_channelA1[offsets[1]].data[5] = data[1][5];
			HBM_channelA1[offsets[1]].data[6] = data[1][6];
			HBM_channelA1[offsets[1]].data[7] = data[1][7];
			HBM_channelA1[offsets[1]].data[8] = data[1][8];
			HBM_channelA1[offsets[1]].data[9] = data[1][9];
			HBM_channelA1[offsets[1]].data[10] = data[1][10];
			HBM_channelA1[offsets[1]].data[11] = data[1][11];
			HBM_channelA1[offsets[1]].data[12] = data[1][12];
			HBM_channelA1[offsets[1]].data[13] = data[1][13];
			HBM_channelA1[offsets[1]].data[14] = data[1][14];
			HBM_channelA1[offsets[1]].data[15] = data[1][15];
			HBM_channelA1[offsets[1]].data[16] = data[1][16];
			HBM_channelA1[offsets[1]].data[17] = data[1][17];
			HBM_channelA1[offsets[1]].data[18] = data[1][18];
			HBM_channelA1[offsets[1]].data[19] = data[1][19];
			HBM_channelA1[offsets[1]].data[20] = data[1][20];
			HBM_channelA1[offsets[1]].data[21] = data[1][21];
			HBM_channelA1[offsets[1]].data[22] = data[1][22];
			HBM_channelA1[offsets[1]].data[23] = data[1][23];
			HBM_channelA1[offsets[1]].data[24] = data[1][24];
			HBM_channelA1[offsets[1]].data[25] = data[1][25];
			HBM_channelA1[offsets[1]].data[26] = data[1][26];
			HBM_channelA1[offsets[1]].data[27] = data[1][27];
			HBM_channelA1[offsets[1]].data[28] = data[1][28];
			HBM_channelA1[offsets[1]].data[29] = data[1][29];
			HBM_channelA1[offsets[1]].data[30] = data[1][30];
			HBM_channelA1[offsets[1]].data[31] = data[1][31];
		}
		if(ens[2]==true){
			HBM_channelA2[offsets[2]].data[0] = data[2][0];
			HBM_channelA2[offsets[2]].data[1] = data[2][1];
			HBM_channelA2[offsets[2]].data[2] = data[2][2];
			HBM_channelA2[offsets[2]].data[3] = data[2][3];
			HBM_channelA2[offsets[2]].data[4] = data[2][4];
			HBM_channelA2[offsets[2]].data[5] = data[2][5];
			HBM_channelA2[offsets[2]].data[6] = data[2][6];
			HBM_channelA2[offsets[2]].data[7] = data[2][7];
			HBM_channelA2[offsets[2]].data[8] = data[2][8];
			HBM_channelA2[offsets[2]].data[9] = data[2][9];
			HBM_channelA2[offsets[2]].data[10] = data[2][10];
			HBM_channelA2[offsets[2]].data[11] = data[2][11];
			HBM_channelA2[offsets[2]].data[12] = data[2][12];
			HBM_channelA2[offsets[2]].data[13] = data[2][13];
			HBM_channelA2[offsets[2]].data[14] = data[2][14];
			HBM_channelA2[offsets[2]].data[15] = data[2][15];
			HBM_channelA2[offsets[2]].data[16] = data[2][16];
			HBM_channelA2[offsets[2]].data[17] = data[2][17];
			HBM_channelA2[offsets[2]].data[18] = data[2][18];
			HBM_channelA2[offsets[2]].data[19] = data[2][19];
			HBM_channelA2[offsets[2]].data[20] = data[2][20];
			HBM_channelA2[offsets[2]].data[21] = data[2][21];
			HBM_channelA2[offsets[2]].data[22] = data[2][22];
			HBM_channelA2[offsets[2]].data[23] = data[2][23];
			HBM_channelA2[offsets[2]].data[24] = data[2][24];
			HBM_channelA2[offsets[2]].data[25] = data[2][25];
			HBM_channelA2[offsets[2]].data[26] = data[2][26];
			HBM_channelA2[offsets[2]].data[27] = data[2][27];
			HBM_channelA2[offsets[2]].data[28] = data[2][28];
			HBM_channelA2[offsets[2]].data[29] = data[2][29];
			HBM_channelA2[offsets[2]].data[30] = data[2][30];
			HBM_channelA2[offsets[2]].data[31] = data[2][31];
		}
		if(ens[3]==true){
			HBM_channelA3[offsets[3]].data[0] = data[3][0];
			HBM_channelA3[offsets[3]].data[1] = data[3][1];
			HBM_channelA3[offsets[3]].data[2] = data[3][2];
			HBM_channelA3[offsets[3]].data[3] = data[3][3];
			HBM_channelA3[offsets[3]].data[4] = data[3][4];
			HBM_channelA3[offsets[3]].data[5] = data[3][5];
			HBM_channelA3[offsets[3]].data[6] = data[3][6];
			HBM_channelA3[offsets[3]].data[7] = data[3][7];
			HBM_channelA3[offsets[3]].data[8] = data[3][8];
			HBM_channelA3[offsets[3]].data[9] = data[3][9];
			HBM_channelA3[offsets[3]].data[10] = data[3][10];
			HBM_channelA3[offsets[3]].data[11] = data[3][11];
			HBM_channelA3[offsets[3]].data[12] = data[3][12];
			HBM_channelA3[offsets[3]].data[13] = data[3][13];
			HBM_channelA3[offsets[3]].data[14] = data[3][14];
			HBM_channelA3[offsets[3]].data[15] = data[3][15];
			HBM_channelA3[offsets[3]].data[16] = data[3][16];
			HBM_channelA3[offsets[3]].data[17] = data[3][17];
			HBM_channelA3[offsets[3]].data[18] = data[3][18];
			HBM_channelA3[offsets[3]].data[19] = data[3][19];
			HBM_channelA3[offsets[3]].data[20] = data[3][20];
			HBM_channelA3[offsets[3]].data[21] = data[3][21];
			HBM_channelA3[offsets[3]].data[22] = data[3][22];
			HBM_channelA3[offsets[3]].data[23] = data[3][23];
			HBM_channelA3[offsets[3]].data[24] = data[3][24];
			HBM_channelA3[offsets[3]].data[25] = data[3][25];
			HBM_channelA3[offsets[3]].data[26] = data[3][26];
			HBM_channelA3[offsets[3]].data[27] = data[3][27];
			HBM_channelA3[offsets[3]].data[28] = data[3][28];
			HBM_channelA3[offsets[3]].data[29] = data[3][29];
			HBM_channelA3[offsets[3]].data[30] = data[3][30];
			HBM_channelA3[offsets[3]].data[31] = data[3][31];
		}
		if(ens[4]==true){
			HBM_channelA4[offsets[4]].data[0] = data[4][0];
			HBM_channelA4[offsets[4]].data[1] = data[4][1];
			HBM_channelA4[offsets[4]].data[2] = data[4][2];
			HBM_channelA4[offsets[4]].data[3] = data[4][3];
			HBM_channelA4[offsets[4]].data[4] = data[4][4];
			HBM_channelA4[offsets[4]].data[5] = data[4][5];
			HBM_channelA4[offsets[4]].data[6] = data[4][6];
			HBM_channelA4[offsets[4]].data[7] = data[4][7];
			HBM_channelA4[offsets[4]].data[8] = data[4][8];
			HBM_channelA4[offsets[4]].data[9] = data[4][9];
			HBM_channelA4[offsets[4]].data[10] = data[4][10];
			HBM_channelA4[offsets[4]].data[11] = data[4][11];
			HBM_channelA4[offsets[4]].data[12] = data[4][12];
			HBM_channelA4[offsets[4]].data[13] = data[4][13];
			HBM_channelA4[offsets[4]].data[14] = data[4][14];
			HBM_channelA4[offsets[4]].data[15] = data[4][15];
			HBM_channelA4[offsets[4]].data[16] = data[4][16];
			HBM_channelA4[offsets[4]].data[17] = data[4][17];
			HBM_channelA4[offsets[4]].data[18] = data[4][18];
			HBM_channelA4[offsets[4]].data[19] = data[4][19];
			HBM_channelA4[offsets[4]].data[20] = data[4][20];
			HBM_channelA4[offsets[4]].data[21] = data[4][21];
			HBM_channelA4[offsets[4]].data[22] = data[4][22];
			HBM_channelA4[offsets[4]].data[23] = data[4][23];
			HBM_channelA4[offsets[4]].data[24] = data[4][24];
			HBM_channelA4[offsets[4]].data[25] = data[4][25];
			HBM_channelA4[offsets[4]].data[26] = data[4][26];
			HBM_channelA4[offsets[4]].data[27] = data[4][27];
			HBM_channelA4[offsets[4]].data[28] = data[4][28];
			HBM_channelA4[offsets[4]].data[29] = data[4][29];
			HBM_channelA4[offsets[4]].data[30] = data[4][30];
			HBM_channelA4[offsets[4]].data[31] = data[4][31];
		}
		if(ens[5]==true){
			HBM_channelA5[offsets[5]].data[0] = data[5][0];
			HBM_channelA5[offsets[5]].data[1] = data[5][1];
			HBM_channelA5[offsets[5]].data[2] = data[5][2];
			HBM_channelA5[offsets[5]].data[3] = data[5][3];
			HBM_channelA5[offsets[5]].data[4] = data[5][4];
			HBM_channelA5[offsets[5]].data[5] = data[5][5];
			HBM_channelA5[offsets[5]].data[6] = data[5][6];
			HBM_channelA5[offsets[5]].data[7] = data[5][7];
			HBM_channelA5[offsets[5]].data[8] = data[5][8];
			HBM_channelA5[offsets[5]].data[9] = data[5][9];
			HBM_channelA5[offsets[5]].data[10] = data[5][10];
			HBM_channelA5[offsets[5]].data[11] = data[5][11];
			HBM_channelA5[offsets[5]].data[12] = data[5][12];
			HBM_channelA5[offsets[5]].data[13] = data[5][13];
			HBM_channelA5[offsets[5]].data[14] = data[5][14];
			HBM_channelA5[offsets[5]].data[15] = data[5][15];
			HBM_channelA5[offsets[5]].data[16] = data[5][16];
			HBM_channelA5[offsets[5]].data[17] = data[5][17];
			HBM_channelA5[offsets[5]].data[18] = data[5][18];
			HBM_channelA5[offsets[5]].data[19] = data[5][19];
			HBM_channelA5[offsets[5]].data[20] = data[5][20];
			HBM_channelA5[offsets[5]].data[21] = data[5][21];
			HBM_channelA5[offsets[5]].data[22] = data[5][22];
			HBM_channelA5[offsets[5]].data[23] = data[5][23];
			HBM_channelA5[offsets[5]].data[24] = data[5][24];
			HBM_channelA5[offsets[5]].data[25] = data[5][25];
			HBM_channelA5[offsets[5]].data[26] = data[5][26];
			HBM_channelA5[offsets[5]].data[27] = data[5][27];
			HBM_channelA5[offsets[5]].data[28] = data[5][28];
			HBM_channelA5[offsets[5]].data[29] = data[5][29];
			HBM_channelA5[offsets[5]].data[30] = data[5][30];
			HBM_channelA5[offsets[5]].data[31] = data[5][31];
		}
		if(ens[6]==true){
			HBM_channelA6[offsets[6]].data[0] = data[6][0];
			HBM_channelA6[offsets[6]].data[1] = data[6][1];
			HBM_channelA6[offsets[6]].data[2] = data[6][2];
			HBM_channelA6[offsets[6]].data[3] = data[6][3];
			HBM_channelA6[offsets[6]].data[4] = data[6][4];
			HBM_channelA6[offsets[6]].data[5] = data[6][5];
			HBM_channelA6[offsets[6]].data[6] = data[6][6];
			HBM_channelA6[offsets[6]].data[7] = data[6][7];
			HBM_channelA6[offsets[6]].data[8] = data[6][8];
			HBM_channelA6[offsets[6]].data[9] = data[6][9];
			HBM_channelA6[offsets[6]].data[10] = data[6][10];
			HBM_channelA6[offsets[6]].data[11] = data[6][11];
			HBM_channelA6[offsets[6]].data[12] = data[6][12];
			HBM_channelA6[offsets[6]].data[13] = data[6][13];
			HBM_channelA6[offsets[6]].data[14] = data[6][14];
			HBM_channelA6[offsets[6]].data[15] = data[6][15];
			HBM_channelA6[offsets[6]].data[16] = data[6][16];
			HBM_channelA6[offsets[6]].data[17] = data[6][17];
			HBM_channelA6[offsets[6]].data[18] = data[6][18];
			HBM_channelA6[offsets[6]].data[19] = data[6][19];
			HBM_channelA6[offsets[6]].data[20] = data[6][20];
			HBM_channelA6[offsets[6]].data[21] = data[6][21];
			HBM_channelA6[offsets[6]].data[22] = data[6][22];
			HBM_channelA6[offsets[6]].data[23] = data[6][23];
			HBM_channelA6[offsets[6]].data[24] = data[6][24];
			HBM_channelA6[offsets[6]].data[25] = data[6][25];
			HBM_channelA6[offsets[6]].data[26] = data[6][26];
			HBM_channelA6[offsets[6]].data[27] = data[6][27];
			HBM_channelA6[offsets[6]].data[28] = data[6][28];
			HBM_channelA6[offsets[6]].data[29] = data[6][29];
			HBM_channelA6[offsets[6]].data[30] = data[6][30];
			HBM_channelA6[offsets[6]].data[31] = data[6][31];
		}
		if(ens[7]==true){
			HBM_channelA7[offsets[7]].data[0] = data[7][0];
			HBM_channelA7[offsets[7]].data[1] = data[7][1];
			HBM_channelA7[offsets[7]].data[2] = data[7][2];
			HBM_channelA7[offsets[7]].data[3] = data[7][3];
			HBM_channelA7[offsets[7]].data[4] = data[7][4];
			HBM_channelA7[offsets[7]].data[5] = data[7][5];
			HBM_channelA7[offsets[7]].data[6] = data[7][6];
			HBM_channelA7[offsets[7]].data[7] = data[7][7];
			HBM_channelA7[offsets[7]].data[8] = data[7][8];
			HBM_channelA7[offsets[7]].data[9] = data[7][9];
			HBM_channelA7[offsets[7]].data[10] = data[7][10];
			HBM_channelA7[offsets[7]].data[11] = data[7][11];
			HBM_channelA7[offsets[7]].data[12] = data[7][12];
			HBM_channelA7[offsets[7]].data[13] = data[7][13];
			HBM_channelA7[offsets[7]].data[14] = data[7][14];
			HBM_channelA7[offsets[7]].data[15] = data[7][15];
			HBM_channelA7[offsets[7]].data[16] = data[7][16];
			HBM_channelA7[offsets[7]].data[17] = data[7][17];
			HBM_channelA7[offsets[7]].data[18] = data[7][18];
			HBM_channelA7[offsets[7]].data[19] = data[7][19];
			HBM_channelA7[offsets[7]].data[20] = data[7][20];
			HBM_channelA7[offsets[7]].data[21] = data[7][21];
			HBM_channelA7[offsets[7]].data[22] = data[7][22];
			HBM_channelA7[offsets[7]].data[23] = data[7][23];
			HBM_channelA7[offsets[7]].data[24] = data[7][24];
			HBM_channelA7[offsets[7]].data[25] = data[7][25];
			HBM_channelA7[offsets[7]].data[26] = data[7][26];
			HBM_channelA7[offsets[7]].data[27] = data[7][27];
			HBM_channelA7[offsets[7]].data[28] = data[7][28];
			HBM_channelA7[offsets[7]].data[29] = data[7][29];
			HBM_channelA7[offsets[7]].data[30] = data[7][30];
			HBM_channelA7[offsets[7]].data[31] = data[7][31];
		}
		if(ens[8]==true){
			HBM_channelA8[offsets[8]].data[0] = data[8][0];
			HBM_channelA8[offsets[8]].data[1] = data[8][1];
			HBM_channelA8[offsets[8]].data[2] = data[8][2];
			HBM_channelA8[offsets[8]].data[3] = data[8][3];
			HBM_channelA8[offsets[8]].data[4] = data[8][4];
			HBM_channelA8[offsets[8]].data[5] = data[8][5];
			HBM_channelA8[offsets[8]].data[6] = data[8][6];
			HBM_channelA8[offsets[8]].data[7] = data[8][7];
			HBM_channelA8[offsets[8]].data[8] = data[8][8];
			HBM_channelA8[offsets[8]].data[9] = data[8][9];
			HBM_channelA8[offsets[8]].data[10] = data[8][10];
			HBM_channelA8[offsets[8]].data[11] = data[8][11];
			HBM_channelA8[offsets[8]].data[12] = data[8][12];
			HBM_channelA8[offsets[8]].data[13] = data[8][13];
			HBM_channelA8[offsets[8]].data[14] = data[8][14];
			HBM_channelA8[offsets[8]].data[15] = data[8][15];
			HBM_channelA8[offsets[8]].data[16] = data[8][16];
			HBM_channelA8[offsets[8]].data[17] = data[8][17];
			HBM_channelA8[offsets[8]].data[18] = data[8][18];
			HBM_channelA8[offsets[8]].data[19] = data[8][19];
			HBM_channelA8[offsets[8]].data[20] = data[8][20];
			HBM_channelA8[offsets[8]].data[21] = data[8][21];
			HBM_channelA8[offsets[8]].data[22] = data[8][22];
			HBM_channelA8[offsets[8]].data[23] = data[8][23];
			HBM_channelA8[offsets[8]].data[24] = data[8][24];
			HBM_channelA8[offsets[8]].data[25] = data[8][25];
			HBM_channelA8[offsets[8]].data[26] = data[8][26];
			HBM_channelA8[offsets[8]].data[27] = data[8][27];
			HBM_channelA8[offsets[8]].data[28] = data[8][28];
			HBM_channelA8[offsets[8]].data[29] = data[8][29];
			HBM_channelA8[offsets[8]].data[30] = data[8][30];
			HBM_channelA8[offsets[8]].data[31] = data[8][31];
		}
		if(ens[9]==true){
			HBM_channelA9[offsets[9]].data[0] = data[9][0];
			HBM_channelA9[offsets[9]].data[1] = data[9][1];
			HBM_channelA9[offsets[9]].data[2] = data[9][2];
			HBM_channelA9[offsets[9]].data[3] = data[9][3];
			HBM_channelA9[offsets[9]].data[4] = data[9][4];
			HBM_channelA9[offsets[9]].data[5] = data[9][5];
			HBM_channelA9[offsets[9]].data[6] = data[9][6];
			HBM_channelA9[offsets[9]].data[7] = data[9][7];
			HBM_channelA9[offsets[9]].data[8] = data[9][8];
			HBM_channelA9[offsets[9]].data[9] = data[9][9];
			HBM_channelA9[offsets[9]].data[10] = data[9][10];
			HBM_channelA9[offsets[9]].data[11] = data[9][11];
			HBM_channelA9[offsets[9]].data[12] = data[9][12];
			HBM_channelA9[offsets[9]].data[13] = data[9][13];
			HBM_channelA9[offsets[9]].data[14] = data[9][14];
			HBM_channelA9[offsets[9]].data[15] = data[9][15];
			HBM_channelA9[offsets[9]].data[16] = data[9][16];
			HBM_channelA9[offsets[9]].data[17] = data[9][17];
			HBM_channelA9[offsets[9]].data[18] = data[9][18];
			HBM_channelA9[offsets[9]].data[19] = data[9][19];
			HBM_channelA9[offsets[9]].data[20] = data[9][20];
			HBM_channelA9[offsets[9]].data[21] = data[9][21];
			HBM_channelA9[offsets[9]].data[22] = data[9][22];
			HBM_channelA9[offsets[9]].data[23] = data[9][23];
			HBM_channelA9[offsets[9]].data[24] = data[9][24];
			HBM_channelA9[offsets[9]].data[25] = data[9][25];
			HBM_channelA9[offsets[9]].data[26] = data[9][26];
			HBM_channelA9[offsets[9]].data[27] = data[9][27];
			HBM_channelA9[offsets[9]].data[28] = data[9][28];
			HBM_channelA9[offsets[9]].data[29] = data[9][29];
			HBM_channelA9[offsets[9]].data[30] = data[9][30];
			HBM_channelA9[offsets[9]].data[31] = data[9][31];
		}
		if(ens[10]==true){
			HBM_channelA10[offsets[10]].data[0] = data[10][0];
			HBM_channelA10[offsets[10]].data[1] = data[10][1];
			HBM_channelA10[offsets[10]].data[2] = data[10][2];
			HBM_channelA10[offsets[10]].data[3] = data[10][3];
			HBM_channelA10[offsets[10]].data[4] = data[10][4];
			HBM_channelA10[offsets[10]].data[5] = data[10][5];
			HBM_channelA10[offsets[10]].data[6] = data[10][6];
			HBM_channelA10[offsets[10]].data[7] = data[10][7];
			HBM_channelA10[offsets[10]].data[8] = data[10][8];
			HBM_channelA10[offsets[10]].data[9] = data[10][9];
			HBM_channelA10[offsets[10]].data[10] = data[10][10];
			HBM_channelA10[offsets[10]].data[11] = data[10][11];
			HBM_channelA10[offsets[10]].data[12] = data[10][12];
			HBM_channelA10[offsets[10]].data[13] = data[10][13];
			HBM_channelA10[offsets[10]].data[14] = data[10][14];
			HBM_channelA10[offsets[10]].data[15] = data[10][15];
			HBM_channelA10[offsets[10]].data[16] = data[10][16];
			HBM_channelA10[offsets[10]].data[17] = data[10][17];
			HBM_channelA10[offsets[10]].data[18] = data[10][18];
			HBM_channelA10[offsets[10]].data[19] = data[10][19];
			HBM_channelA10[offsets[10]].data[20] = data[10][20];
			HBM_channelA10[offsets[10]].data[21] = data[10][21];
			HBM_channelA10[offsets[10]].data[22] = data[10][22];
			HBM_channelA10[offsets[10]].data[23] = data[10][23];
			HBM_channelA10[offsets[10]].data[24] = data[10][24];
			HBM_channelA10[offsets[10]].data[25] = data[10][25];
			HBM_channelA10[offsets[10]].data[26] = data[10][26];
			HBM_channelA10[offsets[10]].data[27] = data[10][27];
			HBM_channelA10[offsets[10]].data[28] = data[10][28];
			HBM_channelA10[offsets[10]].data[29] = data[10][29];
			HBM_channelA10[offsets[10]].data[30] = data[10][30];
			HBM_channelA10[offsets[10]].data[31] = data[10][31];
		}
		if(ens[11]==true){
			HBM_channelA11[offsets[11]].data[0] = data[11][0];
			HBM_channelA11[offsets[11]].data[1] = data[11][1];
			HBM_channelA11[offsets[11]].data[2] = data[11][2];
			HBM_channelA11[offsets[11]].data[3] = data[11][3];
			HBM_channelA11[offsets[11]].data[4] = data[11][4];
			HBM_channelA11[offsets[11]].data[5] = data[11][5];
			HBM_channelA11[offsets[11]].data[6] = data[11][6];
			HBM_channelA11[offsets[11]].data[7] = data[11][7];
			HBM_channelA11[offsets[11]].data[8] = data[11][8];
			HBM_channelA11[offsets[11]].data[9] = data[11][9];
			HBM_channelA11[offsets[11]].data[10] = data[11][10];
			HBM_channelA11[offsets[11]].data[11] = data[11][11];
			HBM_channelA11[offsets[11]].data[12] = data[11][12];
			HBM_channelA11[offsets[11]].data[13] = data[11][13];
			HBM_channelA11[offsets[11]].data[14] = data[11][14];
			HBM_channelA11[offsets[11]].data[15] = data[11][15];
			HBM_channelA11[offsets[11]].data[16] = data[11][16];
			HBM_channelA11[offsets[11]].data[17] = data[11][17];
			HBM_channelA11[offsets[11]].data[18] = data[11][18];
			HBM_channelA11[offsets[11]].data[19] = data[11][19];
			HBM_channelA11[offsets[11]].data[20] = data[11][20];
			HBM_channelA11[offsets[11]].data[21] = data[11][21];
			HBM_channelA11[offsets[11]].data[22] = data[11][22];
			HBM_channelA11[offsets[11]].data[23] = data[11][23];
			HBM_channelA11[offsets[11]].data[24] = data[11][24];
			HBM_channelA11[offsets[11]].data[25] = data[11][25];
			HBM_channelA11[offsets[11]].data[26] = data[11][26];
			HBM_channelA11[offsets[11]].data[27] = data[11][27];
			HBM_channelA11[offsets[11]].data[28] = data[11][28];
			HBM_channelA11[offsets[11]].data[29] = data[11][29];
			HBM_channelA11[offsets[11]].data[30] = data[11][30];
			HBM_channelA11[offsets[11]].data[31] = data[11][31];
		}
		if(ens[12]==true){
			HBM_channelA12[offsets[12]].data[0] = data[12][0];
			HBM_channelA12[offsets[12]].data[1] = data[12][1];
			HBM_channelA12[offsets[12]].data[2] = data[12][2];
			HBM_channelA12[offsets[12]].data[3] = data[12][3];
			HBM_channelA12[offsets[12]].data[4] = data[12][4];
			HBM_channelA12[offsets[12]].data[5] = data[12][5];
			HBM_channelA12[offsets[12]].data[6] = data[12][6];
			HBM_channelA12[offsets[12]].data[7] = data[12][7];
			HBM_channelA12[offsets[12]].data[8] = data[12][8];
			HBM_channelA12[offsets[12]].data[9] = data[12][9];
			HBM_channelA12[offsets[12]].data[10] = data[12][10];
			HBM_channelA12[offsets[12]].data[11] = data[12][11];
			HBM_channelA12[offsets[12]].data[12] = data[12][12];
			HBM_channelA12[offsets[12]].data[13] = data[12][13];
			HBM_channelA12[offsets[12]].data[14] = data[12][14];
			HBM_channelA12[offsets[12]].data[15] = data[12][15];
			HBM_channelA12[offsets[12]].data[16] = data[12][16];
			HBM_channelA12[offsets[12]].data[17] = data[12][17];
			HBM_channelA12[offsets[12]].data[18] = data[12][18];
			HBM_channelA12[offsets[12]].data[19] = data[12][19];
			HBM_channelA12[offsets[12]].data[20] = data[12][20];
			HBM_channelA12[offsets[12]].data[21] = data[12][21];
			HBM_channelA12[offsets[12]].data[22] = data[12][22];
			HBM_channelA12[offsets[12]].data[23] = data[12][23];
			HBM_channelA12[offsets[12]].data[24] = data[12][24];
			HBM_channelA12[offsets[12]].data[25] = data[12][25];
			HBM_channelA12[offsets[12]].data[26] = data[12][26];
			HBM_channelA12[offsets[12]].data[27] = data[12][27];
			HBM_channelA12[offsets[12]].data[28] = data[12][28];
			HBM_channelA12[offsets[12]].data[29] = data[12][29];
			HBM_channelA12[offsets[12]].data[30] = data[12][30];
			HBM_channelA12[offsets[12]].data[31] = data[12][31];
		}
		if(ens[13]==true){
			HBM_channelA13[offsets[13]].data[0] = data[13][0];
			HBM_channelA13[offsets[13]].data[1] = data[13][1];
			HBM_channelA13[offsets[13]].data[2] = data[13][2];
			HBM_channelA13[offsets[13]].data[3] = data[13][3];
			HBM_channelA13[offsets[13]].data[4] = data[13][4];
			HBM_channelA13[offsets[13]].data[5] = data[13][5];
			HBM_channelA13[offsets[13]].data[6] = data[13][6];
			HBM_channelA13[offsets[13]].data[7] = data[13][7];
			HBM_channelA13[offsets[13]].data[8] = data[13][8];
			HBM_channelA13[offsets[13]].data[9] = data[13][9];
			HBM_channelA13[offsets[13]].data[10] = data[13][10];
			HBM_channelA13[offsets[13]].data[11] = data[13][11];
			HBM_channelA13[offsets[13]].data[12] = data[13][12];
			HBM_channelA13[offsets[13]].data[13] = data[13][13];
			HBM_channelA13[offsets[13]].data[14] = data[13][14];
			HBM_channelA13[offsets[13]].data[15] = data[13][15];
			HBM_channelA13[offsets[13]].data[16] = data[13][16];
			HBM_channelA13[offsets[13]].data[17] = data[13][17];
			HBM_channelA13[offsets[13]].data[18] = data[13][18];
			HBM_channelA13[offsets[13]].data[19] = data[13][19];
			HBM_channelA13[offsets[13]].data[20] = data[13][20];
			HBM_channelA13[offsets[13]].data[21] = data[13][21];
			HBM_channelA13[offsets[13]].data[22] = data[13][22];
			HBM_channelA13[offsets[13]].data[23] = data[13][23];
			HBM_channelA13[offsets[13]].data[24] = data[13][24];
			HBM_channelA13[offsets[13]].data[25] = data[13][25];
			HBM_channelA13[offsets[13]].data[26] = data[13][26];
			HBM_channelA13[offsets[13]].data[27] = data[13][27];
			HBM_channelA13[offsets[13]].data[28] = data[13][28];
			HBM_channelA13[offsets[13]].data[29] = data[13][29];
			HBM_channelA13[offsets[13]].data[30] = data[13][30];
			HBM_channelA13[offsets[13]].data[31] = data[13][31];
		}
		if(ens[14]==true){
			HBM_channelA14[offsets[14]].data[0] = data[14][0];
			HBM_channelA14[offsets[14]].data[1] = data[14][1];
			HBM_channelA14[offsets[14]].data[2] = data[14][2];
			HBM_channelA14[offsets[14]].data[3] = data[14][3];
			HBM_channelA14[offsets[14]].data[4] = data[14][4];
			HBM_channelA14[offsets[14]].data[5] = data[14][5];
			HBM_channelA14[offsets[14]].data[6] = data[14][6];
			HBM_channelA14[offsets[14]].data[7] = data[14][7];
			HBM_channelA14[offsets[14]].data[8] = data[14][8];
			HBM_channelA14[offsets[14]].data[9] = data[14][9];
			HBM_channelA14[offsets[14]].data[10] = data[14][10];
			HBM_channelA14[offsets[14]].data[11] = data[14][11];
			HBM_channelA14[offsets[14]].data[12] = data[14][12];
			HBM_channelA14[offsets[14]].data[13] = data[14][13];
			HBM_channelA14[offsets[14]].data[14] = data[14][14];
			HBM_channelA14[offsets[14]].data[15] = data[14][15];
			HBM_channelA14[offsets[14]].data[16] = data[14][16];
			HBM_channelA14[offsets[14]].data[17] = data[14][17];
			HBM_channelA14[offsets[14]].data[18] = data[14][18];
			HBM_channelA14[offsets[14]].data[19] = data[14][19];
			HBM_channelA14[offsets[14]].data[20] = data[14][20];
			HBM_channelA14[offsets[14]].data[21] = data[14][21];
			HBM_channelA14[offsets[14]].data[22] = data[14][22];
			HBM_channelA14[offsets[14]].data[23] = data[14][23];
			HBM_channelA14[offsets[14]].data[24] = data[14][24];
			HBM_channelA14[offsets[14]].data[25] = data[14][25];
			HBM_channelA14[offsets[14]].data[26] = data[14][26];
			HBM_channelA14[offsets[14]].data[27] = data[14][27];
			HBM_channelA14[offsets[14]].data[28] = data[14][28];
			HBM_channelA14[offsets[14]].data[29] = data[14][29];
			HBM_channelA14[offsets[14]].data[30] = data[14][30];
			HBM_channelA14[offsets[14]].data[31] = data[14][31];
		}
		if(ens[15]==true){
			HBM_channelA15[offsets[15]].data[0] = data[15][0];
			HBM_channelA15[offsets[15]].data[1] = data[15][1];
			HBM_channelA15[offsets[15]].data[2] = data[15][2];
			HBM_channelA15[offsets[15]].data[3] = data[15][3];
			HBM_channelA15[offsets[15]].data[4] = data[15][4];
			HBM_channelA15[offsets[15]].data[5] = data[15][5];
			HBM_channelA15[offsets[15]].data[6] = data[15][6];
			HBM_channelA15[offsets[15]].data[7] = data[15][7];
			HBM_channelA15[offsets[15]].data[8] = data[15][8];
			HBM_channelA15[offsets[15]].data[9] = data[15][9];
			HBM_channelA15[offsets[15]].data[10] = data[15][10];
			HBM_channelA15[offsets[15]].data[11] = data[15][11];
			HBM_channelA15[offsets[15]].data[12] = data[15][12];
			HBM_channelA15[offsets[15]].data[13] = data[15][13];
			HBM_channelA15[offsets[15]].data[14] = data[15][14];
			HBM_channelA15[offsets[15]].data[15] = data[15][15];
			HBM_channelA15[offsets[15]].data[16] = data[15][16];
			HBM_channelA15[offsets[15]].data[17] = data[15][17];
			HBM_channelA15[offsets[15]].data[18] = data[15][18];
			HBM_channelA15[offsets[15]].data[19] = data[15][19];
			HBM_channelA15[offsets[15]].data[20] = data[15][20];
			HBM_channelA15[offsets[15]].data[21] = data[15][21];
			HBM_channelA15[offsets[15]].data[22] = data[15][22];
			HBM_channelA15[offsets[15]].data[23] = data[15][23];
			HBM_channelA15[offsets[15]].data[24] = data[15][24];
			HBM_channelA15[offsets[15]].data[25] = data[15][25];
			HBM_channelA15[offsets[15]].data[26] = data[15][26];
			HBM_channelA15[offsets[15]].data[27] = data[15][27];
			HBM_channelA15[offsets[15]].data[28] = data[15][28];
			HBM_channelA15[offsets[15]].data[29] = data[15][29];
			HBM_channelA15[offsets[15]].data[30] = data[15][30];
			HBM_channelA15[offsets[15]].data[31] = data[15][31];
		}
		if(ens[16]==true){
			HBM_channelA16[offsets[16]].data[0] = data[16][0];
			HBM_channelA16[offsets[16]].data[1] = data[16][1];
			HBM_channelA16[offsets[16]].data[2] = data[16][2];
			HBM_channelA16[offsets[16]].data[3] = data[16][3];
			HBM_channelA16[offsets[16]].data[4] = data[16][4];
			HBM_channelA16[offsets[16]].data[5] = data[16][5];
			HBM_channelA16[offsets[16]].data[6] = data[16][6];
			HBM_channelA16[offsets[16]].data[7] = data[16][7];
			HBM_channelA16[offsets[16]].data[8] = data[16][8];
			HBM_channelA16[offsets[16]].data[9] = data[16][9];
			HBM_channelA16[offsets[16]].data[10] = data[16][10];
			HBM_channelA16[offsets[16]].data[11] = data[16][11];
			HBM_channelA16[offsets[16]].data[12] = data[16][12];
			HBM_channelA16[offsets[16]].data[13] = data[16][13];
			HBM_channelA16[offsets[16]].data[14] = data[16][14];
			HBM_channelA16[offsets[16]].data[15] = data[16][15];
			HBM_channelA16[offsets[16]].data[16] = data[16][16];
			HBM_channelA16[offsets[16]].data[17] = data[16][17];
			HBM_channelA16[offsets[16]].data[18] = data[16][18];
			HBM_channelA16[offsets[16]].data[19] = data[16][19];
			HBM_channelA16[offsets[16]].data[20] = data[16][20];
			HBM_channelA16[offsets[16]].data[21] = data[16][21];
			HBM_channelA16[offsets[16]].data[22] = data[16][22];
			HBM_channelA16[offsets[16]].data[23] = data[16][23];
			HBM_channelA16[offsets[16]].data[24] = data[16][24];
			HBM_channelA16[offsets[16]].data[25] = data[16][25];
			HBM_channelA16[offsets[16]].data[26] = data[16][26];
			HBM_channelA16[offsets[16]].data[27] = data[16][27];
			HBM_channelA16[offsets[16]].data[28] = data[16][28];
			HBM_channelA16[offsets[16]].data[29] = data[16][29];
			HBM_channelA16[offsets[16]].data[30] = data[16][30];
			HBM_channelA16[offsets[16]].data[31] = data[16][31];
		}
		if(ens[17]==true){
			HBM_channelA17[offsets[17]].data[0] = data[17][0];
			HBM_channelA17[offsets[17]].data[1] = data[17][1];
			HBM_channelA17[offsets[17]].data[2] = data[17][2];
			HBM_channelA17[offsets[17]].data[3] = data[17][3];
			HBM_channelA17[offsets[17]].data[4] = data[17][4];
			HBM_channelA17[offsets[17]].data[5] = data[17][5];
			HBM_channelA17[offsets[17]].data[6] = data[17][6];
			HBM_channelA17[offsets[17]].data[7] = data[17][7];
			HBM_channelA17[offsets[17]].data[8] = data[17][8];
			HBM_channelA17[offsets[17]].data[9] = data[17][9];
			HBM_channelA17[offsets[17]].data[10] = data[17][10];
			HBM_channelA17[offsets[17]].data[11] = data[17][11];
			HBM_channelA17[offsets[17]].data[12] = data[17][12];
			HBM_channelA17[offsets[17]].data[13] = data[17][13];
			HBM_channelA17[offsets[17]].data[14] = data[17][14];
			HBM_channelA17[offsets[17]].data[15] = data[17][15];
			HBM_channelA17[offsets[17]].data[16] = data[17][16];
			HBM_channelA17[offsets[17]].data[17] = data[17][17];
			HBM_channelA17[offsets[17]].data[18] = data[17][18];
			HBM_channelA17[offsets[17]].data[19] = data[17][19];
			HBM_channelA17[offsets[17]].data[20] = data[17][20];
			HBM_channelA17[offsets[17]].data[21] = data[17][21];
			HBM_channelA17[offsets[17]].data[22] = data[17][22];
			HBM_channelA17[offsets[17]].data[23] = data[17][23];
			HBM_channelA17[offsets[17]].data[24] = data[17][24];
			HBM_channelA17[offsets[17]].data[25] = data[17][25];
			HBM_channelA17[offsets[17]].data[26] = data[17][26];
			HBM_channelA17[offsets[17]].data[27] = data[17][27];
			HBM_channelA17[offsets[17]].data[28] = data[17][28];
			HBM_channelA17[offsets[17]].data[29] = data[17][29];
			HBM_channelA17[offsets[17]].data[30] = data[17][30];
			HBM_channelA17[offsets[17]].data[31] = data[17][31];
		}
		if(ens[18]==true){
			HBM_channelA18[offsets[18]].data[0] = data[18][0];
			HBM_channelA18[offsets[18]].data[1] = data[18][1];
			HBM_channelA18[offsets[18]].data[2] = data[18][2];
			HBM_channelA18[offsets[18]].data[3] = data[18][3];
			HBM_channelA18[offsets[18]].data[4] = data[18][4];
			HBM_channelA18[offsets[18]].data[5] = data[18][5];
			HBM_channelA18[offsets[18]].data[6] = data[18][6];
			HBM_channelA18[offsets[18]].data[7] = data[18][7];
			HBM_channelA18[offsets[18]].data[8] = data[18][8];
			HBM_channelA18[offsets[18]].data[9] = data[18][9];
			HBM_channelA18[offsets[18]].data[10] = data[18][10];
			HBM_channelA18[offsets[18]].data[11] = data[18][11];
			HBM_channelA18[offsets[18]].data[12] = data[18][12];
			HBM_channelA18[offsets[18]].data[13] = data[18][13];
			HBM_channelA18[offsets[18]].data[14] = data[18][14];
			HBM_channelA18[offsets[18]].data[15] = data[18][15];
			HBM_channelA18[offsets[18]].data[16] = data[18][16];
			HBM_channelA18[offsets[18]].data[17] = data[18][17];
			HBM_channelA18[offsets[18]].data[18] = data[18][18];
			HBM_channelA18[offsets[18]].data[19] = data[18][19];
			HBM_channelA18[offsets[18]].data[20] = data[18][20];
			HBM_channelA18[offsets[18]].data[21] = data[18][21];
			HBM_channelA18[offsets[18]].data[22] = data[18][22];
			HBM_channelA18[offsets[18]].data[23] = data[18][23];
			HBM_channelA18[offsets[18]].data[24] = data[18][24];
			HBM_channelA18[offsets[18]].data[25] = data[18][25];
			HBM_channelA18[offsets[18]].data[26] = data[18][26];
			HBM_channelA18[offsets[18]].data[27] = data[18][27];
			HBM_channelA18[offsets[18]].data[28] = data[18][28];
			HBM_channelA18[offsets[18]].data[29] = data[18][29];
			HBM_channelA18[offsets[18]].data[30] = data[18][30];
			HBM_channelA18[offsets[18]].data[31] = data[18][31];
		}
		if(ens[19]==true){
			HBM_channelA19[offsets[19]].data[0] = data[19][0];
			HBM_channelA19[offsets[19]].data[1] = data[19][1];
			HBM_channelA19[offsets[19]].data[2] = data[19][2];
			HBM_channelA19[offsets[19]].data[3] = data[19][3];
			HBM_channelA19[offsets[19]].data[4] = data[19][4];
			HBM_channelA19[offsets[19]].data[5] = data[19][5];
			HBM_channelA19[offsets[19]].data[6] = data[19][6];
			HBM_channelA19[offsets[19]].data[7] = data[19][7];
			HBM_channelA19[offsets[19]].data[8] = data[19][8];
			HBM_channelA19[offsets[19]].data[9] = data[19][9];
			HBM_channelA19[offsets[19]].data[10] = data[19][10];
			HBM_channelA19[offsets[19]].data[11] = data[19][11];
			HBM_channelA19[offsets[19]].data[12] = data[19][12];
			HBM_channelA19[offsets[19]].data[13] = data[19][13];
			HBM_channelA19[offsets[19]].data[14] = data[19][14];
			HBM_channelA19[offsets[19]].data[15] = data[19][15];
			HBM_channelA19[offsets[19]].data[16] = data[19][16];
			HBM_channelA19[offsets[19]].data[17] = data[19][17];
			HBM_channelA19[offsets[19]].data[18] = data[19][18];
			HBM_channelA19[offsets[19]].data[19] = data[19][19];
			HBM_channelA19[offsets[19]].data[20] = data[19][20];
			HBM_channelA19[offsets[19]].data[21] = data[19][21];
			HBM_channelA19[offsets[19]].data[22] = data[19][22];
			HBM_channelA19[offsets[19]].data[23] = data[19][23];
			HBM_channelA19[offsets[19]].data[24] = data[19][24];
			HBM_channelA19[offsets[19]].data[25] = data[19][25];
			HBM_channelA19[offsets[19]].data[26] = data[19][26];
			HBM_channelA19[offsets[19]].data[27] = data[19][27];
			HBM_channelA19[offsets[19]].data[28] = data[19][28];
			HBM_channelA19[offsets[19]].data[29] = data[19][29];
			HBM_channelA19[offsets[19]].data[30] = data[19][30];
			HBM_channelA19[offsets[19]].data[31] = data[19][31];
		}
		if(ens[20]==true){
			HBM_channelA20[offsets[20]].data[0] = data[20][0];
			HBM_channelA20[offsets[20]].data[1] = data[20][1];
			HBM_channelA20[offsets[20]].data[2] = data[20][2];
			HBM_channelA20[offsets[20]].data[3] = data[20][3];
			HBM_channelA20[offsets[20]].data[4] = data[20][4];
			HBM_channelA20[offsets[20]].data[5] = data[20][5];
			HBM_channelA20[offsets[20]].data[6] = data[20][6];
			HBM_channelA20[offsets[20]].data[7] = data[20][7];
			HBM_channelA20[offsets[20]].data[8] = data[20][8];
			HBM_channelA20[offsets[20]].data[9] = data[20][9];
			HBM_channelA20[offsets[20]].data[10] = data[20][10];
			HBM_channelA20[offsets[20]].data[11] = data[20][11];
			HBM_channelA20[offsets[20]].data[12] = data[20][12];
			HBM_channelA20[offsets[20]].data[13] = data[20][13];
			HBM_channelA20[offsets[20]].data[14] = data[20][14];
			HBM_channelA20[offsets[20]].data[15] = data[20][15];
			HBM_channelA20[offsets[20]].data[16] = data[20][16];
			HBM_channelA20[offsets[20]].data[17] = data[20][17];
			HBM_channelA20[offsets[20]].data[18] = data[20][18];
			HBM_channelA20[offsets[20]].data[19] = data[20][19];
			HBM_channelA20[offsets[20]].data[20] = data[20][20];
			HBM_channelA20[offsets[20]].data[21] = data[20][21];
			HBM_channelA20[offsets[20]].data[22] = data[20][22];
			HBM_channelA20[offsets[20]].data[23] = data[20][23];
			HBM_channelA20[offsets[20]].data[24] = data[20][24];
			HBM_channelA20[offsets[20]].data[25] = data[20][25];
			HBM_channelA20[offsets[20]].data[26] = data[20][26];
			HBM_channelA20[offsets[20]].data[27] = data[20][27];
			HBM_channelA20[offsets[20]].data[28] = data[20][28];
			HBM_channelA20[offsets[20]].data[29] = data[20][29];
			HBM_channelA20[offsets[20]].data[30] = data[20][30];
			HBM_channelA20[offsets[20]].data[31] = data[20][31];
		}
		if(ens[21]==true){
			HBM_channelA21[offsets[21]].data[0] = data[21][0];
			HBM_channelA21[offsets[21]].data[1] = data[21][1];
			HBM_channelA21[offsets[21]].data[2] = data[21][2];
			HBM_channelA21[offsets[21]].data[3] = data[21][3];
			HBM_channelA21[offsets[21]].data[4] = data[21][4];
			HBM_channelA21[offsets[21]].data[5] = data[21][5];
			HBM_channelA21[offsets[21]].data[6] = data[21][6];
			HBM_channelA21[offsets[21]].data[7] = data[21][7];
			HBM_channelA21[offsets[21]].data[8] = data[21][8];
			HBM_channelA21[offsets[21]].data[9] = data[21][9];
			HBM_channelA21[offsets[21]].data[10] = data[21][10];
			HBM_channelA21[offsets[21]].data[11] = data[21][11];
			HBM_channelA21[offsets[21]].data[12] = data[21][12];
			HBM_channelA21[offsets[21]].data[13] = data[21][13];
			HBM_channelA21[offsets[21]].data[14] = data[21][14];
			HBM_channelA21[offsets[21]].data[15] = data[21][15];
			HBM_channelA21[offsets[21]].data[16] = data[21][16];
			HBM_channelA21[offsets[21]].data[17] = data[21][17];
			HBM_channelA21[offsets[21]].data[18] = data[21][18];
			HBM_channelA21[offsets[21]].data[19] = data[21][19];
			HBM_channelA21[offsets[21]].data[20] = data[21][20];
			HBM_channelA21[offsets[21]].data[21] = data[21][21];
			HBM_channelA21[offsets[21]].data[22] = data[21][22];
			HBM_channelA21[offsets[21]].data[23] = data[21][23];
			HBM_channelA21[offsets[21]].data[24] = data[21][24];
			HBM_channelA21[offsets[21]].data[25] = data[21][25];
			HBM_channelA21[offsets[21]].data[26] = data[21][26];
			HBM_channelA21[offsets[21]].data[27] = data[21][27];
			HBM_channelA21[offsets[21]].data[28] = data[21][28];
			HBM_channelA21[offsets[21]].data[29] = data[21][29];
			HBM_channelA21[offsets[21]].data[30] = data[21][30];
			HBM_channelA21[offsets[21]].data[31] = data[21][31];
		}
		if(ens[22]==true){
			HBM_channelA22[offsets[22]].data[0] = data[22][0];
			HBM_channelA22[offsets[22]].data[1] = data[22][1];
			HBM_channelA22[offsets[22]].data[2] = data[22][2];
			HBM_channelA22[offsets[22]].data[3] = data[22][3];
			HBM_channelA22[offsets[22]].data[4] = data[22][4];
			HBM_channelA22[offsets[22]].data[5] = data[22][5];
			HBM_channelA22[offsets[22]].data[6] = data[22][6];
			HBM_channelA22[offsets[22]].data[7] = data[22][7];
			HBM_channelA22[offsets[22]].data[8] = data[22][8];
			HBM_channelA22[offsets[22]].data[9] = data[22][9];
			HBM_channelA22[offsets[22]].data[10] = data[22][10];
			HBM_channelA22[offsets[22]].data[11] = data[22][11];
			HBM_channelA22[offsets[22]].data[12] = data[22][12];
			HBM_channelA22[offsets[22]].data[13] = data[22][13];
			HBM_channelA22[offsets[22]].data[14] = data[22][14];
			HBM_channelA22[offsets[22]].data[15] = data[22][15];
			HBM_channelA22[offsets[22]].data[16] = data[22][16];
			HBM_channelA22[offsets[22]].data[17] = data[22][17];
			HBM_channelA22[offsets[22]].data[18] = data[22][18];
			HBM_channelA22[offsets[22]].data[19] = data[22][19];
			HBM_channelA22[offsets[22]].data[20] = data[22][20];
			HBM_channelA22[offsets[22]].data[21] = data[22][21];
			HBM_channelA22[offsets[22]].data[22] = data[22][22];
			HBM_channelA22[offsets[22]].data[23] = data[22][23];
			HBM_channelA22[offsets[22]].data[24] = data[22][24];
			HBM_channelA22[offsets[22]].data[25] = data[22][25];
			HBM_channelA22[offsets[22]].data[26] = data[22][26];
			HBM_channelA22[offsets[22]].data[27] = data[22][27];
			HBM_channelA22[offsets[22]].data[28] = data[22][28];
			HBM_channelA22[offsets[22]].data[29] = data[22][29];
			HBM_channelA22[offsets[22]].data[30] = data[22][30];
			HBM_channelA22[offsets[22]].data[31] = data[22][31];
		}
		if(ens[23]==true){
			HBM_channelA23[offsets[23]].data[0] = data[23][0];
			HBM_channelA23[offsets[23]].data[1] = data[23][1];
			HBM_channelA23[offsets[23]].data[2] = data[23][2];
			HBM_channelA23[offsets[23]].data[3] = data[23][3];
			HBM_channelA23[offsets[23]].data[4] = data[23][4];
			HBM_channelA23[offsets[23]].data[5] = data[23][5];
			HBM_channelA23[offsets[23]].data[6] = data[23][6];
			HBM_channelA23[offsets[23]].data[7] = data[23][7];
			HBM_channelA23[offsets[23]].data[8] = data[23][8];
			HBM_channelA23[offsets[23]].data[9] = data[23][9];
			HBM_channelA23[offsets[23]].data[10] = data[23][10];
			HBM_channelA23[offsets[23]].data[11] = data[23][11];
			HBM_channelA23[offsets[23]].data[12] = data[23][12];
			HBM_channelA23[offsets[23]].data[13] = data[23][13];
			HBM_channelA23[offsets[23]].data[14] = data[23][14];
			HBM_channelA23[offsets[23]].data[15] = data[23][15];
			HBM_channelA23[offsets[23]].data[16] = data[23][16];
			HBM_channelA23[offsets[23]].data[17] = data[23][17];
			HBM_channelA23[offsets[23]].data[18] = data[23][18];
			HBM_channelA23[offsets[23]].data[19] = data[23][19];
			HBM_channelA23[offsets[23]].data[20] = data[23][20];
			HBM_channelA23[offsets[23]].data[21] = data[23][21];
			HBM_channelA23[offsets[23]].data[22] = data[23][22];
			HBM_channelA23[offsets[23]].data[23] = data[23][23];
			HBM_channelA23[offsets[23]].data[24] = data[23][24];
			HBM_channelA23[offsets[23]].data[25] = data[23][25];
			HBM_channelA23[offsets[23]].data[26] = data[23][26];
			HBM_channelA23[offsets[23]].data[27] = data[23][27];
			HBM_channelA23[offsets[23]].data[28] = data[23][28];
			HBM_channelA23[offsets[23]].data[29] = data[23][29];
			HBM_channelA23[offsets[23]].data[30] = data[23][30];
			HBM_channelA23[offsets[23]].data[31] = data[23][31];
		}
	
		#endif 
	#endif 
	return;
}
void master_retrievemany_vec(unsigned int offsets[NUM_PEs], unsigned int data[NUM_PEs][HBM_CHANNEL_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
	#ifdef FPGA_IMPL 
		data[0][0] = HBM_channelA0[offsets[0]].range(31, 0);
		data[0][1] = HBM_channelA0[offsets[0]].range(63, 32);
		data[0][2] = HBM_channelA0[offsets[0]].range(95, 64);
		data[0][3] = HBM_channelA0[offsets[0]].range(127, 96);
		data[0][4] = HBM_channelA0[offsets[0]].range(159, 128);
		data[0][5] = HBM_channelA0[offsets[0]].range(191, 160);
		data[0][6] = HBM_channelA0[offsets[0]].range(223, 192);
		data[0][7] = HBM_channelA0[offsets[0]].range(255, 224);
		data[0][8] = HBM_channelA0[offsets[0]].range(287, 256);
		data[0][9] = HBM_channelA0[offsets[0]].range(319, 288);
		data[0][10] = HBM_channelA0[offsets[0]].range(351, 320);
		data[0][11] = HBM_channelA0[offsets[0]].range(383, 352);
		data[0][12] = HBM_channelA0[offsets[0]].range(415, 384);
		data[0][13] = HBM_channelA0[offsets[0]].range(447, 416);
		data[0][14] = HBM_channelA0[offsets[0]].range(479, 448);
		data[0][15] = HBM_channelA0[offsets[0]].range(511, 480);
		data[0][16] = HBM_channelB0[offsets[0]].range(31, 0);
		data[0][17] = HBM_channelB0[offsets[0]].range(63, 32);
		data[0][18] = HBM_channelB0[offsets[0]].range(95, 64);
		data[0][19] = HBM_channelB0[offsets[0]].range(127, 96);
		data[0][20] = HBM_channelB0[offsets[0]].range(159, 128);
		data[0][21] = HBM_channelB0[offsets[0]].range(191, 160);
		data[0][22] = HBM_channelB0[offsets[0]].range(223, 192);
		data[0][23] = HBM_channelB0[offsets[0]].range(255, 224);
		data[0][24] = HBM_channelB0[offsets[0]].range(287, 256);
		data[0][25] = HBM_channelB0[offsets[0]].range(319, 288);
		data[0][26] = HBM_channelB0[offsets[0]].range(351, 320);
		data[0][27] = HBM_channelB0[offsets[0]].range(383, 352);
		data[0][28] = HBM_channelB0[offsets[0]].range(415, 384);
		data[0][29] = HBM_channelB0[offsets[0]].range(447, 416);
		data[0][30] = HBM_channelB0[offsets[0]].range(479, 448);
		data[0][31] = HBM_channelB0[offsets[0]].range(511, 480);
		data[1][0] = HBM_channelA1[offsets[1]].range(31, 0);
		data[1][1] = HBM_channelA1[offsets[1]].range(63, 32);
		data[1][2] = HBM_channelA1[offsets[1]].range(95, 64);
		data[1][3] = HBM_channelA1[offsets[1]].range(127, 96);
		data[1][4] = HBM_channelA1[offsets[1]].range(159, 128);
		data[1][5] = HBM_channelA1[offsets[1]].range(191, 160);
		data[1][6] = HBM_channelA1[offsets[1]].range(223, 192);
		data[1][7] = HBM_channelA1[offsets[1]].range(255, 224);
		data[1][8] = HBM_channelA1[offsets[1]].range(287, 256);
		data[1][9] = HBM_channelA1[offsets[1]].range(319, 288);
		data[1][10] = HBM_channelA1[offsets[1]].range(351, 320);
		data[1][11] = HBM_channelA1[offsets[1]].range(383, 352);
		data[1][12] = HBM_channelA1[offsets[1]].range(415, 384);
		data[1][13] = HBM_channelA1[offsets[1]].range(447, 416);
		data[1][14] = HBM_channelA1[offsets[1]].range(479, 448);
		data[1][15] = HBM_channelA1[offsets[1]].range(511, 480);
		data[1][16] = HBM_channelB1[offsets[1]].range(31, 0);
		data[1][17] = HBM_channelB1[offsets[1]].range(63, 32);
		data[1][18] = HBM_channelB1[offsets[1]].range(95, 64);
		data[1][19] = HBM_channelB1[offsets[1]].range(127, 96);
		data[1][20] = HBM_channelB1[offsets[1]].range(159, 128);
		data[1][21] = HBM_channelB1[offsets[1]].range(191, 160);
		data[1][22] = HBM_channelB1[offsets[1]].range(223, 192);
		data[1][23] = HBM_channelB1[offsets[1]].range(255, 224);
		data[1][24] = HBM_channelB1[offsets[1]].range(287, 256);
		data[1][25] = HBM_channelB1[offsets[1]].range(319, 288);
		data[1][26] = HBM_channelB1[offsets[1]].range(351, 320);
		data[1][27] = HBM_channelB1[offsets[1]].range(383, 352);
		data[1][28] = HBM_channelB1[offsets[1]].range(415, 384);
		data[1][29] = HBM_channelB1[offsets[1]].range(447, 416);
		data[1][30] = HBM_channelB1[offsets[1]].range(479, 448);
		data[1][31] = HBM_channelB1[offsets[1]].range(511, 480);
		data[2][0] = HBM_channelA2[offsets[2]].range(31, 0);
		data[2][1] = HBM_channelA2[offsets[2]].range(63, 32);
		data[2][2] = HBM_channelA2[offsets[2]].range(95, 64);
		data[2][3] = HBM_channelA2[offsets[2]].range(127, 96);
		data[2][4] = HBM_channelA2[offsets[2]].range(159, 128);
		data[2][5] = HBM_channelA2[offsets[2]].range(191, 160);
		data[2][6] = HBM_channelA2[offsets[2]].range(223, 192);
		data[2][7] = HBM_channelA2[offsets[2]].range(255, 224);
		data[2][8] = HBM_channelA2[offsets[2]].range(287, 256);
		data[2][9] = HBM_channelA2[offsets[2]].range(319, 288);
		data[2][10] = HBM_channelA2[offsets[2]].range(351, 320);
		data[2][11] = HBM_channelA2[offsets[2]].range(383, 352);
		data[2][12] = HBM_channelA2[offsets[2]].range(415, 384);
		data[2][13] = HBM_channelA2[offsets[2]].range(447, 416);
		data[2][14] = HBM_channelA2[offsets[2]].range(479, 448);
		data[2][15] = HBM_channelA2[offsets[2]].range(511, 480);
		data[2][16] = HBM_channelB2[offsets[2]].range(31, 0);
		data[2][17] = HBM_channelB2[offsets[2]].range(63, 32);
		data[2][18] = HBM_channelB2[offsets[2]].range(95, 64);
		data[2][19] = HBM_channelB2[offsets[2]].range(127, 96);
		data[2][20] = HBM_channelB2[offsets[2]].range(159, 128);
		data[2][21] = HBM_channelB2[offsets[2]].range(191, 160);
		data[2][22] = HBM_channelB2[offsets[2]].range(223, 192);
		data[2][23] = HBM_channelB2[offsets[2]].range(255, 224);
		data[2][24] = HBM_channelB2[offsets[2]].range(287, 256);
		data[2][25] = HBM_channelB2[offsets[2]].range(319, 288);
		data[2][26] = HBM_channelB2[offsets[2]].range(351, 320);
		data[2][27] = HBM_channelB2[offsets[2]].range(383, 352);
		data[2][28] = HBM_channelB2[offsets[2]].range(415, 384);
		data[2][29] = HBM_channelB2[offsets[2]].range(447, 416);
		data[2][30] = HBM_channelB2[offsets[2]].range(479, 448);
		data[2][31] = HBM_channelB2[offsets[2]].range(511, 480);
		data[3][0] = HBM_channelA3[offsets[3]].range(31, 0);
		data[3][1] = HBM_channelA3[offsets[3]].range(63, 32);
		data[3][2] = HBM_channelA3[offsets[3]].range(95, 64);
		data[3][3] = HBM_channelA3[offsets[3]].range(127, 96);
		data[3][4] = HBM_channelA3[offsets[3]].range(159, 128);
		data[3][5] = HBM_channelA3[offsets[3]].range(191, 160);
		data[3][6] = HBM_channelA3[offsets[3]].range(223, 192);
		data[3][7] = HBM_channelA3[offsets[3]].range(255, 224);
		data[3][8] = HBM_channelA3[offsets[3]].range(287, 256);
		data[3][9] = HBM_channelA3[offsets[3]].range(319, 288);
		data[3][10] = HBM_channelA3[offsets[3]].range(351, 320);
		data[3][11] = HBM_channelA3[offsets[3]].range(383, 352);
		data[3][12] = HBM_channelA3[offsets[3]].range(415, 384);
		data[3][13] = HBM_channelA3[offsets[3]].range(447, 416);
		data[3][14] = HBM_channelA3[offsets[3]].range(479, 448);
		data[3][15] = HBM_channelA3[offsets[3]].range(511, 480);
		data[3][16] = HBM_channelB3[offsets[3]].range(31, 0);
		data[3][17] = HBM_channelB3[offsets[3]].range(63, 32);
		data[3][18] = HBM_channelB3[offsets[3]].range(95, 64);
		data[3][19] = HBM_channelB3[offsets[3]].range(127, 96);
		data[3][20] = HBM_channelB3[offsets[3]].range(159, 128);
		data[3][21] = HBM_channelB3[offsets[3]].range(191, 160);
		data[3][22] = HBM_channelB3[offsets[3]].range(223, 192);
		data[3][23] = HBM_channelB3[offsets[3]].range(255, 224);
		data[3][24] = HBM_channelB3[offsets[3]].range(287, 256);
		data[3][25] = HBM_channelB3[offsets[3]].range(319, 288);
		data[3][26] = HBM_channelB3[offsets[3]].range(351, 320);
		data[3][27] = HBM_channelB3[offsets[3]].range(383, 352);
		data[3][28] = HBM_channelB3[offsets[3]].range(415, 384);
		data[3][29] = HBM_channelB3[offsets[3]].range(447, 416);
		data[3][30] = HBM_channelB3[offsets[3]].range(479, 448);
		data[3][31] = HBM_channelB3[offsets[3]].range(511, 480);
		data[4][0] = HBM_channelA4[offsets[4]].range(31, 0);
		data[4][1] = HBM_channelA4[offsets[4]].range(63, 32);
		data[4][2] = HBM_channelA4[offsets[4]].range(95, 64);
		data[4][3] = HBM_channelA4[offsets[4]].range(127, 96);
		data[4][4] = HBM_channelA4[offsets[4]].range(159, 128);
		data[4][5] = HBM_channelA4[offsets[4]].range(191, 160);
		data[4][6] = HBM_channelA4[offsets[4]].range(223, 192);
		data[4][7] = HBM_channelA4[offsets[4]].range(255, 224);
		data[4][8] = HBM_channelA4[offsets[4]].range(287, 256);
		data[4][9] = HBM_channelA4[offsets[4]].range(319, 288);
		data[4][10] = HBM_channelA4[offsets[4]].range(351, 320);
		data[4][11] = HBM_channelA4[offsets[4]].range(383, 352);
		data[4][12] = HBM_channelA4[offsets[4]].range(415, 384);
		data[4][13] = HBM_channelA4[offsets[4]].range(447, 416);
		data[4][14] = HBM_channelA4[offsets[4]].range(479, 448);
		data[4][15] = HBM_channelA4[offsets[4]].range(511, 480);
		data[4][16] = HBM_channelB4[offsets[4]].range(31, 0);
		data[4][17] = HBM_channelB4[offsets[4]].range(63, 32);
		data[4][18] = HBM_channelB4[offsets[4]].range(95, 64);
		data[4][19] = HBM_channelB4[offsets[4]].range(127, 96);
		data[4][20] = HBM_channelB4[offsets[4]].range(159, 128);
		data[4][21] = HBM_channelB4[offsets[4]].range(191, 160);
		data[4][22] = HBM_channelB4[offsets[4]].range(223, 192);
		data[4][23] = HBM_channelB4[offsets[4]].range(255, 224);
		data[4][24] = HBM_channelB4[offsets[4]].range(287, 256);
		data[4][25] = HBM_channelB4[offsets[4]].range(319, 288);
		data[4][26] = HBM_channelB4[offsets[4]].range(351, 320);
		data[4][27] = HBM_channelB4[offsets[4]].range(383, 352);
		data[4][28] = HBM_channelB4[offsets[4]].range(415, 384);
		data[4][29] = HBM_channelB4[offsets[4]].range(447, 416);
		data[4][30] = HBM_channelB4[offsets[4]].range(479, 448);
		data[4][31] = HBM_channelB4[offsets[4]].range(511, 480);
		data[5][0] = HBM_channelA5[offsets[5]].range(31, 0);
		data[5][1] = HBM_channelA5[offsets[5]].range(63, 32);
		data[5][2] = HBM_channelA5[offsets[5]].range(95, 64);
		data[5][3] = HBM_channelA5[offsets[5]].range(127, 96);
		data[5][4] = HBM_channelA5[offsets[5]].range(159, 128);
		data[5][5] = HBM_channelA5[offsets[5]].range(191, 160);
		data[5][6] = HBM_channelA5[offsets[5]].range(223, 192);
		data[5][7] = HBM_channelA5[offsets[5]].range(255, 224);
		data[5][8] = HBM_channelA5[offsets[5]].range(287, 256);
		data[5][9] = HBM_channelA5[offsets[5]].range(319, 288);
		data[5][10] = HBM_channelA5[offsets[5]].range(351, 320);
		data[5][11] = HBM_channelA5[offsets[5]].range(383, 352);
		data[5][12] = HBM_channelA5[offsets[5]].range(415, 384);
		data[5][13] = HBM_channelA5[offsets[5]].range(447, 416);
		data[5][14] = HBM_channelA5[offsets[5]].range(479, 448);
		data[5][15] = HBM_channelA5[offsets[5]].range(511, 480);
		data[5][16] = HBM_channelB5[offsets[5]].range(31, 0);
		data[5][17] = HBM_channelB5[offsets[5]].range(63, 32);
		data[5][18] = HBM_channelB5[offsets[5]].range(95, 64);
		data[5][19] = HBM_channelB5[offsets[5]].range(127, 96);
		data[5][20] = HBM_channelB5[offsets[5]].range(159, 128);
		data[5][21] = HBM_channelB5[offsets[5]].range(191, 160);
		data[5][22] = HBM_channelB5[offsets[5]].range(223, 192);
		data[5][23] = HBM_channelB5[offsets[5]].range(255, 224);
		data[5][24] = HBM_channelB5[offsets[5]].range(287, 256);
		data[5][25] = HBM_channelB5[offsets[5]].range(319, 288);
		data[5][26] = HBM_channelB5[offsets[5]].range(351, 320);
		data[5][27] = HBM_channelB5[offsets[5]].range(383, 352);
		data[5][28] = HBM_channelB5[offsets[5]].range(415, 384);
		data[5][29] = HBM_channelB5[offsets[5]].range(447, 416);
		data[5][30] = HBM_channelB5[offsets[5]].range(479, 448);
		data[5][31] = HBM_channelB5[offsets[5]].range(511, 480);
		data[6][0] = HBM_channelA6[offsets[6]].range(31, 0);
		data[6][1] = HBM_channelA6[offsets[6]].range(63, 32);
		data[6][2] = HBM_channelA6[offsets[6]].range(95, 64);
		data[6][3] = HBM_channelA6[offsets[6]].range(127, 96);
		data[6][4] = HBM_channelA6[offsets[6]].range(159, 128);
		data[6][5] = HBM_channelA6[offsets[6]].range(191, 160);
		data[6][6] = HBM_channelA6[offsets[6]].range(223, 192);
		data[6][7] = HBM_channelA6[offsets[6]].range(255, 224);
		data[6][8] = HBM_channelA6[offsets[6]].range(287, 256);
		data[6][9] = HBM_channelA6[offsets[6]].range(319, 288);
		data[6][10] = HBM_channelA6[offsets[6]].range(351, 320);
		data[6][11] = HBM_channelA6[offsets[6]].range(383, 352);
		data[6][12] = HBM_channelA6[offsets[6]].range(415, 384);
		data[6][13] = HBM_channelA6[offsets[6]].range(447, 416);
		data[6][14] = HBM_channelA6[offsets[6]].range(479, 448);
		data[6][15] = HBM_channelA6[offsets[6]].range(511, 480);
		data[6][16] = HBM_channelB6[offsets[6]].range(31, 0);
		data[6][17] = HBM_channelB6[offsets[6]].range(63, 32);
		data[6][18] = HBM_channelB6[offsets[6]].range(95, 64);
		data[6][19] = HBM_channelB6[offsets[6]].range(127, 96);
		data[6][20] = HBM_channelB6[offsets[6]].range(159, 128);
		data[6][21] = HBM_channelB6[offsets[6]].range(191, 160);
		data[6][22] = HBM_channelB6[offsets[6]].range(223, 192);
		data[6][23] = HBM_channelB6[offsets[6]].range(255, 224);
		data[6][24] = HBM_channelB6[offsets[6]].range(287, 256);
		data[6][25] = HBM_channelB6[offsets[6]].range(319, 288);
		data[6][26] = HBM_channelB6[offsets[6]].range(351, 320);
		data[6][27] = HBM_channelB6[offsets[6]].range(383, 352);
		data[6][28] = HBM_channelB6[offsets[6]].range(415, 384);
		data[6][29] = HBM_channelB6[offsets[6]].range(447, 416);
		data[6][30] = HBM_channelB6[offsets[6]].range(479, 448);
		data[6][31] = HBM_channelB6[offsets[6]].range(511, 480);
		data[7][0] = HBM_channelA7[offsets[7]].range(31, 0);
		data[7][1] = HBM_channelA7[offsets[7]].range(63, 32);
		data[7][2] = HBM_channelA7[offsets[7]].range(95, 64);
		data[7][3] = HBM_channelA7[offsets[7]].range(127, 96);
		data[7][4] = HBM_channelA7[offsets[7]].range(159, 128);
		data[7][5] = HBM_channelA7[offsets[7]].range(191, 160);
		data[7][6] = HBM_channelA7[offsets[7]].range(223, 192);
		data[7][7] = HBM_channelA7[offsets[7]].range(255, 224);
		data[7][8] = HBM_channelA7[offsets[7]].range(287, 256);
		data[7][9] = HBM_channelA7[offsets[7]].range(319, 288);
		data[7][10] = HBM_channelA7[offsets[7]].range(351, 320);
		data[7][11] = HBM_channelA7[offsets[7]].range(383, 352);
		data[7][12] = HBM_channelA7[offsets[7]].range(415, 384);
		data[7][13] = HBM_channelA7[offsets[7]].range(447, 416);
		data[7][14] = HBM_channelA7[offsets[7]].range(479, 448);
		data[7][15] = HBM_channelA7[offsets[7]].range(511, 480);
		data[7][16] = HBM_channelB7[offsets[7]].range(31, 0);
		data[7][17] = HBM_channelB7[offsets[7]].range(63, 32);
		data[7][18] = HBM_channelB7[offsets[7]].range(95, 64);
		data[7][19] = HBM_channelB7[offsets[7]].range(127, 96);
		data[7][20] = HBM_channelB7[offsets[7]].range(159, 128);
		data[7][21] = HBM_channelB7[offsets[7]].range(191, 160);
		data[7][22] = HBM_channelB7[offsets[7]].range(223, 192);
		data[7][23] = HBM_channelB7[offsets[7]].range(255, 224);
		data[7][24] = HBM_channelB7[offsets[7]].range(287, 256);
		data[7][25] = HBM_channelB7[offsets[7]].range(319, 288);
		data[7][26] = HBM_channelB7[offsets[7]].range(351, 320);
		data[7][27] = HBM_channelB7[offsets[7]].range(383, 352);
		data[7][28] = HBM_channelB7[offsets[7]].range(415, 384);
		data[7][29] = HBM_channelB7[offsets[7]].range(447, 416);
		data[7][30] = HBM_channelB7[offsets[7]].range(479, 448);
		data[7][31] = HBM_channelB7[offsets[7]].range(511, 480);
		data[8][0] = HBM_channelA8[offsets[8]].range(31, 0);
		data[8][1] = HBM_channelA8[offsets[8]].range(63, 32);
		data[8][2] = HBM_channelA8[offsets[8]].range(95, 64);
		data[8][3] = HBM_channelA8[offsets[8]].range(127, 96);
		data[8][4] = HBM_channelA8[offsets[8]].range(159, 128);
		data[8][5] = HBM_channelA8[offsets[8]].range(191, 160);
		data[8][6] = HBM_channelA8[offsets[8]].range(223, 192);
		data[8][7] = HBM_channelA8[offsets[8]].range(255, 224);
		data[8][8] = HBM_channelA8[offsets[8]].range(287, 256);
		data[8][9] = HBM_channelA8[offsets[8]].range(319, 288);
		data[8][10] = HBM_channelA8[offsets[8]].range(351, 320);
		data[8][11] = HBM_channelA8[offsets[8]].range(383, 352);
		data[8][12] = HBM_channelA8[offsets[8]].range(415, 384);
		data[8][13] = HBM_channelA8[offsets[8]].range(447, 416);
		data[8][14] = HBM_channelA8[offsets[8]].range(479, 448);
		data[8][15] = HBM_channelA8[offsets[8]].range(511, 480);
		data[8][16] = HBM_channelB8[offsets[8]].range(31, 0);
		data[8][17] = HBM_channelB8[offsets[8]].range(63, 32);
		data[8][18] = HBM_channelB8[offsets[8]].range(95, 64);
		data[8][19] = HBM_channelB8[offsets[8]].range(127, 96);
		data[8][20] = HBM_channelB8[offsets[8]].range(159, 128);
		data[8][21] = HBM_channelB8[offsets[8]].range(191, 160);
		data[8][22] = HBM_channelB8[offsets[8]].range(223, 192);
		data[8][23] = HBM_channelB8[offsets[8]].range(255, 224);
		data[8][24] = HBM_channelB8[offsets[8]].range(287, 256);
		data[8][25] = HBM_channelB8[offsets[8]].range(319, 288);
		data[8][26] = HBM_channelB8[offsets[8]].range(351, 320);
		data[8][27] = HBM_channelB8[offsets[8]].range(383, 352);
		data[8][28] = HBM_channelB8[offsets[8]].range(415, 384);
		data[8][29] = HBM_channelB8[offsets[8]].range(447, 416);
		data[8][30] = HBM_channelB8[offsets[8]].range(479, 448);
		data[8][31] = HBM_channelB8[offsets[8]].range(511, 480);
		data[9][0] = HBM_channelA9[offsets[9]].range(31, 0);
		data[9][1] = HBM_channelA9[offsets[9]].range(63, 32);
		data[9][2] = HBM_channelA9[offsets[9]].range(95, 64);
		data[9][3] = HBM_channelA9[offsets[9]].range(127, 96);
		data[9][4] = HBM_channelA9[offsets[9]].range(159, 128);
		data[9][5] = HBM_channelA9[offsets[9]].range(191, 160);
		data[9][6] = HBM_channelA9[offsets[9]].range(223, 192);
		data[9][7] = HBM_channelA9[offsets[9]].range(255, 224);
		data[9][8] = HBM_channelA9[offsets[9]].range(287, 256);
		data[9][9] = HBM_channelA9[offsets[9]].range(319, 288);
		data[9][10] = HBM_channelA9[offsets[9]].range(351, 320);
		data[9][11] = HBM_channelA9[offsets[9]].range(383, 352);
		data[9][12] = HBM_channelA9[offsets[9]].range(415, 384);
		data[9][13] = HBM_channelA9[offsets[9]].range(447, 416);
		data[9][14] = HBM_channelA9[offsets[9]].range(479, 448);
		data[9][15] = HBM_channelA9[offsets[9]].range(511, 480);
		data[9][16] = HBM_channelB9[offsets[9]].range(31, 0);
		data[9][17] = HBM_channelB9[offsets[9]].range(63, 32);
		data[9][18] = HBM_channelB9[offsets[9]].range(95, 64);
		data[9][19] = HBM_channelB9[offsets[9]].range(127, 96);
		data[9][20] = HBM_channelB9[offsets[9]].range(159, 128);
		data[9][21] = HBM_channelB9[offsets[9]].range(191, 160);
		data[9][22] = HBM_channelB9[offsets[9]].range(223, 192);
		data[9][23] = HBM_channelB9[offsets[9]].range(255, 224);
		data[9][24] = HBM_channelB9[offsets[9]].range(287, 256);
		data[9][25] = HBM_channelB9[offsets[9]].range(319, 288);
		data[9][26] = HBM_channelB9[offsets[9]].range(351, 320);
		data[9][27] = HBM_channelB9[offsets[9]].range(383, 352);
		data[9][28] = HBM_channelB9[offsets[9]].range(415, 384);
		data[9][29] = HBM_channelB9[offsets[9]].range(447, 416);
		data[9][30] = HBM_channelB9[offsets[9]].range(479, 448);
		data[9][31] = HBM_channelB9[offsets[9]].range(511, 480);
		data[10][0] = HBM_channelA10[offsets[10]].range(31, 0);
		data[10][1] = HBM_channelA10[offsets[10]].range(63, 32);
		data[10][2] = HBM_channelA10[offsets[10]].range(95, 64);
		data[10][3] = HBM_channelA10[offsets[10]].range(127, 96);
		data[10][4] = HBM_channelA10[offsets[10]].range(159, 128);
		data[10][5] = HBM_channelA10[offsets[10]].range(191, 160);
		data[10][6] = HBM_channelA10[offsets[10]].range(223, 192);
		data[10][7] = HBM_channelA10[offsets[10]].range(255, 224);
		data[10][8] = HBM_channelA10[offsets[10]].range(287, 256);
		data[10][9] = HBM_channelA10[offsets[10]].range(319, 288);
		data[10][10] = HBM_channelA10[offsets[10]].range(351, 320);
		data[10][11] = HBM_channelA10[offsets[10]].range(383, 352);
		data[10][12] = HBM_channelA10[offsets[10]].range(415, 384);
		data[10][13] = HBM_channelA10[offsets[10]].range(447, 416);
		data[10][14] = HBM_channelA10[offsets[10]].range(479, 448);
		data[10][15] = HBM_channelA10[offsets[10]].range(511, 480);
		data[10][16] = HBM_channelB10[offsets[10]].range(31, 0);
		data[10][17] = HBM_channelB10[offsets[10]].range(63, 32);
		data[10][18] = HBM_channelB10[offsets[10]].range(95, 64);
		data[10][19] = HBM_channelB10[offsets[10]].range(127, 96);
		data[10][20] = HBM_channelB10[offsets[10]].range(159, 128);
		data[10][21] = HBM_channelB10[offsets[10]].range(191, 160);
		data[10][22] = HBM_channelB10[offsets[10]].range(223, 192);
		data[10][23] = HBM_channelB10[offsets[10]].range(255, 224);
		data[10][24] = HBM_channelB10[offsets[10]].range(287, 256);
		data[10][25] = HBM_channelB10[offsets[10]].range(319, 288);
		data[10][26] = HBM_channelB10[offsets[10]].range(351, 320);
		data[10][27] = HBM_channelB10[offsets[10]].range(383, 352);
		data[10][28] = HBM_channelB10[offsets[10]].range(415, 384);
		data[10][29] = HBM_channelB10[offsets[10]].range(447, 416);
		data[10][30] = HBM_channelB10[offsets[10]].range(479, 448);
		data[10][31] = HBM_channelB10[offsets[10]].range(511, 480);
		data[11][0] = HBM_channelA11[offsets[11]].range(31, 0);
		data[11][1] = HBM_channelA11[offsets[11]].range(63, 32);
		data[11][2] = HBM_channelA11[offsets[11]].range(95, 64);
		data[11][3] = HBM_channelA11[offsets[11]].range(127, 96);
		data[11][4] = HBM_channelA11[offsets[11]].range(159, 128);
		data[11][5] = HBM_channelA11[offsets[11]].range(191, 160);
		data[11][6] = HBM_channelA11[offsets[11]].range(223, 192);
		data[11][7] = HBM_channelA11[offsets[11]].range(255, 224);
		data[11][8] = HBM_channelA11[offsets[11]].range(287, 256);
		data[11][9] = HBM_channelA11[offsets[11]].range(319, 288);
		data[11][10] = HBM_channelA11[offsets[11]].range(351, 320);
		data[11][11] = HBM_channelA11[offsets[11]].range(383, 352);
		data[11][12] = HBM_channelA11[offsets[11]].range(415, 384);
		data[11][13] = HBM_channelA11[offsets[11]].range(447, 416);
		data[11][14] = HBM_channelA11[offsets[11]].range(479, 448);
		data[11][15] = HBM_channelA11[offsets[11]].range(511, 480);
		data[11][16] = HBM_channelB11[offsets[11]].range(31, 0);
		data[11][17] = HBM_channelB11[offsets[11]].range(63, 32);
		data[11][18] = HBM_channelB11[offsets[11]].range(95, 64);
		data[11][19] = HBM_channelB11[offsets[11]].range(127, 96);
		data[11][20] = HBM_channelB11[offsets[11]].range(159, 128);
		data[11][21] = HBM_channelB11[offsets[11]].range(191, 160);
		data[11][22] = HBM_channelB11[offsets[11]].range(223, 192);
		data[11][23] = HBM_channelB11[offsets[11]].range(255, 224);
		data[11][24] = HBM_channelB11[offsets[11]].range(287, 256);
		data[11][25] = HBM_channelB11[offsets[11]].range(319, 288);
		data[11][26] = HBM_channelB11[offsets[11]].range(351, 320);
		data[11][27] = HBM_channelB11[offsets[11]].range(383, 352);
		data[11][28] = HBM_channelB11[offsets[11]].range(415, 384);
		data[11][29] = HBM_channelB11[offsets[11]].range(447, 416);
		data[11][30] = HBM_channelB11[offsets[11]].range(479, 448);
		data[11][31] = HBM_channelB11[offsets[11]].range(511, 480);
		data[12][0] = HBM_channelA12[offsets[12]].range(31, 0);
		data[12][1] = HBM_channelA12[offsets[12]].range(63, 32);
		data[12][2] = HBM_channelA12[offsets[12]].range(95, 64);
		data[12][3] = HBM_channelA12[offsets[12]].range(127, 96);
		data[12][4] = HBM_channelA12[offsets[12]].range(159, 128);
		data[12][5] = HBM_channelA12[offsets[12]].range(191, 160);
		data[12][6] = HBM_channelA12[offsets[12]].range(223, 192);
		data[12][7] = HBM_channelA12[offsets[12]].range(255, 224);
		data[12][8] = HBM_channelA12[offsets[12]].range(287, 256);
		data[12][9] = HBM_channelA12[offsets[12]].range(319, 288);
		data[12][10] = HBM_channelA12[offsets[12]].range(351, 320);
		data[12][11] = HBM_channelA12[offsets[12]].range(383, 352);
		data[12][12] = HBM_channelA12[offsets[12]].range(415, 384);
		data[12][13] = HBM_channelA12[offsets[12]].range(447, 416);
		data[12][14] = HBM_channelA12[offsets[12]].range(479, 448);
		data[12][15] = HBM_channelA12[offsets[12]].range(511, 480);
		data[12][16] = HBM_channelB12[offsets[12]].range(31, 0);
		data[12][17] = HBM_channelB12[offsets[12]].range(63, 32);
		data[12][18] = HBM_channelB12[offsets[12]].range(95, 64);
		data[12][19] = HBM_channelB12[offsets[12]].range(127, 96);
		data[12][20] = HBM_channelB12[offsets[12]].range(159, 128);
		data[12][21] = HBM_channelB12[offsets[12]].range(191, 160);
		data[12][22] = HBM_channelB12[offsets[12]].range(223, 192);
		data[12][23] = HBM_channelB12[offsets[12]].range(255, 224);
		data[12][24] = HBM_channelB12[offsets[12]].range(287, 256);
		data[12][25] = HBM_channelB12[offsets[12]].range(319, 288);
		data[12][26] = HBM_channelB12[offsets[12]].range(351, 320);
		data[12][27] = HBM_channelB12[offsets[12]].range(383, 352);
		data[12][28] = HBM_channelB12[offsets[12]].range(415, 384);
		data[12][29] = HBM_channelB12[offsets[12]].range(447, 416);
		data[12][30] = HBM_channelB12[offsets[12]].range(479, 448);
		data[12][31] = HBM_channelB12[offsets[12]].range(511, 480);
		data[13][0] = HBM_channelA13[offsets[13]].range(31, 0);
		data[13][1] = HBM_channelA13[offsets[13]].range(63, 32);
		data[13][2] = HBM_channelA13[offsets[13]].range(95, 64);
		data[13][3] = HBM_channelA13[offsets[13]].range(127, 96);
		data[13][4] = HBM_channelA13[offsets[13]].range(159, 128);
		data[13][5] = HBM_channelA13[offsets[13]].range(191, 160);
		data[13][6] = HBM_channelA13[offsets[13]].range(223, 192);
		data[13][7] = HBM_channelA13[offsets[13]].range(255, 224);
		data[13][8] = HBM_channelA13[offsets[13]].range(287, 256);
		data[13][9] = HBM_channelA13[offsets[13]].range(319, 288);
		data[13][10] = HBM_channelA13[offsets[13]].range(351, 320);
		data[13][11] = HBM_channelA13[offsets[13]].range(383, 352);
		data[13][12] = HBM_channelA13[offsets[13]].range(415, 384);
		data[13][13] = HBM_channelA13[offsets[13]].range(447, 416);
		data[13][14] = HBM_channelA13[offsets[13]].range(479, 448);
		data[13][15] = HBM_channelA13[offsets[13]].range(511, 480);
		data[13][16] = HBM_channelB13[offsets[13]].range(31, 0);
		data[13][17] = HBM_channelB13[offsets[13]].range(63, 32);
		data[13][18] = HBM_channelB13[offsets[13]].range(95, 64);
		data[13][19] = HBM_channelB13[offsets[13]].range(127, 96);
		data[13][20] = HBM_channelB13[offsets[13]].range(159, 128);
		data[13][21] = HBM_channelB13[offsets[13]].range(191, 160);
		data[13][22] = HBM_channelB13[offsets[13]].range(223, 192);
		data[13][23] = HBM_channelB13[offsets[13]].range(255, 224);
		data[13][24] = HBM_channelB13[offsets[13]].range(287, 256);
		data[13][25] = HBM_channelB13[offsets[13]].range(319, 288);
		data[13][26] = HBM_channelB13[offsets[13]].range(351, 320);
		data[13][27] = HBM_channelB13[offsets[13]].range(383, 352);
		data[13][28] = HBM_channelB13[offsets[13]].range(415, 384);
		data[13][29] = HBM_channelB13[offsets[13]].range(447, 416);
		data[13][30] = HBM_channelB13[offsets[13]].range(479, 448);
		data[13][31] = HBM_channelB13[offsets[13]].range(511, 480);
		data[14][0] = HBM_channelA14[offsets[14]].range(31, 0);
		data[14][1] = HBM_channelA14[offsets[14]].range(63, 32);
		data[14][2] = HBM_channelA14[offsets[14]].range(95, 64);
		data[14][3] = HBM_channelA14[offsets[14]].range(127, 96);
		data[14][4] = HBM_channelA14[offsets[14]].range(159, 128);
		data[14][5] = HBM_channelA14[offsets[14]].range(191, 160);
		data[14][6] = HBM_channelA14[offsets[14]].range(223, 192);
		data[14][7] = HBM_channelA14[offsets[14]].range(255, 224);
		data[14][8] = HBM_channelA14[offsets[14]].range(287, 256);
		data[14][9] = HBM_channelA14[offsets[14]].range(319, 288);
		data[14][10] = HBM_channelA14[offsets[14]].range(351, 320);
		data[14][11] = HBM_channelA14[offsets[14]].range(383, 352);
		data[14][12] = HBM_channelA14[offsets[14]].range(415, 384);
		data[14][13] = HBM_channelA14[offsets[14]].range(447, 416);
		data[14][14] = HBM_channelA14[offsets[14]].range(479, 448);
		data[14][15] = HBM_channelA14[offsets[14]].range(511, 480);
		data[14][16] = HBM_channelB14[offsets[14]].range(31, 0);
		data[14][17] = HBM_channelB14[offsets[14]].range(63, 32);
		data[14][18] = HBM_channelB14[offsets[14]].range(95, 64);
		data[14][19] = HBM_channelB14[offsets[14]].range(127, 96);
		data[14][20] = HBM_channelB14[offsets[14]].range(159, 128);
		data[14][21] = HBM_channelB14[offsets[14]].range(191, 160);
		data[14][22] = HBM_channelB14[offsets[14]].range(223, 192);
		data[14][23] = HBM_channelB14[offsets[14]].range(255, 224);
		data[14][24] = HBM_channelB14[offsets[14]].range(287, 256);
		data[14][25] = HBM_channelB14[offsets[14]].range(319, 288);
		data[14][26] = HBM_channelB14[offsets[14]].range(351, 320);
		data[14][27] = HBM_channelB14[offsets[14]].range(383, 352);
		data[14][28] = HBM_channelB14[offsets[14]].range(415, 384);
		data[14][29] = HBM_channelB14[offsets[14]].range(447, 416);
		data[14][30] = HBM_channelB14[offsets[14]].range(479, 448);
		data[14][31] = HBM_channelB14[offsets[14]].range(511, 480);
		data[15][0] = HBM_channelA15[offsets[15]].range(31, 0);
		data[15][1] = HBM_channelA15[offsets[15]].range(63, 32);
		data[15][2] = HBM_channelA15[offsets[15]].range(95, 64);
		data[15][3] = HBM_channelA15[offsets[15]].range(127, 96);
		data[15][4] = HBM_channelA15[offsets[15]].range(159, 128);
		data[15][5] = HBM_channelA15[offsets[15]].range(191, 160);
		data[15][6] = HBM_channelA15[offsets[15]].range(223, 192);
		data[15][7] = HBM_channelA15[offsets[15]].range(255, 224);
		data[15][8] = HBM_channelA15[offsets[15]].range(287, 256);
		data[15][9] = HBM_channelA15[offsets[15]].range(319, 288);
		data[15][10] = HBM_channelA15[offsets[15]].range(351, 320);
		data[15][11] = HBM_channelA15[offsets[15]].range(383, 352);
		data[15][12] = HBM_channelA15[offsets[15]].range(415, 384);
		data[15][13] = HBM_channelA15[offsets[15]].range(447, 416);
		data[15][14] = HBM_channelA15[offsets[15]].range(479, 448);
		data[15][15] = HBM_channelA15[offsets[15]].range(511, 480);
		data[15][16] = HBM_channelB15[offsets[15]].range(31, 0);
		data[15][17] = HBM_channelB15[offsets[15]].range(63, 32);
		data[15][18] = HBM_channelB15[offsets[15]].range(95, 64);
		data[15][19] = HBM_channelB15[offsets[15]].range(127, 96);
		data[15][20] = HBM_channelB15[offsets[15]].range(159, 128);
		data[15][21] = HBM_channelB15[offsets[15]].range(191, 160);
		data[15][22] = HBM_channelB15[offsets[15]].range(223, 192);
		data[15][23] = HBM_channelB15[offsets[15]].range(255, 224);
		data[15][24] = HBM_channelB15[offsets[15]].range(287, 256);
		data[15][25] = HBM_channelB15[offsets[15]].range(319, 288);
		data[15][26] = HBM_channelB15[offsets[15]].range(351, 320);
		data[15][27] = HBM_channelB15[offsets[15]].range(383, 352);
		data[15][28] = HBM_channelB15[offsets[15]].range(415, 384);
		data[15][29] = HBM_channelB15[offsets[15]].range(447, 416);
		data[15][30] = HBM_channelB15[offsets[15]].range(479, 448);
		data[15][31] = HBM_channelB15[offsets[15]].range(511, 480);
		data[16][0] = HBM_channelA16[offsets[16]].range(31, 0);
		data[16][1] = HBM_channelA16[offsets[16]].range(63, 32);
		data[16][2] = HBM_channelA16[offsets[16]].range(95, 64);
		data[16][3] = HBM_channelA16[offsets[16]].range(127, 96);
		data[16][4] = HBM_channelA16[offsets[16]].range(159, 128);
		data[16][5] = HBM_channelA16[offsets[16]].range(191, 160);
		data[16][6] = HBM_channelA16[offsets[16]].range(223, 192);
		data[16][7] = HBM_channelA16[offsets[16]].range(255, 224);
		data[16][8] = HBM_channelA16[offsets[16]].range(287, 256);
		data[16][9] = HBM_channelA16[offsets[16]].range(319, 288);
		data[16][10] = HBM_channelA16[offsets[16]].range(351, 320);
		data[16][11] = HBM_channelA16[offsets[16]].range(383, 352);
		data[16][12] = HBM_channelA16[offsets[16]].range(415, 384);
		data[16][13] = HBM_channelA16[offsets[16]].range(447, 416);
		data[16][14] = HBM_channelA16[offsets[16]].range(479, 448);
		data[16][15] = HBM_channelA16[offsets[16]].range(511, 480);
		data[16][16] = HBM_channelB16[offsets[16]].range(31, 0);
		data[16][17] = HBM_channelB16[offsets[16]].range(63, 32);
		data[16][18] = HBM_channelB16[offsets[16]].range(95, 64);
		data[16][19] = HBM_channelB16[offsets[16]].range(127, 96);
		data[16][20] = HBM_channelB16[offsets[16]].range(159, 128);
		data[16][21] = HBM_channelB16[offsets[16]].range(191, 160);
		data[16][22] = HBM_channelB16[offsets[16]].range(223, 192);
		data[16][23] = HBM_channelB16[offsets[16]].range(255, 224);
		data[16][24] = HBM_channelB16[offsets[16]].range(287, 256);
		data[16][25] = HBM_channelB16[offsets[16]].range(319, 288);
		data[16][26] = HBM_channelB16[offsets[16]].range(351, 320);
		data[16][27] = HBM_channelB16[offsets[16]].range(383, 352);
		data[16][28] = HBM_channelB16[offsets[16]].range(415, 384);
		data[16][29] = HBM_channelB16[offsets[16]].range(447, 416);
		data[16][30] = HBM_channelB16[offsets[16]].range(479, 448);
		data[16][31] = HBM_channelB16[offsets[16]].range(511, 480);
		data[17][0] = HBM_channelA17[offsets[17]].range(31, 0);
		data[17][1] = HBM_channelA17[offsets[17]].range(63, 32);
		data[17][2] = HBM_channelA17[offsets[17]].range(95, 64);
		data[17][3] = HBM_channelA17[offsets[17]].range(127, 96);
		data[17][4] = HBM_channelA17[offsets[17]].range(159, 128);
		data[17][5] = HBM_channelA17[offsets[17]].range(191, 160);
		data[17][6] = HBM_channelA17[offsets[17]].range(223, 192);
		data[17][7] = HBM_channelA17[offsets[17]].range(255, 224);
		data[17][8] = HBM_channelA17[offsets[17]].range(287, 256);
		data[17][9] = HBM_channelA17[offsets[17]].range(319, 288);
		data[17][10] = HBM_channelA17[offsets[17]].range(351, 320);
		data[17][11] = HBM_channelA17[offsets[17]].range(383, 352);
		data[17][12] = HBM_channelA17[offsets[17]].range(415, 384);
		data[17][13] = HBM_channelA17[offsets[17]].range(447, 416);
		data[17][14] = HBM_channelA17[offsets[17]].range(479, 448);
		data[17][15] = HBM_channelA17[offsets[17]].range(511, 480);
		data[17][16] = HBM_channelB17[offsets[17]].range(31, 0);
		data[17][17] = HBM_channelB17[offsets[17]].range(63, 32);
		data[17][18] = HBM_channelB17[offsets[17]].range(95, 64);
		data[17][19] = HBM_channelB17[offsets[17]].range(127, 96);
		data[17][20] = HBM_channelB17[offsets[17]].range(159, 128);
		data[17][21] = HBM_channelB17[offsets[17]].range(191, 160);
		data[17][22] = HBM_channelB17[offsets[17]].range(223, 192);
		data[17][23] = HBM_channelB17[offsets[17]].range(255, 224);
		data[17][24] = HBM_channelB17[offsets[17]].range(287, 256);
		data[17][25] = HBM_channelB17[offsets[17]].range(319, 288);
		data[17][26] = HBM_channelB17[offsets[17]].range(351, 320);
		data[17][27] = HBM_channelB17[offsets[17]].range(383, 352);
		data[17][28] = HBM_channelB17[offsets[17]].range(415, 384);
		data[17][29] = HBM_channelB17[offsets[17]].range(447, 416);
		data[17][30] = HBM_channelB17[offsets[17]].range(479, 448);
		data[17][31] = HBM_channelB17[offsets[17]].range(511, 480);
		data[18][0] = HBM_channelA18[offsets[18]].range(31, 0);
		data[18][1] = HBM_channelA18[offsets[18]].range(63, 32);
		data[18][2] = HBM_channelA18[offsets[18]].range(95, 64);
		data[18][3] = HBM_channelA18[offsets[18]].range(127, 96);
		data[18][4] = HBM_channelA18[offsets[18]].range(159, 128);
		data[18][5] = HBM_channelA18[offsets[18]].range(191, 160);
		data[18][6] = HBM_channelA18[offsets[18]].range(223, 192);
		data[18][7] = HBM_channelA18[offsets[18]].range(255, 224);
		data[18][8] = HBM_channelA18[offsets[18]].range(287, 256);
		data[18][9] = HBM_channelA18[offsets[18]].range(319, 288);
		data[18][10] = HBM_channelA18[offsets[18]].range(351, 320);
		data[18][11] = HBM_channelA18[offsets[18]].range(383, 352);
		data[18][12] = HBM_channelA18[offsets[18]].range(415, 384);
		data[18][13] = HBM_channelA18[offsets[18]].range(447, 416);
		data[18][14] = HBM_channelA18[offsets[18]].range(479, 448);
		data[18][15] = HBM_channelA18[offsets[18]].range(511, 480);
		data[18][16] = HBM_channelB18[offsets[18]].range(31, 0);
		data[18][17] = HBM_channelB18[offsets[18]].range(63, 32);
		data[18][18] = HBM_channelB18[offsets[18]].range(95, 64);
		data[18][19] = HBM_channelB18[offsets[18]].range(127, 96);
		data[18][20] = HBM_channelB18[offsets[18]].range(159, 128);
		data[18][21] = HBM_channelB18[offsets[18]].range(191, 160);
		data[18][22] = HBM_channelB18[offsets[18]].range(223, 192);
		data[18][23] = HBM_channelB18[offsets[18]].range(255, 224);
		data[18][24] = HBM_channelB18[offsets[18]].range(287, 256);
		data[18][25] = HBM_channelB18[offsets[18]].range(319, 288);
		data[18][26] = HBM_channelB18[offsets[18]].range(351, 320);
		data[18][27] = HBM_channelB18[offsets[18]].range(383, 352);
		data[18][28] = HBM_channelB18[offsets[18]].range(415, 384);
		data[18][29] = HBM_channelB18[offsets[18]].range(447, 416);
		data[18][30] = HBM_channelB18[offsets[18]].range(479, 448);
		data[18][31] = HBM_channelB18[offsets[18]].range(511, 480);
		data[19][0] = HBM_channelA19[offsets[19]].range(31, 0);
		data[19][1] = HBM_channelA19[offsets[19]].range(63, 32);
		data[19][2] = HBM_channelA19[offsets[19]].range(95, 64);
		data[19][3] = HBM_channelA19[offsets[19]].range(127, 96);
		data[19][4] = HBM_channelA19[offsets[19]].range(159, 128);
		data[19][5] = HBM_channelA19[offsets[19]].range(191, 160);
		data[19][6] = HBM_channelA19[offsets[19]].range(223, 192);
		data[19][7] = HBM_channelA19[offsets[19]].range(255, 224);
		data[19][8] = HBM_channelA19[offsets[19]].range(287, 256);
		data[19][9] = HBM_channelA19[offsets[19]].range(319, 288);
		data[19][10] = HBM_channelA19[offsets[19]].range(351, 320);
		data[19][11] = HBM_channelA19[offsets[19]].range(383, 352);
		data[19][12] = HBM_channelA19[offsets[19]].range(415, 384);
		data[19][13] = HBM_channelA19[offsets[19]].range(447, 416);
		data[19][14] = HBM_channelA19[offsets[19]].range(479, 448);
		data[19][15] = HBM_channelA19[offsets[19]].range(511, 480);
		data[19][16] = HBM_channelB19[offsets[19]].range(31, 0);
		data[19][17] = HBM_channelB19[offsets[19]].range(63, 32);
		data[19][18] = HBM_channelB19[offsets[19]].range(95, 64);
		data[19][19] = HBM_channelB19[offsets[19]].range(127, 96);
		data[19][20] = HBM_channelB19[offsets[19]].range(159, 128);
		data[19][21] = HBM_channelB19[offsets[19]].range(191, 160);
		data[19][22] = HBM_channelB19[offsets[19]].range(223, 192);
		data[19][23] = HBM_channelB19[offsets[19]].range(255, 224);
		data[19][24] = HBM_channelB19[offsets[19]].range(287, 256);
		data[19][25] = HBM_channelB19[offsets[19]].range(319, 288);
		data[19][26] = HBM_channelB19[offsets[19]].range(351, 320);
		data[19][27] = HBM_channelB19[offsets[19]].range(383, 352);
		data[19][28] = HBM_channelB19[offsets[19]].range(415, 384);
		data[19][29] = HBM_channelB19[offsets[19]].range(447, 416);
		data[19][30] = HBM_channelB19[offsets[19]].range(479, 448);
		data[19][31] = HBM_channelB19[offsets[19]].range(511, 480);
		data[20][0] = HBM_channelA20[offsets[20]].range(31, 0);
		data[20][1] = HBM_channelA20[offsets[20]].range(63, 32);
		data[20][2] = HBM_channelA20[offsets[20]].range(95, 64);
		data[20][3] = HBM_channelA20[offsets[20]].range(127, 96);
		data[20][4] = HBM_channelA20[offsets[20]].range(159, 128);
		data[20][5] = HBM_channelA20[offsets[20]].range(191, 160);
		data[20][6] = HBM_channelA20[offsets[20]].range(223, 192);
		data[20][7] = HBM_channelA20[offsets[20]].range(255, 224);
		data[20][8] = HBM_channelA20[offsets[20]].range(287, 256);
		data[20][9] = HBM_channelA20[offsets[20]].range(319, 288);
		data[20][10] = HBM_channelA20[offsets[20]].range(351, 320);
		data[20][11] = HBM_channelA20[offsets[20]].range(383, 352);
		data[20][12] = HBM_channelA20[offsets[20]].range(415, 384);
		data[20][13] = HBM_channelA20[offsets[20]].range(447, 416);
		data[20][14] = HBM_channelA20[offsets[20]].range(479, 448);
		data[20][15] = HBM_channelA20[offsets[20]].range(511, 480);
		data[20][16] = HBM_channelB20[offsets[20]].range(31, 0);
		data[20][17] = HBM_channelB20[offsets[20]].range(63, 32);
		data[20][18] = HBM_channelB20[offsets[20]].range(95, 64);
		data[20][19] = HBM_channelB20[offsets[20]].range(127, 96);
		data[20][20] = HBM_channelB20[offsets[20]].range(159, 128);
		data[20][21] = HBM_channelB20[offsets[20]].range(191, 160);
		data[20][22] = HBM_channelB20[offsets[20]].range(223, 192);
		data[20][23] = HBM_channelB20[offsets[20]].range(255, 224);
		data[20][24] = HBM_channelB20[offsets[20]].range(287, 256);
		data[20][25] = HBM_channelB20[offsets[20]].range(319, 288);
		data[20][26] = HBM_channelB20[offsets[20]].range(351, 320);
		data[20][27] = HBM_channelB20[offsets[20]].range(383, 352);
		data[20][28] = HBM_channelB20[offsets[20]].range(415, 384);
		data[20][29] = HBM_channelB20[offsets[20]].range(447, 416);
		data[20][30] = HBM_channelB20[offsets[20]].range(479, 448);
		data[20][31] = HBM_channelB20[offsets[20]].range(511, 480);
		data[21][0] = HBM_channelA21[offsets[21]].range(31, 0);
		data[21][1] = HBM_channelA21[offsets[21]].range(63, 32);
		data[21][2] = HBM_channelA21[offsets[21]].range(95, 64);
		data[21][3] = HBM_channelA21[offsets[21]].range(127, 96);
		data[21][4] = HBM_channelA21[offsets[21]].range(159, 128);
		data[21][5] = HBM_channelA21[offsets[21]].range(191, 160);
		data[21][6] = HBM_channelA21[offsets[21]].range(223, 192);
		data[21][7] = HBM_channelA21[offsets[21]].range(255, 224);
		data[21][8] = HBM_channelA21[offsets[21]].range(287, 256);
		data[21][9] = HBM_channelA21[offsets[21]].range(319, 288);
		data[21][10] = HBM_channelA21[offsets[21]].range(351, 320);
		data[21][11] = HBM_channelA21[offsets[21]].range(383, 352);
		data[21][12] = HBM_channelA21[offsets[21]].range(415, 384);
		data[21][13] = HBM_channelA21[offsets[21]].range(447, 416);
		data[21][14] = HBM_channelA21[offsets[21]].range(479, 448);
		data[21][15] = HBM_channelA21[offsets[21]].range(511, 480);
		data[21][16] = HBM_channelB21[offsets[21]].range(31, 0);
		data[21][17] = HBM_channelB21[offsets[21]].range(63, 32);
		data[21][18] = HBM_channelB21[offsets[21]].range(95, 64);
		data[21][19] = HBM_channelB21[offsets[21]].range(127, 96);
		data[21][20] = HBM_channelB21[offsets[21]].range(159, 128);
		data[21][21] = HBM_channelB21[offsets[21]].range(191, 160);
		data[21][22] = HBM_channelB21[offsets[21]].range(223, 192);
		data[21][23] = HBM_channelB21[offsets[21]].range(255, 224);
		data[21][24] = HBM_channelB21[offsets[21]].range(287, 256);
		data[21][25] = HBM_channelB21[offsets[21]].range(319, 288);
		data[21][26] = HBM_channelB21[offsets[21]].range(351, 320);
		data[21][27] = HBM_channelB21[offsets[21]].range(383, 352);
		data[21][28] = HBM_channelB21[offsets[21]].range(415, 384);
		data[21][29] = HBM_channelB21[offsets[21]].range(447, 416);
		data[21][30] = HBM_channelB21[offsets[21]].range(479, 448);
		data[21][31] = HBM_channelB21[offsets[21]].range(511, 480);
		data[22][0] = HBM_channelA22[offsets[22]].range(31, 0);
		data[22][1] = HBM_channelA22[offsets[22]].range(63, 32);
		data[22][2] = HBM_channelA22[offsets[22]].range(95, 64);
		data[22][3] = HBM_channelA22[offsets[22]].range(127, 96);
		data[22][4] = HBM_channelA22[offsets[22]].range(159, 128);
		data[22][5] = HBM_channelA22[offsets[22]].range(191, 160);
		data[22][6] = HBM_channelA22[offsets[22]].range(223, 192);
		data[22][7] = HBM_channelA22[offsets[22]].range(255, 224);
		data[22][8] = HBM_channelA22[offsets[22]].range(287, 256);
		data[22][9] = HBM_channelA22[offsets[22]].range(319, 288);
		data[22][10] = HBM_channelA22[offsets[22]].range(351, 320);
		data[22][11] = HBM_channelA22[offsets[22]].range(383, 352);
		data[22][12] = HBM_channelA22[offsets[22]].range(415, 384);
		data[22][13] = HBM_channelA22[offsets[22]].range(447, 416);
		data[22][14] = HBM_channelA22[offsets[22]].range(479, 448);
		data[22][15] = HBM_channelA22[offsets[22]].range(511, 480);
		data[22][16] = HBM_channelB22[offsets[22]].range(31, 0);
		data[22][17] = HBM_channelB22[offsets[22]].range(63, 32);
		data[22][18] = HBM_channelB22[offsets[22]].range(95, 64);
		data[22][19] = HBM_channelB22[offsets[22]].range(127, 96);
		data[22][20] = HBM_channelB22[offsets[22]].range(159, 128);
		data[22][21] = HBM_channelB22[offsets[22]].range(191, 160);
		data[22][22] = HBM_channelB22[offsets[22]].range(223, 192);
		data[22][23] = HBM_channelB22[offsets[22]].range(255, 224);
		data[22][24] = HBM_channelB22[offsets[22]].range(287, 256);
		data[22][25] = HBM_channelB22[offsets[22]].range(319, 288);
		data[22][26] = HBM_channelB22[offsets[22]].range(351, 320);
		data[22][27] = HBM_channelB22[offsets[22]].range(383, 352);
		data[22][28] = HBM_channelB22[offsets[22]].range(415, 384);
		data[22][29] = HBM_channelB22[offsets[22]].range(447, 416);
		data[22][30] = HBM_channelB22[offsets[22]].range(479, 448);
		data[22][31] = HBM_channelB22[offsets[22]].range(511, 480);
		data[23][0] = HBM_channelA23[offsets[23]].range(31, 0);
		data[23][1] = HBM_channelA23[offsets[23]].range(63, 32);
		data[23][2] = HBM_channelA23[offsets[23]].range(95, 64);
		data[23][3] = HBM_channelA23[offsets[23]].range(127, 96);
		data[23][4] = HBM_channelA23[offsets[23]].range(159, 128);
		data[23][5] = HBM_channelA23[offsets[23]].range(191, 160);
		data[23][6] = HBM_channelA23[offsets[23]].range(223, 192);
		data[23][7] = HBM_channelA23[offsets[23]].range(255, 224);
		data[23][8] = HBM_channelA23[offsets[23]].range(287, 256);
		data[23][9] = HBM_channelA23[offsets[23]].range(319, 288);
		data[23][10] = HBM_channelA23[offsets[23]].range(351, 320);
		data[23][11] = HBM_channelA23[offsets[23]].range(383, 352);
		data[23][12] = HBM_channelA23[offsets[23]].range(415, 384);
		data[23][13] = HBM_channelA23[offsets[23]].range(447, 416);
		data[23][14] = HBM_channelA23[offsets[23]].range(479, 448);
		data[23][15] = HBM_channelA23[offsets[23]].range(511, 480);
		data[23][16] = HBM_channelB23[offsets[23]].range(31, 0);
		data[23][17] = HBM_channelB23[offsets[23]].range(63, 32);
		data[23][18] = HBM_channelB23[offsets[23]].range(95, 64);
		data[23][19] = HBM_channelB23[offsets[23]].range(127, 96);
		data[23][20] = HBM_channelB23[offsets[23]].range(159, 128);
		data[23][21] = HBM_channelB23[offsets[23]].range(191, 160);
		data[23][22] = HBM_channelB23[offsets[23]].range(223, 192);
		data[23][23] = HBM_channelB23[offsets[23]].range(255, 224);
		data[23][24] = HBM_channelB23[offsets[23]].range(287, 256);
		data[23][25] = HBM_channelB23[offsets[23]].range(319, 288);
		data[23][26] = HBM_channelB23[offsets[23]].range(351, 320);
		data[23][27] = HBM_channelB23[offsets[23]].range(383, 352);
		data[23][28] = HBM_channelB23[offsets[23]].range(415, 384);
		data[23][29] = HBM_channelB23[offsets[23]].range(447, 416);
		data[23][30] = HBM_channelB23[offsets[23]].range(479, 448);
		data[23][31] = HBM_channelB23[offsets[23]].range(511, 480);
	#else 
		#ifdef ___USE_AXI_CHANNEL___
		data[0][0] = HBM_channelA0[offsets[0]].data[0];
		data[0][1] = HBM_channelA0[offsets[0]].data[1];
		data[0][2] = HBM_channelA0[offsets[0]].data[2];
		data[0][3] = HBM_channelA0[offsets[0]].data[3];
		data[0][4] = HBM_channelA0[offsets[0]].data[4];
		data[0][5] = HBM_channelA0[offsets[0]].data[5];
		data[0][6] = HBM_channelA0[offsets[0]].data[6];
		data[0][7] = HBM_channelA0[offsets[0]].data[7];
		data[0][8] = HBM_channelA0[offsets[0]].data[8];
		data[0][9] = HBM_channelA0[offsets[0]].data[9];
		data[0][10] = HBM_channelA0[offsets[0]].data[10];
		data[0][11] = HBM_channelA0[offsets[0]].data[11];
		data[0][12] = HBM_channelA0[offsets[0]].data[12];
		data[0][13] = HBM_channelA0[offsets[0]].data[13];
		data[0][14] = HBM_channelA0[offsets[0]].data[14];
		data[0][15] = HBM_channelA0[offsets[0]].data[15];
		data[0][16] = HBM_channelB0[offsets[0]].data[0];
		data[0][17] = HBM_channelB0[offsets[0]].data[1];
		data[0][18] = HBM_channelB0[offsets[0]].data[2];
		data[0][19] = HBM_channelB0[offsets[0]].data[3];
		data[0][20] = HBM_channelB0[offsets[0]].data[4];
		data[0][21] = HBM_channelB0[offsets[0]].data[5];
		data[0][22] = HBM_channelB0[offsets[0]].data[6];
		data[0][23] = HBM_channelB0[offsets[0]].data[7];
		data[0][24] = HBM_channelB0[offsets[0]].data[8];
		data[0][25] = HBM_channelB0[offsets[0]].data[9];
		data[0][26] = HBM_channelB0[offsets[0]].data[10];
		data[0][27] = HBM_channelB0[offsets[0]].data[11];
		data[0][28] = HBM_channelB0[offsets[0]].data[12];
		data[0][29] = HBM_channelB0[offsets[0]].data[13];
		data[0][30] = HBM_channelB0[offsets[0]].data[14];
		data[0][31] = HBM_channelB0[offsets[0]].data[15];
		data[1][0] = HBM_channelA1[offsets[1]].data[0];
		data[1][1] = HBM_channelA1[offsets[1]].data[1];
		data[1][2] = HBM_channelA1[offsets[1]].data[2];
		data[1][3] = HBM_channelA1[offsets[1]].data[3];
		data[1][4] = HBM_channelA1[offsets[1]].data[4];
		data[1][5] = HBM_channelA1[offsets[1]].data[5];
		data[1][6] = HBM_channelA1[offsets[1]].data[6];
		data[1][7] = HBM_channelA1[offsets[1]].data[7];
		data[1][8] = HBM_channelA1[offsets[1]].data[8];
		data[1][9] = HBM_channelA1[offsets[1]].data[9];
		data[1][10] = HBM_channelA1[offsets[1]].data[10];
		data[1][11] = HBM_channelA1[offsets[1]].data[11];
		data[1][12] = HBM_channelA1[offsets[1]].data[12];
		data[1][13] = HBM_channelA1[offsets[1]].data[13];
		data[1][14] = HBM_channelA1[offsets[1]].data[14];
		data[1][15] = HBM_channelA1[offsets[1]].data[15];
		data[1][16] = HBM_channelB1[offsets[1]].data[0];
		data[1][17] = HBM_channelB1[offsets[1]].data[1];
		data[1][18] = HBM_channelB1[offsets[1]].data[2];
		data[1][19] = HBM_channelB1[offsets[1]].data[3];
		data[1][20] = HBM_channelB1[offsets[1]].data[4];
		data[1][21] = HBM_channelB1[offsets[1]].data[5];
		data[1][22] = HBM_channelB1[offsets[1]].data[6];
		data[1][23] = HBM_channelB1[offsets[1]].data[7];
		data[1][24] = HBM_channelB1[offsets[1]].data[8];
		data[1][25] = HBM_channelB1[offsets[1]].data[9];
		data[1][26] = HBM_channelB1[offsets[1]].data[10];
		data[1][27] = HBM_channelB1[offsets[1]].data[11];
		data[1][28] = HBM_channelB1[offsets[1]].data[12];
		data[1][29] = HBM_channelB1[offsets[1]].data[13];
		data[1][30] = HBM_channelB1[offsets[1]].data[14];
		data[1][31] = HBM_channelB1[offsets[1]].data[15];
		data[2][0] = HBM_channelA2[offsets[2]].data[0];
		data[2][1] = HBM_channelA2[offsets[2]].data[1];
		data[2][2] = HBM_channelA2[offsets[2]].data[2];
		data[2][3] = HBM_channelA2[offsets[2]].data[3];
		data[2][4] = HBM_channelA2[offsets[2]].data[4];
		data[2][5] = HBM_channelA2[offsets[2]].data[5];
		data[2][6] = HBM_channelA2[offsets[2]].data[6];
		data[2][7] = HBM_channelA2[offsets[2]].data[7];
		data[2][8] = HBM_channelA2[offsets[2]].data[8];
		data[2][9] = HBM_channelA2[offsets[2]].data[9];
		data[2][10] = HBM_channelA2[offsets[2]].data[10];
		data[2][11] = HBM_channelA2[offsets[2]].data[11];
		data[2][12] = HBM_channelA2[offsets[2]].data[12];
		data[2][13] = HBM_channelA2[offsets[2]].data[13];
		data[2][14] = HBM_channelA2[offsets[2]].data[14];
		data[2][15] = HBM_channelA2[offsets[2]].data[15];
		data[2][16] = HBM_channelB2[offsets[2]].data[0];
		data[2][17] = HBM_channelB2[offsets[2]].data[1];
		data[2][18] = HBM_channelB2[offsets[2]].data[2];
		data[2][19] = HBM_channelB2[offsets[2]].data[3];
		data[2][20] = HBM_channelB2[offsets[2]].data[4];
		data[2][21] = HBM_channelB2[offsets[2]].data[5];
		data[2][22] = HBM_channelB2[offsets[2]].data[6];
		data[2][23] = HBM_channelB2[offsets[2]].data[7];
		data[2][24] = HBM_channelB2[offsets[2]].data[8];
		data[2][25] = HBM_channelB2[offsets[2]].data[9];
		data[2][26] = HBM_channelB2[offsets[2]].data[10];
		data[2][27] = HBM_channelB2[offsets[2]].data[11];
		data[2][28] = HBM_channelB2[offsets[2]].data[12];
		data[2][29] = HBM_channelB2[offsets[2]].data[13];
		data[2][30] = HBM_channelB2[offsets[2]].data[14];
		data[2][31] = HBM_channelB2[offsets[2]].data[15];
		data[3][0] = HBM_channelA3[offsets[3]].data[0];
		data[3][1] = HBM_channelA3[offsets[3]].data[1];
		data[3][2] = HBM_channelA3[offsets[3]].data[2];
		data[3][3] = HBM_channelA3[offsets[3]].data[3];
		data[3][4] = HBM_channelA3[offsets[3]].data[4];
		data[3][5] = HBM_channelA3[offsets[3]].data[5];
		data[3][6] = HBM_channelA3[offsets[3]].data[6];
		data[3][7] = HBM_channelA3[offsets[3]].data[7];
		data[3][8] = HBM_channelA3[offsets[3]].data[8];
		data[3][9] = HBM_channelA3[offsets[3]].data[9];
		data[3][10] = HBM_channelA3[offsets[3]].data[10];
		data[3][11] = HBM_channelA3[offsets[3]].data[11];
		data[3][12] = HBM_channelA3[offsets[3]].data[12];
		data[3][13] = HBM_channelA3[offsets[3]].data[13];
		data[3][14] = HBM_channelA3[offsets[3]].data[14];
		data[3][15] = HBM_channelA3[offsets[3]].data[15];
		data[3][16] = HBM_channelB3[offsets[3]].data[0];
		data[3][17] = HBM_channelB3[offsets[3]].data[1];
		data[3][18] = HBM_channelB3[offsets[3]].data[2];
		data[3][19] = HBM_channelB3[offsets[3]].data[3];
		data[3][20] = HBM_channelB3[offsets[3]].data[4];
		data[3][21] = HBM_channelB3[offsets[3]].data[5];
		data[3][22] = HBM_channelB3[offsets[3]].data[6];
		data[3][23] = HBM_channelB3[offsets[3]].data[7];
		data[3][24] = HBM_channelB3[offsets[3]].data[8];
		data[3][25] = HBM_channelB3[offsets[3]].data[9];
		data[3][26] = HBM_channelB3[offsets[3]].data[10];
		data[3][27] = HBM_channelB3[offsets[3]].data[11];
		data[3][28] = HBM_channelB3[offsets[3]].data[12];
		data[3][29] = HBM_channelB3[offsets[3]].data[13];
		data[3][30] = HBM_channelB3[offsets[3]].data[14];
		data[3][31] = HBM_channelB3[offsets[3]].data[15];
		data[4][0] = HBM_channelA4[offsets[4]].data[0];
		data[4][1] = HBM_channelA4[offsets[4]].data[1];
		data[4][2] = HBM_channelA4[offsets[4]].data[2];
		data[4][3] = HBM_channelA4[offsets[4]].data[3];
		data[4][4] = HBM_channelA4[offsets[4]].data[4];
		data[4][5] = HBM_channelA4[offsets[4]].data[5];
		data[4][6] = HBM_channelA4[offsets[4]].data[6];
		data[4][7] = HBM_channelA4[offsets[4]].data[7];
		data[4][8] = HBM_channelA4[offsets[4]].data[8];
		data[4][9] = HBM_channelA4[offsets[4]].data[9];
		data[4][10] = HBM_channelA4[offsets[4]].data[10];
		data[4][11] = HBM_channelA4[offsets[4]].data[11];
		data[4][12] = HBM_channelA4[offsets[4]].data[12];
		data[4][13] = HBM_channelA4[offsets[4]].data[13];
		data[4][14] = HBM_channelA4[offsets[4]].data[14];
		data[4][15] = HBM_channelA4[offsets[4]].data[15];
		data[4][16] = HBM_channelB4[offsets[4]].data[0];
		data[4][17] = HBM_channelB4[offsets[4]].data[1];
		data[4][18] = HBM_channelB4[offsets[4]].data[2];
		data[4][19] = HBM_channelB4[offsets[4]].data[3];
		data[4][20] = HBM_channelB4[offsets[4]].data[4];
		data[4][21] = HBM_channelB4[offsets[4]].data[5];
		data[4][22] = HBM_channelB4[offsets[4]].data[6];
		data[4][23] = HBM_channelB4[offsets[4]].data[7];
		data[4][24] = HBM_channelB4[offsets[4]].data[8];
		data[4][25] = HBM_channelB4[offsets[4]].data[9];
		data[4][26] = HBM_channelB4[offsets[4]].data[10];
		data[4][27] = HBM_channelB4[offsets[4]].data[11];
		data[4][28] = HBM_channelB4[offsets[4]].data[12];
		data[4][29] = HBM_channelB4[offsets[4]].data[13];
		data[4][30] = HBM_channelB4[offsets[4]].data[14];
		data[4][31] = HBM_channelB4[offsets[4]].data[15];
		data[5][0] = HBM_channelA5[offsets[5]].data[0];
		data[5][1] = HBM_channelA5[offsets[5]].data[1];
		data[5][2] = HBM_channelA5[offsets[5]].data[2];
		data[5][3] = HBM_channelA5[offsets[5]].data[3];
		data[5][4] = HBM_channelA5[offsets[5]].data[4];
		data[5][5] = HBM_channelA5[offsets[5]].data[5];
		data[5][6] = HBM_channelA5[offsets[5]].data[6];
		data[5][7] = HBM_channelA5[offsets[5]].data[7];
		data[5][8] = HBM_channelA5[offsets[5]].data[8];
		data[5][9] = HBM_channelA5[offsets[5]].data[9];
		data[5][10] = HBM_channelA5[offsets[5]].data[10];
		data[5][11] = HBM_channelA5[offsets[5]].data[11];
		data[5][12] = HBM_channelA5[offsets[5]].data[12];
		data[5][13] = HBM_channelA5[offsets[5]].data[13];
		data[5][14] = HBM_channelA5[offsets[5]].data[14];
		data[5][15] = HBM_channelA5[offsets[5]].data[15];
		data[5][16] = HBM_channelB5[offsets[5]].data[0];
		data[5][17] = HBM_channelB5[offsets[5]].data[1];
		data[5][18] = HBM_channelB5[offsets[5]].data[2];
		data[5][19] = HBM_channelB5[offsets[5]].data[3];
		data[5][20] = HBM_channelB5[offsets[5]].data[4];
		data[5][21] = HBM_channelB5[offsets[5]].data[5];
		data[5][22] = HBM_channelB5[offsets[5]].data[6];
		data[5][23] = HBM_channelB5[offsets[5]].data[7];
		data[5][24] = HBM_channelB5[offsets[5]].data[8];
		data[5][25] = HBM_channelB5[offsets[5]].data[9];
		data[5][26] = HBM_channelB5[offsets[5]].data[10];
		data[5][27] = HBM_channelB5[offsets[5]].data[11];
		data[5][28] = HBM_channelB5[offsets[5]].data[12];
		data[5][29] = HBM_channelB5[offsets[5]].data[13];
		data[5][30] = HBM_channelB5[offsets[5]].data[14];
		data[5][31] = HBM_channelB5[offsets[5]].data[15];
		data[6][0] = HBM_channelA6[offsets[6]].data[0];
		data[6][1] = HBM_channelA6[offsets[6]].data[1];
		data[6][2] = HBM_channelA6[offsets[6]].data[2];
		data[6][3] = HBM_channelA6[offsets[6]].data[3];
		data[6][4] = HBM_channelA6[offsets[6]].data[4];
		data[6][5] = HBM_channelA6[offsets[6]].data[5];
		data[6][6] = HBM_channelA6[offsets[6]].data[6];
		data[6][7] = HBM_channelA6[offsets[6]].data[7];
		data[6][8] = HBM_channelA6[offsets[6]].data[8];
		data[6][9] = HBM_channelA6[offsets[6]].data[9];
		data[6][10] = HBM_channelA6[offsets[6]].data[10];
		data[6][11] = HBM_channelA6[offsets[6]].data[11];
		data[6][12] = HBM_channelA6[offsets[6]].data[12];
		data[6][13] = HBM_channelA6[offsets[6]].data[13];
		data[6][14] = HBM_channelA6[offsets[6]].data[14];
		data[6][15] = HBM_channelA6[offsets[6]].data[15];
		data[6][16] = HBM_channelB6[offsets[6]].data[0];
		data[6][17] = HBM_channelB6[offsets[6]].data[1];
		data[6][18] = HBM_channelB6[offsets[6]].data[2];
		data[6][19] = HBM_channelB6[offsets[6]].data[3];
		data[6][20] = HBM_channelB6[offsets[6]].data[4];
		data[6][21] = HBM_channelB6[offsets[6]].data[5];
		data[6][22] = HBM_channelB6[offsets[6]].data[6];
		data[6][23] = HBM_channelB6[offsets[6]].data[7];
		data[6][24] = HBM_channelB6[offsets[6]].data[8];
		data[6][25] = HBM_channelB6[offsets[6]].data[9];
		data[6][26] = HBM_channelB6[offsets[6]].data[10];
		data[6][27] = HBM_channelB6[offsets[6]].data[11];
		data[6][28] = HBM_channelB6[offsets[6]].data[12];
		data[6][29] = HBM_channelB6[offsets[6]].data[13];
		data[6][30] = HBM_channelB6[offsets[6]].data[14];
		data[6][31] = HBM_channelB6[offsets[6]].data[15];
		data[7][0] = HBM_channelA7[offsets[7]].data[0];
		data[7][1] = HBM_channelA7[offsets[7]].data[1];
		data[7][2] = HBM_channelA7[offsets[7]].data[2];
		data[7][3] = HBM_channelA7[offsets[7]].data[3];
		data[7][4] = HBM_channelA7[offsets[7]].data[4];
		data[7][5] = HBM_channelA7[offsets[7]].data[5];
		data[7][6] = HBM_channelA7[offsets[7]].data[6];
		data[7][7] = HBM_channelA7[offsets[7]].data[7];
		data[7][8] = HBM_channelA7[offsets[7]].data[8];
		data[7][9] = HBM_channelA7[offsets[7]].data[9];
		data[7][10] = HBM_channelA7[offsets[7]].data[10];
		data[7][11] = HBM_channelA7[offsets[7]].data[11];
		data[7][12] = HBM_channelA7[offsets[7]].data[12];
		data[7][13] = HBM_channelA7[offsets[7]].data[13];
		data[7][14] = HBM_channelA7[offsets[7]].data[14];
		data[7][15] = HBM_channelA7[offsets[7]].data[15];
		data[7][16] = HBM_channelB7[offsets[7]].data[0];
		data[7][17] = HBM_channelB7[offsets[7]].data[1];
		data[7][18] = HBM_channelB7[offsets[7]].data[2];
		data[7][19] = HBM_channelB7[offsets[7]].data[3];
		data[7][20] = HBM_channelB7[offsets[7]].data[4];
		data[7][21] = HBM_channelB7[offsets[7]].data[5];
		data[7][22] = HBM_channelB7[offsets[7]].data[6];
		data[7][23] = HBM_channelB7[offsets[7]].data[7];
		data[7][24] = HBM_channelB7[offsets[7]].data[8];
		data[7][25] = HBM_channelB7[offsets[7]].data[9];
		data[7][26] = HBM_channelB7[offsets[7]].data[10];
		data[7][27] = HBM_channelB7[offsets[7]].data[11];
		data[7][28] = HBM_channelB7[offsets[7]].data[12];
		data[7][29] = HBM_channelB7[offsets[7]].data[13];
		data[7][30] = HBM_channelB7[offsets[7]].data[14];
		data[7][31] = HBM_channelB7[offsets[7]].data[15];
		data[8][0] = HBM_channelA8[offsets[8]].data[0];
		data[8][1] = HBM_channelA8[offsets[8]].data[1];
		data[8][2] = HBM_channelA8[offsets[8]].data[2];
		data[8][3] = HBM_channelA8[offsets[8]].data[3];
		data[8][4] = HBM_channelA8[offsets[8]].data[4];
		data[8][5] = HBM_channelA8[offsets[8]].data[5];
		data[8][6] = HBM_channelA8[offsets[8]].data[6];
		data[8][7] = HBM_channelA8[offsets[8]].data[7];
		data[8][8] = HBM_channelA8[offsets[8]].data[8];
		data[8][9] = HBM_channelA8[offsets[8]].data[9];
		data[8][10] = HBM_channelA8[offsets[8]].data[10];
		data[8][11] = HBM_channelA8[offsets[8]].data[11];
		data[8][12] = HBM_channelA8[offsets[8]].data[12];
		data[8][13] = HBM_channelA8[offsets[8]].data[13];
		data[8][14] = HBM_channelA8[offsets[8]].data[14];
		data[8][15] = HBM_channelA8[offsets[8]].data[15];
		data[8][16] = HBM_channelB8[offsets[8]].data[0];
		data[8][17] = HBM_channelB8[offsets[8]].data[1];
		data[8][18] = HBM_channelB8[offsets[8]].data[2];
		data[8][19] = HBM_channelB8[offsets[8]].data[3];
		data[8][20] = HBM_channelB8[offsets[8]].data[4];
		data[8][21] = HBM_channelB8[offsets[8]].data[5];
		data[8][22] = HBM_channelB8[offsets[8]].data[6];
		data[8][23] = HBM_channelB8[offsets[8]].data[7];
		data[8][24] = HBM_channelB8[offsets[8]].data[8];
		data[8][25] = HBM_channelB8[offsets[8]].data[9];
		data[8][26] = HBM_channelB8[offsets[8]].data[10];
		data[8][27] = HBM_channelB8[offsets[8]].data[11];
		data[8][28] = HBM_channelB8[offsets[8]].data[12];
		data[8][29] = HBM_channelB8[offsets[8]].data[13];
		data[8][30] = HBM_channelB8[offsets[8]].data[14];
		data[8][31] = HBM_channelB8[offsets[8]].data[15];
		data[9][0] = HBM_channelA9[offsets[9]].data[0];
		data[9][1] = HBM_channelA9[offsets[9]].data[1];
		data[9][2] = HBM_channelA9[offsets[9]].data[2];
		data[9][3] = HBM_channelA9[offsets[9]].data[3];
		data[9][4] = HBM_channelA9[offsets[9]].data[4];
		data[9][5] = HBM_channelA9[offsets[9]].data[5];
		data[9][6] = HBM_channelA9[offsets[9]].data[6];
		data[9][7] = HBM_channelA9[offsets[9]].data[7];
		data[9][8] = HBM_channelA9[offsets[9]].data[8];
		data[9][9] = HBM_channelA9[offsets[9]].data[9];
		data[9][10] = HBM_channelA9[offsets[9]].data[10];
		data[9][11] = HBM_channelA9[offsets[9]].data[11];
		data[9][12] = HBM_channelA9[offsets[9]].data[12];
		data[9][13] = HBM_channelA9[offsets[9]].data[13];
		data[9][14] = HBM_channelA9[offsets[9]].data[14];
		data[9][15] = HBM_channelA9[offsets[9]].data[15];
		data[9][16] = HBM_channelB9[offsets[9]].data[0];
		data[9][17] = HBM_channelB9[offsets[9]].data[1];
		data[9][18] = HBM_channelB9[offsets[9]].data[2];
		data[9][19] = HBM_channelB9[offsets[9]].data[3];
		data[9][20] = HBM_channelB9[offsets[9]].data[4];
		data[9][21] = HBM_channelB9[offsets[9]].data[5];
		data[9][22] = HBM_channelB9[offsets[9]].data[6];
		data[9][23] = HBM_channelB9[offsets[9]].data[7];
		data[9][24] = HBM_channelB9[offsets[9]].data[8];
		data[9][25] = HBM_channelB9[offsets[9]].data[9];
		data[9][26] = HBM_channelB9[offsets[9]].data[10];
		data[9][27] = HBM_channelB9[offsets[9]].data[11];
		data[9][28] = HBM_channelB9[offsets[9]].data[12];
		data[9][29] = HBM_channelB9[offsets[9]].data[13];
		data[9][30] = HBM_channelB9[offsets[9]].data[14];
		data[9][31] = HBM_channelB9[offsets[9]].data[15];
		data[10][0] = HBM_channelA10[offsets[10]].data[0];
		data[10][1] = HBM_channelA10[offsets[10]].data[1];
		data[10][2] = HBM_channelA10[offsets[10]].data[2];
		data[10][3] = HBM_channelA10[offsets[10]].data[3];
		data[10][4] = HBM_channelA10[offsets[10]].data[4];
		data[10][5] = HBM_channelA10[offsets[10]].data[5];
		data[10][6] = HBM_channelA10[offsets[10]].data[6];
		data[10][7] = HBM_channelA10[offsets[10]].data[7];
		data[10][8] = HBM_channelA10[offsets[10]].data[8];
		data[10][9] = HBM_channelA10[offsets[10]].data[9];
		data[10][10] = HBM_channelA10[offsets[10]].data[10];
		data[10][11] = HBM_channelA10[offsets[10]].data[11];
		data[10][12] = HBM_channelA10[offsets[10]].data[12];
		data[10][13] = HBM_channelA10[offsets[10]].data[13];
		data[10][14] = HBM_channelA10[offsets[10]].data[14];
		data[10][15] = HBM_channelA10[offsets[10]].data[15];
		data[10][16] = HBM_channelB10[offsets[10]].data[0];
		data[10][17] = HBM_channelB10[offsets[10]].data[1];
		data[10][18] = HBM_channelB10[offsets[10]].data[2];
		data[10][19] = HBM_channelB10[offsets[10]].data[3];
		data[10][20] = HBM_channelB10[offsets[10]].data[4];
		data[10][21] = HBM_channelB10[offsets[10]].data[5];
		data[10][22] = HBM_channelB10[offsets[10]].data[6];
		data[10][23] = HBM_channelB10[offsets[10]].data[7];
		data[10][24] = HBM_channelB10[offsets[10]].data[8];
		data[10][25] = HBM_channelB10[offsets[10]].data[9];
		data[10][26] = HBM_channelB10[offsets[10]].data[10];
		data[10][27] = HBM_channelB10[offsets[10]].data[11];
		data[10][28] = HBM_channelB10[offsets[10]].data[12];
		data[10][29] = HBM_channelB10[offsets[10]].data[13];
		data[10][30] = HBM_channelB10[offsets[10]].data[14];
		data[10][31] = HBM_channelB10[offsets[10]].data[15];
		data[11][0] = HBM_channelA11[offsets[11]].data[0];
		data[11][1] = HBM_channelA11[offsets[11]].data[1];
		data[11][2] = HBM_channelA11[offsets[11]].data[2];
		data[11][3] = HBM_channelA11[offsets[11]].data[3];
		data[11][4] = HBM_channelA11[offsets[11]].data[4];
		data[11][5] = HBM_channelA11[offsets[11]].data[5];
		data[11][6] = HBM_channelA11[offsets[11]].data[6];
		data[11][7] = HBM_channelA11[offsets[11]].data[7];
		data[11][8] = HBM_channelA11[offsets[11]].data[8];
		data[11][9] = HBM_channelA11[offsets[11]].data[9];
		data[11][10] = HBM_channelA11[offsets[11]].data[10];
		data[11][11] = HBM_channelA11[offsets[11]].data[11];
		data[11][12] = HBM_channelA11[offsets[11]].data[12];
		data[11][13] = HBM_channelA11[offsets[11]].data[13];
		data[11][14] = HBM_channelA11[offsets[11]].data[14];
		data[11][15] = HBM_channelA11[offsets[11]].data[15];
		data[11][16] = HBM_channelB11[offsets[11]].data[0];
		data[11][17] = HBM_channelB11[offsets[11]].data[1];
		data[11][18] = HBM_channelB11[offsets[11]].data[2];
		data[11][19] = HBM_channelB11[offsets[11]].data[3];
		data[11][20] = HBM_channelB11[offsets[11]].data[4];
		data[11][21] = HBM_channelB11[offsets[11]].data[5];
		data[11][22] = HBM_channelB11[offsets[11]].data[6];
		data[11][23] = HBM_channelB11[offsets[11]].data[7];
		data[11][24] = HBM_channelB11[offsets[11]].data[8];
		data[11][25] = HBM_channelB11[offsets[11]].data[9];
		data[11][26] = HBM_channelB11[offsets[11]].data[10];
		data[11][27] = HBM_channelB11[offsets[11]].data[11];
		data[11][28] = HBM_channelB11[offsets[11]].data[12];
		data[11][29] = HBM_channelB11[offsets[11]].data[13];
		data[11][30] = HBM_channelB11[offsets[11]].data[14];
		data[11][31] = HBM_channelB11[offsets[11]].data[15];
		data[12][0] = HBM_channelA12[offsets[12]].data[0];
		data[12][1] = HBM_channelA12[offsets[12]].data[1];
		data[12][2] = HBM_channelA12[offsets[12]].data[2];
		data[12][3] = HBM_channelA12[offsets[12]].data[3];
		data[12][4] = HBM_channelA12[offsets[12]].data[4];
		data[12][5] = HBM_channelA12[offsets[12]].data[5];
		data[12][6] = HBM_channelA12[offsets[12]].data[6];
		data[12][7] = HBM_channelA12[offsets[12]].data[7];
		data[12][8] = HBM_channelA12[offsets[12]].data[8];
		data[12][9] = HBM_channelA12[offsets[12]].data[9];
		data[12][10] = HBM_channelA12[offsets[12]].data[10];
		data[12][11] = HBM_channelA12[offsets[12]].data[11];
		data[12][12] = HBM_channelA12[offsets[12]].data[12];
		data[12][13] = HBM_channelA12[offsets[12]].data[13];
		data[12][14] = HBM_channelA12[offsets[12]].data[14];
		data[12][15] = HBM_channelA12[offsets[12]].data[15];
		data[12][16] = HBM_channelB12[offsets[12]].data[0];
		data[12][17] = HBM_channelB12[offsets[12]].data[1];
		data[12][18] = HBM_channelB12[offsets[12]].data[2];
		data[12][19] = HBM_channelB12[offsets[12]].data[3];
		data[12][20] = HBM_channelB12[offsets[12]].data[4];
		data[12][21] = HBM_channelB12[offsets[12]].data[5];
		data[12][22] = HBM_channelB12[offsets[12]].data[6];
		data[12][23] = HBM_channelB12[offsets[12]].data[7];
		data[12][24] = HBM_channelB12[offsets[12]].data[8];
		data[12][25] = HBM_channelB12[offsets[12]].data[9];
		data[12][26] = HBM_channelB12[offsets[12]].data[10];
		data[12][27] = HBM_channelB12[offsets[12]].data[11];
		data[12][28] = HBM_channelB12[offsets[12]].data[12];
		data[12][29] = HBM_channelB12[offsets[12]].data[13];
		data[12][30] = HBM_channelB12[offsets[12]].data[14];
		data[12][31] = HBM_channelB12[offsets[12]].data[15];
		data[13][0] = HBM_channelA13[offsets[13]].data[0];
		data[13][1] = HBM_channelA13[offsets[13]].data[1];
		data[13][2] = HBM_channelA13[offsets[13]].data[2];
		data[13][3] = HBM_channelA13[offsets[13]].data[3];
		data[13][4] = HBM_channelA13[offsets[13]].data[4];
		data[13][5] = HBM_channelA13[offsets[13]].data[5];
		data[13][6] = HBM_channelA13[offsets[13]].data[6];
		data[13][7] = HBM_channelA13[offsets[13]].data[7];
		data[13][8] = HBM_channelA13[offsets[13]].data[8];
		data[13][9] = HBM_channelA13[offsets[13]].data[9];
		data[13][10] = HBM_channelA13[offsets[13]].data[10];
		data[13][11] = HBM_channelA13[offsets[13]].data[11];
		data[13][12] = HBM_channelA13[offsets[13]].data[12];
		data[13][13] = HBM_channelA13[offsets[13]].data[13];
		data[13][14] = HBM_channelA13[offsets[13]].data[14];
		data[13][15] = HBM_channelA13[offsets[13]].data[15];
		data[13][16] = HBM_channelB13[offsets[13]].data[0];
		data[13][17] = HBM_channelB13[offsets[13]].data[1];
		data[13][18] = HBM_channelB13[offsets[13]].data[2];
		data[13][19] = HBM_channelB13[offsets[13]].data[3];
		data[13][20] = HBM_channelB13[offsets[13]].data[4];
		data[13][21] = HBM_channelB13[offsets[13]].data[5];
		data[13][22] = HBM_channelB13[offsets[13]].data[6];
		data[13][23] = HBM_channelB13[offsets[13]].data[7];
		data[13][24] = HBM_channelB13[offsets[13]].data[8];
		data[13][25] = HBM_channelB13[offsets[13]].data[9];
		data[13][26] = HBM_channelB13[offsets[13]].data[10];
		data[13][27] = HBM_channelB13[offsets[13]].data[11];
		data[13][28] = HBM_channelB13[offsets[13]].data[12];
		data[13][29] = HBM_channelB13[offsets[13]].data[13];
		data[13][30] = HBM_channelB13[offsets[13]].data[14];
		data[13][31] = HBM_channelB13[offsets[13]].data[15];
		data[14][0] = HBM_channelA14[offsets[14]].data[0];
		data[14][1] = HBM_channelA14[offsets[14]].data[1];
		data[14][2] = HBM_channelA14[offsets[14]].data[2];
		data[14][3] = HBM_channelA14[offsets[14]].data[3];
		data[14][4] = HBM_channelA14[offsets[14]].data[4];
		data[14][5] = HBM_channelA14[offsets[14]].data[5];
		data[14][6] = HBM_channelA14[offsets[14]].data[6];
		data[14][7] = HBM_channelA14[offsets[14]].data[7];
		data[14][8] = HBM_channelA14[offsets[14]].data[8];
		data[14][9] = HBM_channelA14[offsets[14]].data[9];
		data[14][10] = HBM_channelA14[offsets[14]].data[10];
		data[14][11] = HBM_channelA14[offsets[14]].data[11];
		data[14][12] = HBM_channelA14[offsets[14]].data[12];
		data[14][13] = HBM_channelA14[offsets[14]].data[13];
		data[14][14] = HBM_channelA14[offsets[14]].data[14];
		data[14][15] = HBM_channelA14[offsets[14]].data[15];
		data[14][16] = HBM_channelB14[offsets[14]].data[0];
		data[14][17] = HBM_channelB14[offsets[14]].data[1];
		data[14][18] = HBM_channelB14[offsets[14]].data[2];
		data[14][19] = HBM_channelB14[offsets[14]].data[3];
		data[14][20] = HBM_channelB14[offsets[14]].data[4];
		data[14][21] = HBM_channelB14[offsets[14]].data[5];
		data[14][22] = HBM_channelB14[offsets[14]].data[6];
		data[14][23] = HBM_channelB14[offsets[14]].data[7];
		data[14][24] = HBM_channelB14[offsets[14]].data[8];
		data[14][25] = HBM_channelB14[offsets[14]].data[9];
		data[14][26] = HBM_channelB14[offsets[14]].data[10];
		data[14][27] = HBM_channelB14[offsets[14]].data[11];
		data[14][28] = HBM_channelB14[offsets[14]].data[12];
		data[14][29] = HBM_channelB14[offsets[14]].data[13];
		data[14][30] = HBM_channelB14[offsets[14]].data[14];
		data[14][31] = HBM_channelB14[offsets[14]].data[15];
		data[15][0] = HBM_channelA15[offsets[15]].data[0];
		data[15][1] = HBM_channelA15[offsets[15]].data[1];
		data[15][2] = HBM_channelA15[offsets[15]].data[2];
		data[15][3] = HBM_channelA15[offsets[15]].data[3];
		data[15][4] = HBM_channelA15[offsets[15]].data[4];
		data[15][5] = HBM_channelA15[offsets[15]].data[5];
		data[15][6] = HBM_channelA15[offsets[15]].data[6];
		data[15][7] = HBM_channelA15[offsets[15]].data[7];
		data[15][8] = HBM_channelA15[offsets[15]].data[8];
		data[15][9] = HBM_channelA15[offsets[15]].data[9];
		data[15][10] = HBM_channelA15[offsets[15]].data[10];
		data[15][11] = HBM_channelA15[offsets[15]].data[11];
		data[15][12] = HBM_channelA15[offsets[15]].data[12];
		data[15][13] = HBM_channelA15[offsets[15]].data[13];
		data[15][14] = HBM_channelA15[offsets[15]].data[14];
		data[15][15] = HBM_channelA15[offsets[15]].data[15];
		data[15][16] = HBM_channelB15[offsets[15]].data[0];
		data[15][17] = HBM_channelB15[offsets[15]].data[1];
		data[15][18] = HBM_channelB15[offsets[15]].data[2];
		data[15][19] = HBM_channelB15[offsets[15]].data[3];
		data[15][20] = HBM_channelB15[offsets[15]].data[4];
		data[15][21] = HBM_channelB15[offsets[15]].data[5];
		data[15][22] = HBM_channelB15[offsets[15]].data[6];
		data[15][23] = HBM_channelB15[offsets[15]].data[7];
		data[15][24] = HBM_channelB15[offsets[15]].data[8];
		data[15][25] = HBM_channelB15[offsets[15]].data[9];
		data[15][26] = HBM_channelB15[offsets[15]].data[10];
		data[15][27] = HBM_channelB15[offsets[15]].data[11];
		data[15][28] = HBM_channelB15[offsets[15]].data[12];
		data[15][29] = HBM_channelB15[offsets[15]].data[13];
		data[15][30] = HBM_channelB15[offsets[15]].data[14];
		data[15][31] = HBM_channelB15[offsets[15]].data[15];
		data[16][0] = HBM_channelA16[offsets[16]].data[0];
		data[16][1] = HBM_channelA16[offsets[16]].data[1];
		data[16][2] = HBM_channelA16[offsets[16]].data[2];
		data[16][3] = HBM_channelA16[offsets[16]].data[3];
		data[16][4] = HBM_channelA16[offsets[16]].data[4];
		data[16][5] = HBM_channelA16[offsets[16]].data[5];
		data[16][6] = HBM_channelA16[offsets[16]].data[6];
		data[16][7] = HBM_channelA16[offsets[16]].data[7];
		data[16][8] = HBM_channelA16[offsets[16]].data[8];
		data[16][9] = HBM_channelA16[offsets[16]].data[9];
		data[16][10] = HBM_channelA16[offsets[16]].data[10];
		data[16][11] = HBM_channelA16[offsets[16]].data[11];
		data[16][12] = HBM_channelA16[offsets[16]].data[12];
		data[16][13] = HBM_channelA16[offsets[16]].data[13];
		data[16][14] = HBM_channelA16[offsets[16]].data[14];
		data[16][15] = HBM_channelA16[offsets[16]].data[15];
		data[16][16] = HBM_channelB16[offsets[16]].data[0];
		data[16][17] = HBM_channelB16[offsets[16]].data[1];
		data[16][18] = HBM_channelB16[offsets[16]].data[2];
		data[16][19] = HBM_channelB16[offsets[16]].data[3];
		data[16][20] = HBM_channelB16[offsets[16]].data[4];
		data[16][21] = HBM_channelB16[offsets[16]].data[5];
		data[16][22] = HBM_channelB16[offsets[16]].data[6];
		data[16][23] = HBM_channelB16[offsets[16]].data[7];
		data[16][24] = HBM_channelB16[offsets[16]].data[8];
		data[16][25] = HBM_channelB16[offsets[16]].data[9];
		data[16][26] = HBM_channelB16[offsets[16]].data[10];
		data[16][27] = HBM_channelB16[offsets[16]].data[11];
		data[16][28] = HBM_channelB16[offsets[16]].data[12];
		data[16][29] = HBM_channelB16[offsets[16]].data[13];
		data[16][30] = HBM_channelB16[offsets[16]].data[14];
		data[16][31] = HBM_channelB16[offsets[16]].data[15];
		data[17][0] = HBM_channelA17[offsets[17]].data[0];
		data[17][1] = HBM_channelA17[offsets[17]].data[1];
		data[17][2] = HBM_channelA17[offsets[17]].data[2];
		data[17][3] = HBM_channelA17[offsets[17]].data[3];
		data[17][4] = HBM_channelA17[offsets[17]].data[4];
		data[17][5] = HBM_channelA17[offsets[17]].data[5];
		data[17][6] = HBM_channelA17[offsets[17]].data[6];
		data[17][7] = HBM_channelA17[offsets[17]].data[7];
		data[17][8] = HBM_channelA17[offsets[17]].data[8];
		data[17][9] = HBM_channelA17[offsets[17]].data[9];
		data[17][10] = HBM_channelA17[offsets[17]].data[10];
		data[17][11] = HBM_channelA17[offsets[17]].data[11];
		data[17][12] = HBM_channelA17[offsets[17]].data[12];
		data[17][13] = HBM_channelA17[offsets[17]].data[13];
		data[17][14] = HBM_channelA17[offsets[17]].data[14];
		data[17][15] = HBM_channelA17[offsets[17]].data[15];
		data[17][16] = HBM_channelB17[offsets[17]].data[0];
		data[17][17] = HBM_channelB17[offsets[17]].data[1];
		data[17][18] = HBM_channelB17[offsets[17]].data[2];
		data[17][19] = HBM_channelB17[offsets[17]].data[3];
		data[17][20] = HBM_channelB17[offsets[17]].data[4];
		data[17][21] = HBM_channelB17[offsets[17]].data[5];
		data[17][22] = HBM_channelB17[offsets[17]].data[6];
		data[17][23] = HBM_channelB17[offsets[17]].data[7];
		data[17][24] = HBM_channelB17[offsets[17]].data[8];
		data[17][25] = HBM_channelB17[offsets[17]].data[9];
		data[17][26] = HBM_channelB17[offsets[17]].data[10];
		data[17][27] = HBM_channelB17[offsets[17]].data[11];
		data[17][28] = HBM_channelB17[offsets[17]].data[12];
		data[17][29] = HBM_channelB17[offsets[17]].data[13];
		data[17][30] = HBM_channelB17[offsets[17]].data[14];
		data[17][31] = HBM_channelB17[offsets[17]].data[15];
		data[18][0] = HBM_channelA18[offsets[18]].data[0];
		data[18][1] = HBM_channelA18[offsets[18]].data[1];
		data[18][2] = HBM_channelA18[offsets[18]].data[2];
		data[18][3] = HBM_channelA18[offsets[18]].data[3];
		data[18][4] = HBM_channelA18[offsets[18]].data[4];
		data[18][5] = HBM_channelA18[offsets[18]].data[5];
		data[18][6] = HBM_channelA18[offsets[18]].data[6];
		data[18][7] = HBM_channelA18[offsets[18]].data[7];
		data[18][8] = HBM_channelA18[offsets[18]].data[8];
		data[18][9] = HBM_channelA18[offsets[18]].data[9];
		data[18][10] = HBM_channelA18[offsets[18]].data[10];
		data[18][11] = HBM_channelA18[offsets[18]].data[11];
		data[18][12] = HBM_channelA18[offsets[18]].data[12];
		data[18][13] = HBM_channelA18[offsets[18]].data[13];
		data[18][14] = HBM_channelA18[offsets[18]].data[14];
		data[18][15] = HBM_channelA18[offsets[18]].data[15];
		data[18][16] = HBM_channelB18[offsets[18]].data[0];
		data[18][17] = HBM_channelB18[offsets[18]].data[1];
		data[18][18] = HBM_channelB18[offsets[18]].data[2];
		data[18][19] = HBM_channelB18[offsets[18]].data[3];
		data[18][20] = HBM_channelB18[offsets[18]].data[4];
		data[18][21] = HBM_channelB18[offsets[18]].data[5];
		data[18][22] = HBM_channelB18[offsets[18]].data[6];
		data[18][23] = HBM_channelB18[offsets[18]].data[7];
		data[18][24] = HBM_channelB18[offsets[18]].data[8];
		data[18][25] = HBM_channelB18[offsets[18]].data[9];
		data[18][26] = HBM_channelB18[offsets[18]].data[10];
		data[18][27] = HBM_channelB18[offsets[18]].data[11];
		data[18][28] = HBM_channelB18[offsets[18]].data[12];
		data[18][29] = HBM_channelB18[offsets[18]].data[13];
		data[18][30] = HBM_channelB18[offsets[18]].data[14];
		data[18][31] = HBM_channelB18[offsets[18]].data[15];
		data[19][0] = HBM_channelA19[offsets[19]].data[0];
		data[19][1] = HBM_channelA19[offsets[19]].data[1];
		data[19][2] = HBM_channelA19[offsets[19]].data[2];
		data[19][3] = HBM_channelA19[offsets[19]].data[3];
		data[19][4] = HBM_channelA19[offsets[19]].data[4];
		data[19][5] = HBM_channelA19[offsets[19]].data[5];
		data[19][6] = HBM_channelA19[offsets[19]].data[6];
		data[19][7] = HBM_channelA19[offsets[19]].data[7];
		data[19][8] = HBM_channelA19[offsets[19]].data[8];
		data[19][9] = HBM_channelA19[offsets[19]].data[9];
		data[19][10] = HBM_channelA19[offsets[19]].data[10];
		data[19][11] = HBM_channelA19[offsets[19]].data[11];
		data[19][12] = HBM_channelA19[offsets[19]].data[12];
		data[19][13] = HBM_channelA19[offsets[19]].data[13];
		data[19][14] = HBM_channelA19[offsets[19]].data[14];
		data[19][15] = HBM_channelA19[offsets[19]].data[15];
		data[19][16] = HBM_channelB19[offsets[19]].data[0];
		data[19][17] = HBM_channelB19[offsets[19]].data[1];
		data[19][18] = HBM_channelB19[offsets[19]].data[2];
		data[19][19] = HBM_channelB19[offsets[19]].data[3];
		data[19][20] = HBM_channelB19[offsets[19]].data[4];
		data[19][21] = HBM_channelB19[offsets[19]].data[5];
		data[19][22] = HBM_channelB19[offsets[19]].data[6];
		data[19][23] = HBM_channelB19[offsets[19]].data[7];
		data[19][24] = HBM_channelB19[offsets[19]].data[8];
		data[19][25] = HBM_channelB19[offsets[19]].data[9];
		data[19][26] = HBM_channelB19[offsets[19]].data[10];
		data[19][27] = HBM_channelB19[offsets[19]].data[11];
		data[19][28] = HBM_channelB19[offsets[19]].data[12];
		data[19][29] = HBM_channelB19[offsets[19]].data[13];
		data[19][30] = HBM_channelB19[offsets[19]].data[14];
		data[19][31] = HBM_channelB19[offsets[19]].data[15];
		data[20][0] = HBM_channelA20[offsets[20]].data[0];
		data[20][1] = HBM_channelA20[offsets[20]].data[1];
		data[20][2] = HBM_channelA20[offsets[20]].data[2];
		data[20][3] = HBM_channelA20[offsets[20]].data[3];
		data[20][4] = HBM_channelA20[offsets[20]].data[4];
		data[20][5] = HBM_channelA20[offsets[20]].data[5];
		data[20][6] = HBM_channelA20[offsets[20]].data[6];
		data[20][7] = HBM_channelA20[offsets[20]].data[7];
		data[20][8] = HBM_channelA20[offsets[20]].data[8];
		data[20][9] = HBM_channelA20[offsets[20]].data[9];
		data[20][10] = HBM_channelA20[offsets[20]].data[10];
		data[20][11] = HBM_channelA20[offsets[20]].data[11];
		data[20][12] = HBM_channelA20[offsets[20]].data[12];
		data[20][13] = HBM_channelA20[offsets[20]].data[13];
		data[20][14] = HBM_channelA20[offsets[20]].data[14];
		data[20][15] = HBM_channelA20[offsets[20]].data[15];
		data[20][16] = HBM_channelB20[offsets[20]].data[0];
		data[20][17] = HBM_channelB20[offsets[20]].data[1];
		data[20][18] = HBM_channelB20[offsets[20]].data[2];
		data[20][19] = HBM_channelB20[offsets[20]].data[3];
		data[20][20] = HBM_channelB20[offsets[20]].data[4];
		data[20][21] = HBM_channelB20[offsets[20]].data[5];
		data[20][22] = HBM_channelB20[offsets[20]].data[6];
		data[20][23] = HBM_channelB20[offsets[20]].data[7];
		data[20][24] = HBM_channelB20[offsets[20]].data[8];
		data[20][25] = HBM_channelB20[offsets[20]].data[9];
		data[20][26] = HBM_channelB20[offsets[20]].data[10];
		data[20][27] = HBM_channelB20[offsets[20]].data[11];
		data[20][28] = HBM_channelB20[offsets[20]].data[12];
		data[20][29] = HBM_channelB20[offsets[20]].data[13];
		data[20][30] = HBM_channelB20[offsets[20]].data[14];
		data[20][31] = HBM_channelB20[offsets[20]].data[15];
		data[21][0] = HBM_channelA21[offsets[21]].data[0];
		data[21][1] = HBM_channelA21[offsets[21]].data[1];
		data[21][2] = HBM_channelA21[offsets[21]].data[2];
		data[21][3] = HBM_channelA21[offsets[21]].data[3];
		data[21][4] = HBM_channelA21[offsets[21]].data[4];
		data[21][5] = HBM_channelA21[offsets[21]].data[5];
		data[21][6] = HBM_channelA21[offsets[21]].data[6];
		data[21][7] = HBM_channelA21[offsets[21]].data[7];
		data[21][8] = HBM_channelA21[offsets[21]].data[8];
		data[21][9] = HBM_channelA21[offsets[21]].data[9];
		data[21][10] = HBM_channelA21[offsets[21]].data[10];
		data[21][11] = HBM_channelA21[offsets[21]].data[11];
		data[21][12] = HBM_channelA21[offsets[21]].data[12];
		data[21][13] = HBM_channelA21[offsets[21]].data[13];
		data[21][14] = HBM_channelA21[offsets[21]].data[14];
		data[21][15] = HBM_channelA21[offsets[21]].data[15];
		data[21][16] = HBM_channelB21[offsets[21]].data[0];
		data[21][17] = HBM_channelB21[offsets[21]].data[1];
		data[21][18] = HBM_channelB21[offsets[21]].data[2];
		data[21][19] = HBM_channelB21[offsets[21]].data[3];
		data[21][20] = HBM_channelB21[offsets[21]].data[4];
		data[21][21] = HBM_channelB21[offsets[21]].data[5];
		data[21][22] = HBM_channelB21[offsets[21]].data[6];
		data[21][23] = HBM_channelB21[offsets[21]].data[7];
		data[21][24] = HBM_channelB21[offsets[21]].data[8];
		data[21][25] = HBM_channelB21[offsets[21]].data[9];
		data[21][26] = HBM_channelB21[offsets[21]].data[10];
		data[21][27] = HBM_channelB21[offsets[21]].data[11];
		data[21][28] = HBM_channelB21[offsets[21]].data[12];
		data[21][29] = HBM_channelB21[offsets[21]].data[13];
		data[21][30] = HBM_channelB21[offsets[21]].data[14];
		data[21][31] = HBM_channelB21[offsets[21]].data[15];
		data[22][0] = HBM_channelA22[offsets[22]].data[0];
		data[22][1] = HBM_channelA22[offsets[22]].data[1];
		data[22][2] = HBM_channelA22[offsets[22]].data[2];
		data[22][3] = HBM_channelA22[offsets[22]].data[3];
		data[22][4] = HBM_channelA22[offsets[22]].data[4];
		data[22][5] = HBM_channelA22[offsets[22]].data[5];
		data[22][6] = HBM_channelA22[offsets[22]].data[6];
		data[22][7] = HBM_channelA22[offsets[22]].data[7];
		data[22][8] = HBM_channelA22[offsets[22]].data[8];
		data[22][9] = HBM_channelA22[offsets[22]].data[9];
		data[22][10] = HBM_channelA22[offsets[22]].data[10];
		data[22][11] = HBM_channelA22[offsets[22]].data[11];
		data[22][12] = HBM_channelA22[offsets[22]].data[12];
		data[22][13] = HBM_channelA22[offsets[22]].data[13];
		data[22][14] = HBM_channelA22[offsets[22]].data[14];
		data[22][15] = HBM_channelA22[offsets[22]].data[15];
		data[22][16] = HBM_channelB22[offsets[22]].data[0];
		data[22][17] = HBM_channelB22[offsets[22]].data[1];
		data[22][18] = HBM_channelB22[offsets[22]].data[2];
		data[22][19] = HBM_channelB22[offsets[22]].data[3];
		data[22][20] = HBM_channelB22[offsets[22]].data[4];
		data[22][21] = HBM_channelB22[offsets[22]].data[5];
		data[22][22] = HBM_channelB22[offsets[22]].data[6];
		data[22][23] = HBM_channelB22[offsets[22]].data[7];
		data[22][24] = HBM_channelB22[offsets[22]].data[8];
		data[22][25] = HBM_channelB22[offsets[22]].data[9];
		data[22][26] = HBM_channelB22[offsets[22]].data[10];
		data[22][27] = HBM_channelB22[offsets[22]].data[11];
		data[22][28] = HBM_channelB22[offsets[22]].data[12];
		data[22][29] = HBM_channelB22[offsets[22]].data[13];
		data[22][30] = HBM_channelB22[offsets[22]].data[14];
		data[22][31] = HBM_channelB22[offsets[22]].data[15];
		data[23][0] = HBM_channelA23[offsets[23]].data[0];
		data[23][1] = HBM_channelA23[offsets[23]].data[1];
		data[23][2] = HBM_channelA23[offsets[23]].data[2];
		data[23][3] = HBM_channelA23[offsets[23]].data[3];
		data[23][4] = HBM_channelA23[offsets[23]].data[4];
		data[23][5] = HBM_channelA23[offsets[23]].data[5];
		data[23][6] = HBM_channelA23[offsets[23]].data[6];
		data[23][7] = HBM_channelA23[offsets[23]].data[7];
		data[23][8] = HBM_channelA23[offsets[23]].data[8];
		data[23][9] = HBM_channelA23[offsets[23]].data[9];
		data[23][10] = HBM_channelA23[offsets[23]].data[10];
		data[23][11] = HBM_channelA23[offsets[23]].data[11];
		data[23][12] = HBM_channelA23[offsets[23]].data[12];
		data[23][13] = HBM_channelA23[offsets[23]].data[13];
		data[23][14] = HBM_channelA23[offsets[23]].data[14];
		data[23][15] = HBM_channelA23[offsets[23]].data[15];
		data[23][16] = HBM_channelB23[offsets[23]].data[0];
		data[23][17] = HBM_channelB23[offsets[23]].data[1];
		data[23][18] = HBM_channelB23[offsets[23]].data[2];
		data[23][19] = HBM_channelB23[offsets[23]].data[3];
		data[23][20] = HBM_channelB23[offsets[23]].data[4];
		data[23][21] = HBM_channelB23[offsets[23]].data[5];
		data[23][22] = HBM_channelB23[offsets[23]].data[6];
		data[23][23] = HBM_channelB23[offsets[23]].data[7];
		data[23][24] = HBM_channelB23[offsets[23]].data[8];
		data[23][25] = HBM_channelB23[offsets[23]].data[9];
		data[23][26] = HBM_channelB23[offsets[23]].data[10];
		data[23][27] = HBM_channelB23[offsets[23]].data[11];
		data[23][28] = HBM_channelB23[offsets[23]].data[12];
		data[23][29] = HBM_channelB23[offsets[23]].data[13];
		data[23][30] = HBM_channelB23[offsets[23]].data[14];
		data[23][31] = HBM_channelB23[offsets[23]].data[15];
		#else 
		data[0][0] = HBM_channelA0[offsets[0]].data[0];
		data[0][1] = HBM_channelA0[offsets[0]].data[1];
		data[0][2] = HBM_channelA0[offsets[0]].data[2];
		data[0][3] = HBM_channelA0[offsets[0]].data[3];
		data[0][4] = HBM_channelA0[offsets[0]].data[4];
		data[0][5] = HBM_channelA0[offsets[0]].data[5];
		data[0][6] = HBM_channelA0[offsets[0]].data[6];
		data[0][7] = HBM_channelA0[offsets[0]].data[7];
		data[0][8] = HBM_channelA0[offsets[0]].data[8];
		data[0][9] = HBM_channelA0[offsets[0]].data[9];
		data[0][10] = HBM_channelA0[offsets[0]].data[10];
		data[0][11] = HBM_channelA0[offsets[0]].data[11];
		data[0][12] = HBM_channelA0[offsets[0]].data[12];
		data[0][13] = HBM_channelA0[offsets[0]].data[13];
		data[0][14] = HBM_channelA0[offsets[0]].data[14];
		data[0][15] = HBM_channelA0[offsets[0]].data[15];
		data[0][16] = HBM_channelA0[offsets[0]].data[16];
		data[0][17] = HBM_channelA0[offsets[0]].data[17];
		data[0][18] = HBM_channelA0[offsets[0]].data[18];
		data[0][19] = HBM_channelA0[offsets[0]].data[19];
		data[0][20] = HBM_channelA0[offsets[0]].data[20];
		data[0][21] = HBM_channelA0[offsets[0]].data[21];
		data[0][22] = HBM_channelA0[offsets[0]].data[22];
		data[0][23] = HBM_channelA0[offsets[0]].data[23];
		data[0][24] = HBM_channelA0[offsets[0]].data[24];
		data[0][25] = HBM_channelA0[offsets[0]].data[25];
		data[0][26] = HBM_channelA0[offsets[0]].data[26];
		data[0][27] = HBM_channelA0[offsets[0]].data[27];
		data[0][28] = HBM_channelA0[offsets[0]].data[28];
		data[0][29] = HBM_channelA0[offsets[0]].data[29];
		data[0][30] = HBM_channelA0[offsets[0]].data[30];
		data[0][31] = HBM_channelA0[offsets[0]].data[31];
		data[1][0] = HBM_channelA1[offsets[1]].data[0];
		data[1][1] = HBM_channelA1[offsets[1]].data[1];
		data[1][2] = HBM_channelA1[offsets[1]].data[2];
		data[1][3] = HBM_channelA1[offsets[1]].data[3];
		data[1][4] = HBM_channelA1[offsets[1]].data[4];
		data[1][5] = HBM_channelA1[offsets[1]].data[5];
		data[1][6] = HBM_channelA1[offsets[1]].data[6];
		data[1][7] = HBM_channelA1[offsets[1]].data[7];
		data[1][8] = HBM_channelA1[offsets[1]].data[8];
		data[1][9] = HBM_channelA1[offsets[1]].data[9];
		data[1][10] = HBM_channelA1[offsets[1]].data[10];
		data[1][11] = HBM_channelA1[offsets[1]].data[11];
		data[1][12] = HBM_channelA1[offsets[1]].data[12];
		data[1][13] = HBM_channelA1[offsets[1]].data[13];
		data[1][14] = HBM_channelA1[offsets[1]].data[14];
		data[1][15] = HBM_channelA1[offsets[1]].data[15];
		data[1][16] = HBM_channelA1[offsets[1]].data[16];
		data[1][17] = HBM_channelA1[offsets[1]].data[17];
		data[1][18] = HBM_channelA1[offsets[1]].data[18];
		data[1][19] = HBM_channelA1[offsets[1]].data[19];
		data[1][20] = HBM_channelA1[offsets[1]].data[20];
		data[1][21] = HBM_channelA1[offsets[1]].data[21];
		data[1][22] = HBM_channelA1[offsets[1]].data[22];
		data[1][23] = HBM_channelA1[offsets[1]].data[23];
		data[1][24] = HBM_channelA1[offsets[1]].data[24];
		data[1][25] = HBM_channelA1[offsets[1]].data[25];
		data[1][26] = HBM_channelA1[offsets[1]].data[26];
		data[1][27] = HBM_channelA1[offsets[1]].data[27];
		data[1][28] = HBM_channelA1[offsets[1]].data[28];
		data[1][29] = HBM_channelA1[offsets[1]].data[29];
		data[1][30] = HBM_channelA1[offsets[1]].data[30];
		data[1][31] = HBM_channelA1[offsets[1]].data[31];
		data[2][0] = HBM_channelA2[offsets[2]].data[0];
		data[2][1] = HBM_channelA2[offsets[2]].data[1];
		data[2][2] = HBM_channelA2[offsets[2]].data[2];
		data[2][3] = HBM_channelA2[offsets[2]].data[3];
		data[2][4] = HBM_channelA2[offsets[2]].data[4];
		data[2][5] = HBM_channelA2[offsets[2]].data[5];
		data[2][6] = HBM_channelA2[offsets[2]].data[6];
		data[2][7] = HBM_channelA2[offsets[2]].data[7];
		data[2][8] = HBM_channelA2[offsets[2]].data[8];
		data[2][9] = HBM_channelA2[offsets[2]].data[9];
		data[2][10] = HBM_channelA2[offsets[2]].data[10];
		data[2][11] = HBM_channelA2[offsets[2]].data[11];
		data[2][12] = HBM_channelA2[offsets[2]].data[12];
		data[2][13] = HBM_channelA2[offsets[2]].data[13];
		data[2][14] = HBM_channelA2[offsets[2]].data[14];
		data[2][15] = HBM_channelA2[offsets[2]].data[15];
		data[2][16] = HBM_channelA2[offsets[2]].data[16];
		data[2][17] = HBM_channelA2[offsets[2]].data[17];
		data[2][18] = HBM_channelA2[offsets[2]].data[18];
		data[2][19] = HBM_channelA2[offsets[2]].data[19];
		data[2][20] = HBM_channelA2[offsets[2]].data[20];
		data[2][21] = HBM_channelA2[offsets[2]].data[21];
		data[2][22] = HBM_channelA2[offsets[2]].data[22];
		data[2][23] = HBM_channelA2[offsets[2]].data[23];
		data[2][24] = HBM_channelA2[offsets[2]].data[24];
		data[2][25] = HBM_channelA2[offsets[2]].data[25];
		data[2][26] = HBM_channelA2[offsets[2]].data[26];
		data[2][27] = HBM_channelA2[offsets[2]].data[27];
		data[2][28] = HBM_channelA2[offsets[2]].data[28];
		data[2][29] = HBM_channelA2[offsets[2]].data[29];
		data[2][30] = HBM_channelA2[offsets[2]].data[30];
		data[2][31] = HBM_channelA2[offsets[2]].data[31];
		data[3][0] = HBM_channelA3[offsets[3]].data[0];
		data[3][1] = HBM_channelA3[offsets[3]].data[1];
		data[3][2] = HBM_channelA3[offsets[3]].data[2];
		data[3][3] = HBM_channelA3[offsets[3]].data[3];
		data[3][4] = HBM_channelA3[offsets[3]].data[4];
		data[3][5] = HBM_channelA3[offsets[3]].data[5];
		data[3][6] = HBM_channelA3[offsets[3]].data[6];
		data[3][7] = HBM_channelA3[offsets[3]].data[7];
		data[3][8] = HBM_channelA3[offsets[3]].data[8];
		data[3][9] = HBM_channelA3[offsets[3]].data[9];
		data[3][10] = HBM_channelA3[offsets[3]].data[10];
		data[3][11] = HBM_channelA3[offsets[3]].data[11];
		data[3][12] = HBM_channelA3[offsets[3]].data[12];
		data[3][13] = HBM_channelA3[offsets[3]].data[13];
		data[3][14] = HBM_channelA3[offsets[3]].data[14];
		data[3][15] = HBM_channelA3[offsets[3]].data[15];
		data[3][16] = HBM_channelA3[offsets[3]].data[16];
		data[3][17] = HBM_channelA3[offsets[3]].data[17];
		data[3][18] = HBM_channelA3[offsets[3]].data[18];
		data[3][19] = HBM_channelA3[offsets[3]].data[19];
		data[3][20] = HBM_channelA3[offsets[3]].data[20];
		data[3][21] = HBM_channelA3[offsets[3]].data[21];
		data[3][22] = HBM_channelA3[offsets[3]].data[22];
		data[3][23] = HBM_channelA3[offsets[3]].data[23];
		data[3][24] = HBM_channelA3[offsets[3]].data[24];
		data[3][25] = HBM_channelA3[offsets[3]].data[25];
		data[3][26] = HBM_channelA3[offsets[3]].data[26];
		data[3][27] = HBM_channelA3[offsets[3]].data[27];
		data[3][28] = HBM_channelA3[offsets[3]].data[28];
		data[3][29] = HBM_channelA3[offsets[3]].data[29];
		data[3][30] = HBM_channelA3[offsets[3]].data[30];
		data[3][31] = HBM_channelA3[offsets[3]].data[31];
		data[4][0] = HBM_channelA4[offsets[4]].data[0];
		data[4][1] = HBM_channelA4[offsets[4]].data[1];
		data[4][2] = HBM_channelA4[offsets[4]].data[2];
		data[4][3] = HBM_channelA4[offsets[4]].data[3];
		data[4][4] = HBM_channelA4[offsets[4]].data[4];
		data[4][5] = HBM_channelA4[offsets[4]].data[5];
		data[4][6] = HBM_channelA4[offsets[4]].data[6];
		data[4][7] = HBM_channelA4[offsets[4]].data[7];
		data[4][8] = HBM_channelA4[offsets[4]].data[8];
		data[4][9] = HBM_channelA4[offsets[4]].data[9];
		data[4][10] = HBM_channelA4[offsets[4]].data[10];
		data[4][11] = HBM_channelA4[offsets[4]].data[11];
		data[4][12] = HBM_channelA4[offsets[4]].data[12];
		data[4][13] = HBM_channelA4[offsets[4]].data[13];
		data[4][14] = HBM_channelA4[offsets[4]].data[14];
		data[4][15] = HBM_channelA4[offsets[4]].data[15];
		data[4][16] = HBM_channelA4[offsets[4]].data[16];
		data[4][17] = HBM_channelA4[offsets[4]].data[17];
		data[4][18] = HBM_channelA4[offsets[4]].data[18];
		data[4][19] = HBM_channelA4[offsets[4]].data[19];
		data[4][20] = HBM_channelA4[offsets[4]].data[20];
		data[4][21] = HBM_channelA4[offsets[4]].data[21];
		data[4][22] = HBM_channelA4[offsets[4]].data[22];
		data[4][23] = HBM_channelA4[offsets[4]].data[23];
		data[4][24] = HBM_channelA4[offsets[4]].data[24];
		data[4][25] = HBM_channelA4[offsets[4]].data[25];
		data[4][26] = HBM_channelA4[offsets[4]].data[26];
		data[4][27] = HBM_channelA4[offsets[4]].data[27];
		data[4][28] = HBM_channelA4[offsets[4]].data[28];
		data[4][29] = HBM_channelA4[offsets[4]].data[29];
		data[4][30] = HBM_channelA4[offsets[4]].data[30];
		data[4][31] = HBM_channelA4[offsets[4]].data[31];
		data[5][0] = HBM_channelA5[offsets[5]].data[0];
		data[5][1] = HBM_channelA5[offsets[5]].data[1];
		data[5][2] = HBM_channelA5[offsets[5]].data[2];
		data[5][3] = HBM_channelA5[offsets[5]].data[3];
		data[5][4] = HBM_channelA5[offsets[5]].data[4];
		data[5][5] = HBM_channelA5[offsets[5]].data[5];
		data[5][6] = HBM_channelA5[offsets[5]].data[6];
		data[5][7] = HBM_channelA5[offsets[5]].data[7];
		data[5][8] = HBM_channelA5[offsets[5]].data[8];
		data[5][9] = HBM_channelA5[offsets[5]].data[9];
		data[5][10] = HBM_channelA5[offsets[5]].data[10];
		data[5][11] = HBM_channelA5[offsets[5]].data[11];
		data[5][12] = HBM_channelA5[offsets[5]].data[12];
		data[5][13] = HBM_channelA5[offsets[5]].data[13];
		data[5][14] = HBM_channelA5[offsets[5]].data[14];
		data[5][15] = HBM_channelA5[offsets[5]].data[15];
		data[5][16] = HBM_channelA5[offsets[5]].data[16];
		data[5][17] = HBM_channelA5[offsets[5]].data[17];
		data[5][18] = HBM_channelA5[offsets[5]].data[18];
		data[5][19] = HBM_channelA5[offsets[5]].data[19];
		data[5][20] = HBM_channelA5[offsets[5]].data[20];
		data[5][21] = HBM_channelA5[offsets[5]].data[21];
		data[5][22] = HBM_channelA5[offsets[5]].data[22];
		data[5][23] = HBM_channelA5[offsets[5]].data[23];
		data[5][24] = HBM_channelA5[offsets[5]].data[24];
		data[5][25] = HBM_channelA5[offsets[5]].data[25];
		data[5][26] = HBM_channelA5[offsets[5]].data[26];
		data[5][27] = HBM_channelA5[offsets[5]].data[27];
		data[5][28] = HBM_channelA5[offsets[5]].data[28];
		data[5][29] = HBM_channelA5[offsets[5]].data[29];
		data[5][30] = HBM_channelA5[offsets[5]].data[30];
		data[5][31] = HBM_channelA5[offsets[5]].data[31];
		data[6][0] = HBM_channelA6[offsets[6]].data[0];
		data[6][1] = HBM_channelA6[offsets[6]].data[1];
		data[6][2] = HBM_channelA6[offsets[6]].data[2];
		data[6][3] = HBM_channelA6[offsets[6]].data[3];
		data[6][4] = HBM_channelA6[offsets[6]].data[4];
		data[6][5] = HBM_channelA6[offsets[6]].data[5];
		data[6][6] = HBM_channelA6[offsets[6]].data[6];
		data[6][7] = HBM_channelA6[offsets[6]].data[7];
		data[6][8] = HBM_channelA6[offsets[6]].data[8];
		data[6][9] = HBM_channelA6[offsets[6]].data[9];
		data[6][10] = HBM_channelA6[offsets[6]].data[10];
		data[6][11] = HBM_channelA6[offsets[6]].data[11];
		data[6][12] = HBM_channelA6[offsets[6]].data[12];
		data[6][13] = HBM_channelA6[offsets[6]].data[13];
		data[6][14] = HBM_channelA6[offsets[6]].data[14];
		data[6][15] = HBM_channelA6[offsets[6]].data[15];
		data[6][16] = HBM_channelA6[offsets[6]].data[16];
		data[6][17] = HBM_channelA6[offsets[6]].data[17];
		data[6][18] = HBM_channelA6[offsets[6]].data[18];
		data[6][19] = HBM_channelA6[offsets[6]].data[19];
		data[6][20] = HBM_channelA6[offsets[6]].data[20];
		data[6][21] = HBM_channelA6[offsets[6]].data[21];
		data[6][22] = HBM_channelA6[offsets[6]].data[22];
		data[6][23] = HBM_channelA6[offsets[6]].data[23];
		data[6][24] = HBM_channelA6[offsets[6]].data[24];
		data[6][25] = HBM_channelA6[offsets[6]].data[25];
		data[6][26] = HBM_channelA6[offsets[6]].data[26];
		data[6][27] = HBM_channelA6[offsets[6]].data[27];
		data[6][28] = HBM_channelA6[offsets[6]].data[28];
		data[6][29] = HBM_channelA6[offsets[6]].data[29];
		data[6][30] = HBM_channelA6[offsets[6]].data[30];
		data[6][31] = HBM_channelA6[offsets[6]].data[31];
		data[7][0] = HBM_channelA7[offsets[7]].data[0];
		data[7][1] = HBM_channelA7[offsets[7]].data[1];
		data[7][2] = HBM_channelA7[offsets[7]].data[2];
		data[7][3] = HBM_channelA7[offsets[7]].data[3];
		data[7][4] = HBM_channelA7[offsets[7]].data[4];
		data[7][5] = HBM_channelA7[offsets[7]].data[5];
		data[7][6] = HBM_channelA7[offsets[7]].data[6];
		data[7][7] = HBM_channelA7[offsets[7]].data[7];
		data[7][8] = HBM_channelA7[offsets[7]].data[8];
		data[7][9] = HBM_channelA7[offsets[7]].data[9];
		data[7][10] = HBM_channelA7[offsets[7]].data[10];
		data[7][11] = HBM_channelA7[offsets[7]].data[11];
		data[7][12] = HBM_channelA7[offsets[7]].data[12];
		data[7][13] = HBM_channelA7[offsets[7]].data[13];
		data[7][14] = HBM_channelA7[offsets[7]].data[14];
		data[7][15] = HBM_channelA7[offsets[7]].data[15];
		data[7][16] = HBM_channelA7[offsets[7]].data[16];
		data[7][17] = HBM_channelA7[offsets[7]].data[17];
		data[7][18] = HBM_channelA7[offsets[7]].data[18];
		data[7][19] = HBM_channelA7[offsets[7]].data[19];
		data[7][20] = HBM_channelA7[offsets[7]].data[20];
		data[7][21] = HBM_channelA7[offsets[7]].data[21];
		data[7][22] = HBM_channelA7[offsets[7]].data[22];
		data[7][23] = HBM_channelA7[offsets[7]].data[23];
		data[7][24] = HBM_channelA7[offsets[7]].data[24];
		data[7][25] = HBM_channelA7[offsets[7]].data[25];
		data[7][26] = HBM_channelA7[offsets[7]].data[26];
		data[7][27] = HBM_channelA7[offsets[7]].data[27];
		data[7][28] = HBM_channelA7[offsets[7]].data[28];
		data[7][29] = HBM_channelA7[offsets[7]].data[29];
		data[7][30] = HBM_channelA7[offsets[7]].data[30];
		data[7][31] = HBM_channelA7[offsets[7]].data[31];
		data[8][0] = HBM_channelA8[offsets[8]].data[0];
		data[8][1] = HBM_channelA8[offsets[8]].data[1];
		data[8][2] = HBM_channelA8[offsets[8]].data[2];
		data[8][3] = HBM_channelA8[offsets[8]].data[3];
		data[8][4] = HBM_channelA8[offsets[8]].data[4];
		data[8][5] = HBM_channelA8[offsets[8]].data[5];
		data[8][6] = HBM_channelA8[offsets[8]].data[6];
		data[8][7] = HBM_channelA8[offsets[8]].data[7];
		data[8][8] = HBM_channelA8[offsets[8]].data[8];
		data[8][9] = HBM_channelA8[offsets[8]].data[9];
		data[8][10] = HBM_channelA8[offsets[8]].data[10];
		data[8][11] = HBM_channelA8[offsets[8]].data[11];
		data[8][12] = HBM_channelA8[offsets[8]].data[12];
		data[8][13] = HBM_channelA8[offsets[8]].data[13];
		data[8][14] = HBM_channelA8[offsets[8]].data[14];
		data[8][15] = HBM_channelA8[offsets[8]].data[15];
		data[8][16] = HBM_channelA8[offsets[8]].data[16];
		data[8][17] = HBM_channelA8[offsets[8]].data[17];
		data[8][18] = HBM_channelA8[offsets[8]].data[18];
		data[8][19] = HBM_channelA8[offsets[8]].data[19];
		data[8][20] = HBM_channelA8[offsets[8]].data[20];
		data[8][21] = HBM_channelA8[offsets[8]].data[21];
		data[8][22] = HBM_channelA8[offsets[8]].data[22];
		data[8][23] = HBM_channelA8[offsets[8]].data[23];
		data[8][24] = HBM_channelA8[offsets[8]].data[24];
		data[8][25] = HBM_channelA8[offsets[8]].data[25];
		data[8][26] = HBM_channelA8[offsets[8]].data[26];
		data[8][27] = HBM_channelA8[offsets[8]].data[27];
		data[8][28] = HBM_channelA8[offsets[8]].data[28];
		data[8][29] = HBM_channelA8[offsets[8]].data[29];
		data[8][30] = HBM_channelA8[offsets[8]].data[30];
		data[8][31] = HBM_channelA8[offsets[8]].data[31];
		data[9][0] = HBM_channelA9[offsets[9]].data[0];
		data[9][1] = HBM_channelA9[offsets[9]].data[1];
		data[9][2] = HBM_channelA9[offsets[9]].data[2];
		data[9][3] = HBM_channelA9[offsets[9]].data[3];
		data[9][4] = HBM_channelA9[offsets[9]].data[4];
		data[9][5] = HBM_channelA9[offsets[9]].data[5];
		data[9][6] = HBM_channelA9[offsets[9]].data[6];
		data[9][7] = HBM_channelA9[offsets[9]].data[7];
		data[9][8] = HBM_channelA9[offsets[9]].data[8];
		data[9][9] = HBM_channelA9[offsets[9]].data[9];
		data[9][10] = HBM_channelA9[offsets[9]].data[10];
		data[9][11] = HBM_channelA9[offsets[9]].data[11];
		data[9][12] = HBM_channelA9[offsets[9]].data[12];
		data[9][13] = HBM_channelA9[offsets[9]].data[13];
		data[9][14] = HBM_channelA9[offsets[9]].data[14];
		data[9][15] = HBM_channelA9[offsets[9]].data[15];
		data[9][16] = HBM_channelA9[offsets[9]].data[16];
		data[9][17] = HBM_channelA9[offsets[9]].data[17];
		data[9][18] = HBM_channelA9[offsets[9]].data[18];
		data[9][19] = HBM_channelA9[offsets[9]].data[19];
		data[9][20] = HBM_channelA9[offsets[9]].data[20];
		data[9][21] = HBM_channelA9[offsets[9]].data[21];
		data[9][22] = HBM_channelA9[offsets[9]].data[22];
		data[9][23] = HBM_channelA9[offsets[9]].data[23];
		data[9][24] = HBM_channelA9[offsets[9]].data[24];
		data[9][25] = HBM_channelA9[offsets[9]].data[25];
		data[9][26] = HBM_channelA9[offsets[9]].data[26];
		data[9][27] = HBM_channelA9[offsets[9]].data[27];
		data[9][28] = HBM_channelA9[offsets[9]].data[28];
		data[9][29] = HBM_channelA9[offsets[9]].data[29];
		data[9][30] = HBM_channelA9[offsets[9]].data[30];
		data[9][31] = HBM_channelA9[offsets[9]].data[31];
		data[10][0] = HBM_channelA10[offsets[10]].data[0];
		data[10][1] = HBM_channelA10[offsets[10]].data[1];
		data[10][2] = HBM_channelA10[offsets[10]].data[2];
		data[10][3] = HBM_channelA10[offsets[10]].data[3];
		data[10][4] = HBM_channelA10[offsets[10]].data[4];
		data[10][5] = HBM_channelA10[offsets[10]].data[5];
		data[10][6] = HBM_channelA10[offsets[10]].data[6];
		data[10][7] = HBM_channelA10[offsets[10]].data[7];
		data[10][8] = HBM_channelA10[offsets[10]].data[8];
		data[10][9] = HBM_channelA10[offsets[10]].data[9];
		data[10][10] = HBM_channelA10[offsets[10]].data[10];
		data[10][11] = HBM_channelA10[offsets[10]].data[11];
		data[10][12] = HBM_channelA10[offsets[10]].data[12];
		data[10][13] = HBM_channelA10[offsets[10]].data[13];
		data[10][14] = HBM_channelA10[offsets[10]].data[14];
		data[10][15] = HBM_channelA10[offsets[10]].data[15];
		data[10][16] = HBM_channelA10[offsets[10]].data[16];
		data[10][17] = HBM_channelA10[offsets[10]].data[17];
		data[10][18] = HBM_channelA10[offsets[10]].data[18];
		data[10][19] = HBM_channelA10[offsets[10]].data[19];
		data[10][20] = HBM_channelA10[offsets[10]].data[20];
		data[10][21] = HBM_channelA10[offsets[10]].data[21];
		data[10][22] = HBM_channelA10[offsets[10]].data[22];
		data[10][23] = HBM_channelA10[offsets[10]].data[23];
		data[10][24] = HBM_channelA10[offsets[10]].data[24];
		data[10][25] = HBM_channelA10[offsets[10]].data[25];
		data[10][26] = HBM_channelA10[offsets[10]].data[26];
		data[10][27] = HBM_channelA10[offsets[10]].data[27];
		data[10][28] = HBM_channelA10[offsets[10]].data[28];
		data[10][29] = HBM_channelA10[offsets[10]].data[29];
		data[10][30] = HBM_channelA10[offsets[10]].data[30];
		data[10][31] = HBM_channelA10[offsets[10]].data[31];
		data[11][0] = HBM_channelA11[offsets[11]].data[0];
		data[11][1] = HBM_channelA11[offsets[11]].data[1];
		data[11][2] = HBM_channelA11[offsets[11]].data[2];
		data[11][3] = HBM_channelA11[offsets[11]].data[3];
		data[11][4] = HBM_channelA11[offsets[11]].data[4];
		data[11][5] = HBM_channelA11[offsets[11]].data[5];
		data[11][6] = HBM_channelA11[offsets[11]].data[6];
		data[11][7] = HBM_channelA11[offsets[11]].data[7];
		data[11][8] = HBM_channelA11[offsets[11]].data[8];
		data[11][9] = HBM_channelA11[offsets[11]].data[9];
		data[11][10] = HBM_channelA11[offsets[11]].data[10];
		data[11][11] = HBM_channelA11[offsets[11]].data[11];
		data[11][12] = HBM_channelA11[offsets[11]].data[12];
		data[11][13] = HBM_channelA11[offsets[11]].data[13];
		data[11][14] = HBM_channelA11[offsets[11]].data[14];
		data[11][15] = HBM_channelA11[offsets[11]].data[15];
		data[11][16] = HBM_channelA11[offsets[11]].data[16];
		data[11][17] = HBM_channelA11[offsets[11]].data[17];
		data[11][18] = HBM_channelA11[offsets[11]].data[18];
		data[11][19] = HBM_channelA11[offsets[11]].data[19];
		data[11][20] = HBM_channelA11[offsets[11]].data[20];
		data[11][21] = HBM_channelA11[offsets[11]].data[21];
		data[11][22] = HBM_channelA11[offsets[11]].data[22];
		data[11][23] = HBM_channelA11[offsets[11]].data[23];
		data[11][24] = HBM_channelA11[offsets[11]].data[24];
		data[11][25] = HBM_channelA11[offsets[11]].data[25];
		data[11][26] = HBM_channelA11[offsets[11]].data[26];
		data[11][27] = HBM_channelA11[offsets[11]].data[27];
		data[11][28] = HBM_channelA11[offsets[11]].data[28];
		data[11][29] = HBM_channelA11[offsets[11]].data[29];
		data[11][30] = HBM_channelA11[offsets[11]].data[30];
		data[11][31] = HBM_channelA11[offsets[11]].data[31];
		data[12][0] = HBM_channelA12[offsets[12]].data[0];
		data[12][1] = HBM_channelA12[offsets[12]].data[1];
		data[12][2] = HBM_channelA12[offsets[12]].data[2];
		data[12][3] = HBM_channelA12[offsets[12]].data[3];
		data[12][4] = HBM_channelA12[offsets[12]].data[4];
		data[12][5] = HBM_channelA12[offsets[12]].data[5];
		data[12][6] = HBM_channelA12[offsets[12]].data[6];
		data[12][7] = HBM_channelA12[offsets[12]].data[7];
		data[12][8] = HBM_channelA12[offsets[12]].data[8];
		data[12][9] = HBM_channelA12[offsets[12]].data[9];
		data[12][10] = HBM_channelA12[offsets[12]].data[10];
		data[12][11] = HBM_channelA12[offsets[12]].data[11];
		data[12][12] = HBM_channelA12[offsets[12]].data[12];
		data[12][13] = HBM_channelA12[offsets[12]].data[13];
		data[12][14] = HBM_channelA12[offsets[12]].data[14];
		data[12][15] = HBM_channelA12[offsets[12]].data[15];
		data[12][16] = HBM_channelA12[offsets[12]].data[16];
		data[12][17] = HBM_channelA12[offsets[12]].data[17];
		data[12][18] = HBM_channelA12[offsets[12]].data[18];
		data[12][19] = HBM_channelA12[offsets[12]].data[19];
		data[12][20] = HBM_channelA12[offsets[12]].data[20];
		data[12][21] = HBM_channelA12[offsets[12]].data[21];
		data[12][22] = HBM_channelA12[offsets[12]].data[22];
		data[12][23] = HBM_channelA12[offsets[12]].data[23];
		data[12][24] = HBM_channelA12[offsets[12]].data[24];
		data[12][25] = HBM_channelA12[offsets[12]].data[25];
		data[12][26] = HBM_channelA12[offsets[12]].data[26];
		data[12][27] = HBM_channelA12[offsets[12]].data[27];
		data[12][28] = HBM_channelA12[offsets[12]].data[28];
		data[12][29] = HBM_channelA12[offsets[12]].data[29];
		data[12][30] = HBM_channelA12[offsets[12]].data[30];
		data[12][31] = HBM_channelA12[offsets[12]].data[31];
		data[13][0] = HBM_channelA13[offsets[13]].data[0];
		data[13][1] = HBM_channelA13[offsets[13]].data[1];
		data[13][2] = HBM_channelA13[offsets[13]].data[2];
		data[13][3] = HBM_channelA13[offsets[13]].data[3];
		data[13][4] = HBM_channelA13[offsets[13]].data[4];
		data[13][5] = HBM_channelA13[offsets[13]].data[5];
		data[13][6] = HBM_channelA13[offsets[13]].data[6];
		data[13][7] = HBM_channelA13[offsets[13]].data[7];
		data[13][8] = HBM_channelA13[offsets[13]].data[8];
		data[13][9] = HBM_channelA13[offsets[13]].data[9];
		data[13][10] = HBM_channelA13[offsets[13]].data[10];
		data[13][11] = HBM_channelA13[offsets[13]].data[11];
		data[13][12] = HBM_channelA13[offsets[13]].data[12];
		data[13][13] = HBM_channelA13[offsets[13]].data[13];
		data[13][14] = HBM_channelA13[offsets[13]].data[14];
		data[13][15] = HBM_channelA13[offsets[13]].data[15];
		data[13][16] = HBM_channelA13[offsets[13]].data[16];
		data[13][17] = HBM_channelA13[offsets[13]].data[17];
		data[13][18] = HBM_channelA13[offsets[13]].data[18];
		data[13][19] = HBM_channelA13[offsets[13]].data[19];
		data[13][20] = HBM_channelA13[offsets[13]].data[20];
		data[13][21] = HBM_channelA13[offsets[13]].data[21];
		data[13][22] = HBM_channelA13[offsets[13]].data[22];
		data[13][23] = HBM_channelA13[offsets[13]].data[23];
		data[13][24] = HBM_channelA13[offsets[13]].data[24];
		data[13][25] = HBM_channelA13[offsets[13]].data[25];
		data[13][26] = HBM_channelA13[offsets[13]].data[26];
		data[13][27] = HBM_channelA13[offsets[13]].data[27];
		data[13][28] = HBM_channelA13[offsets[13]].data[28];
		data[13][29] = HBM_channelA13[offsets[13]].data[29];
		data[13][30] = HBM_channelA13[offsets[13]].data[30];
		data[13][31] = HBM_channelA13[offsets[13]].data[31];
		data[14][0] = HBM_channelA14[offsets[14]].data[0];
		data[14][1] = HBM_channelA14[offsets[14]].data[1];
		data[14][2] = HBM_channelA14[offsets[14]].data[2];
		data[14][3] = HBM_channelA14[offsets[14]].data[3];
		data[14][4] = HBM_channelA14[offsets[14]].data[4];
		data[14][5] = HBM_channelA14[offsets[14]].data[5];
		data[14][6] = HBM_channelA14[offsets[14]].data[6];
		data[14][7] = HBM_channelA14[offsets[14]].data[7];
		data[14][8] = HBM_channelA14[offsets[14]].data[8];
		data[14][9] = HBM_channelA14[offsets[14]].data[9];
		data[14][10] = HBM_channelA14[offsets[14]].data[10];
		data[14][11] = HBM_channelA14[offsets[14]].data[11];
		data[14][12] = HBM_channelA14[offsets[14]].data[12];
		data[14][13] = HBM_channelA14[offsets[14]].data[13];
		data[14][14] = HBM_channelA14[offsets[14]].data[14];
		data[14][15] = HBM_channelA14[offsets[14]].data[15];
		data[14][16] = HBM_channelA14[offsets[14]].data[16];
		data[14][17] = HBM_channelA14[offsets[14]].data[17];
		data[14][18] = HBM_channelA14[offsets[14]].data[18];
		data[14][19] = HBM_channelA14[offsets[14]].data[19];
		data[14][20] = HBM_channelA14[offsets[14]].data[20];
		data[14][21] = HBM_channelA14[offsets[14]].data[21];
		data[14][22] = HBM_channelA14[offsets[14]].data[22];
		data[14][23] = HBM_channelA14[offsets[14]].data[23];
		data[14][24] = HBM_channelA14[offsets[14]].data[24];
		data[14][25] = HBM_channelA14[offsets[14]].data[25];
		data[14][26] = HBM_channelA14[offsets[14]].data[26];
		data[14][27] = HBM_channelA14[offsets[14]].data[27];
		data[14][28] = HBM_channelA14[offsets[14]].data[28];
		data[14][29] = HBM_channelA14[offsets[14]].data[29];
		data[14][30] = HBM_channelA14[offsets[14]].data[30];
		data[14][31] = HBM_channelA14[offsets[14]].data[31];
		data[15][0] = HBM_channelA15[offsets[15]].data[0];
		data[15][1] = HBM_channelA15[offsets[15]].data[1];
		data[15][2] = HBM_channelA15[offsets[15]].data[2];
		data[15][3] = HBM_channelA15[offsets[15]].data[3];
		data[15][4] = HBM_channelA15[offsets[15]].data[4];
		data[15][5] = HBM_channelA15[offsets[15]].data[5];
		data[15][6] = HBM_channelA15[offsets[15]].data[6];
		data[15][7] = HBM_channelA15[offsets[15]].data[7];
		data[15][8] = HBM_channelA15[offsets[15]].data[8];
		data[15][9] = HBM_channelA15[offsets[15]].data[9];
		data[15][10] = HBM_channelA15[offsets[15]].data[10];
		data[15][11] = HBM_channelA15[offsets[15]].data[11];
		data[15][12] = HBM_channelA15[offsets[15]].data[12];
		data[15][13] = HBM_channelA15[offsets[15]].data[13];
		data[15][14] = HBM_channelA15[offsets[15]].data[14];
		data[15][15] = HBM_channelA15[offsets[15]].data[15];
		data[15][16] = HBM_channelA15[offsets[15]].data[16];
		data[15][17] = HBM_channelA15[offsets[15]].data[17];
		data[15][18] = HBM_channelA15[offsets[15]].data[18];
		data[15][19] = HBM_channelA15[offsets[15]].data[19];
		data[15][20] = HBM_channelA15[offsets[15]].data[20];
		data[15][21] = HBM_channelA15[offsets[15]].data[21];
		data[15][22] = HBM_channelA15[offsets[15]].data[22];
		data[15][23] = HBM_channelA15[offsets[15]].data[23];
		data[15][24] = HBM_channelA15[offsets[15]].data[24];
		data[15][25] = HBM_channelA15[offsets[15]].data[25];
		data[15][26] = HBM_channelA15[offsets[15]].data[26];
		data[15][27] = HBM_channelA15[offsets[15]].data[27];
		data[15][28] = HBM_channelA15[offsets[15]].data[28];
		data[15][29] = HBM_channelA15[offsets[15]].data[29];
		data[15][30] = HBM_channelA15[offsets[15]].data[30];
		data[15][31] = HBM_channelA15[offsets[15]].data[31];
		data[16][0] = HBM_channelA16[offsets[16]].data[0];
		data[16][1] = HBM_channelA16[offsets[16]].data[1];
		data[16][2] = HBM_channelA16[offsets[16]].data[2];
		data[16][3] = HBM_channelA16[offsets[16]].data[3];
		data[16][4] = HBM_channelA16[offsets[16]].data[4];
		data[16][5] = HBM_channelA16[offsets[16]].data[5];
		data[16][6] = HBM_channelA16[offsets[16]].data[6];
		data[16][7] = HBM_channelA16[offsets[16]].data[7];
		data[16][8] = HBM_channelA16[offsets[16]].data[8];
		data[16][9] = HBM_channelA16[offsets[16]].data[9];
		data[16][10] = HBM_channelA16[offsets[16]].data[10];
		data[16][11] = HBM_channelA16[offsets[16]].data[11];
		data[16][12] = HBM_channelA16[offsets[16]].data[12];
		data[16][13] = HBM_channelA16[offsets[16]].data[13];
		data[16][14] = HBM_channelA16[offsets[16]].data[14];
		data[16][15] = HBM_channelA16[offsets[16]].data[15];
		data[16][16] = HBM_channelA16[offsets[16]].data[16];
		data[16][17] = HBM_channelA16[offsets[16]].data[17];
		data[16][18] = HBM_channelA16[offsets[16]].data[18];
		data[16][19] = HBM_channelA16[offsets[16]].data[19];
		data[16][20] = HBM_channelA16[offsets[16]].data[20];
		data[16][21] = HBM_channelA16[offsets[16]].data[21];
		data[16][22] = HBM_channelA16[offsets[16]].data[22];
		data[16][23] = HBM_channelA16[offsets[16]].data[23];
		data[16][24] = HBM_channelA16[offsets[16]].data[24];
		data[16][25] = HBM_channelA16[offsets[16]].data[25];
		data[16][26] = HBM_channelA16[offsets[16]].data[26];
		data[16][27] = HBM_channelA16[offsets[16]].data[27];
		data[16][28] = HBM_channelA16[offsets[16]].data[28];
		data[16][29] = HBM_channelA16[offsets[16]].data[29];
		data[16][30] = HBM_channelA16[offsets[16]].data[30];
		data[16][31] = HBM_channelA16[offsets[16]].data[31];
		data[17][0] = HBM_channelA17[offsets[17]].data[0];
		data[17][1] = HBM_channelA17[offsets[17]].data[1];
		data[17][2] = HBM_channelA17[offsets[17]].data[2];
		data[17][3] = HBM_channelA17[offsets[17]].data[3];
		data[17][4] = HBM_channelA17[offsets[17]].data[4];
		data[17][5] = HBM_channelA17[offsets[17]].data[5];
		data[17][6] = HBM_channelA17[offsets[17]].data[6];
		data[17][7] = HBM_channelA17[offsets[17]].data[7];
		data[17][8] = HBM_channelA17[offsets[17]].data[8];
		data[17][9] = HBM_channelA17[offsets[17]].data[9];
		data[17][10] = HBM_channelA17[offsets[17]].data[10];
		data[17][11] = HBM_channelA17[offsets[17]].data[11];
		data[17][12] = HBM_channelA17[offsets[17]].data[12];
		data[17][13] = HBM_channelA17[offsets[17]].data[13];
		data[17][14] = HBM_channelA17[offsets[17]].data[14];
		data[17][15] = HBM_channelA17[offsets[17]].data[15];
		data[17][16] = HBM_channelA17[offsets[17]].data[16];
		data[17][17] = HBM_channelA17[offsets[17]].data[17];
		data[17][18] = HBM_channelA17[offsets[17]].data[18];
		data[17][19] = HBM_channelA17[offsets[17]].data[19];
		data[17][20] = HBM_channelA17[offsets[17]].data[20];
		data[17][21] = HBM_channelA17[offsets[17]].data[21];
		data[17][22] = HBM_channelA17[offsets[17]].data[22];
		data[17][23] = HBM_channelA17[offsets[17]].data[23];
		data[17][24] = HBM_channelA17[offsets[17]].data[24];
		data[17][25] = HBM_channelA17[offsets[17]].data[25];
		data[17][26] = HBM_channelA17[offsets[17]].data[26];
		data[17][27] = HBM_channelA17[offsets[17]].data[27];
		data[17][28] = HBM_channelA17[offsets[17]].data[28];
		data[17][29] = HBM_channelA17[offsets[17]].data[29];
		data[17][30] = HBM_channelA17[offsets[17]].data[30];
		data[17][31] = HBM_channelA17[offsets[17]].data[31];
		data[18][0] = HBM_channelA18[offsets[18]].data[0];
		data[18][1] = HBM_channelA18[offsets[18]].data[1];
		data[18][2] = HBM_channelA18[offsets[18]].data[2];
		data[18][3] = HBM_channelA18[offsets[18]].data[3];
		data[18][4] = HBM_channelA18[offsets[18]].data[4];
		data[18][5] = HBM_channelA18[offsets[18]].data[5];
		data[18][6] = HBM_channelA18[offsets[18]].data[6];
		data[18][7] = HBM_channelA18[offsets[18]].data[7];
		data[18][8] = HBM_channelA18[offsets[18]].data[8];
		data[18][9] = HBM_channelA18[offsets[18]].data[9];
		data[18][10] = HBM_channelA18[offsets[18]].data[10];
		data[18][11] = HBM_channelA18[offsets[18]].data[11];
		data[18][12] = HBM_channelA18[offsets[18]].data[12];
		data[18][13] = HBM_channelA18[offsets[18]].data[13];
		data[18][14] = HBM_channelA18[offsets[18]].data[14];
		data[18][15] = HBM_channelA18[offsets[18]].data[15];
		data[18][16] = HBM_channelA18[offsets[18]].data[16];
		data[18][17] = HBM_channelA18[offsets[18]].data[17];
		data[18][18] = HBM_channelA18[offsets[18]].data[18];
		data[18][19] = HBM_channelA18[offsets[18]].data[19];
		data[18][20] = HBM_channelA18[offsets[18]].data[20];
		data[18][21] = HBM_channelA18[offsets[18]].data[21];
		data[18][22] = HBM_channelA18[offsets[18]].data[22];
		data[18][23] = HBM_channelA18[offsets[18]].data[23];
		data[18][24] = HBM_channelA18[offsets[18]].data[24];
		data[18][25] = HBM_channelA18[offsets[18]].data[25];
		data[18][26] = HBM_channelA18[offsets[18]].data[26];
		data[18][27] = HBM_channelA18[offsets[18]].data[27];
		data[18][28] = HBM_channelA18[offsets[18]].data[28];
		data[18][29] = HBM_channelA18[offsets[18]].data[29];
		data[18][30] = HBM_channelA18[offsets[18]].data[30];
		data[18][31] = HBM_channelA18[offsets[18]].data[31];
		data[19][0] = HBM_channelA19[offsets[19]].data[0];
		data[19][1] = HBM_channelA19[offsets[19]].data[1];
		data[19][2] = HBM_channelA19[offsets[19]].data[2];
		data[19][3] = HBM_channelA19[offsets[19]].data[3];
		data[19][4] = HBM_channelA19[offsets[19]].data[4];
		data[19][5] = HBM_channelA19[offsets[19]].data[5];
		data[19][6] = HBM_channelA19[offsets[19]].data[6];
		data[19][7] = HBM_channelA19[offsets[19]].data[7];
		data[19][8] = HBM_channelA19[offsets[19]].data[8];
		data[19][9] = HBM_channelA19[offsets[19]].data[9];
		data[19][10] = HBM_channelA19[offsets[19]].data[10];
		data[19][11] = HBM_channelA19[offsets[19]].data[11];
		data[19][12] = HBM_channelA19[offsets[19]].data[12];
		data[19][13] = HBM_channelA19[offsets[19]].data[13];
		data[19][14] = HBM_channelA19[offsets[19]].data[14];
		data[19][15] = HBM_channelA19[offsets[19]].data[15];
		data[19][16] = HBM_channelA19[offsets[19]].data[16];
		data[19][17] = HBM_channelA19[offsets[19]].data[17];
		data[19][18] = HBM_channelA19[offsets[19]].data[18];
		data[19][19] = HBM_channelA19[offsets[19]].data[19];
		data[19][20] = HBM_channelA19[offsets[19]].data[20];
		data[19][21] = HBM_channelA19[offsets[19]].data[21];
		data[19][22] = HBM_channelA19[offsets[19]].data[22];
		data[19][23] = HBM_channelA19[offsets[19]].data[23];
		data[19][24] = HBM_channelA19[offsets[19]].data[24];
		data[19][25] = HBM_channelA19[offsets[19]].data[25];
		data[19][26] = HBM_channelA19[offsets[19]].data[26];
		data[19][27] = HBM_channelA19[offsets[19]].data[27];
		data[19][28] = HBM_channelA19[offsets[19]].data[28];
		data[19][29] = HBM_channelA19[offsets[19]].data[29];
		data[19][30] = HBM_channelA19[offsets[19]].data[30];
		data[19][31] = HBM_channelA19[offsets[19]].data[31];
		data[20][0] = HBM_channelA20[offsets[20]].data[0];
		data[20][1] = HBM_channelA20[offsets[20]].data[1];
		data[20][2] = HBM_channelA20[offsets[20]].data[2];
		data[20][3] = HBM_channelA20[offsets[20]].data[3];
		data[20][4] = HBM_channelA20[offsets[20]].data[4];
		data[20][5] = HBM_channelA20[offsets[20]].data[5];
		data[20][6] = HBM_channelA20[offsets[20]].data[6];
		data[20][7] = HBM_channelA20[offsets[20]].data[7];
		data[20][8] = HBM_channelA20[offsets[20]].data[8];
		data[20][9] = HBM_channelA20[offsets[20]].data[9];
		data[20][10] = HBM_channelA20[offsets[20]].data[10];
		data[20][11] = HBM_channelA20[offsets[20]].data[11];
		data[20][12] = HBM_channelA20[offsets[20]].data[12];
		data[20][13] = HBM_channelA20[offsets[20]].data[13];
		data[20][14] = HBM_channelA20[offsets[20]].data[14];
		data[20][15] = HBM_channelA20[offsets[20]].data[15];
		data[20][16] = HBM_channelA20[offsets[20]].data[16];
		data[20][17] = HBM_channelA20[offsets[20]].data[17];
		data[20][18] = HBM_channelA20[offsets[20]].data[18];
		data[20][19] = HBM_channelA20[offsets[20]].data[19];
		data[20][20] = HBM_channelA20[offsets[20]].data[20];
		data[20][21] = HBM_channelA20[offsets[20]].data[21];
		data[20][22] = HBM_channelA20[offsets[20]].data[22];
		data[20][23] = HBM_channelA20[offsets[20]].data[23];
		data[20][24] = HBM_channelA20[offsets[20]].data[24];
		data[20][25] = HBM_channelA20[offsets[20]].data[25];
		data[20][26] = HBM_channelA20[offsets[20]].data[26];
		data[20][27] = HBM_channelA20[offsets[20]].data[27];
		data[20][28] = HBM_channelA20[offsets[20]].data[28];
		data[20][29] = HBM_channelA20[offsets[20]].data[29];
		data[20][30] = HBM_channelA20[offsets[20]].data[30];
		data[20][31] = HBM_channelA20[offsets[20]].data[31];
		data[21][0] = HBM_channelA21[offsets[21]].data[0];
		data[21][1] = HBM_channelA21[offsets[21]].data[1];
		data[21][2] = HBM_channelA21[offsets[21]].data[2];
		data[21][3] = HBM_channelA21[offsets[21]].data[3];
		data[21][4] = HBM_channelA21[offsets[21]].data[4];
		data[21][5] = HBM_channelA21[offsets[21]].data[5];
		data[21][6] = HBM_channelA21[offsets[21]].data[6];
		data[21][7] = HBM_channelA21[offsets[21]].data[7];
		data[21][8] = HBM_channelA21[offsets[21]].data[8];
		data[21][9] = HBM_channelA21[offsets[21]].data[9];
		data[21][10] = HBM_channelA21[offsets[21]].data[10];
		data[21][11] = HBM_channelA21[offsets[21]].data[11];
		data[21][12] = HBM_channelA21[offsets[21]].data[12];
		data[21][13] = HBM_channelA21[offsets[21]].data[13];
		data[21][14] = HBM_channelA21[offsets[21]].data[14];
		data[21][15] = HBM_channelA21[offsets[21]].data[15];
		data[21][16] = HBM_channelA21[offsets[21]].data[16];
		data[21][17] = HBM_channelA21[offsets[21]].data[17];
		data[21][18] = HBM_channelA21[offsets[21]].data[18];
		data[21][19] = HBM_channelA21[offsets[21]].data[19];
		data[21][20] = HBM_channelA21[offsets[21]].data[20];
		data[21][21] = HBM_channelA21[offsets[21]].data[21];
		data[21][22] = HBM_channelA21[offsets[21]].data[22];
		data[21][23] = HBM_channelA21[offsets[21]].data[23];
		data[21][24] = HBM_channelA21[offsets[21]].data[24];
		data[21][25] = HBM_channelA21[offsets[21]].data[25];
		data[21][26] = HBM_channelA21[offsets[21]].data[26];
		data[21][27] = HBM_channelA21[offsets[21]].data[27];
		data[21][28] = HBM_channelA21[offsets[21]].data[28];
		data[21][29] = HBM_channelA21[offsets[21]].data[29];
		data[21][30] = HBM_channelA21[offsets[21]].data[30];
		data[21][31] = HBM_channelA21[offsets[21]].data[31];
		data[22][0] = HBM_channelA22[offsets[22]].data[0];
		data[22][1] = HBM_channelA22[offsets[22]].data[1];
		data[22][2] = HBM_channelA22[offsets[22]].data[2];
		data[22][3] = HBM_channelA22[offsets[22]].data[3];
		data[22][4] = HBM_channelA22[offsets[22]].data[4];
		data[22][5] = HBM_channelA22[offsets[22]].data[5];
		data[22][6] = HBM_channelA22[offsets[22]].data[6];
		data[22][7] = HBM_channelA22[offsets[22]].data[7];
		data[22][8] = HBM_channelA22[offsets[22]].data[8];
		data[22][9] = HBM_channelA22[offsets[22]].data[9];
		data[22][10] = HBM_channelA22[offsets[22]].data[10];
		data[22][11] = HBM_channelA22[offsets[22]].data[11];
		data[22][12] = HBM_channelA22[offsets[22]].data[12];
		data[22][13] = HBM_channelA22[offsets[22]].data[13];
		data[22][14] = HBM_channelA22[offsets[22]].data[14];
		data[22][15] = HBM_channelA22[offsets[22]].data[15];
		data[22][16] = HBM_channelA22[offsets[22]].data[16];
		data[22][17] = HBM_channelA22[offsets[22]].data[17];
		data[22][18] = HBM_channelA22[offsets[22]].data[18];
		data[22][19] = HBM_channelA22[offsets[22]].data[19];
		data[22][20] = HBM_channelA22[offsets[22]].data[20];
		data[22][21] = HBM_channelA22[offsets[22]].data[21];
		data[22][22] = HBM_channelA22[offsets[22]].data[22];
		data[22][23] = HBM_channelA22[offsets[22]].data[23];
		data[22][24] = HBM_channelA22[offsets[22]].data[24];
		data[22][25] = HBM_channelA22[offsets[22]].data[25];
		data[22][26] = HBM_channelA22[offsets[22]].data[26];
		data[22][27] = HBM_channelA22[offsets[22]].data[27];
		data[22][28] = HBM_channelA22[offsets[22]].data[28];
		data[22][29] = HBM_channelA22[offsets[22]].data[29];
		data[22][30] = HBM_channelA22[offsets[22]].data[30];
		data[22][31] = HBM_channelA22[offsets[22]].data[31];
		data[23][0] = HBM_channelA23[offsets[23]].data[0];
		data[23][1] = HBM_channelA23[offsets[23]].data[1];
		data[23][2] = HBM_channelA23[offsets[23]].data[2];
		data[23][3] = HBM_channelA23[offsets[23]].data[3];
		data[23][4] = HBM_channelA23[offsets[23]].data[4];
		data[23][5] = HBM_channelA23[offsets[23]].data[5];
		data[23][6] = HBM_channelA23[offsets[23]].data[6];
		data[23][7] = HBM_channelA23[offsets[23]].data[7];
		data[23][8] = HBM_channelA23[offsets[23]].data[8];
		data[23][9] = HBM_channelA23[offsets[23]].data[9];
		data[23][10] = HBM_channelA23[offsets[23]].data[10];
		data[23][11] = HBM_channelA23[offsets[23]].data[11];
		data[23][12] = HBM_channelA23[offsets[23]].data[12];
		data[23][13] = HBM_channelA23[offsets[23]].data[13];
		data[23][14] = HBM_channelA23[offsets[23]].data[14];
		data[23][15] = HBM_channelA23[offsets[23]].data[15];
		data[23][16] = HBM_channelA23[offsets[23]].data[16];
		data[23][17] = HBM_channelA23[offsets[23]].data[17];
		data[23][18] = HBM_channelA23[offsets[23]].data[18];
		data[23][19] = HBM_channelA23[offsets[23]].data[19];
		data[23][20] = HBM_channelA23[offsets[23]].data[20];
		data[23][21] = HBM_channelA23[offsets[23]].data[21];
		data[23][22] = HBM_channelA23[offsets[23]].data[22];
		data[23][23] = HBM_channelA23[offsets[23]].data[23];
		data[23][24] = HBM_channelA23[offsets[23]].data[24];
		data[23][25] = HBM_channelA23[offsets[23]].data[25];
		data[23][26] = HBM_channelA23[offsets[23]].data[26];
		data[23][27] = HBM_channelA23[offsets[23]].data[27];
		data[23][28] = HBM_channelA23[offsets[23]].data[28];
		data[23][29] = HBM_channelA23[offsets[23]].data[29];
		data[23][30] = HBM_channelA23[offsets[23]].data[30];
		data[23][31] = HBM_channelA23[offsets[23]].data[31];
		#endif 
	#endif 
	return;
}

unsigned int master_retrieve(unsigned int offset, unsigned int v, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, unsigned int inst){
	unsigned int datas[HBM_CHANNEL_PACK_SIZE];
	master_retrieve_vec(offset, datas, HBM_channelA, HBM_channelB, inst);
	return datas[v];
}
void master_retrievemany(unsigned int offsets[NUM_PEs], unsigned int v, unsigned int data[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
	unsigned int datas[NUM_PEs][HBM_CHANNEL_PACK_SIZE]; 
	master_retrievemany_vec(offsets, datas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
	for(unsigned int n=0; n<NUM_PEs; n++){
		data[n] = datas[n][v];
	}
}

//////////////////////////////////////////////////////////////////
void insert_cfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_channelX_t * HBM_center){
	unsigned int data_[HBM_CHANNEL_PACK_SIZE];
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data_[2*v] = data[v].key;
		data_[2*v + 1] = data[v].value;
	}
	master_centerinsert_vec((p_u * MAX_UPARTITION_VECSIZE) + t, data_, HBM_center);
}
void retrieve_cfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_channelX_t * HBM_center){
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
	checkoutofbounds("acts_kernel::ERROR 901::", (index / HBM_CHANNEL_PACK_SIZE), wwsize, index, HBM_CHANNEL_PACK_SIZE, NAp);
	#endif 
	return master_retrieve(base_offset__ + (index / HBM_CHANNEL_PACK_SIZE), index % HBM_CHANNEL_PACK_SIZE, HBM_channelA, HBM_channelB, inst);
}	
void dretrievemany_vptrdram(unsigned int base_offset__, unsigned int offsets[NUM_PEs], unsigned int data[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
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
void dretrievemany_actpackvptrdram(unsigned int base_offset__, unsigned int p_u, unsigned int llp_set, map_t maps[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
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
	master_retrievemany(offsets_, j, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
	for(unsigned int n=0; n<NUM_PEs; n++){
	#pragma HLS UNROLL
		maps[n].offset = data_[n];
	}
	master_retrievemany(offsets_, j + 1, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
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
void dretrievemany_act_pack_edges(unsigned int base_offset__, unsigned int offsets[NUM_PEs], unsigned int t, edge3_vec_dt edge3_vecs[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
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
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
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
void dinsertmany_updatesdram(unsigned int offset__, unsigned int llp_set, unsigned int offsets[NUM_PEs], unsigned int t, uint512_vec_dt data[NUM_PEs], bool ens[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, unsigned int updatesptrs[MAX_NUM_LLPSETS]){
	#ifdef _DEBUGMODE_CHECKS3
	unsigned int wwsize = globalparams_debug[GLOBALPARAMSCODE__WWSIZE__UPDATES];
	for(unsigned int n=0; n<NUM_PEs; n++){ checkoutofbounds("acts_kernel::ERROR 708a::", updatesptrs[llp_set] + offsets[n] + t, updatesptrs[llp_set + 1], NAp, NAp, NAp); }
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
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
}
void dretrievemany_udatesdram(unsigned int offset__, unsigned int llp_set, unsigned int index, uint512_vec_dt data[NUM_PEs],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23, unsigned int updatesptrs[MAX_NUM_LLPSETS]){
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
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
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
void dinsertmany_vdatadram(unsigned int offset__, unsigned int offsets[NUM_PEs], unsigned int t, vprop_t datas[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
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
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
	return;
}
void dretrievemany_vdatadram(unsigned int offset__, unsigned int offsets[NUM_PEs], unsigned int t, vprop_t datas[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
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
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
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
void dinsertmany_cfrontierdram_tmp(unsigned int base_offset__, unsigned int offsets[NUM_PEs], unsigned int t, keyvalue_t datas[NUM_PEs][EDGE_PACK_SIZE], bool ens[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
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
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
	return;
}
void dretrievemany_cfrontierdram_tmp(unsigned int base_offset__, unsigned int offsets[NUM_PEs], unsigned int t, keyvalue_t datas[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
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
	master_retrievemany_vec(offsets_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
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
void dinsertmany_nfrontierdram(unsigned int offset__, unsigned int p_u, unsigned int offsets[NUM_PEs], unsigned int t, keyvalue_t datas[NUM_PEs][EDGE_PACK_SIZE], bool ens[NUM_PEs][EDGE_PACK_SIZE],  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23){
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
	master_insertmany_vec(offsets_, ens_, data_,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
}

//////////////////////////////////////////////////////////////////
// vprop_t retrieve_2DDbuffer(unsigned int v, unsigned int index, vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
	// #ifdef _DEBUGMODE_CHECKS3
	// checkoutofbounds("acts_kernel::ERROR 101::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	// #endif 
	// return vdata_buffer[v][index];
// }

void insert_2DVbuffer(unsigned int v, unsigned int index, vprop_t data, vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 102::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	vdata_buffer[v][index] = data; // vdata_buffer
}
vprop_t retrieve_2DVbuffer(unsigned int v, unsigned int index, vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 103::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return vdata_buffer[v][index];
}
void insert_2DVbuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 104::", index, MAX_UPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		vdata_buffer[v][index] = data[v];
	}
}
vprop_t retrieve_2DVbuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], vprop_t * vdata_buffer[EDGE_PACK_SIZE]){
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

void insert_vptrbuffer(unsigned int t, vtr_t data, vtr_t * _1D_buffer){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 129::", t, VPTR_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	_1D_buffer[t] = data;
}
vtr_t retrieve_vptrbuffer(unsigned int t, vtr_t * _1D_buffer){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 130::", t, VPTR_BUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return _1D_buffer[t];
}

void insert_frontierbuffer(unsigned int v, unsigned int t, frontier_t data, keyvalue_t * _2D_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 113::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	_2D_buffer[v][t] = data;
}
frontier_t retrieve_frontierbuffer(unsigned int v, unsigned int t, keyvalue_t * _2D_buffer[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 114::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return _2D_buffer[v][t];
}
void insert_frontierbuffer(unsigned int t, frontier_t data[EDGE_PACK_SIZE], keyvalue_t * _2D_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 115::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		_2D_buffer[v][t] = data[v];
	}
}
void retrieve_frontierbuffer(unsigned int t, frontier_t data[EDGE_PACK_SIZE], keyvalue_t * _2D_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 116::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = _2D_buffer[v][t];
	}
	return; 
}

void insert_statstmpbuffer(unsigned int t, unsigned int v, unsigned int data, unsigned int * statsbuffer_maskbased_tmp[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 121::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	statsbuffer_maskbased_tmp[v][t] = data;
}
unsigned int retrieve_statstmpbuffer(unsigned int t, unsigned int v, unsigned int * statsbuffer_maskbased_tmp[EDGE_PACK_SIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 1242::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return statsbuffer_maskbased_tmp[v][t]; 
}
void insert_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], unsigned int * statsbuffer_maskbased_tmp[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 123::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		statsbuffer_maskbased_tmp[v][t] = data[v];
	}
}
void retrieve_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], unsigned int * statsbuffer_maskbased_tmp[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_kernel::ERROR 124::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = statsbuffer_maskbased_tmp[v][t];
	}
	return; 
}

void insert_statsbuffer(unsigned int t, unsigned int v, unsigned int data, unsigned int * statsbuffer_maskbased[MAX_NUM_APPLYPARTITIONS]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 125::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	statsbuffer_maskbased[v][t] = data;
}
unsigned int retrieve_statsbuffer(unsigned int t, unsigned int v, unsigned int * statsbuffer_maskbased[MAX_NUM_APPLYPARTITIONS]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 126::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	return statsbuffer_maskbased[v][t]; 
}

void insert_stats2buffer(unsigned int t, unsigned int v, unsigned int data, unsigned int * statsbuffer_idbased[MAX_NUM_APPLYPARTITIONS]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 127::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	statsbuffer_idbased[v][t] = data;
}
unsigned int retrieve_stats2buffer(unsigned int t, unsigned int v, unsigned int * statsbuffer_idbased[MAX_NUM_APPLYPARTITIONS]){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_kernel::ERROR 128::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
	#endif 
	return statsbuffer_idbased[v][t]; 
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
			#ifdef ___FORCE_SUCCESS___
			if(*edges_buffer___size + 8 < EDGE_BUFFER_SIZE){ *edges_buffer___size += 1; } // FIXME. 'forces successful run' // NEW
			#else 
			*edges_buffer___size += 1;	
			#endif 
		}
	}
	return;
}

void transport_frontier(unsigned int inst, unsigned int p_v, unsigned int baseoffset_nfdram, unsigned int * cfrontier_dram___size, unsigned int * nfrontier_dram___size[EDGE_PACK_SIZE], offset_t * upartition_vertices, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB, HBM_channelX_t * HBM_center, unsigned int * totalactvvs2){
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
			#ifdef _DEBUGMODE_KERNELPRINTS4 // NEW
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA){ *totalactvvs2 += 1; }} // FIXME.
			#endif 
			insert_cfrontierdram(p_actvv_, cfrontier_dram___size[p_actvv_], actvv, HBM_center); cfrontier_dram___size[p_actvv_] += 1;
		}
	}
}

#ifdef FPGA_IMPL
void top_function(HBM_channelX_t * HBM_center,  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23)
#else 
void acts_kernel::top_function(HBM_channelX_t * HBM_center,  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23)
#endif 
{	
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
#pragma HLS INTERFACE m_axi port = HBM_channelA6 offset = slave bundle = gmem12
#pragma HLS INTERFACE m_axi port = HBM_channelB6 offset = slave bundle = gmem13
#pragma HLS INTERFACE m_axi port = HBM_channelA7 offset = slave bundle = gmem14
#pragma HLS INTERFACE m_axi port = HBM_channelB7 offset = slave bundle = gmem15
#pragma HLS INTERFACE m_axi port = HBM_channelA8 offset = slave bundle = gmem16
#pragma HLS INTERFACE m_axi port = HBM_channelB8 offset = slave bundle = gmem17
#pragma HLS INTERFACE m_axi port = HBM_channelA9 offset = slave bundle = gmem18
#pragma HLS INTERFACE m_axi port = HBM_channelB9 offset = slave bundle = gmem19
#pragma HLS INTERFACE m_axi port = HBM_channelA10 offset = slave bundle = gmem20
#pragma HLS INTERFACE m_axi port = HBM_channelB10 offset = slave bundle = gmem21
#pragma HLS INTERFACE m_axi port = HBM_channelA11 offset = slave bundle = gmem22
#pragma HLS INTERFACE m_axi port = HBM_channelB11 offset = slave bundle = gmem23
#pragma HLS INTERFACE m_axi port = HBM_channelA12 offset = slave bundle = gmem24
#pragma HLS INTERFACE m_axi port = HBM_channelB12 offset = slave bundle = gmem25
#pragma HLS INTERFACE m_axi port = HBM_channelA13 offset = slave bundle = gmem26
#pragma HLS INTERFACE m_axi port = HBM_channelB13 offset = slave bundle = gmem27
#pragma HLS INTERFACE m_axi port = HBM_channelA14 offset = slave bundle = gmem28
#pragma HLS INTERFACE m_axi port = HBM_channelB14 offset = slave bundle = gmem29
#pragma HLS INTERFACE m_axi port = HBM_channelA15 offset = slave bundle = gmem30
#pragma HLS INTERFACE m_axi port = HBM_channelB15 offset = slave bundle = gmem31
#pragma HLS INTERFACE m_axi port = HBM_channelA16 offset = slave bundle = gmem32
#pragma HLS INTERFACE m_axi port = HBM_channelB16 offset = slave bundle = gmem33
#pragma HLS INTERFACE m_axi port = HBM_channelA17 offset = slave bundle = gmem34
#pragma HLS INTERFACE m_axi port = HBM_channelB17 offset = slave bundle = gmem35
#pragma HLS INTERFACE m_axi port = HBM_channelA18 offset = slave bundle = gmem36
#pragma HLS INTERFACE m_axi port = HBM_channelB18 offset = slave bundle = gmem37
#pragma HLS INTERFACE m_axi port = HBM_channelA19 offset = slave bundle = gmem38
#pragma HLS INTERFACE m_axi port = HBM_channelB19 offset = slave bundle = gmem39
#pragma HLS INTERFACE m_axi port = HBM_channelA20 offset = slave bundle = gmem40
#pragma HLS INTERFACE m_axi port = HBM_channelB20 offset = slave bundle = gmem41
#pragma HLS INTERFACE m_axi port = HBM_channelA21 offset = slave bundle = gmem42
#pragma HLS INTERFACE m_axi port = HBM_channelB21 offset = slave bundle = gmem43
#pragma HLS INTERFACE m_axi port = HBM_channelA22 offset = slave bundle = gmem44
#pragma HLS INTERFACE m_axi port = HBM_channelB22 offset = slave bundle = gmem45
#pragma HLS INTERFACE m_axi port = HBM_channelA23 offset = slave bundle = gmem46
#pragma HLS INTERFACE m_axi port = HBM_channelB23 offset = slave bundle = gmem47
#pragma HLS INTERFACE m_axi port = HBM_center offset = slave bundle = gmem48

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
#pragma HLS INTERFACE s_axilite port = HBM_channelA6 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB6 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA7 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB7 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA8 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB8 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA9 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB9 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA10 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB10 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA11 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB11 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA12 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB12 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA13 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB13 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA14 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB14 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA15 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB15 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA16 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB16 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA17 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB17 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA18 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB18 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA19 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB19 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA20 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB20 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA21 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB21 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA22 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB22 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelA23 bundle = control
#pragma HLS INTERFACE s_axilite port = HBM_channelB23 bundle = control
	
#pragma HLS INTERFACE s_axilite port = HBM_center bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = HBM_channelA0
#pragma HLS DATA_PACK variable = HBM_channelB0
#pragma HLS DATA_PACK variable = HBM_channelA1
#pragma HLS DATA_PACK variable = HBM_channelB1
#pragma HLS DATA_PACK variable = HBM_channelA2
#pragma HLS DATA_PACK variable = HBM_channelB2
#pragma HLS DATA_PACK variable = HBM_channelA3
#pragma HLS DATA_PACK variable = HBM_channelB3
#pragma HLS DATA_PACK variable = HBM_channelA4
#pragma HLS DATA_PACK variable = HBM_channelB4
#pragma HLS DATA_PACK variable = HBM_channelA5
#pragma HLS DATA_PACK variable = HBM_channelB5
#pragma HLS DATA_PACK variable = HBM_channelA6
#pragma HLS DATA_PACK variable = HBM_channelB6
#pragma HLS DATA_PACK variable = HBM_channelA7
#pragma HLS DATA_PACK variable = HBM_channelB7
#pragma HLS DATA_PACK variable = HBM_channelA8
#pragma HLS DATA_PACK variable = HBM_channelB8
#pragma HLS DATA_PACK variable = HBM_channelA9
#pragma HLS DATA_PACK variable = HBM_channelB9
#pragma HLS DATA_PACK variable = HBM_channelA10
#pragma HLS DATA_PACK variable = HBM_channelB10
#pragma HLS DATA_PACK variable = HBM_channelA11
#pragma HLS DATA_PACK variable = HBM_channelB11
#pragma HLS DATA_PACK variable = HBM_channelA12
#pragma HLS DATA_PACK variable = HBM_channelB12
#pragma HLS DATA_PACK variable = HBM_channelA13
#pragma HLS DATA_PACK variable = HBM_channelB13
#pragma HLS DATA_PACK variable = HBM_channelA14
#pragma HLS DATA_PACK variable = HBM_channelB14
#pragma HLS DATA_PACK variable = HBM_channelA15
#pragma HLS DATA_PACK variable = HBM_channelB15
#pragma HLS DATA_PACK variable = HBM_channelA16
#pragma HLS DATA_PACK variable = HBM_channelB16
#pragma HLS DATA_PACK variable = HBM_channelA17
#pragma HLS DATA_PACK variable = HBM_channelB17
#pragma HLS DATA_PACK variable = HBM_channelA18
#pragma HLS DATA_PACK variable = HBM_channelB18
#pragma HLS DATA_PACK variable = HBM_channelA19
#pragma HLS DATA_PACK variable = HBM_channelB19
#pragma HLS DATA_PACK variable = HBM_channelA20
#pragma HLS DATA_PACK variable = HBM_channelB20
#pragma HLS DATA_PACK variable = HBM_channelA21
#pragma HLS DATA_PACK variable = HBM_channelB21
#pragma HLS DATA_PACK variable = HBM_channelA22
#pragma HLS DATA_PACK variable = HBM_channelB22
#pragma HLS DATA_PACK variable = HBM_channelA23
#pragma HLS DATA_PACK variable = HBM_channelB23
#pragma HLS DATA_PACK variable = HBM_center
	
	#ifdef _DEBUGMODE_KERNELPRINTS4
	cout<<"acts_kernel::run:: acts started "<<endl;
	#endif 

	// declaration of BRAM variables
	#ifdef FPGA_IMPL		
	keyvalue_t nfrontier_buffer[NUM_PEs][EDGE_PACK_SIZE][MAX_APPLYPARTITION_VECSIZE]; // FIXME. remove it-obsolete
	keyvalue_t cfrontier_bufferREAL0[EDGE_PACK_SIZE]; 
	keyvalue_t cfrontier_bufferREAL1[NUM_PEs][MAX_APPLYPARTITION_VECSIZE];
	unsigned int statsbuffer_maskbased_tmp[NUM_PEs][EDGE_PACK_SIZE][BLOCKRAM_SIZE];
	unsigned int statsbuffer_maskbased[NUM_PEs][MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE];
	unsigned int statsbuffer_idbased[NUM_PEs][MAX_NUM_APPLYPARTITIONS][BLOCKRAM_SIZE];
	keyvalue_t updates_tmpbuffer[NUM_TMPS_BUFFER][NUM_PEs][CSRBUFFER_SIZE]; // FIXME (CSRBUFFER_SIZE too large)
	keyvalue_t updates_buffer[NUM_PEs][EDGE_PACK_SIZE][MAX_APPLYPARTITION_VECSIZE];
	vtr_t vptr_buffer[NUM_PEs][VPTR_BUFFER_SIZE];
	edge3_type edges_buffer[NUM_PEs][EDGE_PACK_SIZE][EDGE_BUFFER_SIZE];
	vprop_t vdata_buffer[NUM_PEs][EDGE_PACK_SIZE];
	#else 
	keyvalue_t * nfrontier_buffer[NUM_PEs][EDGE_PACK_SIZE]; // FIXME. remove it-obsolete
	keyvalue_t * nfrontier_bufferOPTIMAL[NUM_PEs][EDGE_PACK_SIZE]; 
	keyvalue_t * cfrontier_bufferREAL0[EDGE_PACK_SIZE]; 
	keyvalue_t * cfrontier_bufferREAL1[NUM_PEs]; 
	unsigned int * statsbuffer_maskbased_tmp[NUM_PEs][EDGE_PACK_SIZE]; 
	unsigned int * statsbuffer_maskbased[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	unsigned int * statsbuffer_idbased[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	keyvalue_t * updates_tmpbuffer[NUM_TMPS_BUFFER][NUM_PEs]; 
	keyvalue_t * updates_buffer[NUM_PEs][EDGE_PACK_SIZE];
	vtr_t * vptr_buffer[NUM_PEs]; 
	edge3_type * edges_buffer[NUM_PEs][EDGE_PACK_SIZE];
	vprop_t * vdata_buffer[NUM_PEs][EDGE_PACK_SIZE]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer[i][v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_bufferOPTIMAL[i][v] = new keyvalue_t[MAX_ACTVV_VECSIZE]; }}
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cfrontier_bufferREAL0[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ cfrontier_bufferREAL1[i] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ statsbuffer_maskbased_tmp[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ statsbuffer_maskbased[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ statsbuffer_idbased[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_TMPS_BUFFER; i++){ for(unsigned int p=0; p<NUM_PEs; p++){ updates_tmpbuffer[i][p] = new keyvalue_t[CSRBUFFER_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer[i][v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ vptr_buffer[i] = new vtr_t[VPTR_BUFFER_SIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ edges_buffer[i][v] = new edge3_type[EDGE_BUFFER_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ vdata_buffer[i][v] = new vprop_t[MAX_APPLYPARTITION_VECSIZE]; }}
	#endif 
	
	#ifdef FPGA_IMPL	
	unsigned int cfrontier_dram___size[MAX_NUM_UPARTITIONS]; 
	unsigned int nfrontier_dram___size[NUM_PEs][EDGE_PACK_SIZE][MAX_NUM_UPARTITIONS]; // FIXME. remove first dimension - not necessary
	#pragma HLS ARRAY_PARTITION variable = nfrontier_dram___size complete dim=1
	unsigned int nfrontier_buffer___size[NUM_PEs][EDGE_PACK_SIZE][NUM_ACTVVPARTITIONS_PER_APPLYPARTITION]; 
	#pragma HLS ARRAY_PARTITION variable = nfrontier_buffer___size complete dim=1
	unsigned int updates_tmpbuffer___size[NUM_PEs][NUM_PEs]; 
	#pragma HLS ARRAY_PARTITION variable = updates_tmpbuffer___size complete dim=1
	unsigned int updates_buffer___size[NUM_PEs][EDGE_PACK_SIZE][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = updates_buffer___size complete dim=1
	unsigned int stats_buffer___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = stats_buffer___size complete dim=1
	unsigned int stats2_buffer___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = stats2_buffer___size complete dim=1
	offset_t upartition_vertices[MAX_NUM_UPARTITIONS]; 
	offset_t vpartition_vertices[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = vpartition_vertices complete dim=1
	unsigned int actpackupdates_dram___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	#pragma HLS ARRAY_PARTITION variable = actpackupdates_dram___size complete dim=1
	unsigned int vptrbuffer___size[NUM_PEs]; 
	unsigned int edges_buffer___size[NUM_PEs]; 
	#else 
	unsigned int * cfrontier_dram___size = new unsigned int[MAX_NUM_UPARTITIONS]; 
	// FIXME. remove first dimension - not necessary
	unsigned int * nfrontier_dram___size[NUM_PEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_dram___size[i][v] = new unsigned int[MAX_NUM_UPARTITIONS]; }}
	unsigned int * nfrontier_buffer___size[NUM_PEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer___size[i][v] = new unsigned int[NUM_ACTVVPARTITIONS_PER_APPLYPARTITION]; }}
	unsigned int updates_tmpbuffer___size[NUM_PEs][NUM_PEs]; 
	unsigned int * updates_buffer___size[NUM_PEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer___size[i][v] = new unsigned int[MAX_NUM_APPLYPARTITIONS]; }}
	unsigned int stats_buffer___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	unsigned int stats2_buffer___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	offset_t * upartition_vertices = new offset_t[MAX_NUM_UPARTITIONS]; 
	offset_t * vpartition_vertices[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ vpartition_vertices[i] = new offset_t[MAX_NUM_APPLYPARTITIONS];  }
	unsigned int actpackupdates_dram___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	unsigned int vptrbuffer___size[NUM_PEs]; 
	unsigned int edges_buffer___size[NUM_PEs]; 
	#endif 
	
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
	frontier_t actvv[EDGE_PACK_SIZE];
	unsigned int vid_first0[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable = vid_first0 complete
	unsigned int vid_first1[NUM_PEs]; 
	#pragma HLS ARRAY_PARTITION variable = vid_first1 complete
	unsigned int globalparams[32];
	unsigned int updatesptrs[MAX_NUM_LLPSETS];
	
	unsigned int CONST1 = NUM_PEs * EDGE_PACK_SIZE; 
	unsigned int CONST2 = MAX_ACTVV_VECSIZE * NUM_PEs * EDGE_PACK_SIZE;
	
	// load global parameters
	#pragma HLS ARRAY_PARTITION variable = globalparams complete
	for(unsigned int t=0; t<32; t++){
		globalparams[t] = dretrieve_globalparams(0, t, HBM_channelA0, HBM_channelB0, 0);
	}
	#ifndef FPGA_IMPL
	globalparams_debug = (unsigned int *)&globalparams[0];
	#endif 
	
	// unsigned int __NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	unsigned int __NUM_UPARTITIONS = (globalparams[GLOBALPARAMSCODE__PARAM__NUM_VERTICES] + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	unsigned int __NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; 
	cout<<"acts_kernel::run:: universalparams.NUM_VERTICES: "<<universalparams.NUM_VERTICES<<", universalparams.NUM_EDGES: "<<universalparams.NUM_EDGES<<", MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<", __NUM_UPARTITIONS: "<<__NUM_UPARTITIONS<<", MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<", __NUM_APPLYPARTITIONS: "<<__NUM_APPLYPARTITIONS<<endl;			
	
	for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ cfrontier_dram___size[p] = 0; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ nfrontier_dram___size[i][v][p] = 0; }}}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p++){ nfrontier_buffer___size[i][v][p] = 0; }}}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_APPLYPARTITIONS; p++){ updates_buffer___size[i][v][p] = 0; }}}
	for(unsigned int p=0; p<__NUM_UPARTITIONS; p++){ upartition_vertices[p].offset = 0; upartition_vertices[p].size = 0; upartition_vertices[p].count = 0; } upartition_vertices[0].count = 1;
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ vpartition_vertices[i][p].offset = 0; vpartition_vertices[i][p].size = 0; vpartition_vertices[i][p].count = 0; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ actpackupdates_dram___size[i][p] = 0; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ vptrbuffer___size[i] = 0; }
	for(unsigned int i=0; i<NUM_PEs; i++){ edges_buffer___size[i] = 0; }
						
	unsigned int num_LLPs = universalparams.NUM_APPLYPARTITIONS * universalparams.NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (universalparams.NUM_PARTITIONS - 1)) / universalparams.NUM_PARTITIONS;
	unsigned int maxGraphIter = 0;
	
	bool run_vertex_centric = true;
	unsigned int * hybrid_map[MAXNUMGRAPHITERATIONS];
	for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ hybrid_map[i] = new unsigned int[MAX_NUM_UPARTITIONS]; }
	for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ for(unsigned int t=0; t<MAX_NUM_UPARTITIONS; t++){ hybrid_map[i][t] = NAp; }}
	bool enable___vertexcentric = true;
	unsigned int threshold___activefrontiers = 128; // 16, 128*; // NEW
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
	for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int t=0; t<MAX_UPARTITION_VECSIZE; t++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ insert_2DVbuffer(v, t, prop, vdata_buffer[inst]); }}} // 

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
				for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ insert_statsbuffer(t, p_v, 0, statsbuffer_maskbased[inst]); }
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
				dinsertmany_cfrontierdram_tmp(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], offsets, t, kvdatas, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
			}
			
			// parallel-read and map active frontiers [done]
			for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = 0; }
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				for(unsigned int t=0; t<cfrontier_dram___size[p_u]; t++){
				#pragma HLS PIPELINE II=1
					dretrievemany_cfrontierdram_tmp(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP], offsets, t, kvdatas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
					// insert_cfrontierbuffer(t, kvdatas[0], cfrontier_bufferREAL0);
					insert_frontierbuffer(t, kvdatas[0], cfrontier_bufferREAL0); // NEW
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
					#pragma HLS UNROLL
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							if(kvdatas[inst][v].key != INVALIDDATA){
								unsigned int srcvid_lpv = get_local_to_upartition(kvdatas[inst][v].key) / EDGE_PACK_SIZE; // FIXME.
								vprop_t prop; prop.prop = kvdatas[inst][v].value; prop.mask = MASK_CODE_PE; 
								insert_2DVbuffer(v, srcvid_lpv, prop, vdata_buffer[inst]); 
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
							insert_statstmpbuffer(t, v, 0, statsbuffer_maskbased_tmp[inst]); 
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
						// frontier_t actvv = retrieve_cfrontierbuffer(pingpong % EDGE_PACK_SIZE, pingpong / EDGE_PACK_SIZE, cfrontier_bufferREAL0);
						frontier_t actvv = retrieve_frontierbuffer(pingpong % EDGE_PACK_SIZE, pingpong / EDGE_PACK_SIZE, cfrontier_bufferREAL0); // NEW
						if(actvv.key != INVALIDDATA){
							unsigned int inst_ = actvv.key % NUM_PEs;
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
							unsigned int hvid = 0;
							if(t < cfrontier_bufferREAL___size[n]){ hvid = actvvs[n].key / NUM_PEs; } // FIXME. (send local instead?)
							offsets[n] = hvid; 
						}
						dretrievemany_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], offsets, begins_u32,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
						
						VC_READVPTRS_LOOP2C: for(unsigned int n=0; n<NUM_PEs; n++){
						#pragma HLS UNROLL
							offsets[n] += 1; 
						}
						dretrievemany_vptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS], offsets, ends_u32,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
						
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
						
						// load edges
						VC_READ_EDGES_LOOP1: for(unsigned int k=0; k<batch_size; k++){ 
						#pragma HLS PIPELINE II=1
	
							load_edges(0, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[0], vptr_buffer[0], &edges_buffer___size[0], edges_buffer[0], HBM_channelA0, HBM_channelB0);
	
							load_edges(1, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[1], vptr_buffer[1], &edges_buffer___size[1], edges_buffer[1], HBM_channelA1, HBM_channelB1);
	
							load_edges(2, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[2], vptr_buffer[2], &edges_buffer___size[2], edges_buffer[2], HBM_channelA2, HBM_channelB2);
	
							load_edges(3, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[3], vptr_buffer[3], &edges_buffer___size[3], edges_buffer[3], HBM_channelA3, HBM_channelB3);
	
							load_edges(4, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[4], vptr_buffer[4], &edges_buffer___size[4], edges_buffer[4], HBM_channelA4, HBM_channelB4);
	
							load_edges(5, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[5], vptr_buffer[5], &edges_buffer___size[5], edges_buffer[5], HBM_channelA5, HBM_channelB5);
	
							load_edges(6, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[6], vptr_buffer[6], &edges_buffer___size[6], edges_buffer[6], HBM_channelA6, HBM_channelB6);
	
							load_edges(7, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[7], vptr_buffer[7], &edges_buffer___size[7], edges_buffer[7], HBM_channelA7, HBM_channelB7);
	
							load_edges(8, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[8], vptr_buffer[8], &edges_buffer___size[8], edges_buffer[8], HBM_channelA8, HBM_channelB8);
	
							load_edges(9, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[9], vptr_buffer[9], &edges_buffer___size[9], edges_buffer[9], HBM_channelA9, HBM_channelB9);
	
							load_edges(10, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[10], vptr_buffer[10], &edges_buffer___size[10], edges_buffer[10], HBM_channelA10, HBM_channelB10);
	
							load_edges(11, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[11], vptr_buffer[11], &edges_buffer___size[11], edges_buffer[11], HBM_channelA11, HBM_channelB11);
	
							load_edges(12, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[12], vptr_buffer[12], &edges_buffer___size[12], edges_buffer[12], HBM_channelA12, HBM_channelB12);
	
							load_edges(13, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[13], vptr_buffer[13], &edges_buffer___size[13], edges_buffer[13], HBM_channelA13, HBM_channelB13);
	
							load_edges(14, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[14], vptr_buffer[14], &edges_buffer___size[14], edges_buffer[14], HBM_channelA14, HBM_channelB14);
	
							load_edges(15, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[15], vptr_buffer[15], &edges_buffer___size[15], edges_buffer[15], HBM_channelA15, HBM_channelB15);
	
							load_edges(16, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[16], vptr_buffer[16], &edges_buffer___size[16], edges_buffer[16], HBM_channelA16, HBM_channelB16);
	
							load_edges(17, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[17], vptr_buffer[17], &edges_buffer___size[17], edges_buffer[17], HBM_channelA17, HBM_channelB17);
	
							load_edges(18, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[18], vptr_buffer[18], &edges_buffer___size[18], edges_buffer[18], HBM_channelA18, HBM_channelB18);
	
							load_edges(19, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[19], vptr_buffer[19], &edges_buffer___size[19], edges_buffer[19], HBM_channelA19, HBM_channelB19);
	
							load_edges(20, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[20], vptr_buffer[20], &edges_buffer___size[20], edges_buffer[20], HBM_channelA20, HBM_channelB20);
	
							load_edges(21, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[21], vptr_buffer[21], &edges_buffer___size[21], edges_buffer[21], HBM_channelA21, HBM_channelB21);
	
							load_edges(22, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[22], vptr_buffer[22], &edges_buffer___size[22], edges_buffer[22], HBM_channelA22, HBM_channelB22);
	
							load_edges(23, ((n * batch_size) + k), globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES], vptrbuffer___size[23], vptr_buffer[23], &edges_buffer___size[23], edges_buffer[23], HBM_channelA23, HBM_channelB23);
		
						}			
					
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
										if(t < edges_buffer___size[inst]){ 
											insert_updatesbuffer(inst_mirror, updates_tmpbuffer___size[inst][inst_mirror], update, updates_tmpbuffer[inst]); // 
											updates_tmpbuffer___size[inst][inst_mirror] += 1;	
										}
									}
								}
							}
						}
					}
					
					// exchange updates between HBMs [done]
					VC_EXCHANGE_UPDATES_LOOP1: for(unsigned int inst_mirror=0; inst_mirror<NUM_PEs; inst_mirror++){
						VC_EXCHANGE_UPDATES_LOOP1B: for(unsigned int inst=0; inst<NUM_PEs; inst++){
							VC_EXCHANGE_UPDATES_LOOP1C: for(unsigned int t=0; t<updates_tmpbuffer___size[inst][inst_mirror]; t++){
								// keyvalue_t update = retrieve_updatestmpbuffer(inst_mirror, t, updates_tmpbuffer[inst]); // 
								keyvalue_t update = retrieve_updatesbuffer(inst_mirror, t, updates_tmpbuffer[inst]); // 
								unsigned int p_v = get_local(update.key) / MAX_APPLYPARTITION_SIZE; // FIXME.
								keyvalue_t update_local = update; update_local.key = get_local_to_vpartition(get_local(update.key));
								unsigned int v = update_local.key % EDGE_PACK_SIZE;
								#ifdef _DEBUGMODE_KERNELPRINTS//4
								cout<<"VC - EXCHANGE UPDATE SEEN @: [inst: "<<inst<<", inst_mirror: "<<inst_mirror<<"], [update.key: "<<update.key<<", update.value: "<<update.value<<"]"<<endl;
								#endif 
								
								// if(get_local(update.key) % EDGE_PACK_SIZE != v){ cout<<"~~~~~~~~~~~~ get_local(update.key) % EDGE_PACK_SIZE != v. [update.key: "<<update.key<<", update_local.key: "<<update_local.key<<"] EXITING.... "<<endl; exit(EXIT_FAILURE); }
								// #ifdef _DEBUGMODE_CHECKS3
								// if((updates_vecs[inst].data[v].key % EDGE_PACK_SIZE) != v){ cout<<"acts_kernel 1234:: updates_vecs["<<inst<<"].data["<<v<<"].key("<<updates_vecs[inst].data[v].key<<") % v("<<v<<") != 0. EXITING..."<<endl; for(unsigned int v1=0; v1<EDGE_PACK_SIZE; v1++){ cout<<"updates_vecs["<<inst<<"].data["<<v1<<"].key: "<<updates_vecs[inst].data[v1].key<<""<<endl; } exit(EXIT_FAILURE); }
								// #endif 
								
								#ifdef _DEBUGMODE_CHECKS3
								checkoutofbounds("acts_kernel::ERROR 123::", p_v, __NUM_APPLYPARTITIONS, update.key, get_local(update.key), MAX_APPLYPARTITION_SIZE);
								checkoutofbounds("acts_kernel::ERROR 122::", (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + updates_buffer___size[inst_mirror][v][p_v], MAX_APPLYPARTITION_VECSIZE, updates_buffer___size[inst_mirror][v][p_v], inst_mirror, p_v);
								#endif 
								insert_updatesbuffer(v, (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + updates_buffer___size[inst_mirror][v][p_v], update_local, updates_buffer[inst_mirror]);
								#ifdef ___FORCE_SUCCESS___
								if(updates_buffer___size[inst_mirror][v][p_v] + 8 < MAX_APPLYPARTITION_VECSIZE){ updates_buffer___size[inst_mirror][v][p_v] += 1; } // FIXME. 'forces successful run' // NEW
								#else 
								updates_buffer___size[inst_mirror][v][p_v] += 1;
								#endif 
								vpartition_vertices[0][p_v].count = 1;
								
								unsigned int dstvid_lpv = update_local.key / EDGE_PACK_SIZE; // FIXME.
								checkoutofbounds("acts_kernel::ERROR 171::", dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE, BLOCKRAM_SIZE, NAp, NAp, NAp);
								checkoutofbounds("acts_kernel::ERROR 172::", p_v, __NUM_APPLYPARTITIONS, NAp, NAp, NAp);
								if(enable___collectactivedstvids == true){ insert_statsbuffer(dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE, p_v, 1, statsbuffer_maskbased[inst_mirror]); }
							}
						}
					}
				} else { // edge-centric [done]
					hybrid_map[GraphIter][p_u] = 1;
					EC_PROCESS_EDGES_LOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
						map_t maps[NUM_PEs];
						dretrievemany_actpackvptrdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS], p_u, llp_set, maps,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
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
								
								dretrievemany_act_pack_edges(globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES], offsets, t, edge3_vecs,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
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
											
											uprop[inst][v] = retrieve_2DVbuffer(v, srcvid_lpv, vdata_buffer[inst]);
											
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
											insert_updatesbuffer(v, updates_tmpbuffer___size[inst][0], updates_vec.data[v], updates_tmpbuffer[inst]); 
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
												insert_statstmpbuffer(index, v, 1, statsbuffer_maskbased_tmp[inst]);
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
										updates_vecs[inst].data[v] = retrieve_updatesbuffer(v, t, updates_tmpbuffer[inst]); 
										if(t < updates_tmpbuffer___size[inst][0]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
									}
								}
								dinsertmany_updatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[llp_set], llp_set, offsets, t, updates_vecs, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, updatesptrs);
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
								if(enable___collectactivedstvids == true){ retrieve_statstmpbuffer(t, data, statsbuffer_maskbased_tmp[inst]); }
								checkoutofbounds("acts_kernel::ERROR 023::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
								if(data[0]==1 || data[1]==1 || data[2]==1 || data[3]==1 || data[4]==1 || data[5]==1 || data[6]==1 || data[7]==1 || data[8]==1 || data[9]==1 || data[10]==1 || data[11]==1 || data[12]==1 || data[13]==1 || data[14]==1 || data[15]==1){ 
									if(enable___collectactivedstvids == true){ insert_statsbuffer(t, llp_set, 1, statsbuffer_maskbased[inst]); }
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
						if(t < updates_buffer___size[inst][v][p_v]){ 
							updates_vecs[inst].data[v] = retrieve_updatesbuffer(v, (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + t, updates_buffer[inst]); 
							
							#ifdef _DEBUGMODE_CHECKS3
							if((updates_vecs[inst].data[v].key % EDGE_PACK_SIZE) != v){ cout<<"acts_kernel 1234:: updates_vecs["<<inst<<"].data["<<v<<"].key("<<updates_vecs[inst].data[v].key<<") % v("<<v<<") != 0. EXITING..."<<endl; for(unsigned int v1=0; v1<EDGE_PACK_SIZE; v1++){ cout<<"updates_vecs["<<inst<<"].data["<<v1<<"].key: "<<updates_vecs[inst].data[v1].key<<""<<endl; } exit(EXIT_FAILURE); }
							#endif 
						}
						else { updates_vecs[inst].data[v].key = INVALIDDATA; updates_vecs[inst].data[v].value = INVALIDDATA;  }
						if(t < max_limits[inst]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						if(t < updates_buffer___size[inst][v][p_v]){ cout<<"VC - COMMIT CSR UPDATES SEEN @: [inst: "<<inst<<"], [updates_vecs["<<inst<<"].data["<<v<<"].key: "<<updates_vecs[inst].data[v].key<<", updates_vecs["<<inst<<"].data["<<v<<"].value: "<<updates_vecs[inst].data[v].value<<"]"<<endl; }
						#endif
					}
				}
				checkoutofbounds("acts_kernel::ERROR 152::", actpackupdates_dram___size[0][p_v], HBM_CHANNEL_SIZE, NAp, NAp, NAp);
				dinsertmany_updatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v], p_v, offsets, t, updates_vecs, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, updatesptrs);
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
						if(retrieve_statsbuffer(t, p_v, statsbuffer_maskbased[inst]) == 1){ 
							#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
							cout<<"VC - PREPARE ACTIVE DESTIDS SEEN @: [inst: "<<inst<<"], [p_v: "<<p_v<<", t: "<<t<<"]"<<endl;
							#endif
							insert_stats2buffer(stats2_buffer___size[inst][p_v], p_v, t, statsbuffer_idbased[inst]); 
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
							unsigned int offset = retrieve_stats2buffer(k, p_v, statsbuffer_idbased[inst]) * ACTVUPDATESBLOCK_VECSIZE;
							checkoutofbounds("acts_kernel::ERROR 117::", offset, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
							for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								if(offset + t < MAX_APPLYPARTITION_VECSIZE){ 
									dretrieve_vdatadram(voffset, offset + t, data, HBM_channelA[inst], HBM_channelB[inst], inst);
									insert_2DVbuffer(offset + t, data, vdata_buffer[inst]);
								}
							}
						}	
					}
					#endif 
				} else {
					for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
					#pragma HLS PIPELINE II=1
						dretrievemany_vdatadram(voffset, offsets, t, datas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
						#pragma HLS UNROLL
							insert_2DVbuffer(t, datas[inst], vdata_buffer[inst]);
						}
					}
				}
				
				// apply updates [done]
				uint512_vec_dt updates_vecs[NUM_PEs];
				max_limit = 0; for(unsigned int inst=0; inst<NUM_PEs; inst++){ limits[inst] = actpackupdates_dram___size[inst][p_v]; } for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}
				for(unsigned int t=0; t<max_limit; t++){
				#pragma HLS PIPELINE II=1
					dretrievemany_udatesdram(globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] + updatesptrs[p_v], p_v, t, updates_vecs,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23, updatesptrs); // NEW
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
					#pragma HLS UNROLL
						uint512_vec_dt updates_vec = updates_vecs[inst];
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							keyvalue_t update = updates_vec.data[v];
							unsigned int dstvid_lp = update.key; 
							unsigned int dstvid_lpv = update.key / EDGE_PACK_SIZE; 
							if(update.key != INVALIDDATA){
								#ifdef _DEBUGMODE_CHECKS3
								#ifndef ___FORCE_SUCCESS___
								if(t < limits[inst] && ((dstvid_lp % EDGE_PACK_SIZE) != v)){ cout<<"acts_kernel 234:: dstvid_lp("<<dstvid_lp<<") % v("<<v<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }
								#endif 
								if(t < limits[inst]){ checkoutofbounds("acts_kernel::ERROR 727::", dstvid_lp, MAX_APPLYPARTITION_SIZE, NAp, inst, update.key); checkoutofbounds("acts_kernel::ERROR 727::", dstvid_lpv, MAX_APPLYPARTITION_VECSIZE, NAp, inst, update.key); }
								#endif 
								vprop_t vprop =  retrieve_2DVbuffer(v, dstvid_lpv, vdata_buffer[inst]);
								unsigned int new_vprop = 0; if(update.value < vprop.prop){ new_vprop = update.value; } else { new_vprop = vprop.prop; }
								if(new_vprop != vprop.prop){
									#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
									cout<<"APPLY (ACT-PACK) UPDATE SEEN @: inst: ["<<inst<<"]: dstvid_lp: "<<dstvid_lp<<", dstvid_lpv: "<<dstvid_lpv<<", new_vprop: "<<new_vprop<<", vid: "<<update.key<<endl;
									#endif
									vprop_t newprop; newprop.prop = new_vprop; newprop.mask = MASK_CODE_AU;
									if(t < limits[inst]){ insert_2DVbuffer(v, dstvid_lpv, newprop, vdata_buffer[inst]); } // NEW
								}
							}
						}
					}
				}

				// collect and save frontier information
				for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = 0; }
				for(unsigned int n=0; n<NUM_PEs; n++){ 
				#pragma HLS UNROLL
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
					#pragma HLS UNROLL
						for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){ nfrontier_buffer___size[n][v][p_actvv] = 0; }
					}
				}
				for(unsigned int n=0; n<NUM_PEs; n++){ 
				#pragma HLS UNROLL
					vid_first0[n] = get_global(((p_v * MAX_APPLYPARTITION_SIZE) + ((0 * MAX_ACTVV_VECSIZE + 0) * EDGE_PACK_SIZE + 0)), n); 
				}


				for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
					// collect frontier information [done]
					max_limit = 0; for(unsigned int inst=0; inst<NUM_PEs; inst++){ limits[inst] = MAX_ACTVV_VECSIZE; } for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
					#pragma HLS UNROLL
						vid_first1[inst] = vid_first0[inst]; 
					}
					for(unsigned int t=0; t<max_limit; t++){
					#pragma HLS PIPELINE II=1
						unsigned int dstvid_lpv = p_actvv * MAX_ACTVV_VECSIZE + t;
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
						#pragma HLS UNROLL
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							#pragma HLS UNROLL
								#ifdef _DEBUGMODE_CHECKS3
								checkoutofbounds("acts_kernel::ERROR 713::", dstvid_lpv, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
								#endif 
								vprop_t vprop =  retrieve_2DVbuffer(v, dstvid_lpv, vdata_buffer[inst]);
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
										checkoutofbounds("acts_kernel::ERROR 21888::", nfrontier_buffer___size[inst][v][p_actvv], MAX_ACTVV_VECSIZE, NAp, NAp, NAp);
										insert_frontierbuffer(v, nfrontier_buffer___size[inst][v][p_actvv], actvv, nfrontier_bufferOPTIMAL[inst]); // NEW*
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

					// save frontier information [done]
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
							checkoutofbounds("acts_kernel::ERROR 21777::", t, MAX_ACTVV_VECSIZE, NAp, NAp, NAp);
							retrieve_frontierbuffer(t, actvvs[inst], nfrontier_bufferOPTIMAL[inst]); // NEW*
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ // 
							#pragma HLS UNROLL
								if(t >= nfrontier_buffer___size[inst][v][p_actvv]){ actvvs[inst][v].key = INVALIDDATA; actvvs[inst][v].value = INVALIDDATA; }
								if(t < max_limits[inst]){ ens[inst][v] = true; } else { ens[inst][v] = false; }
							}
						}}
						dinsertmany_nfrontierdram(offset, p_actvv_, offsets, t, actvvs, ens,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
						for(unsigned int n=0; n<NUM_PEs; n++){ 
						#pragma HLS UNROLL
							if(t < max_limits[n]){ nfrontier_dram___size[n][0][p_actvv_] += 1; }
						}
					}
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
					#pragma HLS UNROLL
						vid_first0[inst] += CONST2; 
					}
				}
				
				// save destination properties
				for(unsigned int n=0; n<NUM_PEs; n++){ offsets[n] = 0; }
				if(stats_buffer___size[0][p_v] < threshold___activedstvids && enable___collectactivedstvids == true){
					#ifdef ___ENABLE___COLLECTACTIVEDSTVIDS___
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ // FIXME.
						for(unsigned int k=0; k<stats2_buffer___size[inst][p_v]; k++){	
							unsigned int offset = retrieve_stats2buffer(k, p_v, statsbuffer_idbased[inst]) * ACTVUPDATESBLOCK_VECSIZE;
							for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								if(offset + t < MAX_APPLYPARTITION_VECSIZE){ 
									checkoutofbounds("acts_kernel::ERROR 217::", offset + t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
									retrieve_2DVbuffer(offset + t, data, vdata_buffer[inst]);
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
							retrieve_2DVbuffer(t, datas[inst], vdata_buffer[inst]);
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
							#pragma HLS UNROLL
								datas[inst][v].mask = INVALIDMASK; // clear mask. 
							}
						}
						dinsertmany_vdatadram(voffset, offsets, t, datas,  HBM_channelA0, HBM_channelB0, HBM_channelA1, HBM_channelB1, HBM_channelA2, HBM_channelB2, HBM_channelA3, HBM_channelB3, HBM_channelA4, HBM_channelB4, HBM_channelA5, HBM_channelB5, HBM_channelA6, HBM_channelB6, HBM_channelA7, HBM_channelB7, HBM_channelA8, HBM_channelB8, HBM_channelA9, HBM_channelB9, HBM_channelA10, HBM_channelB10, HBM_channelA11, HBM_channelB11, HBM_channelA12, HBM_channelB12, HBM_channelA13, HBM_channelB13, HBM_channelA14, HBM_channelB14, HBM_channelA15, HBM_channelB15, HBM_channelA16, HBM_channelB16, HBM_channelA17, HBM_channelB17, HBM_channelA18, HBM_channelB18, HBM_channelA19, HBM_channelB19, HBM_channelA20, HBM_channelB20, HBM_channelA21, HBM_channelB21, HBM_channelA22, HBM_channelB22, HBM_channelA23, HBM_channelB23);
					}
				}
			}
		} // p_v

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
		
		for(unsigned int t=0; t<__NUM_UPARTITIONS; t++){ if(hybrid_map[GraphIter][t] == 0){ cout<<"V, "; } else if(hybrid_map[GraphIter][t] == 1){ cout<<"E, "; } else { cout<<"-, "; }} cout<<endl;
		if(run_vertex_centric == true){ cout<<"### Vertex-Centric GAS iteration: "<<GraphIter + 1<<" ["<<totalactvvs2<<" active vertices]"<<endl; } else { cout<<"### Edge-Centric GAS iteration: "<<GraphIter + 1<<" ["<<totalactvvs2<<" active vertices]"<<endl;	 }
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ if(upartition_vertices[p_u].count > 0){ cout<<"1, "; } else { cout<<"0, "; }} cout<<endl;
		if(totalactvvs2 == 0 || GraphIter >= MAXNUMGRAPHITERATIONS){ cout<<"no more active vertices to process. breaking out... "<<endl; maxGraphIter = GraphIter; break; }
		for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ nfrontier_dram___size[inst][0][p_u] = 0; }}
	} // NUM_ITs
	return;
}











