#ifndef UTILITY_H
#define UTILITY_H
#include <string.h>
#include <mutex>
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
	void printmessages(string message, uint512_vec_dt * keyvalues);
	void printallparameters();
	void printvaluesgreaterthan(string message, unsigned int * values, unsigned int size, unsigned int threshold);
	void printvalueslessthan(string message, unsigned int * values, unsigned int size, unsigned int threshold);
	
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound, unsigned int msgdata1, unsigned int msgdata2, unsigned int msgdata3);
	
	void stopTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx);
	void stopBTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx);
	
	size_t hsub(size_t val1, size_t val2);
	size_t hceildiv(size_t val1, size_t val2);
	int hmin(size_t val1, size_t val2);
	int hmax(size_t val1, size_t val2);
	void InsertBit(unsigned int * edgeoffsetbits, size_t index, unsigned int bit);
	unsigned int RetrieveBit(unsigned int * edgeoffsetbits, size_t index);
	int runActs(unsigned int IterCount);
	unsigned int GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(unsigned int ssdpartitonid);
	void resetkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void allignandappendinvalids(keyvalue_t * buffer, unsigned int size);
	unsigned int allignhigher_KV(unsigned int val);
	void setarray(unsigned int array[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int _1dimsize, unsigned int _2dimsize, unsigned int value);
	void copy(unsigned int * array1, unsigned int * array2, unsigned int size);
	void countkeyvalueswithvalueequalto(string message, keyvalue_t * keyvalues, unsigned int size, unsigned int value);
	
	#ifdef FPGA_IMPL
	void set_callback(cl_event event, const char *queue_name);
	#endif 
private:
};
#endif







