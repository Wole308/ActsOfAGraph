#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include "EdgeProcess.h"
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h"
#include "sortreduce.h"
#include "types.h"
#include "sr.h"
#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"
#include "EdgeProcess.h"
#include "VertexValues.h"
#include "../../include/common.h"
using namespace std;

sr::sr(){}
sr::~sr(){}

#ifdef GRAFBOOST_SETUP
void sr::srtopkernel(SortReduce<uint64_t,uint32_t>* _sr, keyvalue_t * kvdram, vertex_t size){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> SortReduce Launched... size: "<<size<<endl; 
	#endif
	
	unsigned int kvuploadcount = 0;
	for (uint64_t i = 0; i < size; i++){
		uint64_t key = (uint64_t)(kvdram[i].key);
		uint64_t value = (uint64_t)(kvdram[i].value);
		if(i % 1000000 == 0){ cout<<"i: "<<i<<", key: "<<key<<", value: "<<value<<endl; }
		while ( !_sr->Update(key, value) ) { }
		kvuploadcount += 1;
	}
	cout<<"srtopkernel: number of key-value pairs uploaded for sorting: "<<kvuploadcount<<endl;
	return;
}
#endif 





