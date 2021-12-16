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
	
	std::string binaryFile1;
	std::string binaryFile2;
	dataset * datasetobj = new dataset();
	utility * utilityobj = new utility();
	#ifdef FPGA_IMPL
	#ifdef ACTS_2by1by1
	if (argc != 3) { // 2,3
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    binaryFile1 = argv[1];
	binaryFile2 = argv[2];
	#else 
	if (argc != 2) { // 2,3
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    binaryFile1 = argv[1];
	binaryFile2 = binaryFile1;	
	#endif 
	#endif
	/* #ifdef FPGA_IMPL
	if (argc != 2) { // 2,3
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    binaryFile1 = argv[1];
	binaryFile2 = binaryFile1;
	#endif */
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
	// makeundirectedgraph_orderbyindegree * createundirectedgraphobj2 = new makeundirectedgraph_orderbyindegree(datasetobj->getdatasetid());
	// createundirectedgraphobj2->start();
	// exit(EXIT_SUCCESS);
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

