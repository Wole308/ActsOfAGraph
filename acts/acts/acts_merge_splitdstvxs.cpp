#include "acts_merge_splitdstvxs.h"
using namespace std;

#ifdef SW
acts_merge_splitdstvxs::acts_merge_splitdstvxs(mydebug * _mydebugobj){ 
	actsutilityobj = new actsutility(); 
	acts_utilobj = new acts_util(_mydebugobj); 
	processedges_splitdstvxsobj = new processedges_splitdstvxs(_mydebugobj);
	partitionupdatesobj = new partitionupdates(_mydebugobj);
	reduceupdatesobj = new reduceupdates(_mydebugobj);
	mem_accessobj = new mem_access_splitdstvxs(_mydebugobj);
	actsobj = new acts(_mydebugobj);
	mydebugobj = _mydebugobj;
}
acts_merge_splitdstvxs::~acts_merge_splitdstvxs(){}
#endif

unsigned int acts_all::MERGE_SPLIT_actvpstatsoffset(globalparams_t globalparams){
	unsigned int actvpstats_beginoffset = 0;
	#ifdef ENABLERECURSIVEPARTITIONING
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH-1; k++){ actvpstats_beginoffset += (1 << (NUM_PARTITIONS_POW * k)); }
	#else 
	for(unsigned int k=0; k<globalparams.ACTSPARAMS_TREEDEPTH; k++){ actvpstats_beginoffset += (1 << (NUM_PARTITIONS_POW * k)); }
	#endif
	return actvpstats_beginoffset;
}

value_t acts_all::MERGE_SPLIT_applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo){
	value_t temp = 0;
	if(GraphAlgo == PAGERANK){
		// temp = vtemp + res;
		// temp = 0.5 + (1 - 0.5)*res / vdeg; // IDEAL
		temp = res;
	} else if(GraphAlgo == BFS){
		temp = res;
	} else if(GraphAlgo == SSSP){
		temp = UTIL_amin(vtemp, res);
	} else {
		temp = NAp;
	}
	return temp;
}

void acts_all::MERGE_SPLIT_broadcastVs1(uint512_dt * kvdram0, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs1: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram1]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs1: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram1]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs1: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram1]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs2: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram2]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs2: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram2]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs2: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram2]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs3: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram3]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs3: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram3]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs3: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram3]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs4: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram4]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs4: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram4]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs4: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs5: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram5]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs5: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram5]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs5: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs6: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram6]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs6: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram6]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs6: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs7: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram7]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs7: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram7]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs7: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs8: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram8]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs8: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram8]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs8: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs9: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram9]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs9: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram9]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs9: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs10: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram10]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs10: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram10]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs10: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs11: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram11]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs11: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram11]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs11: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_broadcastVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[BLOCKRAM_SIZE]; // AUTOMATEME FOR LARGER SIZES.
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
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
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs12: ERROR 23", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		#ifdef _WIDEWORD
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].range(31, 0);
		#else 
		psizes_kvs[partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition].data[0].key;
		#endif 
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram12]."<<endl; 	
	#endif
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs12: ERROR 24", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VDATACHUNKSZ_KVS2; continue; } 
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VDATAS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXS_LOOP: for(unsigned int k=0; k<_VDATACHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
					kvdram11[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + offseti_kvs + k];
				}
				offseti_kvs += _VDATACHUNKSZ_KVS2;
			}
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram12]."<<endl; 	
	#endif
	
	offseti_kvs = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		unsigned int p_index = 0;
		for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
			for(unsigned int s=0; s<NUM_PEs; s++){
				#ifdef _DEBUGMODE_CHECKS2
				actsutilityobj->checkoutofbounds("MERGE_SPLIT_broadcastVs12: ERROR 25", p_index, BLOCKRAM_SIZE, NAp, NAp, NAp);
				#endif
				if(psizes_kvs[p_index++] == 0){ offseti_kvs += _VMASKCHUNKSZ_KVS2; continue; }
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"MERGE::BROADCAST VMASKS:: partition "<<(partition * NUM_PEs) + s<<" is active. [partition: "<<partition<<", s: "<<s<<", offseti_kvs: "<<offseti_kvs<<"]"<<endl; 
				#endif 
				
				MERGE_SPLIT_BROADCASTVTXMS_LOOP: for(unsigned int k=0; k<_VMASKCHUNKSZ_KVS2; k++){
				#pragma HLS PIPELINE II=1
					kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
					kvdram11[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + offseti_kvs + k];
				}
				offseti_kvs += _VMASKCHUNKSZ_KVS2;
			}
		}
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}

void acts_all::MERGE_SPLIT_BASICbroadcastVs1(uint512_dt * kvdram0, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram1]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram1]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram1]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram2]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram2]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram2]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram3]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram3]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices partition masks from vdram to [kvdram0:kvdram3]."<<endl; 			 
	#endif
	for(unsigned int partition=0; partition<globalparams.NUM_PROCESSEDGESPARTITIONS; partition++){
		#pragma HLS PIPELINE II=1
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESPARTITIONMASK + partition];
	}
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram4]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram4]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram5]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram5]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram6]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram6]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram7]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram7]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram8]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram8]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram9]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram9]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram10]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram10]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram11]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram11]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}
void acts_all::MERGE_SPLIT_BASICbroadcastVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: ACTS BROADCAST LAUNCHED. Broadcasting vertices..."<<endl; 
	#endif
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices data masks from vdram to [kvdram0:kvdram12]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2 * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram1[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram2[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram3[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram4[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram5[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram6[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram7[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram8[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram9[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram10[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
		kvdram11[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + t] = vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + t];
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::BROADCAST:: broadcasting vertices masks from vdram to [kvdram0:kvdram12]."<<endl; 	
	#endif
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
		kvdram11[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t] = vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + t];
	}

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
	
	/////////////////////////////////////////////// REMOVEME
	// #ifdef _DEBUGMODE_CHECKS2
	// for(unsigned int t=0; t<globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; t++){
		// for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			// unsigned int vmdata;
			// unsigned int data;
			// if(v%2==0){ data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			// 			// vmdata = UTIL_READFROM_UINT(data, 0, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 1, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 2, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 3, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 4, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 5, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 6, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 7, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 8, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 9, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 10, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 11, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 12, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 13, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 14, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 15, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 16, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 17, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 18, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 19, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 20, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 21, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 22, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 23, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 24, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 25, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 26, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 27, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 28, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 29, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 30, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 			// vmdata = UTIL_READFROM_UINT(data, 31, 1);
			// if(vmdata==1){ mydebugobj->increment(3, globalparams.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			// 		// }
	// }
	// #endif 
	///////////////////////////////////////////////
	return;
}

void acts_all::MERGE_SPLIT_mergeVs1(uint512_dt * kvdram0, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[1][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 1:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs1: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 1:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 1:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs1: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 1:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 1:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs1: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs2(uint512_dt * kvdram0,uint512_dt * kvdram1, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[2][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 2:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs2: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 2:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 2:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs2: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 2:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 2:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs2: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 2:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 2:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs2: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 2:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 2:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs2: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs3(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 3:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 3:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 3:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 3:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 3:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs3: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 3:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 3:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs3: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 3:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 3:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs3: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 3:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 3:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs3: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 3:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 3:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs3: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs4(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[4][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 4:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 4:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 4:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 4:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 4:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 4:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 4:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs4: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 4:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 4:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs4: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 4:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 4:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs4: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 4:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 4:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs4: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 4:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 4:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs4: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 4:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 4:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs4: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs5(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[5][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 5:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 5:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 5:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 5:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 5:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 5:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 5:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 5:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 5:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs5: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 5:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 5:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs5: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 5:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 5:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs5: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 5:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 5:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs5: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 5:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 5:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs5: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 5:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 5:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs5: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 5:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 5:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs5: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs6(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[6][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 6:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs6: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 6:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 6:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs6: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs7(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[7][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 7:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs7: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 7:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 7:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs7: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs8(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[8][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 8:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 7, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs8: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 8:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 8:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs8: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs9(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[9][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[8], globalparamsv, actvpstats_beginoffset + 8 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 9:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 7, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 8, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs9: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 9:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 9:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs9: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs10(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[10][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram9, psizes_kvs[9], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[8], globalparamsv, actvpstats_beginoffset + 8 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[9], globalparamsv, actvpstats_beginoffset + 9 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 10:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 7, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 8, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 9, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs10: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 10:: [instance 9, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 10:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs10: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs11(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[11][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram9, psizes_kvs[9], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram10, psizes_kvs[10], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[8], globalparamsv, actvpstats_beginoffset + 8 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[9], globalparamsv, actvpstats_beginoffset + 9 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[10], globalparamsv, actvpstats_beginoffset + 10 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 10, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 11:: [instance 10, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 7, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 8, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 9, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 9, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 10, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs11: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 11:: [instance 10, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 11:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs11: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}
void acts_all::MERGE_SPLIT_mergeVs12(uint512_dt * kvdram0,uint512_dt * kvdram1,uint512_dt * kvdram2,uint512_dt * kvdram3,uint512_dt * kvdram4,uint512_dt * kvdram5,uint512_dt * kvdram6,uint512_dt * kvdram7,uint512_dt * kvdram8,uint512_dt * kvdram9,uint512_dt * kvdram10,uint512_dt * kvdram11, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: ACTS MERGE LAUNCHED. Merging vertices..."<<endl; 
	#endif
	
	unsigned int psizes_kvs[12][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = psizes_kvs
	unsigned int partitionmasks[NUM_PEs];
	#pragma HLS ARRAY_PARTITION variable=partitionmasks complete
	for(unsigned int k=0; k<NUM_PEs; k++){ partitionmasks[k] = 0; }
	
	globalparams_t globalparams = UTIL_getglobalparams(kvdram0);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsv);
	unsigned int vpmoffset_kvs = 0;
	
	//>> expensive variables 
	unsigned int lbasevoffset_kvs2;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevoffset_kvs2 = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevoffset_kvs2 = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevoffset_kvs2 = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS2_REDUCEPARTITION; }					
	
	unsigned int lbasevmoffset_kvs = 0;
	if(globalparams.ACTSPARAMS_INSTID >= 0 && globalparams.ACTSPARAMS_INSTID < NUMCOMPUTEUNITS_SLR2){ lbasevmoffset_kvs = 0; }
	else if(globalparams.ACTSPARAMS_INSTID >= NUMCOMPUTEUNITS_SLR2 && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1))){ lbasevmoffset_kvs = NUMCOMPUTEUNITS_SLR2 * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	else if((globalparams.ACTSPARAMS_INSTID >= (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)) && (globalparams.ACTSPARAMS_INSTID < (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0))){ lbasevmoffset_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparams.NUM_REDUCEPARTITIONS * globalparams.SIZEKVS_VMASKBUFFER; }
	//>>
	
	// stats
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging stats."<<endl; 
	#endif	
MEMACCESS_SPL_retreievekvstats(kvdram0, psizes_kvs[0], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram1, psizes_kvs[1], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram2, psizes_kvs[2], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram3, psizes_kvs[3], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram4, psizes_kvs[4], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram5, psizes_kvs[5], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram6, psizes_kvs[6], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram7, psizes_kvs[7], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram8, psizes_kvs[8], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram9, psizes_kvs[9], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram10, psizes_kvs[10], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_retreievekvstats(kvdram11, psizes_kvs[11], globalparams, actvpstats_beginoffset, globalparams.NUM_REDUCEPARTITIONS);
	
	
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[0], globalparamsv, actvpstats_beginoffset + 0 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[1], globalparamsv, actvpstats_beginoffset + 1 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[2], globalparamsv, actvpstats_beginoffset + 2 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[3], globalparamsv, actvpstats_beginoffset + 3 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[4], globalparamsv, actvpstats_beginoffset + 4 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[5], globalparamsv, actvpstats_beginoffset + 5 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[6], globalparamsv, actvpstats_beginoffset + 6 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[7], globalparamsv, actvpstats_beginoffset + 7 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[8], globalparamsv, actvpstats_beginoffset + 8 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[9], globalparamsv, actvpstats_beginoffset + 9 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[10], globalparamsv, actvpstats_beginoffset + 10 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
MEMACCESS_SPL_commitkvstats2(vdram, psizes_kvs[11], globalparamsv, actvpstats_beginoffset + 11 * globalparams.NUM_REDUCEPARTITIONS, globalparams.NUM_REDUCEPARTITIONS);
	
	// vertices data
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices. [begin offset: "<<lbasevoffset_kvs2<<"]"<<endl; 
	#endif
	unsigned int voffset_kvs2 = 0;
	unsigned int voffseti_kvs2 = 0;
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 0, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 0, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram0[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 1, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 1, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram1[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 2, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 2, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram2[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 3, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 3, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram3[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 4, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 4, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram4[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 5, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 5, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram5[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 6, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 6, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram6[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 7, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 7, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram7[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 8, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 8, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram8[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 9, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 9, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram9[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 10, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 10, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram10[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	voffseti_kvs2 = 0;
	MERGE_SPLIT_MERGEVSLOOP11: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 11, partition "<<partition<<"]: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 20", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[11][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Vertices 12:: [instance 11, partition "<<partition<<"] is active: [offset: "<<(lbasevoffset_kvs2 + voffseti_kvs2)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS2_REDUCEPARTITION<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS2_REDUCEPARTITION; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + lbasevoffset_kvs2 + voffset_kvs2 + k] = kvdram11[globalparams.BASEOFFSETKVS_DESTVERTICESDATA + voffseti_kvs2 + k];
			}
		}
		voffset_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
		voffseti_kvs2 += globalparams.SIZEKVS2_REDUCEPARTITION;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices. [end offset: "<<lbasevoffset_kvs2 + voffset_kvs2<<"]"<<endl; 
	#endif
	
	// vertices data masks 
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging vertices masks. [begin offset: "<<lbasevmoffset_kvs<<"]"<<endl; 
	#endif
	unsigned int vmoffset_kvs = 0;
	unsigned int vmoffseti_kvs = 0;
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP0: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 0, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[0][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 0, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[0]["<<partition<<"]: "<<psizes_kvs[0][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram0[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP1: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 1, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[1][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 1, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[1]["<<partition<<"]: "<<psizes_kvs[1][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram1[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP2: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 2, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[2][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 2, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[2]["<<partition<<"]: "<<psizes_kvs[2][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram2[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP3: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 3, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[3][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 3, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[3]["<<partition<<"]: "<<psizes_kvs[3][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram3[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP4: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 4, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[4][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 4, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[4]["<<partition<<"]: "<<psizes_kvs[4][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram4[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP5: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 5, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[5][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 5, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[5]["<<partition<<"]: "<<psizes_kvs[5][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram5[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP6: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 6, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[6][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 6, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[6]["<<partition<<"]: "<<psizes_kvs[6][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram6[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP7: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 7, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[7][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 7, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[7]["<<partition<<"]: "<<psizes_kvs[7][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram7[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP8: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 8, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[8][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 8, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[8]["<<partition<<"]: "<<psizes_kvs[8][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram8[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP9: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 9, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[9][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 9, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[9]["<<partition<<"]: "<<psizes_kvs[9][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram9[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP10: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 10, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[10][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 10, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[10]["<<partition<<"]: "<<psizes_kvs[10][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram10[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	vmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVMSLOOP11: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		#ifdef _DEBUGMODE_KERNELPRINTS
		cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 11, reduce partition "<<partition<<"]: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
		#endif 
		#ifdef _DEBUGMODE_CHECKS2
		actsutilityobj->checkoutofbounds("MERGE_SPLIT_mergeVs12: ERROR 21", partition, BLOCKRAM_SIZE, NAp, NAp, NAp);
		#endif
		if(psizes_kvs[11][partition] > 0){
			#ifdef _DEBUGMODE_KERNELPRINTS
			cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Masks 12:: [instance 11, reduce partition "<<partition<<"] is active: [offset: "<<(lbasevmoffset_kvs + vmoffseti_kvs)*VECTOR2_SIZE<<", size: "<<globalparams.SIZEKVS_VMASKBUFFER<<", psizes_kvs[11]["<<partition<<"]: "<<psizes_kvs[11][partition]<<"] "<<endl;
			#endif
			for(unsigned int k=0; k<globalparams.SIZEKVS_VMASKBUFFER; k++){
			#pragma HLS PIPELINE II=1
				vdram[globalparamsv.BASEOFFSETKVS_VERTICESDATAMASK + lbasevmoffset_kvs + vmoffset_kvs + k] = kvdram11[globalparams.BASEOFFSETKVS_VERTICESDATAMASK + vmoffseti_kvs + k];
			}
		}
		vmoffset_kvs += globalparams.SIZEKVS_VMASKBUFFER;
		vmoffseti_kvs += globalparams.SIZEKVS_VMASKBUFFER;
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: FINISHED merging vertices masks. [end offset: "<<lbasevmoffset_kvs + vmoffset_kvs<<"]"<<endl; 
	#endif
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::MERGE:: merging partition masks. [begin offset: 0]"<<endl; 
	#endif
	unsigned int runningpartition_i = 0;
	unsigned int vpmoffseti_kvs = 0;
	MERGE_SPLIT_MERGEVPMSLOOP: for(unsigned int partition=0; partition<globalparams.NUM_REDUCEPARTITIONS; partition++){
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram0[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram1[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram2[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram3[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram4[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram5[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram6[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram7[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram8[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram9[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram10[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			partitionmasks[n] = partitionmasks[n] | kvdram11[globalparams.BASEOFFSETKVS_VERTICESPARTITIONMASK + vpmoffseti_kvs + n]
				#ifdef _WIDEWORD
				.range(31, 0);
				#else 
				.data[0].key;
				#endif 
		}
		
		for(unsigned int n=0; n<NUM_PEs; n++){
		#pragma HLS PIPELINE II=1
			#ifdef _DEBUGMODE_KERNELPRINTS
			if(partitionmasks[n] > 0){ cout<<"acts_merge_splitdstvxs::MERGE_SPLIT_merge Partition Masks 12:: [process partition "<<runningpartition_i + n<<"] is active: [partitionmasks["<<runningpartition_i + n<<"]: "<<partitionmasks[n]<<""<<endl; }				
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
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_mergeVs12: Finished. voffset: "<<voffset_kvs2 * VECTOR2_SIZE<<", vmoffset: "<<vmoffset_kvs * VECTOR2_SIZE<<", runningpartition_i: "<<runningpartition_i<<endl;
	#endif
	// exit(EXIT_SUCCESS); //
	return;
}

void acts_all::MERGE_SPLIT_exchangeVs(uint512_dt * vdramA, uint512_dt * vdramB, uint512_dt * vdramC, uint512_dt * vdram){
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: ACTS EXCHANGE LAUNCHED. Exchanging vertices across different SLRs..."<<endl; 
	#endif
	
	unsigned int statssizes_kvs[3][BLOCKRAM_SIZE];
	#pragma HLS array_partition variable = statssizes_kvs
	unsigned int partitionoffseti = 0;
	
	globalparams_t globalparamsvA = UTIL_getglobalparams(vdramA);
	globalparams_t globalparamsvB = UTIL_getglobalparams(vdramB);
	globalparams_t globalparamsvC = UTIL_getglobalparams(vdramC);
	globalparams_t globalparamsv = UTIL_getglobalparams(vdram);
	unsigned int actvpstats_beginoffset = MERGE_SPLIT_actvpstatsoffset(globalparamsvA);
	
	//>> expensive variables 
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR2 = NUMCOMPUTEUNITS_SLR2 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR1 = NUMCOMPUTEUNITS_SLR1 * globalparamsvA.NUM_REDUCEPARTITIONS;
	unsigned int _TOTALNUMREDUCEPARTITIONS_SLR0 = NUMCOMPUTEUNITS_SLR0 * globalparamsvA.NUM_REDUCEPARTITIONS;
	
	unsigned int voffsetA_kvs = 0;
	unsigned int voffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS2_REDUCEPARTITION;
	unsigned int voffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS2_REDUCEPARTITION;
	
	unsigned int vmoffsetA_kvs = 0;
	unsigned int vmoffsetB_kvs = NUMCOMPUTEUNITS_SLR2 * globalparamsvB.NUM_REDUCEPARTITIONS * globalparamsvB.SIZEKVS_VMASKBUFFER;
	unsigned int vmoffsetC_kvs = (NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1) * globalparamsvC.NUM_REDUCEPARTITIONS * globalparamsvC.SIZEKVS_VMASKBUFFER;
	//>>
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE::EXCHANGE:: retrieving stats from vdramA, vdramB & vdramC. "<<endl; 
	#endif 
MEMACCESS_SPL_retreievekvstats(vdramA, statssizes_kvs[0], globalparamsvA, actvpstats_beginoffset, _TOTALNUMREDUCEPARTITIONS_SLR2);
MEMACCESS_SPL_retreievekvstats(vdramB, statssizes_kvs[1], globalparamsvB, actvpstats_beginoffset, _TOTALNUMREDUCEPARTITIONS_SLR1);
MEMACCESS_SPL_retreievekvstats(vdramC, statssizes_kvs[2], globalparamsvC, actvpstats_beginoffset, _TOTALNUMREDUCEPARTITIONS_SLR0);

	// vertices
	partitionoffseti = 0;
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices from vdramA to vdramB & vdramC. [begin offset: "<<voffsetA_kvs<<"]"<<endl; 
	#endif
	MERGE_SPLIT_EXCHANGEVS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("MERGE_SPLIT_exchangeVs: ERROR 20", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[0][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge_splitdstvxs::exchange vertices ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_SPLIT_EXCHANGEVS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetA_kvs + k];
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
	MERGE_SPLIT_EXCHANGEVS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		for(unsigned int partition=0; partition<globalparamsvB.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("MERGE_SPLIT_exchangeVs: ERROR 21", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[1][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge_splitdstvxs::exchange vertices ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_SPLIT_EXCHANGEVS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetB_kvs + k];
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
	MERGE_SPLIT_EXCHANGEVS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){ // NUMCOMPUTEUNITS_SLR1AND2
		for(unsigned int partition=0; partition<globalparamsvC.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("MERGE_SPLIT_exchangeVs: ERROR 22", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[2][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge_splitdstvxs::exchange vertices ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<voffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS2_REDUCEPARTITION<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_SPLIT_EXCHANGEVS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS2_REDUCEPARTITION; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k];
					vdram[globalparamsv.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_DESTVERTICESDATA + voffsetC_kvs + k];
				}
			}
			partitionoffseti += 1;
			voffsetC_kvs += globalparamsvC.SIZEKVS2_REDUCEPARTITION;
		}
	}
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: FINISHED transferring vertices from vdramC to vdramA & vdramB. [end offset: "<<voffsetC_kvs<<"]"<<endl; 
	#endif
	
	// vertices masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices masks from vdramA to vdramB & vdramC."<<endl; 
	#endif
	partitionoffseti = 0;
	MERGE_SPLIT_EXCHANGEVMS_LOOP1A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR2; i++){
		for(unsigned int partition=0; partition<globalparamsvA.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("MERGE_SPLIT_exchangeVs: ERROR 23", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[0][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge_splitdstvxs::exchange vmasks ::[A->B,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<vmoffsetA_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvA.SIZEKVS_VMASKBUFFER<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_SPLIT_EXCHANGEVMS_LOOP1B: for(unsigned int k=0; k<globalparamsvA.SIZEKVS_VMASKBUFFER; k++){
				#pragma HLS PIPELINE II=1
					vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetA_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetA_kvs + k] = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetA_kvs + k];
				}
			}
			partitionoffseti += 1;
			vmoffsetA_kvs += globalparamsvA.SIZEKVS_VMASKBUFFER;
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices masks from vdramB to vdramA & vdramC."<<endl; 
	#endif
	partitionoffseti = 0;
	MERGE_SPLIT_EXCHANGEVMS_LOOP2A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR1; i++){
		for(unsigned int partition=0; partition<globalparamsvB.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("MERGE_SPLIT_exchangeVs: ERROR 24", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[1][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge_splitdstvxs::exchange vmasks ::[B->A,C] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<vmoffsetB_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvB.SIZEKVS_VMASKBUFFER<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_SPLIT_EXCHANGEVMS_LOOP2B: for(unsigned int k=0; k<globalparamsvB.SIZEKVS_VMASKBUFFER; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetB_kvs + k];
					vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetB_kvs + k] = vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetB_kvs + k];
				}
			}
			partitionoffseti += 1;
			vmoffsetB_kvs += globalparamsvB.SIZEKVS_VMASKBUFFER;
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: transferring vertices masks from vdramC to vdramA & vdramB."<<endl; 
	#endif
	partitionoffseti = 0;
	MERGE_SPLIT_EXCHANGEVMS_LOOP3A: for(unsigned int i=0; i<NUMCOMPUTEUNITS_SLR0; i++){
		for(unsigned int partition=0; partition<globalparamsvC.NUM_REDUCEPARTITIONS; partition++){
			#ifdef _DEBUGMODE_CHECKS2
			actsutilityobj->checkoutofbounds("MERGE_SPLIT_exchangeVs: ERROR 25", partitionoffseti, BLOCKRAM_SIZE, NAp, NAp, NAp);
			#endif
			if(statssizes_kvs[2][partitionoffseti] > 0){
				#ifdef _DEBUGMODE_KERNELPRINTS
				cout<<"acts_merge_splitdstvxs::exchange vmasks ::[C->A,B] [instance "<<i<<", partition "<<partition<<"] is active: [offset: "<<vmoffsetC_kvs*VECTOR2_SIZE<<", size: "<<globalparamsvC.SIZEKVS_VMASKBUFFER<<", statssizes_kvs[0]["<<partitionoffseti<<"]: "<<statssizes_kvs[0][partitionoffseti]<<"] "<<endl;
				#endif
				MERGE_SPLIT_EXCHANGEVMS_LOOP3B: for(unsigned int k=0; k<globalparamsvC.SIZEKVS_VMASKBUFFER; k++){
				#pragma HLS PIPELINE II=1
					vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k];
					vdramB[globalparamsvB.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k] = vdramC[globalparamsvC.BASEOFFSETKVS_VERTICESDATAMASK + vmoffsetC_kvs + k];
				}
			}
			partitionoffseti += 1;
			vmoffsetC_kvs += globalparamsvC.SIZEKVS_VMASKBUFFER;
		}
	}
	
	// vertices partition masks
	#ifdef _DEBUGMODE_KERNELPRINTS3
	cout<<"MERGE::EXCHANGE:: merging vertices partition masks across vdramA, vdramB & vdramC."<<endl; 
	#endif
	unsigned int pA = 0; unsigned int pB = 0; unsigned int pC = 0; 
	MERGE_SPLIT_EXCHANGEVPMS_LOOP1: for(unsigned int partition=0; partition<globalparamsvA.NUM_PROCESSEDGESPARTITIONS; partition++){
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
		if(p > 0){ cout<<"MERGE_SPLIT_exchangeVs:: partition mask: "<<partition<<" is active: pA: "<<pA<<", pB: "<<pB<<", pC: "<<pC<<endl; }
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
		cout<<"--- MERGE_SPLIT_exchangeVs: vdramA["<<partition<<"]: "<<p<<endl; 
		#endif
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"MERGE_SPLIT_exchangeVs: Finished. voffsetA: "<<voffsetA_kvs * VECTOR_SIZE<<", voffsetB: "<<voffsetB_kvs * VECTOR_SIZE<<", voffsetC: "<<voffsetC_kvs * VECTOR_SIZE<<endl;
	cout<<"MERGE_SPLIT_exchangeVs: Finished. vmoffsetA: "<<vmoffsetA_kvs * VECTOR_SIZE<<", vmoffsetB: "<<vmoffsetB_kvs * VECTOR_SIZE<<", vmoffsetC: "<<vmoffsetC_kvs * VECTOR_SIZE<<endl;
	#endif
	
	/* ///////////////////////////////////////////////// REMOVEME
	#ifdef _DEBUGMODE_CHECKS2
	for(unsigned int t=0; t<NUMREDUCEPARTITIONS * VMASKBUFFERSZ_KVS * NUM_PEs; t++){
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			unsigned int vmdata;
			unsigned int data;
			if(v%2==0){ data = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].key; } else { data = vdramA[globalparamsvA.BASEOFFSETKVS_VERTICESDATAMASK + t].data[v/2].value; }
			vmdata = UTIL_READFROM_UINT(data, 0, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 1, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 2, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 3, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 4, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 5, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 6, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 7, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 8, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 9, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 10, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 11, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 12, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 13, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 14, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 15, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 16, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 17, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 18, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 19, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 20, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 21, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 22, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 23, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 24, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 25, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 26, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 27, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 28, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 29, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 30, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
			vmdata = UTIL_READFROM_UINT(data, 31, 1);
			if(vmdata==1){ mydebugobj->increment(3, globalparamsvA.ALGORITHMINFO_GRAPHITERATIONID, 1); }
	
		}
	}
	#endif 
	///////////////////////////////////////////////// */
	
	// exit(EXIT_SUCCESS); //
	return;
}
 
