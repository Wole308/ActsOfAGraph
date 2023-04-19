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
#include "create_act_pack.h"
#include "prepare_graph.h"
// #include "make_graph.h"
#include "algorithm.h"
// #include "app_hw.h"
#include "host.h"
// #include "host_fpga.h"
// #include "host_fpga_async.h"
#include "../acts_templates/acts_kernel.h"	
#include "../include/common.h"

class app {
public:
	app();
	~app();
	void finish();
	
	universalparams_t get_universalparams(std::string algorithm, unsigned int numiterations, unsigned int rootvid, unsigned int num_vertices, unsigned int num_edges, bool graphisundirected);
	
	void run(std::string algo, unsigned int rootvid, int graphisundirected, unsigned int numiterations, std::string _binaryFile1, string graph_path);
	
	void summary();
	
private:
	utility * utilityobj;
	algorithm * algorithmobj;
};
#endif








