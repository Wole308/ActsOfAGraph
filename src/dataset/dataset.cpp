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
	datasetid = 1;
	#endif
	#ifdef _HOLLYWOOD_1M_57M
	datasetid = 2;
	#endif
	#ifdef _KRON20_1M_45M
	datasetid = 3;
	#endif
	#ifdef _KRON21_2M_91M
	datasetid = 4;
	#endif
	#ifdef _KRON22_4M_200M
	datasetid = 5;
	#endif
	
	#ifdef _RMAT22_SPARSE0
	datasetid = 30;
	#endif
	#ifdef _RMAT22_SPARSE1
	datasetid = 31;
	#endif
	#ifdef _RMAT22_SPARSE2
	datasetid = 32;
	#endif
	#ifdef _RMAT22_SPARSE3
	datasetid = 33;
	#endif
	#ifdef _RMAT22_SPARSE4
	datasetid = 34;
	#endif
	#ifdef _RMAT22_SPARSE5
	datasetid = 35;
	#endif
	#ifdef _RMAT22_SPARSE6
	datasetid = 36;
	#endif
	#ifdef _RMAT22_SPARSE7
	datasetid = 37;
	#endif
	
	#ifdef _RMAT_RANGE0
	datasetid = 40;
	#endif
	#ifdef _RMAT_RANGE1
	datasetid = 41;
	#endif
	#ifdef _RMAT_RANGE2
	datasetid = 42;
	#endif
	#ifdef _RMAT_RANGE3
	datasetid = 43;
	#endif
	#ifdef _RMAT_RANGE4
	datasetid = 44;
	#endif
	#ifdef _RMAT_RANGE5
	datasetid = 45;
	#endif
	#ifdef _RMAT_RANGE6
	datasetid = 46;
	#endif
	#ifdef _RMAT_RANGE7
	datasetid = 47;
	#endif
	
	#ifdef _INDOCHINA_7M_194M
	datasetid = 10;
	#endif
	#ifdef _RGG_4M_32M
	datasetid = 11;
	#endif
	#ifdef _RGG_17M_132M
	datasetid = 12;
	#endif
	#ifdef _ROADNET_2M_3M
	datasetid = 13;
	#endif
	#ifdef _FLICKR_1M_10M
	datasetid = 14;
	#endif
	
	// large dataset 
	#ifdef _TWITTER_67M
	datasetid = 20;
	#endif
	#ifdef _MOLIERE2016_33M
	datasetid = 21;
	#endif
	#ifdef _LARGEDATASET_67M
	datasetid = 22;
	#endif
	#ifdef _LARGEDATASET_268M
	datasetid = 23;
	#endif
	#ifdef _LARGEDATASET_1B
	datasetid = 24;
	#endif
	#ifdef _LARGEDATASET_4B
	datasetid = 25;
	#endif
	return datasetid;
}










