#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../examples/test/test.h"
#include "../../examples/pagerank/pagerank.h"
#include "../../examples/bfs/bfs.h"
#include "../../src/graphs/creategraph.h"
#include "../../src/dataset/dataset.h"
#include "../../include/common.h"
#ifdef FPGA_IMPL
// #include "xcl2.hpp"
#include "../xcl.h"
#endif
using namespace std;

int main(int argc, char** argv){
	cout<<"Hostprocess:: Graph Analytics Started..."<<endl;
	
	std::string binaryFile;
	dataset * datasetobj = new dataset();
	utility * utilityobj = new utility();
	#ifdef FPGA_IMPL
	if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    binaryFile = argv[1];
	#endif
	utilityobj->printallparameters();

	#ifdef _GENERATE2DGRAPH
	creategraph * creategraphobj = new creategraph();
	creategraphobj->create2Dgraph();
	exit(EXIT_SUCCESS);
	#endif
	
	#ifdef _DEBUGMODE_TIMERS2
	std::chrono::steady_clock::time_point begintime_overallexecution = std::chrono::steady_clock::now();
	#endif
	
	#ifdef TESTKERNEL
	test * testobj = new test();
	testobj->run();
	exit(EXIT_SUCCESS);
	#endif
	
	#if (defined(ACTGRAPH_SETUP) & defined(PR_ALGORITHM) & defined(SW))
	pagerank * pagerankobj = new pagerank(NAp, datasetobj->getdatasetid(), binaryFile);
	pagerankobj->run();
	#endif
	#if (defined(ACTGRAPH_SETUP) & defined(BFS_ALGORITHM) & defined(SW))
	bfs * bfsobj = new bfs(NAp, datasetobj->getdatasetid(), binaryFile);
	bfsobj->run();
	#endif
	
	#ifdef _DEBUGMODE_TIMERS2
	std::cout << "TEST FINISHED" << std::endl; 
	utilityobj->stopBTIME("HOSTPROCESS:: TIMING SUMMARY: TOTAL TIME ELAPSED: ", begintime_overallexecution, NAp);
	#endif
	return EXIT_SUCCESS;
}

