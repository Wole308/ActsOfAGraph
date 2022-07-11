#include "loadedges.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

#define LOADSLICEDEDGES_CORRECT	
#define CALCULATELOCALDSTVIDS
#define INSERTBITMAP
#define LOADEDGES
#define CALCULATEOFFSETS

#define LOADEDGES_ENSUREPERFECTACCURACY

#define TWOO 2

// #define MYVDATA_SHRINK_FACTOR VDATA_SHRINK_RATIO
#define MYVDATA_SHRINK_FACTOR 1

bool debugb = false;//false;
bool debug2b = false; //true;//false;
bool debug3b = false;

loadedges::loadedges(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	evalparamsobj = new evalparams();
	universalparams = _universalparams;
}
loadedges::~loadedges(){} 

bool myfunction(edge2_type i, edge2_type j) { return (i.srcvid < j.srcvid); }

unsigned int loadedges::gethash(unsigned int vid){
	return vid % NUM_PEs;
}
unsigned int loadedges::getlocalvid(unsigned int vid){
	unsigned int s = gethash(vid);
	return (vid - s) / NUM_PEs; 
}

void loadedges::getXYLayoutVx8(unsigned int s, unsigned int depths[EDGEDATA_PACKINGSIZE]){
	unsigned int s_ = s % EDGEDATA_PACKINGSIZE;
	if(s_==0){ 
		depths[0] = 0; 
		depths[1] = 1; 
		depths[2] = 2; 
		depths[3] = 3; 
		depths[4] = 4; 
		depths[5] = 5; 
		depths[6] = 6; 
		depths[7] = 7; 
	} else if(s_==1){ 
		depths[0] = 7; 
		depths[1] = 0; 
		depths[2] = 1; 
		depths[3] = 2; 
		depths[4] = 3; 
		depths[5] = 4; 
		depths[6] = 5; 
		depths[7] = 6; 
	} else if(s_==2){ 
		depths[0] = 6; 
		depths[1] = 7; 
		depths[2] = 0; 
		depths[3] = 1; 
		depths[4] = 2; 
		depths[5] = 3; 
		depths[6] = 4; 
		depths[7] = 5; 
	} else if(s_==3){ 
		depths[0] = 5; 
		depths[1] = 6; 
		depths[2] = 7; 
		depths[3] = 0; 
		depths[4] = 1; 
		depths[5] = 2; 
		depths[6] = 3; 
		depths[7] = 4; 
	} else if(s_==4){ 
		depths[0] = 4; 
		depths[1] = 5; 
		depths[2] = 6; 
		depths[3] = 7; 
		depths[4] = 0; 
		depths[5] = 1; 
		depths[6] = 2; 
		depths[7] = 3; 
	} else if(s_==5){ 
		depths[0] = 3; 
		depths[1] = 4; 
		depths[2] = 5; 
		depths[3] = 6; 
		depths[4] = 7; 
		depths[5] = 0; 
		depths[6] = 1; 
		depths[7] = 2; 
	} else if(s_==6){ 
		depths[0] = 2; 
		depths[1] = 3; 
		depths[2] = 4; 
		depths[3] = 5; 
		depths[4] = 6; 
		depths[5] = 7; 
		depths[6] = 0; 
		depths[7] = 1; 
	} else { 
		depths[0] = 1; 
		depths[1] = 2; 
		depths[2] = 3; 
		depths[3] = 4; 
		depths[4] = 5; 
		depths[5] = 6; 
		depths[6] = 7; 
		depths[7] = 0; 
	}
	return;
}
edge2_vec_dt loadedges::rearrangeLayoutVx8(unsigned int s, edge2_vec_dt edge_vec){
	edge2_vec_dt edge_vec2;
	unsigned int s_ = s;
	if(s_==0){ 
		edge_vec2.data[0] = edge_vec.data[0]; 
		edge_vec2.data[1] = edge_vec.data[1]; 
		edge_vec2.data[2] = edge_vec.data[2]; 
		edge_vec2.data[3] = edge_vec.data[3]; 
		edge_vec2.data[4] = edge_vec.data[4]; 
		edge_vec2.data[5] = edge_vec.data[5]; 
		edge_vec2.data[6] = edge_vec.data[6]; 
		edge_vec2.data[7] = edge_vec.data[7]; 
	} else if(s_==1){ 
		edge_vec2.data[7] = edge_vec.data[0]; 
		edge_vec2.data[0] = edge_vec.data[1]; 
		edge_vec2.data[1] = edge_vec.data[2]; 
		edge_vec2.data[2] = edge_vec.data[3]; 
		edge_vec2.data[3] = edge_vec.data[4]; 
		edge_vec2.data[4] = edge_vec.data[5]; 
		edge_vec2.data[5] = edge_vec.data[6]; 
		edge_vec2.data[6] = edge_vec.data[7]; 
	} else if(s_==2){ 
		edge_vec2.data[6] = edge_vec.data[0]; 
		edge_vec2.data[7] = edge_vec.data[1]; 
		edge_vec2.data[0] = edge_vec.data[2]; 
		edge_vec2.data[1] = edge_vec.data[3]; 
		edge_vec2.data[2] = edge_vec.data[4]; 
		edge_vec2.data[3] = edge_vec.data[5]; 
		edge_vec2.data[4] = edge_vec.data[6]; 
		edge_vec2.data[5] = edge_vec.data[7]; 
	} else if(s_==3){ 
		edge_vec2.data[5] = edge_vec.data[0]; 
		edge_vec2.data[6] = edge_vec.data[1]; 
		edge_vec2.data[7] = edge_vec.data[2]; 
		edge_vec2.data[0] = edge_vec.data[3]; 
		edge_vec2.data[1] = edge_vec.data[4]; 
		edge_vec2.data[2] = edge_vec.data[5]; 
		edge_vec2.data[3] = edge_vec.data[6]; 
		edge_vec2.data[4] = edge_vec.data[7]; 
	} else if(s_==4){ 
		edge_vec2.data[4] = edge_vec.data[0]; 
		edge_vec2.data[5] = edge_vec.data[1]; 
		edge_vec2.data[6] = edge_vec.data[2]; 
		edge_vec2.data[7] = edge_vec.data[3]; 
		edge_vec2.data[0] = edge_vec.data[4]; 
		edge_vec2.data[1] = edge_vec.data[5]; 
		edge_vec2.data[2] = edge_vec.data[6]; 
		edge_vec2.data[3] = edge_vec.data[7]; 
	} else if(s_==5){ 
		edge_vec2.data[3] = edge_vec.data[0]; 
		edge_vec2.data[4] = edge_vec.data[1]; 
		edge_vec2.data[5] = edge_vec.data[2]; 
		edge_vec2.data[6] = edge_vec.data[3]; 
		edge_vec2.data[7] = edge_vec.data[4]; 
		edge_vec2.data[0] = edge_vec.data[5]; 
		edge_vec2.data[1] = edge_vec.data[6]; 
		edge_vec2.data[2] = edge_vec.data[7]; 
	} else if(s_==6){ 
		edge_vec2.data[2] = edge_vec.data[0]; 
		edge_vec2.data[3] = edge_vec.data[1]; 
		edge_vec2.data[4] = edge_vec.data[2]; 
		edge_vec2.data[5] = edge_vec.data[3]; 
		edge_vec2.data[6] = edge_vec.data[4]; 
		edge_vec2.data[7] = edge_vec.data[5]; 
		edge_vec2.data[0] = edge_vec.data[6]; 
		edge_vec2.data[1] = edge_vec.data[7]; 
	} else { 
		edge_vec2.data[1] = edge_vec.data[0]; 
		edge_vec2.data[2] = edge_vec.data[1]; 
		edge_vec2.data[3] = edge_vec.data[2]; 
		edge_vec2.data[4] = edge_vec.data[3]; 
		edge_vec2.data[5] = edge_vec.data[4]; 
		edge_vec2.data[6] = edge_vec.data[5]; 
		edge_vec2.data[7] = edge_vec.data[6]; 
		edge_vec2.data[0] = edge_vec.data[7]; 
	}		
	return edge_vec2;
}

void loadedges::getXYLayoutVx16(unsigned int s, unsigned int depths[EDGEDATA_PACKINGSIZE]){
	unsigned int s_ = s % EDGEDATA_PACKINGSIZE;
	if(s_==0){ 
		depths[0] = 0; 
		depths[1] = 1; 
		depths[2] = 2; 
		depths[3] = 3; 
		depths[4] = 4; 
		depths[5] = 5; 
		depths[6] = 6; 
		depths[7] = 7; 
		depths[8] = 8; 
		depths[9] = 9; 
		depths[10] = 10; 
		depths[11] = 11; 
		depths[12] = 12; 
		depths[13] = 13; 
		depths[14] = 14; 
		depths[15] = 15; 
	} else if(s_==1){ 
		depths[0] = 15; 
		depths[1] = 0; 
		depths[2] = 1; 
		depths[3] = 2; 
		depths[4] = 3; 
		depths[5] = 4; 
		depths[6] = 5; 
		depths[7] = 6; 
		depths[8] = 7; 
		depths[9] = 8; 
		depths[10] = 9; 
		depths[11] = 10; 
		depths[12] = 11; 
		depths[13] = 12; 
		depths[14] = 13; 
		depths[15] = 14; 
	} else if(s_==2){ 
		depths[0] = 14; 
		depths[1] = 15; 
		depths[2] = 0; 
		depths[3] = 1; 
		depths[4] = 2; 
		depths[5] = 3; 
		depths[6] = 4; 
		depths[7] = 5; 
		depths[8] = 6; 
		depths[9] = 7; 
		depths[10] = 8; 
		depths[11] = 9; 
		depths[12] = 10; 
		depths[13] = 11; 
		depths[14] = 12; 
		depths[15] = 13; 
	} else if(s_==3){ 
		depths[0] = 13; 
		depths[1] = 14; 
		depths[2] = 15; 
		depths[3] = 0; 
		depths[4] = 1; 
		depths[5] = 2; 
		depths[6] = 3; 
		depths[7] = 4; 
		depths[8] = 5; 
		depths[9] = 6; 
		depths[10] = 7; 
		depths[11] = 8; 
		depths[12] = 9; 
		depths[13] = 10; 
		depths[14] = 11; 
		depths[15] = 12; 
	} else if(s_==4){ 
		depths[0] = 12; 
		depths[1] = 13; 
		depths[2] = 14; 
		depths[3] = 15; 
		depths[4] = 0; 
		depths[5] = 1; 
		depths[6] = 2; 
		depths[7] = 3; 
		depths[8] = 4; 
		depths[9] = 5; 
		depths[10] = 6; 
		depths[11] = 7; 
		depths[12] = 8; 
		depths[13] = 9; 
		depths[14] = 10; 
		depths[15] = 11; 
	} else if(s_==5){ 
		depths[0] = 11; 
		depths[1] = 12; 
		depths[2] = 13; 
		depths[3] = 14; 
		depths[4] = 15; 
		depths[5] = 0; 
		depths[6] = 1; 
		depths[7] = 2; 
		depths[8] = 3; 
		depths[9] = 4; 
		depths[10] = 5; 
		depths[11] = 6; 
		depths[12] = 7; 
		depths[13] = 8; 
		depths[14] = 9; 
		depths[15] = 10; 
	} else if(s_==6){ 
		depths[0] = 10; 
		depths[1] = 11; 
		depths[2] = 12; 
		depths[3] = 13; 
		depths[4] = 14; 
		depths[5] = 15; 
		depths[6] = 0; 
		depths[7] = 1; 
		depths[8] = 2; 
		depths[9] = 3; 
		depths[10] = 4; 
		depths[11] = 5; 
		depths[12] = 6; 
		depths[13] = 7; 
		depths[14] = 8; 
		depths[15] = 9; 
	} else if(s_==7){ 
		depths[0] = 9; 
		depths[1] = 10; 
		depths[2] = 11; 
		depths[3] = 12; 
		depths[4] = 13; 
		depths[5] = 14; 
		depths[6] = 15; 
		depths[7] = 0; 
		depths[8] = 1; 
		depths[9] = 2; 
		depths[10] = 3; 
		depths[11] = 4; 
		depths[12] = 5; 
		depths[13] = 6; 
		depths[14] = 7; 
		depths[15] = 8; 
	} else if(s_==8){ 
		depths[0] = 8; 
		depths[1] = 9; 
		depths[2] = 10; 
		depths[3] = 11; 
		depths[4] = 12; 
		depths[5] = 13; 
		depths[6] = 14; 
		depths[7] = 15; 
		depths[8] = 0; 
		depths[9] = 1; 
		depths[10] = 2; 
		depths[11] = 3; 
		depths[12] = 4; 
		depths[13] = 5; 
		depths[14] = 6; 
		depths[15] = 7; 
	} else if(s_==9){ 
		depths[0] = 7; 
		depths[1] = 8; 
		depths[2] = 9; 
		depths[3] = 10; 
		depths[4] = 11; 
		depths[5] = 12; 
		depths[6] = 13; 
		depths[7] = 14; 
		depths[8] = 15; 
		depths[9] = 0; 
		depths[10] = 1; 
		depths[11] = 2; 
		depths[12] = 3; 
		depths[13] = 4; 
		depths[14] = 5; 
		depths[15] = 6; 
	} else if(s_==10){ 
		depths[0] = 6; 
		depths[1] = 7; 
		depths[2] = 8; 
		depths[3] = 9; 
		depths[4] = 10; 
		depths[5] = 11; 
		depths[6] = 12; 
		depths[7] = 13; 
		depths[8] = 14; 
		depths[9] = 15; 
		depths[10] = 0; 
		depths[11] = 1; 
		depths[12] = 2; 
		depths[13] = 3; 
		depths[14] = 4; 
		depths[15] = 5; 
	} else if(s_==11){ 
		depths[0] = 5; 
		depths[1] = 6; 
		depths[2] = 7; 
		depths[3] = 8; 
		depths[4] = 9; 
		depths[5] = 10; 
		depths[6] = 11; 
		depths[7] = 12; 
		depths[8] = 13; 
		depths[9] = 14; 
		depths[10] = 15; 
		depths[11] = 0; 
		depths[12] = 1; 
		depths[13] = 2; 
		depths[14] = 3; 
		depths[15] = 4; 
	} else if(s_==12){ 
		depths[0] = 4; 
		depths[1] = 5; 
		depths[2] = 6; 
		depths[3] = 7; 
		depths[4] = 8; 
		depths[5] = 9; 
		depths[6] = 10; 
		depths[7] = 11; 
		depths[8] = 12; 
		depths[9] = 13; 
		depths[10] = 14; 
		depths[11] = 15; 
		depths[12] = 0; 
		depths[13] = 1; 
		depths[14] = 2; 
		depths[15] = 3; 
	} else if(s_==13){ 
		depths[0] = 3; 
		depths[1] = 4; 
		depths[2] = 5; 
		depths[3] = 6; 
		depths[4] = 7; 
		depths[5] = 8; 
		depths[6] = 9; 
		depths[7] = 10; 
		depths[8] = 11; 
		depths[9] = 12; 
		depths[10] = 13; 
		depths[11] = 14; 
		depths[12] = 15; 
		depths[13] = 0; 
		depths[14] = 1; 
		depths[15] = 2; 
	} else if(s_==14){ 
		depths[0] = 2; 
		depths[1] = 3; 
		depths[2] = 4; 
		depths[3] = 5; 
		depths[4] = 6; 
		depths[5] = 7; 
		depths[6] = 8; 
		depths[7] = 9; 
		depths[8] = 10; 
		depths[9] = 11; 
		depths[10] = 12; 
		depths[11] = 13; 
		depths[12] = 14; 
		depths[13] = 15; 
		depths[14] = 0; 
		depths[15] = 1; 
	} else { 
		depths[0] = 1; 
		depths[1] = 2; 
		depths[2] = 3; 
		depths[3] = 4; 
		depths[4] = 5; 
		depths[5] = 6; 
		depths[6] = 7; 
		depths[7] = 8; 
		depths[8] = 9; 
		depths[9] = 10; 
		depths[10] = 11; 
		depths[11] = 12; 
		depths[12] = 13; 
		depths[13] = 14; 
		depths[14] = 15; 
		depths[15] = 0; 
	}
}
edge2_vec_dt loadedges::rearrangeLayoutVx16(unsigned int s, edge2_vec_dt edge_vec){
	edge2_vec_dt edge_vec2;
	unsigned int s_ = s;
	if(s_==0){ 
		edge_vec2.data[0] = edge_vec.data[0]; 
		edge_vec2.data[1] = edge_vec.data[1]; 
		edge_vec2.data[2] = edge_vec.data[2]; 
		edge_vec2.data[3] = edge_vec.data[3]; 
		edge_vec2.data[4] = edge_vec.data[4]; 
		edge_vec2.data[5] = edge_vec.data[5]; 
		edge_vec2.data[6] = edge_vec.data[6]; 
		edge_vec2.data[7] = edge_vec.data[7]; 
		edge_vec2.data[8] = edge_vec.data[8]; 
		edge_vec2.data[9] = edge_vec.data[9]; 
		edge_vec2.data[10] = edge_vec.data[10]; 
		edge_vec2.data[11] = edge_vec.data[11]; 
		edge_vec2.data[12] = edge_vec.data[12]; 
		edge_vec2.data[13] = edge_vec.data[13]; 
		edge_vec2.data[14] = edge_vec.data[14]; 
		edge_vec2.data[15] = edge_vec.data[15]; 
	} else if(s_==1){ 
		edge_vec2.data[15] = edge_vec.data[0]; 
		edge_vec2.data[0] = edge_vec.data[1]; 
		edge_vec2.data[1] = edge_vec.data[2]; 
		edge_vec2.data[2] = edge_vec.data[3]; 
		edge_vec2.data[3] = edge_vec.data[4]; 
		edge_vec2.data[4] = edge_vec.data[5]; 
		edge_vec2.data[5] = edge_vec.data[6]; 
		edge_vec2.data[6] = edge_vec.data[7]; 
		edge_vec2.data[7] = edge_vec.data[8]; 
		edge_vec2.data[8] = edge_vec.data[9]; 
		edge_vec2.data[9] = edge_vec.data[10]; 
		edge_vec2.data[10] = edge_vec.data[11]; 
		edge_vec2.data[11] = edge_vec.data[12]; 
		edge_vec2.data[12] = edge_vec.data[13]; 
		edge_vec2.data[13] = edge_vec.data[14]; 
		edge_vec2.data[14] = edge_vec.data[15]; 
	} else if(s_==2){ 
		edge_vec2.data[14] = edge_vec.data[0]; 
		edge_vec2.data[15] = edge_vec.data[1]; 
		edge_vec2.data[0] = edge_vec.data[2]; 
		edge_vec2.data[1] = edge_vec.data[3]; 
		edge_vec2.data[2] = edge_vec.data[4]; 
		edge_vec2.data[3] = edge_vec.data[5]; 
		edge_vec2.data[4] = edge_vec.data[6]; 
		edge_vec2.data[5] = edge_vec.data[7]; 
		edge_vec2.data[6] = edge_vec.data[8]; 
		edge_vec2.data[7] = edge_vec.data[9]; 
		edge_vec2.data[8] = edge_vec.data[10]; 
		edge_vec2.data[9] = edge_vec.data[11]; 
		edge_vec2.data[10] = edge_vec.data[12]; 
		edge_vec2.data[11] = edge_vec.data[13]; 
		edge_vec2.data[12] = edge_vec.data[14]; 
		edge_vec2.data[13] = edge_vec.data[15]; 
	} else if(s_==3){ 
		edge_vec2.data[13] = edge_vec.data[0]; 
		edge_vec2.data[14] = edge_vec.data[1]; 
		edge_vec2.data[15] = edge_vec.data[2]; 
		edge_vec2.data[0] = edge_vec.data[3]; 
		edge_vec2.data[1] = edge_vec.data[4]; 
		edge_vec2.data[2] = edge_vec.data[5]; 
		edge_vec2.data[3] = edge_vec.data[6]; 
		edge_vec2.data[4] = edge_vec.data[7]; 
		edge_vec2.data[5] = edge_vec.data[8]; 
		edge_vec2.data[6] = edge_vec.data[9]; 
		edge_vec2.data[7] = edge_vec.data[10]; 
		edge_vec2.data[8] = edge_vec.data[11]; 
		edge_vec2.data[9] = edge_vec.data[12]; 
		edge_vec2.data[10] = edge_vec.data[13]; 
		edge_vec2.data[11] = edge_vec.data[14]; 
		edge_vec2.data[12] = edge_vec.data[15]; 
	} else if(s_==4){ 
		edge_vec2.data[12] = edge_vec.data[0]; 
		edge_vec2.data[13] = edge_vec.data[1]; 
		edge_vec2.data[14] = edge_vec.data[2]; 
		edge_vec2.data[15] = edge_vec.data[3]; 
		edge_vec2.data[0] = edge_vec.data[4]; 
		edge_vec2.data[1] = edge_vec.data[5]; 
		edge_vec2.data[2] = edge_vec.data[6]; 
		edge_vec2.data[3] = edge_vec.data[7]; 
		edge_vec2.data[4] = edge_vec.data[8]; 
		edge_vec2.data[5] = edge_vec.data[9]; 
		edge_vec2.data[6] = edge_vec.data[10]; 
		edge_vec2.data[7] = edge_vec.data[11]; 
		edge_vec2.data[8] = edge_vec.data[12]; 
		edge_vec2.data[9] = edge_vec.data[13]; 
		edge_vec2.data[10] = edge_vec.data[14]; 
		edge_vec2.data[11] = edge_vec.data[15]; 
	} else if(s_==5){ 
		edge_vec2.data[11] = edge_vec.data[0]; 
		edge_vec2.data[12] = edge_vec.data[1]; 
		edge_vec2.data[13] = edge_vec.data[2]; 
		edge_vec2.data[14] = edge_vec.data[3]; 
		edge_vec2.data[15] = edge_vec.data[4]; 
		edge_vec2.data[0] = edge_vec.data[5]; 
		edge_vec2.data[1] = edge_vec.data[6]; 
		edge_vec2.data[2] = edge_vec.data[7]; 
		edge_vec2.data[3] = edge_vec.data[8]; 
		edge_vec2.data[4] = edge_vec.data[9]; 
		edge_vec2.data[5] = edge_vec.data[10]; 
		edge_vec2.data[6] = edge_vec.data[11]; 
		edge_vec2.data[7] = edge_vec.data[12]; 
		edge_vec2.data[8] = edge_vec.data[13]; 
		edge_vec2.data[9] = edge_vec.data[14]; 
		edge_vec2.data[10] = edge_vec.data[15]; 
	} else if(s_==6){ 
		edge_vec2.data[10] = edge_vec.data[0]; 
		edge_vec2.data[11] = edge_vec.data[1]; 
		edge_vec2.data[12] = edge_vec.data[2]; 
		edge_vec2.data[13] = edge_vec.data[3]; 
		edge_vec2.data[14] = edge_vec.data[4]; 
		edge_vec2.data[15] = edge_vec.data[5]; 
		edge_vec2.data[0] = edge_vec.data[6]; 
		edge_vec2.data[1] = edge_vec.data[7]; 
		edge_vec2.data[2] = edge_vec.data[8]; 
		edge_vec2.data[3] = edge_vec.data[9]; 
		edge_vec2.data[4] = edge_vec.data[10]; 
		edge_vec2.data[5] = edge_vec.data[11]; 
		edge_vec2.data[6] = edge_vec.data[12]; 
		edge_vec2.data[7] = edge_vec.data[13]; 
		edge_vec2.data[8] = edge_vec.data[14]; 
		edge_vec2.data[9] = edge_vec.data[15]; 
	} else if(s_==7){ 
		edge_vec2.data[9] = edge_vec.data[0]; 
		edge_vec2.data[10] = edge_vec.data[1]; 
		edge_vec2.data[11] = edge_vec.data[2]; 
		edge_vec2.data[12] = edge_vec.data[3]; 
		edge_vec2.data[13] = edge_vec.data[4]; 
		edge_vec2.data[14] = edge_vec.data[5]; 
		edge_vec2.data[15] = edge_vec.data[6]; 
		edge_vec2.data[0] = edge_vec.data[7]; 
		edge_vec2.data[1] = edge_vec.data[8]; 
		edge_vec2.data[2] = edge_vec.data[9]; 
		edge_vec2.data[3] = edge_vec.data[10]; 
		edge_vec2.data[4] = edge_vec.data[11]; 
		edge_vec2.data[5] = edge_vec.data[12]; 
		edge_vec2.data[6] = edge_vec.data[13]; 
		edge_vec2.data[7] = edge_vec.data[14]; 
		edge_vec2.data[8] = edge_vec.data[15]; 
	} else if(s_==8){ 
		edge_vec2.data[8] = edge_vec.data[0]; 
		edge_vec2.data[9] = edge_vec.data[1]; 
		edge_vec2.data[10] = edge_vec.data[2]; 
		edge_vec2.data[11] = edge_vec.data[3]; 
		edge_vec2.data[12] = edge_vec.data[4]; 
		edge_vec2.data[13] = edge_vec.data[5]; 
		edge_vec2.data[14] = edge_vec.data[6]; 
		edge_vec2.data[15] = edge_vec.data[7]; 
		edge_vec2.data[0] = edge_vec.data[8]; 
		edge_vec2.data[1] = edge_vec.data[9]; 
		edge_vec2.data[2] = edge_vec.data[10]; 
		edge_vec2.data[3] = edge_vec.data[11]; 
		edge_vec2.data[4] = edge_vec.data[12]; 
		edge_vec2.data[5] = edge_vec.data[13]; 
		edge_vec2.data[6] = edge_vec.data[14]; 
		edge_vec2.data[7] = edge_vec.data[15]; 
	} else if(s_==9){ 
		edge_vec2.data[7] = edge_vec.data[0]; 
		edge_vec2.data[8] = edge_vec.data[1]; 
		edge_vec2.data[9] = edge_vec.data[2]; 
		edge_vec2.data[10] = edge_vec.data[3]; 
		edge_vec2.data[11] = edge_vec.data[4]; 
		edge_vec2.data[12] = edge_vec.data[5]; 
		edge_vec2.data[13] = edge_vec.data[6]; 
		edge_vec2.data[14] = edge_vec.data[7]; 
		edge_vec2.data[15] = edge_vec.data[8]; 
		edge_vec2.data[0] = edge_vec.data[9]; 
		edge_vec2.data[1] = edge_vec.data[10]; 
		edge_vec2.data[2] = edge_vec.data[11]; 
		edge_vec2.data[3] = edge_vec.data[12]; 
		edge_vec2.data[4] = edge_vec.data[13]; 
		edge_vec2.data[5] = edge_vec.data[14]; 
		edge_vec2.data[6] = edge_vec.data[15]; 
	} else if(s_==10){ 
		edge_vec2.data[6] = edge_vec.data[0]; 
		edge_vec2.data[7] = edge_vec.data[1]; 
		edge_vec2.data[8] = edge_vec.data[2]; 
		edge_vec2.data[9] = edge_vec.data[3]; 
		edge_vec2.data[10] = edge_vec.data[4]; 
		edge_vec2.data[11] = edge_vec.data[5]; 
		edge_vec2.data[12] = edge_vec.data[6]; 
		edge_vec2.data[13] = edge_vec.data[7]; 
		edge_vec2.data[14] = edge_vec.data[8]; 
		edge_vec2.data[15] = edge_vec.data[9]; 
		edge_vec2.data[0] = edge_vec.data[10]; 
		edge_vec2.data[1] = edge_vec.data[11]; 
		edge_vec2.data[2] = edge_vec.data[12]; 
		edge_vec2.data[3] = edge_vec.data[13]; 
		edge_vec2.data[4] = edge_vec.data[14]; 
		edge_vec2.data[5] = edge_vec.data[15]; 
	} else if(s_==11){ 
		edge_vec2.data[5] = edge_vec.data[0]; 
		edge_vec2.data[6] = edge_vec.data[1]; 
		edge_vec2.data[7] = edge_vec.data[2]; 
		edge_vec2.data[8] = edge_vec.data[3]; 
		edge_vec2.data[9] = edge_vec.data[4]; 
		edge_vec2.data[10] = edge_vec.data[5]; 
		edge_vec2.data[11] = edge_vec.data[6]; 
		edge_vec2.data[12] = edge_vec.data[7]; 
		edge_vec2.data[13] = edge_vec.data[8]; 
		edge_vec2.data[14] = edge_vec.data[9]; 
		edge_vec2.data[15] = edge_vec.data[10]; 
		edge_vec2.data[0] = edge_vec.data[11]; 
		edge_vec2.data[1] = edge_vec.data[12]; 
		edge_vec2.data[2] = edge_vec.data[13]; 
		edge_vec2.data[3] = edge_vec.data[14]; 
		edge_vec2.data[4] = edge_vec.data[15]; 
	} else if(s_==12){ 
		edge_vec2.data[4] = edge_vec.data[0]; 
		edge_vec2.data[5] = edge_vec.data[1]; 
		edge_vec2.data[6] = edge_vec.data[2]; 
		edge_vec2.data[7] = edge_vec.data[3]; 
		edge_vec2.data[8] = edge_vec.data[4]; 
		edge_vec2.data[9] = edge_vec.data[5]; 
		edge_vec2.data[10] = edge_vec.data[6]; 
		edge_vec2.data[11] = edge_vec.data[7]; 
		edge_vec2.data[12] = edge_vec.data[8]; 
		edge_vec2.data[13] = edge_vec.data[9]; 
		edge_vec2.data[14] = edge_vec.data[10]; 
		edge_vec2.data[15] = edge_vec.data[11]; 
		edge_vec2.data[0] = edge_vec.data[12]; 
		edge_vec2.data[1] = edge_vec.data[13]; 
		edge_vec2.data[2] = edge_vec.data[14]; 
		edge_vec2.data[3] = edge_vec.data[15]; 
	} else if(s_==13){ 
		edge_vec2.data[3] = edge_vec.data[0]; 
		edge_vec2.data[4] = edge_vec.data[1]; 
		edge_vec2.data[5] = edge_vec.data[2]; 
		edge_vec2.data[6] = edge_vec.data[3]; 
		edge_vec2.data[7] = edge_vec.data[4]; 
		edge_vec2.data[8] = edge_vec.data[5]; 
		edge_vec2.data[9] = edge_vec.data[6]; 
		edge_vec2.data[10] = edge_vec.data[7]; 
		edge_vec2.data[11] = edge_vec.data[8]; 
		edge_vec2.data[12] = edge_vec.data[9]; 
		edge_vec2.data[13] = edge_vec.data[10]; 
		edge_vec2.data[14] = edge_vec.data[11]; 
		edge_vec2.data[15] = edge_vec.data[12]; 
		edge_vec2.data[0] = edge_vec.data[13]; 
		edge_vec2.data[1] = edge_vec.data[14]; 
		edge_vec2.data[2] = edge_vec.data[15]; 
	} else if(s_==14){ 
		edge_vec2.data[2] = edge_vec.data[0]; 
		edge_vec2.data[3] = edge_vec.data[1]; 
		edge_vec2.data[4] = edge_vec.data[2]; 
		edge_vec2.data[5] = edge_vec.data[3]; 
		edge_vec2.data[6] = edge_vec.data[4]; 
		edge_vec2.data[7] = edge_vec.data[5]; 
		edge_vec2.data[8] = edge_vec.data[6]; 
		edge_vec2.data[9] = edge_vec.data[7]; 
		edge_vec2.data[10] = edge_vec.data[8]; 
		edge_vec2.data[11] = edge_vec.data[9]; 
		edge_vec2.data[12] = edge_vec.data[10]; 
		edge_vec2.data[13] = edge_vec.data[11]; 
		edge_vec2.data[14] = edge_vec.data[12]; 
		edge_vec2.data[15] = edge_vec.data[13]; 
		edge_vec2.data[0] = edge_vec.data[14]; 
		edge_vec2.data[1] = edge_vec.data[15]; 
	} else { 
		edge_vec2.data[1] = edge_vec.data[0]; 
		edge_vec2.data[2] = edge_vec.data[1]; 
		edge_vec2.data[3] = edge_vec.data[2]; 
		edge_vec2.data[4] = edge_vec.data[3]; 
		edge_vec2.data[5] = edge_vec.data[4]; 
		edge_vec2.data[6] = edge_vec.data[5]; 
		edge_vec2.data[7] = edge_vec.data[6]; 
		edge_vec2.data[8] = edge_vec.data[7]; 
		edge_vec2.data[9] = edge_vec.data[8]; 
		edge_vec2.data[10] = edge_vec.data[9]; 
		edge_vec2.data[11] = edge_vec.data[10]; 
		edge_vec2.data[12] = edge_vec.data[11]; 
		edge_vec2.data[13] = edge_vec.data[12]; 
		edge_vec2.data[14] = edge_vec.data[13]; 
		edge_vec2.data[15] = edge_vec.data[14]; 
		edge_vec2.data[0] = edge_vec.data[15]; 
	}
	return edge_vec2;
}

void loadedges::getXYLayoutV(unsigned int s, unsigned int depths[EDGEDATA_PACKINGSIZE]){
	#ifdef CONFIG_UPDATEPACKINGx16
	return getXYLayoutVx16(s, depths);
	#else 
	return getXYLayoutVx8(s, depths);
	#endif 
}
edge2_vec_dt loadedges::rearrangeLayoutV(unsigned int s, edge2_vec_dt edge_vec){
	#ifdef CONFIG_UPDATEPACKINGx16
	return rearrangeLayoutVx16(s, edge_vec);
	#else 
	return rearrangeLayoutVx8(s, edge_vec);
	#endif 
}

globalparams_TWOt loadedges::start(unsigned int col, vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, vptr_type * vptrs[MAXNUM_PEs], uint512_vec_dt * edges[MAXNUM_PEs], vector<edge2_vec_dt> (&edges_final)[MAXNUM_PEs], map_t * edges_map[MAXNUM_PEs][MAXNUM_VPs], keyvalue_t * edgeblock_map[MAXNUM_PEs][MAXNUM_VPs], container_t * container, globalparams_TWOt globalparams){						
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"loadedges::start:: loading edges... "<<endl;
	#endif 
	
	unsigned int kvdata_range__div__vptr_shrink_ratio = (universalparams.KVDATA_RANGE + (universalparams.VPTR_SHRINK_RATIO-1)) / universalparams.VPTR_SHRINK_RATIO;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsK.SIZE_MESSAGESDRAM;
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsE.SIZE_MESSAGESDRAM; 
	
	unsigned int num_vPs = universalparams.NUMPROCESSEDGESPARTITIONS;
	unsigned int vsize_vP = universalparams.PROCESSPARTITIONSZ;
	unsigned int num_LLPs = universalparams.NUMREDUCEPARTITIONS * universalparams.NUM_PARTITIONS; 
	unsigned int vsize_LLP; 
	if(universalparams.ALGORITHM == BFS){ vsize_LLP = 262144; } //  || universalparams.ALGORITHM == SSSP
	else { vsize_LLP = universalparams.REDUCEPARTITIONSZ_KVS2; } //
	unsigned int num_LLPset = (num_LLPs + (universalparams.NUM_PARTITIONS - 1)) / universalparams.NUM_PARTITIONS;
	
	unsigned int _NUM_PEs = NUM_PEs;
	unsigned int _num_vPs = num_vPs; // CRIICAL FIXME.
	unsigned int _num_LLPs = num_LLPs;
	unsigned int _num_LLPset = num_LLPset;
	
	vector<edge2_type> edges_in_channel[MAXNUM_PEs];
	vector<edge2_type> edgesin_srcvp[MAXNUM_VPs];
	vector<edge2_type> edgesin_srcvp_lldstvp[num_LLPs];
	vector<edge2_type> edgesin_srcvp_lldstvp_srcv2p[num_LLPs][OPT_NUM_PARTITIONS]; // EDGEDATA_PACKINGSIZE
	unsigned int edge_count_in_vpartition[MAXNUM_PEs][MAXNUM_VPs];
	std::vector<edge2_type> edges_temp[EDGEDATA_PACKINGSIZE];
	vector<edge2_type> edgeblock_temp[EDGEDATA_PACKINGSIZE][MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION]; 
	
	for(unsigned int k=0; k<EDGEDATA_PACKINGSIZE; k++){ for(unsigned int t=0; t<MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION; t++){ edgeblock_temp[k][t].clear(); }}
	keyvalue_t * temp_stats[EDGEDATA_PACKINGSIZE];  for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ temp_stats[v] = new keyvalue_t[MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION]; }
	for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ for(unsigned int t=0; t<MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION; t++){ temp_stats[v][t].key = 0; temp_stats[v][t].value = 0; }}
		
	for(unsigned int i=0; i<_NUM_PEs; i++){ edges_final[i].clear(); }
	for(unsigned int i=0; i<_NUM_PEs; i++){ for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ edges_map[i][v_p][ll_p].offset = 0; edges_map[i][v_p][ll_p].size = 0; }}}
	for(unsigned int i=0; i<_NUM_PEs; i++){ for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ edge_count_in_vpartition[i][v_p] = 0; }}
		
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadedges:: [num_LLPs: "<<num_LLPs<<", num_LLPset: "<<num_LLPset<<", vsize_LLP: "<<vsize_LLP<<", num_vPs: "<<num_vPs<<", vsize_vP: "<<vsize_vP<<", INVALIDDATA: "<<INVALIDDATA<<"] "<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// partiition into HBM channels 
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"loading edges [STAGE 0]: distributing edges into HBM channels "<<endl;
	#endif 
	for(unsigned int vid=0; vid<utilityobj->hmin(universalparams.NUM_VERTICES, universalparams.KVDATA_RANGE)-1; vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		if(vid % 10000000 == 0){ cout<<"### loadedges::start:: vid: "<<vid<<" (of "<<universalparams.NUM_VERTICES<<" vertices), vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		#endif 
		#ifdef _DEBUGMODE_HOSTCHECKS3
		utilityobj->checkoutofbounds("loadedges::calculate counts_validedges_for_channel(19)::", vid, universalparams.KVDATA_RANGE, NAp, NAp, NAp);
		utilityobj->checkoutofbounds("loadedges::calculate counts_validedges_for_channel(19b)::", vid+1, universalparams.KVDATA_RANGE, NAp, NAp, NAp);
		#endif 
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ continue; cout<<">>> loadedges:: vptr_end("<<vptr_end<<") < vptr_begin("<<vptr_begin<<"). EXITING... (vid: "<<vid<<", edges_size: "<<edges_size<<")"<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int i=0; i<edges_size; i++){
			edge2_type edge = edgedatabuffer[vptr_begin + i];
			#ifdef _DEBUGMODE_HOSTCHECKS3
			if(edge.srcvid >= universalparams.KVDATA_RANGE || edge.dstvid >= universalparams.KVDATA_RANGE){ continue; } // edge.dstvid = edge.dstvid % universalparams.KVDATA_RANGE; } // CRIICAL FIXME.
			#endif 
			
			#ifdef CONFIG_EDGEHASHSCHEME_SETVIDS
			unsigned int H = (edge.dstvid % (EDGEDATA_PACKINGSIZE * NUM_PEs)) / EDGEDATA_PACKINGSIZE;
			edge.dstvid = ((edge.dstvid / (EDGEDATA_PACKINGSIZE * NUM_PEs)) * EDGEDATA_PACKINGSIZE) + (edge.dstvid % EDGEDATA_PACKINGSIZE); 
			#endif 
			
			#ifdef CONFIG_EDGEHASHSCHEME_SINGLEVID
			unsigned int H = edge.dstvid % NUM_PEs;
			edge.dstvid = (edge.dstvid - H) / NUM_PEs; 
			#endif 
			
			edges_in_channel[H].push_back(edge);
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS4
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		cout<<"loadedges:: edges_in_channel["<<i<<"].size(): "<<edges_in_channel[i].size()<<endl; 
		if(edges_in_channel[i].size() > 0){ for(unsigned int t=0; t<0; t++){ cout<<"+++ loadedges:: edges_in_channel["<<i<<"]["<<t<<"]: srcvid: "<<edges_in_channel[i][t].srcvid<<", dstvid: "<<edges_in_channel[i][t].dstvid<<", hashed dstvid: "<<edges_in_channel[i][t].dstvid % (EDGEDATA_PACKINGSIZE * NUM_PEs)<<endl; }}
	}
	#endif 
	// exit(EXIT_SUCCESS);
	
	bool debug = false; // true, false;
	bool debug_detail = false;
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loading edges [STAGE 1]: preparing edges..."<<endl;
	#endif 
	for(unsigned int i=0; i<_NUM_PEs; i++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadedges:: [PE: "<<i<<"]"<<endl;
		#endif 
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		unsigned int index = 0;
		unsigned int last_offset = 0;
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ edgesin_srcvp[v_p].clear(); } // clear 
		
		// within a HBM channel, partition into v-partitions 
		if(debug){ cout<<"STAGE 1: within a HBM channel, partition into v-partitions "<<endl; }
		for(unsigned int t=0; t<edges_in_channel[i].size(); t++){
			edge2_type edge = edges_in_channel[i][t];
			unsigned int vP = (edge.srcvid / vsize_vP);
			#ifdef _DEBUGMODE_HOSTCHECKS3
			utilityobj->checkoutofbounds("loadedges::ERROR 22::", vP, num_vPs, edge.srcvid, edge.srcvid, vsize_vP);
			#endif 
			edgesin_srcvp[vP].push_back(edge);
		}
		
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ edgesin_srcvp_lldstvp[ll_p].clear(); } // clear
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ for(unsigned int p=0; p<universalparams.NUM_PARTITIONS; p++){ edgesin_srcvp_lldstvp_srcv2p[ll_p][p].clear(); }} // clear
		
			// within a v-partition, partition into last-level-partitions (LLPs) --- according to reduce concurrency degree (i.e. 8 for kv=u64, 16 for kv=u32)
			if(debug){ cout<<"STAGE 2: within a v-partition ("<<v_p<<", partition into last-level-partitions (LLPs)"<<endl; } // debug
			// [[0,16,...][1,17...][2,18,...]...[15,31,...]]...[[0,16,...][1,17,...][2,18,...]...[15,31,...]]. distributed by modulo(%) function
			for(unsigned int t=0; t<edgesin_srcvp[v_p].size(); t++){
				edge2_type edge = edgesin_srcvp[v_p][t];
				unsigned int ll_p = edge.dstvid / vsize_LLP;
				unsigned int newll_p = ((ll_p / EDGEDATA_PACKINGSIZE) * EDGEDATA_PACKINGSIZE) + (edge.dstvid % EDGEDATA_PACKINGSIZE);
				#ifdef _DEBUGMODE_HOSTCHECKS3
				utilityobj->checkoutofbounds("loadedges::ERROR 23::", newll_p, num_LLPs, edge.dstvid, vsize_LLP, ll_p);
				#endif 
				
				edgesin_srcvp_lldstvp[newll_p].push_back(edge);
			}
			if(false){ cout<<"loadedges[STAGE 2 check]:: {srcvid, dstvid}"<<endl; }
			
			// witihin a LLP, re-arrange by srcvids 
			if(debug){ cout<<"STAGE 3: witihin a LLP, re-arrange by srcvids "<<endl; }
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){
				for(unsigned int t=0; t<edgesin_srcvp_lldstvp[ll_p].size(); t++){
					edge2_type edge = edgesin_srcvp_lldstvp[ll_p][t];
					#ifdef _DEBUGMODE_HOSTCHECKS3
					unsigned int u_l = ((ll_p+1) + (OPT_NUM_PARTITIONS-1)) / OPT_NUM_PARTITIONS;
					if(edge.dstvid != INVALIDDATA && ((edge.srcvid - (v_p * vsize_vP) >= vsize_vP) || (edge.dstvid >= u_l * (vsize_LLP * VDATA_PACKINGSIZE)))){ cout<<"loadedges:: ERROR 234. edge_vec data is out of bounds. edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", v_p: "<<v_p<<", ll_p: "<<ll_p<<", vsize_vP: "<<vsize_vP<<", vsize_LLP: "<<vsize_LLP<<", u_l: "<<u_l<<". EXITING..."<<endl; exit(EXIT_FAILURE); }
					#endif 
					edgesin_srcvp_lldstvp_srcv2p[ll_p][edge.srcvid % EDGEDATA_PACKINGSIZE].push_back(edge);
				}
			}
			// exit(EXIT_SUCCESS);
			
			// witihin a LLP, re-arrange by srcvids, also place edges in dram
			if(debug){ cout<<"STAGE 4: preparing edges and loading into dram..."<<endl; }
			// edge2_type dummy_edge; dummy_edge.srcvid = INVALIDDATA; dummy_edge.dstvid = INVALIDDATA; 
			unsigned int depths[EDGEDATA_PACKINGSIZE];
			for(unsigned int llp_set=0; llp_set<_num_LLPset; llp_set++){ // ll_p set 
				unsigned int offset_llpset = llp_set * EDGEDATA_PACKINGSIZE; // universalparams.NUM_PARTITIONS;
				
				for(unsigned int t=0; t<EDGEDATA_PACKINGSIZE; t++){ edges_temp[t].clear(); } // clear
				for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ for(unsigned int t=0; t<MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION; t++){ edgeblock_temp[v][t].clear(); }}
				edge2_type last_edge[EDGEDATA_PACKINGSIZE]; for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ last_edge[v].srcvid = 0; last_edge[v].dstvid = 0; }
				
				for(unsigned int llp_id=0; llp_id<EDGEDATA_PACKINGSIZE; llp_id++){ 
					if(false){ cout<<"loadedges:: [llp_set: "<<llp_set<<", llp_id: "<<llp_id<<"]"<<endl; }
				
					// get layout
					getXYLayoutV(llp_id, depths);
					
					// get maximum
					unsigned int max_kvs = 0; 
					for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ if(edgesin_srcvp_lldstvp_srcv2p[offset_llpset + v][depths[v]].size() > max_kvs){ max_kvs = edgesin_srcvp_lldstvp_srcv2p[v][depths[v]].size(); }
						else if(edgesin_srcvp_lldstvp_srcv2p[offset_llpset + v][depths[v]].size() == 0){ if(debug_detail){ cout<<"!!!!!!!!!!!!!!!!! loadedges:: EMPTY PARTITION SEEN @ p: "<<offset_llpset + v<<", v: "<<v<<", llp_set: "<<llp_set<<", llp_id: "<<llp_id<<", max_kvs: "<<max_kvs<<endl; }}}
				
					for(buffer_type t=0; t<max_kvs; t++){
						edge2_vec_dt edge_vec;
						for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ 
							if(t < edgesin_srcvp_lldstvp_srcv2p[offset_llpset + v][depths[v]].size()){ 
								edge_vec.data[v] = edgesin_srcvp_lldstvp_srcv2p[offset_llpset + v][depths[v]][t]; 
								edge_vec.data[v].valid = true; 
								last_edge[depths[v]] = edge_vec.data[v]; 
							} 
							else { 
								// edge_vec.data[v] = dummy_edge; 
								edge_vec.data[v].srcvid = last_edge[depths[v]].srcvid; 
								edge_vec.data[v].dstvid = last_edge[depths[v]].dstvid; 
								edge_vec.data[v].valid = false; 
							} // use srcvid of last known valid edge 
						}
						
						#ifdef _DEBUGMODE_HOSTCHECKS3
						for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ 
							if(edge_vec.data[v].dstvid != INVALIDDATA){ 
								if(edge_vec.data[v].dstvid % EDGEDATA_PACKINGSIZE != v || edge_vec.data[v].dstvid - (llp_set * vsize_LLP * EDGEDATA_PACKINGSIZE) >= vsize_LLP * EDGEDATA_PACKINGSIZE){ 
									cout<<"loadedges:: ERROR 452. edge_vec.data["<<v<<"].dstvid("<<edge_vec.data[v].dstvid<<") % EDGEDATA_PACKINGSIZE("<<EDGEDATA_PACKINGSIZE<<") != v("<<v<<"). EXITING..."<<endl; 
									cout<<"loadedges:: ERROR 452b. edge_vec.data[v].dstvid - (llp_set("<<llp_set<<") * vsize_LLP("<<vsize_LLP<<") * EDGEDATA_PACKINGSIZE("<<EDGEDATA_PACKINGSIZE<<")(="<<llp_set * vsize_LLP * EDGEDATA_PACKINGSIZE<<")  < vsize_LLP * EDGEDATA_PACKINGSIZE(="<<vsize_LLP * EDGEDATA_PACKINGSIZE<<"). EXITING..."<<endl; 
									for(unsigned int v1=0; v1<EDGEDATA_PACKINGSIZE; v1++){ cout<<"edge_vec.data["<<v1<<"].srcvid: "<<edge_vec.data[v1].srcvid<<"("<<edge_vec.data[v1].srcvid % EDGEDATA_PACKINGSIZE<<"), edge_vec.data["<<v1<<"].dstvid: "<<edge_vec.data[v1].dstvid<<"("<<edge_vec.data[v1].dstvid % EDGEDATA_PACKINGSIZE<<"), depths["<<v1<<"]: "<<depths[v1]<<endl; } 
									exit(EXIT_FAILURE); 
								}
							}
						}
						#endif 
						
						// re-arrange
						edge2_vec_dt edge_vec2 = rearrangeLayoutV(llp_id, edge_vec);
						
						#ifdef _DEBUGMODE_HOSTCHECKS3
						for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){
							if(edge_vec2.data[v].dstvid != INVALIDDATA){
								if(edge_vec2.data[v].srcvid % EDGEDATA_PACKINGSIZE != v || edge_vec2.data[v].srcvid - (v_p * vsize_vP) >= vsize_vP){ 
									cout<<"loadedges:: ERROR 453. edge_vec2.data["<<v<<"].srcvid("<<edge_vec2.data[v].srcvid<<") % EDGEDATA_PACKINGSIZE("<<EDGEDATA_PACKINGSIZE<<")(="<<edge_vec2.data[v].srcvid % EDGEDATA_PACKINGSIZE<<") != v("<<v<<"). EXITING..."<<endl; 
									cout<<"loadedges:: ERROR 452b. edge_vec2.data["<<v<<"].srcvid("<<edge_vec2.data[v].srcvid<<") - ((v_p("<<v_p<<") * vsize_vP("<<vsize_vP<<")(="<<v_p * vsize_vP<<")) >= vsize_vP(="<<vsize_vP<<"). EXITING..."<<endl; 
									for(unsigned int v1=0; v1<EDGEDATA_PACKINGSIZE; v1++){ cout<<"edge_vec2.data["<<v1<<"].srcvid: "<<edge_vec2.data[v1].srcvid<<"("<<edge_vec2.data[v1].srcvid % EDGEDATA_PACKINGSIZE<<"), edge_vec2.data["<<v1<<"].dstvid: "<<edge_vec2.data[v1].dstvid<<"("<<edge_vec2.data[v1].dstvid % EDGEDATA_PACKINGSIZE<<"), depths["<<v1<<"]: "<<depths[v1]<<endl; }
									exit(EXIT_FAILURE); 
								}
							}
						}
						#endif
						
						// use local ids
						edge2_vec_dt edge_vec2_local = edge_vec2;
						#ifdef CONFIG_SEND_LOCAL_VERTEXIDS_ONLY
						// for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ edge_vec2_local.data[v].srcvid = edge_vec2.data[v].srcvid - (v_p * vsize_vP); edge_vec2_local.data[v].dstvid = edge_vec2.data[v].dstvid - (llp_set * vsize_LLP * EDGEDATA_PACKINGSIZE); }
						// for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ edge_vec2_local.data[v].srcvid = (edge_vec2.data[v].srcvid - (v_p * vsize_vP)) / EDGEDATA_PACKINGSIZE; edge_vec2_local.data[v].dstvid = (edge_vec2.data[v].dstvid - (llp_set * vsize_LLP * EDGEDATA_PACKINGSIZE)) / EDGEDATA_PACKINGSIZE; }
						for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ edge_vec2_local.data[v].srcvid = (edge_vec2.data[v].srcvid - (v_p * vsize_vP)) / EDGEDATA_PACKINGSIZE; edge_vec2_local.data[v].dstvid = edge_vec2.data[v].dstvid - (llp_set * vsize_LLP * EDGEDATA_PACKINGSIZE); }
						#endif 
						
						// update edges_temp & edgeblock_temp
						for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ edges_temp[v].push_back(edge_vec2.data[v]); }
						#ifdef _DEBUGMODE_HOSTCHECKS3
						for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ utilityobj->checkoutofbounds("loadedges::ERROR 561::", (edges_temp[v][t].srcvid - (v_p * vsize_vP)) / MAXVSIZE_ACTVEDGEBLOCK, MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION, edges_temp[v][t].srcvid, v_p, vsize_vP);	}
						#endif 
						for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ edgeblock_temp[v][(edges_temp[v][t].srcvid - (v_p * vsize_vP)) / MAXVSIZE_ACTVEDGEBLOCK].push_back(edges_temp[v][t]); }
						
						// update edges dram 
						for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v+=EDGEDATA_PACKINGSIZE / VECTOR_SIZE){
							#ifdef CONFIG_UPDATEPACKINGx16
							utilityobj->checkoutofbounds("loadedges::ERROR 56::", globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index, universalparams.MAXHBMCAPACITY_KVS2 * VECTOR_SIZE, globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA, t, NAp);	
							utilityobj->checkoutofbounds("loadedges::ERROR 57::", edge_vec2_local.data[v].srcvid, (1 << SRCVID_BITSZ), globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA, t, v);	
							utilityobj->checkoutofbounds("loadedges::ERROR 58::", edge_vec2_local.data[v].dstvid, (1 << DSTVID_BITSZ), globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA, t, v);	
							utilityobj->checkoutofbounds("loadedges::ERROR 57::", edge_vec2_local.data[v+1].srcvid, (1 << SRCVID_BITSZ), globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA, t, v);	
							utilityobj->checkoutofbounds("loadedges::ERROR 58::", edge_vec2_local.data[v+1].dstvid, (1 << DSTVID_BITSZ), globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA, t, v);	
							if(edge_vec2.data[v].valid == true){ edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v/2].key = (edge_vec2_local.data[v].srcvid << DSTVID_BITSZ) | edge_vec2_local.data[v].dstvid; } else { edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v/2].key = INVALIDDATA; }
							if(edge_vec2.data[v+1].valid == true){ edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v/2].value = (edge_vec2_local.data[v+1].srcvid << DSTVID_BITSZ) | edge_vec2_local.data[v+1].dstvid; } else { edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v/2].value = INVALIDDATA; }	
							#else 
							utilityobj->checkoutofbounds("loadedges::ERROR 56::", globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index, universalparams.MAXHBMCAPACITY_KVS2 * VECTOR_SIZE, globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA, t, NAp);	
							if(edge_vec2.data[v].valid == true){ edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v].key = edge_vec2.data[v].dstvid; } else { edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v].key = INVALIDDATA; }
							if(edge_vec2.data[v].valid == true){ edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v].value = edge_vec2.data[v].srcvid; } else { edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v].value = INVALIDDATA; }
							#endif 
							
							#ifdef _DEBUGMODE_KERNELPRINTS
							cout<<"edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + "<<index<<"].data["<<v/2<<"].key: "<<edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v/2].key<<", edge_vec2_local.data["<<v<<"].srcvid: "<<edge_vec2_local.data[v].srcvid<<", edge_vec2_local.data["<<v<<"].dstvid: "<<edge_vec2_local.data[v].dstvid<<endl;
							cout<<"edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + "<<index<<"].value["<<v/2<<"].key: "<<edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + index].data[v/2].value<<", edge_vec2_local.data["<<v+1<<"].srcvid: "<<edge_vec2_local.data[v+1].srcvid<<", edge_vec2_local.data["<<v+1<<"].dstvid: "<<edge_vec2_local.data[v+1].dstvid<<endl;
							#endif 
							// exit(EXIT_SUCCESS);
						}	
						// for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){
							// if(i==0 && v_p==0 && v==1 & edge_vec2_local.data[v].srcvid == 0){
								// cout<<"~~~~~~~~ edge_vec2_local.data["<<v<<"].srcvid: "<<edge_vec2_local.data[v].srcvid<<", edge_vec2_local.data["<<v<<"].dstvid: "<<edge_vec2_local.data[v].dstvid<<" ::::::: edge_vec2.data["<<v<<"].srcvid: "<<edge_vec2.data[v].srcvid<<", edge_vec2.data["<<v<<"].dstvid: "<<edge_vec2.data[v].dstvid<<endl;
							// }
						// }
						// exit(EXIT_SUCCESS);
						// index += EDGEDATA_PACKINGSIZE / VECTOR_SIZE; // 1;
						index += 1;
						
						// update edges_final, edges_map & edge_count_in_vpartition
						edges_final[i].push_back(edge_vec2); // edge_vec2
						edges_map[i][v_p][llp_set].size += EDGEDATA_PACKINGSIZE; 
						edge_count_in_vpartition[i][v_p] += EDGEDATA_PACKINGSIZE; 	
					}
				} // iteration end: llp_id:EDGEDATA_PACKINGSIZE
				// exit(EXIT_SUCCESS);
				
				for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ sort(edges_temp[v].begin(), edges_temp[v].end(), myfunction); } // CRIICAL FIXME.
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout << "\nArray edges_temp is sorted by srcvids: "<<endl; for (int t = 0; t < 8; t++){ for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ cout << "{"<< edges_temp[v][t].srcvid << ", "<<edges_temp[v][t].dstvid<<"} "; } cout<<endl; }
				#endif 
				
				if(false){ cout<<"loadedges:: load edgeblock maps"<<endl; }
				unsigned int totalsz = 0;
				for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ for(unsigned int t=0; t<MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION; t++){ temp_stats[v][t].key = 0; temp_stats[v][t].value = edgeblock_temp[v][t].size(); }}
				for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ utilityobj->calculateoffsets(temp_stats[v], MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION, 0, VECTOR_SIZE); }
				for (int t = 0; t < MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION; t++){
					// load edgeblock_map
					unsigned int min = (1 << 31); unsigned int max = 0;
					for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ if(min > temp_stats[v][t].key){ min = temp_stats[v][t].key; }}
					for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ if(max < temp_stats[v][t].key + temp_stats[v][t].value){ max = temp_stats[v][t].key + temp_stats[v][t].value; }}
					
					edgeblock_map[i][v_p][t].key = min;
					edgeblock_map[i][v_p][t].value = max - min;
					totalsz += max - min;
					
					if(false && i==0 && v_p==0 && t<16){ cout<<"### loadedges:: ["<<i<<"]["<<v_p<<"]["<<t<<"]: min offset: "<<min<<", max offset: "<<max<<", size: "<<(max - min)<<endl; }
				}
				if(false){ cout<<"### edges_final["<<i<<"].size(): "<<edges_final[i].size()<<", edges_temp[0].size(): "<<edges_temp[0].size()<<", totalsz: "<<totalsz<<endl; }
				// exit(EXIT_SUCCESS);
			} // iteration end: llp_set:num_LLPset
			#ifdef _DEBUGMODE_KERNELPRINTS3
			if(i==0 && v_p == 0){ cout<<"loadedges:: edges_final[0].size(): "<<edges_final[0].size()<<", last_offset: "<<last_offset<<", last_offset(uedge): "<<last_offset * EDGEDATA_PACKINGSIZE<<endl<<endl; }
			#endif 
		} // iteration end: v_p:num_vPs 
		// exit(EXIT_SUCCESS);
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(true){ cout<<"+++ loadedges:: PE: "<<i<<": edges_final["<<i<<"].size(): "<<edges_final[i].size() * EDGEDATA_PACKINGSIZE<<", last offset: "<<last_offset * EDGEDATA_PACKINGSIZE<<""<<endl; }
		for(unsigned int t=0; t<4; t++){ for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ cout<<"[^"<<(edges_final[i][t].data[v].srcvid / vsize_vP)<<", $"<<(edges_final[i][t].data[v].dstvid / vsize_LLP)<<"], "; } cout<<endl; }
		for(unsigned int t=0; t<4; t++){ for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ cout<<"[&"<<(edges_final[i][t].data[v].srcvid % EDGEDATA_PACKINGSIZE)<<", $"<<(edges_final[i][t].data[v].dstvid % universalparams.NUM_PARTITIONS)<<"], "; } cout<<endl; }
		for(unsigned int t=0; t<4; t++){ for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ cout<<"[#"<<edges_final[i][t].data[v].srcvid<<", $"<<edges_final[i][t].data[v].dstvid<<"], "; } cout<<endl; }
		for(unsigned int t=0; t<0; t++){ for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ cout<<"[#"<<edges_final[i][t].data[v].srcvid<<"("<<(edges_final[i][t].data[v].srcvid % EDGEDATA_PACKINGSIZE)<<")("<<(edges_final[i][t].data[v].srcvid / vsize_vP)<<"), $"<<edges_final[i][t].data[v].dstvid<<"("<<(edges_final[i][t].data[v].dstvid % universalparams.NUM_PARTITIONS)<<")("<<(edges_final[i][t].data[v].dstvid / vsize_LLP)<<")], "; } cout<<endl; }			
		cout<<"=== Edge content in dram "<<i<<": "<<edges_final[i].size() * EDGEDATA_PACKINGSIZE<<endl; 
		#endif 
		// exit(EXIT_SUCCESS);
	} // iteration end: i(NUM_PEs) end here
	// exit(EXIT_SUCCESS);
	
	// load vptrs into dram.
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0; 
	globalparams.globalparamsK.SIZE_EDGES = 0; 
	
	globalparams.globalparamsE.SIZE_EDGES = (edges_final[0].size() * EDGEDATA_PACKINGSIZE) + 1000; 
	globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + (globalparams.globalparamsE.SIZE_EDGES / EDGEDATA_PACKINGSIZE) + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsE.SIZE_VERTEXPTRS = (kvdata_range__div__vptr_shrink_ratio) + universalparams.DRAMPADD_VPTRS; 
	globalparams_t globalparamsVPTRS; globalparamsVPTRS = globalparams.globalparamsE; 
			
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"loading edges [STAGE 3]: calculating vptrs and loading into dram..."<<endl;
	#endif 
	unsigned int vptr_baseoffset = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR2_SIZE;
	for(unsigned int i=0; i<_NUM_PEs; i++){
		if(utilityobj->isbufferused(i) == false){ continue; }
		vptrs[i][vptr_baseoffset + 0].key = 0;
		for(unsigned int v_p=1; v_p<_num_vPs; v_p++){
			vptrs[i][vptr_baseoffset + v_p].key = vptrs[i][vptr_baseoffset + v_p - 1].key + edge_count_in_vpartition[i][v_p - 1]; 
		}
		for(unsigned int v_p=_num_vPs; v_p<_num_vPs + universalparams.DRAMPADD_VPTRS; v_p++){ // dummy filling...
			vptrs[i][vptr_baseoffset + v_p].key = vptrs[i][vptr_baseoffset + _num_vPs - 1].key; 
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"loading edges [STAGE 4]: calculating edge-map offsets..."<<endl;
	#endif 
	for(unsigned int i=0; i<_NUM_PEs; i++){
		if(utilityobj->isbufferused(i) == false){ continue; }
		unsigned int offset = 0;
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){
			if(debug_detail){ cout<<"loadedges:: [PE: "<<i<<", v_p: "<<v_p<<"]"<<endl; }
			for(unsigned int ll_p=0; ll_p<_num_LLPset; ll_p++){ // MAXNUM_LLPs
				edges_map[i][v_p][ll_p].offset = offset;
				offset += edges_map[i][v_p][ll_p].size;
			}
		}
	}
	// exit(EXIT_SUCCESS);
	
	// load container
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"loading edges [STAGE 5]: loading container..."<<endl;
	#endif 
	for(unsigned int i=0; i<NUM_PEs; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = utilityobj->allignhigher_FACTOR(universalparams.NUM_VERTICES, 16);
		container->edgessize[i] = edges_final[0].size() * EDGEDATA_PACKINGSIZE;
		container->runsize[i] = edges_final[0].size() * EDGEDATA_PACKINGSIZE;
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
	}
	
	// debug 
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"projected total edge count for all drams: "<<edges_final[0].size() * EDGEDATA_PACKINGSIZE * NUM_PEs<<", universalparams.NUM_EDGES: "<<universalparams.NUM_EDGES<<" ["<<(((edges_final[0].size() * EDGEDATA_PACKINGSIZE * NUM_PEs) - universalparams.NUM_EDGES) * 100) / universalparams.NUM_EDGES<<"% increase]"<<endl;
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	for(unsigned int i=0; i<_NUM_PEs; i++){ cout<<"Edge content in dram "<<i<<": "<<edges_final[i].size() * EDGEDATA_PACKINGSIZE<<endl; }
	cout<<"projected total edge count for all drams: "<<edges_final[0].size() * EDGEDATA_PACKINGSIZE * NUM_PEs<<", universalparams.NUM_EDGES: "<<universalparams.NUM_EDGES<<" ["<<(((edges_final[0].size() * EDGEDATA_PACKINGSIZE * NUM_PEs) - universalparams.NUM_EDGES) * 100) / universalparams.NUM_EDGES<<"% increase]"<<endl;
	for(unsigned int i=0; i<1; i++){ // _NUM_PEs
		cout<<"loadedges:: SEEN 0a"<<endl;
		if(utilityobj->isbufferused(i) == false){ continue; }
		cout<<"loadedges:: SEEN 0b"<<endl;
		if(debug_detail){ for(unsigned int t=0; t<16; t++){ for(unsigned int v=0; v<EDGEDATA_PACKINGSIZE; v++){ cout<<"{"<<edges_final[i][t].data[v].srcvid<<", "<<edges_final[i][t].data[v].dstvid<<"}, "; } cout<<endl;}}
		unsigned int offset = 0;
		cout<<"loadedges:: SEEN 1"<<endl;
		for(unsigned int v_p=0; v_p<8; v_p++){ // _num_vPs
			cout<<"Vertex Ptr in dram:: vptrs["<<i<<"][~ + "<<v_p<<"].key: "<<vptrs[i][vptr_baseoffset + v_p].key<<", vptrs["<<i<<"][~ + "<<v_p<<"].key_kvs: "<<vptrs[i][vptr_baseoffset + v_p].key / EDGEDATA_PACKINGSIZE<<", num edges["<<i<<"]["<<v_p<<"]: "<<edge_count_in_vpartition[i][v_p]<<endl; 
		}
		cout<<"loadedges:: SEEN 2"<<endl;
		for(unsigned int v_p=0; v_p<1; v_p++){ // _num_vPs
			for(unsigned int ll_p=0; ll_p<16; ll_p++){ // MAXNUM_LLPs, _num_LLPset
				cout<<"Edge Map in dram "<<i<<": edges_map["<<i<<"]["<<v_p<<"]["<<ll_p<<"].offset: "<<edges_map[i][v_p][ll_p].offset<<", edges_map["<<i<<"]["<<v_p<<"]["<<ll_p<<"].size: "<<edges_map[i][v_p][ll_p].size<<endl; 
			}
			for(unsigned int t=0; t<16; t++){ // MAXNUM_ACTVEDGEBLOCKS_PER_VPARTITION
				cout<<"Edgeblock Map in dram "<<i<<": edgeblock_map["<<i<<"]["<<v_p<<"]["<<t<<"].key: "<<edgeblock_map[i][v_p][t].key<<", edgeblock_map["<<i<<"]["<<v_p<<"]["<<t<<"].value: "<<edgeblock_map[i][v_p][t].value<<endl; 
			}
		}
	}
	#endif 
	
	// clear unused vectors and return
	for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ edgesin_srcvp[v_p].clear(); } // clear 
	for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ edgesin_srcvp_lldstvp[ll_p].clear(); } // clear
	for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ for(unsigned int p=0; p<universalparams.NUM_PARTITIONS; p++){ edgesin_srcvp_lldstvp_srcv2p[ll_p][p].clear(); }} 
	
	int A = edges_final[0].size(); int B = edges_final[NUMCOMPUTEUNITS_SLR2].size(); int C = edges_final[NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1].size(); 
	if(abs(A - B) > 3000000 || abs(A - C) > 3000000 || abs(B - C) > 3000000 || abs(B - A) > 3000000){ cout<<"loadedges: ERROR 45. Load Balancing Failed. EXITING..."<<endl; exit(EXIT_FAILURE); }
	// exit(EXIT_SUCCESS);
	return globalparams;
}





