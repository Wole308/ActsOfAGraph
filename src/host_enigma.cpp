#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "host_common.h"
#include "common.h"
#include "../debugger/host_debugger.h"
#include "EdgeProcess.h" // Grafboost header
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../algorithm/algorithm.h"
#include "../utility/utility.h"
#ifdef SW
#include "../kernels/kernelprocess.h"
#endif
#include "host_enigma.h"
using namespace std;
#define YES
#define NUMINSTANCESETS 3

#ifdef FPGA_IMPL
#define OCL_CHECK(call)							\
	do {								\
		cl_int err = call;					\
		if (err != CL_SUCCESS) {				\
			cout << "ERR: Error calling " #call		\
				", error code is: " << err << endl;	\
			exit(EXIT_FAILURE);				\
		}							\
	} while (0);
#endif 

host_enigma::host_enigma(graph * _graphobj){
	graphobj = _graphobj;
	
	#ifdef SW
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvsourcedram[i][ddr] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvdestdram[i][ddr] = new uint512_vec_dt[KVDATA_RANGE_PERSSDPARTITION_KVS]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvstats[i][ddr] = new keyvalue_t[KVSTATSDRAMSZ]; }
	}
	#else
	// #ifdef FPGA_IMPL & defined(SW)
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvsourcedram[i][ddr] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt))); }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvdestdram[i][ddr] = (uint512_vec_dt *) aligned_alloc(4096, (KVDATA_RANGE_PERSSDPARTITION_KVS * sizeof(uint512_vec_dt))); }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvstats[i][ddr] = (keyvalue_t *) aligned_alloc(4096, (KVSTATSDRAMSZ * sizeof(keyvalue_t))); }
	}
	// #endif
	#endif 
	
	#ifdef SW
	kernelprocess * kernelobj = new kernelprocess();
	#endif 
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edgeprocessobj[i] = new edge_process(graphobj); }
	srand (0);
}
host_enigma::~host_enigma(){
	cout<<"host_enigma::~host_enigma:: finish destroying memory structures... "<<endl;
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){		
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvsourcedram[i][ddr]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvdestdram[i][ddr]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvstats[i][ddr]; }
	}
	#ifdef FPGA_IMPL
	finishOCL();
	#endif 
}

void host_enigma::run(){
	start();
	return;
}
void host_enigma::start() {
	WorkerThread(0);
}
void host_enigma::WorkerThread(int threadidx){
	unsigned int iteration_size = DRAMBATCHFACTOR;
	for(unsigned int IterCount=0; IterCount<iteration_size; IterCount++){
		loaddrams(threadidx, IterCount);
		if((runActs(IterCount) == 1) || (IterCount == (iteration_size-1))){ 
			calculatealloffsets(threadidx); 
			for(unsigned int ddr=0; ddr<NUMINSTANCES; ddr++){ kvstats[threadidx][ddr][getmessagesAddr(MESSAGES_RUNKERNELCOMMANDID)].key = ON; }
		} else { for(unsigned int ddr=0; ddr<NUMINSTANCES; ddr++){ kvstats[threadidx][ddr][getmessagesAddr(MESSAGES_RUNKERNELCOMMANDID)].key = OFF; } }
		
		#ifdef _DEBUGMODE_KERNELPRINTSX
		for(unsigned int i=0; i<DRAMBATCHFACTOR; i++){ kernelobj->printkeyvalues("BEFORE", (keyvalue_t *)&kvsourcedram[threadidx][0][i*KVDATA_BATCHSIZE_KVS], 16); }
		#endif 
		
		#ifdef SW
		launchswkernel(threadidx);
		#else 
		launchkernel(threadidx);
		#endif 
		
		#ifdef _DEBUGMODE_KERNELPRINTSX
		for(unsigned int i=0; i<DRAMBATCHFACTOR; i++){ kernelobj->printkeyvalues("AFTER", (keyvalue_t *)&kvsourcedram[threadidx][0][i*KVDATA_BATCHSIZE_KVS], 16); }
		#endif
	}
	return;
}
void host_enigma::finish(){
	#if (defined(FPGA_IMPL) && defined(PR_ALGORITHM))
	finishOCL();
	#endif
}

void host_enigma::loaddrams(int threadidx, unsigned int IterCount){
	for(unsigned int ddr=0; ddr<NUMINSTANCES; ddr++){
		loadkvdram(kvsourcedram[threadidx][ddr], 0, KVDATA_BATCHSIZE_KVS);
		loadmessages(kvstats[threadidx][ddr], BASEOFFSET_MESSAGESDRAM, IterCount);
	}
	return;
}
void host_enigma::calculatealloffsets(int threadidx){	
	for(unsigned int ddr=0; ddr<NUMINSTANCES; ddr++){
		kvstats[threadidx][ddr][0].value = KVDATA_BATCHSIZE * DRAMBATCHFACTOR; // calculateoffsets(kvstats[threadidx][ddr], KVDATA_BATCHSIZE * DRAMBATCHFACTOR, BASEOFFSET_STATSDRAM, pow(NUM_PARTITIONS, TREE_DEPTH));
	}
	return;
}

void host_enigma::loadkvdram(uint512_vec_dt * kvdram, unsigned int baseoffset_kvs, unsigned int kvsize_kvs){
	for(unsigned int i=0; i<kvsize_kvs; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram[baseoffset_kvs + i].data[v].key = rand() % KVDATA_RANGE_PERSSDPARTITION;
			kvdram[baseoffset_kvs + i].data[v].value = 0;
		}
	}
	return;
}
void host_enigma::calculateoffsets(keyvalue_t * kvstats, vertex_t kvsize, vertex_t kvstatsoffset, unsigned int LLOPnumpartitions){				
	kvstats[kvstatsoffset + 0].key = 0;
	for (unsigned int p = 1; p<LLOPnumpartitions; p++){
		kvstats[kvstatsoffset + p].key = utilityobj[0]->hallignup_KV(kvstats[kvstatsoffset + p-1].key + kvstats[kvstatsoffset + p-1].value + LASTLEVELPARTITIONPADDING);
	}	
	
	for(unsigned int i = 0; i<KVSTATS_SIZE; i++){ kvstats[kvstatsoffset + i].value = 0; }
	kvstats[kvstatsoffset + 0].value = kvsize;
	return;
}
void host_enigma::loadmessages(keyvalue_t * messages, vertex_t offset, unsigned int IterCount){
	messages[getmessagesAddr(MESSAGES_PROCESSCOMMANDID)].key = ON;
	messages[getmessagesAddr(MESSAGES_PARTITIONCOMMANDID)].key = ON;
	messages[getmessagesAddr(MESSAGES_APPLYUPDATESCOMMANDID)].key = ON;
	messages[getmessagesAddr(MESSAGES_VOFFSET)].key = 0;
	messages[getmessagesAddr(MESSAGES_VSIZE)].key = KVDATA_RANGE_PERSSDPARTITION;
	messages[getmessagesAddr(MESSAGES_TREEDEPTH)].key = 3;//TREE_DEPTH;
	messages[getmessagesAddr(MESSAGES_FINALNUMPARTITIONS)].key = pow(NUM_PARTITIONS, TREE_DEPTH);
	messages[getmessagesAddr(MESSAGES_GRAPHITERATIONID)].key = 0;
	messages[getmessagesAddr(MESSAGES_ITERATIONID)].key = IterCount;
}

void host_enigma::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound){
	if(data > upper_bound){ cout<<"host_enigma::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<endl; exit(EXIT_FAILURE); }
}
void host_enigma::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; }
}
unsigned int host_enigma::getmessagesAddr(unsigned int addr){
	#pragma HLS INLINE
	return BASEOFFSET_MESSAGESDRAM + addr;
}
int host_enigma::runActs(unsigned int IterCount){
	if((IterCount % DRAMBATCHFACTOR) == (DRAMBATCHFACTOR - 1)){ return 1; }
	else { return 0; }
}

#ifdef SW
void host_enigma::launchswkernel(int threadidx){
	kernelobj->topkernel(
(uint512_dt *)kvsourcedram[threadidx][0]
		,(uint512_dt *)kvdestdram[threadidx][0]
		,(keyvalue_t *)kvstats[threadidx][0]
	);
	return;
}
#endif 
#ifdef FPGA_IMPL
void host_enigma::loadOCLstructures(std::string binaryFile){
	cl_int err;
	global = 1; local = 1;

	// kvsource_size_bytes = KVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvsource_size_bytes = PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt);
	kvdest_size_bytes = KVDATA_RANGE_PERSSDPARTITION_KVS * sizeof(uint512_vec_dt);
	kvstats_size_bytes = KVSTATSDRAMSZ * sizeof(keyvalue_t);
	
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
	for(int flag=0; flag<2; flag++){
		std::cout << "Allocate Buffer in Global Memory" << std::endl;
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_kvsourcedram[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvsource_size_bytes, kvsourcedram[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvsourcedram%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_kvdestdram[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvdest_size_bytes, kvdestdram[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvdestdram%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_kvstatsdram[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, kvstats[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvstatsdram%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
	}
	return;
}
void host_enigma::launchkernel(unsigned int flag){
	#ifdef LAUNCHKERNEL
	// Set the Kernel Arguments. Using setArg(), i.e. setting kernel arguments, explicitly before enqueueMigrateMemObjects(), 
	// i.e. copying host memory to device memory,  allowing runtime to associate buffer with correct DDR banks automatically. 
	std::cout << "Set the Kernel Arguments" << std::endl;
	int nargs=0;
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvsourcedram[flag][ddr]);
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvdestdram[flag][ddr]);
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvstatsdram[flag][ddr]);
	}
	
	// Copy input data to device global memory
	std::cout << "Copy input data to device global memory" << std::endl;
	array<cl_event, 1*NUMINSTANCESETS> write_events;
	
	// Copy data from Host to Device
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvsourcedram[flag][ddr], CL_FALSE, 0, kvsource_size_bytes, kvsourcedram[flag][ddr], 0, NULL, &write_events[ddr] ));
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdestdram[flag][ddr], CL_FALSE, 0, kvdest_size_bytes, kvdestdram[flag][ddr], 0, NULL, &write_events[1 + ddr] ));
	}
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvstatsdram[flag][ddr], CL_FALSE, 0, kvstats_size_bytes, kvstats[flag][ddr], 0, NULL, &write_events[2 + ddr] ));
	}
	
	// Launch the Kernel			
	printf("Enqueueing NDRange kernel.\n");
	std::cout << "Launch the Kernel" << std::endl;
	std::vector<cl_event> waitList; 
	for(unsigned int i=0; i<1*NUMINSTANCESETS; i++){ waitList.push_back(write_events[i]); }
	OCL_CHECK(clEnqueueNDRangeKernel(world.command_queue, kernel, 1, nullptr,
									&global, &local, 1*NUMINSTANCESETS, waitList.data(),
									&kernel_events[flag]));
	
	// Copy Result from Device Global Memory to Host Local Memory
	utilityobj[0]->set_callback(kernel_events[flag], "ooo_queue");
	clWaitForEvents(1, &kernel_events[flag]);
	std::cout << "Getting Results (Device to Host)..." << std::endl;
	printf("Enqueueing Migrate Mem Object (Device to Host) calls\n");
	#endif
}
void host_enigma::finishOCL(){
	cout<<"host_enigma::finish destroying memory structures... "<<endl;
	for(unsigned int flag=0; flag<2; flag++){ clWaitForEvents(1, &kernel_events[flag]); }
	
	cout<<"host_enigma::finish: releasing and destroying all OCL structures... "<<endl;
	#ifdef LAUNCHKERNEL
	clFlush(world.command_queue);
	clFinish(world.command_queue);
	for(int flag=0; flag<2; flag++){
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvsourcedram[flag][ddr])); }
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvdestdram[flag][ddr])); }
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvstatsdram[flag][ddr])); }
		clWaitForEvents(1, &kernel_events[flag]);
	}
	
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
	/** cout<<"free(world.platform_id)"<<endl;
	free(world.platform_id);
	cout<<"free(world.device_id)"<<endl;
	free(world.device_id); */
	cout<<"host_enigma::finish: released and destroyed all OCL structures"<<endl;
	#endif 
}
#endif 


