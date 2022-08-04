// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Vitis/multiple_kernels/vadd_kernel/krnl_vadd.cpp
// https://github.com/Xilinx/SDAccel_Examples/blob/master/getting_started/dataflow/dataflow_stream_c/src/adder.cpp
// https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/1d19087a2b4aa90fa2d86cf556aa883d3b413247/Dataflow/Channels/using_fifos/diamond.cpp  *

void acts_all::rearrangeLayoutVx16F(unsigned int s, keyvalue_t in[EDGEDATA_PACKINGSIZE], keyvalue_t out[EDGEDATA_PACKINGSIZE]){
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("rearrangeLayoutVx16F(1)", s, UPDATEDATA_PACKINGSIZE, NAp, NAp, NAp);
	#endif
	unsigned int s_ = s;
	if(s_==0){ 
		out[0] = in[0]; 
		out[1] = in[1]; 
		out[2] = in[2]; 
		out[3] = in[3]; 
		out[4] = in[4]; 
		out[5] = in[5]; 
		out[6] = in[6]; 
		out[7] = in[7]; 
		out[8] = in[8]; 
		out[9] = in[9]; 
		out[10] = in[10]; 
		out[11] = in[11]; 
		out[12] = in[12]; 
		out[13] = in[13]; 
		out[14] = in[14]; 
		out[15] = in[15]; 
	} else if(s_==1){ 
		out[15] = in[0]; 
		out[0] = in[1]; 
		out[1] = in[2]; 
		out[2] = in[3]; 
		out[3] = in[4]; 
		out[4] = in[5]; 
		out[5] = in[6]; 
		out[6] = in[7]; 
		out[7] = in[8]; 
		out[8] = in[9]; 
		out[9] = in[10]; 
		out[10] = in[11]; 
		out[11] = in[12]; 
		out[12] = in[13]; 
		out[13] = in[14]; 
		out[14] = in[15]; 
	} else if(s_==2){ 
		out[14] = in[0]; 
		out[15] = in[1]; 
		out[0] = in[2]; 
		out[1] = in[3]; 
		out[2] = in[4]; 
		out[3] = in[5]; 
		out[4] = in[6]; 
		out[5] = in[7]; 
		out[6] = in[8]; 
		out[7] = in[9]; 
		out[8] = in[10]; 
		out[9] = in[11]; 
		out[10] = in[12]; 
		out[11] = in[13]; 
		out[12] = in[14]; 
		out[13] = in[15]; 
	} else if(s_==3){ 
		out[13] = in[0]; 
		out[14] = in[1]; 
		out[15] = in[2]; 
		out[0] = in[3]; 
		out[1] = in[4]; 
		out[2] = in[5]; 
		out[3] = in[6]; 
		out[4] = in[7]; 
		out[5] = in[8]; 
		out[6] = in[9]; 
		out[7] = in[10]; 
		out[8] = in[11]; 
		out[9] = in[12]; 
		out[10] = in[13]; 
		out[11] = in[14]; 
		out[12] = in[15]; 
	} else if(s_==4){ 
		out[12] = in[0]; 
		out[13] = in[1]; 
		out[14] = in[2]; 
		out[15] = in[3]; 
		out[0] = in[4]; 
		out[1] = in[5]; 
		out[2] = in[6]; 
		out[3] = in[7]; 
		out[4] = in[8]; 
		out[5] = in[9]; 
		out[6] = in[10]; 
		out[7] = in[11]; 
		out[8] = in[12]; 
		out[9] = in[13]; 
		out[10] = in[14]; 
		out[11] = in[15]; 
	} else if(s_==5){ 
		out[11] = in[0]; 
		out[12] = in[1]; 
		out[13] = in[2]; 
		out[14] = in[3]; 
		out[15] = in[4]; 
		out[0] = in[5]; 
		out[1] = in[6]; 
		out[2] = in[7]; 
		out[3] = in[8]; 
		out[4] = in[9]; 
		out[5] = in[10]; 
		out[6] = in[11]; 
		out[7] = in[12]; 
		out[8] = in[13]; 
		out[9] = in[14]; 
		out[10] = in[15]; 
	} else if(s_==6){ 
		out[10] = in[0]; 
		out[11] = in[1]; 
		out[12] = in[2]; 
		out[13] = in[3]; 
		out[14] = in[4]; 
		out[15] = in[5]; 
		out[0] = in[6]; 
		out[1] = in[7]; 
		out[2] = in[8]; 
		out[3] = in[9]; 
		out[4] = in[10]; 
		out[5] = in[11]; 
		out[6] = in[12]; 
		out[7] = in[13]; 
		out[8] = in[14]; 
		out[9] = in[15]; 
	} else if(s_==7){ 
		out[9] = in[0]; 
		out[10] = in[1]; 
		out[11] = in[2]; 
		out[12] = in[3]; 
		out[13] = in[4]; 
		out[14] = in[5]; 
		out[15] = in[6]; 
		out[0] = in[7]; 
		out[1] = in[8]; 
		out[2] = in[9]; 
		out[3] = in[10]; 
		out[4] = in[11]; 
		out[5] = in[12]; 
		out[6] = in[13]; 
		out[7] = in[14]; 
		out[8] = in[15]; 
	} else if(s_==8){ 
		out[8] = in[0]; 
		out[9] = in[1]; 
		out[10] = in[2]; 
		out[11] = in[3]; 
		out[12] = in[4]; 
		out[13] = in[5]; 
		out[14] = in[6]; 
		out[15] = in[7]; 
		out[0] = in[8]; 
		out[1] = in[9]; 
		out[2] = in[10]; 
		out[3] = in[11]; 
		out[4] = in[12]; 
		out[5] = in[13]; 
		out[6] = in[14]; 
		out[7] = in[15]; 
	} else if(s_==9){ 
		out[7] = in[0]; 
		out[8] = in[1]; 
		out[9] = in[2]; 
		out[10] = in[3]; 
		out[11] = in[4]; 
		out[12] = in[5]; 
		out[13] = in[6]; 
		out[14] = in[7]; 
		out[15] = in[8]; 
		out[0] = in[9]; 
		out[1] = in[10]; 
		out[2] = in[11]; 
		out[3] = in[12]; 
		out[4] = in[13]; 
		out[5] = in[14]; 
		out[6] = in[15]; 
	} else if(s_==10){ 
		out[6] = in[0]; 
		out[7] = in[1]; 
		out[8] = in[2]; 
		out[9] = in[3]; 
		out[10] = in[4]; 
		out[11] = in[5]; 
		out[12] = in[6]; 
		out[13] = in[7]; 
		out[14] = in[8]; 
		out[15] = in[9]; 
		out[0] = in[10]; 
		out[1] = in[11]; 
		out[2] = in[12]; 
		out[3] = in[13]; 
		out[4] = in[14]; 
		out[5] = in[15]; 
	} else if(s_==11){ 
		out[5] = in[0]; 
		out[6] = in[1]; 
		out[7] = in[2]; 
		out[8] = in[3]; 
		out[9] = in[4]; 
		out[10] = in[5]; 
		out[11] = in[6]; 
		out[12] = in[7]; 
		out[13] = in[8]; 
		out[14] = in[9]; 
		out[15] = in[10]; 
		out[0] = in[11]; 
		out[1] = in[12]; 
		out[2] = in[13]; 
		out[3] = in[14]; 
		out[4] = in[15]; 
	} else if(s_==12){ 
		out[4] = in[0]; 
		out[5] = in[1]; 
		out[6] = in[2]; 
		out[7] = in[3]; 
		out[8] = in[4]; 
		out[9] = in[5]; 
		out[10] = in[6]; 
		out[11] = in[7]; 
		out[12] = in[8]; 
		out[13] = in[9]; 
		out[14] = in[10]; 
		out[15] = in[11]; 
		out[0] = in[12]; 
		out[1] = in[13]; 
		out[2] = in[14]; 
		out[3] = in[15]; 
	} else if(s_==13){ 
		out[3] = in[0]; 
		out[4] = in[1]; 
		out[5] = in[2]; 
		out[6] = in[3]; 
		out[7] = in[4]; 
		out[8] = in[5]; 
		out[9] = in[6]; 
		out[10] = in[7]; 
		out[11] = in[8]; 
		out[12] = in[9]; 
		out[13] = in[10]; 
		out[14] = in[11]; 
		out[15] = in[12]; 
		out[0] = in[13]; 
		out[1] = in[14]; 
		out[2] = in[15]; 
	} else if(s_==14){ 
		out[2] = in[0]; 
		out[3] = in[1]; 
		out[4] = in[2]; 
		out[5] = in[3]; 
		out[6] = in[4]; 
		out[7] = in[5]; 
		out[8] = in[6]; 
		out[9] = in[7]; 
		out[10] = in[8]; 
		out[11] = in[9]; 
		out[12] = in[10]; 
		out[13] = in[11]; 
		out[14] = in[12]; 
		out[15] = in[13]; 
		out[0] = in[14]; 
		out[1] = in[15]; 
	} else { 
		out[1] = in[0]; 
		out[2] = in[1]; 
		out[3] = in[2]; 
		out[4] = in[3]; 
		out[5] = in[4]; 
		out[6] = in[5]; 
		out[7] = in[6]; 
		out[8] = in[7]; 
		out[9] = in[8]; 
		out[10] = in[9]; 
		out[11] = in[10]; 
		out[12] = in[11]; 
		out[13] = in[12]; 
		out[14] = in[13]; 
		out[15] = in[14]; 
		out[0] = in[15]; 
	}
	return;
}	
	
void acts_all::rearrangeLayoutVx16B(unsigned int s, keyvalue_t in[EDGEDATA_PACKINGSIZE], keyvalue_t out[EDGEDATA_PACKINGSIZE]){
	unsigned int s_ = s;
	#ifdef _DEBUGMODE_CHECKS3
	actsutilityobj->checkoutofbounds("rearrangeLayoutVx16B(1)", s, UPDATEDATA_PACKINGSIZE, NAp, NAp, NAp);
	#endif
 if(s_==0){ 
		out[0] = in[0]; 
		out[1] = in[1]; 
		out[2] = in[2]; 
		out[3] = in[3]; 
		out[4] = in[4]; 
		out[5] = in[5]; 
		out[6] = in[6]; 
		out[7] = in[7]; 
		out[8] = in[8]; 
		out[9] = in[9]; 
		out[10] = in[10]; 
		out[11] = in[11]; 
		out[12] = in[12]; 
		out[13] = in[13]; 
		out[14] = in[14]; 
		out[15] = in[15]; 
	}
else if(s_==1){ 
		out[1] = in[0]; 
		out[2] = in[1]; 
		out[3] = in[2]; 
		out[4] = in[3]; 
		out[5] = in[4]; 
		out[6] = in[5]; 
		out[7] = in[6]; 
		out[8] = in[7]; 
		out[9] = in[8]; 
		out[10] = in[9]; 
		out[11] = in[10]; 
		out[12] = in[11]; 
		out[13] = in[12]; 
		out[14] = in[13]; 
		out[15] = in[14]; 
		out[0] = in[15]; 
	}
else if(s_==2){ 
		out[2] = in[0]; 
		out[3] = in[1]; 
		out[4] = in[2]; 
		out[5] = in[3]; 
		out[6] = in[4]; 
		out[7] = in[5]; 
		out[8] = in[6]; 
		out[9] = in[7]; 
		out[10] = in[8]; 
		out[11] = in[9]; 
		out[12] = in[10]; 
		out[13] = in[11]; 
		out[14] = in[12]; 
		out[15] = in[13]; 
		out[0] = in[14]; 
		out[1] = in[15]; 
	}
else if(s_==3){ 
		out[3] = in[0]; 
		out[4] = in[1]; 
		out[5] = in[2]; 
		out[6] = in[3]; 
		out[7] = in[4]; 
		out[8] = in[5]; 
		out[9] = in[6]; 
		out[10] = in[7]; 
		out[11] = in[8]; 
		out[12] = in[9]; 
		out[13] = in[10]; 
		out[14] = in[11]; 
		out[15] = in[12]; 
		out[0] = in[13]; 
		out[1] = in[14]; 
		out[2] = in[15]; 
	}
else if(s_==4){ 
		out[4] = in[0]; 
		out[5] = in[1]; 
		out[6] = in[2]; 
		out[7] = in[3]; 
		out[8] = in[4]; 
		out[9] = in[5]; 
		out[10] = in[6]; 
		out[11] = in[7]; 
		out[12] = in[8]; 
		out[13] = in[9]; 
		out[14] = in[10]; 
		out[15] = in[11]; 
		out[0] = in[12]; 
		out[1] = in[13]; 
		out[2] = in[14]; 
		out[3] = in[15]; 
	}
else if(s_==5){ 
		out[5] = in[0]; 
		out[6] = in[1]; 
		out[7] = in[2]; 
		out[8] = in[3]; 
		out[9] = in[4]; 
		out[10] = in[5]; 
		out[11] = in[6]; 
		out[12] = in[7]; 
		out[13] = in[8]; 
		out[14] = in[9]; 
		out[15] = in[10]; 
		out[0] = in[11]; 
		out[1] = in[12]; 
		out[2] = in[13]; 
		out[3] = in[14]; 
		out[4] = in[15]; 
	}
else if(s_==6){ 
		out[6] = in[0]; 
		out[7] = in[1]; 
		out[8] = in[2]; 
		out[9] = in[3]; 
		out[10] = in[4]; 
		out[11] = in[5]; 
		out[12] = in[6]; 
		out[13] = in[7]; 
		out[14] = in[8]; 
		out[15] = in[9]; 
		out[0] = in[10]; 
		out[1] = in[11]; 
		out[2] = in[12]; 
		out[3] = in[13]; 
		out[4] = in[14]; 
		out[5] = in[15]; 
	}
else if(s_==7){ 
		out[7] = in[0]; 
		out[8] = in[1]; 
		out[9] = in[2]; 
		out[10] = in[3]; 
		out[11] = in[4]; 
		out[12] = in[5]; 
		out[13] = in[6]; 
		out[14] = in[7]; 
		out[15] = in[8]; 
		out[0] = in[9]; 
		out[1] = in[10]; 
		out[2] = in[11]; 
		out[3] = in[12]; 
		out[4] = in[13]; 
		out[5] = in[14]; 
		out[6] = in[15]; 
	}
else if(s_==8){ 
		out[8] = in[0]; 
		out[9] = in[1]; 
		out[10] = in[2]; 
		out[11] = in[3]; 
		out[12] = in[4]; 
		out[13] = in[5]; 
		out[14] = in[6]; 
		out[15] = in[7]; 
		out[0] = in[8]; 
		out[1] = in[9]; 
		out[2] = in[10]; 
		out[3] = in[11]; 
		out[4] = in[12]; 
		out[5] = in[13]; 
		out[6] = in[14]; 
		out[7] = in[15]; 
	}
else if(s_==9){ 
		out[9] = in[0]; 
		out[10] = in[1]; 
		out[11] = in[2]; 
		out[12] = in[3]; 
		out[13] = in[4]; 
		out[14] = in[5]; 
		out[15] = in[6]; 
		out[0] = in[7]; 
		out[1] = in[8]; 
		out[2] = in[9]; 
		out[3] = in[10]; 
		out[4] = in[11]; 
		out[5] = in[12]; 
		out[6] = in[13]; 
		out[7] = in[14]; 
		out[8] = in[15]; 
	}
else if(s_==10){ 
		out[10] = in[0]; 
		out[11] = in[1]; 
		out[12] = in[2]; 
		out[13] = in[3]; 
		out[14] = in[4]; 
		out[15] = in[5]; 
		out[0] = in[6]; 
		out[1] = in[7]; 
		out[2] = in[8]; 
		out[3] = in[9]; 
		out[4] = in[10]; 
		out[5] = in[11]; 
		out[6] = in[12]; 
		out[7] = in[13]; 
		out[8] = in[14]; 
		out[9] = in[15]; 
	}
else if(s_==11){ 
		out[11] = in[0]; 
		out[12] = in[1]; 
		out[13] = in[2]; 
		out[14] = in[3]; 
		out[15] = in[4]; 
		out[0] = in[5]; 
		out[1] = in[6]; 
		out[2] = in[7]; 
		out[3] = in[8]; 
		out[4] = in[9]; 
		out[5] = in[10]; 
		out[6] = in[11]; 
		out[7] = in[12]; 
		out[8] = in[13]; 
		out[9] = in[14]; 
		out[10] = in[15]; 
	}
else if(s_==12){ 
		out[12] = in[0]; 
		out[13] = in[1]; 
		out[14] = in[2]; 
		out[15] = in[3]; 
		out[0] = in[4]; 
		out[1] = in[5]; 
		out[2] = in[6]; 
		out[3] = in[7]; 
		out[4] = in[8]; 
		out[5] = in[9]; 
		out[6] = in[10]; 
		out[7] = in[11]; 
		out[8] = in[12]; 
		out[9] = in[13]; 
		out[10] = in[14]; 
		out[11] = in[15]; 
	}
else if(s_==13){ 
		out[13] = in[0]; 
		out[14] = in[1]; 
		out[15] = in[2]; 
		out[0] = in[3]; 
		out[1] = in[4]; 
		out[2] = in[5]; 
		out[3] = in[6]; 
		out[4] = in[7]; 
		out[5] = in[8]; 
		out[6] = in[9]; 
		out[7] = in[10]; 
		out[8] = in[11]; 
		out[9] = in[12]; 
		out[10] = in[13]; 
		out[11] = in[14]; 
		out[12] = in[15]; 
	}
else if(s_==14){ 
		out[14] = in[0]; 
		out[15] = in[1]; 
		out[0] = in[2]; 
		out[1] = in[3]; 
		out[2] = in[4]; 
		out[3] = in[5]; 
		out[4] = in[6]; 
		out[5] = in[7]; 
		out[6] = in[8]; 
		out[7] = in[9]; 
		out[8] = in[10]; 
		out[9] = in[11]; 
		out[10] = in[12]; 
		out[11] = in[13]; 
		out[12] = in[14]; 
		out[13] = in[15]; 
	}
else { 
		out[15] = in[0]; 
		out[0] = in[1]; 
		out[1] = in[2]; 
		out[2] = in[3]; 
		out[3] = in[4]; 
		out[4] = in[5]; 
		out[5] = in[6]; 
		out[6] = in[7]; 
		out[7] = in[8]; 
		out[8] = in[9]; 
		out[9] = in[10]; 
		out[10] = in[11]; 
		out[11] = in[12]; 
		out[12] = in[13]; 
		out[13] = in[14]; 
		out[14] = in[15]; 
	}
	return;
}

void acts_all::check_if_contiguous(keyvalue_t keyvalue[EDGEDATA_PACKINGSIZE], keyvalue_t msg1[EDGEDATA_PACKINGSIZE], keyvalue_t msg2[EDGEDATA_PACKINGSIZE], unsigned int msg1_str, unsigned int msg2_str, unsigned int msg3_str){
	#ifdef _DEBUGMODE_CHECKS3
	for(int v = 0; v < UPDATEDATA_PACKINGSIZE; v++){ 
		if(keyvalue[v].key != INVALIDDATA){ 
			if(keyvalue[v].key % UPDATEDATA_PACKINGSIZE != v){ 
				cout<<"check_if_contiguous(1). ERROR 34. v["<<v<<"].key("<<keyvalue[v].key<<") % UPDATEDATA_PACKINGSIZE("<<UPDATEDATA_PACKINGSIZE<<")(="<<keyvalue[v].key % UPDATEDATA_PACKINGSIZE<<") != v("<<v<<"). [msg1_str: "<<msg1_str<<", msg2_str: "<<msg2_str<<", msg3_str: "<<msg3_str<<"] EXITING..."<<endl; 
				for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){ cout<<"+++ keyvalue["<<v<<"].key: "<<keyvalue[v].key<<"("<<keyvalue[v].key%EDGEDATA_PACKINGSIZE<<"), keyvalue["<<v<<"].value: "<<keyvalue[v].value<<"("<<keyvalue[v].value%EDGEDATA_PACKINGSIZE<<")"<<endl; }
				for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){ cout<<"+++ msg1["<<v<<"].key: "<<msg1[v].key<<"("<<msg1[v].key%EDGEDATA_PACKINGSIZE<<"), msg1["<<v<<"].value: "<<msg1[v].value<<"("<<msg1[v].value%EDGEDATA_PACKINGSIZE<<")"<<endl; }
				for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){ cout<<"+++ msg2["<<v<<"].key: "<<msg2[v].key<<"("<<msg2[v].key%EDGEDATA_PACKINGSIZE<<"), msg2["<<v<<"].value: "<<msg2[v].value<<"("<<msg2[v].value%EDGEDATA_PACKINGSIZE<<")"<<endl; }
				exit(EXIT_FAILURE); 
			}
		}
	}
	#endif
}

keyvalue_t acts_all::process_edge(unsigned int mode, bool enx, unsigned int v, unsigned int loc, keyvalue_t edge_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparams){				
	#pragma HLS INLINE
	
	// flag 
	// if(loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION){ loc = 0; }
	// bool special_loc = false; if(loc == 16383){ loc = 0; special_loc = true; } // header information
	#ifdef _DEBUGMODE_CHECKS3
	// if(enx == true && loc >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION && loc != 16383){ cout<<"processvector::ERROR SEEN @@ loc("<<loc<<") >= globalparams.SIZEKVS2_PROCESSEDGESPARTITION("<<globalparams.SIZEKVS2_PROCESSEDGESPARTITION<<"). edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<", mode: "<<mode<<". EXITING... "<<endl; exit(EXIT_FAILURE); }					
	if(enx == true && loc >= MAX_BLOCKRAM_VDESTDATA_SIZE && loc != 16383){ cout<<"processvector::ERROR SEEN @@ loc("<<loc<<") >= MAX_BLOCKRAM_VDESTDATA_SIZE("<<MAX_BLOCKRAM_VDESTDATA_SIZE<<"). edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", v: "<<v<<", INVALIDDATA: "<<INVALIDDATA<<", mode: "<<mode<<". EXITING... "<<endl; exit(EXIT_FAILURE); }					
	#endif 
	
	// read
	value_t combo = 0; if(enx == true && loc != 16383){ combo = vbuffer[loc].data; }
	value_t mask; if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ mask = 1; } else { mask = combo & 0x1; }
	value_t udata = combo >> 1; 
	#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
	if(mask == 1 && mode == ACTSPROCESSMODE){ cout<<">>> PROCESS VECTOR:: PROCESS EDGE SEEN @ v: "<<v<<", loc: "<<loc<<", edge_data.key: "<<edge_data.key<<", edge_data.value: "<<edge_data.value<<", udata: "<<udata<<", mask: "<<mask<<", srcvid: "<<(edge_data.value * EDGEDATA_PACKINGSIZE) + v<<", dstvid*: "<<UTILP0_GETREALVID(edge_data.key, globalparams.ACTSPARAMS_INSTID)<<", ldstvid: "<<edge_data.key<<endl; }
	#endif
	
	// process
	keyvalue_t vupdate; 
	if(mode == ACTSPROCESSMODE){
		value_t res = process_func(udata, 1, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		vupdate; if(mask == 1 && loc != 16383){ vupdate.key = edge_data.key; vupdate.value = res; } else { vupdate.key = INVALIDDATA; vupdate.value = INVALIDDATA; }
	} else {
		vupdate.key = combo; vupdate.value = combo;
	}
	return vupdate;
}

void acts_all::reduce_update(unsigned int mode, bool enx, unsigned int v, unsigned int loc, keyvalue_t update_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int stats[BLOCKRAM_SIZE], unsigned int memory[1], globalparams_t globalparams){
	#pragma HLS INLINE

	// flag 
	bool en = true;
	
	// checks
	#ifdef _DEBUGMODE_CHECKS3
	if(enx == true && loc >= (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE)){ cout<<"reduce_update::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION("<<globalparams.SIZEKVS2_REDUCEPARTITION<<"). update_data.key: "<<update_data.key<<", update_data.value: "<<update_data.value<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	// if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION && en == true){ loc = 0; } 
	// if(enx == true && mode == ACTSREDUCEMODE){ if(loc == memory[0]){ cout<<"reduce_update:: Error. loc: "<<loc<<", memory[0]: "<<memory[0]<<", mode: "<<mode<<endl; exit(EXIT_FAILURE); }} // CRITICAL FIXME.
	// if(enx == true && mode == ACTSREDUCEMODE){ if(loc == memory[0]){ loc = (loc + 1) % 8; } memory[0] = loc; } // FIXME.
	#ifdef _DEBUGMODE_CHECKS3
	if(enx == true){ actsutilityobj->checkoutofbounds("reducevector(114)::DEBUG CODE 113::1", loc, MAX_BLOCKRAM_VDESTDATA_SIZE, update_data.key, update_data.value, mode); }
	#endif

	// read & reduce 
	value_t new_combo; value_t mask; value_t vdata_tmp; value_t new_vprop;
	if(mode == ACTSREDUCEMODE){ 
		value_t combo = 0;
		if(enx == true){ combo = vbuffer[loc].data; }
		if(globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ mask = 1; } else { mask = combo & 0x1; }
		vdata_tmp = combo >> 1; 
		
		new_vprop = reduce_func(vdata_tmp, vdata_tmp, update_data.value, globalparams.ALGORITHMINFO_GRAPHITERATIONID, globalparams.ALGORITHMINFO_GRAPHALGORITHMID);
		new_combo = (new_vprop << 1) | 0x1;
		if(enx == true && new_vprop != vdata_tmp){ en = true; } else { en = false; }
	} else { new_combo = update_data.value; }
	
	// write-back
	if(en == true){ // REMOVEME.
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3  // REMOVEME.
		if(mode == ACTSREDUCEMODE){ cout<<">>> REDUCE VECTOR:: REDUCE UPDATE SEEN @: v: "<<v<<", loc: "<<loc<<", vdata_tmp: "<<vdata_tmp<<", mask: "<<mask<<", update_data.key: "<<update_data.key<<", update_data.value: "<<update_data.value<<", new_vprop: "<<new_vprop<<", new combo: "<<((new_vprop << 1) | 0x1)<<", dstvid: "<<UTILP0_GETREALVID(update_data.key, globalparams.ACTSPARAMS_INSTID)<<endl; }
		#endif
		
		vbuffer[loc].data = new_combo;
	}
	return;
}	

#ifdef FPGA_IMPL
void load(uint512_dt *in, hls::stream<uint512_evec_dt >& out, batch_type sourcebaseaddr_kvs, unsigned int srcoffset_kvs, int size){
	Loop_Ld: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		uint512_evec_dt data = UTILP0_ReadEdges(in, sourcebaseaddr_kvs + srcoffset_kvs + i);
		out.write(data);
	}
}

void compute_and_store(unsigned int mode, unsigned int llp_set, hls::stream<uint512_evec_dt >& in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],
					batch_type destbaseaddr_kvs, unsigned int destoffset_kvs, unsigned int destindex, int size,
						unsigned int offset_buffer, // SAVEVPROPERTYMODE parameters
							collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){
	
	keyvalue_t res[UPDATEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas2 complete
	keyvalue_t edges[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = edges complete
	keyvalue_t res_out[EDGEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res_out complete
	keyvalue_t data[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = data complete
	unsigned int index[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = index complete
	bool en[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = en complete
	
	for(int v = 0; v < VECTOR2_SIZE; v++){ en[v] = true; }
	unsigned int offsetkvs_dstvid = llp_set * (globalparamsK.SIZEKVS2_REDUCEPARTITION * EDGEDATA_PACKINGSIZE); // CHECKME.
	
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; //   (1 << (globalparamsK.POW_BATCHRANGE - BLOCKRAM_UPDATEBLOCK_POW)) / VDATA_PACKINGSIZE;
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock;	
	
	ACTIT_COMPUTEANDSTORE_MAINLOOP2A: for (int i = 0; i < size; i++){
	#pragma HLS PIPELINE II=1
		// read
		uint512_evec_dt in_data = in.read();
		
		// decode 
		if(mode == ACTSPROCESSMODE){ 
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(in_data.data[v] != INVALIDDATA){ en[v] = true; } else { en[v] = false; }
			}
			
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(in_data.data[v] != INVALIDDATA){ edges[v].key = in_data.data[v] & MASK_DSTVID; edges[v].value = (in_data.data[v] >> DSTVID_BITSZ) & MASK_SRCVID; } // srcvid is upper, dstvid is lower 
				else { edges[v].key = INVALIDDATA; edges[v].value = INVALIDDATA; }
			}
			
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				data[v] = edges[v]; index[v] = edges[v].value;
			}
		} else {
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				index[v] = offset_buffer + i; data[v].key = datas[v]; data[v].value = datas[v]; en[v] = true;
			}
		}
		
		// process
		for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){
		#pragma HLS UNROLL
			res[v] = process_edge(mode, en[v], v, index[v], data[v], vbuffer_source[v], globalparamsK);
		}	
		
		// encode 
		if(mode == ACTSPROCESSMODE){ 
			// rotateby >>>
			unsigned int rotateby = 0; unsigned int sample_key = INVALIDDATA; unsigned int sample_u = 0; unsigned int rotate_forward = 1; 
			if(edges[0].value == 16383){ rotateby = edges[0].key >> 1; rotate_forward = edges[0].key & 0x1; } 
			else { sample_key = edges[0].key % UPDATEDATA_PACKINGSIZE; sample_u = 0; if(sample_key > sample_u){ rotateby = sample_key - sample_u; rotate_forward = 0; } else { rotateby = sample_u - sample_key; rotate_forward = 1; }}		
			if(rotate_forward == 0){ rearrangeLayoutVx16B(rotateby, res, res_out); } else{ rearrangeLayoutVx16F(rotateby, res, res_out); }
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 59::", rotateby, EDGEDATA_PACKINGSIZE, sample_key, edges[0].value, datas[0]);	// sample_key, edges[0].value, datas[0], sample_u, edges[0].key
			actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 60::", rotate_forward, 2, sample_key, sample_u, edges[0].key);	
			check_if_contiguous(res_out, edges, res, sample_key, rotate_forward, rotateby);
			#endif 
			
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				if(res_out[v].key != INVALIDDATA){ datas2[v] = (res_out[v].value << DSTVID_BITSZ) | res_out[v].key; } else { datas2[v] = INVALIDDATA; }
			}
		} else {
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				datas2[v] = res[v].value; 
			}
		}

		// store
		UTILP0_WriteDatas(out, destbaseaddr_kvs + destoffset_kvs + destindex + i, datas2);
		
		// colelect stats
		if(mode == ACTSPROCESSMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				#ifdef _DEBUGMODE_CHECKS3
				if(res[v].key != INVALIDDATA){ actsutilityobj->checkoutofbounds("write__process(112)::DEBUG CODE 112::1", (offsetkvs_dstvid + edges[v].key) >> (globalparamsK.POW_BATCHRANGE - BLOCKRAM_UPDATEBLOCK_POW), BLOCKRAM_SIZE, edges[v].key, globalparamsK.POW_BATCHRANGE, globalparamsK.NUM_REDUCEPARTITIONS); }
				#endif	
				if(res[v].key != INVALIDDATA){
					// if(res[v].key != INVALIDDATA){ stats[v][((offsetkvs_dstvid + edges[v].key) / EDGEDATA_PACKINGSIZE) / numkvs_vertices_per_updateblock] = 0xFFFFFFFF; } // = vertexid / number of vertices in an update block
					if(res[v].key != INVALIDDATA){ stats[v][((offsetkvs_dstvid + edges[v].key) / VDATA_PACKINGSIZE) / (((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE)] = 0xFFFFFFFF; } // = vertexid / number of vertices in an update block
				}
			}
		}
		
		// collect stats 
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(mode == ACTSPROCESSMODE){ 
			collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
			collections[PROCESSEDGES_COLLECTIONID].data1 += EDGEDATA_PACKINGSIZE; // *** used in PR, CF implementations ONLY (not BFS) ***
		}
		if(en == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE); }
		#endif 
	}
}
#endif 

void acts_all::ACTSP0_write__process(unsigned int mode, unsigned int llp_set, uint512_dt *in, uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], 
			batch_type sourcebaseaddr_kvs, unsigned int srcoffset_kvs, batch_type destbaseaddr_kvs, unsigned int destoffset_kvs, unsigned int destindex, int size,
				unsigned int offset_buffer, // SAVEVPROPERTYMODE parameters
					collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){
	
	keyvalue_t res[UPDATEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas2 complete
	keyvalue_t edges[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = edges complete
	keyvalue_t res_out[EDGEDATA_PACKINGSIZE]; 
	#pragma HLS ARRAY_PARTITION variable = res_out complete
	keyvalue_t data[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = data complete
	unsigned int index[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = index complete
	bool en[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = en complete
	
	for(int v = 0; v < VECTOR2_SIZE; v++){ en[v] = true; }
	unsigned int offsetkvs_dstvid = llp_set * (globalparamsK.SIZEKVS2_REDUCEPARTITION * EDGEDATA_PACKINGSIZE); // CHECKME.
	
	// unsigned int offsetkvs_stats = llp_set * numkvs_vertices_per_updateblock;
	
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; //   (1 << (globalparamsK.POW_BATCHRANGE - BLOCKRAM_UPDATEBLOCK_POW)) / VDATA_PACKINGSIZE;
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock;	
	unsigned int offsetkvs_stats = llp_set * numkvs_vertices_per_updateblock; // NEWCHANGE.
	
	for(int i = 0; i < size; i++){
		// read 
		if(mode == ACTSPROCESSMODE){ 
			UTILP0_ReadEdges(in, sourcebaseaddr_kvs + srcoffset_kvs + i, datas);
		}
		
		// decode 
		if(mode == ACTSPROCESSMODE){ 
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(datas[v] != INVALIDDATA){ en[v] = true; } else { en[v] = false; }
			}
			
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(datas[v] != INVALIDDATA){ edges[v].key = datas[v] & MASK_DSTVID; edges[v].value = (datas[v] >> DSTVID_BITSZ) & MASK_SRCVID; } // srcvid is upper[31-18], dstvid is lower[17-0]
				else { edges[v].key = INVALIDDATA; edges[v].value = INVALIDDATA; }
			}
			
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				data[v] = edges[v]; index[v] = edges[v].value;
			}
		} else {
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				// index[v] = i; data[v].key = datas[v]; data[v].value = datas[v]; en[v] = true;
				index[v] = offset_buffer + i; data[v].key = datas[v]; data[v].value = datas[v]; en[v] = true;
			}
		}
		
		// process
		for(int v = 0; v < EDGEDATA_PACKINGSIZE; v++){
		#pragma HLS UNROLL
			res[v] = process_edge(mode, en[v], v, index[v], data[v], vbuffer_source[v], globalparamsK);
		}	
		
		// encode 
		if(mode == ACTSPROCESSMODE){ 
			// rotateby >>>
			unsigned int rotateby = 0; unsigned int sample_key = INVALIDDATA; unsigned int sample_u = 0; unsigned int rotate_forward = 1; 
			if(edges[0].value == 16383){ rotateby = edges[0].key >> 1; rotate_forward = edges[0].key & 0x1; } 
			else { sample_key = edges[0].key % UPDATEDATA_PACKINGSIZE; sample_u = 0; if(sample_key > sample_u){ rotateby = sample_key - sample_u; rotate_forward = 0; } else { rotateby = sample_u - sample_key; rotate_forward = 1; }}		
			if(rotate_forward == 0){ rearrangeLayoutVx16B(rotateby, res, res_out); } else{ rearrangeLayoutVx16F(rotateby, res, res_out); }
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 59::", rotateby, EDGEDATA_PACKINGSIZE, sample_key, edges[0].value, datas[0]);	// sample_key, edges[0].value, datas[0], sample_u, edges[0].key
			actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 60::", rotate_forward, 2, sample_key, sample_u, edges[0].key);	
			check_if_contiguous(res_out, edges, res, sample_key, rotate_forward, rotateby);
			#endif 
			
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				if(res_out[v].key != INVALIDDATA){ datas2[v] = (res_out[v].value << DSTVID_BITSZ) | res_out[v].key; } else { datas2[v] = INVALIDDATA; }
			}
		} else { // if(mode == SAVEVPROPERTYMODE){ 
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				datas2[v] = res[v].value; 
			}
		}

		// store
		UTILP0_WriteDatas(out, destbaseaddr_kvs + destoffset_kvs + destindex + i, datas2); 
		
		// colelect stats
		if(mode == ACTSPROCESSMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				// unsigned int loc = ((offsetkvs_dstvid + edges[v].key) / VDATA_PACKINGSIZE) / (((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE);
				unsigned int loc = (edges[v].key / VDATA_PACKINGSIZE) / (((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE);
				#ifdef _DEBUGMODE_CHECKS3
				if(res[v].key != INVALIDDATA){ actsutilityobj->checkoutofbounds("write__process(112)::DEBUG CODE 112::1", loc, numkvs_updateblocks_per_reducepartition, edges[v].key, globalparamsK.POW_BATCHRANGE, globalparamsK.NUM_REDUCEPARTITIONS); }
				if(res[v].key != INVALIDDATA){ actsutilityobj->checkoutofbounds("write__process(112b)::DEBUG CODE 112::1", loc, BLOCKRAM_UPDATEBLOCK_SIZE, edges[v].key, globalparamsK.POW_BATCHRANGE, globalparamsK.NUM_REDUCEPARTITIONS); }
				#endif	
				// if(res[v].key != INVALIDDATA){ stats[v][((offsetkvs_dstvid + edges[v].key) / EDGEDATA_PACKINGSIZE) / numkvs_vertices_per_updateblock] = 0xFFFFFFFF; }
				if(res[v].key != INVALIDDATA){ stats[v][offsetkvs_stats + loc] = 0xFFFFFFFF; } // = vertexid / number of vertices in an update block
			}
		}
		
		// collect stats for debugging
		#ifdef _DEBUGMODE_STATS
		if(mode == ACTSPROCESSMODE){ out[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key += EDGEDATA_PACKINGSIZE; }
		else { out[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += EDGEDATA_PACKINGSIZE; }
		#endif
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(mode == ACTSPROCESSMODE){ 
			collections[TRAVERSEDEDGES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE;
			collections[PROCESSEDGES_COLLECTIONID].data1 += EDGEDATA_PACKINGSIZE; // *** used in PR, CF implementations ONLY (not BFS) ***
		}
		if(en == true){
			actsutilityobj->globalstats_countkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE);
			actsutilityobj->globalstats_processedges_countvalidkvsprocessed(globalparamsK.ACTSPARAMS_INSTID, EDGEDATA_PACKINGSIZE); }
		#endif 
	}
	// exit(EXIT_SUCCESS);
}

void acts_all::ACTSP0_write__process_base(unsigned int mode, unsigned int llp_set, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],	
			unsigned int sourcebaseaddr_kvs, workload_t workload_kvs, unsigned int destbaseaddr_kvs, unsigned int destoffset_kvs, unsigned int destindex,
				unsigned int offset_buffer, // SAVEVPROPERTYMODE parameters
					collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK, globalposition_t globalposition){

	#ifdef FPGA_IMPL
	hls::stream<uint512_evec_dt> c0; //, c1, c2, c3, c4, c5;
	#pragma HLS STREAM variable = c0 depth = 512

	#pragma HLS dataflow
	if(mode == ACTSPROCESSMODE){ load(kvdram, c0, sourcebaseaddr_kvs, workload_kvs.offset_begin, workload_kvs.size); }
	compute_and_store(mode, llp_set, c0, kvdram, vbuffer_source, stats,
					destbaseaddr_kvs, destoffset_kvs, destindex, workload_kvs.size, 
						collections, globalparamsK, globalposition);
						
	#else 
	ACTSP0_write__process(mode, llp_set, kvdram, kvdram, vbuffer_source, stats,
			sourcebaseaddr_kvs, workload_kvs.offset_begin, destbaseaddr_kvs, destoffset_kvs, destindex, workload_kvs.size, 
				offset_buffer,
					collections, globalparamsK, globalposition);
	#endif
}

void acts_all::ACTSP0_read__reduce(unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],
			unsigned int sourcebaseaddr_kvs, workload_t workload_kvs, 
				unsigned int offset_ubuffer, unsigned int s, // READUPROPERTYMODE parameters
					unsigned int offsetkvs_vbuffer, // READVPROPERTYMODE parameters
						collection_t collections[COLLECTIONS_BUFFERSZ], sweepparams_t sweepparams, globalparams_t globalparamsK, globalposition_t globalposition){
	
	unsigned int memory[VECTOR2_SIZE][1];
	#pragma HLS ARRAY_PARTITION variable = memory 
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas2 complete
	keyvalue_t updates_in[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = updates_in complete
	keyvalue_t updates_inter[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = updates_inter complete
	keyvalue_t updates_out[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = updates_out complete
	keyvalue_t data[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = data complete
	unsigned int index[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = index complete
	bool en[EDGEDATA_PACKINGSIZE];
	#pragma HLS ARRAY_PARTITION variable = en complete
	
	
	for(int v = 0; v < VECTOR2_SIZE; v++){ updates_in[v].key = INVALIDDATA; updates_in[v].value = INVALIDDATA; memory[v][0] = 777; }
	if(mode == READVPROPERTYMODE){ for(int v = 0; v < VECTOR2_SIZE; v++){ updates_in[v].key = 0; updates_in[v].value = 0; }}
	for(int v = 0; v < VECTOR2_SIZE; v++){ en[v] = true; }
	unsigned int ind = 0;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(globalparamsK.ACTSPARAMS_INSTID==0){ cout<<"actit(reduce): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][workload_kvs.size: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"][workbuffer_size: "<<globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"][num_chunks: "<<(workload_kvs.offset_end - workload_kvs.offset_begin) / globalparamsK.ACTSPARAMS_WORKBUFFER_SIZE<<"] ... "<<endl; }		 // REMOVEME. 2.		
	#endif
	ACTIT_READANDREDUCE_MAINLOOP2A: for(batch_type offset_kvs=workload_kvs.offset_begin; offset_kvs<workload_kvs.offset_begin + workload_kvs.size; offset_kvs++){
	#pragma HLS PIPELINE II=1
	#pragma HLS dependence variable=vbuffer_dest inter false
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(globalparamsK.ACTSPARAMS_INSTID==0){ cout<<"actit(reduce): processing chunk [offset_kvs: "<<offset_kvs<<"]: [workload_kvs.offset_begin: "<<workload_kvs.offset_begin<<"]: [workload_kvs.offset_end: "<<workload_kvs.offset_end<<"] ... "<<endl; } // REMOVEME. 2.
		#endif
		
		// read 
		UTILP0_ReadDatas(kvdram, sourcebaseaddr_kvs + offset_kvs, datas);
		
		// decode
		if(mode == ACTSREDUCEMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL 
				if(datas[v] != INVALIDDATA){ updates_in[v].key = datas[v] & MASK_DSTVID; updates_in[v].value = (datas[v] >> DSTVID_BITSZ) & MASK_SRCVID; } // 16, 18, srcvid is upper, dstvid is lower 
				else { updates_in[v].key = INVALIDDATA; updates_in[v].value = INVALIDDATA; }
			}
		} else {
			if(mode == READUPROPERTYMODE){
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas2[v] = datas[v]; // & 0xFFFFFFFE; 
				}
			} else {
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas2[v] = datas[v] & 0xFFFFFFFE; 
				}
			}
		}
		
		// prepare inputs
		if(mode == ACTSREDUCEMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){ 
			#pragma HLS UNROLL 
				index[v] = updates_in[v].key / UPDATEDATA_PACKINGSIZE; data[v] = updates_in[v]; if(updates_in[v].key != INVALIDDATA){ en[v] = true; } else { en[v] = false; }
			}
		} else {
			if(mode == READUPROPERTYMODE){
				for(int v = 0; v < VECTOR2_SIZE; v++){ 
				#pragma HLS UNROLL
					index[v] = offset_ubuffer + s + ind; data[v].value = datas2[v]; en[v] = true;
				}
				ind += NUM_PEs;
			} else { // if(mode == READVPROPERTYMODE){ 
				for(int v = 0; v < VECTOR2_SIZE; v++){ 
				#pragma HLS UNROLL 
					index[v] = offsetkvs_vbuffer + (offset_kvs - workload_kvs.offset_begin); data[v].value = datas2[v]; en[v] = true; // CRITICAL NEWCHANGE
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, v, offsetkvs_vbuffer, mode); 
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, v, offsetkvs_vbuffer, offset_kvs); 
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112b::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, offsetkvs_vbuffer, offset_kvs, workload_kvs.offset_begin); 
					#endif	
				}
			}
		}
		
		// reduce	
		for(int v = 0; v < UPDATEDATA_PACKINGSIZE; v++){ 
		#pragma HLS UNROLL 
			reduce_update(mode, en[v], v, index[v], data[v], vbuffer_dest[v], stats[v], memory[v], globalparamsK);
		}
		
		#ifdef _DEBUGMODE_STATS
		if(mode == ACTSREDUCEMODE){ kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key += UPDATEDATA_PACKINGSIZE; }
		else { kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[1].key += UPDATEDATA_PACKINGSIZE; }
		#endif 
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(mode == ACTSREDUCEMODE){ collections[REDUCEUPDATES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE; }
		#endif 
	}
	// exit(EXIT_SUCCESS);
}

void acts_all::ACTSP0_actit(bool_type enable, unsigned int mode,
		uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE],	stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],
			keyvalue_t globalcapsule[BLOCKRAM_GLOBALSTATS_SIZE], globalparams_t globalparamsE, globalparams_t globalparamsK, globalposition_t globalposition, sweepparams_t sweepparams, travstate_t ptravstate, batch_type sourcebaseaddr_kvs, batch_type destbaseaddr_kvs,
				bool_type resetenv, bool_type flush, unsigned int edgebankID, collection_t collections[COLLECTIONS_BUFFERSZ]){

	unsigned int memory[VECTOR2_SIZE][1];
	#pragma HLS ARRAY_PARTITION variable = memory 
	keyvalue_vbuffer_t vbuffer_sourcebits[VDATA_PACKINGSIZE][BLOCKRAM_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable = vbuffer_sourcebits
	value_t datas[VECTOR_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	stats_t temp_stats[BLOCKRAM_UPDATEBLOCK_SIZE];
	#pragma HLS ARRAY_PARTITION variable = temp_stats
	unsigned int metadata_stats[VDATA_PACKINGSIZE];
	unsigned int buffer_offsets[BLOCKRAM_SIZE];
	unsigned int xblock_ids[BLOCKRAM_SIZE];
	workload_t xload_kvs[BLOCKRAM_SIZE];
	
	collection_t collections_tmp[COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable=collections_tmp complete
	collections_tmp[TRAVERSEDEDGES_COLLECTIONID] = collections[TRAVERSEDEDGES_COLLECTIONID];
	collections_tmp[PROCESSEDGES_COLLECTIONID] = collections[PROCESSEDGES_COLLECTIONID];
	collections_tmp[ACTIVEEDGESPROCESSED_COLLECTIONID] = collections[ACTIVEEDGESPROCESSED_COLLECTIONID];
	collections_tmp[REDUCEUPDATES_COLLECTIONID] = collections[REDUCEUPDATES_COLLECTIONID];
	
	unsigned int num_vPs = globalparamsK.NUM_PROCESSEDGESPARTITIONS;
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	unsigned int upperlimit = sweepparams.source_partition * globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION;
	
	unsigned int depth = globalparamsK.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
	
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; // 16 
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock; // 512		
	// cout<<"-------------------- actit: numkvs_vertices_per_updateblock: "<<numkvs_vertices_per_updateblock<<", numkvs_updateblocks_per_reducepartition: "<<numkvs_updateblocks_per_reducepartition<<endl;
	#ifdef _DEBUGMODE_CHECKS3
	if(globalparamsK.POW_BATCHRANGE < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)){ cout<<"actit:: ERROR 234. globalparamsK.POW_BATCHRANGE("<<globalparamsK.POW_BATCHRANGE<<") < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)("<<BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
	if(globalparamsK.POW_BATCHRANGE < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)){ cout<<"actit:: ERROR 234. globalparamsK.POW_BATCHRANGE("<<globalparamsK.POW_BATCHRANGE<<") < (BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW)("<<BLOCKRAM_UPDATEBLOCK_POW + VECTOR2_SIZE_POW<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
	#endif 
	
	bool sparse_readu = false; if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
	bool sparse_process = false; if(globalposition.num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ sparse_process = true; } else { sparse_process = false; }
	bool sparse_readv = false; if((stats[17][sweepparams.source_partition + 1] - stats[17][sweepparams.source_partition]) < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION){ sparse_readv = true; } else { sparse_readv = false; }
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(mode == ACTSPROCESSMODE){ if(sparse_readu == true){ cout<<"actit:: sparse_readu is TRUE. "<<endl; } }
	if(mode == ACTSPROCESSMODE){ if(sparse_process == true){ cout<<"actit:: sparse_process is TRUE. "<<endl; } }
	if(mode == ACTSREDUCEMODE){ if(sparse_readv == true){ cout<<"actit:: sparse_readv is TRUE. stats[17]["<<sweepparams.source_partition<<"]: "<<stats[17][sweepparams.source_partition]<<", stats[17]["<<sweepparams.source_partition+1<<"]: "<<stats[17][sweepparams.source_partition+1]<<endl; }}					
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(mode == ACTSPROCESSMODE){ if(sparse_readu == true){ cout<<"actit:: sparse_readu is TRUE. "<<endl; } else { cout<<"actit:: sparse_readu is FALSE. "<<endl; }}
	if(mode == ACTSPROCESSMODE){ if(sparse_process == true){ cout<<"actit:: sparse_process is TRUE. "<<endl; } else { cout<<"actit:: sparse_process is FALSE. "<<endl; }}
	if(mode == ACTSREDUCEMODE){ if(sparse_readv == true){ cout<<"actit:: sparse_readv is TRUE. stats[17]["<<sweepparams.source_partition<<"]: "<<stats[17][sweepparams.source_partition]<<", stats[17]["<<sweepparams.source_partition+1<<"]: "<<stats[17][sweepparams.source_partition+1]<<endl; } else { cout<<"actit:: sparse_readv is FALSE. stats[17]["<<sweepparams.source_partition<<"]: "<<stats[17][sweepparams.source_partition]<<", stats[17]["<<sweepparams.source_partition+1<<"]: "<<stats[17][sweepparams.source_partition+1]<<endl; }}					
	if(false){ cout<<"actit:: [num_LLPs: "<<num_LLPs<<", num_LLPset: "<<num_LLPset<<", num_vPs: "<<num_vPs<<"] "<<endl; }
	if(false){ cout<<"actit:: globalposition.source_partition: "<<globalposition.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<", globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION: "<<globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION<<""<<endl; }
	#endif 
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(mode == ACTSPROCESSMODE){ cout<<"-------------------- actit: v_p: "<<sweepparams.source_partition<<", globalposition.num_active_edgeblocks: "<<globalposition.num_active_edgeblocks<<endl; }
	#endif 
	
	bool en = true;
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS != ALGORITHMCLASS_ALLVERTEXISACTIVE && mode == ACTSPROCESSMODE && globalposition.num_active_edgeblocks == 0){ en = false; } // check if vertex partition is active

	if(en == true){
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(mode == ACTSPROCESSMODE){ cout<<"processit->actit: source_partition: "<<globalposition.source_partition<<" is active (Instance "<<globalparamsK.ACTSPARAMS_INSTID<<")"<<endl; }
		#endif 
	
		if(mode == ACTSPROCESSMODE || mode == ACTSPARTITIONMODE){
			#ifdef CONFIG_RELEASE_VERSION_BROADCASTUPROPS	
			unsigned int num_its = mergeP0_get_upropblock_workload(true, sweepparams.source_partition, kvdram, globalposition.num_active_edgeblocks, globalparamsK, xload_kvs, buffer_offsets, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
			// for(int t = 0; t < MAX_BLOCKRAM_VDESTDATA_SIZE; t++){ for(int v = 0; v < VDATA_PACKINGSIZE; v++){ vbuffer_source[v][t].data = 0; }} ////////////////////////////////////////// CRITICAL REMOVEME.
			for(unsigned int n=0; n<num_its; n++){
				unsigned int voffset_kvs = 0;
				for(unsigned int s=0; s<NUM_PEs; s++){
					ACTSP0_read__reduce(READUPROPERTYMODE, kvdram, vbuffer_source, stats,
						globalparamsK.BASEOFFSETKVS_SRCVERTICESDATA + voffset_kvs, xload_kvs[n], 
							buffer_offsets[n], s, // READUPROPERTYMODE parameters
								NAp, // READVPROPERTYMODE parameters
									collections_tmp, sweepparams, globalparamsK, globalposition);
					voffset_kvs += depth;
				}	
			}	
			#endif
			
			ACTIT_MAINLOOP1: for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
				batch_type destoffset_kvs = (globalcapsule[1 + llp_set].key + globalcapsule[1 + llp_set].value) / EDGEDATA_PACKINGSIZE;
				unsigned int destindex = 0; 
				workload_t last_workload;
				unsigned int num_its = mergeP0_get_edgeblock_workload(true, sweepparams.source_partition, llp_set, kvdram, globalposition.num_active_edgeblocks, globalparamsK, globalparamsE, xload_kvs, globalparamsK.ALGORITHMINFO_GRAPHITERATIONID);
				
				READ_PROCESS_PARTITION_STORE_LOOP1B: for(unsigned int n=0; n<num_its; n++){
					workload_t workload_kvs = xload_kvs[n];
					if(num_its > 1 && n > 0){ // optimization - skip already read edge-blocks
						if(xload_kvs[n].offset_begin > last_workload.offset_begin && xload_kvs[n].offset_begin < last_workload.offset_end){
							if(xload_kvs[n].offset_end < last_workload.offset_end){ workload_kvs.offset_begin = 0; workload_kvs.size = 0; } 
							else { workload_kvs.offset_begin = last_workload.offset_end; workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin; }
						}
					}
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("actit(2)", workload_kvs.size, (1 << 28) / 8, NAp, NAp, n);
					#endif
					ACTSP0_write__process_base(mode, llp_set, kvdram, vbuffer_source, stats,	
							sourcebaseaddr_kvs, workload_kvs, destbaseaddr_kvs, destoffset_kvs, destindex,
								NAp, // SAVEVPROPERTYMODE parameters
									collections_tmp, globalparamsK, globalposition);
					
					globalcapsule[1 + llp_set].value += workload_kvs.size * EDGEDATA_PACKINGSIZE;	
					destindex += workload_kvs.size;
					if(num_its > 1){ last_workload = xload_kvs[n]; }
				}
			}
		}
		
		if(mode == ACTSREDUCEMODE){
			workload_t workload_kvs;
			workload_kvs.offset_begin = globalcapsule[1 + sweepparams.source_partition].key / UPDATEDATA_PACKINGSIZE; 
			workload_kvs.size = globalcapsule[1 + sweepparams.source_partition].value / UPDATEDATA_PACKINGSIZE;
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			#ifdef _DEBUGMODE_CHECKS3
			if(workload_kvs.offset_end < workload_kvs.offset_begin){ cout<<"actit(reduce): ERROR 23: workload_kvs.offset_end("<<workload_kvs.offset_end<<") < workload_kvs.offset_begin("<<workload_kvs.offset_begin<<"). EXITING..."<<endl; exit(EXIT_FAILURE); }
			#endif
			
			#ifdef _DEBUGMODE_KERNELPRINTS3
			cout<<"actit(reduce): processing all chunks [begin_kvs: "<<workload_kvs.offset_begin<<"][end_kvs: "<<workload_kvs.offset_end<<"][size_kvs: "<<workload_kvs.size<<"][size: "<<workload_kvs.size * UPDATEDATA_PACKINGSIZE<<"] ... "<<endl;					
			#endif

			unsigned int num_its = mergeP0_get_updateblock_workload(true, sweepparams.source_partition, stats[16], stats[17], globalparamsK, xload_kvs, buffer_offsets);
			// for(int t = 0; t < MAX_BLOCKRAM_VDESTDATA_SIZE; t++){ for(int v = 0; v < VDATA_PACKINGSIZE; v++){ vbuffer_dest[v][t].data = 0; }} ///////////////////////////////////////// CRITICAL REMOVEME.
			for(unsigned int n=0; n<num_its; n++){
				ACTSP0_read__reduce(READVPROPERTYMODE, kvdram, vbuffer_dest, stats,
					globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, xload_kvs[n], 
						NAp, NAp, // READUPROPERTYMODE parameters
							buffer_offsets[n], // READVPROPERTYMODE parameters
								collections_tmp, sweepparams, globalparamsK, globalposition);
			}
		
			ACTSP0_read__reduce(ACTSREDUCEMODE, kvdram, vbuffer_dest, stats,
					sourcebaseaddr_kvs, workload_kvs, 
						NAp, NAp, // READUPROPERTYMODE parameters
							NAp, // READVPROPERTYMODE parameters
								collections_tmp, sweepparams, globalparamsK, globalposition);

			for(unsigned int n=0; n<num_its; n++){
				ACTSP0_write__process_base(SAVEVPROPERTYMODE, NAp, kvdram, vbuffer_dest, stats,
						sourcebaseaddr_kvs, xload_kvs[n], globalparamsK.BASEOFFSETKVS_DESTVERTICESDATA, xload_kvs[n].offset_begin, 0,
							buffer_offsets[n], // SAVEVPROPERTYMODE parameters
								collections_tmp, globalparamsK, globalposition);
			}
		}
	}
	
	// prepare stats
	if(mode == ACTSPROCESSMODE && UTILP0__processit__i_am_last__(globalposition) == true){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"-------------------------------- __processit__i_am_last__ == true, partition: "<<sweepparams.source_partition<<" ----------------------------------"<<endl;
		#endif
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit:: numkvs_vertices_per_updateblock: "<<numkvs_vertices_per_updateblock<<", numkvs_updateblocks_per_reducepartition: "<<numkvs_updateblocks_per_reducepartition<<", sweepparams.source_partition: "<<sweepparams.source_partition<<endl;
		#endif 
		unsigned int stats_tmp[VECTOR2_SIZE]; for(int v = 0; v < VECTOR2_SIZE; v++){ stats_tmp[v] = 0; }
		unsigned int index = 0;
		for(int t = 0; t < BLOCKRAM_UPDATEBLOCK_SIZE; t++){
		#pragma HLS PIPELINE II=1
			unsigned int llp_set = t / numkvs_updateblocks_per_reducepartition;
			unsigned int ind = t;
			unsigned int stat = stats[0][ind] | stats[1][ind] | stats[2][ind] | stats[3][ind] | stats[4][ind] | stats[5][ind] | stats[6][ind] | 
				stats[7][ind] | stats[8][ind] | stats[9][ind] | stats[10][ind] | stats[11][ind] | stats[12][ind] | stats[13][ind] | stats[14][ind] | stats[15][ind];
			if(stat > 0){ stats[16][index] = t; }
			// if(stat > 0){ vbuffer_dest[index % VECTOR2_SIZE][1 + (index / VECTOR2_SIZE)].data = t; }
			if(stat > 0){ stats_tmp[llp_set] += 1; }
			if(stat > 0){ index += 1; }
		}
		stats[17][0] = 0; for(unsigned int i=1; i<VECTOR2_SIZE; i++){ stats[17][i] = stats[17][i-1] + stats_tmp[i-1];  } // vbuffer_dest[i][0].data = stats[17][i];// calculate offsets
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"processit:: stats_tmp["<<0<<"]: "<<stats_tmp[0]<<endl;
		for(unsigned int i=0; i<4; i++){ cout<<"actit:: stats[16]["<<i<<"]: "<<stats[16][i]<<", "<<endl; }
		for(unsigned int i=0; i<4; i++){ cout<<"actit:: stats[17]["<<i<<"]: "<<stats[17][i]<<", "<<endl; }
		#endif

		for(unsigned int t=0; t<BLOCKRAM_UPDATEBLOCK_SIZE; t++){
		#pragma HLS PIPELINE II=1	
			datas[0] = stats[16][t];
			datas[1] = stats[17][t];
			UTILP0_WriteDatas(kvdram, globalparamsK.BASEOFFSETKVS_ACTIVEUPDATEBLOCKS + t, datas);
		}
	}
	
	collections[TRAVERSEDEDGES_COLLECTIONID] = collections_tmp[TRAVERSEDEDGES_COLLECTIONID];
	collections[PROCESSEDGES_COLLECTIONID] = collections_tmp[PROCESSEDGES_COLLECTIONID];
	collections[ACTIVEEDGESPROCESSED_COLLECTIONID] = collections_tmp[ACTIVEEDGESPROCESSED_COLLECTIONID];
	collections[REDUCEUPDATES_COLLECTIONID] = collections_tmp[REDUCEUPDATES_COLLECTIONID];
	// exit(EXIT_SUCCESS);
}

