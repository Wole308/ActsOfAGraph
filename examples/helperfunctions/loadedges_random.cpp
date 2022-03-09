#include "loadedges_random.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

#define LOADSLICEDEDGES_CORRECT
// #define LOADSLICEDEDGES_INCORRECT
// #define CHECK4_VERIFYOFFSETS
#define CALCULATELOCALDSTVIDS
#define INSERTBITMAP
#define LOADEDGES

#define TWOO 2

bool debugb = false;//false;
bool debug2b = false; //true;//false;
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

unsigned int loadedges_random::allignlower(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = val / V_SIZE;
	// cout<<"fac: "<<fac<<", val: "<<val<<", V_SIZE: "<<V_SIZE<<", (fac * V_SIZE): "<<(fac * V_SIZE)<<endl;
	return (fac * V_SIZE);
}
unsigned int loadedges_random::allignhigher(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = (val + (V_SIZE-1)) / V_SIZE;
	return (fac * V_SIZE);
}

unsigned int loadedges_random::gethash(unsigned int vid){
	return vid % NUM_PEs;
}
unsigned int loadedges_random::getlocalvid(unsigned int vid){
	unsigned int s = gethash(vid);
	return (vid - s) / NUM_PEs; 
}

unsigned int loadedges_random::twist_getpartition(unsigned int partition, unsigned int s){
	return (partition + s) % NUM_PARTITIONS;
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
tuple_t loadedges_random::get_partition_and_incr(unsigned int vid, vertex_t upperlimit){
	
	unsigned int lvid = vid % PROCESSPARTITIONSZ; // FIXME.

	unsigned int s = gethash(lvid);
	unsigned int bank_lvid = getlocalvid(lvid);
	unsigned int lcol = bank_lvid % VECTOR2_SIZE;
	unsigned int lrow = (bank_lvid / VECTOR2_SIZE);
	unsigned int incr = (s * (PROCESSPARTITIONSZ_KVS2 / NUM_PEs)) + lrow;
	
	if(incr >= 1024){ cout<<"loadedges_random::getpartition(112)::. out of bounds incr: "<<incr<<", vid: "<<vid<<", lvid: "<<lvid<<", bank_lvid: "<<bank_lvid<<", lcol: "<<lcol<<", lrow: "<<lrow<<", incr: "<<incr<<", s: "<<s<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"get_partition_and_incr:: ";
	<<", vid: "<<vid
	<<", lvid: "<<lvid
	<<", bank_lvid: "<<bank_lvid
	<<", lcol: "<<lcol
	<<", lrow: "<<lrow
	<<", incr: "<<incr
	<<", s: "<<s
	<<endl;
	#endif 
	
	tuple_t res;
	// res.A = lcol; // twist_getpartition(lcol, s); // partition 
	res.A = twist_getpartition(lcol, s); // partition 
	res.B = incr; // incr 
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	if((res.B==42 && res.A == 1 && vid < 64)){ 
	cout<<"get_partition_and_incr:: res.B==42:";
	<<", vid: "<<vid
	<<", lvid: "<<lvid
	<<", bank_lvid: "<<bank_lvid
	<<", lcol: "<<lcol
	<<", lrow: "<<lrow
	<<", incr: "<<incr
	<<", s: "<<s
	<<endl; }
	#endif 		
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"get_partition_and_incr(113):: partition: "<<res.A
	<<", incr: "<<res.B
	<<endl;
	#endif 
	return res;
}

void loadedges_random::calculateoffsets(keyvalue_t * buffer, unsigned int size){
	for(buffer_type i=1; i<size; i++){ 
		// buffer[i].key = allignlower(buffer[i-1].key + buffer[i-1].value, VECTOR2_SIZE); 
		buffer[i].key = buffer[i-1].key + buffer[i-1].value; 
	}
	return;
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
	unsigned int errcount = 0;
	unsigned int num_zeros = 0;
	unsigned int total_num_zeros = 0;
	for(unsigned int vid=0; vid<utilityobj->hmin(graphobj->get_num_vertices(), KVDATA_RANGE); vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		if(vid % 1000000 == 0){ cout<<"### loadedges_random::loadedges:: vid: "<<vid<<" (of "<<graphobj->get_num_vertices()<<" vertices), vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		#endif 
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ continue; cout<<">>> loadedges_random:: vptr_end("<<vptr_end<<") < vptr_begin("<<vptr_begin<<"). EXITING... (vid: "<<vid<<", edges_size: "<<edges_size<<")"<<endl; exit(EXIT_FAILURE); }
		// if(edges_size > 100000){ cout<<">>> loadedges_random:: edges_size("<<edges_size<<") > 10000. EXITING... (vid: "<<vid<<", edges_size: "<<edges_size<<")"<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int i=0; i<edges_size; i++){
			edge2_type edge = edgedatabuffer[vptr_begin + i];
			if(edge.dstvid == 0 && edge.srcvid == 0){ total_num_zeros += 1; }
			
			unsigned int H = gethash(edge.dstvid);
			edgedatabuffers_temp[H].push_back(edge);
		}
	}
	for(unsigned int i=0; i<NUM_PEs; i++){
		cout<<">>> loadedges_random:: edgedatabuffers_temp["<<i<<"].size(): "<<edgedatabuffers_temp[i].size()<<endl;
	}
	cout<<"loadedges_random:: total_num_zeros: "<<total_num_zeros<<", graphobj->getedgessize(0): "<<graphobj->getedgessize(0)<<endl;
	if(total_num_zeros > 1000000){ cout<<">>> loadedges_random:: TOO MANY ZEROS. EXITING... (total_num_zeros: "<<total_num_zeros<<", num_vertices: "<<graphobj->get_num_vertices()<<")"<<endl; exit(EXIT_FAILURE); }
	// exit(EXIT_SUCCESS);
	
	// load edges into memory channels according to edge representation format
	#ifdef LOADSLICEDEDGES_CORRECT
	cout<<"### loadedges_random::loadedges:: loading edges into memory channels according to edge representation format..."<<endl;
	unsigned int tempe_index = 0; 
	unsigned int index = 0;
	unsigned int counts_alldatas[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_alldatas[i] = 0; }
	unsigned int counts_alldata = 0;
	unsigned int srcvid_lastvechead = 0xFFFFFFFF;
	unsigned int srcvid_lastseen = 0;
	unsigned int numskippededges = 0;
	keyvalue_t block_partitions[NUM_PARTITIONS];
	unsigned int numcheckpoints = 0;
	bool loadcapsulestats = false;
	
	edge2_type edgeblock[_TOTWORKBUFFER_SIZE];
	
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<0; t++){ cout<<"sample edges:: edgedatabuffers_temp["<<i<<"]["<<t<<"].srcvid: "<<edgedatabuffers_temp[i][t].srcvid<<", edgedatabuffers_temp["<<i<<"]["<<t<<"].dstvid: "<<edgedatabuffers_temp[i][t].dstvid<<endl; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ // CRITICAL REMOVEME.
		cout<<endl<<">>>  loadedges_random::loadedges:: loading edges into PE: "<<i<<", edgedatabuffers_temp["<<i<<"].size(): "<<edgedatabuffers_temp[i].size()<<"..."<<endl;
		tempe_index = 0;
		index = 0;
		srcvid_lastvechead = 0xFFFFFFFF; 
		unsigned int edgeblockid = 0;
		while(tempe_index < edgedatabuffers_temp[i].size()){
			// collect stats of next '512*VECTOR2_SIZE' edges (i.e. an edge block)
			// NB: *** source here is tailored to suite FPGA impl. ***
			edge2_type firstedgeinblock;
			unsigned int srcvtxoffsetofblock;
			if(tempe_index % _TOTWORKBUFFER_SIZE == 0){
				
				unsigned int _TOTWORKBUFFER_SIZE_EDGECONDITIONS_ = utilityobj->hmin(_TOTWORKBUFFER_SIZE, edgedatabuffers_temp[i].size() - tempe_index);
				if(_TOTWORKBUFFER_SIZE_EDGECONDITIONS_ != _TOTWORKBUFFER_SIZE){ cout<<"loadedges_random::edge condition:: i: "<<i<<", tempe_index: "<<tempe_index<<", _TOTWORKBUFFER_SIZE: "<<_TOTWORKBUFFER_SIZE<<", _TOTWORKBUFFER_SIZE_EDGECONDITIONS_: "<<_TOTWORKBUFFER_SIZE_EDGECONDITIONS_<<endl; }
				
				if(debug2b==true){ for(unsigned int t=0; t<_TOTWORKBUFFER_SIZE_EDGECONDITIONS_; t++){ edgeblock[t] = edgedatabuffers_temp[i][tempe_index + t]; }}
				if(debug2b==true){ for(unsigned int t=0; t<4; t++){ cout<<"loadedges_random[BEFORE]:: edgeblock["<<t<<"].srcvid: "<<edgeblock[t].srcvid<<", edgeblock["<<t<<"].dstvid: "<<edgeblock[t].dstvid<<endl; }}
				
				if(numcheckpoints < 8){ cout<<"loadedges_random:: CHECKPOINT SEEN: tempe_index: "<<tempe_index<<", edgedatabuffers_temp[i].size(): "<<edgedatabuffers_temp[i].size()<<", _TOTWORKBUFFER_SIZE: "<<_TOTWORKBUFFER_SIZE<<endl; }
				firstedgeinblock = edgedatabuffers_temp[i][tempe_index];
				srcvtxoffsetofblock = allignlower(firstedgeinblock.srcvid, PROCESSPARTITIONSZ);
				if(debug2b==true){ cout<<"loadedges_random:: srcvtxoffsetofblock: "<<srcvtxoffsetofblock<<", firstedgeinblock.srcvid: "<<firstedgeinblock.srcvid<<endl; }
				
				for(unsigned int p=0; p<NUM_PARTITIONS; p++){ block_partitions[p].key = 0; block_partitions[p].value = 0; }
				for(unsigned int t=0; t<_TOTWORKBUFFER_SIZE_EDGECONDITIONS_; t++){
					edge2_type thisedge = edgedatabuffers_temp[i][tempe_index + t];
					tuple_t _p = get_partition_and_incr(thisedge.srcvid, srcvtxoffsetofblock);
					block_partitions[_p.A].value += 1;
				}
				calculateoffsets(block_partitions, NUM_PARTITIONS);
				if(debug2b==true){ for(unsigned int _p=0; _p<NUM_PARTITIONS; _p++){ cout<<"loadedges_random:: block_partitions["<<_p<<"].key: "<<block_partitions[_p].key<<", block_partitions["<<_p<<"].value: "<<block_partitions[_p].value<<endl; }}
				
				// load capsule stats 
				if(loadcapsulestats == true){
					for(unsigned int p=0; p<NUM_PARTITIONS; p++){ 
						edge3_type edge_temp; edge_temp.srcvid = 0; edge_temp.dstvid = block_partitions[p].value; edge_temp.status = EDGESTATUS_INVALIDEDGE; edge_temp.metadata = 0;
						if(p==NUM_PARTITIONS-1){ edge_temp.dstvid = 8888888; } // JUST FOR DEBUGGING. REMOVEME.
						edges_temp[i].push_back(edge_temp);
						counts_alledges_for_channel[i][firstedgeinblock.srcvid] += 1;
					}
				}
				
				// load edge block 
				for(unsigned int p=0; p<NUM_PARTITIONS; p++){ block_partitions[p].value = 0; }
				for(unsigned int t=0; t<_TOTWORKBUFFER_SIZE_EDGECONDITIONS_; t++){
					edge2_type thisedge = edgedatabuffers_temp[i][tempe_index + t];
					utilityobj->checkoutofbounds("loadedges_random(21)::", thisedge.dstvid, graphobj->get_num_vertices()+10, thisedge.dstvid, tempe_index, t);
					tuple_t _p = get_partition_and_incr(thisedge.srcvid, srcvtxoffsetofblock);
					edgeblock[block_partitions[_p.A].key + block_partitions[_p.A].value] = thisedge;
					if(debug2b==true){ if(block_partitions[_p.A].value < 8){ cout<<"loadedges_random::contents:: _p: "<<_p.A<<", thisedge.srcvid: "<<thisedge.srcvid<<", thisedge.dstvid: "<<thisedge.dstvid<<endl; }}
					block_partitions[_p.A].value += 1;
				}
				if(debug2b==true){ for(unsigned int t=0; t<4; t++){ cout<<"loadedges_random[AFTER]:: edgeblock["<<t<<"].srcvid: "<<edgeblock[t].srcvid<<", edgeblock["<<t<<"].dstvid: "<<edgeblock[t].dstvid<<endl; }}
				// exit(EXIT_SUCCESS); ////////////////
				
				// insert edges into buffer...
				for(unsigned int t=0; t<_TOTWORKBUFFER_SIZE_EDGECONDITIONS_; t++){
					edge2_type edge2 = edgeblock[t];
					edge2_type edge = edgeblock[t]; 
					utilityobj->checkoutofbounds("loadedges_random(22)::", edge.dstvid, graphobj->get_num_vertices()+10, edge.srcvid, edge.dstvid, graphobj->get_num_vertices());
					if(debug2b==true){ cout<<">>> edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", edge2.srcvid: "<<edge2.srcvid<<", edge2.dstvid: "<<edge2.dstvid<<" [-]"<<endl; }
					if(debug2b==true){ cout<<">>> edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<" [-]"<<endl; }
					
					//////////////////////////
					// NB: source here is tailored to suite FPGA impl.
					unsigned int partition = get_partition_and_incr(edge2.srcvid, srcvtxoffsetofblock).A;
					unsigned int incr = get_partition_and_incr(edge2.srcvid, srcvtxoffsetofblock).B;
					#ifdef _DEBUGMODE_HOSTPRINTS
					if(incr==42 && partition == 1 && edge2.srcvid < 64){ cout<< TIMINGRESULTSCOLOR << "loadedges:: incr==42: incr: "<<incr<<", partition: "<<partition<<" [edge2.srcvid: "<<edge2.srcvid<<", edge2.dstvid: "<<edge2.dstvid<<"]"<< RESET << endl; }
					#endif 	
					edge2.srcvid = incr;
					utilityobj->checkoutofbounds("loadedges_random(24)::", edge2.srcvid, 1024, srcvtxoffsetofblock, _TOTWORKBUFFER_SIZE_EDGECONDITIONS_, firstedgeinblock.srcvid);
					//////////////////////////
					
					// insert edge
					edge3_type edge_temp; edge_temp.srcvid = edge.srcvid; edge_temp.dstvid = edge.dstvid; edge_temp.status = EDGESTATUS_VALIDEDGE; edge_temp.metadata = 0; edges_temp[i].push_back(edge_temp);
					edge3_type edge2_temp; edge2_temp.srcvid = edge2.srcvid; edge2_temp.dstvid = edge2.dstvid; edge2_temp.status = EDGESTATUS_VALIDEDGE; edge2_temp.metadata = 0, edges2_temp[i].push_back(edge2_temp);
					utilityobj->checkoutofbounds("loadedges_random(25)::", edge_temp.dstvid, graphobj->get_num_vertices()+10, edge.srcvid, edge.dstvid, graphobj->get_num_vertices());
					utilityobj->checkoutofbounds("loadedges_random(26)::", edge2_temp.dstvid, graphobj->get_num_vertices()+10, edge2.srcvid, edge2.dstvid, graphobj->get_num_vertices());
					if(debugb==true){ cout<<">>> edge_temp.srcvid: "<<edge_temp.srcvid<<", edge_temp.dstvid: "<<edge_temp.dstvid<<" [3]"<<endl; }
					tempe_index += 1;
					index += 1;
					counts_validedges_for_channel[i][edge_temp.srcvid] += 1;
					counts_alledges_for_channel[i][edge_temp.srcvid] += 1;
				}
				
				if(tempe_index % 1000000 == 0 && false){ cout<<"loadedges_random::loadedges:: filling edges... index: "<<index<<", tempe_index: "<<tempe_index<<endl; }
				numcheckpoints += 1;
				edgeblockid += 1;
				// exit(EXIT_SUCCESS); ////////////////
			} else {
				cout<<"loadedges_random: ERROR SOMEWHERE. (tempe_index("<<tempe_index<<") % _TOTWORKBUFFER_SIZE("<<_TOTWORKBUFFER_SIZE<<") != 0). EXITING...."<<endl;
				exit(EXIT_FAILURE);
			}
		}
		
		counts_alldatas[i] += index;
		counts_alldata += index;
		
		#ifdef _DEBUGMODE_HOSTPRINTS3
		utilityobj->printtriples("loadedges_random::[insert.edges] printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8);
		cout<<"### edges_temp["<<i<<"].size(): "<<edges_temp[i].size()<<", edges2_temp["<<i<<"].size(): "<<edges2_temp[i].size()<<endl;
		cout<<"### loadedges_random::[insert.edges] memory channel "<<i<<": tempe_index: "<<tempe_index<<", index: "<<index<<endl;
		#endif
		// exit(EXIT_SUCCESS);////////////////////
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	utilityobj->printvalues(">>> loadedges_random:[insert.edges] total number of edges in channels[0-N]", (value_t *)&counts_alldatas[0], NUM_PEs);
	cout<<">>> loadedges_random::[insert.edges] total number of edges in all memory channels: counts_alldata: "<<counts_alldata<<", NAp: "<<NAp<<endl;
	#endif 
	cout<<"### loadedges_random::[insert.edges] total number of skipped edges: "<<numskippededges<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// load edges into memory channels according to edge representation format
	#ifdef LOADSLICEDEDGES_INCORRECT
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
	#ifdef _DEBUGMODE_HOSTPRINTS3
	utilityobj->printvalues(">>> loadedges_random:[insert.edges] total number of edges in channels[0-N]", (value_t *)&counts_alldatas[0], NUM_PEs);
	cout<<">>> loadedges_random::[insert.edges] total number of edges in all memory channels: counts_alldata: "<<counts_alldata<<", NAp: "<<NAp<<endl;
	#endif 
	cout<<"### loadedges_random::[insert.edges] total number of skipped edges: "<<numskippededges<<endl;
	#endif
	// exit(EXIT_SUCCESS); 
	
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
				
				unsigned int codededge = 0;
				WRITETO_UINT(&codededge, OFFSETOF_SRCV_IN_EDGEDSTVDATA, SIZEOF_SRCV_IN_EDGEDSTVDATA, edges2_temp[i][j+v].srcvid);
				WRITETO_UINT(&codededge, OFFSETOF_DSTV_IN_EDGEDSTVDATA, SIZEOF_DSTV_IN_EDGEDSTVDATA, edges2_temp[i][j+v].dstvid);
				edges2_temp[i][j+v].dstvid = codededge;
				
				edges2_temp[i][j+v].metadata = edges2_temp[i][j+v].srcvid; 
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



