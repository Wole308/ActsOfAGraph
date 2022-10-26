#ifndef ACTS_SW_H
#define ACTS_SW_H
#include "../include/common.h"
#ifndef FPGA_IMPL
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
#include <bits/stdc++.h> 
#include <iostream> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <algorithm>
#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include <string.h>
#include <mutex>
#include "../host_srcs/algorithm.h"
#include "../host_srcs/utility.h"
#endif 
#ifndef FPGA_IMPL
using namespace std;
#endif 

class acts_kernel {
public:
	acts_kernel(universalparams_t universalparams);
	~acts_kernel();
	
	void top_function(HBM_channelX_t * HBM_center,  HBM_channelTHIS_t * HBM_channelA0, HBM_channelTHIS_t * HBM_channelB0, HBM_channelTHIS_t * HBM_channelA1, HBM_channelTHIS_t * HBM_channelB1, HBM_channelTHIS_t * HBM_channelA2, HBM_channelTHIS_t * HBM_channelB2, HBM_channelTHIS_t * HBM_channelA3, HBM_channelTHIS_t * HBM_channelB3, HBM_channelTHIS_t * HBM_channelA4, HBM_channelTHIS_t * HBM_channelB4, HBM_channelTHIS_t * HBM_channelA5, HBM_channelTHIS_t * HBM_channelB5, HBM_channelTHIS_t * HBM_channelA6, HBM_channelTHIS_t * HBM_channelB6, HBM_channelTHIS_t * HBM_channelA7, HBM_channelTHIS_t * HBM_channelB7, HBM_channelTHIS_t * HBM_channelA8, HBM_channelTHIS_t * HBM_channelB8, HBM_channelTHIS_t * HBM_channelA9, HBM_channelTHIS_t * HBM_channelB9, HBM_channelTHIS_t * HBM_channelA10, HBM_channelTHIS_t * HBM_channelB10, HBM_channelTHIS_t * HBM_channelA11, HBM_channelTHIS_t * HBM_channelB11, HBM_channelTHIS_t * HBM_channelA12, HBM_channelTHIS_t * HBM_channelB12, HBM_channelTHIS_t * HBM_channelA13, HBM_channelTHIS_t * HBM_channelB13, HBM_channelTHIS_t * HBM_channelA14, HBM_channelTHIS_t * HBM_channelB14, HBM_channelTHIS_t * HBM_channelA15, HBM_channelTHIS_t * HBM_channelB15, HBM_channelTHIS_t * HBM_channelA16, HBM_channelTHIS_t * HBM_channelB16, HBM_channelTHIS_t * HBM_channelA17, HBM_channelTHIS_t * HBM_channelB17, HBM_channelTHIS_t * HBM_channelA18, HBM_channelTHIS_t * HBM_channelB18, HBM_channelTHIS_t * HBM_channelA19, HBM_channelTHIS_t * HBM_channelB19, HBM_channelTHIS_t * HBM_channelA20, HBM_channelTHIS_t * HBM_channelB20, HBM_channelTHIS_t * HBM_channelA21, HBM_channelTHIS_t * HBM_channelB21, HBM_channelTHIS_t * HBM_channelA22, HBM_channelTHIS_t * HBM_channelB22, HBM_channelTHIS_t * HBM_channelA23, HBM_channelTHIS_t * HBM_channelB23);

private:
	utility * utilityobj;
	universalparams_t universalparams;
	algorithm * algorithmobj;
};
#endif






