#include "loadedges_random.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

#define LOADSLICEDEDGES
// #define CHECK4_VERIFYOFFSETS
#define CALCULATELOCALDSTVIDS
#define INSERTBITMAP
#define LOADEDGES

#define TWOO 2

bool debugb = false;//false;
bool debug2b = false;
bool debug3b = false;

loadedges_random::loadedges_random(graph * _graphobj, stats * _statsobj){
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	statsobj = _statsobj;
	evalparamsobj = new evalparams();
}
loadedges_random::loadedges_random(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
loadedges_random::~loadedges_random(){} 

unsigned int loadedges_random::GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int loadedges_random::READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & GETMASK_UINT((index), (size))) >> (index)); 
}
void loadedges_random::WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
	/** WRITETO_UINT(&data, 28, 4, 1) => 010000000000000000000000000000
	WRITETO_UINT(&data, 28, 4, 2) => 100000000000000000000000000000
	WRITETO_UINT(&data, 28, 4, 3) => 110000000000000000000000000000 */
	
	unsigned int tempdata = *data;
	unsigned int A = ((value) << (index));
	unsigned int B = (~GETMASK_UINT((index), (size)));
	unsigned int C = ((tempdata) & (B));
	unsigned int D = (C) | A;
	*data = D;
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	cout<<"WRITETO_ULONG. index: "<<index<<", size: "<<size<<", value: "<<value<<endl;
	cout<<"WRITETO_ULONG. tempdata"<<endl; actsutilityobj->ULONGTOBINARY(tempdata);
	cout<<"WRITETO_ULONG. A"<<endl; actsutilityobj->ULONGTOBINARY(A);
	cout<<"WRITETO_ULONG. B (~mask)"<<endl; actsutilityobj->ULONGTOBINARY(B);
	cout<<"WRITETO_ULONG. C"<<endl; actsutilityobj->ULONGTOBINARY(C);
	cout<<"WRITETO_ULONG. D (result)"<<endl; actsutilityobj->ULONGTOBINARY(D);
	#endif
	return; 
}

unsigned int loadedges_random::gethash(unsigned int vid){
	return vid % NUM_PEs;
}
unsigned int loadedges_random::allignlower(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = val / V_SIZE;
	return (fac * V_SIZE);
}
unsigned int loadedges_random::allignhigher(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = (val + (V_SIZE-1)) / V_SIZE;
	return (fac * V_SIZE);
}

unsigned int loadedges_random::getpartition(unsigned int data, vertex_t upperlimit){
	// unsigned int partition = (data - upperlimit) / ((WORKBUFFER_SIZE*VECTOR2_SIZE) / NUM_PARTITIONS);
	unsigned int partition = (data - upperlimit) % VECTOR2_SIZE;
	// cout<<"loadedges_random::getpartition::. partition out of bounds partition: "<<partition<<", data: "<<data<<", upperlimit: "<<upperlimit<<", _TOTWORKBUFFER_SIZE: "<<_TOTWORKBUFFER_SIZE<<". "<<endl; 
	if(partition >= NUM_PARTITIONS){ 
		cout<<"loadedges_random::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", data: "<<data<<", upperlimit: "<<upperlimit<<", _TOTWORKBUFFER_SIZE: "<<WORKBUFFER_SIZE * VECTOR2_SIZE<<". EXITING... "<<endl; 
		exit(EXIT_FAILURE); 
	}
	return partition;
}

globalparams_TWOt loadedges_random::loadedges(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){			
	cout<<"loadedges_random::loadedges:: loading edges (rowwise)... "<<endl;

	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsK.SIZE_MESSAGESDRAM;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsE.SIZE_MESSAGESDRAM;
	#endif  
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	#else 
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	#endif 
	
	vector<edge2_type> edgedatabuffers_temp[NUMSUBCPUTHREADS];
	vptr_type * tempvptrs[NUMSUBCPUTHREADS]; for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ tempvptrs[i] = new vptr_type[KVDATA_RANGE]; }
	vector<edge3_type> edges2_temp[NUMSUBCPUTHREADS];
	for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ edges_temp[j].clear(); }
	for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ edges2_temp[j].clear(); }
	
	unsigned int * counts_validedges_for_channel[NUMSUBCPUTHREADS];
	unsigned int * counts_alledges_for_channel[NUMSUBCPUTHREADS];
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		counts_validedges_for_channel[i] = new unsigned int[KVDATA_RANGE];
		counts_alledges_for_channel[i] = new unsigned int[KVDATA_RANGE];
		for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){ counts_validedges_for_channel[i][vid] = 0; counts_alledges_for_channel[i][vid] = 0; }
	}
	unsigned int counts_totalvalidedges_for_channel[NUMSUBCPUTHREADS]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_totalvalidedges_for_channel[i] = 0; }
	unsigned int counts_totalalledges_for_channel[NUMSUBCPUTHREADS]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_totalalledges_for_channel[i] = 0; }
	unsigned int totalcount_validedges_for_all_channels = 0;
	unsigned int totalcount_alledges_for_all_channels = 0;
	unsigned int isFirst = 0;
	unsigned int isLast = 0;
	unsigned int _TOTWORKBUFFER_SIZE = (WORKBUFFER_SIZE * VECTOR2_SIZE) / 2;
	
	// calculate counts_validedges_for_channel
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		if(vid % 1000000 == 0){ cout<<"### loadedges_random::loadedges:: vid: "<<vid<<", vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		#endif 
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		for(unsigned int i=0; i<edges_size; i++){
			edge2_type edge = edgedatabuffer[vptr_begin + i];
			unsigned int H = gethash(edge.dstvid);
			edgedatabuffers_temp[H].push_back(edge);
		}
	}
	
	// load edges into memory channels according to edge representation format
	#ifdef LOADSLICEDEDGES
	cout<<"### loadedges_random::loadedges:: loading edges into memory channels according to edge representation format..."<<endl;
	unsigned int tempe_index = 0; 
	unsigned int tempg_index = 0;
	unsigned int index = 0;
	unsigned int counts_alldatas[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_alldatas[i] = 0; }
	unsigned int counts_alldata = 0;
	unsigned int srcvid_lastvechead = 0xFFFFFFFF;
	unsigned int srcvid_lastseen = 0;
	unsigned int numskippededges = 0;
	unsigned int block_partitions[NUM_PARTITIONS];
	unsigned int numcheckpoints = 0;
	bool enablestats = false;//true;

	for(unsigned int i=0; i<NUM_PEs; i++){
		cout<<"### loadedges_random::loadedges:: loading edges into PE: "<<i<<"..."<<endl;
		tempe_index = 0; 
		tempg_index = 0;
		index = 0;
		srcvid_lastvechead = 0xFFFFFFFF; 
		while(tempe_index < edgedatabuffers_temp[i].size()){
			
			// collect stats of next '512*VECTOR2_SIZE' edges (i.e. an edge block)
			// NB: *** source here is tailored to suite FPGA impl. ***
			edge2_type firstedgeinblock;
			if(enablestats){
				if(tempe_index % _TOTWORKBUFFER_SIZE == 0){
					if(numcheckpoints < 8){ cout<<"loadedges_random:: CHECKPOINT SEEN: tempe_index: "<<tempe_index<<", tempg_index: "<<tempg_index<<", offset_kvs: "<<tempg_index/VECTOR2_SIZE<<", edgedatabuffers_temp[i].size(): "<<edgedatabuffers_temp[i].size()<<", _TOTWORKBUFFER_SIZE: "<<_TOTWORKBUFFER_SIZE<<endl; }
					firstedgeinblock = edgedatabuffers_temp[i][tempe_index];
					for(unsigned int p=0; p<NUM_PARTITIONS; p++){ block_partitions[p] = 0; }
					for(unsigned int k=0; k<_TOTWORKBUFFER_SIZE; k++){
						edge2_type thisedge = edgedatabuffers_temp[i][tempe_index + k];
						unsigned int _p = getpartition(thisedge.srcvid, firstedgeinblock.srcvid);
						block_partitions[_p] += 1;
					}
					for(unsigned int p=0; p<NUM_PARTITIONS; p++){ 
						edge3_type edge_temp; edge_temp.srcvid = 0; edge_temp.dstvid = block_partitions[p]; edge_temp.status = EDGESTATUS_INVALIDEDGE; edge_temp.metadata = 0;
						if(p==NUM_PARTITIONS-1){ edge_temp.dstvid = 8888888; } // JUST FOR DEBUGGING. REMOVEME.
						edges_temp[i].push_back(edge_temp);
						counts_alledges_for_channel[i][firstedgeinblock.srcvid] += 1;
					}
					tempg_index += NUM_PARTITIONS;
					#ifdef _DEBUGMODE_HOSTPRINTS
					unsigned int totalps = 0;
					for(unsigned int _p=0; _p<NUM_PARTITIONS; _p++){ cout<<"loadedges_random:: block_partitions["<<_p<<"]: "<<block_partitions[_p]<<endl; totalps += block_partitions[_p]; }
					cout<<"loadedges_random:: totalps: "<<totalps<<", _TOTWORKBUFFER_SIZE: "<<_TOTWORKBUFFER_SIZE<<endl;
					#endif
					numcheckpoints += 1;
				}
			}
			edge2_type edge2 = edgedatabuffers_temp[i][tempe_index];
			if(enablestats){
				edge2.srcvid = (edge2.srcvid - firstedgeinblock.srcvid) % VECTOR2_SIZE; // NB: source here is tailored to suite FPGA impl.
				if(edge2.srcvid >= 1024){ cout<<"loadedges_random::getpartition::ERROR 1. out of bounds edge2.srcvid: "<<edge2.srcvid<<", firstedgeinblock.srcvid: "<<firstedgeinblock.srcvid<<", _TOTWORKBUFFER_SIZE: "<<_TOTWORKBUFFER_SIZE<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
			}
			// exit(EXIT_SUCCESS);
			
			if(tempe_index % 1000000 == 0 && false){ cout<<"loadedges_random::loadedges:: filling edges... tempe_index: "<<tempe_index<<endl; }
			edge2_type edge = edgedatabuffers_temp[i][tempe_index];
			if(debug2b==true){ cout<<">>> edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<" [-]"<<endl; }
			
			// insert edge
			edge3_type edge_temp; edge_temp.srcvid = edge.srcvid; edge_temp.dstvid = edge.dstvid; edge_temp.status = EDGESTATUS_VALIDEDGE; edge_temp.metadata = 0; edges_temp[i].push_back(edge_temp);
			edge3_type edge2_temp; edge2_temp.srcvid = edge2.srcvid; edge2_temp.dstvid = edge2.dstvid; edge2_temp.status = EDGESTATUS_VALIDEDGE; edge2_temp.metadata = 0, edges2_temp[i].push_back(edge2_temp);
			if(debugb==true){ cout<<">>> edge_temp.srcvid: "<<edge_temp.srcvid<<", edge_temp.dstvid: "<<edge_temp.dstvid<<" [3]"<<endl; }
			tempe_index += 1;
			tempg_index += 1;
			index += 1;
			counts_validedges_for_channel[i][edge_temp.srcvid] += 1;
			counts_alledges_for_channel[i][edge_temp.srcvid] += 1;
		}
		
		counts_alldatas[i] += index;
		counts_alldata += index;
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"edges_temp["<<i<<"].size(): "<<edges_temp[i].size()<<", edges2_temp["<<i<<"].size(): "<<edges2_temp[i].size()<<endl;
		utilityobj->printtriples("loadedges_random::[insert.edges] printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8);
		cout<<"### loadedges_random::[insert.edges] memory channel "<<i<<": tempe_index: "<<tempe_index<<", index: "<<index<<endl;
		#endif
	}
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printvalues(">>> loadedges_random:[insert.edges] total number of edges in channels[0-N]", (value_t *)&counts_alldatas[0], NUM_PEs);
	cout<<">>> loadedges_random::[insert.edges] total number of edges in all memory channels: counts_alldata: "<<counts_alldata<<", NAp: "<<NAp<<endl;
	#endif 
	cout<<"### loadedges_random::[insert.edges] total number of skipped edges: "<<numskippededges<<endl;
	#endif 
	cout<<"### loadedges_random::[insert.edges] total number of skipped edges: "<<numskippededges<<endl;
	// exit(EXIT_SUCCESS);////////////////////
	
	// calculate local edge dstvids 
	#ifdef CALCULATELOCALDSTVIDS
	cout<<"### loadedges_random::insert.bitmap:: calculating local edge dstvids..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int k=0; k<edges_temp[i].size(); k++){
			if(edges_temp[i][k].status == EDGESTATUS_VALIDEDGE){ 
				edges_temp[i][k].dstvid = (edges_temp[i][k].dstvid - i) / NUM_PEs; 
				edges2_temp[i][k].dstvid = (edges2_temp[i][k].dstvid - i) / NUM_PEs; 
			}
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printtriples("loadedges_random::[insert.local.edge.dstvids] printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8);
		#endif 
	}
	cout<<"### loadedges_random::[insert.local.edge.dstvids] calculating local edge dstvids inserted successfully"<<endl;
	#endif 

	// insert bitmap 
	#ifdef INSERTBITMAP
	cout<<"### loadedges_random::insert.bitmap:: inserting bitmap..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		if(debug2b==true){ utilityobj->printtriples("loadedges_random::insert.bitmap::[before.insert.bitmap]: printing edges2_temp["+std::to_string(i)+"][~]", (triple_t *)&edges2_temp[i][0], 8); }
		for(unsigned int j=0; j<edges2_temp[i].size(); j+=VECTOR2_SIZE){
			for(unsigned int v=1; v<VECTOR2_SIZE; v++){
				if(j+v >= edges2_temp[i].size()){ continue; }
				if(edges2_temp[i][j+v].status == EDGESTATUS_DUMMYEDGE || edges2_temp[i][j+v].status == EDGESTATUS_INVALIDEDGE){ continue; }
				
				unsigned int incr = edges2_temp[i][j+v].srcvid; // % 16; // edges_temp[i][j+v].srcvid - srcvid_head; // CRITICAL FIXME.
				// unsigned int loc = v;
				// if(incr > 15){ cout<<"loadedges_random::insert.bitmap:: ERROR 4. incr("<<incr<<") > 3. exiting..."<<endl; exit(EXIT_FAILURE); }
				// if(loc > 15){ cout<<"loadedges_random::insert.bitmap:: ERROR 4. loc("<<loc<<") > 31. exiting..."<<endl; exit(EXIT_FAILURE); }
				
				// WRITETO_UINT((unsigned int *)&edges2_temp[i][j+v].dstvid, OFFSETOF_SRCV_IN_EDGEDSTVDATA, SIZEOF_SRCV_IN_EDGEDSTVDATA, incr); // NEWCHANGE.
				edges2_temp[i][j+v].metadata = incr; 
			}
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printtriples("loadedges_random::insert.bitmap::[after.insert.bitmap]: printing edges2_temp["+std::to_string(i)+"][~]", (triple_t *)&edges2_temp[i][0], 8);
		#endif 
	}
	cout<<"### loadedges_random::insert.bitmap::[insert.bitmap] bitmap inserted successfully"<<endl;
	#endif
	
	// load edges
	#ifdef LOADEDGES
	cout<<"### loadedges_random::insert.bitmap:: loading edges..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int k=0; k<edges2_temp[i].size(); k++){
			edges[i][TWOO*_BASEOFFSET_EDGESDATA + k].dstvid = edges2_temp[i][k].dstvid;
		}
		if(debug2b==true){ utilityobj->printvalues("loadedges_random[after]::loadedges: printing edges["+std::to_string(i)+"][~]", (value_t *)&edges[i][TWOO*_BASEOFFSET_EDGESDATA], 8); }
	}
	cout<<"### loadedges_random::insert.bitmap::[insert.bitmap] loading edges completed successfully"<<endl;
	#endif 
	
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){ counts_totalalledges_for_channel[i] += counts_alledges_for_channel[i][vid]; totalcount_alledges_for_all_channels += counts_alledges_for_channel[i][vid]; }}
	unsigned int max_totalalledges_for_channel=0; for(unsigned int i=0; i<NUM_PEs; i++){ if(max_totalalledges_for_channel < counts_totalalledges_for_channel[i]){ max_totalalledges_for_channel = counts_totalalledges_for_channel[i]; }}
	cout<<"loadedges_random::loadedges: max_totalalledges_for_channel: "<<max_totalalledges_for_channel<<endl;

	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0; 
	globalparams.globalparamsK.SIZE_EDGES = 0; 
	
	globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + ((max_totalalledges_for_channel/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + DRAMPADD_KVS;
	globalparams.globalparamsE.SIZE_VERTEXPTRS = KVDATA_RANGE / VPTR_SHRINK_RATIO; 
	globalparams.globalparamsE.SIZE_EDGES = max_totalalledges_for_channel + 1000; // '1000' is padding 
	#else 
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA + ((max_totalalledges_for_channel/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + DRAMPADD_KVS;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = KVDATA_RANGE / VPTR_SHRINK_RATIO;
	globalparams.globalparamsK.SIZE_EDGES = max_totalalledges_for_channel + 1000;
	#endif
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams_t globalparamsVPTRS = globalparams.globalparamsE;
	#else
	globalparams_t globalparamsVPTRS = globalparams.globalparamsK;
	#endif 
	
	// calculate offsets
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		tempvptrs[i][0].key = 0;
		for(unsigned int vid=1; vid<KVDATA_RANGE; vid++){
			tempvptrs[i][vid].key = tempvptrs[i][vid-1].key + counts_alledges_for_channel[i][vid-1]; 
		}
	}
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		unsigned int _index = 0;
		for(unsigned int vid=0; vid<KVDATA_RANGE; vid+=VPTR_SHRINK_RATIO){	
			vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE) + _index].key = utilityobj->allignlowerto16_KV(tempvptrs[i][vid].key); // tempvptrs[i][vid].key; // NEWCHANGE.
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(i==0){ cout<<"vptrs[i]["<<_index<<"].key: "<<vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE) + _index].key<<endl; }
			#endif 
			_index += 1;
		}
		for(unsigned int vid=0; vid<(DRAMPADD/2); vid++){
			vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE) + _index].key = counts_totalalledges_for_channel[i];
			_index += 1;
		}
	}
	
	// CHECK 4: check vptrs against edges
	#ifdef CHECK4_VERIFYOFFSETS
	cout<<"### loadedges_random::insert.bitmap:: checking vptrs against edges (CHECK 4(a&b))..."<<endl;
	unsigned int vptr_offset = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid+=VPTR_SHRINK_RATIO){	
			if(false){ if(tempvptrs[i][vptr_offset+vid].key % VECTOR2_SIZE != 0){ cout<<"CHECK 4(a): ERROR CHECKING FAILED. tempvptrs["<<i<<"]["<<vptr_offset+vid<<"].key("<<tempvptrs[i][vptr_offset+vid].key<<") % VECTOR2_SIZE != 0. EXITING... "<<endl; exit(EXIT_FAILURE); }} // too expensive
			for(unsigned int k=tempvptrs[i][vptr_offset+vid].key; k<tempvptrs[i][vptr_offset+vid+1].key; k++){
				if(edges_temp[i][k].srcvid != vid){
					cout<<"### loadedges_random::loadedges:: CHECK 4(a): ERROR CHECKING FAILED: i: "<<i<<", vid:"<<vid<<", k: "<<k<<", tempvptrs["<<i<<"]["<<vid<<"]: "<<tempvptrs[i][vptr_offset+vid].key<<", tempvptrs["<<i<<"]["<<vid+1<<"]: "<<tempvptrs[i][vptr_offset+vid+1].key<<". EXITING..."<<endl;
					for(unsigned int v1=0; v1<VECTOR2_SIZE; v1++){
						cout<<v1<<": edges_temp["<<i<<"]["<<k+v1<<"].srcvid: "<<edges_temp[i][k+v1].srcvid<<", edges_temp["<<i<<"]["<<k+v1<<"].dstvid: "<<edges_temp[i][k+v1].dstvid<<", edges_temp["<<i<<"]["<<k+v1<<"].status: "<<edges_temp[i][k+v1].status<<", edges_temp["<<i<<"]["<<k+v1<<"].metadata: "<<edges_temp[i][k+v1].metadata<<endl;
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	cout<<"### loadedges_random::loadedges:: CHECK 4(a&b): ERROR CHECKING SUCCESSFUL."<<endl;
	#endif
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = utilityobj->allignhigherto16_KV(graphobj->get_num_vertices());
		container->edgessize[i] = counts_totalalledges_for_channel[i]; 
		container->runsize[i] = counts_totalalledges_for_channel[i];
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){ counts_totalvalidedges_for_channel[i] += counts_validedges_for_channel[i][vid]; totalcount_validedges_for_all_channels += counts_validedges_for_channel[i][vid]; }}
	utilityobj->printvalues(">>> loadedges_random::loadedges: printing counts_totalvalidedges_for_channel", (value_t *)&counts_totalvalidedges_for_channel[0], NUM_PEs);
	utilityobj->printvalues(">>> loadedges_random::loadedges: printing counts_totalalledges_for_channel", (value_t *)&counts_totalalledges_for_channel[0], NUM_PEs);
	utilityobj->printvalues(">>> loadedges_random::loadedges: printing counts_alldatas[0-N]", (value_t *)&counts_alldatas[0], NUM_PEs);
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_random[after]::loadedges: printing counts_validedges_for_channel["+std::to_string(i)+"][~]", (value_t *)&counts_validedges_for_channel[i][0], 4); } 
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_random[after]::loadedges: printing counts_alledges_for_channel["+std::to_string(i)+"][~]", (value_t *)&counts_alledges_for_channel[i][0], 4); } 
	for(unsigned int i=0; i<0; i++){ utilityobj->printkeyvalues("loadedges_random[after]::loadedges: printing edgedatabuffers_temp["+std::to_string(i)+"][~]", (keyvalue_t *)&edgedatabuffers_temp[i][0], 4); }
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_random::loadedges: printing vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE)], 8); } 
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadedges_random::loadedges: totalcount_validedges_for_all_channels: "<<totalcount_validedges_for_all_channels<<", totalcount_alledges_for_all_channels: "<<totalcount_alledges_for_all_channels<<", *counts_alldata: "<<counts_alldata<<endl;
	#endif 
	// exit(EXIT_SUCCESS); //
	
	for(unsigned int i=0; i<NUM_PEs; i++){ edgedatabuffers_temp[i].clear(); }
	for(unsigned int i=0; i<NUM_PEs; i++){ delete tempvptrs[i]; }
	return globalparams;
}


