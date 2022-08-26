// #define MEMACCESS_ENABLE_USEHLSSTREAM

void MEMACCESSP0_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){
	if(enable == OFF){ return; }

	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		UTILP0_ReadDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
		
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){
		#pragma HLS UNROLL 
			buffer[v][bufferoffset_kvs + i].data = datas[v];
		}
	}
	return;
}

// -------------------- workload -------------------- //

void rearrangeLayoutVx16F(unsigned int s, keyvalue_t in[EDGEDATA_PACKINGSIZE], keyvalue_t out[EDGEDATA_PACKINGSIZE]){
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
	
void rearrangeLayoutVx16B(unsigned int s, keyvalue_t in[EDGEDATA_PACKINGSIZE], keyvalue_t out[EDGEDATA_PACKINGSIZE]){
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

void check_if_contiguous(keyvalue_t keyvalue[EDGEDATA_PACKINGSIZE], keyvalue_t msg1[EDGEDATA_PACKINGSIZE], keyvalue_t msg2[EDGEDATA_PACKINGSIZE], unsigned int msg1_str, unsigned int msg2_str, unsigned int msg3_str){
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

keyvalue_t process_edge(unsigned int mode, bool enx, unsigned int v, unsigned int loc, keyvalue_t edge_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], globalparams_t globalparams){				
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

void reduce_update(unsigned int mode, bool enx, unsigned int v, unsigned int loc, keyvalue_t update_data, keyvalue_vbuffer_t vbuffer[MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int stats[BLOCKRAM_SIZE], unsigned int memory[1], globalparams_t globalparams){
	#pragma HLS INLINE

	// flag 
	bool en = true;
	
	// checks
	// if(loc >= globalparams.SIZEKVS2_REDUCEPARTITION){ loc = 0; en = false; } // REMOVEME
	#ifdef _DEBUGMODE_CHECKS3
	if(enx == true && loc >= (globalparams.SIZEKVS2_REDUCEPARTITION * VDATA_PACKINGSIZE)){ cout<<"reduce_update::ERROR SEEN @ loc("<<loc<<") >= globalparams.SIZEKVS2_REDUCEPARTITION("<<globalparams.SIZEKVS2_REDUCEPARTITION<<"). update_data.key: "<<update_data.key<<", update_data.value: "<<update_data.value<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	if(enx == true && mode == ACTSREDUCEMODE){ if(loc == memory[0]){ loc = (loc + 1) % 8; } memory[0] = loc; } // CRITICAL FIXME.
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
void load(uint512_dt *in, hls::stream<uint512_evec_dt >& out, workload_t workload_kvs){
	Loop_Ld: for (int i = 0; i < workload_kvs.size; i++){ // workload_kvs.size, 32
	#pragma HLS PIPELINE II=1
		uint512_evec_dt data = UTILP0_ReadEdges(in, workload_kvs.offset_srcbase + i);
		out.write(data);
	}
}
#endif 

void load2(unsigned int chunk_id, uint512_dt *in, uint512_evec_dt out[VDATA_PACKINGSIZE], workload_t workload_kvs){
	unsigned int sz = BLOCKRAM_SIZE;
	if((chunk_id+1) * BLOCKRAM_SIZE >= workload_kvs.size){ sz = workload_kvs.size - (chunk_id * BLOCKRAM_SIZE); }
	
	Loop_Ld2: for (int i = 0; i < sz; i++){ 
	#pragma HLS PIPELINE II=1
		uint512_evec_dt data = UTILP0_ReadEdges(in, workload_kvs.offset_srcbase + (chunk_id * BLOCKRAM_SIZE) + i);
		out[i] = data;
	}
}

void MEMACCESSP0_write__process(unsigned int chunk_id, unsigned int mode, unsigned int llp_set, 
		#ifdef MEMACCESS_ENABLE_USEHLSSTREAM
			#ifdef FPGA_IMPL
			hls::stream<uint512_evec_dt >& in, 
			#else 
			uint512_dt *in,
			#endif 
		#else 
			uint512_evec_dt in2[BLOCKRAM_SIZE],
		#endif 
		uint512_dt *out, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], workload_t workload_kvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK){
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
	unsigned int offsetkvs_dstvid = llp_set * (globalparamsK.SIZEKVS2_REDUCEPARTITION * EDGEDATA_PACKINGSIZE); 
	
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; 
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock;	
	unsigned int offsetkvs_stats = llp_set * numkvs_vertices_per_updateblock;	
	
	#ifdef MEMACCESS_ENABLE_USEHLSSTREAM
	unsigned int sz = workload_kvs.size;
	#else 
	unsigned int sz = BLOCKRAM_SIZE;	
	if((chunk_id+1) * BLOCKRAM_SIZE >= workload_kvs.size){ sz = workload_kvs.size - (chunk_id * BLOCKRAM_SIZE); }
	#endif 

	ACTIT_COMPUTEANDSTORE_MAINLOOP2A: for (int i = 0; i < sz; i++){
	#pragma HLS PIPELINE II=1
		// read
		if(mode == ACTSPROCESSMODE){
			#ifdef MEMACCESS_ENABLE_USEHLSSTREAM
				#ifdef FPGA_IMPL
				uint512_evec_dt in_data = in.read();
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas[v] = in_data.data[v];
				}
				#else 
				UTILP0_ReadEdges(in, workload_kvs.offset_srcbase + (index * BLOCKRAM_SIZE) + i, datas);	
				#endif 
			#else 
				uint512_evec_dt in_data = in2[i];
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas[v] = in_data.data[v];
				}
			#endif 
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
				index[v] = workload_kvs.offset_buffer_begin + i; data[v].key = datas[v]; data[v].value = datas[v]; en[v] = true;
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
			
			#ifdef CONFIG_RELEASE_VERSION_CYCLICSHIFTS
			if(rotate_forward == 0){ rearrangeLayoutVx16B(rotateby, res, res_out); } else{ rearrangeLayoutVx16F(rotateby, res, res_out); }
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 59::", rotateby, EDGEDATA_PACKINGSIZE, sample_key, edges[0].value, datas[0]);	// sample_key, edges[0].value, datas[0], sample_u, edges[0].key
			actsutilityobj->checkoutofbounds("read_process_partition_and_write::ERROR 60::", rotate_forward, 2, sample_key, sample_u, edges[0].key);	
			check_if_contiguous(res_out, edges, res, sample_key, rotate_forward, rotateby);
			#endif 
			#else 
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL
					res_out[v] = res[v]; 
				}
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
		UTILP0_WriteDatas(out, workload_kvs.offset_dstbase + i, datas2); 
		
		// collect stats
		if(mode == ACTSPROCESSMODE){
			for(int v = 0; v < VECTOR2_SIZE; v++){
			#pragma HLS UNROLL
				unsigned int loc = (edges[v].key / VDATA_PACKINGSIZE) / (((1 << globalparamsK.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE);
				#ifdef _DEBUGMODE_CHECKS3
				if(res[v].key != INVALIDDATA){ actsutilityobj->checkoutofbounds("write__process(112)::DEBUG CODE 112::1", loc, numkvs_updateblocks_per_reducepartition, edges[v].key, globalparamsK.POW_BATCHRANGE, globalparamsK.NUM_REDUCEPARTITIONS); }
				if(res[v].key != INVALIDDATA){ actsutilityobj->checkoutofbounds("write__process(112b)::DEBUG CODE 112::1", loc, BLOCKRAM_UPDATEBLOCK_SIZE, edges[v].key, globalparamsK.POW_BATCHRANGE, globalparamsK.NUM_REDUCEPARTITIONS); }
				#endif	
				if(res[v].key != INVALIDDATA){ stats[v][offsetkvs_stats + loc] = 0xFFFFFFFF; } // = vertexid / number of vertices in an update block
			}
		}
		
		// collect stats for debugging
		#ifdef _DEBUGMODE_STATS
		if(mode == ACTSPROCESSMODE){ out[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key += EDGEDATA_PACKINGSIZE; }
		else { out[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_NUMVERTICESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key += EDGEDATA_PACKINGSIZE; }
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
}

void MEMACCESSP0_write__process_base(unsigned int mode, unsigned int llp_set, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_source[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], workload_t workload_kvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK){
	uint512_evec_dt c1[BLOCKRAM_SIZE]; 
	
	#ifdef MEMACCESS_ENABLE_USEHLSSTREAM
		#ifdef FPGA_IMPL
		hls::stream<uint512_evec_dt> c0; //, c1, c2, c3, c4, c5;
		#pragma HLS STREAM variable = c0 depth = 512

		#pragma HLS dataflow
		load(kvdram, c0, workload_kvs);	
		#endif
			MEMACCESSP0_write__process(0, mode, llp_set, 
				#ifdef FPGA_IMPL
				c0, 
				#else 
				kvdram,	
				#endif 
				kvdram, vbuffer_source, stats, workload_kvs, collections, globalparamsK);
	#else
		for (int chunk_id = 0; chunk_id < (workload_kvs.size + (BLOCKRAM_SIZE - 1)) / BLOCKRAM_SIZE; chunk_id++){
			load2(chunk_id, kvdram, c1, workload_kvs);
			
			MEMACCESSP0_write__process(chunk_id, mode, llp_set, c1, kvdram, vbuffer_source, stats, workload_kvs, collections, globalparamsK);
		}
	#endif 
}

void MEMACCESSP0_read__reduce(unsigned int mode, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer_dest[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE],
			workload_t workload_kvs, collection_t collections[COLLECTIONS_BUFFERSZ], globalparams_t globalparamsK){
	// #pragma HLS INLINE OFF
	
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
	
	// if(workload_kvs.offset_srcbase + workload_kvs.offset_begin + workload_kvs.size > (((1 << 28) / 4) / VECTOR2_SIZE)){ return; }
	for(int v = 0; v < VECTOR2_SIZE; v++){ updates_in[v].key = INVALIDDATA; updates_in[v].value = INVALIDDATA; memory[v][0] = 777; }
	if(mode == READVPROPERTYMODE){ for(int v = 0; v < VECTOR2_SIZE; v++){ updates_in[v].key = 0; updates_in[v].value = 0; }} //  || mode == READDATAMODE
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
		UTILP0_ReadDatas(kvdram, workload_kvs.offset_srcbase + offset_kvs, datas);
		
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
			} else if (mode == READVPROPERTYMODE) {
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas2[v] = datas[v] & 0xFFFFFFFE; 
				}
			} else {
				for(int v = 0; v < VECTOR2_SIZE; v++){
				#pragma HLS UNROLL 
					datas2[v] = datas[v]; 
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
					index[v] = workload_kvs.offset_buffer_begin + ind; data[v].value = datas2[v]; en[v] = true;
				}
				ind += NUM_PEs;
			} else { // if (mode == READVPROPERTYMODE) {
				for(int v = 0; v < VECTOR2_SIZE; v++){ 
				#pragma HLS UNROLL 
					index[v] = workload_kvs.offset_buffer_begin + (offset_kvs - workload_kvs.offset_begin); data[v].value = datas2[v]; en[v] = true;
					#ifdef _DEBUGMODE_CHECKS3
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, v, workload_kvs.offset_buffer_begin, mode); 
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, v, workload_kvs.offset_buffer_begin, offset_kvs); 
					actsutilityobj->checkoutofbounds("read__reduce(112)::DEBUG CODE 112b::1", index[v], MAX_BLOCKRAM_VDESTDATA_SIZE, workload_kvs.offset_buffer_begin, offset_kvs, workload_kvs.offset_begin); 
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
		if(mode == ACTSREDUCEMODE){ kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_NUMEDGESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key += UPDATEDATA_PACKINGSIZE; }
		else { kvdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_NUMVERTICESTRAVERSED + globalparamsK.ALGORITHMINFO_GRAPHITERATIONID].data[0].key += UPDATEDATA_PACKINGSIZE; }
		#endif 
		#ifdef _DEBUGMODE_STATS___NOTUSED
		if(mode == ACTSREDUCEMODE){ collections[REDUCEUPDATES_COLLECTIONID].data1 += UPDATEDATA_PACKINGSIZE; }
		#endif 
	}
	// exit(EXIT_SUCCESS);
}

// -------------------- stats -------------------- //

unsigned int MEMACCESSP0_get_updateblock_workload(bool en, unsigned int reduce_partition, unsigned int * stats_offsets, unsigned int * stats_metadata, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE]){
	// #pragma HLS INLINE
	
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable = datas complete
	workload_t workload_kvs;
	
	bool sparse_v = false; if(stats_metadata[reduce_partition+1] - stats_metadata[reduce_partition] < globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPDATEBLOCKS_PER_VPARTITION){ sparse_v = true; } else { sparse_v = false; }
	if(en == false || globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_v = false; }
	unsigned int numkvs_vertices_per_updateblock = ((1 << globalparams.POW_BATCHRANGE) / VDATA_PACKINGSIZE) / BLOCKRAM_UPDATEBLOCK_SIZE; // 16
	unsigned int numkvs_updateblocks_per_reducepartition = (1 << MAX_RED_SRAMSZ_POW) / numkvs_vertices_per_updateblock; // 512
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	if(sparse_v == true){ cout<<"merge:: sparse_sync is TRUE. stats[inst:"<<globalparams.ACTSPARAMS_INSTID<<"][1]["<<reduce_partition<<"]: "<<stats_metadata[reduce_partition]<<"->"<<stats_metadata[reduce_partition+1]<<endl; } else { cout<<"merge:: sparse_sync is FALSE. stats[inst:"<<globalparams.ACTSPARAMS_INSTID<<"][1]["<<reduce_partition<<"]: "<<stats_metadata[reduce_partition]<<"->"<<stats_metadata[reduce_partition+1]<<endl; }					
	#endif 
	unsigned int num_its; if(sparse_v == true){ num_its = stats_metadata[reduce_partition+1] - stats_metadata[reduce_partition]; } else { num_its = 1; }
	#ifdef _DEBUGMODE_CHECKS3
	if((stats_metadata[reduce_partition+1] < stats_metadata[reduce_partition]) || (num_its > 600)){ cout<<"_get_updateblock_workload. ERROR 23. error somewhere. EXIITING... "<<endl; exit(EXIT_FAILURE); }
	#endif 
	for(unsigned int n=0; n<num_its; n++){
		if(sparse_v == true){
			unsigned int updateblock_id = stats_offsets[stats_metadata[reduce_partition] + n]; 
			workload_kvs.offset_begin = updateblock_id * numkvs_vertices_per_updateblock; 
			workload_kvs.size = numkvs_vertices_per_updateblock;  
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			xload_kvs[n] = workload_kvs;
			unsigned int buffer_offset = updateblock_id * numkvs_vertices_per_updateblock;
			buffer_offsets[n] = buffer_offset;
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_updateblock_workload(112)::DEBUG CODE 112::1", buffer_offset, MAX_BLOCKRAM_VDESTDATA_SIZE, updateblock_id, numkvs_vertices_per_updateblock, numkvs_updateblocks_per_reducepartition); 
			#endif	
		} else {
			workload_kvs.offset_begin = 0;
			workload_kvs.size = globalparams.SIZEKVS2_REDUCEPARTITION;
			workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
			xload_kvs[n] = workload_kvs;
			unsigned int buffer_offset = 0;
			buffer_offsets[n] = buffer_offset;
		}
	}
	return num_its;
}

unsigned int MEMACCESSP0_get_upropblock_workload(bool en, unsigned int process_partition, uint512_dt * dram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int num_active_upropblocks, globalparams_t globalparams, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int buffer_offsets[BLOCKRAM_SIZE], unsigned int graphiterationid){
	// #pragma HLS INLINE
	
	collection_t collections_tmp[COLLECTIONS_BUFFERSZ];
	#pragma HLS ARRAY_PARTITION variable=collections_tmp complete
	unsigned int upropblock_ids[BLOCKRAM_SIZE];
	
	bool sparse_readu = false; if(num_active_upropblocks < globalparams.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVUPROPBLOCKS_PER_VPARTITION){ sparse_readu = true; } else { sparse_readu = false; }
	if(en == false || globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_readu = false; }
	#ifdef _COLLECT_METADATA_FROM_KERNEL_
	unsigned int upropblockoffset_vs = (process_partition * MAXNUM_UPROPBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;	
	#else  
	unsigned int upropblockoffset_vs = ((process_partition * MAXNUMGRAPHITERATIONS * MAXNUM_UPROPBLOCKS_PER_VPARTITION) + (graphiterationid * MAXNUM_UPROPBLOCKS_PER_VPARTITION)) / VECTOR2_SIZE;
	#endif 
	unsigned int num_its; if(sparse_readu == true){ num_its = num_active_upropblocks; } else { num_its = 1; }
	workload_t workload_kvs;
	unsigned int buffer_offset = 0;
	unsigned int upropblock_id = NAp;
	
	if(sparse_readu == true){
		workload_t wkl_kvs; 
		wkl_kvs.offset_begin = 0; 
		wkl_kvs.size = ((1 + num_its) + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE;
		wkl_kvs.offset_end = wkl_kvs.offset_begin + wkl_kvs.size;
		wkl_kvs.offset_srcbase = globalparams.BASEOFFSETKVS_ACTIVEUPROPBLOCKS + upropblockoffset_vs; 
		wkl_kvs.offset_buffer_begin = 0;
		MEMACCESSP0_read__reduce(READDATAMODE, dram, buffer, stats, wkl_kvs, collections_tmp, globalparams);
		for(unsigned int n=0; n<1 + num_its; n++){ upropblock_ids[n] = buffer[n % VDATA_PACKINGSIZE][n / VDATA_PACKINGSIZE].data; }	
	}
	
	for(unsigned int n=0; n<num_its; n++){
		if(sparse_readu == true){
			// upropblock_id = UTILP0_ReadData(dram, globalparams.BASEOFFSETKVS_ACTIVEUPROPBLOCKS + upropblockoffset_vs, 1 + n);
			upropblock_id = upropblock_ids[1 + n];
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21a", upropblock_id, MAXNUM_UPROPBLOCKS_PER_VPARTITION, process_partition, n, num_its);
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21b", upropblock_id, MAXNUM_UPROPBLOCKS_PER_VPARTITION, process_partition, n, num_active_upropblocks);
			actsutilityobj->checkoutofbounds("_get_upropblock_workload:: ERROR 21c", upropblock_id, MAXNUM_UPROPBLOCKS_PER_VPARTITION, process_partition, n, globalparams.ALGORITHMINFO_GRAPHITERATIONID);
			#endif
			workload_kvs.offset_begin = 0;
			workload_kvs.size = 1; 
			buffer_offset = upropblock_id * NUM_VERTICESKVS_PER_UPROPBLOCK;
		} else {
			workload_kvs.offset_begin = 0;
			workload_kvs.size = globalparams.SIZEKVS2_PROCESSEDGESPARTITION / NUM_PEs;
			buffer_offset = 0;
		}
		workload_kvs.offset_end = workload_kvs.offset_begin + workload_kvs.size;
		xload_kvs[n] = workload_kvs;
		buffer_offsets[n] = buffer_offset;
		#ifdef _DEBUGMODE_KERNELPRINTS//4 // sparse_readu == true
		if(n==0 && false){ if(sparse_readu == true){ cout<<"UB<sparse><num_its:"<<num_its<<">:: "; } else { cout<<"<dense><num_its:"<<num_its<<">:: "; }
		cout<<"<instid:"<<globalparams.ACTSPARAMS_INSTID<<"><v_p:"<<process_partition<<"><n:"<<n<<"><upropblock_id:"<<upropblock_id<<"><num_active_upropblocks:"<<num_active_upropblocks<<">:: ";
		cout<<"[offset_begin: "<<workload_kvs.offset_begin<<", offset_end: "<<workload_kvs.offset_end<<", size: "<<workload_kvs.size<<"] "<<endl; }
		#endif
	}
	return num_its;
}

workload_t MEMACCESSP0_get_edgeblock_offset(bool en, unsigned int process_partition, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE){			
	// #pragma HLS INLINE
	
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	
	bool sparse_process = false; if(num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ sparse_process = true; } else { sparse_process = false; }
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = false; }	

	workload_t wkl_kvs; 
	if(sparse_process == true){
		wkl_kvs.offset_begin = 0; 
		wkl_kvs.size = (num_active_edgeblocks + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE; 
		wkl_kvs.offset_end = wkl_kvs.offset_begin + wkl_kvs.size;
		unsigned int edgeblockoffset_vs = (process_partition * MAXNUM_UPROPBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
		wkl_kvs.offset_srcbase = globalparamsK.BASEOFFSETKVS_ACTIVEUPROPBLOCKS + edgeblockoffset_vs; 
		wkl_kvs.offset_buffer_begin = 0;
	} else {
		wkl_kvs.offset_begin = 0; 
		wkl_kvs.size = 2; // 1; 
		wkl_kvs.offset_end = wkl_kvs.offset_begin + wkl_kvs.size;
		wkl_kvs.offset_srcbase = globalparamsE.BASEOFFSETKVS_EDGESMAP + process_partition;
		wkl_kvs.offset_buffer_begin = 0;
	}
	
	return wkl_kvs;
}

void MEMACCESSP0_get_edgeblock_ids__or__offsets(bool en, unsigned int process_partition, uint512_dt * dram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], unsigned int edgeblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], collection_t collections_tmp[COLLECTIONS_BUFFERSZ], unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE){			
	// #pragma HLS INLINE
	
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS; 
	
	bool sparse_process = false; if(num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ sparse_process = true; } else { sparse_process = false; }
	if(globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = false; }	

	workload_t wkl_kvs; 
	if(sparse_process == true){
		wkl_kvs.offset_begin = 0; 
		wkl_kvs.size = (num_active_edgeblocks + (VECTOR2_SIZE - 1)) / VECTOR2_SIZE; 
		wkl_kvs.offset_end = wkl_kvs.offset_begin + wkl_kvs.size;
		unsigned int edgeblockoffset_vs = (process_partition * MAXNUM_UPROPBLOCKS_PER_VPARTITION) / VECTOR2_SIZE;
		wkl_kvs.offset_srcbase = globalparamsK.BASEOFFSETKVS_ACTIVEUPROPBLOCKS + edgeblockoffset_vs; 
		wkl_kvs.offset_buffer_begin = 0;
	} else {
		wkl_kvs.offset_begin = 0; 
		wkl_kvs.size = 2; // 1; 
		wkl_kvs.offset_end = wkl_kvs.offset_begin + wkl_kvs.size;
		wkl_kvs.offset_srcbase = globalparamsE.BASEOFFSETKVS_EDGESMAP + process_partition;
		wkl_kvs.offset_buffer_begin = 0;
	}
	MEMACCESSP0_read__reduce(READDATAMODE, dram, buffer, stats, wkl_kvs, collections_tmp, globalparamsE);
	
	for(unsigned int t=0; t<wkl_kvs.size; t++){
	#pragma HLS PIPELINE II=1 // REMOVEME?
		for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
		#pragma HLS UNROLL 
			edgeblock_ids[v][t] = buffer[v][t].data; 
		}
	}
	return;
}

unsigned int MEMACCESSP0_get_edgeblock_workload(bool en, uint512_dt * dram, unsigned int process_partition, unsigned int llp_set, unsigned int edgeblock_ids[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], stats_t stats[STATS_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int num_active_edgeblocks, globalparams_t globalparamsK, globalparams_t globalparamsE, workload_t xload_kvs[BLOCKRAM_SIZE], unsigned int graphiterationid){
	// #pragma HLS INLINE
	
	bool sparse_process = false; if(num_active_edgeblocks < globalparamsK.THRESHOLD_HYBRIDGPMODE_MAXLIMIT_ACTVEDGEBLOCKS_PER_VPARTITION){ sparse_process = true; } else { sparse_process = false; }
	if(en == false || globalparamsK.ALGORITHMINFO_GRAPHALGORITHMCLASS == ALGORITHMCLASS_ALLVERTEXISACTIVE){ sparse_process = false; }
	unsigned int num_LLPs = globalparamsK.NUM_REDUCEPARTITIONS * OPT_NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (OPT_NUM_PARTITIONS - 1)) / OPT_NUM_PARTITIONS;
	unsigned int num_its; if(sparse_process == true){ num_its = num_active_edgeblocks; } else { num_its = 1; }
	
	READ_PROCESS_PARTITION_WRITE_GETWORKLOADSTATS_LOOP: for(unsigned int n=0; n<num_its; n++){
		workload_t workload_kvs;
		unsigned int edgeblock_id = NAp;		
		if(sparse_process == true){
			edgeblock_id = edgeblock_ids[n % VECTOR2_SIZE][n / VECTOR2_SIZE];
			workload_kvs.offset_begin = UTILP0_ReadData(dram, globalparamsE.BASEOFFSETKVS_VERTEXPTR, edgeblock_id) / EDGEDATA_PACKINGSIZE; // CRITICAL FIXME.
			workload_kvs.offset_end = UTILP0_ReadData(dram, globalparamsE.BASEOFFSETKVS_VERTEXPTR, edgeblock_id + 1) / EDGEDATA_PACKINGSIZE; // CRITICAL FIXME.
			workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;	
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_edgeblock_workload(2)", workload_kvs.offset_begin, ((1 << 28) / 4) / 16, NAp, NAp, n);
			actsutilityobj->checkoutofbounds("_get_edgeblock_workload(3)", workload_kvs.size, ((1 << 28) / 4) / 16, NAp, NAp, n);
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"_get_edgeblock_workload/sparse_process:: active edge-block seen @ ["<<globalparamsK.ACTSPARAMS_INSTID<<"]["<<process_partition<<"]["<<llp_set<<"]: edgeblock-id: "<<edgeblock_id<<", edgeblock-map: "<<"["<<workload_kvs.offset_begin<<", "<<workload_kvs.size<<"]"<<endl;
			#endif 
		} else {
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_edgeblock_workload(4)", llp_set + 1, VECTOR2_SIZE, NAp, NAp, n);
			#endif
			workload_kvs.offset_begin = edgeblock_ids[llp_set][0] / EDGEDATA_PACKINGSIZE;
			workload_kvs.offset_end = edgeblock_ids[llp_set + 1][0] / EDGEDATA_PACKINGSIZE;
			workload_kvs.size = workload_kvs.offset_end - workload_kvs.offset_begin;
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("_get_edgeblock_workload(5)", workload_kvs.size, ((1 << 28) / 4) / 16, NAp, NAp, n);
			#endif
			#ifdef _DEBUGMODE_KERNELPRINTS//4
			cout<<"_get_edgeblock_workload/dense_process:: full edge-block seen @ ["<<globalparamsK.ACTSPARAMS_INSTID<<"]["<<process_partition<<"]: edgeblock-id: NAp, edgeblock-map: "<<"["<<workload_kvs.offset_begin<<", "<<workload_kvs.size<<", "<<workload_kvs.offset_end<<"]"<<endl;
			#endif 
		}
	
		if(workload_kvs.offset_end < workload_kvs.offset_begin){ workload_kvs.size = 0; }
		#ifdef _DEBUGMODE_KERNELPRINTS//4
		if(n==0 && false){ if(sparse_process == true){ cout<<"<sparse><num_its:"<<num_its<<">:: "; } else { cout<<"<dense><num_its:"<<num_its<<">:: "; }
		cout<<"<instid:"<<globalparamsK.ACTSPARAMS_INSTID<<"><v_p:"<<process_partition<<"><llp_set:"<<llp_set<<"><n:"<<n<<"><edgeblock_id:"<<edgeblock_id<<"><num_active_edgeblocks:"<<num_active_edgeblocks<<">:: ";
		cout<<"[offset_begin: "<<workload_kvs.offset_begin<<", offset_end: "<<workload_kvs.offset_end<<", size: "<<workload_kvs.size<<"] "<<endl; }
		#endif
		xload_kvs[n] = workload_kvs;
	}
	return num_its;
}

// -------------------- stats -------------------- //

void MEMACCESSP0_readglobalstats(bool_type enable, uint512_dt * kvdram, keyvalue_t globalstatsbuffer[BLOCKRAM_GLOBALSTATS_SIZE], batch_type offset_kvs, globalparams_t globalparams){ 
	if(enable == OFF){ return; }
	#ifdef _DEBUGMODE_CHECKS2
	actsutilityobj->checkoutofbounds("readglobalstats", offset_kvs + globalparams.ACTSPARAMS_NUM_PARTITIONS, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS + 1, NAp, NAp, NAp);
	#endif
	
	keyvalue_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READGLOBALSTATS_LOOP: for (buffer_type i=0; i<1 + globalparams.NUM_REDUCEPARTITIONS; i++){
	#pragma HLS PIPELINE II=1
		uint512_vec_dt vec;
		
		// UTILP0_ReadDatas(kvdram, offset_kvs + i, datas); // NEWCHANGE.
		// globalstatsbuffer[i] = datas[globalparams.VARS_WORKBATCH];
		
		#ifdef _WIDEWORD
		vec.data[0].key = kvdram[offset_kvs + i].range(31, 0); 
		vec.data[0].value = kvdram[offset_kvs + i].range(63, 32); 
		vec.data[1].key = kvdram[offset_kvs + i].range(95, 64); 
		vec.data[1].value = kvdram[offset_kvs + i].range(127, 96); 
		vec.data[2].key = kvdram[offset_kvs + i].range(159, 128); 
		vec.data[2].value = kvdram[offset_kvs + i].range(191, 160); 
		vec.data[3].key = kvdram[offset_kvs + i].range(223, 192); 
		vec.data[3].value = kvdram[offset_kvs + i].range(255, 224); 
		vec.data[4].key = kvdram[offset_kvs + i].range(287, 256); 
		vec.data[4].value = kvdram[offset_kvs + i].range(319, 288); 
		vec.data[5].key = kvdram[offset_kvs + i].range(351, 320); 
		vec.data[5].value = kvdram[offset_kvs + i].range(383, 352); 
		vec.data[6].key = kvdram[offset_kvs + i].range(415, 384); 
		vec.data[6].value = kvdram[offset_kvs + i].range(447, 416); 
		vec.data[7].key = kvdram[offset_kvs + i].range(479, 448); 
		vec.data[7].value = kvdram[offset_kvs + i].range(511, 480); 
		#else 
		vec.data[0].key = kvdram[offset_kvs + i].data[0].key; 
		vec.data[0].value = kvdram[offset_kvs + i].data[0].value; 
		vec.data[1].key = kvdram[offset_kvs + i].data[1].key; 
		vec.data[1].value = kvdram[offset_kvs + i].data[1].value; 
		vec.data[2].key = kvdram[offset_kvs + i].data[2].key; 
		vec.data[2].value = kvdram[offset_kvs + i].data[2].value; 
		vec.data[3].key = kvdram[offset_kvs + i].data[3].key; 
		vec.data[3].value = kvdram[offset_kvs + i].data[3].value; 
		vec.data[4].key = kvdram[offset_kvs + i].data[4].key; 
		vec.data[4].value = kvdram[offset_kvs + i].data[4].value; 
		vec.data[5].key = kvdram[offset_kvs + i].data[5].key; 
		vec.data[5].value = kvdram[offset_kvs + i].data[5].value; 
		vec.data[6].key = kvdram[offset_kvs + i].data[6].key; 
		vec.data[6].value = kvdram[offset_kvs + i].data[6].value; 
		vec.data[7].key = kvdram[offset_kvs + i].data[7].key; 
		vec.data[7].value = kvdram[offset_kvs + i].data[7].value; 
		#endif 
		globalstatsbuffer[i] = vec.data[globalparams.VARS_WORKBATCH];	
	}		
	
	#ifdef _DEBUGMODE_KERNELPRINTS//4
	actsutilityobj->printkeyvalues("mem_access:: readglobalstats.globalstatsbuffer", globalstatsbuffer, 1 + globalparams.NUM_REDUCEPARTITIONS); 
	#endif
	#ifdef _DEBUGMODE_CHECKS3
	for (buffer_type i=0; i<globalparams.NUM_REDUCEPARTITIONS; i++){ actsutilityobj->checkoutofbounds("_readglobalstats 23", globalstatsbuffer[i].key + globalstatsbuffer[i].value, globalparams.SIZE_KVDRAM, i, globalparams.ALGORITHMINFO_GRAPHITERATIONID, NAp); }
	#endif
	// exit(EXIT_SUCCESS); 
	return;
}

void MEMACCESSP0_readhelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, globalparams_t globalparams){
	
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		UTILP0_ReadDatas(vdram, offset_kvs + i, datas);
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("readhelperstats. ERROR.", GraphIter, MAXNUMGRAPHITERATIONS, NAp, NAp, NAp);
		#endif
		
		pmask[i] = datas[GraphIter];
		// cout<<"_readhelperstats:: pmask["<<offset_kvs + i<<"]["<<i<<"]: "<<pmask[i]<<endl;
	}
	// exit(EXIT_SUCCESS);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<< TIMINGRESULTSCOLOR << ">>> readpmasks: printing active vertex partitions: GraphIter: "<<GraphIter<<": ";
	unsigned int num_actvps = 0;
	for(unsigned int t=0; t<BLOCKRAM_CURRPMASK_SIZE; t++){
		if(pmask[t] > 0  && t < 16){ cout<<t<<", "; }
		if(pmask[t] > 0){ num_actvps += 1; }
	}
	cout<<" ("<<num_actvps<<" active partitions, "<<size_kvs<<" total partitions)"<< RESET << endl;
	#endif 
	return;
}

void MEMACCESSP0_savehelperstats(uint512_dt * vdram, pmask_dt pmask[BLOCKRAM_CURRPMASK_SIZE], batch_type offset_kvs, batch_type size_kvs, unsigned int GraphIter, globalparams_t globalparams){
	
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	READMANYPMASKS_LOOP1: for (buffer_type i=0; i<size_kvs; i++){	
	#pragma HLS PIPELINE II=1
		datas[GraphIter] = pmask[i];
		UTILP0_WriteDatas(vdram, offset_kvs + i, datas);
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("readhelperstats. ERROR.", GraphIter, MAXNUMGRAPHITERATIONS, NAp, NAp, NAp);
		#endif
	}
	return;
}

// -------------------- workload (obsolete) -------------------- //

void MEMACCESSP0_readV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t buffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){
	if(enable == OFF){ return; }

	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	value_t datas2[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas2 complete
	
	READVDATA_LOOP1: for (buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
		UTILP0_ReadDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
	
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
			value_t vdata = datas[v] >> 1; value_t mask = datas[v] & 0x1;
			if(vdata < 1000){ cout<<"readV: ACTIVE VDATA SEEN: @ i: "<<i<<" v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<endl; }}
		#endif
		
		// clear masks
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
		#pragma HLS UNROLL
			datas2[v] = datas[v] & 0xFFFFFFFE; 
			// datas2[v] = datas[v]; // CRITICAL REMOVEME.
		}
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){ datas2[v] = datas[v]; }
		
 vdata[0].data = datas2[0];  vdata[1].data = datas2[1];  vdata[2].data = datas2[2];  vdata[3].data = datas2[3];  vdata[4].data = datas2[4];  vdata[5].data = datas2[5];  vdata[6].data = datas2[6];  vdata[7].data = datas2[7];  vdata[8].data = datas2[8];  vdata[9].data = datas2[9];  vdata[10].data = datas2[10];  vdata[11].data = datas2[11];  vdata[12].data = datas2[12];  vdata[13].data = datas2[13];  vdata[14].data = datas2[14];  vdata[15].data = datas2[15]; 	
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSRRRP0_readV 21", bufferoffset_kvs + i, MAX_BLOCKRAM_VDESTDATA_SIZE, size_kvs, bufferoffset_kvs, i);
		#endif
		buffer[0][bufferoffset_kvs + i] = vdata[0];
		buffer[1][bufferoffset_kvs + i] = vdata[1];
		buffer[2][bufferoffset_kvs + i] = vdata[2];
		buffer[3][bufferoffset_kvs + i] = vdata[3];
		buffer[4][bufferoffset_kvs + i] = vdata[4];
		buffer[5][bufferoffset_kvs + i] = vdata[5];
		buffer[6][bufferoffset_kvs + i] = vdata[6];
		buffer[7][bufferoffset_kvs + i] = vdata[7];
		buffer[8][bufferoffset_kvs + i] = vdata[8];
		buffer[9][bufferoffset_kvs + i] = vdata[9];
		buffer[10][bufferoffset_kvs + i] = vdata[10];
		buffer[11][bufferoffset_kvs + i] = vdata[11];
		buffer[12][bufferoffset_kvs + i] = vdata[12];
		buffer[13][bufferoffset_kvs + i] = vdata[13];
		buffer[14][bufferoffset_kvs + i] = vdata[14];
		buffer[15][bufferoffset_kvs + i] = vdata[15];
	}
	return;
}

void MEMACCESSP0_saveV(bool_type enable, uint512_dt * kvdram, keyvalue_vbuffer_t vbuffer[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VDESTDATA_SIZE], batch_type baseoffset_kvs, batch_type offset_kvs, batch_type bufferoffset_kvs, buffer_type size_kvs, globalposition_t globalposition, globalparams_t globalparams){				
	if(enable == OFF){ return; }
	
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	SAVEVDATA_LOOP1: for(buffer_type i=0; i<size_kvs; i++){
	#pragma HLS PIPELINE II=1
	
		vdata[0] = vbuffer[0][bufferoffset_kvs + i];
	
		vdata[1] = vbuffer[1][bufferoffset_kvs + i];
	
		vdata[2] = vbuffer[2][bufferoffset_kvs + i];
	
		vdata[3] = vbuffer[3][bufferoffset_kvs + i];
	
		vdata[4] = vbuffer[4][bufferoffset_kvs + i];
	
		vdata[5] = vbuffer[5][bufferoffset_kvs + i];
	
		vdata[6] = vbuffer[6][bufferoffset_kvs + i];
	
		vdata[7] = vbuffer[7][bufferoffset_kvs + i];
	
		vdata[8] = vbuffer[8][bufferoffset_kvs + i];
	
		vdata[9] = vbuffer[9][bufferoffset_kvs + i];
	
		vdata[10] = vbuffer[10][bufferoffset_kvs + i];
	
		vdata[11] = vbuffer[11][bufferoffset_kvs + i];
	
		vdata[12] = vbuffer[12][bufferoffset_kvs + i];
	
		vdata[13] = vbuffer[13][bufferoffset_kvs + i];
	
		vdata[14] = vbuffer[14][bufferoffset_kvs + i];
	
		vdata[15] = vbuffer[15][bufferoffset_kvs + i];
		
 datas[0] = vdata[0].data;  datas[1] = vdata[1].data;  datas[2] = vdata[2].data;  datas[3] = vdata[3].data;  datas[4] = vdata[4].data;  datas[5] = vdata[5].data;  datas[6] = vdata[6].data;  datas[7] = vdata[7].data;  datas[8] = vdata[8].data;  datas[9] = vdata[9].data;  datas[10] = vdata[10].data;  datas[11] = vdata[11].data;  datas[12] = vdata[12].data;  datas[13] = vdata[13].data;  datas[14] = vdata[14].data;  datas[15] = vdata[15].data; 	
		
		#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){ 
			value_t vdata = datas[v] >> 1; value_t mask = datas[v] & 0x1;
			if(vdata < 1000){ cout<<"saveV: ACTIVE VDATA SEEN: @ i: "<<i<<" v: "<<v<<", vdata: "<<vdata<<", mask: "<<mask<<endl; }}
		#endif
		
		#ifdef _DEBUGMODE_CHECKS3
		actsutilityobj->checkoutofbounds("MEMACCESSP0_saveV 23", baseoffset_kvs + offset_kvs + i, globalparams.ACTSPARAMS_MAXHBMCAPACITY_KVS, NAp, NAp, NAp);
		#endif
		UTILP0_WriteDatas(kvdram, baseoffset_kvs + offset_kvs + i, datas);
	}
	return;
}

void MEMACCESSP0_readANDRVchunks1(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks1 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks2(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks2 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks3(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks3 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks4(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks4 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks5(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks5 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks6(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks6 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks7(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks7 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks8(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks8 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks9(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks9 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks10(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks10 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			vbuffer9[0][index + s] = vdata[0];
			vbuffer9[1][index + s] = vdata[1];
			vbuffer9[2][index + s] = vdata[2];
			vbuffer9[3][index + s] = vdata[3];
			vbuffer9[4][index + s] = vdata[4];
			vbuffer9[5][index + s] = vdata[5];
			vbuffer9[6][index + s] = vdata[6];
			vbuffer9[7][index + s] = vdata[7];
			vbuffer9[8][index + s] = vdata[8];
			vbuffer9[9][index + s] = vdata[9];
			vbuffer9[10][index + s] = vdata[10];
			vbuffer9[11][index + s] = vdata[11];
			vbuffer9[12][index + s] = vdata[12];
			vbuffer9[13][index + s] = vdata[13];
			vbuffer9[14][index + s] = vdata[14];
			vbuffer9[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks11(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks11 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			vbuffer9[0][index + s] = vdata[0];
			vbuffer9[1][index + s] = vdata[1];
			vbuffer9[2][index + s] = vdata[2];
			vbuffer9[3][index + s] = vdata[3];
			vbuffer9[4][index + s] = vdata[4];
			vbuffer9[5][index + s] = vdata[5];
			vbuffer9[6][index + s] = vdata[6];
			vbuffer9[7][index + s] = vdata[7];
			vbuffer9[8][index + s] = vdata[8];
			vbuffer9[9][index + s] = vdata[9];
			vbuffer9[10][index + s] = vdata[10];
			vbuffer9[11][index + s] = vdata[11];
			vbuffer9[12][index + s] = vdata[12];
			vbuffer9[13][index + s] = vdata[13];
			vbuffer9[14][index + s] = vdata[14];
			vbuffer9[15][index + s] = vdata[15];
			vbuffer10[0][index + s] = vdata[0];
			vbuffer10[1][index + s] = vdata[1];
			vbuffer10[2][index + s] = vdata[2];
			vbuffer10[3][index + s] = vdata[3];
			vbuffer10[4][index + s] = vdata[4];
			vbuffer10[5][index + s] = vdata[5];
			vbuffer10[6][index + s] = vdata[6];
			vbuffer10[7][index + s] = vdata[7];
			vbuffer10[8][index + s] = vdata[8];
			vbuffer10[9][index + s] = vdata[9];
			vbuffer10[10][index + s] = vdata[10];
			vbuffer10[11][index + s] = vdata[11];
			vbuffer10[12][index + s] = vdata[12];
			vbuffer10[13][index + s] = vdata[13];
			vbuffer10[14][index + s] = vdata[14];
			vbuffer10[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}
void MEMACCESSP0_readANDRVchunks12(bool_type enable, uint512_dt * vdram, keyvalue_vbuffer_t vbuffer0[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer1[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer2[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer3[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer4[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer5[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer6[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer7[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer8[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer9[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer10[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE],keyvalue_vbuffer_t vbuffer11[VDATA_PACKINGSIZE][MAX_BLOCKRAM_VSRCDATA_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, batch_type vsz_kvs, globalposition_t globalposition, globalparams_t globalparamsV){			
	// return;//
	keyvalue_vbuffer_t vdata[VECTOR2_SIZE];
	#pragma HLS ARRAY_PARTITION variable=vdata complete
	value_t datas[VECTOR2_SIZE]; 
	#pragma HLS ARRAY_PARTITION variable=datas complete
	
	unsigned int depth = (globalparamsV.NUM_REDUCEPARTITIONS * globalparamsV.SIZEKVS2_REDUCEPARTITION) / VDATA_SHRINK_RATIO; // NEWCHANGE.
	unsigned int depth_i = 0;
	unsigned int limit = globalparamsV.SIZE_SRCVERTICESDATA / VECTOR2_SIZE;
	
	READANDRVCHUNKS_LOOP2: for(unsigned int s=0; s<NUM_PEs; s++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"MEMACCESSP0_readANDRVchunks:: size loaded @ s("<<s<<"): vsz_kvs: "<<vsz_kvs<<", voffset_kvs: "<<voffset_kvs<<", depth: "<<depth<<", depth_i: "<<depth_i<<endl;
		#endif
		unsigned int index = 0;
		// unsigned int index = s;
		READANDRVCHUNKS_LOOP2B: for (buffer_type i=0; i<vsz_kvs; i++){
		#pragma HLS PIPELINE II=1
		
			// unsigned int offset_kvs; if(voffset_kvs + depth_i + i >= limit){ offset_kvs = 0; } else { offset_kvs = voffset_kvs + depth_i + i; } // CRITICAL FIXME. 
			unsigned int offset_kvs = voffset_kvs + depth_i + i;
			UTILP0_ReadDatas(vdram, vbaseoffset_kvs + offset_kvs, datas);	
			
 vdata[0].data = datas[0];  vdata[1].data = datas[1];  vdata[2].data = datas[2];  vdata[3].data = datas[3];  vdata[4].data = datas[4];  vdata[5].data = datas[5];  vdata[6].data = datas[6];  vdata[7].data = datas[7];  vdata[8].data = datas[8];  vdata[9].data = datas[9];  vdata[10].data = datas[10];  vdata[11].data = datas[11];  vdata[12].data = datas[12];  vdata[13].data = datas[13];  vdata[14].data = datas[14];  vdata[15].data = datas[15]; 	
			
			#ifdef _DEBUGMODE_KERNELPRINTS_TRACE3
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){ if(datas[v] == 1 || datas[v] < 1000){ cout<<"readANDRVchunks: ACTIVE MASK SEEN: @ s: "<<s<<", i: "<<i<<" v: "<<v<<", voffset_kvs: "<<voffset_kvs<<", index + s: "<<index + s<<", datas["<<v<<"]: "<<datas[v]<<endl; }}
			#endif
			
			#ifdef _DEBUGMODE_CHECKS3
			actsutilityobj->checkoutofbounds("MEMACCESSP0_readANDRVchunks12 25", index + s, MAX_BLOCKRAM_VSRCDATA_SIZE, vsz_kvs, index, i);
			#endif
			vbuffer0[0][index + s] = vdata[0];
			vbuffer0[1][index + s] = vdata[1];
			vbuffer0[2][index + s] = vdata[2];
			vbuffer0[3][index + s] = vdata[3];
			vbuffer0[4][index + s] = vdata[4];
			vbuffer0[5][index + s] = vdata[5];
			vbuffer0[6][index + s] = vdata[6];
			vbuffer0[7][index + s] = vdata[7];
			vbuffer0[8][index + s] = vdata[8];
			vbuffer0[9][index + s] = vdata[9];
			vbuffer0[10][index + s] = vdata[10];
			vbuffer0[11][index + s] = vdata[11];
			vbuffer0[12][index + s] = vdata[12];
			vbuffer0[13][index + s] = vdata[13];
			vbuffer0[14][index + s] = vdata[14];
			vbuffer0[15][index + s] = vdata[15];
			vbuffer1[0][index + s] = vdata[0];
			vbuffer1[1][index + s] = vdata[1];
			vbuffer1[2][index + s] = vdata[2];
			vbuffer1[3][index + s] = vdata[3];
			vbuffer1[4][index + s] = vdata[4];
			vbuffer1[5][index + s] = vdata[5];
			vbuffer1[6][index + s] = vdata[6];
			vbuffer1[7][index + s] = vdata[7];
			vbuffer1[8][index + s] = vdata[8];
			vbuffer1[9][index + s] = vdata[9];
			vbuffer1[10][index + s] = vdata[10];
			vbuffer1[11][index + s] = vdata[11];
			vbuffer1[12][index + s] = vdata[12];
			vbuffer1[13][index + s] = vdata[13];
			vbuffer1[14][index + s] = vdata[14];
			vbuffer1[15][index + s] = vdata[15];
			vbuffer2[0][index + s] = vdata[0];
			vbuffer2[1][index + s] = vdata[1];
			vbuffer2[2][index + s] = vdata[2];
			vbuffer2[3][index + s] = vdata[3];
			vbuffer2[4][index + s] = vdata[4];
			vbuffer2[5][index + s] = vdata[5];
			vbuffer2[6][index + s] = vdata[6];
			vbuffer2[7][index + s] = vdata[7];
			vbuffer2[8][index + s] = vdata[8];
			vbuffer2[9][index + s] = vdata[9];
			vbuffer2[10][index + s] = vdata[10];
			vbuffer2[11][index + s] = vdata[11];
			vbuffer2[12][index + s] = vdata[12];
			vbuffer2[13][index + s] = vdata[13];
			vbuffer2[14][index + s] = vdata[14];
			vbuffer2[15][index + s] = vdata[15];
			vbuffer3[0][index + s] = vdata[0];
			vbuffer3[1][index + s] = vdata[1];
			vbuffer3[2][index + s] = vdata[2];
			vbuffer3[3][index + s] = vdata[3];
			vbuffer3[4][index + s] = vdata[4];
			vbuffer3[5][index + s] = vdata[5];
			vbuffer3[6][index + s] = vdata[6];
			vbuffer3[7][index + s] = vdata[7];
			vbuffer3[8][index + s] = vdata[8];
			vbuffer3[9][index + s] = vdata[9];
			vbuffer3[10][index + s] = vdata[10];
			vbuffer3[11][index + s] = vdata[11];
			vbuffer3[12][index + s] = vdata[12];
			vbuffer3[13][index + s] = vdata[13];
			vbuffer3[14][index + s] = vdata[14];
			vbuffer3[15][index + s] = vdata[15];
			vbuffer4[0][index + s] = vdata[0];
			vbuffer4[1][index + s] = vdata[1];
			vbuffer4[2][index + s] = vdata[2];
			vbuffer4[3][index + s] = vdata[3];
			vbuffer4[4][index + s] = vdata[4];
			vbuffer4[5][index + s] = vdata[5];
			vbuffer4[6][index + s] = vdata[6];
			vbuffer4[7][index + s] = vdata[7];
			vbuffer4[8][index + s] = vdata[8];
			vbuffer4[9][index + s] = vdata[9];
			vbuffer4[10][index + s] = vdata[10];
			vbuffer4[11][index + s] = vdata[11];
			vbuffer4[12][index + s] = vdata[12];
			vbuffer4[13][index + s] = vdata[13];
			vbuffer4[14][index + s] = vdata[14];
			vbuffer4[15][index + s] = vdata[15];
			vbuffer5[0][index + s] = vdata[0];
			vbuffer5[1][index + s] = vdata[1];
			vbuffer5[2][index + s] = vdata[2];
			vbuffer5[3][index + s] = vdata[3];
			vbuffer5[4][index + s] = vdata[4];
			vbuffer5[5][index + s] = vdata[5];
			vbuffer5[6][index + s] = vdata[6];
			vbuffer5[7][index + s] = vdata[7];
			vbuffer5[8][index + s] = vdata[8];
			vbuffer5[9][index + s] = vdata[9];
			vbuffer5[10][index + s] = vdata[10];
			vbuffer5[11][index + s] = vdata[11];
			vbuffer5[12][index + s] = vdata[12];
			vbuffer5[13][index + s] = vdata[13];
			vbuffer5[14][index + s] = vdata[14];
			vbuffer5[15][index + s] = vdata[15];
			vbuffer6[0][index + s] = vdata[0];
			vbuffer6[1][index + s] = vdata[1];
			vbuffer6[2][index + s] = vdata[2];
			vbuffer6[3][index + s] = vdata[3];
			vbuffer6[4][index + s] = vdata[4];
			vbuffer6[5][index + s] = vdata[5];
			vbuffer6[6][index + s] = vdata[6];
			vbuffer6[7][index + s] = vdata[7];
			vbuffer6[8][index + s] = vdata[8];
			vbuffer6[9][index + s] = vdata[9];
			vbuffer6[10][index + s] = vdata[10];
			vbuffer6[11][index + s] = vdata[11];
			vbuffer6[12][index + s] = vdata[12];
			vbuffer6[13][index + s] = vdata[13];
			vbuffer6[14][index + s] = vdata[14];
			vbuffer6[15][index + s] = vdata[15];
			vbuffer7[0][index + s] = vdata[0];
			vbuffer7[1][index + s] = vdata[1];
			vbuffer7[2][index + s] = vdata[2];
			vbuffer7[3][index + s] = vdata[3];
			vbuffer7[4][index + s] = vdata[4];
			vbuffer7[5][index + s] = vdata[5];
			vbuffer7[6][index + s] = vdata[6];
			vbuffer7[7][index + s] = vdata[7];
			vbuffer7[8][index + s] = vdata[8];
			vbuffer7[9][index + s] = vdata[9];
			vbuffer7[10][index + s] = vdata[10];
			vbuffer7[11][index + s] = vdata[11];
			vbuffer7[12][index + s] = vdata[12];
			vbuffer7[13][index + s] = vdata[13];
			vbuffer7[14][index + s] = vdata[14];
			vbuffer7[15][index + s] = vdata[15];
			vbuffer8[0][index + s] = vdata[0];
			vbuffer8[1][index + s] = vdata[1];
			vbuffer8[2][index + s] = vdata[2];
			vbuffer8[3][index + s] = vdata[3];
			vbuffer8[4][index + s] = vdata[4];
			vbuffer8[5][index + s] = vdata[5];
			vbuffer8[6][index + s] = vdata[6];
			vbuffer8[7][index + s] = vdata[7];
			vbuffer8[8][index + s] = vdata[8];
			vbuffer8[9][index + s] = vdata[9];
			vbuffer8[10][index + s] = vdata[10];
			vbuffer8[11][index + s] = vdata[11];
			vbuffer8[12][index + s] = vdata[12];
			vbuffer8[13][index + s] = vdata[13];
			vbuffer8[14][index + s] = vdata[14];
			vbuffer8[15][index + s] = vdata[15];
			vbuffer9[0][index + s] = vdata[0];
			vbuffer9[1][index + s] = vdata[1];
			vbuffer9[2][index + s] = vdata[2];
			vbuffer9[3][index + s] = vdata[3];
			vbuffer9[4][index + s] = vdata[4];
			vbuffer9[5][index + s] = vdata[5];
			vbuffer9[6][index + s] = vdata[6];
			vbuffer9[7][index + s] = vdata[7];
			vbuffer9[8][index + s] = vdata[8];
			vbuffer9[9][index + s] = vdata[9];
			vbuffer9[10][index + s] = vdata[10];
			vbuffer9[11][index + s] = vdata[11];
			vbuffer9[12][index + s] = vdata[12];
			vbuffer9[13][index + s] = vdata[13];
			vbuffer9[14][index + s] = vdata[14];
			vbuffer9[15][index + s] = vdata[15];
			vbuffer10[0][index + s] = vdata[0];
			vbuffer10[1][index + s] = vdata[1];
			vbuffer10[2][index + s] = vdata[2];
			vbuffer10[3][index + s] = vdata[3];
			vbuffer10[4][index + s] = vdata[4];
			vbuffer10[5][index + s] = vdata[5];
			vbuffer10[6][index + s] = vdata[6];
			vbuffer10[7][index + s] = vdata[7];
			vbuffer10[8][index + s] = vdata[8];
			vbuffer10[9][index + s] = vdata[9];
			vbuffer10[10][index + s] = vdata[10];
			vbuffer10[11][index + s] = vdata[11];
			vbuffer10[12][index + s] = vdata[12];
			vbuffer10[13][index + s] = vdata[13];
			vbuffer10[14][index + s] = vdata[14];
			vbuffer10[15][index + s] = vdata[15];
			vbuffer11[0][index + s] = vdata[0];
			vbuffer11[1][index + s] = vdata[1];
			vbuffer11[2][index + s] = vdata[2];
			vbuffer11[3][index + s] = vdata[3];
			vbuffer11[4][index + s] = vdata[4];
			vbuffer11[5][index + s] = vdata[5];
			vbuffer11[6][index + s] = vdata[6];
			vbuffer11[7][index + s] = vdata[7];
			vbuffer11[8][index + s] = vdata[8];
			vbuffer11[9][index + s] = vdata[9];
			vbuffer11[10][index + s] = vdata[10];
			vbuffer11[11][index + s] = vdata[11];
			vbuffer11[12][index + s] = vdata[12];
			vbuffer11[13][index + s] = vdata[13];
			vbuffer11[14][index + s] = vdata[14];
			vbuffer11[15][index + s] = vdata[15];
			index += NUM_PEs;

			#ifdef _DEBUGMODE_STATS
			actsutilityobj->globalstats_countvsread(VECTOR2_SIZE);
			#endif
		}
		depth_i += depth;
	}
	// exit(EXIT_SUCCESS); /////////
	return;
}








