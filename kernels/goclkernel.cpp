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

void goclkernel::launchkernel(uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){ // REMOVEME.
	double kernel_time_in_sec = 0, result = 0;
    std::chrono::duration<double> kernel_time(0);
	#ifdef _DEBUGMODE_TIMERS2
	cout<<"goclkernel::launchkernel:: Launching "<<NUMACTIVEKERNELS<<" active Kernels..."<<endl;
	#endif
	
    auto kernel_start = std::chrono::high_resolution_clock::now();
	
	unsigned int bufferid = 0;
	#ifdef TESTKERNEL_FULLBANDWIDTH
	for(unsigned int i=0; i<1; i++){
		//Setting the k_vadd Arguments
		#if NUMSUBWORKERS==2
		OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(1, buffer_kvsourcedram[0]));
		#endif 
		#if NUMSUBWORKERS==4
		OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(1, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(2, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(3, buffer_kvsourcedram[0]));
		#endif 
		#if NUMSUBWORKERS==8
		OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(1, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(2, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(3, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(4, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(5, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(6, buffer_kvsourcedram[0]));
		OCL_CHECK(err, err = krnls[i].setArg(7, buffer_kvsourcedram[0]));
		#endif 
		
		//Invoking the kernel
		std::vector<cl::Event> waitList;
        waitList.push_back(write_event[i]);
		OCL_CHECK(err,
                  err = q.enqueueNDRangeKernel(
                      krnls[i], 0, 1, 1, &waitList, &kernel_events[i]));
		set_callback2(kernel_events[i], "ooo_queue");
	}
	#else 
	for(unsigned int i=0; i<NUMACTIVEKERNELS; i++){
		//Setting the k_vadd Arguments
		OCL_CHECK(err, err = krnls[i].setArg(0, buffer_kvsourcedram[bufferid++]));

		//Invoking the kernel
		std::vector<cl::Event> waitList;
        waitList.push_back(write_event[i]);
		OCL_CHECK(err,
                  err = q.enqueueNDRangeKernel(
                      krnls[i], 0, 1, 1, &waitList, &kernel_events[i]));
		set_callback2(kernel_events[i], "ooo_queue");
	}
	#endif 
    q.finish();
	
    auto kernel_end = std::chrono::high_resolution_clock::now();
    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
	#ifdef _DEBUGMODE_TIMERS2
	std::cout<< TIMINGRESULTSCOLOR <<">>> total time elapsed: "<<kernel_time.count() * 1000<<" ms"<< RESET <<std::endl;
	#endif 
	return;
}

void goclkernel::writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"goclkernel::writetokernel::1st of "<<TOTALNUMACTCUSTORUN<<":: hostbeginoffset: "<<hostbeginoffset<<", beginoffset "<<beginoffset<<" size: "<<size<<", PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;
	#endif
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){ kernel_events[flag*i].wait(); } // REMOVEME.
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		unsigned int threadid = i / NUMSUBCPUTHREADS;
		unsigned int subthreadid = i % NUMSUBCPUTHREADS;
		OCL_CHECK(err, err = q.enqueueWriteBuffer(buffer_kvsourcedram[i], CL_FALSE, beginoffset * sizeof(keyvalue_t), size * sizeof(keyvalue_t), (keyvalue_t *)(&kvsourcedram[threadid][subthreadid][(hostbeginoffset / VECTOR_SIZE)])));
	}
    q.finish();
}
void goclkernel::writetokernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size[NUMCPUTHREADS][NUMSUBCPUTHREADS]){
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){ kernel_events[flag*i].wait(); } // REMOVEME.
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		unsigned int threadid = i / NUMSUBCPUTHREADS;
		unsigned int subthreadid = i % NUMSUBCPUTHREADS;
		
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"goclkernel::writetokernel:: hostbeginoffset: "<<hostbeginoffset[threadid][subthreadid]<<", beginoffset["<<threadid<<"]["<<subthreadid<<"] "<<beginoffset[threadid][subthreadid]<<" size["<<threadid<<"]["<<subthreadid<<"]: "<<size[threadid][subthreadid]<<", PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;					
		#endif
		if(size[threadid][subthreadid] == 0){ size[threadid][subthreadid] = 1024; } // REMOVEME?
		#ifdef ACCESSFPGABY_ENQUEUEWRITEBUFFER
		OCL_CHECK(err, err = q.enqueueWriteBuffer(buffer_kvsourcedram[i], CL_FALSE, beginoffset[threadid][subthreadid] * sizeof(keyvalue_t), size[threadid][subthreadid] * sizeof(keyvalue_t), (keyvalue_t *)(&kvsourcedram[threadid][subthreadid][(hostbeginoffset[threadid][subthreadid] / VECTOR_SIZE)]), NULL, &write_event[i])); 											
		#else 
		OCL_CHECK(err,
				  err = q.enqueueMigrateMemObjects(
					  {buffer_kvsourcedram[i]},
					  0,
					  NULL,
					  &write_event[i]));
		#endif
		set_callback2(write_event[i], "ooo_queue");
	}
    q.finish();
}

void goclkernel::readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset, unsigned int beginoffset, unsigned int size){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"goclkernel::readfromkernel::1st of "<<TOTALNUMACTCUSTORUN<<":: hostbeginoffset: "<<hostbeginoffset<<", beginoffset "<<beginoffset<<" size: "<<size<<", PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;
	#endif
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){ kernel_events[flag*i].wait(); } // REMOVEME.
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		unsigned int threadid = i / NUMSUBCPUTHREADS;
		unsigned int subthreadid = i % NUMSUBCPUTHREADS;
		OCL_CHECK(err, err = q.enqueueReadBuffer(buffer_kvsourcedram[i], CL_FALSE, beginoffset * sizeof(keyvalue_t), size * sizeof(keyvalue_t), (keyvalue_t *)(&kvsourcedram[threadid][subthreadid][(hostbeginoffset / VECTOR_SIZE)])));
	}
    q.finish();
}
void goclkernel::readfromkernel(unsigned int flag, uint512_vec_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int hostbeginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int beginoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int size[NUMCPUTHREADS][NUMSUBCPUTHREADS]){				
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"goclkernel::readfromkernel::1st of "<<TOTALNUMACTCUSTORUN<<":: hostbeginoffset: "<<hostbeginoffset[0][0]<<", beginoffset[0][0] "<<beginoffset[0][0]<<" size[0][0]: "<<size[0][0]<<", PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;					
	#endif
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){ OCL_CHECK(err, err = kernel_events[flag*i].wait()); } // REMOVEME.
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		unsigned int threadid = i / NUMSUBCPUTHREADS;
		unsigned int subthreadid = i % NUMSUBCPUTHREADS;
		
		std::vector<cl::Event> eventList;
        eventList.push_back(kernel_events[flag*i]);
		
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"goclkernel::readfromkernel:: hostbeginoffset: "<<hostbeginoffset[threadid][subthreadid]<<", beginoffset["<<threadid<<"]["<<subthreadid<<"] "<<beginoffset[threadid][subthreadid]<<" size["<<threadid<<"]["<<subthreadid<<"]: "<<size[threadid][subthreadid]<<", PADDEDKVSOURCEDRAMSZ: "<<PADDEDKVSOURCEDRAMSZ<<endl;					
		#endif 
		#ifdef ACCESSFPGABY_ENQUEUEWRITEBUFFER
		OCL_CHECK(err, err = q.enqueueReadBuffer(buffer_kvsourcedram[i], CL_FALSE, beginoffset[threadid][subthreadid] * sizeof(keyvalue_t), size[threadid][subthreadid] * sizeof(keyvalue_t), (keyvalue_t *)(&kvsourcedram[threadid][subthreadid][(hostbeginoffset[threadid][subthreadid] / VECTOR_SIZE)]), &eventList, &read_events[flag*i]));
		#else 
		OCL_CHECK(err,
				  err = q.enqueueMigrateMemObjects(
					  {buffer_kvsourcedram[i]},
					  CL_MIGRATE_MEM_OBJECT_HOST,
					  &eventList,
                      &read_events[flag*i]));	
		#endif
		set_callback2(read_events[flag*i], "ooo_queue");
		OCL_CHECK(err, err = read_events[flag*i].wait());
	}
    q.finish();
}

void goclkernel::loadOCLstructures(std::string _binaryFile, uint512_vec_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){		
	binaryFile = _binaryFile;

	inputdata_size_bytes = PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt);
	cout<<"goclkernel::loadOCLstructures:: inputdata_size_bytes: "<<inputdata_size_bytes<<endl;
	
	// read_events.resize((2 * NUMACTIVEKERNELS));
	read_events.resize((2 * NUMACTIVEKERNELS));
	kernel_events.resize((2 * NUMACTIVEKERNELS));
	write_event.resize(NUMACTIVEKERNELS);
	
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"goclkernel::loadOCLstructures: printing OCL parameters"<<endl;
	cout<<"TOTALNUMKERNELS: "<<TOTALNUMKERNELS<<endl;
	cout<<"NUMHBMSPERKERNEL: "<<NUMHBMSPERKERNEL<<endl;
	cout<<"TOTALNUMACTCUSTORUN: "<<TOTALNUMACTCUSTORUN<<endl;
	cout<<"NUMACTSCUSPERKERNEL: "<<NUMACTSCUSPERKERNEL<<endl;
	cout<<"TOTALAVAILABLEACTSCUS: "<<TOTALAVAILABLEACTSCUS<<endl;
	cout<<"NUMACTIVEKERNELS: "<<NUMACTIVEKERNELS<<endl;
	cout<<"NUMACTIVEINSTANCES: "<<NUMACTIVEINSTANCES<<endl;
	#endif 
	
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
	for(unsigned int i=0; i<1; i++){ // TOTALNUMKERNELS // REMOVEME
			std::string cu_id = std::to_string((i+1));
			#ifdef TESTKERNEL_FULLBANDWIDTH
			std::string krnl_name_full =
				krnl_name + ":{" + "topkernel_1" + "}";
			#else 
			std::string krnl_name_full =
				krnl_name + ":{" + "topkernel_" + cu_id + "}"; 
			#endif

			printf("Creating a kernel [%s] for CU(%d)\n",
				   krnl_name_full.c_str(),
				   (i+1));

			//Here Kernel object is created by specifying kernel name along with compute unit.
			//For such case, this kernel object can only access the specific Compute unit
			OCL_CHECK(err,
					  krnls[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	}
	
	unsigned int flag=0;
	unsigned int counter = 0;
	uint512_vec_dt ** kvsourcedramarr = (uint512_vec_dt **)(&kvsourcedram[0]);
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
		cout<<"attaching bufferExt "<<i<<" to HBM bank: "<<i<<endl;
		#ifdef TESTKERNEL_FULLBANDWIDTH
		inoutBufExt[i].obj = kvsourcedramarr[0];
		inoutBufExt[i].param = 0;
		inoutBufExt[i].flags = bank[0]; 
		#else 
		inoutBufExt[i].obj = kvsourcedramarr[counter++];
		inoutBufExt[i].param = 0;
		inoutBufExt[i].flags = bank[i];
		#endif
	}
	
    // These commands will allocate memory on the FPGA. The cl::Buffer objects can
    // be used to reference the memory locations on the device.
    // Creating Buffers
	for(unsigned int i=0; i<TOTALNUMACTCUSTORUN; i++){
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
	return;
}
void goclkernel::finishOCL(){
	cout<<"test::finish destroying memory structures... "<<endl;
	q.finish();
}
#endif 











