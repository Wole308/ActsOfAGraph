#ifndef HOST_KERNELPROCESS_H
#define HOST_KERNELPROCESS_H
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../algorithm/algorithm.h"
#include "../utility/utility.h"
#include "../kernels/kernelprocess.h"
#include "common.h"

class host_kernelprocess {
public:
	host_kernelprocess(graph * _graphobj);
	~host_kernelprocess();
	
	void run();
	void start();	
	void WorkerThread(int threadidx);
	void finish();
	
	void loaddrams(int threadidx);
	void loadkvdram(uint512_vec_dt * kvdram, unsigned int baseoffset_kvs, unsigned int kvsize_kvs);
	void loadcapsulestats(uint512_vec_dt * stats, unsigned int baseoffset_kvs, unsigned int kvsize_kvs);
	void loadstats(uint512_vec_dt * kvdram, keyvalue_t * kvstats, vertex_t kvdramoffset, vertex_t kvdramsz, vertex_t kvrangeoffset, unsigned int LLOPnumpartitions, unsigned int LLOPrangepartitions);
	void loadmessages(int * messages);
	void checkoutofbounds(string message, unsigned int data, unsigned int upper_bound);
	void printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size);
	
	#ifdef SW
	void launchswkernel(int threadidx);
	#endif
	#ifdef FPGA_IMPL
	void loadOCLstructures(std::string binaryFile);
	void launchkernel(unsigned int flag);
	void finishOCL();
	#endif 
	
private:
	uint512_vec_dt * kvdram_source[NUMCPUTHREADS][NUMDRAMBANKS];
	keyvalue_t * kvstats[NUMCPUTHREADS][NUMDRAMBANKS];
	int * messages[NUMCPUTHREADS][NUMDRAMBANKS];
	
	#ifdef SW
	// legion * kernelobj;
	kernelprocess * kernelobj;
	#endif
	edge_process * edgeprocessobj[NUMCPUTHREADS];
	utility * utilityobj[NUMCPUTHREADS];
	graph * graphobj;
	
	#ifdef FPGA_IMPL
	cl_int err;	
	size_t global; 
	size_t local;
	
	size_t kvsource_size_bytes; 
	size_t kvstats_size_bytes;
	size_t messages_size_bytes;
	
	cl_event kernel_events[2];
	cl_event read_event[8][2];
	
	xcl_world world;
	cl_program program;
	cl_kernel kernel;
	
	cl_mem buffer_sourcekvdram[2][NUMDRAMBANKS];
	cl_mem buffer_kvstats[2][NUMDRAMBANKS];
	cl_mem buffer_messages[2][NUMDRAMBANKS];
	#endif
};
#endif







