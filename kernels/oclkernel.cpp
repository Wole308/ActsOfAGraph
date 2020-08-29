#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "../../src/utility/utility.h"
#include "../../include/common.h"
// #include "xcl2.hpp"
#include "../xcl.h"
#include "oclkernel.h"
using namespace std;

#define OCL_CHECK(call)							\
	do {								\
		cl_int err = call;					\
		if (err != CL_SUCCESS) {				\
			cout << "ERR: Error calling " #call		\
				", error code is: " << err << endl;	\
			exit(EXIT_FAILURE);				\
		}							\
	} while (0);

oclkernel::oclkernel(){
	utilityobj = new utility();
}
oclkernel::~oclkernel(){} 

#ifdef FPGA_IMPL 
void oclkernel::launchkernel(uint512_dt * kvsourcedram[NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int flag){		
	std::cout << "Set the Kernel Arguments" << std::endl;
	int nargs=0;
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvsourcedram[flag][j][k]);
		}
	}
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvdestdram[flag][j][k]);
		}
	}
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvstatsdram[flag][j][k]);
		}
	}
	
	// Copy input data to device global memory
	std::cout << "Copy input data to device global memory" << std::endl;
	array<cl_event, (NUMCPUTHREADS + NUMSUBCPUTHREADS)* NUM_KAPI> write_events;
	
	// Copy data from Host to Device
	unsigned int pos = 0;
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvsourcedram[flag][j][k], CL_TRUE, 0, kvsource_size_bytes, (uint512_vec_dt *)kvsourcedram[j][k], 0, NULL, &write_events[pos] ));
			pos += 1;
		}
	}
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdestdram[flag][j][k], CL_TRUE, 0, kvdest_size_bytes, (uint512_vec_dt *)kvdestdram[j][k], 0, NULL, &write_events[pos] ));
			pos += 1;
		}
	}
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvstatsdram[flag][j][k], CL_TRUE, 0, kvstats_size_bytes, kvstats[j][k], 0, NULL, &write_events[pos] ));
			pos += 1;
		}
	}
	
	// Launch the Kernel
	printf("Enqueueing NDRange kernel.\n");
	std::cout << "Launch the Kernel" << std::endl;
	std::vector<cl_event> waitList; 
	for(unsigned int i=0; i<pos; i++){ waitList.push_back(write_events[i]); }
	OCL_CHECK(clEnqueueNDRangeKernel(world.command_queue, kernel, 1, nullptr,
									&global, &local, pos, waitList.data(),
									&kernel_events[flag]));
	
	// Copy Result from Device Global Memory to Host Local Memory
	utilityobj->set_callback(kernel_events[flag], "ooo_queue");
	#ifdef LOCKE
	clWaitForEvents(1, &kernel_events[flag]);
	#endif 
	std::cout << "Getting Results (Device to Host)..." << std::endl;
	printf("Enqueueing Migrate Mem Object (Device to Host) calls\n");
}

void oclkernel::writeVstokernel(unsigned int flag){
	/* std::cout << "Copy input data to kernel global memory" << std::endl;
	array<cl_event, {{context['NUMINSTANCES']}}> write_events;
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdestdram[0][j][k], CL_TRUE, 0, kvdest_size_bytes, kvdestdram[0][j][k], 0, NULL, &write_events[ddr] ));
		}
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ clWaitForEvents(1, &write_events[ddr]); } */
}
void oclkernel::readVsfromkernel(unsigned int flag){
	/* std::cout << "Copy input data to host global memory" << std::endl;
	array<cl_event, {{context['NUMINSTANCES']}}> write_events;
	for(unsigned int j=0; j<NUMCPUTHREADS; j++){
		for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
			OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdestdram[0][j][k], CL_TRUE, 0, kvdest_size_bytes, kvdestdram[0][j][k], 0, NULL, &write_events[ddr] ));
		}
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ clWaitForEvents(1, &write_events[ddr]); } */
}

void oclkernel::loadOCLstructures(std::string binaryFile, uint512_dt * kvsourcedram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], uint512_dt * kvdestdram[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS], keyvalue_t * kvstats[NUMFLAGS][NUMCPUTHREADS][NUMSUBCPUTHREADS]){		
	cl_int err;
	global = 1; local = 1;

	kvsource_size_bytes = KVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvdest_size_bytes = 8 * sizeof(uint512_vec_dt);
	kvstats_size_bytes = MESSAGESDRAMSZ * sizeof(keyvalue_t);
	
	// FPGA World, program and kernel objects
	world = { 0 };
	world = xcl_world_single();
	clReleaseCommandQueue(world.command_queue);
	world.command_queue =
		clCreateCommandQueue(world.context, world.device_id,
						   CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
					
	// Create Program and Kernel
	program = xcl_import_binary_file(world, binaryFile.c_str()); // -44 means INVALID PROGRAM
	kernel = xcl_get_kernel(program, "topkernel");
	
	// Allocate Buffer in Global Memory
	std::cout << "Allocate Buffer in Global Memory" << std::endl;
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){
		for(unsigned int j=0; j<NUMCPUTHREADS; j++){
			for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
				buffer_kvsourcedram[flag][j][k] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvsource_size_bytes, kvsourcedram[flag][j][k], &err);		
				if(err != CL_SUCCESS) {
					printf("Error: Failed to create buffer_kvsourcedram[%i][%i] buffer arg. %i \n", j, k, err);
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){
		for(unsigned int j=0; j<NUMCPUTHREADS; j++){
			for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
				buffer_kvdestdram[flag][j][k] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvdest_size_bytes, kvdestdram[flag][j][k], &err);
				if(err != CL_SUCCESS) {
					printf("Error: Failed to create buffer_kvdestdram[%i][%i] buffer arg. %i \n", j, k, err);
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){
		for(unsigned int j=0; j<NUMCPUTHREADS; j++){
			for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
				buffer_kvstatsdram[flag][j][k] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, kvstats[flag][j][k], &err);
				if(err != CL_SUCCESS) {
					printf("Error: Failed to create buffer_kvstatsdram[%i][%i] buffer arg. %i \n", j, k, err);
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	return;
}
void oclkernel::finishOCL(){
	cout<<"test::finish destroying memory structures... "<<endl;
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){ clWaitForEvents(1, &kernel_events[flag]); }
	
	cout<<"test::finish: releasing and destroying all OCL structures... "<<endl;
	clFlush(world.command_queue);
	clFinish(world.command_queue);
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){
		for(unsigned int j=0; j<NUMCPUTHREADS; j++){
			for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
				OCL_CHECK(clReleaseMemObject(buffer_kvsourcedram[flag][j][k]));
			}
		}
	}
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){
		for(unsigned int j=0; j<NUMCPUTHREADS; j++){
			for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
				OCL_CHECK(clReleaseMemObject(buffer_kvdestdram[flag][j][k])); 
			}
		}
	}
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){
		for(unsigned int j=0; j<NUMCPUTHREADS; j++){
			for(unsigned int k=0; k<NUMSUBCPUTHREADS; k++){
				OCL_CHECK(clReleaseMemObject(buffer_kvstatsdram[flag][j][k])); 
			}
		}
	}
	for(unsigned int flag=0; flag<NUMFLAGS; flag++){ clWaitForEvents(1, &kernel_events[flag]); }
	
	cout<<"clReleaseCommandQueue"<<endl;
	clReleaseCommandQueue(world.command_queue);
	cout<<"clReleaseContext"<<endl;
	clReleaseContext(world.context);
	cout<<"clReleaseDevice"<<endl;
	clReleaseDevice(world.device_id); 
	cout<<"clReleaseKernel"<<endl;
	clReleaseKernel(kernel);
	cout<<"clReleaseProgram"<<endl;
	clReleaseProgram(program);
	cout<<"test::finish: released and destroyed all OCL structures"<<endl;
}
#endif 











