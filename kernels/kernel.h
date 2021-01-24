#ifndef KERNEL_H
#define KERNEL_H
#include "swkernel.h"
// #include "oclkernel.h"
#include "goclkernel.h"
#include "../src/stats/stats.h"
#include "../src/utility/utility.h"
#include "../acts/include/actscommon.h" //
#include "../include/common.h"

class kernel {
public:
	kernel(stats * _statsobj);
	~kernel();
	
	void launchkernel(uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int flag);
	
	#ifdef FPGA_IMPL 
	void loadOCLstructures(std::string binaryFile, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	
	void writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int bufferbeginoffset_kvs, unsigned int beginoffset, unsigned int size);
	void writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	
	void readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], unsigned int bufferbeginoffset_kvs, unsigned int beginoffset, unsigned int size);
	void readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]);
	
	void finishOCL();
	#endif
private:
	utility * utilityobj;
	
	#ifdef FPGA_IMPL 
	// oclkernel * kernelobj;
	goclkernel * kernelobj;
	#else 
	swkernel * kernelobj;	
	#endif
	stats * statsobj;
	
	unsigned int beginoffset[NUMSUBCPUTHREADS];
	unsigned int size[NUMSUBCPUTHREADS];
};
#endif







