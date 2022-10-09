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
	
	void run(vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, unsigned int * v_ptr[NUM_PEs], vector<edge2_type> edges_in_channel[NUM_PEs], vector<edge3_vec_dt> (&edges_temp)[NUM_PEs], map_t * edges_map[NUM_PEs][MAX_NUM_UPARTITIONS]);

private:
	utility * utilityobj;
	universalparams_t universalparams;
	algorithm * algorithmobj;
};
#endif







