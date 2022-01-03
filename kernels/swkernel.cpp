#include "swkernel.h"
using namespace std;

#define ENABLE_ACTSPROC
#define ENABLE_ACTSSYNC

swkernel::swkernel(graph * _graphobj, algorithm * _algorithmobj, stats * _statsobj){
	utilityobj = new utility();
	statsobj = _statsobj;
	graphobj = _graphobj;
	algorithmobj = _algorithmobj;
	
	#ifdef SW
	// for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i] = new actsproc(); }
	// for(unsigned int i=0; i<1; i++){ kernelobjs_process[i] = new actsproc(); }
	for(unsigned int i=0; i<1; i++){ kernelobjs_process[i] = new top_nusrcv_nudstv(); }
	kernelobjs_synchronize = new actssync();
	kernelobjs_merge = new acts_merge();
	kernelobjs_synchronize_slicedgraph = new actssync_slicedgraph();
	// kernelobjs = new acts();
	#endif
}
swkernel::~swkernel(){}

#if defined(SW)
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], long double timeelapsed_totals[128][8], unsigned int numValidIters){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; 
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; } // reset
	
	unsigned int hybridmodeoffset_kvs = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_OTHERINFOS].data[0].key; // hybrid-mode enables
	for(unsigned int it=0; it<64; it++){ vdram[hybridmodeoffset_kvs + it].data[0].key = OFF; } 
	// for(unsigned int it=0; it<64; it++){ if(it>=0 && it<64){ vdram[hybridmodeoffset_kvs + it].data[0].key = ON; } else { vdram[hybridmodeoffset_kvs + it].data[0].key = OFF; }} 
	// for(unsigned int it=0; it<64; it++){ vdram[hybridmodeoffset_kvs + it].data[0].key = ON; } // hybrid-mode enables
	
	unsigned int ind = 0;
	unsigned int _PROCESSCOMMAND = ON; unsigned int _PARTITIONCOMMAND = ON; unsigned int _APPLYUPDATESCOMMAND = ON;
	#ifdef ENABLE_KERNEL_PROFILING
	unsigned int analysis_icount = 3;
	#else 
	unsigned int analysis_icount = 1;
	#endif 
	
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
	
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){ // numIters // CRITICAL REMOVEME.
		cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<", numIters: "<<numIters<<endl;
		
		for(unsigned int analysis_i=0; analysis_i<analysis_icount; analysis_i++){
			#ifdef ENABLE_KERNEL_PROFILING
			if(analysis_i==0){ _PROCESSCOMMAND = ON; _PARTITIONCOMMAND = OFF; _APPLYUPDATESCOMMAND = OFF; }
			if(analysis_i==1){ _PROCESSCOMMAND = ON; _PARTITIONCOMMAND = ON; _APPLYUPDATESCOMMAND = OFF; }
			if(analysis_i==2){ _PROCESSCOMMAND = ON; _PARTITIONCOMMAND = ON; _APPLYUPDATESCOMMAND = ON; }
			cout<<"swkernel::runapp: analysis_i: "<<analysis_i<<"(PROCESSCOMMAND:"<<_PROCESSCOMMAND<<", PARTITIONCOMMAND:"<<_PARTITIONCOMMAND<<", APPLYUPDATESCOMMAND:"<<_APPLYUPDATESCOMMAND<<")"<<endl;
			
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
				kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key = ON;
				kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key = _PROCESSCOMMAND; 
				kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key = _PARTITIONCOMMAND; 
				kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key = _APPLYUPDATESCOMMAND;
			}
			#endif 
			
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
			
			kernelobjs_merge->MERGE_mergeVsAcrossSLRs((uint512_dt *)vdramA, (uint512_dt *)vdramB, (uint512_dt *)vdramC);
			exit(EXIT_SUCCESS); //
			
			//////////////////////////////////////////////// FIXME.
			unsigned int _SIZE_SRCVERTICESDATA_D = vdramB[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_SRCVERTICESDATA].data[0].key;
			unsigned int _BASEOFFSETKVS_DESTVERTICESDATA_D = vdramB[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_DESTVERTICESDATA].data[0].key;
			for(unsigned int k=0; k<PADDEDKVSOURCEDRAMSZ_KVS; k++){ 
				vdramB[k] = vdramD[k];
				vdramC[k] = vdramD[k];
			}
			kernelobjs_synchronize->topkernelsync((uint512_dt *)vdramA, (uint512_dt *)vdramB, (uint512_dt *)vdramC, (uint512_dt *)vdram);
			////////////////////////////////////////////////

			// kernelobjs_synchronize->topkernelsync
			/* kernelobjs_synchronize_slicedgraph->topkernelsync
			(
				(uint512_dt *)vdramA,
				(uint512_dt *)vdramB,
				(uint512_dt *)vdramC,
				(uint512_dt *)vdram
			); */
			/* #ifdef CONFIG_UNIFIED_VDRAM
			kernelobjs_synchronize->topkernelsync(
				(uint512_dt *)vdramA,
				(uint512_dt *)vdramB,
				(uint512_dt *)vdramC,
				(uint512_dt *)vdram
			);
			#else 
			kernelobjs_synchronize->topkernelsync(
				(uint512_dt *)kvsourcedram[0],
				#if NUM_PEs>1
				(uint512_dt *)kvsourcedram[1],
				#if NUM_PEs>2
				(uint512_dt *)kvsourcedram[2],
				#if NUM_PEs>3
				(uint512_dt *)kvsourcedram[3],
				#if NUM_PEs>4
				(uint512_dt *)kvsourcedram[4],
				#if NUM_PEs>5
				(uint512_dt *)kvsourcedram[5],
				#if NUM_PEs>6
				(uint512_dt *)kvsourcedram[6],
				#if NUM_PEs>7
				(uint512_dt *)kvsourcedram[7],
				#if NUM_PEs>8
				(uint512_dt *)kvsourcedram[8],
				#if NUM_PEs>9
				(uint512_dt *)kvsourcedram[9],
				#if NUM_PEs>10
				(uint512_dt *)kvsourcedram[10],
				#if NUM_PEs>11
				(uint512_dt *)kvsourcedram[11],
				#if NUM_PEs>12
				(uint512_dt *)kvsourcedram[12],
				#if NUM_PEs>13
				(uint512_dt *)kvsourcedram[13],
				#if NUM_PEs>14
				(uint512_dt *)kvsourcedram[14],
				#if NUM_PEs>15
				(uint512_dt *)kvsourcedram[15],
				#if NUM_PEs>16
				(uint512_dt *)kvsourcedram[16],
				#if NUM_PEs>17
				(uint512_dt *)kvsourcedram[17],
				#if NUM_PEs>18
				(uint512_dt *)kvsourcedram[18],
				#if NUM_PEs>19
				(uint512_dt *)kvsourcedram[19],
				#if NUM_PEs>20
				(uint512_dt *)kvsourcedram[20],
				#if NUM_PEs>21
				(uint512_dt *)kvsourcedram[21],
				#if NUM_PEs>22
				(uint512_dt *)kvsourcedram[22],
				#if NUM_PEs>23
				(uint512_dt *)kvsourcedram[23],
				#if NUM_PEs>24
				(uint512_dt *)kvsourcedram[24],
				#if NUM_PEs>25
				(uint512_dt *)kvsourcedram[25],
				#if NUM_PEs>26
				(uint512_dt *)kvsourcedram[26],
				#if NUM_PEs>27
				(uint512_dt *)kvsourcedram[27],
				#if NUM_PEs>28
				(uint512_dt *)kvsourcedram[28],
				#if NUM_PEs>29
				(uint512_dt *)kvsourcedram[29],
				#if NUM_PEs>30
				(uint512_dt *)kvsourcedram[30],
				#if NUM_PEs>31
				(uint512_dt *)kvsourcedram[31],
				#endif 
				#endif 
				#endif
				#endif
				#endif
				#endif 
				#endif 
				#endif
				#endif
				#endif
				#endif 
				#endif 
				#endif
				#endif
				#endif
				#endif 
				#endif
				#endif 
				#endif
				#endif
				#endif
				#endif 
				#endif 
				#endif
				#endif
				#endif
				#endif 
				#endif
				#endif 
				#endif
				#endif
				(uint512_dt *)vdram
			);
			#endif  */
			
			long double total_time_elapsed_proc = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - beginkerneltime_proc).count();
			cout<<"analysis_i: total_time_elapsed_proc: "<<total_time_elapsed_proc<<"ms"<<endl;
		}
		
		/* #ifdef CONFIG_UNIFIED_VDRAM
		uint512_vec_dt * ref = (uint512_vec_dt *)vdram;
		#else 
		uint512_vec_dt * ref = (uint512_vec_dt *)kvsourcedram[0];	
		#endif  */
		uint512_vec_dt * ref = (uint512_vec_dt *)vdram;
		unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = ref[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
		unsigned int BLOP = pow(NUM_PARTITIONS, (TREE_DEPTH-1));
		unsigned int totalactvvp = 0;
		cout<<endl<<"active partitions for iteration "<<GraphIter+1<<": ";
		for(unsigned int i=0; i<256; i++){
			unsigned int gmask = ref[_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].data[0].key;
			totalactvvp += gmask;
			if(gmask > 0){ cout<<i<<", "; }
		}
		cout<<""<<endl;
		if(totalactvvp == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
	}
	
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











