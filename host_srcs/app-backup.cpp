#include "app.h"
using namespace std;

#define MAX_HBM_PC_COUNT 32
#define PC_NAME(n) n | XCL_MEM_TOPOLOGY
const int pc[MAX_HBM_PC_COUNT] = {
    PC_NAME(0),  PC_NAME(1),  PC_NAME(2),  PC_NAME(3),  PC_NAME(4),  PC_NAME(5),  PC_NAME(6),  PC_NAME(7),
    PC_NAME(8),  PC_NAME(9),  PC_NAME(10), PC_NAME(11), PC_NAME(12), PC_NAME(13), PC_NAME(14), PC_NAME(15),
    PC_NAME(16), PC_NAME(17), PC_NAME(18), PC_NAME(19), PC_NAME(20), PC_NAME(21), PC_NAME(22), PC_NAME(23),
    PC_NAME(24), PC_NAME(25), PC_NAME(26), PC_NAME(27), PC_NAME(28), PC_NAME(29), PC_NAME(30), PC_NAME(31)};

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// active vertices area {actv vertices data}
// stats area {actv uprop blocks, active edge blocks, active update blocks}
// stats area {edges map, edge block map, vertex partition mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

// 1. Vitis profile summary 
// https://xilinx.github.io/Vitis-Tutorials/2020-1/docs/Pathway3/ProfileAndTraceReports.html

// 2. Select the Profile Summary report, a... (vitis profile summary has both Compute and Memory information to calculate Arithmetic Intensity)
// https://xilinx.github.io/Vitis-Tutorials/2020-1/docs/Pathway3/ProfileAndTraceReports.html 
// https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Profile-Summary-Report

/*
// datasets 
snap.stanford.edu/snap/download.html (downloading snap)
python -m pip install snap-stanford (installing snap)
https://groups.google.com/g/snap-datasets/c/4_sMYlFRf9Q?pli=1 (smaller twitter7 dataset)
graphchallenge.mit.edu/data-sets
dango.rocks/datasets
http://konect.cc/networks/wikipedia_link_en/
http://konect.cc/categories/Hyperlink/
http://konect.cc/networks/delicious-ti/ (pr: 500, bfs: 726 MiEdges/s)
lgylym.github.io/big-graph/dataset.html
https://github.com/snap-stanford/snap-python/blob/master/examples/benchmark.py (SNAP graph in python)
https://snap.stanford.edu/snappy/index.html
https://snap.stanford.edu/snappy/index.html (snap RMAT? datasets)
Degree-aware Hybrid Graph Traversal on FPGA-HMC Platform (Jing (Jane) Li)(https://dl.acm.org/doi/pdf/10.1145/3174243.3174245)
Large-Scale Graph Processing on FPGAs with Caches for Thousands of Simultaneous Misses (https://www.epfl.ch/labs/lap/wp-content/uploads/2021/10/AsiaticiJun21_LargeScaleGraphProcessingOnFpgasWithCachesForThousandsOfSimultaneousMisses_ISCA21.pdf)
FDGLib: A Communication Library for Efficient Large-Scale Graph Processing in FPGA-Accelerated Data Centers (https://link.springer.com/content/pdf/10.1007/s11390-021-1242-y.pdf)

graphalytics.org/datasets (*)
github.com/GRAND-Lab/graph_datasets

# lightweight graph re-ordering: https://github.com/faldupriyank/dbg 
*/

#define BUILD_GRAPH_FOR_DEVICE
#define APP_LOADEDGES
#define APP_LOADSRCVERTICES
#define APP_LOADDESTVERTICES
#define APP_LOADMASKS
#define APP_LOADROOTVID
#define APP_LOADSTATSINFO
#define APP_LOADMESSAGES // THE CAUSE OF ERROR.
#define APP_RUNSWVERSION
#define APP_RUNHWVERSION 

#define NUM_KERNEL 24 //24
	
app::app(){
	algorithmobj = new algorithm();
}
app::~app(){
	cout<<"app::~app:: finish destroying memory structures... "<<endl;
}

unsigned int get_local3(unsigned int vid){
	unsigned int W = (FOLD_SIZE * EDGE_PACK_SIZE) * NUM_PEs;
	unsigned int y = vid / W; 
	unsigned int x = vid % (FOLD_SIZE * EDGE_PACK_SIZE);
	unsigned int lvid = (y * (FOLD_SIZE * EDGE_PACK_SIZE)) + x;
	return lvid;
}

universalparams_t app::get_universalparams(std::string algo, unsigned int numiterations, unsigned int rootvid, unsigned int num_vertices, unsigned int num_edges, bool graphisundirected){
	universalparams_t universalparams;
	// algorithm * algorithmobj = new algorithm();
	
	universalparams.ALGORITHM = algorithmobj->get_algorithm_id(algo);
	universalparams.NUM_ITERATIONS = numiterations; if(universalparams.ALGORITHM != BFS && universalparams.ALGORITHM != SSSP){ universalparams.NUM_ITERATIONS = 1; }
	universalparams.ROOTVID = rootvid;
	
	universalparams.NUM_VERTICES = num_vertices; 
	universalparams.NUM_EDGES = num_edges; 

	universalparams.NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	if(universalparams.NUM_UPARTITIONS > MAX_NUM_UPARTITIONS){ universalparams.NUM_UPARTITIONS = MAX_NUM_UPARTITIONS; } // FIXME.
	universalparams.NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs
	
	universalparams.NUM_PARTITIONS = 16;
	
	return universalparams;
}

#ifdef TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
void app::run(std::string setup, std::string algo, unsigned int rootvid, string graph_path, int graphisundirected, unsigned int numiterations, std::string _binaryFile1){
	cout<<"app::run:: app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;
	// exit(EXIT_SUCCESS);
	
	std::string binaryFile[2]; binaryFile[0] = _binaryFile1;
	
	unsigned int dataSize = 64 * 1024 * 1024; // taking maximum possible data size value for an HBM bank
    unsigned int num_times = 1024;            // num_times specify, number of times a kernel
                                              // will execute the same operation. This is
                                              // needed
    // to keep the kernel busy to test the actual bandwidth of all banks running
    // concurrently.
	
	std::vector<int, aligned_allocator<int> > source_in1(dataSize);
	std::generate(source_in1.begin(), source_in1.end(), std::rand);

	// auto binaryFile = argv[1];
	// std::string binaryFile = binaryFile; // binaryFile__[0]; 
	cl_int err;
	cl::CommandQueue q;
	cl::Context context;
	cl::Kernel krnl_vadd;

	// OPENCL HOST CODE AREA START
	// get_xil_devices() is a utility API which will find the xilinx
	// platforms and will return list of devices connected to Xilinx platform
	std::cout << "Creating Context..." << std::endl;
	auto devices = xcl::get_xil_devices();

	// read_binary_file() is a utility API which will load the binaryFile
	// and will return the pointer to file buffer.
	auto fileBuf = xcl::read_binary_file(binaryFile[0]);
	cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
	bool valid_device = false;
	for (unsigned int i = 0; i < devices.size(); i++) {
		auto device = devices[i];
		// Creating Context and Command Queue for selected Device
		OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
		// This example will use an out of order command queue. The default command
		// queue created by cl::CommandQueue is an inorder command queue.
		OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err));

		std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
		cl::Program program(context, {device}, bins, nullptr, &err);
		if (err != CL_SUCCESS) {
			std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
		} else {
			std::cout << "Device[" << i << "]: program successful!\n";
			OCL_CHECK(err, krnl_vadd = cl::Kernel(program, "top_function", &err));
			valid_device = true;
			break; // we break because we found a valid device
		}
	}
	if (!valid_device) {
		std::cout << "Failed to program any device found, exit!\n";
		exit(EXIT_FAILURE);
	}

    std::vector<cl_mem_ext_ptr_t> inBufExt1(NUM_KERNEL);
    std::vector<cl_mem_ext_ptr_t> inBufExt2(NUM_KERNEL);
    std::vector<cl_mem_ext_ptr_t> outAddBufExt(NUM_KERNEL);
    std::vector<cl_mem_ext_ptr_t> outMulBufExt(NUM_KERNEL);

    std::vector<cl::Buffer> buffer_input1(NUM_KERNEL);
    std::vector<cl::Buffer> buffer_input2(NUM_KERNEL);
    std::vector<cl::Buffer> buffer_output_add(NUM_KERNEL);
    std::vector<cl::Buffer> buffer_output_mul(NUM_KERNEL);

    // For Allocating Buffer to specific Global Memory PC, user has to use
    // cl_mem_ext_ptr_t
    // and provide the PCs
    for (int i = 0; i < NUM_KERNEL; i++) {
		inBufExt1[i].obj = source_in1.data();
        inBufExt1[i].param = 0;
        inBufExt1[i].flags = pc[i];
    }

    // These commands will allocate memory on the FPGA. The cl::Buffer objects can
    // be used to reference the memory locations on the device.
    // Creating Buffers
    for (int i = 0; i < NUM_KERNEL; i++) {
		cout<<"------------------------------ creating buffer "<<i<<" -----------------------------"<<endl;
        OCL_CHECK(err,
                  buffer_input1[i] = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
                                                sizeof(uint32_t) * dataSize, &inBufExt1[i%12], &err));
    }
	cout<<"------------------------------ FINISH -----------------------------"<<endl;
	exit(EXIT_SUCCESS);
}
#endif 
#ifdef TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
void app::run(std::string setup, std::string algo, unsigned int rootvid, string graph_path, int graphisundirected, unsigned int numiterations, std::string _binaryFile1){
	cout<<"app::run:: app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;
	// exit(EXIT_SUCCESS);
	
	std::string binaryFile[2]; binaryFile[0] = _binaryFile1;
	
	// #ifdef RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int ARRAY_SIZE = 1024; // HBM_CHANNEL_SIZE * HBM_AXI_PACK_SIZE;
	int flag = 0;
	// cl_int err;
	
	// auto binaryFile = argv[1];
	// std::string binaryFile = binaryFile; // binaryFile__[0]; 
	cl_int err;
	cl::CommandQueue q;
	cl::Context context;
	cl::Kernel krnl_vadd;

	// OPENCL HOST CODE AREA START
	// get_xil_devices() is a utility API which will find the xilinx
	// platforms and will return list of devices connected to Xilinx platform
	std::cout << "Creating Context..." << std::endl;
	auto devices = xcl::get_xil_devices();

	// read_binary_file() is a utility API which will load the binaryFile
	// and will return the pointer to file buffer.
	auto fileBuf = xcl::read_binary_file(binaryFile[0]);
	cl::Program::Binaries bins{{fileBuf.data(), fileBuf.size()}};
	bool valid_device = false;
	for (unsigned int i = 0; i < devices.size(); i++) {
		auto device = devices[i];
		// Creating Context and Command Queue for selected Device
		OCL_CHECK(err, context = cl::Context(device, nullptr, nullptr, nullptr, &err));
		// This example will use an out of order command queue. The default command
		// queue created by cl::CommandQueue is an inorder command queue.
		OCL_CHECK(err, q = cl::CommandQueue(context, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err));

		std::cout << "Trying to program device[" << i << "]: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
		cl::Program program(context, {device}, bins, nullptr, &err);
		if (err != CL_SUCCESS) {
			std::cout << "Failed to program device[" << i << "] with xclbin file!\n";
		} else {
			std::cout << "Device[" << i << "]: program successful!\n";
			OCL_CHECK(err, krnl_vadd = cl::Kernel(program, "top_function", &err));
			valid_device = true;
			break; // we break because we found a valid device
		}
	}
	if (!valid_device) {
		std::cout << "Failed to program any device found, exit!\n";
		exit(EXIT_FAILURE);
	}

	// We will break down our problem into multiple iterations. Each iteration
	// will perform computation on a subset of the entire data-set.
	// size_t elements_per_iteration = ARRAY_SIZE; // 2048;
	// size_t bytes_per_iteration = ARRAY_SIZE * sizeof(int); // elements_per_iteration * sizeof(int);
	size_t bytes_per_iteration = 64 * sizeof(int); // 1000000 * sizeof(int); ////////////////////////////////////// REMOVEME.
	size_t num_iterations = 1; 
	unsigned int batch = 0;

	std::vector<int, aligned_allocator<int> > HHX[32]; for(unsigned int i=0; i<NUM_PEs*2; i++){ HHX[i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }
	std::vector<int, aligned_allocator<int> > HHC[2]; for(unsigned int i=0; i<2; i++){ HHC[i] = std::vector<int, aligned_allocator<int> >(ARRAY_SIZE); }

	vector<cl::Event> kernel_events(2);
	vector<cl::Event> read_events(2);
	cl::Buffer buffer_hbm0, buffer_hbm1, buffer_hbm2, buffer_hbm3, buffer_hbm4, buffer_hbm5, buffer_hbm6, buffer_hbm7, buffer_hbm8, buffer_hbm9, buffer_hbm10, buffer_hbm11, buffer_hbm12, buffer_hbm13,
		buffer_hbm14, buffer_hbm15, buffer_hbm16, buffer_hbm17, buffer_hbm18, buffer_hbm19, buffer_hbm20, buffer_hbm21, buffer_hbm22, buffer_hbm23, buffer_hbm24, buffer_hbm25;
	
	cl_mem_ext_ptr_t inBufExt0, inBufExt1, inBufExt2, inBufExt3, inBufExt4, inBufExt5, inBufExt6, inBufExt7, inBufExt8, inBufExt9, inBufExt10, inBufExt11, inBufExt12, inBufExt13,
		inBufExt14, inBufExt15, inBufExt16, inBufExt17, inBufExt18, inBufExt19, inBufExt20, inBufExt21, inBufExt22, inBufExt23, inBufExt24, inBufExt25;

	inBufExt0.obj = HHX[0].data();
	inBufExt0.param = 0;
	inBufExt0.flags = pc[0];
	
	inBufExt1.obj = HHX[1].data();
	inBufExt1.param = 0;
	inBufExt1.flags = pc[1];

	inBufExt2.obj = HHX[2].data();
	inBufExt2.param = 0;
	inBufExt2.flags = pc[2];
	
	inBufExt3.obj = HHX[3].data();
	inBufExt3.param = 0;
	inBufExt3.flags = pc[3];
	
	inBufExt4.obj = HHX[4].data();
	inBufExt4.param = 0;
	inBufExt4.flags = pc[4];
	
	inBufExt5.obj = HHX[5].data();
	inBufExt5.param = 0;
	inBufExt5.flags = pc[5];
	
	inBufExt6.obj = HHX[6].data();
	inBufExt6.param = 0;
	inBufExt6.flags = pc[6];
	
	inBufExt7.obj = HHX[7].data();
	inBufExt7.param = 0;
	inBufExt7.flags = pc[7];
	
	inBufExt8.obj = HHX[8].data();
	inBufExt8.param = 0;
	inBufExt8.flags = pc[8];
	
	inBufExt9.obj = HHX[9].data();
	inBufExt9.param = 0;
	inBufExt9.flags = pc[9];
	
	inBufExt10.obj = HHX[10].data();
	inBufExt10.param = 0;
	inBufExt10.flags = pc[10];
	
	inBufExt11.obj = HHX[11].data();
	inBufExt11.param = 0;
	inBufExt11.flags = pc[11];
	
	inBufExt12.obj = HHX[12].data();
	inBufExt12.param = 0;
	inBufExt12.flags = pc[12];
	
	inBufExt13.obj = HHX[13].data();
	inBufExt13.param = 0;
	inBufExt13.flags = pc[13];
	
	inBufExt14.obj = HHX[14].data();
	inBufExt14.param = 0;
	inBufExt14.flags = pc[14];
	
	inBufExt15.obj = HHX[15].data();
	inBufExt15.param = 0;
	inBufExt15.flags = pc[15];
	
	inBufExt16.obj = HHX[16].data();
	inBufExt16.param = 0;
	inBufExt16.flags = pc[16];
	
	inBufExt17.obj = HHX[17].data();
	inBufExt17.param = 0;
	inBufExt17.flags = pc[17];
	
	inBufExt18.obj = HHX[18].data();
	inBufExt18.param = 0;
	inBufExt18.flags = pc[18];
	
	inBufExt19.obj = HHX[19].data();
	inBufExt19.param = 0;
	inBufExt19.flags = pc[19];
	
	inBufExt20.obj = HHX[20].data();
	inBufExt20.param = 0;
	inBufExt20.flags = pc[20];
	
	inBufExt21.obj = HHX[21].data();
	inBufExt21.param = 0;
	inBufExt21.flags = pc[21];
	
	inBufExt22.obj = HHX[22].data();
	inBufExt22.param = 0;
	inBufExt22.flags = pc[22];
	
	inBufExt23.obj = HHX[23].data();
	inBufExt23.param = 0;
	inBufExt23.flags = pc[23];

	inBufExt24.obj = HHC[0].data();
	inBufExt24.param = 0;
	inBufExt24.flags = pc[24];
	
	inBufExt25.obj = HHC[1].data();
	inBufExt25.param = 0;
	inBufExt25.flags = pc[25];
	
	OCL_CHECK(err, buffer_hbm0 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt0, &err));
	OCL_CHECK(err, buffer_hbm1 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt1, &err));	
	OCL_CHECK(err, buffer_hbm2 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt2, &err));	
	OCL_CHECK(err, buffer_hbm3 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt3, &err));	
	OCL_CHECK(err, buffer_hbm4 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt4, &err));	
	OCL_CHECK(err, buffer_hbm5 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt5, &err));	
	OCL_CHECK(err, buffer_hbm6 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt6, &err));	
	OCL_CHECK(err, buffer_hbm7 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt7, &err));	
	OCL_CHECK(err, buffer_hbm8 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt8, &err));	
	OCL_CHECK(err, buffer_hbm9 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt9, &err));	
	OCL_CHECK(err, buffer_hbm10 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt10, &err));	
	OCL_CHECK(err, buffer_hbm11 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt11, &err));	
	OCL_CHECK(err, buffer_hbm12 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt12, &err));	
	OCL_CHECK(err, buffer_hbm13 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt13, &err));
	buffer_hbm14 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt14, &err);
	// CL_INVALID_CONTEXT if context is not a valid context.
	// CL_INVALID_VALUE if values specified in flags are not valid as defined in the table above.
	// CL_INVALID_BUFFER_SIZE if size is 0.
	// Implementations may return CL_INVALID_BUFFER_SIZE if size is greater than the CL_DEVICE_MAX_MEM_ALLOC_SIZE value specified in the table of allowed values for param_name for clGetDeviceInfo for all devices in context.
	// CL_INVALID_HOST_PTR if host_ptr is NULL and CL_MEM_USE_HOST_PTR or CL_MEM_COPY_HOST_PTR are set in flags or if host_ptr is not NULL but CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are not set in flags.
	// CL_MEM_OBJECT_ALLOCATION_FAILURE if there is a failure to allocate memory for buffer object.
	// CL_OUT_OF_RESOURCES if there is a failure to allocate resources required by the OpenCL implementation on the device.
	// CL_OUT_OF_HOST_MEMORY if there is a failure to allocate resources required by the OpenCL implementation on the host.
	cout<<"err=="<<err<<" ------------"<<endl;
	if(err==CL_INVALID_CONTEXT){
		cout<<"err==CL_INVALID_CONTEXT ------------"<<endl;
	}
	if(err==CL_INVALID_VALUE){
		cout<<"err==CL_INVALID_VALUE ------------"<<endl;
	}
	if(err==CL_INVALID_BUFFER_SIZE){
		cout<<"err==CL_INVALID_BUFFER_SIZE ------------"<<endl;
	}
	if(err==CL_INVALID_HOST_PTR){
		cout<<"err==CL_INVALID_HOST_PTR ------------"<<endl;
	}
	if(err==CL_MEM_OBJECT_ALLOCATION_FAILURE){
		cout<<"err==CL_MEM_OBJECT_ALLOCATION_FAILURE ------------"<<endl;
	}
	if(err==CL_OUT_OF_RESOURCES){
		cout<<"err==CL_OUT_OF_RESOURCES ------------"<<endl;
	}
	if(err==CL_OUT_OF_HOST_MEMORY){
		cout<<"err==CL_OUT_OF_HOST_MEMORY ------------"<<endl;
	}
	if(err==CL_INVALID_CONTEXT){
		cout<<"err==CL_INVALID_CONTEXT ------------"<<endl;
	}
	OCL_CHECK(err, buffer_hbm14 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt14, &err));
	OCL_CHECK(err, buffer_hbm15 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt15, &err));
	OCL_CHECK(err, buffer_hbm16 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt16, &err));
	OCL_CHECK(err, buffer_hbm17 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt17, &err));
	OCL_CHECK(err, buffer_hbm18 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt18, &err));
	OCL_CHECK(err, buffer_hbm19 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt19, &err));
	OCL_CHECK(err, buffer_hbm20 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt20, &err));
	OCL_CHECK(err, buffer_hbm21 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt21, &err));
	OCL_CHECK(err, buffer_hbm22 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt22, &err));
	OCL_CHECK(err, buffer_hbm23 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt23, &err));
	OCL_CHECK(err, buffer_hbm24 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt24, &err));
	OCL_CHECK(err, buffer_hbm25 = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
										bytes_per_iteration, &inBufExt25, &err));
	exit(EXIT_SUCCESS); /////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// #endif 
}
#endif 
// #ifdef TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
void app::run(std::string setup, std::string algo, unsigned int rootvid, string graph_path, int graphisundirected, unsigned int numiterations, std::string _binaryFile1){
	cout<<"app::run:: app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;
	// exit(EXIT_SUCCESS);
	
	std::string binaryFile[2]; binaryFile[0] = _binaryFile1;
	std::cout << std::setprecision(2) << std::fixed;
	
	vector<edge3_type> edgedatabuffer;
	vector<edge_t> vertexptrbuffer;
	
	vector<edge3_type> edgedatabuffers[NUM_PROCS];
	vector<edge_t> vertexptrbuffers[NUM_PROCS];
	
	HBM_channel_t * HBM_center = new HBM_channel_t[HBM_CHANNEL_SIZE];
	HBM_channel_t * HBM_channel[NUM_PEs];
	HBM_channelAXISW_t * HBM_axichannel[2][NUM_PEs]; 
	HBM_channelAXISW_t * HBM_axicenter[2]; 
	unsigned int globalparams[1024];
	vector<edge3_type> csr_pack_edges[NUM_PEs]; 
	vector<edge3_vec_dt> act_pack_edges[NUM_PEs];
	map_t * act_pack_map[NUM_PEs][MAX_NUM_UPARTITIONS];
	map_t * act_pack_map2[NUM_PEs][MAX_NUM_UPARTITIONS];
	vector<edge3_vec_dt> act_pack_edgeupdates[NUM_PEs];
	map_t * act_pack_edgeupdates_map[NUM_PEs][MAX_NUM_UPARTITIONS];
	
	// allocate HBM memory
	cout<<"app: initializing HBM_centers..."<<endl;
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){ HBM_center[t].data[v] = INVALIDDATA; }}
	
	// HBM_channel_t * HBM_channel[NUM_PEs];
	cout<<"app: initializing HBM_channels..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ HBM_channel[i] = new HBM_channel_t[HBM_CHANNEL_SIZE]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){ HBM_channel[i][t].data[v] = 0; }}}
	
	// allocate AXI HBM memory
	cout<<"app: initializing HBM_axichannels..."<<endl;
	for(unsigned int n=0; n<2; n++){
		for(unsigned int i=0; i<NUM_PEs; i++){ HBM_axichannel[n][i] = new HBM_channelAXISW_t[HBM_CHANNEL_SIZE]; }
		for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_axichannel[n][i][t].data[v] = 0; }}}
	}
	
	cout<<"app: initializing HBM_axicenters"<<endl;
	for(unsigned int n=0; n<2; n++){
		HBM_axicenter[n] = new HBM_channelAXISW_t[HBM_CHANNEL_SIZE]; 
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ HBM_axicenter[n][t].data[v] = 0; }}
	}
	
	string GRAPH_NAME = ""; 
	string GRAPH_PATH = graph_path;
	bool graphisundirected_bool = true; if(graphisundirected == 0){ graphisundirected_bool = false; }
	
	#ifdef GGGGGG
	make_graph * make_graphobj = new make_graph();
	graphisundirected_bool = false;
	make_graphobj->start(GRAPH_PATH, edgedatabuffers, vertexptrbuffers, graphisundirected_bool);
	// start(vector<edge3_type> (&edgesbuffer_outs)[NUM_PROCS], vector<edge_t> (&vptr_out)[NUM_PROCS], bool graphisundirected)
	exit(EXIT_SUCCESS); //
	#endif 
	
	prepare_graph * prepare_graphobj = new prepare_graph();
	prepare_graphobj->start(GRAPH_PATH, edgedatabuffer, vertexptrbuffer, graphisundirected_bool);
	unsigned int num_edges = edgedatabuffer.size();
	unsigned int num_vertices = vertexptrbuffer.size();
	cout<<"app:run: num_vertices: "<<num_vertices<<", num_edges: "<<num_edges<<endl;
	
	universalparams_t universalparams = get_universalparams(algo, numiterations, rootvid, num_vertices, num_edges, graphisundirected_bool);
	cout<<"app::run:: NUM_VERTICES: "<<universalparams.NUM_VERTICES<<", NUM_EDGES: "<<universalparams.NUM_EDGES<<", NUM_UPARTITIONS: "<<universalparams.NUM_UPARTITIONS<<", NUM_APPLYPARTITIONS: "<<universalparams.NUM_APPLYPARTITIONS<<", VERTEX RANGE: "<<universalparams.NUM_VERTICES / NUM_PEs<<endl;			
	utility * utilityobj = new utility(universalparams);
	// utilityobj->printallparameters();

	unsigned int __NUM_UPARTITIONS = (universalparams.NUM_VERTICES + (MAX_UPARTITION_SIZE - 1)) /  MAX_UPARTITION_SIZE;
	unsigned int __NUM_APPLYPARTITIONS = ((universalparams.NUM_VERTICES / NUM_PEs) + (MAX_APPLYPARTITION_SIZE - 1)) /  MAX_APPLYPARTITION_SIZE; // NUM_PEs
	
	// create act-pack format
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ act_pack_map[i][p_u] = new map_t[MAX_NUM_LLPSETS]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ act_pack_map[i][p_u][t].offset = 0; act_pack_map[i][p_u][t].size = 0; }}}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ act_pack_map2[i][p_u] = new map_t[MAX_NUM_LLP_PER_UPARTITION]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ for(unsigned int t=0; t<MAX_NUM_LLP_PER_UPARTITION; t++){ act_pack_map2[i][p_u][t].offset = 0; act_pack_map2[i][p_u][t].size = 0; }}}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ act_pack_edgeupdates_map[i][p_u] = new map_t[MAX_NUM_LLP_PER_UPARTITION]; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ for(unsigned int t=0; t<MAX_NUM_LLP_PER_UPARTITION; t++){ act_pack_edgeupdates_map[i][p_u][t].offset = 0; act_pack_edgeupdates_map[i][p_u][t].size = 0; }}}
	
	act_pack * pack = new act_pack(universalparams);
	pack->pack(vertexptrbuffer, edgedatabuffer, act_pack_edges, act_pack_map, act_pack_map2, act_pack_edgeupdates, act_pack_edgeupdates_map); // CRITICAL REMOVEME.
	for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"act-pack-edgeupdates:: PE: "<<i<<": act_pack_edgeupdates["<<i<<"].size(): "<<act_pack_edgeupdates[i].size() * EDGE_PACK_SIZE<<""<<endl; }

	// create csr format
	unsigned int * degrees[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ degrees[i] = new unsigned int[(universalparams.NUM_VERTICES / NUM_PEs) + 64]; }
	unsigned int * v_ptr[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ v_ptr[i] = new unsigned int[(universalparams.NUM_VERTICES / NUM_PEs) + 64]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<(universalparams.NUM_VERTICES / NUM_PEs) + 64; t++){ v_ptr[i][t] = 0; degrees[i][t] = 0; }}
	unsigned int H = 0;
	for(unsigned int vid=0; vid<universalparams.NUM_VERTICES-1; vid++){
		unsigned int vptr_begin = vertexptrbuffer[vid];
		unsigned int vptr_end = vertexptrbuffer[vid+1];
		unsigned int edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ continue; }
		utilityobj->checkoutofbounds("app::ERROR 211::", vid / MAX_UPARTITION_SIZE, __NUM_UPARTITIONS, NAp, NAp, NAp);
		for(unsigned int i=0; i<edges_size; i++){
			unsigned int H_hash = H % NUM_PEs;
			edge3_type edge = edgedatabuffer[vptr_begin + i];
			csr_pack_edges[H_hash].push_back(edge);
			degrees[H_hash][edge.srcvid / NUM_PEs] += 1;
		}
		H += 1;
	}
	
	edge3_type first_edge = csr_pack_edges[0][0];
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ first_edge.srcvid: "<<first_edge.srcvid<<", first_edge.dstvid: "<<first_edge.dstvid<<endl;
	#endif 
	#ifdef ___FORCE_SUCCESS_SINGLE_CHANNEL___
	rootvid = first_edge.srcvid;
	universalparams.ROOTVID = first_edge.srcvid;
	cout<<"app::run:: [CHANGED] app algo started. (algo: "<<algo<<", numiterations: "<<numiterations<<", rootvid: "<<rootvid<<", graph path: "<<graph_path<<", graph dir: "<<graphisundirected<<", _binaryFile1: "<<_binaryFile1<<")"<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	unsigned int max_degree = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ unsigned int index = 0, ind = 0; for(unsigned int vid=0; vid<universalparams.NUM_VERTICES; vid++){ if(vid % NUM_PEs == i){ 
		if(max_degree < degrees[i][vid / NUM_PEs]){ max_degree = degrees[i][vid / NUM_PEs]; }
		v_ptr[i][vid / NUM_PEs] = index; index += degrees[i][vid / NUM_PEs]; ind += 1; }}}			
	for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"csr-pack:: PE: 21: act_pack_edges["<<i<<"].size(): "<<csr_pack_edges[i].size()<<endl; }
	cout<<"app::csr-pack:: max_degree: "<<max_degree<<endl;

	// load globalparams: {vptrs, edges, updatesptrs, updates, vertexprops, frontiers}
	cout<<"app: loading global addresses: {vptrs, edges, updates, vertexprops, frontiers}..."<<endl;
	unsigned int numcsredges = 0; for(unsigned int i=0; i<NUM_PEs; i++){ if(numcsredges < csr_pack_edges[i].size()){ numcsredges = csr_pack_edges[i].size(); }} 
	unsigned int numww_csredges = (numcsredges / EDGE_PACK_SIZE) + 16;
	unsigned int numww_actpackedges = 0; for(unsigned int i=0; i<NUM_PEs; i++){ if(numww_actpackedges < act_pack_edges[i].size()){ numww_actpackedges = act_pack_edges[i].size(); }} 
	unsigned int numww_actpackedgeupdates = 0; for(unsigned int i=0; i<NUM_PEs; i++){ if(numww_actpackedgeupdates < act_pack_edgeupdates[i].size()){ numww_actpackedgeupdates = act_pack_edgeupdates[i].size(); }} 
	
	unsigned int csrvptrsz_u32 = ((universalparams.NUM_VERTICES / NUM_PEs) + 64); 
	unsigned int vdatasz_u32 = __NUM_APPLYPARTITIONS * MAX_APPLYPARTITION_VECSIZE * EDGE_PACK_SIZE * 2;
	unsigned int cfrontiersz_u32 = 1 * MAX_APPLYPARTITION_VECSIZE * EDGE_PACK_SIZE * 2;
	unsigned int nfrontiersz_u32 = (__NUM_APPLYPARTITIONS * MAX_ACTVV_VECSIZE * NUM_ACTVVPARTITIONS_PER_APPLYPARTITION * EDGE_PACK_SIZE) * 2;
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"--_________________________--------------------- nfrontiersz_u32: "<<nfrontiersz_u32<<", vdatasz_u32: "<<vdatasz_u32<<", globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS] = "<<((nfrontiersz_u32 / (HBM_CHANNEL_PACK_SIZE / 2)) + 16)<<" "<<endl;
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app: EDGE_PACK_SIZE: "<<EDGE_PACK_SIZE<<endl;
	cout<<"app: HBM_CHANNEL_PACK_SIZE: "<<HBM_CHANNEL_PACK_SIZE<<endl;
	cout<<"app: HBM_AXI_PACK_SIZE: "<<HBM_AXI_PACK_SIZE<<endl;
	cout<<"app: HBM_AXI_PACK_BITSIZE: "<<HBM_AXI_PACK_BITSIZE<<endl;
	cout<<"app: HBM_CHANNEL_BYTESIZE: "<<HBM_CHANNEL_BYTESIZE<<endl;
	cout<<"app: MAX_APPLYPARTITION_VECSIZE: "<<MAX_APPLYPARTITION_VECSIZE<<endl;
	cout<<"app: MAX_APPLYPARTITION_SIZE: "<<MAX_APPLYPARTITION_SIZE<<endl;
	cout<<"app: MAX_UPARTITION_VECSIZE: "<<MAX_UPARTITION_VECSIZE<<endl;
	cout<<"app: MAX_UPARTITION_SIZE: "<<MAX_UPARTITION_SIZE<<endl;
	cout<<"app: HBM_CHANNEL_BYTESIZE: "<<HBM_CHANNEL_BYTESIZE<<endl;
	cout<<"app: HBM_CHANNEL_SIZE: "<<HBM_CHANNEL_SIZE<<endl;
	cout<<"app: UPDATES_BUFFER_PACK_SIZE: "<<UPDATES_BUFFER_PACK_SIZE<<endl;
	cout<<"app: __NUM_UPARTITIONS: "<<__NUM_UPARTITIONS<<endl;
	cout<<"app: __NUM_APPLYPARTITIONS: "<<__NUM_APPLYPARTITIONS<<endl;
	cout<<"app: NUM_ACTVVPARTITIONS_PER_APPLYPARTITION: "<<NUM_ACTVVPARTITIONS_PER_APPLYPARTITION<<endl;
	cout<<"app: MAX_ACTVV_VECSIZE: "<<MAX_ACTVV_VECSIZE<<endl;
	cout<<"app: UPDATES_BUFFER_PACK_SIZE: "<<UPDATES_BUFFER_PACK_SIZE<<endl;
	#endif 
	
	// unsigned int globalparams[1024];
	for(unsigned int t=0; t<1024; t++){ globalparams[t] = 0; }
	
	// load actions 
	for(unsigned int i=0; i<NUM_PEs; i++){ globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTIONS] = 512; } 
	unsigned int size_u32 = 16 * EDGE_PACK_SIZE;	
	
	// load raw edge-update vptrs 
	for(unsigned int i=0; i<NUM_PEs; i++){  
		globalparams[GLOBALPARAMSCODE__WWSIZE__ACTIONS] = (size_u32 / HBM_AXI_PACK_SIZE) + 16;  // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTIONS] + globalparams[GLOBALPARAMSCODE__WWSIZE__ACTIONS]; 
	}
	size_u32 = 0;
	// vector<edge3_type> final_edge_updates[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS];
	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
	vector<edge3_type> final_edge_updates[NUM_PEs][MAX_NUM_UPARTITIONS][MAX_NUM_LLPSETS];
	pack->load_edgeupdates(vertexptrbuffer, edgedatabuffer, final_edge_updates);
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS];
		for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ 
			for(unsigned int llp_set=0; llp_set<MAX_NUM_LLPSETS; llp_set++){
				unsigned int min = 0xFFFFFFF; for(unsigned int n=0; n<NUM_PEs; n++){ if(min > final_edge_updates[i][p_u][llp_set].size()){ min = final_edge_updates[i][p_u][llp_set].size(); } }
				unsigned int permissible_load_wwsz = 0; if(min < EDGE_UPDATES_WORKBUFFER_SIZE * EDGE_PACK_SIZE){ permissible_load_wwsz = min / EDGE_PACK_SIZE; } else { permissible_load_wwsz = EDGE_UPDATES_WORKBUFFER_SIZE; }

				HBM_channel[i][base_offset + (index / HBM_AXI_PACK_SIZE)].data[index % HBM_AXI_PACK_SIZE] = permissible_load_wwsz; 
				if(false && i==0 && p_u < universalparams.NUM_UPARTITIONS && llp_set < universalparams.NUM_APPLYPARTITIONS){ cout<<"final_edge_updates["<<i<<"]["<<p_u<<"]["<<llp_set<<"].size(): "<<final_edge_updates[i][p_u][llp_set].size()<<"[=>"<<permissible_load_wwsz*EDGE_PACK_SIZE<<"]"<<endl; }
				index += 1;
				if(i==0){ size_u32 += 1; }
			}
		}
	}
	#endif 
	
	// load edge udpdates vptrs  
	for(unsigned int i=0; i<NUM_PEs; i++){  
		globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS] = (size_u32 / HBM_AXI_PACK_SIZE) + 16;  // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS]; 
	}
	size_u32 = 0;
	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
	for(unsigned int i=0; i<NUM_PEs; i++){  // FIXME?
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS];
		for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ 
			for(unsigned int t=0; t<MAX_NUM_LLP_PER_UPARTITION; t++){ 
				HBM_channel[i][base_offset + (index / HBM_AXI_PACK_SIZE)].data[index % HBM_AXI_PACK_SIZE] = act_pack_edgeupdates_map[i][p_u][t].offset; 
				#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
				HBM_channel[i][base_offset + ((index + 1) / HBM_AXI_PACK_SIZE)].data[(index + 1) % HBM_AXI_PACK_SIZE] = 0; 
				#else 
				HBM_channel[i][base_offset + ((index + 1) / HBM_AXI_PACK_SIZE)].data[(index + 1) % HBM_AXI_PACK_SIZE] = act_pack_edgeupdates_map[i][p_u][t].size; 
				#endif 
				index += 2;
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	#endif 
	
	// load csr vptrs 
	for(unsigned int i=0; i<NUM_PEs; i++){  
		globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS] = (size_u32 / HBM_AXI_PACK_SIZE) + 16; // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS]; 
	}
	size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS];
		unsigned int * T = (unsigned int *)&HBM_channel[i][base_offset].data[0];
		for(unsigned int t=0; t<csrvptrsz_u32; t++){
			T[index] = v_ptr[i][t]; // HBM_channel[i].v_ptr[t];
			index += 1;
			if(i==0){ size_u32 += 1; }
		}
	}
	
	// load act-pack vptrs  
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS]; 
	}
	size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS];
		for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ 
			for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
				HBM_channel[i][base_offset + (index / HBM_AXI_PACK_SIZE)].data[index % HBM_AXI_PACK_SIZE] = act_pack_map[i][p_u][t].offset; 
				HBM_channel[i][base_offset + ((index + 1) / HBM_AXI_PACK_SIZE)].data[(index + 1) % HBM_AXI_PACK_SIZE] = act_pack_map[i][p_u][t].size; 
				index += 2;
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	
	// load act-pack vptrs2
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS] = (size_u32 / HBM_AXI_PACK_SIZE) + 16; // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS]; 
	}
	size_u32 = 0;
	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___ 
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2];
		for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ 
			for(unsigned int t=0; t<MAX_NUM_LLP_PER_UPARTITION; t++){
				HBM_channel[i][base_offset + (index / HBM_AXI_PACK_SIZE)].data[index % HBM_AXI_PACK_SIZE] = act_pack_map2[i][p_u][t].offset; 
				HBM_channel[i][base_offset + ((index + 1) / HBM_AXI_PACK_SIZE)].data[(index + 1) % HBM_AXI_PACK_SIZE] = act_pack_map2[i][p_u][t].size; 
				index += 2;
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	#endif 
	
	// load vertex-updates ptrs
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2] = (size_u32 / HBM_AXI_PACK_SIZE) + 16; // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2] + globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2]; 
	}
	size_u32 = 0;
	keyvalue_t updateswwcount[NUM_PEs][MAX_NUM_LLPSETS]; 
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ updateswwcount[i][t].value = 0; updateswwcount[i][t].key = 0; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
			for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ 
				updateswwcount[i][t].value += act_pack_map[i][p_u][t].size;
			}
		}
	}
	for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ 
		if(false){ cout<<"--- updateswwcount[0]["<<t<<"].value: "<<updateswwcount[0][t].value<<endl; }
	}
	keyvalue_t maxs[MAX_NUM_LLPSETS]; for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ maxs[t].key = 0; maxs[t].value = 0; }
	for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){ for(unsigned int n=0; n<NUM_PEs; n++){ if(maxs[t].value < updateswwcount[n][t].value){ maxs[t].value = updateswwcount[n][t].value; }}}
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
		cout<<"##### maxs["<<t<<"].key: "<<maxs[t].key<<", maxs["<<t<<"].value: "<<maxs[t].value<<endl; 
	}
	#endif 
	for(unsigned int t=1; t<MAX_NUM_LLPSETS; t++){ maxs[t].key = maxs[t-1].key + maxs[t-1].value + 0; }
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS];
		for(unsigned int t=0; t<MAX_NUM_LLPSETS; t++){
			if(false){ if(i==0){ cout<<"maxs["<<t<<"].key: "<<maxs[t].key<<", maxs["<<t<<"].value: "<<maxs[t].value<<endl; }}
			HBM_channel[i][base_offset + t].data[0] = maxs[t].key;
			if(i==0){ size_u32 += HBM_AXI_PACK_SIZE; }
		}
	}
	unsigned int max_num_updates = maxs[MAX_NUM_LLPSETS-1].key;
	if(true){ cout<<"app: max_num_updates: "<<max_num_updates<<endl; }
	
	// load raw edge updates
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS] = (size_u32 / HBM_AXI_PACK_SIZE) + 16; // NB: not 'HBM_CHANNEL_PACK_SIZE' because only half of dual-HBM channel is used.
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS] + globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS]; 
	}
	size_u32 = 0; 
	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___ 
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int p_u=0; p_u<universalparams.NUM_UPARTITIONS; p_u++){
			for(unsigned int llp_set=0; llp_set<universalparams.NUM_APPLYPARTITIONS; llp_set++){
				unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES];
				unsigned int permissible_load_wwsz = 0; if(final_edge_updates[i][p_u][llp_set].size() < EDGE_UPDATES_DRAMBUFFER_SIZE * EDGE_PACK_SIZE){ permissible_load_wwsz = final_edge_updates[i][p_u][llp_set].size() / EDGE_PACK_SIZE; } else { permissible_load_wwsz = EDGE_UPDATES_DRAMBUFFER_SIZE; }
				unsigned int index = 0;
				
				for(unsigned int t=0; t<EDGE_UPDATES_DRAMBUFFER_SIZE; t++){ 
					if(t < permissible_load_wwsz){
					for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){	
						unsigned int offset = (p_u * universalparams.NUM_APPLYPARTITIONS * EDGE_UPDATES_DRAMBUFFER_SIZE) + (llp_set * EDGE_UPDATES_DRAMBUFFER_SIZE);				
						HBM_channel[i][base_offset + offset + t].data[2 * v] = final_edge_updates[i][p_u][llp_set][index].srcvid % MAX_UPARTITION_SIZE;
						HBM_channel[i][base_offset + offset + t].data[2 * v + 1] = final_edge_updates[i][p_u][llp_set][index].dstvid % MAX_UPARTITION_SIZE;
						index += 1;
					}
					}
					if(i==0){ size_u32 += EDGE_PACK_SIZE * 2; }
				}
			}
		}
	}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int p_u=0; p_u<MAX_NUM_UPARTITIONS; p_u++){ for(unsigned int llp_set=0; llp_set<MAX_NUM_LLPSETS; llp_set++){ final_edge_updates[i][p_u][llp_set].clear(); }}}
	#endif 
	
	// load partial-processed edge updates
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES] + globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES]; 
	}
	size_u32 = EDGE_UPDATES_DRAMBUFFER_SIZE * 4 * HBM_CHANNEL_PACK_SIZE; // '4' is padding
	
	// load edge updates (actpack format)
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES] + globalparams[GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES]; 
	}
	size_u32 = 0; 
	#ifdef ___ENABLE___DYNAMICGRAPHANALYTICS___
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES];
		for(unsigned int t=0; t<numww_actpackedges; t++){ 	// numww_actpackedges, numww_actpackedgeupdates
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				if(false){
				HBM_channel[i][base_offset + t].data[2 * v] = act_pack_edges[i][t].data[v].srcvid % MAX_UPARTITION_SIZE;
				HBM_channel[i][base_offset + t].data[2 * v + 1] = act_pack_edges[i][t].data[v].dstvid % MAX_UPARTITION_SIZE;	
				}
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	#endif 
	
	// load csr edges 
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES] + globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES]; 
	}
	size_u32 = 0; 
	#ifdef TRAVERSAL_ALGORITHM_TYPE
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		unsigned int index = 0;
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES];
		for(unsigned int t=0; t<numww_csredges; t++){ 
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				HBM_channel[i][base_offset + t].data[2 * v] = csr_pack_edges[i][index].srcvid;
				HBM_channel[i][base_offset + t].data[2 * v + 1] = (csr_pack_edges[i][index].dstvid << 1) | csr_pack_edges[i][index].valid;	
				index += 1;
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	#endif 
	
	// load act-pack edges 
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES] + globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES]; 
	}
	size_u32 = 0; unsigned int sourceid = 0; unsigned int destid = 0; unsigned int weight = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES];
		for(unsigned int t=0; t<numww_actpackedges; t++){ 	
			for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
				unsigned int isvalid = act_pack_edges[i][t].data[v].valid;

				unsigned int _sample_key = INVALIDDATA; unsigned int _sample_u = 0; 
				for(unsigned int v1=0; v1<EDGE_PACK_SIZE; v1++){ if(act_pack_edges[i][t].data[v1].valid == 1){ _sample_key = act_pack_edges[i][t].data[v1].dstvid % EDGE_PACK_SIZE; _sample_u = v1; }} 
				unsigned int _rotateby = 0; if(_sample_key > _sample_u){ _rotateby = _sample_key - _sample_u; } else { _rotateby = _sample_u - _sample_key; }
				
				if(isvalid==1){
					sourceid = act_pack_edges[i][t].data[v].srcvid % MAX_UPARTITION_VECSIZE;
					destid = get_local3(act_pack_edges[i][t].data[v].dstvid) % MAX_UPARTITION_VECSIZE;
					weight = 1;
				} else {
					sourceid = INVALIDDATA;
					destid = get_local3(act_pack_edges[i][t].data[v].dstvid) % MAX_UPARTITION_VECSIZE;
					weight = 1;
				}
				if(v==0){ weight = _rotateby; }
				HBM_channel[i][base_offset + t].data[v] = ((weight & MAXLOCALVALUE2_ACTPACK_EDGEID) << (MAXNUMBITS2_ACTPACK_DESTVID + MAXNUMBITS2_ACTPACK_SRCVID)) | ((sourceid & MAXLOCALVALUE2_ACTPACK_SRCVID) << MAXNUMBITS2_ACTPACK_DESTVID) | (destid & MAXLOCALVALUE2_ACTPACK_DESTVID);		
				///////////////////////// FIXME?
				HBM_channel[i][base_offset + t].data[HBM_AXI_PACK_SIZE + v] = ((weight & MAXLOCALVALUE2_ACTPACK_EDGEID) << (MAXNUMBITS2_ACTPACK_DESTVID + MAXNUMBITS2_ACTPACK_SRCVID)) | ((sourceid & MAXLOCALVALUE2_ACTPACK_SRCVID) << MAXNUMBITS2_ACTPACK_DESTVID) | (destid & MAXLOCALVALUE2_ACTPACK_DESTVID);		
				/////////////////////////
				if(i==0){ size_u32 += 2; }
			}
		}
	}
	
	// load vertex updates 
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES] = (size_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES] + 0; // act-pack edges occupy 1/2 of hbmchannel
	}
	
	// load vertex properties
	for(unsigned int i=0; i<NUM_PEs; i++){
		globalparams[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES] = max_num_updates + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES] + globalparams[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES]; 
	}
	size_u32 = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int base_offset = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS];
		for(unsigned int p=0; p<__NUM_APPLYPARTITIONS; p++){ 
			for(unsigned int t=0; t<MAX_APPLYPARTITION_VECSIZE; t++){ 
				for(unsigned int v=0; v<EDGE_PACK_SIZE; v++){
					unsigned int index = p*MAX_APPLYPARTITION_VECSIZE*EDGE_PACK_SIZE + t*EDGE_PACK_SIZE + v;
					HBM_channel[i][base_offset + (p * MAX_APPLYPARTITION_VECSIZE + t)].data[2 * v] = algorithmobj->vertex_initdata(universalparams.ALGORITHM, index);
					HBM_channel[i][base_offset + (p * MAX_APPLYPARTITION_VECSIZE + t)].data[2 * v + 1] = 0;
					if(i==0){ size_u32 += 2; }
				}
			}
		}
	}
	
	// cfrontier 
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS] = (vdatasz_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS] + globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS]; 
	}
	// nfrontier
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP] = (cfrontiersz_u32 / HBM_CHANNEL_PACK_SIZE) + 16;
		globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP] + globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP]; 
		globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS] = (nfrontiersz_u32 / (HBM_CHANNEL_PACK_SIZE / 2)) + 16;
	}	
	
	globalparams[GLOBALPARAMSCODE__PARAM__NUM_VERTICES] = universalparams.NUM_VERTICES;
	globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES] = universalparams.NUM_EDGES;

	// load globalparams
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__ACTIONS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTIONS];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS]; //
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__CSREDGES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__VDATAS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP];
		HBM_channel[i][GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS].data[0] = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS];
		
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__ACTIONS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTIONS];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__CSRVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS]; //
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__CSREDGES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__VDATAS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP];
		HBM_channel[i][GLOBALPARAMSCODE__WWSIZE__NFRONTIERS].data[0] = globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS];
		
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_VERTICES].data[0] = universalparams.NUM_VERTICES;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_EDGES].data[0] = universalparams.NUM_EDGES;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_UPARTITIONS].data[0] = universalparams.NUM_UPARTITIONS;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_APPLYPARTITIONS].data[0] = universalparams.NUM_APPLYPARTITIONS;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_ITERATIONS].data[0] = 1;//universalparams.NUM_ITERATIONS; // FIXME.
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEFRONTIERSFORCONTROLSWITCH].data[0] = 128;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__MAXDEGREE].data[0] = max_degree;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__ALGORITHM].data[0] = universalparams.ALGORITHM;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__ROOTVID].data[0] = rootvid;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__RANGEPERCHANNEL].data[0] = num_vertices / NUM_PEs;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__THRESHOLD__ACTIVEDSTVID].data[0] = 16;
		HBM_channel[i][GLOBALPARAMSCODE__PARAM__NUM_RUNS].data[0] = 1; // 

		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___RESETBUFFERSATSTART].data[0] = 1;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___PREPAREEDGEUPDATES].data[0] = 0;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___PROCESSEDGEUPDATES].data[0] = 0;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___PROCESSEDGES].data[0] = 1;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___READ_FRONTIER_PROPERTIES].data[0] = 1;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___VCPROCESSEDGES].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___ECUPDATEEDGES].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___ECPROCESSEDGES].data[0] = 1; // FIXME
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___SAVEVCUPDATES].data[0] = 1; // FIXME? CAUSE OF HANGING?
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___COLLECTACTIVEDSTVIDS].data[0] = 1;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___APPLYUPDATESMODULE].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___READ_DEST_PROPERTIES].data[0] = 1;
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___APPLYUPDATES].data[0] = 1; ////////////////////
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___COLLECT_AND_SAVE_FRONTIER_PROPERTIES].data[0] = 1; 
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___SAVE_DEST_PROPERTIES].data[0] = 1; 
		
		//////////////////////////////////////////////////////////////////////////////////////////////
		
		HBM_channel[i][GLOBALPARAMSCODE___ENABLE___EXCHANGEFRONTIERINFOS].data[0] = 1;
		
		HBM_channel[i][GLOBALPARAMSCODE__ASYNC__BATCH].data[0] = 0;
		HBM_channel[i][GLOBALPARAMSCODE__ASYNC__BATCHSIZE].data[0] = universalparams.NUM_APPLYPARTITIONS;
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__ACTIONS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__ACTIONS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__CSRVPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__CSRVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKVPTRS2]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__UPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__RAWEDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__PARTIALLYPROCESSEDEDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__EDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__CSREDGES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__CSREDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__ACTPACKEDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__VERTEXUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__VDATAS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__VDATAS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__CFRONTIERSTMP]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__WWSIZE__NFRONTIERS: "<<globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__ACTIONS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTIONS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSRVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKVPTRS2]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__UPDATESPTRS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__RAWEDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__PARTIALLYPROCESSEDEDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__EDGEUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__CSREDGES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__CSREDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__ACTPACKEDGES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__VDATAS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__VDATAS]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__CFRONTIERSTMP]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS: "<<globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS]<<endl;
	unsigned int lastww_addr = globalparams[GLOBALPARAMSCODE__BASEOFFSET__NFRONTIERS] + globalparams[GLOBALPARAMSCODE__WWSIZE__NFRONTIERS];
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__BASEOFFSET__END: "<<lastww_addr<<" (of "<< ((1 << 28) / 4 / HBM_AXI_PACK_SIZE) <<" wide-words) ("<<lastww_addr * HBM_CHANNEL_PACK_SIZE * 4<<" bytes)"<<endl;
	utilityobj->checkoutofbounds("app::ERROR 2234::", lastww_addr, ((1 << 28) / 4 / HBM_AXI_PACK_SIZE), __NUM_APPLYPARTITIONS, MAX_APPLYPARTITION_VECSIZE, NAp);
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__PARAM__NUM_VERTICES: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_VERTICES]<<endl;
	cout<<"app:: BASEOFFSET: GLOBALPARAMSCODE__PARAM__NUM_EDGES: "<<globalparams[GLOBALPARAMSCODE__PARAM__NUM_EDGES]<<endl;
	#endif 
	// exit(EXIT_SUCCESS);

	// clear 
	for(unsigned int i=0; i<NUM_PEs; i++){ csr_pack_edges[i].clear(); act_pack_edges[i].clear(); } // clear 

	// assign root vid 
	keyvalue_t root; root.key = rootvid; root.value = 0; keyvalue_t invalid; invalid.key = INVALIDDATA; invalid.value = INVALIDDATA; 
	for(unsigned int t=0; t<16; t++){ 
		for(unsigned int v=0; v<HBM_CHANNEL_PACK_SIZE; v++){ 
			HBM_center[t].data[v] = INVALIDDATA;
		} 
	}
	unsigned int hash_loc = root.key % EDGE_PACK_SIZE;
	unsigned int index = 2*hash_loc;
	HBM_center[index / HBM_CHANNEL_PACK_SIZE].data[index % HBM_CHANNEL_PACK_SIZE] = root.key; 
	HBM_center[(index + 1) / HBM_CHANNEL_PACK_SIZE].data[(index + 1) % HBM_CHANNEL_PACK_SIZE] = root.value; 
	
	cout<<"app: copying to axi-friendly channels..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				HBM_axichannel[0][i][t].data[v] = HBM_channel[i][t].data[v];
			}
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				HBM_axichannel[1][i][t].data[v] = HBM_channel[i][t].data[HBM_AXI_PACK_SIZE + v];
			}
		}
	}
		
	cout<<"app: copying to axi-friendly channels..."<<endl;
	for(unsigned int t=0; t<HBM_CHANNEL_SIZE; t++){ 
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			HBM_axicenter[0][t].data[v] = HBM_center[t].data[v];
		}
		for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
			HBM_axicenter[1][t].data[v] = HBM_center[t].data[HBM_AXI_PACK_SIZE + v];
		}
	}
	
	//Free each sub-array
    // for(unsigned int i=0; i<NUM_PEs; i++){ 
        // delete[] HBM_channel[i];   
    // }
    // delete[] HBM_channel;
	edgedatabuffer.clear();
	vertexptrbuffer.clear();
	// exit(EXIT_SUCCESS); 
	
	#ifdef HOST_PRINT_RESULTS_XXXX
	cout<<"---------------------------------------------- app:: before ---------------------------------------------- "<<endl;
	unsigned int base_offset__ = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES];
	for(unsigned int i=0; i<1; i++){
		for(unsigned int t=0; t<2; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				cout<<HBM_axichannel[1][i][base_offset__ + t].data[v]<<", ";	
			}
			cout<<endl;
		}
	}
	#endif 
	
	// run acts
	action_t action;
	action.module = ALL_MODULES;
	action.start_pu = 0; 
	action.size_pu = universalparams.NUM_UPARTITIONS; 
	action.start_pv = 0;
	action.size_pv = universalparams.NUM_APPLYPARTITIONS; 
	action.start_llpset = 0; 
	action.size_llpset = universalparams.NUM_APPLYPARTITIONS; 
	action.start_llpid = 0; 
	action.size_llpid = EDGE_PACK_SIZE; 
	action.start_gv = 0; 
	action.size_gv = NUM_VALID_PEs;
	action.finish = 1;
	
	#ifdef FPGA_IMPL
	host_fpga * fpgaobj = new host_fpga(universalparams);
	#else 
	acts_kernel * acts = new acts_kernel(universalparams);
	#endif 
	
	unsigned int num_burst_computes = 1;
	// unsigned int num_burst_computes = universalparams.NUM_UPARTITIONS + universalparams.NUM_APPLYPARTITIONS;
	for(unsigned int burst_compute=0; burst_compute<num_burst_computes; burst_compute++){
		if(num_burst_computes == 1){
			action.module = ALL_MODULES;
			action.start_pu = 0; 
			action.size_pu = universalparams.NUM_UPARTITIONS; 
			action.start_pv = 0;
			action.size_pv = universalparams.NUM_APPLYPARTITIONS; 
			action.start_llpset = 0; 
			action.size_llpset = universalparams.NUM_APPLYPARTITIONS; 
			action.start_llpid = 0; 
			action.size_llpid = EDGE_PACK_SIZE; 
			action.start_gv = 0; 
			action.size_gv = NUM_VALID_PEs;
			action.finish = 1;
		} else {
			// scatter <===> transport
			if(burst_compute >= 0 && burst_compute < universalparams.NUM_UPARTITIONS){ 
				action.module = PROCESS_EDGES_MODULE;
				action.start_pu = burst_compute; 
				action.size_pu = 1; 
				action.finish = 0;
			}
			
			// apply and gatherDSTs <===> transport
			if(burst_compute >= universalparams.NUM_UPARTITIONS && burst_compute < universalparams.NUM_UPARTITIONS + universalparams.NUM_APPLYPARTITIONS){
				action.module = APPLY_UPDATES_MODULE___AND___GATHER_DSTPROPERTIES_MODULE;
				action.start_pv = burst_compute - universalparams.NUM_UPARTITIONS; 
				action.size_pv = 1; 
				action.start_gv = burst_compute - universalparams.NUM_UPARTITIONS; 
				action.size_gv = 1;
				action.finish = 0;
			}		
		}		
			
		#ifdef FPGA_IMPL
		fpgaobj->runapp(action, binaryFile, HBM_axichannel, HBM_axicenter, globalparams, universalparams);
		#else 
		acts->top_function(
			(HBM_channelAXI_t *)HBM_axichannel[0][0], (HBM_channelAXI_t *)HBM_axichannel[1][0]
			#if NUM_VALID_HBM_CHANNELS>1
			,(HBM_channelAXI_t *)HBM_axichannel[0][1], (HBM_channelAXI_t *)HBM_axichannel[1][1] 
			,(HBM_channelAXI_t *)HBM_axichannel[0][2], (HBM_channelAXI_t *)HBM_axichannel[1][2] 
			,(HBM_channelAXI_t *)HBM_axichannel[0][3], (HBM_channelAXI_t *)HBM_axichannel[1][3] 
			,(HBM_channelAXI_t *)HBM_axichannel[0][4], (HBM_channelAXI_t *)HBM_axichannel[1][4] 
			,(HBM_channelAXI_t *)HBM_axichannel[0][5], (HBM_channelAXI_t *)HBM_axichannel[1][5] 
			#if NUM_VALID_HBM_CHANNELS>6
			,(HBM_channelAXI_t *)HBM_axichannel[0][6], (HBM_channelAXI_t *)HBM_axichannel[1][6] 
			,(HBM_channelAXI_t *)HBM_axichannel[0][7], (HBM_channelAXI_t *)HBM_axichannel[1][7] 
			,(HBM_channelAXI_t *)HBM_axichannel[0][8], (HBM_channelAXI_t *)HBM_axichannel[1][8] 
			,(HBM_channelAXI_t *)HBM_axichannel[0][9], (HBM_channelAXI_t *)HBM_axichannel[1][9] 
			,(HBM_channelAXI_t *)HBM_axichannel[0][10], (HBM_channelAXI_t *)HBM_axichannel[1][10] 
			,(HBM_channelAXI_t *)HBM_axichannel[0][11], (HBM_channelAXI_t *)HBM_axichannel[1][11]
			#if NUM_VALID_HBM_CHANNELS>12
			,(HBM_channelAXI_t *)HBM_axichannel[0][12], (HBM_channelAXI_t *)HBM_axichannel[1][12]
			#endif 
			#endif 
			#endif
			,(HBM_channelAXI_t *)HBM_axicenter[0], (HBM_channelAXI_t *)HBM_axicenter[1]
			,action.module ,action.start_pu ,action.size_pu ,action.start_pv ,action.size_pv ,action.start_llpset ,action.size_llpset ,action.start_llpid ,action.size_llpid ,action.start_gv ,action.size_gv ,action.finish
			);
		#endif 
	}

	#ifdef HOST_PRINT_RESULTS_XXXX
	cout<<"---------------------------------------------- app:: after ---------------------------------------------- "<<endl;
	base_offset__ = globalparams[GLOBALPARAMSCODE__BASEOFFSET__VERTEXUPDATES];
	for(unsigned int i=0; i<1; i++){
		for(unsigned int t=0; t<2; t++){ 
			for(unsigned int v=0; v<HBM_AXI_PACK_SIZE; v++){ 
				cout<<HBM_axichannel[1][i][base_offset__ + t].data[v]<<", ";	
			}
			cout<<endl;
		}
	}
	#endif 
	return;
}
// #endif 

void app::summary(){
	return;
}















