#include "acts_sw.h"
using namespace std;

acts_sw::acts_sw(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
	algorithmobj = new algorithm();
}
acts_sw::~acts_sw(){} 

void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3){
	if(data >= upper_bound){ std::cout<<"utility::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<", msgdata1: "<<msgdata1<<", msgdata2: "<<msgdata2<<", msgdata3: "<<msgdata3<<std::endl; exit(EXIT_FAILURE); }			
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
edge3_vec_dt retrieve_csr_pack_edges_arr(unsigned int index, HBM_channel_t * HBM_channel){
	return HBM_channel->csr_pack_edges_arr[index];
}

edge3_vec_dt retrieve_act_pack_edges_arr(unsigned int index, HBM_channel_t * HBM_channel){
	return HBM_channel->act_pack_edges_arr[index];
}

void insert_vdatadram(unsigned int p_v, unsigned int index, vprop_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		HBM_channel->vdatas_dram[p_v][index].data[v] = data[v];
	}
	return;
}
void retrieve_vdatadram(unsigned int p_v, unsigned int index, vprop_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v] = HBM_channel->vdatas_dram[p_v][index].data[v];
	}
	return;
}

void insert_cfrontierdram_i(unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		HBM_channel->cfrontier_dram_i[t].data[v] = data[v];
	}
}
void retrieve_cfrontierdram_i(unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = HBM_channel->cfrontier_dram_i[t].data[v];
	}
	return; 
}

void insert_nfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		HBM_channel->nfrontier_dram[p_u][t].data[v] = data[v];
	}
}
void retrieve_nfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], HBM_channel_t * HBM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = HBM_channel->nfrontier_dram[p_u][t].data[v];
	}
	return; 
}

void insert_actpackupdatesdram(unsigned int llp_set, unsigned int index, uint512_uvec_dt data, HBM_channel_t * HBM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		HBM_channel->actpackupdates_dram[llp_set][index] = data;					
	}
}
void retrieve_actpackupdatesdram(unsigned int llp_set, unsigned int index, uint512_uvec_dt * data, HBM_channel_t * HBM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		*data = HBM_channel->actpackupdates_dram[llp_set][index];					
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
	BRAM_channel->vertex_buffer[v][index].mask = 0; // vdata_buffer
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

void insert_csrupdatesbuffer(unsigned int index, unsigned int t, keyvalue_t data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 109::", index, NUM_PEs, NAp, NAp, NAp);
	checkoutofbounds("acts_sw::ERROR 110::", t, CSRBUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->csrupdates_buffer[index][t] = data;
}
keyvalue_t retrieve_csrupdatesbuffer(unsigned int index, unsigned int t, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 111::", index, NUM_PEs, NAp, NAp, NAp);
	checkoutofbounds("acts_sw::ERROR 112::", t, CSRBUFFER_SIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->csrupdates_buffer[index][t];
}

void insert_cfrontierbuffer_i(unsigned int v, unsigned int t, frontier_t data, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 113::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	BRAM_channel->cfrontier_buffer_i[v][t] = data;
}
frontier_t retrieve_cfrontierbuffer_i(unsigned int v, unsigned int t, BRAM_channel_t * BRAM_channel){
	#ifdef _DEBUGMODE_CHECKS3
	checkoutofbounds("acts_sw::ERROR 114::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
	#endif 
	return BRAM_channel->cfrontier_buffer_i[v][t];
}
void insert_cfrontierbuffer_i(unsigned int t, frontier_t data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 115::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		BRAM_channel->cfrontier_buffer_i[v][t] = data[v];
	}
}
void retrieve_cfrontierbuffer_i(unsigned int t, frontier_t data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		#ifdef _DEBUGMODE_CHECKS3
		checkoutofbounds("acts_sw::ERROR 116::", t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
		#endif 
		data[v] = BRAM_channel->cfrontier_buffer_i[v][t];
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
	BRAM_channel->stats_tmpbuffer[v][t] = data;
}
unsigned int retrieve_statstmpbuffer(unsigned int t, unsigned int v, BRAM_channel_t * BRAM_channel){
	return BRAM_channel->stats_tmpbuffer[v][t]; 
}
void insert_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		BRAM_channel->stats_tmpbuffer[v][t] = data[v];
	}
}
void retrieve_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], BRAM_channel_t * BRAM_channel){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = BRAM_channel->stats_tmpbuffer[v][t];
	}
	return; 
}

void insert_statsbuffer(unsigned int t, unsigned int v, unsigned int data, BRAM_channel_t * BRAM_channel){
	BRAM_channel->stats_buffer[v][t] = data;
}
unsigned int retrieve_statsbuffer(unsigned int t, unsigned int v, BRAM_channel_t * BRAM_channel){
	return BRAM_channel->stats_buffer[v][t]; 
}

void insert_stats2buffer(unsigned int t, unsigned int v, unsigned int data, BRAM_channel_t * BRAM_channel){
	BRAM_channel->stats2_buffer[v][t] = data;
}
unsigned int retrieve_stats2buffer(unsigned int t, unsigned int v, BRAM_channel_t * BRAM_channel){
	return BRAM_channel->stats2_buffer[v][t]; 
}

//////////////////////////////////////////////////////////////////

void acts_sw::run(unsigned int * v_ptr[NUM_PEs], map_t * act_pack_map[NUM_PEs][MAX_NUM_UPARTITIONS], HBM_center_t * HBM_center, HBM_channel_t HBM_channel[NUM_PEs]){					

	unsigned int __NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	unsigned int __NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs
	cout<<"acts_sw::run:: universalparams.NUM_VERTICES: "<<universalparams.NUM_VERTICES<<", universalparams.NUM_EDGES: "<<universalparams.NUM_EDGES<<", MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<", __NUM_UPARTITIONS: "<<__NUM_UPARTITIONS<<", MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<", __NUM_APPLYPARTITIONS: "<<__NUM_APPLYPARTITIONS<<endl;			
	
	BRAM_channel_t BRAM_channel[NUM_PEs];
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].nfrontier_buffer[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].cfrontier_buffer_i[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].stats_tmpbuffer[v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ BRAM_channel[i].stats_buffer[v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ BRAM_channel[i].stats2_buffer[v] = new unsigned int[BLOCKRAM_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<NUM_PEs; p++){ BRAM_channel[i].csrupdates_buffer[p] = new keyvalue_t[CSRBUFFER_SIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].vertex_buffer[v] = new vprop_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ BRAM_channel[i].updates_buffer[v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	
	unsigned int * cfrontier_dram___size[EDGE_PACK_SIZE]; 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cfrontier_dram___size[v] = new unsigned int[MAX_NUM_UPARTITIONS]; }
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ cfrontier_dram___size[v][p] = 0; }}
	
	unsigned int * nfrontier_dram___size[NUM_PEs][EDGE_PACK_SIZE]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_dram___size[i][v] = new unsigned int[MAX_NUM_UPARTITIONS]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ nfrontier_dram___size[i][v][p] = 0; }}}
	
	unsigned int * nfrontier_buffer___size[NUM_PEs][EDGE_PACK_SIZE]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer___size[i][v] = new unsigned int[NUM_ACTVVPARTITIONS_PER_APPLYPARTITION]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p++){ nfrontier_buffer___size[i][v][p] = 0; }}}
	
	unsigned int * updates_buffer___size[NUM_PEs][EDGE_PACK_SIZE]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_buffer___size[i][v] = new unsigned int[MAX_NUM_APPLYPARTITIONS]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_APPLYPARTITIONS; p++){ updates_buffer___size[i][v][p] = 0; }}}
	
	unsigned int stats_buffer___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	unsigned int stats2_buffer___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	
	offset_t * upartition_vertices = new offset_t[MAX_NUM_UPARTITIONS]; 
	for(unsigned int p=0; p<__NUM_UPARTITIONS; p++){ upartition_vertices[p].offset = 0; upartition_vertices[p].size = 0; upartition_vertices[p].count = 0; } upartition_vertices[0].count = 1;
	
	offset_t * vpartition_vertices[NUM_PEs]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ vpartition_vertices[i] = new offset_t[MAX_NUM_APPLYPARTITIONS];  }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ vpartition_vertices[i][p].offset = 0; vpartition_vertices[i][p].size = 0; vpartition_vertices[i][p].count = 0; }}
	
	keyvalue_t * csrupdates_dram[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ csrupdates_dram[i][p] = new keyvalue_t[CSRDRAM_SIZE]; }}
	
	unsigned int csrupdates_buffer___size[NUM_PEs][NUM_PEs]; 
	
	unsigned int actpackupdates_dram___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ actpackupdates_dram___size[i][p] = 0; }}
	
	vtr_t * vptr_buffer[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ vptr_buffer[i] = new vtr_t[BLOCKRAM_SIZE]; }
	unsigned int vptr_buffer___size[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ vptr_buffer___size[i] = 0; }
	
	edge3_type edges[NUM_PEs][EDGE_PACK_SIZE];
	unsigned int res[NUM_PEs][EDGE_PACK_SIZE];
	unsigned int vtemp_in[NUM_PEs][EDGE_PACK_SIZE];
	unsigned int vtemp_out[NUM_PEs][EDGE_PACK_SIZE];
	vprop_t uprop[NUM_PEs][EDGE_PACK_SIZE];
	keyvalue_t update_in[NUM_PEs][EDGE_PACK_SIZE];
	keyvalue_t update_out[NUM_PEs][EDGE_PACK_SIZE];
						
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
	unsigned int max_limits[NUM_PEs];
	unsigned int max_limit = 0; 
	
	unsigned int rootvid = 1; keyvalue_t root; root.key = rootvid; root.value = 0; keyvalue_t invalid; invalid.key = INVALIDDATA; invalid.value = INVALIDDATA; 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(v==1){ HBM_center->cfrontier_dram[v][0][0] = root; } else { HBM_center->cfrontier_dram[v][0][0] = invalid; } cfrontier_dram___size[v][0] = 1; }
	
	if(run_vertex_centric == true){ cout<<"### Vertex-Centric GAS iteration: 0 [1 active vertices]"<<endl; } else { cout<<"### Edge-Centric GAS iteration: 0 [1 active vertices]"<<endl; }
	
	// reset ubuffer 
	vprop_t prop; prop.prop = 0; prop.mask = 0; 
	for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int t=0; t<MAX_UPARTITION_VECSIZE; t++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ insert_udatabuffer(v, t, prop, &BRAM_channel[inst]); }}} // 
			
	for(unsigned int GraphIter=0; GraphIter<12; GraphIter++){ // MAXNUMGRAPHITERATIONS
		// unsigned int MASK_CODE = 1; // NEW-MASK*
		// unsigned int MASK_CODE = 7;
		unsigned int MASK_CODE = 5 + GraphIter;
		
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
			bool ___use_vertex_centric___ = false; if(upartition_vertices[p_u].count < threshold___activefrontiers && enable___vertexcentric == true){ ___use_vertex_centric___ = true; }
	
			// broadcast active frontiers
			for(unsigned int t=0; t<cfrontier_dram___size[0][p_u]; t++){
			#pragma HLS PIPELINE II=1
				retrieve_cfrontierdram(p_u, t, data, HBM_center);
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
				#pragma HLS UNROLL
					insert_cfrontierdram_i(t, data, &HBM_channel[inst]);
				}
			}
			
			// parallel-read active frontiers
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				for(unsigned int t=0; t<cfrontier_dram___size[0][p_u]; t++){
				#pragma HLS PIPELINE II=1
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
					#pragma HLS UNROLL
						retrieve_cfrontierdram_i(t, data, &HBM_channel[inst]);
						insert_cfrontierbuffer_i(t, data, &BRAM_channel[inst]); // 
					}
				}
			}
			
			// clear ubuffer // FIXME.
			// vprop_t prop; prop.prop = 0; prop.mask = 0; 
			// for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int t=0; t<MAX_UPARTITION_VECSIZE; t++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ insert_udatabuffer(v, t, prop, &BRAM_channel[inst]); }}} // 
			
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
			for(unsigned int t=0; t<cfrontier_dram___size[0][p_u]; t++){
			#pragma HLS PIPELINE II=1
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
					keyvalue_t actvv[EDGE_PACK_SIZE];
					retrieve_cfrontierbuffer_i(t, actvv, &BRAM_channel[inst]); 
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						if(actvv[v].key != INVALIDDATA){
							unsigned int srcvid_lpv = get_local_to_upartition(actvv[v].key) / EDGE_PACK_SIZE; 
							vprop_t prop; prop.prop = actvv[v].value; prop.mask = MASK_CODE; // NEW-MASK*
							insert_udatabuffer(v, srcvid_lpv, prop, &BRAM_channel[inst]); 
						}
					}
				}
			}
			
			for(unsigned int i=0; i<NUM_PEs; i++){ 
			#pragma HLS UNROLL
				for(unsigned int j=0; j<NUM_PEs; j++){ csrupdates_buffer___size[i][j] = 0; }
			} // clear
			
			// process-edges and partition-updates
			if(upartition_vertices[p_u].count > 0){
				// if(upartition_vertices[p_u].count < threshold___activefrontiers && enable___vertexcentric == true){ // vertex-centric
				if(___use_vertex_centric___ == true){
					hybrid_map[GraphIter][p_u] = 0;
					
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ vptr_buffer___size[inst] = 0; }
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
						// read edge locations
						VC_READVPTRS_LOOP1: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							VC_READVPTRS_LOOP1B: for(unsigned int t=0; t<cfrontier_dram___size[0][p_u]; t++){ 
							#pragma HLS PIPELINE II=1
								frontier_t actvv = retrieve_cfrontierbuffer_i(v, t, &BRAM_channel[inst]);
								unsigned int vid = actvv.key; 
								unsigned int hvid = vid / NUM_PEs;
								
								vtr_t data;
								data.begin = v_ptr[inst][hvid];
								data.end = v_ptr[inst][hvid+1];
								data.size = data.end - data.begin;
								data.vid = actvv.key; 
								data.prop = actvv.value; 
								
								if((actvv.key != INVALIDDATA) && (vid % NUM_PEs == inst)){ 
									checkoutofbounds("acts_sw::ERROR 003::", vptr_buffer___size[inst], CSRBUFFER_SIZE, NAp, NAp, NAp);
									vptr_buffer[inst][vptr_buffer___size[inst]] = data; 
									vptr_buffer___size[inst] += 1;
								}
							}
						}
						
						// process edges 
						VC_PROCESS_EDGES_LOOP1: for(unsigned int t=0; t<vptr_buffer___size[inst]; t++){ 
						#pragma HLS PIPELINE II=1
							vtr_t vptr_data = vptr_buffer[inst][t];
							unsigned int vid = vptr_data.vid;
							unsigned int edges_size = vptr_data.end - vptr_data.begin;
							unsigned int edgelist_size = (edges_size / EDGE_PACK_SIZE) + 2; if((edges_size == 0) || (vptr_data.end < vptr_data.begin)){ edgelist_size = 0; }
							unsigned int uprop = vptr_data.prop;
							
							VC_PROCESS_EDGES_LOOP1B: for(unsigned int k=0; k<edgelist_size; k++){
								edge3_vec_dt edges = retrieve_csr_pack_edges_arr((vptr_data.begin / EDGE_PACK_SIZE) + k, &HBM_channel[inst]);
								
								for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
									if(edges.data[v].srcvid == vid && edges.data[v].srcvid != INVALIDDATA){
										edge3_type edge = edges.data[v];
										
										// process edge 
										unsigned int res = uprop + 1;
										unsigned int vtemp = min(uprop, res);
										#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
										cout<<"VC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", uprop: "<<uprop<<", res: "<<res<<", vtemp: "<<vtemp<<"]"<<endl;
										cout<<"VC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [local partition: "<<p_u<<", vid: "<<vid<<"]"<<endl;
										#endif 
										
										// partition 
										unsigned int inst_mirror = get_H(edge.dstvid);
										keyvalue_t update; update.key = edge.dstvid; update.value = vtemp; // res;
										checkoutofbounds("acts_sw::ERROR 113::", csrupdates_buffer___size[inst][inst_mirror], CSRBUFFER_SIZE, NAp, NAp, NAp);
										insert_csrupdatesbuffer(inst_mirror, csrupdates_buffer___size[inst][inst_mirror], update, &BRAM_channel[inst]); // 
										csrupdates_buffer___size[inst][inst_mirror] += 1;	
									}
								}
							}
						}
					}
					
					// exchange updates between HBMs
					VC_EXCHANGE_UPDATES_LOOP1: for(unsigned int inst_mirror=0; inst_mirror<NUM_PEs; inst_mirror++){
						VC_EXCHANGE_UPDATES_LOOP1B: for(unsigned int inst=0; inst<NUM_PEs; inst++){
							VC_EXCHANGE_UPDATES_LOOP1C: for(unsigned int t=0; t<csrupdates_buffer___size[inst][inst_mirror]; t++){
								keyvalue_t update = retrieve_csrupdatesbuffer(inst_mirror, t, &BRAM_channel[inst]); // 
								unsigned int p_v = get_local(update.key) / MAX_APPLYPARTITION_SIZE; // FIXME.
								keyvalue_t update_local = update; update_local.key = get_local_to_vpartition(get_local(update.key));
								unsigned int v = update_local.key % EDGE_PACK_SIZE;
								#ifdef _DEBUGMODE_KERNELPRINTS//4
								cout<<"VC - EXCHANGE UPDATE SEEN @: [inst: "<<inst<<", inst_mirror: "<<inst_mirror<<"], [update.key: "<<update.key<<", update.value: "<<update.value<<"]"<<endl;
								#endif 
								
								checkoutofbounds("acts_sw::ERROR 123::", p_v, __NUM_APPLYPARTITIONS, NAp, NAp, NAp);
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
				} else { // edge-centric
					hybrid_map[GraphIter][p_u] = 1;
					EC_PROCESS_EDGES_LOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
						max_limit = 0; for(unsigned int inst=0; inst<NUM_PEs; inst++){ limits[inst] = act_pack_map[inst][p_u][llp_set].size; } for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}
						EC_PROCESS_EDGES_LOOP1B: for(unsigned int t=0; t<max_limit; t++){
						#pragma HLS PIPELINE II=1
							EC_PROCESS_EDGES_LOOP1C: for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(t < limits[inst]){
							#pragma HLS UNROLL
								edge3_vec_dt edge_vec = retrieve_act_pack_edges_arr(act_pack_map[inst][p_u][llp_set].offset + t, &HBM_channel[inst]);
								unsigned int rotate_forward = edge_vec.data[EDGE_PACK_SIZE].srcvid; unsigned int rotateby = edge_vec.data[EDGE_PACK_SIZE + 1].srcvid; unsigned int sample_key = NAp; unsigned int sample_u = NAp;
								
								EC_PROCESS_EDGES_LOOP1D: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
								#pragma HLS UNROLL
									if(edge_vec.data[v].srcvid != INVALIDDATA){
										edges[inst][v] = edge_vec.data[v];
										unsigned int srcvid_lp = get_local_to_upartition(edges[inst][v].srcvid);
										unsigned int srcvid_lpv = get_local_to_upartition(edges[inst][v].srcvid) / EDGE_PACK_SIZE;
										checkoutofbounds("acts_sw::ERROR 223::", srcvid_lp, MAX_UPARTITION_SIZE, NAp, NAp, NAp);
										if(srcvid_lp % EDGE_PACK_SIZE != v){ cout<<"acts_sw:: srcvid_lp("<<srcvid_lp<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
										
										uprop[inst][v] = retrieve_udatabuffer(v, srcvid_lpv, &BRAM_channel[inst]);
										
										res[inst][v] = uprop[inst][v].prop + 1;
										vtemp_in[inst][v] = min(uprop[inst][v].prop, res[inst][v]);
										update_in[inst][v].key = edges[inst][v].dstvid; update_in[inst][v].value = vtemp_in[inst][v];
										if(uprop[inst][v].mask != MASK_CODE){ update_in[inst][v].key = INVALIDDATA; update_in[inst][v].value = INVALIDDATA; } // NEW-MASK*
										#ifdef _DEBUGMODE_KERNELPRINTS//4
										if(uprop[inst][v].mask == MASK_CODE){ cout<<"EC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [srcvid_lp: "<<srcvid_lp<<", dstvid: "<<edges[inst][v].dstvid % MAX_APPLYPARTITION_SIZE<<"], [edge.srcvid: "<<edges[inst][v].srcvid<<", dstvid: "<<edges[inst][v].dstvid<<"]"<<endl; }
										#endif 
									} else {
										uprop[inst][v].mask = 0; update_in[inst][v].key = INVALIDDATA; update_in[inst][v].value = INVALIDDATA;
									}
								}
								
								// circular shift >>>
								checkoutofbounds("acts_sw::ERROR 213::", rotateby, EDGE_PACK_SIZE, NAp, NAp, NAp);
								if(rotate_forward == 0){ rearrangeLayoutVx16B(rotateby, update_in[inst], update_out[inst]); } else{ rearrangeLayoutVx16F(rotateby, update_in[inst], update_out[inst]); }
								#ifdef _DEBUGMODE_CHECKS3
								checkoutofbounds("acts_sw::ERROR 59::", rotateby, EDGE_PACK_SIZE, sample_key, NAp, NAp);
								checkoutofbounds("acts_sw::ERROR 60::", rotate_forward, 2, sample_key, NAp, NAp);	
								check_if_contiguous(update_out[inst], update_in[inst], update_in[inst], sample_key, rotate_forward, rotateby);
								#endif 
								
								// partition and save updates
								uint512_uvec_dt updates_vec; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_vec.data[v] = update_out[inst][v]; }
								bool isvalid = false; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(update_out[inst][v].key != INVALIDDATA){ isvalid = true; }} // FIXME.
								if(isvalid == true){ insert_actpackupdatesdram(llp_set, actpackupdates_dram___size[inst][llp_set], updates_vec, &HBM_channel[inst]); actpackupdates_dram___size[inst][llp_set] += 1; } // 
							
								// collect stats information 
								if(enable___collectactivedstvids == true && isvalid == true){ 
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
										if(update_out[inst][v].key != INVALIDDATA){
											unsigned int dstvid_lpv = update_out[inst][v].key / EDGE_PACK_SIZE; 
											unsigned int index = dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE;
											checkoutofbounds("acts_sw::ERROR 123::", index, BLOCKRAM_SIZE, update_out[inst][v].key, dstvid_lpv, (MAX_APPLYPARTITION_VECSIZE / BLOCKRAM_SIZE));
											insert_statstmpbuffer(index, v, 1, &BRAM_channel[inst]);
										}
									}
								}
							}}
						}
						
						// gather stats 
						unsigned int data[EDGE_PACK_SIZE];
						for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
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
		
		// save csr updates
		for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
			for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
				unsigned int max = 0; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(max < updates_buffer___size[inst][v][p_v]){ max = updates_buffer___size[inst][v][p_v]; }}
				for(unsigned int t=0; t<max; t++){
					uint512_uvec_dt updates_vec;
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						if(t < updates_buffer___size[inst][v][p_v]){
							updates_vec.data[v] = retrieve_updatesbuffer(v, (p_v * MAX_CSRUPDATES_VECSIZE_PER__APPLYPARTITION) + t, &BRAM_channel[inst]);
							#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
							cout<<"VC - COMMIT CSR UPDATES SEEN @: [inst: "<<inst<<"], [updates_vec.data["<<v<<"].key: "<<updates_vec.data[v].key<<", updates_vec.data["<<v<<"].value: "<<updates_vec.data[v].value<<"]"<<endl;
							#endif
						} else {
							updates_vec.data[v].key = INVALIDDATA; updates_vec.data[v].value = INVALIDDATA; 
						}
					}
					checkoutofbounds("acts_sw::ERROR 152::", actpackupdates_dram___size[inst][p_v], HBM_CHANNEL_SIZE, NAp, NAp, NAp);
					insert_actpackupdatesdram(p_v, actpackupdates_dram___size[inst][p_v], updates_vec, &HBM_channel[inst]);
					actpackupdates_dram___size[inst][p_v] += 1;
				}
			}
		}
		
		// prepare active destID stats
		if(enable___collectactivedstvids == true){
			for(unsigned int inst=0; inst<NUM_PEs; inst++){
				for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
					for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
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
			if(vpartition_vertices[0][p_v].count > 0){
				// read destination properties
				vprop_t data[EDGE_PACK_SIZE];
				if(stats_buffer___size[0][p_v] <= threshold___activedstvids && enable___collectactivedstvids == true){
				// if(false){
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
						for(unsigned int k=0; k<stats2_buffer___size[inst][p_v]; k++){
							unsigned int offset = retrieve_stats2buffer(k, p_v, &BRAM_channel[inst]) * ACTVUPDATESBLOCK_VECSIZE;
							checkoutofbounds("acts_sw::ERROR 117::", offset, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
							for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								if(offset + t < MAX_APPLYPARTITION_VECSIZE){ 
									retrieve_vdatadram(p_v, offset + t, data, &HBM_channel[inst]);
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
							retrieve_vdatadram(p_v, t, data, &HBM_channel[inst]);
							insert_vdatabuffer(t, data, &BRAM_channel[inst]);
						}
					}
				}
				
				// apply updates
				max_limit = 0; for(unsigned int inst=0; inst<NUM_PEs; inst++){ limits[inst] = actpackupdates_dram___size[inst][p_v]; } for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}
				for(unsigned int t=0; t<max_limit; t++){
				#pragma HLS PIPELINE II=1
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(t < limits[inst]){
					#pragma HLS UNROLL
						uint512_uvec_dt updates_vec;
						retrieve_actpackupdatesdram(p_v, t, &updates_vec, &HBM_channel[inst]); // 
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							keyvalue_t update = updates_vec.data[v];
							unsigned int dstvid_lp = update.key; 
							unsigned int dstvid_lpv = update.key / EDGE_PACK_SIZE; 
							if(update.key != INVALIDDATA){
								if((dstvid_lp % EDGE_PACK_SIZE) != v){ cout<<"acts_sw 234:: dstvid_lp("<<dstvid_lp<<") % v("<<v<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }
								checkoutofbounds("acts_sw::ERROR 727::", dstvid_lp, MAX_APPLYPARTITION_SIZE, NAp, NAp, update.key);
								vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, &BRAM_channel[inst]);
								unsigned int new_vprop = 0; if(update.value < vprop.prop){ new_vprop = update.value; } else { new_vprop = vprop.prop; }
								if(new_vprop != vprop.prop){
									#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
									cout<<"APPLY (ACT-PACK) UPDATE SEEN @: inst: ["<<inst<<"]: dstvid_lp: "<<dstvid_lp<<", dstvid_lpv: "<<dstvid_lpv<<", new_vprop: "<<new_vprop<<", vid: "<<update.key<<endl;
									#endif
									vprop_t newprop; newprop.prop = new_vprop; newprop.mask = MASK_CODE; // 1; // NEW-MASK*
									insert_vdatabuffer(v, dstvid_lpv, newprop, &BRAM_channel[inst]);
								}
							}
						}
					}}
				}

				// collect frontier information
				unsigned int vid_first000[NUM_PEs];
				unsigned int vid_first01s[NUM_PEs]; 
				for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){ nfrontier_buffer___size[inst][v][p_actvv] = 0; }}}
				for(unsigned int inst=0; inst<NUM_PEs; inst++){ vid_first000[inst] = get_global(((p_v * MAX_APPLYPARTITION_SIZE) + ((0 * MAX_ACTVV_VECSIZE + 0) * EDGE_PACK_SIZE + 0)), inst); }
				for(unsigned int n=0; n<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; n++){
					max_limit = 0; for(unsigned int inst=0; inst<NUM_PEs; inst++){ limits[inst] = MAX_ACTVV_VECSIZE; } for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(max_limit < limits[inst]){ max_limit = limits[inst]; }}
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
					#pragma HLS UNROLL
						vid_first01s[inst] = vid_first000[inst]; 
					}
					for(unsigned int t=0; t<max_limit; t++){
					#pragma HLS PIPELINE II=1
						unsigned int dstvid_lpv = n * MAX_ACTVV_VECSIZE + t;
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(t < limits[inst]){
						#pragma HLS UNROLL
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							#pragma HLS UNROLL
								checkoutofbounds("acts_sw::ERROR 713::", dstvid_lpv, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
								vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, &BRAM_channel[inst]);
								unsigned int vid = vid_first01s[inst] + v;
								#ifdef _DEBUGMODE_CHECKS3
								if(vid % EDGE_PACK_SIZE != v){ cout<<"acts_sw:: ERROR 234. vid("<<vid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<vid % EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
								#endif
								unsigned int p_actvv = dstvid_lpv / MAX_ACTVV_VECSIZE;
								if(vprop.mask == MASK_CODE){ // NEW-MASK*
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
							vid_first01s[inst] += (NUM_PEs * EDGE_PACK_SIZE); 
						} 
					}
					for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
					#pragma HLS UNROLL
						vid_first000[inst] += (MAX_ACTVV_VECSIZE * NUM_PEs * EDGE_PACK_SIZE); 
					}
				}
				
				// commit frontier information
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
					frontier_t actvv[EDGE_PACK_SIZE];
					for(unsigned int t=0; t<max_limit; t++){
					#pragma HLS PIPELINE II=1
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ if(t < max_limits[inst]){
							retrieve_nfrontierbuffer(p_actvv * MAX_ACTVV_VECSIZE + t, actvv, &BRAM_channel[inst]);
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ // 
							#pragma HLS UNROLL
								if(t >= nfrontier_buffer___size[inst][v][p_actvv]){ actvv[v].key = INVALIDDATA; actvv[v].value = INVALIDDATA; }
							}
							insert_nfrontierdram(p_actvv_, nfrontier_dram___size[inst][0][p_actvv_], actvv, &HBM_channel[inst]);
							nfrontier_dram___size[inst][0][p_actvv_] += 1;
						}}
					}
				}
				
				// commit destination properties
				if(stats_buffer___size[0][p_v] <= threshold___activedstvids && enable___collectactivedstvids == true){
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
						for(unsigned int k=0; k<stats2_buffer___size[inst][p_v]; k++){	
							unsigned int offset = retrieve_stats2buffer(k, p_v, &BRAM_channel[inst]) * ACTVUPDATESBLOCK_VECSIZE;
							for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								if(offset + t < MAX_APPLYPARTITION_VECSIZE){ 
									checkoutofbounds("acts_sw::ERROR 217::", offset + t, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
									retrieve_vdatabuffer(offset + t, data, &BRAM_channel[inst]);
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
									#pragma HLS UNROLL
										data[v].mask = 0; // clear mask. // NEW-MASK*
									} 
									insert_vdatadram(p_v, offset + t, data, &HBM_channel[inst]);
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
								data[v].mask = 0; // clear mask. NEW-MASK*
							} 
							insert_vdatadram(p_v, t, data, &HBM_channel[inst]);
						}
					}
				}
			}
		} // p_v
		
		// end of GAS iteration - gathering frontier information
		unsigned int totalactvvs2 = 0; frontier_t actvv[EDGE_PACK_SIZE]; 
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ upartition_vertices[p_u].count = 0; }
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cfrontier_dram___size[v][p_u] = 0; }} // reset
		for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			for(unsigned int inst=0; inst<NUM_PEs; inst++){
				for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
					unsigned int p_actvv_ = p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION + p_actvv;
					checkoutofbounds("acts_sw::ERROR 225::", p_actvv_, MAX_NUM_UPARTITIONS, NAp, NAp, NAp);
					for(unsigned int t=0; t<nfrontier_dram___size[inst][0][p_actvv_]; t++){
					#pragma HLS PIPELINE II=1
						upartition_vertices[p_actvv_].count += 1; 
						retrieve_nfrontierdram(p_actvv_, t, actvv, &HBM_channel[inst]);
						#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
						for(unsigned int v=0; v<0; v++){ cout<<"TRANSFER VID-BASED FRONTIERS SEEN @: H: "<<inst<<" => vid: "<<actvv[v].key<<", p_actvv_: "<<p_actvv_<<endl; }
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA && actvv[v].key / MAX_UPARTITION_SIZE != p_actvv_){ cout<<"ERROR 234: actvv["<<v<<"].key("<<actvv[v].key<<") / MAX_UPARTITION_SIZE("<<MAX_UPARTITION_SIZE<<")(="<<actvv[v].key / MAX_UPARTITION_SIZE<<") != p_actvv_("<<p_actvv_<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }}				
						#endif 
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA){ totalactvvs2 += 1; }} // FIXME.
						insert_cfrontierdram(p_actvv_, cfrontier_dram___size[0][p_actvv_], actvv, HBM_center); cfrontier_dram___size[0][p_actvv_] += 1;
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












