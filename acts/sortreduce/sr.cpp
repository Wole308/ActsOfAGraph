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
#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"
#include "EdgeProcess.h"
#include "VertexValues.h"
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#include "../../src/utility/utility.h"
#include "../../acts/actsutility/actsutility.h"
#include "sr.h"
using namespace std;

sr::sr(){ actsutilityobj = new actsutility(); }
sr::~sr(){}

#ifdef GRAFBOOST_SETUP
void sr::srtopkernel(SortReduce<uint64_t,uint32_t>* _sr, uint512_dt * kvdram){
	unsigned int srcvoffset = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key;
	unsigned int srcvsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key;
	unsigned int runsize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key;
	unsigned int runsize_kvs = runsize / VECTOR_SIZE;
	unsigned int edgessize = kvdram[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE].data[0].key;
	unsigned int kvuploadcount = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<">>> SortReduce Launched... runsize: "<<runsize<<endl; 
	#endif
	
	for (unsigned int i = 0; i < runsize_kvs; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			uint64_t key = (uint64_t)(kvdram[BASEOFFSET_KVDRAM_KVS + i].data[v].key);
			uint64_t value = (uint64_t)(kvdram[BASEOFFSET_KVDRAM_KVS + i].data[v].value);
			if(i % 1000000 == 0){ cout<<"i: "<<i<<", key: "<<key<<", value: "<<value<<endl; }
			while ( !_sr->Update(key, value) ) { }
			kvuploadcount += 1;
		}
	}
	cout<<"srtopkernel: number of key-value pairs uploaded for sorting: "<<kvuploadcount<<endl;
	return;
}
#endif 





