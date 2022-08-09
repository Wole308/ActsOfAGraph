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

void acts_helper::load_edgeblock_stats(unsigned int vid, unsigned int v_p, int GraphIter, vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer,
		uint512_ivec_dt * tempvdram, uint512_ivec_dt * tempkvdram[NUM_PEs], unsigned int vdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS, unsigned int kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS,
			unsigned int * edgeblock_mask[MAXNUM_PEs], unsigned int * indexes[MAXNUM_PEs]){
	unsigned int vsize_vP = myuniversalparams.PROCESSPARTITIONSZ;
	unsigned int vsize_LLP = myuniversalparams.REDUCEPARTITIONSZ_KVS2;
	unsigned int vsize_LLPset = vsize_LLP * VDATA_PACKINGSIZE;
	unsigned int max___ = ((myuniversalparams.NUM_EDGES / EDGEDATA_PACKINGSIZE) / NUM_PEs) / NUM_EDGESKVS_PER_UPROPBLOCK + 8192; // ~20K
	
	edge_t vptr_begin = vertexptrbuffer[vid];
	edge_t vptr_end = vertexptrbuffer[vid+1];
	edge_t edges_size = vptr_end - vptr_begin;
	if(vptr_end < vptr_begin){ return; } // FIXME. 
	// unsigned int v_p_ = vid / myuniversalparams.PROCESSPARTITIONSZ;
	#ifdef _DEBUGMODE_CHECKS3		
	if(vptr_end < vptr_begin){ cout<<"extract_stats: ERROR 29: vptr_end("<<vptr_end<<") < vptr_begin("<<vptr_begin<<"). exiting..."<<endl; exit(EXIT_FAILURE); }
	#endif
	for(unsigned int k=0; k<edges_size; k++){
		edge2_type edge = edgedatabuffer[vptr_begin + k];
		unsigned int H = (edge.dstvid % (EDGEDATA_PACKINGSIZE * NUM_PEs)) / EDGEDATA_PACKINGSIZE;
		unsigned int v_p_unused = (edge.srcvid / myuniversalparams.PROCESSPARTITIONSZ);
		unsigned int ldstvid = utilityobj->UTIL_GETLOCALVID(edge.dstvid, H);
		unsigned int llp_set = ldstvid / vsize_LLPset; 
		utilityobj->checkoutofbounds("extract_stats::ERROR 561::", edge.eblockid, max___, vid, k, NAp);	
		if(edgeblock_mask[H][edge.eblockid] == 0){
			unsigned int offset = ((v_p * MAXNUMGRAPHITERATIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION) + ((GraphIter + 1) * MAXNUM_EDGEBLOCKS_PER_VPARTITION));
			utilityobj->checkoutofbounds("extract_stats::ERROR 562::", H, MAXNUM_PEs, NAp, offset, indexes[H][llp_set]);	
			utilityobj->checkoutofbounds("extract_stats::ERROR 563::", llp_set, myuniversalparams.NUMREDUCEPARTITIONS, NAp, offset, indexes[H][llp_set]);	
			utilityobj->checkoutofbounds("extract_stats::ERROR 564::", vdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset + 1 + indexes[H][llp_set], ((1 << 28) / 4) / 16, NAp, offset, indexes[H][llp_set]);	
			if(indexes[H][llp_set] < MAXNUM_EDGEBLOCKS_PER_VPARTITION - 1){ tempkvdram[H][kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset + 1 + indexes[H][llp_set]].data[llp_set] = edge.eblockid; }
			edgeblock_mask[H][edge.eblockid] = 1;
			indexes[H][llp_set] += 1;
		}
	}
	return;
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
			long double edgesprocessed_totals[128], tuple_t * vpartition_stats[MAXNUMGRAPHITERATIONS], unsigned int * upropblock_stats[MAXNUMGRAPHITERATIONS][MAXNUM_VPs], unsigned int * edgeblock_stats[MAXNUM_PEs], unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS]){						
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
	unsigned int vdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEEDGEBLOCKS].data[0].key;
	unsigned int kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS = kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEEDGEBLOCKS].data[0].key;
	
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ 
		for(unsigned int v_p=0; v_p<MAXNUM_VPs; v_p++){ 
			upropblock_stats[iter][v_p] = new unsigned int[MAXNUM_EDGEBLOCKS_PER_VPARTITION]; 
			for(unsigned int t=0; t<MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){ upropblock_stats[iter][v_p][t] = 0; }
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
	
	// reset stats 
	for(GraphIter=0; GraphIter<MAXNUMGRAPHITERATIONS; GraphIter++){ // MAXNUMGRAPHITERATIONS
		for(unsigned int H=0; H<NUM_PEs; H++){ // NUM_PEs
			for(unsigned int v_p=0; v_p<myuniversalparams.NUMPROCESSEDGESPARTITIONS; v_p++){ // myuniversalparams.NUMPROCESSEDGESPARTITIONS
				for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){ // num_LLPset
					for(unsigned int t=0; t<MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){ // MAXNUM_EDGEBLOCKS_PER_VPARTITION
						unsigned int offset = ((v_p * MAXNUMGRAPHITERATIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION) + ((GraphIter + 0) * MAXNUM_EDGEBLOCKS_PER_VPARTITION));
						tempkvdram[H][kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset + t].data[llp_set] = 0; 	
					}
				}
			}
		}
	}

	// setting root vid...
	unsigned int rootvid = actvvs[0];
	unsigned int v_p_ = rootvid / myuniversalparams.PROCESSPARTITIONSZ;
	load_edgeblock_stats(rootvid, v_p_, -1, vertexptrbuffer, edgedatabuffer,
					tempvdram, tempkvdram, vdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS, kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS,
						edgeblock_mask, indexes);	
	// set headers for edgeblock stats 
	set_edgeblock_headers(-1, 0, 
					tempvdram, tempkvdram, vdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS, kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS,
						indexes);
	
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
				load_edgeblock_stats(vid, v_p_, GraphIter, vertexptrbuffer, edgedatabuffer,
					tempvdram, tempkvdram, vdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS, kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS,
						edgeblock_mask, indexes);	
			}
			
			// set headers for edgeblock stats 
			if(vid % myuniversalparams.PROCESSPARTITIONSZ == myuniversalparams.PROCESSPARTITIONSZ - 1){ 
				set_edgeblock_headers(GraphIter, v_p, 
					tempvdram, tempkvdram, vdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS, kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS,
						indexes);
			}
			
			if(vid % myuniversalparams.PROCESSPARTITIONSZ == myuniversalparams.PROCESSPARTITIONSZ - 1){ v_p += 1; }
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
	unsigned int vdram_BASEOFFSETKVS_ACTIVEUPROPBLOCKS = vdram[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEUPROPBLOCKS].data[0].key;
	unsigned int kvdram_BASEOFFSETKVS_ACTIVEUPROPBLOCKS = kvbuffer[0][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEUPROPBLOCKS].data[0].key;
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){
		// for(unsigned int t=0; t<myuniversalparams.NUMPROCESSEDGESPARTITIONS; t++){ tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + t].data[iter] = vpartition_stats[iter][t].A; }		
		for(unsigned int v_p=0; v_p<myuniversalparams.NUMPROCESSEDGESPARTITIONS; v_p++){
			unsigned int max = 0;
			for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){
				for(unsigned int H=0; H<NUM_PEs; H++){
					unsigned int offset = ((v_p * MAXNUMGRAPHITERATIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION) + (iter * MAXNUM_EDGEBLOCKS_PER_VPARTITION));
					unsigned int data = tempkvdram[H][kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset + 0].data[llp_set]; 
					if(max < data){ max = data; }
				}
			}
			// cout<<"extract_stats: -------------- iter: "<<iter<<", v_p: "<<v_p<<", max: "<<max<<" ---------------------"<<endl;
			tempvdram[vdram_BASEOFFSETKVS_VERTICESPARTITIONMASK + v_p].data[iter] = max;
		}
	}
	
	// load upropblock stats
	#ifdef _DEBUGMODE_HOSTPRINTS4
	cout<<">>> extract_stats: populating metadata (upropblock_stats) ... "<<endl;		
	#endif 
	for(unsigned int v_p=0; v_p<myuniversalparams.NUMPROCESSEDGESPARTITIONS; v_p++){ 
		for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){
			unsigned int index = 1;
			unsigned int offset = ((v_p * MAXNUMGRAPHITERATIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION) + (iter * MAXNUM_EDGEBLOCKS_PER_VPARTITION)) / VECTOR2_SIZE;
			for(unsigned int t=0; t<MAXNUM_EDGEBLOCKS_PER_VPARTITION; t++){
				if(upropblock_stats[iter][v_p][t] == 1){ // > 0
					tempvdram[vdram_BASEOFFSETKVS_ACTIVEUPROPBLOCKS + offset + (index / VECTOR2_SIZE)].data[index % VECTOR2_SIZE] = t; 
					for(unsigned int i=0; i<NUM_PEs; i++){ tempkvdram[i][kvdram_BASEOFFSETKVS_ACTIVEUPROPBLOCKS + offset + (index / VECTOR2_SIZE)].data[index % VECTOR2_SIZE] = t; }
					index += 1;
				}
			}
		}		
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS3 // debug - print edge block stats // 4*
	for(GraphIter=0; GraphIter<8; GraphIter++){ // MAXNUMGRAPHITERATIONS
	cout<<"+++ acts_helper: printing edgeblock stats for iteration "<<GraphIter<<endl;	
		for(unsigned int H=0; H<1; H++){ // NUM_PEs
			for(unsigned int v_p=0; v_p<1; v_p++){ // myuniversalparams.NUMPROCESSEDGESPARTITIONS
				for(unsigned int llp_set=0; llp_set<num_LLPset; llp_set++){ // num_LLPset
					for(unsigned int t=0; t<8; t++){ // MAXNUM_EDGEBLOCKS_PER_VPARTITION
						unsigned int offset = ((v_p * MAXNUMGRAPHITERATIONS * MAXNUM_EDGEBLOCKS_PER_VPARTITION) + ((GraphIter) * MAXNUM_EDGEBLOCKS_PER_VPARTITION));
						unsigned int data = tempkvdram[H][kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS + offset + t].data[llp_set]; 	
						cout<<"extract_stats:: tempkvdram["<<H<<"]["<<v_p<<"]["<<kvdram_BASEOFFSETKVS_ACTIVEEDGEBLOCKS<<" + "<<offset + t<<"].data["<<llp_set<<"]: "<<data<<endl;
					}
				}
			}
		}
	}
	#endif 
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

unsigned int acts_helper::getfeedback(string message, string graphpath, uint512_vec_dt * vdram, uint512_vec_dt * vdramtemp0, uint512_vec_dt * vdramtemp1, uint512_vec_dt * vdramtemp2, uint512_vec_dt * kvbuffer[NUM_PEs], universalparams_t universalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"acts_helper::getfeedback: getting feedback... "<<endl;
	#endif
	
	unsigned int F0 = 0;
	unsigned int F1 = 1;
	unsigned int F2 = 2;
	unsigned int num_traversed_edges = 0;
	
	unsigned int num_iters_toprint = universalparams.NUM_ITERATIONS; // MAXNUMGRAPHITERATIONS;
	if(universalparams.ALGORITHM != BFS && universalparams.ALGORITHM != SSSP){ num_iters_toprint = 1; }
	
	for(unsigned int i=0; i<1; i++){ // NUM_PEs
		for(unsigned int GraphIter=0; GraphIter<num_iters_toprint; GraphIter++){ 
			unsigned int num_edgestraversed = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + GraphIter].data[0].key;	
			unsigned int num_verticestraversed = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + GraphIter].data[1].key;
			
			#ifdef _DEBUGMODE_HOSTPRINTS4
			cout<<"[PE:"<<i<<"][Iter: "<<GraphIter<<"]:: edges trav: "<<num_edgestraversed<<", vertices trav: "<<num_verticestraversed<<""<<endl;	
			#endif 
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<NUM_PEs; i++){ // NUM_PEs
		if(i == 0 && i%NUMCOMPUTEUNITS_SLR2==0){} else { continue; } 
		for(unsigned int GraphIter=0; GraphIter<num_iters_toprint; GraphIter++){ 
			unsigned int num_edgestraversed = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + GraphIter].data[0].key;	
			unsigned int num_edgesprocessed = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + GraphIter].data[0].value;	
			unsigned int num_updatesreduced = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + GraphIter].data[1].key;	
			
			#ifdef _DEBUGMODE_HOSTPRINTS4
			cout<<"[PE:"<<i<<"][Iter: "<<GraphIter<<"]:: num edges traversed: "<<num_edgestraversed<<", num edges processed: "<<num_edgesprocessed<<", num vertex updates reduced: "<<num_updatesreduced<<""<<endl;	
			#endif 
		}
	}
	#endif 
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int GraphIter=0; GraphIter<num_iters_toprint; GraphIter++){ 
		unsigned int num_edgestraversed = 0;
		unsigned int num_edgesprocessed = 0;
		unsigned int num_updatesreduced = 0;
		for(unsigned int i=0; i<NUM_PEs; i++){
			num_edgestraversed += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + GraphIter].data[0].key;	
			num_edgesprocessed += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + GraphIter].data[0].value;	
			num_updatesreduced += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESTRAVERSED + GraphIter].data[1].key;	
		}
		num_traversed_edges += num_edgesprocessed;
		#ifdef _DEBUGMODE_HOSTPRINTS4
		cout<<"[PE:ALL][Iter: "<<GraphIter<<"]:: num edges traversed: "<<num_edgestraversed<<", num edges processed: "<<num_edgesprocessed<<", num vertex updates reduced: "<<num_updatesreduced<<""<<endl;	
		#endif 
	}
	#endif 
	
	// num_traversed_edges = num_edgesprocessed;
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<">>> acts_helper:: num_traversed_edges: "<<num_traversed_edges<<", num_edgesprocessed: "<<endl;
	#endif 
	return num_traversed_edges;
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














