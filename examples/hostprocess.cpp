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
#include "../examples/pagerank/pagerank.h"
// #include "../examples/bfs/bfs.h"
#include "../examples/advance_op/advance_op.h"
#include "../src/graphs/creategraph.h"
#include "../src/graphs/createNDgraph.h"
#include "../src/dataset/dataset.h"
#include "../include/common.h"
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
	creategraphobj->create2Dgraf(datasetobj->getdatasetid());
	// creategraphobj->analyzegraf(datasetobj->getdatasetid());
	exit(EXIT_SUCCESS);
	#endif
	
	#ifdef _DEBUGMODE_TIMERS2
	std::chrono::steady_clock::time_point begintime_overallexecution = std::chrono::steady_clock::now();
	#endif
	
	#ifdef TESTKERNEL
	test * testobj = new test(binaryFile);
	testobj->run();
	exit(EXIT_SUCCESS);
	#endif
	
	#if (defined(ACTGRAPH_SETUP) & defined(PR_ALGORITHM) & not defined(ADVANCE_ALGORITHM))
	pagerank * pagerankobj = new pagerank(NAp, datasetobj->getdatasetid(), binaryFile);
	pagerankobj->run();
	#endif
	#if (defined(ACTGRAPH_SETUP) & defined(BFS_ALGORITHM) & not defined(ADVANCE_ALGORITHM))
	bfs * bfsobj = new bfs(NAp, datasetobj->getdatasetid(), binaryFile);
	bfsobj->run();
	#endif
	#if (defined(ACTGRAPH_SETUP) & defined(ADVANCE_ALGORITHM))
	advance_op * advanceobj = new advance_op(NAp, datasetobj->getdatasetid(), binaryFile);
	advanceobj->run();
	#endif
	
	#ifdef _DEBUGMODE_TIMERS2
	std::cout << "TEST FINISHED" << std::endl; 
	utilityobj->stopBTIME("HOSTPROCESS:: TIMING SUMMARY: TOTAL TIME ELAPSED: ", begintime_overallexecution, NAp);
	#endif
	return EXIT_SUCCESS;
}

