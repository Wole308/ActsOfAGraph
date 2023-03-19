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
#define NUM_HBMIO_ARGS 2
#define NUM_HBMIO_CHKPTS_ARGS 2

#define NUM_IMPORT_BUFFERS 16 // FIXME. AUTOMATE.
#define NUM_EXPORT_BUFFERS 16

#define ___SYNC___

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
void event_cb(cl_event event1, cl_int cmd_status, void* data) {
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
void set_callback(cl::Event event, const char* queue_name) {
    cl_int err;
    OCL_CHECK(err, err = event.setCallback(CL_COMPLETE, event_cb, (void*)queue_name));
}

void _set_args___actions(cl::Kernel * krnl_vadd, action_t action, cl_int err){
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS, int(action.module)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 1, int(action.graph_iteration)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 2, int(action.start_pu)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 3, int(action.size_pu)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 4, int(action.start_pv)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 5, int(action.size_pv)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 6, int(action.start_llpset)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 7, int(action.size_llpset)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 8, int(action.start_llpid)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 9, int(action.size_llpid)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 10, int(action.start_gv)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 11, int(action.size_gv)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 12, int(action.id_import)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 13, int(action.id_export)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 14, int(action.size_import_export)));
	OCL_CHECK(err, err = krnl_vadd->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 15, int(action.status)));
}
#endif 

#ifdef FPGA_IMPL
#define MIGRATE_HOST_TO_DEVICE() void _migrate_host_to_device(cl::CommandQueue * q, cl_int err, std::vector<cl::Buffer> &buffer_hbm, std::vector<cl::Buffer> &buffer_hbmc)
#define MIGRATE_DEVICE_TO_HOST() void _migrate_device_to_host(cl::CommandQueue * q, cl_int err, std::vector<cl::Buffer> &buffer_hbm, std::vector<cl::Buffer> &buffer_hbmc)
#define IMPORT_HOST_TO_DEVICE() void _import_host_to_device(cl::CommandQueue * q, cl_int err, std::vector<cl::Buffer> &buffer_import, unsigned int flag)
#define EXPORT_DEVICE_TO_HOST() void _export_device_to_host(cl::CommandQueue * q, cl_int err, std::vector<cl::Buffer> &buffer_export, unsigned int flag)
#else
#define MIGRATE_HOST_TO_DEVICE() void _migrate_host_to_device()
#define MIGRATE_DEVICE_TO_HOST() void _migrate_device_to_host()
#define IMPORT_HOST_TO_DEVICE() void _import_host_to_device()
#define EXPORT_DEVICE_TO_HOST() void _export_device_to_host()
#endif

action_t _get_action(unsigned int launch_idx, unsigned int launch_type, universalparams_t universalparams){
	action_t action;
	if(launch_type == 0){
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
		action.status = 1;
	} else {		
		if(launch_idx >= 0 && launch_idx < universalparams.NUM_UPARTITIONS){ 
			// scatter <===> transport
			action.module = PROCESS_EDGES_MODULE;
			action.start_pu = launch_idx; 
			action.size_pu = 1; 
			action.status = 0;
		} else if(launch_idx >= universalparams.NUM_UPARTITIONS && launch_idx < universalparams.NUM_UPARTITIONS + universalparams.NUM_APPLYPARTITIONS){
			// apply and gatherDSTs <===> transport
			action.module = APPLY_UPDATES_MODULE___AND___GATHER_DSTPROPERTIES_MODULE;
			action.start_pv = launch_idx - universalparams.NUM_UPARTITIONS; 
			action.size_pv = 1; 
			action.start_gv = launch_idx - universalparams.NUM_UPARTITIONS; 
			action.size_gv = 1;
			action.status = 0;
		} else {
			cout<<"ERROR 234. launch_idx is out-of-range. EXITING... "<<endl;
			exit(EXIT_FAILURE);
		}
	}
	action.size_import_export = IMPORT_EXPORT_GRANULARITY_VECSIZE;
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

IMPORT_HOST_TO_DEVICE(){
	std::cout << "Host to FPGA Transfer..." << std::endl;
	std::chrono::steady_clock::time_point begin_time2 = std::chrono::steady_clock::now();
	#ifdef FPGA_IMPL
	for(unsigned int t=0; t<64; t++){
	OCL_CHECK(err, err = q->enqueueMigrateMemObjects({buffer_import[flag]}, 0));
	OCL_CHECK(err, err = q->finish());
	}
	#else 
		
	#endif 
	double end_time2 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time2).count()) / 1000;	
	std::cout <<">>> Host to FPGA Transfer time elapsed : "<<end_time2<<" ms, "<<(end_time2 * 1000)<<" microsecs, "<<std::endl;
	return;
}
EXPORT_DEVICE_TO_HOST(){
	std::cout << "FPGA to Host Transfer..." << std::endl;
	std::chrono::steady_clock::time_point begin_time2 = std::chrono::steady_clock::now();
	#ifdef FPGA_IMPL
	for(unsigned int t=0; t<64; t++){
	OCL_CHECK(err, err = q->enqueueMigrateMemObjects({buffer_export[flag]}, CL_MIGRATE_MEM_OBJECT_HOST));
	OCL_CHECK(err, err = q->finish());
	}
	#else 
		
	#endif 
	double end_time2 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time2).count()) / 1000;	
	std::cout <<">>> FPGA to Host Transfer time elapsed : "<<end_time2<<" ms, "<<(end_time2 * 1000)<<" microsecs, "<<std::endl;
	return;
}

long double host::runapp(action_t action__, std::string binaryFile__[2], HBM_channelAXISW_t * HBM_axichannel[2][NUM_PEs], HBM_channelAXISW_t * HBM_axicenter[2], unsigned int hbm_channel_wwsize, unsigned int globalparams[1024], universalparams_t universalparams,
	vector<edge3_type> (&final_edge_updates)[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS]){					
	unsigned int ARRAY_SIZE = hbm_channel_wwsize * HBM_AXI_PACK_SIZE; // REMOVEME.
	unsigned int IO_ARRAY_SIZE = IMPORT_EXPORT_GRANULARITY_VECSIZE * HBM_AXI_PACK_SIZE; // REMOVEME.
	
	cout<<"host::runapp_sync: NUM_HBM_ARGS: "<<NUM_HBM_ARGS<<" ---"<<endl;
	cout<<"host::runapp_sync: ARRAY_SIZE: "<<ARRAY_SIZE<<" ---"<<endl;
	cout<<"host::runapp_sync: IO_ARRAY_SIZE: "<<IO_ARRAY_SIZE<<" ---"<<endl;
	cout<<"host::runapp_sync: TOTAL: "<<ARRAY_SIZE + IO_ARRAY_SIZE<<" ---"<<endl;
	
	HBM_channelAXISW_t * vertex_properties[MAX_NUM_UPARTITIONS];
	unsigned int * vertex_properties_metadata___iteration = new unsigned int[MAX_NUM_UPARTITIONS];
	for(unsigned int p=0; p<universalparams.NUM_UPARTITIONS + 4; p++){ vertex_properties[p] = new HBM_channelAXISW_t[8192]; }
	for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ vertex_properties_metadata___iteration[p] = 0; }
	
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
		#ifdef ___SYNC___
		OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &err)); ////////// FIXME.
		#else 
		OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err));	
		#endif 

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
    size_t bytes_per_iteration = ARRAY_SIZE * sizeof(int); // elements_per_iteration * sizeof(int);
	size_t import_export_bytes_per_iteration = IO_ARRAY_SIZE * sizeof(int);
	cout<<"--- host::runapp_sync: bytes_per_iteration: "<<bytes_per_iteration<<" ---"<<endl;

	#ifdef FPGA_IMPL
	std::vector<int, aligned_allocator<int> > HHX[32]; for(unsigned int i=0; i<NUM_PEs*2; i++){ HHX[i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }
	std::vector<int, aligned_allocator<int> > HHC[2][2]; for(unsigned int flag=0; flag<2; flag++){ for(unsigned int i=0; i<2; i++){ HHC[flag][i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }}
	std::vector<int, aligned_allocator<int> > HBM_import[NUM_IMPORT_BUFFERS]; for(unsigned int i=0; i<NUM_IMPORT_BUFFERS; i++){ HBM_import[i] = std::vector<int, aligned_allocator<int> >(IO_ARRAY_SIZE); }
	std::vector<int, aligned_allocator<int> > HBM_export[NUM_EXPORT_BUFFERS]; for(unsigned int i=0; i<NUM_EXPORT_BUFFERS; i++){ HBM_export[i] = std::vector<int, aligned_allocator<int> >(IO_ARRAY_SIZE); }
	for(unsigned int i=0; i<NUM_PEs; i++){ // REMOVEME.
		for(unsigned int t=0; t<hbm_channel_wwsize; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HHX[2*i][t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[0][i][t].data[v]; }
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HHX[2*i+1][t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[1][i][t].data[v]; }
		}
	}
	for(unsigned int flag=0; flag<2; flag++){ 
		for(unsigned int t=0; t<hbm_channel_wwsize; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HHC[flag][0][t*HBM_AXI_PACK_SIZE + v] = HBM_axicenter[0][t].data[v]; }
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HHC[flag][1][t*HBM_AXI_PACK_SIZE + v] = HBM_axicenter[1][t].data[v]; }
		}
	}
	// checkpoint_t * HBM_import_chkpt = new checkpoint_t[MAX_NUM_UPARTITIONS];
	// checkpoint_t * HBM_export_chkpt = new checkpoint_t[MAX_NUM_UPARTITIONS];
	#else 
	HBM_channelAXISW_t * HBM_import[MAX_NUM_UPARTITIONS]; 
	HBM_channelAXISW_t * HBM_export[MAX_NUM_UPARTITIONS]; 
	for(unsigned int n=0; n<MAX_NUM_UPARTITIONS; n++){
		HBM_import[n] = new HBM_channelAXISW_t[IMPORT_EXPORT_GRANULARITY_VECSIZE]; 
		HBM_export[n] = new HBM_channelAXISW_t[IMPORT_EXPORT_GRANULARITY_VECSIZE]; 
	}
	// checkpoint_t * HBM_import_chkpt = new checkpoint_t[MAX_NUM_UPARTITIONS];
	// checkpoint_t * HBM_export_chkpt = new checkpoint_t[MAX_NUM_UPARTITIONS];
	#endif 
	
	checkpoint_t * HBM_import_chkpt = new checkpoint_t[MAX_NUM_UPARTITIONS];
	checkpoint_t * HBM_export_chkpt = new checkpoint_t[MAX_NUM_UPARTITIONS];
	for(unsigned int t=0; t<MAX_NUM_UPARTITIONS; t++){
		HBM_import_chkpt[t].msg = 1; HBM_import_chkpt[t].graph_iteration = 0;
		HBM_export_chkpt[t].msg = 0; HBM_export_chkpt[t].graph_iteration = 0;
	}	
	HBM_import_chkpt[0].ptr = 0;//3;
	HBM_export_chkpt[0].ptr = 0; 
	
	#ifdef FPGA_IMPL
	std::vector<int, aligned_allocator<int> > HBM_import_chkpt_vec(MAX_NUM_UPARTITIONS * 3); 
	std::vector<int, aligned_allocator<int> > HBM_export_chkpt_vec(MAX_NUM_UPARTITIONS * 3); 
	#endif 
	
    // THIS PAIR OF EVENTS WILL BE USED TO TRACK WHEN A KERNEL IS FINISHED WITH
    // THE INPUT BUFFERS. ONCE THE KERNEL IS FINISHED PROCESSING THE DATA, A NEW
    // SET OF ELEMENTS WILL BE WRITTEN INTO THE BUFFER.
	#ifdef FPGA_IMPL
    vector<cl::Event> kernel_events(2);
    vector<cl::Event> read_events(2);
	
	std::vector<cl::Buffer> buffer_hbm(32);
	std::vector<cl::Buffer> buffer_hbmc(2*2);
	std::vector<cl::Buffer> buffer_import(NUM_IMPORT_BUFFERS);
	std::vector<cl::Buffer> buffer_export(NUM_EXPORT_BUFFERS);
	std::vector<cl::Buffer> buffer_import_chkpt(1);
	std::vector<cl::Buffer> buffer_export_chkpt(1);
	std::vector<cl_mem_ext_ptr_t> inBufExt(32);
	std::vector<cl_mem_ext_ptr_t> inBufExt_c(2);
	std::vector<cl_mem_ext_ptr_t> inBufExt_import(NUM_IMPORT_BUFFERS); // REMOVEME.
	std::vector<cl_mem_ext_ptr_t> inBufExt_export(NUM_EXPORT_BUFFERS); // REMOVEME.
	std::vector<cl_mem_ext_ptr_t> inBufExt_import_chkpt(1);
	std::vector<cl_mem_ext_ptr_t> inBufExt_export_chkpt(1);
	
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
	
	// for (int i = 0; i < NUM_IMPORT_BUFFERS; i++) { // REMOVEME.
		// inBufExt_import[i].obj = HBM_import[i].data();
		// inBufExt_import[i].param = 0;
		// inBufExt_import[i].flags = pc[4];
	// }
	// for (int i = 0; i < NUM_EXPORT_BUFFERS; i++) { // REMOVEME.
		// inBufExt_export[i].obj = HBM_export[i].data();
		// inBufExt_export[i].param = 0;
		// inBufExt_export[i].flags = pc[4];
	// }
	
	inBufExt_import_chkpt[0].obj = HBM_import_chkpt_vec.data(); // &HBM_import_chkpt[0];
	inBufExt_import_chkpt[0].param = 0;
	inBufExt_import_chkpt[0].flags = pc[0];
	
	inBufExt_export_chkpt[0].obj = HBM_export_chkpt_vec.data(); // &HBM_export_chkpt[0];
	inBufExt_export_chkpt[0].param = 0;
	inBufExt_export_chkpt[0].flags = pc[0];
	#endif 
	
	// Allocate Buffer in Global Memory
	// Buffers are allocated using CL_MEM_USE_HOST_PTR for efficient memory and
	// Device-to-host communication
	#ifdef FPGA_IMPL
	std::cout << "Creating Buffers..." << std::endl;
	for (int i = 0; i < NUM_HBM_ARGS; i++) {
		std::cout << "Creating Buffer "<<i<<"..." << std::endl;
		OCL_CHECK(err, buffer_hbm[i] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt[i], &err));
	}
	
	std::cout << "Creating Center Buffers..." << std::endl;
	for (int i = 0; i < NUM_HBMC_ARGS; i++) {
		std::cout << "Creating Center Buffer "<<i<<"..." << std::endl;
		OCL_CHECK(err, buffer_hbmc[i] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt_c[i], &err)); 
	}
	
	OCL_CHECK(err, buffer_import_chkpt[0] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										MAX_NUM_UPARTITIONS * sizeof(checkpoint_t), &inBufExt_import_chkpt[0], &err)); 
	OCL_CHECK(err, buffer_export_chkpt[0] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										MAX_NUM_UPARTITIONS * sizeof(checkpoint_t), &inBufExt_export_chkpt[0], &err)); 
	#endif
	
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
	OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + 0, buffer_import_chkpt[0]));
	OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + 1, buffer_export_chkpt[0]));
	#endif 
	
	// Copy input data to device global memory
	#ifdef FPGA_IMPL
	_migrate_host_to_device(&q, err, buffer_hbm, buffer_hbmc);
	#else 
	_migrate_host_to_device();	
	#endif 
	
	// run acts
	action_t action;
	action.module = ALL_MODULES;
	action.graph_iteration = 0;
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
	action.status = 1;
	
	// unsigned int launch_type = 0; // 0:full run,1:segmented runs 
	// unsigned int num_launches = 1;
	
	unsigned int launch_type = 1; // 0:full run,1:segmented runs 
	unsigned int num_launches = universalparams.NUM_UPARTITIONS + universalparams.NUM_APPLYPARTITIONS;
	
	#ifndef FPGA_IMPL
	acts_kernel * acts = new acts_kernel(universalparams);
	#endif 
	
	// run kernel
	// unsigned int partition_exported = 0;
	std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();
	for (unsigned int iteration_idx = 0; iteration_idx < 1; iteration_idx++) {
		for(unsigned int launch_idx=0; launch_idx<num_launches; launch_idx++){
			std::cout << endl << TIMINGRESULTSCOLOR <<"-------------------------------- host: GAS iteration: "<<iteration_idx<<", launch_idx "<<launch_idx<<" started... --------------------------------"<< RESET << std::endl;
			
			// int flag = launch_idx % 2;
			int flag = ((iteration_idx * num_launches) + launch_idx) % 2;
			
			// set scalar arguments
			std::cout << "Setting Scalar Arguments..." << std::endl;
			action_t action = _get_action(launch_idx, launch_type, universalparams);
			action.graph_iteration = iteration_idx;
			action.id_import = launch_idx; // FIXME.
			action.id_export = launch_idx; // FIXME
			action.size_import_export = IMPORT_EXPORT_GRANULARITY_VECSIZE; // 
			
			#ifdef FPGA_IMPL	
			if (launch_idx >= 2) {
				OCL_CHECK(err, err = read_events[flag].wait());
			}
			#endif 	
			
			// Allocate Buffer in Global Memory
			// Buffers are allocated using CL_MEM_USE_HOST_PTR for efficient memory and
			// Device-to-host communication
			#ifdef FPGA_IMPL	
			std::cout << "Creating Import Buffers..." << std::endl;
			OCL_CHECK(err, buffer_import[flag] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
											import_export_bytes_per_iteration, &HBM_import[action.id_import][0], &err)); // REMOVEME 'i%6'
		
			std::cout << "Creating Export Buffers..." << std::endl;
			OCL_CHECK(err, buffer_export[flag] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
											import_export_bytes_per_iteration, &HBM_export[action.id_export][0], &err)); // REMOVEME 'i%6'
			#endif 	
			
			#ifdef FPGA_IMPL
			vector<cl::Event> write_event(1);
			#endif 
											   
			std::cout << "Setting Import/Export Arguments..." << std::endl;
			#ifdef FPGA_IMPL
			OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS, buffer_import[flag]));
			OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 1, buffer_export[flag]));
			#endif 
		
			// set scalar arguments
			// std::cout << "Setting Scalar Arguments..." << std::endl;
			// action_t action = _get_action(launch_idx, launch_type, universalparams);
			// action.graph_iteration = iteration_idx;
			// action.id_import = launch_idx; // FIXME.
			// action.id_export = launch_idx; // FIXME
			// action.size_import_export = IMPORT_EXPORT_GRANULARITY_VECSIZE; // 
			
			// set scalar arguments
			#ifdef FPGA_IMPL
			_set_args___actions(&krnl_vadd, action, err);
			#endif 
			
			// import 
			#ifdef FPGA_IMPL
			// _import_host_to_device(&q, err, buffer_import, flag);
			std::cout << "Host to FPGA Transfer..." << std::endl;
			std::chrono::steady_clock::time_point begin_time2 = std::chrono::steady_clock::now();
			for(unsigned int t=0; t<1; t++){
				OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_import[flag]}, 0, nullptr, &write_event[0]));
				set_callback(write_event[0], "ooo_queue");
				#ifdef ___SYNC___
				OCL_CHECK(err, err = write_event[0].wait()); ////////////////////////////////////
				#endif 
			}
			double end_time2 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time2).count()) / 1000;	
			std::cout << TIMINGRESULTSCOLOR <<">>> Host to FPGA Transfer time elapsed : "<<end_time2<<" ms, "<<(end_time2 * 1000)<<" microsecs, "<< RESET << std::endl;		
			#endif
			
			// run kernel 
			printf("Enqueueing NDRange kernel.\n");
			std::chrono::steady_clock::time_point begin_time1 = std::chrono::steady_clock::now();
			cout<<"$$$ host: running acts... [import target: partition "<<action.id_import<<"][export target: partition "<<action.id_export<<"] "<<endl;
			#ifdef FPGA_IMPL
				#ifdef ___SYNC___
				OCL_CHECK(err, err = q.enqueueNDRangeKernel(krnl_vadd, 0, 1, 1, NULL, &kernel_events[flag]));
				set_callback(kernel_events[flag], "ooo_queue");
				OCL_CHECK(err, err = kernel_events[flag].wait()); ////////////////////////////////////
				#else 
				std::vector<cl::Event> waitList;
				waitList.push_back(write_event[0]);
				OCL_CHECK(err, err = q.enqueueNDRangeKernel(krnl_vadd, 0, 1, 1, &waitList, &kernel_events[flag]));
				set_callback(kernel_events[flag], "ooo_queue");
				#endif 			
			#else 
			for(unsigned int n=0; n<NUM_FPGAS; n++){
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
					,(HBM_channelAXI_t *)HBM_import[action.id_import], (HBM_channelAXI_t *)HBM_export[action.id_export]
					,HBM_import_chkpt ,HBM_export_chkpt
					,action.module ,action.graph_iteration ,action.start_pu ,action.size_pu ,action.start_pv ,action.size_pv ,action.start_llpset ,action.size_llpset ,action.start_llpid ,action.size_llpid ,action.start_gv ,action.size_gv ,action.id_import ,action.id_export ,action.size_import_export ,action.status ,final_edge_updates				
					);		
			}
			#endif 
			double end_time1 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time1).count()) / 1000;	
			std::cout << TIMINGRESULTSCOLOR << ">>> kernel time elapsed for iteration "<<iteration_idx<<", launch_idx "<<launch_idx<<" : "<<end_time1<<" ms, "<<(end_time1 * 1000)<<" microsecs, "<< RESET <<std::endl;
			
			// export
			#ifdef FPGA_IMPL
			// _export_device_to_host(&q, err, buffer_export, flag);
			std::cout << "FPGA to Host Transfer..." << std::endl;
			std::chrono::steady_clock::time_point begin_time3 = std::chrono::steady_clock::now();
			std::cout << "Getting Results (Device to Host)..." << std::endl;
			std::vector<cl::Event> eventList;
			eventList.push_back(kernel_events[flag]);
			for(unsigned int t=0; t<1; t++){
				OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_export[flag]}, CL_MIGRATE_MEM_OBJECT_HOST, &eventList,
                                                        &read_events[flag]));
				set_callback(read_events[flag], "ooo_queue");
				#ifdef ___SYNC___
				OCL_CHECK(err, err = read_events[flag].wait()); ////////////////////////////////////
				#endif 
			}
			double end_time3 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time3).count()) / 1000;	
			std::cout << TIMINGRESULTSCOLOR << ">>> FPGA to Host Transfer time elapsed : "<<end_time3<<" ms, "<<(end_time3 * 1000)<<" microsecs, "<< RESET << std::endl;
			#endif
		}
    }
	
	// for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS + 8; t++){
		// cout<<"host: HBM_export_chkpt["<<t<<"].msg: "<<HBM_export_chkpt[t].msg<<endl;
	// }	
	
	double end_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time).count()) / 1000;	
	std::cout << TIMINGRESULTSCOLOR <<">>> total kernel time elapsed for all iterations : "<<end_time<<" ms, "<<(end_time * 1000)<<" microsecs, "<< RESET << std::endl;
	
	// Wait for all of the OpenCL operations to complete
    printf("Waiting...\n");
	#ifdef FPGA_IMPL
    OCL_CHECK(err, err = q.flush());
    OCL_CHECK(err, err = q.finish());
	#endif 
	
	// double end_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time).count()) / 1000;	
	// std::cout << TIMINGRESULTSCOLOR <<">>> total kernel time elapsed for all iterations : "<<end_time<<" ms, "<<(end_time * 1000)<<" microsecs, "<< RESET << std::endl;

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

