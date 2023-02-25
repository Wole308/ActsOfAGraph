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
#include "host_fpga.h"
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
#define NUM_HBM_ARGS ((NUM_VALID_HBM_CHANNELS * 2) + 2)

host_fpga::host_fpga(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	myuniversalparams = _universalparams;
}
host_fpga::~host_fpga(){} 

// Number of HBM PCs required
#define MAX_HBM_PC_COUNT 32
#define PC_NAME(n) n | XCL_MEM_TOPOLOGY
#ifdef FPGA_IMPL
const int pc[MAX_HBM_PC_COUNT] = {
    PC_NAME(0),  PC_NAME(1),  PC_NAME(2),  PC_NAME(3),  PC_NAME(4),  PC_NAME(5),  PC_NAME(6),  PC_NAME(7),
    PC_NAME(8),  PC_NAME(9),  PC_NAME(10), PC_NAME(11), PC_NAME(12), PC_NAME(13), PC_NAME(14), PC_NAME(15),
    PC_NAME(16), PC_NAME(17), PC_NAME(18), PC_NAME(19), PC_NAME(20), PC_NAME(21), PC_NAME(22), PC_NAME(23),
    PC_NAME(24), PC_NAME(25), PC_NAME(26), PC_NAME(27), PC_NAME(28), PC_NAME(29), PC_NAME(30), PC_NAME(31)};

// const int ARRAY_SIZE = 1 << 14;

int gen_random() {
    static default_random_engine e;
    static uniform_int_distribution<int> dist(0, 100);

    return dist(e);
}

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

long double host_fpga::runapp(action_t action, std::string binaryFile__[2], HBM_channelAXISW_t * HBM_axichannel[2][NUM_PEs], HBM_channelAXISW_t * HBM_axicenter[2], unsigned int globalparams[1024], universalparams_t universalparams){
	unsigned int ARRAY_SIZE = HBM_CHANNEL_SIZE * HBM_AXI_PACK_SIZE;
	
	cout<<"--- NUM_HBM_ARGS: "<<NUM_HBM_ARGS<<" ---"<<endl;
	cout<<"--- ARRAY_SIZE: "<<ARRAY_SIZE<<" ---"<<endl;

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

    // We will break down our problem into multiple iterations. Each iteration
    // will perform computation on a subset of the entire data-set.
    // size_t elements_per_iteration = ARRAY_SIZE; // 2048;
    size_t bytes_per_iteration = ARRAY_SIZE * sizeof(int); // elements_per_iteration * sizeof(int);
	// size_t bytes_per_iteration = 64 * sizeof(int); // 1000000 * sizeof(int); ////////////////////////////////////// REMOVEME.
    size_t num_iterations = 1; 
	unsigned int batch = 0;
	
	// std::vector<int, aligned_allocator<int> > HHX[NUM_PEs*2]; for(unsigned int i=0; i<NUM_PEs*2; i++){ HHX[i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }
	std::vector<int, aligned_allocator<int> > HHX[32]; for(unsigned int i=0; i<NUM_PEs*2; i++){ HHX[i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }
	std::vector<int, aligned_allocator<int> > HHC[2]; for(unsigned int i=0; i<2; i++){ HHC[i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				HHX[2*i][t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[0][i][t].data[v];
			}
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				HHX[2*i+1][t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[1][i][t].data[v];
			}
		}
	}
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			HHC[0][t*HBM_AXI_PACK_SIZE + v] = HBM_axicenter[0][t].data[v];
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			HHC[1][t*HBM_AXI_PACK_SIZE + v] = HBM_axicenter[1][t].data[v];
		}
	}
	vector<int, aligned_allocator<int> > device_result(ARRAY_SIZE);

    // THIS PAIR OF EVENTS WILL BE USED TO TRACK WHEN A KERNEL IS FINISHED WITH
    // THE INPUT BUFFERS. ONCE THE KERNEL IS FINISHED PROCESSING THE DATA, A NEW
    // SET OF ELEMENTS WILL BE WRITTEN INTO THE BUFFER.
    vector<cl::Event> kernel_events(2);
    vector<cl::Event> read_events(2);

	std::vector<cl::Buffer> buffer_hbm(32);
	std::vector<cl_mem_ext_ptr_t> inBufExt(32);
	for (int i = 0; i < NUM_HBM_ARGS; i++) {
        inBufExt[i].obj = HHX[i].data();
        inBufExt[i].param = 0;
        inBufExt[i].flags = pc[i];
    }
	
	#ifdef HOST_PRINT_RESULTS_XXXX
	cout<<"---------------------------------------------- host_fpga:: before ---------------------------------------------- "<<endl;
	unsigned int base_offsetttt__ = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATES] * HBM_AXI_PACK_SIZE;
	for(unsigned int i=0; i<1; i++){
		for(unsigned int t=0; t<4; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				cout<<HHX[2*i+1][base_offsetttt__ + (t*HBM_AXI_PACK_SIZE + v)]<<", ";	
			}
			cout<<endl;
		}
	}
	#endif 

	std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();
    for (size_t iteration_idx = 0; iteration_idx < num_iterations; iteration_idx++) {
        std::cout <<"------------------------- iteration "<<iteration_idx<<" started... -------------------------"<<std::endl;
		int flag = iteration_idx % 2;

        if (iteration_idx >= 2) {
            OCL_CHECK(err, err = read_events[flag].wait());
        }

        // Allocate Buffer in Global Memory
        // Buffers are allocated using CL_MEM_USE_HOST_PTR for efficient memory and
        // Device-to-host communication
		std::cout << "Creating Buffers..." << std::endl;
		for (int i = 0; i < NUM_HBM_ARGS; i++) {
			std::cout << "Creating Buffer "<<i<<"..." << std::endl;
			OCL_CHECK(err, buffer_hbm[i] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
                                            bytes_per_iteration, &inBufExt[i], &err)); // REMOVEME 'i%6'
		}
		
		std::cout << "Setting the k_vadd Arguments..." << std::endl;
		for (int i = 0; i < NUM_HBM_ARGS; i++) {
			std::cout << "Setting the k_vadd Argument for argument "<<i<<"..." << std::endl;
			OCL_CHECK(err, err = krnl_vadd.setArg(i, buffer_hbm[i]));
		}
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS, int(action.module)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 1, int(action.start_pu)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 2, int(action.size_pu)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 3, int(action.start_pv)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 4, int(action.size_pv)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 5, int(action.start_llpset)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 6, int(action.size_llpset)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 7, int(action.start_llpid)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 8, int(action.size_llpid)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 9, int(action.start_gv)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 10, int(action.size_gv)));
		OCL_CHECK(err, err = krnl_vadd.setArg(NUM_HBM_ARGS + 11, int(action.finish)));

        // Copy input data to device global memory
        std::cout << "Copying data (Host to Device)..." << std::endl;
		for (int i = 0; i < NUM_HBM_ARGS; i++) {
			std::cout << "Copying data @ channel "<<i<<" (Host to Device)..." << std::endl;
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_hbm[i]}, 0));
		}
		q.finish();

		std::chrono::steady_clock::time_point begin_time1 = std::chrono::steady_clock::now();
		printf("Enqueueing NDRange kernel.\n");
		OCL_CHECK(err, err = q.enqueueTask(krnl_vadd));
		q.finish();
		double end_time1 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time1).count()) / 1000;	
		std::cout <<">>> kernel time elapsed for current iteration : "<<end_time1<<" ms, "<<(end_time1 * 1000)<<" microsecs, "<<std::endl;

        // Copy Result from Device Global Memory to Host Local Memory
        std::cout << "Getting Results (Device to Host)..." << std::endl;
		for (int i = 0; i < NUM_HBM_ARGS; i++) {
			std::cout << "Getting Results @ channel "<<i<<" (Device to Host)..." << std::endl;
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_hbm[i]}, CL_MIGRATE_MEM_OBJECT_HOST));
		}
		q.finish();
    }
	
    // Wait for all of the OpenCL operations to complete
    printf("Waiting...\n");
    OCL_CHECK(err, err = q.flush());
    OCL_CHECK(err, err = q.finish());
    // OPENCL HOST CODE AREA ENDS
	
	double end_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time).count()) / 1000;	
	std::cout <<">>> kernel time elapsed for all iterations : "<<end_time<<" ms, "<<(end_time * 1000)<<" microsecs, "<<std::endl;
	
    bool match = true;
    // Verify the results
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int host_result = HHX[12][i] + HHX[13][i];
        if (device_result[i] != host_result) {
            printf("Error: Result mismatch:\n");
            printf("i = %d CPU result = %d Device result = %d\n", i, host_result, device_result[i]);
            match = false;
            break;
        }
    }
	
	#ifdef HOST_PRINT_RESULTS_XXXX
	cout<<"---------------------------------------------- host_fpga:: after ---------------------------------------------- "<<endl;
	for(unsigned int i=0; i<1; i++){
		for(unsigned int t=0; t<4; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				cout<<HHX[2*i+1][base_offsetttt__ + (t*HBM_AXI_PACK_SIZE + v)]<<", ";	
			}
			cout<<endl;
		}
	}
	#endif 

	printf("TEST %s\n", "PASSED");
    return EXIT_SUCCESS;
}
#endif 

