#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#ifndef FPGA_IMPL
#include "../../src/utility/utility.h"
#endif 
#ifndef HW
#include "../../acts/actsutility/actsutility.h"
#endif 
#include "acts.h"
#ifndef FPGA_IMPL
#include "actsproc.h"
#include "actssync.h"
#endif 
using namespace std;

// #ifdef NOTIMPLEMENTEDXXX
#ifdef SW
acts::acts(){}
acts::~acts(){}
#endif
#ifdef SW
actsproc actsprocobj;
actssync actssyncobj;
#endif 

#ifdef SW
void
	#ifdef SW 
	acts::
	#endif 
loadsrcvs( uint512_dt * edges0, uint512_dt * kvdram0,  uint512_dt * edges1, uint512_dt * kvdram1,  uint512_dt * edges2, uint512_dt * kvdram2,  uint512_dt * vdram){
	#pragma HLS INLINE
	analysis_type analysis_treedepth = TREE_DEPTH;
	analysis_type analysis_loop1 = 1;
	
	globalparams_t globalparamsK[NUMSUBCPUTHREADS];
	globalparams_t globalparamsE;
	travstate_t rtravstate[NUMSUBCPUTHREADS];
	
	globalparamsK[0] = actssyncobj.SYNC_getglobalparams(kvdram0);
	globalparamsK[1] = globalparamsK[0];
	globalparamsK[2] = globalparamsK[0];
	globalparamsE = actssyncobj.SYNC_getglobalparams(edges0);
	
	unsigned int BASEOFFSETKVS_VERTICESDATA_K = globalparamsK[0].BASEOFFSETKVS_DESTVERTICESDATA;
	unsigned int BASEOFFSETKVS_VERTICESDATA_E = globalparamsE.BASEOFFSETKVS_DESTVERTICESDATA;
	
	#ifdef ENABLERECURSIVEPARTITIONING
	step_type currentLOP = globalparamsK[0].ACTSPARAMS_TREEDEPTH;
	batch_type num_source_partitions = actssyncobj.SYNC_get_num_source_partitions(globalparamsK[0].ACTSPARAMS_TREEDEPTH);
	#else
	step_type currentLOP = globalparamsK[0].ACTSPARAMS_TREEDEPTH + 1;
	batch_type num_source_partitions = NUM_PARTITIONS;
	#endif
	buffer_type reducebuffersz = globalparamsK[0].SIZE_REDUCE / 2; // 512
	
	unsigned int sourcestatsmarker = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparamsK[0].ACTSPARAMS_TREEDEPTH-1; k++)
	#else 
	for(unsigned int k=0; k<globalparamsK[0].ACTSPARAMS_TREEDEPTH; k++)
	#endif 
	{
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_treedepth avg=analysis_treedepth
		sourcestatsmarker += (1 << (NUM_PARTITIONS_POW * k)); 
	}

	for(batch_type iterationidx=0; iterationidx<num_source_partitions; iterationidx+=1){
	#pragma HLS LOOP_TRIPCOUNT min=0 max=analysis_loop1 avg=analysis_loop1
	
		unsigned int ntravszs = 0;
		rtravstate[0] = actssyncobj.SYNC_gettravstate(ON, kvdram0, globalparamsK[0], currentLOP, sourcestatsmarker);
		rtravstate[1] = actssyncobj.SYNC_gettravstate(ON, kvdram1, globalparamsK[1], currentLOP, sourcestatsmarker);
		rtravstate[2] = actssyncobj.SYNC_gettravstate(ON, kvdram2, globalparamsK[2], currentLOP, sourcestatsmarker);
		for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ ntravszs += rtravstate[i].size_kvs; }
		// for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ cout<<"acts::loadsrcvs: rtravstate["<<i<<"].size_kvs: "<<rtravstate[i].size_kvs<<endl; } // REMOVEME.
		// if(ntravszs > 0){ cout<<"acts::loadsrcvs: populating sourcev: partition "<<iterationidx<<", ntravszs: "<<ntravszs<<endl; } // REMOVEME.
		
		if(ntravszs > 0){
			for(unsigned int k=(iterationidx * reducebuffersz * FETFACTOR); k<((iterationidx * reducebuffersz * FETFACTOR) + reducebuffersz); k++){ 
			#pragma HLS PIPELINE II=1
				edges0[BASEOFFSETKVS_VERTICESDATA_E + k] = kvdram0[BASEOFFSETKVS_VERTICESDATA_K + k];
				edges1[BASEOFFSETKVS_VERTICESDATA_E + k] = kvdram1[BASEOFFSETKVS_VERTICESDATA_K + k];
				edges2[BASEOFFSETKVS_VERTICESDATA_E + k] = kvdram2[BASEOFFSETKVS_VERTICESDATA_K + k];
			}
		}
		
		sourcestatsmarker += 1;
	}
	
	/* POPULATESRC: for(unsigned int k=0; k<KVDATA_RANGE/2; k++){ // CRITICAL FIXME. OPTIMIZEME.
	#pragma HLS PIPELINE II=1
		edges0[BASEOFFSETKVS_VERTICESDATA_E + k] = kvdram0[BASEOFFSETKVS_VERTICESDATA_K + k];
		edges1[BASEOFFSETKVS_VERTICESDATA_E + k] = kvdram1[BASEOFFSETKVS_VERTICESDATA_K + k];
		edges2[BASEOFFSETKVS_VERTICESDATA_E + k] = kvdram2[BASEOFFSETKVS_VERTICESDATA_K + k];
	} */
	return;
}

extern "C" {
void 
	#ifdef SW 
	acts:: 
	#endif
topkernel(
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	uint512_dt * edges0,
	#endif 
	uint512_dt * kvdram0,
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	uint512_dt * edges1,
	#endif 
	uint512_dt * kvdram1,
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	uint512_dt * edges2,
	#endif 
	uint512_dt * kvdram2,
 
	uint512_dt * vdram){
		
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem3
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem4
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem5
#else 
#pragma HLS INTERFACE m_axi port = edges0 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = edges1 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = edges2 offset = slave bundle = gmem2
	
#endif 

#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem6

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges0 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges1 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS INTERFACE s_axilite port = edges2 bundle = control
#endif 
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control

#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges0
#endif 
#pragma HLS DATA_PACK variable = kvdram0
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges1
#endif 
#pragma HLS DATA_PACK variable = kvdram1
#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
#pragma HLS DATA_PACK variable = edges2
#endif 
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS3) || defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int numIters = actsprocobj.GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0);
	GRAPHITER_LOOP: for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp(ACTS_1by1): Iteration: "<<GraphIter<<endl;
		
		// preparing variables
		#ifdef _WIDEWORD
		unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].range(31, 0);
 
		kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
 
		kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].range(31, 0) = GraphIter;
		#else
		unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
 
		kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
		#endif 
		
		/* // run acts
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
actsprocobj.topkernelproc(edges0, kvdram0);	
actsprocobj.topkernelproc(edges0, kvdram1);	
actsprocobj.topkernelproc(edges0, kvdram2);	
		#else 
actsprocobj.topkernelproc(kvdram0);	
actsprocobj.topkernelproc(kvdram1);	
actsprocobj.topkernelproc(kvdram2);	
	
		#endif 
actssyncobj.topkernelsync(kvdram0,kvdram1,kvdram2, vdram);
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		loadsrcvs( edges0, kvdram0,  edges1, kvdram1,  edges2, kvdram2,  vdram);
		#endif  */
		
		// checking for exit 
		unsigned int totalactvvp = 0;
		#ifdef SW 
		cout<<endl<<"active partitions for iteration "<<GraphIter+1<<": ";
		#endif 
		for(unsigned int i=0; i<256; i++){
			#ifdef _WIDEWORD
			unsigned int gmask = kvdram0[_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].range(31, 0);
			#else 
			unsigned int gmask = kvdram0[_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].data[0].key;
			#endif 
			totalactvvp += gmask;
			#ifdef SW 
			if(gmask > 0){ cout<<i<<", "; }
			#endif 
		}
		#ifdef SW 
		cout<<""<<endl;
		if(totalactvvp == 0){ cout<<"swkernel::runapp: no more active vertices to process. exiting... "<<endl; break; }
		#endif 
	}
	return;
}
}
#endif



