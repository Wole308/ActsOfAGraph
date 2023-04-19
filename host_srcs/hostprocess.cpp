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
	
	/* std::string binaryFile1;
	utility * utilityobj = new utility();
	#ifdef FPGA_IMPL
	if (argc < 2) { // 2,3
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }
	binaryFile1 = argv[5];
	#endif */
	
	
	
	if (argc < 1) { 
        std::cout << "Usage: " << argv[1] << " <algo:pr,sssp,etc.>" << std::endl;
		std::cout << "Usage: " << argv[2] << " <rootvid>" << std::endl;
		std::cout << "Usage: " << argv[3] << " <direction:0,1>" << std::endl;
		std::cout << "Usage: " << argv[4] << " <numiterations>" << std::endl;
		std::cout << "Usage: " << argv[5] << " <XCLBIN File>" << std::endl;
		std::cout << "Usage: " << argv[6] << " <graph_path:.mtx>" << std::endl;
        return EXIT_FAILURE;
    }
	utility * utilityobj = new utility();
	std::string binaryFile1 = argv[6];
	// void app::run(std::string algo, unsigned int rootvid, int graphisundirected, unsigned int numiterations, std::string _binaryFile1, string graph_path){
	
	
	
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime_overallexecution = std::chrono::steady_clock::now();
	#endif
	
	app * appobj = new app();	
	// void run(std::string algorithm, unsigned int numiterations, unsigned int rootvid, string graph_path, int graphisundirected, std::string _binaryFile1);
	appobj->run(argv[1], stoi(argv[2]), stoi(argv[3]), stoi(argv[4]), argv[5], argv[6]);
	// appobj->run("nap", "sssp", 1, "/home/oj2zf/Documents/dataset/soc-LiveJournal1.mtx", 1, 16, "NAp");
	
	#ifdef _DEBUGMODE_TIMERS3
	std::cout << endl << "TEST FINISHED" << std::endl; 
	cout<<"HOSTPROCESS:: FINISHED RUNNING "<<argv[4]<<endl;
	utilityobj->stopTIME("HOSTPROCESS:: TIMING SUMMARY: TOTAL TIME ELAPSED: ", begintime_overallexecution, NAp);
	#endif
	return EXIT_SUCCESS;
}

