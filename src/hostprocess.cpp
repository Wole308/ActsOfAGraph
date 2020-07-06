#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "host_common.h"
#include "../debugger/host_debugger.h"
#include "../graphs/graph.h"
#include "../utility/utility.h"
#include "../algorithm/algorithm.h"
#include "../graphs/generateactivevertices.h"
#include "EdgeProcess.h" // Grafboost header
#include "VertexValues.h" // Grafboost header
#include "sortreduce.h" // Grafboost header
#include "filekvreader.h" // Grafboost header
#include "actgraph_pr_sw.h"
#include "grafboost_pr_sw.h"
#include "actgraph_bfs_sw.h"
#include "actgraph_bc_sw.h"
#include "grafboost_bfs_sw.h"
#include "grafboost_bc_sw.h"
// #include "../kernels/kernelprocess.h"
// #include "host_legion.h"
// #include "../kernels/legion.h"
#include "host_enigma.h"
// #include "../kernels/enigma.h"
// #include "host_titan.h"
// #include "../kernels/titan.h"
#include "../graphs/createsmartgraph.h"
#include "../graphs/create2Dgraph.h"
#include "../kernels/srkernelprocess.h"
#include "sortreduce.h"
#include "types.h"
#include "sortreduce.h"
#include "filekvreader.h"
#include "types.h"
#include "EdgeProcess.h"
#include "VertexValues.h"
#include "../heuristics/heuristics.h"
#ifdef FPGA_IMPL
// #include "xcl2.hpp"
#include "../xcl.h"
#endif
using namespace std;
#define YES
#define TESTKERNEL

#ifdef YES
unsigned long globaldebugger_totalbytesreadfromfile; 
unsigned long globaldebugger_totalbyteswrittentofile; 
edge_t * globaldebugger_totalkvstats;
edge_t globaldebugger_totalkeyvaluesstransferredtokernel;
edge_t globaldebugger_totalkeyvaluesstransferredfromkernel;
#endif

int main(int argc, char** argv){
	cout<<"Hostprocess:: Graph Analytics Started..."<<endl;
	#ifdef LOCKE
	if(NUMCPUTHREADS != 4){ std::cout<<"hostprocess:: WARNING: no need to use LOCKE setting with more than 1 thread. returning..."<<std::endl; return EXIT_SUCCESS; }
	#endif
	#ifdef GRAFBOOST_SETUP
	if(NUMCPUTHREADS < 2){ std::cout<<"hostprocess:: WARNING: grafboost not working with more than 2 threads. returning..."<<std::endl; return EXIT_SUCCESS; }
	#endif
	#ifdef ACTGRAPH_SETUP
	if(NUMCPUTHREADS > MAXNUMVERTEXBANKS){ std::cout<<"hostprocess:: WARNING: maxed out on available parallelism. NUMCPUTHREADS is greater than MAXNUMVERTEXBANKS. returning..."<<std::endl; return EXIT_SUCCESS; }	
	#endif 
	
	// Preparations
	#ifdef FPGA_IMPL
	if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string binaryFile = argv[1];
	#endif
	
	// heuristics object 
	heuristics * heuristicsobj = new heuristics();
	algorithm * algorithmobj = new algorithm();
	utility * utilityobj = new utility();
	
	// graph object
	unsigned int datasetid;
	#ifdef _LARGEDATASET_1M
	datasetid = 1;
	#endif 
	#ifdef _LARGEDATASET_67M
	datasetid = 4;
	#endif
	#ifdef _LARGEDATASET_268M
	datasetid = 5;
	#endif
	#ifdef _LARGEDATASET_1B
	datasetid = 6;
	#endif
	#ifdef _LARGEDATASET_4B
	datasetid = 7;
	#endif
	graph * graphobj = new graph(algorithmobj, datasetid, heuristicsobj->getdefaultnumvertexbanks(), heuristicsobj->getdefaultnumedgebanks());
	printallparameters(graphobj->getdataset());
	
	// implementing kernel_legion
	#ifdef TESTKERNEL
	host_enigma * hostXobj = new host_enigma(graphobj);
	#ifdef FPGA_IMPL
	hostXobj->loadOCLstructures(binaryFile);
	#endif
	hostXobj->run();
	hostXobj->finish();
	exit(EXIT_SUCCESS);
	#endif 
	
	// create graph
	#ifdef _GENERATE2DGRAPH
	cout<<"graphobj->getnumedgebanks(): "<<graphobj->getnumedgebanks()<<endl;
	if(graphobj->getnumedgebanks() == 16){		
		create2Dgraph * create2Dgraphobj = new create2Dgraph(graphobj, 0, (graphobj->getnumedgebanks() / 4));
		create2Dgraphobj->start();
		
		create2Dgraphobj->reload((graphobj->getnumedgebanks() / 4), (graphobj->getnumedgebanks() / 4));
		create2Dgraphobj->start();
		
		create2Dgraphobj->reload((graphobj->getnumedgebanks() / 2), (graphobj->getnumedgebanks() / 4));
		create2Dgraphobj->start();
		
		create2Dgraphobj->reload(((graphobj->getnumedgebanks() * 3) / 4), (graphobj->getnumedgebanks() / 4));
		create2Dgraphobj->start();
		
	} else if (graphobj->getnumedgebanks() == 4){		
		create2Dgraph * create2Dgraphobj = new create2Dgraph(graphobj, 0, (graphobj->getnumedgebanks() / 2));
		create2Dgraphobj->start();
		
		create2Dgraphobj->reload((graphobj->getnumedgebanks() / 2), (graphobj->getnumedgebanks() / 2));
		create2Dgraphobj->start();
		
	} else if (graphobj->getnumedgebanks() == 1){
		create2Dgraph * create2Dgraphobj = new create2Dgraph(graphobj, 0, graphobj->getnumedgebanks());
		create2Dgraphobj->start();
	}	
	cout<<"hostprocess:: finished generating 2D graph."<<endl;
	exit(EXIT_SUCCESS);
	#endif
	#ifdef _GENERATEACTIVEVERTICES
	generateactivevertices * generateactiveverticesobj = new generateactivevertices(graphobj);
	generateactiveverticesobj->generate();
	exit(EXIT_SUCCESS);
	#endif 
	#ifdef _GENERATESMARTGRAPH
	graphobj->openfilesforreading();
	createsmartgraph * createsmartgraphobj = new createsmartgraph(graphobj);
	createsmartgraphobj->create();
	exit(EXIT_SUCCESS);
	#endif
	
	// Variables declaration
	#ifdef YES
	srand (time(NULL));
	float totaltime_topkernel_ms; totaltime_topkernel_ms = 0;
	float totaltime_populatekvdram_ms; totaltime_populatekvdram_ms = 0;
	float totaltime_populatekvdram2_ms; totaltime_populatekvdram2_ms = 0;
	float totaltime_fetchactvvertices_ms; totaltime_fetchactvvertices_ms = 0;
	float totaltime_savevertexupdates_ms; totaltime_savevertexupdates_ms = 0;
	unsigned int it_size = 0;
	globaldebugger_totalbytesreadfromfile = 0;
	globaldebugger_totalbyteswrittentofile = 0;	
	globaldebugger_totalkvstats = new edge_t[KVSTATS_SIZE];
	for(unsigned int p=0; p<KVSTATS_SIZE; p++){ globaldebugger_totalkvstats[p] = 0; }
	globaldebugger_totalkeyvaluesstransferredtokernel = 0;
	globaldebugger_totalkeyvaluesstransferredfromkernel = 0;
	#endif 
	
	// open files
	graphobj->openfilesforreading();
	graphobj->opentemporaryfilesforwriting();
	graphobj->opentemporaryfilesforreading();

	// initializations (ACTGraph)
	#ifdef ACTGRAPH_SETUP
	graphobj->generateverticesdata();
	graphobj->generatevertexproperties();
	#endif
	
	#ifdef _DEBUGMODE_TIMERS2
	std::chrono::steady_clock::time_point begintime_overallexecution = std::chrono::steady_clock::now();
	#endif
	#if (defined(ACTGRAPH_SETUP) & defined(PR_ALGORITHM) & defined(SW))
	actgraph_pr_sw * actgraph_pr_sw_obj = new actgraph_pr_sw(graphobj);
	actgraph_pr_sw_obj->run();
	actgraph_pr_sw_obj->finish();
	#endif 
	#if (defined(ACTGRAPH_SETUP) & defined(BFS_ALGORITHM) & defined(SW))
	actgraph_pr_sw * actgraph_pr_sw_obj = new actgraph_pr_sw(graphobj);		
	actgraph_bfs_sw * actgraph_bfs_sw_obj = new actgraph_bfs_sw(graphobj, heuristicsobj, actgraph_pr_sw_obj, "");					
	actgraph_bfs_sw_obj->run();
	actgraph_bfs_sw_obj->finish();
	#endif
	#if (defined(ACTGRAPH_SETUP) & defined(BC_ALGORITHM) & defined(SW))
	actgraph_pr_sw * actgraph_pr_sw_obj = new actgraph_pr_sw(graphobj);	
	actgraph_bc_sw * actgraph_bc_sw_obj = new actgraph_bc_sw(graphobj, heuristicsobj, actgraph_pr_sw_obj, "");					
	actgraph_bc_sw_obj->forwardrun();
	actgraph_bc_sw_obj->reverserun();
	actgraph_bc_sw_obj->finish();
	#endif
	
	#if (defined(ACTGRAPH_SETUP) & defined(PR_ALGORITHM) & defined(FPGA_IMPL))
	actgraph_pr_sw * actgraph_pr_sw_obj = new actgraph_pr_sw(graphobj);
	actgraph_pr_sw_obj->loadOCLstructures(binaryFile);
	actgraph_pr_sw_obj->run();
	actgraph_pr_sw_obj->finish();
	#endif 
	#if (defined(ACTGRAPH_SETUP) & defined(BFS_ALGORITHM) & defined(FPGA_IMPL))
	actgraph_pr_sw * actgraph_pr_sw_obj = new actgraph_pr_sw(graphobj);
	actgraph_bfs_sw * actgraph_bfs_sw_obj = new actgraph_bfs_sw(graphobj, heuristicsobj, actgraph_pr_sw_obj, binaryFile);
	actgraph_bfs_sw_obj->loadOCLstructures(binaryFile);
	actgraph_bfs_sw_obj->run();
	actgraph_bfs_sw_obj->finish();
	#endif 
	#if (defined(ACTGRAPH_SETUP) & defined(BC_ALGORITHM) & defined(FPGA_IMPL))
	actgraph_pr_sw * actgraph_pr_sw_obj = new actgraph_pr_sw(graphobj);		
	actgraph_bc_sw * actgraph_bc_sw_obj = new actgraph_bc_sw(graphobj, heuristicsobj, actgraph_pr_sw_obj, binaryFile);
	actgraph_bc_sw_obj->loadOCLstructures(binaryFile);
	actgraph_bc_sw_obj->forwardrun();
	actgraph_bc_sw_obj->reverserun();
	actgraph_bc_sw_obj->finish();
	#endif
	
	#if (defined(GRAFBOOST_SETUP) & defined(PR_ALGORITHM))
	grafboost_pr_sw * grafboost_pr_sw_obj = new grafboost_pr_sw(graphobj);	
	grafboost_pr_sw_obj->run();
	grafboost_pr_sw_obj->finish();
	#endif 
	#if (defined(GRAFBOOST_SETUP) & defined(BFS_ALGORITHM))
	grafboost_bfs_sw * grafboost_bfs_sw_obj = new grafboost_bfs_sw(graphobj);	
	grafboost_bfs_sw_obj->run();
	grafboost_bfs_sw_obj->finish();
	#endif 
	#if (defined(GRAFBOOST_SETUP) & defined(BC_ALGORITHM))
	grafboost_bc_sw * grafboost_bc_sw_obj = new grafboost_bc_sw(graphobj);	
	grafboost_bc_sw_obj->forwardrun();
	grafboost_bc_sw_obj->reverserun();
	grafboost_bc_sw_obj->finish();
	#endif 
	#ifdef _DEBUGMODE_TIMERS2
	std::cout << "TEST FINISHED" << std::endl; 
	// printallparameters(graphobj->getdataset());
	std::chrono::steady_clock::time_point endtime_overallexecution = std::chrono::steady_clock::now();
	float timeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds> (endtime_overallexecution - begintime_overallexecution).count();
	float timeelapsed_s = std::chrono::duration_cast<std::chrono::seconds> (endtime_overallexecution - begintime_overallexecution).count();
	float timeelapsed_populatekvdram1_ms = totaltime_populatekvdram_ms;
	float timeelapsed_populatekvdram2_ms = totaltime_populatekvdram2_ms;	
	float timeelapsed_populatekvdram_ms = 
				#ifdef ACTGRAPH_SETUP
				totaltime_populatekvdram2_ms;
				#else 
				totaltime_populatekvdram_ms;
				#endif				
	float timeelapsed_C1_ms = timeelapsed_ms;
	float timeelapsed_C2_ms = timeelapsed_C1_ms - timeelapsed_populatekvdram_ms;	
	float timeelapsed_C3_ms = timeelapsed_C1_ms - timeelapsed_populatekvdram_ms - totaltime_fetchactvvertices_ms;
	float timeelapsed_C4_ms = timeelapsed_C1_ms - totaltime_fetchactvvertices_ms;	
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [FETCHING ACTIVE VERTICES]: " << (totaltime_fetchactvvertices_ms / 1000) << " [seconds]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [POPULATE KVDRAM 1]: " << (timeelapsed_populatekvdram1_ms / 1000) << " [seconds]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [POPULATE KVDRAM 2]: " << (timeelapsed_populatekvdram2_ms / 1000) << " [seconds]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [POPULATE KVDRAM]: " << (timeelapsed_populatekvdram_ms / 1000) << " [seconds]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [SAVE VERTEX UPDATES]: " << (totaltime_savevertexupdates_ms / 1000) << " [seconds]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [KERNEL]: " << (totaltime_topkernel_ms / 1000) << " [seconds]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [C1]: " << timeelapsed_ms << " [milli seconds]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [C1]: " << (timeelapsed_C1_ms / 1000) << " [seconds]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [C2]: " << (timeelapsed_C2_ms / 1000) << " [seconds]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [C3]: " << (timeelapsed_C3_ms / 1000) << " [seconds]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: TOTAL TIME ELAPSED [C4*]: " << (timeelapsed_C4_ms / 1000) << " [seconds]" << RESET << std::endl;	
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: globaldebugger_totalbytesreadfromfile: " << globaldebugger_totalbytesreadfromfile << " [bytes]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: globaldebugger_totalbyteswrittentofile: " << globaldebugger_totalbyteswrittentofile << " [bytes]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: globaldebugger_totalkeyvaluesstransferredtokernel: " << globaldebugger_totalkeyvaluesstransferredtokernel << " [key values]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: globaldebugger_totalkeyvaluesstransferredfromkernel: " << globaldebugger_totalkeyvaluesstransferredfromkernel << " [key values]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: globaldebugger_totalkvstats["<<0<<"]: " << globaldebugger_totalkvstats[0] << " [key values]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: globaldebugger_totalkvstats["<<((KVSTATS_SIZE-1) / 2)<<"]: " << globaldebugger_totalkvstats[((KVSTATS_SIZE-1) / 2)] << " [key values]" << RESET << std::endl;
	std::cout<< TIMINGRESULTSCOLOR  << ">>> TIMING SUMMARY: globaldebugger_totalkvstats["<<(KVSTATS_SIZE-1)<<"]: " << globaldebugger_totalkvstats[(KVSTATS_SIZE-1)] << " [key values]" << RESET << std::endl;	
	std::cout << std::endl;
	#endif

	#ifdef ACTGRAPH_SETUP
	graphobj->closefilesforreading();
	graphobj->closetemporaryfilesforwriting();
	graphobj->closetemporaryfilesforreading();
	#endif
	return EXIT_SUCCESS;
}
