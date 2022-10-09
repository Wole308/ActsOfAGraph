#ifndef APP_H
#define APP_H
#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <bits/stdc++.h> 
#include <iostream> 
#include <sys/stat.h> 
#include <sys/types.h>
#include <algorithm>
#include <thread>
#include <iostream>
#include <mutex>
#include <vector>
#include <iomanip>
#include "utility.h"
#include "act_pack.h"
#include "prepare_graph.h"
#include "algorithm.h"
#include "../acts_templates/acts_sw.h"
#include "../include/common.h"

class app {
public:
	app();
	~app();
	void finish();
	
	universalparams_t get_universalparams(std::string algorithm, unsigned int numiterations, unsigned int rootvid, unsigned int num_vertices, unsigned int num_edges, bool graphisundirected);
	
	void run(std::string setup, std::string algorithm, unsigned int numiterations, unsigned int rootvid, string graph_path, int graphisundirected, std::string _binaryFile1);
	
	void summary();
	
private:
	utility * utilityobj;
	// universalparams_t universalparams;
};
#endif








