#include "app_hw.h"
using namespace std;

//HBM Banks requirements
#define MAX_HBM_BANKCOUNT 32
#define BANK_NAME(n) n | XCL_MEM_TOPOLOGY

app_hw::app_hw(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	myuniversalparams = _universalparams;
}
app_hw::~app_hw(){} 

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

long double app_hw::runapp(std::string binaryFile[2], HBM_channelTHIS_t * HBM_axichannel[2][NUM_PEs], HBM_channelTHIS_t * HBM_axicenter[2], universalparams_t universalparams){				
	cout<<">>> app_hw::runapp:: runapp started."<<endl;

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
    std::string device_name = device.getInfo<CL_DEVICE_NAME>();
    std::cout << "Found Device=" << device_name.c_str() << std::endl;

	auto fileBuf_procandsync = xcl::read_binary_file(binaryFile[0]);

    bool match = true;
	std::vector<cl_mem_ext_ptr_t> inoutBufExt_kvdram[1]; for(unsigned int i=0; i<1; i++){ inoutBufExt_kvdram[i] = std::vector<cl_mem_ext_ptr_t>(32); }
	cl_mem_ext_ptr_t inoutBufExt_vdram[2];
	std::vector<cl::Buffer> buffer_kvdram[1]; for(unsigned int i=0; i<1; i++){ buffer_kvdram[i] = std::vector<cl::Buffer>(32); }
	cl::Buffer buffer_vdram[2];
	std::vector<cl::Kernel> krnls_proc(3);
	std::vector<cl::Event> kernel_events(48);
	std::vector<cl::Event> write_events(48);
	std::vector<cl::Event> read_events(48);
	
	long double WALLprocandsynkerneltimelapse_ms = 0;

	unsigned int kvindex = 0;
	unsigned int assigned_kvindex = 0;
	unsigned int edgesindex = 0;
	unsigned int hbmindex = 0;
	unsigned int numI = 0;
	unsigned int maxnumI = 0;
	
	for(unsigned int i=0; i<VALID_NUMPEs; i+=1){
		inoutBufExt_kvdram[0][i].obj = HBM_axichannel[i];
		inoutBufExt_kvdram[0][i].param = 0;
		inoutBufExt_kvdram[0][i].flags = bank[i];
		cout<<"attaching inoutBufExt_kvdram[0]["<<i<<"] to kvdram to HBM["<<i<<"]: "<<endl;
	}
	for(unsigned int i=0; i<2; i+=1){
		inoutBufExt_vdram[i].obj = HBM_axicenter[i];
		inoutBufExt_vdram[i].param = 0;
		inoutBufExt_vdram[i].flags = bank[VALID_NUMPEs + i];
		cout<<"attaching inoutBufExt_vdram["<<i<<"] to vdram to HBM["<<VALID_NUMPEs + i<<"]: "<<endl;
	}
	
	cout<<"app_hw:: creating OCL buffers (P2)..."<<endl;
	for(unsigned int i=0; i<VALID_NUMPEs; i++){ 
		OCL_CHECK(err,
			  buffer_kvdram[0][i] =
				  cl::Buffer(context,
							 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
							 HBM_CHANNEL_BYTESIZE,
							 &inoutBufExt_kvdram[0][i], 
							 &err));
		cout<<"+++ app_hw: created buffer_kvdram[0]["<<i<<"] "<<endl;
	}
	for(unsigned int i=0; i<2; i++){ 
		OCL_CHECK(err,
			  buffer_vdram[i] =
				  cl::Buffer(context,
							 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
							 HBM_CHANNEL_BYTESIZE,
							 &inoutBufExt_vdram[i], 
							 &err));
		cout<<"+++ app_hw: created buffer_vdram["<<i<<"] "<<endl;
	}
	
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	printf("INFO: loading vmul kernel\n");
	#endif
	cl::Program::Binaries vmul_bins{{fileBuf_procandsync.data(), fileBuf_procandsync.size()}};
	devices.resize(1);
	OCL_CHECK(err,
			  cl::Program program(context, devices, vmul_bins, NULL, &err));
	
	// Create kernel objects
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	cout<<"app_hw:: creating kernel object..."<<endl;
	#endif
	std::string krnl_name = "top_function"; 
	std::string krnl_name_ = krnl_name + "_";
	std::string cu_id = std::to_string((1));
	std::string krnl2_name_full = krnl_name + ":{" + krnl_name_ + cu_id + "}"; 
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	printf("Creating a kernel [%s] for CU(%d)\n", krnl2_name_full.c_str(), (1));
	#endif
	OCL_CHECK(err, krnls_proc[0] = cl::Kernel(program, krnl2_name_full.c_str(), &err));

	// Setting kernel arguments (P2)...
	for(unsigned int i=0; i<VALID_NUMPEs; i+=1){
		OCL_CHECK(err, err = krnls_proc[0].setArg(i, buffer_kvdram[0][i])); 
		cout<<"setting argument krnls_proc[0].setArg("<<i<<", buffer_kvdram[0]["<<i<<"])"<<endl;
	}
	for(unsigned int i=0; i<2; i+=1){
		OCL_CHECK(err, err = krnls_proc[0].setArg(VALID_NUMPEs + i, buffer_vdram[i]));
		cout<<"setting argument krnls_proc[0].setArg("<<i<<", buffer_vdram[0])"<<endl;
	}

	// Migrating input data to device global memory...
	cout<<"app_hw:: Migrating input data to device global memory..."<<endl;
	#if NUM_PEs==12
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], 
													buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_kvdram[0][7], 
													buffer_kvdram[0][8], buffer_kvdram[0][9], buffer_kvdram[0][10], buffer_kvdram[0][11],
													buffer_vdram[0], buffer_vdram[1]}, 
													0, NULL, &write_events[0]));
	#elif NUM_PEs==24
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({
													buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], 
													buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_kvdram[0][7], 
													buffer_kvdram[0][8], buffer_kvdram[0][9], buffer_kvdram[0][10], buffer_kvdram[0][11], 
													buffer_kvdram[0][12], buffer_kvdram[0][13], buffer_kvdram[0][14], buffer_kvdram[0][15], 
													buffer_kvdram[0][16], buffer_kvdram[0][17], buffer_kvdram[0][18], buffer_kvdram[0][19], 
													buffer_kvdram[0][20], buffer_kvdram[0][21], buffer_kvdram[0][22], buffer_kvdram[0][23], 
													buffer_vdram[0], buffer_vdram[1]}, 
													0, NULL, &write_events[0]));
	#else 
	// NOT IMPLEMENTED
	#endif
	OCL_CHECK(err, err = write_events[0].wait());
	OCL_CHECK(err, err = q.finish());
	
	cout<<"app_hw:: Processing Graph... "<<universalparams.NUM_ITERATIONS<<" iterations specified"<<endl;
	std::chrono::steady_clock::time_point beginkerneltime1 = std::chrono::steady_clock::now();	
	for(unsigned int GraphIter=0; GraphIter<universalparams.NUM_ITERATIONS; GraphIter++){
		cout<< TIMINGRESULTSCOLOR <<">>> app_hw::runapp: iteration: "<<GraphIter<<" (of "<<universalparams.NUM_ITERATIONS<<" iterations)"<< RESET <<endl;
		std::chrono::steady_clock::time_point iter_starttime = std::chrono::steady_clock::now();

		std::cout <<">>> app_hw: processing edges and applying updates in iteration "<<GraphIter<<"..."<<endl;
		OCL_CHECK(err, err = q.enqueueTask(krnls_proc[0], NULL, &kernel_events[0])); 
		OCL_CHECK(err, err = kernel_events[0].wait()); 

		#ifdef CONFIG_ENABLE_PROFILING
		long double iter_timeelapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - iter_starttime).count();
		cout<<">>> app_hw: iteration: "<<GraphIter<<": Time elapsed: "<<iter_timeelapsed<<"ms, num edges processed: "<<universalparams.NUM_EDGES<<" edges, throughput: "<<((universalparams.NUM_EDGES / iter_timeelapsed) * (1000))<<" edges/sec ("<<(((universalparams.NUM_EDGES / iter_timeelapsed) * (1000)) / 1000000)<<" ME/s)"<<endl;	
		#endif 
	}
	WALLprocandsynkerneltimelapse_ms = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime1).count()) / 1000;			
	std::cout <<">>> kernel time elapsed for all iterations : "<<WALLprocandsynkerneltimelapse_ms<<" ms, "<<(WALLprocandsynkerneltimelapse_ms * 1000)<<" microsecs, "<<std::endl;
	OCL_CHECK(err, err = q.finish());
	
	cout<<"app_hw:: migrating workload back to HOST..."<<endl;
	#if NUM_PEs==12
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_vdram[0],
													buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_vdram[1],
													buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_vdram[2]}, 
													CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
	#elif NUM_PEs==24
	OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_kvdram[0][7], buffer_kvdram[0][8], buffer_vdram[0],
													buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_kvdram[1][7], buffer_kvdram[1][8], buffer_vdram[1],
													buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2]}, 
													CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
	#else 
	// NOT IMPLEMENTED
	#endif
	OCL_CHECK(err, err = read_events[0].wait());
	OCL_CHECK(err, err = q.finish());

	OCL_CHECK(err, err = read_events[0].wait());
	OCL_CHECK(err, err = q.finish());
	return WALLprocandsynkerneltimelapse_ms;
}
#endif








