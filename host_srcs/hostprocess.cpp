#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "utility.h"
#include "algorithm.h"
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
	// void run(std::string algorithm, unsigned int numiterations, unsigned int rootvid, string graph_path, std::string _binaryFile1);
	appobj->run(argv[1], argv[2], stoi(argv[3]), stoi(argv[4]), argv[5], argv[6]);
	// appobj->run("nap", "pr", 8, 1, "/home/oj2zf/dataset/com-Orkut.mtx", "NAp");
	
	#ifdef _DEBUGMODE_TIMERS3
	std::cout << "TEST FINISHED" << std::endl; 
	utilityobj->stopBTIME("HOSTPROCESS:: TIMING SUMMARY: TOTAL TIME ELAPSED: ", begintime_overallexecution, NAp);
	#endif
	return EXIT_SUCCESS;
}

