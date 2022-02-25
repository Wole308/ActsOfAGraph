#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include "../../examples/helperfunctions/evalparams.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "loadedges_sequential.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

#define TWOO 2
#define BITMAPCODE 0b10000000000000000000000000000000
#define BITMAPCODE_WITHHEADSRCV 0b11000000000000000000000000000000

#define FILLEMPTYVERTICESINCHANNELS
#define CHECK1_ENSUREALLVERTICESAREREPRESENTED
#define LOADSLICEDEDGES
#define CHECK2_ENSUREALLVERTICESAREREPRESENTED
#define CHECK3_CHECKFORNUMUNIQUESRCVIDSINVECTOR // CRITICAL FIXME.
#define CHECK4_VERIFYOFFSETS
#define CALCULATELOCALDSTVIDS
#define INSERTBITMAP
#define LOADEDGES

bool debug = false;//false;
bool debug2 = false;
bool debug3 = false;

loadedges_sequential::loadedges_sequential(graph * _graphobj, stats * _statsobj){
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	statsobj = _statsobj;
	evalparamsobj = new evalparams();
}
loadedges_sequential::loadedges_sequential(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
loadedges_sequential::~loadedges_sequential(){} 

unsigned int loadedges_sequential::GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int loadedges_sequential::READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & GETMASK_UINT((index), (size))) >> (index)); 
}
void loadedges_sequential::WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
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

unsigned int loadedges_sequential::gethash(unsigned int vid){
	return vid % NUM_PEs;
}
unsigned int loadedges_sequential::allignlower(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = val / V_SIZE;
	return (fac * V_SIZE);
}
unsigned int loadedges_sequential::allignhigher(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = (val + (V_SIZE-1)) / V_SIZE;
	return (fac * V_SIZE);
}

globalparams_TWOt loadedges_sequential::loadedges(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){			
	cout<<"loadedges_sequential::loadedges:: loading edges (rowwise)... "<<endl;

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
	vector<edge2_type> edgedatabuffers_temp2[NUMSUBCPUTHREADS];
	vptr_type * tempvptrs[NUMSUBCPUTHREADS]; for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ tempvptrs[i] = new vptr_type[KVDATA_RANGE]; }
	for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ edges_temp[j].clear(); }
	
	unsigned int * counts_validedges_for_channel[NUMSUBCPUTHREADS];
	unsigned int * counts_alledges_for_channel[NUMSUBCPUTHREADS];
	unsigned int counts_srcvslots_for_channel[NUMSUBCPUTHREADS];
	unsigned int counts_bitmapslots_for_channel[NUMSUBCPUTHREADS];
	unsigned int counts_dummyedgeslots_for_channel[NUMSUBCPUTHREADS];
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		counts_validedges_for_channel[i] = new unsigned int[KVDATA_RANGE];
		counts_alledges_for_channel[i] = new unsigned int[KVDATA_RANGE];
		counts_srcvslots_for_channel[i] = 0;
		counts_bitmapslots_for_channel[i] = 0;
		counts_dummyedgeslots_for_channel[i] = 0;
		for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){ counts_validedges_for_channel[i][vid] = 0; counts_alledges_for_channel[i][vid] = 0; }
	}
	unsigned int counts_totalvalidedges_for_channel[NUMSUBCPUTHREADS]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_totalvalidedges_for_channel[i] = 0; }
	unsigned int counts_totalalledges_for_channel[NUMSUBCPUTHREADS]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_totalalledges_for_channel[i] = 0; }
	unsigned int totalcount_validedges_for_all_channels = 0;
	unsigned int totalcount_alledges_for_all_channels = 0;
	unsigned int isFirst = 0;
	unsigned int isLast = 0;
	
	// calculate counts_validedges_for_channel
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		if(vid % 700000 == 0){ cout<<"### loadedges_sequential::loadedges:: vid: "<<vid<<", vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
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
	
	#ifdef FILLEMPTYVERTICESINCHANNELS
	edge2_type dummyedge; // dummyedge.srcvid = INVALIDDATA; dummyedge.dstvid = INVALIDDATA;
	unsigned int gap = 0;
	unsigned int totalgap = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		// cout<<">>> ======================= loadedges_sequential::loadedges:: i: "<<i<<endl;
		gap = 0;
		edgedatabuffers_temp2[i].push_back(edgedatabuffers_temp[i][0]);
		for(unsigned int k=1; k<edgedatabuffers_temp[i].size(); k++){
			unsigned int currsrcvid = edgedatabuffers_temp[i][k].srcvid;
			unsigned int prevsrcvid = edgedatabuffers_temp[i][k-1].srcvid;
			// if(k%1000000==0){ cout<<"+++++ loadedges_sequential::loadedges:: ["<<k<<"]: prevsrcvid: "<<prevsrcvid<<" currsrcvid: "<<currsrcvid<<endl; }
			// if(k<10){ cout<<"... loadedges_sequential::loadedges:: edgedatabuffers_temp["<<i<<"]["<<k<<"].srcvid: "<<edgedatabuffers_temp[i][k].srcvid<<", edgedatabuffers_temp["<<i<<"]["<<k<<"].dstvid: "<<edgedatabuffers_temp[i][k].dstvid<<endl; }
			if(currsrcvid >= prevsrcvid){
				if(currsrcvid > prevsrcvid + 1){
					for(unsigned int m=prevsrcvid+1; m<currsrcvid; m++){
						if(i==0 && gap < 8){ cout<<"loadedges_sequential::loadedges:: prevsrcvid: "<<prevsrcvid<<" currsrcvid: "<<currsrcvid<<", m: "<<m<<endl; }
						dummyedge.srcvid = m;
						dummyedge.dstvid = 888888; // FIXME.
						edgedatabuffers_temp2[i].push_back(dummyedge); 
						gap += 1;
						totalgap += 1;
					}
					edgedatabuffers_temp2[i].push_back(edgedatabuffers_temp[i][k]); 
				} else {
					edgedatabuffers_temp2[i].push_back(edgedatabuffers_temp[i][k]); 
				}
			} else {
				cout<<"###^^^ loadedges_sequential::loadedges:: ERROR: currsrcvid("<<currsrcvid<<") < prevsrcvid("<<prevsrcvid<<") "<<endl;
				for(unsigned int t=0; t<16; t++){ if(true){ cout<<"... loadedges_sequential::loadedges:: edgedatabuffers_temp["<<i<<"]["<<t<<"].srcvid: "<<edgedatabuffers_temp[i][t].srcvid<<", edgedatabuffers_temp["<<i<<"]["<<t<<"].dstvid: "<<edgedatabuffers_temp[i][t].dstvid<<endl; }}
				exit(EXIT_FAILURE);
			}
		}
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"### loadedges_sequential::loadedges:: number of empty vertices filled for memory channel "<<i<<": "<<gap<<endl;
		#endif 
	}
	cout<<"### loadedges_sequential::loadedges:: total number of empty vertices filled for all memory channels: "<<totalgap<<endl;
	if(debug2==true){ for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"edgedatabuffers_temp["<<i<<"].size(): "<<edgedatabuffers_temp[i].size()<<endl; }}
	if(debug2==true){ for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"edgedatabuffers_temp2["<<i<<"].size(): "<<edgedatabuffers_temp2[i].size()<<endl; }}
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// CHECK 1: ensure all vertices are represented
	#ifdef CHECK1_ENSUREALLVERTICESAREREPRESENTED
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int k=1; k<edgedatabuffers_temp2[i].size(); k++){
			unsigned int currsrcvid = edgedatabuffers_temp2[i][k].srcvid;
			unsigned int prevsrcvid = edgedatabuffers_temp2[i][k-1].srcvid;
			if(currsrcvid > prevsrcvid + 1 || currsrcvid < prevsrcvid){
				cout<<"### loadedges_sequential::loadedges:: CHECK 1: ERROR CHECKING FAILED: k:"<<k<<", edgedatabuffers_temp2["<<i<<"].size(): "<<edgedatabuffers_temp2[i].size()<<", (currsrcvid("<<currsrcvid<<") > prevsrcvid("<<prevsrcvid<<") + 1 || currsrcvid("<<currsrcvid<<") < prevsrcvid("<<prevsrcvid<<")). EXITING..."<<endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	cout<<"### loadedges_sequential::loadedges:: CHECK 1: ERROR CHECKING SUCCESSFUL."<<endl;
	#endif 
	// exit(EXIT_SUCCESS); //

	// load edges into memory channels according to edge representation format
	#ifdef LOADSLICEDEDGES
	cout<<"### loadedges_sequential::loadedges:: loading edges into memory channels according to edge representation format..."<<endl;
	unsigned int tempe_index = 0; 
	unsigned int index = 0;
	unsigned int counts_alldatas[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_alldatas[i] = 0; }
	unsigned int counts_alldata = 0;
	unsigned int srcvid_lastvechead = 0xFFFFFFFF;
	unsigned int srcvid_lastseen = 0;
	unsigned int numskippededges = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		cout<<"### loadedges_sequential::loadedges:: loading edges into PE: "<<i<<"..."<<endl;
		tempe_index = 0; 
		index = 0;
		srcvid_lastvechead = 0xFFFFFFFF;
		srcvid_lastseen = 0;
		while(tempe_index < edgedatabuffers_temp2[i].size()){
			if(tempe_index % 1000000 == 0 && false){ cout<<"loadedges_sequential::loadedges:: filling edges... tempe_index: "<<tempe_index<<endl; }
			edge2_type edge = edgedatabuffers_temp2[i][tempe_index];
			// if(index%VECTOR2_SIZE == 0 && edge.dstvid == 888888){ cout<<"loadedges_sequential: skipping edge (edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<")............."<<endl; }
			if(index%VECTOR2_SIZE == 0 && edge.dstvid == 888888){ numskippededges += 1; tempe_index += 1; continue; }
			// if(index%VECTOR2_SIZE == 0 && edge.dstvid == 888888){ numskippededges += 1; tempe_index += 1; continue; }
			
			edge2_type last_edge; if(tempe_index==0){ last_edge = edge; } else { last_edge = edgedatabuffers_temp2[i][tempe_index-1]; }
			if(debug2==true){ cout<<">>> edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<" [-]"<<endl; }
		
			if(index%VECTOR2_SIZE == 0){
				// insert source at head
				edge3_type edge_temp; edge_temp.srcvid = edge.srcvid; edge_temp.dstvid = edge.srcvid; edge_temp.status = EDGESTATUS_SOURCEV; edge_temp.metadata = 0, edges_temp[i].push_back(edge_temp);
				if(debug==true){ cout<<">>> edge_temp.srcvid: "<<edge_temp.srcvid<<", edge_temp.dstvid: "<<edge_temp.dstvid<<" [2]"<<endl; }
				srcvid_lastvechead = edge.srcvid;
				srcvid_lastseen = edge_temp.srcvid;
				index += 1;
				counts_alledges_for_channel[i][edge_temp.srcvid] += 1;
				counts_srcvslots_for_channel[i] += 1;
			} else {
				unsigned int incr = edge.srcvid - edges_temp[i][allignlower(index, VECTOR2_SIZE)].srcvid;
				if(incr < MAX_NUM_UNIQ_EDGES_PER_VEC){
					// insert edge
					edge3_type edge_temp; edge_temp.srcvid = edge.srcvid; edge_temp.dstvid = edge.dstvid; edge_temp.status = EDGESTATUS_VALIDEDGE; edge_temp.metadata = 0, edges_temp[i].push_back(edge_temp);
					if(debug==true){ cout<<">>> edge_temp.srcvid: "<<edge_temp.srcvid<<", edge_temp.dstvid: "<<edge_temp.dstvid<<", incr: "<<incr<<" [3]"<<endl; }
					srcvid_lastseen = edge_temp.srcvid;
					tempe_index += 1;
					index += 1;
					counts_validedges_for_channel[i][edge_temp.srcvid] += 1;
					counts_alledges_for_channel[i][edge_temp.srcvid] += 1;
				} else {
					// fill with dummy edges
					for(unsigned int k=index; k<allignhigher(index, VECTOR2_SIZE); k++){
						edge3_type edge_temp; edge_temp.srcvid = srcvid_lastseen; edge_temp.dstvid = INVALIDDATA; edge_temp.status = EDGESTATUS_DUMMYEDGE; edge_temp.metadata = 0, edges_temp[i].push_back(edge_temp);
						if(debug==true){ cout<<">>> edge_temp.srcvid: "<<edge_temp.srcvid<<", edge_temp.dstvid: "<<edge_temp.dstvid<<", incr: "<<incr<<" [4]"<<endl; }
						index += 1;
						counts_alledges_for_channel[i][edge_temp.srcvid] += 1;
						counts_dummyedgeslots_for_channel[i] += 1;
					}
				}
			}
		}
		
		counts_alldatas[i] += index;
		counts_alldata += index;
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printtriples("loadedges_sequential::[insert.edges] printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8);
		cout<<"### loadedges_sequential::[insert.edges] memory channel "<<i<<": tempe_index: "<<tempe_index<<", index: "<<index<<endl;
		#endif 
		cout<<"###~~~~~~~~~~~~~~~~~~~~~~~~~~ loadedges_sequential::[insert.edges] counts_dummyedgeslots_for_channel["<<i<<"] "<<counts_dummyedgeslots_for_channel[i]<<endl;
	}
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printvalues(">>> loadedges_sequential:[insert.edges] total number of edges in channels[0-N]", (value_t *)&counts_alldatas[0], NUM_PEs);
	cout<<">>> loadedges_sequential::[insert.edges] total number of edges in all memory channels: counts_alldata: "<<counts_alldata<<", NAp: "<<NAp<<endl;
	#endif 
	#endif 
	cout<<"### loadedges_sequential::[insert.edges] total number of skipped edges: "<<numskippededges<<endl;
	
	// CHECK 2: ensure all vertices are represented
	#ifdef CHECK2_ENSUREALLVERTICESAREREPRESENTED
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int j=0; j<edges_temp[i].size(); j+=VECTOR2_SIZE){
			for(unsigned int v=2; v<VECTOR2_SIZE; v++){
				if(j+v >= edges_temp[i].size()){ continue; }
				unsigned int currsrcvid = edges_temp[i][j+v].srcvid;
				unsigned int prevsrcvid = edges_temp[i][j+v-1].srcvid;
				if(currsrcvid > prevsrcvid + 1 || currsrcvid < prevsrcvid){
					cout<<"### loadedges_sequential::check2:: CHECK 2: ERROR CHECKING FAILED: ind:"<<j+v<<", (currsrcvid("<<currsrcvid<<") > prevsrcvid("<<prevsrcvid<<") + 1 || currsrcvid("<<currsrcvid<<") < prevsrcvid("<<prevsrcvid<<")). EXITING..."<<endl;
					for(unsigned int v1=0; v1<VECTOR2_SIZE; v1++){
						cout<<v1<<": edges_temp["<<i<<"]["<<j+v1<<"].srcvid: "<<edges_temp[i][j+v1].srcvid<<", edges_temp["<<i<<"]["<<j+v1<<"].dstvid: "<<edges_temp[i][j+v1].dstvid<<endl;
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	cout<<"### loadedges_sequential::check2:: CHECK 2: ERROR CHECKING SUCCESSFUL."<<endl;
	#endif 
	
	// CHECK 3: check to make sure no vector has more than MAX_NUM_UNIQ_EDGES_PER_VEC distinct srcvids
	#ifdef CHECK3_CHECKFORNUMUNIQUESRCVIDSINVECTOR
	edge3_type temp[VECTOR2_SIZE];
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int j=0; j<edges_temp[i].size(); j+=VECTOR2_SIZE){
			unsigned int num_uniq_scrvids = 0;
			unsigned int v_cnt = 0;
			for(unsigned int v=0; v<VECTOR2_SIZE; v++){
				if(j+v >= edges_temp[i].size()){ continue; }
				if(edges_temp[i][j+v].dstvid != INVALIDDATA && (edges_temp[i][j+v].srcvid != BITMAPCODE || edges_temp[i][j+v].srcvid != BITMAPCODE_WITHHEADSRCV)){
					temp[v_cnt] = edges_temp[i][j+v];
					v_cnt += 1;
				}
			}
			for(unsigned int v=1; v<v_cnt; v++){
				if(temp[v].srcvid != temp[v-1].srcvid){ num_uniq_scrvids += 1; }
				if(num_uniq_scrvids >= MAX_NUM_UNIQ_EDGES_PER_VEC){ // NEWCHANGE.
					cout<<"### loadedges_sequential::check3:: CHECK 3: ERROR CHECKING FAILED: num_uniq_scrvids("<<num_uniq_scrvids<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). index:"<<j+v<<", i: "<<i<<". EXITING..."<<endl;
					for(unsigned int v=0; v<VECTOR2_SIZE; v++){
						cout<<""<<v<<": edges_temp["<<i<<"]["<<j+v<<"].srcvid: "<<edges_temp[i][j+v].srcvid<<", edges_temp["<<i<<"]["<<j+v<<"].dstvid: "<<edges_temp[i][j+v].dstvid<<", edges_temp["<<i<<"]["<<j+v<<"].status: "<<edges_temp[i][j+v].status<<", edges_temp["<<i<<"]["<<j+v<<"].metadata: "<<edges_temp[i][j+v].metadata<<endl;						
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	cout<<"### loadedges_sequential::check3:: CHECK 3: ERROR CHECKING SUCCESSFUL."<<endl;
	#endif 
	
	// calculate local edge dstvids 
	#ifdef CALCULATELOCALDSTVIDS
	cout<<"### loadedges_sequential::insert.bitmap:: calculating local edge dstvids..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int k=0; k<edges_temp[i].size(); k++){
			if(edges_temp[i][k].status == EDGESTATUS_VALIDEDGE){ edges_temp[i][k].dstvid = (edges_temp[i][k].dstvid - i) / NUM_PEs; }
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printtriples("loadedges_sequential::[insert.local.edge.dstvids] printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8);
		#endif 
	}
	cout<<"### loadedges_sequential::[insert.local.edge.dstvids] calculating local edge dstvids inserted successfully"<<endl;
	#endif 
	
	// insert bitmap 
	#ifdef INSERTBITMAP
	cout<<"### loadedges_sequential::insert.bitmap:: inserting bitmap..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		if(debug2==true){ utilityobj->printtriples("loadedges_sequential::insert.bitmap::[before.insert.bitmap]: printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8); }
		for(unsigned int j=0; j<edges_temp[i].size(); j+=VECTOR2_SIZE){
			unsigned int srcvid_head = edges_temp[i][j].srcvid;
			for(unsigned int v=1; v<VECTOR2_SIZE; v++){
				if(j+v >= edges_temp[i].size()){ continue; }
				if(edges_temp[i][j+v].status == EDGESTATUS_DUMMYEDGE){ continue; }
				
				unsigned int incr = edges_temp[i][j+v].srcvid - srcvid_head;
				unsigned int loc = v;
				if(incr > 15){ cout<<"loadedges_sequential::insert.bitmap:: ERROR 4. incr("<<incr<<") > 3. exiting..."<<endl; exit(EXIT_FAILURE); }
				if(loc > 15){ cout<<"loadedges_sequential::insert.bitmap:: ERROR 4. loc("<<loc<<") > 31. exiting..."<<endl; exit(EXIT_FAILURE); }
				
				WRITETO_UINT((unsigned int *)&edges_temp[i][j+v].dstvid, 28, 4, incr); // NEWCHANGE.
				edges_temp[i][j+v].metadata = incr; 
			}
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printtriples("loadedges_sequential::insert.bitmap::[after.insert.bitmap]: printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8);
		#endif 
	}
	cout<<"### loadedges_sequential::insert.bitmap::[insert.bitmap] bitmap inserted successfully"<<endl;
	#endif
	
	// load edges 
	#ifdef LOADEDGES
	cout<<"### loadedges_sequential::insert.bitmap:: loading edges..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int k=0; k<edges_temp[i].size(); k++){
			edges[i][TWOO*_BASEOFFSET_EDGESDATA + k].dstvid = edges_temp[i][k].dstvid;
		}
		if(debug2==true){ utilityobj->printvalues("loadedges_sequential[after]::loadedges: printing edges["+std::to_string(i)+"][~]", (value_t *)&edges[i][TWOO*_BASEOFFSET_EDGESDATA], 8); }
	}
	cout<<"### loadedges_sequential::insert.bitmap::[insert.bitmap] loading edges completed successfully"<<endl;
	#endif 
	
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){ counts_totalalledges_for_channel[i] += counts_alledges_for_channel[i][vid]; totalcount_alledges_for_all_channels += counts_alledges_for_channel[i][vid]; }}
	unsigned int max_totalalledges_for_channel=0; for(unsigned int i=0; i<NUM_PEs; i++){ if(max_totalalledges_for_channel < counts_totalalledges_for_channel[i]){ max_totalalledges_for_channel = counts_totalalledges_for_channel[i]; }}
	cout<<"loadedges_sequential::loadedges: max_totalalledges_for_channel: "<<max_totalalledges_for_channel<<endl;

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
			_index += 1;
		}
		for(unsigned int vid=0; vid<(DRAMPADD/2); vid++){
			vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE) + _index].key = counts_totalalledges_for_channel[i];
			_index += 1;
		}
	}
	
	// CHECK 4: check vptrs against edges
	#ifdef CHECK4_VERIFYOFFSETS
	cout<<"### loadedges_sequential::insert.bitmap:: checking vptrs against edges (CHECK 4(a&b))..."<<endl;
	unsigned int vptr_offset = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid+=VPTR_SHRINK_RATIO){	
			if(false){ if(tempvptrs[i][vptr_offset+vid].key % VECTOR2_SIZE != 0){ cout<<"CHECK 4(a): ERROR CHECKING FAILED. tempvptrs["<<i<<"]["<<vptr_offset+vid<<"].key("<<tempvptrs[i][vptr_offset+vid].key<<") % VECTOR2_SIZE != 0. EXITING... "<<endl; exit(EXIT_FAILURE); }} // too expensive
			for(unsigned int k=tempvptrs[i][vptr_offset+vid].key; k<tempvptrs[i][vptr_offset+vid+1].key; k++){
				if(edges_temp[i][k].srcvid != vid){
					cout<<"### loadedges_sequential::loadedges:: CHECK 4(a): ERROR CHECKING FAILED: i: "<<i<<", vid:"<<vid<<", k: "<<k<<", tempvptrs["<<i<<"]["<<vid<<"]: "<<tempvptrs[i][vptr_offset+vid].key<<", tempvptrs["<<i<<"]["<<vid+1<<"]: "<<tempvptrs[i][vptr_offset+vid+1].key<<". EXITING..."<<endl;
					for(unsigned int v1=0; v1<VECTOR2_SIZE; v1++){
						cout<<v1<<": edges_temp["<<i<<"]["<<k+v1<<"].srcvid: "<<edges_temp[i][k+v1].srcvid<<", edges_temp["<<i<<"]["<<k+v1<<"].dstvid: "<<edges_temp[i][k+v1].dstvid<<", edges_temp["<<i<<"]["<<k+v1<<"].status: "<<edges_temp[i][k+v1].status<<", edges_temp["<<i<<"]["<<k+v1<<"].metadata: "<<edges_temp[i][k+v1].metadata<<endl;
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	vptr_offset = TWOO * globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid+=VPTR_SHRINK_RATIO){	
			for(unsigned int k=vptrs[i][vptr_offset+vid].key; k<vptrs[i][vptr_offset+vid+1].key; k++){
				if(edges_temp[i][k].srcvid < vid || edges_temp[i][k].srcvid >= (vid+VPTR_SHRINK_RATIO)){
					cout<<"### loadedges_sequential::loadedges:: CHECK 4(b): ERROR CHECKING FAILED: i: "<<i<<", vid:"<<vid<<", k: "<<k<<", vptrs["<<i<<"]["<<vid<<"]: "<<vptrs[i][vptr_offset+vid].key<<", vptrs["<<i<<"]["<<vid+1<<"]: "<<vptrs[i][vptr_offset+vid+1].key<<". EXITING..."<<endl;
					for(unsigned int v1=0; v1<VECTOR2_SIZE; v1++){
						cout<<v1<<": edges_temp["<<i<<"]["<<k+v1<<"].srcvid: "<<edges_temp[i][k+v1].srcvid<<", edges_temp["<<i<<"]["<<k+v1<<"].dstvid: "<<edges_temp[i][k+v1].dstvid<<", edges_temp["<<i<<"]["<<k+v1<<"].status: "<<edges_temp[i][k+v1].status<<", edges_temp["<<i<<"]["<<k+v1<<"].metadata: "<<edges_temp[i][k+v1].metadata<<endl;
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	cout<<"### loadedges_sequential::loadedges:: CHECK 4(a&b): ERROR CHECKING SUCCESSFUL."<<endl;
	#endif
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = utilityobj->allignhigherto16_KV(graphobj->get_num_vertices());
		container->edgessize[i] = counts_totalalledges_for_channel[i]; 
		container->runsize[i] = counts_totalalledges_for_channel[i];
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){ counts_totalvalidedges_for_channel[i] += counts_validedges_for_channel[i][vid]; totalcount_validedges_for_all_channels += counts_validedges_for_channel[i][vid]; }}
	utilityobj->printvalues(">>> loadedges_sequential::loadedges: printing counts_totalvalidedges_for_channel", (value_t *)&counts_totalvalidedges_for_channel[0], NUM_PEs);
	utilityobj->printvalues(">>> loadedges_sequential::loadedges: printing counts_totalalledges_for_channel", (value_t *)&counts_totalalledges_for_channel[0], NUM_PEs);
	utilityobj->printvalues(">>> loadedges_sequential::loadedges: printing counts_alldatas[0-N]", (value_t *)&counts_alldatas[0], NUM_PEs);
	utilityobj->printvalues("loadedges_sequential::loadedges: printing counts_srcvslots_for_channel", (value_t *)&counts_srcvslots_for_channel[0], NUM_PEs);
	utilityobj->printvalues("loadedges_sequential::loadedges: printing counts_bitmapslots_for_channel", (value_t *)&counts_bitmapslots_for_channel[0], NUM_PEs);
	utilityobj->printvalues("loadedges_sequential::loadedges: printing counts_dummyedgeslots_for_channel", (value_t *)&counts_dummyedgeslots_for_channel[0], NUM_PEs);
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_sequential[after]::loadedges: printing counts_validedges_for_channel["+std::to_string(i)+"][~]", (value_t *)&counts_validedges_for_channel[i][0], 4); } 
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_sequential[after]::loadedges: printing counts_alledges_for_channel["+std::to_string(i)+"][~]", (value_t *)&counts_alledges_for_channel[i][0], 4); } 
	for(unsigned int i=0; i<0; i++){ utilityobj->printkeyvalues("loadedges_sequential[after]::loadedges: printing edgedatabuffers_temp["+std::to_string(i)+"][~]", (keyvalue_t *)&edgedatabuffers_temp[i][0], 4); }
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_sequential::loadedges: printing vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE)], 8); } 
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadedges_sequential::loadedges: totalcount_validedges_for_all_channels: "<<totalcount_validedges_for_all_channels<<", totalcount_alledges_for_all_channels: "<<totalcount_alledges_for_all_channels<<", *counts_alldata: "<<counts_alldata<<endl;
	#endif 
	// exit(EXIT_SUCCESS); //
	
	for(unsigned int i=0; i<NUM_PEs; i++){ edgedatabuffers_temp[i].clear(); }
	for(unsigned int i=0; i<NUM_PEs; i++){ edgedatabuffers_temp2[i].clear(); }
	for(unsigned int i=0; i<NUM_PEs; i++){ delete tempvptrs[i]; }
	return globalparams;
}




