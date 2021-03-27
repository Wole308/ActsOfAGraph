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
	kernelobjs_synchronize = new actssync(); //////
}
goclkernel::~goclkernel(){} 

long double goclkernel::getaveragetimeelapsed(long double kerneltimelapse_ms[NUMSUBCPUTHREADS]){
	long double av = 0;
	for(unsigned int i=1; i<NUMSUBCPUTHREADS; i++){ // FIXME. 0 adds some unwanted latency
		av += kerneltimelapse_ms[i];
	}
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
	cout<<"goclkernel::getaveragetimeelapsed: average time elapsed: "<<av / (NUMSUBCPUTHREADS-1)<<endl;
	#endif 
	return av / (NUMSUBCPUTHREADS-1);
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
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
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

long double goclkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){	
	#if NUMCOMPUTEUNITS>20
	return runapp2(binaryFile, vdram, kvsourcedram);
	#else 
	return runapp1(binaryFile, vdram, kvsourcedram);	
	#endif
}

long double goclkernel::runapp1(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){		
	long double total_time_elapsed = 0;
	long double avs_proc[32];
	long double avs_sync[32];

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
	
	auto fileBuf_proc = xcl::read_binary_file(binaryFile[0]);
	auto fileBuf_sync = xcl::read_binary_file(binaryFile[1]);

    int vector_length = LENGTH;
    bool match = true;
	inputdata_size_bytes = PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt);
	
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
		cout<<"goclkernel::runapp1: Iteration "<<GraphIter<<endl;
		#endif 
		
		#ifdef ENABLE_ACTSPROC
		{
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"--------------- goclkernel[actssync]:: running ACTS PROCESS (Iteration "<<GraphIter<<")... ---------------"<<endl;
			#endif 
			
			cl_int err;
			std::vector<cl_mem_ext_ptr_t> inoutBufExt(32);
			std::vector<cl::Buffer> buffer_kvsourcedram(32);
			std::vector<cl::Kernel> krnls(32);
			
			vector<cl::Event> kernel_events;
			kernel_events.resize(32);
			
			long double kerneltimelapse_ms[NUMSUBCPUTHREADS];
			
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			printf("INFO: loading vmul kernel\n");
			#endif
			cl::Program::Binaries vmul_bins{{fileBuf_proc.data(), fileBuf_proc.size()}};
			devices.resize(1);
			OCL_CHECK(err,
					  cl::Program program(context, devices, vmul_bins, NULL, &err));
			
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: creating kernel object..."<<endl;
			#endif
			std::string krnl_name = "topkernelproc"; //
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
					std::string cu_id = std::to_string((i+1));
					std::string krnl_name_full = krnl_name + ":{" + "topkernelproc_" + cu_id + "}"; 

					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					printf("Creating a kernel [%s] for CU(%d)\n",
						   krnl_name_full.c_str(),
						   (i+1));
					#endif

					OCL_CHECK(err,
							  krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
			}

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: creating OCL buffers..."<<endl;
			#endif
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
				OCL_CHECK(err,
				  buffer_kvsourcedram[i] =
					  cl::Buffer(context,
								 CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
								 sizeof(uint512_vec_dt) * PADDEDKVSOURCEDRAMSZ_KVS,
								 kvsourcedram[i],
								 &err));
			}

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: setting kernel arguments..."<<endl;
			#endif
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[i])); }

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: migrating workload to FPGA..."<<endl;
			#endif
			#if NUMCOMPUTEUNITS==4
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3]}, 0));
			#endif 
			#if NUMCOMPUTEUNITS==20
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4], buffer_kvsourcedram[5], buffer_kvsourcedram[6], buffer_kvsourcedram[7],
																buffer_kvsourcedram[8], buffer_kvsourcedram[9], buffer_kvsourcedram[10], buffer_kvsourcedram[11], buffer_kvsourcedram[12], buffer_kvsourcedram[13], buffer_kvsourcedram[14], buffer_kvsourcedram[15], 
																	buffer_kvsourcedram[16], buffer_kvsourcedram[17], buffer_kvsourcedram[18], buffer_kvsourcedram[19]},
																		0));
			#endif
			
			// This function will execute the kernel on the FPGA
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: launching the kernel..."<<endl;
			#endif
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
				std::chrono::steady_clock::time_point beginkerneltime = std::chrono::steady_clock::now();	  
				OCL_CHECK(err, err = q.enqueueTask(krnls[i], NULL, &kernel_events[i])); 
				OCL_CHECK(err, err = kernel_events[i].wait());
				long double kerneltimelapse_microsec = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime).count();
				kerneltimelapse_ms[i] = kerneltimelapse_microsec / 1000;
				#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
				std::cout <<">>> kernel (proc) "<<i<<" time elapsed: "<<kerneltimelapse_ms[i]<<" ms, "<<kerneltimelapse_microsec<<" microsecs, "<<std::endl;
				#endif
			}

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: migrating workload back to HOST..."<<endl;
			#endif
			#if NUMCOMPUTEUNITS==4
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3]}, CL_MIGRATE_MEM_OBJECT_HOST));		
			#endif 
			#if NUMCOMPUTEUNITS==20
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4], buffer_kvsourcedram[5], buffer_kvsourcedram[6], buffer_kvsourcedram[7],
																buffer_kvsourcedram[8], buffer_kvsourcedram[9], buffer_kvsourcedram[10], buffer_kvsourcedram[11], buffer_kvsourcedram[12], buffer_kvsourcedram[13], buffer_kvsourcedram[14], buffer_kvsourcedram[15], 
																	buffer_kvsourcedram[16], buffer_kvsourcedram[17], buffer_kvsourcedram[18], buffer_kvsourcedram[19]},
																		CL_MIGRATE_MEM_OBJECT_HOST));
			#endif 
			OCL_CHECK(err, err = q.finish());
			
			avs_proc[GraphIter] = getaveragetimeelapsed(kerneltimelapse_ms);
			total_time_elapsed += avs_proc[GraphIter];
			std::cout <<">>> SUMMARY: kernel (proc) average time elapsed for Iteration "<<GraphIter<<": "<<avs_proc[GraphIter]<<" ms"<<std::endl;
		}
		#endif 
		
		#ifdef ENABLE_ACTSSYNC
		{
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"--------------- goclkernel[actssync]:: running ACTS SYNCHRONZE (Iteration "<<GraphIter<<") ---------------"<<endl;
			#endif 
			
			cl_int err;
			std::vector<cl_mem_ext_ptr_t> inoutBufExt(32);
			std::vector<cl::Buffer> buffer_kvsourcedram(32);
			std::vector<cl::Kernel> krnls(1);
			
			vector<cl::Event> kernel_events;
			kernel_events.resize(1);
			
			long double kerneltimelapse_ms[1];
			
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			printf("INFO: loading vmul kernel\n");
			#endif
			cl::Program::Binaries vmul_bins{{fileBuf_sync.data(), fileBuf_sync.size()}};
			devices.resize(1);
			OCL_CHECK(err,
					  cl::Program program(context, devices, vmul_bins, NULL, &err));
					  
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: creating kernel object..."<<endl;
			#endif
			std::string krnl_name = "topkernelsync";
			for(unsigned int i=0; i<1; i++){ 
					std::string cu_id = std::to_string((i+1));
					std::string krnl_name_full = krnl_name + ":{" + "topkernelsync_" + cu_id + "}"; 

					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					printf("Creating a kernel [%s] for CU(%d)\n",
						   krnl_name_full.c_str(),
						   (i+1));
					#endif

					OCL_CHECK(err,
							  krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
			}

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel[actssync]:: creating OCL buffers..."<<endl;
			#endif
			for(unsigned int i=0; i<NUMSUBCPUTHREADS + 1; i++){ 
				#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
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

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel[actssync]:: setting kernel arguments..."<<endl;
			#endif
			for(unsigned int i=0; i<NUMSUBCPUTHREADS + 1; i++){ OCL_CHECK(err, err = krnls[0].setArg(i, buffer_kvsourcedram[i])); }
			
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel[actssync]:: migrating workload to FPGA..."<<endl;
			#endif
			#if NUMCOMPUTEUNITS==4
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4]}, 0));
			#endif
			#if NUMCOMPUTEUNITS==20
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4], buffer_kvsourcedram[5], buffer_kvsourcedram[6], buffer_kvsourcedram[7],
																buffer_kvsourcedram[8], buffer_kvsourcedram[9], buffer_kvsourcedram[10], buffer_kvsourcedram[11], buffer_kvsourcedram[12], buffer_kvsourcedram[13], buffer_kvsourcedram[14], buffer_kvsourcedram[15], 
																	buffer_kvsourcedram[16], buffer_kvsourcedram[17], buffer_kvsourcedram[18], buffer_kvsourcedram[19], 
																		buffer_kvsourcedram[20]},
																			0));
			#endif
																		
			// This function will execute the kernel on the FPGA
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel[actssync]:: launching the kernel..."<<endl;
			#endif
			std::chrono::steady_clock::time_point beginkerneltime = std::chrono::steady_clock::now();	  
			OCL_CHECK(err, err = q.enqueueTask(krnls[0], NULL, &kernel_events[0])); 
			OCL_CHECK(err, err = kernel_events[0].wait());
			kerneltimelapse_ms[0] = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime).count();
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			std::cout <<">>> kernel (sync) time elapsed: "<<kerneltimelapse_ms[0]/1000<<" ms, "<<kerneltimelapse_ms[0]<<" microsecs, "<<std::endl;
			#endif 
			
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel[actssync]:: migrating workload back to HOST..."<<endl;
			#endif
			#if NUMCOMPUTEUNITS==4
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4]}, CL_MIGRATE_MEM_OBJECT_HOST));
			#endif
			#if NUMCOMPUTEUNITS==20
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4], buffer_kvsourcedram[5], buffer_kvsourcedram[6], buffer_kvsourcedram[7],
																buffer_kvsourcedram[8], buffer_kvsourcedram[9], buffer_kvsourcedram[10], buffer_kvsourcedram[11], buffer_kvsourcedram[12], buffer_kvsourcedram[13], buffer_kvsourcedram[14], buffer_kvsourcedram[15], 
																	buffer_kvsourcedram[16], buffer_kvsourcedram[17], buffer_kvsourcedram[18], buffer_kvsourcedram[19], 
																		buffer_kvsourcedram[20]},
																			CL_MIGRATE_MEM_OBJECT_HOST));
			#endif
			OCL_CHECK(err, err = q.finish());
			
			avs_sync[GraphIter] = 0; // kerneltimelapse_ms[0]; // FIXME. unwanted latency
			total_time_elapsed += avs_sync[GraphIter];
			// std::cout <<">>> SUMMARY: kernel (sync) average time elapsed for Iteration "<<GraphIter<<": "<<avs_sync[GraphIter]<<" ms"<<std::endl;
		}
		#endif
		
		// unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
		// unsigned int BLOP = pow(NUM_PARTITIONS, (TREE_DEPTH-1));
		// unsigned int totalactvvp = 0;
		// cout<<endl<<"active partitions for iteration "<<GraphIter+1<<": ";
		// for(unsigned int i=0; i<256; i++){
			// unsigned int gmask = kvsourcedram[0][_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].data[0].key;
			// totalactvvp += gmask;
			// if(gmask > 0){ cout<<i<<", "; }
		// }
		// cout<<""<<endl;
		// if(totalactvvp == 0){ cout<<"goclkernel::runapp1: no more active vertices to process. exiting... "<<endl; break; }
		if(ExitCheck(kvsourcedram[0], GraphIter) == 1){ cout<<"goclkernel::runapp2: no more active vertices to process. exiting... "<<endl; break; }
	}
	std::cout << TIMINGRESULTSCOLOR <<">>> SUMMARY: Total kernel time: "<<total_time_elapsed<<" ms"<< RESET << std::endl;
	return total_time_elapsed;
}

long double goclkernel::runapp2(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){		
	long double total_time_elapsed = 0;
	long double avs_proc[32];
	long double avs_sync[32];
	
	// Create tempkvbuffer
	tempkvsourcedram = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	for(unsigned int i=0; i<4; i++){ tempkvsourcedrams[i] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }
	tempvdram = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	
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
	
	auto fileBuf_proc = xcl::read_binary_file(binaryFile[0]);
	auto fileBuf_sync = xcl::read_binary_file(binaryFile[1]);

    int vector_length = LENGTH;
    bool match = true;
	inputdata_size_bytes = PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt);
	
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
		cout<<"goclkernel::runapp2: Iteration "<<GraphIter<<endl;
		#endif 
		
		#ifdef ENABLE_ACTSPROC
		{
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"--------------- goclkernel[actssync]:: running ACTS PROCESS (Iteration "<<GraphIter<<")... ---------------"<<endl;
			#endif 
			
			/* ////////////////////////////////////////////////////////////////////////////////////// CRITICAL REMOVEME.
			if(GraphIter==4){
				for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
					kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key = ON;//ON;  
					kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key = OFF;//ON;  
				}
			}
			///////////////////////////////////////////////////////////////////////////////////// */
			
			cl_int err;
			std::vector<cl_mem_ext_ptr_t> inoutBufExt(32);
			cl::Buffer buffer_kvsourcedram;
			cl::Kernel krnls;
			
			cl::Event kernel_events;
			cl::Event write_events;
			cl::Event read_events;
			
			long double kerneltimelapse_ms[NUMSUBCPUTHREADS];
			
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			printf("INFO: loading vmul kernel\n");
			#endif
			cl::Program::Binaries vmul_bins{{fileBuf_proc.data(), fileBuf_proc.size()}};
			devices.resize(1);
			OCL_CHECK(err,
					  cl::Program program(context, devices, vmul_bins, NULL, &err));
			
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: creating kernel object..."<<endl;
			#endif
			std::string krnl_name = "topkernelproc"; //
			std::string cu_id = std::to_string((1));
					std::string krnl_name_full = krnl_name + ":{" + "topkernelproc_" + cu_id + "}"; 

					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					printf("Creating a kernel [%s] for CU(%d)\n",
						   krnl_name_full.c_str(),
						   (1));
					#endif

					OCL_CHECK(err,
							  krnls = cl::Kernel(program, krnl_name_full.c_str(), &err));

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: creating OCL buffers..."<<endl;
			#endif
			OCL_CHECK(err,
				  buffer_kvsourcedram =
					  cl::Buffer(context,
								 CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
								 sizeof(uint512_vec_dt) * PADDEDKVSOURCEDRAMSZ_KVS,
								 tempkvsourcedram,
								 &err));

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: setting kernel arguments..."<<endl;
			#endif
			OCL_CHECK(err, err = krnls.setArg(0, buffer_kvsourcedram));
			
			for(unsigned int inst=0; inst<NUMSUBCPUTHREADS; inst++){
				// write
				for(unsigned int i=0; i<PADDEDKVSOURCEDRAMSZ_KVS; i++){
					for(unsigned int v=0; v<VECTOR_SIZE; v++){
						tempkvsourcedram[i].data[v] = kvsourcedram[inst][i].data[v];
					}
				}
				
				#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
				cout<<"goclkernel:: migrating workload to FPGA..."<<endl;
				#endif
				OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram}, 0, NULL, &write_events));
				OCL_CHECK(err, err = write_events.wait());
				
				// This function will execute the kernel on the FPGA
				#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
				cout<<"goclkernel:: launching the kernel..."<<endl;
				#endif
				std::chrono::steady_clock::time_point beginkerneltime = std::chrono::steady_clock::now();	  
				OCL_CHECK(err, err = q.enqueueTask(krnls, NULL, &kernel_events)); 
				OCL_CHECK(err, err = kernel_events.wait());
				long double kerneltimelapse_microsec = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime).count();			
				kerneltimelapse_ms[inst] = kerneltimelapse_microsec / 1000;
				// #ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
				std::cout <<">>> kernel (proc) "<<inst<<" time elapsed: "<<kerneltimelapse_ms[inst]<<" ms, "<<kerneltimelapse_microsec<<" microsecs, "<<std::endl;
				// #endif

				#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
				cout<<"goclkernel:: migrating workload back to HOST..."<<endl;
				#endif
				OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events));	
				OCL_CHECK(err, err = read_events.wait());
				OCL_CHECK(err, err = q.finish());
				
				// readback 
				for(unsigned int i=0; i<PADDEDKVSOURCEDRAMSZ_KVS; i++){
					for(unsigned int v=0; v<VECTOR_SIZE; v++){
						kvsourcedram[inst][i].data[v] = tempkvsourcedram[i].data[v];
					}
				}
			}
			
			avs_proc[GraphIter] = getaveragetimeelapsed(kerneltimelapse_ms);
			total_time_elapsed += avs_proc[GraphIter];
			std::cout <<">>> SUMMARY: kernel (proc) average time elapsed for Iteration "<<GraphIter<<": "<<avs_proc[GraphIter]<<" ms"<<std::endl;
		}
		#endif 
		
		#if defined(ENABLE_ACTSSYNC) //  && not defined(ALLVERTEXISACTIVE_ALGORITHM)
		{
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"--------------- goclkernel[actssync]:: running ACTS SYNCHRONZE (Iteration "<<GraphIter<<") ---------------"<<endl;
			#endif 
			
			kernelobjs_synchronize->topkernelsync_sw(
				(uint512_dt *)kvsourcedram[0],
				(uint512_dt *)kvsourcedram[1],
				#if NUMCOMPUTEUNITS>2
				(uint512_dt *)kvsourcedram[2],
				(uint512_dt *)kvsourcedram[3],
				#if NUMCOMPUTEUNITS>4
				(uint512_dt *)kvsourcedram[4],
				(uint512_dt *)kvsourcedram[5],
				(uint512_dt *)kvsourcedram[6],
				(uint512_dt *)kvsourcedram[7],
				#if NUMCOMPUTEUNITS>8
				(uint512_dt *)kvsourcedram[8],
				(uint512_dt *)kvsourcedram[9],
				(uint512_dt *)kvsourcedram[10],
				(uint512_dt *)kvsourcedram[11],
				#if NUMCOMPUTEUNITS>12
				(uint512_dt *)kvsourcedram[12],
				(uint512_dt *)kvsourcedram[13],
				(uint512_dt *)kvsourcedram[14],
				(uint512_dt *)kvsourcedram[15],
				#if NUMCOMPUTEUNITS>16
				(uint512_dt *)kvsourcedram[16],
				(uint512_dt *)kvsourcedram[17],
				(uint512_dt *)kvsourcedram[18],
				(uint512_dt *)kvsourcedram[19],
				#if NUMCOMPUTEUNITS>20
				(uint512_dt *)kvsourcedram[20],
				(uint512_dt *)kvsourcedram[21],
				(uint512_dt *)kvsourcedram[22],
				(uint512_dt *)kvsourcedram[23],
				#if NUMCOMPUTEUNITS>24
				(uint512_dt *)kvsourcedram[24],
				(uint512_dt *)kvsourcedram[25],
				(uint512_dt *)kvsourcedram[26],
				(uint512_dt *)kvsourcedram[27],
				#if NUMCOMPUTEUNITS>28
				(uint512_dt *)kvsourcedram[28],
				(uint512_dt *)kvsourcedram[29],
				(uint512_dt *)kvsourcedram[30],
				(uint512_dt *)kvsourcedram[31],
				#endif
				#endif
				#endif
				#endif
				#endif
				#endif
				#endif
				#endif 
				(uint512_dt *)vdram
			);
		}
		#endif
		
		#ifdef KOKOOOOOOO
		#if defined(ENABLE_ACTSSYNC_XXXXXX) && defined(ALLVERTEXISACTIVE_ALGORITHM)
		{
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"--------------- goclkernel[actssync]:: running ACTS SYNCHRONZE (Iteration "<<GraphIter<<") ---------------"<<endl;
			#endif 
			
			cl_int err;
			std::vector<cl_mem_ext_ptr_t> inoutBufExt(32);
			std::vector<cl::Buffer> buffer_kvsourcedram(32);
			cl::Kernel krnl;
			
			cl::Event kernel_events;
			cl::Event write_events;
			cl::Event read_events;
			
			long double kerneltimelapse_ms[1];
			
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			printf("INFO: loading vmul kernel\n");
			#endif
			cl::Program::Binaries vmul_bins{{fileBuf_sync.data(), fileBuf_sync.size()}};
			devices.resize(1);
			OCL_CHECK(err,
					  cl::Program program(context, devices, vmul_bins, NULL, &err));
					  
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: creating kernel object..."<<endl;
			#endif
			std::string krnl_name = "topkernelsync";
			std::string cu_id = std::to_string((1));
			std::string krnl_name_full = krnl_name + ":{" + "topkernelsync_" + cu_id + "}"; 
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			printf("Creating a kernel [%s] for CU(%d)\n",
				   krnl_name_full.c_str(),
				   (1));
			#endif
			OCL_CHECK(err,
					  krnl = cl::Kernel(program, krnl_name_full.c_str(), &err));

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel[actssync]:: creating OCL buffers..."<<endl;
			#endif
			for(unsigned int i=0; i<4; i++){ // NUMSUBCPUTHREADS
				#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
				cout<<"creating buffer for ACTS: "<<i<<endl;
				#endif
				OCL_CHECK(err,
					buffer_kvsourcedram[i] =
						cl::Buffer(context,
							CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
							sizeof(uint512_vec_dt) * PADDEDKVSOURCEDRAMSZ_KVS,
							tempkvsourcedrams[i],
							&err));
			}
			OCL_CHECK(err,
				buffer_kvsourcedram[4] =
					cl::Buffer(context,
						CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
						sizeof(uint512_vec_dt) * PADDEDKVSOURCEDRAMSZ_KVS,
						vdram,
						&err));

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel[actssync]:: setting kernel arguments..."<<endl;
			#endif
			for(unsigned int i=0; i<4 + 1; i++){ OCL_CHECK(err, err = krnl.setArg(i, buffer_kvsourcedram[i])); } // NUMSUBCPUTHREADS
			
			////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////// CRITICAL REMOVEME.
			unsigned int _BASEOFFSETKVS_STATSDRAM = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key;
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
				for(unsigned int k=0; k<182; k++){ // 182, 280
					for(unsigned int v=0; v<VECTOR_SIZE; v++){
						kvsourcedram[i][_BASEOFFSETKVS_STATSDRAM + k].data[v].key = 7777; 
						kvsourcedram[i][_BASEOFFSETKVS_STATSDRAM + k].data[v].value = 7777; 
					}
				}
			}
			/////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////
			
			for(unsigned int round=0; round<1; round++){ // 2
				for(unsigned int inst=0; inst<NUMSUBCPUTHREADS; inst+=4){
					// write 
					for(unsigned int i=0; i<4; i++){
						for(unsigned int k=0; k<PADDEDKVSOURCEDRAMSZ_KVS; k++){
							for(unsigned int v=0; v<VECTOR_SIZE; v++){
								tempkvsourcedrams[i][k].data[v] = kvsourcedram[inst + i][k].data[v];
							}
						}
					}
					
					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					cout<<"goclkernel[actssync]:: migrating workload to FPGA..."<<endl;
					#endif
					OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4]}, 0, NULL, &write_events));
					OCL_CHECK(err, err = write_events.wait());
					
					// This function will execute the kernel on the FPGA
					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					cout<<"goclkernel[actssync]:: launching the kernel..."<<endl;
					#endif
					std::chrono::steady_clock::time_point beginkerneltime = std::chrono::steady_clock::now();	  
					OCL_CHECK(err, err = q.enqueueTask(krnl, NULL, &kernel_events)); 
					OCL_CHECK(err, err = kernel_events.wait());
					kerneltimelapse_ms[0] = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime).count();
					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					std::cout <<">>> kernel (sync) time elapsed: "<<kerneltimelapse_ms[0]/1000<<" ms, "<<kerneltimelapse_ms[0]<<" microsecs, "<<std::endl;
					#endif 
					
					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					cout<<"goclkernel[actssync]:: migrating workload back to HOST..."<<endl;
					#endif
					OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4]}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events));
					OCL_CHECK(err, err = read_events.wait());
					
					OCL_CHECK(err, err = q.finish());
					
					// readback 
					for(unsigned int i=0; i<4; i++){
						for(unsigned int k=0; k<PADDEDKVSOURCEDRAMSZ_KVS; k++){
							for(unsigned int v=0; v<VECTOR_SIZE; v++){
								kvsourcedram[inst + i][k].data[v] = tempkvsourcedrams[i][k].data[v];
							}
						}
					}
				}
			}
			
			avs_sync[GraphIter] = 0; // kerneltimelapse_ms[0]; // FIXME. unwanted latency
			total_time_elapsed += avs_sync[GraphIter];
			// std::cout <<">>> SUMMARY: kernel (sync) average time elapsed for Iteration "<<GraphIter<<": "<<avs_sync[GraphIter]<<" ms"<<std::endl;
		}
		#endif
		
		////////////////////////////////////////////////////////////////////////
		#if defined(ENABLE_ACTSSYNC_XXX) && not defined(ALLVERTEXISACTIVE_ALGORITHM)
		{
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"--------------- goclkernel[actssync]:: running ACTS SYNCHRONZE (Iteration "<<GraphIter<<") ---------------"<<endl;
			#endif 
			
			cl_int err;
			std::vector<cl_mem_ext_ptr_t> inoutBufExt(32);
			std::vector<cl::Buffer> buffer_kvsourcedram(32);
			cl::Kernel krnl;
			
			cl::Event kernel_events;
			cl::Event write_events;
			cl::Event read_events;
			
			long double kerneltimelapse_ms[1];
			
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			printf("INFO: loading vmul kernel\n");
			#endif
			cl::Program::Binaries vmul_bins{{fileBuf_sync.data(), fileBuf_sync.size()}};
			devices.resize(1);
			OCL_CHECK(err,
					  cl::Program program(context, devices, vmul_bins, NULL, &err));
					  
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel:: creating kernel object..."<<endl;
			#endif
			std::string krnl_name = "topkernelsync";
			std::string cu_id = std::to_string((1));
			std::string krnl_name_full = krnl_name + ":{" + "topkernelsync_" + cu_id + "}"; 
			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			printf("Creating a kernel [%s] for CU(%d)\n",
				   krnl_name_full.c_str(),
				   (1));
			#endif
			OCL_CHECK(err,
					  krnl = cl::Kernel(program, krnl_name_full.c_str(), &err));

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel[actssync]:: creating OCL buffers..."<<endl;
			#endif
			for(unsigned int i=0; i<4; i++){ // NUMSUBCPUTHREADS
				#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
				cout<<"creating buffer for ACTS: "<<i<<endl;
				#endif
				OCL_CHECK(err,
					buffer_kvsourcedram[i] =
						cl::Buffer(context,
							CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
							sizeof(uint512_vec_dt) * PADDEDKVSOURCEDRAMSZ_KVS,
							tempkvsourcedrams[i],
							&err));
			}
			OCL_CHECK(err,
				buffer_kvsourcedram[4] =
					cl::Buffer(context,
						CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
						sizeof(uint512_vec_dt) * PADDEDKVSOURCEDRAMSZ_KVS,
						tempvdram,
						&err));

			#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
			cout<<"goclkernel[actssync]:: setting kernel arguments..."<<endl;
			#endif
			for(unsigned int i=0; i<4 + 1; i++){ OCL_CHECK(err, err = krnl.setArg(i, buffer_kvsourcedram[i])); } // NUMSUBCPUTHREADS
			
			////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////// CRITICAL REMOVEME.
			/* unsigned int _BASEOFFSETKVS_STATSDRAM = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key;
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
				for(unsigned int k=0; k<182; k++){ // 182, 280
					for(unsigned int v=0; v<VECTOR_SIZE; v++){
						kvsourcedram[i][_BASEOFFSETKVS_STATSDRAM + k].data[v].key = 7777; 
						kvsourcedram[i][_BASEOFFSETKVS_STATSDRAM + k].data[v].value = 7777; 
					}
				}
			} */
			/////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////
			
			for(unsigned int round=0; round<2; round++){ // 2
				for(unsigned int inst=0; inst<NUMSUBCPUTHREADS; inst+=4){
					// write 
					for(unsigned int i=0; i<4; i++){
						for(unsigned int k=0; k<PADDEDKVSOURCEDRAMSZ_KVS; k++){
							for(unsigned int v=0; v<VECTOR_SIZE; v++){
								tempkvsourcedrams[i][k].data[v] = kvsourcedram[inst + i][k].data[v];
							}
						}
					}
					for(unsigned int k=0; k<PADDEDKVSOURCEDRAMSZ_KVS; k++){
						for(unsigned int v=0; v<VECTOR_SIZE; v++){
							if(round==1){ tempvdram[k].data[v] = vdram[k].data[v]; }
							else{ tempvdram[k].data[v] = kvsourcedram[0][k].data[v]; }
						}
					}
					
					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					cout<<"goclkernel[actssync]:: migrating workload to FPGA..."<<endl;
					#endif
					OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4]}, 0, NULL, &write_events));
					OCL_CHECK(err, err = write_events.wait());
					
					// This function will execute the kernel on the FPGA
					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					cout<<"goclkernel[actssync]:: launching the kernel..."<<endl;
					#endif
					std::chrono::steady_clock::time_point beginkerneltime = std::chrono::steady_clock::now();	  
					OCL_CHECK(err, err = q.enqueueTask(krnl, NULL, &kernel_events)); 
					OCL_CHECK(err, err = kernel_events.wait());
					kerneltimelapse_ms[0] = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime).count();
					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					std::cout <<">>> kernel (sync) time elapsed: "<<kerneltimelapse_ms[0]/1000<<" ms, "<<kerneltimelapse_ms[0]<<" microsecs, "<<std::endl;
					#endif 
					
					#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
					cout<<"goclkernel[actssync]:: migrating workload back to HOST..."<<endl;
					#endif
					OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvsourcedram[0], buffer_kvsourcedram[1], buffer_kvsourcedram[2], buffer_kvsourcedram[3], buffer_kvsourcedram[4]}, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events));
					OCL_CHECK(err, err = read_events.wait());
					
					OCL_CHECK(err, err = q.finish());
					
					// readback 
					for(unsigned int i=0; i<4; i++){
						for(unsigned int k=0; k<PADDEDKVSOURCEDRAMSZ_KVS; k++){
							for(unsigned int v=0; v<VECTOR_SIZE; v++){
								kvsourcedram[inst + i][k].data[v] = tempkvsourcedrams[i][k].data[v];
							}
						}
					}
					for(unsigned int k=0; k<PADDEDKVSOURCEDRAMSZ_KVS; k++){
						for(unsigned int v=0; v<VECTOR_SIZE; v++){
							if(round==1){ vdram[k].data[v] = tempvdram[k].data[v]; }
							// else{ kvsourcedram[0][k].data[v] = tempvdram[k].data[v]; }
						}
					}
				}
			}
			
			avs_sync[GraphIter] = 0; // kerneltimelapse_ms[0]; // FIXME. unwanted latency
			total_time_elapsed += avs_sync[GraphIter];
			// std::cout <<">>> SUMMARY: kernel (sync) average time elapsed for Iteration "<<GraphIter<<": "<<avs_sync[GraphIter]<<" ms"<<std::endl;
		}
		#endif
		////////////////////////////////////////////////////////////////////////
		#endif 
		
		if(ExitCheck(kvsourcedram[0], GraphIter) == 1){ cout<<"goclkernel::runapp2: no more active vertices to process. exiting... "<<endl; break; }
	}
	std::cout << TIMINGRESULTSCOLOR <<">>> SUMMARY: Total kernel time: "<<total_time_elapsed<<" ms"<< RESET << std::endl;
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







