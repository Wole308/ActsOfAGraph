#ifndef UTILITY_H
#define UTILITY_H
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
	void printedges(string message, edge2_type * edges, unsigned int size);
	void printmessages(string message, uint512_vec_dt * keyvalues);
	void printallparameters();
	void printvalues(string message, unsigned int * values, unsigned int size);
	void printvalues(string message, vector<value_t> & values, unsigned int size);
	void printvaluesgreaterthan(string message, unsigned int * values, unsigned int size, unsigned int threshold);
	void printvalueslessthan(string message, unsigned int * values, unsigned int size, unsigned int threshold);
	void printstructuresbeforekernelrun(string message, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size);
	void printstructuresafterkernelrun(string message, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size);
	void printcontainer(container_t * container);
	
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
	void checkforlessthanthan(string message, unsigned int data1, unsigned int data2);
	
	void stopTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx);
	void stopBTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx);
	
	size_t hsub(size_t val1, size_t val2);
	size_t hceildiv(size_t val1, size_t val2);
	int hmin(size_t val1, size_t val2);
	int hmax(size_t val1, size_t val2);
	void InsertBit(unsigned int * edgeoffsetbits, size_t index, unsigned int bit);
	unsigned int RetrieveBit(unsigned int * edgeoffsetbits, size_t index);
	int runActs(unsigned int IterCount);
	int runActs(unsigned int IterCount, bool forcetrue);
	unsigned int GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(unsigned int ssdpartitonid);
	void resetkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void resetkeyvalues(keyvalue_t * keyvalues, unsigned int size);
	void allignandappendinvalids(keyvalue_t * buffer, unsigned int size);
	unsigned int allignhigher_KV(unsigned int val);
	unsigned int allignlower_KV(unsigned int val);
	void setarray(unsigned int array[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int _1dimsize, unsigned int _2dimsize, unsigned int value);
	void copy(unsigned int * array1, unsigned int * array2, unsigned int size);
	void countkeyvalueswithvalueequalto(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int value);
	unsigned int countvaluesgreaterthan(string message, unsigned int * values, unsigned int size, unsigned int threshold);
	void scankeyvalues(string message, keyvalue_t * keyvalues, keyvalue_t * stats, unsigned int numberofpartitions, unsigned int rangeperpartition, unsigned int upperlimit);
	unsigned int geterrorkeyvalues(keyvalue_t * keyvalues, unsigned int begin, unsigned int end, unsigned int lowerrangeindex, unsigned int upperrangeindex);
	void createdirectory(const char* directory);
	void countvalueslessthan(string message, value_t * values, unsigned int size, unsigned int data);
	void paddkeyvalues(keyvalue_t * keyvalues, unsigned int size, unsigned int padddata);
	void clearkeyvalues(uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth);
	
	#ifdef FPGA_IMPL
	void set_callback(cl_event event, const char *queue_name);
	#endif 
	
	void printedgestats(vector<vertex_t> &srcvids, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, unsigned int * edges_count, unsigned int * edgesdstv_sum);
private:
};
#endif







