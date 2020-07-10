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
#include "../src/host_common.h"
#include "../src/common.h"
#include "../debugger/host_debugger.h"
#include "EdgeProcess.h"
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../kernels/kernelprocess.h"
#include "../src/grafboost_pr_sw.h"
#include "sortreduce.h"
#include "types.h"
#include "srkernelprocess.h"
#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"
#include "EdgeProcess.h"
#include "VertexValues.h"
using namespace std;

void srkernelprocess::srtopkernel(
		SortReduce<uint64_t,uint32_t>* sr
		,keyvalue_t * kvdramA
		,keyvalue_t * kvdramB
		,keyvalue_t * kvdramC
		,keyvalue_t * kvdramD
		,vertex_t kvbatchszA
		,vertex_t kvbatchszB
		,vertex_t kvbatchszC
		,vertex_t kvbatchszD
        ){
	#ifdef GRAFBOOST_SETUP
	#ifdef _DEBUGMODE_HOSTPRINTS
	printf( "srtopkernel Started!\n" ); fflush(stdout);
	#endif
	
	unsigned int kvuploadcount = 0;
	for (uint64_t i = 0; i < kvbatchszA; i++) {
		uint64_t key = (uint64_t)(kvdramA[i].key);
		uint64_t value = (uint64_t)(kvdramA[i].value);
		while ( !sr->Update(key, value) ) { }
		kvuploadcount += 1;
	}
	
	for (uint64_t i = 0; i < kvbatchszB; i++) {
		uint64_t key = (uint64_t)(kvdramB[i].key);
		uint64_t value = (uint64_t)(kvdramB[i].value);
		while ( !sr->Update(key, value) ) { }
		kvuploadcount += 1;
	}
	
	for (uint64_t i = 0; i < kvbatchszC; i++) {
		uint64_t key = (uint64_t)(kvdramC[i].key);
		uint64_t value = (uint64_t)(kvdramC[i].value);
		while ( !sr->Update(key, value) ) { }
		kvuploadcount += 1;
	}
	
	for (uint64_t i = 0; i < kvbatchszD; i++) {
		uint64_t key = (uint64_t)(kvdramD[i].key);
		uint64_t value = (uint64_t)(kvdramD[i].value);
		while ( !sr->Update(key, value) ) { }
		kvuploadcount += 1;
	}
	
	cout<<"srtopkernel: number of key-value pairs uploaded for sorting: "<<kvuploadcount<<endl;
	#endif 
	return;
}
srkernelprocess::srkernelprocess(){}
srkernelprocess::~srkernelprocess(){}
