#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include "../acts/acts/acts.h"
#include "../acts/acts/actsproc.h"
#include "../acts/acts/actssync.h"
#include "../src/stats/stats.h"
#include "../src/algorithm/algorithm.h"
#include "../src/graphs/graph.h"
#include "../src/utility/utility.h"
#include "../include/common.h"
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
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i] = new actsproc(); }
	kernelobjs_synchronize = new actssync();
	kernelobjs = new acts();
	#endif
}
swkernel::~swkernel(){}

#if defined(SW) & not defined(ACTS_1by1)
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], long double timeelapsed_totals[128][8]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	uint512_vec_dt * tempvdram = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	
	unsigned int _BASEOFFSETKVS_VERTICESDATA = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key;
	unsigned int _BASEOFFSETKVS_VERTICESDATAMASK = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].data[0].key;
	unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	unsigned int _BASEOFFSETKVS_STATSDRAM = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key;
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	
	unsigned int ind = 0;
	unsigned int _PROCESSCOMMAND = ON; unsigned int _PARTITIONCOMMAND = ON; unsigned int _APPLYUPDATESCOMMAND = ON;
	#ifdef ENABLE_KERNEL_PROFILING
	unsigned int analysis_icount = 3;
	#else 
	unsigned int analysis_icount = 1;
	#endif 
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter; }
		
		#ifdef ENABLE_ACTSPROC
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
			
			#ifdef ACTSPROC_1by1
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i]); }
			#endif 
			#ifdef ACTSPROC_2by1
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i+=4){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i], (uint512_dt *)kvsourcedram[i+1]); }				
			#endif 
			#ifdef ACTSPROC_4by1
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i+=4){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i], (uint512_dt *)kvsourcedram[i+1], (uint512_dt *)kvsourcedram[i+2], (uint512_dt *)kvsourcedram[i+3]); }				
			#endif 
			#ifdef ACTSPROC_8by1
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i+=8){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i], (uint512_dt *)kvsourcedram[i+1], (uint512_dt *)kvsourcedram[i+2], (uint512_dt *)kvsourcedram[i+3], (uint512_dt *)kvsourcedram[i+4], (uint512_dt *)kvsourcedram[i+5], (uint512_dt *)kvsourcedram[i+6], (uint512_dt *)kvsourcedram[i+7]); }					
			#endif 
			long double total_time_elapsed_proc = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - beginkerneltime_proc).count();
			cout<<"analysis_i: total_time_elapsed_proc: "<<total_time_elapsed_proc<<"ms"<<endl;
		}
		#endif 
		
		#if defined(ENABLE_ACTSSYNC) & not defined(TESTKERNEL)
		#if NUMSYNCTHREADS<NUMSUBCPUTHREADS
		for(unsigned int k=0; k<KVSTATSDRAMSZ_KVS; k++){
			unsigned int maxstats = 0;
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ maxstats += kvsourcedram[i][_BASEOFFSETKVS_STATSDRAM + k].data[0].value; }
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kvsourcedram[i][_BASEOFFSETKVS_STATSDRAM + k].data[0].value = maxstats; }
		}
		#endif
	
		#if NUMSYNCTHREADS<NUMSUBCPUTHREADS
		for(unsigned int k=0; k<VERTICESDATASZ_KVS; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				tempvdram[_BASEOFFSETKVS_VERTICESDATA + k].data[v].key = algorithmobj->vertex_initdata();
				tempvdram[_BASEOFFSETKVS_VERTICESDATA + k].data[v].value = algorithmobj->vertex_initdata();
			}
		}
		#endif 
		
		#if NUMSYNCTHREADS<NUMSUBCPUTHREADS
		for(unsigned int i=0; i<2; i++){
			cout<<"swkernel::runapp: unifying... "<<endl;
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
				kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASK].data[0].key = OFF;
				kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASKP].data[0].key = OFF;
			}
			for(unsigned int ind=0; ind<32; ind+=NUMSYNCTHREADS){
			kernelobjs_synchronize->topkernelsync(
				(uint512_dt *)kvsourcedram[ind],
				(uint512_dt *)kvsourcedram[ind+1],
				(uint512_dt *)kvsourcedram[ind+2],
				(uint512_dt *)kvsourcedram[ind+3],
				(uint512_dt *)kvsourcedram[ind+4],
				(uint512_dt *)kvsourcedram[ind+5],
				(uint512_dt *)kvsourcedram[ind+6],
				(uint512_dt *)kvsourcedram[ind+7],
				(uint512_dt *)kvsourcedram[ind+8],
				(uint512_dt *)kvsourcedram[ind+9],
				(uint512_dt *)kvsourcedram[ind+10],
				(uint512_dt *)kvsourcedram[ind+11],
				(uint512_dt *)kvsourcedram[ind+12],
				(uint512_dt *)kvsourcedram[ind+13],
				(uint512_dt *)kvsourcedram[ind+14],
				(uint512_dt *)kvsourcedram[ind+15],
				#if NUMSYNCTHREADS>16
				(uint512_dt *)kvsourcedram[ind+16],//
				(uint512_dt *)kvsourcedram[ind+17],
				(uint512_dt *)kvsourcedram[ind+18],
				(uint512_dt *)kvsourcedram[ind+19],
				(uint512_dt *)kvsourcedram[ind+20],
				(uint512_dt *)kvsourcedram[ind+21],
				(uint512_dt *)kvsourcedram[ind+22],
				(uint512_dt *)kvsourcedram[ind+23],
				(uint512_dt *)kvsourcedram[ind+24],
				(uint512_dt *)kvsourcedram[ind+25],
				(uint512_dt *)kvsourcedram[ind+26],
				(uint512_dt *)kvsourcedram[ind+27],
				(uint512_dt *)kvsourcedram[ind+28],
				(uint512_dt *)kvsourcedram[ind+29],
				(uint512_dt *)kvsourcedram[ind+30],
				(uint512_dt *)kvsourcedram[ind+31],//
				#endif 
				(uint512_dt *)tempvdram
			);
			}
		}
		#endif 
		
		cout<<"swkernel::runapp: applying... "<<endl;
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
			kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASK].data[0].key = ON;
			kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASKP].data[0].key = ON;
		}
		for(unsigned int ind=0; ind<32; ind+=NUMSYNCTHREADS){
		kernelobjs_synchronize->topkernelsync(
			(uint512_dt *)kvsourcedram[ind],
			(uint512_dt *)kvsourcedram[ind+1],
			(uint512_dt *)kvsourcedram[ind+2],
			(uint512_dt *)kvsourcedram[ind+3],
			#if NUMSYNCTHREADS>4
			(uint512_dt *)kvsourcedram[ind+4],
			(uint512_dt *)kvsourcedram[ind+5],
			(uint512_dt *)kvsourcedram[ind+6],
			(uint512_dt *)kvsourcedram[ind+7],
			#if NUMSYNCTHREADS>8
			(uint512_dt *)kvsourcedram[ind+8],
			(uint512_dt *)kvsourcedram[ind+9],
			(uint512_dt *)kvsourcedram[ind+10],
			(uint512_dt *)kvsourcedram[ind+11],
			#if NUMSYNCTHREADS>12
			(uint512_dt *)kvsourcedram[ind+12],
			(uint512_dt *)kvsourcedram[ind+13],
			(uint512_dt *)kvsourcedram[ind+14],
			(uint512_dt *)kvsourcedram[ind+15],
			#if NUMSYNCTHREADS>16
			(uint512_dt *)kvsourcedram[ind+16],
			(uint512_dt *)kvsourcedram[ind+17],
			(uint512_dt *)kvsourcedram[ind+18],
			(uint512_dt *)kvsourcedram[ind+19],
			(uint512_dt *)kvsourcedram[ind+20],
			(uint512_dt *)kvsourcedram[ind+21],
			(uint512_dt *)kvsourcedram[ind+22],
			(uint512_dt *)kvsourcedram[ind+23],
			#if NUMSYNCTHREADS>24
			(uint512_dt *)kvsourcedram[ind+24],
			(uint512_dt *)kvsourcedram[ind+25],
			(uint512_dt *)kvsourcedram[ind+26],
			(uint512_dt *)kvsourcedram[ind+27],
			(uint512_dt *)kvsourcedram[ind+28],
			(uint512_dt *)kvsourcedram[ind+29],
			(uint512_dt *)kvsourcedram[ind+30],
			(uint512_dt *)kvsourcedram[ind+31],
			#endif 
			#endif 
			#endif
			#endif
			#endif
			(uint512_dt *)vdram
		);
		}
	
		#if NUMSYNCTHREADS<NUMSUBCPUTHREADS
		for(unsigned int i=1; i<NUMSUBCPUTHREADS; i++){
			memcpy(&kvsourcedram[i][_BASEOFFSETKVS_VERTICESDATAMASK], &kvsourcedram[0][_BASEOFFSETKVS_VERTICESDATAMASK], (VERTICESDATAMASKSZ/VECTOR_SIZE) * sizeof(uint512_vec_dt));
			memcpy(&kvsourcedram[i][_BASEOFFSETKVS_VERTICESPARTITIONMASK], &kvsourcedram[0][_BASEOFFSETKVS_VERTICESPARTITIONMASK], 512 * sizeof(uint512_vec_dt));
		}
		#endif 
		#endif 
		unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
		unsigned int BLOP = pow(NUM_PARTITIONS, (TREE_DEPTH-1));
		unsigned int totalactvvp = 0;
		cout<<endl<<"active partitions for iteration "<<GraphIter+1<<": ";
		for(unsigned int i=0; i<256; i++){
			unsigned int gmask = kvsourcedram[0][_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].data[0].key;
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
#endif 

#if defined(SW) & defined(ACTS_1by1)
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * edges[NUMSUBCPUTHREADS], uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS], long double timeelapsed_totals[128][8]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	unsigned int _BASEOFFSETKVS_VERTICESDATA = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key;
	unsigned int _BASEOFFSETKVS_VERTICESDATAMASK = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].data[0].key;
	unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	unsigned int _BASEOFFSETKVS_STATSDRAM = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key;
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;

	std::chrono::steady_clock::time_point beginkerneltime_proc = std::chrono::steady_clock::now();
	kernelobjs->topkernel(
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[0],
			#endif
		(uint512_dt *)kvsourcedram[0],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[1],
			#endif
		(uint512_dt *)kvsourcedram[1],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[2],
			#endif		
		(uint512_dt *)kvsourcedram[2],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[3],
			#endif	
		(uint512_dt *)kvsourcedram[3],
		#if NUMSYNCTHREADS>4
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[4],
			#endif	
		(uint512_dt *)kvsourcedram[4],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[5],
			#endif	
		(uint512_dt *)kvsourcedram[5],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[6],
			#endif	
		(uint512_dt *)kvsourcedram[6],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[7],
			#endif	
		(uint512_dt *)kvsourcedram[7],
		#if NUMSYNCTHREADS>8
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[8],
			#endif	
		(uint512_dt *)kvsourcedram[8],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[9],
			#endif	
		(uint512_dt *)kvsourcedram[9],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[10],
			#endif	
		(uint512_dt *)kvsourcedram[10],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[11],
			#endif	
		(uint512_dt *)kvsourcedram[11],
		#if NUMSYNCTHREADS>12
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[12],
			#endif
		(uint512_dt *)kvsourcedram[12],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[13],
			#endif	
		(uint512_dt *)kvsourcedram[13],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[14],
			#endif	
		(uint512_dt *)kvsourcedram[14],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[15],
			#endif	
		(uint512_dt *)kvsourcedram[15],
		#if NUMSYNCTHREADS>16
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[16],
			#endif	
		(uint512_dt *)kvsourcedram[16],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[17],
			#endif	
		(uint512_dt *)kvsourcedram[17],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[18],
			#endif	
		(uint512_dt *)kvsourcedram[18],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[19],
			#endif	
		(uint512_dt *)kvsourcedram[19],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[20],
			#endif	
		(uint512_dt *)kvsourcedram[20],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[21],
			#endif	
		(uint512_dt *)kvsourcedram[21],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[22],
			#endif	
		(uint512_dt *)kvsourcedram[22],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[23],
			#endif	
		(uint512_dt *)kvsourcedram[23],
		#if NUMSYNCTHREADS>24
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[24],
			#endif	
		(uint512_dt *)kvsourcedram[24],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[25],
			#endif	
		(uint512_dt *)kvsourcedram[25],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[26],
			#endif	
		(uint512_dt *)kvsourcedram[26],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[27],
			#endif	
		(uint512_dt *)kvsourcedram[27],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[28],
			#endif	
		(uint512_dt *)kvsourcedram[28],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[29],
			#endif	
		(uint512_dt *)kvsourcedram[29],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[30],
			#endif	
		(uint512_dt *)kvsourcedram[30],
			#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
			(uint512_dt *)edges[31],
			#endif	
		(uint512_dt *)kvsourcedram[31],
		#endif 
		#endif 
		#endif
		#endif
		#endif
		(uint512_dt *)vdram
	);
	long double total_time_elapsed_proc = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - beginkerneltime_proc).count();
	cout<<"analysis_i: total_time_elapsed_proc: "<<total_time_elapsed_proc<<"ms"<<endl;
	
	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(total_time_elapsed);
	#endif
	return total_time_elapsed;
}
#endif 












