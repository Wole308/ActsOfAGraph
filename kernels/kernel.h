#ifndef KERNEL_H
#define KERNEL_H
#include "swkernel.h"
// #include "oclkernel.h"
#include "goclkernel.h"
#include "../src/utility/utility.h"
#include "../acts/include/actscommon.h" //
#include "../include/common.h"

class kernel {
public:
	kernel();
	~kernel();
	
	void launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	#ifdef FPGA_IMPL 
	void loadOCLstructures(std::string binaryFile, uint512_vec_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	
	void writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int bufferbeginoffset_kvs, unsigned int beginoffset, unsigned int size);
	void writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	
	void readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int bufferbeginoffset_kvs, unsigned int beginoffset, unsigned int size);
	void readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	
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
	
	unsigned int beginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int size[NUMCPUTHREADS][NUMSUBCPUTHREADS];
};
#endif







