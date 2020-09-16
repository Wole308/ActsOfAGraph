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
	
	void launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag);
	
	#ifdef FPGA_IMPL 
	void loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	void writeVstokernel(unsigned int flag);
	void readVsfromkernel(unsigned int flag);
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
};
#endif







