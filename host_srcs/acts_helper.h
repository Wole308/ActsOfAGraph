#ifndef ACTS_HELPER_H
#define ACTS_HELPER_H
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
#include "utility.h"
#include "algorithm.h"
#include "../include/common.h"
using namespace std;

class acts_helper {
public:
	acts_helper(universalparams_t universalparams);
	acts_helper();
	~acts_helper();
	
	unsigned int gethash(unsigned int vid);
	unsigned int getlocalvid(unsigned int vid);
	
	unsigned int extract_stats(vector<vertex_t> &srcvids, vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, long double edgesprocessed_totals[128], tuple_t * iteration_stats[MAXNUMGRAPHITERATIONS], tuple_t * vpmaskstats_merge[MAXNUMGRAPHITERATIONS][NUM_PEs], unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS]);

	unsigned int getfeedback(string message, string graphpath, uint512_vec_dt * vdram, uint512_vec_dt * vdramtemp0, uint512_vec_dt * vdramtemp1, uint512_vec_dt * vdramtemp2, uint512_vec_dt * kvbuffer[NUM_PEs], universalparams_t universalparams);
	
	void verifyresults(uint512_vec_dt * vbuffer, globalparams_t globalparams, universalparams_t universalparams);
	
private:
	utility * utilityobj;
	universalparams_t myuniversalparams;
};
#endif







