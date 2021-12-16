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

#define TESTRUN
#ifdef TESTRUN
#define NUMCOMPUTEUNITS_SLR1AND2 1 
#define NUMCOMPUTEUNITS_SLR0 1 
#else 
	#if NUM_PEs==12
	#define NUMCOMPUTEUNITS_SLR1AND2 5 
	#define NUMCOMPUTEUNITS_SLR0 5
	#endif 
	#if NUM_PEs==22
	#define NUMCOMPUTEUNITS_SLR1AND2 8 
	#define NUMCOMPUTEUNITS_SLR0 6
	#endif 
	#if NUM_PEs==24
	#define NUMCOMPUTEUNITS_SLR1AND2 9 
	#define NUMCOMPUTEUNITS_SLR0 6
	#endif 
	#if NUM_PEs==25
	#define NUMCOMPUTEUNITS_SLR1AND2 9 
	#define NUMCOMPUTEUNITS_SLR0 7
	#endif 
#endif 

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
	
	unsigned int index_count = 0;
	unsigned int edgessz_kvs = 0;
	unsigned int kvdramsz_kvs = 0;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	unsigned int A = edges[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key;
	unsigned int B = edges[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key / VECTOR_SIZE;
	edgessz_kvs = A + B;
	#endif 
	unsigned int C = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key;
	unsigned int D = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key / VECTOR_SIZE;
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	
	vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; } // reset
	
	unsigned int hybridmodeoffset_kvs = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_OTHERINFOS].data[0].key;
	for(unsigned int it=0; it<64; it++){ vdram[hybridmodeoffset_kvs + it].data[0].key = OFF; } // hybrid-mode enables
	// vdram[hybridmodeoffset_kvs + 0].data[0].key = ON; // CRITICAL FIXME.
	// vdram[hybridmodeoffset_kvs + 1].data[0].key = ON;
	// vdram[hybridmodeoffset_kvs + 2].data[0].key = ON;
	// vdram[hybridmodeoffset_kvs + 3].data[0].key = ON;
	// vdram[hybridmodeoffset_kvs + 4].data[0].key = OFF;
	// vdram[hybridmodeoffset_kvs + 5].data[0].key = OFF;
	// vdram[hybridmodeoffset_kvs + 6].data[0].key = OFF;
	// vdram[hybridmodeoffset_kvs + 7].data[0].key = ON;
	
	unsigned int baseoffsetkvs_verticesdata = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA].data[0].key;
	kvdramsz_kvs = C + D;
	edgessz_kvs = PADDEDKVSOURCEDRAMSZ_KVS; // KVSOURCEDRAMSZ_KVS;  // CRITICAL REMOVEME.
	kvdramsz_kvs = PADDEDKVSOURCEDRAMSZ_KVS; // KVSOURCEDRAMSZ_KVS;
	cout<<">>> goclkernel::runapp:: edgessz: "<<edgessz_kvs*VECTOR_SIZE<<" (edgessz_kvs: "<<edgessz_kvs*VECTOR_SIZE*sizeof(keyvalue_t)<<"  bytes), kvdramsz: "<<kvdramsz_kvs*VECTOR_SIZE<<" (kvdramsz: "<<kvdramsz_kvs*VECTOR_SIZE*sizeof(keyvalue_t)<<" bytes), NUMSUBCPUTHREADS: "<<NUMSUBCPUTHREADS<<endl;
	
	uint512_vec_dt * vdramtemp[3];
	#ifndef SW_IMPL 
	#ifdef FPGA_IMPL			
	vdramtemp[0] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	vdramtemp[1] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	vdramtemp[2] = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	#else
	vdramtemp[0] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	vdramtemp[1] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	vdramtemp[2] = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	#endif
	#endif 
	for(unsigned int i=0; i<PADDEDKVSOURCEDRAMSZ_KVS; i++){ vdramtemp[0][i] = vdram[i]; vdramtemp[1][i] = vdram[i]; vdramtemp[2][i] = vdram[i]; }

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

	auto fileBuf_procandsync = xcl::read_binary_file(binaryFile[0]);

    int vector_length = LENGTH;
    bool match = true;
	
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	cout<<">>> goclkernel[actsproc]:: running ACTS PROCESS (Switching kernel args)..."<<endl;
	#endif 
	
	// cl_int err;
	std::vector<cl_mem_ext_ptr_t> inoutBufExt_edges[3]; for(unsigned int i=0; i<3; i++){ inoutBufExt_edges[i] = std::vector<cl_mem_ext_ptr_t>(32); }
	std::vector<cl_mem_ext_ptr_t> inoutBufExt_kvdram[3]; for(unsigned int i=0; i<3; i++){ inoutBufExt_kvdram[i] = std::vector<cl_mem_ext_ptr_t>(32); }
	std::vector<cl_mem_ext_ptr_t> inoutBufExt_kvdramTTT(32);
	cl_mem_ext_ptr_t inoutBufExt_vdram[4];
	std::vector<cl::Buffer> buffer_kvdram[3]; for(unsigned int i=0; i<3; i++){ buffer_kvdram[i] = std::vector<cl::Buffer>(32); }
	std::vector<cl::Buffer> buffer_edgesdram[3]; for(unsigned int i=0; i<3; i++){ buffer_edgesdram[i] = std::vector<cl::Buffer>(32); }
	cl::Buffer buffer_vdram[4];
	std::vector<cl::Kernel> krnls_proc(3);
	cl::Kernel krnls_sync;
	std::vector<cl::Event> kernel_events(48);
	std::vector<cl::Event> write_events(48);
	std::vector<cl::Event> read_events(48);
	
	long double WALLprocandsynkerneltimelapse_ms = 0;

	unsigned int kvindex = 0;
	unsigned int edgesindex = 0;
	unsigned int hbmindex = 0;
	unsigned int numI = 0;
	for(unsigned int s=0; s<3; s+=1){
		if(s==0){ numI = NUMCOMPUTEUNITS_SLR1AND2; } // SLR 2
		if(s==1){ numI = NUMCOMPUTEUNITS_SLR1AND2; } // SLR 1
		if(s==2){ numI = NUMCOMPUTEUNITS_SLR0; } // SLR 0
		for(unsigned int i=0; i<numI; i+=1){ 
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM // CRITICAL FIXME.
			for(unsigned int j=0; j<NUM_EDGE_BANKS; j++){ 
				inoutBufExt_edges[s][edgesindex].obj = edges[edgesindex];
				inoutBufExt_edges[s][edgesindex].param = 0;
				inoutBufExt_edges[s][edgesindex].flags = bank[hbmindex];
				cout<<"attaching inoutBufExt_edges["<<s<<"]["<<edgesindex<<"] to Edges["<<edgesindex<<"] to HBM["<<hbmindex<<"]: "<<endl;
				edgesindex += 1;
				hbmindex += 1;
			}
			#endif 
			inoutBufExt_kvdram[s][i].obj = kvsourcedram[kvindex];
			inoutBufExt_kvdram[s][i].param = 0;
			inoutBufExt_kvdram[s][i].flags = bank[hbmindex];
			cout<<"attaching inoutBufExt_kvdram["<<s<<"]["<<kvindex<<"] to HBM["<<hbmindex<<"]: "<<endl;
			kvindex += 1;
			hbmindex += 1;
		}
		inoutBufExt_vdram[s].obj = vdramtemp[s];
		inoutBufExt_vdram[s].param = 0;
		inoutBufExt_vdram[s].flags = bank[hbmindex];
		cout<<"attaching inoutBufExt_vdram["<<s<<"] to vdramtemp to HBM["<<hbmindex<<"]: "<<endl;
		hbmindex += 1;
	}
	inoutBufExt_vdram[3].obj = vdram;
	inoutBufExt_vdram[3].param = 0;
	inoutBufExt_vdram[3].flags = bank[hbmindex];
	cout<<"attaching inoutBufExt_vdram[3] to vdram to HBM["<<hbmindex<<"]: "<<endl;
	
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	cout<<"goclkernel:: creating OCL buffers..."<<endl;
	#endif
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM // CRITICAL FIXME.
	for(unsigned int s=0; s<3; s+=1){ 
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
			OCL_CHECK(err,
				  buffer_edgesdram[s][i] =
					  cl::Buffer(context,
								 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
										 CL_MEM_USE_HOST_PTR,
								 sizeof(uint512_vec_dt) * edgessz_kvs,
								 &inoutBufExt_edges[s][i], 
								 &err));
			cout<<"+++ goclkernel: created buffer_edgesdram["<<s<<"]["<<i<<"] "<<endl;
		}
	}
	#endif 
	unsigned int NUM = 0;
	for(unsigned int s=0; s<3; s+=1){
		if(s==0 || s==1){ NUM=NUMCOMPUTEUNITS_SLR1AND2; } else { NUM=NUMCOMPUTEUNITS_SLR0; }
		for(unsigned int i=0; i<NUM; i++){ 
			OCL_CHECK(err,
				  buffer_kvdram[s][i] =
					  cl::Buffer(context,
								 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
										 CL_MEM_USE_HOST_PTR,
								 sizeof(uint512_vec_dt) * kvdramsz_kvs,
								 &inoutBufExt_kvdram[s][i], 
								 &err));
			cout<<"+++ goclkernel: created buffer_kvdram["<<s<<"]["<<i<<"] "<<endl;
		}
		OCL_CHECK(err,
			  buffer_vdram[s] =
				  cl::Buffer(context,
							 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
							 sizeof(uint512_vec_dt) * kvdramsz_kvs,
							 &inoutBufExt_vdram[s], 
							 &err));
		cout<<"+++ goclkernel: created buffer_vdram["<<s<<"] "<<endl;
	}
	OCL_CHECK(err,
		  buffer_vdram[3] =
			  cl::Buffer(context,
						 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
								 CL_MEM_USE_HOST_PTR,
						 sizeof(uint512_vec_dt) * kvdramsz_kvs,
						 &inoutBufExt_vdram[3], 
						 &err));
	cout<<"+++ goclkernel: created buffer_vdram[3] "<<endl;

	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	printf("INFO: loading vmul kernel\n");
	#endif
	cl::Program::Binaries vmul_bins{{fileBuf_procandsync.data(), fileBuf_procandsync.size()}};
	devices.resize(1);
	OCL_CHECK(err,
			  cl::Program program(context, devices, vmul_bins, NULL, &err));
	
	// Create kernel objects
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	cout<<"goclkernel:: creating kernel object..."<<endl;
	#endif
	std::string krnl1and2_name = "topkernelP"+std::to_string(NUMCOMPUTEUNITS_SLR1AND2);
	std::string krnl3_name = "topkernelP"+std::to_string(NUMCOMPUTEUNITS_SLR0);	
	#ifdef TESTRUN
	unsigned int _C = 2;
	#else
	unsigned int _C = 0;
	#endif 
	for (int i = 0; i < 2; i++) {
		std::string krnl_name = krnl1and2_name; 
		std::string krnl_name_ = krnl_name + "_";
		std::string cu_id = std::to_string((i+1));
		std::string krnl_name_full = krnl_name + ":{" + krnl_name_ + cu_id + "}"; 
		#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
		printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), (i+1));
		#endif
		OCL_CHECK(err, krnls_proc[i] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	}
	std::string krnl_name = krnl3_name;
	std::string krnl_name_ = krnl_name + "_"; 
	std::string cu_id = std::to_string((_C+1));
	std::string krnl_name_full = krnl_name + ":{" + krnl_name_ + cu_id + "}"; 
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), (_C+1));
	#endif
	OCL_CHECK(err, krnls_proc[2] = cl::Kernel(program, krnl_name_full.c_str(), &err));
	
	krnl_name = "topkernelsync";
	cu_id = std::to_string((1));
	std::string krnl2_name_full = krnl_name + ":{" + "topkernelsync_" + cu_id + "}"; 
	OCL_CHECK(err, krnls_sync = cl::Kernel(program, krnl2_name_full.c_str(), &err));
	
	// Setting kernel arguments...
	cout<<"goclkernel:: Setting kernel (proc) arguments..."<<endl;
	for(unsigned int s=0; s<3; s+=1){
		if(s==0 || s==1){ NUM=NUMCOMPUTEUNITS_SLR1AND2; } else { NUM=NUMCOMPUTEUNITS_SLR0; }
		for(unsigned int i=0; i<NUM; i+=1){
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM // CRITICAL FIXME.
			for(unsigned int j=0; j<NUM_EDGE_BANKS; j+=1){ 
				OCL_CHECK(err, err = krnls_proc[s].setArg(i, buffer_edgesdram[i+j]));
				cout<<"setting argument krnls_proc["<<s<<"].setArg("<<i<<", buffer_edgesdram["<<i+j<<"])"<<endl;
			}
			#endif 
			OCL_CHECK(err, err = krnls_proc[s].setArg(i, buffer_kvdram[s][i]));
			cout<<"setting argument krnls_proc["<<s<<"].setArg("<<i<<", buffer_kvdram["<<s<<"]["<<i<<"])"<<endl;
		}
		OCL_CHECK(err, err = krnls_proc[s].setArg(NUM, buffer_vdram[s]));
		cout<<"setting argument krnls_proc["<<s<<"].setArg("<<NUM<<", buffer_vdram["<<s<<"])"<<endl;
	}
	OCL_CHECK(err, err = q.finish());
	
	// Setting kernel arguments...
	cout<<"goclkernel:: Setting kernel (sync) arguments..."<<endl;
	for(unsigned int s=0; s<4; s++){ 
		OCL_CHECK(err, err = krnls_sync.setArg(s, buffer_vdram[s]));
		cout<<"setting argument krnls_sync.setArg("<<s<<", buffer_vdram["<<s<<"])"<<endl;
	}
	OCL_CHECK(err, err = q.finish());
	
	// Migrating input data to device global memory...
	cout<<"goclkernel:: Migrating input data to device global memory..."<<endl;
	#ifdef TESTRUN
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_vdram[0],
													buffer_kvdram[1][0], buffer_vdram[1],
													buffer_kvdram[2][0], buffer_vdram[2],
													buffer_vdram[3]}, 
													0, NULL, &write_events[0]));
	#else 
		#if NUM_PEs==12
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_vdram[0],
														buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_vdram[1],
														buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_vdram[2],
														buffer_vdram[3]}, 
														0, NULL, &write_events[0]));
		#elif NUM_PEs==22
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_kvdram[0][7], buffer_vdram[0],
														buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_kvdram[1][7], buffer_vdram[1],
														buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2],
														buffer_vdram[3]}, 
														0, NULL, &write_events[0]));
		#elif NUM_PEs==24
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_kvdram[0][7], buffer_kvdram[0][8], buffer_vdram[0],
														buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_kvdram[1][7], buffer_kvdram[1][8], buffer_vdram[1],
														buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2],
														buffer_vdram[3]}, 
														0, NULL, &write_events[0]));
		#elif NUM_PEs==25
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_kvdram[0][7], buffer_kvdram[0][8], buffer_vdram[0],
														buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_kvdram[1][7], buffer_kvdram[1][8], buffer_vdram[1],
														buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_kvdram[2][6], buffer_vdram[2],
														buffer_vdram[3]}, 
														0, NULL, &write_events[0]));
		#else 
		NOT IMPLEMENTED
		#endif
	#endif 
	OCL_CHECK(err, err = write_events[0].wait());
	OCL_CHECK(err, err = q.finish());
	
	std::chrono::steady_clock::time_point beginkerneltime1 = std::chrono::steady_clock::now();	
	#ifdef TESTRUN // CRITICAL REMOVEME
	OCL_CHECK(err, err = q.enqueueTask(krnls_proc[0], NULL, &kernel_events[0])); // CRITICAL REMOVEME.
	OCL_CHECK(err, err = kernel_events[0].wait());
	#else 
	for(unsigned int GraphIter=0; GraphIter<numValidIters; GraphIter++){ //
		for (unsigned int s = 0; s < 3; s++){ 
			std::cout <<">>> goclkernel: processing krnls_proc["<<s<<"] in iteration "<<GraphIter<<"..."<<endl;
			OCL_CHECK(err, err = q.enqueueTask(krnls_proc[s], NULL, &kernel_events[s])); 
			#ifdef TESTRUN
			
			#endif 
		}
		for (unsigned int s = 0; s < 3; s++){ OCL_CHECK(err, err = kernel_events[s].wait()); }
		std::cout <<">>> goclkernel: processing krnls_sync in iteration "<<GraphIter<<"..."<<endl;
		OCL_CHECK(err, err = q.enqueueTask(krnls_sync, NULL, &kernel_events[3])); 
		OCL_CHECK(err, err = kernel_events[3].wait());
	}
	#endif 
	WALLprocandsynkerneltimelapse_ms = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime1).count()) / 1000;			
	std::cout <<">>> kernel time elapsed for all iterations : "<<WALLprocandsynkerneltimelapse_ms<<" ms, "<<(WALLprocandsynkerneltimelapse_ms * 1000)<<" microsecs, "<<std::endl;
	OCL_CHECK(err, err = q.finish());
	
	cout<<"goclkernel:: migrating workload back to HOST..."<<endl;
	#ifdef TESTRUN
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_vdram[0],
													buffer_kvdram[1][0], buffer_vdram[1],
													buffer_kvdram[2][0], buffer_vdram[2],
													buffer_vdram[3]}, 
													CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
	#else 
		#if NUM_PEs==12
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_vdram[0],
														buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_vdram[1],
														buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_vdram[2],
														buffer_vdram[3]}, 
														CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
		#elif NUM_PEs==22
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_kvdram[0][7], buffer_vdram[0],
														buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_kvdram[1][7], buffer_vdram[1],
														buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2],
														buffer_vdram[3]}, 
														CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
		#elif NUM_PEs==24
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_kvdram[0][7], buffer_kvdram[0][8], buffer_vdram[0],
														buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_kvdram[1][7], buffer_kvdram[1][8], buffer_vdram[1],
														buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2],
														buffer_vdram[3]}, 
														CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
		#elif NUM_PEs==25
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_kvdram[0][7], buffer_kvdram[0][8], buffer_vdram[0],
														buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_kvdram[1][7], buffer_kvdram[1][8], buffer_vdram[1],
														buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_kvdram[2][6], buffer_vdram[2],
														buffer_vdram[3]}, 
														CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
		#else 
		NOT IMPLEMENTED
		#endif
	#endif
	
	OCL_CHECK(err, err = read_events[0].wait());
	OCL_CHECK(err, err = q.finish());
	return WALLprocandsynkerneltimelapse_ms;
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





