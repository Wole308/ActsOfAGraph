#ifndef HOST_COMMON_H
#define HOST_COMMON_H
#include "config_params.h"
#include "common.h"
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#ifdef FPGA_IMPL
#include <ap_int.h>
// #include "CL/cl.h"
#include <CL/opencl.h>
#include "../xcl.h"
// #include "xcl2.hpp"
#endif
using namespace std;

typedef struct {
	string graphtopname;
	string graphname;
	string graph_path; // const char *
	string vertices_path;
	string edges_path;
	string vertices_path_bin;
	string edges_path_bin;
	vertex_t min_vertex;
	vertex_t max_vertex;
    vertex_t num_vertices;
	size_t num_edges;
	int graphdirectiontype;
	int graphorder;
	int graphgroup;
	unsigned int groupvoffset[4];
} dataset_t;

typedef struct {
	unsigned int graph_algorithmidx;
	unsigned int graph_iterationidx;
	unsigned int groupid;
	unsigned int groupbasevoffset;
} hostglobalparams_t;
#endif




