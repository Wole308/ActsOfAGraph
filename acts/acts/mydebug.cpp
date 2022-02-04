#include "mydebug.h"
using namespace std;

mydebug::mydebug(){
	for(int i=0; i<8; i++){
		for(int j=0; j<128; j++){
			global_datas[i][j] = 0;
		}
	}
}
mydebug::~mydebug(){}

// record buffers
void mydebug::set(unsigned int index1, unsigned int index2, unsigned int data){
	global_datas[index1][index2] = data;
}
unsigned int mydebug::get(unsigned int index1, unsigned int index2){
	return global_datas[index1][index2];
}
void mydebug::increment(unsigned int index1, unsigned int index2, unsigned int incr){
	global_datas[index1][index2] += incr;
}

// bit manipulation
unsigned int mydebug::DEBUG_GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int mydebug::DEBUG_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	unsigned int res = 0;
	res = (((data) & DEBUG_GETMASK_UINT((index), (size))) >> (index)); 
	return res;
}
void mydebug::DEBUG_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
	unsigned int tempdata = *data;
	unsigned int A = ((value) << (index));
	unsigned int B = (~DEBUG_GETMASK_UINT((index), (size)));
	unsigned int C = ((tempdata) & (B));
	unsigned int D = (C) | A;
	*data = D;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"WRITETO_ULONG. index: "<<index<<", size: "<<size<<", value: "<<value<<endl;
	cout<<"WRITETO_ULONG. tempdata"<<endl; actsutilityobj->ULONGTOBINARY(tempdata);
	cout<<"WRITETO_ULONG. A"<<endl; actsutilityobj->ULONGTOBINARY(A);
	cout<<"WRITETO_ULONG. B (~mask)"<<endl; actsutilityobj->ULONGTOBINARY(B);
	cout<<"WRITETO_ULONG. C"<<endl; actsutilityobj->ULONGTOBINARY(C);
	cout<<"WRITETO_ULONG. D (result)"<<endl; actsutilityobj->ULONGTOBINARY(D);
	#endif
	return; 
}
unsigned int mydebug::DEBUG_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size){
	#pragma HLS INLINE
	unsigned int res = 0;
	#ifdef _WIDEWORD
	res = data.range(index + size - 1, index); 
	#else
	res = DEBUG_READFROM_UINT(data, index, size);
	#endif
	return res;
}
void mydebug::DEBUG_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value){
	#pragma HLS INLINE
	#ifdef _WIDEWORD
	data->range(index + size - 1, index) = value;
	#else
	DEBUG_WRITETO_UINT(data, index, size, value);
	#endif
	return; 
}

// printers
void mydebug::printvdata(unsigned int vdata[VECTOR2_SIZE], unsigned int data1, unsigned int data2){
	cout<<"printvdata:["<<data1<<"]["<<data2<<"]:"; for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<vdata[v]<<", "; } cout<<endl;
}
void mydebug::printvdatas(keyvalue_vbuffer_t vdatas[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int data1, unsigned int data2, unsigned int limit){
	for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
		for(unsigned int i=0; i<BLOCKRAM_SIZE; i++){
			if(i < limit){ 
				cout<<"["<<2*i<<"]:"; 
				for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<vdatas[v][i].key<<", "; } 
				cout<<endl; 
				cout<<"["<<2*i+1<<"]:"; 
				for(unsigned int v=0; v<VECTOR2_SIZE; v++){ cout<<vdatas[v][i].value<<", "; }
				cout<<endl; 
			}
		}
	}
	return;
}
void mydebug::printvdatawithvmask(unsigned int vdata[VECTOR2_SIZE], unsigned int data1, unsigned int data2){
	cout<<"printvdatawithvmask:["<<data1<<"]["<<data2<<"]:"; 
	for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
		cout<<DEBUG_READBITSFROM_UINTV(vdata[v], 0, SIZEOF_VDATA0)<<""; 
		cout<<"("<<DEBUG_READBITSFROM_UINTV(vdata[v], SIZEOF_VDATA0, SIZEOF_VMASK0)<<"), "; 
	}
	cout<<endl;
}















