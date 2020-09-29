#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <mutex>
#include <thread>
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../kernels/kernel.h"
#include "../../include/common.h"
using namespace std;

class helperfunctions {
public:
	helperfunctions(graph * graphobj);
	helperfunctions();
	~helperfunctions();
	
	#ifdef ACTSMODEL
	void replicateverticesdata(keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	void workerthread_replicateverticesdata(int threadidx, keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	#endif 
	#ifdef ACTSMODEL_LW
	void replicateverticesdata(value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	void workerthread_replicateverticesdata(int threadidx, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	#endif 
	
	#ifdef ACTSMODEL
	void cummulateverticesdata(keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	void workerthread_cummulateverticesdata(int threadidx, keyvalue_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	#endif 
	#ifdef ACTSMODEL_LW
	void cummulateverticesdata(value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	void workerthread_cummulateverticesdata(int threadidx, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int offset, unsigned int size);
	#endif 
	
	#ifdef ACTSMODEL
	void applyvertices(unsigned int bank, unsigned int fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, unsigned int graph_iterationidx);
	void workerthread_applyvertices(int ithreadidx, unsigned int bank, unsigned int fdoffset, keyvalue_t * buffer, vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, unsigned int graph_iterationidx);						
	#endif 
	#ifdef ACTSMODEL_LW
	void applyvertices(unsigned int bank, unsigned int fdoffset, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, hostglobalparams_t globalparams);
	void workerthread_applyvertices(int ithreadidx, unsigned int bank, unsigned int fdoffset, value_t * buffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vertex_t bufferoffset, vertex_t datasize,  unsigned int voffset, hostglobalparams_t globalparams);						
	#endif 
	
	void launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	#ifdef ACTSMODEL
	void updatemessagesbeforelaunch(unsigned int globaliteration_idx, unsigned int voffset, unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int keyvaluecount[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int messagesbaseoffset, unsigned int kvstatsbaseoffset, hostglobalparams_t globalparams);												
	void updatemessagesafterlaunch(unsigned int globaliteration_idx, keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int messagesbaseoffset, unsigned int kvstatsbaseoffset);
	#endif 
	#ifdef ACTSMODEL_LW
	void updatemessagesbeforelaunch(unsigned int globaliteration_idx, unsigned int voffset, unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int keyvaluecount[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_vec_dt * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int messagesbaseoffset_kvs, unsigned int kvstatsbaseoffset_kvs, hostglobalparams_t globalparams);												
	void updatemessagesafterlaunch(unsigned int globaliteration_idx, uint512_vec_dt * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int messagesbaseoffset_kvs, unsigned int kvstatsbaseoffset_kvs);
	#endif 
	
	void loadvertexpropertiesfromfile();
	void workerthread_loadvertexpropertiesfromfile(int ithreadidx, int fd, unsigned int offset, vertexprop_t * buffer, vertex_t bufferoffset, vertex_t size);
	
	unsigned int getflag(unsigned int globaliteration_idx);
	
	#ifdef FPGA_IMPL 
	void loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void writeVstokernel(unsigned int flag);
	void readVsfromkernel(unsigned int flag);
	void finishOCL();
	#endif 
	
private:
	kernel * kernelobj;
	parameters * parametersobj;
	utility * utilityobj;
	graph * graphobj;
	algorithm * algorithmobj;
	std::thread mythread[NUMCPUTHREADS];
	std::thread mykernelthread[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif







