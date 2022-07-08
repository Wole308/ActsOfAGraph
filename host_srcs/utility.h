#ifndef UTILITY_H
#define UTILITY_H

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
#include "../include/common.h"
#ifdef FPGA_IMPL
#include <ap_int.h>
// #include "CL/cl.h"
#include <CL/opencl.h>
#include "../xcl.h"
// #include "xcl2.hpp"
#endif
using namespace std;

#define UNVISITED 0
#define VISITED_IN_CURRENT_ITERATION 1
#define VISITED_IN_PAST_ITERATION 3

class utility {
public:
	utility(universalparams_t universalparams);
	utility();
	~utility();
	
	void printallparameters();
	void print1(string messagea, unsigned int dataa);
	void print2(string messagea, string messageb, unsigned int dataa, unsigned int datab);
	void print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad);
	void print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae);
	void print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef);
	
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int skipsize);
	void printvalues(string message, unsigned int * values, unsigned int size);
	
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
	void checkforlessthanthan(string message, unsigned int data1, unsigned int data2);
	
	void stopTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx);

	bool isbufferused(unsigned int id);
	
	size_t hsub(size_t val1, size_t val2);
	size_t hceildiv(size_t val1, size_t val2);
	int hmin(size_t val1, size_t val2);
	int hmax(size_t val1, size_t val2);
	void InsertBit(unsigned int * edgeoffsetbits, size_t index, unsigned int bit);
	unsigned int RetrieveBit(unsigned int * edgeoffsetbits, size_t index);
	void resetkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void resetkeyvalues(keyvalue_t * keyvalues, unsigned int size);

	unsigned int allignlower_FACTOR(unsigned int val, unsigned int _FACTOR);
	unsigned int allignhigher_FACTOR(unsigned int val, unsigned int _FACTOR);

	void calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int * skipspacing, unsigned int allign_factor);
	void calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int allign_factor);
	void getmarkerpositions(keyvalue_t * stats, unsigned int size, unsigned int allign_factor);
	
	void calculateunallignedoffsets(keyvalue_t * keyvalues, unsigned int size);
	
	// compact graph utilities
	void DECTOBINARY(int n);
	void ULONGTOBINARY(unsigned long n);
	void ULONGTOBINARY(keyvalue_t keyvalue);
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned long GETMASK_ULONG(unsigned long index, unsigned long size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	unsigned int READFROM_ULONG(unsigned long data, unsigned long index, unsigned long size);
	unsigned int READFROM_ULONG(keyvalue_t keyvalue, unsigned long index, unsigned long size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	void WRITETO_ULONG(unsigned long * data, unsigned long index, unsigned long size, unsigned long value);
	void WRITETO_ULONG(keyvalue_t * keyvalue, unsigned long index, unsigned long size, unsigned long value);
	unsigned int READBITSFROM_UINTV(unsigned int data, unsigned int index, unsigned int size);
	void WRITEBITSTO_UINTV(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	unsigned int UTIL_GETLOCALVID(unsigned int vid, unsigned int instid);
	unsigned int UTIL_GETREALVID(unsigned int lvid, unsigned int instid);

	void printallfeedback(string message, string graphpath, uint512_vec_dt * vdram, uint512_vec_dt * vdramtemp0, uint512_vec_dt * vdramtemp1, uint512_vec_dt * vdramtemp2, uint512_vec_dt * kvbuffer[NUM_PEs]);
	
	#ifdef FPGA_IMPL
	void set_callback(cl_event event, const char *queue_name);
	#endif

private:
	universalparams_t universalparams;
};
#endif







