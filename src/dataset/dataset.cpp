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
#include "../../include/host_common.h"
#include "../../include/common.h"
#include "dataset.h"
using namespace std;

dataset::dataset(){}
dataset::~dataset(){} 

unsigned int dataset::getdatasetid(){
	unsigned int datasetid = 0;
	
	// small dataset 
	#ifdef _ORKUT_3M_106M
	datasetid = 30;
	#endif
	#ifdef _HOLLYWOOD_1M_57M
	datasetid = 31;
	#endif
	#ifdef _INDOCHINA_7M_194M
	datasetid = 32;
	#endif
	#ifdef _KRON21_2M_91M
	datasetid = 33;
	#endif
	#ifdef _RGG_17M_132M
	datasetid = 34;
	#endif
	#ifdef _ROADNET_2M_3M
	datasetid = 35;
	#endif
	#ifdef _FLICKR_1M_10M
	datasetid = 36;
	#endif
	
	// large dataset 
	#ifdef _TWITTER_67M
	datasetid = 2;
	#endif
	#ifdef _MOLIERE2016_33M
	datasetid = 3;
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
	return datasetid;
}










