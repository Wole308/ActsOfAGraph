#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/edgeprocess/edge_process.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../src/stats/stats.h"
#include "../../src/dataaccess/dataaccess.h"
#include "../../include/common.h"
#include "test.h"
using namespace std;

test::test(std::string binaryFile){
	utilityobj = new utility();
	helperfunctionsobj = new helperfunctions(); 
	
	#ifdef FPGA_IMPL
	// for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ kvsourcedram[flag][i][j] = (uint512_vec_dt *) aligned_alloc(4096, (KVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt))); }}}			
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ kvsourcedram[flag][i][j] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }}} // REMOVEME.	
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ kvdestdram[flag][i][j] = (uint512_vec_dt *) aligned_alloc(4096, (BATCH_RANGE_KVS * sizeof(uint512_vec_dt))); }}}
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ kvstats[flag][i][j] = (keyvalue_t *) aligned_alloc(4096, (KVSTATSDRAMSZ * sizeof(keyvalue_t))); }}}
	#else 
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ kvsourcedram[flag][i][j] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }}}
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ kvdestdram[flag][i][j] = new uint512_vec_dt[BATCH_RANGE_KVS]; }}}
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){ for(unsigned int i=0; i<NUMCPUTHREADS; i++){ for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ kvstats[flag][i][j] = new keyvalue_t[KVSTATSDRAMSZ]; }}}
	#endif
	
	#ifdef FPGA_IMPL
	helperfunctionsobj->loadOCLstructures(binaryFile, (uint512_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvsourcedram, (uint512_dt* (*)[NUMCPUTHREADS][NUMSUBCPUTHREADS])kvdestdram, kvstats); 	
	#endif
	
	srand (0);
	// srand(time(0)); 
}
test::~test(){
	cout<<"test::~test:: finish destroying memory structures... "<<endl;
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){	
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ delete [] kvsourcedram[i][j]; }
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ delete [] kvdestdram[i][j]; }
		for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ delete [] kvstats[i][j]; }
	}
	#ifdef FPGA_IMPL
	helperfunctionsobj->finishOCL();
	#endif 
}

void test::run(){
	unsigned int globaliteration_idx = 0;
	unsigned int voffset = 0; 
	
	unsigned int fdoffset[NUMCPUTHREADS];
	unsigned int loadsize[NUMCPUTHREADS];
	
	unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int runsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]; 
	utilityobj->setarray(batchoffset, NUMCPUTHREADS, NUMSUBCPUTHREADS, BASEOFFSET_KVDRAMBUFFER);
	utilityobj->setarray(batchsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
	utilityobj->setarray(runsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, 0);
	
	for(unsigned int iteration_idx=0; iteration_idx<1; iteration_idx++){
		
		// Populate kvdrams
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ fdoffset[i] = (iteration_idx + i) * KVDATA_BATCHSIZE; }
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ loadsize[i] = KVDATA_BATCHSIZE; }			
		utilityobj->setarray(batchsize, NUMCPUTHREADS, NUMSUBCPUTHREADS, KVDATA_BATCHSIZE); 
		
		loadkvdram((keyvalue_t* (*)[NUMSUBCPUTHREADS])kvsourcedram[0], batchoffset, batchsize); 
		for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ runsize[i][j] += batchsize[i][j]; }}
		helperfunctionsobj->updatemessagesbeforelaunch(globaliteration_idx, 0, voffset, batchsize, runsize, kvstats[0]);
		
		// Launch the Kernel
		helperfunctionsobj->launchkernel((uint512_dt* (*)[NUMSUBCPUTHREADS])kvsourcedram[0], (uint512_dt* (*)[NUMSUBCPUTHREADS])kvdestdram[0], (keyvalue_t* (*)[NUMSUBCPUTHREADS])kvstats[0], 0);
		
		helperfunctionsobj->updatemessagesafterlaunch(globaliteration_idx, kvstats[0]);
		globaliteration_idx += 1;
	}
	return;
}
void test::finish(){
	#if (defined(FPGA_IMPL) && defined(PR_ALGORITHM))
	helperfunctionsobj->finishOCL();
	#endif
}

void test::loadkvdram(keyvalue_t * batch[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	unsigned int voffset = 0;
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			voffset = j * BATCH_RANGE;
			for(unsigned int k=0; k<batchsize[i][j]; k++){ batch[i][j][batchoffset[i][j] + k].key = voffset + (rand() % BATCH_RANGE); batch[i][j][batchoffset[i][j] + k].value = 0; }
			batchsize[i][j] = KVDATA_BATCHSIZE;
		}
	}
	return;
}

