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
  device or between two operations in the same contexts. You can also use events
  to time a particular operation if the command queue was created using the
  CL_QUEUE_PROFILING_ENABLE flag.

  Most enqueuing commands return events by accepting a cl_event pointer as their
  last argument of the call. These events can be queried using the
  clGetEventInfo function to get the status of a particular operation.

  Many functions also accept event lists that can be used to enforce ordering in
  an OpenCL contexts. These events lists are especially important in the contexts
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
#define NUM_HBMIO_CHKPTS_ARGS 0//2

// #if NUM_FPGAS>1
#define ___PRE_RUN___
#define ___POST_RUN___
// #endif 

// #define NUM_UPARTITIONS_GATHERERD_PER_LAUNCH  1

// #define ___SYNC___

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

void _set_args___actions(cl::Kernel * kernels, action_t action, cl_int err){
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS, int(action.fpga)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 1, int(action.module)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 2, int(action.graph_iteration)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 3, int(action.start_pu)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 4, int(action.size_pu)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 5, int(action.skip_pu)));	
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 6, int(action.start_pv_fpga)));	
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 7, int(action.start_pv)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 8, int(action.size_pv)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 9, int(action.start_llpset)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 10, int(action.size_llpset)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 11, int(action.start_llpid)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 12, int(action.size_llpid)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 13, int(action.start_gv)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 14, int(action.size_gv)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 15, int(action.id_process)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 16, int(action.id_import)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 17, int(action.id_export)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 18, int(action.size_import_export)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 19, int(action.status)));
}
#endif 

#ifdef FPGA_IMPL
#define MIGRATE_HOST_TO_DEVICE() void _migrate_host_to_device(cl::CommandQueue * q, cl_int err, unsigned int fpga, std::vector<cl::Buffer> &buffer_hbm, std::vector<cl::Buffer> &buffer_hbmc)
#define MIGRATE_DEVICE_TO_HOST() void _migrate_device_to_host(cl::CommandQueue * q, cl_int err, unsigned int fpga, std::vector<cl::Buffer> &buffer_hbm, std::vector<cl::Buffer> &buffer_hbmc)
#define IMPORT_HOST_TO_DEVICE() void _import_host_to_device(cl::CommandQueue * q, cl_int err, unsigned int fpga, std::vector<cl::Buffer> &buffer_import, unsigned int flag)
#define EXPORT_DEVICE_TO_HOST() void _export_device_to_host(cl::CommandQueue * q, cl_int err, unsigned int fpga, std::vector<cl::Buffer> &buffer_export, unsigned int flag)
#else
#define MIGRATE_HOST_TO_DEVICE() void _migrate_host_to_device()
#define MIGRATE_DEVICE_TO_HOST() void _migrate_device_to_host()
#define IMPORT_HOST_TO_DEVICE() void _import_host_to_device()
#define EXPORT_DEVICE_TO_HOST() void _export_device_to_host()
#endif

void load_actions1(unsigned int launch_type, unsigned int fpga, action_t * actions[NUM_FPGAS], universalparams_t universalparams){
	action_t action;
	
	action.module = ALL_MODULES;
	action.graph_iteration = NAp;
	
	action.start_pu = fpga; 
	action.size_pu = universalparams.NUM_UPARTITIONS; 
	action.skip_pu = NUM_FPGAS; 
	
	action.start_pv_fpga = 0;
	action.start_pv = 0;
	action.size_pv = universalparams.NUM_APPLYPARTITIONS; 
	
	action.start_gv = 0; 
	action.size_gv = universalparams.NUM_UPARTITIONS; // NUM_VALID_PEs;
	
	action.start_llpset = 0; 
	action.size_llpset = universalparams.NUM_APPLYPARTITIONS; 
	action.start_llpid = 0; 
	action.size_llpid = EDGE_PACK_SIZE; 
	
	action.id_process = 0;
	action.id_import = 0;
	action.id_export = 0;
	action.size_import_export = IMPORT_EXPORT_GRANULARITY_VECSIZE;
	action.status = 1;
	
	actions[fpga][0] = action;
	return;
}
void load_actions2(unsigned int launch_type, unsigned int fpga, action_t * actions[NUM_FPGAS], unsigned int num_upartitions_gathered_per_launch, universalparams_t universalparams){
	unsigned int index = 0;
	
	for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t++){
		action_t action;
		
		action.module = PROCESS_EDGES_MODULE;
		action.graph_iteration = NAp;
		
		action.start_pu = t;
		action.size_pu = 1; 
		action.skip_pu = 1;
		
		action.start_pv_fpga = NAp;
		action.start_pv = NAp;
		action.size_pv = NAp; 
		
		action.start_gv = NAp; 
		action.size_gv = NAp;
		
		action.start_llpset = NAp; 
		action.size_llpset = NAp; 
		action.start_llpid = NAp; 
		action.size_llpid = NAp; 
		
		action.id_process = INVALID_IOBUFFER_ID;
		action.id_import = INVALID_IOBUFFER_ID;
		action.id_export = INVALID_IOBUFFER_ID;
		action.size_import_export = IMPORT_EXPORT_GRANULARITY_VECSIZE;
		action.status = 0;
		
		actions[fpga][index] = action;
		index += 1;
	}
	
	for(unsigned int mmm_fpga=0; mmm_fpga<NUM_FPGAS; mmm_fpga++){
		for(unsigned int t=0; t<universalparams.NUM_APPLYPARTITIONS; t++){ //  * NUM_FPGAS
			action_t action;
			
			action.module = APPLY_UPDATES_MODULE;
			action.graph_iteration = NAp;
			
			action.start_pu = NAp; 
			action.size_pu = NAp; 
			action.skip_pu = NAp;
			
			action.start_pv_fpga = mmm_fpga;
			action.start_pv = t; 
			action.size_pv = 1; 
			
			action.start_gv = NAp; 
			action.size_gv = NAp;
			
			action.start_llpset = NAp; 
			action.size_llpset = NAp; 
			action.start_llpid = NAp; 
			action.size_llpid = NAp;  
			
			action.id_process = INVALID_IOBUFFER_ID;
			action.id_import = INVALID_IOBUFFER_ID;
			action.id_export = INVALID_IOBUFFER_ID;
			action.size_import_export = IMPORT_EXPORT_GRANULARITY_VECSIZE;
			action.status = 0;
			
			actions[fpga][index] = action;
			index += 1;
		}
	}
	
	for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / num_upartitions_gathered_per_launch; t++){
		action_t action;
		
		action.module = GATHER_FRONTIERS_MODULE;
		action.graph_iteration = NAp;
		
		action.start_pu = NAp; 
		action.size_pu = NAp; 
		action.skip_pu = NAp;
		
		action.start_pv_fpga = NAp;
		action.start_pv = NAp; 
		action.size_pv = NAp;
		
		action.start_gv = t * num_upartitions_gathered_per_launch; 
		action.size_gv = num_upartitions_gathered_per_launch;
		
		action.start_llpset = NAp; 
		action.size_llpset = NAp; 
		action.start_llpid = NAp; 
		action.size_llpid = NAp;  
		
		action.id_process = INVALID_IOBUFFER_ID;
		action.id_import = INVALID_IOBUFFER_ID;
		action.id_export = INVALID_IOBUFFER_ID;
		action.size_import_export = IMPORT_EXPORT_GRANULARITY_VECSIZE;
		action.status = 0;
		
		actions[fpga][index] = action;
		index += 1;
	}
	return;
}

MIGRATE_HOST_TO_DEVICE(){
	std::cout << "Copying data (Host to Device)..." << std::endl;
	std::chrono::steady_clock::time_point begin_time0 = std::chrono::steady_clock::now();
	for (int i = 0; i < NUM_HBM_ARGS; i++) {
		std::cout << "Copying data @ channel "<<i<<" (Host to Device)..." << std::endl;
		#ifdef FPGA_IMPL
		OCL_CHECK(err, err = q->enqueueMigrateMemObjects({buffer_hbm[i]}, 0));
		#endif 
	}
	for (int i = 0; i < NUM_HBMC_ARGS; i++) {
		std::cout << "Copying data @ center channel "<<i<<" (Host to Device)..." << std::endl;
		#ifdef FPGA_IMPL
		OCL_CHECK(err, err = q->enqueueMigrateMemObjects({buffer_hbmc[i]}, 0));
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

long double host::runapp(std::string binaryFile__[2], HBM_channelAXISW_t * HBM_axichannel[NUM_FPGAS][2][NUM_PEs], HBM_channelAXISW_t * HBM_axicenter[NUM_FPGAS][2], unsigned int hbm_channel_wwsize, unsigned int globalparams[1024], universalparams_t universalparams,
	vector<edge3_type> (&final_edge_updates)[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS]){					
	unsigned int ARRAY_SIZE = hbm_channel_wwsize * HBM_AXI_PACK_SIZE; // REMOVEME.
	unsigned int IO_ARRAY_SIZE = IMPORT_EXPORT_GRANULARITY_VECSIZE * HBM_AXI_PACK_SIZE; // REMOVEME.
	unsigned int num_upartitions_gathered_per_launch = NUM_FPGAS; // 1, NUM_FPGAS, universalparams.NUM_UPARTITIONS
	
	unsigned int report_statistics[64]; for(unsigned int t=0; t<64; t++){ report_statistics[t] = 0; }
	
	cout<<"host::runapp_sync: NUM_HBM_ARGS: "<<NUM_HBM_ARGS<<" ---"<<endl;
	cout<<"host::runapp_sync: ARRAY_SIZE: "<<ARRAY_SIZE<<" ---"<<endl;
	cout<<"host::runapp_sync: IO_ARRAY_SIZE: "<<IO_ARRAY_SIZE<<" ---"<<endl;
	cout<<"host::runapp_sync: TOTAL: "<<ARRAY_SIZE + IO_ARRAY_SIZE<<" ---"<<endl;
	
	// load necessary commands
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for(unsigned int i=0; i<NUM_PEs; i++){ 
			#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___PREPAREEDGEUPDATES].data[0] = 1; //
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___PROCESSEDGEUPDATES].data[0] = 1; //
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___PROCESSEDGES].data[0] = 1; //
			#else 
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___PREPAREEDGEUPDATES].data[0] = 0; //
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___PROCESSEDGEUPDATES].data[0] = 0; //
			HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE___ENABLE___PROCESSEDGES].data[0] = 1; //	
			#endif 
		}
	}
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int i=0; i<NUM_PEs; i++){ HBM_axichannel[fpga][0][i][GLOBALPARAMSCODE__COMMANDS__COMMAND0].data[0] = 2; }}
	
	HBM_channelAXISW_t * vertex_properties[MAX_NUM_UPARTITIONS];
	unsigned int * vertex_properties_metadata___iteration = new unsigned int[MAX_NUM_UPARTITIONS];
	for(unsigned int p=0; p<universalparams.NUM_UPARTITIONS + 4; p++){ vertex_properties[p] = new HBM_channelAXISW_t[8192]; }
	for(unsigned int p=0; p<MAX_NUM_UPARTITIONS; p++){ vertex_properties_metadata___iteration[p] = 0; }
	
	// prepare OCL variables 
	#ifdef FPGA_IMPL
    // auto binaryFile = argv[1];
	std::string binaryFile = binaryFile__[0]; 
    cl_int err;
	
	unsigned int device_count = NUM_FPGAS;
	vector<cl::Context> contexts(device_count);
    vector<cl::Program> programs(device_count);
    vector<cl::Kernel> kernels(device_count);
    vector<cl::CommandQueue> q(device_count);
    vector<std::string> device_name(device_count);

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
    // for (unsigned int d = 0; d < devices.size(); d++) {
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
        auto device = devices[fpga];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, contexts[fpga] = cl::Context(device, nullptr, nullptr, nullptr, &err));
        // This example will use an out of order command queue. The default command
        // queue created by cl::CommandQueue is an inorder command queue.
        // OCL_CHECK(err, q = cl::CommandQueue(contexts[fpga], device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err));
		// OCL_CHECK(err, q = cl::CommandQueue(contexts[fpga], device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &err)); 
		#ifdef ___SYNC___
		OCL_CHECK(err, q[fpga] = cl::CommandQueue(contexts[fpga], device, CL_QUEUE_PROFILING_ENABLE, &err)); ////////// FIXME.
		#else 
		OCL_CHECK(err, q[fpga] = cl::CommandQueue(contexts[fpga], device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err));	
		#endif 

        std::cout << "Trying to program device[" << fpga << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(contexts[fpga], {device}, bins, nullptr, &err);
		programs[fpga] = program;
		// programs[fpga] = load_cl2_binary(bins[fpga], devices[fpga], contexts[fpga]); // NEWCHANGE
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << fpga << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << fpga << "]: program successful!\n";
            OCL_CHECK(err, kernels[fpga] = cl::Kernel(programs[fpga], "top_function", &err));
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

	// allocate sw buffers
	#ifdef FPGA_IMPL
	std::vector<int, aligned_allocator<int> > HBM_axichannel_vector[NUM_FPGAS][32]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int i=0; i<NUM_PEs*2; i++){ HBM_axichannel_vector[fpga][i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }}
	std::vector<int, aligned_allocator<int> > HBM_axicenter_vector[NUM_FPGAS][2][2]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int flag=0; flag<2; flag++){ for(unsigned int i=0; i<2; i++){ HBM_axicenter_vector[fpga][flag][i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }}}
	std::vector<int, aligned_allocator<int> > HBM_import[NUM_FPGAS][NUM_IMPORT_BUFFERS]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int i=0; i<NUM_IMPORT_BUFFERS; i++){ HBM_import[fpga][i] = std::vector<int, aligned_allocator<int> >(IO_ARRAY_SIZE); }}
	std::vector<int, aligned_allocator<int> > HBM_export[NUM_FPGAS][NUM_EXPORT_BUFFERS]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int i=0; i<NUM_EXPORT_BUFFERS; i++){ HBM_export[fpga][i] = std::vector<int, aligned_allocator<int> >(IO_ARRAY_SIZE); }}
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		for(unsigned int i=0; i<NUM_PEs; i++){ // REMOVEME.
			for(unsigned int t=0; t<hbm_channel_wwsize; t++){ 
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_axichannel_vector[fpga][2*i][t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[fpga][0][i][t].data[v]; }
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_axichannel_vector[fpga][2*i+1][t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[fpga][1][i][t].data[v]; }
			}
		}
	}
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		for(unsigned int flag=0; flag<2; flag++){ 
			for(unsigned int t=0; t<hbm_channel_wwsize; t++){ 
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_axicenter_vector[fpga][flag][0][t*HBM_AXI_PACK_SIZE + v] = HBM_axicenter[fpga][0][t].data[v]; }
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_axicenter_vector[fpga][flag][1][t*HBM_AXI_PACK_SIZE + v] = HBM_axicenter[fpga][1][t].data[v]; }
			}
		}
	}
	#else 
	HBM_channelAXISW_t * HBM_import[NUM_FPGAS][NUM_IMPORT_BUFFERS]; 
	HBM_channelAXISW_t * HBM_export[NUM_FPGAS][NUM_EXPORT_BUFFERS]; 
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		for(unsigned int n=0; n<NUM_IMPORT_BUFFERS; n++){
			HBM_import[fpga][n] = new HBM_channelAXISW_t[IMPORT_EXPORT_GRANULARITY_VECSIZE]; 
			HBM_export[fpga][n] = new HBM_channelAXISW_t[IMPORT_EXPORT_GRANULARITY_VECSIZE]; 
		}
	}
	#endif 
	
    // THIS PAIR OF EVENTS WILL BE USED TO TRACK WHEN A KERNEL IS FINISHED WITH
    // THE INPUT BUFFERS. ONCE THE KERNEL IS FINISHED PROCESSING THE DATA, A NEW
    // SET OF ELEMENTS WILL BE WRITTEN INTO THE BUFFER.
	#ifdef FPGA_IMPL
    vector<cl::Event> kernel_events(2 * NUM_FPGAS);
    vector<cl::Event> read_events(2 * NUM_FPGAS);
	
	std::vector<cl::Buffer> buffer_hbm(32 * NUM_FPGAS);
	std::vector<cl::Buffer> buffer_hbmc(NUM_HBMC_ARGS * NUM_FPGAS);
	// std::vector<cl::Buffer> buffer_import(NUM_IMPORT_BUFFERS * NUM_FPGAS);
	// std::vector<cl::Buffer> buffer_export(NUM_EXPORT_BUFFERS * NUM_FPGAS);
	std::vector<cl::Buffer> buffer_import(2 * NUM_FPGAS);
	std::vector<cl::Buffer> buffer_export(2 * NUM_FPGAS);
	std::vector<cl::Buffer> buffer_import_chkpt(1 * NUM_FPGAS);
	std::vector<cl::Buffer> buffer_export_chkpt(1 * NUM_FPGAS);
	std::vector<cl_mem_ext_ptr_t> inBufExt(32 * NUM_FPGAS);
	std::vector<cl_mem_ext_ptr_t> inBufExt_c(2 * NUM_FPGAS);
	// std::vector<cl_mem_ext_ptr_t> inBufExt_import(NUM_IMPORT_BUFFERS * NUM_FPGAS); // REMOVEME.
	// std::vector<cl_mem_ext_ptr_t> inBufExt_export(NUM_EXPORT_BUFFERS * NUM_FPGAS); // REMOVEME.
	
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for (int i = 0; i < NUM_HBM_ARGS; i++) {
			inBufExt[fpga*NUM_FPGAS + i].obj = HBM_axichannel_vector[fpga][i].data();
			inBufExt[fpga*NUM_FPGAS + i].param = 0;
			inBufExt[fpga*NUM_FPGAS + i].flags = pc[i];
		}
	}
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for (int i = 0; i < NUM_HBMC_ARGS; i++) {
			inBufExt_c[fpga*NUM_FPGAS + i].obj = HBM_axicenter_vector[fpga][0][i].data();
			inBufExt_c[fpga*NUM_FPGAS + i].param = 0;
			inBufExt_c[fpga*NUM_FPGAS + i].flags = pc[NUM_HBM_ARGS + i];
		}
	}
	#endif 
	
	// Allocate Buffer in Global Memory
	// Buffers are allocated using CL_MEM_USE_HOST_PTR for efficient memory and
	// Device-to-host communication
	#ifdef FPGA_IMPL
	std::cout << "Creating Buffers..." << std::endl;
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for (int i = 0; i < NUM_HBM_ARGS; i++) {
			std::cout << "Creating Buffer "<<i<<"..." << std::endl;
			OCL_CHECK(err, buffer_hbm[fpga*NUM_FPGAS + i] = cl::Buffer(contexts[fpga], CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
											bytes_per_iteration, &inBufExt[fpga*NUM_FPGAS + i], &err));
		}
	}
	
	std::cout << "Creating Center Buffers..." << std::endl;
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for (int i = 0; i < NUM_HBMC_ARGS; i++) {
			std::cout << "Creating Center Buffer "<<i<<"..." << std::endl;
			OCL_CHECK(err, buffer_hbmc[fpga*NUM_FPGAS + i] = cl::Buffer(contexts[fpga], CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
											bytes_per_iteration, &inBufExt_c[fpga*NUM_FPGAS + i], &err)); 
		}
	}
	#endif
	
	// Set Kernel Arguments
	#ifdef FPGA_IMPL
	std::cout << "Setting Kernel Arguments..." << std::endl;
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for (int i = 0; i < NUM_HBM_ARGS; i++) {
			std::cout << "Setting the k_vadd Argument for argument "<<i<<"..." << std::endl;
			OCL_CHECK(err, err = kernels[fpga].setArg(i, buffer_hbm[fpga*NUM_FPGAS + i]));
		}
	}
	std::cout << "Setting Kernel Arguments (center HBM)..." << std::endl;
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for (int i = 0; i < NUM_HBMC_ARGS; i++) {
			std::cout << "Setting Kernel Argument for argument "<<NUM_HBM_ARGS + i<<"..." << std::endl;
			OCL_CHECK(err, err = kernels[fpga].setArg(NUM_HBM_ARGS + i, buffer_hbmc[fpga*NUM_FPGAS + i]));
		}
	}
	#endif 
	
	// Copy input data to device global memory
	#ifdef FPGA_IMPL
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ _migrate_host_to_device(&q[fpga], err, fpga, buffer_hbm, buffer_hbmc); }
	#else 
	_migrate_host_to_device();	
	#endif 
	
	// exit(EXIT_SUCCESS);/////////////////
	
	// unsigned int launch_type = 0; // 0:full run,1:segmented runs
	// unsigned int num_launches = 1;
	
	unsigned int launch_type = 1; // 0:full run,1:segmented runs 
	unsigned int num_launches = (universalparams.NUM_UPARTITIONS / NUM_FPGAS) + (universalparams.NUM_APPLYPARTITIONS * NUM_FPGAS);// + (universalparams.NUM_UPARTITIONS / num_upartitions_gathered_per_launch);
	
	#ifndef FPGA_IMPL
	acts_kernel * acts = new acts_kernel(universalparams);
	#endif 
	
	gas_t gas[MAX_NUM_UPARTITIONS]; 
	gas_import_t gas_import[NUM_FPGAS][MAX_NUM_UPARTITIONS]; 
	gas_process_t gas_process[NUM_FPGAS][MAX_NUM_UPARTITIONS]; 
	gas_export_t gas_export[NUM_FPGAS][MAX_NUM_UPARTITIONS]; 
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ 
			// gas_import[fpga][t].ready_for_import = 1;
			// gas_import[fpga][t].iteration = 0;
			// gas_process[fpga][t].ready_for_process = 0;
			// gas_process[fpga][t].iteration = 0;
			// gas_export[fpga][t].ready_for_export = 0;
			// gas_export[fpga][t].iteration = 0;
			
			gas_import[fpga][t].ready_for_import = 0;
			gas_import[fpga][t].iteration = 0;
			gas_process[fpga][t].ready_for_process = 1;
			gas_process[fpga][t].iteration = 0;
			gas_export[fpga][t].ready_for_export = 0;
			gas_export[fpga][t].iteration = 0;
			
			// gas_import[fpga][t].ready_for_import = 1;
			// gas_process[fpga][t].ready_for_process = 1;
			// gas_export[fpga][t].ready_for_export = 1;
		}
	}
	
	mapping_t * mapping[NUM_FPGAS]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ mapping[fpga] = new mapping_t[MAX_NUM_UPARTITIONS]; }
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t+=1){
			if(t % NUM_FPGAS != fpga){ 
				mapping[fpga][t].fpga = t % NUM_FPGAS;
				mapping[fpga][t].io_id = t / NUM_FPGAS;				
				if(t<16){ cout<<"$$$ host: [fpga: "<<fpga<<", io_id: "<<t<<"] mapped to [fpga: "<<mapping[fpga][t].fpga<<", io_id: "<<mapping[fpga][t].io_id<<"]"<<endl; }
			} else {
				mapping[fpga][t].fpga = INVALIDDATA;
				mapping[fpga][t].io_id = INVALIDDATA; 
				if(t<16){ cout<<"$$$ host: [fpga: "<<fpga<<", io_id: "<<t<<"] mapped to [fpga: -, io_id: -]"<<endl; }
			}
			// mapping[fpga][t].data_transferred = false;
		}
	}
	
	mapping_t * HBM_xxx_ptr[NUM_FPGAS]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ HBM_xxx_ptr[fpga] = new mapping_t[NUM_IMPORT_BUFFERS]; }
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int t=0; t<NUM_IMPORT_BUFFERS; t+=1){ HBM_xxx_ptr[fpga][t].fpga = 0; HBM_xxx_ptr[fpga][t].io_id = 0; }}
	// exit(EXIT_SUCCESS);
	
	action_t * actions[NUM_FPGAS]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ actions[fpga] = new action_t[1024]; }
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		if(launch_type == 0){ load_actions1(launch_type, fpga, actions, universalparams); }
		else { load_actions2(launch_type, fpga, actions, num_upartitions_gathered_per_launch, universalparams); }
	}
	
	// run kernel
	std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();
	for (unsigned int iteration_idx = 0; iteration_idx < 1; iteration_idx++) {
		std::cout << endl << TIMINGRESULTSCOLOR <<"#################################################################### GAS iteration: "<<iteration_idx<< " ####################################################################"<< RESET << std::endl;
		for(unsigned int launch_idx=0; launch_idx<num_launches; launch_idx+=1){
			std::cout << endl << TIMINGRESULTSCOLOR <<"-------------------------------- host: GAS iteration: "<<iteration_idx<<", launch_idx "<<launch_idx<<", fpgas [0 - "<<NUM_FPGAS-1<<"] started... --------------------------------"<< RESET << std::endl; 
			
			action_t action[NUM_FPGAS];
			unsigned int import_pointer[NUM_FPGAS];
			unsigned int process_pointer[NUM_FPGAS];
			unsigned int export_pointer[NUM_FPGAS];
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ action[fpga] = actions[fpga][launch_idx]; }
			
			int flag = ((iteration_idx * num_launches) + launch_idx) % 2;
			
			// set scalar arguments
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				action[fpga].graph_iteration = iteration_idx;
				action[fpga].id_import = 0; 
				action[fpga].id_export = 0; 
				action[fpga].id_process = launch_idx;
				action[fpga].size_import_export = IMPORT_EXPORT_GRANULARITY_VECSIZE; // 
			}
			
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ std::cout<<"imports @ fpga "<<fpga<<": "; for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t++){ std::cout<<gas_import[fpga][t].ready_for_import<<", "; } cout<<endl; }
			
			// pre-run
			#ifdef ___PRE_RUN___
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				import_pointer[fpga] = INVALID_IOBUFFER_ID;
				action[fpga].id_import = import_pointer[fpga];
				for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t++){
					if(gas_import[fpga][t].ready_for_import == 1){ // && gas_import[fpga][t].iteration == iteration_idx){ ///////////////////
						import_pointer[fpga] = t;
						action[fpga].id_import = import_pointer[fpga]; 
						break; 
					}
				}
				
				process_pointer[fpga] = INVALID_IOBUFFER_ID;
				action[fpga].id_process = process_pointer[fpga];
				if(action[fpga].module != APPLY_UPDATES_MODULE && action[fpga].module != GATHER_FRONTIERS_MODULE){
					for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t++){
						if(gas_process[fpga][t].ready_for_process == 1){ // && gas_process[fpga][t].iteration == iteration_idx){ 	//////////////////////
							process_pointer[fpga] = t;
							action[fpga].id_process = process_pointer[fpga]; 
							action[fpga].start_pu = process_pointer[fpga]; ////////////
							break; 
						}
					}
				}
				
				export_pointer[fpga] = INVALID_IOBUFFER_ID;//88;
				action[fpga].id_export = export_pointer[fpga];
				for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){
					if(mapping[fpga][t].fpga != INVALIDDATA){
						if(gas_export[fpga][t].ready_for_export == 1){
							export_pointer[fpga] = t;
							action[fpga].id_export = export_pointer[fpga]; 
							break; 
						}
					}
				}
			}
			#endif 
			
			#ifdef FPGA_IMPL	
			if (launch_idx >= 2) {
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
					OCL_CHECK(err, err = read_events[2*fpga + flag].wait());
				}
			}
			#endif 	
			
			// Allocate Buffer in Global Memory
			// Buffers are allocated using CL_MEM_USE_HOST_PTR for efficient memory and
			// Device-to-host communication
			#ifdef FPGA_IMPL	
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
			std::cout << "Creating Import Buffers @ fpga "<<fpga<<"..." << std::endl;
				OCL_CHECK(err, buffer_import[2*fpga + flag] = cl::Buffer(contexts[fpga], CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
												import_export_bytes_per_iteration, &HBM_import[fpga][action[fpga].id_import][0], &err)); // REMOVEME 'i%6'
			
				std::cout << "Creating Export Buffers..." << std::endl;
				OCL_CHECK(err, buffer_export[2*fpga + flag] = cl::Buffer(contexts[fpga], CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
												import_export_bytes_per_iteration, &HBM_export[fpga][action[fpga].id_export][0], &err)); // REMOVEME 'i%6'
			}
			#endif 	
			
			#ifdef FPGA_IMPL
			vector<cl::Event> write_event(1 * NUM_FPGAS);
			#endif 
			
			#ifdef FPGA_IMPL
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				std::cout << "Setting Import/Export Arguments..." << std::endl;
				OCL_CHECK(err, err = kernels[fpga].setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS, buffer_import[2*fpga + flag]));
				OCL_CHECK(err, err = kernels[fpga].setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 1, buffer_export[2*fpga + flag]));
			}
			#endif 
			
			// set scalar arguments
			#ifdef FPGA_IMPL
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				std::cout << "Setting Scalar Arguments..." << std::endl;
				_set_args___actions(&kernels[fpga], action[fpga], err);
			}
			#endif 
			
			// import 
			#ifdef FPGA_IMPL
			std::cout << "Host to FPGA Transfer..." << std::endl;
			std::chrono::steady_clock::time_point begin_time2 = std::chrono::steady_clock::now();
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				OCL_CHECK(err, err = q[fpga].enqueueMigrateMemObjects({buffer_import[2*fpga + flag]}, 0, nullptr, &write_event[fpga]));
				set_callback(write_event[fpga], "ooo_queue");
				#ifdef ___SYNC___
				OCL_CHECK(err, err = write_event[fpga].wait()); 
				#endif 
			}
			double end_time2 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time2).count()) / 1000;	
			std::cout << TIMINGRESULTSCOLOR <<">>> Host to FPGA Transfer time elapsed : "<<end_time2<<" ms, "<<(end_time2 * 1000)<<" microsecs, "<< RESET << std::endl;		
			#endif
			
			// run kernel 
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				#ifdef FPGA_IMPL
				printf("Enqueueing NDRange kernel.\n");
				cout<<"$$$ host: running acts... [import target: partition "<<action[fpga].id_import<<"][export target: partition "<<action[fpga].id_export<<"] "<<endl;
				#endif 
				// cout<<"$$$ host: running acts.&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.. fpga "<<fpga<<"] "<<endl;
				std::chrono::steady_clock::time_point begin_time1 = std::chrono::steady_clock::now();
				#ifdef FPGA_IMPL
					#ifdef ___SYNC___
					OCL_CHECK(err, err = q[fpga].enqueueNDRangeKernel(kernels[fpga], 0, 1, 1, NULL, &kernel_events[2*fpga + flag]));
					set_callback(kernel_events[2*fpga + flag], "ooo_queue");
					OCL_CHECK(err, err = kernel_events[2*fpga + flag].wait()); ////////////////////////////////////
					#else 
					std::vector<cl::Event> waitList;
					waitList.push_back(write_event[fpga]);
					OCL_CHECK(err, err = q[fpga].enqueueNDRangeKernel(kernels[fpga], 0, 1, 1, &waitList, &kernel_events[2*fpga + flag]));
					set_callback(kernel_events[2*fpga + flag], "ooo_queue");
					#endif 			
				#else 
					acts->top_function(
						(HBM_channelAXI_t *)HBM_axichannel[fpga][0][0], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][0]
						#if NUM_VALID_HBM_CHANNELS>1
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][1], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][1] 
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][2], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][2] 
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][3], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][3] 
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][4], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][4] 
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][5], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][5] 
						#if NUM_VALID_HBM_CHANNELS>6
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][6], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][6] 
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][7], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][7] 
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][8], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][8] 
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][9], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][9] 
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][10], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][10] 
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][11], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][11] 
						#if NUM_VALID_HBM_CHANNELS>12
						,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][12], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][12]
						#endif 
						#endif 
						#endif
						,(HBM_channelAXI_t *)HBM_axicenter[fpga][0], (HBM_channelAXI_t *)HBM_axicenter[fpga][1]
						// ,(HBM_channelAXI_t *)HBM_import[fpga][action[fpga].id_import], (HBM_channelAXI_t *)HBM_export[fpga][action[fpga].id_export]
						,(HBM_channelAXI_t *)HBM_export[HBM_xxx_ptr[fpga][action[fpga].id_import].fpga][HBM_xxx_ptr[fpga][action[fpga].id_import].io_id], (HBM_channelAXI_t *)HBM_export[fpga][action[fpga].id_export]
						,fpga ,action[fpga].module ,action[fpga].graph_iteration ,action[fpga].start_pu ,action[fpga].size_pu ,action[fpga].skip_pu ,action[fpga].start_pv_fpga ,action[fpga].start_pv ,action[fpga].size_pv ,action[fpga].start_llpset ,action[fpga].size_llpset ,action[fpga].start_llpid ,action[fpga].size_llpid ,action[fpga].start_gv ,action[fpga].size_gv ,action[fpga].id_process ,action[fpga].id_import ,action[fpga].id_export ,action[fpga].size_import_export ,action[fpga].status ,final_edge_updates ,report_statistics				
						);		
				#endif 
				double end_time1 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time1).count()) / 1000;	
				#ifdef FPGA_IMPL
				std::cout << TIMINGRESULTSCOLOR << ">>> kernel time elapsed for iteration "<<iteration_idx<<", launch_idx "<<launch_idx<<" : "<<end_time1<<" ms, "<<(end_time1 * 1000)<<" microsecs, "<< RESET <<std::endl;
				#endif 
			}
			
			// export
			#ifdef FPGA_IMPL
			std::cout << "FPGA to Host Transfer..." << std::endl;
			std::chrono::steady_clock::time_point begin_time3 = std::chrono::steady_clock::now();
			std::cout << "Getting Results (Device to Host)..." << std::endl;
			std::vector<cl::Event> eventList;
			eventList.push_back(kernel_events[2*fpga + flag]);
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				OCL_CHECK(err, err = q[fpga].enqueueMigrateMemObjects({buffer_export[2*fpga + flag]}, CL_MIGRATE_MEM_OBJECT_HOST, &eventList,
														&read_events[2*fpga + flag]));			
				set_callback(read_events[2*fpga + flag], "ooo_queue");
				#ifdef ___SYNC___
				OCL_CHECK(err, err = read_events[2*fpga + flag].wait()); 
				#endif 
			}
			double end_time3 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time3).count()) / 1000;	
			std::cout << TIMINGRESULTSCOLOR << ">>> FPGA to Host Transfer time elapsed : "<<end_time3<<" ms, "<<(end_time3 * 1000)<<" microsecs, "<< RESET << std::endl;
			#endif
			
			// post-run
			#ifdef ___POST_RUN___
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				gas_import[fpga][import_pointer[fpga]].ready_for_import = 0; 
				gas_process[fpga][import_pointer[fpga]].ready_for_process = 1;
			}
			
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				gas_process[fpga][process_pointer[fpga]].ready_for_process = 0; 
				if(action[fpga].module == GATHER_FRONTIERS_MODULE){
					for(unsigned int t=0; t<action[fpga].size_gv; t++){
						if(action[fpga].start_gv + t >= universalparams.NUM_UPARTITIONS){ cout<<"host: ERROR 23: (action["<<fpga<<"].start_gv (="<<action[fpga].start_gv<<") + t ("<<t<<") > universalparams.NUM_UPARTITIONS("<<universalparams.NUM_UPARTITIONS<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
						gas_export[fpga][action[fpga].start_gv + t].ready_for_export = 1;
					}
				}
			}
			
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				gas_export[fpga][export_pointer[fpga]].ready_for_export = 0; 
			}
			#endif 
			
			// post run (export->import transfers)
			#ifdef ___POST_RUN___
			for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
				if(action[fpga].id_export != INVALID_IOBUFFER_ID){				
					mapping_t map = mapping[fpga][action[fpga].id_export];
					cout<< TIMINGRESULTSCOLOR << "<-> mapping exported vpartition: from (fpga: "<<fpga<<", id_export: "<<action[fpga].id_export<<") --- to (mapped_fpga: "<<map.fpga<<", id_mapped_import: "<<map.io_id<<")"<< RESET <<endl;
					if(false){ for(unsigned int t=0; t<IMPORT_EXPORT_GRANULARITY_VECSIZE; t++){ HBM_import[map.fpga][map.io_id] = HBM_export[fpga][action[fpga].id_export]; }} // FIXME.
					HBM_xxx_ptr[fpga][action[fpga].id_export] = map;
					gas_import[map.fpga][map.io_id].ready_for_import = 1; 
					gas_import[map.fpga][map.io_id].iteration = iteration_idx + 1;
				}
			}
			#endif 
			// exit(EXIT_SUCCESS);
		}
    
		#ifdef _DEBUGMODE_HOSTPRINTS//4
		for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ std::cout<<">>> imports @ fpga "<<fpga<<": "; for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t++){ std::cout<<gas_import[fpga][t].ready_for_import<<", "; } cout<<endl; }
		#endif 
	}
	double end_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time).count()) / 1000;	
	std::cout << TIMINGRESULTSCOLOR <<">>> total kernel time elapsed for all iterations : "<<end_time<<" ms, "<<(end_time * 1000)<<" microsecs, "<< RESET << std::endl;
	
	// Wait for all of the OpenCL operations to complete
    printf("Waiting...\n");
	#ifdef FPGA_IMPL
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		OCL_CHECK(err, err = q[fpga].flush());
		OCL_CHECK(err, err = q[fpga].finish());
	}
	#endif 
	
	cout<<"[RESETBUFFERSATSTART, READ_FRONTIERS, PROCESSEDGES, READ_DESTS, APPLYUPDATES, COLLECT_FRONTIERS, SAVE_DEST, GATHER_FRONTIERS]"<<endl;																									
	cout<<">>> [";
	cout<<report_statistics[___CODE___RESETBUFFERSATSTART___]<<", ";
	cout<<report_statistics[___CODE___READ_FRONTIER_PROPERTIES___] * EDGE_PACK_SIZE<<", ";
	cout<<report_statistics[___CODE___ECPROCESSEDGES___] * EDGE_PACK_SIZE<<", ";
	cout<<report_statistics[___CODE___READ_DEST_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs<<", ";
	cout<<report_statistics[___CODE___APPLYUPDATES___] * EDGE_PACK_SIZE<<", ";
	cout<<report_statistics[___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs<<", ";
	cout<<report_statistics[___CODE___SAVE_DEST_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs<<", ";
	cout<<report_statistics[___CODE___GATHER_FRONTIERINFOS___] * EDGE_PACK_SIZE * NUM_PEs<<"";
	cout<<"]"<<endl;

	// Copy Result from Device Global Memory to Host Local Memory
	#ifdef FPGA_IMPL
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ _migrate_device_to_host(&q[fpga], err, fpga, buffer_hbm, buffer_hbmc); }
	#else 
	_migrate_device_to_host();	
	#endif 
			
	// Wait for all of the OpenCL operations to complete
	#ifdef FPGA_IMPL
    printf("Waiting...\n");
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		OCL_CHECK(err, err = q[fpga].flush());
		OCL_CHECK(err, err = q[fpga].finish());
	}
	#endif 
    // OPENCL HOST CODE AREA ENDS

	printf("TEST %s\n", "PASSED");
    return EXIT_SUCCESS;
}

