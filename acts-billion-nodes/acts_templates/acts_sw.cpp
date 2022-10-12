#include "acts_sw.h"
using namespace std;

#define FOLD_SIZE 1
#define EDGE_PACK_SIZE 16 // 16, *1

#define SOURCE_PROPERTY_ARRANGEMENT_SERIAL

acts_sw::acts_sw(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
	algorithmobj = new algorithm();
}
acts_sw::~acts_sw(){} 

unsigned int get_H(unsigned int vid){
	return (vid % (FOLD_SIZE * EDGE_PACK_SIZE * NUM_PEs)) / (FOLD_SIZE * EDGE_PACK_SIZE);
	// return vid % NUM_PEs;
	// return utilityobj->get_H(vid);
}
unsigned int get_local(unsigned int vid){
	unsigned int W = (FOLD_SIZE * EDGE_PACK_SIZE) * NUM_PEs;
	unsigned int y = vid / W; 
	unsigned int x = vid % (FOLD_SIZE * EDGE_PACK_SIZE);
	unsigned int lvid = (y * (FOLD_SIZE * EDGE_PACK_SIZE)) + x;
	return lvid;
	// return (vid - get_H(vid)) / NUM_PEs;
	// return utilityobj->get_local(vid);
}
unsigned int get_global(unsigned int lvid, unsigned int H){
	unsigned int W = (FOLD_SIZE * EDGE_PACK_SIZE) * NUM_PEs;
	unsigned int y2 = lvid / (FOLD_SIZE * EDGE_PACK_SIZE);
	unsigned int x2 = lvid % (FOLD_SIZE * EDGE_PACK_SIZE);		
	unsigned int vid = (y2 * W) + (H * (FOLD_SIZE * EDGE_PACK_SIZE)) + x2;
	return vid;
	// return (lvid * NUM_PEs) + H;
	// return utilityobj->get_global(lvid, H);
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

void cache_vdatadram(unsigned int p_v, vprop_vec_t * vdatas_dram[MAX_NUM_APPLYPARTITIONS], vprop_t ** vdata_buffer){
	for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			vdata_buffer[v][t] = vdatas_dram[p_v][t].data[v];
		}
	}
	return;
}
void commit_vdatadram(unsigned int p_v, vprop_vec_t * vdatas_dram[MAX_NUM_APPLYPARTITIONS], vprop_t ** vdata_buffer){
	for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
	#pragma HLS PIPELINE II=1
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
		#pragma HLS UNROLL
			vdatas_dram[p_v][t].data[v] = vdata_buffer[v][t];
		}
	}
	return;
}

void insert_vdatadram(unsigned int p_v, unsigned int index, vprop_t data[EDGE_PACK_SIZE], vprop_vec_t * vdatas_dram[MAX_NUM_APPLYPARTITIONS]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		vdatas_dram[p_v][index].data[v] = data[v];
	}
	return;
}
void retrieve_vdatadram(unsigned int p_v, unsigned int index, vprop_t data[EDGE_PACK_SIZE], vprop_vec_t * vdatas_dram[MAX_NUM_APPLYPARTITIONS]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
	#pragma HLS UNROLL
		data[v] = vdatas_dram[p_v][index].data[v];
	}
	return;
}

void insert_udatabuffer(unsigned int v, unsigned int index, vprop_t data, vprop_t ** udata_buffer){
	udata_buffer[v][index] = data;
}
vprop_t retrieve_udatabuffer(unsigned int v, unsigned int index, vprop_t ** udata_buffer){
	return udata_buffer[v][index];
}

void insert_vdatabuffer(unsigned int v, unsigned int index, vprop_t data, vprop_t ** vdata_buffer){
	vdata_buffer[v][index] = data;
}
vprop_t retrieve_vdatabuffer(unsigned int v, unsigned int index, vprop_t ** vdata_buffer){
	return vdata_buffer[v][index];
}
void insert_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], vprop_t ** vdata_buffer){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		vdata_buffer[v][index] = data[v];
	}
}
vprop_t retrieve_vdatabuffer(unsigned int index, vprop_t data[EDGE_PACK_SIZE], vprop_t ** vdata_buffer){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = vdata_buffer[v][index];
	}
}

void insert_csrupdatesbuffer(unsigned int index, keyvalue_t data, vector<keyvalue_t> (&csrupdates_buffer)[NUM_PEs]){
	csrupdates_buffer[index].push_back(data);
}
keyvalue_t retrieve_csrupdatesbuffer(unsigned int index, unsigned int t, vector<keyvalue_t> (&csrupdates_buffer)[NUM_PEs]){
	return csrupdates_buffer[index][t];
}

void insert_csrupdatesbuffer(unsigned int index, unsigned int t, keyvalue_t data, keyvalue_t * csrupdates2_buffer[NUM_PEs]){
	csrupdates2_buffer[index][t] = data;
}
keyvalue_t retrieve_csrupdatesbuffer(unsigned int index, unsigned int t, keyvalue_t * csrupdates2_buffer[NUM_PEs]){
	return csrupdates2_buffer[index][t];
}

void insert_csrupdatesdram(unsigned int index, keyvalue_t data, vector<keyvalue_t> (&csrupdates_dram)[MAX_NUM_APPLYPARTITIONS]){
	csrupdates_dram[index].push_back(data);
}
keyvalue_t retrieve_csrupdatesdram(unsigned int index, unsigned int t, vector<keyvalue_t> (&csrupdates_dram)[MAX_NUM_APPLYPARTITIONS]){
	return csrupdates_dram[index][t];
}

void insert_cfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], keyvalue_t * cfrontier_dram[EDGE_PACK_SIZE][MAX_NUM_UPARTITIONS]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		cfrontier_dram[v][p_u][t] = data[v];
	}
}
void retrieve_cfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], keyvalue_t * cfrontier_dram[EDGE_PACK_SIZE][MAX_NUM_UPARTITIONS]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = cfrontier_dram[v][p_u][t];
	}
	return; 
}

void insert_cfrontierdram_i(unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], keyvalue_t * cfrontier_dram_i[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		cfrontier_dram_i[v][t] = data[v];
	}
}
void retrieve_cfrontierdram_i(unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], keyvalue_t * cfrontier_dram_i[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = cfrontier_dram_i[v][t];
	}
	return; 
}

void insert_cfrontierbuffer_i(unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], keyvalue_t * cfrontier_buffer_i[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		cfrontier_buffer_i[v][t] = data[v];
	}
}
void retrieve_cfrontierbuffer_i(unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], keyvalue_t * cfrontier_buffer_i[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = cfrontier_buffer_i[v][t];
	}
	return; 
}

void insert_nfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], keyvalue_t * nfrontier_dram[EDGE_PACK_SIZE][MAX_NUM_UPARTITIONS]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		nfrontier_dram[v][p_u][t] = data[v];
	}
}
void retrieve_nfrontierdram(unsigned int p_u, unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], keyvalue_t * nfrontier_dram[EDGE_PACK_SIZE][MAX_NUM_UPARTITIONS]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = nfrontier_dram[v][p_u][t];
	}
	return; 
}

void insert_nfrontierbuffer(unsigned int t, unsigned int v, keyvalue_t data, keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]){
	nfrontier_buffer[v][t] = data;
}
keyvalue_t retrieve_nfrontierbuffer(unsigned int t, unsigned int v, keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]){
	return nfrontier_buffer[v][t];
}
void insert_nfrontierbuffer(unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		nfrontier_buffer[v][t] = data[v];
	}
}
void retrieve_nfrontierbuffer(unsigned int t, keyvalue_t data[EDGE_PACK_SIZE], keyvalue_t * nfrontier_buffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = nfrontier_buffer[v][t];
	}
	return; 
}

void insertto_actvvsnextit(unsigned int index, keyvalue_t data, vector<keyvalue_t> (&actvvs_nextit)[MAX_NUM_UPARTITIONS]){
	actvvs_nextit[index].push_back(data);
}
keyvalue_t retrievefrom_actvvsnextit(unsigned int index, unsigned int t, vector<keyvalue_t> (&actvvs_nextit)[MAX_NUM_UPARTITIONS]){
	return actvvs_nextit[index][t];
}

void insert_actpackupdatesdram(unsigned int llp_set, unsigned int index, uint512_uvec_dt data, uint512_uvec_dt * actpackupdatesdram[MAX_NUM_APPLYPARTITIONS]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		actpackupdatesdram[llp_set][index] = data;					
	}
}
void retrieve_actpackupdatesdram(unsigned int llp_set, unsigned int index, uint512_uvec_dt * data, uint512_uvec_dt * actpackupdatesdram[MAX_NUM_APPLYPARTITIONS]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		*data = actpackupdatesdram[llp_set][index];					
	}
	return; 
}

void insert_statstmpbuffer(unsigned int t, unsigned int v, unsigned int data, unsigned int * stats_tmpbuffer[EDGE_PACK_SIZE]){
	stats_tmpbuffer[v][t] = data;
}
unsigned int retrieve_statstmpbuffer(unsigned int t, unsigned int v, unsigned int * stats_tmpbuffer[EDGE_PACK_SIZE]){
	return stats_tmpbuffer[v][t]; 
}
void insert_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], unsigned int * stats_tmpbuffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		stats_tmpbuffer[v][t] = data[v];
	}
}
void retrieve_statstmpbuffer(unsigned int t, unsigned int data[EDGE_PACK_SIZE], unsigned int * stats_tmpbuffer[EDGE_PACK_SIZE]){
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ 
	#pragma HLS UNROLL
		data[v] = stats_tmpbuffer[v][t];
	}
	return; 
}

void insert_statsbuffer(unsigned int t, unsigned int v, unsigned int data, unsigned int * stats_buffer[MAX_NUM_APPLYPARTITIONS]){
	stats_buffer[v][t] = data;
}
unsigned int retrieve_statsbuffer(unsigned int t, unsigned int v, unsigned int * stats_buffer[MAX_NUM_APPLYPARTITIONS]){
	return stats_buffer[v][t]; 
}

void acts_sw::run(vector<unsigned int> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, unsigned int * v_ptr[NUM_PEs], vector<edge2_type> edges_in_channel[NUM_PEs], vector<edge3_vec_dt> (&act_pack_edges)[NUM_PEs], map_t * act_pack_map[NUM_PEs][MAX_NUM_UPARTITIONS]){						
	
	unsigned int __NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	unsigned int __NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs
	cout<<"acts_sw::run:: universalparams.NUM_VERTICES: "<<universalparams.NUM_VERTICES<<", universalparams.NUM_EDGES: "<<universalparams.NUM_EDGES<<", MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<", __NUM_UPARTITIONS: "<<__NUM_UPARTITIONS<<", MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<", __NUM_APPLYPARTITIONS: "<<__NUM_APPLYPARTITIONS<<endl;			
	
	vprop_vec_t * vdatas_dram[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<MAX_NUM_APPLYPARTITIONS; p++){ vdatas_dram[i][p] = new vprop_vec_t[MAX_APPLYPARTITION_VECSIZE]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ vdatas_dram[i][p][t].data[v].data = algorithmobj->vertex_initdata(universalparams.ALGORITHM); vdatas_dram[i][p][t].data[v].mask = 0; }}}}		
	
	keyvalue_t * cfrontier_dram[EDGE_PACK_SIZE][MAX_NUM_UPARTITIONS]; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ cfrontier_dram[v][p] = new keyvalue_t[MAX_UPARTITION_SIZE]; }}
	unsigned int * cfrontier_dram___size[EDGE_PACK_SIZE]; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cfrontier_dram___size[v] = new unsigned int[MAX_NUM_UPARTITIONS]; }
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ cfrontier_dram___size[v][p] = 0; }}
	
	keyvalue_t * cfrontier_dram_i[NUM_PEs][EDGE_PACK_SIZE]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cfrontier_dram_i[i][v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	
	keyvalue_t * nfrontier_dram[NUM_PEs][EDGE_PACK_SIZE][MAX_NUM_UPARTITIONS]; for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ nfrontier_dram[i][v][p] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}}
	unsigned int * nfrontier_dram___size[NUM_PEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_dram___size[i][v] = new unsigned int[MAX_NUM_UPARTITIONS]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ nfrontier_dram___size[i][v][p] = 0; }}}
	
	keyvalue_t * nfrontier_buffer[NUM_PEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer[i][v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	unsigned int * nfrontier_buffer___size[NUM_PEs][EDGE_PACK_SIZE]; for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ nfrontier_buffer___size[i][v] = new unsigned int[NUM_ACTVVPARTITIONS_PER_APPLYPARTITION]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p=0; p<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p++){ nfrontier_buffer___size[i][v][p] = 0; }}}
	
	keyvalue_t * cfrontier_buffer_i[NUM_PEs][EDGE_PACK_SIZE]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cfrontier_buffer_i[i][v] = new keyvalue_t[MAX_APPLYPARTITION_VECSIZE]; }}
	
	unsigned int * stats_tmpbuffer[NUM_PEs][EDGE_PACK_SIZE]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ stats_tmpbuffer[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	
	unsigned int * stats_buffer[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<MAX_NUM_APPLYPARTITIONS; v++){ stats_buffer[i][v] = new unsigned int[BLOCKRAM_SIZE]; }}
	unsigned int stats_buffer___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	
	offset_t * upartition_vertices = new offset_t[MAX_NUM_UPARTITIONS]; 
	for(unsigned int p=0; p<__NUM_UPARTITIONS; p++){ upartition_vertices[p].offset = 0; upartition_vertices[p].size = 0; upartition_vertices[p].count = 0; } upartition_vertices[0].count = 1;
	
	offset_t * vpartition_vertices[NUM_PEs]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ vpartition_vertices[i] = new offset_t[MAX_NUM_APPLYPARTITIONS];  }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ vpartition_vertices[i][p].offset = 0; vpartition_vertices[i][p].size = 0; vpartition_vertices[i][p].count = 0; }}
	
	vector<keyvalue_t> csrupdates_dram[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; // NEW
	vector<keyvalue_t> csrupdates_buffer[NUM_PEs][NUM_PEs]; 
	
	keyvalue_t * csrupdates2_dram[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ csrupdates2_dram[i][p] = new keyvalue_t[CSRDRAM_SIZE]; }}
	unsigned int csrupdates2_dram___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	
	keyvalue_t * csrupdates2_buffer[NUM_PEs][NUM_PEs]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<NUM_PEs; p++){ csrupdates2_buffer[i][p] = new keyvalue_t[CSRBUFFER_SIZE]; }} // CSRBUFFER_SIZE
	unsigned int csrupdates2_buffer___size[NUM_PEs][NUM_PEs]; 
	
	uint512_uvec_dt * actpackupdates_dram[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ actpackupdates_dram[i][p] = new uint512_uvec_dt[HBM_CHANNEL_SIZE]; }}
	unsigned int actpackupdates_dram___size[NUM_PEs][MAX_NUM_APPLYPARTITIONS]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ actpackupdates_dram___size[i][p] = 0; }}
	
	vprop_t * udata_buffer[NUM_PEs][EDGE_PACK_SIZE]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ udata_buffer[i][v] = new vprop_t[MAX_UPARTITION_VECSIZE]; }}
	
	vprop_t * vdata_buffer[NUM_PEs][EDGE_PACK_SIZE]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int j=0; j<EDGE_PACK_SIZE; j++){ vdata_buffer[i][j] = new vprop_t[MAX_APPLYPARTITION_VECSIZE]; }}
	
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
	bool enable___collectactivedstvids = false;
	unsigned int threshold___activefrontiers = 128;
	unsigned int threshold___activedstvids = 0xFFFFFFFF;
	
	unsigned int rootvid = 1; keyvalue_t root; root.key = rootvid; root.value = 0; keyvalue_t invalid; invalid.key = INVALIDDATA; invalid.value = INVALIDDATA; 
	for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(v==1){ cfrontier_dram[v][0][0] = root; } else { cfrontier_dram[v][0][0] = invalid; } cfrontier_dram___size[v][0] = 1; }
	
	if(run_vertex_centric == true){ cout<<"### Vertex-Centric GAS iteration: 0 [1 active vertices]"<<endl; } else { cout<<"### Edge-Centric GAS iteration: 0 [1 active vertices]"<<endl; }
	
	for(unsigned int GraphIter=0; GraphIter<12; GraphIter++){ // MAXNUMGRAPHITERATIONS
		unsigned int totalactvvs1 = 0; for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ totalactvvs1 += cfrontier_dram___size[v][p_u]; }}
		for(unsigned int p_=0; p_<__NUM_APPLYPARTITIONS; p_++){ for(unsigned int inst=0; inst<NUM_PEs; inst++){ csrupdates_dram[inst][p_].clear(); }}
		for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int j=0; j<MAX_NUM_APPLYPARTITIONS; j++){ csrupdates2_dram___size[i][j] = 0; }}
		for(unsigned int p_=0; p_<__NUM_APPLYPARTITIONS; p_++){ for(unsigned int inst=0; inst<NUM_PEs; inst++){ actpackupdates_dram___size[inst][p_] = 0; }}
		
		// clear stats_buffer
		for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ for(unsigned int p_v=0; p_v<MAX_NUM_APPLYPARTITIONS; p_v++){ stats_buffer[inst][p_v][t] = 0; }}}
		for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int p_v=0; p_v<MAX_NUM_APPLYPARTITIONS; p_v++){ stats_buffer___size[inst][p_v] = 0; }}
		
		// clear vpartition_vertices // FIXME.
		for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ vpartition_vertices[i][p].offset = 0; vpartition_vertices[i][p].size = 0; vpartition_vertices[i][p].count = 0; }}
		
		// process-edges and partition-updates
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){
			keyvalue_t data[EDGE_PACK_SIZE];
	
			// broadcast active frontiers
			for(unsigned int t=0; t<cfrontier_dram___size[0][p_u]; t++){
			#pragma HLS PIPELINE II=1
				// keyvalue_t data[EDGE_PACK_SIZE]; // NEW*
				retrieve_cfrontierdram(p_u, t, data, cfrontier_dram);
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
				#pragma HLS UNROLL
					insert_cfrontierdram_i(t, data, cfrontier_dram_i[inst]);
				}
			}
			
			// parallel-read active frontiers
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				for(unsigned int t=0; t<cfrontier_dram___size[0][p_u]; t++){
				#pragma HLS PIPELINE II=1
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
					#pragma HLS UNROLL
						// keyvalue_t data[EDGE_PACK_SIZE]; // NEW*
						retrieve_cfrontierdram_i(t, data, cfrontier_dram_i[inst]);
						insert_cfrontierbuffer_i(t, data, cfrontier_buffer_i[inst]);
					}
				}
			}
			
			// clear ubuffer // FIXME.
			for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int t=0; t<MAX_UPARTITION_VECSIZE; t++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ udata_buffer[inst][v][t].data = 0; udata_buffer[inst][v][t].mask = 0; }}}
			
			// clear stats buffer // FIXME.
			for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ stats_tmpbuffer[inst][v][t] = 0; }}}
			
			// load active frontiers  properties 
			for(unsigned int inst=0; inst<NUM_PEs; inst++){
				for(unsigned int t=0; t<cfrontier_dram___size[0][p_u]; t++){
				#pragma HLS PIPELINE II=1
					keyvalue_t actvv[EDGE_PACK_SIZE];
					retrieve_cfrontierbuffer_i(t, actvv, cfrontier_buffer_i[inst]);
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					#pragma HLS UNROLL
						if(actvv[v].key != INVALIDDATA){
							unsigned int srcvid_lp  = get_local_to_upartition(actvv[v].key); 
							unsigned int srcvid_lpv = srcvid_lp / EDGE_PACK_SIZE;
							vprop_t prop; prop.data = actvv[v].value; prop.mask = 1;
							insert_udatabuffer(v, srcvid_lpv, prop, udata_buffer[inst]);
						}
					}
				}
			}
			
			for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int j=0; j<NUM_PEs; j++){ csrupdates2_buffer___size[i][j] = 0; }} // NEW*
			
			// process-edges and partition-updates
			if(upartition_vertices[p_u].count > 0){
				if(upartition_vertices[p_u].count < threshold___activefrontiers && enable___vertexcentric == true){ // vertex-centric
					hybrid_map[GraphIter][p_u] = 0;
					for(unsigned int inst=0; inst<NUM_PEs; inst++){
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
							for(unsigned int t=0; t<cfrontier_dram___size[0][p_u]; t++){ // NEW* cfrontier_dram___size[v][p_u]
								keyvalue_t actvv = cfrontier_buffer_i[inst][v][t];
								unsigned int vid = actvv.key;
								if(actvv.key != INVALIDDATA && vid % NUM_PEs == inst){ 
									unsigned int hvid = vid / NUM_PEs;
									unsigned int vptr_begin = v_ptr[inst][hvid];
									unsigned int vptr_end = v_ptr[inst][hvid+1];
									unsigned int edges_size = vptr_end - vptr_begin;
									if(vptr_end < vptr_begin){ continue; } 
									
									unsigned int srcvid_lp = get_local_to_upartition(vid);
									vprop_t uprop = retrieve_udatabuffer(srcvid_lp % EDGE_PACK_SIZE, srcvid_lp / EDGE_PACK_SIZE, udata_buffer[inst]);
									
									VC_PROCESS_EDGES_LOOP1: for(unsigned int k=0; k<edges_size; k++){
										edge2_type edge = edges_in_channel[inst][vptr_begin + k];
										#ifdef _DEBUGMODE_KERNELPRINTS//4
										cout<<"acts_sw: inst: "<<inst<<", vid: "<<vid<<", dstvid: "<<edge.dstvid<<endl;
										#endif 
										
										// scatter 
										unsigned int res = uprop.data + 1;
										unsigned int vtemp = min(uprop.data, res);
										#ifdef _DEBUGMODE_KERNELPRINTS//4
										cout<<"VC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [srcvid_lp: "<<srcvid_lp<<", edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", uprop: "<<uprop.data<<", res: "<<res<<", vtemp: "<<vtemp<<"]"<<endl;
										cout<<"VC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [srcvid_lp: "<<srcvid_lp<<", local partition: "<<p_u<<", vid: "<<vid<<"]"<<endl;
										#endif 
										
										// partition 
										unsigned int inst_mirror = get_H(edge.dstvid);
										keyvalue_t update; update.key = edge.dstvid; update.value = vtemp; // res;
										
										utilityobj->checkoutofbounds("acts_sw::ERROR 113::", csrupdates2_buffer___size[inst][inst_mirror], CSRBUFFER_SIZE, NAp, NAp, NAp);
										insert_csrupdatesbuffer(inst_mirror, csrupdates2_buffer___size[inst][inst_mirror], update, csrupdates2_buffer[inst]); // NEW*
										csrupdates2_buffer___size[inst][inst_mirror] += 1;
										
										// collect stats information 
										if(enable___collectactivedstvids == true){ 
											unsigned int dstvid_lp = get_local_to_vpartition(get_local(edge.dstvid));
											unsigned int dstvid_lpv = get_local_to_vpartition(get_local(edge.dstvid)) / EDGE_PACK_SIZE;
											unsigned int index = dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE;
											utilityobj->checkoutofbounds("acts_sw::ERROR 123::", index, BLOCKRAM_SIZE, update_out[inst][v].key, dstvid_lpv, (MAX_APPLYPARTITION_VECSIZE / BLOCKRAM_SIZE));
											insert_statstmpbuffer(index, 0, 1, stats_tmpbuffer[inst]);
										}
									}
								}
							}
						}
					}
					
					// exchange updates between HBMs
					for(unsigned int inst_mirror=0; inst_mirror<NUM_PEs; inst_mirror++){
						for(unsigned int inst=0; inst<NUM_PEs; inst++){
							for(unsigned int t=0; t<csrupdates2_buffer___size[inst][inst_mirror]; t++){
								keyvalue_t update = retrieve_csrupdatesbuffer(inst_mirror, t, csrupdates2_buffer[inst]); // NEW*
								unsigned int p_v = get_local(update.key) / MAX_APPLYPARTITION_SIZE;
								utilityobj->checkoutofbounds("acts_sw::ERROR 121::", csrupdates2_dram___size[inst_mirror][p_v], CSRDRAM_SIZE, NAp, NAp, NAp);
								csrupdates2_dram[inst_mirror][p_v][csrupdates2_dram___size[inst_mirror][p_v]] = update;
								csrupdates2_dram___size[inst_mirror][p_v] += 1; // if(csrupdates2_dram___size[inst_mirror][p_v] + 2 < CSRDRAM_SIZE){ csrupdates2_dram___size[inst_mirror][p_v] += 1; }
								vpartition_vertices[0][p_v].count = 1;
							}
							
						}
					}
				} else { // edge-centric
					hybrid_map[GraphIter][p_u] = 1;
					for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
						for(unsigned int inst=0; inst<NUM_PEs; inst++){
							for(unsigned int t=act_pack_map[inst][p_u][llp_set].offset; t<act_pack_map[inst][p_u][llp_set].offset + act_pack_map[inst][p_u][llp_set].size; t++){
							#pragma HLS PIPELINE II=1
								edge3_vec_dt edge_vec = act_pack_edges[inst][t];
								unsigned int rotate_forward = edge_vec.data[EDGE_PACK_SIZE].srcvid; unsigned int rotateby = edge_vec.data[EDGE_PACK_SIZE + 1].srcvid; unsigned int sample_key = NAp; unsigned int sample_u = NAp;
								
								EC_PROCESS_EDGES_LOOP1: for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
								#pragma HLS UNROLL
									if(edge_vec.data[v].srcvid != INVALIDDATA){
										edges[inst][v] = edge_vec.data[v];
										unsigned int srcvid_lp = get_local_to_upartition(edges[inst][v].srcvid);
										unsigned int srcvid_lpv = get_local_to_upartition(edges[inst][v].srcvid) / EDGE_PACK_SIZE;
										utilityobj->checkoutofbounds("acts_sw::ERROR 223::", srcvid_lp, MAX_UPARTITION_SIZE, NAp, NAp, NAp);
										if(srcvid_lp % EDGE_PACK_SIZE != v){ cout<<"acts_sw:: srcvid_lp("<<srcvid_lp<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
										
										uprop[inst][v] = retrieve_udatabuffer(v, srcvid_lpv, udata_buffer[inst]);
										
										res[inst][v] = uprop[inst][v].data + 1;
										vtemp_in[inst][v] = min(uprop[inst][v].data, res[inst][v]);
										update_in[inst][v].key = edges[inst][v].dstvid; update_in[inst][v].value = vtemp_in[inst][v];
										if(uprop[inst][v].mask != 1){ update_in[inst][v].key = INVALIDDATA; update_in[inst][v].value = INVALIDDATA; }
										#ifdef _DEBUGMODE_KERNELPRINTS//4
										if(uprop[inst][v].mask == 1){ cout<<"EC - PROCESS EDGE SEEN @: inst: ["<<inst<<"], [srcvid_lp: "<<srcvid_lp<<", dstvid: "<<edges[inst][v].dstvid % MAX_APPLYPARTITION_SIZE<<"], [edge.srcvid: "<<edges[inst][v].srcvid<<", dstvid: "<<edges[inst][v].dstvid<<"]"<<endl; }
										#endif 
									} else {
										uprop[inst][v].mask = 0; update_in[inst][v].key = INVALIDDATA; update_in[inst][v].value = INVALIDDATA;
									}
								}
								
								// circular shift >>>
								utilityobj->checkoutofbounds("acts_sw::ERROR 213::", rotateby, EDGE_PACK_SIZE, NAp, NAp, NAp);
								if(rotate_forward == 0){ rearrangeLayoutVx16B(rotateby, update_in[inst], update_out[inst]); } else{ rearrangeLayoutVx16F(rotateby, update_in[inst], update_out[inst]); }
								#ifdef _DEBUGMODE_CHECKS3
								utilityobj->checkoutofbounds("acts_sw::ERROR 59::", rotateby, EDGE_PACK_SIZE, sample_key, NAp, NAp);
								utilityobj->checkoutofbounds("acts_sw::ERROR 60::", rotate_forward, 2, sample_key, NAp, NAp);	
								check_if_contiguous(update_out[inst], update_in[inst], update_in[inst], sample_key, rotate_forward, rotateby);
								#endif 
								
								// partition updates
								uint512_uvec_dt updates_vec; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ updates_vec.data[v] = update_out[inst][v]; }
								bool isvalid = false; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(update_out[inst][v].key != INVALIDDATA){ isvalid = true; }} // FIXME.
								if(isvalid == true){ insert_actpackupdatesdram(llp_set, actpackupdates_dram___size[inst][llp_set], updates_vec, actpackupdates_dram[inst]); actpackupdates_dram___size[inst][llp_set] += 1; } // NEW
							
								// collect stats information 
								if(enable___collectactivedstvids == true && isvalid == true){ 
									for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
										if(update_out[inst][v].key != INVALIDDATA){
											unsigned int dstvid_lp = get_local_to_vpartition(get_local(update_out[inst][v].key));
											unsigned int dstvid_lpv = get_local_to_vpartition(get_local(update_out[inst][v].key)) / EDGE_PACK_SIZE;
											unsigned int index = dstvid_lpv / ACTVUPDATESBLOCK_VECSIZE;
											utilityobj->checkoutofbounds("acts_sw::ERROR 123::", index, BLOCKRAM_SIZE, update_out[inst][v].key, dstvid_lpv, (MAX_APPLYPARTITION_VECSIZE / BLOCKRAM_SIZE));
											insert_statstmpbuffer(index, v, 1, stats_tmpbuffer[inst]);
										}
									}
								}
							}
							
							// gather stats 
							if(enable___collectactivedstvids == true){
								unsigned int data[EDGE_PACK_SIZE];
								for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
								#pragma HLS PIPELINE II=1
									retrieve_statstmpbuffer(t, data, stats_tmpbuffer[inst]);
									utilityobj->checkoutofbounds("acts_sw::ERROR 023::", t, BLOCKRAM_SIZE, NAp, NAp, NAp);
									if(data[0]==1 || data[1]==1 || data[2]==1 || data[3]==1 || data[4]==1 || data[5]==1 || data[6]==1 || data[7]==1 || data[8]==1 || data[9]==1 || data[10]==1 || data[11]==1 || data[12]==1 || data[13]==1 || data[14]==1 || data[15]==1){ 
										insert_statsbuffer(t, llp_set, 1, stats_buffer[inst]);
										vpartition_vertices[inst][llp_set].count = 1;
									}
								}
							}
						}
					}
				}
			}
		}
		
		// apply updatess
		for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			// if(vpartition_vertices[0][p_v].count > 0){
			if(true){
				// read destination properties 
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
					vprop_t data[EDGE_PACK_SIZE];
					if(stats_buffer___size[inst][p_v] <= threshold___activedstvids && enable___collectactivedstvids == true){
						for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
							unsigned int offset = t * ACTVUPDATESBLOCK_VECSIZE;
							utilityobj->checkoutofbounds("acts_sw::ERROR 117::", offset, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
							if(retrieve_statsbuffer(t, p_v, stats_buffer[inst]) == 1){
								for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
								#pragma HLS PIPELINE II=1
									retrieve_vdatadram(p_v, offset + t, data, vdatas_dram[inst]); // NEW*
									insert_vdatabuffer(offset + t, data, vdata_buffer[inst]); // NEW*
								}
							}
						}
					} else if (false){ // NOT YET IMPLEMENTED.
						for(unsigned int k=0; k<stats_buffer___size[inst][p_v]; k++){
							unsigned int offset = retrieve_statsbuffer(k, p_v, stats_buffer[inst]) * ACTVUPDATESBLOCK_VECSIZE;
							utilityobj->checkoutofbounds("acts_sw::ERROR 117::", offset, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
							for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								retrieve_vdatadram(p_v, offset + t, data, vdatas_dram[inst]); // NEW*
								insert_vdatabuffer(offset + t, data, vdata_buffer[inst]); // NEW*
							}
						}	
					} else {
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
							for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								retrieve_vdatadram(p_v, t, data, vdatas_dram[inst]);
								insert_vdatabuffer(t, data, vdata_buffer[inst]);
							}
						}
					}
				}
				
				// apply 2 (csr)
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
					for(unsigned int t=0; t<csrupdates2_dram___size[inst][p_v]; t++){
					#pragma HLS PIPELINE II=1
						keyvalue_t update = csrupdates2_dram[inst][p_v][t];
						if(update.key != INVALIDDATA){
							unsigned int dstvid_lp = get_local(update.key) % MAX_APPLYPARTITION_SIZE; // hashing
							utilityobj->checkoutofbounds("acts_sw::ERROR 227::", dstvid_lp, MAX_APPLYPARTITION_SIZE, NAp, NAp, NAp);
							
							vprop_t vprop =  retrieve_vdatabuffer(dstvid_lp % EDGE_PACK_SIZE, dstvid_lp / EDGE_PACK_SIZE, vdata_buffer[inst]);
							unsigned int new_vprop = 0; if(update.value < vprop.data){ new_vprop = update.value; } else { new_vprop = vprop.data; }
							if(new_vprop != vprop.data){
								#ifdef _DEBUGMODE_KERNELPRINTS//4
								cout<<"APPLY (CSR) UPDATE SEEN @: inst: ["<<inst<<"]: local dstvid: "<<dstvid_lp<<", new_vprop: "<<new_vprop<<", vid: "<<update.key<<endl;
								#endif
								vprop_t newprop; newprop.data = new_vprop; newprop.mask = 1; 
								insert_vdatabuffer(dstvid_lp % EDGE_PACK_SIZE, dstvid_lp / EDGE_PACK_SIZE, newprop, vdata_buffer[inst]);
							}
						}
					}
				}
				
				// apply 1 (act-pack)
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
					for(unsigned int t=0; t<actpackupdates_dram___size[inst][p_v]; t++){
					#pragma HLS PIPELINE II=1
						uint512_uvec_dt updates_vec;
						retrieve_actpackupdatesdram(p_v, t, &updates_vec, actpackupdates_dram[inst]); // NEW
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							keyvalue_t update = updates_vec.data[v];
							unsigned int dstvid_lp = get_local_to_vpartition(get_local(update.key));
							unsigned int dstvid_lpv = get_local_to_vpartition(get_local(update.key)) / EDGE_PACK_SIZE;
							if(update.key != INVALIDDATA){
								if((dstvid_lp % EDGE_PACK_SIZE) != v){ cout<<"acts_sw 234:: dstvid_lp("<<dstvid_lp<<") % v("<<v<<") != 0. EXITING..."<<endl; exit(EXIT_FAILURE); }
								utilityobj->checkoutofbounds("acts_sw::ERROR 227::", dstvid_lp, MAX_APPLYPARTITION_SIZE, NAp, NAp, NAp);
								vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, vdata_buffer[inst]);
								unsigned int new_vprop = 0; if(update.value < vprop.data){ new_vprop = update.value; } else { new_vprop = vprop.data; }
								if(new_vprop != vprop.data){
									#ifdef _DEBUGMODE_KERNELPRINTS//4
									cout<<"APPLY (ACT-PACK) UPDATE SEEN @: inst: ["<<inst<<"]: dstvid_lp: "<<dstvid_lp<<", dstvid_lpv: "<<dstvid_lpv<<", new_vprop: "<<new_vprop<<", vid: "<<update.key<<endl;
									#endif
									vprop_t newprop; newprop.data = new_vprop; newprop.mask = 1; 
									insert_vdatabuffer(v, dstvid_lpv, newprop, vdata_buffer[inst]);
								}
							}
						}
					}
				}
				
				// collect frontier information (vid-based) 
				for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){ nfrontier_buffer___size[inst][v][p_actvv] = 0; }}}
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
					for(unsigned int dstvid_lpv=0; dstvid_lpv<MAX_APPLYPARTITION_VECSIZE; dstvid_lpv++){
					#pragma HLS PIPELINE II=1
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
						#pragma HLS UNROLL
							vprop_t vprop =  retrieve_vdatabuffer(v, dstvid_lpv, vdata_buffer[inst]);
							unsigned int vid = get_global(((p_v * MAX_APPLYPARTITION_SIZE) +  (dstvid_lpv * EDGE_PACK_SIZE + v)), inst); // unhashing
							#ifdef _DEBUGMODE_CHECKS3
							if(vid % EDGE_PACK_SIZE != v){ cout<<"acts_sw:: ERROR 234. vid("<<vid<<") % EDGE_PACK_SIZE("<<EDGE_PACK_SIZE<<")(="<<vid % EDGE_PACK_SIZE<<") != v("<<v<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
							#endif
							unsigned int pl__actv__ = dstvid_lpv / MAX_ACTVV_VECSIZE;
							if(vprop.mask == 1){
								#ifdef _DEBUGMODE_KERNELPRINTS//4
								cout<<"COLLECT FRONTIER INFORMATION SEEN @: inst: ["<<inst<<"]: dstvid_lpv: "<<dstvid_lpv<<", v: "<<v<<", p__u__: "<<(vid / MAX_UPARTITION_SIZE)<<", vid: "<<vid<<endl;
								#endif
								keyvalue_t actvv; actvv.key = vid;  actvv.value = vprop.data; 
								insert_nfrontierbuffer((pl__actv__ * MAX_ACTVV_VECSIZE) + nfrontier_buffer___size[inst][v][pl__actv__], v, actvv, nfrontier_buffer[inst]); // NEW*
								nfrontier_buffer___size[inst][v][pl__actv__] += 1;
							}
						}
					}
				}
				
				// commit frontier information
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
					for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
						unsigned int p_actvv_ = (p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION) + p_actvv;
						unsigned int max = 0; for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(max < nfrontier_buffer___size[inst][v][p_actvv]){ max = nfrontier_buffer___size[inst][v][p_actvv]; }}
						keyvalue_t actvv[EDGE_PACK_SIZE];
						for(unsigned int t=0; t<max; t++){
						#pragma HLS PIPELINE II=1
							retrieve_nfrontierbuffer(p_actvv * MAX_ACTVV_VECSIZE + t, actvv, nfrontier_buffer[inst]);
							for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ // NEW*
							#pragma HLS UNROLL
								if(t >= nfrontier_buffer___size[inst][v][p_actvv]){ actvv[v].key = INVALIDDATA; actvv[v].value = INVALIDDATA; }
							}
							insert_nfrontierdram(p_actvv_, nfrontier_dram___size[inst][0][p_actvv_], actvv, nfrontier_dram[inst]);
							nfrontier_dram___size[inst][0][p_actvv_] += 1;
						}
					}
				}
				
				// clear vbuffer. FIXME.
				for(unsigned int inst=0; inst<NUM_PEs; inst++){ for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ vdata_buffer[inst][v][t].mask = 0; }}}
				
				// commit destination properties 
				for(unsigned int inst=0; inst<NUM_PEs; inst++){
					vprop_t data[EDGE_PACK_SIZE];
					if(stats_buffer___size[inst][p_v] <= threshold___activedstvids && enable___collectactivedstvids == true){
						for(unsigned int t=0; t<BLOCKRAM_SIZE; t++){
							unsigned int offset = t * ACTVUPDATESBLOCK_VECSIZE;
							utilityobj->checkoutofbounds("acts_sw::ERROR 117::", offset, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
							if(retrieve_statsbuffer(t, p_v, stats_buffer[inst]) == 1){
								for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
								#pragma HLS PIPELINE II=1
									retrieve_vdatabuffer(offset + t, data, vdata_buffer[inst]); // NEW*
									insert_vdatadram(p_v, offset + t, data, vdatas_dram[inst]); // NEW*
								}
							}
						}
					} else if(false){
						for(unsigned int k=0; k<stats_buffer___size[inst][p_v]; k++){	
							unsigned int offset = retrieve_statsbuffer(k, p_v, stats_buffer[inst]) * ACTVUPDATESBLOCK_VECSIZE;
							utilityobj->checkoutofbounds("acts_sw::ERROR 217::", offset, MAX_APPLYPARTITION_VECSIZE, NAp, NAp, NAp);
							for(unsigned int t=0; t<ACTVUPDATESBLOCK_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								retrieve_vdatabuffer(offset + t, data, vdata_buffer[inst]); // NEW*
								insert_vdatadram(p_v, offset + t, data, vdatas_dram[inst]); // NEW*
							}
						}							
					} else {
						for(unsigned int inst=0; inst<NUM_PEs; inst++){ 
							for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){
							#pragma HLS PIPELINE II=1
								retrieve_vdatabuffer(t, data, vdata_buffer[inst]);
								insert_vdatadram(p_v, t, data, vdatas_dram[inst]);
							}
						}
					}
				}
				
			}
		}
		
		// gather frontier information
		unsigned int totalactvvs2 = 0; keyvalue_t actvv[EDGE_PACK_SIZE]; 
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ upartition_vertices[p_u].count = 0; }
		for(unsigned int p_u=0; p_u<__NUM_UPARTITIONS; p_u++){ for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ cfrontier_dram___size[v][p_u] = 0; }}
		for(unsigned int p_v=0; p_v<__NUM_APPLYPARTITIONS; p_v++){
			for(unsigned int inst=0; inst<NUM_PEs; inst++){
				for(unsigned int p_actvv=0; p_actvv<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION; p_actvv++){
					unsigned int p_actvv_ = p_v * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION + p_actvv;
					utilityobj->checkoutofbounds("acts_sw::ERROR 225::", p_actvv_, MAX_NUM_UPARTITIONS, NAp, NAp, NAp);
					for(unsigned int t=0; t<nfrontier_dram___size[inst][0][p_actvv_]; t++){
					#pragma HLS PIPELINE II=1
						upartition_vertices[p_actvv_].count += 1; 
						retrieve_nfrontierdram(p_actvv_, t, actvv, nfrontier_dram[inst]);
						#ifdef _DEBUGMODE_KERNELPRINTS4
						for(unsigned int v=0; v<0; v++){ cout<<"TRANSFER VID-BASED FRONTIERS SEEN @: H: "<<inst<<" => vid: "<<actvv[v].key<<", p_actvv_: "<<p_actvv_<<endl; }
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA && actvv[v].key / MAX_UPARTITION_SIZE != p_actvv_){ cout<<"ERROR 234: actvv["<<v<<"].key("<<actvv[v].key<<") / MAX_UPARTITION_SIZE("<<MAX_UPARTITION_SIZE<<")(="<<actvv[v].key / MAX_UPARTITION_SIZE<<") != p_actvv_("<<p_actvv_<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }}				
						for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){ if(actvv[v].key != INVALIDDATA){ totalactvvs2 += 1; }}
						#endif 
						insert_cfrontierdram(p_actvv_, cfrontier_dram___size[0][p_actvv_], actvv, cfrontier_dram); cfrontier_dram___size[0][p_actvv_] += 1;
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












