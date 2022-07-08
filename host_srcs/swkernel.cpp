#include "swkernel.h"
using namespace std;

#define ENABLE_ACTSPROC
#define ENABLE_ACTSSYNC

swkernel::swkernel(universalparams_t _universalparams){
	#ifndef FPGA_IMPL
	utilityobj = new utility(_universalparams);
	mydebugobj = new mydebug();
	myuniversalparams = _universalparams;
	
	for(unsigned int i=0; i<1; i++){ kernelobjs_process[i] = new acts_all(mydebugobj); }
	#endif 
}
swkernel::~swkernel(){}

#ifndef FPGA_IMPL
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs], long double timeelapsed_totals[128][8], 
		unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS], vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, universalparams_t universalparams){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	unsigned int beginIter = 0;
	vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = beginIter; 
	for(unsigned int i=0; i<NUM_PEs; i++){ kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = beginIter; } // reset
	
	unsigned int ind = 0;
	unsigned int _PROCESSCOMMAND = ON; unsigned int _PARTITIONCOMMAND = ON; unsigned int _APPLYUPDATESCOMMAND = ON;
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<< TIMINGRESULTSCOLOR <<">>> swkernel::runapp: ACTS started. Parameters: NUM_PEs: ["<<NUM_PEs<<"]"<< RESET <<endl;	
	#endif 
	
	uint512_vec_dt * vdramA; vdramA = new uint512_vec_dt[universalparams.MAXHBMCAPACITY_KVS2];
	uint512_vec_dt * vdramB; vdramB = new uint512_vec_dt[universalparams.MAXHBMCAPACITY_KVS2];
	uint512_vec_dt * vdramC; vdramC = new uint512_vec_dt[universalparams.MAXHBMCAPACITY_KVS2];
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<">>> swkernel::runapp: populating vdramA, vdramB and vdramC... "<<endl;
	#endif 
	for(unsigned int i=0; i<universalparams.MAXHBMCAPACITY_KVS2; i++){ vdramA[i] = vdram[i]; vdramB[i] = vdram[i]; vdramC[i] = vdram[i]; }
	
	unsigned int mode = 1;

	for(unsigned int GraphIter=beginIter; GraphIter<universalparams.NUM_ITERATIONS; GraphIter++){ 
		#ifdef _DEBUGMODE_HOSTPRINTS4
		cout<< endl << TIMINGRESULTSCOLOR <<">>> swkernel::runapp: Iteration: "<<GraphIter<<" (of "<<universalparams.NUM_ITERATIONS<<" iterations)"<< RESET <<endl;
		#endif 
		std::chrono::steady_clock::time_point beginkerneltime_proc = std::chrono::steady_clock::now();
		
		#ifdef TESTKERNEL
			// run3(vdramA, vdramB, vdramC, edges, kvsourcedram);
			run24(vdramA, vdramB, vdramC, edges, kvsourcedram);
		#else 
			#if NUM_PEs==3
			run3(vdramA, vdramB, vdramC, edges, kvsourcedram);
			#elif NUM_PEs==12
			run12(vdramA, vdramB, vdramC, edges, kvsourcedram);
			#elif NUM_PEs==16
			run16(vdramA, vdramB, vdramC, edges, kvsourcedram);
			#elif NUM_PEs==18
			run18(vdramA, vdramB, vdramC, edges, kvsourcedram);
			#elif NUM_PEs==20
			run20(vdramA, vdramB, vdramC, edges, kvsourcedram);
			#elif NUM_PEs==22
			run22(vdramA, vdramB, vdramC, edges, kvsourcedram);
			#elif NUM_PEs==24
			run24(vdramA, vdramB, vdramC, edges, kvsourcedram);
			#elif NUM_PEs==25
			run25(vdramA, vdramB, vdramC, edges, kvsourcedram);
			#elif NUM_PEs==32
			run32(vdramA, vdramB, vdramC, edges, kvsourcedram);
			#else 
			NOT DEFINED.
			#endif
		#endif 
		
		kernelobjs_process[0]->TOPP0_topkernelS((uint512_dt *)vdramA, (uint512_dt *)vdramB, (uint512_dt *)vdramC);	
		
		#ifdef _DEBUGMODE_HOSTPRINTS4
		long double iter_timeelapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - beginkerneltime_proc).count();
		cout<<">>> swkernel: iteration: "<<GraphIter<<": Time elapsed: "<<iter_timeelapsed<<"ms, num edges processed: "<<num_edges_processed[GraphIter]<<" edges, throughput: "<<((num_edges_processed[GraphIter] / iter_timeelapsed) * (1000))<<" edges/sec ("<<(((num_edges_processed[GraphIter] / iter_timeelapsed) * (1000)) / 1000000)<<" ME/s)"<<endl;
		#endif 
	}
	for(unsigned int i=0; i<universalparams.MAXHBMCAPACITY_KVS2; i++){ vdram[i] = vdramA[i]; }
	
	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	#endif
	return total_time_elapsed;
}

void swkernel::run3(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]){
	cout<<"------------------------------------ topkernelP1: processing P1 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP1( // REMOVEME.
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)vdramA
	);
	
	cout<<"------------------------------------ topkernelP1: processing P1 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP1(
		(uint512_dt *)kvsourcedram[NUMCOMPUTEUNITS_SLR2],
		(uint512_dt *)vdramB
	);
	
	cout<<"------------------------------------ topkernelP1: processing P1 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP1(
		(uint512_dt *)kvsourcedram[NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1],
		(uint512_dt *)vdramC
	);
}
void swkernel::run12(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]){
	unsigned int A_OFFSET = 0;
	unsigned int B_OFFSET = 5;
	unsigned int C_OFFSET = 10;
	
	cout<<"------------------------------------ topkernelP1: processing P5 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP5(
		(uint512_dt *)kvsourcedram[A_OFFSET + 0],
		(uint512_dt *)kvsourcedram[A_OFFSET + 1],
		(uint512_dt *)kvsourcedram[A_OFFSET + 2],
		(uint512_dt *)kvsourcedram[A_OFFSET + 3],
		(uint512_dt *)kvsourcedram[A_OFFSET + 4],
		(uint512_dt *)vdramA
	);
	
	cout<<"------------------------------------ topkernelP1: processing P5 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP5(
		(uint512_dt *)kvsourcedram[B_OFFSET + 0],
		(uint512_dt *)kvsourcedram[B_OFFSET + 1],
		(uint512_dt *)kvsourcedram[B_OFFSET + 2],
		(uint512_dt *)kvsourcedram[B_OFFSET + 3],
		(uint512_dt *)kvsourcedram[B_OFFSET + 4],
		(uint512_dt *)vdramB
	);
	
	cout<<"------------------------------------ topkernelP1: processing P5 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP2(
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)vdramC
	);
}
void swkernel::run16(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]){
	cout<<"------------------------------------ topkernelP1: processing P7 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP6(
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)kvsourcedram[3],
		(uint512_dt *)kvsourcedram[4],
		(uint512_dt *)kvsourcedram[5],
		(uint512_dt *)vdramA
	);
	
	cout<<"------------------------------------ topkernelP1: processing P7 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP6(
		(uint512_dt *)kvsourcedram[6],
		(uint512_dt *)kvsourcedram[7],
		(uint512_dt *)kvsourcedram[8],
		(uint512_dt *)kvsourcedram[9],
		(uint512_dt *)kvsourcedram[10],
		(uint512_dt *)kvsourcedram[11],
		(uint512_dt *)vdramB
	);
	
	cout<<"------------------------------------ topkernelP1: processing P6 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP4(
		(uint512_dt *)kvsourcedram[12],
		(uint512_dt *)kvsourcedram[13],
		(uint512_dt *)kvsourcedram[14],
		(uint512_dt *)kvsourcedram[15],
		(uint512_dt *)vdramC
	);
}
void swkernel::run18(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]){
	cout<<"------------------------------------ topkernelP1: processing P7 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP6(
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)kvsourcedram[3],
		(uint512_dt *)kvsourcedram[4],
		(uint512_dt *)kvsourcedram[5],
		(uint512_dt *)vdramA
	);
	
	cout<<"------------------------------------ topkernelP1: processing P7 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP6(
		(uint512_dt *)kvsourcedram[6],
		(uint512_dt *)kvsourcedram[7],
		(uint512_dt *)kvsourcedram[8],
		(uint512_dt *)kvsourcedram[9],
		(uint512_dt *)kvsourcedram[10],
		(uint512_dt *)kvsourcedram[11],
		(uint512_dt *)vdramB
	);
	
	cout<<"------------------------------------ topkernelP1: processing P6 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP6(
		(uint512_dt *)kvsourcedram[12],
		(uint512_dt *)kvsourcedram[13],
		(uint512_dt *)kvsourcedram[14],
		(uint512_dt *)kvsourcedram[15],
		(uint512_dt *)kvsourcedram[16],
		(uint512_dt *)kvsourcedram[17],
		(uint512_dt *)vdramC
	);
}
void swkernel::run20(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]){
	cout<<"------------------------------------ topkernelP1: processing P7 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP7(
		(uint512_dt *)kvsourcedram[0],
		(uint512_dt *)kvsourcedram[1],
		(uint512_dt *)kvsourcedram[2],
		(uint512_dt *)kvsourcedram[3],
		(uint512_dt *)kvsourcedram[4],
		(uint512_dt *)kvsourcedram[5],
		(uint512_dt *)kvsourcedram[6],
		(uint512_dt *)vdramA
	);
	
	cout<<"------------------------------------ topkernelP1: processing P7 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP7(
		(uint512_dt *)kvsourcedram[7],
		(uint512_dt *)kvsourcedram[8],
		(uint512_dt *)kvsourcedram[9],
		(uint512_dt *)kvsourcedram[10],
		(uint512_dt *)kvsourcedram[11],
		(uint512_dt *)kvsourcedram[12],
		(uint512_dt *)kvsourcedram[13],
		(uint512_dt *)vdramB
	);
	
	cout<<"------------------------------------ topkernelP1: processing P6 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP6(
		(uint512_dt *)kvsourcedram[14],
		(uint512_dt *)kvsourcedram[15],
		(uint512_dt *)kvsourcedram[16],
		(uint512_dt *)kvsourcedram[17],
		(uint512_dt *)kvsourcedram[18],
		(uint512_dt *)kvsourcedram[19],
		(uint512_dt *)vdramC
	);
}
void swkernel::run22(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]){
	cout<<"------------------------------------ topkernelP1: processing P8 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP8(
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
	
	cout<<"------------------------------------ topkernelP1: processing P8 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP8(
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
	
	cout<<"------------------------------------ topkernelP1: processing P6 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP6(
		(uint512_dt *)kvsourcedram[16],
		(uint512_dt *)kvsourcedram[17],
		(uint512_dt *)kvsourcedram[18],
		(uint512_dt *)kvsourcedram[19],
		(uint512_dt *)kvsourcedram[20],
		(uint512_dt *)kvsourcedram[21],
		(uint512_dt *)vdramC
	);
}
void swkernel::run24(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]){
	unsigned int A_OFFSET = 0;
	unsigned int B_OFFSET = 9;
	unsigned int C_OFFSET = 18;
	
	cout<<"------------------------------------ topkernelP1: processing P9 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP9(
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
	// exit(EXIT_SUCCESS);///////////////
	
	cout<<"------------------------------------ topkernelP1: processing P9 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP9(
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
	// exit(EXIT_SUCCESS); //
	
	cout<<"------------------------------------ topkernelP1: processing P6 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP6(
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)kvsourcedram[C_OFFSET + 2],
		(uint512_dt *)kvsourcedram[C_OFFSET + 3],
		(uint512_dt *)kvsourcedram[C_OFFSET + 4],
		(uint512_dt *)kvsourcedram[C_OFFSET + 5],
		(uint512_dt *)vdramC
	);
}
void swkernel::run25(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]){
	unsigned int A_OFFSET = 0;
	unsigned int B_OFFSET = 9;
	unsigned int C_OFFSET = 18;
	
	cout<<"------------------------------------ topkernelP1: processing P9 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP9(
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
	
	cout<<"------------------------------------ topkernelP1: processing P9 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP9(
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
	
	cout<<"------------------------------------ topkernelP1: processing P7 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP7(
		(uint512_dt *)kvsourcedram[C_OFFSET + 0],
		(uint512_dt *)kvsourcedram[C_OFFSET + 1],
		(uint512_dt *)kvsourcedram[C_OFFSET + 2],
		(uint512_dt *)kvsourcedram[C_OFFSET + 3],
		(uint512_dt *)kvsourcedram[C_OFFSET + 4],
		(uint512_dt *)kvsourcedram[C_OFFSET + 5],
		(uint512_dt *)kvsourcedram[C_OFFSET + 6],
		(uint512_dt *)vdramC
	);
}
void swkernel::run32(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvsourcedram[MAXNUM_PEs]){
	unsigned int A_OFFSET = 0;
	unsigned int B_OFFSET = 10;
	unsigned int C_OFFSET = 20;
	
	cout<<"------------------------------------ topkernelP1: processing P10 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP10(
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
	
	cout<<"------------------------------------ topkernelP1: processing P10 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP10(
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
	
	cout<<"------------------------------------ topkernelP1: processing P12 instances ------------------------------------"<<endl;
	kernelobjs_process[0]->TOPP0_topkernelP12(
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
}
#endif 











