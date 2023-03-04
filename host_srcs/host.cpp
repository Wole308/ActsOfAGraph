/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

/*
  Overlap Host Code

  There are many applications where all of the data cannot reside in an FPGA.
  For example, the data is too big to fit in an FPGA or the data is being
  streamed from a sensor or the network. In these situations data must be
  transferred to the host memory to the FPGA before the computation can be
  performed.

  Because PCIe is an full-duplex interconnect, you can transfer data to and from
  the FPGA simultaneously. Xilinx FPGAs can also perform computations during
  these data transfers. Performing all three of these operations at the same
  time allows you to keep the FPGA busy and take full advantage of all of the
  hardware on your system.

  In this example, we will demonstrate how to perform this using an out of order
  command queue.

  +---------+---------+---------+----------+---------+---------+---------
  | WriteA1 | WriteB1 | WriteA2 | Write B2 | WriteA1 | WriteB1 |   Wri...
  +---------+---------+---------+----------+---------+---------+---------
                      |       Compute1     |     Compute2      |  Compu...
                      +--------------------+-------------------+--------+
                                           | ReadC1 |          | ReadC2 |
                                           +--------+          +--------+

  Many OpenCL commands are asynchronous. This means that whenever you call an
  OpenCL function, the function will return before the operation has completed.
  Asynchronous nature of OpenCL allows you to simultaneously perform tasks on
  the host CPU as well as the FPGA.

  Memory transfer operations are asynchronous when the blocking_read,
  blocking_write parameters are set to CL_FALSE. These operations are behaving
  on host memory so it is important to make sure that the command has completed
  before that memory is used.

  You can make sure an operation has completed by querying events returned by
  these commands. Events are OpenCL objects that track the status of operations.
  Event objects are created by kernel execution commands, read, write, copy
  commands on memory objects or user events created using clCreateUserEvent.

  Events can be used to synchronize operations between the host thread and the
  device or between two operations in the same context. You can also use events
  to time a particular operation if the command queue was created using the
  CL_QUEUE_PROFILING_ENABLE flag.

  Most enqueuing commands return events by accepting a cl_event pointer as their
  last argument of the call. These events can be queried using the
  clGetEventInfo function to get the status of a particular operation.

  Many functions also accept event lists that can be used to enforce ordering in
  an OpenCL context. These events lists are especially important in the context
  of out of order command queues as they are the only way specify dependency.
  Normal in-order command queues do not need this because dependency is enforced
  in the order the operation was enqueued. See the concurrent execution example
  for additional details on how create an use these types of command queues.
 */
#include "host.h"
#ifdef FPGA_IMPL
#include "xcl2.hpp"
#endif 

#include <algorithm>
#include <cstdio>
#include <random>
#include <vector>

using namespace std;
using std::default_random_engine;
using std::generate;
using std::uniform_int_distribution;
using std::vector;

#define NUM_KERNEL 1
#define NUM_HBM_ARGS (NUM_VALID_HBM_CHANNELS * 2)
#define NUM_HBMC_ARGS 2

host::host(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	myuniversalparams = _universalparams;
}
host::~host(){} 

// Number of HBM PCs required
#define MAX_HBM_PC_COUNT 32
#define PC_NAME(n) n | XCL_MEM_TOPOLOGY
#ifdef FPGA_IMPL
const int pc[MAX_HBM_PC_COUNT] = {
    PC_NAME(0),  PC_NAME(1),  PC_NAME(2),  PC_NAME(3),  PC_NAME(4),  PC_NAME(5),  PC_NAME(6),  PC_NAME(7),
    PC_NAME(8),  PC_NAME(9),  PC_NAME(10), PC_NAME(11), PC_NAME(12), PC_NAME(13), PC_NAME(14), PC_NAME(15),
    PC_NAME(16), PC_NAME(17), PC_NAME(18), PC_NAME(19), PC_NAME(20), PC_NAME(21), PC_NAME(22), PC_NAME(23),
    PC_NAME(24), PC_NAME(25), PC_NAME(26), PC_NAME(27), PC_NAME(28), PC_NAME(29), PC_NAME(30), PC_NAME(31)};

// An event callback function that prints the operations performed by the OpenCL
// runtime.
void _event_cb_async(cl_event event1, cl_int cmd_status, void* data) {
    cl_int err;
    cl_command_type command;
    cl::Event event(event1, true);
    OCL_CHECK(err, err = event.getInfo(CL_EVENT_COMMAND_TYPE, &command));
    cl_int status;
    OCL_CHECK(err, err = event.getInfo(CL_EVENT_COMMAND_EXECUTION_STATUS, &status));
    const char* command_str;
    const char* status_str;
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
    printf("[%s]: %s %s\n", reinterpret_cast<char*>(data), status_str, command_str);
    fflush(stdout);
}

// Sets the callback for a particular event
void _set_callback_async(cl::Event event, const char* queue_name) {
    cl_int err;
    OCL_CHECK(err, err = event.setCallback(CL_COMPLETE, _event_cb_async, (void*)queue_name));
}

void _set_args___actions(cl::Kernel * krnl_vadd, action_t action, cl_int err){
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS, int(action.module)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 1, int(action.start_pu)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 2, int(action.size_pu)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 3, int(action.start_pv)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 4, int(action.size_pv)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 5, int(action.start_llpset)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 6, int(action.size_llpset)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 7, int(action.start_llpid)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 8, int(action.size_llpid)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 9, int(action.start_gv)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 10, int(action.size_gv)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 11, int(action.finish)));
}
#endif 

#ifdef FPGA_IMPL
#define MIGRATE_HOST_TO_DEVICE() void _migrate_host_to_device(cl::CommandQueue * q, cl_int err, std::vector<cl::Buffer> &buffer_hbm, std::vector<cl::Buffer> &buffer_hbmc)
#define MIGRATE_DEVICE_TO_HOST() void _migrate_device_to_host(cl::CommandQueue * q, cl_int err, std::vector<cl::Buffer> &buffer_hbm, std::vector<cl::Buffer> &buffer_hbmc)
#define IMPORT_HOST_TO_DEVICE() void _import_host_to_device(cl::CommandQueue * q, cl_int err, std::vector<cl::Buffer> buffer_import)
#define EXPORT_DEVICE_TO_HOST() void _export_device_to_host(cl::CommandQueue * q, cl_int err, std::vector<cl::Buffer> buffer_export)
#else
#define MIGRATE_HOST_TO_DEVICE() void _migrate_host_to_device()
#define MIGRATE_DEVICE_TO_HOST() void _migrate_device_to_host()
#define IMPORT_HOST_TO_DEVICE() void _import_host_to_device()
#define EXPORT_DEVICE_TO_HOST() void _export_device_to_host()
#endif

action_t _get_action(unsigned int burst_compute, unsigned int num_burst_computes, universalparams_t universalparams){
	action_t action;
	if(num_burst_computes == 1){
		action.module = ALL_MODULES;
		action.start_pu = 0; 
		action.size_pu = universalparams.NUM_UPARTITIONS; 
		action.start_pv = 0;
		action.size_pv = universalparams.NUM_APPLYPARTITIONS; 
		action.start_llpset = 0; 
		action.size_llpset = universalparams.NUM_APPLYPARTITIONS; 
		action.start_llpid = 0; 
		action.size_llpid = EDGE_PACK_SIZE; 
		action.start_gv = 0; 
		action.size_gv = NUM_VALID_PEs;
		action.finish = 1;
	} else {
		if(burst_compute >= 0 && burst_compute < universalparams.NUM_UPARTITIONS){ 
			// scatter <===> transport
			action.module = PROCESS_EDGES_MODULE;
			action.start_pu = burst_compute; 
			action.size_pu = 1; 
			action.finish = 0;
		} else if(burst_compute >= universalparams.NUM_UPARTITIONS && burst_compute < universalparams.NUM_UPARTITIONS + universalparams.NUM_APPLYPARTITIONS){
			// apply and gatherDSTs <===> transport
			action.module = APPLY_UPDATES_MODULE___AND___GATHER_DSTPROPERTIES_MODULE;
			action.start_pv = burst_compute - universalparams.NUM_UPARTITIONS; 
			action.size_pv = 1; 
			action.start_gv = burst_compute - universalparams.NUM_UPARTITIONS; 
			action.size_gv = 1;
			action.finish = 0;
		} else {
			cout<<"ERROR 234. burst_compute is out-of-range. EXITING... "<<endl;
			exit(EXIT_FAILURE);
		}
	}	
	return action;
}

MIGRATE_HOST_TO_DEVICE(){
	std::cout << "Copying data (Host to Device)..." << std::endl;
	std::chrono::steady_clock::time_point begin_time0 = std::chrono::steady_clock::now();
	for (int i = 0; i < NUM_HBM_ARGS; i++) {
		std::cout << "Copying data @ channel "<<i<<" (Host to Device)..." << std::endl;
		#ifdef FPGA_IMPL
		OCL_CHECK(err, err = q->enqueueMigrateMemObjects({buffer_hbm[i]}, 0));
		#else 
			
		#endif 
	}
	for (int i = 0; i < NUM_HBMC_ARGS; i++) {
		std::cout << "Copying data @ center channel "<<i<<" (Host to Device)..." << std::endl;
		#ifdef FPGA_IMPL
		OCL_CHECK(err, err = q->enqueueMigrateMemObjects({buffer_hbmc[i]}, 0));
		#else 
			
		#endif 
	}
	#ifdef FPGA_IMPL
	OCL_CHECK(err, err = q->finish());
	#endif 
	double end_time0 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time0).count()) / 1000;	
	std::cout <<">>> write-to-FPGA time elapsed : "<<end_time0<<" ms, "<<(end_time0 * 1000)<<" microsecs, "<<std::endl;
	return;
}
MIGRATE_DEVICE_TO_HOST(){
	std::cout << "Getting Results (Device to Host)..." << std::endl;
	std::chrono::steady_clock::time_point begin_time2 = std::chrono::steady_clock::now();
	for (int i = 0; i < NUM_HBMC_ARGS; i++) {
		std::cout << "Copying data @ center channel "<<i<<" (Device to Host)..." << std::endl;
		#ifdef FPGA_IMPL
		OCL_CHECK(err, err = q->enqueueMigrateMemObjects({buffer_hbmc[i]}, CL_MIGRATE_MEM_OBJECT_HOST));
		#else 
			
		#endif 
	}
	#ifdef FPGA_IMPL
	OCL_CHECK(err, err = q->finish());
	#endif 
	double end_time2 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time2).count()) / 1000;	
	std::cout <<">>> read-from-FPGA time elapsed : "<<end_time2<<" ms, "<<(end_time2 * 1000)<<" microsecs, "<<std::endl;
	return;
}
void _migrate_frontiers_host_to_device(unsigned int p_u, uint32_t* in_hbmc_pu[NUM_HBMC_ARGS][MAX_NUM_UPARTITIONS], universalparams_t universalparams){
	std::cout << "Copying frontier partition "<<p_u<<" data (Host to Device)..." << std::endl;
	std::chrono::steady_clock::time_point begin_time0 = std::chrono::steady_clock::now();
	algorithm * algorithmobj = new algorithm();
	unsigned int index = 0; 
	for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){ 
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
			in_hbmc_pu[0][p_u][index + v] = 1; // algorithmobj->vertex_initdata(universalparams.ALGORITHM, index);
		}
		for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
			in_hbmc_pu[1][p_u][index + v] = 1; // algorithmobj->vertex_initdata(universalparams.ALGORITHM, index);
		}
		index += EDGE_PACK_SIZE;
	}
	double end_time0 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time0).count()) / 1000;	
	std::cout <<">>> write-to-FPGA time elapsed : "<<end_time0<<" ms, "<<(end_time0 * 1000)<<" microsecs, "<<std::endl;
	return;
}
EXPORT_DEVICE_TO_HOST(){
	std::cout << "Getting Export Results (Device to Host)..." << std::endl;
	std::chrono::steady_clock::time_point begin_time2 = std::chrono::steady_clock::now();
	#ifdef FPGA_IMPL
	OCL_CHECK(err, err = q->enqueueMigrateMemObjects({buffer_export}, CL_MIGRATE_MEM_OBJECT_HOST));
	OCL_CHECK(err, err = q->finish());
	#else 
		
	#endif 
	double end_time2 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time2).count()) / 1000;	
	std::cout <<">>> read-from-FPGA time elapsed : "<<end_time2<<" ms, "<<(end_time2 * 1000)<<" microsecs, "<<std::endl;
	return;
}

long double host::runapp(action_t action__, std::string binaryFile__[2], HBM_channelAXISW_t * HBM_axichannel[2][NUM_PEs], HBM_channelAXISW_t * HBM_axicenter[2], HBM_channelAXISW_t * HBM_import_export[2], unsigned int globalparams[1024], universalparams_t universalparams){					
	unsigned int ARRAY_SIZE = HBM_CHANNEL_SIZE * HBM_AXI_PACK_SIZE;
	
	cout<<"--- host::runapp_sync: NUM_HBM_ARGS: "<<NUM_HBM_ARGS<<" ---"<<endl;
	cout<<"--- host::runapp_sync: ARRAY_SIZE: "<<ARRAY_SIZE<<" ---"<<endl;
	
	// set arguments
	#ifdef RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
	acts_kernel * acts2 = new acts_kernel(universalparams);
	action_t action2 = _get_action(0, 1, universalparams);
	// run kernel 
	printf("Enqueueing NDRange kernel.\n");
	std::chrono::steady_clock::time_point begin_time1 = std::chrono::steady_clock::now();
	#ifdef FPGA_IMPL
	OCL_CHECK(err, err = q.enqueueTask(krnl_vadd));
	OCL_CHECK(err, err = q.finish());
	#else 
	acts2->top_function(
		(HBM_channelAXI_t *)HBM_axichannel[0][0], (HBM_channelAXI_t *)HBM_axichannel[1][0]
		#if NUM_VALID_HBM_CHANNELS>1
		,(HBM_channelAXI_t *)HBM_axichannel[0][1], (HBM_channelAXI_t *)HBM_axichannel[1][1] 
		,(HBM_channelAXI_t *)HBM_axichannel[0][2], (HBM_channelAXI_t *)HBM_axichannel[1][2] 
		,(HBM_channelAXI_t *)HBM_axichannel[0][3], (HBM_channelAXI_t *)HBM_axichannel[1][3] 
		,(HBM_channelAXI_t *)HBM_axichannel[0][4], (HBM_channelAXI_t *)HBM_axichannel[1][4] 
		,(HBM_channelAXI_t *)HBM_axichannel[0][5], (HBM_channelAXI_t *)HBM_axichannel[1][5] 
		#if NUM_VALID_HBM_CHANNELS>6
		,(HBM_channelAXI_t *)HBM_axichannel[0][6], (HBM_channelAXI_t *)HBM_axichannel[1][6] 
		,(HBM_channelAXI_t *)HBM_axichannel[0][7], (HBM_channelAXI_t *)HBM_axichannel[1][7] 
		,(HBM_channelAXI_t *)HBM_axichannel[0][8], (HBM_channelAXI_t *)HBM_axichannel[1][8] 
		,(HBM_channelAXI_t *)HBM_axichannel[0][9], (HBM_channelAXI_t *)HBM_axichannel[1][9] 
		,(HBM_channelAXI_t *)HBM_axichannel[0][10], (HBM_channelAXI_t *)HBM_axichannel[1][10] 
		,(HBM_channelAXI_t *)HBM_axichannel[0][11], (HBM_channelAXI_t *)HBM_axichannel[1][11]
		#if NUM_VALID_HBM_CHANNELS>12
		,(HBM_channelAXI_t *)HBM_axichannel[0][12], (HBM_channelAXI_t *)HBM_axichannel[1][12]
		#endif 
		#endif 
		#endif
		,(HBM_channelAXI_t *)HBM_axicenter[0], (HBM_channelAXI_t *)HBM_axicenter[1]
		,action2.module ,action2.start_pu ,action2.size_pu ,action2.start_pv ,action2.size_pv ,action2.start_llpset ,action2.size_llpset ,action2.start_llpid ,action2.size_llpid ,action2.start_gv ,action2.size_gv ,action2.finish
		);	
	#endif 
	exit(EXIT_SUCCESS);
	#endif 
		
	// prepare OCL variables 
	#ifdef FPGA_IMPL
    // auto binaryFile = argv[1];
	std::string binaryFile = binaryFile__[0]; 
    cl_int err;
    cl::CommandQueue q;
    cl::Context context;
    cl::Kernel krnl_vadd;

    // OPENCL HOST CODE AREA START
    // get_xil_devices() is a utility API which will find the xilinx
    // platforms and will return list of devices connected to Xilinx platform
    std::cout << "Creating Context..." << std::endl;
    auto devices = xcl::get_xil_devices();

    // read_binary_file() is a utility API which will load the binaryFile
    // and will return the pointer to file buffer.
    auto fileBuf = xcl::read_binary_file(binaryFile);
    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    bool valid_device = false;
    for (unsigned int i = 0; i < devices.size(); i++) {
        auto device = devices[i];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
        // This example will use an out of order command queue. The default command
        // queue created by cl::CommandQueue is an inorder command queue.
        // OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err));
		// OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &err)); 
		OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &err)); ////////// FIXME.

        std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";
            OCL_CHECK(err, krnl_vadd = cl::Kernel(program, "top_function", &err));
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }
	#endif 

    // We will break down our problem into multiple iterations. Each iteration
    // will perform computation on a subset of the entire data-set.
    // size_t elements_per_iteration = ARRAY_SIZE; // 2048;
    size_t bytes_per_iteration = ARRAY_SIZE * sizeof(int); // elements_per_iteration * sizeof(int);
	// size_t bytes_per_iteration = 64 * sizeof(int); // 1000000 * sizeof(int); ////////////////////////////////////// REMOVEME.
	cout<<"--- host::runapp_sync: bytes_per_iteration: "<<bytes_per_iteration<<" ---"<<endl;
	
	#ifdef FPGA_IMPL
	std::vector<int, aligned_allocator<int> > HHX[32]; for(unsigned int i=0; i<NUM_PEs*2; i++){ HHX[i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }
	std::vector<int, aligned_allocator<int> > HHC[2][2]; for(unsigned int flag=0; flag<2; flag++){ for(unsigned int i=0; i<2; i++){ HHC[flag][i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }}
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HHX[2*i][t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[0][i][t].data[v]; }
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HHX[2*i+1][t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[1][i][t].data[v]; }
		}
	}
	for(unsigned int flag=0; flag<2; flag++){ 
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HHC[flag][0][t*HBM_AXI_PACK_SIZE + v] = HBM_axicenter[0][t].data[v]; }
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HHC[flag][1][t*HBM_AXI_PACK_SIZE + v] = HBM_axicenter[1][t].data[v]; }
		}
	}
	#endif 
	
    // THIS PAIR OF EVENTS WILL BE USED TO TRACK WHEN A KERNEL IS FINISHED WITH
    // THE INPUT BUFFERS. ONCE THE KERNEL IS FINISHED PROCESSING THE DATA, A NEW
    // SET OF ELEMENTS WILL BE WRITTEN INTO THE BUFFER.
	#ifdef FPGA_IMPL
    vector<cl::Event> kernel_events(2);
    vector<cl::Event> read_events(2);
	
	std::vector<cl::Buffer> buffer_hbm(32);
	std::vector<cl::Buffer> buffer_hbmc(2*2);
	std::vector<cl_mem_ext_ptr_t> inBufExt(32);
	std::vector<cl_mem_ext_ptr_t> inBufExt_c(2);
	
	for (int i = 0; i < NUM_HBM_ARGS; i++) {
        inBufExt[i].obj = HHX[i].data();
        inBufExt[i].param = 0;
        inBufExt[i].flags = pc[i];
    }
	for (int i = 0; i < NUM_HBMC_ARGS; i++) {
        inBufExt_c[i].obj = HHC[0][i].data();
        inBufExt_c[i].param = 0;
        inBufExt_c[i].flags = pc[NUM_HBM_ARGS + i];
    }
	#endif 
	
	// Allocate Buffer in Global Memory
	// Buffers are allocated using CL_MEM_USE_HOST_PTR for efficient memory and
	// Device-to-host communication
	#ifdef FPGA_IMPL
	std::cout << "Creating Buffers..." << std::endl;
	for (int i = 0; i < NUM_HBM_ARGS; i++) {
		std::cout << "Creating Buffer "<<i<<"..." << std::endl;
		OCL_CHECK(err, buffer_hbm[i] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt[i], &err)); // REMOVEME 'i%6'
	}
	std::cout << "Creating Center Buffers..." << std::endl;
	for (int i = 0; i < NUM_HBMC_ARGS; i++) {
		std::cout << "Creating Center Buffer "<<i<<"..." << std::endl;
		OCL_CHECK(err, buffer_hbmc[i] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt_c[i], &err)); // REMOVEME 'i%6'
	}
	#endif 
	
	// Map frontier partitions
	std::cout << "Mapping frontier partitions..." << std::endl;
	uint32_t* in_hbmc_pu[NUM_FPGAS][NUM_HBMC_ARGS][MAX_NUM_UPARTITIONS];
	for (int i = 0; i < NUM_HBMC_ARGS; i++) {
		for (int p_u = 0; p_u < universalparams.NUM_UPARTITIONS; p_u++) {
			std::cout << "Mapping frontier partition ("<<i<<", "<<p_u<<") to host memory..." << std::endl;
			#ifdef FPGA_IMPL
			OCL_CHECK(err, in_hbmc_pu[0][i][p_u] = (uint32_t*)q.enqueueMapBuffer(buffer_hbmc[i], CL_TRUE, CL_MAP_WRITE, (p_u * MAX_UPARTITION_VECSIZE * EDGE_PACK_SIZE * sizeof(int)), ((p_u + 1) * MAX_UPARTITION_VECSIZE * EDGE_PACK_SIZE * sizeof(int)), nullptr,
															nullptr, &err));
			#else 
			in_hbmc_pu[0][i][p_u] = (unsigned int *)&HBM_axicenter[i][p_u * MAX_UPARTITION_VECSIZE * EDGE_PACK_SIZE];
			#endif 
		}
	}
	
	// Set Kernel Arguments
	#ifdef FPGA_IMPL
	std::cout << "Setting Kernel Arguments..." << std::endl;
	for (int i = 0; i < NUM_HBM_ARGS; i++) {
		std::cout << "Setting the k_vadd Argument for argument "<<i<<"..." << std::endl;
		OCL_CHECK(err, err = krnl_vadd.setArg(i, buffer_hbm[i]));
	}
	std::cout << "Setting Kernel Arguments (center HBM)..." << std::endl;
	for (int i = 0; i < NUM_HBMC_ARGS; i++) {
		std::cout << "Setting Kernel Argument for argument "<<i<<"..." << std::endl;
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + i, buffer_hbmc[i]));
	}
	#endif 
	
	// Copy input data to device global memory
	#ifdef FPGA_IMPL
	_migrate_host_to_device(&q, err, buffer_hbm, buffer_hbmc);
	#else 
	_migrate_host_to_device();	
	#endif 
	for (int fpga = 0; fpga < NUM_FPGAS; fpga++) {
		for(unsigned int p_u=0; p_u<universalparams.NUM_UPARTITIONS; p_u++){
			// _migrate_frontiers_host_to_device(p_u, in_hbmc_pu[fpga], universalparams);
		}
	}
		
	// run acts
	action_t action;
	action.module = ALL_MODULES;
	action.start_pu = 0; 
	action.size_pu = universalparams.NUM_UPARTITIONS; 
	action.start_pv = 0;
	action.size_pv = universalparams.NUM_APPLYPARTITIONS; 
	action.start_llpset = 0; 
	action.size_llpset = universalparams.NUM_APPLYPARTITIONS; 
	action.start_llpid = 0; 
	action.size_llpid = EDGE_PACK_SIZE; 
	action.start_gv = 0; 
	action.size_gv = NUM_VALID_PEs;
	action.finish = 1;
	
	unsigned int num_burst_computes = 1;
	// unsigned int num_burst_computes = universalparams.NUM_UPARTITIONS;
	// unsigned int num_burst_computes = universalparams.NUM_UPARTITIONS + universalparams.NUM_APPLYPARTITIONS;
	#ifndef FPGA_IMPL
	acts_kernel * acts = new acts_kernel(universalparams);
	#endif 
	
	std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();
	for (unsigned int iteration_idx = 0; iteration_idx < 1; iteration_idx++) {
		for(unsigned int burst_compute=0; burst_compute<num_burst_computes; burst_compute++){
			std::cout <<"------------------------- host: burst_compute "<<burst_compute<<" started... -------------------------"<<std::endl;
		
			// transport burst frontiers
			std::chrono::steady_clock::time_point begin_time0 = std::chrono::steady_clock::now();
			if(burst_compute >= 0 && burst_compute < universalparams.NUM_UPARTITIONS){
				_migrate_frontiers_host_to_device(burst_compute, in_hbmc_pu[0], universalparams);
			}
			double end_time0 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time0).count()) / 1000;	
			std::cout << TIMINGRESULTSCOLOR << ">>> time elapsed (frontier partition "<<burst_compute<<") for iteration "<<iteration_idx<<", burst_compute "<<burst_compute<<" : "<<end_time0<<" ms, "<<(end_time0 * 1000)<<" microsecs, "<< RESET <<std::endl;
			
			// set arguments
			action_t action = _get_action(burst_compute, num_burst_computes, universalparams);
			#ifdef FPGA_IMPL
			_set_args___actions(&krnl_vadd, action, err);
			#endif 
			
			// import  
			#ifdef ____NOT___YET___IMPLEMENTED___
			#ifdef FPGA_IMPL
			_import_host_to_device(&q, err, buffer_import);
			#else 
				
			#endif 
			#endif

			// run kernel 
			printf("Enqueueing NDRange kernel.\n");
			std::chrono::steady_clock::time_point begin_time1 = std::chrono::steady_clock::now();
			#ifdef FPGA_IMPL
			OCL_CHECK(err, err = q.enqueueTask(krnl_vadd));
			OCL_CHECK(err, err = q.finish());
			#else 
			acts->top_function(
				(HBM_channelAXI_t *)HBM_axichannel[0][0], (HBM_channelAXI_t *)HBM_axichannel[1][0]
				#if NUM_VALID_HBM_CHANNELS>1
				,(HBM_channelAXI_t *)HBM_axichannel[0][1], (HBM_channelAXI_t *)HBM_axichannel[1][1] 
				,(HBM_channelAXI_t *)HBM_axichannel[0][2], (HBM_channelAXI_t *)HBM_axichannel[1][2] 
				,(HBM_channelAXI_t *)HBM_axichannel[0][3], (HBM_channelAXI_t *)HBM_axichannel[1][3] 
				,(HBM_channelAXI_t *)HBM_axichannel[0][4], (HBM_channelAXI_t *)HBM_axichannel[1][4] 
				,(HBM_channelAXI_t *)HBM_axichannel[0][5], (HBM_channelAXI_t *)HBM_axichannel[1][5] 
				#if NUM_VALID_HBM_CHANNELS>6
				,(HBM_channelAXI_t *)HBM_axichannel[0][6], (HBM_channelAXI_t *)HBM_axichannel[1][6] 
				,(HBM_channelAXI_t *)HBM_axichannel[0][7], (HBM_channelAXI_t *)HBM_axichannel[1][7] 
				,(HBM_channelAXI_t *)HBM_axichannel[0][8], (HBM_channelAXI_t *)HBM_axichannel[1][8] 
				,(HBM_channelAXI_t *)HBM_axichannel[0][9], (HBM_channelAXI_t *)HBM_axichannel[1][9] 
				,(HBM_channelAXI_t *)HBM_axichannel[0][10], (HBM_channelAXI_t *)HBM_axichannel[1][10] 
				,(HBM_channelAXI_t *)HBM_axichannel[0][11], (HBM_channelAXI_t *)HBM_axichannel[1][11]
				#if NUM_VALID_HBM_CHANNELS>12
				,(HBM_channelAXI_t *)HBM_axichannel[0][12], (HBM_channelAXI_t *)HBM_axichannel[1][12]
				#endif 
				#endif 
				#endif
				,(HBM_channelAXI_t *)HBM_axicenter[0], (HBM_channelAXI_t *)HBM_axicenter[1]
				,(HBM_channelAXI_t *)HBM_import_export[0], (HBM_channelAXI_t *)HBM_import_export[1]
				,action.module ,action.start_pu ,action.size_pu ,action.start_pv ,action.size_pv ,action.start_llpset ,action.size_llpset ,action.start_llpid ,action.size_llpid ,action.start_gv ,action.size_gv ,action.finish
				);	
			#endif 
			double end_time1 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time1).count()) / 1000;	
			std::cout << TIMINGRESULTSCOLOR << ">>> kernel time elapsed for iteration "<<iteration_idx<<", burst_compute "<<burst_compute<<" : "<<end_time1<<" ms, "<<(end_time1 * 1000)<<" microsecs, "<< RESET <<std::endl;
		
			// export 
			#ifdef ____NOT___YET___IMPLEMENTED___
			#ifdef FPGA_IMPL
			_export_device_to_host(&q, err, buffer_export);
			#else 
				
			#endif 
			#endif 
		}
    }
	double end_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time).count()) / 1000;	
	std::cout <<">>> kernel time elapsed for all iterations : "<<end_time<<" ms, "<<(end_time * 1000)<<" microsecs, "<<std::endl;

	// Copy Result from Device Global Memory to Host Local Memory
	#ifdef FPGA_IMPL
	_migrate_device_to_host(&q, err, buffer_hbm, buffer_hbmc);
	#else 
	_migrate_device_to_host();	
	#endif 
			
	// Wait for all of the OpenCL operations to complete
	#ifdef FPGA_IMPL
    printf("Waiting...\n");
    OCL_CHECK(err, err = q.flush());
    OCL_CHECK(err, err = q.finish());
	#endif 
    // OPENCL HOST CODE AREA ENDS

	printf("TEST %s\n", "PASSED");
    return EXIT_SUCCESS;
}

