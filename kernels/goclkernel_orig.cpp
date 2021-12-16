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
#include "../acts/acts/actssync.h"
#include "../src/stats/stats.h"
#include "../src/algorithm/algorithm.h"
#include "../src/graphs/graph.h"
#include "../src/utility/utility.h"
#include "../include/common.h"
#include "../include/config_params.h"
#ifdef FPGA_IMPL
#include "xcl2.hpp"
#endif 
#include "goclkernel.h"
using namespace std;

// other actssync impls at ActsofAGraph69 & 70 backups

//HBM Banks requirements
#define MAX_HBM_BANKCOUNT 32
#define BANK_NAME(n) n | XCL_MEM_TOPOLOGY

#define ENABLE_ACTSPROC
// #define ENABLE_ACTSSYNC // NOTE: if this is used, adjust the value of context['NUMSYNCTHREADS'] in gen.py to 16

#define LENGTH PADDEDKVSOURCEDRAMSZ // 1024

goclkernel::goclkernel(graph * _graphobj, algorithm * _algorithmobj, stats * _statsobj){
	utilityobj = new utility();
	statsobj = _statsobj;
	graphobj = _graphobj;
	algorithmobj = _algorithmobj;
	#ifndef ACTS_1by1
	// kernelobjs_synchronize = new actssync();
	#endif 
}
goclkernel::~goclkernel(){} 

long double goclkernel::getaveragetimeelapsed(long double kerneltimelapse_ms[NUMSUBCPUTHREADS]){
	long double av = 0;
	
	long double count = NUMSUBCPUTHREADS;
	unsigned int startind = 1;
	unsigned int numinds = NUMSUBCPUTHREADS-1;
	if(count == 1){ startind = 0; numinds = NUMSUBCPUTHREADS; }
	else { startind = 1; numinds = NUMSUBCPUTHREADS-1; }
	
	for(unsigned int i=startind; i<NUMSUBCPUTHREADS; i++){ // FIXME. 0 adds some unwanted latency
		av += kerneltimelapse_ms[i];
	}
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
	cout<<"goclkernel::getaveragetimeelapsed: average time elapsed: "<<av / (NUMSUBCPUTHREADS-1)<<endl;
	#endif 
	
	/* #if NUMSUBCPUTHREADS==1
	
	#else 
	return av / (NUMSUBCPUTHREADS-1);
	#endif  */
	return av / numinds;
}

#ifdef FPGA_IMPL
const int bank[MAX_HBM_BANKCOUNT] = {
    BANK_NAME(0),  BANK_NAME(1),  BANK_NAME(2),  BANK_NAME(3),  BANK_NAME(4),
    BANK_NAME(5),  BANK_NAME(6),  BANK_NAME(7),  BANK_NAME(8),  BANK_NAME(9),
    BANK_NAME(10), BANK_NAME(11), BANK_NAME(12), BANK_NAME(13), BANK_NAME(14),
    BANK_NAME(15), BANK_NAME(16), BANK_NAME(17), BANK_NAME(18), BANK_NAME(19),
    BANK_NAME(20), BANK_NAME(21), BANK_NAME(22), BANK_NAME(23), BANK_NAME(24),
    BANK_NAME(25), BANK_NAME(26), BANK_NAME(27), BANK_NAME(28), BANK_NAME(29),
    BANK_NAME(30), BANK_NAME(31)};
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
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
    printf("[%s]: %s %s\n",
           reinterpret_cast<char *>(data),
           status_str,
           command_str);
	#endif 
    fflush(stdout);
}
void set_callback2(cl::Event event, const char *queue_name){
    cl_int err;
    OCL_CHECK(err,
              err =
                  event.setCallback(CL_COMPLETE, event_cb2, (void *)queue_name));
}

long double goclkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], long double timeelapsed_totals[128][8], unsigned int numValidIters){				
	cout<<">>> goclkernel::runapp:: runapp started."<<endl;
	long double total_time_elapsed = 0;
	long double avs_proc[128][8]; //[GraphIter][CLOP]
	long double avs_sync[32];
	for(unsigned int i=0; i<128; i++){ for(unsigned int j=0; j<8; j++){ avs_proc[i][j] = 0; }}
	for(unsigned int i=0; i<32; i++){ avs_sync[i] = 0; }
	for(unsigned int i=0; i<128; i++){ for(unsigned int j=0; j<8; j++){ timeelapsed_totals[i][j] = 0; }}
	
	unsigned int A = edges[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key;
	unsigned int B = edges[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key / VECTOR_SIZE;
	unsigned int C = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key;
	unsigned int D = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key / VECTOR_SIZE;
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; } // reset
	unsigned int baseoffsetkvs_verticesdata = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA].data[0].key;
	unsigned int edgessz_kvs = A + B;
	unsigned int kvdramsz_kvs = C + D;
	// unsigned int edgessz_kvs = KVSOURCEDRAMSZ_KVS;  // CRITICAL REMOVEME.
	// unsigned int kvdramsz_kvs = KVSOURCEDRAMSZ_KVS;
	cout<<">>> goclkernel::runapp:: edgessz: "<<edgessz_kvs*VECTOR_SIZE<<" (edgessz_kvs: "<<edgessz_kvs*VECTOR_SIZE*sizeof(keyvalue_t)<<"  bytes), kvdramsz: "<<kvdramsz_kvs*VECTOR_SIZE<<" (kvdramsz: "<<kvdramsz_kvs*VECTOR_SIZE*sizeof(keyvalue_t)<<" bytes)"<<endl;							
	
	unsigned int analysis_i = 0;
	unsigned int analysis_begincount;
	unsigned int analysis_icount;
	#ifdef ENABLE_KERNEL_PROFILING
	analysis_icount = 1;//3;
		#if defined(ACTS_PARTITION_AND_REDUCE_STRETEGY)
		analysis_begincount = 0;
		#elif defined(BASIC_PARTITION_AND_REDUCE_STRETEGY)
		analysis_begincount = 0;
		#elif defined(TRAD_PARTITION_AND_REDUCE_STRETEGY)
		analysis_begincount = 2;
		#else 
		analysis_begincount = 0;
		#endif 
	#else 
	analysis_icount = 1;
	#endif
	unsigned int _PROCESSCOMMAND = ON; unsigned int _PARTITIONCOMMAND = ON; unsigned int _APPLYUPDATESCOMMAND = ON;
	
	cl_int err;
    auto devices = xcl::get_xil_devices();
    auto device = devices[0];

    //Creating Context and Command Queue for selected Device
    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
	OCL_CHECK(err,
              cl::CommandQueue q(context,
                                 device,
                                 CL_QUEUE_PROFILING_ENABLE |
                                     CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
                                 &err));
	/* OCL_CHECK( // CRITICAL RESTOREME.
        err,
        cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err)); */
    std::string device_name = device.getInfo<CL_DEVICE_NAME>();
    std::cout << "Found Device=" << device_name.c_str() << std::endl;
	
	auto fileBuf_proc = xcl::read_binary_file(binaryFile[0]);
	// auto fileBuf_sync = xcl::read_binary_file(binaryFile[1]);

    int vector_length = LENGTH;
    bool match = true;
	
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	cout<<">>> goclkernel[actsproc]:: running ACTS PROCESS (Switching kernel args)..."<<endl;
	#endif 
	
	// cl_int err;
	std::vector<cl_mem_ext_ptr_t> inoutBufExt_edges(32);
	std::vector<cl_mem_ext_ptr_t> inoutBufExt_kvdram(32);
	cl_mem_ext_ptr_t inoutBufExt_vdram;
	std::vector<cl::Buffer> buffer_kvdram(32);
	std::vector<cl::Buffer> buffer_edgesdram(32);
	cl::Buffer buffer_vdram;
	std::vector<cl::Kernel> krnls_proc(32);
	cl::Kernel krnls_sync;
	std::vector<cl::Event> kernel_events(32);
	std::vector<cl::Event> write_events(32);
	std::vector<cl::Event> read_events(32);
	
	long double prockerneltimelapses_ms[NUMSUBCPUTHREADS]; for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ prockerneltimelapses_ms[i] = 0; }
	long double prockerneltimelapse_ms = 0;
	long double synkerneltimelapse_ms = 0;
	long double procandsynkerneltimelapse_ms = 0;
	long double WALLprocandsynkerneltimelapse_ms = 0;
	long double totalprocandsynkerneltimelapse_ms = 0;
	long double totalWALLprocandsynkerneltimelapse_ms = 0;
	long double endtoendWALLkerneltimelapse_ms = 0;

	unsigned int kvindex = 0;
	unsigned int edgesindex = 0;
	unsigned int hbmindex = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i+=NUM_EDGE_BANKS){ 
		for(unsigned int j=0; j<NUM_EDGE_BANKS; j++){ 
			inoutBufExt_edges[edgesindex].obj = edges[edgesindex];
			inoutBufExt_edges[edgesindex].param = 0;
			inoutBufExt_edges[edgesindex].flags = bank[hbmindex];
			cout<<"attaching inoutBufExt_edges["<<edgesindex<<"] to Edges["<<edgesindex<<"] to HBM["<<hbmindex<<"]: "<<endl;
			edgesindex += 1;
			hbmindex += 1;
		}
		inoutBufExt_kvdram[kvindex].obj = kvsourcedram[kvindex];
		inoutBufExt_kvdram[kvindex].param = 0;
		inoutBufExt_kvdram[kvindex].flags = bank[hbmindex];
		cout<<"attaching inoutBufExt_kvdram["<<kvindex<<"] to Kvdram["<<kvindex<<"] to HBM["<<hbmindex<<"]: "<<endl;
		kvindex += 1;
		hbmindex += 1;
	}
	inoutBufExt_vdram.obj = vdram;
	inoutBufExt_vdram.param = 0;
	inoutBufExt_vdram.flags = bank[hbmindex];
	cout<<"attaching inoutBufExt_vdram to vdram to HBM["<<hbmindex<<"]: "<<endl;
	
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	cout<<"goclkernel:: creating OCL buffers..."<<endl;
	#endif
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		OCL_CHECK(err,
			  buffer_edgesdram[i] =
				  cl::Buffer(context,
							 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
							 sizeof(uint512_vec_dt) * edgessz_kvs,
							 &inoutBufExt_edges[i], 
							 &err));
		cout<<"+++ goclkernel: created buffer_edgesdram["<<i<<"] "<<endl;
	}
	for(unsigned int i=0; i<NUMSUBCPUTHREADS / NUM_EDGE_BANKS; i++){						 
		OCL_CHECK(err,
			  buffer_kvdram[i] =
				  cl::Buffer(context,
							 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
							 sizeof(uint512_vec_dt) * kvdramsz_kvs,
							 &inoutBufExt_kvdram[i], 
							 &err));
		cout<<"+++ goclkernel: created buffer_kvdram["<<i<<"] "<<endl;
	}
	OCL_CHECK(err,
			  buffer_vdram =
				  cl::Buffer(context,
							 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
							 sizeof(uint512_vec_dt) * kvdramsz_kvs,
							 &inoutBufExt_vdram, 
							 &err));
	cout<<"+++ goclkernel: created buffer_vdram "<<endl;
	
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	printf("INFO: loading vmul kernel\n");
	#endif
	cl::Program::Binaries vmul_bins{{fileBuf_proc.data(), fileBuf_proc.size()}};
	devices.resize(1);
	OCL_CHECK(err,
			  cl::Program program(context, devices, vmul_bins, NULL, &err));
	
	// Create kernel objects
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	cout<<"goclkernel:: creating kernel object..."<<endl;
	#endif
	for (int i = 0; i < NUMSUBCPUTHREADS / NUM_EDGE_BANKS; i++) {
		std::string krnl_name = "topkernelproc"; //
		std::string cu_id = std::to_string((i+1));
		std::string krnl_name_full = krnl_name + ":{" + "topkernelproc_" + cu_id + "}"; 
		#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
		printf("Creating a kernel [%s] for CU(%d)\n",
			   krnl_name_full.c_str(),
			   (i+1));
		#endif
		OCL_CHECK(err, krnls_proc[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	}
	
	std::string krnl_name = "topkernelsync";
	std::string cu_id = std::to_string((1));
	std::string krnl2_name_full = krnl_name + ":{" + "topkernelsync_" + cu_id + "}"; 
	OCL_CHECK(err, krnls_sync = cl::Kernel(program, krnl2_name_full.c_str(), &err));
	
	// Setting kernel arguments...
	cout<<"goclkernel:: Setting kernel (proc) arguments..."<<endl;
	kvindex = 0;
	unsigned int argindex = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i+=NUM_EDGE_BANKS){ 
		for(unsigned int j=0; j<NUM_EDGE_BANKS; j+=1){ 
			OCL_CHECK(err, err = krnls_proc[kvindex].setArg(argindex, buffer_edgesdram[i+j]));
			cout<<"setting argument krnls_proc["<<kvindex<<"].setArg("<<argindex<<", buffer_edgesdram["<<i+j<<"])"<<endl;
			argindex += 1;
		}
		OCL_CHECK(err, err = krnls_proc[kvindex].setArg(argindex, buffer_kvdram[kvindex]));
		cout<<"setting argument krnls_proc["<<kvindex<<"].setArg("<<argindex<<", buffer_kvdram["<<kvindex<<"])"<<endl;
		kvindex += 1;
		argindex = 0;
	}
	OCL_CHECK(err, err = q.finish());
	
	// Setting kernel arguments...
	cout<<"goclkernel:: Setting kernel (sync) arguments..."<<endl;
	for(unsigned int index=0; index<NUMSUBCPUTHREADS / NUM_EDGE_BANKS; index++){ 
		OCL_CHECK(err, err = krnls_sync.setArg(index, buffer_kvdram[index]));
		cout<<"setting argument krnls_sync.setArg("<<index<<", buffer_kvdram["<<index<<"])"<<endl;
	}
	OCL_CHECK(err, err = krnls_sync.setArg(NUMSUBCPUTHREADS / NUM_EDGE_BANKS, buffer_vdram));
	cout<<"setting argument krnls_sync.setArg("<<NUMSUBCPUTHREADS / NUM_EDGE_BANKS<<", buffer_vdram)"<<endl;
	OCL_CHECK(err, err = q.finish());
	
	// Migrating input data to device global memory...
	cout<<"goclkernel:: Migrating input data to device global memory..."<<endl;
	kvindex = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i+=NUM_EDGE_BANKS){
		#if NUM_EDGE_BANKS==1
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_kvdram[kvindex]}, 0, NULL, &write_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==2
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_kvdram[kvindex]}, 0, NULL, &write_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==3
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_edgesdram[i+2], buffer_kvdram[kvindex]}, 0, NULL, &write_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==4
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_edgesdram[i+2], buffer_edgesdram[i+3], buffer_kvdram[kvindex], buffer_vdram}, 0, NULL, &write_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==5
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_edgesdram[i+2], buffer_edgesdram[i+3], buffer_edgesdram[i+4], buffer_kvdram[kvindex]}, 0, NULL, &write_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==6
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_edgesdram[i+2], buffer_edgesdram[i+3], buffer_edgesdram[i+4], buffer_edgesdram[i+5], buffer_kvdram[kvindex]}, 0, NULL, &write_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==7
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_edgesdram[i+2], buffer_edgesdram[i+3], buffer_edgesdram[i+4], buffer_edgesdram[i+5], buffer_edgesdram[i+6], buffer_kvdram[kvindex]}, 0, NULL, &write_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS>7
		cout<<"goclkernel:enqueueMigrateMemObjects: ERROR. NOT YET IMPLEMENTED. exiting...."<<endl; exit(EXIT_FAILURE);
		#endif 
		OCL_CHECK(err, err = write_events[kvindex].wait()); // CRITICAL RESTOREME.
		kvindex += 1;
	}
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_vdram}, 0));
	OCL_CHECK(err, err = q.finish());
	
	std::chrono::steady_clock::time_point beginkerneltime4 = std::chrono::steady_clock::now();
	for(unsigned int GraphIter=0; GraphIter<numValidIters; GraphIter++){ // numIters, numValidIters
		
		// Processing phase started...
		cout<< TIMINGRESULTSCOLOR << "goclkernel:: Processing phase (Iteration: "<<GraphIter<<" of "<<numValidIters<<") started..."<< RESET <<endl;
		std::chrono::steady_clock::time_point beginkerneltime3 = std::chrono::steady_clock::now();
		for(unsigned int i=0; i<NUMSUBCPUTHREADS / NUM_EDGE_BANKS; i++){
			std::chrono::steady_clock::time_point beginkerneltime1 = std::chrono::steady_clock::now();	

			// Launch the kernel
			OCL_CHECK(err, err = q.enqueueTask(krnls_proc[i], NULL, &kernel_events[i])); 
			
			#ifdef _DEBUGMODE_TIMERS2
			OCL_CHECK(err, err = kernel_events[i].wait()); // REMOVEME.
			prockerneltimelapses_ms[i] = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime1).count()) / 1000;			
			std::cout <<">>> kernel (proc["<<i<<"]) time elapsed for iteration "<<GraphIter<<": "<<prockerneltimelapses_ms[i]<<" ms, "<<(prockerneltimelapses_ms[i] * 1000)<<" microsecs, "<<std::endl;
			#endif
		}
		for(unsigned int i=0; i<NUMSUBCPUTHREADS / NUM_EDGE_BANKS; i++){ OCL_CHECK(err, err = kernel_events[i].wait()); }
		#ifdef _DEBUGMODE_TIMERS2
		prockerneltimelapse_ms = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime3).count()) / 1000;	
		std::cout <<">>> kernel (proc[1 to "<<NUMSUBCPUTHREADS / NUM_EDGE_BANKS<<"]) time elapsed for " << TIMINGRESULTSCOLOR <<" iteration "<<GraphIter<<": "<<prockerneltimelapse_ms<<" ms, " <<(prockerneltimelapse_ms * 1000)<<" microsecs, " << RESET << std::endl;
		#endif 
		
		// Apply phase started...
		cout<<"goclkernel:: Apply phase (Iteration: "<<GraphIter<<" of "<<numValidIters<<") started..."<<endl;
		std::chrono::steady_clock::time_point beginkerneltime2 = std::chrono::steady_clock::now();	
		OCL_CHECK(err, err = q.enqueueTask(krnls_sync, NULL, &kernel_events[0])); 
		OCL_CHECK(err, err = kernel_events[0].wait());
		#ifdef _DEBUGMODE_TIMERS2
		synkerneltimelapse_ms = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime2).count()) / 1000;			
		std::cout <<">>> kernel (sync) time elapsed for iteration "<<GraphIter<<": "<<synkerneltimelapse_ms<<" ms, "<<(synkerneltimelapse_ms * 1000)<<" microsecs, "<<std::endl;
		#endif
		
		#ifdef _DEBUGMODE_TIMERS2
		procandsynkerneltimelapse_ms = prockerneltimelapses_ms[(NUMSUBCPUTHREADS / NUM_EDGE_BANKS)-1];// + (synkerneltimelapse_ms / 2); /// FIXME.
		WALLprocandsynkerneltimelapse_ms = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime3).count()) / 1000;	
		std::cout <<"### kernel (proc & syn) time elapsed for iteration "<<GraphIter<<": "<<procandsynkerneltimelapse_ms<<" ms, "<<(procandsynkerneltimelapse_ms * 1000)<<" microsecs, "<<std::endl;
		#endif 
		#ifdef _DEBUGMODE_TIMERS2
		totalprocandsynkerneltimelapse_ms += procandsynkerneltimelapse_ms;
		totalWALLprocandsynkerneltimelapse_ms += WALLprocandsynkerneltimelapse_ms;
		#endif 
	}
	
	#ifdef _DEBUGMODE_TIMERS2
	endtoendWALLkerneltimelapse_ms = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime4).count()) / 1000;			
	std::cout <<"+++ total kernel time elapsed for all iterations: "<<totalprocandsynkerneltimelapse_ms<<" ms, "<<(totalprocandsynkerneltimelapse_ms * 1000)<<" microsecs "<<std::endl;
	std::cout <<"+++ total kernel time elapsed for all iterations: "<<totalWALLprocandsynkerneltimelapse_ms<<" ms, "<<(totalWALLprocandsynkerneltimelapse_ms * 1000)<<" microsecs "<<std::endl;
	std::cout <<"+++ total kernel time elapsed (end-to-end) for all iterations: "<<endtoendWALLkerneltimelapse_ms<<" ms, "<<(endtoendWALLkerneltimelapse_ms * 1000)<<" microsecs "<<std::endl;
	#endif 
	
	// Finish
	OCL_CHECK(err, err = q.finish());
	
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	cout<<"goclkernel:: migrating workload back to HOST..."<<endl;
	#endif
	kvindex = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i+=NUM_EDGE_BANKS){
		// OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i], buffer_kvdram[i]},					
																	// CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[i]));
		#if NUM_EDGE_BANKS==1
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_kvdram[kvindex]}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==2
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_kvdram[kvindex]}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==3
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_edgesdram[i+2], buffer_kvdram[kvindex]}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==4
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_edgesdram[i+2], buffer_edgesdram[i+3], buffer_kvdram[kvindex]}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==5
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_edgesdram[i+2], buffer_edgesdram[i+3], buffer_edgesdram[i+4], buffer_kvdram[kvindex]}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==6
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_edgesdram[i+2], buffer_edgesdram[i+3], buffer_edgesdram[i+4], buffer_edgesdram[i+5], buffer_kvdram[kvindex]}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[kvindex]));
		#endif
		#if NUM_EDGE_BANKS==7
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_edgesdram[i+0], buffer_edgesdram[i+1], buffer_edgesdram[i+2], buffer_edgesdram[i+3], buffer_edgesdram[i+4], buffer_edgesdram[i+5], buffer_edgesdram[i+6], buffer_kvdram[kvindex]}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[kvindex]));					
		#endif
		#if NUM_EDGE_BANKS>7
		cout<<"goclkernel:enqueueMigrateMemObjects: ERROR. NOT YET IMPLEMENTED. exiting...."<<endl; exit(EXIT_FAILURE);
		#endif 
		OCL_CHECK(err, err = read_events[kvindex].wait()); // CRITICAL RESTOREME.
		kvindex += 1;
	}
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_vdram}, CL_MIGRATE_MEM_OBJECT_HOST));
	OCL_CHECK(err, err = q.finish());
	
	timeelapsed_totals[0][0] = totalprocandsynkerneltimelapse_ms;
	// timeelapsed_totals[0][0] = endtoendWALLkerneltimelapse_ms; 
	total_time_elapsed = timeelapsed_totals[0][0];
	return total_time_elapsed;
}
#endif

unsigned int goclkernel::ExitCheck(uint512_vec_dt * kvsourcedram, unsigned int GraphIter){
	unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvsourcedram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	unsigned int BLOP = pow(NUM_PARTITIONS, (TREE_DEPTH-1));
	unsigned int totalactvvp = 0;
	cout<<endl<<"active partitions for iteration "<<GraphIter+1<<": ";
	for(unsigned int i=0; i<256; i++){
		unsigned int gmask = kvsourcedram[_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].data[0].key;
		totalactvvp += gmask;
		if(gmask > 0){ cout<<i<<", "; }
	}
	cout<<""<<endl;
	if(totalactvvp == 0){ return 1; } 
	else { return 0; }
}

void goclkernel::verifyresults_hw(uint512_vec_dt * kvbuffer, unsigned int baseoffsetkvs_verticesdata){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"goclkernel::verifyresults_hw: verifying vertex data... "<<endl;
	#endif
	
	unsigned int vdatas[64];
	for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	
	uint512_vec_dt buff[REDUCEBUFFERSZ];
	for(unsigned int offset_kvs=0; offset_kvs<VERTICESDATASZ_KVS; offset_kvs+=REDUCEBUFFERSZ){
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
			buff[i] = kvbuffer[baseoffsetkvs_verticesdata + offset_kvs + i];
		}
		
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				unsigned int pos = i*VECTOR_SIZE + v;
				unsigned int vid1 = offset_kvs*REDUCESZ*VECTOR_SIZE + v*REDUCESZ + i*2;
				unsigned int vid2 = vid1 + 1;
				unsigned int vdata1 = buff[i].data[v].key;
				unsigned int vdata2 = buff[i].data[v].value;
				
				if(vdata1 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS
					cout<<"goclkernel:verifyresults_hw: vid1: "<<vid1<<",vdata1: "<<vdata1<<endl;
					#endif 
					vdatas[vdata1] += 1; 
				}
				if(vdata2 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS
					cout<<"goclkernel:verifyresults_hw: vid2: "<<vid2<<",vdata2: "<<vdata2<<endl;
					#endif
					vdatas[vdata2] += 1; 
				}
			}
		}
	}
	utilityobj->printvalues("goclkernel::verifyresults_hw.vdatas: verifying results after kernel run", vdatas, 16);
	return;
}





