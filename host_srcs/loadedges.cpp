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

globalparams_TWOt loadedges::start(unsigned int col, vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, vptr_type * vptrs[MAXNUMSUBCPUTHREADS], edge_type * edges[MAXNUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[MAXNUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){						
	cout<<"loadedges::start:: loading edges... "<<endl;
	
	unsigned int kvdata_range__div__vptr_shrink_ratio = (universalparams.KVDATA_RANGE + (universalparams.VPTR_SHRINK_RATIO-1)) / universalparams.VPTR_SHRINK_RATIO;
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsK.SIZE_MESSAGESDRAM;
	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){ globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsE.SIZE_MESSAGESDRAM; } 
	
	unsigned int baseoffset_edgesdata = 0;
	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){ baseoffset_edgesdata = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE; } 
	else { baseoffset_edgesdata = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE; }

	vector<edge2_type> edgedatabuffers_temp[MAXNUM_PEs];
	#ifdef CHECK4_VERIFYOFFSETS
	vptr_type * tempvptrs[MAXNUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ tempvptrs[i] = new vptr_type[universalparams.KVDATA_RANGE]; }
	#endif 
	vector<edge3_type> edges2_temp[MAXNUM_PEs];
	for(unsigned int j=0; j<NUM_PEs; j++){ edges_temp[j].clear(); }
	for(unsigned int j=0; j<NUM_PEs; j++){ edges2_temp[j].clear(); }
	
	unsigned int * counts_validedges_for_channel[MAXNUM_PEs];
	unsigned int * counts_alledges_for_channel[MAXNUM_PEs];
	unsigned int * counts_alledges_for_vpartition[MAXNUM_PEs];
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		counts_validedges_for_channel[i] = new unsigned int[universalparams.KVDATA_RANGE];
		counts_alledges_for_channel[i] = new unsigned int[universalparams.KVDATA_RANGE];
		counts_alledges_for_vpartition[i] = new unsigned int[kvdata_range__div__vptr_shrink_ratio];
		for(unsigned int vid=0; vid<universalparams.KVDATA_RANGE; vid++){ counts_validedges_for_channel[i][vid] = 0; counts_alledges_for_channel[i][vid] = 0; }
		for(unsigned int v_partition=0; v_partition<kvdata_range__div__vptr_shrink_ratio; v_partition++){ counts_alledges_for_vpartition[i][v_partition] = 0; }
	}
	unsigned int counts_totalvalidedges_for_channel[MAXNUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_totalvalidedges_for_channel[i] = 0; }
	unsigned int counts_totalalledges_for_channel[MAXNUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_totalalledges_for_channel[i] = 0; }
	unsigned int totalcount_validedges_for_all_channels = 0;
	unsigned int totalcount_alledges_for_all_channels = 0;
	unsigned int isFirst = 0;
	unsigned int isLast = 0;
	unsigned int _ACTS_READEDGEGRANULARITY = (universalparams.WORKBUFFER_SIZE * VECTOR2_SIZE) / 2; // this is the granularity with which edges are retrieved from memory 
	// exit(EXIT_SUCCESS);
	
	// calculate counts_validedges_for_channel
	unsigned int errcount = 0;
	unsigned int num_zeros = 0;
	unsigned int total_num_zeros = 0;
	unsigned int * edgecount[MAXNUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ edgecount[i] = new unsigned int[universalparams.KVDATA_RANGE]; }
	unsigned int * edgecount_vpartition[MAXNUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ edgecount_vpartition[i] = new unsigned int[kvdata_range__div__vptr_shrink_ratio]; }
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<kvdata_range__div__vptr_shrink_ratio; t++){ edgecount_vpartition[i][t] = 0; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<universalparams.KVDATA_RANGE; t++){ edgecount[i][t] = 0; }}
	cout<<"loadedges::start:: printing variables... kvdata_range__div__vptr_shrink_ratio: "<<kvdata_range__div__vptr_shrink_ratio<<endl;
	
	for(unsigned int vid=0; vid<utilityobj->hmin(universalparams.NUM_VERTICES, universalparams.KVDATA_RANGE)-1; vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		if(vid % 1000000 == 0){ cout<<"### loadedges::start:: vid: "<<vid<<" (of "<<universalparams.NUM_VERTICES<<" vertices), vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		#endif 
		
		utilityobj->checkoutofbounds("loadedges::calculate counts_validedges_for_channel(19)::", vid, universalparams.KVDATA_RANGE, NAp, NAp, NAp);
		utilityobj->checkoutofbounds("loadedges::calculate counts_validedges_for_channel(19b)::", vid+1, universalparams.KVDATA_RANGE, NAp, NAp, NAp);
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		if(vptr_end < vptr_begin){ continue; cout<<">>> loadedges:: vptr_end("<<vptr_end<<") < vptr_begin("<<vptr_begin<<"). EXITING... (vid: "<<vid<<", edges_size: "<<edges_size<<")"<<endl; exit(EXIT_FAILURE); }
		// if(edges_size > 100000){ cout<<">>> loadedges:: edges_size("<<edges_size<<") > 10000. EXITING... (vid: "<<vid<<", edges_size: "<<edges_size<<")"<<endl; exit(EXIT_FAILURE); }
		
		for(unsigned int i=0; i<edges_size; i++){
			edge2_type edge = edgedatabuffer[vptr_begin + i];
			
			#ifdef _DEBUGMODE_HOSTCHECKS3
			if(edge.dstvid == 0 && edge.srcvid == 0){ total_num_zeros += 1; }
			if(edge.srcvid >= universalparams.KVDATA_RANGE || edge.dstvid >= universalparams.KVDATA_RANGE){ continue; } // edge.dstvid = edge.dstvid % universalparams.KVDATA_RANGE; } // CRIICAL FIXME.
			#endif 
			
			unsigned int H = gethash(edge.dstvid);
			
			#ifdef TESTKERNEL // ignore unused loads when testing
			if(utilityobj->isbufferused(H) == false){ continue; }
			#endif 
			
			utilityobj->checkoutofbounds("loadedges::calculate counts_validedges_for_channel(20)::", H, NUM_PEs, NAp, NAp, NAp);
			utilityobj->checkoutofbounds("loadedges::calculate counts_validedges_for_channel(21)::", edge.srcvid, universalparams.KVDATA_RANGE, NAp, NAp, NAp);
			edgedatabuffers_temp[H].push_back(edge);
			
			edgecount[H][edge.srcvid] += 1;
		}
	}
	unsigned int v_partition = 0;
	unsigned int total_num_edges = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		v_partition = 0;
		total_num_edges = 0;
		for(unsigned int vid=1; vid<universalparams.KVDATA_RANGE; vid++){
			#ifdef _DEBUGMODE_HOSTPRINTS3
			if(v_partition >= kvdata_range__div__vptr_shrink_ratio){ cout<<"v_partition("<<v_partition<<") > universalparams.KVDATA_RANGE("<<universalparams.KVDATA_RANGE<<") / universalparams.VPTR_SHRINK_RATIO("<<universalparams.VPTR_SHRINK_RATIO<<") := ["<<kvdata_range__div__vptr_shrink_ratio<<"]. universalparams.VPTR_SHRINK_RATIO: "<<universalparams.VPTR_SHRINK_RATIO<<". EXITING..."<<endl; exit(EXIT_FAILURE); }
			#endif
			edgecount_vpartition[i][v_partition] += edgecount[i][vid-1]; // track number of edges in each source partition
			total_num_edges += edgecount[i][vid-1];
			if(vid % universalparams.VPTR_SHRINK_RATIO == 0){ v_partition += 1; } // cout<<"---+++- v_partition: "<<v_partition<<", vid: "<<vid<<endl;
		}
		if(false){ cout<<">>> loadedges:: : "<<i<<": total_num_edges: "<<total_num_edges<<endl; }
	}
	for(unsigned int i=0; i<NUM_PEs; i++){ delete edgecount[i]; } // kvdata_range__div__vptr_shrink_ratio
	for(unsigned int i=0; i<NUM_PEs; i++){ cout<<">>> loadedges:: edgedatabuffers_temp["<<i<<"].size(): "<<edgedatabuffers_temp[i].size()<<endl; }
	for(unsigned int i=0; i<1; i++){ for(unsigned int k=0; k<0; k++){ cout<<">>> loadedges:: edgecount_vpartition["<<i<<"]["<<k<<"]: "<<edgecount_vpartition[i][k]<<", v_partition: "<<v_partition<<endl; }}
	cout<<"loadedges:: total_num_zeros: "<<total_num_zeros<<", universalparams.NUM_EDGES: "<<universalparams.NUM_EDGES<<", universalparams.VPTR_SHRINK_RATIO: "<<universalparams.VPTR_SHRINK_RATIO<<endl;
	if(total_num_zeros > 1000000){ cout<<">>> loadedges:: TOO MANY ZEROS. EXITING... (total_num_zeros: "<<total_num_zeros<<", num_vertices: "<<universalparams.NUM_VERTICES<<")"<<endl; exit(EXIT_FAILURE); }
	// exit(EXIT_SUCCESS);
	
	// load edges into memory channels according to edge representation format
	unsigned int counts_alldatas[MAXNUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_alldatas[i] = 0; }
	unsigned int counts_alldata = 0;
	#ifdef LOADSLICEDEDGES_CORRECT
	cout<<"### loadedges::start:: loading edges into memory channels according to edge representation format..."<<endl;
	unsigned int tempe_index = 0; 
	unsigned int tempe_vpartition_index = 0;
	unsigned int index = 0;
	unsigned int srcvid_lastvechead = 0xFFFFFFFF;
	unsigned int srcvid_lastseen = 0;
	unsigned int numskippededges = 0;
	keyvalue_t block_partitions[universalparams.NUM_PARTITIONS];
	unsigned int numcheckpoints = 0;
	
	edge2_type edgeblock[_ACTS_READEDGEGRANULARITY];
	cout<<endl<<">>> loadedges::start:: loading edges into PE: _ACTS_READEDGEGRANULARITY: "<<_ACTS_READEDGEGRANULARITY<<endl;
	
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int t=0; t<0; t++){ cout<<"sample edges:: edgedatabuffers_temp["<<i<<"]["<<t<<"].srcvid: "<<edgedatabuffers_temp[i][t].srcvid<<", edgedatabuffers_temp["<<i<<"]["<<t<<"].dstvid: "<<edgedatabuffers_temp[i][t].dstvid<<endl; }}
	for(unsigned int i=0; i<NUM_PEs; i++){ // NUM_PEs // CRIICAL FIXME.
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		
		cout<<endl<<">>>  loadedges::start:: loading edges into PE: "<<i<<", edgedatabuffers_temp["<<i<<"].size(): "<<edgedatabuffers_temp[i].size()<<"..."<<endl;
		tempe_index = 0;
		index = 0;
		srcvid_lastvechead = 0xFFFFFFFF; 
		unsigned int edgeblockid = 0;
		edge2_type edge;
		edge2_type edge2;
		
		for(unsigned int v_partition=0; v_partition<kvdata_range__div__vptr_shrink_ratio; v_partition++){
			#ifdef _DEBUGMODE_HOSTPRINTS3
			if(v_partition==0){ cout<<"$$$ VERTEX PARTITION LOOP: loadedges:: i: "<<i<<", v_partition: "<<v_partition<<" (of "<<kvdata_range__div__vptr_shrink_ratio<<"), num edges in v_partition: "<<edgecount_vpartition[i][v_partition]<<", num edge blocks: "<<(edgecount_vpartition[i][v_partition] + (_ACTS_READEDGEGRANULARITY-1)) / _ACTS_READEDGEGRANULARITY<<", vid: "<<v_partition*(kvdata_range__div__vptr_shrink_ratio)<<", edges_temp["<<i<<"].size(): "<<edges_temp[i].size()<<" (%16="<<edges_temp[i].size() % 16<<")"<<endl; }				
			#endif 
			tempe_vpartition_index = 0;
			
			for(unsigned int eid_offset=0; eid_offset<(edgecount_vpartition[i][v_partition] + (_ACTS_READEDGEGRANULARITY-1)) / _ACTS_READEDGEGRANULARITY; eid_offset++){
				unsigned int chunk_size = utilityobj->hmin(_ACTS_READEDGEGRANULARITY, edgecount_vpartition[i][v_partition] - tempe_vpartition_index);
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"=== EDGE BLOCK LOOP: loadedges:: i: "<<i<<", v_partition: "<<v_partition<<" (of "<<kvdata_range__div__vptr_shrink_ratio<<")"<<", eid_offset: "<<eid_offset<<" (of "<<(edgecount_vpartition[i][v_partition] + (_ACTS_READEDGEGRANULARITY-1)) / _ACTS_READEDGEGRANULARITY<<"), edgecount_vpartition["<<i<<"]["<<v_partition<<"]: "<<edgecount_vpartition[i][v_partition]<<", tempe_index: "<<tempe_index<<", chunk_size: "<<chunk_size<<endl;
				// cout<<"=== loadedges:: i: "<<i<<", counts_alledges_for_vpartition["<<i<<"]["<<v_partition<<"]: "<<counts_alledges_for_vpartition[i][v_partition]<<endl;
				#endif
				
				edge2_type firstedgeinblock;
				unsigned int srcvtxoffsetofblock;
				
				if(debug2b==true){ for(unsigned int t=0; t<chunk_size; t++){ edgeblock[t] = edgedatabuffers_temp[i][tempe_index + t]; }}
				if(debug2b==true){ for(unsigned int t=0; t<4; t++){ cout<<"loadedges[BEFORE]:: edgeblock["<<t<<"].srcvid: "<<edgeblock[t].srcvid<<", edgeblock["<<t<<"].dstvid: "<<edgeblock[t].dstvid<<endl; }}
				
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"loadedges:: CHECKPOINT SEEN: tempe_index: "<<tempe_index<<", edgedatabuffers_temp["<<i<<"].size(): "<<edgedatabuffers_temp[i].size()<<", srcvid: "<<edgedatabuffers_temp[i][tempe_index].srcvid<<", _ACTS_READEDGEGRANULARITY: "<<_ACTS_READEDGEGRANULARITY<<endl;
				#endif
				if(tempe_index >= edgedatabuffers_temp[i].size()){ cout<<"loadedges::getpartition(112)::. tempe_index("<<tempe_index<<") >= edgedatabuffers_temp[i].size()("<<edgedatabuffers_temp[i].size()<<"), NAp: "<<NAp<<", NAp: "<<NAp<<", NAp: "<<NAp<<", NAp: "<<NAp<<", NAp: "<<NAp<<", NAp: "<<NAp<<", NAp: "<<NAp<<". EXITING... "<<endl; exit(EXIT_FAILURE); }
					
				firstedgeinblock = edgedatabuffers_temp[i][tempe_index];
				srcvtxoffsetofblock = allignlower(firstedgeinblock.srcvid, universalparams.PROCESSPARTITIONSZ);
				if(debug2b==true){ cout<<"loadedges:: srcvtxoffsetofblock: "<<srcvtxoffsetofblock<<", firstedgeinblock.srcvid: "<<firstedgeinblock.srcvid<<endl; }
				
				for(unsigned int p=0; p<universalparams.NUM_PARTITIONS; p++){ block_partitions[p].key = 0; block_partitions[p].value = 0; }
				for(unsigned int yy=0; yy<2; yy++){
					// if(yy==0){ for(unsigned int p=0; p<universalparams.NUM_PARTITIONS; p++){ block_partitions[p].key = 0; block_partitions[p].value = 0; }}
					for(unsigned int t=0; t<chunk_size; t++){
						edge2_type thisedge = edgedatabuffers_temp[i][tempe_index + t];
						if(gethash(thisedge.dstvid) != i){ continue; } // throw away
						if(i==3 && (thisedge.dstvid==0)){ cout<<"~~~~~ BREAKING 234 loadedges ^%^**%$#@. t: "<<t<<", thisedge.srcvid: "<<thisedge.srcvid<<", thisedge.dstvid: "<<thisedge.dstvid<<", edgedatabuffers_temp["<<i<<"].size(): "<<edgedatabuffers_temp[i].size()<<", tempe_index("<<tempe_index<<") + t("<<t<<"): "<<tempe_index + t<<", _ACTS_READEDGEGRANULARITY: "<<_ACTS_READEDGEGRANULARITY<<endl; exit(EXIT_FAILURE); }
						tuple_t _p = get_partition_and_incr(thisedge.srcvid, NAp);
						if(yy==1){ edgeblock[block_partitions[_p.A].key + block_partitions[_p.A].value] = thisedge; }
						block_partitions[_p.A].value += 1;						
						if(yy==1){ 
							if(_p.A < universalparams.NUM_PARTITIONS-1){ if(block_partitions[_p.A].key + block_partitions[_p.A].value > block_partitions[_p.A+1].key){ 
								cout<<"loadedges::contents:: ERROR 3435, block_partitions["<<_p.A<<"].key("<<block_partitions[_p.A].key<<") + block_partitions["<<_p.A<<"].value("<<block_partitions[_p.A].value<<") >= block_partitions["<<_p.A+1<<"].key("<<block_partitions[_p.A+1].key<<"). EXITING..."<<endl; 
								for(unsigned int pt=0; pt<universalparams.NUM_PARTITIONS; pt++){ cout<<"loadedges:: block_partitions["<<pt<<"].key: "<<block_partitions[pt].key<<", block_partitions["<<pt<<"].value: "<<block_partitions[pt].value<<endl; }
								exit(EXIT_FAILURE); }}
						}							
					}
					if(yy==0){ calculateoffsets(block_partitions, universalparams.NUM_PARTITIONS); }
					if(debug2b==true){ 
					for(unsigned int _p=0; _p<universalparams.NUM_PARTITIONS; _p++){ cout<<"loadedges:: block_partitions["<<_p<<"].key: "<<block_partitions[_p].key<<", block_partitions["<<_p<<"].value: "<<block_partitions[_p].value<<endl; }
					}
					
					// load capsule stats 
					#ifdef CONFIG_INSERTSTATSMETADATAINEDGES
					if(yy==0){ 
						for(unsigned int p=0; p<universalparams.NUM_PARTITIONS; p++){
							edge3_type edge_temp; edge_temp.srcvid = 0; edge_temp.dstvid = block_partitions[p].value; edge_temp.status = EDGESTATUS_INVALIDEDGE; edge_temp.metadata = 0;
							if(p==universalparams.NUM_PARTITIONS-1){ edge_temp.dstvid = 8888888; } // JUST FOR DEBUGGING. REMOVEME.
							edges_temp[i].push_back(edge_temp); // CAUSE OF ERROR.
							edges2_temp[i].push_back(edge_temp);
							#ifdef _DEBUGMODE_HOSTPRINTS
							if(eid_offset==0 && p==universalparams.NUM_PARTITIONS-1){ cout<<"loadedges: 16th INVALID edge in v_partition and edgeblock: srcvid: "<<edge_temp.srcvid<<", dstvid: "<<edge_temp.dstvid<<", edges_temp["<<i<<"].size(): "<<edges_temp[i].size()<<endl; }
							#endif 
							counts_alledges_for_channel[i][firstedgeinblock.srcvid] += 1;
							counts_alledges_for_vpartition[i][v_partition] += 1;
						}
					}
					#endif 
					
					if(yy==0){ for(unsigned int p=0; p<universalparams.NUM_PARTITIONS; p++){ block_partitions[p].value = 0; }}
				}
				
				// insert edges into buffer...
				for(unsigned int t=0; t<chunk_size; t++){
					edge = edgeblock[t]; 
					edge2 = edgeblock[t];
					
					utilityobj->checkoutofbounds("loadedges(22)::", edge.dstvid, universalparams.NUM_VERTICES+10, edge.srcvid, edge.dstvid, universalparams.NUM_VERTICES);
					if(debug2b==true){ cout<<">>> edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<", edge2.srcvid: "<<edge2.srcvid<<", edge2.dstvid: "<<edge2.dstvid<<" [-]"<<endl; }
					if(debug2b==true){ cout<<">>> edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<" [-]"<<endl; }
					
					// transform srcvid to pick its data in acts.vbuffer accurately
					tuple_t _p = get_partition_and_incr(edge2.srcvid, edge2.dstvid);
					unsigned int partition = _p.A; 
					unsigned int incr = _p.B;
					utilityobj->checkoutofbounds("loadedges::(202)::", incr, (1 << SIZEOF_DSTV_IN_EDGEDSTVDATA), partition, edge2.srcvid, edge2.dstvid); ///////////
					#ifdef _DEBUGMODE_HOSTPRINTS
					if(incr==42 && partition == 1 && edge2.srcvid < 64){ cout<< TIMINGRESULTSCOLOR << "loadedges:: incr==42: incr: "<<incr<<", partition: "<<partition<<" [edge2.srcvid: "<<edge2.srcvid<<", edge2.dstvid: "<<edge2.dstvid<<"]"<< RESET << endl; }
					#endif 	
					edge2.srcvid = incr;
					utilityobj->checkoutofbounds("loadedges(24)::", edge2.srcvid, universalparams.PROCESSPARTITIONSZ_KVS2, srcvtxoffsetofblock, chunk_size, firstedgeinblock.srcvid);
					
					// insert edge
					edge3_type edge_temp; edge_temp.srcvid = edge.srcvid; edge_temp.dstvid = edge.dstvid; edge_temp.status = EDGESTATUS_VALIDEDGE; edge_temp.metadata = 0; edges_temp[i].push_back(edge_temp);
					edge3_type edge2_temp; edge2_temp.srcvid = edge2.srcvid; edge2_temp.dstvid = edge2.dstvid; edge2_temp.status = EDGESTATUS_VALIDEDGE; edge2_temp.metadata = 0, edges2_temp[i].push_back(edge2_temp);
					utilityobj->checkoutofbounds("loadedges::(203)::", ((edge2_temp.dstvid - i) / NUM_PEs), (1 << SIZEOF_DSTV_IN_EDGEDSTVDATA), partition, edge2.srcvid, edge2.dstvid); ///////////
					
					#ifdef _DEBUGMODE_CHECKS
					if(i==2 && partition==1 && edge2.srcvid==42 && edge2.dstvid<1000){ cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ loadedges: i: "<<i<<", edge2.srcvid: "<<edge2.srcvid<<", edge2.dstvid: "<<edge2.dstvid<<", ldstvid: "<<(edge2.dstvid - i) / NUM_PEs<<", partition: "<<partition<<", edges2_temp["<<i<<"].size(): "<<edges2_temp[i].size()<<endl; }
					#endif 
					
					utilityobj->checkoutofbounds("loadedges(25)::", edge_temp.dstvid, universalparams.NUM_VERTICES+10, edge.srcvid, edge.dstvid, universalparams.NUM_VERTICES);
					utilityobj->checkoutofbounds("loadedges(26)::", edge2_temp.dstvid, universalparams.NUM_VERTICES+10, edge2.srcvid, edge2.dstvid, universalparams.NUM_VERTICES);
					if(debugb==true){ cout<<">>> edge_temp.srcvid: "<<edge_temp.srcvid<<", edge_temp.dstvid: "<<edge_temp.dstvid<<" [3]"<<endl; }
					tempe_index += 1;
					tempe_vpartition_index += 1;
					index += 1;
					counts_validedges_for_channel[i][edge_temp.srcvid] += 1;
					counts_alledges_for_channel[i][edge_temp.srcvid] += 1;
					counts_alledges_for_vpartition[i][v_partition] += 1;
				}
				
				if(tempe_index % 1000000 == 0 && false){ cout<<"loadedges::start:: filling edges... index: "<<index<<", tempe_index: "<<tempe_index<<endl; }
				numcheckpoints += 1;
				edgeblockid += 1;
				// exit(EXIT_SUCCESS);
			} // end of edge block loop
			
			// padding 
			unsigned int sz = edges_temp[i].size();
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"--- loadedges: [BEFORE PADDING]: edge.srcvid: "<<edge.srcvid<<", counts_alledges_for_vpartition["<<i<<"]["<<v_partition<<"]: "<<counts_alledges_for_vpartition[i][v_partition]<<" (%16="<<counts_alledges_for_vpartition[i][v_partition] % 16<<")"<<endl;	
			#endif 
			for(unsigned int t=0; t<utilityobj->allignhigherto16_KV(sz) - sz; t++){
				edge3_type edge_temp; edge_temp.srcvid = 0; edge_temp.dstvid = INVALIDDATA; edge_temp.status = EDGESTATUS_INVALIDEDGE; edge_temp.metadata = 0; // edge.srcvid
				edges_temp[i].push_back(edge_temp);
				edges2_temp[i].push_back(edge_temp); 
				counts_alledges_for_channel[i][edge.srcvid] += 1;
				counts_alledges_for_vpartition[i][v_partition] += 1;
			}
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"--- loadedges: [AFTER PADDING]: edge.srcvid: "<<edge.srcvid<<", counts_alledges_for_vpartition["<<i<<"]["<<v_partition<<"]: "<<counts_alledges_for_vpartition[i][v_partition]<<" (%16="<<counts_alledges_for_vpartition[i][v_partition] % 16<<")"<<endl;	
			#endif
			// exit(EXIT_SUCCESS);
		} // end of vertex partition loop
		
		counts_alldatas[i] += index;
		counts_alldata += index;
		
		#ifdef _DEBUGMODE_HOSTPRINTS3
		utilityobj->printtriples("loadedges::[insert.edges] printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 4);
		cout<<"### edges_temp["<<i<<"].size(): "<<edges_temp[i].size()<<", edges2_temp["<<i<<"].size(): "<<edges2_temp[i].size()<<endl;
		cout<<"### loadedges::[insert.edges] memory channel "<<i<<": tempe_index: "<<tempe_index<<", index: "<<index<<endl;
		#endif
		// exit(EXIT_SUCCESS);
	} // end of PEs loop
	#ifdef _DEBUGMODE_HOSTPRINTS3
	utilityobj->printvalues(">>> loadedges:[insert.edges] total number of edges in channels[0-N]", (value_t *)&counts_alldatas[0], NUM_PEs);
	cout<<">>> loadedges::[insert.edges] total number of edges in all memory channels: counts_alldata: "<<counts_alldata<<", NAp: "<<NAp<<endl;
	#endif 
	cout<<"### loadedges::[insert.edges] total number of skipped edges: "<<numskippededges<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	
	// calculate local edge dstvids 
	#ifdef CALCULATELOCALDSTVIDS
	cout<<"### loadedges:: calculating local edge dstvids..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"### loadedges:: calculating local edge dstvids for instance "<<i<<": (edges_temp["<<i<<"].size(): "<<edges_temp[i].size()<<", edges2_temp["<<i<<"].size(): "<<edges2_temp[i].size()<<")"<<endl;
		#endif 
		for(unsigned int k=0; k<edges_temp[i].size(); k++){
			if(edges_temp[i][k].status == EDGESTATUS_VALIDEDGE){ 
				#ifdef _DEBUGMODE_HOSTCHECKS3
				if(edges_temp[i][k].dstvid < i){ cout<<"loadedges.calculate local dstvids: ERROR 64. i:"<<i<<", k:"<<k<<", edges_temp["<<i<<"]["<<k<<"].dstvid("<<edges_temp[i][k].dstvid<<"          ---------      edges2_temp["<<i<<"]["<<k<<"].dstvid("<<edges2_temp[i][k].dstvid<<")          -----------------   ) < i("<<i<<"), edges_temp["<<i<<"].size(): "<<edges_temp[i].size()<<", EXITING..."<<endl; exit(EXIT_FAILURE); }
				if(edges2_temp[i][k].dstvid < i){ cout<<"loadedges.calculate local dstvids: ERROR 65. i:"<<i<<", k:"<<k<<", edges2_temp["<<i<<"]["<<k<<"].dstvid("<<edges2_temp[i][k].dstvid<<") < i("<<i<<"), edges2_temp["<<i<<"].size(): "<<edges2_temp[i].size()<<", EXITING..."<<endl; exit(EXIT_FAILURE); }			
				#endif 
				
				edges_temp[i][k].dstvid = (edges_temp[i][k].dstvid - i) / NUM_PEs; 
				edges2_temp[i][k].dstvid = (edges2_temp[i][k].dstvid - i) / NUM_PEs; 
			}
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printtriples("loadedges::[insert.local.edge.dstvids] printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8);
		#endif 
	}
	cout<<"### loadedges::[insert.local.edge.dstvids] calculating local edge dstvids inserted successfully"<<endl;
	#endif 

	// insert bitmap 
	#ifdef INSERTBITMAP
	cout<<"### loadedges::insert.bitmap:: inserting bitmap..."<<endl;
	unsigned int num_errors = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ // NUM_PEs
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		
		if(debug2b==true){ utilityobj->printtriples("loadedges::insert.bitmap::[before.insert.bitmap]: printing edges2_temp["+std::to_string(i)+"][~]", (triple_t *)&edges2_temp[i][0], 8); }
		for(unsigned int j=0; j<edges2_temp[i].size(); j+=VECTOR2_SIZE){
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){
				if(j+v >= edges2_temp[i].size()){ continue; }
				if(edges2_temp[i][j+v].status != EDGESTATUS_VALIDEDGE){ continue; }
				
				// #ifndef LOADEDGES_ENSUREPERFECTACCURACY
				if(edges2_temp[i][j+v].srcvid >= (1 << SIZEOF_SRCV_IN_EDGEDSTVDATA)){ num_errors += 1; }
				if(edges2_temp[i][j+v].dstvid >= (1 << SIZEOF_DSTV_IN_EDGEDSTVDATA)){ num_errors += 1; }
				// #endif 
				
				#ifdef _DEBUGMODE_HOSTCHECKS3
				if(edges2_temp[i][j+v].srcvid >= (1 << SIZEOF_SRCV_IN_EDGEDSTVDATA)){ cout<<"loadedges.insertbitmap: ERROR 65. i:"<<i<<", j:"<<j<<", v:"<<v<<", edges2_temp["<<i<<"]["<<j+v<<"].srcvid("<<edges2_temp[i][j+v].srcvid<<") >= (1 << SIZEOF_SRCV_IN_EDGEDSTVDATA)("<<(1 << SIZEOF_SRCV_IN_EDGEDSTVDATA)<<"). edges2_temp["<<i<<"]["<<j+v<<"].dstvid: "<<edges2_temp[i][j+v].dstvid<<", edges2_temp["<<i<<"]["<<j+v<<"].status: "<<edges2_temp[i][j+v].status<<", edges2_temp["<<i<<"].size(): "<<edges2_temp[i].size()<<", EXITING..."<<endl; 
					for(unsigned int t=0; t<8; t++){ cout<<"loadedges.insertbitmap: edges2_temp["<<i<<"]["<<j+t<<"].srcvid: "<<edges2_temp[i][j+t].srcvid<<", edges2_temp["<<i<<"]["<<j+t<<"].dstvid: "<<edges2_temp[i][j+t].dstvid<<", edges2_temp["<<i<<"]["<<j+t<<"].status: "<<edges2_temp[i][j+t].status<<endl; }
					exit(EXIT_FAILURE); }
				if(edges2_temp[i][j+v].dstvid >= (1 << SIZEOF_DSTV_IN_EDGEDSTVDATA)){ cout<<"loadedges.insertbitmap: ERROR 66. i:"<<i<<", j:"<<j<<", v:"<<v<<", edges2_temp["<<i<<"]["<<j+v<<"].dstvid("<<edges2_temp[i][j+v].dstvid<<") >= (1 << SIZEOF_DSTV_IN_EDGEDSTVDATA)("<<(1 << SIZEOF_DSTV_IN_EDGEDSTVDATA)<<"). edges2_temp["<<i<<"]["<<j+v<<"].dstvid: "<<edges2_temp[i][j+v].dstvid<<", edges2_temp["<<i<<"]["<<j+v<<"].status: "<<edges2_temp[i][j+v].status<<", edges2_temp["<<i<<"].size(): "<<edges2_temp[i].size()<<", EXITING..."<<endl; 
					for(unsigned int t=0; t<8; t++){ cout<<"loadedges.insertbitmap: edges2_temp["<<i<<"]["<<j+t<<"].srcvid: "<<edges2_temp[i][j+t].srcvid<<", edges2_temp["<<i<<"]["<<j+t<<"].dstvid: "<<edges2_temp[i][j+t].dstvid<<", edges2_temp["<<i<<"]["<<j+t<<"].status: "<<edges2_temp[i][j+t].status<<endl; }
					exit(EXIT_FAILURE); }				
				#endif 
				
				unsigned int codededge = 0;
				WRITETO_UINT(&codededge, OFFSETOF_SRCV_IN_EDGEDSTVDATA, SIZEOF_SRCV_IN_EDGEDSTVDATA, edges2_temp[i][j+v].srcvid);
				WRITETO_UINT(&codededge, OFFSETOF_DSTV_IN_EDGEDSTVDATA, SIZEOF_DSTV_IN_EDGEDSTVDATA, edges2_temp[i][j+v].dstvid);
				
				// debug
				#ifdef _DEBUGMODE_CHECKS
				parsededge_t parsededge;
				parsededge.incr = READFROM_UINT(codededge, OFFSETOF_SRCV_IN_EDGEDSTVDATA, SIZEOF_SRCV_IN_EDGEDSTVDATA);
				parsededge.dstvid = READFROM_UINT(codededge, OFFSETOF_DSTV_IN_EDGEDSTVDATA, SIZEOF_DSTV_IN_EDGEDSTVDATA);
				if(i==2 && parsededge.incr==42 && parsededge.dstvid<50 && j+v <= 3456){ cout<<"################### loadedges.insertbitmap: i:"<<i<<", j:"<<j<<", v:"<<v<<", parsededge.incr("<<parsededge.incr<<")==42. parsededge.dstvid: "<<parsededge.dstvid<<", edges2_temp["<<i<<"]["<<j+v<<"].srcvid: "<<edges2_temp[i][j+v].srcvid<<", edges2_temp["<<i<<"]["<<j+v<<"].dstvid: "<<edges2_temp[i][j+v].dstvid<<", codededge: "<<codededge<<endl; }
				#endif 
				
				edges2_temp[i][j+v].dstvid = codededge;
				
				edges2_temp[i][j+v].metadata = edges2_temp[i][j+v].srcvid; 
			}
		}
		#ifdef _DEBUGMODE_HOSTCHECKS
		cout<<"loadedges::insert.bitmap:: "<<num_errors<<" error seen (instance "<<i<<")"<<endl;
		if(num_errors > 10000){ cout<<"loadedges.insertbitmap: ERROR 635. EXITING... "<<endl; exit(EXIT_FAILURE); }				
		#endif 
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printtriples("loadedges::insert.bitmap::[after.insert.bitmap]: printing edges2_temp["+std::to_string(i)+"][~]", (triple_t *)&edges2_temp[i][0], 8);
		#endif 
		// exit(EXIT_SUCCESS);
	}
	cout<<"### loadedges::insert.bitmap::[insert.bitmap] bitmap inserted successfully"<<endl;
	#endif
	// exit(EXIT_SUCCESS);
	
	// load edges
	#ifdef LOADEDGES
	cout<<"### loadedges::insert.bitmap:: loading edges..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		
		for(unsigned int k=0; k<edges2_temp[i].size(); k++){		
			utilityobj->checkoutofbounds("loadedges::insert.bitmap(20)::", TWOO*baseoffset_edgesdata + k, universalparams.MAXHBMCAPACITY_KVS * VECTOR_SIZE, NAp, NAp, NAp);		
			edges[i][TWOO*baseoffset_edgesdata + k].dstvid = edges2_temp[i][k].dstvid;		
		}	
		if(debug2b==true){ utilityobj->printvalues("loadedges[after]::loadedges: printing edges["+std::to_string(i)+"][~]", (value_t *)&edges[i][TWOO*baseoffset_edgesdata], 8); }
	}
	cout<<"### loadedges::insert.bitmap::[insert.bitmap] loading edges completed successfully"<<endl;
	#endif 
	
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int vid=0; vid<universalparams.KVDATA_RANGE; vid++){ counts_totalalledges_for_channel[i] += counts_alledges_for_channel[i][vid]; totalcount_alledges_for_all_channels += counts_alledges_for_channel[i][vid]; }}
	unsigned int max_totalalledges_for_channel=0; for(unsigned int i=0; i<NUM_PEs; i++){ if(max_totalalledges_for_channel < counts_totalalledges_for_channel[i]){ max_totalalledges_for_channel = counts_totalalledges_for_channel[i]; }}
	cout<<"loadedges::start: max_totalalledges_for_channel: "<<max_totalalledges_for_channel<<endl;

	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){
		globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA;
		globalparams.globalparamsK.SIZE_VERTEXPTRS = 0; 
		globalparams.globalparamsK.SIZE_EDGES = 0; 
		
		globalparams.globalparamsE.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA + ((max_totalalledges_for_channel/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + universalparams.DRAMPADD_KVS;
		globalparams.globalparamsE.SIZE_VERTEXPTRS = (kvdata_range__div__vptr_shrink_ratio) + universalparams.DRAMPADD_VPTRS; 
		globalparams.globalparamsE.SIZE_EDGES = max_totalalledges_for_channel + 1000; // '1000' is padding 
	} else {
		globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA + ((max_totalalledges_for_channel/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE) + universalparams.DRAMPADD_KVS;
		globalparams.globalparamsK.SIZE_VERTEXPTRS = (kvdata_range__div__vptr_shrink_ratio) + universalparams.DRAMPADD_VPTRS;
		globalparams.globalparamsK.SIZE_EDGES = max_totalalledges_for_channel + 1000;
	}
	globalparams_t globalparamsVPTRS;
	if(universalparams.EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM == 1){ globalparamsVPTRS = globalparams.globalparamsE; } 
	else { globalparamsVPTRS = globalparams.globalparamsK; }
	
	// calculate offsets
	#ifdef CALCULATEOFFSETS
	unsigned int _baseoffset = TWOO * (globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE);
	for(unsigned int i=0; i<NUM_PEs; i++){
		#ifdef TESTKERNEL // ignore unused loads when testing
		if(utilityobj->isbufferused(i) == false){ continue; }
		#endif 
		
		vptrs[i][_baseoffset + 0].key = 0;
		for(unsigned int vid=1; vid<kvdata_range__div__vptr_shrink_ratio; vid++){
			vptrs[i][_baseoffset + vid].key = vptrs[i][_baseoffset + vid - 1].key + counts_alledges_for_vpartition[i][vid-1]; 
			#ifdef _DEBUGMODE_HOSTPRINTS
			// if(i==0 && vid<0){ cout<<"loadedges:: vptrs[i]["<<_baseoffset + vid<<"].key: "<<vptrs[i][_baseoffset + vid].key<<" (%16="<<vptrs[i][_baseoffset + vid].key % 16<<")"<<endl; }
			if(i==0){ cout<<"loadedges:: num edges["<<i<<"]["<<vid<<"]: "<<vptrs[i][_baseoffset + vid].key - vptrs[i][_baseoffset + vid - 1].key<<endl; }
			#endif 
		}
		for(unsigned int vid=kvdata_range__div__vptr_shrink_ratio; vid<(kvdata_range__div__vptr_shrink_ratio) + universalparams.DRAMPADD_VPTRS; vid++){ // dummy filling...
			vptrs[i][_baseoffset + vid].key = counts_totalalledges_for_channel[i];
		}
	}
	#ifdef _DEBUGMODE_HOSTPRINTS
	// for(unsigned int i=0; i<1; i++){ utilityobj->printvalues("loadedges::start: printing vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE)], 8); } 
	for(unsigned int i=0; i<1; i++){ utilityobj->printvalues("loadedges::start: printing vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE)], universalparams.NUMPROCESSEDGESPARTITIONS); } 
	#endif
	#endif 
	
	// CHECK 4: check vptrs against edges
	#ifdef CHECK4_VERIFYOFFSETS
	cout<<"### loadedges::insert.bitmap:: checking vptrs against edges (CHECK 4(a&b))..."<<endl;
	unsigned int vptr_offset = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){ 
		for(unsigned int vid=0; vid<universalparams.KVDATA_RANGE-1; vid+=universalparams.VPTR_SHRINK_RATIO){	
			if(false){ if(tempvptrs[i][vptr_offset+vid].key % VECTOR2_SIZE != 0){ cout<<"CHECK 4(a): ERROR CHECKING FAILED. tempvptrs["<<i<<"]["<<vptr_offset+vid<<"].key("<<tempvptrs[i][vptr_offset+vid].key<<") % VECTOR2_SIZE != 0. EXITING... "<<endl; exit(EXIT_FAILURE); }} // too expensive
			for(unsigned int k=tempvptrs[i][vptr_offset+vid].key; k<tempvptrs[i][vptr_offset+vid+1].key; k++){
				if(edges_temp[i][k].srcvid != vid){
					cout<<"### loadedges::start:: CHECK 4(a): ERROR CHECKING FAILED: i: "<<i<<", vid:"<<vid<<", k: "<<k<<", tempvptrs["<<i<<"]["<<vid<<"]: "<<tempvptrs[i][vptr_offset+vid].key<<", tempvptrs["<<i<<"]["<<vid+1<<"]: "<<tempvptrs[i][vptr_offset+vid+1].key<<". EXITING..."<<endl;
					for(unsigned int v1=0; v1<VECTOR2_SIZE; v1++){
						cout<<v1<<": edges_temp["<<i<<"]["<<k+v1<<"].srcvid: "<<edges_temp[i][k+v1].srcvid<<", edges_temp["<<i<<"]["<<k+v1<<"].dstvid: "<<edges_temp[i][k+v1].dstvid<<", edges_temp["<<i<<"]["<<k+v1<<"].status: "<<edges_temp[i][k+v1].status<<", edges_temp["<<i<<"]["<<k+v1<<"].metadata: "<<edges_temp[i][k+v1].metadata<<endl;
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	cout<<"### loadedges::start:: CHECK 4(a&b): ERROR CHECKING SUCCESSFUL."<<endl;
	#endif
	
	for(unsigned int i=0; i<NUM_PEs; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = utilityobj->allignhigherto16_KV(universalparams.NUM_VERTICES);
		container->edgessize[i] = counts_totalalledges_for_channel[i]; 
		container->runsize[i] = counts_totalalledges_for_channel[i];
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int vid=0; vid<universalparams.KVDATA_RANGE; vid++){ counts_totalvalidedges_for_channel[i] += counts_validedges_for_channel[i][vid]; totalcount_validedges_for_all_channels += counts_validedges_for_channel[i][vid]; }}
	utilityobj->printvalues(">>> loadedges::start: printing counts_totalvalidedges_for_channel", (value_t *)&counts_totalvalidedges_for_channel[0], NUM_PEs);
	utilityobj->printvalues(">>> loadedges::start: printing counts_totalalledges_for_channel", (value_t *)&counts_totalalledges_for_channel[0], NUM_PEs);
	utilityobj->printvalues(">>> loadedges::start: printing counts_alldatas[0-N]", (value_t *)&counts_alldatas[0], NUM_PEs);
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges[after]::loadedges: printing counts_validedges_for_channel["+std::to_string(i)+"][~]", (value_t *)&counts_validedges_for_channel[i][0], 4); } 
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges[after]::loadedges: printing counts_alledges_for_channel["+std::to_string(i)+"][~]", (value_t *)&counts_alledges_for_channel[i][0], 4); } 
	for(unsigned int i=0; i<0; i++){ utilityobj->printkeyvalues("loadedges[after]::loadedges: printing edgedatabuffers_temp["+std::to_string(i)+"][~]", (keyvalue_t *)&edgedatabuffers_temp[i][0], 4); }
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges::start: printing vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE)], 8); } 
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadedges::start: totalcount_validedges_for_all_channels: "<<totalcount_validedges_for_all_channels<<", totalcount_alledges_for_all_channels: "<<totalcount_alledges_for_all_channels<<", *counts_alldata: "<<counts_alldata<<endl;
	#endif 
	// exit(EXIT_SUCCESS); //
	
	for(unsigned int i=0; i<NUM_PEs; i++){ edgedatabuffers_temp[i].clear(); }
	#ifdef CHECK4_VERIFYOFFSETS
	for(unsigned int i=0; i<NUM_PEs; i++){ delete tempvptrs[i]; }
	#endif 
	return globalparams;
}



