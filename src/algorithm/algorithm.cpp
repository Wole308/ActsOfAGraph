#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include "algorithm.h"
using namespace std;

algorithm::algorithm(){}
algorithm::~algorithm(){} 

unsigned algorithm::vertex_initdata(unsigned int Algo, unsigned int vid) {
	if(Algo == PAGERANK){ return 1; } 
	else if(Algo == CF){ return 1; } 
	else if(Algo == CC){ return 1; } 
	#ifdef ALGORITHMTYPE_REPRESENTVDATASASBITS
	else if(Algo == BFS){ 0b0; } // return MAXVDATA; } 0b01111111111111111111111111111111
	#else 
	else if(Algo == BFS){ return MAXVDATA; } 
	#endif 
	else if(Algo == SSSP){ return MAXVDATA; } 
	else { return 0; }
	return 0;
}






