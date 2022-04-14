#ifndef UTILITY_H
#define UTILITY_H

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
#include "../../examples/include/examplescommon.h"
#include "../../acts/include/actscommon.h" //
#include "../../include/common.h"
#ifdef FPGA_IMPL
#include <ap_int.h>
// #include "CL/cl.h"
#include <CL/opencl.h>
#include "../../xcl.h"
// #include "xcl2.hpp"
#endif
using namespace std;

#define UNVISITED 0
#define VISITED_IN_CURRENT_ITERATION 1
#define VISITED_IN_PAST_ITERATION 3

class utility {
public:
	utility();
	~utility();
	
	void print1(string messagea, unsigned int dataa);
	void print2(string messagea, string messageb, unsigned int dataa, unsigned int datab);
	void print4(string messagea, string messageb, string messagec, string messaged, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad);
	void print5(string messagea, string messageb, string messagec, string messaged, string messagee, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae);
	void print6(string messagea, string messageb, string messagec, string messaged, string messagee, string messagef, unsigned int dataa, unsigned int datab, unsigned int datac, unsigned int datad, unsigned int datae, unsigned int datef);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int skipsize);
	void printkeyvalues(string message, keyvalue_t * keyvalues[VECTOR_SIZE], unsigned int size);
	void printkeyvalues(string message, keyvalue_t keyvalues[VECTOR_SIZE][BLOCKRAM_SIZE], unsigned int numcols, unsigned int size);
	void printkeyvalues(string message, keyvalue_vec_bittype keyvalues[NUM_PARTITIONS][BLOCKRAM_SIZE], unsigned int numcols, unsigned int size);
	void printtriples(string message, triple_t * triples, unsigned int size);
	void printedges(string message, edge2_type * edges, unsigned int size);
	void printmessages(string message, uint512_vec_dt * keyvalues);
	void printallparameters();
	void printvalues(string message, unsigned int * values, unsigned int size);
	void printvalues(string message, vector<value_t> & values, unsigned int size);
	void printvaluesgreaterthan(string message, unsigned int * values, unsigned int size, unsigned int threshold);
	void printvalueslessthan(string message, unsigned int * values, unsigned int size, unsigned int threshold);
	void printstructuresbeforekernelrun(string message, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int size);
	void printstructuresafterkernelrun(string message, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int size);
	void printcontainer(container_t * container);
	
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
	void checkforlessthanthan(string message, unsigned int data1, unsigned int data2);
	
	void stopTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx);
	void stopBTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx);
	
	bool isbufferused(unsigned int id);
	
	size_t hsub(size_t val1, size_t val2);
	size_t hceildiv(size_t val1, size_t val2);
	int hmin(size_t val1, size_t val2);
	int hmax(size_t val1, size_t val2);
	void InsertBit(unsigned int * edgeoffsetbits, size_t index, unsigned int bit);
	unsigned int RetrieveBit(unsigned int * edgeoffsetbits, size_t index);
	unsigned int GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(unsigned int ssdpartitonid);
	void resetkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void resetkeyvalues(keyvalue_t * keyvalues, unsigned int size);
	void allignandappendinvalids(keyvalue_t * buffer, unsigned int size);
	unsigned int allignhigher_KV(unsigned int val);
	unsigned int allignlower_KV(unsigned int val);
	unsigned int allignhigherto16_KV(unsigned int val);
	unsigned int allignlowerto16_KV(unsigned int val);
	unsigned int allignhigher_FACTOR(unsigned int val, unsigned int _FACTOR);
	void setarray(unsigned int array[NUMSUBCPUTHREADS], unsigned int size, unsigned int value);
	void copy(unsigned int * array1, unsigned int * array2, unsigned int size);
	void countkeyvalueswithvalueequalto(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int value);
	unsigned int countvalues(string message, keyvalue_t * keyvalue, unsigned int size);
	unsigned int countvaluesgreaterthan(string message, unsigned int * values, unsigned int size, unsigned int threshold);
	void scankeyvalues(string message, keyvalue_t * keyvalues, keyvalue_t * stats, unsigned int numberofpartitions, unsigned int rangeperpartition, unsigned int upperlimit);
	unsigned int geterrorkeyvalues(keyvalue_t * keyvalues, unsigned int begin, unsigned int end, unsigned int lowerrangeindex, unsigned int upperrangeindex);
	void createdirectory(const char* directory);
	void countvalueslessthan(string message, value_t * values, unsigned int size, unsigned int data);
	void paddkeyvalues(keyvalue_t * keyvalues, unsigned int size, unsigned int padddata);
	void clearkeyvalues(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS]);
	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth);
	
	void calculateoffsets(keyvalue_t * buffer, unsigned int size, unsigned int base, unsigned int * skipspacing);
	void getmarkerpositions(keyvalue_t * stats, unsigned int size);
	
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

	unsigned int runsssp_sw(vector<vertex_t> &srcvids, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, unsigned int NumGraphIters, long double edgesprocessed_totals[128], unsigned int * numValidIters, 
			unsigned int vpmaskbuffer[MAXNUMGRAPHITERATIONS][NUMPROCESSEDGESPARTITIONS], uint512_ivec_dt * mdram, uint512_ivec_dt * vdram, uint512_ivec_dt * kvbuffer[NUMSUBCPUTHREADS], bool savemasks, bool printactivepartitios);							
	
	void printallfeedback(string message, uint512_vec_dt * vdram, uint512_vec_dt * vdramtemp0, uint512_vec_dt * vdramtemp1, uint512_vec_dt * vdramtemp2, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS]);
	
	#ifdef FPGA_IMPL
	void set_callback(cl_event event, const char *queue_name);
	#endif

private:
};
#endif







