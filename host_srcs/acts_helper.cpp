#include "acts_helper.h"
using namespace std;

acts_helper::acts_helper(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	universalparams = _universalparams;
}
acts_helper::~acts_helper(){} 

unsigned int acts_helper::gethash(unsigned int vid){
	return vid % NUM_PEs;
}
unsigned int acts_helper::getlocalvid(unsigned int vid){
	unsigned int s = gethash(vid);
	return (vid - s) / NUM_PEs; 
}

unsigned int acts_helper::extract_stats(vector<vertex_t> &srcvids, vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, long double edgesprocessed_totals[128], tuple_t * vpmaskstats[MAXNUMGRAPHITERATIONS], tuple_t * vpmaskstats_merge[MAXNUMGRAPHITERATIONS][NUM_PEs], unsigned int num_edges_processed[MAXNUMGRAPHITERATIONS]){						
	if(universalparams.ALGORITHM == BFS || universalparams.ALGORITHM == SSSP){} else {
		for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){ vpmaskstats[iter][t].A = 10; vpmaskstats[iter][t].B = 11; }}
		edgesprocessed_totals[0] = edgedatabuffer.size(); 
		num_edges_processed[0] = edgedatabuffer.size(); 
		return 1; }
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<endl<<"acts_helper::extract_stats:: running traditional sssp... "<<endl;
	#endif 
	
	vector<value_t> actvvs;
	vector<value_t> actvvs_nextit;
	unsigned int * vdatas = new unsigned int[universalparams.KVDATA_RANGE];
	
	for(unsigned int i=0; i<universalparams.KVDATA_RANGE; i++){ vdatas[i] = 0xFFFFFFFF; }
	unsigned int total_edges_processed = 0;
	for(unsigned int i=0; i<128; i++){ edgesprocessed_totals[i] = 0; }
	for(unsigned int i=0; i<srcvids.size(); i++){ actvvs.push_back(srcvids[i]); }
	cout<<"acts_helper::extract_stats: number of active vertices for iteration 0: "<<actvvs_nextit.size()<<endl;
	for(unsigned int i=0; i<actvvs.size(); i++){ vdatas[actvvs[i]] = 0; }
	unsigned int GraphIter=0;
	vpmaskstats[0][0].A = 1; 
	
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ num_edges_processed[iter] = 0; }
	for(GraphIter=0; GraphIter<MAXNUMGRAPHITERATIONS; GraphIter++){ // 64
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
			
			vpmaskstats[GraphIter][vid / universalparams.PROCESSPARTITIONSZ].B += edges_size; // 
			// cout<<"~~~ vpmaskstats["<<GraphIter<<"]["<<vid / universalparams.PROCESSPARTITIONSZ<<"].B: "<<vpmaskstats[GraphIter][vid / universalparams.PROCESSPARTITIONSZ].B<<endl;
			
			for(unsigned int k=0; k<edges_size; k++){
				unsigned int dstvid = edgedatabuffer[vptr_begin + k].dstvid;
				
				unsigned int res = vdatas[vid] + 1;
				value_t vprop = vdatas[dstvid];
				value_t vtemp = min(vprop, res);
				vdatas[dstvid] = vtemp;
				if(vtemp != vprop){ 
					actvvs_nextit.push_back(dstvid);
					utilityobj->checkoutofbounds("acts_helper::extract_stats:: ERROR 20", dstvid / universalparams.PROCESSPARTITIONSZ, universalparams.NUMPROCESSEDGESPARTITIONS, dstvid, vid, dstvid);
					
					if(GraphIter+1 < MAXNUMGRAPHITERATIONS){
						vpmaskstats[GraphIter+1][dstvid / universalparams.PROCESSPARTITIONSZ].A += 1;
						vpmaskstats_merge[GraphIter][gethash(dstvid)][getlocalvid(dstvid) / universalparams.REDUCEPARTITIONSZ].A += 1;
					}			
				} 
			
				total_edges_processed += 1;
				edgesprocessed_totals[GraphIter] += 1;
			}
		}
		
		cout<<"acts_helper::extract_stats: number of active vertices for iteration "<<GraphIter + 1<<": "<<actvvs_nextit.size()<<""<<endl;
		if(actvvs_nextit.size() == 0){ cout<<"no more activer vertices to process. breaking out... "<<endl; break; }
	
		actvvs.clear();
		for(unsigned int i=0; i<actvvs_nextit.size(); i++){ actvvs.push_back(actvvs_nextit[i]); }
		actvvs_nextit.clear();
	}
	
	cout<<">>> acts_helper::extract_stats: FINISHED. "<<GraphIter+1<<" iterations required."<<endl;
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){ cout<<">>> utility:: edgesprocessed_totals["<<iter<<"] "<<(unsigned int)edgesprocessed_totals[iter]<<endl; }
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<">>> acts_helper::extract_stats: printing pmasks for process-partition-reduce... "<<endl;	
	for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){
		unsigned int num_actvps = 0;
		for(unsigned int t=0; t<universalparams.NUMPROCESSEDGESPARTITIONS; t++){
			if(vpmaskstats[iter][t].A > 0  && t < 16){ cout<<t<<", "; }
			if(vpmaskstats[iter][t].A > 0){ num_actvps += 1; }
		}
		cout<<" (num active partitions: "<<num_actvps<<", total num partitions: "<<universalparams.NUMPROCESSEDGESPARTITIONS<<" iter: "<<iter<<")"<<endl;	
	}
	cout<<">>> acts_helper::extract_stats: printing pmasks for merge... "<<endl;	
	for(unsigned int i=0; i<1; i++){ // NUM_PEs
		for(unsigned int iter=0; iter<MAXNUMGRAPHITERATIONS; iter++){
			unsigned int num_actvps = 0;
			for(unsigned int t=0; t<universalparams.NUMREDUCEPARTITIONS; t++){
				if(vpmaskstats_merge[iter][i][t].A > 0  && t < 16){ cout<<t<<", "; }
				if(vpmaskstats_merge[iter][i][t].A > 0){ num_actvps += 1; }
			}
			cout<<" (num active partitions (for merge): "<<num_actvps<<", total num partitions: "<<universalparams.NUMREDUCEPARTITIONS<<" iter: "<<iter<<", PE: "<<i<<")"<<endl;
		}
	}
	#endif 
	// exit(EXIT_SUCCESS); 
	// delete vdatas;
	return GraphIter+1;
}

unsigned int acts_helper::getfeedback(string message, string graphpath, uint512_vec_dt * vdram, uint512_vec_dt * vdramtemp0, uint512_vec_dt * vdramtemp1, uint512_vec_dt * vdramtemp2, uint512_vec_dt * kvbuffer[NUM_PEs]){
	unsigned int F0 = 0;
	unsigned int F1 = 1;
	unsigned int F2 = 2;
	unsigned int num_traversed_edges = 0;
	
	for(unsigned int i=0; i<NUM_PEs; i++){ // NUM_PEs
		if(i%NUMCOMPUTEUNITS_SLR2==0 || i%(NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1)==0 || i%(NUMCOMPUTEUNITS_SLR2 + NUMCOMPUTEUNITS_SLR1 + NUMCOMPUTEUNITS_SLR0)==0){} else { continue; } 
		for(unsigned int GraphIter=0; GraphIter<10; GraphIter++){ 
			unsigned int num_edgesprocessed = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key;	
			unsigned int num_vertexupdatesreduced = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key;	
			
			unsigned int num_validedgesprocessed = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].value;	
			unsigned int num_validvertexupdatesreduced = kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].value;	
			
			cout<<">>> acts_helper::[A]["<<message<<"][PE:"<<i<<"][Iter: "<<GraphIter<<"]:: num edges processed: "<<num_edgesprocessed<<"("<<num_validedgesprocessed<<"), num vertex updates reduced: "<<num_vertexupdatesreduced<<"("<<num_validvertexupdatesreduced<<")"<<endl;	
		}
	}

	for(unsigned int GraphIter=0; GraphIter<10; GraphIter++){ 
		unsigned int num_edgesprocessed = 0;
		unsigned int num_vertexupdatesreduced = 0;
		unsigned int num_validedgesprocessed = 0;
		unsigned int num_validvertexupdatesreduced = 0;
		for(unsigned int i=0; i<NUM_PEs; i++){
			num_edgesprocessed += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].key;	
			num_vertexupdatesreduced += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].key;	
			
			num_validedgesprocessed += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMEDGESPROCESSED + GraphIter].data[0].value;	
			num_validvertexupdatesreduced += kvbuffer[i][BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURNVALUES + MESSAGES_RETURNVALUES_CHKPT1_NUMVERTEXUPDATESREDUCED + GraphIter].data[0].value;
			
			// num_traversed_edges += num_edgesprocessed;
		}
		num_traversed_edges += num_edgesprocessed;
		cout<<">>> acts_helper::[A]["<<message<<"][PE:ALL][Iter: "<<GraphIter<<"]:: num edges processed: "<<num_edgesprocessed<<"("<<num_validedgesprocessed<<"), num vertex updates reduced: "<<num_vertexupdatesreduced<<"("<<num_validvertexupdatesreduced<<")"<<endl;	
	}
	// num_traversed_edges = num_edgesprocessed;
	cout<<">>> acts_helper:: -------(((((((((((((((--------- num_traversed_edges: "<<num_traversed_edges<<", num_edgesprocessed: "<<endl;
	return num_traversed_edges;
}












