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
	
	void top_function( HBM_channelAXI_t * HBM_channelA0, HBM_channelAXI_t * HBM_channelB0, HBM_channelAXI_t * HBM_channelA1, HBM_channelAXI_t * HBM_channelB1, HBM_channelAXI_t * HBM_channelA2, HBM_channelAXI_t * HBM_channelB2, HBM_channelAXI_t * HBM_channelA3, HBM_channelAXI_t * HBM_channelB3, HBM_channelAXI_t * HBM_channelA4, HBM_channelAXI_t * HBM_channelB4, HBM_channelAXI_t * HBM_channelA5, HBM_channelAXI_t * HBM_channelB5, HBM_channelAXI_t * HBM_centerA, HBM_channelAXI_t * HBM_centerB, unsigned int batch);

private:
	utility * utilityobj;
	universalparams_t universalparams;
	algorithm * algorithmobj;
};
#endif






