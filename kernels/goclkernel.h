#ifndef GOCLKERNEL_H
#define GOCLKERNEL_H
#include "../../src/utility/utility.h"
#include "../../include/common.h"

#define NUM_KERNEL 8

class oclkernel {
public:
	oclkernel();
	~oclkernel();
	
	void loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]);			
	
	void launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);

	void writeVstokernel();
	void readVsfromkernel();

private:
	utility * utilityobj;
	
	size_t kvsource_size_bytes;
	size_t kvdest_size_bytes; 
	size_t kvstats_size_bytes;
	
	std::vector<cl::Buffer> buffer_kvsourcedram(NUM_KERNEL);
    std::vector<cl::Buffer> buffer_kvdestdram(NUM_KERNEL);
    std::vector<cl::Buffer> buffer_kvstatsdram(NUM_KERNEL);
	
	std::vector<cl::Kernel> krnls(NUM_KERNEL);
};
#endif







