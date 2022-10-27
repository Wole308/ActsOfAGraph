#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "utility.h"
#include "app.h"
#include "../include/common.h"
using namespace std;

int main(int argc, char** argv){
	cout<<"Hostprocess:: Graph Analytics Started..."<<endl;
	
	std::string binaryFile1;
	utility * utilityobj = new utility();
	#ifdef FPGA_IMPL
	if (argc < 2) { // 2,3
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
	binaryFile1 = argv[5];
	#endif
	
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime_overallexecution = std::chrono::steady_clock::now();
	#endif
	
	app * appobj = new app();	
	// void run(std::string algorithm, unsigned int numiterations, unsigned int rootvid, string graph_path, int graphisundirected, std::string _binaryFile1);
	appobj->run(argv[1], argv[2], stoi(argv[3]), stoi(argv[4]), argv[5], stoi(argv[6]), argv[7]);
	// appobj->run("nap", "sssp", 16, 1, "/home/oj2zf/Documents/dataset/soc-LiveJournal1.mtx", 1, "NAp");
	// appobj->run("nap", "sssp", 2, 1, "/home/oj2zf/Documents/dataset/rmat_32m_256m.mtx", 1, "NAp");
	// appobj->run("nap", "sssp", 12, 1, "/home/oj2zf/Documents/dataset/com-Orkut.mtx", 1, "NAp");
	// appobj->run("nap", "pr", 8, 1, "/home/oj2zf/dataset/uk-2002.mtx", 1, "NAp");
	
	#ifdef _DEBUGMODE_TIMERS3
	std::cout << endl << "TEST FINISHED" << std::endl; 
	utilityobj->stopTIME("HOSTPROCESS:: TIMING SUMMARY: TOTAL TIME ELAPSED: ", begintime_overallexecution, NAp);
	#endif
	return EXIT_SUCCESS;
}

