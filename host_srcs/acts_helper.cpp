#include "acts_helper.h"
using namespace std;

acts_helper::acts_helper(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	myuniversalparams = _universalparams;
}
acts_helper::~acts_helper(){} 

unsigned int acts_helper::gethash(unsigned int vid){
	return vid % NUM_PEs;
}
unsigned int acts_helper::getlocalvid(unsigned int vid){
	unsigned int s = gethash(vid);
	return (vid - s) / NUM_PEs; 
}

void acts_helper::set_edgeblock_headers(int GraphIter, unsigned int v_p, 
	uint512_ivec_dt * tempvdram, uint512_ivec_dt * tempkvdram[NUM_PEs], unsigned int vdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS, unsigned int kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS,
		unsigned int * indexes[MAXNUM_PEs]){
	unsigned int num_LLPs = myuniversalparams.NUMREDUCEPARTITIONS * myuniversalparams.NUM_PARTITIONS; 
	unsigned int num_LLPset = (num_LLPs + (myuniversalparams.NUM_PARTITIONS - 1)) / myuniversalparams.NUM_PARTITIONS;
	
	for(unsigned int H=0; H<NUM_PEs; H++){
		if(utilityobj->isbufferused(H) == false){ continue; }
		for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
			#ifdef _DEBUGMODE_HOSTPRINTS3//4 // 4*
			if(v_p == 0){ cout<<"extract_stats++:: iter: "<<GraphIter + 1<<", indexes["<<H<<"]["<<v_p<<"]["<<llp_set<<"]: "<<indexes[H][llp_set]<<endl; } //if(v_p == 0){ }
			#endif 
			unsigned int offset = ((v_p * MAXNUMGRAPHITERATIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION) + ((GraphIter + 1) * MAXNUM_EDGEBLOCKS_PER_VPARTITION));
			tempkvdram[H][kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset + 0].data[llp_set] = indexes[H][llp_set]; // indexes[H][llp_set], 0xFFFFFFFF
		}
	}	
}

unsigned int acts_helper::extract_stats(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[MAXNUM_PEs], 
		vector<vertex_t> &srcvids, vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, 
			long double edgesprocessed_totals[128], tuple_t * vpartition_stats[MAXNUMGRAPHITERATIONS], unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS]){						
	if(myuniversalparams.ALGORITHM == BFS || myuniversalparams.ALGORITHM == SSSP){} else {
		for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ for(unsigned int t=0; t<myuniversalparams.NUMPROCESSEDGESPARTITIONS; t++){ vpartition_stats[iter][t].A = 10; vpartition_stats[iter][t].B = 11; }}
		edgesprocessed_totals[0] = edgedatabuffer.size(); 
		num_edges_processed[0] = edgedatabuffer.size(); 
		return 1; }
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"acts_helper:: running traditional sssp... "<<endl;
	#endif 
	
	unsigned int num_vPs = myuniversalparams.NUMPROCESSEDGESPARTITIONS;
	unsigned int vsize_vP = myuniversalparams.PROCESSPARTITIONSZ;
	unsigned int num_LLPs = myuniversalparams.NUMREDUCEPARTITIONS * myuniversalparams.NUM_PARTITIONS; 
	unsigned int vsize_LLP = myuniversalparams.REDUCEPARTITIONSZ_KVS2;
	unsigned int vsize_LLPset = vsize_LLP * VDATA_PACKINGSIZE;
	unsigned int num_LLPset = (num_LLPs + (myuniversalparams.NUM_PARTITIONS - 1)) / myuniversalparams.NUM_PARTITIONS;
	unsigned int max___ = ((myuniversalparams.NUM_EDGES / EDGEDATA_PACKINGSIZE) / NUM_PEs) / NUM_EDGESKVS_PER_UPROPBLOCK + 8192; // ~20K
	
	uint512_ivec_dt * tempvdram = (uint512_ivec_dt *)vdram;	
	uint512_ivec_dt * tempkvdram[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ tempkvdram[i] = (uint512_ivec_dt *)kvbuffer[i]; }
	
	unsigned int * upropblock_stats[MAXNUMGRAPHITERATIONS][MAXNUM_VPs]; 
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ 
		for(unsigned int v_p=0; v_p<MAXNUM_VPs; v_p++){ 
			upropblock_stats[iter][v_p] = new unsigned int[myuniversalparams.KVDATA_RANGE / NUM_VERTICES_PER_UPROPBLOCK];
			for(unsigned int t=0; t<myuniversalparams.KVDATA_RANGE / NUM_VERTICES_PER_UPROPBLOCK; t++){ upropblock_stats[iter][v_p][t] = 0; }
		}
	}
	
	vector<value_t> actvvs;
	vector<value_t> actvvs_nextit;
	unsigned int * vdatas = new unsigned int[myuniversalparams.KVDATA_RANGE];
	unsigned int * vdatas_tmp = new unsigned int[myuniversalparams.KVDATA_RANGE];
	unsigned int * indexes[MAXNUM_PEs]; for(unsigned int i=0; i<MAXNUM_PEs; i++){ indexes[i] = new unsigned int[MAXNUM_LLPSETs]; }
	unsigned int * edgeblock_mask[MAXNUM_PEs]; for(unsigned int i=0; i<MAXNUM_PEs; i++){ edgeblock_mask[i] = new unsigned int[max___]; }  //[ind]
	tuple_t * iteration_stats[MAXNUMGRAPHITERATIONS]; for(unsigned int i=0; i<MAXNUMGRAPHITERATIONS; i++){ iteration_stats[i] = new tuple_t[myuniversalparams.NUMPROCESSEDGESPARTITIONS]; }
	
	for(unsigned int i=0; i<myuniversalparams.KVDATA_RANGE; i++){ vdatas[i] = 0xFFFFFFFF; vdatas_tmp[i] = 0xFFFFFFFF; }
	unsigned int total_edges_processed = 0;
	for(unsigned int i=0; i<128; i++){ edgesprocessed_totals[i] = 0; }
	for(unsigned int i=0; i<srcvids.size(); i++){ actvvs.push_back(srcvids[i]); }
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"acts_helper: number of active vertices for iteration 0: 1"<<endl;
	#endif 
	for(unsigned int i=0; i<actvvs.size(); i++){ vdatas[actvvs[i]] = 0; }
	unsigned int GraphIter=0;
	vpartition_stats[0][0].A = 1; 
	upropblock_stats[0][0][actvvs[0] / NUM_VERTICES_PER_UPROPBLOCK] = 1;
	for(unsigned int i=0; i<MAXNUM_PEs; i++){ for(unsigned int t=0; t<MAXNUM_LLPSETs; t++){ indexes[i][t] = 0; }}
	for(unsigned int i=0; i<MAXNUM_PEs; i++){ for(unsigned int t=0; t<max___; t++){ edgeblock_mask[i][t] = 0; }}
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ num_edges_processed[iter] = 0; }
	unsigned int num_iters = MAXNUMGRAPHITERATIONS; if(myuniversalparams.NUM_ITERATIONS < 5){ num_iters = myuniversalparams.NUM_ITERATIONS; }
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ for(unsigned int t=0; t<myuniversalparams.NUMPROCESSEDGESPARTITIONS; t++){ iteration_stats[iter][t].A = 0; iteration_stats[iter][t].B = 0; }}
	bool onetime = false;
	
	for(GraphIter=0; GraphIter<MAXNUMGRAPHITERATIONS; GraphIter++){
		for(unsigned int i=0; i<MAXNUM_PEs; i++){ for(unsigned int t=0; t<MAXNUM_LLPSETs; t++){ indexes[i][t] = 0; }}
		for(unsigned int i=0; i<MAXNUM_PEs; i++){ for(unsigned int t=0; t<max___; t++){ edgeblock_mask[i][t] = 0; }}
		
		// processing phase 
		for(unsigned int i=0; i<actvvs.size(); i++){
			unsigned int vid = actvvs[i];
			
			edge_t vptr_begin = vertexptrbuffer[vid];
			edge_t vptr_end = vertexptrbuffer[vid+1];
			edge_t edges_size = vptr_end - vptr_begin;
			if(vptr_end < vptr_begin){ continue; } // FIXME.
			#ifdef _DEBUGMODE_CHECKS3
			if(vptr_end < vptr_begin){ cout<<"ERROR: vptr_end("<<vptr_end<<") < vptr_begin("<<vptr_begin<<"). exiting..."<<endl; exit(EXIT_FAILURE); }
			#endif
			
			num_edges_processed[GraphIter] += edges_size;
			vpartition_stats[GraphIter][vid / myuniversalparams.PROCESSPARTITIONSZ].B += edges_size; // 
			
			for(unsigned int k=0; k<edges_size; k++){
				unsigned int dstvid = edgedatabuffer[vptr_begin + k].dstvid;
				
				unsigned int res = vdatas[vid] + 1;
				value_t vprop = vdatas[dstvid];
				value_t vtemp = min(vprop, res);
				vdatas_tmp[dstvid] = vtemp;	
				
				total_edges_processed += 1;
				edgesprocessed_totals[GraphIter] += 1; 
			}			
		}
		
		// apply phase 
		unsigned int v_p = 0;
		for(unsigned int vid=0; vid<myuniversalparams.KVDATA_RANGE; vid++){
			// resets 
			if(vid % myuniversalparams.PROCESSPARTITIONSZ == 0){ 
				for(unsigned int H=0; H<NUM_PEs; H++){ for(unsigned int llp_set=0; llp_set<myuniversalparams.NUMREDUCEPARTITIONS; llp_set++){ indexes[H][llp_set] = 0; }}
				for(unsigned int H=0; H<NUM_PEs; H++){ for(unsigned int t=0; t<max___; t++){ edgeblock_mask[H][t] = 0; }}
			}
				
			// apply 
			if(vdatas_tmp[vid] != vdatas[vid]){
				vdatas[vid] = vdatas_tmp[vid];
				actvvs_nextit.push_back(vid);
				utilityobj->checkoutofbounds("acts_helper:: ERROR 20", vid / myuniversalparams.PROCESSPARTITIONSZ, myuniversalparams.NUMPROCESSEDGESPARTITIONS, vid, vid, vid);
			
				// load vpartition & upropblock stats
				unsigned int v_p_ = vid / myuniversalparams.PROCESSPARTITIONSZ;
				unsigned int lvid = vid - (v_p_ * myuniversalparams.PROCESSPARTITIONSZ);
				if(upropblock_stats[GraphIter+1][v_p_][lvid / NUM_VERTICES_PER_UPROPBLOCK] == 0){ 
					upropblock_stats[GraphIter+1][v_p_][lvid / NUM_VERTICES_PER_UPROPBLOCK] = 1; 
					vpartition_stats[GraphIter+1][vid / myuniversalparams.PROCESSPARTITIONSZ].A += 1; 
				}
				iteration_stats[GraphIter+1][vid / myuniversalparams.PROCESSPARTITIONSZ].A += 1; 	
	
				// load edgeblock stats
				#ifdef _DEBUGMODE_HOSTPRINTS4
				if(onetime == false){ cout<<">>> extract_stats: populating metadata (edgeblock stats) ... "<<endl; onetime = true; }
				#endif
			}
		}
		
		// check for finish
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"acts_helper: number of active vertices for iteration "<<GraphIter + 1<<": "<<actvvs_nextit.size()<<""<<endl;
		#endif 
		if(actvvs_nextit.size() == 0 || GraphIter >= 14){ 
			#ifdef _DEBUGMODE_HOSTPRINTS3
			cout<<"no more activer vertices to process. breaking out... "<<endl; 
			#endif 
			break; 
		}
	
		actvvs.clear();
		for(unsigned int i=0; i<actvvs_nextit.size(); i++){ actvvs.push_back(actvvs_nextit[i]); }
		actvvs_nextit.clear();
	}
	
	// load vpartition stats
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<">>> extract_stats: populating metadata (vpartition_stats) ... "<<endl;		
	#endif
	unsigned int vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESPARTITIONMASK].data[0].key;
	#ifdef NOT____USED
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){
		for(unsigned int v_p=0; v_p<myuniversalparams.NUMPROCESSEDGESPARTITIONS; v_p++){
			tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + v_p].data[iter] = vpartition_stats[iter][v_p].A;
			if(iter==0 && v_p < 16 && false){ cout<<"~~~ acts_helper: tempvdram["<<vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + v_p<<"].data["<<iter<<"]: "<<tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + v_p].data[iter]<<endl; }
			// if(iter==0){ cout<<"~~~ tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + "<<v_p<<"].data["<<iter<<"]: "<<tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + v_p].data[iter]<<endl; }
		}
	}
	#endif 

	// load upropblock stats
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<">>> extract_stats: populating metadata (upropblock_stats) ... "<<endl;		
	#endif 
	unsigned int vdram_BASEOFFSETKVS_ACTIVEUPROPBLOCKS = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEUPROPBLOCKS].data[0].key;
	unsigned int kvdram_BASEOFFSETKVS_ACTIVEUPROPBLOCKS = kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEUPROPBLOCKS].data[0].key;
	for(unsigned int v_p=0; v_p<myuniversalparams.NUMPROCESSEDGESPARTITIONS; v_p++){ 
		for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){
			unsigned int index = 0;
			unsigned int offset = ((v_p * MAXNUMGRAPHITERATIONS * MAXNUM_UPROPBLOCKS_PER_VPARTITION) + (iter * MAXNUM_UPROPBLOCKS_PER_VPARTITION)) / VECTOR2_SIZE;
			tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + v_p].data[iter] = 0; 
			for(unsigned int t=0; t<MAXNUM_UPROPBLOCKS_PER_VPARTITION; t++){
				if(upropblock_stats[iter][v_p][t] == 1){ // > 0
					if(index < MAXNUM_UPROPBLOCKS_PER_VPARTITION){
						tempvdram[vdram_BASEOFFSETKVS_ACTIVEUPROPBLOCKS + offset + (index / VECTOR2_SIZE)].data[index % VECTOR2_SIZE] = t; 
						for(unsigned int i=0; i<NUM_PEs; i++){ tempkvdram[i][kvdram_BASEOFFSETKVS_ACTIVEUPROPBLOCKS + offset + (index / VECTOR2_SIZE)].data[index % VECTOR2_SIZE] = t; }
						// if(iter==9 && v_p==4){ cout<<"acts_helper.cpp --------------------> t: "<<t<<endl; }
						tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + v_p].data[iter] += 1;
					}
					index += 1;
				}
			}
			// if(iter==9){ cout<<"tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + "<<v_p<<"].data["<<iter<<"]: "<<tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + v_p].data[iter]<<endl; }
		}		
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS//4 // debug - print uprop block stats
	for(unsigned int iter=0; iter<GraphIter+1; iter++){
		cout<<"acts_helper: printing upropblock_stats for iteration "<<iter<<endl;
		for(unsigned int v_p=0; v_p<myuniversalparams.NUMPROCESSEDGESPARTITIONS; v_p++){ 
			cout<<"acts_helper: printing upropblock_stats for iteration "<<iter<<", v_partition: "<<v_p<<endl;
			for(unsigned int t=0; t<MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){ 
				if(upropblock_stats[iter][v_p][t] > 0){ cout<<t<<", "; }
				utilityobj->checkoutofbounds("acts_helper:: ERROR 25", upropblock_stats[iter][v_p][t], 2, NAp, NAp, NAp);
			}
			cout<<endl;
		} 
	}
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS4
	for(unsigned int iter=0; iter<GraphIter+1; iter++){
		unsigned int tot = 0, tot2 = 0, tot3 = 0;
		for(unsigned int t=0; t<myuniversalparams.NUMPROCESSEDGESPARTITIONS; t++){ tot2 += iteration_stats[iter][t].A; tot += vpartition_stats[iter][t].A; tot3 += tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + t].data[iter]; }
		cout<<"acts_helper: number of active vertices for iteration "<<iter<<": "<<tot2<<", (UB:"<<tot<<"), (EB:"<<tot3<<") (E:"<<(unsigned int)edgesprocessed_totals[iter]<<")"<<endl;
	}
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<">>> acts_helper: FINISHED. "<<GraphIter+1<<" iterations required."<<endl;
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<">>> acts_helper: printing pmasks for process-partition-reduce... "<<endl;
	for(unsigned int iter=0; iter<GraphIter+1; iter++){
		unsigned int num_actvps = 0;
		unsigned int total_num_actvvs = 0;	
		for(unsigned int t=0; t<myuniversalparams.NUMPROCESSEDGESPARTITIONS; t++){
			if(vpartition_stats[iter][t].A > 0  && t < 16){ cout<<t<<", "; }
			if(vpartition_stats[iter][t].A > 0){ num_actvps += 1; }
			total_num_actvvs += vpartition_stats[iter][t].A;
		}
		cout<<" (num active partitions: "<<num_actvps<<", total num partitions: "<<myuniversalparams.NUMPROCESSEDGESPARTITIONS<<" iter: "<<iter<<")"<<endl;	
		cout<<"+++ total number of active vertices in all HBM channels (varA): "<<total_num_actvvs<<" ("<<num_actvps<<" active partitions +++"<<endl;
	}
	
	for(unsigned int iter=0; iter<GraphIter+1; iter++){
		unsigned int num_actv_edges = 0;
		for(unsigned int t=0; t<myuniversalparams.NUMPROCESSEDGESPARTITIONS; t++){
			// cout<<"^^****************^ iter "<<iter<<": t: "<<t<<" ^^^"<<endl;
			num_actv_edges += vpartition_stats[iter][t].B;
		}
		cout<<"^^^ iter "<<iter<<": total number of active edges in all HBM channel (varB): "<<num_actv_edges<<" ^^^"<<endl;
	}
	
	for(unsigned int iter=0; iter<GraphIter+1; iter++){
		unsigned int tot = 0, tot2 = 0;
		for(unsigned int t=0; t<myuniversalparams.NUMPROCESSEDGESPARTITIONS; t++){ tot += vpartition_stats[iter][t].A; tot2 += iteration_stats[iter][t].A; }
		cout<<"--- acts_helper: number of active vertices for iteration "<<iter<<": "<<tot2<<", "<<tot<<endl;
	}
	// exit(EXIT_SUCCESS);
	#endif 
	// exit(EXIT_SUCCESS);
	return GraphIter+1;
}

float acts_helper::getfeedbackstats(string message, string graphpath, uint512_vec_dt * vdram, uint512_vec_dt * vdramtemp0, uint512_vec_dt * vdramtemp1, uint512_vec_dt * vdramtemp2, uint512_vec_dt * kvbuffer[NUM_PEs], universalparams_t universalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"acts_helper::getfeedbackstats: getting feedback... "<<endl;
	#endif
	
	unsigned int F0 = 0;
	unsigned int F1 = 1;
	unsigned int F2 = 2;
	float total__latency_ms = 0;
	
	unsigned int num_iters_toprint = universalparams.NUM_ITERATIONS; // MAXNUMGRAPHITERATIONS;
	if(universalparams.ALGORITHM != BFS && universalparams.ALGORITHM != SSSP){ num_iters_toprint = 1; }
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	for(unsigned int i=0; i<1; i++){
		for(unsigned int GraphIter=0; GraphIter<num_iters_toprint; GraphIter++){ 
			unsigned int PROCESSINGPHASE_TRANSFSZ_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].key;	
			unsigned int PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].key;
			unsigned int REDUCEPHASE_TRANSFSZ_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[REDUCEPHASE_TRANSFSZ_COLLECTIONID].key;
			unsigned int SYNCPHASE_TRANSFSZ_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[SYNCPHASE_TRANSFSZ_COLLECTIONID].key;

			cout<<"[PE:"<<i<<"][Iter: "<<GraphIter<<"]:: processing phase transfsz: "<<PROCESSINGPHASE_TRANSFSZ_COLLECTIONID__<<", partitioning phase transfsz: "<<PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID__<<", reduce phase transfsz: "<<REDUCEPHASE_TRANSFSZ_COLLECTIONID__<<", sync phase transfsz: "<<SYNCPHASE_TRANSFSZ_COLLECTIONID__<<endl;				
		}
	}
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	for(unsigned int i=0; i<1; i++){
		for(unsigned int GraphIter=0; GraphIter<num_iters_toprint; GraphIter++){ 
			unsigned int NUMEDGESPROCESSED_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[NUMEDGESPROCESSED_COLLECTIONID].key;	
			unsigned int NUMVERTICESPROCESSED_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[NUMVERTICESPROCESSED_COLLECTIONID].key;
			
			cout<<"[PE:"<<i<<"][Iter: "<<GraphIter<<"]:: num edges processed: "<<NUMEDGESPROCESSED_COLLECTIONID__<<", num vertices processed: "<<NUMVERTICESPROCESSED_COLLECTIONID__<<"."<<" "<<endl;				
		}
	}
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	for(unsigned int i=0; i<1; i++){
		for(unsigned int GraphIter=0; GraphIter<num_iters_toprint; GraphIter++){ 
			unsigned int NUMREADSFROMDRAM_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[NUMREADSFROMDRAM_COLLECTIONID].key;	
			unsigned int NUMWRITESTODRAM_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[NUMWRITESTODRAM_COLLECTIONID].key;
			
			cout<<"[PE:"<<i<<"][Iter: "<<GraphIter<<"]:: num reads from dram: "<<NUMREADSFROMDRAM_COLLECTIONID__<<", num writes to dram: "<<NUMWRITESTODRAM_COLLECTIONID__<<"."<<" "<<endl;				
		}
	}
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<"---- AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC: "<<AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC<<endl;
	for(unsigned int i=0; i<1; i++){
		for(unsigned int GraphIter=0; GraphIter<num_iters_toprint; GraphIter++){ 
			unsigned int PROCESSINGPHASE_TRANSFSZ_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[PROCESSINGPHASE_TRANSFSZ_COLLECTIONID].key;	
			unsigned int PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID].key;
			unsigned int REDUCEPHASE_TRANSFSZ_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[REDUCEPHASE_TRANSFSZ_COLLECTIONID].key;
			unsigned int SYNCPHASE_TRANSFSZ_COLLECTIONID__ = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_STATSCOLLECTED + GraphIter].data[SYNCPHASE_TRANSFSZ_COLLECTIONID].key;

			unsigned int PROCESSINGPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS = (PROCESSINGPHASE_TRANSFSZ_COLLECTIONID__ / 1000) / AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC;
			unsigned int PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS = (PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID__ / 1000) / AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC;
			unsigned int REDUCEPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS = (REDUCEPHASE_TRANSFSZ_COLLECTIONID__ / 1000) / AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC;
			unsigned int SYNCPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS = (SYNCPHASE_TRANSFSZ_COLLECTIONID__ / 1000) / AVERAGE_MEMACCESSTHROUGHPUT_SINGLEHBMCHANNEL_MILIONEDGESPERSEC;
			unsigned int total__iteration_latency_ms = PROCESSINGPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS + PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS + REDUCEPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS + SYNCPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS;
			total__latency_ms += total__iteration_latency_ms;
			
			cout<<"[PE:"<<i<<"][Iter: "<<GraphIter<<"][latencies]:: total latency (ms): "<<total__iteration_latency_ms<<"ms [processing phase: "<<PROCESSINGPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS<<"ms, partitioning phase: "<<PARTITIONINGPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS<<"ms, reduce phase: "<<REDUCEPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS<<"ms, sync phase: "<<SYNCPHASE_TRANSFSZ_COLLECTIONID__LATENCY_MS<<"ms]"<<endl;				
		}
	}
	#endif
	return total__latency_ms;
}

void acts_helper::verifyresults(uint512_vec_dt * vbuffer, globalparams_t globalparams, universalparams_t universalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"acts_helper::verifyresults: verifying results... "<<endl;
	#endif
	
	if(universalparams.ALGORITHM == BFS || universalparams.ALGORITHM == SSSP){} else { return; }
	
	unsigned int vdatas[64]; for(unsigned int k=0; k<64; k++){ vdatas[k] = 0; } 
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<universalparams.NUMREDUCEPARTITIONS; partition++){
			
			unsigned int reducepsz_kvs = universalparams.REDUCEPARTITIONSZ_KVS2; 
			if(universalparams.NUMREDUCEPARTITIONS > 1){ if(partition == universalparams.NUMREDUCEPARTITIONS-1){ reducepsz_kvs = (universalparams.BATCH_RANGE / VECTOR2_SIZE) % universalparams.REDUCEPARTITIONSZ_KVS2; } else { reducepsz_kvs = universalparams.REDUCEPARTITIONSZ_KVS2; }}
			
			for(unsigned int k=0; k<reducepsz_kvs; k++){
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					unsigned int combo1 = vbuffer[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + (i * universalparams.NUMREDUCEPARTITIONS * universalparams.REDUCEPARTITIONSZ_KVS2) + (partition * universalparams.REDUCEPARTITIONSZ_KVS2) + k].data[v].key;
					unsigned int combo2 = vbuffer[globalparams.BASEOFFSETKVS_SRCVERTICESDATA + (i * universalparams.NUMREDUCEPARTITIONS * universalparams.REDUCEPARTITIONSZ_KVS2) + (partition * universalparams.REDUCEPARTITIONSZ_KVS2) + k].data[v].value;
					
					value_t vdata1 = combo1 >> 1; value_t mask1 = combo1 & 0x1; 
					value_t vdata2 = combo2 >> 1; value_t mask2 = combo2 & 0x1; 
	
					unsigned int lvid1 = ((partition * universalparams.REDUCEPARTITIONSZ_KVS2 * VECTOR_SIZE) + (k * VECTOR_SIZE)) + 2*v;
					unsigned int lvid2 = lvid1 + 1;
					unsigned int vid1 = utilityobj->UTIL_GETREALVID(lvid1, i);
					unsigned int vid2 = utilityobj->UTIL_GETREALVID(lvid2, i);
					
					if(vdata1 < 64){
						#ifdef _DEBUGMODE_HOSTPRINTS
						cout<<"acts_helper::verifyresults: vid1: "<<vid1<<", vdata1: "<<vdata1<<", i: "<<i<<", partition: "<<partition<<", k: "<<k<<", v: "<<v<<endl;
						#endif
						vdatas[vdata1] += 1; 
					}
					if(vdata2 < 64){
						#ifdef _DEBUGMODE_HOSTPRINTS
						cout<<"acts_helper::verifyresults: vid2: "<<vid2<<", vdata2: "<<vdata2<<", i: "<<i<<", partition: "<<partition<<", k: "<<k<<", v: "<<v<<endl;
						#endif
						vdatas[vdata2] += 1; 
					}
				}
			}
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS4
	utilityobj->printvalues("acts_helper::verifyresults: results after kernel run", vdatas, 16);
	#endif 
	return;
}














