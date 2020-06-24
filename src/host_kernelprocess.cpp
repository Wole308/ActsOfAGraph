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
#include "VertexValues.h" // Grafboost header
#include "sortreduce.h" // Grafboost header
#include "filekvreader.h" // Grafboost header
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../algorithm/algorithm.h"
#include "../utility/utility.h"
#ifdef SW
#include "../kernels/kernelprocess.h"
#endif
#include "host_kernelprocess.h"
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

host_kernelprocess::host_kernelprocess(graph * _graphobj){
	graphobj = _graphobj;
	
	#ifdef SW
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvdram_source[i][ddr] = new uint512_vec_dt[_x3PADDEDKVDATA_SIZE_KVS]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvstats[i][ddr] = new keyvalue_t[KVSTATS_SIZE]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ messages[i][ddr] = new int[MESSAGES_SIZE]; }
	}
	#else
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvdram_source[i][ddr] = (uint512_vec_dt *) aligned_alloc(4096, (_x3PADDEDKVDATA_SIZE_KVS * sizeof(uint512_vec_dt))); }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ kvstats[i][ddr] = (keyvalue_t *) aligned_alloc(4096, (KVSTATS_SIZE * sizeof(keyvalue_t))); }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ messages[i][ddr] = (int *) aligned_alloc(4096, (MESSAGES_SIZE * sizeof(int))); }
	}
	#endif
	
	#ifdef SW 
	// legion * kernelobj = new legion();
	kernelprocess * kernelobj = new kernelprocess();
	#endif 
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edgeprocessobj[i] = new edge_process(graphobj); }
	// srand (time(NULL));
	srand (0);
}
host_kernelprocess::~host_kernelprocess(){
	cout<<"host_kernelprocess::~host_kernelprocess:: finish destroying memory structures... "<<endl;
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){		
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvdram_source[i][ddr]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] kvstats[i][ddr]; }
		for(unsigned int ddr=0; ddr<NUMDRAMBANKS; ddr++){ delete [] messages[i][ddr]; }
	}
	#ifdef FPGA_IMPL
	finishOCL();
	#endif 
}

void host_kernelprocess::run(){
	start();
	return;
}
void host_kernelprocess::start() {
	WorkerThread(0);
}
void host_kernelprocess::WorkerThread(int threadidx){
	loaddrams(threadidx);
	#ifdef SW
	launchswkernel(threadidx);
	#else 
	launchkernel(threadidx);
	#endif 
}
void host_kernelprocess::finish(){
	#if (defined(FPGA_IMPL) && defined(PR_ALGORITHM))
	finishOCL();
	#endif
}

void host_kernelprocess::loaddrams(int threadidx){
	#ifdef SW
	kernelobj->printparameters();
	kernelobj->clearglobalvars();
	#endif

	loadkvdram(kvdram_source[0][0], 0, KVDATA_BATCHSIZE_KVS);
	loadcapsulestats(kvdram_source[0][0], KVDRAMSZ_KVS, CAPSULESTATSBATCHSZ_KVS);
	loadstats(kvdram_source[0][0], (keyvalue_t *)kvstats[0][0], 0, KVDATA_BATCHSIZE, 0, pow(NUM_PARTITIONS, TREE_DEPTH), (KVDATA_RANGE_PERSSDPARTITION / pow(NUM_PARTITIONS, TREE_DEPTH)));
	loadmessages(messages[0][0]);
	
	#ifdef SW
	cout<<endl<<"=== host_kernelprocess:: loaddrams ENDED. printing summary and out messages === "<<endl;
	kernelobj->printglobalvars();	
	cout<<"host_kernelprocess::loaddrams: test ended."<<endl;
	kernelobj->printparameters();
	#endif 
	return;
}
void host_kernelprocess::loadkvdram(uint512_vec_dt * kvdram, unsigned int baseoffset_kvs, unsigned int kvsize_kvs){
	for(unsigned int i=0; i<kvsize_kvs; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram[baseoffset_kvs + i].data[v].key = rand() % KVDATA_RANGE_PERSSDPARTITION;
			kvdram[baseoffset_kvs + i].data[v].value = 0;
		}
	}
	// printkeyvalues("host_kernelprocess::loadkvdram", (keyvalue_t *)kvdram, 8);
	// exit(EXIT_SUCCESS);
	return;
}
void host_kernelprocess::loadcapsulestats(uint512_vec_dt * capsules, unsigned int baseoffset_kvs, unsigned int kvsize_kvs){
	cout<<"host_kernelprocess:: loadcapsulestats started."<<endl;
	for(unsigned int i_kvs=0; i_kvs<kvsize_kvs; i_kvs++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			capsules[baseoffset_kvs + i_kvs].data[v].key = 0; 
			capsules[baseoffset_kvs + i_kvs].data[v].value = 0;
		}
	}
	return;
}
void host_kernelprocess::loadstats(uint512_vec_dt * kvdram, keyvalue_t * kvstats, vertex_t kvdramoffset, vertex_t kvdramsz, vertex_t kvrangeoffset, unsigned int LLOPnumpartitions, unsigned int LLOPrangepartitions){
	vertex_t kvdramoffset_kvs = kvdramoffset / VECTOR_SIZE;
	vertex_t kvdramsz_kvs = kvdramsz / VECTOR_SIZE;
	unsigned int partition;	
	keyvalue_t * kvstatsbuffer = new keyvalue_t[KVSTATS_SIZE];	
	cout<<"host_kernelprocess::loadstats:: LLOPnumpartitions: "<<LLOPnumpartitions<<", LLOPrangepartitions: "<<LLOPrangepartitions<<endl;
	
	for(int i = 0 ; i<KVSTATS_SIZE; i++){ kvstatsbuffer[i].key = 0; kvstatsbuffer[i].value = 0; }
	for(unsigned int i=0; i<kvdramsz_kvs; i++){
		for(int j=0; j<VECTOR_SIZE; j++){
			partition = (kvdram[kvdramoffset_kvs + i].data[j].key - kvrangeoffset) / LLOPrangepartitions;
			kvstatsbuffer[partition].value += 1;
		}
	}	
	// calculate offsets
	kvstatsbuffer[0].key = 0;
	for (unsigned int p = 1; p<LLOPnumpartitions; p++){
		kvstatsbuffer[p].key = utilityobj[0]->hallignup_KV(kvstatsbuffer[p-1].key + kvstatsbuffer[p-1].value + (PADSIZE_PERLLOP / 2));
	}	
	// assign initial workload size
	for(unsigned int i = 0; i<KVSTATS_SIZE; i++){ kvstatsbuffer[i].value = 0; }
	kvstatsbuffer[0].value = kvdramsz;
	// copy back
	for(unsigned int i=0; i<KVSTATS_SIZE; i++){ kvstats[i] = kvstatsbuffer[i]; }
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	printkeyvalues("print kvstats", kvstatsbuffer, 8);
	cout<<"PADDEDKVDATA_BATCHSIZE: "<<PADDEDKVDATA_BATCHSIZE<<endl;
	cout<<"KVDATA_BATCHSIZE: "<<KVDATA_BATCHSIZE<<endl;
	cout<<"PADSIZE_PERLLOP: "<<PADSIZE_PERLLOP<<endl;
	#endif
	// exit(EXIT_SUCCESS);
	return;
}
void host_kernelprocess::loadmessages(int * messages){
	messages[MESSAGES_COMMANDID] = NAp;		
	messages[MESSAGES_PROCESSCOMMANDID] = OFF;
	messages[MESSAGES_PARTITIONCOMMANDID] = ON;
	messages[MESSAGES_APPLYUPDATESCOMMANDID] = OFF;
	messages[MESSAGES_KVDATASIZEID] = KVDATA_BATCHSIZE;
	messages[MESSAGES_FINALNUMPARTITIONSID] = NAp;
	messages[MESSAGES_TREEDEPTHID] = NAp;
	messages[MESSAGES_ITERATIONID] = 0;
	messages[MESSAGES_SSDPARTITIONID] = 0;
	messages[DRAM_VOFFSET] = 0;
	messages[DRAM_VSIZE] = NAp;
	messages[DRAM_TREEDEPTH] = TREE_DEPTH;
	messages[DRAM_FINALNUMPARTITIONS] = pow(NUM_PARTITIONS, TREE_DEPTH);
	messages[MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID] = PADDEDKVDATA_BATCHSIZE_KVS;
	messages[GRAPH_ITERATIONID] = 0;
}

void host_kernelprocess::checkoutofbounds(string message, unsigned int data, unsigned int upper_bound){
	if(data > upper_bound){ cout<<"host_kernelprocess::checkoutofbounds: ERROR. out of bounds. message: "<<message<<", data: "<<data<<", upper_bound: "<<upper_bound<<endl; exit(EXIT_FAILURE); }
}
void host_kernelprocess::printkeyvalues(string message, keyvalue_t * keyvalues, unsigned int size){
	cout<<endl<<"printkeyvalues:"<<message<<endl;
	for(unsigned int p=0; p<size; p++){ cout<<"keyvalues["<<p<<"].key: "<<keyvalues[p].key<<", keyvalues["<<p<<"].value: "<<keyvalues[p].value<<endl; }
}

#ifdef SW
void host_kernelprocess::launchswkernel(int threadidx){
	kernelobj->topkernel(
(uint512_dt *)kvdram_source[threadidx][0]
		,(metadata_t *)kvstats[threadidx][0]
		,(int *)messages[threadidx][0]
	);
	// cout<<"FINISHED kernel run."<<endl;
	return;
}
#endif 
#ifdef FPGA_IMPL
void host_kernelprocess::loadOCLstructures(std::string binaryFile){
	cl_int err;
	global = 1; local = 1;

	kvsource_size_bytes = _x3PADDEDKVDATA_SIZE_KVS * sizeof(uint512_vec_dt);
	kvstats_size_bytes = KVSTATS_SIZE * sizeof(metadata_t);
	messages_size_bytes = MESSAGES_SIZE * sizeof(int);
	
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
			buffer_sourcekvdram[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvsource_size_bytes, kvdram_source[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_sourcekvdram%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_kvstats[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, kvstats[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_kvstats%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
		for (int ddr = 0; ddr < NUMDRAMBANKS; ddr++){
			buffer_messages[flag][ddr] = clCreateBuffer(world.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, messages_size_bytes, messages[flag][ddr], &err);
			if(err != CL_SUCCESS) {
				printf("Error: Failed to create buffer_messages%i[%i] buffer arg. %i \n", ddr, flag, err);
				exit(EXIT_FAILURE);
			}
		}
	}
	return;
}
void host_kernelprocess::launchkernel(unsigned int flag){
	#ifdef LAUNCHKERNEL
	// Set the Kernel Arguments. Using setArg(), i.e. setting kernel arguments, explicitly before enqueueMigrateMemObjects(), 
	// i.e. copying host memory to device memory,  allowing runtime to associate buffer with correct DDR banks automatically. 
	std::cout << "Set the Kernel Arguments" << std::endl;
	int nargs=0;
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_sourcekvdram[flag][ddr]);
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvstats[flag][ddr]);
		xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_messages[flag][ddr]);
	}
	
	// Copy input data to device global memory
	std::cout << "Copy input data to device global memory" << std::endl;
	array<cl_event, 1*NUMINSTANCESETS> write_events;
	
	// Copy data from Host to Device	
	for (int ddr = 0; ddr < NUMINSTANCES; ddr++){
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_sourcekvdram[flag][ddr], CL_FALSE, 0, kvsource_size_bytes, kvdram_source[flag][ddr], 0, NULL, &write_events[ddr] ));
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvstats[flag][ddr], CL_FALSE, 0, kvstats_size_bytes, kvstats[flag][ddr], 0, NULL, &write_events[1 + ddr] ));
		OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_messages[flag][ddr], CL_FALSE, 0, messages_size_bytes, messages[flag][ddr], 0, NULL, &write_events[2 + ddr] ));
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
	std::cout << "Getting Results (Device to Host)..." << std::endl;
	printf("Enqueueing Migrate Mem Object (Device to Host) calls\n");
	utilityobj[0]->set_callback(kernel_events[flag], "ooo_queue");
	clWaitForEvents(1, &kernel_events[flag]);
	#endif
}
void host_kernelprocess::finishOCL(){
	cout<<"host_kernelprocess::finish destroying memory structures... "<<endl;
	for(unsigned int flag=0; flag<2; flag++){ clWaitForEvents(1, &kernel_events[flag]); }
	
	cout<<"host_kernelprocess::finish: releasing and destroying all OCL structures... "<<endl;
	#ifdef LAUNCHKERNEL
	clFlush(world.command_queue);
	clFinish(world.command_queue);
	for(int flag=0; flag<2; flag++){
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_sourcekvdram[flag][ddr])); }
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_kvstats[flag][ddr])); }
		for (int ddr = 0; ddr < NUMINSTANCES; ddr++){ OCL_CHECK(clReleaseMemObject(buffer_messages[flag][ddr])); }
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
	cout<<"host_kernelprocess::finish: released and destroyed all OCL structures"<<endl;
	#endif 
}
#endif 


