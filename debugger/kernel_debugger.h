#ifndef KERNEL_DEBUGGER_H
#define KERNEL_DEBUGGER_H
#include "../config/config_params.h"
#include "../src/common.h"
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
using namespace std;
#ifdef FPGA_IMPL
#include <ap_int.h>
#endif
#endif

typedef struct {
	unsigned int offset;
  unsigned int weof;
} htracker_t;

#ifdef _DEBUGMODE_STATS
extern int kvpairsread;
extern int kvpairswritten;
#endif

#ifdef _DEBUGMODE_HEADER
void printstats_kernel(metadata_t * stats, unsigned int size, std::string message, unsigned int val1, unsigned int val2, unsigned int val3);
void printstats2_kernel(smallmetadata_t * stats, unsigned int size, unsigned int val1, unsigned int val2, unsigned int val3);
void printstats3_kernel(smalluint_t * stats, unsigned int size, string message, unsigned int val1, unsigned int val2, unsigned int val3);
void printkvbuffer_kernel(uint512_dt * kvbuffer, unsigned int size, string message);
void printkvs2_kernel(keyvalue_t * kvs, string message);
void print1int_kernel(string context, string message1, unsigned int val1);
void print2ints_kernel(string context, string message1, unsigned int val1, string message2, unsigned int val2);
void print4ints_kernel(string context, string message1, unsigned int val1, string message2, unsigned int val2, string message3, unsigned int val3, string message4, unsigned int val4);
void printovs2_kernel(margin_t * ov, unsigned int p, std::string message, unsigned int val1, unsigned int val2, unsigned int val3);

unsigned int getdestskipsize_kernel(int currentLOP, int param);
void checkforoutofbounds_kernel(unsigned int val, unsigned int compsz, std::string message, unsigned int val1, unsigned int val2, unsigned int val3);
void check_kvdram_kernel(uint512_dt * kvdramA, unsigned int baseaddr_kvs, metadata_t * kvstats_tmp, unsigned int partitionoffset, unsigned int currentLOP, unsigned int treedepth, unsigned int finalnumpartitions, unsigned int kvdata_range);
void checkentryforoutofbounds_kernel(unsigned int partitionoffset, unsigned int partition, unsigned int currentLOP, unsigned int treedepth, unsigned int kvdata_range, unsigned int data, string message, unsigned int val1, unsigned int val2, unsigned int val3);	
void checkforzeroornegative_kernel(int val, string message);
#endif
#endif