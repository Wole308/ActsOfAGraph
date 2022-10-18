#ifndef ACTS_SW_H
#define ACTS_SW_H
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
#include "../include/common.h"
using namespace std;

class acts_sw {
public:
	acts_sw(universalparams_t universalparams);
	~acts_sw();
	
	void run(HBM_channel_t * HBM_center, HBM_channelTHIS_t * HBM_channelA, HBM_channelTHIS_t * HBM_channelB);						

private:
	utility * utilityobj;
	universalparams_t universalparams;
	algorithm * algorithmobj;
};
#endif







