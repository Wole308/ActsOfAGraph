#include "mem_convert_and_access.h"
using namespace std;

#ifdef SW
mem_convert_and_access::mem_convert_and_access(mydebug * _mydebugobj){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(_mydebugobj); 
	mydebugobj = _mydebugobj; 
}
mem_convert_and_access::~mem_convert_and_access(){}
#endif

void acts_all::MEMCA_WRITEVDATATOBUFFER(unsigned int v, unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdata){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATATOBUFFER:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	#ifdef _WIDEWORD
	if(index%2==0){ buffer[v][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdata; }
	else{ buffer[v][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdata2; }
	#else 
	if(index%2==0){ buffer[v][index/2].key = vdata; }
	else{ buffer[v][index/2].value = vdata; }	
	#endif 
	return;
}

void acts_all::MEMCA_WRITEVDATASTOBUFFER(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE]){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCA_WRITEVDATASTOBUFFER:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
	if(index%2==0){ 
		buffer[0][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[0]; 
		buffer[1][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[1]; 
		buffer[2][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[2]; 
		buffer[3][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[3]; 
		buffer[4][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[4]; 
		buffer[5][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[5]; 
		buffer[6][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[6]; 
		buffer[7][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[7]; 
		buffer[8][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[8]; 
		buffer[9][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[9]; 
		buffer[10][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[10]; 
		buffer[11][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[11]; 
		buffer[12][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[12]; 
		buffer[13][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[13]; 
		buffer[14][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[14]; 
		buffer[15][index/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[15]; 
	} else {
		buffer[0][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[0];
		buffer[1][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[1];
		buffer[2][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[2];
		buffer[3][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[3];
		buffer[4][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[4];
		buffer[5][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[5];
		buffer[6][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[6];
		buffer[7][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[7];
		buffer[8][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[8];
		buffer[9][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[9];
		buffer[10][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[10];
		buffer[11][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[11];
		buffer[12][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[12];
		buffer[13][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[13];
		buffer[14][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[14];
		buffer[15][index/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[15];
	}
	#else 
	if(index%2==0){ 
		buffer[0][index/2].key = vdatas[0];
		buffer[1][index/2].key = vdatas[1];
		buffer[2][index/2].key = vdatas[2];
		buffer[3][index/2].key = vdatas[3];
		buffer[4][index/2].key = vdatas[4];
		buffer[5][index/2].key = vdatas[5];
		buffer[6][index/2].key = vdatas[6];
		buffer[7][index/2].key = vdatas[7];
		buffer[8][index/2].key = vdatas[8];
		buffer[9][index/2].key = vdatas[9];
		buffer[10][index/2].key = vdatas[10];
		buffer[11][index/2].key = vdatas[11];
		buffer[12][index/2].key = vdatas[12];
		buffer[13][index/2].key = vdatas[13];
		buffer[14][index/2].key = vdatas[14];
		buffer[15][index/2].key = vdatas[15];
	} else {
		buffer[0][index/2].value = vdatas[0];
		buffer[1][index/2].value = vdatas[1];
		buffer[2][index/2].value = vdatas[2];
		buffer[3][index/2].value = vdatas[3];
		buffer[4][index/2].value = vdatas[4];
		buffer[5][index/2].value = vdatas[5];
		buffer[6][index/2].value = vdatas[6];
		buffer[7][index/2].value = vdatas[7];
		buffer[8][index/2].value = vdatas[8];
		buffer[9][index/2].value = vdatas[9];
		buffer[10][index/2].value = vdatas[10];
		buffer[11][index/2].value = vdatas[11];
		buffer[12][index/2].value = vdatas[12];
		buffer[13][index/2].value = vdatas[13];
		buffer[14][index/2].value = vdatas[14];
		buffer[15][index/2].value = vdatas[15];
	}
	#endif 
	return;
}

void acts_all::MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VDATA_PACKINGSIZE]){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[0]/2, BLOCKRAM_SIZE, indexes[0], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[1]/2, BLOCKRAM_SIZE, indexes[1], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[2]/2, BLOCKRAM_SIZE, indexes[2], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[3]/2, BLOCKRAM_SIZE, indexes[3], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[4]/2, BLOCKRAM_SIZE, indexes[4], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[5]/2, BLOCKRAM_SIZE, indexes[5], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[6]/2, BLOCKRAM_SIZE, indexes[6], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[7]/2, BLOCKRAM_SIZE, indexes[7], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[8]/2, BLOCKRAM_SIZE, indexes[8], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[9]/2, BLOCKRAM_SIZE, indexes[9], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[10]/2, BLOCKRAM_SIZE, indexes[10], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[11]/2, BLOCKRAM_SIZE, indexes[11], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[12]/2, BLOCKRAM_SIZE, indexes[12], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[13]/2, BLOCKRAM_SIZE, indexes[13], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[14]/2, BLOCKRAM_SIZE, indexes[14], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[15]/2, BLOCKRAM_SIZE, indexes[15], NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
		if(indexes[0]%2==0){ buffer[0][indexes[0]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[0]; } 
		else{ buffer[0][indexes[0]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[0]; }
		if(indexes[1]%2==0){ buffer[1][indexes[1]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[1]; } 
		else{ buffer[1][indexes[1]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[1]; }
		if(indexes[2]%2==0){ buffer[2][indexes[2]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[2]; } 
		else{ buffer[2][indexes[2]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[2]; }
		if(indexes[3]%2==0){ buffer[3][indexes[3]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[3]; } 
		else{ buffer[3][indexes[3]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[3]; }
		if(indexes[4]%2==0){ buffer[4][indexes[4]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[4]; } 
		else{ buffer[4][indexes[4]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[4]; }
		if(indexes[5]%2==0){ buffer[5][indexes[5]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[5]; } 
		else{ buffer[5][indexes[5]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[5]; }
		if(indexes[6]%2==0){ buffer[6][indexes[6]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[6]; } 
		else{ buffer[6][indexes[6]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[6]; }
		if(indexes[7]%2==0){ buffer[7][indexes[7]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[7]; } 
		else{ buffer[7][indexes[7]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[7]; }
		if(indexes[8]%2==0){ buffer[8][indexes[8]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[8]; } 
		else{ buffer[8][indexes[8]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[8]; }
		if(indexes[9]%2==0){ buffer[9][indexes[9]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[9]; } 
		else{ buffer[9][indexes[9]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[9]; }
		if(indexes[10]%2==0){ buffer[10][indexes[10]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[10]; } 
		else{ buffer[10][indexes[10]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[10]; }
		if(indexes[11]%2==0){ buffer[11][indexes[11]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[11]; } 
		else{ buffer[11][indexes[11]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[11]; }
		if(indexes[12]%2==0){ buffer[12][indexes[12]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[12]; } 
		else{ buffer[12][indexes[12]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[12]; }
		if(indexes[13]%2==0){ buffer[13][indexes[13]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[13]; } 
		else{ buffer[13][indexes[13]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[13]; }
		if(indexes[14]%2==0){ buffer[14][indexes[14]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[14]; } 
		else{ buffer[14][indexes[14]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[14]; }
		if(indexes[15]%2==0){ buffer[15][indexes[15]/2].range(SIZEOF_VDATAKEY - 1, 0) = vdatas[15]; } 
		else{ buffer[15][indexes[15]/2].range(SIZEOF_VDATAKEY + SIZEOF_VDATAVALUE - 1, SIZEOF_VDATAKEY) = vdatas[15]; }
	
	#else 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[0]/2, BLOCKRAM_SIZE, indexes[0], NAp, NAp);
		#endif
		if(indexes[0]%2==0){ buffer[0][indexes[0]/2].key = vdatas[0]; } 
		else{ buffer[0][indexes[0]/2].value = vdatas[0]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[1]/2, BLOCKRAM_SIZE, indexes[1], NAp, NAp);
		#endif
		if(indexes[1]%2==0){ buffer[1][indexes[1]/2].key = vdatas[1]; } 
		else{ buffer[1][indexes[1]/2].value = vdatas[1]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[2]/2, BLOCKRAM_SIZE, indexes[2], NAp, NAp);
		#endif
		if(indexes[2]%2==0){ buffer[2][indexes[2]/2].key = vdatas[2]; } 
		else{ buffer[2][indexes[2]/2].value = vdatas[2]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[3]/2, BLOCKRAM_SIZE, indexes[3], NAp, NAp);
		#endif
		if(indexes[3]%2==0){ buffer[3][indexes[3]/2].key = vdatas[3]; } 
		else{ buffer[3][indexes[3]/2].value = vdatas[3]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[4]/2, BLOCKRAM_SIZE, indexes[4], NAp, NAp);
		#endif
		if(indexes[4]%2==0){ buffer[4][indexes[4]/2].key = vdatas[4]; } 
		else{ buffer[4][indexes[4]/2].value = vdatas[4]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[5]/2, BLOCKRAM_SIZE, indexes[5], NAp, NAp);
		#endif
		if(indexes[5]%2==0){ buffer[5][indexes[5]/2].key = vdatas[5]; } 
		else{ buffer[5][indexes[5]/2].value = vdatas[5]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[6]/2, BLOCKRAM_SIZE, indexes[6], NAp, NAp);
		#endif
		if(indexes[6]%2==0){ buffer[6][indexes[6]/2].key = vdatas[6]; } 
		else{ buffer[6][indexes[6]/2].value = vdatas[6]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[7]/2, BLOCKRAM_SIZE, indexes[7], NAp, NAp);
		#endif
		if(indexes[7]%2==0){ buffer[7][indexes[7]/2].key = vdatas[7]; } 
		else{ buffer[7][indexes[7]/2].value = vdatas[7]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[8]/2, BLOCKRAM_SIZE, indexes[8], NAp, NAp);
		#endif
		if(indexes[8]%2==0){ buffer[8][indexes[8]/2].key = vdatas[8]; } 
		else{ buffer[8][indexes[8]/2].value = vdatas[8]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[9]/2, BLOCKRAM_SIZE, indexes[9], NAp, NAp);
		#endif
		if(indexes[9]%2==0){ buffer[9][indexes[9]/2].key = vdatas[9]; } 
		else{ buffer[9][indexes[9]/2].value = vdatas[9]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[10]/2, BLOCKRAM_SIZE, indexes[10], NAp, NAp);
		#endif
		if(indexes[10]%2==0){ buffer[10][indexes[10]/2].key = vdatas[10]; } 
		else{ buffer[10][indexes[10]/2].value = vdatas[10]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[11]/2, BLOCKRAM_SIZE, indexes[11], NAp, NAp);
		#endif
		if(indexes[11]%2==0){ buffer[11][indexes[11]/2].key = vdatas[11]; } 
		else{ buffer[11][indexes[11]/2].value = vdatas[11]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[12]/2, BLOCKRAM_SIZE, indexes[12], NAp, NAp);
		#endif
		if(indexes[12]%2==0){ buffer[12][indexes[12]/2].key = vdatas[12]; } 
		else{ buffer[12][indexes[12]/2].value = vdatas[12]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[13]/2, BLOCKRAM_SIZE, indexes[13], NAp, NAp);
		#endif
		if(indexes[13]%2==0){ buffer[13][indexes[13]/2].key = vdatas[13]; } 
		else{ buffer[13][indexes[13]/2].value = vdatas[13]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[14]/2, BLOCKRAM_SIZE, indexes[14], NAp, NAp);
		#endif
		if(indexes[14]%2==0){ buffer[14][indexes[14]/2].key = vdatas[14]; } 
		else{ buffer[14][indexes[14]/2].value = vdatas[14]; }
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVDATASTOBUFFER_WITHDEPTHS:", indexes[15]/2, BLOCKRAM_SIZE, indexes[15], NAp, NAp);
		#endif
		if(indexes[15]%2==0){ buffer[15][indexes[15]/2].key = vdatas[15]; } 
		else{ buffer[15][indexes[15]/2].value = vdatas[15]; }
	#endif 
	return;
}

void acts_all::MEMCA_WRITEVMASKSTOBUFFER(unsigned int index, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE]){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS[0][index] = vmdatas[0];
	vmaskBITS[1][index] = vmdatas[1];
	vmaskBITS[2][index] = vmdatas[2];
	vmaskBITS[3][index] = vmdatas[3];
	vmaskBITS[4][index] = vmdatas[4];
	vmaskBITS[5][index] = vmdatas[5];
	vmaskBITS[6][index] = vmdatas[6];
	vmaskBITS[7][index] = vmdatas[7];
	vmaskBITS[8][index] = vmdatas[8];
	vmaskBITS[9][index] = vmdatas[9];
	vmaskBITS[10][index] = vmdatas[10];
	vmaskBITS[11][index] = vmdatas[11];
	vmaskBITS[12][index] = vmdatas[12];
	vmaskBITS[13][index] = vmdatas[13];
	vmaskBITS[14][index] = vmdatas[14];
	vmaskBITS[15][index] = vmdatas[15];
	return;
}

void acts_all::MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS(unsigned int indexes[VDATA_PACKINGSIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE]){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[0], DOUBLE_BLOCKRAM_SIZE, indexes[0], NAp, NAp);
	#endif
	vmaskBITS[0][indexes[0]] = vmdatas[0];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[1], DOUBLE_BLOCKRAM_SIZE, indexes[1], NAp, NAp);
	#endif
	vmaskBITS[1][indexes[1]] = vmdatas[1];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[2], DOUBLE_BLOCKRAM_SIZE, indexes[2], NAp, NAp);
	#endif
	vmaskBITS[2][indexes[2]] = vmdatas[2];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[3], DOUBLE_BLOCKRAM_SIZE, indexes[3], NAp, NAp);
	#endif
	vmaskBITS[3][indexes[3]] = vmdatas[3];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[4], DOUBLE_BLOCKRAM_SIZE, indexes[4], NAp, NAp);
	#endif
	vmaskBITS[4][indexes[4]] = vmdatas[4];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[5], DOUBLE_BLOCKRAM_SIZE, indexes[5], NAp, NAp);
	#endif
	vmaskBITS[5][indexes[5]] = vmdatas[5];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[6], DOUBLE_BLOCKRAM_SIZE, indexes[6], NAp, NAp);
	#endif
	vmaskBITS[6][indexes[6]] = vmdatas[6];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[7], DOUBLE_BLOCKRAM_SIZE, indexes[7], NAp, NAp);
	#endif
	vmaskBITS[7][indexes[7]] = vmdatas[7];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[8], DOUBLE_BLOCKRAM_SIZE, indexes[8], NAp, NAp);
	#endif
	vmaskBITS[8][indexes[8]] = vmdatas[8];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[9], DOUBLE_BLOCKRAM_SIZE, indexes[9], NAp, NAp);
	#endif
	vmaskBITS[9][indexes[9]] = vmdatas[9];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[10], DOUBLE_BLOCKRAM_SIZE, indexes[10], NAp, NAp);
	#endif
	vmaskBITS[10][indexes[10]] = vmdatas[10];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[11], DOUBLE_BLOCKRAM_SIZE, indexes[11], NAp, NAp);
	#endif
	vmaskBITS[11][indexes[11]] = vmdatas[11];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[12], DOUBLE_BLOCKRAM_SIZE, indexes[12], NAp, NAp);
	#endif
	vmaskBITS[12][indexes[12]] = vmdatas[12];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[13], DOUBLE_BLOCKRAM_SIZE, indexes[13], NAp, NAp);
	#endif
	vmaskBITS[13][indexes[13]] = vmdatas[13];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[14], DOUBLE_BLOCKRAM_SIZE, indexes[14], NAp, NAp);
	#endif
	vmaskBITS[14][indexes[14]] = vmdatas[14];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITEVMASKSTOBUFFER_WITHDEPTHS:", indexes[15], DOUBLE_BLOCKRAM_SIZE, indexes[15], NAp, NAp);
	#endif
	vmaskBITS[15][indexes[15]] = vmdatas[15];
	return;
}
