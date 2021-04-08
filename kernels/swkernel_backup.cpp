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
#include "../acts/acts_sw/acts_sw.h"
#include "../src/stats/stats.h"
#include "../src/algorithm/algorithm.h"
#include "../src/graphs/graph.h"
#include "../src/utility/utility.h"
#include "../include/common.h"
#include "swkernel.h"
using namespace std;

swkernel::swkernel(graph * _graphobj, algorithm * _algorithmobj, stats * _statsobj){
	utilityobj = new utility();
	statsobj = _statsobj;
	graphobj = _graphobj;
	algorithmobj = _algorithmobj;
	
	#ifdef SW
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs[i] = new acts(); }
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i] = new actsproc(); }
	kernelobjs_synchronize = new actssync();
	
	actssw_obj = new acts_sw();
	#endif
}
swkernel::~swkernel(){}

#ifdef SW
#ifdef KOKOOOOOOO
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef NACTS_IN_NCOMPUTEUNITS
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i]); }
		
		#ifndef SW_IMPL
		kernelobjs_synchronize->topkernelsync( // topkernelsync_sw
			(uint512_dt *)kvsourcedram[0],
			(uint512_dt *)kvsourcedram[1],
			#if NUMCOMPUTEUNITS>2
			(uint512_dt *)kvsourcedram[2],
			(uint512_dt *)kvsourcedram[3],
			#if NUMCOMPUTEUNITS>4
			(uint512_dt *)kvsourcedram[4],
			(uint512_dt *)kvsourcedram[5],
			(uint512_dt *)kvsourcedram[6],
			(uint512_dt *)kvsourcedram[7],
			#if NUMCOMPUTEUNITS>8
			(uint512_dt *)kvsourcedram[8],
			(uint512_dt *)kvsourcedram[9],
			(uint512_dt *)kvsourcedram[10],
			(uint512_dt *)kvsourcedram[11],
			#if NUMCOMPUTEUNITS>12
			(uint512_dt *)kvsourcedram[12],
			(uint512_dt *)kvsourcedram[13],
			(uint512_dt *)kvsourcedram[14],
			(uint512_dt *)kvsourcedram[15],
			#if NUMCOMPUTEUNITS>16
			(uint512_dt *)kvsourcedram[16],
			(uint512_dt *)kvsourcedram[17],
			(uint512_dt *)kvsourcedram[18],
			(uint512_dt *)kvsourcedram[19],
			#if NUMCOMPUTEUNITS>20
			(uint512_dt *)kvsourcedram[20],
			(uint512_dt *)kvsourcedram[21],
			(uint512_dt *)kvsourcedram[22],
			(uint512_dt *)kvsourcedram[23],
			#if NUMCOMPUTEUNITS>24
			(uint512_dt *)kvsourcedram[24],
			(uint512_dt *)kvsourcedram[25],
			(uint512_dt *)kvsourcedram[26],
			(uint512_dt *)kvsourcedram[27],
			#if NUMCOMPUTEUNITS>28
			(uint512_dt *)kvsourcedram[28],
			(uint512_dt *)kvsourcedram[29],
			(uint512_dt *)kvsourcedram[30],
			(uint512_dt *)kvsourcedram[31],
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
	#endif
	
	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(total_time_elapsed);
	#endif
	return total_time_elapsed;
}
#endif
// #ifdef KOKOOOOOOO
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	unsigned int _BASEOFFSETKVS_VERTICESDATA = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key;
	uint512_vec_dt * tempvdram = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	
	unsigned int indexoff = 0;
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>>>>>>>>>>>>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i]); }
	
		for(unsigned int k=0; k<VERTICESDATASZ_KVS; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				tempvdram[_BASEOFFSETKVS_VERTICESDATA + k].data[v].key = algorithmobj->vertex_initdata();
				tempvdram[_BASEOFFSETKVS_VERTICESDATA + k].data[v].value = algorithmobj->vertex_initdata();
			}
		}
	
		//
		indexoff = 0;
		kernelobjs_synchronize->topkernelsync(
			OFF,
			OFF,
			(uint512_dt *)kvsourcedram[indexoff],
			(uint512_dt *)kvsourcedram[indexoff+1],
			(uint512_dt *)kvsourcedram[indexoff+2],
			(uint512_dt *)kvsourcedram[indexoff+3],
			(uint512_dt *)kvsourcedram[indexoff+4],
			(uint512_dt *)kvsourcedram[indexoff+5],
			(uint512_dt *)kvsourcedram[indexoff+6],
			(uint512_dt *)kvsourcedram[indexoff+7],
			(uint512_dt *)kvsourcedram[indexoff+8],
			(uint512_dt *)kvsourcedram[indexoff+9],
			(uint512_dt *)kvsourcedram[indexoff+10],
			(uint512_dt *)kvsourcedram[indexoff+11],
			(uint512_dt *)kvsourcedram[indexoff+12],
			(uint512_dt *)kvsourcedram[indexoff+13],
			(uint512_dt *)kvsourcedram[indexoff+14],
			(uint512_dt *)kvsourcedram[indexoff+15],
			(uint512_dt *)tempvdram
		);
		
		indexoff = 16;
		kernelobjs_synchronize->topkernelsync(
			OFF,
			OFF,
			(uint512_dt *)kvsourcedram[indexoff],
			(uint512_dt *)kvsourcedram[indexoff+1],
			(uint512_dt *)kvsourcedram[indexoff+2],
			(uint512_dt *)kvsourcedram[indexoff+3],
			(uint512_dt *)kvsourcedram[indexoff+4],
			(uint512_dt *)kvsourcedram[indexoff+5],
			(uint512_dt *)kvsourcedram[indexoff+6],
			(uint512_dt *)kvsourcedram[indexoff+7],
			(uint512_dt *)kvsourcedram[indexoff+8],
			(uint512_dt *)kvsourcedram[indexoff+9],
			(uint512_dt *)kvsourcedram[indexoff+10],
			(uint512_dt *)kvsourcedram[indexoff+11],
			(uint512_dt *)kvsourcedram[indexoff+12],
			(uint512_dt *)kvsourcedram[indexoff+13],
			(uint512_dt *)kvsourcedram[indexoff+14],
			(uint512_dt *)kvsourcedram[indexoff+15],
			(uint512_dt *)tempvdram
		);
		//
		
		//
		indexoff = 0;
		kernelobjs_synchronize->topkernelsync(
			OFF,
			OFF,
			(uint512_dt *)kvsourcedram[indexoff],
			(uint512_dt *)kvsourcedram[indexoff+1],
			(uint512_dt *)kvsourcedram[indexoff+2],
			(uint512_dt *)kvsourcedram[indexoff+3],
			(uint512_dt *)kvsourcedram[indexoff+4],
			(uint512_dt *)kvsourcedram[indexoff+5],
			(uint512_dt *)kvsourcedram[indexoff+6],
			(uint512_dt *)kvsourcedram[indexoff+7],
			(uint512_dt *)kvsourcedram[indexoff+8],
			(uint512_dt *)kvsourcedram[indexoff+9],
			(uint512_dt *)kvsourcedram[indexoff+10],
			(uint512_dt *)kvsourcedram[indexoff+11],
			(uint512_dt *)kvsourcedram[indexoff+12],
			(uint512_dt *)kvsourcedram[indexoff+13],
			(uint512_dt *)kvsourcedram[indexoff+14],
			(uint512_dt *)kvsourcedram[indexoff+15],
			(uint512_dt *)tempvdram
		);
		
		indexoff = 16;
		kernelobjs_synchronize->topkernelsync(
			OFF,
			OFF,
			(uint512_dt *)kvsourcedram[indexoff],
			(uint512_dt *)kvsourcedram[indexoff+1],
			(uint512_dt *)kvsourcedram[indexoff+2],
			(uint512_dt *)kvsourcedram[indexoff+3],
			(uint512_dt *)kvsourcedram[indexoff+4],
			(uint512_dt *)kvsourcedram[indexoff+5],
			(uint512_dt *)kvsourcedram[indexoff+6],
			(uint512_dt *)kvsourcedram[indexoff+7],
			(uint512_dt *)kvsourcedram[indexoff+8],
			(uint512_dt *)kvsourcedram[indexoff+9],
			(uint512_dt *)kvsourcedram[indexoff+10],
			(uint512_dt *)kvsourcedram[indexoff+11],
			(uint512_dt *)kvsourcedram[indexoff+12],
			(uint512_dt *)kvsourcedram[indexoff+13],
			(uint512_dt *)kvsourcedram[indexoff+14],
			(uint512_dt *)kvsourcedram[indexoff+15],
			(uint512_dt *)tempvdram
		);
		//
		
		//
		indexoff = 0;
		kernelobjs_synchronize->topkernelsync(
			ON,
			ON,
			(uint512_dt *)kvsourcedram[indexoff],
			(uint512_dt *)kvsourcedram[indexoff+1],
			(uint512_dt *)kvsourcedram[indexoff+2],
			(uint512_dt *)kvsourcedram[indexoff+3],
			(uint512_dt *)kvsourcedram[indexoff+4],
			(uint512_dt *)kvsourcedram[indexoff+5],
			(uint512_dt *)kvsourcedram[indexoff+6],
			(uint512_dt *)kvsourcedram[indexoff+7],
			(uint512_dt *)kvsourcedram[indexoff+8],
			(uint512_dt *)kvsourcedram[indexoff+9],
			(uint512_dt *)kvsourcedram[indexoff+10],
			(uint512_dt *)kvsourcedram[indexoff+11],
			(uint512_dt *)kvsourcedram[indexoff+12],
			(uint512_dt *)kvsourcedram[indexoff+13],
			(uint512_dt *)kvsourcedram[indexoff+14],
			(uint512_dt *)kvsourcedram[indexoff+15],
			(uint512_dt *)vdram
		);
		
		indexoff = 16;
		kernelobjs_synchronize->topkernelsync(
			ON,
			ON,
			(uint512_dt *)kvsourcedram[indexoff],
			(uint512_dt *)kvsourcedram[indexoff+1],
			(uint512_dt *)kvsourcedram[indexoff+2],
			(uint512_dt *)kvsourcedram[indexoff+3],
			(uint512_dt *)kvsourcedram[indexoff+4],
			(uint512_dt *)kvsourcedram[indexoff+5],
			(uint512_dt *)kvsourcedram[indexoff+6],
			(uint512_dt *)kvsourcedram[indexoff+7],
			(uint512_dt *)kvsourcedram[indexoff+8],
			(uint512_dt *)kvsourcedram[indexoff+9],
			(uint512_dt *)kvsourcedram[indexoff+10],
			(uint512_dt *)kvsourcedram[indexoff+11],
			(uint512_dt *)kvsourcedram[indexoff+12],
			(uint512_dt *)kvsourcedram[indexoff+13],
			(uint512_dt *)kvsourcedram[indexoff+14],
			(uint512_dt *)kvsourcedram[indexoff+15],
			(uint512_dt *)vdram
		);
		//
		// exit(EXIT_SUCCESS); /////
		
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
// #endif
#ifdef KOKOOOOOOO
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	unsigned int _BASEOFFSETKVS_VERTICESDATA = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key;
	uint512_vec_dt * tempvdram = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	
	unsigned int indexoff = 0;
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>>>>>>>>>>>>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i]); }
	
		for(unsigned int k=0; k<VERTICESDATASZ_KVS; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				tempvdram[_BASEOFFSETKVS_VERTICESDATA + k].data[v].key = algorithmobj->vertex_initdata();
				tempvdram[_BASEOFFSETKVS_VERTICESDATA + k].data[v].value = algorithmobj->vertex_initdata();
			}
		}
	
		//
		indexoff = 0;
		kernelobjs_synchronize->topkernelsync(
			OFF,
			OFF,
			(uint512_dt *)kvsourcedram[indexoff],
			(uint512_dt *)kvsourcedram[indexoff+1],
			(uint512_dt *)kvsourcedram[indexoff+2],
			(uint512_dt *)kvsourcedram[indexoff+3],
			(uint512_dt *)kvsourcedram[indexoff+4],
			(uint512_dt *)kvsourcedram[indexoff+5],
			(uint512_dt *)kvsourcedram[indexoff+6],
			(uint512_dt *)kvsourcedram[indexoff+7],
			(uint512_dt *)kvsourcedram[indexoff+8],
			(uint512_dt *)kvsourcedram[indexoff+9],
			(uint512_dt *)kvsourcedram[indexoff+10],
			(uint512_dt *)kvsourcedram[indexoff+11],
			(uint512_dt *)kvsourcedram[indexoff+12],
			(uint512_dt *)kvsourcedram[indexoff+13],
			(uint512_dt *)kvsourcedram[indexoff+14],
			(uint512_dt *)kvsourcedram[indexoff+15],
			(uint512_dt *)kvsourcedram[indexoff+16],
			(uint512_dt *)kvsourcedram[indexoff+17],
			(uint512_dt *)kvsourcedram[indexoff+18],
			(uint512_dt *)kvsourcedram[indexoff+19],
			(uint512_dt *)kvsourcedram[indexoff+20],
			(uint512_dt *)kvsourcedram[indexoff+21],
			(uint512_dt *)kvsourcedram[indexoff+22],
			(uint512_dt *)kvsourcedram[indexoff+23],
			(uint512_dt *)kvsourcedram[indexoff+24],
			(uint512_dt *)kvsourcedram[indexoff+25],
			(uint512_dt *)kvsourcedram[indexoff+26],
			(uint512_dt *)kvsourcedram[indexoff+27],
			(uint512_dt *)kvsourcedram[indexoff+28],
			(uint512_dt *)kvsourcedram[indexoff+29],
			(uint512_dt *)kvsourcedram[indexoff+30],
			(uint512_dt *)kvsourcedram[indexoff+31],
			(uint512_dt *)tempvdram
		);
		
		indexoff = 0;
		kernelobjs_synchronize->topkernelsync(
			OFF,
			OFF,
			(uint512_dt *)kvsourcedram[indexoff],
			(uint512_dt *)kvsourcedram[indexoff+1],
			(uint512_dt *)kvsourcedram[indexoff+2],
			(uint512_dt *)kvsourcedram[indexoff+3],
			(uint512_dt *)kvsourcedram[indexoff+4],
			(uint512_dt *)kvsourcedram[indexoff+5],
			(uint512_dt *)kvsourcedram[indexoff+6],
			(uint512_dt *)kvsourcedram[indexoff+7],
			(uint512_dt *)kvsourcedram[indexoff+8],
			(uint512_dt *)kvsourcedram[indexoff+9],
			(uint512_dt *)kvsourcedram[indexoff+10],
			(uint512_dt *)kvsourcedram[indexoff+11],
			(uint512_dt *)kvsourcedram[indexoff+12],
			(uint512_dt *)kvsourcedram[indexoff+13],
			(uint512_dt *)kvsourcedram[indexoff+14],
			(uint512_dt *)kvsourcedram[indexoff+15],
			(uint512_dt *)kvsourcedram[indexoff+16],
			(uint512_dt *)kvsourcedram[indexoff+17],
			(uint512_dt *)kvsourcedram[indexoff+18],
			(uint512_dt *)kvsourcedram[indexoff+19],
			(uint512_dt *)kvsourcedram[indexoff+20],
			(uint512_dt *)kvsourcedram[indexoff+21],
			(uint512_dt *)kvsourcedram[indexoff+22],
			(uint512_dt *)kvsourcedram[indexoff+23],
			(uint512_dt *)kvsourcedram[indexoff+24],
			(uint512_dt *)kvsourcedram[indexoff+25],
			(uint512_dt *)kvsourcedram[indexoff+26],
			(uint512_dt *)kvsourcedram[indexoff+27],
			(uint512_dt *)kvsourcedram[indexoff+28],
			(uint512_dt *)kvsourcedram[indexoff+29],
			(uint512_dt *)kvsourcedram[indexoff+30],
			(uint512_dt *)kvsourcedram[indexoff+31],
			(uint512_dt *)tempvdram
		);
		//
		
		//
		indexoff = 0;
		kernelobjs_synchronize->topkernelsync(
			ON,
			ON,
			(uint512_dt *)kvsourcedram[indexoff],
			(uint512_dt *)kvsourcedram[indexoff+1],
			(uint512_dt *)kvsourcedram[indexoff+2],
			(uint512_dt *)kvsourcedram[indexoff+3],
			(uint512_dt *)kvsourcedram[indexoff+4],
			(uint512_dt *)kvsourcedram[indexoff+5],
			(uint512_dt *)kvsourcedram[indexoff+6],
			(uint512_dt *)kvsourcedram[indexoff+7],
			(uint512_dt *)kvsourcedram[indexoff+8],
			(uint512_dt *)kvsourcedram[indexoff+9],
			(uint512_dt *)kvsourcedram[indexoff+10],
			(uint512_dt *)kvsourcedram[indexoff+11],
			(uint512_dt *)kvsourcedram[indexoff+12],
			(uint512_dt *)kvsourcedram[indexoff+13],
			(uint512_dt *)kvsourcedram[indexoff+14],
			(uint512_dt *)kvsourcedram[indexoff+15],
			(uint512_dt *)kvsourcedram[indexoff+16],
			(uint512_dt *)kvsourcedram[indexoff+17],
			(uint512_dt *)kvsourcedram[indexoff+18],
			(uint512_dt *)kvsourcedram[indexoff+19],
			(uint512_dt *)kvsourcedram[indexoff+20],
			(uint512_dt *)kvsourcedram[indexoff+21],
			(uint512_dt *)kvsourcedram[indexoff+22],
			(uint512_dt *)kvsourcedram[indexoff+23],
			(uint512_dt *)kvsourcedram[indexoff+24],
			(uint512_dt *)kvsourcedram[indexoff+25],
			(uint512_dt *)kvsourcedram[indexoff+26],
			(uint512_dt *)kvsourcedram[indexoff+27],
			(uint512_dt *)kvsourcedram[indexoff+28],
			(uint512_dt *)kvsourcedram[indexoff+29],
			(uint512_dt *)kvsourcedram[indexoff+30],
			(uint512_dt *)kvsourcedram[indexoff+31],
			(uint512_dt *)vdram
		);
		
		/* indexoff = 0;
		kernelobjs_synchronize->topkernelsync(
			ON,
			ON,
			(uint512_dt *)kvsourcedram[indexoff],
			(uint512_dt *)kvsourcedram[indexoff+1],
			(uint512_dt *)kvsourcedram[indexoff+2],
			(uint512_dt *)kvsourcedram[indexoff+3],
			(uint512_dt *)kvsourcedram[indexoff+4],
			(uint512_dt *)kvsourcedram[indexoff+5],
			(uint512_dt *)kvsourcedram[indexoff+6],
			(uint512_dt *)kvsourcedram[indexoff+7],
			(uint512_dt *)kvsourcedram[indexoff+8],
			(uint512_dt *)kvsourcedram[indexoff+9],
			(uint512_dt *)kvsourcedram[indexoff+10],
			(uint512_dt *)kvsourcedram[indexoff+11],
			(uint512_dt *)kvsourcedram[indexoff+12],
			(uint512_dt *)kvsourcedram[indexoff+13],
			(uint512_dt *)kvsourcedram[indexoff+14],
			(uint512_dt *)kvsourcedram[indexoff+15],
			(uint512_dt *)kvsourcedram[indexoff+16],
			(uint512_dt *)kvsourcedram[indexoff+17],
			(uint512_dt *)kvsourcedram[indexoff+18],
			(uint512_dt *)kvsourcedram[indexoff+19],
			(uint512_dt *)kvsourcedram[indexoff+20],
			(uint512_dt *)kvsourcedram[indexoff+21],
			(uint512_dt *)kvsourcedram[indexoff+22],
			(uint512_dt *)kvsourcedram[indexoff+23],
			(uint512_dt *)kvsourcedram[indexoff+24],
			(uint512_dt *)kvsourcedram[indexoff+25],
			(uint512_dt *)kvsourcedram[indexoff+26],
			(uint512_dt *)kvsourcedram[indexoff+27],
			(uint512_dt *)kvsourcedram[indexoff+28],
			(uint512_dt *)kvsourcedram[indexoff+29],
			(uint512_dt *)kvsourcedram[indexoff+30],
			(uint512_dt *)kvsourcedram[indexoff+31],
			(uint512_dt *)vdram
		); */
		//
		// exit(EXIT_SUCCESS); /////
		
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
long double swkernel::runapp(edge_type * edges[NUMSUBCPUTHREADS], edge_t * vptrs[NUMSUBCPUTHREADS], value_t * vprops, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[NUMSUBCPUTHREADS][TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int numIters){				
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	actssw_obj->start(graphobj, edges, vptrs, vprops, actvvs, actvvs_nextit, kvdram, GraphAlgo, numIters);

	#ifdef _DEBUGMODE_TIMERS3
	long double total_time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begintime).count();
	statsobj->appendkerneltimeelapsed(total_time_elapsed);
	#endif
	return total_time_elapsed;
}
#endif 












