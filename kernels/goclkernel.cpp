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
#include "../src/stats/stats.h"
#include "../src/utility/utility.h"
#include "../include/common.h"
#ifdef FPGA_IMPL
#include "xcl2.hpp"
#endif 
#include "goclkernel.h"
using namespace std;

//HBM Banks requirements
#define MAX_HBM_BANKCOUNT 32
#define BANK_NAME(n) n | XCL_MEM_TOPOLOGY

#define ENABLE_ACTSPROC
#define ENABLE_ACTSSYNC

#define LENGTH PADDEDKVSOURCEDRAMSZ // 1024

goclkernel::goclkernel(stats * _statsobj){
	utilityobj = new utility();
	statsobj = _statsobj;
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

// An event callback function that prints the operations performed by the OpenCL runtime.
void event_cb2(cl_event event1, cl_int cmd_status, void *data){
    cl_int err;
    cl_command_type command;
    cl::Event event(event1, true);
    OCL_CHECK(err, err = event.getInfo(CL_EVENT_COMMAND_TYPE, &command));
    cl_int status;
    OCL_CHECK(err,
              err = event.getInfo(CL_EVENT_COMMAND_EXECUTION_STATUS, &status));
    const char *command_str;
    const char *status_str;
    switch (command) {
    case CL_COMMAND_READ_BUFFER:
        command_str = "buffer read";
        break;
    case CL_COMMAND_WRITE_BUFFER:
        command_str = "buffer write";
        break;
    case CL_COMMAND_NDRANGE_KERNEL:
        command_str = "kernel";
        break;
    case CL_COMMAND_MAP_BUFFER:
        command_str = "kernel";
        break;
    case CL_COMMAND_COPY_BUFFER:
        command_str = "kernel";
        break;
    case CL_COMMAND_MIGRATE_MEM_OBJECTS:
        command_str = "buffer migrate";
        break;
    default:
        command_str = "unknown";
    }
    switch (status) {
    case CL_QUEUED:
        status_str = "Queued";
        break;
    case CL_SUBMITTED:
        status_str = "Submitted";
        break;
    case CL_RUNNING:
        status_str = "Executing";
        break;
    case CL_COMPLETE:
        status_str = "Completed";
        break;
    }
	#ifdef _DEBUGMODE_HOSTPRINTS3
    printf("[%s]: %s %s\n",
           reinterpret_cast<char *>(data),
           status_str,
           command_str);
	#endif 
    fflush(stdout);
}
// Sets the callback for a particular event
void set_callback2(cl::Event event, const char *queue_name){
    cl_int err;
    OCL_CHECK(err,
              err =
                  event.setCallback(CL_COMPLETE, event_cb2, (void *)queue_name));
}

void goclkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){		
	inputdata_size_bytes = PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt);
	
	cl_int err;
    auto devices = xcl::get_xil_devices();
    auto device = devices[0];

    //Creating Context and Command Queue for selected Device
    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
    OCL_CHECK(
        err,
        cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err));
    std::string device_name = device.getInfo<CL_DEVICE_NAME>();
    std::cout << "Found Device=" << device_name.c_str() << std::endl;

    int vector_length = LENGTH;
    bool match = true;
	
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<"goclkernel::runapp: Iteration "<<GraphIter<<endl;
		
		#ifdef ENABLE_ACTSPROC
		// ACTS (process)
		{
			cout<<"--------------- goclkernel[actssync]:: running ACTS PROCESS... ---------------"<<endl;
			
			cl_int err;
			std::vector<cl_mem_ext_ptr_t> inoutBufExt(32);
			std::vector<cl::Buffer> buffer_kvsourcedram(32);
			std::vector<cl::Kernel> krnls(32);
			
			#ifdef _DEBUGMODE_HOSTPRINTS3
			printf("INFO: loading vmul kernel\n");
			#endif
			auto fileBuf = xcl::read_binary_file(binaryFile[0]);
			cl::Program::Binaries vmul_bins{{fileBuf.data(), fileBuf.size()}};
			devices.resize(1);
			OCL_CHECK(err,
					  cl::Program program(context, devices, vmul_bins, NULL, &err));
			
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel:: creating kernel object..."<<endl;
			#endif
			std::string krnl_name = "topkernelproc";
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
					std::string cu_id = std::to_string((i+1));
					std::string krnl_name_full = krnl_name + ":{" + "topkernelproc_" + cu_id + "}"; 

					#ifdef _DEBUGMODE_HOSTPRINTS3
					printf("Creating a kernel [%s] for CU(%d)\n",
						   krnl_name_full.c_str(),
						   (i+1));
					#endif

					OCL_CHECK(err,
							  krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
			}

			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel:: creating OCL buffers..."<<endl;
			#endif
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ // NUMSUBCPUTHREADS
				OCL_CHECK(err,
				  buffer_kvsourcedram[i] =
					  cl::Buffer(context,
								 CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
								 sizeof(uint512_vec_dt) * PADDEDKVSOURCEDRAMSZ_KVS,
								 kvsourcedram[i],
								 &err));
			}

			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel:: setting kernel arguments..."<<endl;
			#endif
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[i])); }

			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel:: migrating workload to FPGA..."<<endl;
			#endif
			OCL_CHECK(err,
					  err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3]}, 		
													   0));

			// This function will execute the kernel on the FPGA
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel:: launching the kernel..."<<endl;
			#endif 
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ OCL_CHECK(err, err = q.enqueueTask(krnls[i])); }

			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel:: migrating workload back to HOST..."<<endl;
			#endif
			OCL_CHECK(err,
					  err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3]}, 
													   CL_MIGRATE_MEM_OBJECT_HOST));
			OCL_CHECK(err, err = q.finish());
		}
		#endif 
		
		#ifdef ENABLE_ACTSSYNC
		// ACTS (synchronize)
		{
			cout<<"--------------- goclkernel[actssync]:: running ACTS SYNCHRONZE... ---------------"<<endl;

			cl_int err;
			std::vector<cl_mem_ext_ptr_t> inoutBufExt(32);
			std::vector<cl::Buffer> buffer_kvsourcedram(32);
			std::vector<cl::Kernel> krnls(1);
			
			#ifdef _DEBUGMODE_HOSTPRINTS3
			printf("INFO: loading vmul kernel\n");
			#endif
			auto fileBuf = xcl::read_binary_file(binaryFile[1]);
			cl::Program::Binaries vmul_bins{{fileBuf.data(), fileBuf.size()}};
			devices.resize(1);
			OCL_CHECK(err,
					  cl::Program program(context, devices, vmul_bins, NULL, &err));
					  
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel:: creating kernel object..."<<endl;
			#endif
			std::string krnl_name = "topkernelsync";
			for(unsigned int i=0; i<1; i++){ 
					std::string cu_id = std::to_string((i+1));
					std::string krnl_name_full = krnl_name + ":{" + "topkernelsync_" + cu_id + "}"; 

					#ifdef _DEBUGMODE_HOSTPRINTS3
					printf("Creating a kernel [%s] for CU(%d)\n",
						   krnl_name_full.c_str(),
						   (i+1));
					#endif

					OCL_CHECK(err,
							  krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
			}

			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel[actssync]:: creating OCL buffers..."<<endl;
			#endif
			for(unsigned int i=0; i<NUMSUBCPUTHREADS + 1; i++){ 
				#ifdef _DEBUGMODE_HOSTPRINTS3
				cout<<"creating buffer for ACTS: "<<i<<endl;
				#endif
				if(i<NUMSUBCPUTHREADS){
					OCL_CHECK(err,
					  buffer_kvsourcedram[i] =
						  cl::Buffer(context,
								 CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
								 sizeof(uint512_vec_dt) * PADDEDKVSOURCEDRAMSZ_KVS,
								 kvsourcedram[i],
								 &err));
				} else {
					OCL_CHECK(err,
					  buffer_kvsourcedram[i] =
						  cl::Buffer(context,
								 CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
								 sizeof(uint512_vec_dt) * PADDEDKVSOURCEDRAMSZ_KVS,
								 vdram,
								 &err));
				}
			}

			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel[actssync]:: setting kernel arguments..."<<endl;
			#endif
			for(unsigned int i=0; i<NUMSUBCPUTHREADS + 1; i++){ OCL_CHECK(err, err = krnls[0].setArg(i, buffer_kvsourcedram[i])); }
			
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel[actssync]:: migrating workload to FPGA..."<<endl;
			#endif
			OCL_CHECK(err,
					  err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4]}, // {d_a, d_b, d_c, d_d, d_temp},		
								0));

			// This function will execute the kernel on the FPGA
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel[actssync]:: launching the kernel..."<<endl;
			#endif
			OCL_CHECK(err, err = q.enqueueTask(krnls[0]));

			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"goclkernel[actssync]:: migrating workload back to HOST..."<<endl;
			#endif
			OCL_CHECK(err,
					  err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4]}, // {d_d},
													   CL_MIGRATE_MEM_OBJECT_HOST));
			OCL_CHECK(err, err = q.finish());
		}
		#endif
	}
	return;
}
#endif 







