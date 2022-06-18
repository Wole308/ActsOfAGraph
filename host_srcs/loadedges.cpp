#include "loadedges.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

#define LOADSLICEDEDGES_CORRECT	
#define CALCULATELOCALDSTVIDS
#define INSERTBITMAP
#define LOADEDGES
#define CALCULATEOFFSETS

#define LOADEDGES_ENSUREPERFECTACCURACY

#define TWOO 2

// #define MYVDATA_SHRINK_FACTOR VDATA_SHRINK_RATIO
#define MYVDATA_SHRINK_FACTOR 1

bool debugb = false;//false;
bool debug2b = false; //true;//false;
bool debug3b = false;

loadedges::loadedges(universalparams_t _universalparams){
	utilityobj = new utility(_universalparams);
	evalparamsobj = new evalparams();
	universalparams = _universalparams;
}
loadedges::~loadedges(){} 

unsigned int loadedges::GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int loadedges::READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & GETMASK_UINT((index), (size))) >> (index)); 
}
void loadedges::WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
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

unsigned int loadedges::allignlower(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = val / V_SIZE;
	// cout<<"fac: "<<fac<<", val: "<<val<<", V_SIZE: "<<V_SIZE<<", (fac * V_SIZE): "<<(fac * V_SIZE)<<endl;
	return (fac * V_SIZE);
}
unsigned int loadedges::allignhigher(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = (val + (V_SIZE-1)) / V_SIZE;
	return (fac * V_SIZE);
}

unsigned int loadedges::gethash(unsigned int vid){
	return vid % NUM_PEs;
}
unsigned int loadedges::getlocalvid(unsigned int vid){
	unsigned int s = gethash(vid);
	return (vid - s) / NUM_PEs; 
}

unsigned int loadedges::twist_getpartition(unsigned int partition, unsigned int s){
	/// NB: This function must be consistent with mem_access::MEMACCESS{{context['id']}}_RearrangeLayoutV
	return (partition + s) % universalparams.NUM_PARTITIONS;
}

unsigned int loadedges::getpartition(unsigned int data, vertex_t upperlimit){
	// unsigned int partition = (data - upperlimit) / ((universalparams.WORKBUFFER_SIZE*VECTOR2_SIZE) / universalparams.NUM_PARTITIONS);
	unsigned int partition = (data - upperlimit) % VECTOR2_SIZE;
	// cout<<"loadedges::getpartition::. partition out of bounds partition: "<<partition<<", data: "<<data<<", upperlimit: "<<upperlimit<<", _ACTS_READEDGEGRANULARITY: "<<_ACTS_READEDGEGRANULARITY<<". "<<endl; 
	if(partition >= universalparams.NUM_PARTITIONS){ 
		cout<<"loadedges::getpartition::ERROR 1. partition out of bounds partition: "<<partition<<", data: "<<data<<", upperlimit: "<<upperlimit<<", _ACTS_READEDGEGRANULARITY: "<<universalparams.WORKBUFFER_SIZE * VECTOR2_SIZE<<". EXITING... "<<endl; 
		exit(EXIT_FAILURE); 
	}
	return partition;
}
tuple_t loadedges::get_partition_and_incr(unsigned int vid, unsigned int dstvid){
	
	//////////////////////////////////
	// vid=1;
	// dstvid=333; // CRIICAL REMOVEME.
	////////////////////////////////////
	
	unsigned int lvid = vid % universalparams.PROCESSPARTITIONSZ; // FIXME.

	unsigned int s = gethash(lvid);
	unsigned int lvid_inbank = getlocalvid(lvid);
	unsigned int lcol = lvid_inbank % VECTOR2_SIZE;
	unsigned int lrow = (lvid_inbank / VECTOR2_SIZE);
	unsigned int incr = (s * (universalparams.PROCESSPARTITIONSZ_KVS2 / NUM_PEs)) + lrow;		
	incr = incr;
	
	// if(incr >= universalparams.PROCESSPARTITIONSZ_KVS2){ cout<<"loadedges::getpartition(112)::. out of bounds incr: "<<incr<<", vid: "<<vid<<", lvid: "<<lvid<<", lvid_inbank: "<<lvid_inbank<<", lcol: "<<lcol<<", lrow: "<<lrow<<", incr: "<<incr<<", s: "<<s<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"get_partition_and_incr:: ";
	<<", vid: "<<vid
	<<", lvid: "<<lvid
	<<", lvid_inbank: "<<lvid_inbank
	<<", lcol: "<<lcol
	<<", lrow: "<<lrow
	<<", incr: "<<incr
	<<", s: "<<s
	<<endl;
	#endif 
	
	tuple_t res;
	// res.A = twist_getpartition(lcol, s);
	#ifdef CONFIG_MEMLAYOUT_ENABLE_REARRANGEVPROP
	res.A = twist_getpartition(lcol, s);
	#else 
	res.A = lcol;
	#endif 
	res.B = incr; // incr 
	
	#ifdef CONFIG_FORCEDCORRECT_LOADEDGES
	res.B = res.B % (1 << SIZEOF_SRCV_IN_EDGEDSTVDATA); // FIXME. FORCED CORRECT.
	#endif 
	
	if(res.A >= universalparams.NUM_PARTITIONS){ cout<<"loadedges::getpartition(111)::. out of bounds res.A: "<<res.A<<", vid: "<<vid<<", lvid: "<<lvid<<", lvid_inbank: "<<lvid_inbank<<", lcol: "<<lcol<<", lrow: "<<lrow<<", incr: "<<incr<<", s: "<<s<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	if(res.B >= universalparams.PROCESSPARTITIONSZ_KVS2){ cout<<"loadedges::getpartition(112)::. out of bounds incr: "<<incr<<", vid: "<<vid<<", lvid: "<<lvid<<", lvid_inbank: "<<lvid_inbank<<", lcol: "<<lcol<<", lrow: "<<lrow<<", incr: "<<incr<<", s: "<<s<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
	
	// res.A = 0;
	// res.B = 0; 
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	if(res.A == 1 && res.B == 42 && vid < universalparams.PROCESSPARTITIONSZ && dstvid != NAp){ 
		cout<<"****************************** loadedges::getpartition(112):: "<<endl;
		cout<<"*** res.A (partition): "<<res.A<<endl;
		cout<<", res.B (incr): "<<res.B<<endl;
		cout<<", srcvid: "<<vid<<endl;
		cout<<", dstvid: "<<dstvid<<endl;
		cout<<", lvid: "<<lvid<<endl;
		cout<<", lvid_inbank: "<<lvid_inbank<<endl;
		cout<<", lcol (partition before twist): "<<lcol<<endl;
		cout<<", lrow: "<<lrow<<endl;
		cout<<", incr: "<<incr<<endl;
		cout<<", s: "<<s<<endl;
		cout<<", universalparams.PROCESSPARTITIONSZ: "<<universalparams.PROCESSPARTITIONSZ<<endl;
		cout<<". "<<endl; 
		// exit(EXIT_SUCCESS);
		
		// cout<<"--- get_partition_and_incr: res.A (partition): "<<res.A<<", res.B (incr): "<<res.B<<", srcvid: "<<vid<<", dstvid: "<<dstvid<<", lcol (partition before twist): "<<lcol<<", universalparams.PROCESSPARTITIONSZ: "<<universalparams.PROCESSPARTITIONSZ<<endl;
	}
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS
	if((res.B==42 && res.A == 1 && vid < 64)){ 
	cout<<"get_partition_and_incr:: res.B==42:";
	<<", vid: "<<vid
	<<", lvid: "<<lvid
	<<", lvid_inbank: "<<lvid_inbank
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
	
	// exit(EXIT_SUCCESS);
	return res;
}

void loadedges::calculateoffsets(keyvalue_t * buffer, unsigned int size){
	buffer[0].key = 0;
	for(buffer_type i=1; i<size; i++){ 
		// buffer[i].key = allignlower(buffer[i-1].key + buffer[i-1].value, VECTOR2_SIZE); 
		buffer[i].key = buffer[i-1].key + buffer[i-1].value; 
	}
	return;
}

globalparams_TWOt loadedges::start(unsigned int col, vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, vptr_type * vptrs[MAXNUM_PEs], uint512_vec_dt * edges[MAXNUM_PEs], vector<edge2_vec_dt> (&edges_final)[MAXNUM_PEs], map_t * edgesmap_final[MAXNUM_PEs][MAXNUM_VPs], container_t * container, globalparams_TWOt globalparams){						
	cout<<"loadedges::start:: loading edges... "<<endl;
	
	unsigned int kvdata_range__div__vptr_shrink_ratio = (universalparams.KVDATA_RANGE + (universalparams.VPTR_SHRINK_RATIO-1)) / universalparams.VPTR_SHRINK_RATIO;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsK.SIZE_MESSAGESDRAM;
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsE.SIZE_MESSAGESDRAM; 
	
	unsigned int num_LLPs = 1 << (OPT_NUM_PARTITIONS_POW * universalparams.TREE_DEPTH);
	unsigned int vsize_LLP = 1 << (universalparams.BATCH_RANGE_POW - (OPT_NUM_PARTITIONS_POW * universalparams.TREE_DEPTH));
	unsigned int num_vPs = universalparams.NUMPROCESSEDGESPARTITIONS;
	unsigned int vsize_vP = universalparams.PROCESSPARTITIONSZ;
	
	unsigned int _NUM_PEs = NUM_PEs;
	unsigned int _num_vPs = num_vPs; // CRIICAL FIXME.
	unsigned int _num_LLPs = num_LLPs;
	
	vector<edge2_type> partitioned_edges_level1[MAXNUM_PEs];
	vector<edge2_type> partitioned_edges_level2[MAXNUM_VPs];
	vector<edge2_type> partitioned_edges_level3[num_LLPs];
	vector<edge2_type> partitioned_edges_level4[num_LLPs][OPT_NUM_PARTITIONS];
	unsigned int edge_count_in_vpartition[MAXNUM_PEs][MAXNUM_VPs];
	
	for(unsigned int i=0; i<_NUM_PEs; i++){ edges_final[i].clear(); }
	for(unsigned int i=0; i<_NUM_PEs; i++){ for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ edgesmap_final[i][v_p][ll_p].data = 0; edgesmap_final[i][v_p][ll_p].size = 0; }}}
	for(unsigned int i=0; i<_NUM_PEs; i++){ for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ edge_count_in_vpartition[i][v_p] = 0; }}
		
	cout<<"loadedges:: [num_LLPs: "<<num_LLPs<<", vsize_LLP: "<<vsize_LLP<<", num_vPs: "<<num_vPs<<", vsize_vP: "<<vsize_vP<<", INVALIDDATA: "<<INVALIDDATA<<"] "<<endl;
	
	// partiition into HBM channels 
	cout<<"STAGE 0: partiition into HBM channels "<<endl;
	for(unsigned int vid=0; vid<utilityobj->hmin(universalparams.NUM_VERTICES, universalparams.KVDATA_RANGE)-1; vid++){
		#ifdef _DEBUGMODE_HOSTCHECKS3
		if(vid % 1000000 == 0){ cout<<"### loadedges::start:: vid: "<<vid<<" (of "<<universalparams.NUM_VERTICES<<" vertices), vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		utilityobj->checkoutofbounds("loadedges::calculate counts_validedges_for_channel(19)::", vid, universalparams.KVDATA_RANGE, NAp, NAp, NAp);
		utilityobj->checkoutofbounds("loadedges::calculate counts_validedges_for_channel(19b)::", vid+1, universalparams.KVDATA_RANGE, NAp, NAp, NAp);
		#endif 
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ continue; cout<<">>> loadedges:: vptr_end("<<vptr_end<<") < vptr_begin("<<vptr_begin<<"). EXITING... (vid: "<<vid<<", edges_size: "<<edges_size<<")"<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int i=0; i<edges_size; i++){
			edge2_type edge = edgedatabuffer[vptr_begin + i];
			#ifdef _DEBUGMODE_HOSTCHECKS3
			if(edge.srcvid >= universalparams.KVDATA_RANGE || edge.dstvid >= universalparams.KVDATA_RANGE){ continue; } // edge.dstvid = edge.dstvid % universalparams.KVDATA_RANGE; } // CRIICAL FIXME.
			#endif 
			
			unsigned int H = gethash(edge.dstvid);
			partitioned_edges_level1[H].push_back(edge);
		}
	}
	
	bool debug = false;
	bool debug_detail = false;
	cout<<"STAGE 1: preparing edges..."<<endl;
	for(unsigned int i=0; i<_NUM_PEs; i++){
		cout<<"loadedges:: [PE: "<<i<<"]"<<endl;
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ partitioned_edges_level2[v_p].clear(); } // clear 
		
		// within a HBM channel, partition into v-partitions 
		if(debug){ cout<<"STAGE 1: within a HBM channel, partition into v-partitions "<<endl; }
		for(unsigned int t=0; t<partitioned_edges_level1[i].size(); t++){
			edge2_type edge = partitioned_edges_level1[i][t];
			unsigned int vP = (edge.srcvid / vsize_vP);
			#ifdef _DEBUGMODE_HOSTCHECKS3
			utilityobj->checkoutofbounds("loadedges::ERROR 22::", vP, num_vPs, edge.srcvid, edge.srcvid, vsize_vP);
			#endif 
		
			partitioned_edges_level2[vP].push_back(edge);
		}
		
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ partitioned_edges_level3[ll_p].clear(); } // clear
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){ for(unsigned int p=0; p<OPT_NUM_PARTITIONS; p++){ partitioned_edges_level4[ll_p][p].clear(); }} // clear
			
			edgesmap_final[i][v_p][0].head = partitioned_edges_level2[v_p][0];
	
			// within a v-partition, partition into last-level-partitions (LLPs)
			if(debug){ cout<<"STAGE 2: within a v-partition, partition into last-level-partitions (LLPs)"<<endl; }
			for(unsigned int t=0; t<partitioned_edges_level2[v_p].size(); t++){
				edge2_type edge = partitioned_edges_level2[v_p][t];
				unsigned int hdstvid = (edge.dstvid - i) / NUM_PEs;
				unsigned int ll_p = (hdstvid / vsize_LLP);
				#ifdef _DEBUGMODE_HOSTCHECKS3
				utilityobj->checkoutofbounds("loadedges::ERROR 23::", ll_p, num_LLPs, edge.dstvid, hdstvid, vsize_LLP);
				#endif 
			
				partitioned_edges_level3[ll_p].push_back(edge);
			}
			
			// witihin a LLP, re-arrange by srcvids 
			if(debug){ cout<<"STAGE 3: witihin a LLP, re-arrange by srcvids "<<endl; }
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){
				for(unsigned int t=0; t<partitioned_edges_level3[ll_p].size(); t++){
					edge2_type edge = partitioned_edges_level3[ll_p][t];
					partitioned_edges_level4[ll_p][edge.srcvid % OPT_NUM_PARTITIONS].push_back(edge);	
				}
			}
			
			// debug & check 
			if(debug){ cout<<"STAGE 3b: checking... "<<endl; }
			if(debug_detail){
				cout<<"loadedges:: [PE: "<<i<<", v_p: "<<v_p<<"] partitioned_edges_level2["<<i<<"]["<<v_p<<"] # elements: "<<partitioned_edges_level2[v_p].size()<<endl;
				for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){
					cout<<"loadedges:: [PE: "<<i<<", v_p: "<<v_p<<", ll_p: "<<ll_p<<"] partitioned_edges_level3["<<i<<"]["<<v_p<<"]["<<ll_p<<"] # elements: "<<partitioned_edges_level3[ll_p].size()<<endl;
					if(ll_p == 15){ for(unsigned int t=0; t<8; t++){ edge2_type edge = partitioned_edges_level3[ll_p][t]; cout<<"loadedges:: [PE: "<<i<<", v_p: "<<v_p<<", ll_p: "<<ll_p<<", t: "<<t<<"] edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", hdstvid: "<<(edge.dstvid - i) / NUM_PEs<<endl; }} // partitioned_edges_level3[ll_p].size()
					for(unsigned int p=0; p<OPT_NUM_PARTITIONS; p++){ cout<<"loadedges:: [PE: "<<i<<", v_p: "<<v_p<<", ll_p: "<<ll_p<<", p: "<<p<<"] partitioned_edges_level4["<<i<<"]["<<v_p<<"]["<<ll_p<<"]["<<p<<"] # elements: "<<partitioned_edges_level4[ll_p][p].size()<<endl; }
				}
			}
			
			// witihin a LLP, re-arrange by srcvids 
			if(debug){ cout<<"STAGE 4: preparing edges..."<<endl; }
			edge2_type dummy_edge; dummy_edge.srcvid = INVALIDDATA; dummy_edge.dstvid = INVALIDDATA; 
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){				
				unsigned int max = 0; 
				for(unsigned int p=0; p<OPT_NUM_PARTITIONS; p++){ if(partitioned_edges_level4[ll_p][p].size() > max){ max = partitioned_edges_level4[ll_p][p].size(); }}
				
				for(unsigned int t=0; t<max; t++){
					edge2_vec_dt edge_vec;
					for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(t < partitioned_edges_level4[ll_p][v].size()){ edge_vec.data[v] = partitioned_edges_level4[ll_p][v][t]; } else { edge_vec.data[v] = dummy_edge; }}
					edges_final[i].push_back(edge_vec); 
					
					edge2_vec_dt edge_vec2;
					for(unsigned int v=0; v<VECTOR_SIZE; v++){ if(t < partitioned_edges_level4[ll_p][VECTOR_SIZE + v].size()){ edge_vec2.data[v] = partitioned_edges_level4[ll_p][VECTOR_SIZE + v][t]; } else { edge_vec2.data[v] = dummy_edge; }}
					edges_final[i].push_back(edge_vec2); 
					
					edgesmap_final[i][v_p][ll_p].size += 2 * VECTOR_SIZE;
					edge_count_in_vpartition[i][v_p] += 2 * VECTOR_SIZE;
					
					if(true){
						if(edge_count_in_vpartition[i][v_p] >= 16 && edge_count_in_vpartition[i][v_p] < 32){ ///////////////////////
							cout<<"v_p: "<<v_p<<": "<<endl;
							// for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"{$"<<edge_vec.data[v].srcvid<<", #"<<edge_vec.data[v].dstvid<<"}, "; } cout<<endl;
							// for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"{$"<<edge_vec2.data[v].srcvid<<", #"<<edge_vec2.data[v].dstvid<<"}, "; } cout<<endl;
							for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"{$"<<edge_vec.data[v].srcvid<<", #"<<(edge_vec.data[v].dstvid - i) / NUM_PEs<<"}, "; } cout<<endl;
							for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"{$"<<edge_vec2.data[v].srcvid<<", #"<<(edge_vec2.data[v].dstvid - i) / NUM_PEs<<"}, "; } cout<<endl;
						}
					}
				}
			}
			
			if(debug_detail){ cout<<"loadedges:: edges_final[0].size(): "<<edges_final[0].size()<<", edge_count_in_vpartition["<<i<<"]["<<v_p<<"]: "<<edge_count_in_vpartition[i][v_p]<<endl; }
		}
	}
	
	// load edges into dram.
	cout<<"STAGE 2: loading edges into dram..."<<endl;
	for(unsigned int i=0; i<_NUM_PEs; i++){
		cout<<"loadedges:: [PE: "<<i<<"]"<<endl;
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		for(unsigned int t=0; t<edges_final[i].size(); t++){		
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				utilityobj->checkoutofbounds("loadedges::ERROR 56::", globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + t, universalparams.MAXHBMCAPACITY_KVS2 * VECTOR_SIZE, globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA, t, NAp);	

				if(edges_final[i][t].data[v].dstvid != INVALIDDATA){
					edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + t].data[v].key = (edges_final[i][t].data[v].dstvid - i) / NUM_PEs; // (edges_final[i][t].data[v].dstvid); // edges_final[i][t].data[v].dstvid;
					edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + t].data[v].value = (edges_final[i][t].data[v].srcvid - v) / OPT_NUM_PARTITIONS; // edges_final[i][t].data[v].srcvid;	
				} else {
					edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + t].data[v].key = INVALIDDATA; 
					edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + t].data[v].value = INVALIDDATA;
				}
			}
		}	
		if(debug2b==true){ utilityobj->printvalues("loadedges[after]::loadedges: printing edges["+std::to_string(i)+"][~]", (value_t *)&edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA], 8); }
	}
	
	// load vptrs into dram.
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0; 
	globalparams.globalparamsK.SIZE_EDGES = 0; 
	
	globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + (universalparams.AVERAGENUM_WORKEDGES_PER_CHANNEL / VECTOR_SIZE) + universalparams.DRAMPADD_KVS;
	globalparams.globalparamsE.SIZE_VERTEXPTRS = (kvdata_range__div__vptr_shrink_ratio) + universalparams.DRAMPADD_VPTRS; 
	globalparams.globalparamsE.SIZE_EDGES = (edges_final[0].size() * VECTOR_SIZE) + 1000; // average_num_edges_per_channel + 1000; // '1000' is padding 
	globalparams_t globalparamsVPTRS; globalparamsVPTRS = globalparams.globalparamsE; 
			
	cout<<"STAGE 3: loading vptrs into dram..."<<endl;
	unsigned int vptr_baseoffset = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR2_SIZE;
	for(unsigned int i=0; i<_NUM_PEs; i++){
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		vptrs[i][vptr_baseoffset + 0].key = 0;
		cout<<"loadedges:: vptrs["<<i<<"][vptr_baseoffset + 0].key: "<<vptrs[i][vptr_baseoffset + 0].key<<", count[0]: "<<0<<endl;
		for(unsigned int v_p=1; v_p<_num_vPs; v_p++){
			vptrs[i][vptr_baseoffset + v_p].key = vptrs[i][vptr_baseoffset + v_p - 1].key + edge_count_in_vpartition[i][v_p - 1]; // NEWCHANGE.
			if(i==0){ cout<<"loadedges:: vptrs["<<i<<"][vptr_baseoffset + "<<v_p<<"].key: "<<vptrs[i][vptr_baseoffset + v_p].key<<", vptrs["<<i<<"][vptr_baseoffset + "<<v_p<<"].key_kvs: "<<vptrs[i][vptr_baseoffset + v_p].key / VECTOR_SIZE<<", edge_count_in_vpartition["<<i<<"]["<<v_p<<"]: "<<edge_count_in_vpartition[i][v_p]<<endl; }
		}
		for(unsigned int v_p=_num_vPs; v_p<_num_vPs + universalparams.DRAMPADD_VPTRS; v_p++){ // dummy filling...
			vptrs[i][vptr_baseoffset + v_p].key = vptrs[i][vptr_baseoffset + _num_vPs - 1].key; 
		}
		
		// verifying...
		for(unsigned int index=vptrs[i][0].key; index<vptrs[i][1].key; index++){
			keyvalue_t kv = edges[i][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + (index / VECTOR_SIZE)].data[index % VECTOR_SIZE];
			utilityobj->checkoutofbounds("loadedges::ERROR 5611::", kv.key, universalparams.PROCESSPARTITIONSZ, kv.key, kv.value, index);	
		}
	}
	
	#ifdef _DEBUGMODE_KERNELPRINTS
	for(unsigned int v_p=0; v_p<4; v_p++){
		cout<<"STAGE X )))))))))))))))))))))))))))))) : v_p: "<<v_p<<endl;
		for(unsigned int t=0; t<4; t++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				cout<<"~~~ loadedges:: edges[0]["<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA<<" + "<<(vptrs[0][vptr_baseoffset + v_p].key / VECTOR_SIZE) + t<<"].data["<<v<<"].key: "<<edges[0][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + (vptrs[0][vptr_baseoffset + v_p].key / VECTOR_SIZE) + t].data[v].key<<", edges[0]["<<globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA<<" + "<<(vptrs[0][vptr_baseoffset + v_p].key / VECTOR_SIZE) + t<<"].data["<<v<<"].value: "<<edges[0][globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + (vptrs[0][vptr_baseoffset + v_p].key / VECTOR_SIZE) + t].data[v].value<<endl;								 	
			}
		}			
	}
	#endif 
	
	// load container
	cout<<"STAGE 4: loading container..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = utilityobj->allignhigherto16_KV(universalparams.NUM_VERTICES);
		container->edgessize[i] = edges_final[0].size() * VECTOR_SIZE;
		container->runsize[i] = edges_final[0].size() * VECTOR_SIZE;
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
	}
	
	// checking...
	cout<<"STAGE 5: checking final stats..."<<endl;
	unsigned int total_edge_count = 0;
	for(unsigned int i=0; i<_NUM_PEs; i++){ 
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		for(unsigned int v_p=0; v_p<_num_vPs; v_p++){ 
			for(unsigned int ll_p=0; ll_p<_num_LLPs; ll_p++){
				total_edge_count += edgesmap_final[i][v_p][ll_p].size;
				if(i==0 && v_p==0){ cout<<"loadedges:: [PE: "<<i<<", v_p: "<<v_p<<", ll_p: "<<ll_p<<"]: edgesmap_final["<<i<<"]["<<v_p<<"]["<<ll_p<<"].size: "<<edgesmap_final[i][v_p][ll_p].size<<endl; }
			}
		}
	}
	
	cout<<"STAGE 6: checking final edges ..."<<endl;
	for(unsigned int i=0; i<1; i++){ // _NUM_PEs
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		for(unsigned int t=0; t<16; t++){ 
			for(unsigned int v=0; v<VECTOR_SIZE; v++){ cout<<"{"<<edges_final[i][t].data[v].srcvid<<", "<<(edges_final[i][t].data[v].dstvid - i) / NUM_PEs<<"}, "; } cout<<endl;
		}
	}		
	unsigned int total1, total2 = 0;
	for(unsigned int i=0; i<_NUM_PEs; i++){ total1 += edges_final[i].size() * VECTOR_SIZE; total2 += partitioned_edges_level1[i].size(); cout<<"STAGE X: checking finished: edges_final["<<i<<"].size(): "<<edges_final[i].size() * VECTOR_SIZE<<endl; }
	cout<<"STAGE Y: checking finished: total_edge_count: "<<total_edge_count<<endl;
	// exit(EXIT_SUCCESS);
	return globalparams;
}






