#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "../src/utility/utility.h"
#include "../src/algorithm/algorithm.h"
#include "../src/graphs/graph.h"
#include "../examples/test/test.h"
#include "../examples/app/app.h"
#include "../src/graphs/creategraphs.h"
#include "../src/graphs/createundirectedgraph.h"
// #include "../src/graphs/makeundirectedgraph_orderbyindegree.h"
#include "../src/dataset/dataset.h"
#include "../include/common.h"
using namespace std;

int main(int argc, char** argv){
	cout<<"Hostprocess:: Graph Analytics Started..."<<endl;
	
	int programglobalvar_numvtxsprocessed = 0; // program global variables
	
	unsigned int datasetid = 0;
	#ifdef DATASET_ORKUT_3M_212M
	datasetid = 1;
	#endif
	#ifdef DATASET_HOLLYWOOD_1M_114M
	datasetid = 2;
	#endif
	#ifdef DATASET_KRON20_1M_45M
	datasetid = 3;
	#endif
	#ifdef DATASET_KRON21_2M_91M
	datasetid = 4;
	#endif
	#ifdef DATASET_KRON22_4M_200M
	datasetid = 5;
	#endif
	#ifdef DATASET_RGG_N_2_24_S0_16M_265M// rgg_n_2_24_s0
	datasetid = 6;
	#endif
	#ifdef DATASET_EUROPE_OSM_50M_108M // europe_osm
	datasetid = 7;
	#endif
	#ifdef DATASET_KMER_V2A_55M_117M // kmer_V2a
	datasetid = 8;
	#endif
	#ifdef DATASET_HUGEBUBBLES_00020_21M_63M // hugebubbles_00020
	datasetid = 9;
	#endif
	#ifdef DATASET_KMER_U1A_67M_138M // kmer_U1a
	datasetid = 10;
	#endif
	
	#ifdef DATASET_RMAT22_SPARSE0
	datasetid = 100;
	#endif
	#ifdef DATASET_RMAT22_SPARSE1
	datasetid = 101;
	#endif
	#ifdef DATASET_RMAT22_SPARSE2
	datasetid = 102;
	#endif
	#ifdef DATASET_RMAT22_SPARSE3
	datasetid = 103;
	#endif
	#ifdef DATASET_RMAT22_SPARSE4
	datasetid = 104;
	#endif
	#ifdef DATASET_RMAT22_SPARSE5
	datasetid = 105;
	#endif
	#ifdef DATASET_RMAT22_SPARSE6
	datasetid = 106;
	#endif
	#ifdef DATASET_RMAT22_SPARSE7
	datasetid = 107;
	#endif
	
	#ifdef DATASET_RMAT24_268M
	datasetid = 200;
	#endif
	#ifdef DATASET_RMAT25_268M
	datasetid = 201;
	#endif
	#ifdef DATASET_RMAT26_268M
	datasetid = 202;
	#endif
	
	#ifdef DATASET_RMAT_RANGE0
	datasetid = 120;
	#endif
	#ifdef DATASET_RMAT_RANGE1
	datasetid = 121;
	#endif
	#ifdef DATASET_RMAT_RANGE2
	datasetid = 122;
	#endif
	#ifdef DATASET_RMAT_RANGE3
	datasetid = 123;
	#endif
	#ifdef DATASET_RMAT_RANGE4
	datasetid = 124;
	#endif
	#ifdef DATASET_RMAT_RANGE5
	datasetid = 125;
	#endif
	#ifdef DATASET_RMAT_RANGE6
	datasetid = 126;
	#endif
	#ifdef DATASET_RMAT_RANGE7
	datasetid = 127;
	#endif
	
	std::string binaryFile1;
	std::string binaryFile2;
	dataset * datasetobj = new dataset(datasetid);
	utility * utilityobj = new utility();
	#ifdef FPGA_IMPL
	if (argc != 2) { // 2,3
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    binaryFile1 = argv[1];
	binaryFile2 = binaryFile1;	
	// #ifdef TESTKERNEL
	// binaryFile1 = "/home/oj2zf/Documents/ActsOfAGraph/outputs/synkernels_acts/goldenkernel${TESTKERNEL}3PEs3WsHW300MHz.xclbin";
	// binaryFile2 = binaryFile1;	
	// #endif 
	#endif
	utilityobj->printallparameters();

	#ifdef _GENERATE2DGRAPH
	creategraphs * creategraphsobj = new creategraphs(datasetobj->getdatasetid());
	creategraphsobj->run();
	exit(EXIT_SUCCESS);
	#endif
	#ifdef _GENERATEUNDIRECTEDGRAPH
	createundirectedgraph * createundirectedgraphobj = new createundirectedgraph(datasetobj->getdatasetid());
	createundirectedgraphobj->start();
	exit(EXIT_SUCCESS);
	#endif
	
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime_overallexecution = std::chrono::steady_clock::now();
	#endif
	
	app * appobj = new app(NAp, datasetobj->getdatasetid(), binaryFile1, binaryFile2);	
	appobj->run();
	
	#ifdef _DEBUGMODE_TIMERS3
	std::cout << "TEST FINISHED" << std::endl; 
	utilityobj->stopBTIME("HOSTPROCESS:: TIMING SUMMARY: TOTAL TIME ELAPSED: ", begintime_overallexecution, NAp);
	#endif
	return EXIT_SUCCESS;
}

