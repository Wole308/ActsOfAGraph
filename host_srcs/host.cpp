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

#define RUN_SW_KERNEL
#define RUN_FPGA_KERNEL

#define ___PRE_RUN___
#define ___POST_RUN___ 

// #define LAUNCH_TYPE 0 // COARSE
#define LAUNCH_TYPE 1 // FINE

// #define ___SYNC___

#define NUM_BBBBBB_FPGAS NUM_FPGAS

#ifdef FPGA_IMPL
bool profiling0 = false; // *false; // true;
#else 
bool profiling0 = true; 	
#endif 
bool profiling1_timing = false;//false;
unsigned int num_prints2 = 1;//NUM_FPGAS;

#ifdef FPGA_IMPL
#define MIGRATE_HOST_TO_DEVICE() void _migrate_host_to_device(cl::CommandQueue * q, cl_int err, unsigned int fpga, std::vector<cl::Buffer> &buffer_hbm, std::vector<cl::Buffer> &buffer_hbmc)
#define MIGRATE_DEVICE_TO_HOST() void _migrate_device_to_host(cl::CommandQueue * q, cl_int err, unsigned int fpga, std::vector<cl::Buffer> &buffer_hbm, std::vector<cl::Buffer> &buffer_hbmc)
#else
#define MIGRATE_HOST_TO_DEVICE() void _migrate_host_to_device()
#define MIGRATE_DEVICE_TO_HOST() void _migrate_device_to_host()
#endif

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
    if(profiling0 == true){ printf("[%s]: %s %s\n", reinterpret_cast<char*>(data), status_str, command_str); }
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
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 13, int(action.start_gv_fpga)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 14, int(action.start_gv)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 15, int(action.size_gv)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 16, int(action.id_process)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 17, int(action.id_import)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 18, int(action.id_export)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 19, int(action.size_import_export)));
	OCL_CHECK(err, err = kernels->setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + NUM_HBMIO_ARGS + NUM_HBMIO_CHKPTS_ARGS + 20, int(action.status)));
}
#endif 

unsigned int load_actions_coarse(unsigned int launch_type, unsigned int fpga, action_t * actions[NUM_FPGAS], universalparams_t universalparams){
	action_t action;
	
	action.module = ALL_MODULES;
	action.graph_iteration = NAp;
	
	action.start_pu = fpga; 
	action.size_pu = universalparams.NUM_UPARTITIONS;  // ssh -J oluwole308@cdsc0.cs.ucla.edu oluwole308@10.10.128.45
	action.skip_pu = NUM_FPGAS; 
	
	action.start_pv_fpga = 0;
	action.start_pv = 0;
	action.size_pv = universalparams.NUM_APPLYPARTITIONS; 
	
	action.start_gv_fpga = 0;
	action.start_gv = 0; 
	action.size_gv = universalparams.NUM_APPLYPARTITIONS; 
	
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
	return 1;
}
unsigned int load_actions_fine(unsigned int launch_type, unsigned int fpga, action_t * actions[NUM_FPGAS], unsigned int num_upartitions_gathered_per_launch, universalparams_t universalparams){
	unsigned int index = 0;
	// process
	for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t+=PE_BATCH_SIZE){ 
		action_t action;
		
		action.module = PROCESS_EDGES_MODULE;
		action.graph_iteration = NAp;
		
		action.start_pu = t;
		action.size_pu = PE_BATCH_SIZE; 
		action.skip_pu = 1;
		
		action.start_pv_fpga = NAp;
		action.start_pv = NAp;
		action.size_pv = NAp; 
		
		action.start_gv_fpga = NAp;
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
	// return index; // FIXME.
	
	for(unsigned int apply_id=0; apply_id<universalparams.NUM_APPLYPARTITIONS; apply_id+=AU_BATCH_SIZE){ 
		// apply
		action_t action;
		
		action.module = APPLY_UPDATES_MODULE;
		action.graph_iteration = NAp;
		
		action.start_pu = NAp; 
		action.size_pu = NAp; 
		action.skip_pu = NAp;
		
		action.start_pv_fpga = NAp;
		action.start_pv = apply_id; // local value 
		action.size_pv = AU_BATCH_SIZE; 
		
		action.start_gv_fpga = NAp;
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
		
		// gather
		// action_t action;
		
		action.module = GATHER_FRONTIERS_MODULE;
		action.graph_iteration = NAp;
		
		action.start_pu = NAp; 
		action.size_pu = NAp; 
		action.skip_pu = NAp;
		
		action.start_pv_fpga = NAp;
		action.start_pv = NAp; 
		action.size_pv = NAp;
		
		action.start_gv_fpga = NAp;
		action.start_gv = (apply_id * (NUM_SUBPARTITION_PER_PARTITION / NUM_FPGAS)) + 0; // t; // local value
		action.size_gv = GF_BATCH_SIZE;
		
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
	return index;
}

void initialize_gas(gas_import_t gas_import[NUM_FPGAS][MAX_NUM_UPARTITIONS], gas_process_t gas_process[NUM_FPGAS][MAX_NUM_UPARTITIONS], gas_export_t gas_export[NUM_FPGAS][MAX_NUM_UPARTITIONS], universalparams_t universalparams){
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ 
			#if NUM_FPGAS==1
			gas_import[fpga][t].ready_for_import = 1;
			gas_process[fpga][t].ready_for_process = 1;// NUM_FPGAS; // 1;
			gas_export[fpga][t].ready_for_export = 1;
			#else 
			gas_import[fpga][t].ready_for_import = 0;
			gas_process[fpga][t].ready_for_process = 1;//NUM_FPGAS; // 1;
			gas_export[fpga][t].ready_for_export = 0;
			#endif 
			
			gas_import[fpga][t].tmp_state = 0;
			gas_process[fpga][t].tmp_state = 0;
			gas_export[fpga][t].tmp_state = 0;
			
			gas_import[fpga][t].iteration = 0;
			gas_process[fpga][t].iteration = 0;
			gas_export[fpga][t].iteration = 0;
		}
	}
}

value_t merge_frontiers(value_t A, value_t B){
	#pragma HLS INLINE 
	#ifdef BAKE_PAGERANK_ALGORITHM_FUNCS
	return A + B;	
	#endif 
	#ifdef BAKE_SPMV_ALGORITHM_FUNCS
	return A + B;	
	#endif 
	#ifdef BAKE_HITS_ALGORITHM_FUNCS	
	return A + B;	
	#endif 
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

void traverse_graph_in_iteration(unsigned int GraphIter, vector<value_t> &actvvs, vector<value_t> &actvvs_nextit, vector<edge_t> &vertexptrbuffer, vector<edge3_type> &edgedatabuffer, unsigned int * vdatas, tuple_t * vpartition_stats, long double vertices_processed[128], long double edges_processed[128], universalparams_t universalparams){						
	for(unsigned int i=0; i<actvvs.size(); i++){
		unsigned int vid = actvvs[i];
		if(false){ cout<<"host: vid: "<<vid<<", edges_size "<<vertexptrbuffer[vid+1] - vertexptrbuffer[vid]<<""<<endl; }
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ continue; } // FIXME.
		#ifdef _DEBUGMODE_CHECKS3
		if(vptr_end < vptr_begin){ cout<<"ERROR: vptr_end("<<vptr_end<<") < vptr_begin("<<vptr_begin<<"). exiting..."<<endl; exit(EXIT_FAILURE); }
		#endif
		vertices_processed[GraphIter] += 1; 
		vpartition_stats[vid / MAX_UPARTITION_SIZE].A += 1; 
		vpartition_stats[vid / MAX_UPARTITION_SIZE].B += edges_size; // 
	
		for(unsigned int k=0; k<edges_size; k++){
			unsigned int dstvid = edgedatabuffer[vptr_begin + k].dstvid;
			unsigned int res = vdatas[vid] + 1;
			value_t vprop = vdatas[dstvid];
			value_t vtemp = min(vprop, res);
			vdatas[dstvid] = vtemp;
			if(vtemp != vprop){ 
				actvvs_nextit.push_back(dstvid);
				#ifdef _DEBUGMODE_CHECKS3
				if(dstvid / MAX_UPARTITION_SIZE >= universalparams.NUM_UPARTITIONS){ cout<<"ERROR 232. dstvid ("<<dstvid<<") / MAX_UPARTITION_SIZE ("<<MAX_UPARTITION_SIZE<<") >= universalparams.NUM_UPARTITIONS ("<<universalparams.NUM_UPARTITIONS<<"). vid: "<<vid<<". EXITING..."<<endl; exit(EXIT_FAILURE); }	
				#endif 
			}
			edges_processed[GraphIter] += 1; 
		}
	}
	return; 
}

long double host::runapp(std::string binaryFile__[2], 
		vector<edge3_type> &edgedatabuffer, vector<edge_t> &vertexptrbuffer, 
			HBM_channelAXISW_t * HBM_axichannel[NUM_FPGAS][2][NUM_PEs], HBM_channelAXISW_t * HBM_axicenter[NUM_FPGAS][2], unsigned int hbm_channel_wwsize, unsigned int globalparams[1024], universalparams_t universalparams,
				vector<edge3_type> (&final_edge_updates)[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS]){
				
	unsigned int ARRAY_SIZE = hbm_channel_wwsize * HBM_AXI_PACK_SIZE; // REMOVEME.
	unsigned int num_upartitions_gathered_per_launch = 1; // 1, NUM_FPGAS, universalparams.NUM_UPARTITIONS
	
	unsigned int report_statistics[64]; for(unsigned int t=0; t<64; t++){ report_statistics[t] = 0; }
	
	bool all_vertices_active_in_all_iterations = false; 
	if(universalparams.ALGORITHM == PAGERANK || universalparams.ALGORITHM == CF || universalparams.ALGORITHM == HITS || universalparams.ALGORITHM == SPMV){ all_vertices_active_in_all_iterations = true; }
	
	// sw sssp
	long double vertices_processed[128]; long double edges_processed[128]; for(unsigned int i=0; i<128; i++){ vertices_processed[i] = 0; edges_processed[i] = 0; }
	vector<value_t> actvvs; vector<value_t> actvvs_nextit; actvvs.push_back(1);
	unsigned int * vdatas = new unsigned int[universalparams.NUM_VERTICES];
	for(unsigned int i=0; i<universalparams.NUM_VERTICES; i++){ vdatas[i] = 0xFFFFFFFF; }
	for(unsigned int i=0; i<actvvs.size(); i++){ vdatas[actvvs[i]] = 0; }
	tuple_t * vpartition_stats = new tuple_t[universalparams.NUM_UPARTITIONS];
	for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ vpartition_stats[t].A = 0; vpartition_stats[t].B = 0; }
	bool __stop_sw_run__ = false;
	float ___hybrid___engine___vertex___threshold___ = (0.7 * universalparams.NUM_VERTICES) / 100; 
	
	cout<<"host::runapp_sync: NUM_HBM_ARGS: "<<NUM_HBM_ARGS<<" ---"<<endl;
	cout<<"host::runapp_sync: ARRAY_SIZE: "<<ARRAY_SIZE<<" ---"<<endl;
	cout<<"host::runapp_sync: IMPORT_EXPORT_GRANULARITY_VECSIZE: "<<IMPORT_EXPORT_GRANULARITY_VECSIZE<<" ---"<<endl;
	cout<<"host::runapp_sync: hbm_channel_wwsize * HBM_AXI_PACK_SIZE: "<<hbm_channel_wwsize * HBM_AXI_PACK_SIZE<<" ---"<<endl;
	
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
	#endif 
	
    // load binary to FPGA 
	#ifdef FPGA_IMPL
    std::cout << "Creating Context..." << std::endl;
    auto devices = xcl::get_xil_devices();
    bool valid_device = false;
	unsigned int device_count = devices.size();
	for(unsigned int fpga=0; fpga<device_count; fpga++){
        auto device = devices[fpga];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, contexts[fpga] = cl::Context(device, nullptr, nullptr, nullptr, &err));
		
		#ifdef ___SYNC___
		OCL_CHECK(err, q[fpga] = cl::CommandQueue(contexts[fpga], device, CL_QUEUE_PROFILING_ENABLE, &err)); ////////// FIXME.
		#else 
		OCL_CHECK(err, q[fpga] = cl::CommandQueue(contexts[fpga], device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err));	
		#endif 
		
		auto fileBuf = xcl::read_binary_file(binaryFile__[0]);
		cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};

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

    // variables 
    size_t bytes_per_iteration = ARRAY_SIZE * sizeof(int);
	cout<<"--- host::runapp_sync: bytes_per_iteration: "<<bytes_per_iteration<<" ---"<<endl;

	// allocate sw buffers
	#ifdef FPGA_IMPL
	std::vector<int, aligned_allocator<int> > HBM_axichannel_vector[NUM_FPGAS][32]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int i=0; i<NUM_VALID_PEs*2; i++){ HBM_axichannel_vector[fpga][i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }}
	std::vector<int, aligned_allocator<int> > HBM_axicenter_vector[NUM_FPGAS][2][2]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int flag=0; flag<2; flag++){ for(unsigned int i=0; i<2; i++){ HBM_axicenter_vector[fpga][flag][i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }}}
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		for(unsigned int i=0; i<NUM_VALID_PEs; i++){ // REMOVEME. NUM_PEs
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
	#endif 
	
	#ifdef FPGA_IMPL
	std::vector<int, aligned_allocator<int> > HBM_import[NUM_IMPORT_BUFFERS]; 
	std::vector<int, aligned_allocator<int> > HBM_export[NUM_FPGAS][NUM_EXPORT_BUFFERS]; 
	#else 
	HBM_channelAXISW_t * HBM_import[NUM_IMPORT_BUFFERS]; 
	HBM_channelAXISW_t * HBM_export[NUM_FPGAS][NUM_EXPORT_BUFFERS]; 
	#endif 
	for(unsigned int i=0; i<NUM_IMPORT_BUFFERS; i++){ 
		#ifdef FPGA_IMPL
		HBM_import[i] = std::vector<int, aligned_allocator<int> >((IMPORT_EXPORT_GRANULARITY_VECSIZE * HBM_AXI_PACK_SIZE)); 
		#else 
		HBM_import[i] = new HBM_channelAXISW_t[IMPORT_EXPORT_GRANULARITY_VECSIZE]; 
		#endif 
	}
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		for(unsigned int i=0; i<NUM_IMPORT_BUFFERS; i++){ 
			#ifdef FPGA_IMPL
			HBM_export[fpga][i] = std::vector<int, aligned_allocator<int> >((IMPORT_EXPORT_GRANULARITY_VECSIZE * HBM_AXI_PACK_SIZE)); 
			#else 
			HBM_export[fpga][i] = new HBM_channelAXISW_t[IMPORT_EXPORT_GRANULARITY_VECSIZE]; 	
			#endif 
		}
	}
		
    // declare buffers and ext_ptrs
	#ifdef FPGA_IMPL
    vector<cl::Event> kernel_events(2 * NUM_FPGAS);
    vector<cl::Event> read_events(2 * NUM_FPGAS);
	
	std::vector<cl::Buffer> buffer_hbm(32 * NUM_FPGAS);
	std::vector<cl::Buffer> buffer_hbmc(NUM_HBMC_ARGS * NUM_FPGAS);
	std::vector<cl::Buffer> buffer_import(2 * NUM_FPGAS);
	std::vector<cl::Buffer> buffer_export(2 * NUM_FPGAS);
	std::vector<cl::Buffer> buffer_import_chkpt(1 * NUM_FPGAS);
	std::vector<cl::Buffer> buffer_export_chkpt(1 * NUM_FPGAS);
	std::vector<cl_mem_ext_ptr_t> inBufExt(32 * NUM_FPGAS);
	std::vector<cl_mem_ext_ptr_t> inBufExt_c(2 * NUM_FPGAS);
	#endif 
	
	// attach ext pointers 
	#ifdef FPGA_IMPL
	for(unsigned int fpga=0; fpga<device_count; fpga++){ 
		for (int i = 0; i < NUM_HBM_ARGS; i++) {
			inBufExt[fpga*NUM_FPGAS + i].obj = HBM_axichannel_vector[fpga][i].data();
			inBufExt[fpga*NUM_FPGAS + i].param = 0;
			inBufExt[fpga*NUM_FPGAS + i].flags = pc[i];
		}
	}
	for(unsigned int fpga=0; fpga<device_count; fpga++){ 
		for (int i = 0; i < NUM_HBMC_ARGS; i++) {
			inBufExt_c[fpga*NUM_FPGAS + i].obj = HBM_axicenter_vector[fpga][0][i].data();
			inBufExt_c[fpga*NUM_FPGAS + i].param = 0;
			inBufExt_c[fpga*NUM_FPGAS + i].flags = pc[NUM_HBM_ARGS + i];
		}
	}
	#endif 
	
	// Allocate Buffer in Global Memory
	#ifdef FPGA_IMPL
	std::cout << "Creating Buffers..." << std::endl;
	for(unsigned int fpga=0; fpga<device_count; fpga++){ 
		for (int i = 0; i < NUM_HBM_ARGS; i++) {
			std::cout << "Creating Buffer "<<i<<"..." << std::endl;
			OCL_CHECK(err, buffer_hbm[fpga*NUM_FPGAS + i] = cl::Buffer(contexts[fpga], CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
											bytes_per_iteration, &inBufExt[fpga*NUM_FPGAS + i], &err));
		}
	}
	
	std::cout << "Creating Center Buffers..." << std::endl;
	for(unsigned int fpga=0; fpga<device_count; fpga++){ 
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
	for(unsigned int fpga=0; fpga<device_count; fpga++){ 
		for (int i = 0; i < NUM_HBM_ARGS; i++) {
			std::cout << "Setting the k_vadd Argument for argument "<<i<<"..." << std::endl;
			OCL_CHECK(err, err = kernels[fpga].setArg(i, buffer_hbm[fpga*NUM_FPGAS + i]));
		}
	}
	std::cout << "Setting Kernel Arguments (center HBM)..." << std::endl;
	for(unsigned int fpga=0; fpga<device_count; fpga++){ 
		for (int i = 0; i < NUM_HBMC_ARGS; i++) {
			std::cout << "Setting Kernel Argument for argument "<<NUM_HBM_ARGS + i<<"..." << std::endl;
			OCL_CHECK(err, err = kernels[fpga].setArg(NUM_HBM_ARGS + i, buffer_hbmc[fpga*NUM_FPGAS + i]));
		}
	}
	#endif 
	
	// Copy input data to device global memory
	#ifdef FPGA_IMPL
	for(unsigned int fpga=0; fpga<device_count; fpga++){ _migrate_host_to_device(&q[fpga], err, fpga, buffer_hbm, buffer_hbmc); }
	#endif 

	#ifndef FPGA_IMPL
	acts_kernel * acts = new acts_kernel(universalparams);
	#endif 
	
	gas_import_t gas_import[NUM_FPGAS][MAX_NUM_UPARTITIONS]; 
	gas_process_t gas_process[NUM_FPGAS][MAX_NUM_UPARTITIONS]; 
	gas_export_t gas_export[NUM_FPGAS][MAX_NUM_UPARTITIONS]; 
	initialize_gas(gas_import, gas_process, gas_export, universalparams);

	// set mappings (export -> import)
	// #ifdef ___NOT_YET_IMPLEMENTED___
	mapping_t * mapping[NUM_FPGAS]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ mapping[fpga] = new mapping_t[MAX_NUM_UPARTITIONS]; }
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
		for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t+=1){
			mapping[fpga][t].fpga = t % NUM_FPGAS;
			mapping[fpga][t].io_id = t / NUM_FPGAS;			
			#ifdef _DEBUGMODE_HOSTPRINTS//4
			if(t<16){ cout<<"$$$ host: [fpga: "<<fpga<<", io_id: "<<t<<"] mapped to [fpga: "<<mapping[fpga][t].fpga<<", io_id: "<<mapping[fpga][t].io_id<<"]"<<endl; }
			#endif 
		}
	}
	// #endif 
	
	mapping_t * HBM_xxx_ptr[NUM_FPGAS]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ HBM_xxx_ptr[fpga] = new mapping_t[NUM_IMPORT_BUFFERS]; }
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ for(unsigned int t=0; t<NUM_IMPORT_BUFFERS; t+=1){ HBM_xxx_ptr[fpga][t].fpga = 0; HBM_xxx_ptr[fpga][t].io_id = 0; }}
	// exit(EXIT_SUCCESS);
	
	unsigned int launch_type = LAUNCH_TYPE;
	unsigned int num_launches = 0;
	action_t * actions[NUM_FPGAS]; for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ actions[fpga] = new action_t[1024]; }
	for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
		if(launch_type == 0){ num_launches = load_actions_coarse(launch_type, fpga, actions, universalparams); }
		else { num_launches = load_actions_fine(launch_type, fpga, actions, num_upartitions_gathered_per_launch, universalparams); }
	}

	// Run kernel 
	// sw kernel 
	#ifdef RUN_SW_KERNEL
	actvvs.clear(); actvvs_nextit.clear(); 
	actvvs.push_back(1);
	for(unsigned int i=0; i<128; i++){ vertices_processed[i] = 0; edges_processed[i] = 0; } 
	for(unsigned int i=0; i<universalparams.NUM_VERTICES; i++){ vdatas[i] = 0xFFFFFFFF; }
	for(unsigned int i=0; i<actvvs.size(); i++){ vdatas[actvvs[i]] = 0; }
	for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ vpartition_stats[t].A = 0; vpartition_stats[t].B = 0; }
	tuple_t active_vertices_in_iteration[2][128]; for(unsigned int i=0; i<128; i++){ active_vertices_in_iteration[0][i].A = 0; active_vertices_in_iteration[0][i].B = 0; active_vertices_in_iteration[1][i].A = 0; active_vertices_in_iteration[1][i].B = 0; } 
	active_vertices_in_iteration[0][0].A = 1;
	unsigned int GraphIter;
	for (GraphIter = 0; GraphIter < 16; GraphIter++) {
		std::chrono::steady_clock::time_point begin_time0 = std::chrono::steady_clock::now();
		
		for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ vpartition_stats[t].A = 0; vpartition_stats[t].B = 0; }
		traverse_graph_in_iteration(GraphIter, actvvs, actvvs_nextit, vertexptrbuffer, edgedatabuffer, vdatas, vpartition_stats, vertices_processed, edges_processed, universalparams);
		
		active_vertices_in_iteration[0][GraphIter + 1].A = actvvs_nextit.size();
		unsigned int num_actv_edges = 0; for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ num_actv_edges += vpartition_stats[t].B; }
		if(actvvs_nextit.size() == 0){ cout<<"no more activer vertices to process. breaking out... "<<endl; break; }
		actvvs.clear(); for(unsigned int i=0; i<actvvs_nextit.size(); i++){ actvvs.push_back(actvvs_nextit[i]); } 
		
		double end_time0 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time0).count()) / 1000;	
		cout<<"host: end of iteration "<<GraphIter<<": "<<actvvs_nextit.size()<<" active vertices generated, "<<num_actv_edges<<" edges processed in ("<<end_time0<<" ms, "<<(end_time0 * 1000)<<" microsecs)"<<endl;
		active_vertices_in_iteration[0][GraphIter].B = end_time0;
		
		actvvs_nextit.clear();
	}	
	// for(unsigned int t=0; t<GraphIter; t++){ 
		// cout<<"--- host: "<<active_vertices_in_iteration[0][t].A<<" active vertices processed in iteration "<<t<<" in "<<active_vertices_in_iteration[0][t].B<<" ms"; 
		// if(active_vertices_in_iteration[0][t].A < ___hybrid___engine___vertex___threshold___){ cout<<" [SW]"<<endl; } else { cout<<" [FPGA]"<<endl; }
	// } 
	#endif 
	// return EXIT_SUCCESS;
	// exit(EXIT_SUCCESS);
	
	// Run kernel
	unsigned int num_iterations = 20; if(all_vertices_active_in_all_iterations == false){ num_iterations = GraphIter; }
	unsigned int run_idx = 0;
	std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();
	for (unsigned int iteration_idx = 0; iteration_idx < num_iterations; iteration_idx++) {
		
		bool __run__iteration__in__SW__mode__ = false;
		bool __run__iteration__in__FPGA__mode__ = false;
		if(active_vertices_in_iteration[0][iteration_idx].A < ___hybrid___engine___vertex___threshold___){ __run__iteration__in__SW__mode__ = true; } else{ __run__iteration__in__FPGA__mode__ = true; }
		if(all_vertices_active_in_all_iterations == true){ __run__iteration__in__SW__mode__ = false; __run__iteration__in__FPGA__mode__ = true; } 
		if(__run__iteration__in__SW__mode__ == true){ std::cout << endl << TIMINGRESULTSCOLOR <<"#################################################################### GAS iteration: "<<iteration_idx<< " [FPGA mode] ####################################################################"<< RESET << std::endl; }
		if(__run__iteration__in__FPGA__mode__ == true){ std::cout << endl << TIMINGRESULTSCOLOR <<"#################################################################### GAS iteration: "<<iteration_idx<< " [FPGA mode] ####################################################################"<< RESET << std::endl; }
		
		#if NUM_FPGAS==1
		initialize_gas(gas_import, gas_process, gas_export, universalparams);
		#endif 
		
		// sw kernel 
		#ifdef ___NOT_YET_IMPLEMENTED___
		if(__run__iteration__in__SW__mode__ == true && __stop_sw_run__ == false){
			std::chrono::steady_clock::time_point begin_time0 = std::chrono::steady_clock::now();
			
			for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ vpartition_stats[t].A = 0; vpartition_stats[t].B = 0; }
			traverse_graph_in_iteration(iteration_idx, actvvs, actvvs_nextit, vertexptrbuffer, edgedatabuffer, vdatas, vpartition_stats, vertices_processed, edges_processed, universalparams);
			unsigned int num_actv_edges = 0; for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ num_actv_edges += vpartition_stats[t].B; }
			cout<<"host: end of iteration "<<iteration_idx<<": ("<<actvvs_nextit.size()<<" active vertices generated, "<<num_actv_edges<<" edges processed)"<<endl;
			if(actvvs_nextit.size() == 0){ cout<<"no more activer vertices to process. breaking out... "<<endl; __stop_sw_run__ = true; }
			actvvs.clear(); for(unsigned int i=0; i<actvvs_nextit.size(); i++){ actvvs.push_back(actvvs_nextit[i]); } actvvs_nextit.clear();
			
			double end_time0 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time0).count()) / 1000;	
			if(true){ std::cout << TIMINGRESULTSCOLOR << ">>> host::sw kernel time elapsed for iteration "<<iteration_idx<<" : "<<end_time0<<" ms, "<<(end_time0 * 1000)<<" microsecs, "<< RESET << std::endl; }	
		}
		#endif 
		
		// fpga kernel 
		#ifdef RUN_FPGA_KERNEL
		if(__run__iteration__in__FPGA__mode__ == true){
			std::chrono::steady_clock::time_point begin_time1 = std::chrono::steady_clock::now();
			for(unsigned int launch_idx=0; launch_idx<num_launches; launch_idx+=1){
				std::cout << endl << TIMINGRESULTSCOLOR <<"-------------------------------- host: GAS iteration: "<<iteration_idx<<", launch_idx "<<launch_idx<<" (of "<<num_launches<<"), fpgas [0 - "<<NUM_FPGAS-1<<"] started... --------------------------------"<< RESET << std::endl; 
				
				action_t action[NUM_FPGAS];
				unsigned int import_pointer[NUM_FPGAS];
				unsigned int process_pointer[NUM_FPGAS];
				unsigned int export_pointer[NUM_FPGAS];
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ action[fpga] = actions[fpga][launch_idx]; }
				
				int flag = run_idx % 2;
				#ifdef FPGA_IMPL
				if (run_idx >= 2) {
					for(unsigned int fpga=0; fpga<device_count; fpga++){ 
						OCL_CHECK(err, err = read_events[2*fpga + flag].wait());
					}
				}
				#endif 
				
				#ifdef _DEBUGMODE_HOSTPRINTS//4
				if(profiling0 == true){ 
					for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ std::cout<<">>> "<<universalparams.NUM_UPARTITIONS / NUM_FPGAS<<" imports queue @ fpga "<<fpga<<": "; for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t++){ std::cout<<gas_import[fpga][t].ready_for_import<<", "; } cout<<endl; }
					for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ std::cout<<">>> "<<universalparams.NUM_UPARTITIONS<<" exports queue @ fpga "<<fpga<<": "; for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ std::cout<<gas_export[fpga][t].ready_for_export<<", "; } cout<<endl; }
					for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ std::cout<<">>> "<<universalparams.NUM_UPARTITIONS / NUM_FPGAS<<" process queue @ fpga "<<fpga<<": "; for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t++){ std::cout<<gas_process[fpga][t].ready_for_process<<", "; } cout<<endl; }
				}
				#endif
				
				// set scalar arguments
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
					action[fpga].graph_iteration = iteration_idx;
					action[fpga].id_import = 0; 
					action[fpga].id_export = 0; 
					action[fpga].id_process = launch_idx;
					action[fpga].size_import_export = IMPORT_EXPORT_GRANULARITY_VECSIZE; // 
				}
				
				#ifdef _DEBUGMODE_HOSTPRINTS//4
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ std::cout<<"imports @ fpga "<<fpga<<": "; for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t++){ std::cout<<gas_import[fpga][t].ready_for_import<<", "; } cout<<endl; }
				#endif 
				
				// pre-run
				std::chrono::steady_clock::time_point begin_time2 = std::chrono::steady_clock::now();
				#ifdef ___PRE_RUN___
				#if NUM_BBBBBB_FPGAS == 1
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
					action[fpga].id_process = action[fpga].start_pu; 
				}
				#endif
				
				#if (NUM_BBBBBB_FPGAS>1)
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
					import_pointer[fpga] = INVALID_IOBUFFER_ID;
					action[fpga].id_import = import_pointer[fpga];
					for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t+=IMPORT_BATCH_SIZE){
						utilityobj->checkoutofbounds("host::ERROR 2111::", gas_import[fpga][t].ready_for_import, MAX_NUM_UPARTITIONS, fpga, t, NAp);
						bool en = true; if(gas_import[fpga][t].ready_for_import == 0){ en = false; } // 'NB: must have something to import' 'NB: checking first one in set suffices.'
						if(en == true){ 
							import_pointer[fpga] = t;
							action[fpga].id_import = import_pointer[fpga]; 
							break; 
						}
					}
					
					process_pointer[fpga] = INVALID_IOBUFFER_ID;
					action[fpga].id_process = process_pointer[fpga];
					if(action[fpga].module != APPLY_UPDATES_MODULE && action[fpga].module != GATHER_FRONTIERS_MODULE){
						for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t+=PE_BATCH_SIZE){
							bool en = true; if(gas_process[fpga][t].ready_for_process == 0){ en = false; } // 'NB: all remote FPGAs must add their contributions before partition is ready to process' 'NB: checking first one in set suffices.'
							if(en == true){
								process_pointer[fpga] = t;
								action[fpga].id_process = process_pointer[fpga]; 
								action[fpga].start_pu = process_pointer[fpga]; 
								break; 
							}
						}
					}
					
					export_pointer[fpga] = INVALID_IOBUFFER_ID;//88;
					action[fpga].id_export = export_pointer[fpga];
					for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t+=EXPORT_BATCH_SIZE){ 
						bool en = true; if(gas_export[fpga][t].ready_for_export == 0){ en = false; } // NEWCHANGE 'NB: checking first one in set suffices.'
						if(en == true){
							export_pointer[fpga] = t;
							action[fpga].id_export = export_pointer[fpga]; 
							break; 
						}
					}
				}
				#endif 
				#endif 
				double end_time2 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time2).count()) / 1000;	
				if(profiling1_timing == true){ std::cout << TIMINGRESULTSCOLOR << ">>> host::pre-run time elapsed : "<<end_time2<<" ms, "<<(end_time2 * 1000)<<" microsecs, "<< RESET << std::endl; }
			
				// Allocate Buffer in Global Memory
				std::chrono::steady_clock::time_point begin_time3 = std::chrono::steady_clock::now();
				#ifdef FPGA_IMPL//_XXX // FIXME.	
				for(unsigned int fpga=0; fpga<device_count; fpga++){ 
					if(profiling0 == true){ std::cout << "Creating Import Buffers @ fpga "<<fpga<<"..." << std::endl; }
					OCL_CHECK(err, buffer_import[2*fpga + flag] = cl::Buffer(contexts[fpga], CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
													(IMPORT_EXPORT_GRANULARITY_VECSIZE * HBM_AXI_PACK_SIZE * sizeof(int)), &HBM_import[action[fpga].id_import][0], &err)); 
				
					if(profiling0 == true){ std::cout << "Creating Export Buffers..." << std::endl; }
					OCL_CHECK(err, buffer_export[2*fpga + flag] = cl::Buffer(contexts[fpga], CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
													(IMPORT_EXPORT_GRANULARITY_VECSIZE * HBM_AXI_PACK_SIZE * sizeof(int)), &HBM_export[fpga][action[fpga].id_export][0], &err)); 
				}
				#endif
				double end_time3 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time3).count()) / 1000;	
				if(profiling1_timing == true){ std::cout << TIMINGRESULTSCOLOR << ">>> host::allocate-buffer-in-global-memory time elapsed "<<end_time3<<" ms, "<<(end_time3 * 1000)<<" microsecs, "<< RESET << std::endl; }
				
				#ifdef FPGA_IMPL
				vector<cl::Event> write_event(1 * NUM_FPGAS);
				#endif 
				
				std::chrono::steady_clock::time_point begin_time4 = std::chrono::steady_clock::now();
				#ifdef FPGA_IMPL//_XXX
				for(unsigned int fpga=0; fpga<device_count; fpga++){ 
					if(profiling0 == true){ std::cout << "Setting Import/Export Arguments..." << std::endl; }
					OCL_CHECK(err, err = kernels[fpga].setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS, buffer_import[2*fpga + flag]));
					OCL_CHECK(err, err = kernels[fpga].setArg(NUM_HBM_ARGS + NUM_HBMC_ARGS + 1, buffer_export[2*fpga + flag]));
				}
				#endif 
				double end_time4 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time4).count()) / 1000;	
				if(profiling1_timing == true){ std::cout << TIMINGRESULTSCOLOR << ">>> host::set-kernel-arguments time elapsed : "<<end_time4<<" ms, "<<(end_time4 * 1000)<<" microsecs, "<< RESET << std::endl; }	
				
				// set scalar arguments
				#ifdef FPGA_IMPL
				for(unsigned int fpga=0; fpga<device_count; fpga++){ 
					if(profiling0 == true){ std::cout << "Setting Scalar Arguments..." << std::endl; }
					_set_args___actions(&kernels[fpga], action[fpga], err);
				}
				#endif 
				
				// import frontiers
				std::chrono::steady_clock::time_point begin_time5 = std::chrono::steady_clock::now();
				#ifdef FPGA_IMPL//_XXX // FIXME.
				if(profiling0 == true){ std::cout << "Host to FPGA Transfer..." << std::endl; }
				for(unsigned int fpga=0; fpga<device_count; fpga++){ 
					OCL_CHECK(err, err = q[fpga].enqueueMigrateMemObjects({buffer_import[2*fpga + flag]}, 0, nullptr, &write_event[fpga]));
					set_callback(write_event[fpga], "ooo_queue");
					#ifdef ___SYNC___
					OCL_CHECK(err, err = write_event[fpga].wait()); 
					#endif 
				}
				#endif
				double end_time5 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time5).count()) / 1000;	
				if(profiling1_timing == true){ std::cout << TIMINGRESULTSCOLOR <<">>> host::import-frontiers time elapsed : "<<end_time5<<" ms, "<<(end_time5 * 1000)<<" microsecs, "<< RESET << std::endl;	}
				
				// run kernel 
				std::chrono::steady_clock::time_point begin_time6 = std::chrono::steady_clock::now();
				#ifdef FPGA_IMPL
					for(unsigned int fpga=0; fpga<device_count; fpga++){ // NUM_FPGAS
						for(unsigned int sub_kernel=0; sub_kernel<1; sub_kernel++){ // NUM_SUBKERNELS_IN_KERNEL
							#ifdef FPGA_IMPL
							if(profiling0 == true){ printf("Enqueueing NDRange kernel.\n"); }
							#endif 				
							#ifdef ___SYNC___
							OCL_CHECK(err, err = q[fpga].enqueueNDRangeKernel(kernels[fpga], 0, 1, 1, NULL, &kernel_events[2*fpga + flag]));
							set_callback(kernel_events[2*fpga + flag], "ooo_queue");
							OCL_CHECK(err, err = kernel_events[2*fpga + flag].wait()); 
							#else 
							std::vector<cl::Event> waitList; waitList.push_back(write_event[fpga]);
							OCL_CHECK(err, err = q[fpga].enqueueNDRangeKernel(kernels[fpga], 0, 1, 1, &waitList, &kernel_events[2*fpga + flag]));
							set_callback(kernel_events[2*fpga + flag], "ooo_queue");
							#endif 	
						}
					}					
				#else 
					for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ // NUM_FPGAS
						for(unsigned int sub_kernel=0; sub_kernel<1; sub_kernel++){ // NUM_SUBKERNELS_IN_KERNEL
							acts->top_function(
								(HBM_channelAXI_t *)HBM_axichannel[fpga][0][0], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][0]
								#if NUM_VALID_HBM_CHANNELS>1
								,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][1], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][1] 
								,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][2], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][2] 
								,(HBM_channelAXI_t *)HBM_axichannel[fpga][0][3], (HBM_channelAXI_t *)HBM_axichannel[fpga][1][3] 
								#if NUM_VALID_HBM_CHANNELS>4
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
								#endif 
								,(HBM_channelAXI_t *)HBM_axicenter[fpga][0], (HBM_channelAXI_t *)HBM_axicenter[fpga][1]
								,(HBM_channelAXI_t *)HBM_export[HBM_xxx_ptr[fpga][action[fpga].id_import].fpga][HBM_xxx_ptr[fpga][action[fpga].id_import].io_id], (HBM_channelAXI_t *)HBM_export[fpga][action[fpga].id_export]
								,fpga ,action[fpga].module ,action[fpga].graph_iteration ,action[fpga].start_pu ,action[fpga].size_pu ,action[fpga].skip_pu ,action[fpga].start_pv_fpga ,action[fpga].start_pv ,action[fpga].size_pv ,action[fpga].start_llpset ,action[fpga].size_llpset ,action[fpga].start_llpid ,action[fpga].size_llpid ,action[fpga].start_gv_fpga ,action[fpga].start_gv ,action[fpga].size_gv ,action[fpga].id_process ,action[fpga].id_import ,action[fpga].id_export ,action[fpga].size_import_export ,action[fpga].status ,final_edge_updates ,report_statistics				
								);	
						}
					}
				#endif 
				double end_time6 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time6).count()) / 1000;	
				if(profiling1_timing == true){ std::cout << TIMINGRESULTSCOLOR << ">>> kernel time elapsed for iteration "<<iteration_idx<<", launch_idx "<<launch_idx<<" : "<<end_time6<<" ms, "<<(end_time6 * 1000)<<" microsecs, "<< RESET <<std::endl; }
				
				// export frontiers
				std::chrono::steady_clock::time_point begin_time7 = std::chrono::steady_clock::now();
				#ifdef FPGA_IMPL//_XXX // FIXME.
				if(profiling0 == true){ std::cout << "FPGA to Host Transfer..." << std::endl; }
				for(unsigned int fpga=0; fpga<device_count; fpga++){ 
					std::vector<cl::Event> eventList; eventList.push_back(kernel_events[2*fpga + flag]);
					OCL_CHECK(err, err = q[fpga].enqueueMigrateMemObjects({buffer_export[2*fpga + flag]}, CL_MIGRATE_MEM_OBJECT_HOST, &eventList,
															&read_events[2*fpga + flag]));			
					set_callback(read_events[2*fpga + flag], "ooo_queue");
					#ifdef ___SYNC___
					OCL_CHECK(err, err = read_events[2*fpga + flag].wait()); 
					#endif 
				}
				#endif
				double end_time7 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time7).count()) / 1000;	
				if(profiling1_timing == true){ std::cout << TIMINGRESULTSCOLOR << ">>> host::export-frontiers time elapsed : "<<end_time7<<" ms, "<<(end_time7 * 1000)<<" microsecs, "<< RESET << std::endl; }
				
				// post-run
				std::chrono::steady_clock::time_point begin_time8 = std::chrono::steady_clock::now();
				#ifdef ___POST_RUN___
				#if (NUM_BBBBBB_FPGAS>1)
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){
					if(import_pointer[fpga] == INVALID_IOBUFFER_ID){ continue; }
					for(unsigned int k=0; k<IMPORT_BATCH_SIZE; k++){
						if(import_pointer[fpga] + k < (universalparams.NUM_UPARTITIONS / NUM_FPGAS)){
							gas_import[fpga][import_pointer[fpga] + k].ready_for_import -= NUM_FPGAS; 
							gas_process[fpga][import_pointer[fpga] + k].tmp_state += NUM_FPGAS;	
							utilityobj->checkoutofbounds("host::ERROR 2121::", gas_import[fpga][import_pointer[fpga] + k].ready_for_import, MAX_NUM_UPARTITIONS, fpga, k, import_pointer[fpga]);
							if(gas_process[fpga][import_pointer[fpga] + k].tmp_state == NUM_FPGAS){ 
								gas_process[fpga][import_pointer[fpga] + k].ready_for_process = 1; 
								gas_process[fpga][import_pointer[fpga] + k].tmp_state = 0;
							}
						}
					}
				}
				
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
					if(process_pointer[fpga] == INVALID_IOBUFFER_ID){ continue; } // NEWCHANGE.
					for(unsigned int k=0; k<PE_BATCH_SIZE; k++){
						if(process_pointer[fpga] + k >= MAX_NUM_UPARTITIONS){ continue; }
						gas_process[fpga][process_pointer[fpga] + k].ready_for_process = 0; 
					}
				}
				
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
					if(action[fpga].module == GATHER_FRONTIERS_MODULE){	
						for(unsigned int t=action[fpga].start_gv; t<action[fpga].start_gv + action[fpga].size_gv; t+=1){							
							for(unsigned int g_v_fpga=0; g_v_fpga<NUM_FPGAS; g_v_fpga++){
								unsigned int global_vertex_partition = (t * NUM_FPGAS) + g_v_fpga;
								if(global_vertex_partition < (universalparams.NUM_APPLYPARTITIONS * NUM_PEs)){
									#ifdef _DEBUGMODE_HOSTPRINTS//4
									if(fpga < num_prints2 && (profiling0 == true)){ cout << TIMINGRESULTSCOLOR << "^^^ activating global vertex partition "<<global_vertex_partition<<" for export... (fpga: "<<fpga<<", g_v_fpga: "<<g_v_fpga<<")" << RESET <<endl; }
									#endif 
									gas_export[fpga][global_vertex_partition].ready_for_export = 1;
								}
							}
						}
					}
				}
				
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
					for(unsigned int k=0; k<EXPORT_BATCH_SIZE; k++){ 
						if(export_pointer[fpga] + k >= MAX_NUM_UPARTITIONS){ continue; }
						gas_export[fpga][export_pointer[fpga] + k].ready_for_export = 0; 
					}
				}
				#endif 
				
				// transfer export to imports
				/* #if (NUM_BBBBBB_FPGAS>1)
				if(action[0].id_export != INVALID_IOBUFFER_ID){							
					for(unsigned int k=0; k<EXPORT_BATCH_SIZE; k++){ 
					unsigned int exp_id = action[0].id_export + k;
					if(exp_id < universalparams.NUM_UPARTITIONS){
						for(unsigned int t=0; t<IMPORT_EXPORT_GRANULARITY_VECSIZE; t++){
							for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){
								value_t res = 0;
								for(unsigned int fpga_l=0; fpga_l<NUM_FPGAS; fpga_l++){
									res = merge_frontiers(res, HBM_export[fpga_l][exp_id][t].data[v]);
								}
								HBM_import[exp_id][t].data[v] = res;
							}
						}
						gas_import[exp_id % NUM_FPGAS][exp_id / NUM_FPGAS].ready_for_import += NUM_FPGAS; 
						gas_import[exp_id % NUM_FPGAS][exp_id / NUM_FPGAS].iteration = iteration_idx + 1; 
					}
					}
				}
				#endif  */
				
				// transfer export to imports
				#if (NUM_BBBBBB_FPGAS>1)
				for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ 
					if(action[fpga].id_export != INVALID_IOBUFFER_ID){	
						for(unsigned int k=0; k<EXPORT_BATCH_SIZE; k++){ 
							if(action[fpga].id_export + k < universalparams.NUM_UPARTITIONS){ // NEWCHANGE.
								mapping_t map = mapping[fpga][action[fpga].id_export + k];
								if(map.fpga == INVALIDDATA){ continue; } // NEWCHANGE
								#ifdef _DEBUGMODE_HOSTPRINTS//4
								if(fpga < num_prints2 && (profiling0 == true)){ cout<< TIMINGRESULTSCOLOR << "<-> mapping exported vpartition: from (fpga: "<<fpga<<", id_export: "<<action[fpga].id_export + k<<") --- to (mapped_fpga: "<<map.fpga<<", id_mapped_import: "<<map.io_id<<") --- for iteration "<<iteration_idx + 1<< RESET <<endl; }
								#endif 
								HBM_xxx_ptr[fpga][action[fpga].id_export + k] = map;
								gas_import[map.fpga][map.io_id].ready_for_import += 1; 
								gas_import[map.fpga][map.io_id].iteration = iteration_idx + 1; 
							}
						}
					}
				}
				#endif 
				#endif 
				double end_time8 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time8).count()) / 1000;	
				if(profiling1_timing == true){ std::cout << TIMINGRESULTSCOLOR << ">>> host::post-process time elapsed : "<<end_time8<<" ms, "<<(end_time8 * 1000)<<" microsecs, "<< RESET << std::endl; }
				
				run_idx += 1;
				// exit(EXIT_SUCCESS);
			}
			double end_time1 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time1).count()) / 1000;	
			std::cout << TIMINGRESULTSCOLOR <<">>> total kernel time elapsed for current iteration : "<<end_time1<<" ms, "<<(end_time1 * 1000)<<" microsecs, "<< RESET << std::endl;
		
			active_vertices_in_iteration[1][iteration_idx].A = universalparams.NUM_VERTICES;
			active_vertices_in_iteration[1][iteration_idx].B = end_time1;
		}
		#endif 
		
		#ifdef _DEBUGMODE_HOSTPRINTS//4
		for(unsigned int fpga=0; fpga<NUM_FPGAS; fpga++){ std::cout<<">>> imports @ fpga "<<fpga<<": "; for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS / NUM_FPGAS; t++){ std::cout<<gas_import[fpga][t].ready_for_import<<", "; } cout<<endl; }
		#endif 
	}
	double end_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time).count()) / 1000;	
	std::cout << TIMINGRESULTSCOLOR <<">>> total kernel time elapsed for all iterations : "<<end_time<<" ms, "<<(end_time * 1000)<<" microsecs, "<< RESET << std::endl;
	
	unsigned int total_vertices_processed = 0; for(unsigned int iter=0; iter<num_iterations; iter++){ total_vertices_processed += vertices_processed[iter]; cout<<"host:: number of active vertices in iteration "<<iter<<": "<<(unsigned int)vertices_processed[iter]<<endl; } cout<<"host:: total: "<<total_vertices_processed<<endl;
	unsigned int total_edges_processed = 0; for(unsigned int iter=0; iter<num_iterations; iter++){ total_edges_processed += edges_processed[iter]; cout<<"host:: number of edges processed in iteration "<<iter<<": "<<(unsigned int)edges_processed[iter]<<endl; } cout<<"host:: total: "<<total_edges_processed<<endl;
	
	// Wait for all of the OpenCL operations to complete
    printf("Waiting...\n");
	#ifdef FPGA_IMPL
	for(unsigned int fpga=0; fpga<device_count; fpga++){
		OCL_CHECK(err, err = q[fpga].flush());
		OCL_CHECK(err, err = q[fpga].finish());
	}
	#endif 
	
	cout<<"[READ_FRONTIERS, PROCESSEDGES, READ_DESTS, APPLYUPDATES, COLLECT_FRONTIERS, SAVE_DEST, GATHER_FRONTIERS]"<<endl;																									
	cout<<">>> [";
	cout<<(report_statistics[___CODE___READ_FRONTIER_PROPERTIES___] * EDGE_PACK_SIZE) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___ECPROCESSEDGES___] * EDGE_PACK_SIZE) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___READ_DEST_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___APPLYUPDATES___] * EDGE_PACK_SIZE) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___SAVE_DEST_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___GATHER_FRONTIERINFOS___] * EDGE_PACK_SIZE) / num_iterations<<"";
	cout<<"][Per FPGA / iteration]"<<endl;
	cout<<">>> [";
	cout<<(report_statistics[___CODE___READ_FRONTIER_PROPERTIES___] * EDGE_PACK_SIZE * NUM_FPGAS) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___ECPROCESSEDGES___] * EDGE_PACK_SIZE * NUM_FPGAS) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___READ_DEST_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs * NUM_FPGAS) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___APPLYUPDATES___] * EDGE_PACK_SIZE * NUM_FPGAS) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs * NUM_FPGAS) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___SAVE_DEST_PROPERTIES___] * EDGE_PACK_SIZE * NUM_PEs * NUM_FPGAS) / num_iterations<<", ";
	cout<<(report_statistics[___CODE___GATHER_FRONTIERINFOS___] * EDGE_PACK_SIZE * NUM_FPGAS) / num_iterations<<"";
	cout<<"][Per Cluster / iteration]"<<endl<<endl;
	
	// sw kernel 
	#ifdef ___NOT_YET_IMPLEMENTED___
	actvvs.clear(); actvvs_nextit.clear(); 
	actvvs.push_back(1);
	for(unsigned int i=0; i<128; i++){ vertices_processed[i] = 0; edges_processed[i] = 0; } 
	for(unsigned int i=0; i<universalparams.NUM_VERTICES; i++){ vdatas[i] = 0xFFFFFFFF; }
	for(unsigned int i=0; i<actvvs.size(); i++){ vdatas[actvvs[i]] = 0; }
	for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ vpartition_stats[t].A = 0; vpartition_stats[t].B = 0; }
	
	for (unsigned int iteration_idx = 0; iteration_idx < 16; iteration_idx++) {
		std::chrono::steady_clock::time_point begin_time0 = std::chrono::steady_clock::now();
		
		for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ vpartition_stats[t].A = 0; vpartition_stats[t].B = 0; }
		traverse_graph_in_iteration(iteration_idx, actvvs, actvvs_nextit, vertexptrbuffer, edgedatabuffer, vdatas, vpartition_stats, vertices_processed, edges_processed, universalparams);
		unsigned int num_actv_edges = 0; for(unsigned int t=0; t<universalparams.NUM_UPARTITIONS; t++){ num_actv_edges += vpartition_stats[t].B; }
		if(actvvs_nextit.size() == 0){ cout<<"no more activer vertices to process. breaking out... "<<endl; break; }
		actvvs.clear(); for(unsigned int i=0; i<actvvs_nextit.size(); i++){ actvvs.push_back(actvvs_nextit[i]); } actvvs_nextit.clear();
		
		double end_time0 = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time0).count()) / 1000;	
		cout<<"host: end of iteration "<<iteration_idx<<": "<<actvvs_nextit.size()<<" active vertices generated, "<<num_actv_edges<<" edges processed in ("<<end_time0<<" ms, "<<(end_time0 * 1000)<<" microsecs)"<<endl;
	}
	#endif 
	
	cout<<"host:: FPGA-Only mode"<<endl;
	unsigned int total_time = 0;
	for(unsigned int t=0; t<num_iterations; t++){ 
		total_time += active_vertices_in_iteration[1][t].B;
		cout<<"--- "<<active_vertices_in_iteration[1][t].A<<" active vertices processed in iteration "<<t<<" in "<<active_vertices_in_iteration[1][t].B<<" ms  [FPGA]"<<endl; 
	}
	std::cout << TIMINGRESULTSCOLOR <<">>> total kernel time elapsed for all iterations : "<<total_time<<" ms, "<<(total_time * 1000)<<" microsecs, "<< RESET << std::endl;
	if(all_vertices_active_in_all_iterations == false){ 
		cout<<"host:: Software-Only mode"<<endl;
		unsigned int total_time = 0;
		for(unsigned int t=0; t<num_iterations; t++){ 
			total_time += active_vertices_in_iteration[0][t].B;
			cout<<"--- "<<active_vertices_in_iteration[0][t].A<<" active vertices processed in iteration "<<t<<" in "<<active_vertices_in_iteration[0][t].B<<" ms  [SW]"<<endl; 
		}
		std::cout << TIMINGRESULTSCOLOR <<">>> total kernel time elapsed for all iterations : "<<total_time<<" ms, "<<(total_time * 1000)<<" microsecs, "<< RESET << std::endl;
		
		cout<<"host:: Hybrid (Software-FPGA) mode"<<endl;
		total_time = 0;
		for(unsigned int t=0; t<num_iterations; t++){ 
			if(active_vertices_in_iteration[0][t].A < ___hybrid___engine___vertex___threshold___){ 
				total_time += active_vertices_in_iteration[0][t].B;
				cout<<"--- "<<active_vertices_in_iteration[0][t].A<<" active vertices processed in iteration "<<t<<" in "<<active_vertices_in_iteration[0][t].B<<" ms  [SW]"<<endl; 
			} else { 
				total_time += active_vertices_in_iteration[1][t].B;
				cout<<"--- "<<active_vertices_in_iteration[1][t].A<<" active vertices processed in iteration "<<t<<" in "<<active_vertices_in_iteration[1][t].B<<" ms  [FPGA]"<<endl; 		
			}
		} 
		std::cout << TIMINGRESULTSCOLOR <<">>> total kernel time elapsed for all iterations : "<<total_time<<" ms, "<<(total_time * 1000)<<" microsecs, "<< RESET << std::endl;
	}

	// Copy Result from Device Global Memory to Host Local Memory
	#ifdef FPGA_IMPL
	for(unsigned int fpga=0; fpga<device_count; fpga++){ _migrate_device_to_host(&q[fpga], err, fpga, buffer_hbm, buffer_hbmc); }	
	#endif 
			
	// Wait for all of the OpenCL operations to complete
	#ifdef FPGA_IMPL
    printf("Waiting...\n");
	for(unsigned int fpga=0; fpga<device_count; fpga++){
		OCL_CHECK(err, err = q[fpga].flush());
		OCL_CHECK(err, err = q[fpga].finish());
	}
	#endif

	printf("TEST %s\n", "PASSED");
    return EXIT_SUCCESS;
}

