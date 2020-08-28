#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../include/common.h"
#include "xcl2.hpp"
// #include "../xcl.h"
#include "goclkernel.h"
using namespace std;

goclkernel::goclkernel(){
	utilityobj = new utility();
}
goclkernel::~goclkernel(){} 

void goclkernel::loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	cl_int err;

	kvsource_size_bytes = KVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvdest_size_bytes = 8 * sizeof(uint512_vec_dt);
	kvstats_size_bytes = MESSAGESDRAMSZ * sizeof(keyvalue_t);
	
	// The get_xil_devices will return vector of Xilinx Devices
    auto devices = xcl::get_xil_devices();
    auto device = devices[0];

    // Creating Context and Command Queue (Out of order) for selected Device
    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
    OCL_CHECK(err,
              cl::CommandQueue q(context,
                                 device,
                                 CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE |
                                     CL_QUEUE_PROFILING_ENABLE,
                                 &err));

    std::string device_name = device.getInfo<CL_DEVICE_NAME>();
    std::cout << "Found Device=" << device_name.c_str() << std::endl;
	
	// read_binary_file() command will find the OpenCL binary file created using the
	// xocc compiler load into OpenCL Binary and return pointer to file buffer.
	auto fileBuf = xcl::read_binary_file(binaryFile);
	
	cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    devices.resize(1);
    OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));

    // Creating Kernel object using Compute unit names
    std::string krnl_name = "krnl_vaddmul";

    for (int i = 0; i < NUM_KERNEL; i++) {
        std::string cu_id = std::to_string(i + 1);
        std::string krnl_name_full =
            krnl_name + ":{" + "krnl_vaddmul_" + cu_id + "}";

        printf("Creating a kernel [%s] for CU(%d)\n",
               krnl_name_full.c_str(),
               i + 1);

        //Here Kernel object is created by specifying kernel name along with compute unit.
        //For such case, this kernel object can only access the specific Compute unit

        OCL_CHECK(err,
                  krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
    }
	
	std::vector<cl_mem_ext_ptr_t> inBufExt1(NUM_KERNEL);
    std::vector<cl_mem_ext_ptr_t> inBufExt2(NUM_KERNEL);
    std::vector<cl_mem_ext_ptr_t> outAddBufExt(NUM_KERNEL);
	
	// For Allocating Buffer to specific Global Memory Bank, user has to use cl_mem_ext_ptr_t
    // and provide the Banks
    for (int i = 0; i < NUM_KERNEL; i++) {
        inBufExt1[i].obj = kvsourcedram[flag][j][k];
        inBufExt1[i].param = 0;
        inBufExt1[i].flags = bank[i * 4];

        inBufExt2[i].obj = kvdestdram[flag][j][k];
        inBufExt2[i].param = 0;
        inBufExt2[i].flags = bank[(i * 4) + 1];

        outAddBufExt[i].obj = kvstats[flag][j][k];
        outAddBufExt[i].param = 0;
        outAddBufExt[i].flags = bank[(i * 4) + 2];
    }

    // These commands will allocate memory on the FPGA. The cl::Buffer objects can
    // be used to reference the memory locations on the device.
    //Creating Buffers
    for (int i = 0; i < NUM_KERNEL; i++) {
        OCL_CHECK(err,
                  buffer_kvsourcedram[i] =
                      cl::Buffer(context,
                                 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
                                     CL_MEM_USE_HOST_PTR,
                                 kvsource_size_bytes,
                                 &inBufExt1[i],
                                 &err));
        OCL_CHECK(err,
                  buffer_kvdestdram[i] =
                      cl::Buffer(context,
                                 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
                                     CL_MEM_USE_HOST_PTR,
                                 kvdest_size_bytes,
                                 &inBufExt2[i],
                                 &err));
        OCL_CHECK(err,
                  buffer_kvstatsdram[i] =
                      cl::Buffer(context,
                                 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
                                     CL_MEM_USE_HOST_PTR,
                                 kvstats_size_bytes,
                                 &outAddBufExt[i],
                                 &err));
    }
	return;
}

void goclkernel::launchkernel(){
	// Copy input data to Device Global Memory
    for (int i = 0; i < NUM_KERNEL; i++) {
        OCL_CHECK(err,
                  err = q.enqueueMigrateMemObjects(
                      {buffer_kvsourcedram[i], buffer_kvdestdram[i], buffer_kvstatsdram[i]},
                      0 /* 0 means from host*/));
    }
    q.finish();
	
	// Set the Kernel Arguments
	std::cout << "Set the Kernel Arguments" << std::endl;
	for (int i = 0; i < NUM_KERNEL; i++) {
        //Setting the k_vadd Arguments
        OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[i]));
        OCL_CHECK(err, err = krnls[i].setArg(1, buffer_kvdestdram[i]));
        OCL_CHECK(err, err = krnls[i].setArg(2, buffer_kvstatsdram[i]));

        //Invoking the kernel
        OCL_CHECK(err, err = q.enqueueTask(krnls[i]));
    }
    q.finish();
	
	// Copy Result from Device Global Memory to Host Local Memory
    for (int i = 0; i < NUM_KERNEL; i++) {
        OCL_CHECK(err,
                  err = q.enqueueMigrateMemObjects(
                      {buffer_kvdestdram[i]},
                      CL_MIGRATE_MEM_OBJECT_HOST));
    }
    q.finish();
}

void goclkernel::writeVstokernel(){
	// Copy input data to Device Global Memory
    for (int i = 0; i < NUM_KERNEL; i++) {
        OCL_CHECK(err,
                  err = q.enqueueMigrateMemObjects(
                      {buffer_kvsourcedram[i], buffer_kvdestdram[i], buffer_kvstatsdram[i]},
                      0 /* 0 means from host*/));
    }
    q.finish();
}
void goclkernel::readVsfromkernel(){
	// Copy Result from Device Global Memory to Host Local Memory
    for (int i = 0; i < NUM_KERNEL; i++) {
        OCL_CHECK(err,
                  err = q.enqueueMigrateMemObjects(
                      {buffer_kvdestdram[i]},
                      CL_MIGRATE_MEM_OBJECT_HOST));
    }
    q.finish();
}












