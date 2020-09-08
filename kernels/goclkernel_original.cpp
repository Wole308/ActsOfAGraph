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

#define NUM_KERNEL (NUMCPUTHREADS * NUMSUBCPUTHREADS)
#define NUMBANKSFOREACHCU 4

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
    // Copy input data to Device Global Memory
	// writeVstokernel(flag); // FIXME. THEISSUE. why swemu is not running correctly

    double kernel_time_in_sec = 0, result = 0;
    std::chrono::duration<double> kernel_time(0);

	cout<<"goclkernel::launchkernel:: Launching "<<NUM_KERNEL<<" Kernels..."<<endl;
    auto kernel_start = std::chrono::high_resolution_clock::now();
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			//Setting the k_vadd Arguments
			OCL_CHECK(err, err = krnls[j][k].setArg(0, buffer_kvsourceAdram[flag][j][k]));
			OCL_CHECK(err, err = krnls[j][k].setArg(1, buffer_kvsourceBdram[flag][j][k]));
			OCL_CHECK(err, err = krnls[j][k].setArg(2, buffer_kvsourceCdram[flag][j][k]));
			OCL_CHECK(err, err = krnls[j][k].setArg(3, buffer_kvsourceDdram[flag][j][k]));
			
			//Invoking the kernel
			OCL_CHECK(err, err = q.enqueueTask(krnls[j][k]));
		}
	}
    q.finish();
    auto kernel_end = std::chrono::high_resolution_clock::now();

    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
	std::cout<<">>> total time elapsed: "<<kernel_time.count() * 1000<<" ms"<<std::endl;

    kernel_time_in_sec = kernel_time.count();
    kernel_time_in_sec /= NUM_KERNEL;
	
	// Copy Result from Device Global Memory to Host Local Memory
	// readVsfromkernel(flag);
    return;
}

void goclkernel::writeVstokernel(unsigned int flag){
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			OCL_CHECK(err,
					  err = q.enqueueMigrateMemObjects(
						  {buffer_kvsourceAdram[flag][j][k]},
						  0 /* 0 means from host*/));
			q.finish();
		}
	}
    q.finish();
}
void goclkernel::readVsfromkernel(unsigned int flag){
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			OCL_CHECK(err,
					  err = q.enqueueMigrateMemObjects(
						  {buffer_kvsourceAdram[flag][j][k]},
						  CL_MIGRATE_MEM_OBJECT_HOST));
		}
	}
    q.finish();
}

void goclkernel::loadOCLstructures(std::string _binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){		
	binaryFile = _binaryFile;

	// kvsource_size_bytes = KVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvsource_size_bytes = PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt); // REMOVEME.
	// kvsource_size_bytes = 8 * sizeof(uint512_vec_dt);
	kvdest_size_bytes = 8 * sizeof(uint512_vec_dt);
	kvstats_size_bytes = MESSAGESDRAMSZ * sizeof(keyvalue_t);
	cout<<"goclkernel::loadOCLstructures:: kvsource_size_bytes: "<<kvsource_size_bytes<<endl;
	cout<<"goclkernel::loadOCLstructures:: kvdest_size_bytes: "<<kvdest_size_bytes<<endl;
	cout<<"goclkernel::loadOCLstructures:: kvstats_size_bytes: "<<kvstats_size_bytes<<endl;
	
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
    /* std::string krnl_name = "topkernel";
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			std::string cu_id = std::to_string(j*NUMSUBCPUTHREADS + k + 1);
			 std::string krnl_name_full =
				krnl_name + ":{" + "topkernel_" + cu_id + "}";

			printf("Creating a kernel [%s] for CU(%d)\n",
				   krnl_name_full.c_str(),
				   j*NUMSUBCPUTHREADS + k + 1);

			//Here Kernel object is created by specifying kernel name along with compute unit.
			//For such case, this kernel object can only access the specific Compute unit
			OCL_CHECK(err,
					  krnls[j][k] = cl::Kernel(program, krnl_name_full.c_str(), &err));
		}
	} */
	std::string krnl_name = "topkernel";
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			unsigned int cu_intid = ((j*NUMSUBCPUTHREADS + k) / NUMINSTANCES) + 1;
			std::string cu_id = std::to_string(cu_intid);
			// std::string cu_id = std::to_string(j*NUMSUBCPUTHREADS + k + 1);
			 std::string krnl_name_full =
				krnl_name + ":{" + "topkernel_" + cu_id + "}";

			printf("Creating a kernel [%s] for CU(%d)\n",
				   krnl_name_full.c_str(),
				   j*NUMSUBCPUTHREADS + k + 1);

			//Here Kernel object is created by specifying kernel name along with compute unit.
			//For such case, this kernel object can only access the specific Compute unit
			OCL_CHECK(err,
					  krnls[j][k] = cl::Kernel(program, krnl_name_full.c_str(), &err));
		}
	}
	// for(unsigned int i=0; i<NUMCPUTHREADS * NUMSUBCPUTHREADS; i++){
		// std::string krnl_name_full =
				// krnl_name + ":{" + "topkernel_" + cu_id + "}";
	// }
	// std::string krnl_name_full =
				// krnl_name + ":{" + "topkernel_1}";
	// OCL_CHECK(err, krnls[j][k] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	
	unsigned int bankindex = 0;
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){
	bankindex = 0;
		for(unsigned int j=0; j<NUMCPUTHREADS; j++){
			for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
				inoutBufExt1[flag][j][k].obj = kvsourcedram[flag][j][k];
				inoutBufExt1[flag][j][k].param = 0;
				inoutBufExt1[flag][j][k].flags = bank[bankindex];
				
				inoutBufExt2[flag][j][k].obj = kvsourcedram[flag][j][k];
				inoutBufExt2[flag][j][k].param = 0;
				inoutBufExt2[flag][j][k].flags = bank[bankindex + 1];

				inoutBufExt3[flag][j][k].obj = kvsourcedram[flag][j][k];
				inoutBufExt3[flag][j][k].param = 0;
				inoutBufExt3[flag][j][k].flags = bank[bankindex + 2];
				
				inoutBufExt4[flag][j][k].obj = kvsourcedram[flag][j][k];
				inoutBufExt4[flag][j][k].param = 0;
				inoutBufExt4[flag][j][k].flags = bank[bankindex + 3];
				
				bankindex+=4;
			}
		}
	}
	
    // These commands will allocate memory on the FPGA. The cl::Buffer objects can
    // be used to reference the memory locations on the device.
    //Creating Buffers
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){
		for(unsigned int j=0; j<NUMCPUTHREADS; j++){
			for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
				OCL_CHECK(err,
						  buffer_kvsourceAdram[flag][j][k] =
							  cl::Buffer(context,
										 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
											 CL_MEM_USE_HOST_PTR,
										 kvsource_size_bytes,
										 &inoutBufExt1[flag][j][k],
										 &err));
				OCL_CHECK(err,
						  buffer_kvsourceBdram[flag][j][k] =
							  cl::Buffer(context,
										 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
											 CL_MEM_USE_HOST_PTR,
										 kvsource_size_bytes,
										 &inoutBufExt2[flag][j][k],
										 &err));
										 
				OCL_CHECK(err,
						  buffer_kvsourceCdram[flag][j][k] =
							  cl::Buffer(context,
										 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
											 CL_MEM_USE_HOST_PTR,
										 kvsource_size_bytes,
										 &inoutBufExt3[flag][j][k],
										 &err));
										 
				OCL_CHECK(err,
						  buffer_kvsourceDdram[flag][j][k] =
							  cl::Buffer(context,
										 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
											 CL_MEM_USE_HOST_PTR,
										 kvsource_size_bytes,
										 &inoutBufExt4[flag][j][k],
										 &err));
			}
		}
	}
	return;
}
void goclkernel::finishOCL(){
	cout<<"test::finish destroying memory structures... "<<endl;
}
#endif 











