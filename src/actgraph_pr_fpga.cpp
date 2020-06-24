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
#include "kernelprocess.h"
#include "edge_process.h"
#include "../graphs/graph.h"
#include "../algorithm/algorithm.h"
#include "../utility/utility.h"
#include "actgraph_pr_fpga.h"
#include "actgraph_pr_sw.h"
#ifdef FPGA_IMPL
// #include "xcl2.hpp"
#include "../xcl.h"
#endif
using namespace std;
#define YES

std::mutex mutex4_workthread;

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
	
void event_cb(cl_event event, cl_int cmd_status, void *data) {
  cl_command_type command;
  clGetEventInfo(event, CL_EVENT_COMMAND_TYPE, sizeof(cl_command_type),
                 &command, nullptr);
  cl_int status;
  clGetEventInfo(event, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(cl_int),
                 &status, nullptr);
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
	printf("[%s]: %s %s\n", reinterpret_cast<char *>(data), status_str,
		 command_str);
	fflush(stdout);
}
void set_callback(cl_event event, const char *queue_name) {
  OCL_CHECK(
      clSetEventCallback(event, CL_COMPLETE, event_cb, (void *)queue_name));
}

actgraph_pr_fpga::actgraph_pr_fpga(unsigned int _graph_iterationidx, graph * graphobj, std::string binaryFile){			
	cout<<"actgraph_pr_fpga:: Constructor called: _graph_iterationidx: "<<_graph_iterationidx<<endl;

	// Variables
	cl_int err;
	global = 1; local = 1;
	
	kvdramsz_kvs = DRAMSZ_KVS; 
	kv_size_bytes = kvdramsz_kvs * sizeof(uint512_vec_dt);
	kvstats_size_bytes = KVSTATS_SIZE * sizeof(metadata_t);
	messages_size_bytes = MESSAGES_SIZE * sizeof(int);
	
	kvV1_size_bytes = PADDEDKVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvV2_size_bytes = PADDEDKVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvV3_size_bytes = VERTEXRANGEPERWORKLOADBANK_KVS * sizeof(uint512_vec_dt);
	
	kvV1_offset_bytes = 0 * sizeof(uint512_vec_dt);
	kvV2_offset_bytes = PADDEDKVDATA_BATCHSIZE_KVS * sizeof(uint512_vec_dt);
	kvV3_offset_bytes = _x2PADDEDKVDATA_SIZE_KVS * sizeof(uint512_vec_dt);

	// Map buffers to host address space 
	cout<<"actgraph_pr_fpga:: Map buffers to host address space "<<endl;
	for(int flag=0; flag<2; flag++){
		cData_kvdramAV[flag] = (uint512_vec_dt *) aligned_alloc(4096, (DRAMSZ_KVS * sizeof(uint512_vec_dt)));
		cData_kvstatsA[flag] = (metadata_t *) aligned_alloc(4096, (KVSTATS_SIZE * sizeof(metadata_t)));
		cData_messagesA[flag] = (int *) aligned_alloc(4096, (MESSAGES_SIZE * sizeof(int)));
		cData_kvdramBV[flag] = (uint512_vec_dt *) aligned_alloc(4096, (DRAMSZ_KVS * sizeof(uint512_vec_dt)));
		cData_kvstatsB[flag] = (metadata_t *) aligned_alloc(4096, (KVSTATS_SIZE * sizeof(metadata_t)));
		cData_messagesB[flag] = (int *) aligned_alloc(4096, (MESSAGES_SIZE * sizeof(int)));
		cData_kvdramCV[flag] = (uint512_vec_dt *) aligned_alloc(4096, (DRAMSZ_KVS * sizeof(uint512_vec_dt)));
		cData_kvstatsC[flag] = (metadata_t *) aligned_alloc(4096, (KVSTATS_SIZE * sizeof(metadata_t)));
		cData_messagesC[flag] = (int *) aligned_alloc(4096, (MESSAGES_SIZE * sizeof(int)));
		cData_kvdramDV[flag] = (uint512_vec_dt *) aligned_alloc(4096, (DRAMSZ_KVS * sizeof(uint512_vec_dt)));
		cData_kvstatsD[flag] = (metadata_t *) aligned_alloc(4096, (KVSTATS_SIZE * sizeof(metadata_t)));
		cData_messagesD[flag] = (int *) aligned_alloc(4096, (MESSAGES_SIZE * sizeof(int)));
 
	}
	
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
		buffer_kvdramA[flag] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kv_size_bytes, cData_kvdramAV[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_kvdramA[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_kvdramB[flag] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kv_size_bytes, cData_kvdramBV[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_kvdramB[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_kvdramC[flag] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kv_size_bytes, cData_kvdramCV[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_kvdramC[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_kvdramD[flag] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kv_size_bytes, cData_kvdramDV[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_kvdramD[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_kvstatsA[flag] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, cData_kvstatsA[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_kvstatsA[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_kvstatsB[flag] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, cData_kvstatsB[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_kvstatsB[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_kvstatsC[flag] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, cData_kvstatsC[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_kvstatsC[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_kvstatsD[flag] = clCreateBuffer(world.context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, kvstats_size_bytes, cData_kvstatsD[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_kvstatsD[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_messagesA[flag] = clCreateBuffer(world.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, messages_size_bytes, cData_messagesA[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_messagesA[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_messagesB[flag] = clCreateBuffer(world.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, messages_size_bytes, cData_messagesB[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_messagesB[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_messagesC[flag] = clCreateBuffer(world.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, messages_size_bytes, cData_messagesC[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_messagesC[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
		buffer_messagesD[flag] = clCreateBuffer(world.context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, messages_size_bytes, cData_messagesD[flag], &err);
		if(err != CL_SUCCESS) {
			printf("Error: Failed to create buffer_messagesD[%i] buffer arg. %i \n", flag, err);
			exit(EXIT_FAILURE);
		}
	}
	
	numvertexbanks = graphobj->getnumvertexbanks();
	numedgebanks = graphobj->getnumedgebanks();
	numverticespervertexbank = KVDATA_RANGE / graphobj->getnumvertexbanks();
	
	for(unsigned int i=0; i<numedgebanks; i++) { for(unsigned int j=0; j<numedgebanks; j++) { nvmeFd_vertexoffsets_r2[i][j] = graphobj->getnvmeFd_edgeoffsets_r2()[i][j]; }}
	for(unsigned int i=0; i<numvertexbanks; i++) { nvmeFd_verticesdata_r2[i] = graphobj->getnvmeFd_verticesdata_r2()[i]; }
	for(unsigned int i=0; i<numvertexbanks; i++) { nvmeFd_verticesdata_w2[i] = graphobj->getnvmeFd_verticesdata_w2()[i]; }
	for(unsigned int i=0; i<numvertexbanks; i++) { nvmeFd_vertexproperties_r2[i] = graphobj->getnvmeFd_vertexproperties_r2()[i]; }
	for(unsigned int i=0; i<numedgebanks; i++) { for(unsigned int j=0; j<numedgebanks; j++) { nvmeFd_edgeproperties_r2[i][j] = graphobj->getnvmeFd_edgeproperties_r2()[i][j]; }}

	graph_iterationidx = _graph_iterationidx;
	
	totaltime_topkernel_ms = 0;
	totaltime_populatekvdram_ms = 0;

	actgraph_pr_sw_obj = new actgraph_pr_sw(); 
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edge_process_obj[i] = new edge_process(graphobj); }
	for(unsigned int i=0; i<NUMDRAMBANKS; i++){ edge_process_obj2[i] = new edge_process(graphobj); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ utilityobj[i] = new utility(); }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ algorithmobj[i] = new algorithm(); }
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ totalnumkvsread[i] = 0; }
	for(unsigned int i=0; i<numvertexbanks; i++){ vertexpropertiesbuffer[i] = new vertexprop_t[numverticespervertexbank]; }
	for(unsigned int j=0; j<NUMDRAMBANKS; j++){ edgesbuffer[j] = new edgeprop2_t[KVDATA_BATCHSIZE]; }
	for(unsigned int j=0; j<NUMDRAMBANKS; j++){ vertexoffsetsbuffer[j] = new prvertexoffset_t[(KVDATA_BATCHSIZE * 2)]; }
}
actgraph_pr_fpga::~actgraph_pr_fpga() {} 

void actgraph_pr_sw::run(){
	for(unsigned int graph_iterationidx=0; graph_iterationidx<1; graph_iterationidx++){
		cout<<"actgraph_pr_sw::run: graph iteration "<<graph_iterationidx<<" of pagerank Started"<<endl;
		start();
		summary();
	}
	return;
}
void actgraph_pr_fpga::start() {
	for(unsigned int i_batch=0; i_batch<numvertexbanks; i_batch += NUMDRAMBANKS){
		cout<<">>> actgraph_pr_fpga::start: super iteration: [i_batch: "<<i_batch<<"][size: "<<numvertexbanks<<"][step: "<<NUMDRAMBANKS<<"]"<<endl;
		for (int i = 0; i < NUMCPUTHREADS; i++) { actgraph_pr_sw_obj->loadvertexpropertiesfromfile(nvmeFd_vertexproperties_r2[(i_batch + i)], 0, vertexpropertiesbuffer[i], 0, numverticespervertexbank); }
		for (int i = 0; i < NUMCPUTHREADS; i++) { WorkerThread(i, i_batch); }
	}
	finish();
}
void actgraph_pr_fpga::finish(){
	cout<<"actgraph_pr_sw::finish destroying memory structures... "<<endl;
	for(unsigned int flag=0; flag<2; flag++){ clWaitForEvents(1, &kernel_events[flag]); }
	
	cout<<"actgraph_pr_sw::finish destroying OCL structures... "<<endl;
	#ifdef LAUNCHKERNEL
	clFlush(world.command_queue);
	clFinish(world.command_queue);
	for(int flag=0; flag<2; flag++){
		OCL_CHECK(clReleaseMemObject(buffer_kvdramA[flag]));
		OCL_CHECK(clReleaseMemObject(buffer_kvdramB[flag]));
		OCL_CHECK(clReleaseMemObject(buffer_kvdramC[flag]));
		OCL_CHECK(clReleaseMemObject(buffer_kvdramD[flag]));
		
		OCL_CHECK(clReleaseMemObject(buffer_kvstatsA[flag]));
		OCL_CHECK(clReleaseMemObject(buffer_kvstatsB[flag]));
		OCL_CHECK(clReleaseMemObject(buffer_kvstatsC[flag]));
		OCL_CHECK(clReleaseMemObject(buffer_kvstatsD[flag]));
		
		OCL_CHECK(clReleaseMemObject(buffer_messagesA[flag]));
		OCL_CHECK(clReleaseMemObject(buffer_messagesB[flag]));
		OCL_CHECK(clReleaseMemObject(buffer_messagesC[flag]));
		OCL_CHECK(clReleaseMemObject(buffer_messagesD[flag]));
		clWaitForEvents(1, &kernel_events[flag]);
		// OCL_CHECK(clReleaseEvent(kernel_events[flag]));
	}
	OCL_CHECK(clReleaseKernel(kernel));
	OCL_CHECK(clReleaseProgram(program));
	OCL_CHECK(clReleaseCommandQueue(world.command_queue));
	OCL_CHECK(clReleaseContext(world.context));
	xcl_release_world(world);
	#endif 
	
	cout<<"actgraph_pr_sw::finish destroying dynamic memory structures... "<<endl;
	for(unsigned int flag=0; flag<2; flag++){
		delete[] cData_kvdramAV[flag];
		delete[] cData_kvstatsA[flag];
		delete[] cData_messagesA[flag];
		delete[] cData_kvdramBV[flag];
		delete[] cData_kvstatsB[flag];
		delete[] cData_messagesB[flag];
		delete[] cData_kvdramCV[flag];
		delete[] cData_kvstatsC[flag];
		delete[] cData_messagesC[flag];
		delete[] cData_kvdramDV[flag];
		delete[] cData_kvstatsD[flag];
		delete[] cData_messagesD[flag];
	}
}
void actgraph_pr_fpga::totalpopulateKvDRAMtime(){
	edge_t _totalnumkvsread = 0;
	for ( int i = 0; i < NUMCPUTHREADS; i++ ) {
		cout<<">>> actgraph_pr_fpga::totalpopulateKvDRAMtime Total number of kvReads for thread ["<<i<<"]: "<<totalnumkvsread[i]<<endl;
		_totalnumkvsread += totalnumkvsread[i];
	}
	cout<<">>> actgraph_pr_fpga::totalpopulateKvDRAMtime Total number of kvReads for all threads: "<<_totalnumkvsread<<endl;
	#ifdef LOCKE
	cout<<">>> actgraph_pr_fpga::totalpopulateKvDRAMtime Total kernel time: "<<totaltime_topkernel_ms<<" milli seconds"<<endl;
	cout<<">>> actgraph_pr_fpga::totalpopulateKvDRAMtime Total kernel time: "<<(totaltime_topkernel_ms / 1000)<<" seconds"<<endl;	
	cout<<">>> actgraph_pr_fpga::totalpopulateKvDRAMtime Total populate KvDRAM time: "<<totaltime_populatekvdram_ms<<" milli seconds"<<endl;
	cout<<">>> actgraph_pr_fpga::totalpopulateKvDRAMtime Total populate KvDRAM time: "<<(totaltime_populatekvdram_ms / 1000)<<" seconds"<<endl;	
	#endif 
	return;
}
float actgraph_pr_fpga::totalkerneltime(){ return totaltime_topkernel_ms; }
float actgraph_pr_fpga::TotalPopulateKvDRAMTime(){ return totaltime_populatekvdram_ms; }

void actgraph_pr_fpga::WorkerThread(int threadidx, int gbankoffset){
	#ifdef HWLOCKE
	mutex4_workthread.lock(); // lock
	#endif
	#ifdef _DEBUGMODE_TIMERS
	std::chrono::steady_clock::time_point begintime_partitionandapply = std::chrono::steady_clock::now();
	#endif
	
	unsigned int giteration_idx = 0;
	unsigned int numbanksassignedtoeachthread = numedgebanks / NUMCPUTHREADS;
	unsigned int subthreadidxoffset = threadidx * (numedgebanks / NUMCPUTHREADS);
	for(unsigned int subthreadidx = 0; subthreadidx < numbanksassignedtoeachthread; subthreadidx++){ // column

		// load vertices data
		actgraph_pr_sw_obj->loadverticesdatafromfile(nvmeFd_verticesdata_r2[(subthreadidxoffset + subthreadidx)], 0, cData_kvdramAV[0], (2 * PADDEDKVDATA_BATCHSIZE), actgraph_pr_sw_obj->GETKVDATA_RANGE_FORSSDPARTITION_((gbankoffset + subthreadidx)));
		actgraph_pr_sw_obj->replicateverticesdata(cData_kvdramAV[0],cData_kvdramBV[0],cData_kvdramCV[0],cData_kvdramDV[0],(2 * PADDEDKVDATA_BATCHSIZE_KVS), actgraph_pr_sw_obj->GETKVDATA_RANGE_FORSSDPARTITION_((gbankoffset + subthreadidx)));
		
		// each drambank of each threadidx handles a (source) workbank
		for(unsigned int lbankoffset = 0; lbankoffset < NUMCPUTHREADS; lbankoffset += NUMDRAMBANKS){ // bank set

			vertex_t edgepropertyfilesize[NUMDRAMBANKS];
			unsigned int _iteration_size[NUMDRAMBANKS];	
			edgepropertyfilesize[0] = lseek(nvmeFd_edgeproperties_r2[(gbankoffset + lbankoffset + 0)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeof(edgeprop2_t);
			_iteration_size[0] = utilityobj[threadidx]->hceildiv((lseek(nvmeFd_edgeproperties_r2[(gbankoffset + lbankoffset + 0)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeof(edgeprop2_t)), KVDATA_BATCHSIZE);
			edgepropertyfilesize[1] = lseek(nvmeFd_edgeproperties_r2[(gbankoffset + lbankoffset + 1)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeof(edgeprop2_t);
			_iteration_size[1] = utilityobj[threadidx]->hceildiv((lseek(nvmeFd_edgeproperties_r2[(gbankoffset + lbankoffset + 1)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeof(edgeprop2_t)), KVDATA_BATCHSIZE);
			edgepropertyfilesize[2] = lseek(nvmeFd_edgeproperties_r2[(gbankoffset + lbankoffset + 2)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeof(edgeprop2_t);
			_iteration_size[2] = utilityobj[threadidx]->hceildiv((lseek(nvmeFd_edgeproperties_r2[(gbankoffset + lbankoffset + 2)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeof(edgeprop2_t)), KVDATA_BATCHSIZE);
			edgepropertyfilesize[3] = lseek(nvmeFd_edgeproperties_r2[(gbankoffset + lbankoffset + 3)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeof(edgeprop2_t);
			_iteration_size[3] = utilityobj[threadidx]->hceildiv((lseek(nvmeFd_edgeproperties_r2[(gbankoffset + lbankoffset + 3)][(subthreadidxoffset + subthreadidx)], 0, SEEK_END) / sizeof(edgeprop2_t)), KVDATA_BATCHSIZE);
			unsigned int iteration_size = 0;
			for(unsigned int i=0; i<NUMDRAMBANKS; i++){ iteration_size = actgraph_pr_sw_obj->hmax(iteration_size, _iteration_size[i]); }			
			#ifdef _DEBUGMODE_HOSTPRINTS
			for(unsigned int i=0; i<NUMDRAMBANKS; i++){  cout<<"actgraph_pr_sw::WorkerThread: edgepropertyfilesize["<<i<<"]: "<<edgepropertyfilesize[i]<<", numverticespervertexbank: "<<numverticespervertexbank<<", threadidx: "<<threadidx<<", subthreadidx: "<<(subthreadidxoffset + subthreadidx)<<", DRAMSZ_KVS: "<<DRAMSZ_KVS<<endl; }			
			#endif
			
			for (unsigned int iteration_idx = 0; iteration_idx < iteration_size; iteration_idx++) {
				#ifdef _DEBUGMODE_HOSTPRINTS2
				cout<<"PP&A:: [threadidx:"<<threadidx<<"][size:"<<NUMCPUTHREADS<<"][step:1], [subthreadidx:"<<(subthreadidxoffset + subthreadidx)<<"][size:"<<(numedgebanks / NUMCPUTHREADS)<<"][step:1], [bank offset:"<<(gbankoffset + lbankoffset)<<"][size:"<<numedgebanks<<"][step:"<<NUMDRAMBANKS<<"], [iteration_idx:"<<iteration_idx<<"][size:"<<iteration_size<<"][step:1]"<<endl;		
				#endif 
				#ifdef _DEBUGMODE_TIMERS
				std::chrono::steady_clock::time_point begintime_iterationidx = std::chrono::steady_clock::now();
				#endif
				
				// wait for events to finish
				int flag = giteration_idx % 2;
				#ifdef LAUNCHKERNEL
				if (giteration_idx >= 2) {
					#ifdef _DEBUGMODE_HOSTPRINTS
					cout<<"actgraph_pr_fpga::WorkerThread: waiting for events2..."<<endl;
					clWaitForEvents(1, &read_event[7][flag]);
					#else 
					cout<<"actgraph_pr_fpga::WorkerThread: waiting for events..."<<endl;
					clWaitForEvents(1, &kernel_events[flag]);
					#endif 
				}
				#endif 
				
				// Populate kvdrams 
				#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
				std::chrono::steady_clock::time_point begintime_populatekvdram = std::chrono::steady_clock::now();
				#endif
				#ifdef LOCKE
				
				size_t bytes_edgesoffsetA = (iteration_idx * KVDATA_BATCHSIZE) * sizeof(edgeprop2_t);
				vertex_t edgeszA = actgraph_pr_sw_obj->hmin(KVDATA_BATCHSIZE, actgraph_pr_sw_obj->hsub((size_t)edgepropertyfilesize[0], (size_t)((size_t)((size_t)iteration_idx * (size_t)KVDATA_BATCHSIZE))));
				edge_process_obj[threadidx]->generateupdatesII(bytes_edgesoffsetA, edgeszA, nvmeFd_edgeproperties_r2[gbankoffset + lbankoffset + 0][(subthreadidxoffset + subthreadidx)], nvmeFd_vertexoffsets_r2[gbankoffset + lbankoffset + 0][(subthreadidxoffset + subthreadidx)], edgesbuffer[0], vertexoffsetsbuffer[0], vertexpropertiesbuffer[lbankoffset], cData_kvdramAV[flag], cData_kvstatsA[flag], (subthreadidxoffset + subthreadidx));				
				totalnumkvsread[threadidx] += edgeszA;
				
				size_t bytes_edgesoffsetB = (iteration_idx * KVDATA_BATCHSIZE) * sizeof(edgeprop2_t);
				vertex_t edgeszB = actgraph_pr_sw_obj->hmin(KVDATA_BATCHSIZE, actgraph_pr_sw_obj->hsub((size_t)edgepropertyfilesize[1], (size_t)((size_t)((size_t)iteration_idx * (size_t)KVDATA_BATCHSIZE))));
				edge_process_obj[threadidx]->generateupdatesII(bytes_edgesoffsetB, edgeszB, nvmeFd_edgeproperties_r2[gbankoffset + lbankoffset + 1][(subthreadidxoffset + subthreadidx)], nvmeFd_vertexoffsets_r2[gbankoffset + lbankoffset + 1][(subthreadidxoffset + subthreadidx)], edgesbuffer[1], vertexoffsetsbuffer[1], vertexpropertiesbuffer[lbankoffset], cData_kvdramBV[flag], cData_kvstatsB[flag], (subthreadidxoffset + subthreadidx));				
				totalnumkvsread[threadidx] += edgeszB;
				
				size_t bytes_edgesoffsetC = (iteration_idx * KVDATA_BATCHSIZE) * sizeof(edgeprop2_t);
				vertex_t edgeszC = actgraph_pr_sw_obj->hmin(KVDATA_BATCHSIZE, actgraph_pr_sw_obj->hsub((size_t)edgepropertyfilesize[2], (size_t)((size_t)((size_t)iteration_idx * (size_t)KVDATA_BATCHSIZE))));
				edge_process_obj[threadidx]->generateupdatesII(bytes_edgesoffsetC, edgeszC, nvmeFd_edgeproperties_r2[gbankoffset + lbankoffset + 2][(subthreadidxoffset + subthreadidx)], nvmeFd_vertexoffsets_r2[gbankoffset + lbankoffset + 2][(subthreadidxoffset + subthreadidx)], edgesbuffer[2], vertexoffsetsbuffer[2], vertexpropertiesbuffer[lbankoffset], cData_kvdramCV[flag], cData_kvstatsC[flag], (subthreadidxoffset + subthreadidx));				
				totalnumkvsread[threadidx] += edgeszC;
				
				size_t bytes_edgesoffsetD = (iteration_idx * KVDATA_BATCHSIZE) * sizeof(edgeprop2_t);
				vertex_t edgeszD = actgraph_pr_sw_obj->hmin(KVDATA_BATCHSIZE, actgraph_pr_sw_obj->hsub((size_t)edgepropertyfilesize[3], (size_t)((size_t)((size_t)iteration_idx * (size_t)KVDATA_BATCHSIZE))));
				edge_process_obj[threadidx]->generateupdatesII(bytes_edgesoffsetD, edgeszD, nvmeFd_edgeproperties_r2[gbankoffset + lbankoffset + 3][(subthreadidxoffset + subthreadidx)], nvmeFd_vertexoffsets_r2[gbankoffset + lbankoffset + 3][(subthreadidxoffset + subthreadidx)], edgesbuffer[3], vertexoffsetsbuffer[3], vertexpropertiesbuffer[lbankoffset], cData_kvdramDV[flag], cData_kvstatsD[flag], (subthreadidxoffset + subthreadidx));				
				totalnumkvsread[threadidx] += edgeszD;
				#else 				
				// cout<<"launching worker threads to generate kvs..... "<<endl;
				for (int i = 0; i < NUMDRAMBANKS; i++) { genw_thread[i] = std::thread(&actgraph_pr_fpga::workerthread_generatekvs, this, i, subthreadidxoffset, subthreadidx, gbankoffset, lbankoffset, edgepropertyfilesize[i], iteration_idx, flag); }
				// cout<<"joining generate kvs worker threads..... "<<endl;
				for ( int i = 0; i < NUMDRAMBANKS; i++ ) { genw_thread[i].join(); }
				// cout<<"kvs worker threads joined successfully..... "<<endl;
				#endif
				#ifdef _DEBUGMODE_TIMERS
				actgraph_pr_sw_obj->stopTIME("PROCESS, PARTITION AND APPLY PHASE (FPGA): Populate Kv DRAM Time Elapsed: ", begintime_populatekvdram, iteration_idx);
				#endif
				#ifdef LOCKE
				totaltime_populatekvdram_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_populatekvdram).count();
				#endif
				// continue;
				// exit(EXIT_SUCCESS);
				
				cData_messagesA[flag][MESSAGES_COMMANDID] = NAp;		
				cData_messagesA[flag][MESSAGES_PROCESSCOMMANDID] = OFF;
				cData_messagesA[flag][MESSAGES_PARTITIONCOMMANDID] = ON;
				cData_messagesA[flag][MESSAGES_APPLYUPDATESCOMMANDID] = ON; // ON*, OFF
				cData_messagesA[flag][MESSAGES_KVDATASIZEID] = KVDATA_BATCHSIZE;
				cData_messagesA[flag][MESSAGES_FINALNUMPARTITIONSID] = NAp;
				cData_messagesA[flag][MESSAGES_TREEDEPTHID] = NAp;
				cData_messagesA[flag][MESSAGES_ITERATIONID] = iteration_idx;
				cData_messagesA[flag][MESSAGES_SSDPARTITIONID] = (subthreadidxoffset + subthreadidx); 
				cData_messagesA[flag][DRAM_VOFFSET] = 0; 
				cData_messagesA[flag][DRAM_VSIZE] = NAp;
				cData_messagesA[flag][DRAM_TREEDEPTH] = actgraph_pr_sw_obj->GETTREEDEPTH_((subthreadidxoffset + subthreadidx)); 
				cData_messagesA[flag][DRAM_FINALNUMPARTITIONS] = pow(NUM_PARTITIONS, actgraph_pr_sw_obj->GETTREEDEPTH_((subthreadidxoffset + subthreadidx))); 
				cData_messagesA[flag][MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID] = PADDEDKVDATA_BATCHSIZE_KVS;
				cData_messagesA[flag][GRAPH_ITERATIONID] = graph_iterationidx;
				cData_messagesB[flag][MESSAGES_COMMANDID] = NAp;		
				cData_messagesB[flag][MESSAGES_PROCESSCOMMANDID] = OFF;
				cData_messagesB[flag][MESSAGES_PARTITIONCOMMANDID] = ON;
				cData_messagesB[flag][MESSAGES_APPLYUPDATESCOMMANDID] = ON; // ON*, OFF
				cData_messagesB[flag][MESSAGES_KVDATASIZEID] = KVDATA_BATCHSIZE;
				cData_messagesB[flag][MESSAGES_FINALNUMPARTITIONSID] = NAp;
				cData_messagesB[flag][MESSAGES_TREEDEPTHID] = NAp;
				cData_messagesB[flag][MESSAGES_ITERATIONID] = iteration_idx;
				cData_messagesB[flag][MESSAGES_SSDPARTITIONID] = (subthreadidxoffset + subthreadidx); 
				cData_messagesB[flag][DRAM_VOFFSET] = 0; 
				cData_messagesB[flag][DRAM_VSIZE] = NAp;
				cData_messagesB[flag][DRAM_TREEDEPTH] = actgraph_pr_sw_obj->GETTREEDEPTH_((subthreadidxoffset + subthreadidx)); 
				cData_messagesB[flag][DRAM_FINALNUMPARTITIONS] = pow(NUM_PARTITIONS, actgraph_pr_sw_obj->GETTREEDEPTH_((subthreadidxoffset + subthreadidx))); 
				cData_messagesB[flag][MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID] = PADDEDKVDATA_BATCHSIZE_KVS;
				cData_messagesB[flag][GRAPH_ITERATIONID] = graph_iterationidx;
				cData_messagesC[flag][MESSAGES_COMMANDID] = NAp;		
				cData_messagesC[flag][MESSAGES_PROCESSCOMMANDID] = OFF;
				cData_messagesC[flag][MESSAGES_PARTITIONCOMMANDID] = ON;
				cData_messagesC[flag][MESSAGES_APPLYUPDATESCOMMANDID] = ON; // ON*, OFF
				cData_messagesC[flag][MESSAGES_KVDATASIZEID] = KVDATA_BATCHSIZE;
				cData_messagesC[flag][MESSAGES_FINALNUMPARTITIONSID] = NAp;
				cData_messagesC[flag][MESSAGES_TREEDEPTHID] = NAp;
				cData_messagesC[flag][MESSAGES_ITERATIONID] = iteration_idx;
				cData_messagesC[flag][MESSAGES_SSDPARTITIONID] = (subthreadidxoffset + subthreadidx); 
				cData_messagesC[flag][DRAM_VOFFSET] = 0; 
				cData_messagesC[flag][DRAM_VSIZE] = NAp;
				cData_messagesC[flag][DRAM_TREEDEPTH] = actgraph_pr_sw_obj->GETTREEDEPTH_((subthreadidxoffset + subthreadidx)); 
				cData_messagesC[flag][DRAM_FINALNUMPARTITIONS] = pow(NUM_PARTITIONS, actgraph_pr_sw_obj->GETTREEDEPTH_((subthreadidxoffset + subthreadidx))); 
				cData_messagesC[flag][MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID] = PADDEDKVDATA_BATCHSIZE_KVS;
				cData_messagesC[flag][GRAPH_ITERATIONID] = graph_iterationidx;
				cData_messagesD[flag][MESSAGES_COMMANDID] = NAp;		
				cData_messagesD[flag][MESSAGES_PROCESSCOMMANDID] = OFF;
				cData_messagesD[flag][MESSAGES_PARTITIONCOMMANDID] = ON;
				cData_messagesD[flag][MESSAGES_APPLYUPDATESCOMMANDID] = ON; // ON*, OFF
				cData_messagesD[flag][MESSAGES_KVDATASIZEID] = KVDATA_BATCHSIZE;
				cData_messagesD[flag][MESSAGES_FINALNUMPARTITIONSID] = NAp;
				cData_messagesD[flag][MESSAGES_TREEDEPTHID] = NAp;
				cData_messagesD[flag][MESSAGES_ITERATIONID] = iteration_idx;
				cData_messagesD[flag][MESSAGES_SSDPARTITIONID] = (subthreadidxoffset + subthreadidx); 
				cData_messagesD[flag][DRAM_VOFFSET] = 0; 
				cData_messagesD[flag][DRAM_VSIZE] = NAp;
				cData_messagesD[flag][DRAM_TREEDEPTH] = actgraph_pr_sw_obj->GETTREEDEPTH_((subthreadidxoffset + subthreadidx)); 
				cData_messagesD[flag][DRAM_FINALNUMPARTITIONS] = pow(NUM_PARTITIONS, actgraph_pr_sw_obj->GETTREEDEPTH_((subthreadidxoffset + subthreadidx))); 
				cData_messagesD[flag][MESSAGES_PADDEDKVDATA_BATCHSIZE_KVS_ID] = PADDEDKVDATA_BATCHSIZE_KVS;
				cData_messagesD[flag][GRAPH_ITERATIONID] = graph_iterationidx;
	
				
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"Host::Process, Partition and Apply phase (FPGA):: Print results before Kernel run "<<endl;
				printstructures(threadidx, flag);
				#endif
				// exit(EXIT_SUCCESS);
				
				#ifdef LAUNCHKERNEL
				// Prepare & Launch the Kernel
				#if (defined(_DEBUGMODE_TIMERS) || defined(LOCKE))
				std::chrono::steady_clock::time_point begintime_topkernel = std::chrono::steady_clock::now();
				#endif
				// Set the Kernel Arguments. Using setArg(), i.e. setting kernel arguments, explicitly before enqueueMigrateMemObjects(), 
				// i.e. copying host memory to device memory,  allowing runtime to associate buffer with correct DDR banks automatically. 
				std::cout << "Set the Kernel Arguments" << std::endl;
				int nargs=0;
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvdramA[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvdramB[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvdramC[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvdramD[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvstatsA[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvstatsB[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvstatsC[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_kvstatsD[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_messagesA[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_messagesB[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_messagesC[flag]);
				xcl_set_kernel_arg(kernel, nargs++, sizeof(cl_mem), &buffer_messagesD[flag]);
				
				// Copy input data to device global memory
				std::cout << "Copy input data to device global memory" << std::endl;
				array<cl_event, 12> write_events;
				
				// Copy data from Host to Device
				// std::chrono::steady_clock::time_point begintime_enqueuewrite = std::chrono::steady_clock::now();
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdramA[flag], CL_FALSE, kvV1_offset_bytes, kvV1_size_bytes, cData_kvdramAV[flag], 0, NULL, &write_events[0] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdramB[flag], CL_FALSE, kvV1_offset_bytes, kvV1_size_bytes, cData_kvdramBV[flag], 0, NULL, &write_events[1] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdramC[flag], CL_FALSE, kvV1_offset_bytes, kvV1_size_bytes, cData_kvdramCV[flag], 0, NULL, &write_events[2] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvdramD[flag], CL_FALSE, kvV1_offset_bytes, kvV1_size_bytes, cData_kvdramDV[flag], 0, NULL, &write_events[3] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvstatsA[flag], CL_FALSE, 0, kvstats_size_bytes, cData_kvstatsA[flag], 0, NULL, &write_events[4] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvstatsB[flag], CL_FALSE, 0, kvstats_size_bytes, cData_kvstatsB[flag], 0, NULL, &write_events[5] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvstatsC[flag], CL_FALSE, 0, kvstats_size_bytes, cData_kvstatsC[flag], 0, NULL, &write_events[6] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_kvstatsD[flag], CL_FALSE, 0, kvstats_size_bytes, cData_kvstatsD[flag], 0, NULL, &write_events[7] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_messagesA[flag], CL_FALSE, 0, messages_size_bytes, cData_messagesA[flag], 0, NULL, &write_events[8] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_messagesB[flag], CL_FALSE, 0, messages_size_bytes, cData_messagesB[flag], 0, NULL, &write_events[9] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_messagesC[flag], CL_FALSE, 0, messages_size_bytes, cData_messagesC[flag], 0, NULL, &write_events[10] ));
				OCL_CHECK(clEnqueueWriteBuffer(world.command_queue, buffer_messagesD[flag], CL_FALSE, 0, messages_size_bytes, cData_messagesD[flag], 0, NULL, &write_events[11] ));
				// actgraph_pr_sw_obj->stopTIME("PROCESS, PARTITION AND APPLY PHASE (FPGA): Enqueue Write Time Elapsed: ", begintime_enqueuewrite, iteration_idx);
				
				// Launch the Kernel			
				printf("Enqueueing NDRange kernel.\n");
				std::cout << "Launch the Kernel" << std::endl;
				std::vector<cl_event> waitList; 
				for(unsigned int i=0; i<12; i++){ waitList.push_back(write_events[i]); }
				OCL_CHECK(clEnqueueNDRangeKernel(world.command_queue, kernel, 1, nullptr,
												&global, &local, 12, waitList.data(),
												&kernel_events[flag]));
				
				// Copy Result from Device Global Memory to Host Local Memory
				std::cout << "Getting Results (Device to Host)..." << std::endl;
				printf("Enqueueing Migrate Mem Object (Device to Host) calls\n");
				set_callback(kernel_events[flag], "ooo_queue");
				#ifdef LOCKE
				clWaitForEvents(1, &kernel_events[flag]); // REMOVEME
				#endif 
				
				// Copy input data from device global memory back to host
				// This operation only needs to wait for the kernel call. This call will
				// potentially overlap the next kernel call as well as the next read
				// operations
				#if (defined(_DEBUGMODE_HOSTPRINTS) & defined(NO))
				std::cout << "Copy input data to host memory" << std::endl;
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramA[flag], CL_FALSE, kvV1_offset_bytes, kvV1_size_bytes, cData_kvdramAV1[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramA[flag], CL_FALSE, kvV2_offset_bytes, kvV2_size_bytes, cData_kvdramAV2[flag], 1, &kernel_events[flag], &read_event[0][flag] ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramA[flag], CL_FALSE, kvV3_offset_bytes, kvV3_size_bytes, cData_kvdramAV3[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramB[flag], CL_FALSE, kvV1_offset_bytes, kvV1_size_bytes, cData_kvdramBV1[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramB[flag], CL_FALSE, kvV2_offset_bytes, kvV2_size_bytes, cData_kvdramBV2[flag], 1, &read_event[0][flag], &read_event[1][flag] ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramB[flag], CL_FALSE, kvV3_offset_bytes, kvV3_size_bytes, cData_kvdramBV3[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramC[flag], CL_FALSE, kvV1_offset_bytes, kvV1_size_bytes, cData_kvdramCV1[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramC[flag], CL_FALSE, kvV2_offset_bytes, kvV2_size_bytes, cData_kvdramCV2[flag], 1, &read_event[1][flag], &read_event[2][flag] ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramC[flag], CL_FALSE, kvV3_offset_bytes, kvV3_size_bytes, cData_kvdramCV3[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramD[flag], CL_FALSE, kvV1_offset_bytes, kvV1_size_bytes, cData_kvdramDV1[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramD[flag], CL_FALSE, kvV2_offset_bytes, kvV2_size_bytes, cData_kvdramDV2[flag], 1, &read_event[2][flag], &read_event[3][flag] ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvdramD[flag], CL_FALSE, kvV3_offset_bytes, kvV3_size_bytes, cData_kvdramDV3[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvstatsA[flag], CL_FALSE, 0, kvstats_size_bytes, cData_kvstatsA[flag], 1, &read_event[3][flag], &read_event[4][flag] ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvstatsB[flag], CL_FALSE, 0, kvstats_size_bytes, cData_kvstatsB[flag], 1, &read_event[4][flag], &read_event[5][flag] ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvstatsC[flag], CL_FALSE, 0, kvstats_size_bytes, cData_kvstatsC[flag], 1, &read_event[5][flag], &read_event[6][flag] ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_kvstatsD[flag], CL_FALSE, 0, kvstats_size_bytes, cData_kvstatsD[flag], 1, &read_event[6][flag], &read_event[7][flag] ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_messagesA[flag], CL_FALSE, 0, messages_size_bytes, cData_messagesA[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_messagesB[flag], CL_FALSE, 0, messages_size_bytes, cData_messagesB[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_messagesC[flag], CL_FALSE, 0, messages_size_bytes, cData_messagesC[flag], 0, NULL, NULL ));
				OCL_CHECK(clEnqueueReadBuffer(world.command_queue, buffer_messagesD[flag], CL_FALSE, 0, messages_size_bytes, cData_messagesD[flag], 0, NULL, NULL ));
				set_callback(read_event[7][flag], "ooo_queue");
				clWaitForEvents(1, &read_event[7][flag]); // FIXME. remove
				#endif
				#ifdef _DEBUGMODE_TIMERS
				actgraph_pr_sw_obj->stopTIME("PROCESS, PARTITION AND APPLY PHASE (FPGA): Top kernel Time Elapsed: ", begintime_topkernel, iteration_idx);
				#endif 
				#ifdef LOCKE
				totaltime_topkernel_ms += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime_topkernel).count();
				#endif
				#endif 
				
				// Print results after Kernel run
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"Host::Process, Partition and Apply phase (FPGA):: Print results after Kernel run "<<endl;
				printstructures(threadidx, flag);
				#endif
				#if (defined(_DEBUGMODE_HOSTPRINTS) & defined(_DEBUGMODE_CHECKS3) & defined(NO))
				actgraph_pr_sw_obj->recordstats(cData_kvstatsA[flag]);
				actgraph_pr_sw_obj->recordstats(cData_kvstatsB[flag]);
				actgraph_pr_sw_obj->recordstats(cData_kvstatsC[flag]);
				actgraph_pr_sw_obj->recordstats(cData_kvstatsD[flag]);
				#endif
			
				#ifdef _DEBUGMODE_TIMERS
				actgraph_pr_sw_obj->stopTIME("PROCESS, PARTITION AND APPLY PHASE (FPGA): Iteration Time Elapsed: ", begintime_iterationidx, iteration_idx);
				#endif
				giteration_idx += 1;
				cout<<"exiting after test..."<<endl;
				// exit(EXIT_SUCCESS);
			}
		}	
		actgraph_pr_sw_obj->cummulateverticesdata(threadidx, (keyvalue_t *)cData_kvdramAV[0],(keyvalue_t *)cData_kvdramBV[0],(keyvalue_t *)cData_kvdramCV[0],(keyvalue_t *)cData_kvdramDV[0],(2 * PADDEDKVDATA_BATCHSIZE_KVS), actgraph_pr_sw_obj->GETKVDATA_RANGE_FORSSDPARTITION_((subthreadidxoffset + subthreadidx)));		 
		actgraph_pr_sw_obj->writeverticesdatatofile(nvmeFd_verticesdata_w2[(subthreadidxoffset + subthreadidx)], 0, cData_kvdramAV[0], (2 * PADDEDKVDATA_BATCHSIZE), actgraph_pr_sw_obj->GETKVDATA_RANGE_FORSSDPARTITION_((subthreadidxoffset + subthreadidx))); // FIXME. results should be combined first or even better: each DRAM of each thread can host a different col
	}
	#ifdef _DEBUGMODE_TIMERS
	actgraph_pr_sw_obj->stopTIME("Total time elapsed (Entire Process, Partition & Apply process (FPGA)): ", begintime_partitionandapply, NAp);
	#endif
	#ifdef HWLOCKE
	mutex4_workthread.unlock(); // unlock
	#endif	
}
void actgraph_pr_fpga::workerthread_generatekvs(int threadidx, unsigned int subthreadidxoffset, unsigned int subthreadidx, unsigned int gbankoffset, unsigned int lbankoffset, edge_t edgepropertyfilesize, unsigned int iteration_idx, unsigned int flag){
	size_t bytes_edgesoffset = (iteration_idx * KVDATA_BATCHSIZE) * sizeof(edgeprop2_t);
	vertex_t edgesz = actgraph_pr_sw_obj->hmin(KVDATA_BATCHSIZE, actgraph_pr_sw_obj->hsub((size_t)edgepropertyfilesize, (size_t)((size_t)((size_t)iteration_idx * (size_t)KVDATA_BATCHSIZE))));
if(threadidx == 0){ edge_process_obj2[threadidx]->generateupdatesII(bytes_edgesoffset, edgesz, nvmeFd_edgeproperties_r2[gbankoffset + lbankoffset + threadidx][(subthreadidxoffset + subthreadidx)], nvmeFd_vertexoffsets_r2[gbankoffset + lbankoffset + threadidx][(subthreadidxoffset + subthreadidx)], edgesbuffer[threadidx], vertexoffsetsbuffer[threadidx], vertexpropertiesbuffer[lbankoffset], cData_kvdramAV[flag], cData_kvstatsA[flag], (subthreadidxoffset + subthreadidx)); }			
else if(threadidx == 1){ edge_process_obj2[threadidx]->generateupdatesII(bytes_edgesoffset, edgesz, nvmeFd_edgeproperties_r2[gbankoffset + lbankoffset + threadidx][(subthreadidxoffset + subthreadidx)], nvmeFd_vertexoffsets_r2[gbankoffset + lbankoffset + threadidx][(subthreadidxoffset + subthreadidx)], edgesbuffer[threadidx], vertexoffsetsbuffer[threadidx], vertexpropertiesbuffer[lbankoffset], cData_kvdramBV[flag], cData_kvstatsB[flag], (subthreadidxoffset + subthreadidx)); }			
else if(threadidx == 2){ edge_process_obj2[threadidx]->generateupdatesII(bytes_edgesoffset, edgesz, nvmeFd_edgeproperties_r2[gbankoffset + lbankoffset + threadidx][(subthreadidxoffset + subthreadidx)], nvmeFd_vertexoffsets_r2[gbankoffset + lbankoffset + threadidx][(subthreadidxoffset + subthreadidx)], edgesbuffer[threadidx], vertexoffsetsbuffer[threadidx], vertexpropertiesbuffer[lbankoffset], cData_kvdramCV[flag], cData_kvstatsC[flag], (subthreadidxoffset + subthreadidx)); }			
else if(threadidx == 3){ edge_process_obj2[threadidx]->generateupdatesII(bytes_edgesoffset, edgesz, nvmeFd_edgeproperties_r2[gbankoffset + lbankoffset + threadidx][(subthreadidxoffset + subthreadidx)], nvmeFd_vertexoffsets_r2[gbankoffset + lbankoffset + threadidx][(subthreadidxoffset + subthreadidx)], edgesbuffer[threadidx], vertexoffsetsbuffer[threadidx], vertexpropertiesbuffer[lbankoffset], cData_kvdramDV[flag], cData_kvstatsD[flag], (subthreadidxoffset + subthreadidx)); }			
	
	totalnumkvsread[threadidx] += edgesz;
}

size_t actgraph_pr_fpga::hceildiv(size_t val1, size_t val2){
	return (val1 + (val2 - 1)) / val2;
}
void actgraph_pr_fpga::printkvdrams(uint512_vec_dt * kvdram){
	// Print results after Kernel run
	for(unsigned int i = 0 ; i<(16/VECTOR_SIZE) ; i++){
		for(unsigned int j=0; j<VECTOR_SIZE; j++){
			std::cout<<"kvdram["<<i<<"].data["<<j<<"].key: "<<kvdram[i].data[j].key<<std::endl;
		}
    }
	std::cout<<std::endl;
	return;
}
void actgraph_pr_fpga::printstats(unsigned int size, metadata_t * kvstats){
	unsigned int totalsize = 0;
	for(unsigned int p = 0 ; p<size; p++){ std::cout<<"kvstats["<<p<<"]: [offset: "<<kvstats[p].offset<<", size:"<<kvstats[p].size<<"]"<<std::endl; }
	for(unsigned int p = 0 ; p<KVSTATS_SIZE; p++){ totalsize += kvstats[p].size; } 
	cout<<"printstatsIII: total number of kvs in kvstats: "<<totalsize<<endl;
	std::cout<<std::endl;
	return;
}
void actgraph_pr_fpga::printstructures(unsigned int threadidx, unsigned int flag){
	printkvdrams(cData_kvdramAV[flag]);
	printkvdrams(cData_kvdramBV[flag]);
	printkvdrams(cData_kvdramCV[flag]);
	printkvdrams(cData_kvdramDV[flag]);
	printstats(16, cData_kvstatsA[flag]);
	printstats(16, cData_kvstatsB[flag]);
	printstats(16, cData_kvstatsC[flag]);
	printstats(16, cData_kvstatsD[flag]);
	return;
}
#endif 



