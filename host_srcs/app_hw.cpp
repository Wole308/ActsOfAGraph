#include "app_hw.h"
using namespace std;

#define NUM_KERNEL 1

app_hw::app_hw(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	myuniversalparams = _universalparams;
}
app_hw::~app_hw(){} 

// HBM Pseudo-channel(PC) requirements
#define MAX_HBM_PC_COUNT 32
#define PC_NAME(n) n | XCL_MEM_TOPOLOGY
#ifdef FPGA_IMPL
const int pc[MAX_HBM_PC_COUNT] = {
    PC_NAME(0),  PC_NAME(1),  PC_NAME(2),  PC_NAME(3),  PC_NAME(4),  PC_NAME(5),  PC_NAME(6),  PC_NAME(7),
    PC_NAME(8),  PC_NAME(9),  PC_NAME(10), PC_NAME(11), PC_NAME(12), PC_NAME(13), PC_NAME(14), PC_NAME(15),
    PC_NAME(16), PC_NAME(17), PC_NAME(18), PC_NAME(19), PC_NAME(20), PC_NAME(21), PC_NAME(22), PC_NAME(23),
    PC_NAME(24), PC_NAME(25), PC_NAME(26), PC_NAME(27), PC_NAME(28), PC_NAME(29), PC_NAME(30), PC_NAME(31)};

long double app_hw::runapp(std::string binaryFile__[2], HBM_channelAXISW_t * HBM_axichannel[2][NUM_PEs], HBM_channelAXISW_t * HBM_axicenter[2], universalparams_t universalparams){
	unsigned int num_axi_interfaces = (2 * VALID_NUMPEs) + 2;
	unsigned int dataSize = 64 * 1024 * 1024;
	
	cout<<"--------------------------- VALID_NUMPEs: "<<VALID_NUMPEs<<" ----------------------------------"<<endl;

    std::string binaryFile = binaryFile__[0]; 
    cl_int err;
    cl::CommandQueue q;
    std::string krnl_name = "top_function";
    std::vector<cl::Kernel> krnls(NUM_KERNEL);
    cl::Context context;
	
	std::vector<int, aligned_allocator<int> > source_master(HBM_CHANNEL_INTSIZE);
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			source_master[t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[0][0][t].data[v]; 
		}
	}

	// std::vector<int, aligned_allocator<int> > source_in1(num_axi_interfaces, std::vector<int, aligned_allocator<int> >(dataSize));
	std::vector<int, aligned_allocator<int> > source_in1[num_axi_interfaces]; for(unsigned int i=0; i<num_axi_interfaces; i++){ source_in1[i] = std::vector<int, aligned_allocator<int> >(HBM_CHANNEL_INTSIZE); }
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ 
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			if(t<64){ 
				cout<<t<<": ";
				for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
					cout<<HBM_axichannel[0][i][t].data[v]<<", "; 
				} 
				cout<<endl;
			}
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				source_in1[2*i][t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[0][i][t].data[v]; 
				// source_in1[2*i][t*HBM_AXI_PACK_SIZE + v] = t*HBM_AXI_PACK_SIZE + v; // FIXME_HARDWARE
				if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 23: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
			}
		}
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				source_in1[2*i+1][t*HBM_AXI_PACK_SIZE + v] = HBM_axichannel[1][i][t].data[v];
				if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 24: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
			}
		}
	}
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			source_in1[2*VALID_NUMPEs][t*HBM_AXI_PACK_SIZE + v] = HBM_axicenter[0][t].data[v];
			if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 25: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
		}
	} 
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			source_in1[2*VALID_NUMPEs+1][t*HBM_AXI_PACK_SIZE + v] = HBM_axicenter[1][t].data[v];
			if(t*HBM_AXI_PACK_SIZE + v >= HBM_CHANNEL_INTSIZE){ cout<<"ERROR 26: t*HBM_AXI_PACK_SIZE + v(="<<t*HBM_AXI_PACK_SIZE + v<<") >= HBM_CHANNEL_INTSIZE. EXITING..."<<endl; exit(EXIT_FAILURE); }
		}
	}
    // for (int i = 0; i < num_axi_interfaces; i++) { std::generate(source_in1[i].begin(), source_in1[i].end(), std::rand); }

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

    std::vector<cl_mem_ext_ptr_t> inBufExt1(num_axi_interfaces);
    std::vector<cl::Buffer> buffer_input1(num_axi_interfaces);

    // For Allocating Buffer to specific Global Memory PC, user has to use cl_mem_ext_ptr_t and provide the PCs
    for (int i = 0; i < num_axi_interfaces; i++) {
		cout<<"attaching inBufExt1["<<i<<"] @ source_in1["<<i<<"] to pc["<<i<<"]... "<<endl;
		inBufExt1[i].obj = source_in1[i].data();
		// inBufExt1[i].obj = source_master.data();// FIXME_HARDWARE
		inBufExt1[i].param = 0;
		inBufExt1[i].flags = pc[i];
    }

    // These commands will allocate memory on the FPGA. The cl::Buffer objects can
    // be used to reference the memory locations on the device.
    // Creating Buffers
    for (int i = 0; i < num_axi_interfaces; i++) {
		cout<<"+++ app_hw: creating buffer_input1["<<i<<"]... "<<endl;
		OCL_CHECK(err,
			buffer_input1[i] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
												HBM_CHANNEL_BYTESIZE, &inBufExt1[i], &err));
	}

    // Copy input data to Device Global Memory
    // for (int i = 0; i < num_axi_interfaces; i++) {
		// OCL_CHECK(err,
			// err = q.enqueueMigrateMemObjects({buffer_input1[i]}, 0 /* 0 means from host*/));
	// }
	cout<<"app_hw:: Migrating input data from host memory to device global memory..."<<endl;
	#if VALID_NUMPEs==1
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({												
													buffer_input1[0], buffer_input1[1],
													buffer_input1[2], buffer_input1[3]}, 
													0));
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
													0));
	#endif 
    q.finish();

    double kernel_time_in_sec = 0, result = 0;

    std::chrono::duration<double> kernel_time(0);

    auto kernel_start = std::chrono::high_resolution_clock::now();
    // Setting the k_vadd Arguments
	for (int i = 0; i < num_axi_interfaces; i++) {
		cout<<"setting argument krnls[0].setArg("<<i<<", buffer_input1["<<i<<"])"<<endl;
        OCL_CHECK(err, err = krnls[0].setArg(i, buffer_input1[i]));
    }
	
	// Invoking the kernel
	cout<<"Invoking the kernel..."<<endl;
    OCL_CHECK(err, err = q.enqueueTask(krnls[0]));
    q.finish();
    auto kernel_end = std::chrono::high_resolution_clock::now();
	// exit(EXIT_SUCCESS); ///////////////////

    kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);

    kernel_time_in_sec = kernel_time.count();
    kernel_time_in_sec /= NUM_KERNEL;
	std::cout << "THROUGHPUT = " << universalparams.NUM_EDGES / kernel_time_in_sec << " GB/s" << std::endl;

    // Copy Result from Device Global Memory to Host Local Memory
	cout<<"app_hw:: Migrating input data from device global memory to host memory..."<<endl;
    // for (int i = 0; i < NUM_KERNEL; i++) {
        // OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_input1[i]},
                                                        // CL_MIGRATE_MEM_OBJECT_HOST));
    // }
	#if VALID_NUMPEs==1
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({												
													buffer_input1[0], buffer_input1[1],
													buffer_input1[2], buffer_input1[3]}, 
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
    return EXIT_SUCCESS;
}
#endif








