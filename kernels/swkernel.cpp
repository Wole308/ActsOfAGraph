#include "swkernel.h"
using namespace std;

#define ENABLE_ACTSPROC
#define ENABLE_ACTSSYNC

swkernel::swkernel(graph * _graphobj, algorithm * _algorithmobj, stats * _statsobj){
	#ifndef HW
	utilityobj = new utility();
	statsobj = _statsobj;
	graphobj = _graphobj;
	algorithmobj = _algorithmobj;
	mydebugobj = new mydebug();
	
	for(unsigned int i=0; i<1; i++){ kernelobjs_process[i] = new acts_all(mydebugobj); }
	#endif 
}
swkernel::~swkernel(){}

#ifndef HW
void swkernel::verifyresults(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], unsigned int id){
	unsigned int vdatas[64]; for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int _BASEOFFSETKVS_DESTVERTICESDATA = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA].data[0].key;
		for(unsigned int k=0; k<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				unsigned int vdata1 = kvbuffer[i][_BASEOFFSETKVS_DESTVERTICESDATA + k].data[v].key;
				unsigned int vdata2 = kvbuffer[i][_BASEOFFSETKVS_DESTVERTICESDATA + k].data[v].value;
				if(vdata1 < 64){
					vdatas[vdata1] += 1; 
				}
				if(vdata2 < 64){
					vdatas[vdata2] += 1; 
				}
			}
		}
	}
	cout<<">>> swkernel::verifyresults:: Printing results. id: "<<id<<endl;
	utilityobj->printvalues("swkernel::verifyresults:: verifying results (vdatas)", vdatas, 16);
}

long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], long double timeelapsed_totals[128][8], unsigned int numValidIters){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; 
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; } // reset
	
	unsigned int hybridmodeoffset_kvs = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_OTHERINFOS].data[0].key; // hybrid-mode enables
	for(unsigned int it=0; it<64; it++){ vdram[hybridmodeoffset_kvs + it].data[0].key = OFF; }
	
	unsigned int ind = 0;
	unsigned int _PROCESSCOMMAND = ON; unsigned int _PARTITIONCOMMAND = ON; unsigned int _APPLYUPDATESCOMMAND = ON;
	
	#if NUM_EDGE_BANKS>0
	cout<< TIMINGRESULTSCOLOR <<">>> swkernel::runapp: ACTS started. Parameters: NUM_EDGE_BANKS_PER_PROC: ["<<NUM_EDGE_BANKS<<"], NUMPROCTHREADS: ["<<(NUMSUBCPUTHREADS / NUM_EDGE_BANKS)<<"], TOTAL_NUM_BANKS_DEPLOYED: ["<<(NUMSUBCPUTHREADS + (NUMSUBCPUTHREADS / NUM_EDGE_BANKS))<<"]"<< RESET <<endl;							
	#else 
	cout<< TIMINGRESULTSCOLOR <<">>> swkernel::runapp: ACTS started. Parameters: NUM_EDGE_BANKS_PER_PROC: [1], NUMPROCTHREADS: ["<<(NUMSUBCPUTHREADS / 1)<<"], TOTAL_NUM_BANKS_DEPLOYED: ["<<(NUMSUBCPUTHREADS + (NUMSUBCPUTHREADS / 1))<<"]"<< RESET <<endl;		
	#endif 
	cout<< TIMINGRESULTSCOLOR <<">>> swkernel::runapp: ACTS started. Parameters: NUMSUBCPUTHREADS: ["<<NUMSUBCPUTHREADS<<"], NUMSYNCTHREADS: ["<<NUMSYNCTHREADS<<"]"<< RESET <<endl;	
	
	uint512_vec_dt * vdramA; // NEWCHANGE.
	uint512_vec_dt * vdramB; 
	uint512_vec_dt * vdramC; 
	uint512_vec_dt * vdramD; 
	#ifndef SW_IMPL 
	#ifdef FPGA_IMPL			
	vdramA = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	vdramB = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	vdramC = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	vdramD = (uint512_vec_dt *) aligned_alloc(4096, (PADDEDKVSOURCEDRAMSZ_KVS * sizeof(uint512_vec_dt)));
	#else
	vdramA = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	vdramB = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	vdramC = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	vdramD = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	#endif
	#endif
	for(unsigned int i=0; i<PADDEDKVSOURCEDRAMSZ_KVS; i++){ vdramA[i] = vdram[i]; vdramB[i] = vdram[i]; vdramC[i] = vdram[i]; vdramD[i] = vdram[i]; }

	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<< TIMINGRESULTSCOLOR <<">>> swkernel::runapp: Iteration: "<<GraphIter<<" (of "<<numIters<<" iterations"<< RESET <<endl;
		std::chrono::steady_clock::time_point beginkerneltime_proc = std::chrono::steady_clock::now();

		#if NUM_PEs==3
		run3(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
		#elif NUM_PEs==12
		run12(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
		#elif NUM_PEs==22
		run22(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
		#elif NUM_PEs==24
		run24(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
		#elif NUM_PEs==25
		run25(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
		#elif NUM_PEs==32
		run32(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
		#else 
		NOT DEFINED.
		#endif
		
		kernelobjs_process[0]->topkernelS((uint512_dt *)vdramA, (uint512_dt *)vdramB, (uint512_dt *)vdramC, (uint512_dt *)vdram);	
		
		long double total_time_elapsed_proc = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - beginkerneltime_proc).count();
		cout<<"analysis_i: total_time_elapsed_proc: "<<total_time_elapsed_proc<<"ms"<<endl;
		
		#ifdef CONFIG_SPLIT_DESTVTXS
		uint512_vec_dt * ref = (uint512_vec_dt *)vdramA;
		#else 
		uint512_vec_dt * ref = (uint512_vec_dt *)vdram;	
		#endif 
		unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = ref[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
		unsigned int totalactvvp = 0;
		cout<<endl<<"active partitions for iteration "<<GraphIter+1<<": ";
		for(unsigned int i=0; i<256; i++){
			unsigned int gmask = ref[_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].data[0].key;
			totalactvvp += gmask;
			if(gmask > 0){ cout<<i<<", "; }
		}
		cout<<""<<endl;
		
		verifyresults(kvsourcedram, 0);
		
		// if(totalactvvp == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
		// exit(EXIT_SUCCESS); //
	}
	
	verifyresults(kvsourcedram, 1);

	#ifdef CONFIG_SPLIT_DESTVTXS
	for(unsigned int i=0; i<PADDEDKVSOURCEDRAMSZ_KVS; i++){ vdram[i] = vdramA[i]; }
	#endif
	
	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(total_time_elapsed);
	#endif
	return total_time_elapsed;
}

void swkernel::run3(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	cout<<"------------------------------------ topkernelP1: processing P1 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP1(
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)vdramA
	);
	#else 
	kernelobjs_process[0]->topkernelP1(
		(uint512_dt *)edges[0],
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)vdramA
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P1 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP1(
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)vdramB
	);
	#else 
	kernelobjs_process[0]->topkernelP1(
		(uint512_dt *)edges[1],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)vdramB
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P1 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP1(
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)vdramC
	);
	#else 
	kernelobjs_process[0]->topkernelP1(
		(uint512_dt *)edges[2],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)vdramC
	);
	#endif
}
void swkernel::run12(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	unsigned int A_OFFSET = 0;
	unsigned int B_OFFSET = 5;
	unsigned int C_OFFSET = 10;
	
	cout<<"------------------------------------ topkernelP1: processing P5 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP5(
		(uint512_dt *)kvsourcedram[A_OFFSET + 0],
		(uint512_dt *)kvsourcedram[A_OFFSET + 1],
		(uint512_dt *)kvsourcedram[A_OFFSET + 2],
		(uint512_dt *)kvsourcedram[A_OFFSET + 3],
		(uint512_dt *)kvsourcedram[A_OFFSET + 4],
		(uint512_dt *)vdramA
	);
	#else 
	kernelobjs_process[0]->topkernelP5(
		(uint512_dt *)edges[A_OFFSET + 0],
		(uint512_dt *)kvsourcedram[A_OFFSET + 0],
		(uint512_dt *)edges[A_OFFSET + 1],
		(uint512_dt *)kvsourcedram[A_OFFSET + 1],
		(uint512_dt *)edges[A_OFFSET + 2],
		(uint512_dt *)kvsourcedram[A_OFFSET + 2],
		(uint512_dt *)edges[A_OFFSET + 3],
		(uint512_dt *)kvsourcedram[A_OFFSET + 3],
		(uint512_dt *)edges[A_OFFSET + 4],
		(uint512_dt *)kvsourcedram[A_OFFSET + 4],
		(uint512_dt *)vdramA
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P5 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP5(
		(uint512_dt *)kvsourcedram[B_OFFSET + 0],
		(uint512_dt *)kvsourcedram[B_OFFSET + 1],
		(uint512_dt *)kvsourcedram[B_OFFSET + 2],
		(uint512_dt *)kvsourcedram[B_OFFSET + 3],
		(uint512_dt *)kvsourcedram[B_OFFSET + 4],
		(uint512_dt *)vdramB
	);
	#else 
	kernelobjs_process[0]->topkernelP5(
		(uint512_dt *)edges[B_OFFSET + 0],
		(uint512_dt *)kvsourcedram[B_OFFSET + 0],
		(uint512_dt *)edges[B_OFFSET + 1],
		(uint512_dt *)kvsourcedram[B_OFFSET + 1],
		(uint512_dt *)edges[B_OFFSET + 2],
		(uint512_dt *)kvsourcedram[B_OFFSET + 2],
		(uint512_dt *)edges[B_OFFSET + 3],
		(uint512_dt *)kvsourcedram[B_OFFSET + 3],
		(uint512_dt *)edges[B_OFFSET + 4],
		(uint512_dt *)kvsourcedram[B_OFFSET + 4],
		(uint512_dt *)vdramB
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P5 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP2(
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)vdramC
	);
	#else 
	kernelobjs_process[0]->topkernelP2(
		(uint512_dt *)edges[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)edges[C_OFFSET + 1],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)vdramC
	);
	#endif
}
void swkernel::run22(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	cout<<"------------------------------------ topkernelP1: processing P8 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP8(
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)kvsourcedram[3],
		(uint512_dt *)kvsourcedram[4],
		(uint512_dt *)kvsourcedram[5],
		(uint512_dt *)kvsourcedram[6],
		(uint512_dt *)kvsourcedram[7],
		(uint512_dt *)vdramA
	);
	#else 
	kernelobjs_process[0]->topkernelP8(
		(uint512_dt *)edges[0],
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)edges[1],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)edges[2],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)edges[3],
		(uint512_dt *)kvsourcedram[3],
		(uint512_dt *)edges[4],
		(uint512_dt *)kvsourcedram[4],
		(uint512_dt *)edges[5],
		(uint512_dt *)kvsourcedram[5],
		(uint512_dt *)edges[6],
		(uint512_dt *)kvsourcedram[6],
		(uint512_dt *)edges[7],
		(uint512_dt *)kvsourcedram[7],
		(uint512_dt *)vdramA
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P8 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP8(
		(uint512_dt *)kvsourcedram[8],
		(uint512_dt *)kvsourcedram[9],
		(uint512_dt *)kvsourcedram[10],
		(uint512_dt *)kvsourcedram[11],
		(uint512_dt *)kvsourcedram[12],
		(uint512_dt *)kvsourcedram[13],
		(uint512_dt *)kvsourcedram[14],
		(uint512_dt *)kvsourcedram[15],
		(uint512_dt *)vdramB
	);
	#else 
	kernelobjs_process[0]->topkernelP8(
		(uint512_dt *)edges[8],
		(uint512_dt *)kvsourcedram[8],
		(uint512_dt *)edges[9],
		(uint512_dt *)kvsourcedram[9],
		(uint512_dt *)edges[10],
		(uint512_dt *)kvsourcedram[10],
		(uint512_dt *)edges[11],
		(uint512_dt *)kvsourcedram[11],
		(uint512_dt *)edges[12],
		(uint512_dt *)kvsourcedram[12],
		(uint512_dt *)edges[13],
		(uint512_dt *)kvsourcedram[13],
		(uint512_dt *)edges[14],
		(uint512_dt *)kvsourcedram[14],
		(uint512_dt *)edges[15],
		(uint512_dt *)kvsourcedram[15],
		(uint512_dt *)vdramB
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P6 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP6(
		(uint512_dt *)kvsourcedram[16],
		(uint512_dt *)kvsourcedram[17],
		(uint512_dt *)kvsourcedram[18],
		(uint512_dt *)kvsourcedram[19],
		(uint512_dt *)kvsourcedram[20],
		(uint512_dt *)kvsourcedram[21],
		(uint512_dt *)vdramC
	);
	#else 
	kernelobjs_process[0]->topkernelP6(
		(uint512_dt *)edges[16],
		(uint512_dt *)kvsourcedram[16],
		(uint512_dt *)edges[17],
		(uint512_dt *)kvsourcedram[17],
		(uint512_dt *)edges[18],
		(uint512_dt *)kvsourcedram[18],
		(uint512_dt *)edges[19],
		(uint512_dt *)kvsourcedram[19],
		(uint512_dt *)edges[20],
		(uint512_dt *)kvsourcedram[20],
		(uint512_dt *)edges[21],
		(uint512_dt *)kvsourcedram[21],
		(uint512_dt *)vdramC
	);
	#endif
}
void swkernel::run24(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	unsigned int A_OFFSET = 0;
	unsigned int B_OFFSET = 9;
	unsigned int C_OFFSET = 18;
	
	cout<<"------------------------------------ topkernelP1: processing P9 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP9(
		(uint512_dt *)kvsourcedram[A_OFFSET + 0],
		(uint512_dt *)kvsourcedram[A_OFFSET + 1],
		(uint512_dt *)kvsourcedram[A_OFFSET + 2],
		(uint512_dt *)kvsourcedram[A_OFFSET + 3],
		(uint512_dt *)kvsourcedram[A_OFFSET + 4],
		(uint512_dt *)kvsourcedram[A_OFFSET + 5],
		(uint512_dt *)kvsourcedram[A_OFFSET + 6],
		(uint512_dt *)kvsourcedram[A_OFFSET + 7],
		(uint512_dt *)kvsourcedram[A_OFFSET + 8],
		(uint512_dt *)vdramA
	);
	#else 
	kernelobjs_process[0]->topkernelP9(
		(uint512_dt *)edges[A_OFFSET + 0],
		(uint512_dt *)kvsourcedram[A_OFFSET + 0],
		(uint512_dt *)edges[A_OFFSET + 1],
		(uint512_dt *)kvsourcedram[A_OFFSET + 1],
		(uint512_dt *)edges[A_OFFSET + 2],
		(uint512_dt *)kvsourcedram[A_OFFSET + 2],
		(uint512_dt *)edges[A_OFFSET + 3],
		(uint512_dt *)kvsourcedram[A_OFFSET + 3],
		(uint512_dt *)edges[A_OFFSET + 4],
		(uint512_dt *)kvsourcedram[A_OFFSET + 4],
		(uint512_dt *)edges[A_OFFSET + 5],
		(uint512_dt *)kvsourcedram[A_OFFSET + 5],
		(uint512_dt *)edges[A_OFFSET + 6],
		(uint512_dt *)kvsourcedram[A_OFFSET + 6],
		(uint512_dt *)edges[A_OFFSET + 7],
		(uint512_dt *)kvsourcedram[A_OFFSET + 7],
		(uint512_dt *)edges[A_OFFSET + 8],
		(uint512_dt *)kvsourcedram[A_OFFSET + 8],
		(uint512_dt *)vdramA
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P9 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP9(
		(uint512_dt *)kvsourcedram[B_OFFSET + 0],
		(uint512_dt *)kvsourcedram[B_OFFSET + 1],
		(uint512_dt *)kvsourcedram[B_OFFSET + 2],
		(uint512_dt *)kvsourcedram[B_OFFSET + 3],
		(uint512_dt *)kvsourcedram[B_OFFSET + 4],
		(uint512_dt *)kvsourcedram[B_OFFSET + 5],
		(uint512_dt *)kvsourcedram[B_OFFSET + 6],
		(uint512_dt *)kvsourcedram[B_OFFSET + 7],
		(uint512_dt *)kvsourcedram[B_OFFSET + 8],
		(uint512_dt *)vdramB
	);
	#else 
	kernelobjs_process[0]->topkernelP9(
		(uint512_dt *)edges[B_OFFSET + 0],
		(uint512_dt *)kvsourcedram[B_OFFSET + 0],
		(uint512_dt *)edges[B_OFFSET + 1],
		(uint512_dt *)kvsourcedram[B_OFFSET + 1],
		(uint512_dt *)edges[B_OFFSET + 2],
		(uint512_dt *)kvsourcedram[B_OFFSET + 2],
		(uint512_dt *)edges[B_OFFSET + 3],
		(uint512_dt *)kvsourcedram[B_OFFSET + 3],
		(uint512_dt *)edges[B_OFFSET + 4],
		(uint512_dt *)kvsourcedram[B_OFFSET + 4],
		(uint512_dt *)edges[B_OFFSET + 5],
		(uint512_dt *)kvsourcedram[B_OFFSET + 5],
		(uint512_dt *)edges[B_OFFSET + 6],
		(uint512_dt *)kvsourcedram[B_OFFSET + 6],
		(uint512_dt *)edges[B_OFFSET + 7],
		(uint512_dt *)kvsourcedram[B_OFFSET + 7],
		(uint512_dt *)edges[B_OFFSET + 8],
		(uint512_dt *)kvsourcedram[B_OFFSET + 8],
		(uint512_dt *)vdramB
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P6 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP6(
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)kvsourcedram[C_OFFSET + 2],
		(uint512_dt *)kvsourcedram[C_OFFSET + 3],
		(uint512_dt *)kvsourcedram[C_OFFSET + 4],
		(uint512_dt *)kvsourcedram[C_OFFSET + 5],
		(uint512_dt *)vdramC
	);
	#else 
	kernelobjs_process[0]->topkernelP6(
		(uint512_dt *)edges[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)edges[C_OFFSET + 1],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)edges[C_OFFSET + 2],
		(uint512_dt *)kvsourcedram[C_OFFSET + 2],
		(uint512_dt *)edges[C_OFFSET + 3],
		(uint512_dt *)kvsourcedram[C_OFFSET + 3],
		(uint512_dt *)edges[C_OFFSET + 4],
		(uint512_dt *)kvsourcedram[C_OFFSET + 4],
		(uint512_dt *)edges[C_OFFSET + 5],
		(uint512_dt *)kvsourcedram[C_OFFSET + 5],
		(uint512_dt *)vdramC
	);
	#endif
}
void swkernel::run25(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	unsigned int A_OFFSET = 0;
	unsigned int B_OFFSET = 9;
	unsigned int C_OFFSET = 18;
	
	cout<<"------------------------------------ topkernelP1: processing P9 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP9(
		(uint512_dt *)kvsourcedram[A_OFFSET + 0],
		(uint512_dt *)kvsourcedram[A_OFFSET + 1],
		(uint512_dt *)kvsourcedram[A_OFFSET + 2],
		(uint512_dt *)kvsourcedram[A_OFFSET + 3],
		(uint512_dt *)kvsourcedram[A_OFFSET + 4],
		(uint512_dt *)kvsourcedram[A_OFFSET + 5],
		(uint512_dt *)kvsourcedram[A_OFFSET + 6],
		(uint512_dt *)kvsourcedram[A_OFFSET + 7],
		(uint512_dt *)kvsourcedram[A_OFFSET + 8],
		(uint512_dt *)vdramA
	);
	#else 
	kernelobjs_process[0]->topkernelP9(
		(uint512_dt *)edges[A_OFFSET + 0],
		(uint512_dt *)kvsourcedram[A_OFFSET + 0],
		(uint512_dt *)edges[A_OFFSET + 1],
		(uint512_dt *)kvsourcedram[A_OFFSET + 1],
		(uint512_dt *)edges[A_OFFSET + 2],
		(uint512_dt *)kvsourcedram[A_OFFSET + 2],
		(uint512_dt *)edges[A_OFFSET + 3],
		(uint512_dt *)kvsourcedram[A_OFFSET + 3],
		(uint512_dt *)edges[A_OFFSET + 4],
		(uint512_dt *)kvsourcedram[A_OFFSET + 4],
		(uint512_dt *)edges[A_OFFSET + 5],
		(uint512_dt *)kvsourcedram[A_OFFSET + 5],
		(uint512_dt *)edges[A_OFFSET + 6],
		(uint512_dt *)kvsourcedram[A_OFFSET + 6],
		(uint512_dt *)edges[A_OFFSET + 7],
		(uint512_dt *)kvsourcedram[A_OFFSET + 7],
		(uint512_dt *)edges[A_OFFSET + 8],
		(uint512_dt *)kvsourcedram[A_OFFSET + 8],
		(uint512_dt *)vdramA
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P9 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP9(
		(uint512_dt *)kvsourcedram[B_OFFSET + 0],
		(uint512_dt *)kvsourcedram[B_OFFSET + 1],
		(uint512_dt *)kvsourcedram[B_OFFSET + 2],
		(uint512_dt *)kvsourcedram[B_OFFSET + 3],
		(uint512_dt *)kvsourcedram[B_OFFSET + 4],
		(uint512_dt *)kvsourcedram[B_OFFSET + 5],
		(uint512_dt *)kvsourcedram[B_OFFSET + 6],
		(uint512_dt *)kvsourcedram[B_OFFSET + 7],
		(uint512_dt *)kvsourcedram[B_OFFSET + 8],
		(uint512_dt *)vdramB
	);
	#else 
	kernelobjs_process[0]->topkernelP9(
		(uint512_dt *)edges[B_OFFSET + 0],
		(uint512_dt *)kvsourcedram[B_OFFSET + 0],
		(uint512_dt *)edges[B_OFFSET + 1],
		(uint512_dt *)kvsourcedram[B_OFFSET + 1],
		(uint512_dt *)edges[B_OFFSET + 2],
		(uint512_dt *)kvsourcedram[B_OFFSET + 2],
		(uint512_dt *)edges[B_OFFSET + 3],
		(uint512_dt *)kvsourcedram[B_OFFSET + 3],
		(uint512_dt *)edges[B_OFFSET + 4],
		(uint512_dt *)kvsourcedram[B_OFFSET + 4],
		(uint512_dt *)edges[B_OFFSET + 5],
		(uint512_dt *)kvsourcedram[B_OFFSET + 5],
		(uint512_dt *)edges[B_OFFSET + 6],
		(uint512_dt *)kvsourcedram[B_OFFSET + 6],
		(uint512_dt *)edges[B_OFFSET + 7],
		(uint512_dt *)kvsourcedram[B_OFFSET + 7],
		(uint512_dt *)edges[B_OFFSET + 8],
		(uint512_dt *)kvsourcedram[B_OFFSET + 8],
		(uint512_dt *)vdramB
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P7 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP7(
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)kvsourcedram[C_OFFSET + 2],
		(uint512_dt *)kvsourcedram[C_OFFSET + 3],
		(uint512_dt *)kvsourcedram[C_OFFSET + 4],
		(uint512_dt *)kvsourcedram[C_OFFSET + 5],
		(uint512_dt *)kvsourcedram[C_OFFSET + 6],
		(uint512_dt *)vdramC
	);
	#else 
	kernelobjs_process[0]->topkernelP7(
		(uint512_dt *)edges[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)edges[C_OFFSET + 1],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)edges[C_OFFSET + 2],
		(uint512_dt *)kvsourcedram[C_OFFSET + 2],
		(uint512_dt *)edges[C_OFFSET + 3],
		(uint512_dt *)kvsourcedram[C_OFFSET + 3],
		(uint512_dt *)edges[C_OFFSET + 4],
		(uint512_dt *)kvsourcedram[C_OFFSET + 4],
		(uint512_dt *)edges[C_OFFSET + 5],
		(uint512_dt *)kvsourcedram[C_OFFSET + 5],
		(uint512_dt *)edges[C_OFFSET + 6],
		(uint512_dt *)kvsourcedram[C_OFFSET + 6],
		(uint512_dt *)vdramC
	);
	#endif
}
void swkernel::run32(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	unsigned int A_OFFSET = 0;
	unsigned int B_OFFSET = 10;
	unsigned int C_OFFSET = 20;
	
	// #ifdef XXX // CRITICAL REMOVEME.
	cout<<"------------------------------------ topkernelP1: processing P10 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP10(
		(uint512_dt *)kvsourcedram[A_OFFSET + 0],
		(uint512_dt *)kvsourcedram[A_OFFSET + 1],
		(uint512_dt *)kvsourcedram[A_OFFSET + 2],
		(uint512_dt *)kvsourcedram[A_OFFSET + 3],
		(uint512_dt *)kvsourcedram[A_OFFSET + 4],
		(uint512_dt *)kvsourcedram[A_OFFSET + 5],
		(uint512_dt *)kvsourcedram[A_OFFSET + 6],
		(uint512_dt *)kvsourcedram[A_OFFSET + 7],
		(uint512_dt *)kvsourcedram[A_OFFSET + 8],
		(uint512_dt *)kvsourcedram[A_OFFSET + 9],
		(uint512_dt *)vdramA
	);
	#else 
	kernelobjs_process[0]->topkernelP10(
		(uint512_dt *)edges[A_OFFSET + 0],
		(uint512_dt *)kvsourcedram[A_OFFSET + 0],
		(uint512_dt *)edges[A_OFFSET + 1],
		(uint512_dt *)kvsourcedram[A_OFFSET + 1],
		(uint512_dt *)edges[A_OFFSET + 2],
		(uint512_dt *)kvsourcedram[A_OFFSET + 2],
		(uint512_dt *)edges[A_OFFSET + 3],
		(uint512_dt *)kvsourcedram[A_OFFSET + 3],
		(uint512_dt *)edges[A_OFFSET + 4],
		(uint512_dt *)kvsourcedram[A_OFFSET + 4],
		(uint512_dt *)edges[A_OFFSET + 5],
		(uint512_dt *)kvsourcedram[A_OFFSET + 5],
		(uint512_dt *)edges[A_OFFSET + 6],
		(uint512_dt *)kvsourcedram[A_OFFSET + 6],
		(uint512_dt *)edges[A_OFFSET + 7],
		(uint512_dt *)kvsourcedram[A_OFFSET + 7],
		(uint512_dt *)edges[A_OFFSET + 8],
		(uint512_dt *)kvsourcedram[A_OFFSET + 8],
		(uint512_dt *)edges[A_OFFSET + 9],
		(uint512_dt *)kvsourcedram[A_OFFSET + 9],
		(uint512_dt *)vdramA
	);
	#endif 
	
	cout<<"------------------------------------ topkernelP1: processing P10 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP10(
		(uint512_dt *)kvsourcedram[B_OFFSET + 0],
		(uint512_dt *)kvsourcedram[B_OFFSET + 1],
		(uint512_dt *)kvsourcedram[B_OFFSET + 2],
		(uint512_dt *)kvsourcedram[B_OFFSET + 3],
		(uint512_dt *)kvsourcedram[B_OFFSET + 4],
		(uint512_dt *)kvsourcedram[B_OFFSET + 5],
		(uint512_dt *)kvsourcedram[B_OFFSET + 6],
		(uint512_dt *)kvsourcedram[B_OFFSET + 7],
		(uint512_dt *)kvsourcedram[B_OFFSET + 8],
		(uint512_dt *)kvsourcedram[B_OFFSET + 9],
		(uint512_dt *)vdramB
	);
	#else 
	kernelobjs_process[0]->topkernelP10(
		(uint512_dt *)edges[B_OFFSET + 0],
		(uint512_dt *)kvsourcedram[B_OFFSET + 0],
		(uint512_dt *)edges[B_OFFSET + 1],
		(uint512_dt *)kvsourcedram[B_OFFSET + 1],
		(uint512_dt *)edges[B_OFFSET + 2],
		(uint512_dt *)kvsourcedram[B_OFFSET + 2],
		(uint512_dt *)edges[B_OFFSET + 3],
		(uint512_dt *)kvsourcedram[B_OFFSET + 3],
		(uint512_dt *)edges[B_OFFSET + 4],
		(uint512_dt *)kvsourcedram[B_OFFSET + 4],
		(uint512_dt *)edges[B_OFFSET + 5],
		(uint512_dt *)kvsourcedram[B_OFFSET + 5],
		(uint512_dt *)edges[B_OFFSET + 6],
		(uint512_dt *)kvsourcedram[B_OFFSET + 6],
		(uint512_dt *)edges[B_OFFSET + 7],
		(uint512_dt *)kvsourcedram[B_OFFSET + 7],
		(uint512_dt *)edges[B_OFFSET + 8],
		(uint512_dt *)kvsourcedram[B_OFFSET + 8],
		(uint512_dt *)edges[B_OFFSET + 9],
		(uint512_dt *)kvsourcedram[B_OFFSET + 9],
		(uint512_dt *)vdramB
	);
	#endif 
	// #endif 
	
	cout<<"------------------------------------ topkernelP1: processing P12 instances ------------------------------------"<<endl;
	#if NUM_EDGE_BANKS==0
	kernelobjs_process[0]->topkernelP12(
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)kvsourcedram[C_OFFSET + 2],
		(uint512_dt *)kvsourcedram[C_OFFSET + 3],
		(uint512_dt *)kvsourcedram[C_OFFSET + 4],
		(uint512_dt *)kvsourcedram[C_OFFSET + 5],
		(uint512_dt *)kvsourcedram[C_OFFSET + 6],
		(uint512_dt *)kvsourcedram[C_OFFSET + 7],
		(uint512_dt *)kvsourcedram[C_OFFSET + 8],
		(uint512_dt *)kvsourcedram[C_OFFSET + 9],
		(uint512_dt *)kvsourcedram[C_OFFSET + 10],
		(uint512_dt *)kvsourcedram[C_OFFSET + 11],
		(uint512_dt *)vdramC
	);
	#else 
	kernelobjs_process[0]->topkernelP12(
		(uint512_dt *)edges[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)edges[C_OFFSET + 1],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)edges[C_OFFSET + 2],
		(uint512_dt *)kvsourcedram[C_OFFSET + 2],
		(uint512_dt *)edges[C_OFFSET + 3],
		(uint512_dt *)kvsourcedram[C_OFFSET + 3],
		(uint512_dt *)edges[C_OFFSET + 4],
		(uint512_dt *)kvsourcedram[C_OFFSET + 4],
		(uint512_dt *)edges[C_OFFSET + 5],
		(uint512_dt *)kvsourcedram[C_OFFSET + 5],
		(uint512_dt *)edges[C_OFFSET + 6],
		(uint512_dt *)kvsourcedram[C_OFFSET + 6],
		(uint512_dt *)edges[C_OFFSET + 7],
		(uint512_dt *)kvsourcedram[C_OFFSET + 7],
		(uint512_dt *)edges[C_OFFSET + 8],
		(uint512_dt *)kvsourcedram[C_OFFSET + 8],
		(uint512_dt *)edges[C_OFFSET + 9],
		(uint512_dt *)kvsourcedram[C_OFFSET + 9],
		(uint512_dt *)edges[C_OFFSET + 10],
		(uint512_dt *)kvsourcedram[C_OFFSET + 10],
		(uint512_dt *)edges[C_OFFSET + 11],
		(uint512_dt *)kvsourcedram[C_OFFSET + 11],
		(uint512_dt *)vdramC
	);
	#endif 
}
#endif 











