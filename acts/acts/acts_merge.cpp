#define CONFIG_MERGE_VDATAS
#ifndef ALLVERTEXISACTIVE_ALGORITHM
#define CONFIG_MERGE_VPARTITIONS
#endif 

unsigned int MERGEP0_SPLIT_actvpstatsoffset(globalparams_t globalparams){
	unsigned int _offset = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ _offset += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ _offset += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif
	return _offset;
}

void MERGEP0_SPLIT_broadcastVs1(uint512_dt * kvdram0, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs1: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram1]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs1: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram1]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs2: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram2]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs2: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram2]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs3: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram3]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs3: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram3]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs4: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram4]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs4: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram4]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs5: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram5]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs5: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram5]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs6: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram6]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs6: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram6]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs7: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram7]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs7: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram7]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs8: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram8]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs8: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram8]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs9: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram9]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs9: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram9]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs10: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram10]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs10: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram10]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs11: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram11]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs11: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram11]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}
void MERGEP0_SPLIT_broadcastVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int lbasevoffset_kvs2 = 0;
	unsigned int offseti_kvs = 0;
	
	//>> expensive variables 
	unsigned int _VDATACHUNKSZ_KVS2 = globalparams.SIZEKVS2_REDUCEPARTITION / NUM_PEs;
	unsigned int _VMASKCHUNKSZ_KVS2 = globalparams.SIZEKVS_VMASKBUFFER / NUM_PEs;
	unsigned int _TOTALSIZEKVS_VMASKBUFFER = globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER;
	//>>
	
	for(unsigned int partition=0; partition<globalparamsv.NUM_PROCESSEDGESPARTITIONS; partition++){
	#pragma HLS PIPELINE II=1
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs12: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data from vdram to [kvdram0:kvdram12]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_broadcastVs12: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGEP0_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
					kvdram11[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	#endif 
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram12]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram11[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif 
	return;
}

void MERGEP0_SPLIT_BASICbroadcastVs1(uint512_dt * kvdram0, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram1]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram1]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram2]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram2]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram3]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram3]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram4]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram4]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram5]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram5]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram6]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram6]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram7]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram7]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram8]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram8]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram9]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram9]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram10]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram10]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram11]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram11]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}
void MERGEP0_SPLIT_BASICbroadcastVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTILP0_getglobalparams(kvdram0, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram12]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
		kvdram11[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + t];
	}
	#endif
	
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram12]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram11[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	#endif
	return;
}

void MERGEP0_SPLIT_mergeVs1(uint512_dt * kvdram0, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 1:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs1: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 1:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 1:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs1: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 2:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs2: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 2:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 2:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs2: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 2:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 2:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs2: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 3:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 3:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 3:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 3:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 3:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 3:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 3:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer2[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs3: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 4:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 4:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer2[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer3[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs4: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 5:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 5:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer2[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer3[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer4[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs5: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 6:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 6:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer2[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer3[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer4[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer5[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs6: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 7:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 7:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer2[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer3[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer4[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer5[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer6[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs7: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 8:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 8:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer2[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer3[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer4[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer5[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer6[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer7[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs8: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 9:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 9:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer2[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer3[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer4[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer5[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer6[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer7[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer8[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs9: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 10:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 10:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer2[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer3[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer4[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer5[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer6[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer7[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer8[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer9[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs10: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 10, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer10[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 11:: [instance 10, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP10B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 11:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer2[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer3[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer4[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer5[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer6[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer7[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer8[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer9[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer10[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs11: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void MERGEP0_SPLIT_mergeVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram, keyvalue_t globalstatsbuffer0[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer1[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer2[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer3[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer4[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer5[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer6[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer7[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer8[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer9[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer10[BLOCKRAM_SIZE],keyvalue_t globalstatsbuffer11[BLOCKRAM_SIZE], globalparams_t globalparams, globalparams_t globalparamsv){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	keyvalue_t globalstatsbuffer[BLOCKRAM_SIZE];
	
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	unsigned int GraphAlgo = globalparams.ALGORITHMINFO_GRAPHALGORITHMID;
	unsigned int GraphAlgoClass = globalparams.ALGORITHMINFO_GRAPHALGORITHMCLASS;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	//>>
	
	// vertices data
	#ifdef CONFIG_MERGE_VDATAS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer0[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP0B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer1[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP1B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer2[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP2B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer3[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP3B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer4[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP4B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer5[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP5B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer6[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP6B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer7[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP7B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer8[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP8B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer9[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP9B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 10, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer10[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 10, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP10B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGEP0_SPLIT_MERGEVSLOOP11: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 11, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGEP0_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(globalstatsbuffer11[partition].key > 0 || GraphAlgoClass == ALGORITHMCLASS_ALLVERTEXISACTIVE){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge::MERGEP0_SPLIT_merge Vertices 12:: [instance 11, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
			#endif
			MERGEP0_SPLIT_MERGEVSLOOP11B: for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram11[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGEP0_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
			partitionmasks[n] = partitionmasks[n] | kvdram11[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge::MERGEP0_SPLIT_merge Partition Masks 12:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
			#endif
			vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffset_kvs + runningpartition_i + n]
				#ifdef _WIDEWORD
				.range(31, 0)
				#else 
				.data[0].key
				#endif 
				= partitionmasks[n];
		}
		
		for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
		runningpartition_i += NUM_PEs;
		vpmoffseti_kvs += NUM_PEs;
	}
	#endif 
	
	// save stats 
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer0[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer1[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer2[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer3[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer4[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer5[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer6[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer7[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer8[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer9[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer10[k]; }
	for(unsigned int k=0; k<BLOCKRAM_SIZE; k++){ globalstatsbuffer[k] = globalstatsbuffer11[k]; }
	MEMACCESSP0_commitkvstats2(vdram, globalstatsbuffer, globalparamsv, actvpstats_beginoffset, globalparamsv.NUM_REDUCEPARTITIONS); // FIXME.
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGEP0_SPLIT_mergeVs12: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

extern "C" {
void TOPP0_topkernelS(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * vdram){
#pragma HLS INTERFACE m_axi port = vdramA offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = vdramB offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = vdramC offset = slave bundle = gmem2
#pragma HLS INTERFACE m_axi port = vdram offset = slave bundle = gmem3

#pragma HLS INTERFACE s_axilite port = vdramA bundle = control
#pragma HLS INTERFACE s_axilite port = vdramB bundle = control
#pragma HLS INTERFACE s_axilite port = vdramC bundle = control
#pragma HLS INTERFACE s_axilite port = vdram bundle = control

#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATA_PACK variable = vdramA
#pragma HLS DATA_PACK variable = vdramB
#pragma HLS DATA_PACK variable = vdramC
#pragma HLS DATA_PACK variable = vdram

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: ACTS EXCHANGE LAUNCHED. Exchanging vertices across different SLRs..."<<endl; 
	#endif
	
	unsigned int statssizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = statssizes_kvs
	unsigned int partitionoffseti = 0;
	
	globalparams_t globalparamsvA = UTILP0_getglobalparams(vdramA, 0);
	globalparams_t globalparamsvB = UTILP0_getglobalparams(vdramB, 0);
	globalparams_t globalparamsvC = UTILP0_getglobalparams(vdramC, 0);
	globalparams_t globalparamsv = UTILP0_getglobalparams(vdram, 0);
	unsigned int actvpstats_beginoffset = MERGEP0_SPLIT_actvpstatsoffset(globalparamsvA);
	
	//>> expensive variables 
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR2 = NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR1 = NUMCOMPUTEUNITS_SLR1 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR0 = NUMCOMPUTEUNITS_SLR0 * globalparamsvA.NUM_REDUCEPARTITIONS;
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS2_REDUCEPARTITION;
	//>>
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: retrieving stats from vdramA, vdramB & vdramC. "<<endl; 
	#endif 
	MEMACCESSP0_retreievekvstats(vdramA, statssizes_kvs[0], globalparamsvA, actvpstats_beginoffset, _TOTALNUMREDUCEPARTITIONS_SLR2);
	MEMACCESSP0_retreievekvstats(vdramB, statssizes_kvs[1], globalparamsvB, actvpstats_beginoffset, _TOTALNUMREDUCEPARTITIONS_SLR1);
	MEMACCESSP0_retreievekvstats(vdramC, statssizes_kvs[2], globalparamsvC, actvpstats_beginoffset, _TOTALNUMREDUCEPARTITIONS_SLR0);

	// vertices
	#ifdef CONFIG_MERGE_VDATAS
	partitionoffseti = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramA to vdramB & vdramC. [begin offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif
	MERGEP0_SPLIT_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 20", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[0][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGEP0_SPLIT_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetA_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetA_kvs += globalparamsvA.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramA to vdramB & vdramC. [end offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif

	partitionoffseti = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramB to vdramA & vdramC. [begin offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	MERGEP0_SPLIT_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		for(unsigned int partition=0; partition<globalparamsvB.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 21", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[1][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGEP0_SPLIT_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetB_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetB_kvs += globalparamsvB.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramB to vdramA & vdramC. [end offset: "<<voffsetB_kvs<<"]"<<endl; 
	#endif
	
	partitionoffseti = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramC to vdramA & vdramB. [begin offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	MERGEP0_SPLIT_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){ // NUMCOMPUTEUNITS_SLR1AND2
		for(unsigned int partition=0; partition<globalparamsvC.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("TOPP0_topkernelS: ERROR 22", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[2][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge::exchange vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGEP0_SPLIT_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_SRCVERTICESDATA + voffsetC_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetC_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramC to vdramA & vdramB. [end offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	#endif 
	
	// vertices partition masks
	#ifdef CONFIG_MERGE_VPARTITIONS
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: merging vertices partition masks across vdramA, vdramB & vdramC."<<endl; 
	#endif
	unsigned int pA = 0; unsigned int pB = 0; unsigned int pC = 0; 
	MERGEP0_SPLIT_EXCHANGEVPMS_LOOP1: for(unsigned int partition=0; partition<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; partition++){
		#ifdef _WIDEWORD
		pA = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		pB = vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		pC = vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		pA = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		pB = vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		pC = vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
		
		unsigned int p = pA | pB | pC;
		#ifdef _DEBUGMODE_KERNELPRINTS
		if(p > 0){ cout<<"TOPP0_topkernelS:: partition mask: "<<partition<<" is active: pA: "<<pA<<", pB: "<<pB<<", pC: "<<pC<<endl; }
		#endif
		
		#ifdef _WIDEWORD
		vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = p;
		vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = p;
		vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0) = p;
		#else 
		vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = p;
		vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = p;
		vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key = p;
		#endif 
		
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"--- TOPP0_topkernelS: vdramA["<<partition<<"]: "<<p<<endl; 
		#endif
	}
	#endif 
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"TOPP0_topkernelS: Finished. voffsetA: "<<voffsetA_kvs * VECTOR_SIZE<<", voffsetB: "<<voffsetB_kvs * VECTOR_SIZE<<", voffsetC: "<<voffsetC_kvs * VECTOR_SIZE<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
}


