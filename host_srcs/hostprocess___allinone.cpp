#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "utility.h"
// #include "app.h"

#include "utility.h"
#include "act_pack.h"
#include "prepare_graph.h"
#include "algorithm.h"
// #include "app_hw.h"

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
#include "algorithm.h"
#include "utility.h"
#include "../include/common.h"

// #ifdef FPGA_IMPL
#include "xcl2.hpp"
#include <CL/cl2.hpp>
#include <iostream>
#include <fstream>
#include <CL/cl_ext_xilinx.h>
// #endif 

#include "utility.h"
#include "algorithm.h"
#include "../include/common.h"
using namespace std;
using namespace std;

// #define YYYYYYYYYYYYYYYYYYYY
#define AAAAAAAAAAAAAAAAAAAA
// #define ZZZZZZZZZZZZZZZZZZZZ

#define NUM_KERNEL 1
// HBM Pseudo-channel(PC) requirements
#define MAX_HBM_PC_COUNT 32
#define PC_NAME(n) n | XCL_MEM_TOPOLOGY
const int pc[MAX_HBM_PC_COUNT] = {
    PC_NAME(0),  PC_NAME(1),  PC_NAME(2),  PC_NAME(3),  PC_NAME(4),  PC_NAME(5),  PC_NAME(6),  PC_NAME(7),
    PC_NAME(8),  PC_NAME(9),  PC_NAME(10), PC_NAME(11), PC_NAME(12), PC_NAME(13), PC_NAME(14), PC_NAME(15),
    PC_NAME(16), PC_NAME(17), PC_NAME(18), PC_NAME(19), PC_NAME(20), PC_NAME(21), PC_NAME(22), PC_NAME(23),
    PC_NAME(24), PC_NAME(25), PC_NAME(26), PC_NAME(27), PC_NAME(28), PC_NAME(29), PC_NAME(30), PC_NAME(31)};

int main(int argc, char** argv){
	cout<<"Hostprocess:: Graph Analytics Started..."<<endl;
	
	std::string binaryFile1;
	universalparams_t universalparams;
	// utility * utilityobj = new utility();
	algorithm * algorithmobj = new algorithm();
	
	// #ifdef FPGA_IMPL
	// if (argc < 2) { // 2,3
        // std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        // return EXIT_FAILURE;
    // }
	// binaryFile1 = argv[5];
	// #endif
	
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime_overallexecution = std::chrono::steady_clock::now();
	#endif
	
	// app * appobj = new app();	///////////////
	// void run(std::string algorithm, unsigned int numiterations, unsigned int rootvid, string graph_path, int graphisundirected, std::string _binaryFile1);
	// appobj->run(argv[1], argv[2], stoi(argv[3]), stoi(argv[4]), argv[5], stoi(argv[6]), argv[7]); /////////////
	// appobj->run("nap", "sssp", 16, 1, "/home/oj2zf/Documents/dataset/soc-LiveJournal1.mtx", 1, "NAp");
	// appobj->run("nap", "sssp", 2, 1, "/home/oj2zf/Documents/dataset/rmat_32m_256m.mtx", 1, "NAp");
	// appobj->run("nap", "sssp", 12, 1, "/home/oj2zf/Documents/dataset/com-Orkut.mtx", 1, "NAp");
	// appobj->run("nap", "pr", 8, 1, "/home/oj2zf/dataset/uk-2002.mtx", 1, "NAp");
	
	std::string setup = argv[1];
	std::string algo = argv[2]; 
	unsigned int numiterations = stoi(argv[3]); 
	unsigned int rootvid = stoi(argv[4]); 
	string graph_path = argv[5]; 
	int graphisundirected = stoi(argv[6]); 
	std::string _binaryFile1 = argv[7];
	
	cout<<"app::run:: app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;

	// std::string binaryFile[2]; binaryFile[0] = _binaryFile1;
	
	std::cout << std::setprecision(2) << std::fixed;
	
	vector<edge3_type> edgedatabuffer;
	vector<edge_t> vertexptrbuffer;

	string GRAPH_NAME = ""; 
	string GRAPH_PATH = graph_path;
	bool graphisundirected_bool = true; if(graphisundirected == 0){ graphisundirected_bool = false; }
	
	// run acts
	#ifdef FPGA_IMPL 

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef AAAAAAAAAAAAAAAAAAAA
	unsigned int num_axi_interfaces = (2 * VALID_NUMPEs) + 2;
	// unsigned int num_axi_vertex_interfaces = 2 * VALID_NUMPEs;
	unsigned int total_num_axi_interfaces = num_axi_interfaces; // (2 * NUM_PEs) + 2;
	// unsigned int total_num_axi_vertex_interfaces = 2 * NUM_PEs;
	unsigned int dataSize = 64 * 1024 * 1024;
	
	cout<<"--------------------------- VALID_NUMPEs: "<<VALID_NUMPEs<<", HBM_CHANNEL_INTSIZE: "<<HBM_CHANNEL_INTSIZE<<", EDGE_PACK_SIZE: "<<EDGE_PACK_SIZE<<", HBM_AXI_PACK_SIZE: "<<HBM_AXI_PACK_SIZE<<", MAX_UPARTITION_VECSIZE: "<<MAX_UPARTITION_VECSIZE<<" ----------------------------------"<<endl;

    std::string binaryFile = _binaryFile1; // binaryFile__[0]; 
    cl_int err;
    cl::CommandQueue q;
    std::string krnl_name = "top_function";
    std::vector<cl::Kernel> krnls(NUM_KERNEL);
    cl::Context context;
	std::vector<cl::Event> kernel_events(48);
	std::vector<cl::Event> write_events(48);
	std::vector<cl::Event> read_events(48);
	
	std::vector<int, aligned_allocator<int> > input(HBM_CHANNEL_INTSIZE);
	// for(unsigned int t=0; t<HBM_CHANNEL_INTSIZE; t++){ input[t] = 1; }
	for(unsigned int t=0; t<HBM_CHANNEL_INTSIZE; t++){ input[t] = t; }
	
	std::vector<int, aligned_allocator<int> > source_in1[20]; for(unsigned int i=0; i<total_num_axi_interfaces; i++){ source_in1[i] = std::vector<int, aligned_allocator<int> >(HBM_CHANNEL_INTSIZE); }				
	unsigned int bank_index = 0;
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ 
		cout<<"filling host buffer "<<bank_index<<"..."<<endl;
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			// if(t<64 && false){ cout<<t<<": "; for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ cout<<HBM_axichannel[0][i][t].data[v]<<", "; } cout<<endl; }
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				source_in1[bank_index][t*HBM_AXI_PACK_SIZE + v] = input[t*HBM_AXI_PACK_SIZE + v]; // t*HBM_AXI_PACK_SIZE + v;//HBM_axichannel[0][i][t].data[v]; 
				if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 23: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
			}
		}
		bank_index += 1;
		cout<<"filling host buffer "<<bank_index<<"..."<<endl;
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				source_in1[bank_index][t*HBM_AXI_PACK_SIZE + v] = input[t*HBM_AXI_PACK_SIZE + v]; // t*HBM_AXI_PACK_SIZE + v + 2;//HBM_axichannel[1][i][t].data[v];
				if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 24: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
			}
		}
		bank_index += 1;
	}
	cout<<"filling host buffer "<<bank_index<<"..."<<endl;
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			source_in1[bank_index][t*HBM_AXI_PACK_SIZE + v] = input[t*HBM_AXI_PACK_SIZE + v]; // t*HBM_AXI_PACK_SIZE + v + 3;//HBM_axicenter[0][t].data[v];
			if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 25: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
		}
	} 
	bank_index += 1;
	cout<<"filling host buffer "<<bank_index<<"..."<<endl;
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			source_in1[bank_index][t*HBM_AXI_PACK_SIZE + v] = input[t*HBM_AXI_PACK_SIZE + v]; // t*HBM_AXI_PACK_SIZE + v + 4;//HBM_axicenter[1][t].data[v];
			if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 26: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
		}
	}
	bank_index += 1;
    // for (int i = 0; i < total_num_axi_interfaces; i++) { std::generate(source_in1[i].begin(), source_in1[i].end(), std::rand); }
	
	for(unsigned int i=0; i<2*VALID_NUMPEs + 2; i++){
	for (int t = 0; t < 64; t++) {
		// cout<<"~~~~~~~~~~~~~~~~~ source_in1["<<i<<"]["<<(0+t)*2<<"]: "<<source_in1[i][(0+t)*2]<<endl;
	}
	}
	
	source_in1[0][0*2] = 1; //0
	source_in1[0][1*2] = 256; // 1
	source_in1[0][2*2] = 1; // 2
	source_in1[0][3*2] = 1;
	source_in1[0][4*2] = 1;
	
	// source_in1[0][26*2] = 52;
	// cout<<"++++++++++++++++++++++++++++ source_in1[0]["<<26*2<<"]: "<<source_in1[0][26*2]<<endl;
	
	/* for(unsigned int i=0; i<1; i++){ // VALID_NUMPEs + 2
		for(unsigned int t=0; t<1; t++){ 
			for(unsigned int v=0; v<1; v++){ 
				source_in1[i][0*2] = 1;
				source_in1[i][1*2] = 1;
				source_in1[i][2*2] = 1;
				source_in1[i][3*2] = 1;
				source_in1[i][4*2] = 1;
				source_in1[i][5*2] = 1;
				source_in1[i][6*2] = 1;
				source_in1[i][7*2] = 1;
				source_in1[i][8*2] = 1;
				
				source_in1[i][10*2] = 1;
				source_in1[i][11*2] = 1;
				source_in1[i][12*2] = 1;
				source_in1[i][13*2] = 1;
				source_in1[i][14*2] = 1;
				source_in1[i][15*2] = 1;
				source_in1[i][16*2] = 1;
				source_in1[i][17*2] = 1;
				source_in1[i][18*2] = 1;
				
				source_in1[i][24*2] = 3000000;
				source_in1[i][25*2] = 3000000;
				source_in1[i][26*2] = 9; // GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS
				source_in1[i][27*2] = 4;
				// source_in1[i][28*2] = 12; // GLOBALPARAMSCODE__PARAM__NUM_ITERATIONS
				// source_in1[i][29*2] = 128;
				// source_in1[i][30*2] = 12345;
				// source_in1[i][31*2] = PAGERANK;

				source_in1[i][36*2] = 1;
				source_in1[i][37*2] = 1;
				source_in1[i][38*2] = 1;
				source_in1[i][39*2] = 1; 
				source_in1[i][40*2] = 1; 
				source_in1[i][41*2] = 1; 
				source_in1[i][42*2] = 1;
				source_in1[i][43*2] = 1; 
				source_in1[i][44*2] = 1;
				source_in1[i][45*2] = 1; 
				source_in1[i][46*2] = 1; 
				source_in1[i][47*2] = 1; 
				source_in1[i][48*2] = 1; 
			}
		}
	} */
	
	
	
    // OPENCL HOST CODE AREA START
    // The get_xil_devices will return vector of Xilinx Devices
    auto devices = xcl::get_xil_devices();

    // read_binary_file() command will find the OpenCL binary file created using the V++ compiler load into OpenCL Binary and return pointer to file buffer.
    auto fileBuf = xcl::read_binary_file(binaryFile);

    cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
    bool valid_device = false;
    for (unsigned int i = 0; i < devices.size(); i++) {
        auto device = devices[i];
        // Creating Context and Command Queue for selected Device
        OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
        OCL_CHECK(err, q = cl::CommandQueue(context, device,
                                            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &err));

        std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
        cl::Program program(context, {device}, bins, nullptr, &err);
        if (err != CL_SUCCESS) {
            std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
        } else {
            std::cout << "Device[" << i << "]: program successful!\n";
            // Creating Kernel object using Compute unit names
            for (int i = 0; i < NUM_KERNEL; i++) {
                std::string cu_id = std::to_string(i + 1);
                std::string krnl_name_full = krnl_name + ":{" + "top_function_" + cu_id + "}";
                printf("Creating a kernel [%s] for CU(%d)\n", krnl_name_full.c_str(), i + 1);
                OCL_CHECK(err, krnls[0] = cl::Kernel(program, krnl_name_full.c_str(), &err));
            }
            valid_device = true;
            break; // we break because we found a valid device
        }
    }
    if (!valid_device) {
        std::cout << "Failed to program any device found, exit!\n";
        exit(EXIT_FAILURE);
    }

    std::vector<cl_mem_ext_ptr_t> inBufExt1(total_num_axi_interfaces);
    std::vector<cl::Buffer> buffer_input1(total_num_axi_interfaces);

    // For Allocating Buffer to specific Global Memory PC, user has to use cl_mem_ext_ptr_t and provide the PCs
    for (int i = 0; i < total_num_axi_interfaces; i++) {
		cout<<"attaching inBufExt1["<<i<<"] @ source_in1["<<i<<"] to pc["<<i<<"]... "<<endl;
		inBufExt1[i].obj = source_in1[i].data();
		// inBufExt1[i].obj = source_master.data();// FIXME_HARDWARE
		inBufExt1[i].param = 0;
		inBufExt1[i].flags = pc[i];
    }
	// exit(EXIT_SUCCESS);

    // These commands will allocate memory on the FPGA. The cl::Buffer objects can
    // be used to reference the memory locations on the device.
    // Creating Buffers
	unsigned long max_sz = HBM_CHANNEL_BYTESIZE;
    for (int i = 0; i < total_num_axi_interfaces; i++) {
		cout<<"+++ app_hw: creating buffer_input1["<<i<<"]... "<<endl;
		unsigned long bbb = HBM_CHANNEL_INTSIZE;
		cout<<"--------++++++++++++++++-- bbb * sizeof(int): "<<bbb * sizeof(int)<<endl;
		OCL_CHECK(err,
			buffer_input1[i] = cl::Buffer(context, CL_MEM_READ_WRITE  | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
												bbb * sizeof(int), &inBufExt1[i], &err));
	}

    // Copy input data to Device Global Memory
    // for (int i = 0; i < total_num_axi_interfaces; i++) {
		// OCL_CHECK(err,
			// err = q.enqueueMigrateMemObjects({buffer_input1[i]}, 0 /* 0 means from host*/));
	// }
	cout<<"app_hw:: Migrating input data from host memory to device global memory..."<<endl;
	#if VALID_NUMPEs==1
	cout<<"app_hw:: Migrating input data from host memory to device global memory (B)..."<<endl;
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({												
													buffer_input1[0], buffer_input1[1],
													buffer_input1[2], buffer_input1[3]}, 
													0, NULL, &write_events[0]));
	#endif 
	#if VALID_NUMPEs==2
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5]}, 
													0, NULL, &write_events[0]));
	#endif 
	#if VALID_NUMPEs==4
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5], 
													buffer_input1[6], buffer_input1[7], 
													buffer_input1[8], buffer_input1[9]}, 
													0, NULL, &write_events[0]));
	#endif 
	#if VALID_NUMPEs==6
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5], 
													buffer_input1[6], buffer_input1[7], 
													buffer_input1[8], buffer_input1[9], 
													buffer_input1[10], buffer_input1[11], 
													buffer_input1[12], buffer_input1[13]}, 
													0, NULL, &write_events[0]));
	#endif 
	OCL_CHECK(err, err = write_events[0].wait());
    q.finish();

    double kernel_time_in_sec = 0, result = 0;

    std::chrono::duration<double> kernel_time(0);

    auto kernel_start = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();	
    // Setting the k_vadd Arguments
	for (int i = 0; i < num_axi_interfaces; i++) {
		cout<<"setting argument krnls[0].setArg("<<i<<", buffer_input1["<<i<<"])"<<endl;
        OCL_CHECK(err, err = krnls[0].setArg(i, buffer_input1[i]));
    }
	q.finish();
	
	// Invoking the kernel
	cout<<"Invoking the kernel..."<<endl;
    // OCL_CHECK(err, err = q.enqueueTask(krnls[0]));
	OCL_CHECK(err, err = q.enqueueTask(krnls[0], NULL, &kernel_events[0])); 
	OCL_CHECK(err, err = kernel_events[0].wait());
    q.finish();
	double end_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin_time).count()) / 1000;			
	std::cout <<">>> kernel time elapsed for all iterations : "<<end_time<<" ms, "<<(end_time * 1000)<<" microsecs, "<<std::endl;
    auto kernel_end = std::chrono::high_resolution_clock::now();
	// exit(EXIT_SUCCESS); ///////////////////

    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);

    kernel_time_in_sec = kernel_time.count();
    kernel_time_in_sec /= NUM_KERNEL;
	std::cout << "THEORETICAL THROUGHPUT = " << (universalparams.NUM_EDGES / kernel_time_in_sec) / 1000000 << " MTEPS" << std::endl; // kernel_time_in_sec
	std::cout << "ACTUAL THROUGHPUT = " << ((universalparams.NUM_EDGES / kernel_time_in_sec) / 1000000) * (16 / EDGE_PACK_SIZE) << " MTEPS" << std::endl; 
	
    // Copy Result from Device Global Memory to Host Local Memory
	cout<<"app_hw:: Migrating input data from device global memory to host memory..."<<endl;
    // for (int i = 0; i < NUM_KERNEL; i++) {
        // OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_input1[i]},
                                                        // CL_MIGRATE_MEM_OBJECT_HOST));
    // }
	#if VALID_NUMPEs==1
	cout<<"app_hw:: Migrating input data from device global memory to host memory (B)..."<<endl;
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({												
													buffer_input1[0], buffer_input1[1],
													buffer_input1[2], buffer_input1[3]}, 
													CL_MIGRATE_MEM_OBJECT_HOST));
	#endif 
	#if VALID_NUMPEs==2
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5]}, 
													CL_MIGRATE_MEM_OBJECT_HOST));
	#endif 
	#if VALID_NUMPEs==4
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5], 
													buffer_input1[6], buffer_input1[7], 
													buffer_input1[8], buffer_input1[9]}, 
													CL_MIGRATE_MEM_OBJECT_HOST));
	#endif 
	#if VALID_NUMPEs==6
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({													
													buffer_input1[0], buffer_input1[1], 
													buffer_input1[2], buffer_input1[3], 
													buffer_input1[4], buffer_input1[5], 
													buffer_input1[6], buffer_input1[7], 
													buffer_input1[8], buffer_input1[9], 
													buffer_input1[10], buffer_input1[11], 
													buffer_input1[12], buffer_input1[13]}, 
													CL_MIGRATE_MEM_OBJECT_HOST));
	#endif
    q.finish();
	#endif 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	#endif 
	
	// #ifdef _DEBUGMODE_TIMERS3
	// std::cout << endl << "TEST FINISHED" << std::endl; 
	// utilityobj->stopTIME("HOSTPROCESS:: TIMING SUMMARY: TOTAL TIME ELAPSED: ", begintime_overallexecution, NAp);
	// #endif
	return EXIT_SUCCESS;
}

