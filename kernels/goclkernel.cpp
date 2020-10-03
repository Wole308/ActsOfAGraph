#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../include/common.h"
#ifdef FPGA_IMPL
#include "xcl2.hpp"
#endif 
#include "goclkernel.h"
using namespace std;

//HBM Banks requirements
#define MAX_HBM_BANKCOUNT 32
#define BANK_NAME(n) n | XCL_MEM_TOPOLOGY

goclkernel::goclkernel(){
	utilityobj = new utility();
}
goclkernel::~goclkernel(){} 

#ifdef FPGA_IMPL 
const int bank[MAX_HBM_BANKCOUNT] = {
    BANK_NAME(0),  BANK_NAME(1),  BANK_NAME(2),  BANK_NAME(3),  BANK_NAME(4),
    BANK_NAME(5),  BANK_NAME(6),  BANK_NAME(7),  BANK_NAME(8),  BANK_NAME(9),
    BANK_NAME(10), BANK_NAME(11), BANK_NAME(12), BANK_NAME(13), BANK_NAME(14),
    BANK_NAME(15), BANK_NAME(16), BANK_NAME(17), BANK_NAME(18), BANK_NAME(19),
    BANK_NAME(20), BANK_NAME(21), BANK_NAME(22), BANK_NAME(23), BANK_NAME(24),
    BANK_NAME(25), BANK_NAME(26), BANK_NAME(27), BANK_NAME(28), BANK_NAME(29),
    BANK_NAME(30), BANK_NAME(31)};

void goclkernel::launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){
	// writeVstokernel(0, kvsourcedram, 0, 0);
	double kernel_time_in_sec = 0, result = 0;
    std::chrono::duration<double> kernel_time(0);
	
	cout<<"goclkernel::launchkernel:: Launching "<<NUMACTIVEKERNELS<<" active Kernels..."<<endl;
    auto kernel_start = std::chrono::high_resolution_clock::now();
	
	/* unsigned int bufferid = 0;
	for(unsigned int i=0; i<NUMACTIVEKERNELS; i++){
		//Setting the k_vadd Arguments
		OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[bufferid++]));
		
		//Invoking the kernel
		OCL_CHECK(err, err = q.enqueueTask(krnls[i]));
	}
    q.finish(); */
	
	unsigned int bufferid = 0;
	for(unsigned int i=0; i<1; i++){
		//Setting the k_vadd Arguments
		OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[bufferid++]));
		
		//Invoking the kernel
		OCL_CHECK(err, err = q.enqueueTask(krnls[i]));
	}
    q.finish();
	
    auto kernel_end = std::chrono::high_resolution_clock::now();
    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
	std::cout<< TIMINGRESULTSCOLOR <<">>> total time elapsed: "<<kernel_time.count() * 1000<<" ms"<< RESET <<std::endl;
	// readVsfromkernel(0, kvsourcedram, 0, 0);
	return;
}

void goclkernel::writetokernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset, unsigned int size){
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		cout<<"goclkernel::writetokernel::23 beginoffset "<<beginoffset<<" size: "<<size<<", PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;
		OCL_CHECK(err, err = q.enqueueWriteBuffer(buffer_kvsourcedram[i], CL_TRUE, beginoffset * sizeof(keyvalue_t), size * sizeof(keyvalue_t), (uint512_vec_dt *)kvsourcedram[0][i]));
	}
    q.finish();
}
void goclkernel::writetokernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		cout<<"goclkernel::writetokernel::24 beginoffset["<<i<<"] "<<beginoffset[i / NUMSUBCPUTHREADS][i % NUMSUBCPUTHREADS]<<" size["<<i<<"]: "<<size[i / NUMSUBCPUTHREADS][i % NUMSUBCPUTHREADS]<<", PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;
		#ifdef ACCESSFPGABY_ENQUEUEWRITEBUFFER
		OCL_CHECK(err, err = q.enqueueWriteBuffer(buffer_kvsourcedram[i], CL_TRUE, beginoffset[i / NUMSUBCPUTHREADS][i % NUMSUBCPUTHREADS] * sizeof(keyvalue_t), size[i / NUMSUBCPUTHREADS][i % NUMSUBCPUTHREADS] * sizeof(keyvalue_t), (uint512_vec_dt *)kvsourcedram[0][i]));
		#else 
		OCL_CHECK(err,
				  err = q.enqueueMigrateMemObjects(
					  {buffer_kvsourcedram[i]},
					  0));
		#endif
	}
    q.finish();
}

void goclkernel::readfromkernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset, unsigned int size){
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		OCL_CHECK(err, err = q.enqueueReadBuffer(buffer_kvsourcedram[i], CL_TRUE, beginoffset * sizeof(keyvalue_t), size * sizeof(keyvalue_t), (uint512_vec_dt *)kvsourcedram[0][i]));
	}
    q.finish();
}
void goclkernel::readfromkernel(unsigned int flag, uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		#ifdef ACCESSFPGABY_ENQUEUEWRITEBUFFER
		OCL_CHECK(err, err = q.enqueueReadBuffer(buffer_kvsourcedram[i], CL_TRUE, beginoffset[i / NUMSUBCPUTHREADS][i % NUMSUBCPUTHREADS] * sizeof(keyvalue_t), size[i / NUMSUBCPUTHREADS][i % NUMSUBCPUTHREADS] * sizeof(keyvalue_t), (uint512_vec_dt *)kvsourcedram[0][i]));
		#else 
		OCL_CHECK(err,
				  err = q.enqueueMigrateMemObjects(
					  {buffer_kvsourcedram[i]},
					  CL_MIGRATE_MEM_OBJECT_HOST));	
		#endif
	}
    q.finish();
}

void goclkernel::loadOCLstructures(std::string _binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){		
	binaryFile = _binaryFile;

	inputdata_size_bytes = INPUTDATASZ_KVS * sizeof(uint512_vec_dt);
	cout<<"goclkernel::loadOCLstructures:: inputdata_size_bytes: "<<inputdata_size_bytes<<endl;
	
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"goclkernel::loadOCLstructures: printing OCL parameters"<<endl;
	cout<<"TOTALNUMKERNELS: "<<TOTALNUMKERNELS<<endl;
	cout<<"NUMHBMSPERKERNEL: "<<NUMHBMSPERKERNEL<<endl;
	cout<<"TOTALNUMACTCUSTORUN: "<<TOTALNUMACTCUSTORUN<<endl;
	cout<<"NUMACTSCUSPERKERNEL: "<<NUMACTSCUSPERKERNEL<<endl;
	cout<<"TOTALAVAILABLEACTSCUS: "<<TOTALAVAILABLEACTSCUS<<endl;
	cout<<"NUMACTIVEKERNELS: "<<NUMACTIVEKERNELS<<endl;
	cout<<"NUMACTIVEINSTANCES: "<<NUMACTIVEINSTANCES<<endl;
	#endif 
	
	// OPENCL HOST CODE AREA START
    // The get_xil_devices will return vector of Xilinx Devices
    auto devices = xcl::get_xil_devices();
    auto device = devices[0];

    // Creating Context and Command Queue (Out of order) for selected Device
    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
    OCL_CHECK(err,
              cl::CommandQueue _q(context,
                                 device,
                                 CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE |
                                     CL_QUEUE_PROFILING_ENABLE,
                                 &err));
								 
	q = _q;

    std::string device_name = device.getInfo<CL_DEVICE_NAME>();
    std::cout << "Found Device=" << device_name.c_str() << std::endl;

    // read_binary_file() command will find the OpenCL binary file created using the
    // xocc compiler load into OpenCL Binary and return pointer to file buffer.
	auto fileBuf = xcl::read_binary_file(binaryFile);

	cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
	devices.resize(1);
	OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));
	
    // Creating Kernel object using Compute unit names
	std::string krnl_name = "topkernel";
	for(unsigned int i=0; i<TOTALNUMKERNELS; i++){
			std::string cu_id = std::to_string((i+1));
			 std::string krnl_name_full =
				krnl_name + ":{" + "topkernel_" + cu_id + "}";

			printf("Creating a kernel [%s] for CU(%d)\n",
				   krnl_name_full.c_str(),
				   (i+1));

			//Here Kernel object is created by specifying kernel name along with compute unit.
			//For such case, this kernel object can only access the specific Compute unit
			OCL_CHECK(err,
					  krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	}
	
	unsigned int flag=0;
	unsigned int counter = 0;
	uint512_dt ** kvsourcedramarr = (uint512_dt **)kvsourcedram;
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		cout<<"attaching bufferExt "<<i<<" to HBM bank: "<<i<<endl;
		inoutBufExt[i].obj = kvsourcedramarr[counter++];
		inoutBufExt[i].param = 0;
		inoutBufExt[i].flags = bank[i];
	}
	
    // These commands will allocate memory on the FPGA. The cl::Buffer objects can
    // be used to reference the memory locations on the device.
    //Creating Buffers
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		cout<<"creating buffer for ACTS: "<<i<<endl;
		OCL_CHECK(err,
				  buffer_kvsourcedram[i] =
					  cl::Buffer(context,
								 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
								 inputdata_size_bytes,
								 &inoutBufExt[i],
								 &err));
	}
	return;
}
void goclkernel::finishOCL(){
	cout<<"test::finish destroying memory structures... "<<endl;
}
#endif 











