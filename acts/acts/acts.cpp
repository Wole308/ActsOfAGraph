#include "../include/actscommon.h"
#include "../../include/common.h"
#ifndef FPGA_IMPL
#include "../actsutility/actsutility.h"
#endif 
#include "acts.h"
using namespace std;

unsigned int GVcurrentLOP;
unsigned int GVsource_partition;
unsigned int GVi;

#ifndef FPGA_IMPL
acts::acts(){
	actsutilityobj = new actsutility();
}
acts::~acts(){}
#endif

unsigned int allignhigher_KV(unsigned int val){
	unsigned int fac = (val + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
unsigned int allignlower_KV(unsigned int val){
	unsigned int fac = val / VECTOR_SIZE;
	return (fac * VECTOR_SIZE);
}
unsigned int ceildiv(unsigned int val1, unsigned int val2){
	return (val1 + (val2 - 1)) / val2;
}	
unsigned int max(unsigned int val1, unsigned int val2){
	if(val1 > val2){ return val1; }
	else { return val2; }
}
unsigned int min(unsigned int A, unsigned int B){
	#pragma HLS INLINE
	if(A<B){ return A; }
	else { return B; }
}
unsigned int hsub(unsigned int A, unsigned int B){
	#pragma HLS INLINE
	if(A < B){ return 0; }
	else { return A - B; }
}
void checkandforce(unsigned int data, unsigned int upper_bound, unsigned int * datatoforce, unsigned int forceval){
	#pragma HLS INLINE
	#ifdef BC_ALGORITHM // FIXME. REMOVEME.
	if(data >= upper_bound){ *datatoforce = forceval; }
	return;
	#else 
	return;
	#endif 
}
int WithinValidRange(unsigned int val1, unsigned int val2){
	#pragma HLS INLINE
	if(val1 < val2){ return 1; }
	else { return 0; }
}
buffer_type getpartitionwritesz(buffer_type realsize_kvs, buffer_type bramoffset_kvs){
	#pragma HLS INLINE
	buffer_type size_kvs;
	
	#ifdef WRITEPARTITION_TYPE1
	#ifdef FPGA_IMPL
	size_kvs = OPTIMALSIZE;
	#else 
	size_kvs = realsize_kvs;
	#endif
	#endif 
	
	#ifdef WRITEPARTITION_TYPE2
	#ifdef ENABLE_APPROXIMATEPARTITIONWRITES
	size_kvs = OPTIMALSIZE;
	#ifdef SW 
	if((bramoffset_kvs + size_kvs) >= PADDEDDESTBUFFER_SIZE){ size_kvs = PADDEDDESTBUFFER_SIZE - bramoffset_kvs - 1; } 
	#endif 
	#else
	size_kvs = realsize_kvs;
	#endif
	#endif 
	
	#ifdef WRITEPARTITION_TYPE3
	#ifdef ENABLE_APPROXIMATEPARTITIONWRITES
	if(realsize_kvs >= 0*PADSKIP && realsize_kvs < 1*PADSKIP){
		size_kvs = 1*PADSKIP;
	} else if(realsize_kvs >= 1*PADSKIP && realsize_kvs < 2*PADSKIP){
		size_kvs = 2*PADSKIP;
	} else if(realsize_kvs >= 2*PADSKIP && realsize_kvs < 3*PADSKIP){
		size_kvs = 3*PADSKIP;
	} else if(realsize_kvs >= 3*PADSKIP && realsize_kvs < 4*PADSKIP){
		size_kvs = 4*PADSKIP;
	} else if(realsize_kvs >= 4*PADSKIP && realsize_kvs < 5*PADSKIP){
		size_kvs = 5*PADSKIP;
	} else if(realsize_kvs >= 5*PADSKIP && realsize_kvs < 6*PADSKIP){
		size_kvs = 6*PADSKIP;
	} else if(realsize_kvs >= 6*PADSKIP && realsize_kvs < 7*PADSKIP){
		size_kvs = 7*PADSKIP;
	} else if(realsize_kvs >= 7*PADSKIP && realsize_kvs < 8*PADSKIP){
		size_kvs = 8*PADSKIP;
	} else if(realsize_kvs >= 8*PADSKIP && realsize_kvs < 9*PADSKIP){
		size_kvs = 9*PADSKIP;
	} else if(realsize_kvs >= 9*PADSKIP && realsize_kvs < 10*PADSKIP){
		size_kvs = 10*PADSKIP;
	} else if(realsize_kvs >= 10*PADSKIP && realsize_kvs < 11*PADSKIP){
		size_kvs = 11*PADSKIP;
	} else if(realsize_kvs >= 11*PADSKIP && realsize_kvs < 12*PADSKIP){
		size_kvs = 12*PADSKIP;
	} else if(realsize_kvs >= 12*PADSKIP && realsize_kvs < 13*PADSKIP){
		size_kvs = 13*PADSKIP;
	} else if(realsize_kvs >= 13*PADSKIP && realsize_kvs < 14*PADSKIP){
		size_kvs = 14*PADSKIP;
	} else if(realsize_kvs >= 14*PADSKIP && realsize_kvs < 15*PADSKIP){
		size_kvs = 15*PADSKIP;
	} else if(realsize_kvs >= 15*PADSKIP && realsize_kvs < 16*PADSKIP){
		size_kvs = 16*PADSKIP;
	} else if(realsize_kvs >= 16*PADSKIP && realsize_kvs < 17*PADSKIP){
		size_kvs = 17*PADSKIP;
	} else if(realsize_kvs >= 17*PADSKIP && realsize_kvs < 18*PADSKIP){
		size_kvs = 18*PADSKIP;
	} else if(realsize_kvs >= 18*PADSKIP && realsize_kvs < 19*PADSKIP){
		size_kvs = 19*PADSKIP;
	} else if(realsize_kvs >= 19*PADSKIP && realsize_kvs < 20*PADSKIP){
		size_kvs = 20*PADSKIP;
	} else if(realsize_kvs >= 20*PADSKIP && realsize_kvs < 21*PADSKIP){
		size_kvs = 21*PADSKIP;
	} else if(realsize_kvs >= 21*PADSKIP && realsize_kvs < 22*PADSKIP){
		size_kvs = 22*PADSKIP;
	} else if(realsize_kvs >= 22*PADSKIP && realsize_kvs < 23*PADSKIP){
		size_kvs = 23*PADSKIP;
	} else if(realsize_kvs >= 23*PADSKIP && realsize_kvs < 24*PADSKIP){
		size_kvs = 24*PADSKIP;
	} else {
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", OPTIMALSIZE: "<<OPTIMALSIZE<<", PADSKIP: "<<PADSKIP<<endl; 
		exit(EXIT_FAILURE);
		#endif
		size_kvs = PADDEDDESTBUFFER_SIZE;
	}
	#ifdef SW 
	if((bramoffset_kvs + size_kvs) >= PADDEDDESTBUFFER_SIZE){ size_kvs = PADDEDDESTBUFFER_SIZE - bramoffset_kvs - 1; } 
	#endif 
	#else 
	size_kvs = realsize_kvs;
	#endif
	#endif 
	
	#ifdef WRITEPARTITION_TYPE4
	#ifdef ENABLE_APPROXIMATEPARTITIONWRITES
	if(realsize_kvs >= 0*PADSKIP && realsize_kvs < 1*PADSKIP){
		size_kvs = 1*PADSKIP;
	} else if(realsize_kvs >= 1*PADSKIP && realsize_kvs < 2*PADSKIP){
		size_kvs = 2*PADSKIP;
	} else if(realsize_kvs >= 2*PADSKIP && realsize_kvs < 3*PADSKIP){
		size_kvs = 3*PADSKIP;
	} else if(realsize_kvs >= 3*PADSKIP && realsize_kvs < 4*PADSKIP){
		size_kvs = 4*PADSKIP;
	} else if(realsize_kvs >= 4*PADSKIP && realsize_kvs < 5*PADSKIP){
		size_kvs = 5*PADSKIP;
	} else if(realsize_kvs >= 5*PADSKIP && realsize_kvs < 6*PADSKIP){
		size_kvs = 6*PADSKIP;
	} else if(realsize_kvs >= 6*PADSKIP && realsize_kvs < 7*PADSKIP){
		size_kvs = 7*PADSKIP;
	} else if(realsize_kvs >= 7*PADSKIP && realsize_kvs < 8*PADSKIP){
		size_kvs = 8*PADSKIP;
	} else if(realsize_kvs >= 8*PADSKIP && realsize_kvs < 9*PADSKIP){
		size_kvs = 9*PADSKIP;
	} else {
		#ifdef _DEBUGMODE_CHECKS2
		cout<<"WARNING:getpartitionwritesz: should not get here. something might be wrong. realsize_kvs: "<<realsize_kvs<<", size_kvs: "<<size_kvs<<", OPTIMALSIZE: "<<OPTIMALSIZE<<", PADSKIP: "<<PADSKIP<<endl; 
		exit(EXIT_FAILURE);
		#endif
		size_kvs = PADDEDDESTBUFFER_SIZE;
	}
	#ifdef SW 
	if((bramoffset_kvs + size_kvs) >= PADDEDDESTBUFFER_SIZE){ size_kvs = PADDEDDESTBUFFER_SIZE - bramoffset_kvs - 1; } 
	#endif 
	#else 
	size_kvs = realsize_kvs;
	#endif
	#endif
	return size_kvs;
}

unsigned int getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit){
	partition_type val = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * currentLOP)));
	partition_type hashedval = val;
	
	#ifndef ENABLE_PERFECTACCURACY
	if(hashedval >= NUM_PARTITIONS){ hashedval = (((1 << NUM_PARTITIONS_POW) - 1) & (val >> (1 - 1))); } // FIXME. REMOVEME. PERFECTIONTEST.
	#endif

	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::getpartition 35", hashedval, NUM_PARTITIONS, keyvalue.key, upperlimit, currentLOP);
	#endif
	return hashedval;
}
keyvalue_t getkeyvalue(uint512_dt * buffer, batch_type addr, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	if(addr >= (maxaddr_kvs * VECTOR_SIZE)){ addr = 0; } // FIXME. FORCEDFINISH
	keyvalue_t keyvalue;
	batch_type idy = addr / VECTOR_SIZE;
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	if(idy >= maxaddr_kvs){ cout<<"getkeyvalue:: ERROR 45. addr: "<<addr<<", idy: "<<idy<<", idx: "<<idx<<endl; exit(EXIT_FAILURE); }
	actsutilityobj->checkoutofbounds("acts::getkeyvalue 35", idy, maxaddr_kvs, NAp, NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
if(idx==0){	
		keyvalue.key = buffer[idy].range(31, 0);
		keyvalue.value = buffer[idy].range(63, 32);
	}
else if(idx==1){	
		keyvalue.key = buffer[idy].range(95, 64);
		keyvalue.value = buffer[idy].range(127, 96);
	}
else if(idx==2){	
		keyvalue.key = buffer[idy].range(159, 128);
		keyvalue.value = buffer[idy].range(191, 160);
	}
else if(idx==3){	
		keyvalue.key = buffer[idy].range(223, 192);
		keyvalue.value = buffer[idy].range(255, 224);
	}
else if(idx==4){	
		keyvalue.key = buffer[idy].range(287, 256);
		keyvalue.value = buffer[idy].range(319, 288);
	}
else if(idx==5){	
		keyvalue.key = buffer[idy].range(351, 320);
		keyvalue.value = buffer[idy].range(383, 352);
	}
else if(idx==6){	
		keyvalue.key = buffer[idy].range(415, 384);
		keyvalue.value = buffer[idy].range(447, 416);
	}
else if(idx==7){	
		keyvalue.key = buffer[idy].range(479, 448);
		keyvalue.value = buffer[idy].range(511, 480);
	}
	else {}
	#else
	keyvalue = buffer[idy].data[idx];
	#endif
	return keyvalue;
}
keyvalue_t getkeyvalue(uint512_dt * Vtemp, vector_type idx){
	keyvalue_t keyvalue;
	#ifdef _WIDEWORD
if(idx==0){	
		keyvalue.key = Vtemp->range(31, 0);
		keyvalue.value = Vtemp->range(63, 32);
	}
else if(idx==1){	
		keyvalue.key = Vtemp->range(95, 64);
		keyvalue.value = Vtemp->range(127, 96);
	}
else if(idx==2){	
		keyvalue.key = Vtemp->range(159, 128);
		keyvalue.value = Vtemp->range(191, 160);
	}
else if(idx==3){	
		keyvalue.key = Vtemp->range(223, 192);
		keyvalue.value = Vtemp->range(255, 224);
	}
else if(idx==4){	
		keyvalue.key = Vtemp->range(287, 256);
		keyvalue.value = Vtemp->range(319, 288);
	}
else if(idx==5){	
		keyvalue.key = Vtemp->range(351, 320);
		keyvalue.value = Vtemp->range(383, 352);
	}
else if(idx==6){	
		keyvalue.key = Vtemp->range(415, 384);
		keyvalue.value = Vtemp->range(447, 416);
	}
else if(idx==7){	
		keyvalue.key = Vtemp->range(479, 448);
		keyvalue.value = Vtemp->range(511, 480);
	}
	else {}
	#else
	keyvalue = Vtemp->data[idx];
	#endif
	return keyvalue;
}
void setkeyvalue(uint512_dt * buffer, batch_type offset, batch_type addr, keyvalue_t keyvalue, batch_type maxaddr_kvs){
	#pragma HLS INLINE
	if(addr >= (maxaddr_kvs * VECTOR_SIZE)){ addr = 0; } // FIXME. FORCEDFINISH
	batch_type idy = (addr / VECTOR_SIZE);
	vector_type idx = addr % VECTOR_SIZE;
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::setkeyvalue 35", idy, maxaddr_kvs, addr, NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
if(idx==0){	
		buffer[offset + idy].range(31, 0) = keyvalue.key;
		buffer[offset + idy].range(63, 32) = keyvalue.value;
	}
else if(idx==1){	
		buffer[offset + idy].range(95, 64) = keyvalue.key;
		buffer[offset + idy].range(127, 96) = keyvalue.value;
	}
else if(idx==2){	
		buffer[offset + idy].range(159, 128) = keyvalue.key;
		buffer[offset + idy].range(191, 160) = keyvalue.value;
	}
else if(idx==3){	
		buffer[offset + idy].range(223, 192) = keyvalue.key;
		buffer[offset + idy].range(255, 224) = keyvalue.value;
	}
else if(idx==4){	
		buffer[offset + idy].range(287, 256) = keyvalue.key;
		buffer[offset + idy].range(319, 288) = keyvalue.value;
	}
else if(idx==5){	
		buffer[offset + idy].range(351, 320) = keyvalue.key;
		buffer[offset + idy].range(383, 352) = keyvalue.value;
	}
else if(idx==6){	
		buffer[offset + idy].range(415, 384) = keyvalue.key;
		buffer[offset + idy].range(447, 416) = keyvalue.value;
	}
else if(idx==7){	
		buffer[offset + idy].range(479, 448) = keyvalue.key;
		buffer[offset + idy].range(511, 480) = keyvalue.value;
	}
	else {}
	#else
	buffer[offset + idy].data[idx] = keyvalue;
	#endif
	return;
}
void setkeyvalue(uint512_dt * Vtemp, vector_type idx, keyvalue_t keyvalue){
	// #pragma HLS INLINE OFF
	#ifdef _WIDEWORD
if(idx==0){	
		Vtemp->range(31, 0) = keyvalue.key;
		Vtemp->range(63, 32) = keyvalue.value;
	}
else if(idx==1){	
		Vtemp->range(95, 64) = keyvalue.key;
		Vtemp->range(127, 96) = keyvalue.value;
	}
else if(idx==2){	
		Vtemp->range(159, 128) = keyvalue.key;
		Vtemp->range(191, 160) = keyvalue.value;
	}
else if(idx==3){	
		Vtemp->range(223, 192) = keyvalue.key;
		Vtemp->range(255, 224) = keyvalue.value;
	}
else if(idx==4){	
		Vtemp->range(287, 256) = keyvalue.key;
		Vtemp->range(319, 288) = keyvalue.value;
	}
else if(idx==5){	
		Vtemp->range(351, 320) = keyvalue.key;
		Vtemp->range(383, 352) = keyvalue.value;
	}
else if(idx==6){	
		Vtemp->range(415, 384) = keyvalue.key;
		Vtemp->range(447, 416) = keyvalue.value;
	}
else if(idx==7){	
		Vtemp->range(479, 448) = keyvalue.key;
		Vtemp->range(511, 480) = keyvalue.value;
	}
	else {}
	#else
	Vtemp->data[idx] = keyvalue;
	#endif
	return;
}
void setkeyvalues( uint512_dt * Vtemp00,  uint512_dt * Vtemp01,  uint512_dt * Vtemp02,  uint512_dt * Vtemp03,  uint512_dt * Vtemp04,  uint512_dt * Vtemp05,  uint512_dt * Vtemp06,  uint512_dt * Vtemp07,   keyvalue_t keyvalue00,  keyvalue_t keyvalue01,  keyvalue_t keyvalue02,  keyvalue_t keyvalue03,  keyvalue_t keyvalue04,  keyvalue_t keyvalue05,  keyvalue_t keyvalue06,  keyvalue_t keyvalue07,  vector_type idx){
	#pragma HLS INLINE OFF
	#ifdef _WIDEWORD
if(idx==0){
		Vtemp00->range(31, 0) = keyvalue00.key;
		Vtemp00->range(63, 32) = keyvalue00.value;
		Vtemp01->range(31, 0) = keyvalue01.key;
		Vtemp01->range(63, 32) = keyvalue01.value;
		Vtemp02->range(31, 0) = keyvalue02.key;
		Vtemp02->range(63, 32) = keyvalue02.value;
		Vtemp03->range(31, 0) = keyvalue03.key;
		Vtemp03->range(63, 32) = keyvalue03.value;
		Vtemp04->range(31, 0) = keyvalue04.key;
		Vtemp04->range(63, 32) = keyvalue04.value;
		Vtemp05->range(31, 0) = keyvalue05.key;
		Vtemp05->range(63, 32) = keyvalue05.value;
		Vtemp06->range(31, 0) = keyvalue06.key;
		Vtemp06->range(63, 32) = keyvalue06.value;
		Vtemp07->range(31, 0) = keyvalue07.key;
		Vtemp07->range(63, 32) = keyvalue07.value;
	}
else if(idx==1){
		Vtemp00->range(95, 64) = keyvalue00.key;
		Vtemp00->range(127, 96) = keyvalue00.value;
		Vtemp01->range(95, 64) = keyvalue01.key;
		Vtemp01->range(127, 96) = keyvalue01.value;
		Vtemp02->range(95, 64) = keyvalue02.key;
		Vtemp02->range(127, 96) = keyvalue02.value;
		Vtemp03->range(95, 64) = keyvalue03.key;
		Vtemp03->range(127, 96) = keyvalue03.value;
		Vtemp04->range(95, 64) = keyvalue04.key;
		Vtemp04->range(127, 96) = keyvalue04.value;
		Vtemp05->range(95, 64) = keyvalue05.key;
		Vtemp05->range(127, 96) = keyvalue05.value;
		Vtemp06->range(95, 64) = keyvalue06.key;
		Vtemp06->range(127, 96) = keyvalue06.value;
		Vtemp07->range(95, 64) = keyvalue07.key;
		Vtemp07->range(127, 96) = keyvalue07.value;
	}
else if(idx==2){
		Vtemp00->range(159, 128) = keyvalue00.key;
		Vtemp00->range(191, 160) = keyvalue00.value;
		Vtemp01->range(159, 128) = keyvalue01.key;
		Vtemp01->range(191, 160) = keyvalue01.value;
		Vtemp02->range(159, 128) = keyvalue02.key;
		Vtemp02->range(191, 160) = keyvalue02.value;
		Vtemp03->range(159, 128) = keyvalue03.key;
		Vtemp03->range(191, 160) = keyvalue03.value;
		Vtemp04->range(159, 128) = keyvalue04.key;
		Vtemp04->range(191, 160) = keyvalue04.value;
		Vtemp05->range(159, 128) = keyvalue05.key;
		Vtemp05->range(191, 160) = keyvalue05.value;
		Vtemp06->range(159, 128) = keyvalue06.key;
		Vtemp06->range(191, 160) = keyvalue06.value;
		Vtemp07->range(159, 128) = keyvalue07.key;
		Vtemp07->range(191, 160) = keyvalue07.value;
	}
else if(idx==3){
		Vtemp00->range(223, 192) = keyvalue00.key;
		Vtemp00->range(255, 224) = keyvalue00.value;
		Vtemp01->range(223, 192) = keyvalue01.key;
		Vtemp01->range(255, 224) = keyvalue01.value;
		Vtemp02->range(223, 192) = keyvalue02.key;
		Vtemp02->range(255, 224) = keyvalue02.value;
		Vtemp03->range(223, 192) = keyvalue03.key;
		Vtemp03->range(255, 224) = keyvalue03.value;
		Vtemp04->range(223, 192) = keyvalue04.key;
		Vtemp04->range(255, 224) = keyvalue04.value;
		Vtemp05->range(223, 192) = keyvalue05.key;
		Vtemp05->range(255, 224) = keyvalue05.value;
		Vtemp06->range(223, 192) = keyvalue06.key;
		Vtemp06->range(255, 224) = keyvalue06.value;
		Vtemp07->range(223, 192) = keyvalue07.key;
		Vtemp07->range(255, 224) = keyvalue07.value;
	}
else if(idx==4){
		Vtemp00->range(287, 256) = keyvalue00.key;
		Vtemp00->range(319, 288) = keyvalue00.value;
		Vtemp01->range(287, 256) = keyvalue01.key;
		Vtemp01->range(319, 288) = keyvalue01.value;
		Vtemp02->range(287, 256) = keyvalue02.key;
		Vtemp02->range(319, 288) = keyvalue02.value;
		Vtemp03->range(287, 256) = keyvalue03.key;
		Vtemp03->range(319, 288) = keyvalue03.value;
		Vtemp04->range(287, 256) = keyvalue04.key;
		Vtemp04->range(319, 288) = keyvalue04.value;
		Vtemp05->range(287, 256) = keyvalue05.key;
		Vtemp05->range(319, 288) = keyvalue05.value;
		Vtemp06->range(287, 256) = keyvalue06.key;
		Vtemp06->range(319, 288) = keyvalue06.value;
		Vtemp07->range(287, 256) = keyvalue07.key;
		Vtemp07->range(319, 288) = keyvalue07.value;
	}
else if(idx==5){
		Vtemp00->range(351, 320) = keyvalue00.key;
		Vtemp00->range(383, 352) = keyvalue00.value;
		Vtemp01->range(351, 320) = keyvalue01.key;
		Vtemp01->range(383, 352) = keyvalue01.value;
		Vtemp02->range(351, 320) = keyvalue02.key;
		Vtemp02->range(383, 352) = keyvalue02.value;
		Vtemp03->range(351, 320) = keyvalue03.key;
		Vtemp03->range(383, 352) = keyvalue03.value;
		Vtemp04->range(351, 320) = keyvalue04.key;
		Vtemp04->range(383, 352) = keyvalue04.value;
		Vtemp05->range(351, 320) = keyvalue05.key;
		Vtemp05->range(383, 352) = keyvalue05.value;
		Vtemp06->range(351, 320) = keyvalue06.key;
		Vtemp06->range(383, 352) = keyvalue06.value;
		Vtemp07->range(351, 320) = keyvalue07.key;
		Vtemp07->range(383, 352) = keyvalue07.value;
	}
else if(idx==6){
		Vtemp00->range(415, 384) = keyvalue00.key;
		Vtemp00->range(447, 416) = keyvalue00.value;
		Vtemp01->range(415, 384) = keyvalue01.key;
		Vtemp01->range(447, 416) = keyvalue01.value;
		Vtemp02->range(415, 384) = keyvalue02.key;
		Vtemp02->range(447, 416) = keyvalue02.value;
		Vtemp03->range(415, 384) = keyvalue03.key;
		Vtemp03->range(447, 416) = keyvalue03.value;
		Vtemp04->range(415, 384) = keyvalue04.key;
		Vtemp04->range(447, 416) = keyvalue04.value;
		Vtemp05->range(415, 384) = keyvalue05.key;
		Vtemp05->range(447, 416) = keyvalue05.value;
		Vtemp06->range(415, 384) = keyvalue06.key;
		Vtemp06->range(447, 416) = keyvalue06.value;
		Vtemp07->range(415, 384) = keyvalue07.key;
		Vtemp07->range(447, 416) = keyvalue07.value;
	}
else if(idx==7){
		Vtemp00->range(479, 448) = keyvalue00.key;
		Vtemp00->range(511, 480) = keyvalue00.value;
		Vtemp01->range(479, 448) = keyvalue01.key;
		Vtemp01->range(511, 480) = keyvalue01.value;
		Vtemp02->range(479, 448) = keyvalue02.key;
		Vtemp02->range(511, 480) = keyvalue02.value;
		Vtemp03->range(479, 448) = keyvalue03.key;
		Vtemp03->range(511, 480) = keyvalue03.value;
		Vtemp04->range(479, 448) = keyvalue04.key;
		Vtemp04->range(511, 480) = keyvalue04.value;
		Vtemp05->range(479, 448) = keyvalue05.key;
		Vtemp05->range(511, 480) = keyvalue05.value;
		Vtemp06->range(479, 448) = keyvalue06.key;
		Vtemp06->range(511, 480) = keyvalue06.value;
		Vtemp07->range(479, 448) = keyvalue07.key;
		Vtemp07->range(511, 480) = keyvalue07.value;
	}
	else {}
	#else
	Vtemp00->data[idx] = keyvalue00;
	Vtemp01->data[idx] = keyvalue01;
	Vtemp02->data[idx] = keyvalue02;
	Vtemp03->data[idx] = keyvalue03;
	Vtemp04->data[idx] = keyvalue04;
	Vtemp05->data[idx] = keyvalue05;
	Vtemp06->data[idx] = keyvalue06;
	Vtemp07->data[idx] = keyvalue07;
	#endif
	return;
}
unsigned int getvaluecount(keyvalue_t * keyvalues, unsigned int size){
	unsigned int totalnumkeyvalues = 0;
	for(unsigned int p=0; p<size; p++){ totalnumkeyvalues += keyvalues[p].value; }
	return totalnumkeyvalues;
}

void resetkeyandvalues(keyvalue_t * buffer, unsigned int size){
	#pragma HLS INLINE
	RESETKEYVANDVALUES_LOOP: for(partition_type p=0; p<size; p++){ buffer[p].key = 0; buffer[p].value = 0; }
}
void resetvalues(keyvalue_t * buffer, unsigned int size){
	#pragma HLS INLINE
	RESETVALUES_LOOP: for(partition_type p=0; p<size; p++){ buffer[p].value = 0; }
}
void copykeys(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	#pragma HLS INLINE
	COPYKEYS_LOOP: for(partition_type p=0; p<size; p++){ buffer2[p].key = buffer1[p].key; }
}
void actscopy(keyvalue_t * buffer1, keyvalue_t * buffer2, unsigned int size){
	#pragma HLS INLINE
	COPYKEYS_LOOP: for(partition_type p=0; p<size; p++){ buffer2[p] = buffer1[p]; }
}

buffer_type getactivesize(buffer_type i, batch_type i_end, buffer_type buffersize){	
	#pragma HLS INLINE
	buffer_type activesize = 0;
	buffer_type next_i = i + buffersize;
	if(i >= i_end){ activesize = 0; }
	else if ((i < i_end) && (next_i < i_end)) { activesize = buffersize; }
	else if ((i < i_end) && (next_i >= i_end)) { activesize = i_end - i; }
	else {
		i = 0; 
		#ifdef _DEBUGMODE_CHECKS
		cout<<"getactivesize:: should NEVER get here: activesize: "<<activesize<<", i: "<<i<<", next_i: "<<next_i<<", i_end: "<<i_end<<endl;
		exit(EXIT_FAILURE);
		#endif
	}
	#ifdef _DEBUGMODE_KERNELPRINTSX
	actsutilityobj->print4("getactivesize: i", "i_end", "buffersize", "activesize", i, i_end, buffersize, activesize);
	#endif
	return activesize;
}
batch_type getAddr(batch_type addr){
	#pragma HLS INLINE
	return addr;
}
batch_type getmessagesAddr(batch_type addr){
	#pragma HLS INLINE
	return BASEOFFSET_MESSAGESDRAM + addr;
}

unsigned int getsourceskipsize(unsigned int currentLOP, unsigned int param){
	#pragma HLS INLINE
	unsigned int treedepth = TREE_DEPTH; // NB: for debugging purposes only
	unsigned int result = param;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		result = result / NUM_PARTITIONS;
	}
	return result;
}
unsigned int getdestskipsize(unsigned int currentLOP, unsigned int param){
	#pragma HLS INLINE
	unsigned int treedepthloop_tripcount = TREE_DEPTH; // NB: for debugging purposes only
	unsigned int result = param / NUM_PARTITIONS;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepthloop_tripcount avg=treedepthloop_tripcount
		result = result / NUM_PARTITIONS;
	}
	return result;
}
unsigned int get_num_source_partitions(unsigned int currentLOP){
	#pragma HLS INLINE
	unsigned int treedepth = TREE_DEPTH;
	unsigned int pow = 1;
	for(unsigned int i=0; i<(currentLOP-1); i++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=treedepth avg=treedepth
		pow = pow * NUM_PARTITIONS;
	}
	return pow;
}

void calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int skipspacing){
	#pragma HLS INLINE
	capsule[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){ capsule[p].key = allignhigher_KV(capsule[p-1].key + capsule[p-1].value + skipspacing); }
	return;
}
void calculatemanyoffsets(keyvalue_t capsule0[NUM_PARTITIONS], unsigned int skipspacing){
	#pragma HLS INLINE
	capsule0[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){
		capsule0[p].key = allignhigher_KV(capsule0[p-1].key + capsule0[p-1].value + skipspacing);
	}
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::calculatemanyoffsets 34", capsule0[NUM_PARTITIONS-1].key + capsule0[NUM_PARTITIONS-1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, capsule0[NUM_PARTITIONS-1].key, capsule0[NUM_PARTITIONS-1].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE);
	#endif
	return;
}
void calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int offset, unsigned int skipspacing){
	#pragma HLS INLINE
	capsule[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){ capsule[p].key = allignhigher_KV(capsule[p-1].key + capsule[p-1].value + skipspacing); }
	PREPARECAPSULE_LOOP2: for (partition_type p = 0; p<NUM_PARTITIONS; p++){ capsule[p].key += offset; }
	return;
}
void calculateoffsets(keyvalue_t capsule[NUM_PARTITIONS], unsigned int offset, unsigned int skipspacing[NUM_PARTITIONS]){
	#pragma HLS INLINE
	capsule[0].key = 0;
	PREPARECAPSULE_LOOP1: for (partition_type p = 1; p<NUM_PARTITIONS; p++){ capsule[p].key = allignhigher_KV(capsule[p-1].key + capsule[p-1].value + skipspacing[p-1]); }
	PREPARECAPSULE_LOOP2: for (partition_type p = 0; p<NUM_PARTITIONS; p++){ capsule[p].key += offset; }
	return;
}

void loadclopparams(globalparams_t globalparams, clopparams_t * llopparams, unsigned int currentLOP){
	#pragma HLS INLINE
	if((currentLOP % 2) == 1){ llopparams->baseaddr_worksourcekvs_kvs = BASEOFFSET_KVDRAM_KVS; llopparams->baseaddr_workdestkvs_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; }
	else { llopparams->baseaddr_worksourcekvs_kvs = BASEOFFSET_KVDRAMWORKSPACE_KVS; llopparams->baseaddr_workdestkvs_kvs = BASEOFFSET_KVDRAM_KVS; }	
	llopparams->baseaddr_capsule_kvs = BASEOFFSET_CAPSULES_KVS;
	llopparams->baseaddr_stats = BASEOFFSET_STATSDRAM;
	llopparams->num_source_partitions = get_num_source_partitions(currentLOP);
	llopparams->nextsourceoffset_partition = 0;
	llopparams->nextdestoffset_partition = 0;
	llopparams->nextsourceoffset_kv = 0;
	llopparams->nextdestoffset_kv = 0;
	llopparams->sourceskipsize_partition = getsourceskipsize(currentLOP, globalparams.LLOPnumpartitions);
	llopparams->destskipsize_partition = getdestskipsize(currentLOP, globalparams.LLOPnumpartitions);
	llopparams->sourceskipsize_kv = getsourceskipsize(currentLOP, BATCH_RANGE);
	llopparams->destskipsize_kv = getdestskipsize(currentLOP, BATCH_RANGE);
	llopparams->destpartitionrange = (1 << (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * currentLOP)));
	llopparams->currentLOP = currentLOP;
	llopparams->upperlimit = 0; // THEISSUE.
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	actsutilityobj->print1("loadclopparams:: llopparams->baseaddr_worksourcekvs_kvs", llopparams->baseaddr_worksourcekvs_kvs);
	actsutilityobj->print1("loadclopparams:: llopparams->baseaddr_workdestkvs_kvs", llopparams->baseaddr_workdestkvs_kvs);
	actsutilityobj->print1("loadclopparams:: llopparams->baseaddr_capsule_kvs", llopparams->baseaddr_capsule_kvs);
	actsutilityobj->print1("loadclopparams:: llopparams->baseaddr_stats", llopparams->baseaddr_stats);
	actsutilityobj->print1("loadclopparams:: llopparams->num_source_partitions", llopparams->num_source_partitions);
	actsutilityobj->print1("loadclopparams:: llopparams->nextsourceoffset_partition", llopparams->nextsourceoffset_partition);
	actsutilityobj->print1("loadclopparams:: llopparams->nextdestoffset_partition", llopparams->nextdestoffset_partition);
	actsutilityobj->print1("loadclopparams:: llopparams->nextsourceoffset_kv", llopparams->nextsourceoffset_kv);
	actsutilityobj->print1("loadclopparams:: llopparams->nextdestoffset_kv", llopparams->nextdestoffset_kv);
	actsutilityobj->print1("loadclopparams:: llopparams->sourceskipsize_partition", llopparams->sourceskipsize_partition);
	actsutilityobj->print1("loadclopparams:: llopparams->destskipsize_partition", llopparams->destskipsize_partition);
	actsutilityobj->print1("loadclopparams:: llopparams->sourceskipsize_kv", llopparams->sourceskipsize_kv);
	actsutilityobj->print1("loadclopparams:: llopparams->destskipsize_kv", llopparams->destskipsize_kv);
	actsutilityobj->print1("loadclopparams:: llopparams->destpartitionrange", llopparams->destpartitionrange);
	actsutilityobj->print1("loadclopparams:: llopparams->currentLOP", llopparams->currentLOP);
	actsutilityobj->print1("loadclopparams:: llopparams->upperlimit", llopparams->upperlimit);
	#endif
	return;
}
value_t reducefunc(keyy_t vid, value_t value, value_t edgeval, unsigned int GraphIter){
	#pragma HLS INLINE
	value_t ret = 0;
	#ifdef PR_ALGORITHM
	ret = value + edgeval;
	#elif defined(BFS_ALGORITHM)
	ret = min(value, GraphIter);
	#elif defined(BC_ALGORITHM)
	ret = min(value, GraphIter);
	#endif
	return ret;
}
void getchunksize(buffer_type * chunk_size, buffer_type buffer_size, travstate_t travstate, unsigned int localoffset){
	#pragma HLS INLINE
	*chunk_size = buffer_size;
	batch_type i = travstate.i_kvs + localoffset;
	if (i > travstate.end_kvs){ *chunk_size = 0; }
	else if ((i + buffer_size) > travstate.end_kvs){ *chunk_size = travstate.end_kvs - i; }
	else {}
	checkandforce(*chunk_size, buffer_size + 1, chunk_size, 0);
	checkandforce(i + *chunk_size, PADDEDKVSOURCEDRAMSZ_KVS, chunk_size, 0);
	checkandforce(i + *chunk_size, PADDEDKVSOURCEDRAMSZ_KVS, &i, 0);
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("getchunksize 35", *chunk_size, buffer_size + 1, *chunk_size, buffer_size, NAp);
	actsutilityobj->checkoutofbounds("getchunksize 36", i + *chunk_size, PADDEDKVSOURCEDRAMSZ_KVS + 1, i, *chunk_size, PADDEDKVSOURCEDRAMSZ_KVS);
	#endif
	return;
}
int IsReducePhase(unsigned int currentLOP, unsigned int treedepth, globalparams_t globalparams){
	if((currentLOP == (treedepth + 1)) && (globalparams.reducecommand == ON)){ return 1; }
	return 0;
}
void shutdownpartitionparams(partitionparams_t * partitionparams){
	#pragma HLS INLINE
	partitionparams->begin_kvs = 0;
	partitionparams->size_kvs = 0;
	partitionparams->end_kvs = 0;
	return;
}

void resetcapsules(keyvalue_t capsule0[NUM_PARTITIONS],  int enableresetdebugger){
	#pragma HLS INLINE 
	for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		capsule0[p].key = 0; capsule0[p].value = 0;
	}
	return;
}
#ifdef APPENDINVALIDS_TYPE1
void appendinvalids0(unsigned int workerID  ,uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE]  ,keyvalue_t capsule0[NUM_PARTITIONS]){				
	int analysis_appendinvalids = VECTOR_SIZE;
	
	// append INVALID values (edge conditions)
	keyvalue_t NApKV; NApKV.key = INVALIDDATA; NApKV.value = INVALIDDATA;
	SAVEPARTITIONS_RESOLVEEDGECONDITIONS_LOOP: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type ovsize0 = allignhigher_KV(capsule0[p].value) - capsule0[p].value;
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savepartitions_countinvalids(ovsize0);
		#endif 
		
		for(vector_type v=capsule0[p].key + capsule0[p].value; v<capsule0[p].key + capsule0[p].value + ovsize0; v++){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_appendinvalids avg=analysis_appendinvalids
			setkeyvalue(destbuffer0, 0, v, NApKV, PADDEDDESTBUFFER_SIZE);
		}
	}
	return;
}
#endif
#ifdef APPENDINVALIDS_TYPE2 
void appendinvalids0(unsigned int workerID  ,uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE]  ,keyvalue_t capsule0[NUM_PARTITIONS]){				
	int analysis_appendinvalids = VECTOR_SIZE;
	
	// append INVALID values (edge conditions)
	keyvalue_t NApKV; NApKV.key = INVALIDDATA; NApKV.value = INVALIDDATA;
	SAVEPARTITIONS_RESOLVEEDGECONDITIONS_LOOP2: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		buffer_type ovsize0 = allignhigher_KV(capsule0[p].value) - capsule0[p].value;
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savepartitions_countinvalids(ovsize0);
		#endif 
		
		for(vector_type v=0; v<VECTOR_SIZE; v++){
		#pragma HLS PIPELINE
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("acts::appendinvalids0 34", capsule0[p].key + capsule0[p].value + v, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, capsule0[p].key, capsule0[p].value, v);
			#endif
			if(v < ovsize0){ setkeyvalue(destbuffer0, 0, capsule0[p].key + capsule0[p].value + v, NApKV, PADDEDDESTBUFFER_SIZE); }
		}
	}
	return;
}
#endif 

void readkeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	unsigned int analysis_buffersz = SRCBUFFER_SIZE;
	
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &size_kvs, 0);
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &offset_kvs, 0);
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::readkeyvalues dram 36", baseaddress + offset_kvs + size_kvs, maxaddress_kvs + 1, baseaddress, offset_kvs, size_kvs);
	#endif
	
	READKEYVALUES_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz	
		buffer[i] = kvdram[baseaddress + offset_kvs + i];
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}
void readkeyvalues0(keyvalue_t * dram, keyvalue_t * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs){
	int analysis_savebuffer = NUM_PARTITIONS;
	
	READKEYVALUES_LOOP: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
		buffer[i] = dram[baseaddress + offset_kvs + i];
	}
	return;
}
void readkeyvalues0(uint512_dt * kvdram  ,keyvalue_t * buffer00  ,keyvalue_t * buffer01  ,keyvalue_t * buffer02  ,keyvalue_t * buffer03  ,keyvalue_t * buffer04  ,keyvalue_t * buffer05  ,keyvalue_t * buffer06  ,keyvalue_t * buffer07  ,batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	unsigned int analysis_buffersz = SRCBUFFER_SIZE;
	
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &size_kvs, 0);
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &offset_kvs, 0);
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::readkeyvalues dram 37", baseaddress + offset_kvs + size_kvs, maxaddress_kvs, baseaddress, offset_kvs, size_kvs);
	#endif
	
	READKEYVALUES_LOOP: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz	
		#ifdef _WIDEWORD
 
		buffer00[i].key = kvdram[baseaddress + offset_kvs + i].range(31, 0);
		buffer00[i].value = kvdram[baseaddress + offset_kvs + i].range(63, 32);
		buffer01[i].key = kvdram[baseaddress + offset_kvs + i].range(95, 64);
		buffer01[i].value = kvdram[baseaddress + offset_kvs + i].range(127, 96);
		buffer02[i].key = kvdram[baseaddress + offset_kvs + i].range(159, 128);
		buffer02[i].value = kvdram[baseaddress + offset_kvs + i].range(191, 160);
		buffer03[i].key = kvdram[baseaddress + offset_kvs + i].range(223, 192);
		buffer03[i].value = kvdram[baseaddress + offset_kvs + i].range(255, 224);
		buffer04[i].key = kvdram[baseaddress + offset_kvs + i].range(287, 256);
		buffer04[i].value = kvdram[baseaddress + offset_kvs + i].range(319, 288);
		buffer05[i].key = kvdram[baseaddress + offset_kvs + i].range(351, 320);
		buffer05[i].value = kvdram[baseaddress + offset_kvs + i].range(383, 352);
		buffer06[i].key = kvdram[baseaddress + offset_kvs + i].range(415, 384);
		buffer06[i].value = kvdram[baseaddress + offset_kvs + i].range(447, 416);
		buffer07[i].key = kvdram[baseaddress + offset_kvs + i].range(479, 448);
		buffer07[i].value = kvdram[baseaddress + offset_kvs + i].range(511, 480);
 
		#else
 
		buffer00[i] = kvdram[baseaddress + offset_kvs + i].data[0];
		buffer01[i] = kvdram[baseaddress + offset_kvs + i].data[1];
		buffer02[i] = kvdram[baseaddress + offset_kvs + i].data[2];
		buffer03[i] = kvdram[baseaddress + offset_kvs + i].data[3];
		buffer04[i] = kvdram[baseaddress + offset_kvs + i].data[4];
		buffer05[i] = kvdram[baseaddress + offset_kvs + i].data[5];
		buffer06[i] = kvdram[baseaddress + offset_kvs + i].data[6];
		buffer07[i] = kvdram[baseaddress + offset_kvs + i].data[7];
 
		#endif
		
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvsread(VECTOR_SIZE);
		#endif
	}
	return;
}
void readkeyvalues0(unsigned int enable, unsigned int workerID, uint512_dt * kvdram  ,uint512_dt * buffer0  ,batch_type baseaddress ,batch_type offset_kvs, travstate_t travstate, batch_type maxaddress_kvs){			
	if(enable == 0){ return; }
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	readkeyvalues0(kvdram, buffer0, baseaddress, offset_kvs + ((workerID * NUMSUBWORKERS * SRCBUFFER_SIZE) + (0 * SRCBUFFER_SIZE)), chunk0_size, maxaddress_kvs);
	return;
}
void readkeyvalues0(unsigned int workerID, uint512_dt * kvdram  ,uint512_dt * buffer0  ,batch_type baseaddress, keyvalue_t sourcestats[NUMSUBWORKERS], travstate_t travstate, batch_type maxaddress_kvs){			
	
	travstate_t travstate0; 
	travstate0.i_kvs = travstate.i_kvs;
	travstate0.begin_kvs = 0;
	travstate0.end_kvs = (sourcestats[0].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	travstate0.skip_kvs = SRCBUFFER_SIZE;
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate0, 0);
	
	readkeyvalues0(kvdram, buffer0, baseaddress, ((sourcestats[0].key / VECTOR_SIZE) + travstate0.i_kvs), chunk0_size, maxaddress_kvs);
	return;
}
offset_t readcapsules0(unsigned int workerID, uint512_dt * kvdram,  uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE], keyvalue_t capsule0[NUM_PARTITIONS],  batch_type baseaddress_kvs, batch_type offset, int enable, offset_t capsulemetadata){
	if(enable == 0){ return capsulemetadata; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::readcapsules0 35", ((baseaddress_kvs * VECTOR_SIZE) + offset + NUM_PARTITIONS), PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
	#endif
	
	buffer_type loffset = offset % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	batch_type loadoffset0_kvs = 0;
	if(loffset == 0){
		loadoffset0_kvs = capsulemetadata.globaloffset + capsulemetadata.localoffset + 0 * CAPSULEBUFFER_SIZE; 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("acts::readcapsules0 34", baseaddress_kvs + loadoffset0_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		readkeyvalues0(kvdram, BIGcapsule0, baseaddress_kvs, loadoffset0_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvstatsread(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
	
		capsulemetadata.localoffset += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	
	READCAPSULE0_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		capsule0[i] = getkeyvalue(BIGcapsule0, loffset + i, CAPSULEBUFFER_SIZE);
	}
	
	/* 	for(partition_type p=0; p<NUM_PARTITIONS; p++){ capsule0[p].value = 0; } // REMOVEME.
 */
	
	return capsulemetadata;
}

#ifdef COLLECTCAPSULES_TYPE1
void collectstats00(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	if(enable == 0){ return; }
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	
	EXTRACTCAPSULES0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p].value = 0;
	}
	EXTRACTCAPSULES0_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
			partition_type p0;
			if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit); }
			if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ sizes0[p0].value += 1; }
		
		#ifdef _DEBUGMODE_STATS
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		#endif
	}
	EXTRACTCAPSULES0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule0[p].key = 0;
		capsule0[p].value += sizes0[p].value;
	}
	return;
}
#endif 
#ifdef COLLECTCAPSULES_TYPE1
void collectstats01(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	if(enable == 0){ return; }
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	
	EXTRACTCAPSULES0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		sizes0[p].value = 0;
	}
	EXTRACTCAPSULES0_LOOP2: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS PIPELINE II=1
			keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
			partition_type p0;
			if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit); }
			if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ sizes0[p0].value += 1; }
		
		#ifdef _DEBUGMODE_STATS
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		#endif
	}
	EXTRACTCAPSULES0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
	#pragma HLS PIPELINE II=1
		capsule0[p].key = 0;
		capsule0[p].value += sizes0[p].value;
	}
	return;
}
#endif 
#ifdef COLLECTCAPSULES_TYPE2
void collectstats00(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	if(enable == 0){ return; }
	unsigned int analysis_collectstats = SRCBUFFER_SIZE;
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type sizes00[NUM_PARTITIONS];
	buffer_type sizes01[NUM_PARTITIONS];
	buffer_type sizes02[NUM_PARTITIONS];
	buffer_type sizes03[NUM_PARTITIONS];
	buffer_type sizes04[NUM_PARTITIONS];
	buffer_type sizes05[NUM_PARTITIONS];
	buffer_type sizes06[NUM_PARTITIONS];
	buffer_type sizes07[NUM_PARTITIONS];
	
	COLLECTSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
 
		sizes00[p] = 0; 
 
		sizes01[p] = 0; 
 
		sizes02[p] = 0; 
 
		sizes03[p] = 0; 
 
		sizes04[p] = 0; 
 
		sizes05[p] = 0; 
 
		sizes06[p] = 0; 
 
		sizes07[p] = 0; 
 
	}
	COLLECTSTATS0_LOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=2
		keyvalue_t keyvalue00;
		keyvalue_t keyvalue01;
		keyvalue_t keyvalue02;
		keyvalue_t keyvalue03;
		keyvalue_t keyvalue04;
		keyvalue_t keyvalue05;
		keyvalue_t keyvalue06;
		keyvalue_t keyvalue07;
		
		#ifdef _WIDEWORD
		keyvalue00.key = sourcebuffer0[i].range(32 * ((0 * 2) + 1) - 1, (0 * 2) * 32);
		keyvalue00.value = sourcebuffer0[i].range(32 * (((0 * 2) + 1) + 1) - 1, (0 * 2 + 1) * 32);
		keyvalue01.key = sourcebuffer0[i].range(32 * ((1 * 2) + 1) - 1, (1 * 2) * 32);
		keyvalue01.value = sourcebuffer0[i].range(32 * (((1 * 2) + 1) + 1) - 1, (1 * 2 + 1) * 32);
		keyvalue02.key = sourcebuffer0[i].range(32 * ((2 * 2) + 1) - 1, (2 * 2) * 32);
		keyvalue02.value = sourcebuffer0[i].range(32 * (((2 * 2) + 1) + 1) - 1, (2 * 2 + 1) * 32);
		keyvalue03.key = sourcebuffer0[i].range(32 * ((3 * 2) + 1) - 1, (3 * 2) * 32);
		keyvalue03.value = sourcebuffer0[i].range(32 * (((3 * 2) + 1) + 1) - 1, (3 * 2 + 1) * 32);
		keyvalue04.key = sourcebuffer0[i].range(32 * ((4 * 2) + 1) - 1, (4 * 2) * 32);
		keyvalue04.value = sourcebuffer0[i].range(32 * (((4 * 2) + 1) + 1) - 1, (4 * 2 + 1) * 32);
		keyvalue05.key = sourcebuffer0[i].range(32 * ((5 * 2) + 1) - 1, (5 * 2) * 32);
		keyvalue05.value = sourcebuffer0[i].range(32 * (((5 * 2) + 1) + 1) - 1, (5 * 2 + 1) * 32);
		keyvalue06.key = sourcebuffer0[i].range(32 * ((6 * 2) + 1) - 1, (6 * 2) * 32);
		keyvalue06.value = sourcebuffer0[i].range(32 * (((6 * 2) + 1) + 1) - 1, (6 * 2 + 1) * 32);
		keyvalue07.key = sourcebuffer0[i].range(32 * ((7 * 2) + 1) - 1, (7 * 2) * 32);
		keyvalue07.value = sourcebuffer0[i].range(32 * (((7 * 2) + 1) + 1) - 1, (7 * 2 + 1) * 32);
		#else
		keyvalue00 = sourcebuffer0[i].data[0];
		keyvalue01 = sourcebuffer0[i].data[1];
		keyvalue02 = sourcebuffer0[i].data[2];
		keyvalue03 = sourcebuffer0[i].data[3];
		keyvalue04 = sourcebuffer0[i].data[4];
		keyvalue05 = sourcebuffer0[i].data[5];
		keyvalue06 = sourcebuffer0[i].data[6];
		keyvalue07 = sourcebuffer0[i].data[7];
		#endif
		
		partition_type p00;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue00.key != INVALIDDATA)){ p00 = getpartition(keyvalue00, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p01;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue01.key != INVALIDDATA)){ p01 = getpartition(keyvalue01, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p02;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue02.key != INVALIDDATA)){ p02 = getpartition(keyvalue02, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p03;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue03.key != INVALIDDATA)){ p03 = getpartition(keyvalue03, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p04;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue04.key != INVALIDDATA)){ p04 = getpartition(keyvalue04, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p05;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue05.key != INVALIDDATA)){ p05 = getpartition(keyvalue05, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p06;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue06.key != INVALIDDATA)){ p06 = getpartition(keyvalue06, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p07;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue07.key != INVALIDDATA)){ p07 = getpartition(keyvalue07, llopparams.currentLOP, llopparams.upperlimit); }
		
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue00.key != INVALIDDATA)){ sizes00[p00] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue01.key != INVALIDDATA)){ sizes01[p01] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue02.key != INVALIDDATA)){ sizes02[p02] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue03.key != INVALIDDATA)){ sizes03[p03] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue04.key != INVALIDDATA)){ sizes04[p04] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue05.key != INVALIDDATA)){ sizes05[p05] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue06.key != INVALIDDATA)){ sizes06[p06] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue07.key != INVALIDDATA)){ sizes07[p07] += 1; }
		
		#ifdef _DEBUGMODE_STATS
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue00.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue01.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue02.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue03.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue04.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue05.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue06.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue07.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		#endif
	}
	COLLECTSTATS0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		capsule0[p].key = 0;
		capsule0[p].value += ( sizes00[p] + sizes01[p] + sizes02[p] + sizes03[p] + sizes04[p] + sizes05[p] + sizes06[p] + sizes07[p] );
	}
	return;
}
#endif
#ifdef COLLECTCAPSULES_TYPE2
void collectstats01(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], clopparams_t llopparams, travstate_t travstate){
	if(enable == 0){ return; }
	unsigned int analysis_collectstats = SRCBUFFER_SIZE;
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	buffer_type sizes00[NUM_PARTITIONS];
	buffer_type sizes01[NUM_PARTITIONS];
	buffer_type sizes02[NUM_PARTITIONS];
	buffer_type sizes03[NUM_PARTITIONS];
	buffer_type sizes04[NUM_PARTITIONS];
	buffer_type sizes05[NUM_PARTITIONS];
	buffer_type sizes06[NUM_PARTITIONS];
	buffer_type sizes07[NUM_PARTITIONS];
	
	COLLECTSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){
 
		sizes00[p] = 0; 
 
		sizes01[p] = 0; 
 
		sizes02[p] = 0; 
 
		sizes03[p] = 0; 
 
		sizes04[p] = 0; 
 
		sizes05[p] = 0; 
 
		sizes06[p] = 0; 
 
		sizes07[p] = 0; 
 
	}
	COLLECTSTATS0_LOOP2: for(buffer_type i=0; i<SRCBUFFER_SIZE; i++){
	#pragma HLS PIPELINE II=2
		keyvalue_t keyvalue00;
		keyvalue_t keyvalue01;
		keyvalue_t keyvalue02;
		keyvalue_t keyvalue03;
		keyvalue_t keyvalue04;
		keyvalue_t keyvalue05;
		keyvalue_t keyvalue06;
		keyvalue_t keyvalue07;
		
		#ifdef _WIDEWORD
		keyvalue00.key = sourcebuffer0[i].range(32 * ((0 * 2) + 1) - 1, (0 * 2) * 32);
		keyvalue00.value = sourcebuffer0[i].range(32 * (((0 * 2) + 1) + 1) - 1, (0 * 2 + 1) * 32);
		keyvalue01.key = sourcebuffer0[i].range(32 * ((1 * 2) + 1) - 1, (1 * 2) * 32);
		keyvalue01.value = sourcebuffer0[i].range(32 * (((1 * 2) + 1) + 1) - 1, (1 * 2 + 1) * 32);
		keyvalue02.key = sourcebuffer0[i].range(32 * ((2 * 2) + 1) - 1, (2 * 2) * 32);
		keyvalue02.value = sourcebuffer0[i].range(32 * (((2 * 2) + 1) + 1) - 1, (2 * 2 + 1) * 32);
		keyvalue03.key = sourcebuffer0[i].range(32 * ((3 * 2) + 1) - 1, (3 * 2) * 32);
		keyvalue03.value = sourcebuffer0[i].range(32 * (((3 * 2) + 1) + 1) - 1, (3 * 2 + 1) * 32);
		keyvalue04.key = sourcebuffer0[i].range(32 * ((4 * 2) + 1) - 1, (4 * 2) * 32);
		keyvalue04.value = sourcebuffer0[i].range(32 * (((4 * 2) + 1) + 1) - 1, (4 * 2 + 1) * 32);
		keyvalue05.key = sourcebuffer0[i].range(32 * ((5 * 2) + 1) - 1, (5 * 2) * 32);
		keyvalue05.value = sourcebuffer0[i].range(32 * (((5 * 2) + 1) + 1) - 1, (5 * 2 + 1) * 32);
		keyvalue06.key = sourcebuffer0[i].range(32 * ((6 * 2) + 1) - 1, (6 * 2) * 32);
		keyvalue06.value = sourcebuffer0[i].range(32 * (((6 * 2) + 1) + 1) - 1, (6 * 2 + 1) * 32);
		keyvalue07.key = sourcebuffer0[i].range(32 * ((7 * 2) + 1) - 1, (7 * 2) * 32);
		keyvalue07.value = sourcebuffer0[i].range(32 * (((7 * 2) + 1) + 1) - 1, (7 * 2 + 1) * 32);
		#else
		keyvalue00 = sourcebuffer0[i].data[0];
		keyvalue01 = sourcebuffer0[i].data[1];
		keyvalue02 = sourcebuffer0[i].data[2];
		keyvalue03 = sourcebuffer0[i].data[3];
		keyvalue04 = sourcebuffer0[i].data[4];
		keyvalue05 = sourcebuffer0[i].data[5];
		keyvalue06 = sourcebuffer0[i].data[6];
		keyvalue07 = sourcebuffer0[i].data[7];
		#endif
		
		partition_type p00;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue00.key != INVALIDDATA)){ p00 = getpartition(keyvalue00, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p01;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue01.key != INVALIDDATA)){ p01 = getpartition(keyvalue01, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p02;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue02.key != INVALIDDATA)){ p02 = getpartition(keyvalue02, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p03;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue03.key != INVALIDDATA)){ p03 = getpartition(keyvalue03, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p04;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue04.key != INVALIDDATA)){ p04 = getpartition(keyvalue04, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p05;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue05.key != INVALIDDATA)){ p05 = getpartition(keyvalue05, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p06;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue06.key != INVALIDDATA)){ p06 = getpartition(keyvalue06, llopparams.currentLOP, llopparams.upperlimit); }
		partition_type p07;
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue07.key != INVALIDDATA)){ p07 = getpartition(keyvalue07, llopparams.currentLOP, llopparams.upperlimit); }
		
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue00.key != INVALIDDATA)){ sizes00[p00] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue01.key != INVALIDDATA)){ sizes01[p01] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue02.key != INVALIDDATA)){ sizes02[p02] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue03.key != INVALIDDATA)){ sizes03[p03] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue04.key != INVALIDDATA)){ sizes04[p04] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue05.key != INVALIDDATA)){ sizes05[p05] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue06.key != INVALIDDATA)){ sizes06[p06] += 1; }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue07.key != INVALIDDATA)){ sizes07[p07] += 1; }
		
		#ifdef _DEBUGMODE_STATS
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue00.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue01.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue02.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue03.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue04.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue05.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue06.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		if((WithinValidRange(i, chunk0_size) == 1) && (keyvalue07.key != INVALIDDATA)){ actsutilityobj->globalvar_collectstats_counttotalkvsread(1); }
		#endif
	}
	COLLECTSTATS0_LOOP3: for(partition_type p=0; p<NUM_PARTITIONS; p++){
		capsule0[p].key = 0;
		capsule0[p].value += ( sizes00[p] + sizes01[p] + sizes02[p] + sizes03[p] + sizes04[p] + sizes05[p] + sizes06[p] + sizes07[p] );
	}
	return;
}
#endif
void partitionkeyvalues00(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	if(enable == 0){ return; }
	unsigned int analysis_partition = SRCBUFFER_SIZE;
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  sizes0[p].key = capsule0[p].key; sizes0[p].value = capsule0[p].value;  }
	PARTITIONKEYVALUES_MAINLOOP: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partition avg=analysis_partition
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);

		partition_type p0;
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit); }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("acts::partitionkeyvalues0 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, p0);
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ actsutilityobj->print4("partitionkeyvalues00:: keyvalue0.key", "keyvalue0.value", "insert location", "PADDEDDESTBUFFERSIZE", keyvalue0.key, keyvalue0.value, sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE); }
		#endif 
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ setkeyvalue(destbuffer0, 0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE); } // REMOVEME?
		// setkeyvalue(destbuffer0, 0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE); // WARNING: hangs for NUM_PARTITIONS=32
		
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ sizes0[p0].value += 1; }
		
		#ifdef _DEBUGMODE_STATS
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ actsutilityobj->globalstats_countkvspartitioned(1); }
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  }
}
void partitionkeyvalues01(unsigned int enable, unsigned int workerID , uint512_dt sourcebuffer0[SRCBUFFER_SIZE] , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], travstate_t travstate, clopparams_t llopparams){					
	if(enable == 0){ return; }
	unsigned int analysis_partition = SRCBUFFER_SIZE;
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate, ((workerID * NUMSUBWORKERS) + (0 * SRCBUFFER_SIZE)));
	
	keyvalue_t sizes0[NUM_PARTITIONS];
	#pragma HLS ARRAY_PARTITION variable=sizes0 complete
	
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  sizes0[p].key = capsule0[p].key; sizes0[p].value = capsule0[p].value;  }
	PARTITIONKEYVALUES_MAINLOOP: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partition avg=analysis_partition
	#pragma HLS PIPELINE II=1
		keyvalue_t keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);

		partition_type p0;
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ p0 = getpartition(keyvalue0, llopparams.currentLOP, llopparams.upperlimit); }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("acts::partitionkeyvalues0 34", sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE, sizes0[p0].key, sizes0[p0].value, p0);
		#endif
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ actsutilityobj->print4("partitionkeyvalues01:: keyvalue0.key", "keyvalue0.value", "insert location", "PADDEDDESTBUFFERSIZE", keyvalue0.key, keyvalue0.value, sizes0[p0].key + sizes0[p0].value, PADDEDDESTBUFFER_SIZE * VECTOR_SIZE); }
		#endif 
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ setkeyvalue(destbuffer0, 0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE); } // REMOVEME?
		// setkeyvalue(destbuffer0, 0, sizes0[p0].key + sizes0[p0].value, keyvalue0, PADDEDDESTBUFFER_SIZE); // WARNING: hangs for NUM_PARTITIONS=32
		
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ sizes0[p0].value += 1; }
		
		#ifdef _DEBUGMODE_STATS
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ actsutilityobj->globalstats_countkvspartitioned(1); }
		#endif
	}
	for(partition_type p=0; p<NUM_PARTITIONS; p++){  capsule0[p].value = sizes0[p].value;  }
}
void reducepartitions0(unsigned int enable, unsigned int workerID  ,uint512_dt sourcebuffer0[SRCBUFFER_SIZE]   ,uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE]  ,batch_type voffset, keyvalue_t sourcestats[NUMSUBWORKERS], travstate_t travstate, globalparams_t globalparams){
	if(enable == 0){ return; }
	unsigned int analysis_buffersz = SRCBUFFER_SIZE * VECTOR_SIZE;
	
	travstate_t travstate0; 
	travstate0.i_kvs = travstate.i_kvs;
	travstate0.begin_kvs = 0;
	travstate0.end_kvs = (sourcestats[0].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	travstate0.skip_kvs = SRCBUFFER_SIZE;
	
	
	buffer_type chunk0_size = SRCBUFFER_SIZE;
	getchunksize(&chunk0_size, SRCBUFFER_SIZE, travstate0, 0);
	
	value_t voffset0 = voffset + (0 * APPLYVERTEXBUFFERSZ);
	
	REDUCE_MAINLOOP: for(buffer_type k=0; k<SRCBUFFER_SIZE * VECTOR_SIZE; k++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz
	
		keyvalue_t keyvalue0;
		keyvalue_t vprop0;
		
		keyvalue0 = getkeyvalue(sourcebuffer0, k, SRCBUFFER_SIZE);
		
		vertex_t loc0 = keyvalue0.key - voffset0 - globalparams.vbegin;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"reducepartitions0: k: "<<k<<", loc0: "<<loc0<<", keyvalue0.key: "<<keyvalue0.key<<", chunk0_size: "<<chunk0_size<<", voffset0: "<<voffset0<<", globalparams.vbegin: "<<globalparams.vbegin<<endl; 
		#endif 
		
		if((loc0 >= APPLYVERTEXBUFFERSZ) && (WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ 
			#ifdef _DEBUGMODE_CHECKS2
			#ifdef ENABLE_VOICEOUTREDUCEERRORS
			cout<<"ERROR SEEN @ reducepartitions0 45:: k: "<<k<<", loc0: "<<loc0<<", keyvalue0.key: "<<keyvalue0.key<<", chunk0_size: "<<chunk0_size<<", voffset0: "<<voffset0<<", globalparams.vbegin: "<<globalparams.vbegin<<", APPLYVERTEXBUFFERSZ: "<<APPLYVERTEXBUFFERSZ <<endl; 
			#endif 
			#ifdef ENABLE_PERFECTACCURACY
			exit(EXIT_FAILURE); // FIXME. PERFECTIONTEST.
			#endif 
			#endif 
			loc0 = 0;
		} // REMOVEME.
		
		#ifdef _DEBUGMODE_CHECKS2
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ actsutilityobj->checkoutofbounds("legion::reduce_ddr0 34", loc0, APPLYVERTEXBUFFERSZ, k, keyvalue0.key, voffset0); }
		#endif
		
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ vprop0 = getkeyvalue(destbuffer0, loc0, APPLYVERTEXBUFFERSZ_KVS); }
		
		value_t temp0 = reducefunc(vprop0.key, vprop0.value, keyvalue0.value, globalparams.GraphIter);
		
		vprop0.value = temp0;
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ actsutilityobj->print6("reducepartitions0:: keyvalue0.key", "keyvalue0.value", "vprop0.key", "vprop0.value", "loc0", "NAp", keyvalue0.key, keyvalue0.value, vprop0.key, vprop0.value, loc0, NAp); }
		#endif 
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ setkeyvalue(destbuffer0, 0, loc0, vprop0, APPLYVERTEXBUFFERSZ_KVS); }
		
		#ifdef _DEBUGMODE_STATS
		if((WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1) && (keyvalue0.key != INVALIDDATA)){ actsutilityobj->globalstats_reduce_countvalidkvsreduced(1); }
		if(WithinValidRange(k, chunk0_size * VECTOR_SIZE) == 1){ actsutilityobj->globalstats_countkvsreduced(1); }
		#endif
	}
	return;
}

void savekeyvalues0(uint512_dt * kvdram, uint512_dt * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	int analysis_savebuffer = PADDEDDESTBUFFER_SIZE;
	
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &size_kvs, 0);
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &offset_kvs, 0);
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::savekeyvalues dram 35", baseaddress + offset_kvs + size_kvs, maxaddress_kvs + 1, NAp, NAp, NAp);
	#endif
	SAVEKEYVALUES_LOOP: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer	
		kvdram[baseaddress + offset_kvs + i] = buffer[i];
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalstats_countkvswritten(VECTOR_SIZE);
		#endif
	}
	return;
}
void savekeyvalues0(keyvalue_t * dram, keyvalue_t * buffer, batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs){
	int analysis_savebuffer = NUM_PARTITIONS;
	
	SAVEKEYVALUES_LOOP: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
		dram[baseaddress + offset_kvs + i] = buffer[i];
	}
	return;
}
void savekeyvalues0(uint512_dt * kvdram  ,keyvalue_t * buffer00  ,keyvalue_t * buffer01  ,keyvalue_t * buffer02  ,keyvalue_t * buffer03  ,keyvalue_t * buffer04  ,keyvalue_t * buffer05  ,keyvalue_t * buffer06  ,keyvalue_t * buffer07  ,batch_type baseaddress, batch_type offset_kvs, buffer_type size_kvs, batch_type maxaddress_kvs){			
	unsigned int analysis_buffersz = SRCBUFFER_SIZE;
	
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &size_kvs, 0);
	checkandforce(baseaddress + offset_kvs + size_kvs, maxaddress_kvs, &offset_kvs, 0);
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts::savekeyvalues dram 36", baseaddress + offset_kvs + size_kvs, maxaddress_kvs, baseaddress, offset_kvs, size_kvs);
	#endif
	
	SAVEKEYVALUES_LOOP: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_buffersz avg=analysis_buffersz	
		#ifdef _WIDEWORD
 
		kvdram[baseaddress + offset_kvs + i].range(31, 0) = buffer00[i].key;
		kvdram[baseaddress + offset_kvs + i].range(63, 32) = buffer00[i].value;
		kvdram[baseaddress + offset_kvs + i].range(95, 64) = buffer01[i].key;
		kvdram[baseaddress + offset_kvs + i].range(127, 96) = buffer01[i].value;
		kvdram[baseaddress + offset_kvs + i].range(159, 128) = buffer02[i].key;
		kvdram[baseaddress + offset_kvs + i].range(191, 160) = buffer02[i].value;
		kvdram[baseaddress + offset_kvs + i].range(223, 192) = buffer03[i].key;
		kvdram[baseaddress + offset_kvs + i].range(255, 224) = buffer03[i].value;
		kvdram[baseaddress + offset_kvs + i].range(287, 256) = buffer04[i].key;
		kvdram[baseaddress + offset_kvs + i].range(319, 288) = buffer04[i].value;
		kvdram[baseaddress + offset_kvs + i].range(351, 320) = buffer05[i].key;
		kvdram[baseaddress + offset_kvs + i].range(383, 352) = buffer05[i].value;
		kvdram[baseaddress + offset_kvs + i].range(415, 384) = buffer06[i].key;
		kvdram[baseaddress + offset_kvs + i].range(447, 416) = buffer06[i].value;
		kvdram[baseaddress + offset_kvs + i].range(479, 448) = buffer07[i].key;
		kvdram[baseaddress + offset_kvs + i].range(511, 480) = buffer07[i].value;
 
		#else
 
		kvdram[baseaddress + offset_kvs + i].data[0] = buffer00[i];
		kvdram[baseaddress + offset_kvs + i].data[1] = buffer01[i];
		kvdram[baseaddress + offset_kvs + i].data[2] = buffer02[i];
		kvdram[baseaddress + offset_kvs + i].data[3] = buffer03[i];
		kvdram[baseaddress + offset_kvs + i].data[4] = buffer04[i];
		kvdram[baseaddress + offset_kvs + i].data[5] = buffer05[i];
		kvdram[baseaddress + offset_kvs + i].data[6] = buffer06[i];
		kvdram[baseaddress + offset_kvs + i].data[7] = buffer07[i];
 
		#endif
	}
	return;
}
offset_t savecapsules0(unsigned int workerID, uint512_dt * kvdram , uint512_dt BIGcapsule0[CAPSULEBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], keyvalue_t kvdeststats_tmp3[NUM_PARTITIONS], batch_type baseaddress_kvs, batch_type offset, travstate_t travstate, offset_t capsulemetadata){							
	buffer_type loffset = offset % (CAPSULEBUFFER_SIZE * VECTOR_SIZE);
	
	// gathering global information
	APPENDCAPSULE0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ 
		kvdeststats_tmp[p].value += allignhigher_KV(capsule0[p].value);
		#ifdef _DEBUGMODE_STATS
		kvdeststats_tmp3[p].value += capsule0[p].value;
		#endif 
	}
	
	calculatemanyoffsets(capsule0, 0);
	
	SAVECAPSULE0_LOOP2: for(partition_type i=0; i<NUM_PARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		capsule0[i].value = 0; // REMOVEME. 
		setkeyvalue(BIGcapsule0, 0, loffset + i, capsule0[i], CAPSULEBUFFER_SIZE);
	}
	
	if(((loffset + NUM_PARTITIONS) == (CAPSULEBUFFER_SIZE * VECTOR_SIZE)) || ((travstate.i_kvs + travstate.skip_kvs) >= travstate.end_kvs)){
		batch_type saveoffset0_kvs = capsulemetadata.globaloffset + capsulemetadata.localoffset + (0 * CAPSULEBUFFER_SIZE);
		
		checkandforce(baseaddress_kvs + saveoffset0_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS, &saveoffset0_kvs, 0);
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("acts::savecapsules0 34", baseaddress_kvs + saveoffset0_kvs + CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS + 1, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE);
		#endif
		savekeyvalues0(kvdram, BIGcapsule0, baseaddress_kvs, saveoffset0_kvs, CAPSULEBUFFER_SIZE, PADDEDKVSOURCEDRAMSZ_KVS);
		#ifdef _DEBUGMODE_STATS
		actsutilityobj->globalvar_savestats_counttotalstatswritten(CAPSULEBUFFER_SIZE * VECTOR_SIZE);
		#endif
		
		capsulemetadata.localoffset += NUMSUBWORKERS * CAPSULEBUFFER_SIZE;
	}
	return capsulemetadata;
}
void savepartitions0(unsigned int workerID, uint512_dt * kvdram , uint512_dt destbuffer0[PADDEDDESTBUFFER_SIZE] , keyvalue_t capsule0[NUM_PARTITIONS] ,keyvalue_t kvdeststats_tmp[NUM_PARTITIONS], clopparams_t llopparams){				
	int analysis_savebuffer = PADDEDBUFFER_SIZE / NUM_PARTITIONS;
	int analysis_appendinvalids = VECTOR_SIZE;
	
		SAVEPARTITIONS0_MAINLOOP1A: for(partition_type p=0; p<NUM_PARTITIONS; p++){
			batch_type dramoffset_kvs = llopparams.baseaddr_workdestkvs_kvs + ((kvdeststats_tmp[p].key + kvdeststats_tmp[p].value) / VECTOR_SIZE);
			buffer_type bramoffset_kvs = capsule0[p].key / VECTOR_SIZE;
			
			buffer_type realsize_kvs = (capsule0[p].value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			buffer_type size_kvs = getpartitionwritesz(realsize_kvs, bramoffset_kvs);
			
			checkandforce(bramoffset_kvs + size_kvs, PADDEDDESTBUFFER_SIZE, &size_kvs, 0);
			checkandforce(bramoffset_kvs + size_kvs, PADDEDDESTBUFFER_SIZE, &bramoffset_kvs, 0);
			checkandforce(dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, &size_kvs, 0);
			checkandforce(dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, &dramoffset_kvs, 0);
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("acts::savepartitions0 dram 34", dramoffset_kvs + size_kvs, PADDEDKVSOURCEDRAMSZ_KVS, dramoffset_kvs, size_kvs, llopparams.baseaddr_workdestkvs_kvs);
			actsutilityobj->checkoutofbounds("acts::savepartitions0 bram 35", bramoffset_kvs + size_kvs, PADDEDDESTBUFFER_SIZE, bramoffset_kvs, capsule0[p].key, size_kvs);
			#endif

			SAVEPARTITIONS0_MAINLOOP1B: for(buffer_type i=0; i<size_kvs; i++){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_savebuffer avg=analysis_savebuffer
			#pragma HLS PIPELINE II=1
				kvdram[dramoffset_kvs + i] = destbuffer0[bramoffset_kvs + i];
				
				#ifdef _DEBUGMODE_STATS
				actsutilityobj->globalstats_countkvspartitionswritten_actual(VECTOR_SIZE);
				#endif
			}
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countkvspartitionswritten(realsize_kvs * VECTOR_SIZE);
			#endif
		}
		UPDATEGLOBALSTATS0_LOOP1: for(partition_type p=0; p<NUM_PARTITIONS; p++){ kvdeststats_tmp[p].value += allignhigher_KV(capsule0[p].value); }
		#ifdef _DEBUGMODE_CHECKS2
		for(partition_type p=0; p<NUM_PARTITIONS-1; p++){ actsutilityobj->checkoutofbounds("acts::savepartitions0 check 38", kvdeststats_tmp[p].key + kvdeststats_tmp[p].value, kvdeststats_tmp[p+1].key, kvdeststats_tmp[p].key, kvdeststats_tmp[p].value, p); }
		for(partition_type p=0; p<NUM_PARTITIONS; p++){ actsutilityobj->checkoutofbounds("acts::savepartitions0 check 39", kvdeststats_tmp[p].key + kvdeststats_tmp[p].value, BASEOFFSET_CAPSULES, kvdeststats_tmp[p].key, kvdeststats_tmp[p].value, p); }
		for(partition_type p=0; p<NUM_PARTITIONS; p++){ actsutilityobj->checkoutofbounds("acts::savepartitions0 check 40", kvdeststats_tmp[p].key + kvdeststats_tmp[p].value, KVSOURCEDRAMSZ, kvdeststats_tmp[p].key, kvdeststats_tmp[p].value, p); }
		#endif
	return;
}

void partitionandreduce0(uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats, globalparams_t globalparams){
	int analysis_maxtreedepthsz = 1;
	int analysis_avgnumsourcepartitions = 1;
	int analysis_partitionphase = (KVDRAMSZ_KVS / (NUMWORKERS * SRCBUFFER_SIZE)) / VECTOR_SIZE;
	int analysis_reducephase = 1;
	int analysis_chunksize = SRCBUFFER_SIZE;
	int analysis_iterationcount = KVDATA_BATCHSIZE_KVS / SRCBUFFER_SIZE;
	
	uint512_dt sourcebuffer00[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer00 dim=1
	uint512_dt destbuffer00[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule00[NUM_PARTITIONS];
	uint512_dt BIGcapsule00[CAPSULEBUFFER_SIZE];
	uint512_dt sourcebuffer10[2][SRCBUFFER_SIZE];
	#pragma HLS ARRAY_PARTITION variable=sourcebuffer10 dim=1
	uint512_dt destbuffer10[PADDEDDESTBUFFER_SIZE];
	keyvalue_t capsule10[NUM_PARTITIONS];
	uint512_dt BIGcapsule10[CAPSULEBUFFER_SIZE];
	
	unsigned int enable_readkeyvalues;
	unsigned int enable_collectstats;
	unsigned int enable_partitionkeyvalues;
	unsigned int enable_reducekeyvalues;
	
	partitionparams_t partitionparams;
	partitionparams_t CSpartitionparams;
	partitionparams_t PVUpartitionparams;
	travstate_t travstate;
	travstate_t travstate_pp0;
	travstate_t travstate_pp1;
	travstate_t travstateG;
	offset_t capsulemetadata;
	batch_type skipspacing;
	batch_type skipspacings[NUM_PARTITIONS];
	keyvalue_t sourcestats[NUMSUBWORKERS];
	keyvalue_t sourcestats2[NUMSUBWORKERS];
	
	unsigned int numLOPs = 0;
	if(globalparams.runkernelcommand == ON){
		if(globalparams.reducecommand == ON){ numLOPs = globalparams.treedepth + 1 + 1; }
		else { numLOPs = globalparams.treedepth + 1; }
	} else { numLOPs = 1; }
	unsigned int sourcestatsmarker = 0;
	unsigned int deststatsmarker = 1;
	unsigned int destoffset = 0;
	capsulemetadata.globaloffset = 0; 
	capsulemetadata.localoffset = 0; 
	resetcapsules(capsule00, NAp);
	resetcapsules(capsule10, NAp);
	unsigned int loadKvDramsize = 0;
	keyvalue_t firstkeyvalue; firstkeyvalue.key = 0; firstkeyvalue.value = globalparams.runsize;
	kvstats[BASEOFFSET_STATSDRAM] = firstkeyvalue;
	
	#ifdef ENABLE_LOADKVDRAM 
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("loadKvDRAM0 35", globalparams.nextbatchoffset + globalparams.batchsize, PADDEDKVSOURCEDRAMSZ, NAp, NAp, NAp);
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS3
	if(globalparams.runkernelcommand == ON){ cout<<"...running Acts... offset: "<<globalparams.nextbatchoffset<<", size: "<<globalparams.batchsize<<endl; }
	else { cout<<"...loading KvDRAM... offset: "<<globalparams.nextbatchoffset<<", size: "<<globalparams.batchsize<<endl; }
	#endif
	LOADKVDRAMPHASE_LOOP1: for (batch_type i = 0; i < globalparams.batchsize / VECTOR_SIZE; i += SRCBUFFER_SIZE){
	#pragma HLS LOOP_TRIPCOUNT min=analysis_iterationcount max=analysis_iterationcount
		int chunk_size = SRCBUFFER_SIZE;
		if ((i + SRCBUFFER_SIZE) > (globalparams.batchsize / VECTOR_SIZE)){ chunk_size = (globalparams.batchsize / VECTOR_SIZE) - i; }
		readkeyvalues0(kvsourcedram, sourcebuffer00[0], BASEOFFSET_KVDRAMBUFFER_KVS, 0 + i, chunk_size, PADDEDKVSOURCEDRAMSZ_KVS);
		savekeyvalues0(kvsourcedram, sourcebuffer00[0], BASEOFFSET_KVDRAM_KVS, (globalparams.nextbatchoffset / VECTOR_SIZE) + i, chunk_size, PADDEDKVSOURCEDRAMSZ_KVS);
	}
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	#endif 

	// Partition Phase
	PARTITIONANDREDUCE_LOOP1: for(unsigned int currentLOP=1; currentLOP<numLOPs; currentLOP++){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_maxtreedepthsz avg=analysis_maxtreedepthsz
		keyvalue_t kvdeststats_tmp[NUM_PARTITIONS];
		keyvalue_t kvdeststats_tmp2[NUM_PARTITIONS];
		keyvalue_t kvdeststats_tmp3[NUM_PARTITIONS];
		destoffset = 0;
		GVcurrentLOP = currentLOP;
		
		clopparams_t llopparams;
		loadclopparams(globalparams, &llopparams, currentLOP);
		unsigned int num_source_partition_skipsize = 0;
		if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ num_source_partition_skipsize = NUMSUBWORKERS; }
		else { num_source_partition_skipsize = 1; }
		
		#ifdef _DEBUGMODE_KERNELPRINTS2
		cout<<endl;
		actsutilityobj->print4("PartitionPhase0:: num_source_partitions", "num_destpartitions", "sourceskipsize_kv", "NAp", llopparams.num_source_partitions, llopparams.num_source_partitions * NUM_PARTITIONS, llopparams.sourceskipsize_kv, NAp);
		#endif
		PARTITIONANDREDUCE_LOOP2: for(unsigned int source_partition=0; source_partition<llopparams.num_source_partitions; source_partition+=num_source_partition_skipsize){
		#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_avgnumsourcepartitions avg=analysis_avgnumsourcepartitions
		
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("acts::partitionandreduce0 22", BASEOFFSET_STATSDRAM + sourcestatsmarker, KVSTATSDRAMSZ, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("acts::partitionandreduce0 23", destoffset, KVDRAMSZ, destoffset, skipspacing, NAp);
			#endif
			llopparams.upperlimit = globalparams.vbegin + llopparams.nextsourceoffset_kv; // THEISSUE.
			keyvalue_t sourcestat = kvstats[BASEOFFSET_STATSDRAM + sourcestatsmarker];
			partitionparams.begin_kvs = sourcestat.key / VECTOR_SIZE;
			partitionparams.size_kvs = (sourcestat.value + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
			partitionparams.step_kvs = NUMWORKERS * NUMSUBWORKERS * NFACTOR * SRCBUFFER_SIZE;
			partitionparams.end_kvs = partitionparams.begin_kvs + partitionparams.size_kvs;
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ partitionparams.begin_kvs = 0; partitionparams.size_kvs = 0; partitionparams.step_kvs = 0; partitionparams.end_kvs = 0; }
			CSpartitionparams = partitionparams;
			PVUpartitionparams = partitionparams;
			travstate.begin_kvs = partitionparams.begin_kvs;
			travstate.end_kvs = partitionparams.end_kvs;
			travstate.skip_kvs = partitionparams.step_kvs;
			travstate_pp0 = travstate;
			travstate_pp1 = travstate;
			travstateG = travstate;
			#ifdef _DEBUGMODE_KERNELPRINTS2
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ actsutilityobj->print6("ReducePhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * SRCBUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, BATCH_RANGE / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); } 
			else { actsutilityobj->print6("PartitionPhase0:: begin", "size", "step", "currentLOP", "range_per_destpartition", "source_partition", sourcestat.key, sourcestat.value, NUMSUBWORKERS * SRCBUFFER_SIZE * VECTOR_SIZE, llopparams.currentLOP, BATCH_RANGE / pow(NUM_PARTITIONS, llopparams.currentLOP), source_partition); }
			#endif
			resetkeyandvalues(kvdeststats_tmp, NUM_PARTITIONS);
			#ifdef _DEBUGMODE_STATS
			resetkeyandvalues(kvdeststats_tmp2, NUM_PARTITIONS);
			resetkeyandvalues(kvdeststats_tmp3, NUM_PARTITIONS);
			#endif 
			GVsource_partition = source_partition;
			
			// Read already collected capsules
			#ifdef ENABLE_REUSESAVEDCAPSULES
			if(globalparams.statsalreadycollected == 1){
				keyvalue_t KV;
				KV = kvstats[BASEOFFSET_CAPSULEMETADATA + sourcestatsmarker];
				capsulemetadata.globaloffset = KV.key;
				capsulemetadata.localoffset = KV.value;
				shutdownpartitionparams(&CSpartitionparams);
				READKEYVALUES_LOOP1: for(buffer_type i=0; i<NUM_PARTITIONS; i++){ kvdeststats_tmp[i] = kvstats[BASEOFFSET_STATSDRAM + deststatsmarker + i]; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"...stats already exists, skipping collect-capsule stage... "<<endl;
				#endif 
			} else { 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"...stats does not exist, not skipping collect-capsule stage... "<<endl; 
				#endif 
			}
			#endif
			
			// Collect capsules
			capsulemetadata.localoffset = 0;
			COLLECTSTATS_MAINLOOP1: for(int i = CSpartitionparams.begin_kvs; i < CSpartitionparams.end_kvs; i += CSpartitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print4("CollectStatsPhase0:: i", "i_end", "step size", "upperlimit", i, CSpartitionparams.end_kvs, CSpartitionparams.step_kvs, llopparams.upperlimit);
				#endif
				
				travstateG.i_kvs = i;
				GVi = i;
				batch_type capsuleoffset = ((i - CSpartitionparams.begin_kvs) / CSpartitionparams.step_kvs) * NUM_PARTITIONS;
				COLLECTSTATS_MAINLOOP1B: for(vector_type v=0; v<NFACTOR; v+=CS_NUM_READ_PIPELINES){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					
					#ifdef COLLECTCAPSULES_2READPIPELINES
					if((v==0) && (i != CSpartitionparams.begin_kvs)){ enable_readkeyvalues = 0; } else { enable_readkeyvalues = 1; }
					if(v==0){ enable_collectstats = 0; } else { enable_collectstats = 1; }
					#else 
					enable_readkeyvalues = 1;
					enable_collectstats = 1;
					#endif
					
					travstate_pp0.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					readkeyvalues0(enable_readkeyvalues, 0, kvsourcedram  ,sourcebuffer00[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp0.i_kvs, travstate_pp0, PADDEDKVSOURCEDRAMSZ_KVS);
					readkeyvalues0(enable_readkeyvalues, 1, kvsourcedram  ,sourcebuffer10[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp0.i_kvs, travstate_pp0, PADDEDKVSOURCEDRAMSZ_KVS);
					#ifdef COLLECTCAPSULES_2READPIPELINES
					collectstats00(enable_collectstats, 0  ,sourcebuffer00[1]  ,capsule00, llopparams, travstate_pp1);
					collectstats01(enable_collectstats, 1  ,sourcebuffer10[1]  ,capsule10, llopparams, travstate_pp1);
					#endif 
					
					collectstats00(1, 0  ,sourcebuffer00[0]  ,capsule00, llopparams, travstate_pp0);
					collectstats01(1, 1  ,sourcebuffer10[0]  ,capsule10, llopparams, travstate_pp0);
					#ifdef COLLECTCAPSULES_2READPIPELINES
					travstate_pp1.i_kvs = i + ((v + 1) * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					readkeyvalues0(1, 0, kvsourcedram  ,sourcebuffer00[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp1.i_kvs, travstate_pp1, PADDEDKVSOURCEDRAMSZ_KVS);
					readkeyvalues0(1, 1, kvsourcedram  ,sourcebuffer10[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp1.i_kvs, travstate_pp1, PADDEDKVSOURCEDRAMSZ_KVS);
					#endif 
				}
				#ifdef COLLECTCAPSULES_2READPIPELINES
				travstate_pp0.i_kvs = i + CSpartitionparams.step_kvs + (0 * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
				readkeyvalues0(1, 0, kvsourcedram  ,sourcebuffer00[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp0.i_kvs, travstate_pp0, PADDEDKVSOURCEDRAMSZ_KVS);
				readkeyvalues0(1, 1, kvsourcedram  ,sourcebuffer10[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp0.i_kvs, travstate_pp0, PADDEDKVSOURCEDRAMSZ_KVS);
				collectstats00(1, 0  ,sourcebuffer00[1]  ,capsule00, llopparams, travstate_pp1);
				collectstats01(1, 1  ,sourcebuffer10[1]  ,capsule10, llopparams, travstate_pp1);
				#endif
				capsulemetadata = savecapsules0(0, kvsourcedram , BIGcapsule00 , capsule00, kvdeststats_tmp, kvdeststats_tmp3, BASEOFFSET_CAPSULES_KVS, capsuleoffset, travstateG, capsulemetadata);
				capsulemetadata = savecapsules0(1, kvsourcedram , BIGcapsule10 , capsule10, kvdeststats_tmp, kvdeststats_tmp3, BASEOFFSET_CAPSULES_KVS, capsuleoffset, travstateG, capsulemetadata);
				resetcapsules( capsule00, NAp);
				resetcapsules( capsule10, NAp);
			}
			resetcapsules( capsule00, NAp);
			resetcapsules( capsule10, NAp);
			skipspacing = 64;
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 0 && globalparams.statsalreadycollected == 0){ calculateoffsets(kvdeststats_tmp, destoffset, skipspacing); }
			#ifdef _DEBUGMODE_STATS
			actscopy(kvdeststats_tmp, kvdeststats_tmp2, NUM_PARTITIONS);
			#endif
			resetvalues(kvdeststats_tmp, NUM_PARTITIONS);
			if(globalparams.partitioncommand == OFF){ break; } // REMOVEME.
			
			// Partition keyvalues
			capsulemetadata.localoffset = 0;
			PARTITIONPHASE_MAINLOOP1: for (int i = PVUpartitionparams.begin_kvs; i < PVUpartitionparams.end_kvs; i += PVUpartitionparams.step_kvs){
			#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_partitionphase avg=analysis_partitionphase
				#ifdef _DEBUGMODE_KERNELPRINTS
				actsutilityobj->print4("PartitionPhase0:: i", "i_end", "step size", "upperlimit", i, PVUpartitionparams.end_kvs, PVUpartitionparams.step_kvs, llopparams.upperlimit);
				#endif
				
				GVi = i;
				batch_type capsuleoffset = ((i - PVUpartitionparams.begin_kvs) / PVUpartitionparams.step_kvs) * NUM_PARTITIONS;
				capsulemetadata = readcapsules0(0, kvsourcedram,  BIGcapsule00,  capsule00, BASEOFFSET_CAPSULES_KVS, capsuleoffset, 1, capsulemetadata); // FIXME. include workerID in offset
				capsulemetadata = readcapsules0(1, kvsourcedram,  BIGcapsule10,  capsule10, BASEOFFSET_CAPSULES_KVS, capsuleoffset, 1, capsulemetadata); // FIXME. include workerID in offset
				PARTITIONPHASE_MAINLOOP1B: for(vector_type v = 0; v<NFACTOR; v+=PVU_NUM_READ_PIPELINES){
					travstate.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					
					#ifdef PARTITIONKEYVALUES_2READPIPELINES
					if((v==0) && (i != PVUpartitionparams.begin_kvs)){ enable_readkeyvalues = 0; } else { enable_readkeyvalues = 1; }
					if(v==0){ enable_partitionkeyvalues = 0; } else { enable_partitionkeyvalues = 1; }
					#else 
					enable_readkeyvalues = 1;
					enable_partitionkeyvalues = 1;
					#endif
					
					travstate_pp0.i_kvs = i + (v * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					readkeyvalues0(enable_readkeyvalues, 0, kvsourcedram  ,sourcebuffer00[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp0.i_kvs, travstate_pp0, PADDEDKVSOURCEDRAMSZ_KVS);
					readkeyvalues0(enable_readkeyvalues, 1, kvsourcedram  ,sourcebuffer10[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp0.i_kvs, travstate_pp0, PADDEDKVSOURCEDRAMSZ_KVS);
					#ifdef PARTITIONKEYVALUES_2READPIPELINES
					partitionkeyvalues00(enable_partitionkeyvalues, 0  ,sourcebuffer00[1]  ,destbuffer00  ,capsule00, travstate_pp1, llopparams);
					partitionkeyvalues01(enable_partitionkeyvalues, 1  ,sourcebuffer10[1]  ,destbuffer10  ,capsule10, travstate_pp1, llopparams);
					#endif 
					
					partitionkeyvalues00(1, 0  ,sourcebuffer00[0]  ,destbuffer00  ,capsule00, travstate_pp0, llopparams);
					partitionkeyvalues01(1, 1  ,sourcebuffer10[0]  ,destbuffer10  ,capsule10, travstate_pp0, llopparams);
					#ifdef PARTITIONKEYVALUES_2READPIPELINES
					travstate_pp1.i_kvs = i + ((v + 1) * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
					readkeyvalues0(1, 0, kvsourcedram  ,sourcebuffer00[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp1.i_kvs, travstate_pp1, PADDEDKVSOURCEDRAMSZ_KVS);
					readkeyvalues0(1, 1, kvsourcedram  ,sourcebuffer10[1]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp1.i_kvs, travstate_pp1, PADDEDKVSOURCEDRAMSZ_KVS);
					#endif 
				}
				#ifdef PARTITIONKEYVALUES_2READPIPELINES
				travstate_pp0.i_kvs = i + PVUpartitionparams.step_kvs + (0 * NUMWORKERS * NUMSUBWORKERS * SRCBUFFER_SIZE);
				readkeyvalues0(1, 0, kvsourcedram  ,sourcebuffer00[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp0.i_kvs, travstate_pp0, PADDEDKVSOURCEDRAMSZ_KVS);
				readkeyvalues0(1, 1, kvsourcedram  ,sourcebuffer10[0]  ,llopparams.baseaddr_worksourcekvs_kvs ,travstate_pp0.i_kvs, travstate_pp0, PADDEDKVSOURCEDRAMSZ_KVS);
				partitionkeyvalues00(1, 0  ,sourcebuffer00[1]  ,destbuffer00  ,capsule00, travstate_pp1, llopparams);
				partitionkeyvalues01(1, 1  ,sourcebuffer10[1]  ,destbuffer10  ,capsule10, travstate_pp1, llopparams);
				#endif 
				appendinvalids0(0  ,destbuffer00  ,capsule00);
				appendinvalids0(1  ,destbuffer10  ,capsule10);
				savepartitions0(0, kvsourcedram , destbuffer00 , capsule00, kvdeststats_tmp, llopparams);
				savepartitions0(1, kvsourcedram , destbuffer10 , capsule10, kvdeststats_tmp, llopparams);
				resetcapsules(capsule00, NAp);
				resetcapsules(capsule10, NAp);
			}
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->printkeyvalues("partitionandreduce::kvdeststats_tmp", kvdeststats_tmp, NUM_PARTITIONS);
			actsutilityobj->printvaluecount("partitionandreduce::kvdeststats_tmp", kvdeststats_tmp, NUM_PARTITIONS);
			actsutilityobj->printkeyvalues("partitionandreduce::kvdeststats_tmp2", kvdeststats_tmp2, NUM_PARTITIONS);
			actsutilityobj->printvaluecount("partitionandreduce::kvdeststats_tmp2", kvdeststats_tmp2, NUM_PARTITIONS);
			actsutilityobj->printkeyvalues("partitionandreduce::kvdeststats_tmp3", kvdeststats_tmp3, NUM_PARTITIONS);
			actsutilityobj->printvaluecount("partitionandreduce::kvdeststats_tmp3", kvdeststats_tmp3, NUM_PARTITIONS);
			#endif
			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalvar_inmemory_counttotalvalidkeyvalues(getvaluecount(kvdeststats_tmp3, NUM_PARTITIONS));
			#endif
			
			// Reduce phase
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("acts::partitionandreduce0 23", BASEOFFSET_STATSDRAM + sourcestatsmarker + NUMSUBWORKERS, KVSTATSDRAMSZ, NAp, NAp, NAp);
				#endif
				unsigned int maxsz = 0;
				for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats[i] = kvstats[BASEOFFSET_STATSDRAM + sourcestatsmarker + i]; }
				for(unsigned int i=0; i<NUMSUBWORKERS; i++){ sourcestats2[i] = sourcestats[i]; }
				for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ maxsz = max(sourcestats[sw].value, maxsz); }
				unsigned int maxsz_kvs = (maxsz + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
				#ifdef _DEBUGMODE_KERNELPRINTS
				for(unsigned int sw=0; sw<NUMSUBWORKERS; sw++){ actsutilityobj->print6("ReducePhase0:: begin", "size", "subworkerID", "currentLOP", "range_per_destpartition", "source_partition", sourcestats[sw].key, sourcestats[sw].value, sw, llopparams.currentLOP, NAp, source_partition); }
				#endif
				unsigned int voffset = source_partition * APPLYVERTEXBUFFERSZ; 

				readkeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, (llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * APPLYVERTEXBUFFERSZ_KVS), APPLYVERTEXBUFFERSZ_KVS, BATCH_RANGE_KVS);
				REDUCEPHASE_MAINLOOP: for (int i = 0; i < maxsz_kvs; i += 2 * SRCBUFFER_SIZE){
				#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_reducephase avg=analysis_reducephase
					#ifdef _DEBUGMODE_KERNELPRINTS
					actsutilityobj->print5("ReducePhase0:: i", "i_end", "NAp", "NAp", "skip size", i, maxsz_kvs, NAp, NAp, SRCBUFFER_SIZE);
					#endif
					
					if(i==0){ enable_reducekeyvalues = 0; } else { enable_reducekeyvalues = 1; }
					travstate_pp0.i_kvs = i;
					readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[0]  ,llopparams.baseaddr_worksourcekvs_kvs, sourcestats, travstate_pp0, PADDEDKVSOURCEDRAMSZ_KVS);
					reducepartitions0(enable_reducekeyvalues, 0  ,sourcebuffer00[1]   ,destbuffer00  ,voffset, sourcestats2, travstate_pp1, globalparams);
				
					reducepartitions0(1, 0  ,sourcebuffer00[0]   ,destbuffer00  ,voffset, sourcestats2, travstate_pp0, globalparams);
					travstate_pp1.i_kvs = i + SRCBUFFER_SIZE;
					readkeyvalues0(0, kvsourcedram  ,sourcebuffer00[1]  ,llopparams.baseaddr_worksourcekvs_kvs, sourcestats, travstate_pp1, PADDEDKVSOURCEDRAMSZ_KVS);
				}
				reducepartitions0(1, 0  ,sourcebuffer00[1]   ,destbuffer00  ,voffset, sourcestats2, travstate_pp1, globalparams);
				savekeyvalues0(kvdestdram, destbuffer00, globalparams.baseaddr_destkvs_kvs, ((llopparams.nextsourceoffset_kv / VECTOR_SIZE) + (0 * APPLYVERTEXBUFFERSZ_KVS)), APPLYVERTEXBUFFERSZ_KVS, BATCH_RANGE_KVS);
			}

			// updates
			#ifdef _DEBUGMODE_KERNELPRINTS
			actsutilityobj->print2("PartitionPhase0:: capsulemetadata.globaloffset", "capsulemetadata.localoffset", capsulemetadata.globaloffset, capsulemetadata.localoffset);
			#endif
			#ifdef ENABLE_REUSESAVEDCAPSULES
			keyvalue_t kvcapsulemetadata[1]; kvcapsulemetadata[0].key = capsulemetadata.globaloffset; kvcapsulemetadata[0].value = capsulemetadata.localoffset;
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 0 && globalparams.statsalreadycollected == 0){ savekeyvalues0(kvstats, kvcapsulemetadata, BASEOFFSET_CAPSULEMETADATA, sourcestatsmarker, 1); } 
			#endif 
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 0 && globalparams.statsalreadycollected == 0){ savekeyvalues0(kvstats, kvdeststats_tmp, BASEOFFSET_STATSDRAM, deststatsmarker, NUM_PARTITIONS); }
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 0){ deststatsmarker += NUM_PARTITIONS; }
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ sourcestatsmarker += NUMSUBWORKERS; }
			else { sourcestatsmarker += 1; }
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 1){ llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv * NUMSUBWORKERS; } 
			else { llopparams.nextsourceoffset_kv += llopparams.sourceskipsize_kv; }
			if(IsReducePhase(currentLOP, globalparams.treedepth, globalparams) == 0 && globalparams.statsalreadycollected == 0){ destoffset += (getvaluecount(kvdeststats_tmp, NUM_PARTITIONS) + (NUM_PARTITIONS * skipspacing)); }
			#ifdef ENABLE_REUSESAVEDCAPSULES
			if(globalparams.statsalreadycollected == 0){ capsulemetadata.globaloffset += capsulemetadata.localoffset; }
			#endif 
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("acts::partitionandreduce0 35", sourcestatsmarker, STATSDRAMSZ, NAp, NAp, NAp);
			actsutilityobj->checkoutofbounds("acts::partitionandreduce0 36", deststatsmarker, STATSDRAMSZ, NAp, NAp, NAp);
			#endif
		}
		#ifdef _DEBUGMODE_KERNELPRINTS2
		actsutilityobj->printglobalvars();
		actsutilityobj->clearglobalvars();
		#endif
	}
	return;
}

extern "C" {
void topkernel(
uint512_dt * kvsourcedramA
		,uint512_dt * kvdestdramA
		,keyvalue_t * kvstatsA
        ){
#pragma HLS INTERFACE m_axi port=kvsourcedramA offset=slave bundle=gmem0 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=kvdestdramA offset=slave bundle=gmem1 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE m_axi port=kvstatsA offset=slave bundle=gmem2

#pragma HLS INTERFACE s_axilite port=kvsourcedramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvdestdramA  bundle=control
#pragma HLS INTERFACE s_axilite port=kvstatsA  bundle=control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable=kvstatsA
	
	globalparams_t globalparams;
	globalparams.runkernelcommand = kvstatsA[getmessagesAddr(MESSAGES_RUNKERNELCOMMANDID)].key;
	globalparams.processcommand = kvstatsA[getmessagesAddr(MESSAGES_PROCESSCOMMANDID)].key;
	globalparams.collectstatscommand = kvstatsA[getmessagesAddr(MESSAGES_COLLECTSTATSCOMMANDID)].key;
	globalparams.partitioncommand = kvstatsA[getmessagesAddr(MESSAGES_PARTITIONCOMMANDID)].key;
	globalparams.reducecommand = kvstatsA[getmessagesAddr(MESSAGES_APPLYUPDATESCOMMANDID)].key;
	globalparams.vbegin = kvstatsA[getmessagesAddr(MESSAGES_VOFFSET)].key; 
	globalparams.vsize = kvstatsA[getmessagesAddr(MESSAGES_VSIZE)].key; 
	globalparams.treedepth = kvstatsA[getmessagesAddr(MESSAGES_TREEDEPTH)].key; 
	globalparams.LLOPnumpartitions = kvstatsA[getmessagesAddr(MESSAGES_FINALNUMPARTITIONS)].key;
	globalparams.GraphIter = kvstatsA[getmessagesAddr(MESSAGES_GRAPHITERATIONID)].key;
	globalparams.batchsize = kvstatsA[getmessagesAddr(MESSAGES_BATCHSIZE)].key;
	globalparams.runsize = kvstatsA[getmessagesAddr(MESSAGES_RUNSIZE)].key;
	globalparams.nextbatchoffset = kvstatsA[getmessagesAddr(MESSAGES_NEXTBATCHOFFSET)].key;
	globalparams.statsalreadycollected = kvstatsA[getmessagesAddr(MESSAGES_STATSALREADYCOLLECTED)].key; // false;
	globalparams.baseaddr_destkvs_kvs = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS2
	std::cout<<std::endl;
	std::cout<<"Kernel Started: globalparams.runkernelcommand: "<<globalparams.runkernelcommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.processcommand: "<<globalparams.processcommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.partitioncommand: "<<globalparams.partitioncommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.reducecommand: "<<globalparams.reducecommand<<std::endl;
	std::cout<<"Kernel Started: globalparams.vbegin: "<<globalparams.vbegin<<std::endl;
	std::cout<<"Kernel Started: globalparams.vsize: "<<globalparams.vsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.treedepth: "<<globalparams.treedepth<<std::endl;
	std::cout<<"Kernel Started: globalparams.LLOPnumpartitions: "<<globalparams.LLOPnumpartitions<<std::endl;
	std::cout<<"Kernel Started: globalparams.GraphIter: "<<globalparams.GraphIter<<std::endl;
	std::cout<<"Kernel Started: globalparams.batchsize: "<<globalparams.batchsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.runsize: "<<globalparams.runsize<<std::endl;
	std::cout<<"Kernel Started: globalparams.nextbatchoffset: "<<globalparams.nextbatchoffset<<std::endl;
	std::cout<<"Kernel Started: kvstatsA[BASEOFFSET_STATSDRAM + 0].value: "<<kvstatsA[BASEOFFSET_STATSDRAM + 0].value<<std::endl;
	#endif
	#ifdef _DEBUGMODE_KERNELPRINTS2
	actsutilityobj->printparameters();
	actsutilityobj->printglobalvars();
	actsutilityobj->clearglobalvars();
	#endif
	
	partitionandreduce0(kvsourcedramA, kvdestdramA, kvstatsA, globalparams);
	return;
}
}





