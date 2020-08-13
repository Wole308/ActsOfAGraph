#ifndef HOST_ENIGMA_H
#define HOST_ENIGMA_H
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../algorithm/algorithm.h"
#include "../utility/utility.h"
#include "../kernels/acts.h"
#include "common.h"

class host_enigma {
public:
	host_enigma(graph * _graphobj);
	~host_enigma();
	
	void run();
	void start();	
	void WorkerThread(int threadidx);
	void finish();
	
	void loaddrams(int threadidx, unsigned int IterCount);
	void loadkvdram(uint512_vec_dt * kvdram, unsigned int baseoffset_kvs, unsigned int kvsize_kvs);
	void calculatealloffsets(int threadidx);
	void loadstats(uint512_vec_dt * kvdram, keyvalue_t * kvstats, vertex_t kvdramoffset, vertex_t kvdramsz, vertex_t kvstatsoffset, vertex_t kvrangeoffset, unsigned int LLOPnumpartitions, unsigned int LLOPrangepartitions);			
	void loadmessages(keyvalue_t * messages, vertex_t offset, unsigned int IterCount);
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	unsigned int getmessagesAddr(unsigned int addr);
	unsigned int getstatsAddr(unsigned int addr);
	int runActs(unsigned int IterCount);
	
	#ifdef SW
	void topkernel(unsigned int ddr, int threadidx, uint512_dt * kvsourcedram, uint512_dt * kvdestdram, keyvalue_t * kvstats);
	void launchswkernel(int threadidx);
	#endif
	#ifdef FPGA_IMPL
	void loadOCLstructures(std::string binaryFile);
	void launchkernel(unsigned int flag);
	void finishOCL();
	#endif 
	
private:
	#ifdef FPGA_IMPL
	uint512_vec_dt * kvsourcedram[NUMCPUTHREADSY][NUMCPUTHREADS];
	uint512_vec_dt * kvdestdram[NUMCPUTHREADSY][NUMCPUTHREADS];
	keyvalue_t * kvstats[NUMCPUTHREADSY][NUMCPUTHREADS];
	#else 
	uint512_vec_dt * kvsourcedram[NUMCPUTHREADSY][NUMCPUTHREADS];
	uint512_vec_dt * kvdestdram[NUMCPUTHREADSY][NUMCPUTHREADS];
	keyvalue_t * kvstats[NUMCPUTHREADSY][NUMCPUTHREADS];
	#endif 
	
	#ifdef SW
	acts * actsobjs;
	acts * kernelobjs[NUMCPUTHREADSY][NUMCPUTHREADS];
	#endif
	edge_process * edgeprocessobj[NUMCPUTHREADS];
	utility * utilityobj[NUMCPUTHREADS];
	graph * graphobj;
	std::thread runacts_thread[NUMCPUTHREADSY][NUMCPUTHREADS];
	
	#ifdef FPGA_IMPL
	cl_int err;	
	size_t global; 
	size_t local;
	
	size_t kvsource_size_bytes;
	size_t kvdest_size_bytes; 
	size_t kvstats_size_bytes;
	
	cl_event kernel_events[2];
	cl_event read_event[8][2];
	
	xcl_world world;
	cl_program program;
	cl_kernel kernel;
	
	cl_mem buffer_kvsourcedram[NUMCPUTHREADSY][NUMCPUTHREADS];
	cl_mem buffer_kvdestdram[NUMCPUTHREADSY][NUMCPUTHREADS];
	cl_mem buffer_kvstatsdram[NUMCPUTHREADSY][NUMCPUTHREADS];
	#endif
};
#endif







