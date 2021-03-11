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
	#ifdef _DEBUGMODE_HOSTPRINTS2
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

void goclkernel::runapp(std::string binaryFile, uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){		
	cout<<"goclkernel::runapp: TOTALNUMKERNELS: "<<TOTALNUMKERNELS<<", TOTALNUMBUFFERS: "<<TOTALNUMBUFFERS<<", inputdata_size_bytes: "<<PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)<<endl;

	cl_int err;
	inputdata_size_bytes = PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt);

	// create context and command queue for selected device
	cout<<"goclkernel:: creating context and command queue for selected device..."<<endl;
    auto devices = xcl::get_xil_devices();
    auto device = devices[0];

    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
    OCL_CHECK(err,
              cl::CommandQueue q(context,
                                 device,
                                 CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE |
                                     CL_QUEUE_PROFILING_ENABLE,
                                 &err));

    std::string device_name = device.getInfo<CL_DEVICE_NAME>();
    std::cout << "Found Device=" << device_name.c_str() << std::endl;

	// The code block below is in braces because the cl objects
    // are automatically released once the block ends
	{
		vector<cl::Event> read_events;
		vector<cl::Event> kernel_events;
		vector<cl::Event> write_event;
		read_events.resize((2 * TOTALNUMBUFFERS));
		kernel_events.resize((2 * TOTALNUMKERNELS));
		write_event.resize(TOTALNUMBUFFERS);
		
		cl_int err;
		cl_mem_ext_ptr_t inoutBufExt[TOTALNUMBUFFERS];
		cl::Buffer buffer_kvsourcedram[TOTALNUMBUFFERS];
		cl::Kernel krnls[TOTALNUMKERNELS];
		
		// create binary
		cout<<"goclkernel:: creating binary from file..."<<endl;
		auto fileBuf = xcl::read_binary_file(binaryFile);
		cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
		devices.resize(1);
		
		// create program
		cout<<"goclkernel:: creating program object..."<<endl;
		OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));
		
		// create kernels
		cout<<"goclkernel:: creating kernel object..."<<endl;
		#ifdef NACTS_IN_NCOMPUTEUNITS
		std::string krnl_name = "topkernelproc";
		#else 
		std::string krnl_name = "topkernel";	
		#endif
		for(unsigned int i=0; i<TOTALNUMKERNELS; i++){ 
				std::string cu_id = std::to_string((i+1));
				#ifdef NACTS_IN_NCOMPUTEUNITS
				std::string krnl_name_full = krnl_name + ":{" + "topkernelproc_" + cu_id + "}"; 
				#else 
				std::string krnl_name_full = krnl_name + ":{" + "topkernel_" + cu_id + "}"; 
				#endif 

				printf("Creating a kernel [%s] for CU(%d)\n",
					   krnl_name_full.c_str(),
					   (i+1));

				OCL_CHECK(err,
						  krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
		}
		
		// create buffers
		cout<<"goclkernel:: creating OCL buffers..."<<endl;
		unsigned int counter = 0;
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){
			cout<<"attaching bufferExt "<<i<<" to HBM bank: "<<i<<endl;
			inoutBufExt[i].obj = kvsourcedram[i];
			inoutBufExt[i].param = 0;
			inoutBufExt[i].flags = bank[i];
		}
		
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){
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
		
		// migrate workload
		cout<<"goclkernel:: migrating workload to FPGA..."<<endl;
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){
			OCL_CHECK(err,
				err = q.enqueueMigrateMemObjects(
					  {buffer_kvsourcedram[i]},
					  0,
					  NULL,
					  &write_event[i]));
			set_callback2(write_event[i], "ooo_queue");
		}
		
		// set the kernel arguments
		cout<<"goclkernel:: setting kernel arguments..."<<endl;
		#ifdef NACTS_IN_NCOMPUTEUNITS
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){ OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[i])); }
		#else
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){ OCL_CHECK(err, err = krnls[0].setArg(i, buffer_kvsourcedram[i])); }
		#endif 
		
		// Invoking the kernel
		cout<<"goclkernel:: launching the kernel..."<<endl;
		std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
		for(unsigned int i=0; i<1; i++){ // TOTALNUMKERNELS
			cout<<"goclkernel::runapp:: Kernel "<<i<<" Launched"<<endl;
			std::vector<cl::Event> waitList;
			waitList.push_back(write_event[0]);
			OCL_CHECK(err,
					  err = q.enqueueNDRangeKernel(
						  krnls[i], 0, 1, 1, &waitList, &kernel_events[i]));
			set_callback2(kernel_events[i], "ooo_queue");
		}
		q.finish();
		
		// migrate workload
		cout<<"goclkernel:: migrating workload back to HOST..."<<endl;
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){
			OCL_CHECK(err,
				  err = q.enqueueMigrateMemObjects(
					  {buffer_kvsourcedram[i]},
					  CL_MIGRATE_MEM_OBJECT_HOST,
					  NULL,
					  &read_events[i]));	
			set_callback2(read_events[i], "ooo_queue");
			OCL_CHECK(err, err = read_events[i].wait());
		}
		
		// release kernel arguments
		cout<<"goclkernel:: releasing kernel arguments..."<<endl;
		OCL_CHECK(err, err = q.finish());
		long double kerneltimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
		std::cout<< TIMINGRESULTSCOLOR <<">>> total time elapsed(kerneltimeelapsed_ms): "<<kerneltimeelapsed_ms<<" ms"<< RESET <<std::endl;
		statsobj->appendkerneltimeelapsed(kerneltimeelapsed_ms);
	}
	
	// The code block below is in braces because the cl objects
    // are automatically released once the block ends
	{
		vector<cl::Event> read_events;
		vector<cl::Event> kernel_events;
		vector<cl::Event> write_event;
		read_events.resize((2 * TOTALNUMBUFFERS));
		kernel_events.resize((2 * TOTALNUMKERNELS));
		write_event.resize(TOTALNUMBUFFERS);
		
		cl_int err;
		cl_mem_ext_ptr_t inoutBufExt[TOTALNUMBUFFERS];
		cl::Buffer buffer_kvsourcedram[TOTALNUMBUFFERS];
		cl::Kernel krnls[TOTALNUMKERNELS];
		
		// create binary
		cout<<"goclkernel[actssync]:: creating binary from file..."<<endl;
		auto fileBuf = xcl::read_binary_file(binaryFile);
		cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
		// devices.resize(1);
		
		// create program
		cout<<"goclkernel[actssync]:: creating program object..."<<endl;
		OCL_CHECK(err, cl::Program program(context, devices, bins));
		
		// create kernels
		cout<<"goclkernel[actssync]:: creating kernel object..."<<endl;
		#ifdef NACTS_IN_NCOMPUTEUNITS
		std::string krnl_name = "topkernelproc";
		#else 
		std::string krnl_name = "topkernel";	
		#endif
		for(unsigned int i=0; i<TOTALNUMKERNELS; i++){ 
				std::string cu_id = std::to_string((i+1));
				#ifdef NACTS_IN_NCOMPUTEUNITS
				std::string krnl_name_full = krnl_name + ":{" + "topkernelproc_" + cu_id + "}"; 
				#else 
				std::string krnl_name_full = krnl_name + ":{" + "topkernel_" + cu_id + "}"; 
				#endif 

				printf("Creating a kernel [%s] for CU(%d)\n",
					   krnl_name_full.c_str(),
					   (i+1));

				OCL_CHECK(err,
						  krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
		}
		
		// create buffers
		cout<<"goclkernel[actssync]:: creating OCL buffers..."<<endl;
		unsigned int counter = 0;
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){
			cout<<"attaching bufferExt "<<i<<" to HBM bank: "<<i<<endl;
			inoutBufExt[i].obj = kvsourcedram[counter++];
			inoutBufExt[i].param = 0;
			inoutBufExt[i].flags = bank[i];
		}
		
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){
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
		
		// migrate workload
		cout<<"goclkernel[actssync]:: migrating workload to FPGA..."<<endl;
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){
			OCL_CHECK(err,
				err = q.enqueueMigrateMemObjects(
					  {buffer_kvsourcedram[i]},
					  0,
					  NULL,
					  &write_event[i]));
			set_callback2(write_event[i], "ooo_queue");
		}
		
		// set the kernel arguments
		cout<<"goclkernel[actssync]:: setting kernel arguments..."<<endl;
		#ifdef NACTS_IN_NCOMPUTEUNITS
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){ OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[i])); }
		#else
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){ OCL_CHECK(err, err = krnls[0].setArg(i, buffer_kvsourcedram[i])); }
		#endif 
		
		// Invoking the kernel
		cout<<"goclkernel[actssync]:: launching the kernel..."<<endl;
		std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
		for(unsigned int i=0; i<1; i++){ // TOTALNUMKERNELS
			cout<<"goclkernel[actssync]::runapp:: Kernel "<<i<<" Launched"<<endl;
			std::vector<cl::Event> waitList;
			waitList.push_back(write_event[0]);
			OCL_CHECK(err,
					  err = q.enqueueNDRangeKernel(
						  krnls[i], 0, 1, 1, &waitList, &kernel_events[i]));
			set_callback2(kernel_events[i], "ooo_queue");
		}
		q.finish();
		
		// migrate workload
		cout<<"goclkernel[actssync]:: migrating workload back to HOST..."<<endl;
		for(unsigned int i=0; i<TOTALNUMBUFFERS; i++){
			OCL_CHECK(err,
				  err = q.enqueueMigrateMemObjects(
					  {buffer_kvsourcedram[i]},
					  CL_MIGRATE_MEM_OBJECT_HOST,
					  NULL,
					  &read_events[i]));	
			set_callback2(read_events[i], "ooo_queue");
			OCL_CHECK(err, err = read_events[i].wait());
		}
		
		// release kernel arguments
		cout<<"goclkernel[actssync]:: releasing kernel arguments..."<<endl;
		OCL_CHECK(err, err = q.finish());
		long double kerneltimeelapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
		std::cout<< TIMINGRESULTSCOLOR <<">>> total time elapsed(kerneltimeelapsed_ms): "<<kerneltimeelapsed_ms<<" ms"<< RESET <<std::endl;
		statsobj->appendkerneltimeelapsed(kerneltimeelapsed_ms);
	}
	return;
}
#endif 







