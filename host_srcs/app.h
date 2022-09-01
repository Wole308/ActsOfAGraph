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
#include "swkernel.h"
#include "goclkernel.h"
#include "utility.h"
#include "acts_helper.h"
#include "algorithm.h"
#include "loadgraph.h"
#include "loadedges.h"
#include "prepare_graph.h"
#include "../include/common.h"

class app {
public:
	app();
	~app();
	void finish();
	
	universalparams_t get_universalparams(std::string algorithm, unsigned int numiterations, unsigned int rootvid, unsigned int num_vertices, unsigned int num_edges, bool graphisundirected);
	void printallfeedback(string message, string graphpath, uint512_vec_dt * vdram, uint512_vec_dt * vdramtemp0, uint512_vec_dt * vdramtemp1, uint512_vec_dt * vdramtemp2, uint512_vec_dt * kvbuffer[NUM_PEs], universalparams_t universalparams);				
	
	void run(std::string setup, std::string algorithm, unsigned int numiterations, unsigned int rootvid, string graph_path, int graphisundirected, std::string _binaryFile1);
	
	void summary(string graph_path, uint512_vec_dt * kvdram, globalparams_t globalparams);
	
private:
	utility * utilityobj;
	loadgraph * loadgraphobj;
	loadedges * loadedgesobj;
	swkernel * swkernelobj;
	algorithm * algorithmobj;
	// acts_helper * actshelperobj;
	
	uint512_vec_dt * kvbuffer[MAXNUM_PEs];
	uint512_vec_dt * vdram;
	uint512_vec_dt * mdram;
	edge_t * vptrs[MAXNUM_PEs];
	uint512_vec_dt * edges[MAXNUM_PEs];
	
	std::string binaryFile[2];
	
	#if defined(SW)
	swkernel * kernelobj;	
	#elif defined(SW_ALLINONE)
	swkernel * kernelobj;
	#elif defined(FPGA_IMPL)
	goclkernel * kernelobj;
	#else 
	NOT DEFINED.
	#endif 
	
	// universalparams_t universalparams;
};
#endif








