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
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printkeyvalues("goclkernel::launchkernel:: kvdram workspace (before kernel launch)::kvdram", (keyvalue_t *)(&kvsourcedram[0][0][BASEOFFSET_KVDRAM_KVS]), 16);
	utilityobj->printkeyvalues("goclkernel::launchkernel:: kvdram workspace (before kernel launch)::kvdram workspace", (keyvalue_t *)(&kvsourcedram[0][0][BASEOFFSET_KVDRAMWORKSPACE_KVS]), 16);
	utilityobj->printkeyvalues("goclkernel::launchkernel:: global capsule (before kernel launch)::kvstatsdram", (keyvalue_t *)(&kvsourcedram[0][0][BASEOFFSET_STATSDRAM_KVS]), 16);
	#endif

    double kernel_time_in_sec = 0, result = 0;
    std::chrono::duration<double> kernel_time(0);

	cout<<"goclkernel::launchkernel:: Launching "<<NUMKERNELS<<" Kernels..."<<endl;
    auto kernel_start = std::chrono::high_resolution_clock::now();
	
	unsigned int bufferid = 0;
	for(unsigned int i=0; i<NUMKERNELS; i++){
		//Setting the k_vadd Arguments
		for(unsigned int j=0; j<NUMINSTANCES; j++){ 
			OCL_CHECK(err, err = krnls[i].setArg(j, buffer_kvsourcedram[bufferid++]));
		}
		
		//Invoking the kernel
		OCL_CHECK(err, err = q.enqueueTask(krnls[i]));
	}
    q.finish();
	
    auto kernel_end = std::chrono::high_resolution_clock::now();
    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
	std::cout<<">>> total time elapsed: "<<kernel_time.count() * 1000<<" ms"<<std::endl;

    kernel_time_in_sec = kernel_time.count();
    kernel_time_in_sec /= NUMKERNELS;
	
	// Checks
	#ifdef _DEBUGMODE_HOSTPRINTS
	uint512_vec_dt * UVEC = (uint512_vec_dt *)kvsourcedram[0][0];
	cout<<"launchkernel:: printing messages (after kernel launch) "<<endl;
	cout<<"MESSAGES_RUNKERNELCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key<<endl;
	cout<<"MESSAGES_PROCESSCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key<<endl;
	cout<<"MESSAGES_COLLECTSTATSCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key<<endl;
	cout<<"MESSAGES_PARTITIONCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key<<endl;
	cout<<"MESSAGES_APPLYUPDATESCOMMANDID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key<<endl;
	cout<<"MESSAGES_VOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VOFFSET].data[0].key<<endl;
	cout<<"MESSAGES_VSIZE: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_VSIZE].data[0].key<<endl;
	cout<<"MESSAGES_TREEDEPTH: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key<<endl;
	cout<<"MESSAGES_FINALNUMPARTITIONS: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key<<endl;
	cout<<"MESSAGES_GRAPHITERATIONID: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key<<endl;
	cout<<"MESSAGES_BATCHSIZE: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key<<endl;
	cout<<"MESSAGES_RUNSIZE: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key<<endl;
	cout<<"MESSAGES_NEXTBATCHOFFSET: "<<UVEC[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key<<endl;
	
	utilityobj->printkeyvalues("goclkernel::launchkernel:: kvdram workspace (after kernel launch)::kvdram", (keyvalue_t *)(&kvsourcedram[0][0][BASEOFFSET_KVDRAM_KVS]), 16);
	utilityobj->printkeyvalues("goclkernel::launchkernel:: kvdram workspace (after kernel launch)::kvdram workspace", (keyvalue_t *)(&kvsourcedram[0][0][BASEOFFSET_KVDRAMWORKSPACE_KVS]), 16);
	utilityobj->printkeyvalues("goclkernel::launchkernel:: global capsule (after kernel launch)::kvstatsdram", (keyvalue_t *)(&kvsourcedram[0][0][BASEOFFSET_STATSDRAM_KVS]), 16);
	#endif 
	return;
}

void goclkernel::writeVstokernel(unsigned int flag){
	for(unsigned int i=0; i<NUMACTCUSPERAPP; i++){ // NUMACTCUSPERAPP
			OCL_CHECK(err,
					  err = q.enqueueMigrateMemObjects(
						  {buffer_kvsourcedram[i]},
						  0 /* 0 means from host*/));
	}
    q.finish();
}
void goclkernel::readVsfromkernel(unsigned int flag){
	for(unsigned int i=0; i<1; i++){ // NUMACTCUSPERAPP
			OCL_CHECK(err,
					  err = q.enqueueMigrateMemObjects(
						  {buffer_kvsourcedram[i]},
						  CL_MIGRATE_MEM_OBJECT_HOST));
	}
    q.finish();
}

void goclkernel::loadOCLstructures(std::string _binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){		
	binaryFile = _binaryFile;

	// kvsource_size_bytes = KVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvsource_size_bytes = PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt); // REMOVEME.
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
	std::string krnl_name = "topkernel";
	for(unsigned int i=0; i<NUMKERNELS; i++){
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
	
	cout<<"---------- goclkernel:: NUMACTCUSPERAPP: "<<NUMACTCUSPERAPP<<", NUMKERNELS: "<<NUMKERNELS<<", NUMACTSCUSPERKERNEL: "<<NUMACTSCUSPERKERNEL<<endl;
	unsigned int flag=0;
	unsigned int counter = 0;
	uint512_dt ** kvsourcedramarr = (uint512_dt **)kvsourcedram;
	for(unsigned int i=0; i<NUMKERNELS; i++){ 
		for(unsigned int j=0; j<NUMACTSCUSPERKERNEL; j++){ 
			inoutBufExt[i*NUMACTSCUSPERKERNEL + j].obj = kvsourcedramarr[counter++];
			inoutBufExt[i*NUMACTSCUSPERKERNEL + j].param = 0;
			inoutBufExt[i*NUMACTSCUSPERKERNEL + j].flags = bank[i*NUMHBMSPERKERNEL + j];
		}
	}
	
    // These commands will allocate memory on the FPGA. The cl::Buffer objects can
    // be used to reference the memory locations on the device.
    //Creating Buffers
	for(unsigned int i=0; i<NUMACTCUSPERAPP; i++){
		cout<<"SEEN HERE 1."<<endl;
		OCL_CHECK(err,
				  buffer_kvsourcedram[i] =
					  cl::Buffer(context,
								 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
								 kvsource_size_bytes,
								 &inoutBufExt[i],
								 &err));
	}
	cout<<"SEEN HERE 2."<<endl;
	return;
}
void goclkernel::finishOCL(){
	cout<<"test::finish destroying memory structures... "<<endl;
}
#endif 











