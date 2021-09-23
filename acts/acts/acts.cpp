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
#include "actsproc.h"
#include "actssync.h"
using namespace std;

actsproc actsprocobj;
actssync actssyncobj;

#ifdef SW
acts::acts(){}
acts::~acts(){}
#endif

extern "C" {
void 
	#ifdef SW 
	acts:: 
	#endif
topkernel(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11,uint512_dt * kvdram12,uint512_dt * kvdram13,uint512_dt * kvdram14,uint512_dt * kvdram15,uint512_dt * kvdram16,uint512_dt * kvdram17,uint512_dt * kvdram18,uint512_dt * kvdram19,uint512_dt * kvdram20,uint512_dt * kvdram21,uint512_dt * kvdram22,uint512_dt * kvdram23,uint512_dt * kvdram24,uint512_dt * kvdram25,uint512_dt * kvdram26,uint512_dt * kvdram27,uint512_dt * kvdram28,uint512_dt * kvdram29,uint512_dt * kvdram30,uint512_dt * kvdram31, uint512_dt * vdram){
 
#pragma HLS INTERFACE m_axi port = kvdram0 offset = slave bundle = gmem0
 
#pragma HLS INTERFACE m_axi port = kvdram1 offset = slave bundle = gmem1
 
#pragma HLS INTERFACE m_axi port = kvdram2 offset = slave bundle = gmem2
 
#pragma HLS INTERFACE m_axi port = kvdram3 offset = slave bundle = gmem3
 
#pragma HLS INTERFACE m_axi port = kvdram4 offset = slave bundle = gmem4
 
#pragma HLS INTERFACE m_axi port = kvdram5 offset = slave bundle = gmem5
 
#pragma HLS INTERFACE m_axi port = kvdram6 offset = slave bundle = gmem6
 
#pragma HLS INTERFACE m_axi port = kvdram7 offset = slave bundle = gmem7
 
#pragma HLS INTERFACE m_axi port = kvdram8 offset = slave bundle = gmem8
 
#pragma HLS INTERFACE m_axi port = kvdram9 offset = slave bundle = gmem9
 
#pragma HLS INTERFACE m_axi port = kvdram10 offset = slave bundle = gmem10
 
#pragma HLS INTERFACE m_axi port = kvdram11 offset = slave bundle = gmem11
 
#pragma HLS INTERFACE m_axi port = kvdram12 offset = slave bundle = gmem12
 
#pragma HLS INTERFACE m_axi port = kvdram13 offset = slave bundle = gmem13
 
#pragma HLS INTERFACE m_axi port = kvdram14 offset = slave bundle = gmem14
 
#pragma HLS INTERFACE m_axi port = kvdram15 offset = slave bundle = gmem15
 
#pragma HLS INTERFACE m_axi port = kvdram16 offset = slave bundle = gmem16
 
#pragma HLS INTERFACE m_axi port = kvdram17 offset = slave bundle = gmem17
 
#pragma HLS INTERFACE m_axi port = kvdram18 offset = slave bundle = gmem18
 
#pragma HLS INTERFACE m_axi port = kvdram19 offset = slave bundle = gmem19
 
#pragma HLS INTERFACE m_axi port = kvdram20 offset = slave bundle = gmem20
 
#pragma HLS INTERFACE m_axi port = kvdram21 offset = slave bundle = gmem21
 
#pragma HLS INTERFACE m_axi port = kvdram22 offset = slave bundle = gmem22
 
#pragma HLS INTERFACE m_axi port = kvdram23 offset = slave bundle = gmem23
 
#pragma HLS INTERFACE m_axi port = kvdram24 offset = slave bundle = gmem24
 
#pragma HLS INTERFACE m_axi port = kvdram25 offset = slave bundle = gmem25
 
#pragma HLS INTERFACE m_axi port = kvdram26 offset = slave bundle = gmem26
 
#pragma HLS INTERFACE m_axi port = kvdram27 offset = slave bundle = gmem27
 
#pragma HLS INTERFACE m_axi port = kvdram28 offset = slave bundle = gmem28
 
#pragma HLS INTERFACE m_axi port = kvdram29 offset = slave bundle = gmem29
 
#pragma HLS INTERFACE m_axi port = kvdram30 offset = slave bundle = gmem30
 
#pragma HLS INTERFACE m_axi port = kvdram31 offset = slave bundle = gmem31
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem32

#pragma HLS INTERFACE s_axilite port = kvdram0 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram1 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram2 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram3 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram4 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram5 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram6 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram7 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram8 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram9 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram10 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram11 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram12 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram13 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram14 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram15 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram16 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram17 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram18 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram19 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram20 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram21 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram22 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram23 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram24 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram25 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram26 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram27 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram28 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram29 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram30 bundle = control
#pragma HLS INTERFACE s_axilite port = kvdram31 bundle = control
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = kvdram0
#pragma HLS DATA_PACK variable = kvdram1
#pragma HLS DATA_PACK variable = kvdram2
#pragma HLS DATA_PACK variable = kvdram3
#pragma HLS DATA_PACK variable = kvdram4
#pragma HLS DATA_PACK variable = kvdram5
#pragma HLS DATA_PACK variable = kvdram6
#pragma HLS DATA_PACK variable = kvdram7
#pragma HLS DATA_PACK variable = kvdram8
#pragma HLS DATA_PACK variable = kvdram9
#pragma HLS DATA_PACK variable = kvdram10
#pragma HLS DATA_PACK variable = kvdram11
#pragma HLS DATA_PACK variable = kvdram12
#pragma HLS DATA_PACK variable = kvdram13
#pragma HLS DATA_PACK variable = kvdram14
#pragma HLS DATA_PACK variable = kvdram15
#pragma HLS DATA_PACK variable = kvdram16
#pragma HLS DATA_PACK variable = kvdram17
#pragma HLS DATA_PACK variable = kvdram18
#pragma HLS DATA_PACK variable = kvdram19
#pragma HLS DATA_PACK variable = kvdram20
#pragma HLS DATA_PACK variable = kvdram21
#pragma HLS DATA_PACK variable = kvdram22
#pragma HLS DATA_PACK variable = kvdram23
#pragma HLS DATA_PACK variable = kvdram24
#pragma HLS DATA_PACK variable = kvdram25
#pragma HLS DATA_PACK variable = kvdram26
#pragma HLS DATA_PACK variable = kvdram27
#pragma HLS DATA_PACK variable = kvdram28
#pragma HLS DATA_PACK variable = kvdram29
#pragma HLS DATA_PACK variable = kvdram30
#pragma HLS DATA_PACK variable = kvdram31
#pragma HLS DATA_PACK variable = vdram

	#if defined(_DEBUGMODE_KERNELPRINTS) || defined(ALLVERTEXISACTIVE_ALGORITHM)
	cout<<">>> ====================== Light weight ACTS (PR & SYNC) Launched... ====================== "<<endl; 
	#endif
	
	unsigned int numIters = actsprocobj.GETKEYENTRY(kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID], 0); // kvdram[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key;
	
	for(unsigned int GraphIter=0; GraphIter<numIters; GraphIter++){
		cout<<">>> swkernel::runapp(ACTS_1by1): Iteration: "<<GraphIter<<endl;
		
 
		kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram1[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram2[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram3[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram4[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram5[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram6[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram7[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram8[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram9[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram10[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram11[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram12[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram13[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram14[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram15[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram16[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram17[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram18[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram19[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram20[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram21[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram22[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram23[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram24[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram25[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram26[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram27[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram28[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram29[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram30[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
 
		kvdram31[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
		
 
		actsprocobj.topkernelproc(kvdram0);
 
		actsprocobj.topkernelproc(kvdram1);
 
		actsprocobj.topkernelproc(kvdram2);
 
		actsprocobj.topkernelproc(kvdram3);
 
		actsprocobj.topkernelproc(kvdram4);
 
		actsprocobj.topkernelproc(kvdram5);
 
		actsprocobj.topkernelproc(kvdram6);
 
		actsprocobj.topkernelproc(kvdram7);
 
		actsprocobj.topkernelproc(kvdram8);
 
		actsprocobj.topkernelproc(kvdram9);
 
		actsprocobj.topkernelproc(kvdram10);
 
		actsprocobj.topkernelproc(kvdram11);
 
		actsprocobj.topkernelproc(kvdram12);
 
		actsprocobj.topkernelproc(kvdram13);
 
		actsprocobj.topkernelproc(kvdram14);
 
		actsprocobj.topkernelproc(kvdram15);
 
		actsprocobj.topkernelproc(kvdram16);
 
		actsprocobj.topkernelproc(kvdram17);
 
		actsprocobj.topkernelproc(kvdram18);
 
		actsprocobj.topkernelproc(kvdram19);
 
		actsprocobj.topkernelproc(kvdram20);
 
		actsprocobj.topkernelproc(kvdram21);
 
		actsprocobj.topkernelproc(kvdram22);
 
		actsprocobj.topkernelproc(kvdram23);
 
		actsprocobj.topkernelproc(kvdram24);
 
		actsprocobj.topkernelproc(kvdram25);
 
		actsprocobj.topkernelproc(kvdram26);
 
		actsprocobj.topkernelproc(kvdram27);
 
		actsprocobj.topkernelproc(kvdram28);
 
		actsprocobj.topkernelproc(kvdram29);
 
		actsprocobj.topkernelproc(kvdram30);
 
		actsprocobj.topkernelproc(kvdram31);
		
		actssyncobj.topkernelsync(kvdram0,kvdram1,kvdram2,kvdram3,kvdram4,kvdram5,kvdram6,kvdram7,kvdram8,kvdram9,kvdram10,kvdram11,kvdram12,kvdram13,kvdram14,kvdram15,kvdram16,kvdram17,kvdram18,kvdram19,kvdram20,kvdram21,kvdram22,kvdram23,kvdram24,kvdram25,kvdram26,kvdram27,kvdram28,kvdram29,kvdram30,kvdram31, vdram);
		
		unsigned int _BASEOFFSETKVS_VERTICESPARTITIONMASK = kvdram0[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
		unsigned int totalactvvp = 0;
		cout<<endl<<"active partitions for iteration "<<GraphIter+1<<": ";
		for(unsigned int i=0; i<256; i++){
			unsigned int gmask = kvdram0[_BASEOFFSETKVS_VERTICESPARTITIONMASK + i].data[0].key;
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







