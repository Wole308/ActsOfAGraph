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
	void print_active_partitions(unsigned int GraphIter, uint512_vec_dt * dram, universalparams_t universalparams, unsigned int num_partitions, bool printA, bool printB);
	
	void run(std::string setup, std::string algorithm, unsigned int numiterations, unsigned int rootvid, string graph_path, int graphisundirected, std::string _binaryFile1);
	
	void verifyresults(string graph_path, uint512_vec_dt * kvdram, globalparams_t globalparams);
	
private:
	utility * utilityobj;
	loadgraph * loadgraphobj;
	loadedges * loadedgesobj;
	swkernel * swkernelobj;
	algorithm * algorithmobj;
	// acts_helper * actshelperobj;
	
	uint512_vec_dt * kvbuffer[MAXNUMSUBCPUTHREADS];
	uint512_vec_dt * vdram;
	uint512_vec_dt * mdram;
	edge_t * vptrs[MAXNUMSUBCPUTHREADS];
	uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS];
	
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








