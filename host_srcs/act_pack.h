#ifndef ACT_PACK_H
#define ACT_PACK_H
#include <mutex>
#include <thread>
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
#include "utility.h"
#include "algorithm.h"
#include "../include/common.h"
#include "act_pack.h"
using namespace std;

class act_pack {
public:
	act_pack(universalparams_t _universalparams);
	~act_pack();

	void pack(vector<edge_t> &vertexptrbuffer, vector<edge3_type> &edgedatabuffer, vector<edge3_vec_dt> (&edges_temp)[NUM_PEs], map_t * edges_map[NUM_PEs][MAX_NUM_UPARTITIONS]);
	
private:
	utility * utilityobj;
	universalparams_t universalparams;
};
#endif







