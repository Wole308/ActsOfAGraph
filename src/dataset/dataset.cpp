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
	#ifdef _LARGEDATASET_1M
	datasetid = 1;
	#endif
	#ifdef _TWITTERDATASET_67M
	datasetid = 2;
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








