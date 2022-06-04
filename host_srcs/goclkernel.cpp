#include "goclkernel.h"
using namespace std;

//HBM Banks requirements
#define MAX_HBM_BANKCOUNT 32
#define BANK_NAME(n) n | XCL_MEM_TOPOLOGY

#ifdef TESTKERNEL
#define TESTHWKERNEL
#endif 
// #define RUNALLBUFFERS

goclkernel::goclkernel(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
}
goclkernel::~goclkernel(){} 

long double goclkernel::getaveragetimeelapsed(long double kerneltimelapse_ms[MAXNUMSUBCPUTHREADS]){
	long double av = 0;
	
	long double count = NUM_PEs;
	unsigned int startind = 1;
	unsigned int numinds = NUM_PEs-1;
	if(count == 1){ startind = 0; numinds = NUM_PEs; }
	else { startind = 1; numinds = NUM_PEs-1; }
	
	for(unsigned int i=startind; i<NUM_PEs; i++){ // FIXME. 0 adds some unwanted latency
		av += kerneltimelapse_ms[i];
	}
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS
	cout<<"goclkernel::getaveragetimeelapsed: average time elapsed: "<<av / (NUM_PEs-1)<<endl;
	#endif
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

void goclkernel::print_active_vpartitions(unsigned int GraphIter, uint512_vec_dt * vdram){
	pmask_dt pmask0[universalparams.NUMPROCESSEDGESPARTITIONS];
	uint512_ivec_dt * tempvdram = (uint512_ivec_dt *)vdram;		
	unsigned int vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	unsigned int offset_kvs = vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK;
	for (buffer_type i=0; i<universalparams.NUMPROCESSEDGESPARTITIONS; i++){
		pmask0[i] = tempvdram[offset_kvs + i].data[GraphIter];
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<">>> goclkernel::print_active_vpartitions: GraphIter: "<<GraphIter<<endl;
	unsigned int num_actvps = 0;
	for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){
		if(pmask0[t] > 0  && t < 16){ cout<<t<<", "; }
		if(pmask0[t] > 0){ num_actvps += 1; }
	}
	cout<<" ("<<num_actvps<<" active partitions, "<<universalparams.NUMPROCESSEDGESPARTITIONS<<" total partitions)"<<endl;
	#endif 
	return;
}

long double goclkernel::runapp(std::string binaryFile[2], uint512_vec_dt * mdram, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS], long double timeelapsed_totals[128][8], unsigned int numValidIters, 
		unsigned int * vpmaskbuffer[MAXNUMGRAPHITERATIONS], unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS], vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer){				
	cout<<">>> goclkernel::runapp:: runapp started."<<endl;
	
	#ifdef TESTHWKERNEL
	unsigned int _NUMCOMPUTEUNITS_SLR0 = 1; 
	unsigned int _NUMCOMPUTEUNITS_SLR1 = 1; 
	unsigned int _NUMCOMPUTEUNITS_SLR2 = 1; 
	#else 
	unsigned int _NUMCOMPUTEUNITS_SLR0 = NUMCOMPUTEUNITS_SLR0; 
	unsigned int _NUMCOMPUTEUNITS_SLR1 = NUMCOMPUTEUNITS_SLR1; 
	unsigned int _NUMCOMPUTEUNITS_SLR2 = NUMCOMPUTEUNITS_SLR2; 
	#endif 
	cout<<"--------------------------- goclkernel::runapp:: _NUMCOMPUTEUNITS_SLR0: "<<_NUMCOMPUTEUNITS_SLR0<<", _NUMCOMPUTEUNITS_SLR1: "<<_NUMCOMPUTEUNITS_SLR1<<", _NUMCOMPUTEUNITS_SLR2: "<<_NUMCOMPUTEUNITS_SLR2<<endl;
	
	unsigned int index_count = 0;
	unsigned int edgessz_kvs = 0;
	unsigned int kvdramsz_kvs = 0;
	unsigned int mdramsz_kvs = 0;
	edgessz_kvs = universalparams.MAXHBMCAPACITY_KVS2; // KVSOURCEDRAMSZ_KVS;  // CRITICAL REMOVEME.
	kvdramsz_kvs = universalparams.MAXHBMCAPACITY_KVS2; // KVSOURCEDRAMSZ_KVS;
	mdramsz_kvs = kvdramsz_kvs;
	
	unsigned int C = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key;
	unsigned int D = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key / VECTOR_SIZE;
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	
	mdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; 
	vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; } // reset
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	cout<<">>> goclkernel::runapp: populating active streaming partitions... "<<endl;					
	uint512_ivec_dt * tempvdram = (uint512_ivec_dt *)vdram;		
	unsigned int vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;	
	unsigned int vdram_SIZE_VERTICESPARTITIONMASK = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESPARTITIONMASK].data[0].key;	
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ 
		for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){
			tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + t].data[iter] = vpmaskbuffer[iter][t];
		}
	}	
	#if defined(_DEBUGMODE_HOSTPRINTS3)
	pmask_dt pmask0[BLOCKRAM_CURRPMASK_SIZE];
	unsigned int offset_kvs = vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK;
	for(unsigned int GraphIter=0; GraphIter<8; GraphIter++){
		print_active_vpartitions(GraphIter, vdram);

	}
	#endif
	#endif 
	// exit(EXIT_SUCCESS);
	
	#ifdef CONFIG_ACTS_HYBRIDLOGIC
	cout<<">>> goclkernel::runapp: populating edges, vptrs and active vertices into mdram... "<<endl;
	unsigned int mdram_BASEOFFSETKVS_EDGESDATA = mdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].data[0].key;
	unsigned int mdram_BASEOFFSETKVS_VERTEXPTR = mdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].data[0].key;
	unsigned int mdram_BASEOFFSETKVS_ACTIVEVERTICES = mdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key;
	unsigned int * temp = (unsigned int *)mdram; 
	for(unsigned int t=0; t<universalparams.NUM_EDGES; t++){ temp[(mdram_BASEOFFSETKVS_EDGESDATA * VECTOR2_SIZE) + t] = edgedatabuffer[t].dstvid; }
	unsigned int index=0; for(unsigned int index=0; index<universalparams.NUM_VERTICES; index++){ temp[(mdram_BASEOFFSETKVS_VERTEXPTR * VECTOR2_SIZE) + index] = vertexptrbuffer[index]; }
	mdram[mdram_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key = 1;
	#endif
	std::vector<int, aligned_allocator<int> > mdram_vec(mdramsz_kvs * sizeof(uint512_vec_dt));
    memcpy(mdram_vec.data(), mdram, mdramsz_kvs * sizeof(uint512_vec_dt));
	cout<<">>> goclkernel::runapp:: edgessz: "<<edgessz_kvs*VECTOR_SIZE<<" (edgessz_kvs: "<<edgessz_kvs*VECTOR_SIZE*sizeof(keyvalue_t)<<"  bytes), kvdramsz: "<<kvdramsz_kvs*VECTOR_SIZE<<" (kvdramsz: "<<kvdramsz_kvs*VECTOR_SIZE*sizeof(keyvalue_t)<<" bytes), NUM_PEs: "<<NUM_PEs<<endl;
	
	uint512_vec_dt * vdramtemp[3];
	vdramtemp[0] = (uint512_vec_dt *) aligned_alloc(4096, (universalparams.MAXHBMCAPACITY_KVS2 * sizeof(uint512_vec_dt)));
	vdramtemp[1] = (uint512_vec_dt *) aligned_alloc(4096, (universalparams.MAXHBMCAPACITY_KVS2 * sizeof(uint512_vec_dt)));
	vdramtemp[2] = (uint512_vec_dt *) aligned_alloc(4096, (universalparams.MAXHBMCAPACITY_KVS2 * sizeof(uint512_vec_dt)));
	for(unsigned int i=0; i<universalparams.MAXHBMCAPACITY_KVS2; i++){ vdramtemp[0][i] = vdram[i]; vdramtemp[1][i] = vdram[i]; vdramtemp[2][i] = vdram[i]; }
	
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
	// OCL_CHECK( // CRITICAL RESTOREME.
        // err,
        // cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err));
    std::string device_name = device.getInfo<CL_DEVICE_NAME>();
    std::cout << "Found Device=" << device_name.c_str() << std::endl;

	auto fileBuf_procandsync = xcl::read_binary_file(binaryFile[0]);

    bool match = true;
	
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	cout<<">>> goclkernel:: running ACTS PROCESS (Switching kernel args)..."<<endl;
	#endif 
	
	// cl_int err;
	std::vector<cl_mem_ext_ptr_t> inoutBufExt_edges[3]; for(unsigned int i=0; i<3; i++){ inoutBufExt_edges[i] = std::vector<cl_mem_ext_ptr_t>(32); }
	std::vector<cl_mem_ext_ptr_t> inoutBufExt_kvdram[3]; for(unsigned int i=0; i<3; i++){ inoutBufExt_kvdram[i] = std::vector<cl_mem_ext_ptr_t>(32); }
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
	unsigned int assigned_kvindex = 0;
	unsigned int edgesindex = 0;
	unsigned int hbmindex = 0;
	unsigned int numI = 0;
	unsigned int maxnumI = 0;
	for(unsigned int s=0; s<3; s+=1){
		if(s==0){ numI = _NUMCOMPUTEUNITS_SLR2; } // SLR 2
		if(s==1){ numI = _NUMCOMPUTEUNITS_SLR1; } // SLR 1
		if(s==2){ numI = _NUMCOMPUTEUNITS_SLR0; } // SLR 0
		
		for(unsigned int i=0; i<numI; i+=1){ 
			#ifdef TESTHWKERNEL
			if(s==0){ assigned_kvindex = 0; } // SLR 2
			if(s==1){ assigned_kvindex = NUMCOMPUTEUNITS_SLR2; } // SLR 1
			if(s==2){ assigned_kvindex = NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1; } // SLR 0
			kvindex = assigned_kvindex;
			#endif 
			
			inoutBufExt_kvdram[s][i].obj = kvsourcedram[kvindex];
			inoutBufExt_kvdram[s][i].param = 0;
			inoutBufExt_kvdram[s][i].flags = bank[hbmindex];
			cout<<"attaching inoutBufExt_kvdram["<<s<<"]["<<i<<"] to {kvsourcedram["<<kvindex<<"], HBM["<<hbmindex<<"]} "<<endl;
			kvindex += 1;
			hbmindex += 1;
		}
		
		inoutBufExt_vdram[s].obj = vdramtemp[s];
		inoutBufExt_vdram[s].param = 0;
		inoutBufExt_vdram[s].flags = bank[hbmindex];
		cout<<"attaching inoutBufExt_vdram["<<s<<"] to vdramtemp to {vdramtemp["<<s<<"], HBM["<<hbmindex<<"]} "<<endl;
		hbmindex += 1;
	}
	
	inoutBufExt_vdram[3].obj = vdram; // mdram_vec.data(); // mdram; // vdram;
	inoutBufExt_vdram[3].param = 0;
	inoutBufExt_vdram[3].flags = bank[hbmindex];
	cout<<"attaching inoutBufExt_vdram[3] to vdram to HBM["<<hbmindex<<"]: "<<endl;
	
	cout<<"goclkernel:: creating OCL buffers (P2)..."<<endl;
	for(unsigned int i=0; i<_NUMCOMPUTEUNITS_SLR2; i++){ 
		OCL_CHECK(err,
			  buffer_kvdram[0][i] =
				  cl::Buffer(context,
							 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
							 sizeof(uint512_vec_dt) * kvdramsz_kvs,
							 &inoutBufExt_kvdram[0][i], 
							 &err));
		cout<<"+++ goclkernel: created buffer_kvdram[0]["<<i<<"] "<<endl;
	}
	
	cout<<"goclkernel:: creating OCL buffers (P1)..."<<endl;
	for(unsigned int i=0; i<_NUMCOMPUTEUNITS_SLR1; i++){ 
		OCL_CHECK(err,
			  buffer_kvdram[1][i] =
				  cl::Buffer(context,
							 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
							 sizeof(uint512_vec_dt) * kvdramsz_kvs,
							 &inoutBufExt_kvdram[1][i], 
							 &err));
		cout<<"+++ goclkernel: created buffer_kvdram[1]["<<i<<"] "<<endl;
	}
	
	cout<<"goclkernel:: creating OCL buffers (P0)..."<<endl;
	for(unsigned int i=0; i<_NUMCOMPUTEUNITS_SLR0; i++){ 
		OCL_CHECK(err,
			  buffer_kvdram[2][i] =
				  cl::Buffer(context,
							 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
							 sizeof(uint512_vec_dt) * kvdramsz_kvs,
							 &inoutBufExt_kvdram[2][i], 
							 &err));
		cout<<"+++ goclkernel: created buffer_kvdram[2]["<<i<<"] "<<endl;
	}
	
	cout<<"goclkernel:: creating OCL buffers (S)..."<<endl;
	for(unsigned int s=0; s<3; s++){
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
							 // CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
							 CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX |
									 CL_MEM_USE_HOST_PTR,
							 // sizeof(uint512_vec_dt) * mdramsz_kvs, // kvdramsz_kvs,
							 sizeof(uint512_vec_dt) * kvdramsz_kvs,
							 // mdram_vec.data(),
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
	std::string krnl_name = "TOPP0_topkernelP"+std::to_string(_NUMCOMPUTEUNITS_SLR2); 
	std::string krnl_name_ = krnl_name + "_";
	std::string cu_id = std::to_string((1));
	std::string krnl2_name_full = krnl_name + ":{" + krnl_name_ + cu_id + "}"; 
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	printf("Creating a kernel [%s] for CU(%d)\n", krnl2_name_full.c_str(), (1));
	#endif
	OCL_CHECK(err, krnls_proc[0] = cl::Kernel(program, krnl2_name_full.c_str(), &err));
	
	krnl_name = "TOPP1_topkernelP"+std::to_string(_NUMCOMPUTEUNITS_SLR1); 
	krnl_name_ = krnl_name + "_";
	cu_id = std::to_string((1));
	std::string krnl1_name_full = krnl_name + ":{" + krnl_name_ + cu_id + "}"; 
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	printf("Creating a kernel [%s] for CU(%d)\n", krnl1_name_full.c_str(), (1));
	#endif
	OCL_CHECK(err, krnls_proc[1] = cl::Kernel(program, krnl1_name_full.c_str(), &err));
	
	krnl_name = "TOPP2_topkernelP"+std::to_string(_NUMCOMPUTEUNITS_SLR0);
	krnl_name_ = krnl_name + "_"; 
	cu_id = std::to_string((1));
	std::string krnl0_name_full = krnl_name + ":{" + krnl_name_ + cu_id + "}"; 
	#ifdef GOCLKERNEL_DEBUGMODE_HOSTPRINTS3
	printf("Creating a kernel [%s] for CU(%d)\n", krnl0_name_full.c_str(), (1));
	#endif
	OCL_CHECK(err, krnls_proc[2] = cl::Kernel(program, krnl0_name_full.c_str(), &err));
	
	krnl_name = "TOPS_topkernelS";
	cu_id = std::to_string((1));
	std::string krnlS_name_full = krnl_name + ":{" + "TOPS_topkernelS_" + cu_id + "}"; 
	OCL_CHECK(err, krnls_sync = cl::Kernel(program, krnlS_name_full.c_str(), &err));

	// Setting kernel arguments (P2)...
	unsigned int argindex = 0;
	for(unsigned int i=0; i<_NUMCOMPUTEUNITS_SLR2; i+=1){
		OCL_CHECK(err, err = krnls_proc[0].setArg(argindex, buffer_kvdram[0][i])); 
		cout<<"setting argument krnls_proc[0].setArg("<<argindex<<", buffer_kvdram[0]["<<i<<"])"<<endl;
		argindex += 1;
	}
	OCL_CHECK(err, err = krnls_proc[0].setArg(argindex, buffer_vdram[0]));
	cout<<"setting argument krnls_proc[0].setArg("<<argindex<<", buffer_vdram[0])"<<endl;
	// exit(EXIT_SUCCESS);
	
	// Setting kernel arguments (P1)...
	argindex = 0;
	for(unsigned int i=0; i<_NUMCOMPUTEUNITS_SLR1; i+=1){
		OCL_CHECK(err, err = krnls_proc[1].setArg(argindex, buffer_kvdram[1][i])); 
		cout<<"setting argument krnls_proc[1].setArg("<<argindex<<", buffer_kvdram[1]["<<i<<"])"<<endl;
		argindex += 1;
	}
	OCL_CHECK(err, err = krnls_proc[1].setArg(argindex, buffer_vdram[1]));
	cout<<"setting argument krnls_proc[1].setArg("<<argindex<<", buffer_vdram[1])"<<endl;
	
	// Setting kernel arguments (P0)...
	argindex = 0;
	for(unsigned int i=0; i<_NUMCOMPUTEUNITS_SLR0; i+=1){
		OCL_CHECK(err, err = krnls_proc[2].setArg(argindex, buffer_kvdram[2][i])); 
		cout<<"setting argument krnls_proc[2].setArg("<<argindex<<", buffer_kvdram[2]["<<i<<"])"<<endl;
		argindex += 1;
	}
	OCL_CHECK(err, err = krnls_proc[2].setArg(argindex, buffer_vdram[2]));
	cout<<"setting argument krnls_proc[2].setArg("<<argindex<<", buffer_vdram[2])"<<endl;
	
	// Setting kernel arguments (S)...
	cout<<"goclkernel:: Setting kernel (sync) arguments..."<<endl;
	for(unsigned int i=0; i<3; i++){ 
		OCL_CHECK(err, err = krnls_sync.setArg(i, buffer_vdram[i]));
		cout<<"setting argument krnls_sync.setArg("<<i<<", buffer_vdram["<<i<<"])"<<endl;
	}
	OCL_CHECK(err, err = krnls_sync.setArg(3, buffer_vdram[3]));
	cout<<"setting argument krnls_sync.setArg(3, buffer_vdram["<<3<<"])"<<endl;
	OCL_CHECK(err, err = q.finish());
	// exit(EXIT_SUCCESS);
	
	// Migrating input data to device global memory...
	cout<<"goclkernel:: Migrating input data to device global memory..."<<endl;
	#ifdef TESTHWKERNEL
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_vdram[0],
													buffer_kvdram[1][0], buffer_vdram[1],
													buffer_kvdram[2][0], buffer_vdram[2],
													buffer_vdram[3]}, 
													0, NULL, &write_events[0]));
	#else 
		#ifdef CONFIG_IMPACT_OF_SCALING_NUMPEs
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2],
															buffer_vdram[3]}, 
															0, NULL, &write_events[0]));
		#else 
			#if NUM_PEs==12
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_vdram[2],
															buffer_vdram[3]}, 
															0, NULL, &write_events[0]));
			#elif NUM_PEs==14
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_vdram[2],
															buffer_vdram[3]}, 
															0, NULL, &write_events[0]));
			#elif NUM_PEs==16
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_vdram[2],
															buffer_vdram[3]}, 
															0, NULL, &write_events[0]));
			#elif NUM_PEs==18
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2],
															buffer_vdram[3]}, 
															0, NULL, &write_events[0]));
			#elif NUM_PEs==20
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2],
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
			// NOT IMPLEMENTED
			#endif
		#endif 
	#endif 
	OCL_CHECK(err, err = write_events[0].wait());
	OCL_CHECK(err, err = q.finish());
	// exit(EXIT_SUCCESS); ////
	
	unsigned int _NUM_KERNELS_LAUNCHED = 3; // 3
	cout<<"goclkernel:: Processing Graph... "<<numValidIters<<" iterations specified"<<endl;
	std::chrono::steady_clock::time_point beginkerneltime1 = std::chrono::steady_clock::now();	
	for(unsigned int GraphIter=0; GraphIter<numValidIters; GraphIter++){ // numValidIters
		cout<< TIMINGRESULTSCOLOR <<">>> goclkernel::runapp: iteration: "<<GraphIter<<" (of "<<numValidIters<<" iterations"<< RESET <<endl;
		std::chrono::steady_clock::time_point iter_starttime = std::chrono::steady_clock::now();
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		print_active_vpartitions(GraphIter, vdram);
		#endif 
		
		// processing edges and applying updates...
		std::cout <<">>> goclkernel: processing edges and applying updates in iteration "<<GraphIter<<"..."<<endl;
		for (unsigned int s = 0; s < _NUM_KERNELS_LAUNCHED; s++){ // 3
			std::cout <<">>> goclkernel: processing krnls_proc["<<s<<"] in iteration "<<GraphIter<<"..."<<endl;
			OCL_CHECK(err, err = q.enqueueTask(krnls_proc[s], NULL, &kernel_events[s])); 
			// OCL_CHECK(err, err = kernel_events[s].wait());
		}
		for (unsigned int s = 0; s < _NUM_KERNELS_LAUNCHED; s++){ OCL_CHECK(err, err = kernel_events[s].wait()); }
		
		// synchronizing...
		// std::cout <<">>> goclkernel: synchronizing vertices in iteration "<<GraphIter<<"..."<<endl;
		// OCL_CHECK(err, err = q.enqueueTask(krnls_sync, NULL, &kernel_events[3])); 
		// OCL_CHECK(err, err = kernel_events[3].wait());
		
		#ifdef CONFIG_ENABLE_PROFILING
		long double iter_timeelapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - iter_starttime).count();
		cout<<">>> goclkernel: iteration: "<<GraphIter<<": Time elapsed: "<<iter_timeelapsed<<"ms, num edges processed: "<<num_edges_processed[GraphIter]<<" edges, throughput: "<<((num_edges_processed[GraphIter] / iter_timeelapsed) * (1000))<<" edges/sec ("<<(((num_edges_processed[GraphIter] / iter_timeelapsed) * (1000)) / 1000000)<<" ME/s)"<<endl;	
		#endif 
	}
	WALLprocandsynkerneltimelapse_ms = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - beginkerneltime1).count()) / 1000;			
	std::cout <<">>> kernel time elapsed for all iterations : "<<WALLprocandsynkerneltimelapse_ms<<" ms, "<<(WALLprocandsynkerneltimelapse_ms * 1000)<<" microsecs, "<<std::endl;
	OCL_CHECK(err, err = q.finish());
	
	cout<<"goclkernel:: migrating workload back to HOST..."<<endl;
	#ifdef TESTHWKERNEL
		OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_vdram[0],
														buffer_kvdram[1][0], buffer_vdram[1],
														buffer_kvdram[2][0], buffer_vdram[2],
														buffer_vdram[3]}, 
														CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
	#else 
		#ifdef CONFIG_IMPACT_OF_SCALING_NUMPEs
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2],
															buffer_vdram[3]}, 
															CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
		#else 
			#if NUM_PEs==12
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_vdram[2],
															buffer_vdram[3]}, 
															CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
			#elif NUM_PEs==14
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_vdram[2],
															buffer_vdram[3]}, 
															CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
			#elif NUM_PEs==16
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_vdram[2],
															buffer_vdram[3]}, 
															CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
			#elif NUM_PEs==18
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2],
															buffer_vdram[3]}, 
															CL_MIGRATE_MEM_OBJECT_HOST, NULL, &read_events[0]));
			#elif NUM_PEs==20
			OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_kvdram[0][0], buffer_kvdram[0][1], buffer_kvdram[0][2], buffer_kvdram[0][3], buffer_kvdram[0][4], buffer_kvdram[0][5], buffer_kvdram[0][6], buffer_vdram[0],
															buffer_kvdram[1][0], buffer_kvdram[1][1], buffer_kvdram[1][2], buffer_kvdram[1][3], buffer_kvdram[1][4], buffer_kvdram[1][5], buffer_kvdram[1][6], buffer_vdram[1],
															buffer_kvdram[2][0], buffer_kvdram[2][1], buffer_kvdram[2][2], buffer_kvdram[2][3], buffer_kvdram[2][4], buffer_kvdram[2][5], buffer_vdram[2],
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
			// NOT IMPLEMENTED
			#endif
		#endif 
	#endif
	OCL_CHECK(err, err = read_events[0].wait());
	OCL_CHECK(err, err = q.finish());
	
	// verifying results
	utilityobj->printallfeedback("goclkernel", "", vdram, vdramtemp[0], vdramtemp[1], vdramtemp[2], kvsourcedram);
	// utilityobj->printallfeedback("app", graph_path, vdram, vdram, vdram, vdram, kvbuffer);

	OCL_CHECK(err, err = read_events[0].wait());
	OCL_CHECK(err, err = q.finish());
	return WALLprocandsynkerneltimelapse_ms;
}
#endif

unsigned int goclkernel::ExitCheck(uint512_vec_dt * kvsourcedram, unsigned int GraphIter){
	unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvsourcedram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	unsigned int BLOP = pow(universalparams.NUM_PARTITIONS, (universalparams.TREE_DEPTH-1));
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






