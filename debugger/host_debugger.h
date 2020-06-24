#ifndef HOST_DEBUGGER_H
#define HOST_DEBUGGER_H
#include "../config/config_params.h"
#include "../src/host_common.h"
#ifdef _DEBUGMODE_HEADER
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
#include "../src/host_common.h"
#ifdef FPGA_IMPL
#include <ap_int.h>
#include "CL/cl.h"
#include "../xcl.h"
// #include "xcl2.hpp"
#endif
#endif
using namespace std;

typedef struct {
	unsigned int offset;
  unsigned int weof;
} htracker_t;

#ifdef _DEBUGMODE_STATS
extern int kvpairsread;
extern int kvpairswritten;
#endif

#ifdef _DEBUGMODE_HEADER
void printallparameters(dataset_t dataset);
unsigned int hmax2(size_t val1, size_t val2);
void printkvdrams(uint512_vec_dt * kvdramA);
void printkvdramsII(uint512_vec_dt * kvdram, metadata_t * kvstats);
void printkvdramsIII(uint512_vec_dt * kvdram);
void printstatsIII(unsigned int size, metadata_t * kvstatsA);
void print_graph(string path_graph, edge_t begin, edge_t size);
void check_kvdram2(uint512_vec_dt * kvdramX, metadata_t * kvstats, vertex_t vbegin, vertex_t applyvertexbuffersz, unsigned int treedepth, string message);
void checkforzeroornegative(int val, string message);
void printfirstkvs(string message, uint512_vec_dt kvs, unsigned int count);
void printERROR(string message);
#endif
#endif
