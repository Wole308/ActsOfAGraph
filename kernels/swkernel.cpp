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




#ifdef KOKOOOOOOOO
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	#ifdef NACTS_IN_NCOMPUTEUNITS
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		
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


#ifdef KOKOOOOOOOO
long double swkernel::runapp(std::string binaryFile[2], uint512_vec_dt * vdram, uint512_vec_dt * kvsourcedram[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_TIMERS3
	std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
	#endif
	
	uint512_vec_dt * tempkvsourcedram[NUMSUBCPUTHREADS];
	uint512_vec_dt * tempvdram;
	
	//MESSAGES_ENABLE_RUNKERNELCOMMAND MESSAGES_BASEOFFSETKVS_MESSAGESDATA
	// unsigned int _BASEOFFSETKVS_MESSAGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_MESSAGESDATA].data[0].key;
	unsigned int _BASEOFFSETKVS_MESSAGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key;
	unsigned int _BASEOFFSETKVS_EDGESDATA = kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].data[0].key;
	unsigned int _MSIZE = _BASEOFFSETKVS_EDGESDATA - _BASEOFFSETKVS_MESSAGESDATA;
	
	unsigned int _BASEOFFSETKVS_VERTICESDATA = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key;
	unsigned int _BASEOFFSETKVS_KVDRAM = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].data[0].key;
	unsigned int _VSIZE = _BASEOFFSETKVS_KVDRAM - _BASEOFFSETKVS_VERTICESDATA;
	
	unsigned int _TOTSIZE = _MSIZE + _VSIZE;
	
	for(unsigned int i=0; i<NUMSYNCTHREADS; i++){ tempkvsourcedram[i] = new uint512_vec_dt[_TOTSIZE]; }
	tempvdram = new uint512_vec_dt[_TOTSIZE];

	unsigned int _OFF_KVS = 0;
	
	#ifdef NACTS_IN_NCOMPUTEUNITS
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i]); }
		
		////////////////////////////////
		_OFF_KVS = 0;
		for(unsigned int i=0; i<NUMSYNCTHREADS; i++){
			memcpy(&tempkvsourcedram[i][0], &kvsourcedram[i][_BASEOFFSETKVS_MESSAGESDATA], MESSAGESDRAMSZ * sizeof(uint512_vec_dt));
		}
		memcpy(&tempvdram[i][0], &vdram[i][_BASEOFFSETKVS_MESSAGESDATA], MESSAGESDRAMSZ * sizeof(uint512_vec_dt));
		_OFF_KVS += _MSIZE;
		for(unsigned int i=0; i<NUMSYNCTHREADS; i++){
			memcpy(&tempkvsourcedram[i][0 + _OFF_KVS], &kvsourcedram[i][_BASEOFFSETKVS_VERTICESDATA], _VSIZE * sizeof(uint512_vec_dt));
		}
		memcpy(&tempvdram[i][0 + _OFF_KVS], &vdram[i][_BASEOFFSETKVS_VERTICESDATA], _VSIZE * sizeof(uint512_vec_dt));
		_OFF_KVS += _VSIZE; // _TOTSIZE;
		////////////////////////////////
		
		#ifndef SW_IMPL
		kernelobjs_synchronize->topkernelsync( // topkernelsync_sw
			ON,
			ON,
			(uint512_dt *)tempkvsourcedram[0],
			(uint512_dt *)tempkvsourcedram[1],
			#if NUMSYNCTHREADS>2
			(uint512_dt *)tempkvsourcedram[2],
			(uint512_dt *)tempkvsourcedram[3],
			#if NUMSYNCTHREADS>4
			(uint512_dt *)tempkvsourcedram[4],
			(uint512_dt *)tempkvsourcedram[5],
			(uint512_dt *)tempkvsourcedram[6],
			(uint512_dt *)tempkvsourcedram[7],
			#if NUMSYNCTHREADS>8
			(uint512_dt *)tempkvsourcedram[8],
			(uint512_dt *)tempkvsourcedram[9],
			(uint512_dt *)tempkvsourcedram[10],
			(uint512_dt *)tempkvsourcedram[11],
			#if NUMSYNCTHREADS>12
			(uint512_dt *)tempkvsourcedram[12],
			(uint512_dt *)tempkvsourcedram[13],
			(uint512_dt *)tempkvsourcedram[14],
			(uint512_dt *)tempkvsourcedram[15],
			#if NUMSYNCTHREADS>16
			(uint512_dt *)tempkvsourcedram[16],
			(uint512_dt *)tempkvsourcedram[17],
			(uint512_dt *)tempkvsourcedram[18],
			(uint512_dt *)tempkvsourcedram[19],
			#if NUMSYNCTHREADS>20
			(uint512_dt *)tempkvsourcedram[20],
			(uint512_dt *)tempkvsourcedram[21],
			(uint512_dt *)tempkvsourcedram[22],
			(uint512_dt *)tempkvsourcedram[23],
			#if NUMSYNCTHREADS>24
			(uint512_dt *)tempkvsourcedram[24],
			(uint512_dt *)tempkvsourcedram[25],
			(uint512_dt *)tempkvsourcedram[26],
			(uint512_dt *)tempkvsourcedram[27],
			#if NUMSYNCTHREADS>28
			(uint512_dt *)tempkvsourcedram[28],
			(uint512_dt *)tempkvsourcedram[29],
			(uint512_dt *)tempkvsourcedram[30],
			(uint512_dt *)tempkvsourcedram[31],
			#endif
			#endif
			#endif
			#endif
			#endif
			#endif
			#endif
			#endif 
			(uint512_dt *)tempvdram
		);
		#endif
		exit(EXIT_SUCCESS); // REMOVEME.
		
		////////////////////////////////
		_OFF_KVS = 0;
		for(unsigned int i=0; i<NUMSYNCTHREADS; i++){
			memcpy(&kvsourcedram[i][_BASEOFFSETKVS_MESSAGESDATA], &tempkvsourcedram[i][0], MESSAGESDRAMSZ * sizeof(uint512_vec_dt));
		}
		memcpy(&vdram[i][_BASEOFFSETKVS_MESSAGESDATA], &tempvdram[i][0], MESSAGESDRAMSZ * sizeof(uint512_vec_dt));
		_OFF_KVS += _MSIZE;
		for(unsigned int i=0; i<NUMSYNCTHREADS; i++){
			memcpy(&kvsourcedram[i][_BASEOFFSETKVS_VERTICESDATA], &tempkvsourcedram[i][0 + _OFF_KVS], _VSIZE * sizeof(uint512_vec_dt));
		}
		memcpy(&vdram[i][_BASEOFFSETKVS_VERTICESDATA], &tempvdram[i][0 + _OFF_KVS], _VSIZE * sizeof(uint512_vec_dt));
		_OFF_KVS += _VSIZE; // _TOTSIZE;
		////////////////////////////////
		
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
	
	uint512_vec_dt * tempvdram = new uint512_vec_dt[PADDEDKVSOURCEDRAMSZ_KVS];
	
	unsigned int _BASEOFFSETKVS_VERTICESDATA = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key;
	unsigned int _BASEOFFSETKVS_VERTICESDATAMASK = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].data[0].key;
	unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	
	unsigned int ind = 0;
	unsigned int numIters = kvsourcedram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp: Iteration: "<<GraphIter<<endl;
		
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ kernelobjs_process[i]->topkernelproc((uint512_dt *)kvsourcedram[i]); }
	
		for(unsigned int k=0; k<VERTICESDATASZ_KVS; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				tempvdram[_BASEOFFSETKVS_VERTICESDATA + k].data[v].key = algorithmobj->vertex_initdata();
				tempvdram[_BASEOFFSETKVS_VERTICESDATA + k].data[v].value = algorithmobj->vertex_initdata();
			}
		}
	
		for(unsigned int i=0; i<2; i++){
			cout<<"swkernel::runapp: unifying... "<<endl;
			for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
				kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASK].data[0].key = OFF;
				kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASKP].data[0].key = OFF;
			}
			for(unsigned int ind=0; ind<32; ind+=NUMSYNCTHREADS){
			kernelobjs_synchronize->topkernelsync(
				OFF,//OFF,
				OFF,//OFF,
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
		
		cout<<"swkernel::runapp: applying... "<<endl;
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
			kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASK].data[0].key = ON;
			kvsourcedram[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_SAVEVMASKP].data[0].key = ON;
		}
		for(unsigned int ind=0; ind<32; ind+=NUMSYNCTHREADS){
		kernelobjs_synchronize->topkernelsync(
			ON,
			ON,
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
			(uint512_dt *)vdram
		);
		}
	
		for(unsigned int i=1; i<NUMSUBCPUTHREADS; i++){ ////////
			memcpy(&kvsourcedram[i][_BASEOFFSETKVS_VERTICESDATAMASK], &kvsourcedram[0][_BASEOFFSETKVS_VERTICESDATAMASK], (VERTICESDATAMASKSZ/VECTOR_SIZE) * sizeof(uint512_vec_dt));
			memcpy(&kvsourcedram[i][_BASEOFFSETKVS_VERTICESPARTITIONMASK], &kvsourcedram[0][_BASEOFFSETKVS_VERTICESPARTITIONMASK], 512 * sizeof(uint512_vec_dt));
		}
		
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




void swkernel::verifyresults_hw(uint512_vec_dt * kvdram, unsigned int _BASEOFFSETKVS_VERTICESDATA){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"swkernel::verifyresults_hw: verifying vertex data... "<<endl;
	#endif
	
	unsigned int vdatas[64];
	for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; }
	
	uint512_vec_dt buff[REDUCEBUFFERSZ];
	for(unsigned int offset_kvs=0; offset_kvs<VERTICESDATASZ_KVS; offset_kvs+=REDUCEBUFFERSZ){
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
			buff[i] = kvdram[_BASEOFFSETKVS_VERTICESDATA + offset_kvs + i];
		}
		
		for(unsigned int i=0; i<REDUCEBUFFERSZ; i++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				unsigned int pos = i*VECTOR_SIZE + v;
				unsigned int vid1 = offset_kvs*REDUCESZ*VECTOR_SIZE + v*REDUCESZ + i*2;
				unsigned int vid2 = vid1 + 1;
				unsigned int vdata1 = buff[i].data[v].key;
				unsigned int vdata2 = buff[i].data[v].value;
				
				if(vdata1 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS
					cout<<"app:verifyresults_hw: vid1: "<<vid1<<",vdata1: "<<vdata1<<endl;
					#endif 
					vdatas[vdata1] += 1; 
				}
				if(vdata2 < 64){
					#ifdef _DEBUGMODE_HOSTPRINTS
					cout<<"app:verifyresults_hw: vid2: "<<vid2<<",vdata2: "<<vdata2<<endl;
					#endif
					vdatas[vdata2] += 1; 
				}
			}
		}
	}
	utilityobj->printvalues("app::verifyresults_hw.vdatas: verifying results after kernel run", vdatas, 4);
	return;
}






#endif 












