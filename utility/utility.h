#ifndef UTILITY_H
#define UTILITY_H
#include "../src/common.h"
#include "../src/host_common.h"

class utility {
public:
	utility();
	~utility();
	
	size_t hsub(size_t val1, size_t val2);
	size_t hceildiv(size_t val1, size_t val2);
	int hmin(size_t val1, size_t val2);
	int hmax(size_t val1, size_t val2);
	void stopTIME(string caption, std::chrono::steady_clock::time_point begintime, unsigned int iteration_idx);
	vertex_t GETKVDATA_RANGE_FORSSDPARTITION_(unsigned int ssdpartitonid);
	vertex_t GETKVDATA_RANGEOFFSET_FORSSDPARTITION_(unsigned int ssdpartitonid);
	unsigned int GETTREEDEPTH_(unsigned int ssdpartitonid);
	int hallignup_KV(int val);
	int halligndown_KV(int val);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	void printkvdrams(uint512_vec_dt * kvdram);
	void printstats(unsigned int size, keyvalue_t * kvstats);
	void printkvdramsII(uint512_vec_dt * kvdram, keyvalue_t * kvstats);
	void checkkvdrams(keyvalue_t * kvdram, keyvalue_t * kvstats);
	void checkkvstats(keyvalue_t * kvstats, unsigned int _totalnumkeyvalues);
	void recordstats(keyvalue_t * kvstats);	
	void printbits(int fd, size_t fileoffset, unsigned int size);
	void printbits(char * buffer, unsigned int size);
	void InsertBit(unsigned int * edgeoffsetbits, size_t index, unsigned int bit);
	unsigned int RetrieveBit(unsigned int * edgeoffsetbits, size_t index);
	unsigned int getstatsAddr(unsigned int addr);
	unsigned int getmessagesAddr(unsigned int addr);
	int runActs(unsigned int IterCount);

	#ifdef FPGA_IMPL
	// void event_cb(cl_event event, cl_int cmd_status, void *data);
	void set_callback(cl_event event, const char *queue_name);
	#endif
	
private:
};
#endif








