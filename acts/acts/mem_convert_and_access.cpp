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

// vdata 
// vdata:: used in {reduceupdates.cpp}
void acts_all::MEMCA_WRITETOBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VDATA:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif

	#ifdef _WIDEWORD
	if(index%2==0){
UTIL_WRITEBITSTO_UINTV(&buffer[bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdata);
	} else{
UTIL_WRITEBITSTO_UINTV(&buffer[bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdata);
	}
	#else 
	if(index%2==0){
UTIL_WRITEBITSTO_UINTV(&buffer[bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdata);
	} else{
UTIL_WRITEBITSTO_UINTV(&buffer[bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1, vdata);
	}	
	#endif 
	return;
}

// vdata:: used in {dispatch_reduce -> mem_access_splitdstvxs.cpp -> MEMACCESS_SPL_readvdata}
void acts_all::MEMCA_WRITETOBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCA_WRITETOBUFFER_VDATAS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
	if(index%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[0][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[0][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[0]);
UTIL_WRITEBITSTO_UINTV(&buffer[1][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[1][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[1]);
UTIL_WRITEBITSTO_UINTV(&buffer[2][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[2][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[2]);
UTIL_WRITEBITSTO_UINTV(&buffer[3][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[3][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[3]);
UTIL_WRITEBITSTO_UINTV(&buffer[4][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[4][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[4]);
UTIL_WRITEBITSTO_UINTV(&buffer[5][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[5][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[5]);
UTIL_WRITEBITSTO_UINTV(&buffer[6][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[6][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[6]);
UTIL_WRITEBITSTO_UINTV(&buffer[7][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[7][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[7]);
UTIL_WRITEBITSTO_UINTV(&buffer[8][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[8][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[8]);
UTIL_WRITEBITSTO_UINTV(&buffer[9][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[9][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[9]);
UTIL_WRITEBITSTO_UINTV(&buffer[10][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[10][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[10]);
UTIL_WRITEBITSTO_UINTV(&buffer[11][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[11][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[11]);
UTIL_WRITEBITSTO_UINTV(&buffer[12][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[12][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[12]);
UTIL_WRITEBITSTO_UINTV(&buffer[13][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[13][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[13]);
UTIL_WRITEBITSTO_UINTV(&buffer[14][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[14][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[14]);
UTIL_WRITEBITSTO_UINTV(&buffer[15][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[15][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdatas[15]);
	} else {
UTIL_WRITEBITSTO_UINTV(&buffer[0][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[0][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[0]);
UTIL_WRITEBITSTO_UINTV(&buffer[1][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[1][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[1]);
UTIL_WRITEBITSTO_UINTV(&buffer[2][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[2][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[2]);
UTIL_WRITEBITSTO_UINTV(&buffer[3][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[3][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[3]);
UTIL_WRITEBITSTO_UINTV(&buffer[4][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[4][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[4]);
UTIL_WRITEBITSTO_UINTV(&buffer[5][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[5][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[5]);
UTIL_WRITEBITSTO_UINTV(&buffer[6][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[6][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[6]);
UTIL_WRITEBITSTO_UINTV(&buffer[7][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[7][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[7]);
UTIL_WRITEBITSTO_UINTV(&buffer[8][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[8][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[8]);
UTIL_WRITEBITSTO_UINTV(&buffer[9][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[9][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[9]);
UTIL_WRITEBITSTO_UINTV(&buffer[10][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[10][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[10]);
UTIL_WRITEBITSTO_UINTV(&buffer[11][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[11][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[11]);
UTIL_WRITEBITSTO_UINTV(&buffer[12][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[12][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[12]);
UTIL_WRITEBITSTO_UINTV(&buffer[13][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[13][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[13]);
UTIL_WRITEBITSTO_UINTV(&buffer[14][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[14][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[14]);
UTIL_WRITEBITSTO_UINTV(&buffer[15][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, 0);
UTIL_WRITEBITSTO_UINTV(&buffer[15][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdatas[15]);
	}
	#else 
	if(index%2==0){ 
		buffer[0][bufferoffset_kvs + index/2].key = 0; 
		buffer[1][bufferoffset_kvs + index/2].key = 0; 
		buffer[2][bufferoffset_kvs + index/2].key = 0; 
		buffer[3][bufferoffset_kvs + index/2].key = 0; 
		buffer[4][bufferoffset_kvs + index/2].key = 0; 
		buffer[5][bufferoffset_kvs + index/2].key = 0; 
		buffer[6][bufferoffset_kvs + index/2].key = 0; 
		buffer[7][bufferoffset_kvs + index/2].key = 0; 
		buffer[8][bufferoffset_kvs + index/2].key = 0; 
		buffer[9][bufferoffset_kvs + index/2].key = 0; 
		buffer[10][bufferoffset_kvs + index/2].key = 0; 
		buffer[11][bufferoffset_kvs + index/2].key = 0; 
		buffer[12][bufferoffset_kvs + index/2].key = 0; 
		buffer[13][bufferoffset_kvs + index/2].key = 0; 
		buffer[14][bufferoffset_kvs + index/2].key = 0; 
		buffer[15][bufferoffset_kvs + index/2].key = 0; 
	
UTIL_WRITETO_UINT(&buffer[0][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[0][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[0]);
	
UTIL_WRITETO_UINT(&buffer[1][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[1][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[1]);
	
UTIL_WRITETO_UINT(&buffer[2][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[2][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[2]);
	
UTIL_WRITETO_UINT(&buffer[3][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[3][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[3]);
	
UTIL_WRITETO_UINT(&buffer[4][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[4][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[4]);
	
UTIL_WRITETO_UINT(&buffer[5][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[5][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[5]);
	
UTIL_WRITETO_UINT(&buffer[6][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[6][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[6]);
	
UTIL_WRITETO_UINT(&buffer[7][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[7][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[7]);
	
UTIL_WRITETO_UINT(&buffer[8][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[8][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[8]);
	
UTIL_WRITETO_UINT(&buffer[9][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[9][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[9]);
	
UTIL_WRITETO_UINT(&buffer[10][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[10][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[10]);
	
UTIL_WRITETO_UINT(&buffer[11][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[11][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[11]);
	
UTIL_WRITETO_UINT(&buffer[12][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[12][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[12]);
	
UTIL_WRITETO_UINT(&buffer[13][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[13][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[13]);
	
UTIL_WRITETO_UINT(&buffer[14][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[14][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[14]);
	
UTIL_WRITETO_UINT(&buffer[15][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[15][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdatas[15]);
	} else {
		buffer[0][bufferoffset_kvs + index/2].value = 0; 
		buffer[1][bufferoffset_kvs + index/2].value = 0; 
		buffer[2][bufferoffset_kvs + index/2].value = 0; 
		buffer[3][bufferoffset_kvs + index/2].value = 0; 
		buffer[4][bufferoffset_kvs + index/2].value = 0; 
		buffer[5][bufferoffset_kvs + index/2].value = 0; 
		buffer[6][bufferoffset_kvs + index/2].value = 0; 
		buffer[7][bufferoffset_kvs + index/2].value = 0; 
		buffer[8][bufferoffset_kvs + index/2].value = 0; 
		buffer[9][bufferoffset_kvs + index/2].value = 0; 
		buffer[10][bufferoffset_kvs + index/2].value = 0; 
		buffer[11][bufferoffset_kvs + index/2].value = 0; 
		buffer[12][bufferoffset_kvs + index/2].value = 0; 
		buffer[13][bufferoffset_kvs + index/2].value = 0; 
		buffer[14][bufferoffset_kvs + index/2].value = 0; 
		buffer[15][bufferoffset_kvs + index/2].value = 0; 
UTIL_WRITETO_UINT(&buffer[0][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[0][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[0]);
UTIL_WRITETO_UINT(&buffer[1][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[1][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[1]);
UTIL_WRITETO_UINT(&buffer[2][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[2][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[2]);
UTIL_WRITETO_UINT(&buffer[3][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[3][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[3]);
UTIL_WRITETO_UINT(&buffer[4][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[4][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[4]);
UTIL_WRITETO_UINT(&buffer[5][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[5][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[5]);
UTIL_WRITETO_UINT(&buffer[6][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[6][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[6]);
UTIL_WRITETO_UINT(&buffer[7][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[7][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[7]);
UTIL_WRITETO_UINT(&buffer[8][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[8][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[8]);
UTIL_WRITETO_UINT(&buffer[9][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[9][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[9]);
UTIL_WRITETO_UINT(&buffer[10][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[10][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[10]);
UTIL_WRITETO_UINT(&buffer[11][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[11][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[11]);
UTIL_WRITETO_UINT(&buffer[12][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[12][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[12]);
UTIL_WRITETO_UINT(&buffer[13][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[13][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[13]);
UTIL_WRITETO_UINT(&buffer[14][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[14][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[14]);
UTIL_WRITETO_UINT(&buffer[15][bufferoffset_kvs + index/2].value, SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
UTIL_WRITETO_UINT(&buffer[15][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA0, vdatas[15]);
	}	
	#endif
	return;
}

// vdata:: used in {reduceupdates.cpp, processedges_splitdstvxs.cpp}
value_t acts_all::MEMCA_READFROMBUFFER_VDATA(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCA_READFROMBUFFER_VDATA:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	value_t vdata = 0;
	#ifdef _WIDEWORD
	if(index%2==0){ 
		vdata = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	} else{ 
		vdata = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	}
	#else 
	if(index%2==0){ 
		vdata = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
	} else{ 
		vdata = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
	}	
	#endif 
	return vdata;
}

// vdata:: used in {processedges_splitdstvxs.cpp} // soon obsolete
void acts_all::MEMCA_READFROMBUFFER_VDATAS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCA_READFROMBUFFER_VDATAS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vdatas[0] = 0; 
	vdatas[1] = 0; 
	vdatas[2] = 0; 
	vdatas[3] = 0; 
	vdatas[4] = 0; 
	vdatas[5] = 0; 
	vdatas[6] = 0; 
	vdatas[7] = 0; 
	vdatas[8] = 0; 
	vdatas[9] = 0; 
	vdatas[10] = 0; 
	vdatas[11] = 0; 
	vdatas[12] = 0; 
	vdatas[13] = 0; 
	vdatas[14] = 0; 
	vdatas[15] = 0; 
	
	#ifdef _WIDEWORD
	if(index%2==0){ 
		vdata = UTIL_READBITSFROM_UINTV(buffer[0][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[1][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[2][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[3][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[4][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[5][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[6][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[7][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[8][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[9][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[10][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[11][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[12][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[13][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[14][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
		vdata = UTIL_READBITSFROM_UINTV(buffer[15][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	} else {
		vdata = UTIL_READBITSFROM_UINTV(buffer[0][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[1][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[2][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[3][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[4][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[5][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[6][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[7][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[8][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[9][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[10][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[11][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[12][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[13][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[14][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
		vdata = UTIL_READBITSFROM_UINTV(buffer[15][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	}
	#else 
	if(index%2==0){ 
		vdatas[0] = UTIL_READBITSFROM_UINTV(buffer[0][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[1] = UTIL_READBITSFROM_UINTV(buffer[1][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[2] = UTIL_READBITSFROM_UINTV(buffer[2][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[3] = UTIL_READBITSFROM_UINTV(buffer[3][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[4] = UTIL_READBITSFROM_UINTV(buffer[4][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[5] = UTIL_READBITSFROM_UINTV(buffer[5][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[6] = UTIL_READBITSFROM_UINTV(buffer[6][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[7] = UTIL_READBITSFROM_UINTV(buffer[7][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[8] = UTIL_READBITSFROM_UINTV(buffer[8][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[9] = UTIL_READBITSFROM_UINTV(buffer[9][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[10] = UTIL_READBITSFROM_UINTV(buffer[10][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[11] = UTIL_READBITSFROM_UINTV(buffer[11][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[12] = UTIL_READBITSFROM_UINTV(buffer[12][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[13] = UTIL_READBITSFROM_UINTV(buffer[13][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[14] = UTIL_READBITSFROM_UINTV(buffer[14][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vdatas[15] = UTIL_READBITSFROM_UINTV(buffer[15][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
	} else {
		vdatas[0] = UTIL_READBITSFROM_UINTV(buffer[0][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[1] = UTIL_READBITSFROM_UINTV(buffer[1][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[2] = UTIL_READBITSFROM_UINTV(buffer[2][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[3] = UTIL_READBITSFROM_UINTV(buffer[3][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[4] = UTIL_READBITSFROM_UINTV(buffer[4][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[5] = UTIL_READBITSFROM_UINTV(buffer[5][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[6] = UTIL_READBITSFROM_UINTV(buffer[6][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[7] = UTIL_READBITSFROM_UINTV(buffer[7][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[8] = UTIL_READBITSFROM_UINTV(buffer[8][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[9] = UTIL_READBITSFROM_UINTV(buffer[9][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[10] = UTIL_READBITSFROM_UINTV(buffer[10][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[11] = UTIL_READBITSFROM_UINTV(buffer[11][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[12] = UTIL_READBITSFROM_UINTV(buffer[12][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[13] = UTIL_READBITSFROM_UINTV(buffer[13][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[14] = UTIL_READBITSFROM_UINTV(buffer[14][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vdatas[15] = UTIL_READBITSFROM_UINTV(buffer[15][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
	}
	#endif 
	return;
}

// vmasks 
unit1_type acts_all::MEMCA_READFROMBUFFER_VMASK(unsigned int index, unit1_type vmaskBITS[DOUBLE_BLOCKRAM_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_READFROMBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	unit1_type vmdata = vmaskBITS[bufferoffset_kvs + index];
	return vmdata;
}

void acts_all::MEMCA_READFROMBUFFER_VMASKS(unsigned int index, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_READFROMBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmdatas[0] = vmaskBITS[0][bufferoffset_kvs + index];
	vmdatas[1] = vmaskBITS[1][bufferoffset_kvs + index];
	vmdatas[2] = vmaskBITS[2][bufferoffset_kvs + index];
	vmdatas[3] = vmaskBITS[3][bufferoffset_kvs + index];
	vmdatas[4] = vmaskBITS[4][bufferoffset_kvs + index];
	vmdatas[5] = vmaskBITS[5][bufferoffset_kvs + index];
	vmdatas[6] = vmaskBITS[6][bufferoffset_kvs + index];
	vmdatas[7] = vmaskBITS[7][bufferoffset_kvs + index];
	vmdatas[8] = vmaskBITS[8][bufferoffset_kvs + index];
	vmdatas[9] = vmaskBITS[9][bufferoffset_kvs + index];
	vmdatas[10] = vmaskBITS[10][bufferoffset_kvs + index];
	vmdatas[11] = vmaskBITS[11][bufferoffset_kvs + index];
	vmdatas[12] = vmaskBITS[12][bufferoffset_kvs + index];
	vmdatas[13] = vmaskBITS[13][bufferoffset_kvs + index];
	vmdatas[14] = vmaskBITS[14][bufferoffset_kvs + index];
	vmdatas[15] = vmaskBITS[15][bufferoffset_kvs + index];
	return;
}

void acts_all::MEMCA_WRITETOBUFFER_VMASKS(unsigned int index, unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS:", index, DOUBLE_BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	vmaskBITS[0][bufferoffset_kvs + index] = vmdatas[0];
	vmaskBITS[1][bufferoffset_kvs + index] = vmdatas[1];
	vmaskBITS[2][bufferoffset_kvs + index] = vmdatas[2];
	vmaskBITS[3][bufferoffset_kvs + index] = vmdatas[3];
	vmaskBITS[4][bufferoffset_kvs + index] = vmdatas[4];
	vmaskBITS[5][bufferoffset_kvs + index] = vmdatas[5];
	vmaskBITS[6][bufferoffset_kvs + index] = vmdatas[6];
	vmaskBITS[7][bufferoffset_kvs + index] = vmdatas[7];
	vmaskBITS[8][bufferoffset_kvs + index] = vmdatas[8];
	vmaskBITS[9][bufferoffset_kvs + index] = vmdatas[9];
	vmaskBITS[10][bufferoffset_kvs + index] = vmdatas[10];
	vmaskBITS[11][bufferoffset_kvs + index] = vmdatas[11];
	vmaskBITS[12][bufferoffset_kvs + index] = vmdatas[12];
	vmaskBITS[13][bufferoffset_kvs + index] = vmdatas[13];
	vmaskBITS[14][bufferoffset_kvs + index] = vmdatas[14];
	vmaskBITS[15][bufferoffset_kvs + index] = vmdatas[15];
	return;
}

void acts_all::MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS(unsigned int indexes[VDATA_PACKINGSIZE], unit1_type vmaskBITS[VMASK_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[0], DOUBLE_BLOCKRAM_SIZE, indexes[0], NAp, NAp);
	#endif
	vmaskBITS[0][bufferoffset_kvs + indexes[0]] = vmdatas[0];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[1], DOUBLE_BLOCKRAM_SIZE, indexes[1], NAp, NAp);
	#endif
	vmaskBITS[1][bufferoffset_kvs + indexes[1]] = vmdatas[1];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[2], DOUBLE_BLOCKRAM_SIZE, indexes[2], NAp, NAp);
	#endif
	vmaskBITS[2][bufferoffset_kvs + indexes[2]] = vmdatas[2];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[3], DOUBLE_BLOCKRAM_SIZE, indexes[3], NAp, NAp);
	#endif
	vmaskBITS[3][bufferoffset_kvs + indexes[3]] = vmdatas[3];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[4], DOUBLE_BLOCKRAM_SIZE, indexes[4], NAp, NAp);
	#endif
	vmaskBITS[4][bufferoffset_kvs + indexes[4]] = vmdatas[4];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[5], DOUBLE_BLOCKRAM_SIZE, indexes[5], NAp, NAp);
	#endif
	vmaskBITS[5][bufferoffset_kvs + indexes[5]] = vmdatas[5];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[6], DOUBLE_BLOCKRAM_SIZE, indexes[6], NAp, NAp);
	#endif
	vmaskBITS[6][bufferoffset_kvs + indexes[6]] = vmdatas[6];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[7], DOUBLE_BLOCKRAM_SIZE, indexes[7], NAp, NAp);
	#endif
	vmaskBITS[7][bufferoffset_kvs + indexes[7]] = vmdatas[7];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[8], DOUBLE_BLOCKRAM_SIZE, indexes[8], NAp, NAp);
	#endif
	vmaskBITS[8][bufferoffset_kvs + indexes[8]] = vmdatas[8];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[9], DOUBLE_BLOCKRAM_SIZE, indexes[9], NAp, NAp);
	#endif
	vmaskBITS[9][bufferoffset_kvs + indexes[9]] = vmdatas[9];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[10], DOUBLE_BLOCKRAM_SIZE, indexes[10], NAp, NAp);
	#endif
	vmaskBITS[10][bufferoffset_kvs + indexes[10]] = vmdatas[10];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[11], DOUBLE_BLOCKRAM_SIZE, indexes[11], NAp, NAp);
	#endif
	vmaskBITS[11][bufferoffset_kvs + indexes[11]] = vmdatas[11];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[12], DOUBLE_BLOCKRAM_SIZE, indexes[12], NAp, NAp);
	#endif
	vmaskBITS[12][bufferoffset_kvs + indexes[12]] = vmdatas[12];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[13], DOUBLE_BLOCKRAM_SIZE, indexes[13], NAp, NAp);
	#endif
	vmaskBITS[13][bufferoffset_kvs + indexes[13]] = vmdatas[13];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[14], DOUBLE_BLOCKRAM_SIZE, indexes[14], NAp, NAp);
	#endif
	vmaskBITS[14][bufferoffset_kvs + indexes[14]] = vmdatas[14];
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VMASKS_WITHDEPTHS:", indexes[15], DOUBLE_BLOCKRAM_SIZE, indexes[15], NAp, NAp);
	#endif
	vmaskBITS[15][bufferoffset_kvs + indexes[15]] = vmdatas[15];
	return;
}

// vdata & vmasks 
void acts_all::MEMCA_READFROMBUFFER_VDATASWITHVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE], unit1_type vmdatas[VMASK_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCA_READFROMBUFFER_VDATASWITHVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
	if(index%2==0){ 
	
		vmdatas[0] = UTIL_READBITSFROM_UINTV(buffer[0][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[0] = UTIL_READBITSFROM_UINTV(buffer[0][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[1] = UTIL_READBITSFROM_UINTV(buffer[1][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[1] = UTIL_READBITSFROM_UINTV(buffer[1][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[2] = UTIL_READBITSFROM_UINTV(buffer[2][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[2] = UTIL_READBITSFROM_UINTV(buffer[2][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[3] = UTIL_READBITSFROM_UINTV(buffer[3][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[3] = UTIL_READBITSFROM_UINTV(buffer[3][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[4] = UTIL_READBITSFROM_UINTV(buffer[4][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[4] = UTIL_READBITSFROM_UINTV(buffer[4][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[5] = UTIL_READBITSFROM_UINTV(buffer[5][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[5] = UTIL_READBITSFROM_UINTV(buffer[5][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[6] = UTIL_READBITSFROM_UINTV(buffer[6][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[6] = UTIL_READBITSFROM_UINTV(buffer[6][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[7] = UTIL_READBITSFROM_UINTV(buffer[7][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[7] = UTIL_READBITSFROM_UINTV(buffer[7][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[8] = UTIL_READBITSFROM_UINTV(buffer[8][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[8] = UTIL_READBITSFROM_UINTV(buffer[8][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[9] = UTIL_READBITSFROM_UINTV(buffer[9][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[9] = UTIL_READBITSFROM_UINTV(buffer[9][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[10] = UTIL_READBITSFROM_UINTV(buffer[10][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[10] = UTIL_READBITSFROM_UINTV(buffer[10][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[11] = UTIL_READBITSFROM_UINTV(buffer[11][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[11] = UTIL_READBITSFROM_UINTV(buffer[11][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[12] = UTIL_READBITSFROM_UINTV(buffer[12][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[12] = UTIL_READBITSFROM_UINTV(buffer[12][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[13] = UTIL_READBITSFROM_UINTV(buffer[13][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[13] = UTIL_READBITSFROM_UINTV(buffer[13][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[14] = UTIL_READBITSFROM_UINTV(buffer[14][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[14] = UTIL_READBITSFROM_UINTV(buffer[14][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	
		vmdatas[15] = UTIL_READBITSFROM_UINTV(buffer[15][bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[15] = UTIL_READBITSFROM_UINTV(buffer[15][bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0);
	} else {
	
		vmdatas[0] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[0] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[1] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[1] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[2] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[2] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[3] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[3] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[4] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[4] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[5] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[5] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[6] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[6] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[7] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[7] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[8] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[8] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[9] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[9] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[10] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[10] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[11] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[11] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[12] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[12] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[13] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[13] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[14] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[14] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	
		vmdatas[15] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[15] = UTIL_READBITSFROM_UINTV(buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1);
	}
	#else 
	if(index%2==0){ 
		vmdatas[0] = UTIL_READFROM_UINT(buffer[0][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[0] = UTIL_READFROM_UINT(buffer[0][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[1] = UTIL_READFROM_UINT(buffer[1][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[1] = UTIL_READFROM_UINT(buffer[1][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[2] = UTIL_READFROM_UINT(buffer[2][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[2] = UTIL_READFROM_UINT(buffer[2][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[3] = UTIL_READFROM_UINT(buffer[3][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[3] = UTIL_READFROM_UINT(buffer[3][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[4] = UTIL_READFROM_UINT(buffer[4][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[4] = UTIL_READFROM_UINT(buffer[4][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[5] = UTIL_READFROM_UINT(buffer[5][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[5] = UTIL_READFROM_UINT(buffer[5][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[6] = UTIL_READFROM_UINT(buffer[6][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[6] = UTIL_READFROM_UINT(buffer[6][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[7] = UTIL_READFROM_UINT(buffer[7][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[7] = UTIL_READFROM_UINT(buffer[7][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[8] = UTIL_READFROM_UINT(buffer[8][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[8] = UTIL_READFROM_UINT(buffer[8][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[9] = UTIL_READFROM_UINT(buffer[9][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[9] = UTIL_READFROM_UINT(buffer[9][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[10] = UTIL_READFROM_UINT(buffer[10][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[10] = UTIL_READFROM_UINT(buffer[10][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[11] = UTIL_READFROM_UINT(buffer[11][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[11] = UTIL_READFROM_UINT(buffer[11][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[12] = UTIL_READFROM_UINT(buffer[12][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[12] = UTIL_READFROM_UINT(buffer[12][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[13] = UTIL_READFROM_UINT(buffer[13][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[13] = UTIL_READFROM_UINT(buffer[13][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[14] = UTIL_READFROM_UINT(buffer[14][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[14] = UTIL_READFROM_UINT(buffer[14][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
		vmdatas[15] = UTIL_READFROM_UINT(buffer[15][bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0);
		vdatas[15] = UTIL_READFROM_UINT(buffer[15][bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0);
	} else {
		vmdatas[0] = UTIL_READFROM_UINT(buffer[0][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[0] = UTIL_READFROM_UINT(buffer[0][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[1] = UTIL_READFROM_UINT(buffer[1][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[1] = UTIL_READFROM_UINT(buffer[1][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[2] = UTIL_READFROM_UINT(buffer[2][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[2] = UTIL_READFROM_UINT(buffer[2][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[3] = UTIL_READFROM_UINT(buffer[3][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[3] = UTIL_READFROM_UINT(buffer[3][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[4] = UTIL_READFROM_UINT(buffer[4][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[4] = UTIL_READFROM_UINT(buffer[4][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[5] = UTIL_READFROM_UINT(buffer[5][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[5] = UTIL_READFROM_UINT(buffer[5][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[6] = UTIL_READFROM_UINT(buffer[6][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[6] = UTIL_READFROM_UINT(buffer[6][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[7] = UTIL_READFROM_UINT(buffer[7][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[7] = UTIL_READFROM_UINT(buffer[7][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[8] = UTIL_READFROM_UINT(buffer[8][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[8] = UTIL_READFROM_UINT(buffer[8][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[9] = UTIL_READFROM_UINT(buffer[9][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[9] = UTIL_READFROM_UINT(buffer[9][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[10] = UTIL_READFROM_UINT(buffer[10][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[10] = UTIL_READFROM_UINT(buffer[10][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[11] = UTIL_READFROM_UINT(buffer[11][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[11] = UTIL_READFROM_UINT(buffer[11][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[12] = UTIL_READFROM_UINT(buffer[12][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[12] = UTIL_READFROM_UINT(buffer[12][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[13] = UTIL_READFROM_UINT(buffer[13][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[13] = UTIL_READFROM_UINT(buffer[13][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[14] = UTIL_READFROM_UINT(buffer[14][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[14] = UTIL_READFROM_UINT(buffer[14][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
		vmdatas[15] = UTIL_READFROM_UINT(buffer[15][bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1);
		vdatas[15] = UTIL_READFROM_UINT(buffer[15][bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1);
	}
	#endif 
	return;
}

void acts_all::MEMCA_READFROMBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCA_READFROMBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
	if(index%2==0){ 
		vdatas[0] = UTIL_READBITSFROM_UINTV(buffer[0][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[1] = UTIL_READBITSFROM_UINTV(buffer[1][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[2] = UTIL_READBITSFROM_UINTV(buffer[2][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[3] = UTIL_READBITSFROM_UINTV(buffer[3][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[4] = UTIL_READBITSFROM_UINTV(buffer[4][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[5] = UTIL_READBITSFROM_UINTV(buffer[5][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[6] = UTIL_READBITSFROM_UINTV(buffer[6][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[7] = UTIL_READBITSFROM_UINTV(buffer[7][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[8] = UTIL_READBITSFROM_UINTV(buffer[8][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[9] = UTIL_READBITSFROM_UINTV(buffer[9][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[10] = UTIL_READBITSFROM_UINTV(buffer[10][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[11] = UTIL_READBITSFROM_UINTV(buffer[11][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[12] = UTIL_READBITSFROM_UINTV(buffer[12][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[13] = UTIL_READBITSFROM_UINTV(buffer[13][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[14] = UTIL_READBITSFROM_UINTV(buffer[14][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
		vdatas[15] = UTIL_READBITSFROM_UINTV(buffer[15][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY);
	} else {
		vdatas[0] = UTIL_READBITSFROM_UINTV(buffer[0][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[1] = UTIL_READBITSFROM_UINTV(buffer[1][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[2] = UTIL_READBITSFROM_UINTV(buffer[2][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[3] = UTIL_READBITSFROM_UINTV(buffer[3][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[4] = UTIL_READBITSFROM_UINTV(buffer[4][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[5] = UTIL_READBITSFROM_UINTV(buffer[5][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[6] = UTIL_READBITSFROM_UINTV(buffer[6][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[7] = UTIL_READBITSFROM_UINTV(buffer[7][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[8] = UTIL_READBITSFROM_UINTV(buffer[8][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[9] = UTIL_READBITSFROM_UINTV(buffer[9][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[10] = UTIL_READBITSFROM_UINTV(buffer[10][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[11] = UTIL_READBITSFROM_UINTV(buffer[11][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[12] = UTIL_READBITSFROM_UINTV(buffer[12][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[13] = UTIL_READBITSFROM_UINTV(buffer[13][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[14] = UTIL_READBITSFROM_UINTV(buffer[14][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
		vdatas[15] = UTIL_READBITSFROM_UINTV(buffer[15][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE);
	}
	#else 
	if(index%2==0){ 
		vdatas[0] = buffer[0][bufferoffset_kvs + index/2].key;
		vdatas[1] = buffer[1][bufferoffset_kvs + index/2].key;
		vdatas[2] = buffer[2][bufferoffset_kvs + index/2].key;
		vdatas[3] = buffer[3][bufferoffset_kvs + index/2].key;
		vdatas[4] = buffer[4][bufferoffset_kvs + index/2].key;
		vdatas[5] = buffer[5][bufferoffset_kvs + index/2].key;
		vdatas[6] = buffer[6][bufferoffset_kvs + index/2].key;
		vdatas[7] = buffer[7][bufferoffset_kvs + index/2].key;
		vdatas[8] = buffer[8][bufferoffset_kvs + index/2].key;
		vdatas[9] = buffer[9][bufferoffset_kvs + index/2].key;
		vdatas[10] = buffer[10][bufferoffset_kvs + index/2].key;
		vdatas[11] = buffer[11][bufferoffset_kvs + index/2].key;
		vdatas[12] = buffer[12][bufferoffset_kvs + index/2].key;
		vdatas[13] = buffer[13][bufferoffset_kvs + index/2].key;
		vdatas[14] = buffer[14][bufferoffset_kvs + index/2].key;
		vdatas[15] = buffer[15][bufferoffset_kvs + index/2].key;
	} else {
		vdatas[0] = buffer[0][bufferoffset_kvs + index/2].value;
		vdatas[1] = buffer[1][bufferoffset_kvs + index/2].value;
		vdatas[2] = buffer[2][bufferoffset_kvs + index/2].value;
		vdatas[3] = buffer[3][bufferoffset_kvs + index/2].value;
		vdatas[4] = buffer[4][bufferoffset_kvs + index/2].value;
		vdatas[5] = buffer[5][bufferoffset_kvs + index/2].value;
		vdatas[6] = buffer[6][bufferoffset_kvs + index/2].value;
		vdatas[7] = buffer[7][bufferoffset_kvs + index/2].value;
		vdatas[8] = buffer[8][bufferoffset_kvs + index/2].value;
		vdatas[9] = buffer[9][bufferoffset_kvs + index/2].value;
		vdatas[10] = buffer[10][bufferoffset_kvs + index/2].value;
		vdatas[11] = buffer[11][bufferoffset_kvs + index/2].value;
		vdatas[12] = buffer[12][bufferoffset_kvs + index/2].value;
		vdatas[13] = buffer[13][bufferoffset_kvs + index/2].value;
		vdatas[14] = buffer[14][bufferoffset_kvs + index/2].value;
		vdatas[15] = buffer[15][bufferoffset_kvs + index/2].value;
	}
	#endif
	return;
}

void acts_all::MEMCA_WRITETOBUFFER_VDATAWITHVMASK(unsigned int index, keyvalue_vbuffer_t buffer[BLOCKRAM_SIZE], value_t vdata, unit1_type vmdata, batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFER_VDATAWITHVMASK:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
	if(index%2==0){
UTIL_WRITEBITSTO_UINTV((unsigned int *)&buffer[bufferoffset_kvs + index/2], SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata);
UTIL_WRITEBITSTO_UINTV((unsigned int *)&buffer[bufferoffset_kvs + index/2], 0, SIZEOF_VDATA0, vdata);
	} else{
UTIL_WRITEBITSTO_UINTV((unsigned int *)&buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY + SIZEOF_VDATA1, SIZEOF_VMASK1, vmdata);
UTIL_WRITEBITSTO_UINTV((unsigned int *)&buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATAKEY, SIZEOF_VDATA1, vdata);
	}
	#else 
	if(index%2==0){
UTIL_WRITEBITSTO_UINTV((unsigned int *)&buffer[bufferoffset_kvs + index/2].key, SIZEOF_VDATA0, SIZEOF_VMASK0, vmdata);
UTIL_WRITEBITSTO_UINTV((unsigned int *)&buffer[bufferoffset_kvs + index/2].key, 0, SIZEOF_VDATA0, vdata);
	} else{
UTIL_WRITEBITSTO_UINTV((unsigned int *)&buffer[bufferoffset_kvs + index/2].value, SIZEOF_VDATA1, SIZEOF_VMASK1, vmdata);
UTIL_WRITEBITSTO_UINTV((unsigned int *)&buffer[bufferoffset_kvs + index/2].value, 0, SIZEOF_VDATA1, vdata);
	}	
	#endif 
	return;
}

void acts_all::MEMCA_WRITETOBUFFER_VDATASANDVMASKS(unsigned int index, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VECTOR2_SIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCA_WRITETOBUFFER_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
	if(index%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[0][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[0]);
UTIL_WRITEBITSTO_UINTV(&buffer[1][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[1]);
UTIL_WRITEBITSTO_UINTV(&buffer[2][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[2]);
UTIL_WRITEBITSTO_UINTV(&buffer[3][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[3]);
UTIL_WRITEBITSTO_UINTV(&buffer[4][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[4]);
UTIL_WRITEBITSTO_UINTV(&buffer[5][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[5]);
UTIL_WRITEBITSTO_UINTV(&buffer[6][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[6]);
UTIL_WRITEBITSTO_UINTV(&buffer[7][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[7]);
UTIL_WRITEBITSTO_UINTV(&buffer[8][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[8]);
UTIL_WRITEBITSTO_UINTV(&buffer[9][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[9]);
UTIL_WRITEBITSTO_UINTV(&buffer[10][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[10]);
UTIL_WRITEBITSTO_UINTV(&buffer[11][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[11]);
UTIL_WRITEBITSTO_UINTV(&buffer[12][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[12]);
UTIL_WRITEBITSTO_UINTV(&buffer[13][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[13]);
UTIL_WRITEBITSTO_UINTV(&buffer[14][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[14]);
UTIL_WRITEBITSTO_UINTV(&buffer[15][bufferoffset_kvs + index/2], 0, SIZEOF_VDATAKEY, vdatas[15]);
	} else {
UTIL_WRITEBITSTO_UINTV(&buffer[0][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[0]);
UTIL_WRITEBITSTO_UINTV(&buffer[1][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[1]);
UTIL_WRITEBITSTO_UINTV(&buffer[2][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[2]);
UTIL_WRITEBITSTO_UINTV(&buffer[3][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[3]);
UTIL_WRITEBITSTO_UINTV(&buffer[4][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[4]);
UTIL_WRITEBITSTO_UINTV(&buffer[5][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[5]);
UTIL_WRITEBITSTO_UINTV(&buffer[6][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[6]);
UTIL_WRITEBITSTO_UINTV(&buffer[7][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[7]);
UTIL_WRITEBITSTO_UINTV(&buffer[8][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[8]);
UTIL_WRITEBITSTO_UINTV(&buffer[9][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[9]);
UTIL_WRITEBITSTO_UINTV(&buffer[10][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[10]);
UTIL_WRITEBITSTO_UINTV(&buffer[11][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[11]);
UTIL_WRITEBITSTO_UINTV(&buffer[12][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[12]);
UTIL_WRITEBITSTO_UINTV(&buffer[13][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[13]);
UTIL_WRITEBITSTO_UINTV(&buffer[14][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[14]);
UTIL_WRITEBITSTO_UINTV(&buffer[15][bufferoffset_kvs + index/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[15]);
	}
	#else 
	if(index%2==0){ 
		buffer[0][bufferoffset_kvs + index/2].key = vdatas[0];
		buffer[1][bufferoffset_kvs + index/2].key = vdatas[1];
		buffer[2][bufferoffset_kvs + index/2].key = vdatas[2];
		buffer[3][bufferoffset_kvs + index/2].key = vdatas[3];
		buffer[4][bufferoffset_kvs + index/2].key = vdatas[4];
		buffer[5][bufferoffset_kvs + index/2].key = vdatas[5];
		buffer[6][bufferoffset_kvs + index/2].key = vdatas[6];
		buffer[7][bufferoffset_kvs + index/2].key = vdatas[7];
		buffer[8][bufferoffset_kvs + index/2].key = vdatas[8];
		buffer[9][bufferoffset_kvs + index/2].key = vdatas[9];
		buffer[10][bufferoffset_kvs + index/2].key = vdatas[10];
		buffer[11][bufferoffset_kvs + index/2].key = vdatas[11];
		buffer[12][bufferoffset_kvs + index/2].key = vdatas[12];
		buffer[13][bufferoffset_kvs + index/2].key = vdatas[13];
		buffer[14][bufferoffset_kvs + index/2].key = vdatas[14];
		buffer[15][bufferoffset_kvs + index/2].key = vdatas[15];
	} else {
		buffer[0][bufferoffset_kvs + index/2].value = vdatas[0];
		buffer[1][bufferoffset_kvs + index/2].value = vdatas[1];
		buffer[2][bufferoffset_kvs + index/2].value = vdatas[2];
		buffer[3][bufferoffset_kvs + index/2].value = vdatas[3];
		buffer[4][bufferoffset_kvs + index/2].value = vdatas[4];
		buffer[5][bufferoffset_kvs + index/2].value = vdatas[5];
		buffer[6][bufferoffset_kvs + index/2].value = vdatas[6];
		buffer[7][bufferoffset_kvs + index/2].value = vdatas[7];
		buffer[8][bufferoffset_kvs + index/2].value = vdatas[8];
		buffer[9][bufferoffset_kvs + index/2].value = vdatas[9];
		buffer[10][bufferoffset_kvs + index/2].value = vdatas[10];
		buffer[11][bufferoffset_kvs + index/2].value = vdatas[11];
		buffer[12][bufferoffset_kvs + index/2].value = vdatas[12];
		buffer[13][bufferoffset_kvs + index/2].value = vdatas[13];
		buffer[14][bufferoffset_kvs + index/2].value = vdatas[14];
		buffer[15][bufferoffset_kvs + index/2].value = vdatas[15];
	}
	#endif 
	return;
}

void acts_all::MEMCA_WRITETOBUFFERWITHDEPTHS_VDATASANDVMASKS(unsigned int indexes[VDATA_PACKINGSIZE], keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], value_t vdatas[VDATA_PACKINGSIZE], batch_type bufferoffset_kvs){
	#pragma HLS INLINE
	
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[0]/2, BLOCKRAM_SIZE, indexes[0], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[1]/2, BLOCKRAM_SIZE, indexes[1], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[2]/2, BLOCKRAM_SIZE, indexes[2], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[3]/2, BLOCKRAM_SIZE, indexes[3], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[4]/2, BLOCKRAM_SIZE, indexes[4], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[5]/2, BLOCKRAM_SIZE, indexes[5], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[6]/2, BLOCKRAM_SIZE, indexes[6], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[7]/2, BLOCKRAM_SIZE, indexes[7], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[8]/2, BLOCKRAM_SIZE, indexes[8], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[9]/2, BLOCKRAM_SIZE, indexes[9], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[10]/2, BLOCKRAM_SIZE, indexes[10], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[11]/2, BLOCKRAM_SIZE, indexes[11], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[12]/2, BLOCKRAM_SIZE, indexes[12], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[13]/2, BLOCKRAM_SIZE, indexes[13], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[14]/2, BLOCKRAM_SIZE, indexes[14], NAp, NAp);
	actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[15]/2, BLOCKRAM_SIZE, indexes[15], NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
		if(indexes[0]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[0][bufferoffset_kvs + indexes[0]/2], 0, SIZEOF_VDATAKEY, vdatas[0]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[0][bufferoffset_kvs + indexes[0]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[0]);
		}
		if(indexes[1]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[1][bufferoffset_kvs + indexes[1]/2], 0, SIZEOF_VDATAKEY, vdatas[1]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[1][bufferoffset_kvs + indexes[1]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[1]);
		}
		if(indexes[2]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[2][bufferoffset_kvs + indexes[2]/2], 0, SIZEOF_VDATAKEY, vdatas[2]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[2][bufferoffset_kvs + indexes[2]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[2]);
		}
		if(indexes[3]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[3][bufferoffset_kvs + indexes[3]/2], 0, SIZEOF_VDATAKEY, vdatas[3]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[3][bufferoffset_kvs + indexes[3]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[3]);
		}
		if(indexes[4]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[4][bufferoffset_kvs + indexes[4]/2], 0, SIZEOF_VDATAKEY, vdatas[4]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[4][bufferoffset_kvs + indexes[4]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[4]);
		}
		if(indexes[5]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[5][bufferoffset_kvs + indexes[5]/2], 0, SIZEOF_VDATAKEY, vdatas[5]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[5][bufferoffset_kvs + indexes[5]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[5]);
		}
		if(indexes[6]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[6][bufferoffset_kvs + indexes[6]/2], 0, SIZEOF_VDATAKEY, vdatas[6]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[6][bufferoffset_kvs + indexes[6]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[6]);
		}
		if(indexes[7]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[7][bufferoffset_kvs + indexes[7]/2], 0, SIZEOF_VDATAKEY, vdatas[7]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[7][bufferoffset_kvs + indexes[7]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[7]);
		}
		if(indexes[8]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[8][bufferoffset_kvs + indexes[8]/2], 0, SIZEOF_VDATAKEY, vdatas[8]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[8][bufferoffset_kvs + indexes[8]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[8]);
		}
		if(indexes[9]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[9][bufferoffset_kvs + indexes[9]/2], 0, SIZEOF_VDATAKEY, vdatas[9]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[9][bufferoffset_kvs + indexes[9]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[9]);
		}
		if(indexes[10]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[10][bufferoffset_kvs + indexes[10]/2], 0, SIZEOF_VDATAKEY, vdatas[10]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[10][bufferoffset_kvs + indexes[10]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[10]);
		}
		if(indexes[11]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[11][bufferoffset_kvs + indexes[11]/2], 0, SIZEOF_VDATAKEY, vdatas[11]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[11][bufferoffset_kvs + indexes[11]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[11]);
		}
		if(indexes[12]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[12][bufferoffset_kvs + indexes[12]/2], 0, SIZEOF_VDATAKEY, vdatas[12]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[12][bufferoffset_kvs + indexes[12]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[12]);
		}
		if(indexes[13]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[13][bufferoffset_kvs + indexes[13]/2], 0, SIZEOF_VDATAKEY, vdatas[13]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[13][bufferoffset_kvs + indexes[13]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[13]);
		}
		if(indexes[14]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[14][bufferoffset_kvs + indexes[14]/2], 0, SIZEOF_VDATAKEY, vdatas[14]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[14][bufferoffset_kvs + indexes[14]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[14]);
		}
		if(indexes[15]%2==0){ 
UTIL_WRITEBITSTO_UINTV(&buffer[15][bufferoffset_kvs + indexes[15]/2], 0, SIZEOF_VDATAKEY, vdatas[15]);
		} else{ 
UTIL_WRITEBITSTO_UINTV(&buffer[15][bufferoffset_kvs + indexes[15]/2], SIZEOF_VDATAKEY, SIZEOF_VDATAVALUE, vdatas[15]);
		}
	
	#else 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[0]/2, BLOCKRAM_SIZE, indexes[0], NAp, NAp);
		#endif
		if(indexes[0]%2==0){ 
			buffer[0][bufferoffset_kvs + indexes[0]/2].key = vdatas[0]; 
		} else{ 
			buffer[0][bufferoffset_kvs + indexes[0]/2].value = vdatas[0]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[1]/2, BLOCKRAM_SIZE, indexes[1], NAp, NAp);
		#endif
		if(indexes[1]%2==0){ 
			buffer[1][bufferoffset_kvs + indexes[1]/2].key = vdatas[1]; 
		} else{ 
			buffer[1][bufferoffset_kvs + indexes[1]/2].value = vdatas[1]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[2]/2, BLOCKRAM_SIZE, indexes[2], NAp, NAp);
		#endif
		if(indexes[2]%2==0){ 
			buffer[2][bufferoffset_kvs + indexes[2]/2].key = vdatas[2]; 
		} else{ 
			buffer[2][bufferoffset_kvs + indexes[2]/2].value = vdatas[2]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[3]/2, BLOCKRAM_SIZE, indexes[3], NAp, NAp);
		#endif
		if(indexes[3]%2==0){ 
			buffer[3][bufferoffset_kvs + indexes[3]/2].key = vdatas[3]; 
		} else{ 
			buffer[3][bufferoffset_kvs + indexes[3]/2].value = vdatas[3]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[4]/2, BLOCKRAM_SIZE, indexes[4], NAp, NAp);
		#endif
		if(indexes[4]%2==0){ 
			buffer[4][bufferoffset_kvs + indexes[4]/2].key = vdatas[4]; 
		} else{ 
			buffer[4][bufferoffset_kvs + indexes[4]/2].value = vdatas[4]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[5]/2, BLOCKRAM_SIZE, indexes[5], NAp, NAp);
		#endif
		if(indexes[5]%2==0){ 
			buffer[5][bufferoffset_kvs + indexes[5]/2].key = vdatas[5]; 
		} else{ 
			buffer[5][bufferoffset_kvs + indexes[5]/2].value = vdatas[5]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[6]/2, BLOCKRAM_SIZE, indexes[6], NAp, NAp);
		#endif
		if(indexes[6]%2==0){ 
			buffer[6][bufferoffset_kvs + indexes[6]/2].key = vdatas[6]; 
		} else{ 
			buffer[6][bufferoffset_kvs + indexes[6]/2].value = vdatas[6]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[7]/2, BLOCKRAM_SIZE, indexes[7], NAp, NAp);
		#endif
		if(indexes[7]%2==0){ 
			buffer[7][bufferoffset_kvs + indexes[7]/2].key = vdatas[7]; 
		} else{ 
			buffer[7][bufferoffset_kvs + indexes[7]/2].value = vdatas[7]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[8]/2, BLOCKRAM_SIZE, indexes[8], NAp, NAp);
		#endif
		if(indexes[8]%2==0){ 
			buffer[8][bufferoffset_kvs + indexes[8]/2].key = vdatas[8]; 
		} else{ 
			buffer[8][bufferoffset_kvs + indexes[8]/2].value = vdatas[8]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[9]/2, BLOCKRAM_SIZE, indexes[9], NAp, NAp);
		#endif
		if(indexes[9]%2==0){ 
			buffer[9][bufferoffset_kvs + indexes[9]/2].key = vdatas[9]; 
		} else{ 
			buffer[9][bufferoffset_kvs + indexes[9]/2].value = vdatas[9]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[10]/2, BLOCKRAM_SIZE, indexes[10], NAp, NAp);
		#endif
		if(indexes[10]%2==0){ 
			buffer[10][bufferoffset_kvs + indexes[10]/2].key = vdatas[10]; 
		} else{ 
			buffer[10][bufferoffset_kvs + indexes[10]/2].value = vdatas[10]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[11]/2, BLOCKRAM_SIZE, indexes[11], NAp, NAp);
		#endif
		if(indexes[11]%2==0){ 
			buffer[11][bufferoffset_kvs + indexes[11]/2].key = vdatas[11]; 
		} else{ 
			buffer[11][bufferoffset_kvs + indexes[11]/2].value = vdatas[11]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[12]/2, BLOCKRAM_SIZE, indexes[12], NAp, NAp);
		#endif
		if(indexes[12]%2==0){ 
			buffer[12][bufferoffset_kvs + indexes[12]/2].key = vdatas[12]; 
		} else{ 
			buffer[12][bufferoffset_kvs + indexes[12]/2].value = vdatas[12]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[13]/2, BLOCKRAM_SIZE, indexes[13], NAp, NAp);
		#endif
		if(indexes[13]%2==0){ 
			buffer[13][bufferoffset_kvs + indexes[13]/2].key = vdatas[13]; 
		} else{ 
			buffer[13][bufferoffset_kvs + indexes[13]/2].value = vdatas[13]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[14]/2, BLOCKRAM_SIZE, indexes[14], NAp, NAp);
		#endif
		if(indexes[14]%2==0){ 
			buffer[14][bufferoffset_kvs + indexes[14]/2].key = vdatas[14]; 
		} else{ 
			buffer[14][bufferoffset_kvs + indexes[14]/2].value = vdatas[14]; 
		}
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("{context['classname__mem_convert_and_access']}}MEMCA_WRITETOBUFFERWITHDEPTHS_VDATAS:", indexes[15]/2, BLOCKRAM_SIZE, indexes[15], NAp, NAp);
		#endif
		if(indexes[15]%2==0){ 
			buffer[15][bufferoffset_kvs + indexes[15]/2].key = vdatas[15]; 
		} else{ 
			buffer[15][bufferoffset_kvs + indexes[15]/2].value = vdatas[15]; 
		}
	#endif 
	return;
}

void acts_all::MEMCA_READFROMKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, value_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs){
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCA_READFROMKVDRAM_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	#ifdef _WIDEWORD
	vdatas[0] = kvdram[baseoffset_kvs + offset_kvs + index].range(31, 0); 
	vdatas[1] = kvdram[baseoffset_kvs + offset_kvs + index].range(63, 32); 
	vdatas[2] = kvdram[baseoffset_kvs + offset_kvs + index].range(95, 64); 
	vdatas[3] = kvdram[baseoffset_kvs + offset_kvs + index].range(127, 96); 
	vdatas[4] = kvdram[baseoffset_kvs + offset_kvs + index].range(159, 128); 
	vdatas[5] = kvdram[baseoffset_kvs + offset_kvs + index].range(191, 160); 
	vdatas[6] = kvdram[baseoffset_kvs + offset_kvs + index].range(223, 192); 
	vdatas[7] = kvdram[baseoffset_kvs + offset_kvs + index].range(255, 224); 
	vdatas[8] = kvdram[baseoffset_kvs + offset_kvs + index].range(287, 256); 
	vdatas[9] = kvdram[baseoffset_kvs + offset_kvs + index].range(319, 288); 
	vdatas[10] = kvdram[baseoffset_kvs + offset_kvs + index].range(351, 320); 
	vdatas[11] = kvdram[baseoffset_kvs + offset_kvs + index].range(383, 352); 
	vdatas[12] = kvdram[baseoffset_kvs + offset_kvs + index].range(415, 384); 
	vdatas[13] = kvdram[baseoffset_kvs + offset_kvs + index].range(447, 416); 
	vdatas[14] = kvdram[baseoffset_kvs + offset_kvs + index].range(479, 448); 
	vdatas[15] = kvdram[baseoffset_kvs + offset_kvs + index].range(511, 480); 
	#else 
	vdatas[0] = kvdram[baseoffset_kvs + offset_kvs + index].data[0].key;
	vdatas[1] = kvdram[baseoffset_kvs + offset_kvs + index].data[0].value;
	vdatas[2] = kvdram[baseoffset_kvs + offset_kvs + index].data[1].key;
	vdatas[3] = kvdram[baseoffset_kvs + offset_kvs + index].data[1].value;
	vdatas[4] = kvdram[baseoffset_kvs + offset_kvs + index].data[2].key;
	vdatas[5] = kvdram[baseoffset_kvs + offset_kvs + index].data[2].value;
	vdatas[6] = kvdram[baseoffset_kvs + offset_kvs + index].data[3].key;
	vdatas[7] = kvdram[baseoffset_kvs + offset_kvs + index].data[3].value;
	vdatas[8] = kvdram[baseoffset_kvs + offset_kvs + index].data[4].key;
	vdatas[9] = kvdram[baseoffset_kvs + offset_kvs + index].data[4].value;
	vdatas[10] = kvdram[baseoffset_kvs + offset_kvs + index].data[5].key;
	vdatas[11] = kvdram[baseoffset_kvs + offset_kvs + index].data[5].value;
	vdatas[12] = kvdram[baseoffset_kvs + offset_kvs + index].data[6].key;
	vdatas[13] = kvdram[baseoffset_kvs + offset_kvs + index].data[6].value;
	vdatas[14] = kvdram[baseoffset_kvs + offset_kvs + index].data[7].key;
	vdatas[15] = kvdram[baseoffset_kvs + offset_kvs + index].data[7].value;
	#endif
	return;
}

void acts_all::MEMCA_WRITETOKVDRAM_VDATASANDVMASKS(unsigned int index, uint512_dt * kvdram, value_t vdatas[VECTOR2_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs){			
	#pragma HLS INLINE
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("acts_all::MEMCA_WRITETOKVDRAM_VDATASANDVMASKS:", index/2, BLOCKRAM_SIZE, index, NAp, NAp);
	#endif
	
	unsigned int vdata1;
	unsigned int vdata2;
	unsigned int vmdata1;
	unsigned int vmdata2;

	#ifdef _WIDEWORD
	kvdram[baseoffset_kvs + offset_kvs + index].range(31, 0) = vdatas[0];
	kvdram[baseoffset_kvs + offset_kvs + index].range(63, 32) = vdatas[1];
	kvdram[baseoffset_kvs + offset_kvs + index].range(95, 64) = vdatas[2];
	kvdram[baseoffset_kvs + offset_kvs + index].range(127, 96) = vdatas[3];
	kvdram[baseoffset_kvs + offset_kvs + index].range(159, 128) = vdatas[4];
	kvdram[baseoffset_kvs + offset_kvs + index].range(191, 160) = vdatas[5];
	kvdram[baseoffset_kvs + offset_kvs + index].range(223, 192) = vdatas[6];
	kvdram[baseoffset_kvs + offset_kvs + index].range(255, 224) = vdatas[7];
	kvdram[baseoffset_kvs + offset_kvs + index].range(287, 256) = vdatas[8];
	kvdram[baseoffset_kvs + offset_kvs + index].range(319, 288) = vdatas[9];
	kvdram[baseoffset_kvs + offset_kvs + index].range(351, 320) = vdatas[10];
	kvdram[baseoffset_kvs + offset_kvs + index].range(383, 352) = vdatas[11];
	kvdram[baseoffset_kvs + offset_kvs + index].range(415, 384) = vdatas[12];
	kvdram[baseoffset_kvs + offset_kvs + index].range(447, 416) = vdatas[13];
	kvdram[baseoffset_kvs + offset_kvs + index].range(479, 448) = vdatas[14];
	kvdram[baseoffset_kvs + offset_kvs + index].range(511, 480) = vdatas[15];
	#else 
	kvdram[baseoffset_kvs + offset_kvs + index].data[0].key = vdatas[0];
	kvdram[baseoffset_kvs + offset_kvs + index].data[0].value = vdatas[1];
	
	vdata1 = UTIL_READBITSFROM_UINTV(vdatas[0], 0, SIZEOF_VDATA0);
	vdata2 = UTIL_READBITSFROM_UINTV(vdatas[1], 0, SIZEOF_VDATA0);
	vmdata1 = UTIL_READBITSFROM_UINTV(vdatas[0], SIZEOF_VDATA0, SIZEOF_VMASK0);
	vmdata2 = UTIL_READBITSFROM_UINTV(vdatas[1], SIZEOF_VDATA0, SIZEOF_VMASK0);
	if(vdata1 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[0]: "<<vdata1<<", mask: "<<vmdata1<<""<<endl; }
	if(vdata2 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[1]: "<<vdata2<<", mask: "<<vmdata2<<""<<endl; }
	
	kvdram[baseoffset_kvs + offset_kvs + index].data[1].key = vdatas[2];
	kvdram[baseoffset_kvs + offset_kvs + index].data[1].value = vdatas[3];
	
	vdata1 = UTIL_READBITSFROM_UINTV(vdatas[2], 0, SIZEOF_VDATA0);
	vdata2 = UTIL_READBITSFROM_UINTV(vdatas[3], 0, SIZEOF_VDATA0);
	vmdata1 = UTIL_READBITSFROM_UINTV(vdatas[2], SIZEOF_VDATA0, SIZEOF_VMASK0);
	vmdata2 = UTIL_READBITSFROM_UINTV(vdatas[3], SIZEOF_VDATA0, SIZEOF_VMASK0);
	if(vdata1 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[2]: "<<vdata1<<", mask: "<<vmdata1<<""<<endl; }
	if(vdata2 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[3]: "<<vdata2<<", mask: "<<vmdata2<<""<<endl; }
	
	kvdram[baseoffset_kvs + offset_kvs + index].data[2].key = vdatas[4];
	kvdram[baseoffset_kvs + offset_kvs + index].data[2].value = vdatas[5];
	
	vdata1 = UTIL_READBITSFROM_UINTV(vdatas[4], 0, SIZEOF_VDATA0);
	vdata2 = UTIL_READBITSFROM_UINTV(vdatas[5], 0, SIZEOF_VDATA0);
	vmdata1 = UTIL_READBITSFROM_UINTV(vdatas[4], SIZEOF_VDATA0, SIZEOF_VMASK0);
	vmdata2 = UTIL_READBITSFROM_UINTV(vdatas[5], SIZEOF_VDATA0, SIZEOF_VMASK0);
	if(vdata1 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[4]: "<<vdata1<<", mask: "<<vmdata1<<""<<endl; }
	if(vdata2 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[5]: "<<vdata2<<", mask: "<<vmdata2<<""<<endl; }
	
	kvdram[baseoffset_kvs + offset_kvs + index].data[3].key = vdatas[6];
	kvdram[baseoffset_kvs + offset_kvs + index].data[3].value = vdatas[7];
	
	vdata1 = UTIL_READBITSFROM_UINTV(vdatas[6], 0, SIZEOF_VDATA0);
	vdata2 = UTIL_READBITSFROM_UINTV(vdatas[7], 0, SIZEOF_VDATA0);
	vmdata1 = UTIL_READBITSFROM_UINTV(vdatas[6], SIZEOF_VDATA0, SIZEOF_VMASK0);
	vmdata2 = UTIL_READBITSFROM_UINTV(vdatas[7], SIZEOF_VDATA0, SIZEOF_VMASK0);
	if(vdata1 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[6]: "<<vdata1<<", mask: "<<vmdata1<<""<<endl; }
	if(vdata2 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[7]: "<<vdata2<<", mask: "<<vmdata2<<""<<endl; }
	
	kvdram[baseoffset_kvs + offset_kvs + index].data[4].key = vdatas[8];
	kvdram[baseoffset_kvs + offset_kvs + index].data[4].value = vdatas[9];
	
	vdata1 = UTIL_READBITSFROM_UINTV(vdatas[8], 0, SIZEOF_VDATA0);
	vdata2 = UTIL_READBITSFROM_UINTV(vdatas[9], 0, SIZEOF_VDATA0);
	vmdata1 = UTIL_READBITSFROM_UINTV(vdatas[8], SIZEOF_VDATA0, SIZEOF_VMASK0);
	vmdata2 = UTIL_READBITSFROM_UINTV(vdatas[9], SIZEOF_VDATA0, SIZEOF_VMASK0);
	if(vdata1 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[8]: "<<vdata1<<", mask: "<<vmdata1<<""<<endl; }
	if(vdata2 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[9]: "<<vdata2<<", mask: "<<vmdata2<<""<<endl; }
	
	kvdram[baseoffset_kvs + offset_kvs + index].data[5].key = vdatas[10];
	kvdram[baseoffset_kvs + offset_kvs + index].data[5].value = vdatas[11];
	
	vdata1 = UTIL_READBITSFROM_UINTV(vdatas[10], 0, SIZEOF_VDATA0);
	vdata2 = UTIL_READBITSFROM_UINTV(vdatas[11], 0, SIZEOF_VDATA0);
	vmdata1 = UTIL_READBITSFROM_UINTV(vdatas[10], SIZEOF_VDATA0, SIZEOF_VMASK0);
	vmdata2 = UTIL_READBITSFROM_UINTV(vdatas[11], SIZEOF_VDATA0, SIZEOF_VMASK0);
	if(vdata1 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[10]: "<<vdata1<<", mask: "<<vmdata1<<""<<endl; }
	if(vdata2 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[11]: "<<vdata2<<", mask: "<<vmdata2<<""<<endl; }
	
	kvdram[baseoffset_kvs + offset_kvs + index].data[6].key = vdatas[12];
	kvdram[baseoffset_kvs + offset_kvs + index].data[6].value = vdatas[13];
	
	vdata1 = UTIL_READBITSFROM_UINTV(vdatas[12], 0, SIZEOF_VDATA0);
	vdata2 = UTIL_READBITSFROM_UINTV(vdatas[13], 0, SIZEOF_VDATA0);
	vmdata1 = UTIL_READBITSFROM_UINTV(vdatas[12], SIZEOF_VDATA0, SIZEOF_VMASK0);
	vmdata2 = UTIL_READBITSFROM_UINTV(vdatas[13], SIZEOF_VDATA0, SIZEOF_VMASK0);
	if(vdata1 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[12]: "<<vdata1<<", mask: "<<vmdata1<<""<<endl; }
	if(vdata2 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[13]: "<<vdata2<<", mask: "<<vmdata2<<""<<endl; }
	
	kvdram[baseoffset_kvs + offset_kvs + index].data[7].key = vdatas[14];
	kvdram[baseoffset_kvs + offset_kvs + index].data[7].value = vdatas[15];
	
	vdata1 = UTIL_READBITSFROM_UINTV(vdatas[14], 0, SIZEOF_VDATA0);
	vdata2 = UTIL_READBITSFROM_UINTV(vdatas[15], 0, SIZEOF_VDATA0);
	vmdata1 = UTIL_READBITSFROM_UINTV(vdatas[14], SIZEOF_VDATA0, SIZEOF_VMASK0);
	vmdata2 = UTIL_READBITSFROM_UINTV(vdatas[15], SIZEOF_VDATA0, SIZEOF_VMASK0);
	if(vdata1 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[14]: "<<vdata1<<", mask: "<<vmdata1<<""<<endl; }
	if(vdata2 < 64){ cout<<"MEMCA_WRITETOKVDRAM_VDATASANDVMASKS: vdatas[15]: "<<vdata2<<", mask: "<<vmdata2<<""<<endl; }
	
	#endif
	return;
}

