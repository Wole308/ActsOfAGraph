#include "swkernel.h"
using namespace std;

#define ENABLE_ACTSPROC
#define ENABLE_ACTSSYNC

swkernel::swkernel(universalparams_t _universalparams){
	#ifndef FPGA_IMPL
	utilityobj = new utility(_universalparams);
	mydebugobj = new mydebug();
	universalparams = _universalparams;
	
	for(unsigned int i=0; i<1; i++){ kernelobjs_process[i] = new acts_all(mydebugobj); }
	#endif 
}
swkernel::~swkernel(){}

#ifndef FPGA_IMPL
void swkernel::verifyresults(uint512_vec_dt * vdram, unsigned int id){
	unsigned int vdatas[64]; for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	unsigned int _BASEOFFSETKVS_SRCVERTICESDATA = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_SRCVERTICESDATA].data[0].key;
	unsigned int _SIZE_SRCVERTICESDATA = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_SRCVERTICESDATA].data[0].key;
	
	for(unsigned int k=0; k<_SIZE_SRCVERTICESDATA / VECTOR2_SIZE; k++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			unsigned int vdata1 = utilityobj->READFROM_UINT(vdram[_BASEOFFSETKVS_SRCVERTICESDATA + k].data[v].key, OFFSETOF_VDATA, SIZEOF_VDATA);
			unsigned int vdata2 = utilityobj->READFROM_UINT(vdram[_BASEOFFSETKVS_SRCVERTICESDATA + k].data[v].value, OFFSETOF_VDATA, SIZEOF_VDATA);
			
			if(vdata1 < 64){
				vdatas[vdata1] += 1; 
			}
			if(vdata2 < 64){
				vdatas[vdata2] += 1; 
			}
		}
	}
	cout<<">>> swkernel::verifyresults:: Printing results. id: "<<id<<endl;
	utilityobj->printvalues("swkernel::verifyresults:: verifying results (vdatas)", vdatas, 16);
}
void swkernel::print_active_vpartitions(unsigned int GraphIter, uint512_vec_dt * vdram){
	pmask_dt * pmask0; pmask0 = new pmask_dt[universalparams.NUMPROCESSEDGESPARTITIONS];
	uint512_ivec_dt * tempvdram = (uint512_ivec_dt *)vdram;		
	unsigned int vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	unsigned int offset_kvs = vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK;
	for (buffer_type i=0; i<universalparams.NUMPROCESSEDGESPARTITIONS; i++){
		pmask0[i] = tempvdram[offset_kvs + i].data[GraphIter];
	}
	
	#if defined(_DEBUGMODE_HOSTPRINTS3) && defined(CONFIG_ALGORITHMTYPE_RANDOMACTIVEVERTICES)
	cout<<">>> swkernel::print_active_vpartitions: GraphIter: "<<GraphIter<<endl;
	unsigned int num_actvps = 0;
	for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){
		if(pmask0[t] > 0  && t < 16){ cout<<t<<", "; }
		if(pmask0[t] > 0){ num_actvps += 1; }
	}
	cout<<" ("<<num_actvps<<" active partitions, "<<universalparams.NUMPROCESSEDGESPARTITIONS<<" total partitions)"<<endl;
	#endif 
}

long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * mdram, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS], long double timeelapsed_totals[128][8], unsigned int numValidIters, 
		unsigned int * vpmaskbuffer[MAXNUMGRAPHITERATIONS], unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS], vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	mdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; 
	vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; 
	for(unsigned int i=0; i<NUM_PEs; i++){ kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = 0; } // reset
	
	unsigned int ind = 0;
	unsigned int _PROCESSCOMMAND = ON; unsigned int _PARTITIONCOMMAND = ON; unsigned int _APPLYUPDATESCOMMAND = ON;
	
	cout<< TIMINGRESULTSCOLOR <<">>> swkernel::runapp: ACTS started. Parameters: NUM_PEs: ["<<NUM_PEs<<"]"<< RESET <<endl;	
	
	uint512_vec_dt * vdramA; vdramA = new uint512_vec_dt[universalparams.MAXHBMCAPACITY_KVS];
	uint512_vec_dt * vdramB; vdramB = new uint512_vec_dt[universalparams.MAXHBMCAPACITY_KVS];
	uint512_vec_dt * vdramC; vdramC = new uint512_vec_dt[universalparams.MAXHBMCAPACITY_KVS];
	
	#ifdef CONFIG_PRELOADEDVERTEXPARTITIONMASKS
	cout<<">>> swkernel::runapp: populating active streaming partitions... "<<endl;					
	uint512_ivec_dt * tempvdram = (uint512_ivec_dt *)vdram;		
	unsigned int vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
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
	
	#ifdef CONFIG_ACTS_HYBRIDLOGIC
	cout<<">>> swkernel::runapp: populating edges, vptrs and active vertices into mdram... "<<endl;
	unsigned int mdram_BASEOFFSETKVS_EDGESDATA = mdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].data[0].key;
	unsigned int mdram_BASEOFFSETKVS_VERTEXPTR = mdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].data[0].key;
	unsigned int mdram_BASEOFFSETKVS_ACTIVEVERTICES = mdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key;
	unsigned int * temp = (unsigned int *)mdram; 
	for(unsigned int t=0; t<universalparams.NUM_EDGES; t++){ temp[(mdram_BASEOFFSETKVS_EDGESDATA * VECTOR2_SIZE) + t] = edgedatabuffer[t].dstvid; }
	unsigned int index=0; for(unsigned int index=0; index<universalparams.NUM_VERTICES; index++){ temp[(mdram_BASEOFFSETKVS_VERTEXPTR * VECTOR2_SIZE) + index] = vertexptrbuffer[index]; }
	mdram[mdram_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key = 1;
	#endif
	
	cout<<">>> swkernel::runapp: populating vdramA, vdramB and vdramC... "<<endl;
	for(unsigned int i=0; i<universalparams.MAXHBMCAPACITY_KVS; i++){ vdramA[i] = vdram[i]; vdramB[i] = vdram[i]; vdramC[i] = vdram[i]; }
	
	unsigned int mode = 1;

	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){ // numIters
		cout<< endl << TIMINGRESULTSCOLOR <<">>> swkernel::runapp: Iteration: "<<GraphIter<<" (of "<<numIters<<" iterations"<< RESET <<endl;
		std::chrono::steady_clock::time_point beginkerneltime_proc = std::chrono::steady_clock::now();
		
		#ifdef TESTKERNEL
			run3(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
		#else 
			#if NUM_PEs==3
			run3(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
			#elif NUM_PEs==12
			run12(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
			#elif NUM_PEs==16
			run16(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
			#elif NUM_PEs==18
			run18(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
			#elif NUM_PEs==20
			run20(vdramA, vdramB, vdramC, vdram, edges, kvsourcedram);
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
		#endif 
		
		kernelobjs_process[0]->TOPP0_topkernelS((uint512_dt *)vdramA, (uint512_dt *)vdramB, (uint512_dt *)vdramC, 
			#ifdef CONFIG_ACTS_HYBRIDLOGIC
			(uint512_dt *)mdram
			#else 
			(uint512_dt *)vdram	
			#endif 
			);	
		// exit(EXIT_SUCCESS);//////////////////////
		
		#ifdef CONFIG_ENABLE_PROFILING
		long double iter_timeelapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - beginkerneltime_proc).count();
		cout<<">>> swkernel: iteration: "<<GraphIter<<": Time elapsed: "<<iter_timeelapsed<<"ms, num edges processed: "<<num_edges_processed[GraphIter]<<" edges, throughput: "<<((num_edges_processed[GraphIter] / iter_timeelapsed) * (1000))<<" edges/sec ("<<(((num_edges_processed[GraphIter] / iter_timeelapsed) * (1000)) / 1000000)<<" ME/s)"<<endl;
		#endif 
		
		// if(totalactvvp == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
		// exit(EXIT_SUCCESS); //
	}
	for(unsigned int i=0; i<universalparams.MAXHBMCAPACITY_KVS; i++){ vdram[i] = vdramA[i]; }
	
	// cout<<"swkernel: verifying vdramA..."<<endl;
	// verifyresults(vdramA, 0);
			
	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	#endif
	return total_time_elapsed;
}

void swkernel::run3(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]){
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
void swkernel::run12(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]){
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
void swkernel::run16(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]){
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
void swkernel::run18(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]){
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
void swkernel::run20(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]){
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
void swkernel::run22(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]){
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
void swkernel::run24(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]){
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
void swkernel::run25(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]){
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
void swkernel::run32(uint512_vec_dt * vdramA, uint512_vec_dt * vdramB, uint512_vec_dt * vdramC, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[MAXNUMSUBCPUTHREADS]){
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











