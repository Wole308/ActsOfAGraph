#ifndef CREATE_ACT_PACK_H
#define CREATE_ACT_PACK_H
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
using namespace std;

class create_act_pack {
public:
	create_act_pack(universalparams_t _universalparams);
	~create_act_pack();

	unsigned int create_actpack(action_t action, unsigned int destedgeupdates_baseoffset, unsigned int destedgeupdateptrs_baseoffset, edge_update_type * URAM_edges[NUM_VALID_PEs][EDGE_PACK_SIZE], unsigned int _MAX_BUFFER_SIZE, unsigned int _MAX_WORKBUFFER_SIZE, unsigned int globalparams[GLOBALBUFFER_SIZE], HBM_channelAXISW_t * HBM_channelA[NUM_PEs], HBM_channelAXISW_t * HBM_channelB[NUM_PEs], vector<edge3_type> (&final_edge_updates)[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS]);
	
private:
	utility * utilityobj;
	universalparams_t universalparams;
};
#endif







