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
#include "loadedges_splitdstvxs.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

#define TWOO 2
// #define NUM_PEs 16//NUM_PARTITIONS
// #define _VECTOR2_SIZE VECTOR2_SIZE
// #define MAX_NUM_UNIQ_EDGES_PER_VEC 8//4 // FIXME.
#define BITMAPCODE 0b10000000000000000000000000000000
#define BITMAPCODE_WITHHEADSRCV 0b11000000000000000000000000000000

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

loadedges_splitdstvxs::loadedges_splitdstvxs(graph * _graphobj, stats * _statsobj){
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	statsobj = _statsobj;
	evalparamsobj = new evalparams();
}
loadedges_splitdstvxs::loadedges_splitdstvxs(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
loadedges_splitdstvxs::~loadedges_splitdstvxs(){} 

unsigned int loadedges_splitdstvxs::GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int loadedges_splitdstvxs::READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & GETMASK_UINT((index), (size))) >> (index)); 
}
void loadedges_splitdstvxs::WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
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

unsigned int loadedges_splitdstvxs::gethash(unsigned int vid){
	return vid % NUM_PEs;
}
unsigned int loadedges_splitdstvxs::allignlower(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = val / V_SIZE;
	return (fac * V_SIZE);
}
unsigned int loadedges_splitdstvxs::allignhigher(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = (val + (V_SIZE-1)) / V_SIZE;
	return (fac * V_SIZE);
}

globalparams_TWOt loadedges_splitdstvxs::loadedges(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){			
	cout<<"loadedges_splitdstvxs::loadedges:: loading edges (rowwise)... "<<endl;

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
		if(vid % 700000 == 0){ cout<<"### loadedges_splitdstvxs::loadedges:: vid: "<<vid<<", vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
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
	
	edge2_type dummyedge; // dummyedge.srcvid = INVALIDDATA; dummyedge.dstvid = INVALIDDATA;
	unsigned int gap = 0;
	unsigned int totalgap = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		gap = 0;
		edgedatabuffers_temp2[i].push_back(edgedatabuffers_temp[i][0]);
		for(unsigned int k=1; k<edgedatabuffers_temp[i].size(); k++){
			unsigned int currsrcvid = edgedatabuffers_temp[i][k].srcvid;
			unsigned int prevsrcvid = edgedatabuffers_temp[i][k-1].srcvid;
			if(currsrcvid >= prevsrcvid){
				if(currsrcvid > prevsrcvid + 1){
					for(unsigned int m=prevsrcvid+1; m<currsrcvid; m++){
						if(i==0 && gap < 8){ cout<<"loadedges_splitdstvxs::loadedges:: prevsrcvid: "<<prevsrcvid<<" currsrcvid: "<<currsrcvid<<", m: "<<m<<endl; }
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
				cout<<"### loadedges_splitdstvxs::loadedges:: ERROR: currsrcvid("<<currsrcvid<<") < prevsrcvid("<<prevsrcvid<<") "<<endl;
				exit(EXIT_FAILURE);
			}
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"### loadedges_splitdstvxs::loadedges:: number of empty vertices filled for memory channel "<<i<<": "<<gap<<endl;
		#endif 
	}
	cout<<"### loadedges_splitdstvxs::loadedges:: total number of empty vertices filled for all memory channels: "<<totalgap<<endl;
	if(debug2==true){ for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"edgedatabuffers_temp["<<i<<"].size(): "<<edgedatabuffers_temp[i].size()<<endl; }}
	if(debug2==true){ for(unsigned int i=0; i<NUM_PEs; i++){ cout<<"edgedatabuffers_temp2["<<i<<"].size(): "<<edgedatabuffers_temp2[i].size()<<endl; }}
	// exit(EXIT_SUCCESS); //
	
	// CHECK 1: ensure all vertices are represented
	#ifdef CHECK1_ENSUREALLVERTICESAREREPRESENTED
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int k=1; k<edgedatabuffers_temp2[i].size(); k++){
			unsigned int currsrcvid = edgedatabuffers_temp2[i][k].srcvid;
			unsigned int prevsrcvid = edgedatabuffers_temp2[i][k-1].srcvid;
			if(currsrcvid > prevsrcvid + 1 || currsrcvid < prevsrcvid){
				cout<<"### loadedges_splitdstvxs::loadedges:: CHECK 1: ERROR CHECKING FAILED: k:"<<k<<", edgedatabuffers_temp2["<<i<<"].size(): "<<edgedatabuffers_temp2[i].size()<<", (currsrcvid("<<currsrcvid<<") > prevsrcvid("<<prevsrcvid<<") + 1 || currsrcvid("<<currsrcvid<<") < prevsrcvid("<<prevsrcvid<<")). EXITING..."<<endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	cout<<"### loadedges_splitdstvxs::loadedges:: CHECK 1: ERROR CHECKING SUCCESSFUL."<<endl;
	#endif 
	// exit(EXIT_SUCCESS); //

	// load edges into memory channels according to edge representation format
	#ifdef LOADSLICEDEDGES
	cout<<"### loadedges_splitdstvxs::loadedges:: loading edges into memory channels according to edge representation format..."<<endl;
	unsigned int tempe_index = 0; 
	unsigned int index = 0;
	unsigned int counts_alldatas[NUM_PEs]; for(unsigned int i=0; i<NUM_PEs; i++){ counts_alldatas[i] = 0; }
	unsigned int counts_alldata = 0;
	unsigned int srcvid_lastvechead = 0xFFFFFFFF;
	unsigned int srcvid_lastseen = 0;
	for(unsigned int i=0; i<NUM_PEs; i++){
		cout<<"### loadedges_splitdstvxs::loadedges:: loading edges into PE: "<<i<<"..."<<endl;
		tempe_index = 0; 
		index = 0;
		srcvid_lastvechead = 0xFFFFFFFF;
		srcvid_lastseen = 0;
		while(tempe_index < edgedatabuffers_temp2[i].size()){
			if(tempe_index % 1000000 == 0 && false){ cout<<"loadedges_splitdstvxs::loadedges:: filling edges... tempe_index: "<<tempe_index<<endl; }
			edge2_type edge = edgedatabuffers_temp2[i][tempe_index];
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
		utilityobj->printtriples("loadedges_splitdstvxs::[insert.edges] printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8);
		cout<<"### loadedges_splitdstvxs::[insert.edges] memory channel "<<i<<": tempe_index: "<<tempe_index<<", index: "<<index<<endl;
		#endif 
	}
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printvalues(">>> loadedges_splitdstvxs:[insert.edges] total number of edges in channels[0-N]", (value_t *)&counts_alldatas[0], NUM_PEs);
	cout<<">>> loadedges_splitdstvxs::[insert.edges] total number of edges in all memory channels: counts_alldata: "<<counts_alldata<<", NAp: "<<NAp<<endl;
	#endif 
	#endif 
	
	// CHECK 2: ensure all vertices are represented
	#ifdef CHECK2_ENSUREALLVERTICESAREREPRESENTED
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int j=0; j<edges_temp[i].size(); j+=VECTOR2_SIZE){
			for(unsigned int v=2; v<VECTOR2_SIZE; v++){
				if(j+v >= edges_temp[i].size()){ continue; }
				unsigned int currsrcvid = edges_temp[i][j+v].srcvid;
				unsigned int prevsrcvid = edges_temp[i][j+v-1].srcvid;
				if(currsrcvid > prevsrcvid + 1 || currsrcvid < prevsrcvid){
					cout<<"### loadedges_splitdstvxs::check2:: CHECK 2: ERROR CHECKING FAILED: ind:"<<j+v<<", (currsrcvid("<<currsrcvid<<") > prevsrcvid("<<prevsrcvid<<") + 1 || currsrcvid("<<currsrcvid<<") < prevsrcvid("<<prevsrcvid<<")). EXITING..."<<endl;
					for(unsigned int v1=0; v1<VECTOR2_SIZE; v1++){
						cout<<v1<<": edges_temp["<<i<<"]["<<j+v1<<"].srcvid: "<<edges_temp[i][j+v1].srcvid<<", edges_temp["<<i<<"]["<<j+v1<<"].dstvid: "<<edges_temp[i][j+v1].dstvid<<endl;
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	cout<<"### loadedges_splitdstvxs::check2:: CHECK 2: ERROR CHECKING SUCCESSFUL."<<endl;
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
					cout<<"### loadedges_splitdstvxs::check3:: CHECK 3: ERROR CHECKING FAILED: num_uniq_scrvids("<<num_uniq_scrvids<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). index:"<<j+v<<", i: "<<i<<". EXITING..."<<endl;
					for(unsigned int v=0; v<VECTOR2_SIZE; v++){
						cout<<""<<v<<": edges_temp["<<i<<"]["<<j+v<<"].srcvid: "<<edges_temp[i][j+v].srcvid<<", edges_temp["<<i<<"]["<<j+v<<"].dstvid: "<<edges_temp[i][j+v].dstvid<<", edges_temp["<<i<<"]["<<j+v<<"].status: "<<edges_temp[i][j+v].status<<", edges_temp["<<i<<"]["<<j+v<<"].metadata: "<<edges_temp[i][j+v].metadata<<endl;						
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	cout<<"### loadedges_splitdstvxs::check3:: CHECK 3: ERROR CHECKING SUCCESSFUL."<<endl;
	#endif 
	
	// calculate local edge dstvids 
	#ifdef CALCULATELOCALDSTVIDS
	cout<<"### loadedges_splitdstvxs::insert.bitmap:: calculating local edge dstvids..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int k=0; k<edges_temp[i].size(); k++){
			if(edges_temp[i][k].status == EDGESTATUS_VALIDEDGE){ edges_temp[i][k].dstvid = (edges_temp[i][k].dstvid - i) / NUM_PEs; }
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printtriples("loadedges_splitdstvxs::[insert.local.edge.dstvids] printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8);
		#endif 
	}
	cout<<"### loadedges_splitdstvxs::[insert.local.edge.dstvids] calculating local edge dstvids inserted successfully"<<endl;
	#endif 
	
	// insert bitmap 
	#ifdef INSERTBITMAP
	cout<<"### loadedges_splitdstvxs::insert.bitmap:: inserting bitmap..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		if(debug2==true){ utilityobj->printtriples("loadedges_splitdstvxs::insert.bitmap::[before.insert.bitmap]: printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8); }
		for(unsigned int j=0; j<edges_temp[i].size(); j+=VECTOR2_SIZE){
			unsigned int srcvid_head = edges_temp[i][j].srcvid;
			for(unsigned int v=1; v<VECTOR2_SIZE; v++){
				if(j+v >= edges_temp[i].size()){ continue; }
				if(edges_temp[i][j+v].status == EDGESTATUS_DUMMYEDGE){ continue; }
				
				unsigned int incr = edges_temp[i][j+v].srcvid - srcvid_head;
				unsigned int loc = v;
				if(incr > 15){ cout<<"loadedges_splitdstvxs::insert.bitmap:: ERROR 4. incr("<<incr<<") > 3. exiting..."<<endl; exit(EXIT_FAILURE); }
				if(loc > 15){ cout<<"loadedges_splitdstvxs::insert.bitmap:: ERROR 4. loc("<<loc<<") > 31. exiting..."<<endl; exit(EXIT_FAILURE); }
				
				WRITETO_UINT((unsigned int *)&edges_temp[i][j+v].dstvid, 28, 4, incr); // NEWCHANGE.
				edges_temp[i][j+v].metadata = incr; 
			}
		}
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printtriples("loadedges_splitdstvxs::insert.bitmap::[after.insert.bitmap]: printing edges_temp["+std::to_string(i)+"][~]", (triple_t *)&edges_temp[i][0], 8);
		#endif 
	}
	cout<<"### loadedges_splitdstvxs::insert.bitmap::[insert.bitmap] bitmap inserted successfully"<<endl;
	#endif
	
	// load edges 
	#ifdef LOADEDGES
	cout<<"### loadedges_splitdstvxs::insert.bitmap:: loading edges..."<<endl;
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int k=0; k<edges_temp[i].size(); k++){
			edges[i][TWOO*_BASEOFFSET_EDGESDATA + k].dstvid = edges_temp[i][k].dstvid;
		}
		if(debug2==true){ utilityobj->printvalues("loadedges_splitdstvxs[after]::loadedges: printing edges["+std::to_string(i)+"][~]", (value_t *)&edges[i][TWOO*_BASEOFFSET_EDGESDATA], 8); }
	}
	cout<<"### loadedges_splitdstvxs::insert.bitmap::[insert.bitmap] loading edges completed successfully"<<endl;
	#endif 
	
	for(unsigned int i=0; i<NUM_PEs; i++){ for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){ counts_totalalledges_for_channel[i] += counts_alledges_for_channel[i][vid]; totalcount_alledges_for_all_channels += counts_alledges_for_channel[i][vid]; }}
	unsigned int max_totalalledges_for_channel=0; for(unsigned int i=0; i<NUM_PEs; i++){ if(max_totalalledges_for_channel < counts_totalalledges_for_channel[i]){ max_totalalledges_for_channel = counts_totalalledges_for_channel[i]; }}
	cout<<"loadedges_splitdstvxs::loadedges: max_totalalledges_for_channel: "<<max_totalalledges_for_channel<<endl;

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
	
	/* // calculate offsets
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		tempvptrs[i][0].key = 0;
		for(unsigned int vid=1; vid<KVDATA_RANGE; vid++){	
			// tempvptrs[i][vid].key = tempvptrs[i][vid-1].key + counts_alledges_for_channel[i][vid-1]; 
			tempvptrs[i][vid].key = tempvptrs[i][vid-1].key + counts_alledges_for_channel[i][vid-1]; 
		}
	}
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		unsigned int _index = 0;
		for(unsigned int vid=0; vid<KVDATA_RANGE; vid+=VPTR_SHRINK_RATIO){	
			vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE) + _index].key = tempvptrs[i][vid].key; 
			_index += 1;
		}
		for(unsigned int vid=0; vid<(DRAMPADD/2); vid++){ // dummy pads
			vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE) + _index].key = counts_totalalledges_for_channel[i];
			_index += 1;
		}
	} */
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
	cout<<"### loadedges_splitdstvxs::insert.bitmap:: checking vptrs against edges (CHECK 4(a&b))..."<<endl;
	unsigned int vptr_offset = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid+=VPTR_SHRINK_RATIO){	
			if(false){ if(tempvptrs[i][vptr_offset+vid].key % VECTOR2_SIZE != 0){ cout<<"CHECK 4(a): ERROR CHECKING FAILED. tempvptrs["<<i<<"]["<<vptr_offset+vid<<"].key("<<tempvptrs[i][vptr_offset+vid].key<<") % VECTOR2_SIZE != 0. EXITING... "<<endl; exit(EXIT_FAILURE); }} // too expensive
			for(unsigned int k=tempvptrs[i][vptr_offset+vid].key; k<tempvptrs[i][vptr_offset+vid+1].key; k++){
				if(edges_temp[i][k].srcvid != vid){
					cout<<"### loadedges_splitdstvxs::loadedges:: CHECK 4(a): ERROR CHECKING FAILED: i: "<<i<<", vid:"<<vid<<", k: "<<k<<", tempvptrs["<<i<<"]["<<vid<<"]: "<<tempvptrs[i][vptr_offset+vid].key<<", tempvptrs["<<i<<"]["<<vid+1<<"]: "<<tempvptrs[i][vptr_offset+vid+1].key<<". EXITING..."<<endl;
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
					cout<<"### loadedges_splitdstvxs::loadedges:: CHECK 4(b): ERROR CHECKING FAILED: i: "<<i<<", vid:"<<vid<<", k: "<<k<<", vptrs["<<i<<"]["<<vid<<"]: "<<vptrs[i][vptr_offset+vid].key<<", vptrs["<<i<<"]["<<vid+1<<"]: "<<vptrs[i][vptr_offset+vid+1].key<<". EXITING..."<<endl;
					for(unsigned int v1=0; v1<VECTOR2_SIZE; v1++){
						cout<<v1<<": edges_temp["<<i<<"]["<<k+v1<<"].srcvid: "<<edges_temp[i][k+v1].srcvid<<", edges_temp["<<i<<"]["<<k+v1<<"].dstvid: "<<edges_temp[i][k+v1].dstvid<<", edges_temp["<<i<<"]["<<k+v1<<"].status: "<<edges_temp[i][k+v1].status<<", edges_temp["<<i<<"]["<<k+v1<<"].metadata: "<<edges_temp[i][k+v1].metadata<<endl;
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	cout<<"### loadedges_splitdstvxs::loadedges:: CHECK 4(a&b): ERROR CHECKING SUCCESSFUL."<<endl;
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
	utilityobj->printvalues(">>> loadedges_splitdstvxs::loadedges: printing counts_totalvalidedges_for_channel", (value_t *)&counts_totalvalidedges_for_channel[0], NUM_PEs);
	utilityobj->printvalues(">>> loadedges_splitdstvxs::loadedges: printing counts_totalalledges_for_channel", (value_t *)&counts_totalalledges_for_channel[0], NUM_PEs);
	utilityobj->printvalues(">>> loadedges_splitdstvxs::loadedges: printing counts_alldatas[0-N]", (value_t *)&counts_alldatas[0], NUM_PEs);
	utilityobj->printvalues("loadedges_splitdstvxs::loadedges: printing counts_srcvslots_for_channel", (value_t *)&counts_srcvslots_for_channel[0], NUM_PEs);
	utilityobj->printvalues("loadedges_splitdstvxs::loadedges: printing counts_bitmapslots_for_channel", (value_t *)&counts_bitmapslots_for_channel[0], NUM_PEs);
	utilityobj->printvalues("loadedges_splitdstvxs::loadedges: printing counts_dummyedgeslots_for_channel", (value_t *)&counts_dummyedgeslots_for_channel[0], NUM_PEs);
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_splitdstvxs[after]::loadedges: printing counts_validedges_for_channel["+std::to_string(i)+"][~]", (value_t *)&counts_validedges_for_channel[i][0], 4); } 
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_splitdstvxs[after]::loadedges: printing counts_alledges_for_channel["+std::to_string(i)+"][~]", (value_t *)&counts_alledges_for_channel[i][0], 4); } 
	for(unsigned int i=0; i<0; i++){ utilityobj->printkeyvalues("loadedges_splitdstvxs[after]::loadedges: printing edgedatabuffers_temp["+std::to_string(i)+"][~]", (keyvalue_t *)&edgedatabuffers_temp[i][0], 4); }
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_splitdstvxs::loadedges: printing vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][TWOO*(globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE)], 8); } 
	cout<<"loadedges_splitdstvxs::loadedges: totalcount_validedges_for_all_channels: "<<totalcount_validedges_for_all_channels<<", totalcount_alledges_for_all_channels: "<<totalcount_alledges_for_all_channels<<", *counts_alldata: "<<counts_alldata<<endl;
	#endif 
	// exit(EXIT_SUCCESS); //
	
	for(unsigned int i=0; i<NUM_PEs; i++){ edgedatabuffers_temp[i].clear(); }
	for(unsigned int i=0; i<NUM_PEs; i++){ edgedatabuffers_temp2[i].clear(); }
	for(unsigned int i=0; i<NUM_PEs; i++){ delete tempvptrs[i]; }
	return globalparams;
}

unsigned int loadedges_splitdstvxs::getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth){
	unsigned int partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * treedepth)));
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadedges_splitdstvxs::getglobalpartition", partition, (1 << (NUM_PARTITIONS_POW * treedepth)), keyvalue.key, upperlimit, NAp);
	#endif
	return partition;
}
globalparams_TWOt loadedges_splitdstvxs::loadoffsetmarkers(vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadedges_splitdstvxs::loadoffsetmarkers:: loading offset markers... "<<endl;
	#endif
	
	globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK + (globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK / VECTOR_SIZE);
	globalparams.globalparamsK.SIZE_KVSTATSDRAM = ACTIVE_KVSTATSDRAMSZ *
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		NUM_EDGE_BANKS;
		#else 
		1;	
		#endif
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM = globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK + (globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK / VECTOR_SIZE);
	globalparams.globalparamsE.SIZE_KVSTATSDRAM = ACTIVE_KVSTATSDRAMSZ; // * NUM_EDGE_BANKS; // NEWCHANGE.
	#endif 
	
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM + globalparams.globalparamsK.SIZE_KVSTATSDRAM; 
	globalparams.globalparamsK.SIZE_EDGESSTATSDRAM = 
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		0;
		#else 
		EDGESSTATSDRAMSZ;	
		#endif
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESSTATSDRAM = globalparams.globalparamsE.BASEOFFSETKVS_STATSDRAM + globalparams.globalparamsE.SIZE_KVSTATSDRAM;
	globalparams.globalparamsE.SIZE_EDGESSTATSDRAM = EDGESSTATSDRAMSZ;
	#endif 
	
	// calculate best-fit value for NUM_EDGECHUNKS_IN_A_BUFFER
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	unsigned int num_edges_per_channel = globalparams.globalparamsE.SIZE_EDGES;
	unsigned int num_vertices_per_channel = KVDATA_RANGE;
	unsigned int A = 4 * num_edges_per_channel;
	unsigned int B = (256000000/4) - (3 * num_vertices_per_channel);
	unsigned int num_edgechunks_in_a_buffer = (A + (B-1)) / B; // 3V + 2E/N + 2E/N = (256MB/4)
	cout<<"---------------------------------------------------------------------- A: "<<A<<", B: "<<B<<", num_edgechunks_in_a_buffer: "<<num_edgechunks_in_a_buffer<<endl;
	#else 
	unsigned int num_edgechunks_in_a_buffer = 1;	
	#endif
	
	globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = num_edgechunks_in_a_buffer;
	globalparams.globalparamsE.ACTSPARAMS_NUMEDGECHUNKSINABUFFER = globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER;

	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	#else 
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	#endif
	unsigned int _KVSTATSDRAMSZ = globalparams.globalparamsK.SIZE_KVSTATSDRAM; // CHANGEPOINT.
	unsigned int _KVSTATSSZ = _KVSTATSDRAMSZ / VECTOR_SIZE;
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams_t globalparamsVPTRS = globalparams.globalparamsE;
	globalparams_t globalparamsSTATS = globalparams.globalparamsE;
	#else 
	globalparams_t globalparamsVPTRS = globalparams.globalparamsK;
	globalparams_t globalparamsSTATS = globalparams.globalparamsK;
	#endif
	globalparams_t globalparamsEDGESSTATS =  
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		globalparams.globalparamsE;
		#else 
		globalparams.globalparamsK;
		#endif 
	
	unsigned int _BASEOFFSET_STATSDRAM = globalparamsSTATS.BASEOFFSETKVS_STATSDRAM * VECTOR_SIZE; // NEWCHANGE.
	unsigned int _BASEOFFSET_EDGESSTATSDRAM = globalparamsSTATS.BASEOFFSETKVS_EDGESSTATSDRAM * VECTOR_SIZE; 
	unsigned int _BASEOFFSET_VERTEXPTR = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE;
	unsigned int _NUM_EDGECHUNKS_IN_A_BUFFER = globalparams.globalparamsK.ACTSPARAMS_NUMEDGECHUNKSINABUFFER;
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_STATSDRAM", globalparams.globalparamsK.BASEOFFSETKVS_STATSDRAM, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif
	
	unsigned int totalnumpartitions = 0;
	for(unsigned int k=0; k<=TREE_DEPTH; k++){ totalnumpartitions += (unsigned int)pow(NUM_PARTITIONS, k); } 
	unsigned int maxdramsz = 0;

	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int k=0; k<_KVSTATSSZ; k++){
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				statsptrVec[k].data[v].key = 0;
				statsptrVec[k].data[v].value = 0;
			}
		}
	}
	
	keyvalue_t **tempstats = new keyvalue_t*[_NUM_EDGECHUNKS_IN_A_BUFFER];
	for(unsigned int i = 0; i < _NUM_EDGECHUNKS_IN_A_BUFFER; ++i){ tempstats[i] = new keyvalue_t[KVSTATSDRAMSZ]; }
	for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ for(unsigned int k=0; k<KVSTATSDRAMSZ; k++){ tempstats[u][k].key = 0; tempstats[u][k].value = 0; }}
	
	unsigned int src1 = 0;
	unsigned int src2 = 0;
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadedges_splitdstvxs::loadoffsetmarkers:: edgessize["<<i<<"]: "<<container->edgessize[i]<<endl;
		#endif 
		if(i >= NUM_PEs){ continue; }
		
		keyvalue_t * statsptr = (keyvalue_t *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ for(unsigned int k=0; k<KVSTATSDRAMSZ; k++){ tempstats[u][k].key = 0; tempstats[u][k].value = 0; }}
		
		src1 = 0;
		src2 = 0;
		
		unsigned int srC=0;
		unsigned int SRC_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER+1];
		unsigned int PARTITION_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER+1];
		for(unsigned int k=0; k<_NUM_EDGECHUNKS_IN_A_BUFFER; k++){
			unsigned int index = k * (container->edgessize[i] / _NUM_EDGECHUNKS_IN_A_BUFFER);
			for(unsigned int t=0; t<16; t++){ if(edges_temp[i][index+t].status != EDGESTATUS_BITMAP){ srC = edges_temp[i][index+t].srcvid; break; }}
			srC = (srC / (REDUCESZ * VDATA_PACKINGSIZE)) * (REDUCESZ * VDATA_PACKINGSIZE); // round down
			SRC_CHKPT[k] = srC;
			PARTITION_CHKPT[k] = srC / (REDUCESZ * VDATA_PACKINGSIZE);
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"loadedges_splitdstvxs::loadoffsetmarkers: k: "<<k<<", index: "<<index<<", srC: "<<srC<<", SRC_CHKPT["<<k<<"]: "<<SRC_CHKPT[k]<<", PARTITION_CHKPT["<<k<<"]: "<<PARTITION_CHKPT[k]<<", _NUM_EDGECHUNKS_IN_A_BUFFER: "<<_NUM_EDGECHUNKS_IN_A_BUFFER<<endl; 
			#endif
		}
		SRC_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER] = KVDATA_RANGE-1;
		// PARTITION_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER] = (1 << (NUM_PARTITIONS_POW * (TREE_DEPTH-1))); // 256
		PARTITION_CHKPT[_NUM_EDGECHUNKS_IN_A_BUFFER] = KVDATA_RANGE / REDUCEPARTITIONSZ;
		
		for(unsigned int k=0; k<container->edgessize[i]; k++){
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(k % 100000 == 0){ cout<<"### loadedges_splitdstvxs::loadoffsetmarkers:: k: "<<k<<endl; }
			#endif
			
			if(edges_temp[i][k].status != EDGESTATUS_VALIDEDGE){ continue; }
			
			keyvalue_t keyvalue;
			keyvalue.key = READFROM_UINT(edges_temp[i][k].dstvid, 0, 28); // NEWCHANGE.
			unsigned int incr = READFROM_UINT(edges_temp[i][k].dstvid, 28, 4);
			keyvalue.value = 0;
			
			src2 = edges_temp[i][k].srcvid;
			#ifdef _DEBUGMODE_CHECKS
			if(k % VECTOR2_SIZE == 0){ if(k%1000000 == 0){ cout<<"loadedges_splitdstvxs::loadoffsetmarkers: src1: "<<src1<<", src2: "<<src2<<", k: "<<k<<", i: "<<i<<endl; }}
			#endif 
			
			for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
				unsigned int offset = 0;
				for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); } 
				
				unsigned int partitionCLOP = getglobalpartition(keyvalue, 0, BATCH_RANGE_POW, CLOP);
				
				#ifdef _DEBUGMODE_CHECKS
				utilityobj->checkoutofbounds("loadedges_splitdstvxs::loadoffsetmarkers.partitionCLOP.1", partitionCLOP, pow(NUM_PARTITIONS, TREE_DEPTH), edges_temp[i][k].dstvid, edges_temp[i][k].status, container->edgessize[i]);
				utilityobj->checkoutofbounds("loadedges_splitdstvxs::loadoffsetmarkers.partitionCLOP.2", offset + partitionCLOP, _KVSTATSDRAMSZ, keyvalue.key, container->edgessize[i], NAp);
				#endif
				
				#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
				unsigned int p=0;
				for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ if(src2 >= SRC_CHKPT[u] && src2 < SRC_CHKPT[u+1]){ p = u; } }
				#ifdef _DEBUGMODE_CHECKS
				utilityobj->checkoutofbounds("loadedges_splitdstvxs::loadoffsetmarkers.p.1", p, _NUM_EDGECHUNKS_IN_A_BUFFER, keyvalue.key, vptrs[i][TWOO*_BASEOFFSET_VERTEXPTR + keyvalue.key].key, NAp);
				#endif
				tempstats[p][offset + partitionCLOP].value += 1;
				#else 
				tempstats[0][offset + partitionCLOP].value += 1;	
				#endif 
			}
		}
		
		for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
			unsigned int offset = 0;
			for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); }
			
			utilityobj->checkoutofbounds("loadedges_splitdstvxs::loadoffsetmarkers.offset", offset, KVSTATSDRAMSZ, NAp, NAp, NAp);
			for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ utilityobj->getmarkerpositions((keyvalue_t *)&tempstats[u][offset], (unsigned int)pow(NUM_PARTITIONS, CLOP)); }
		}
		
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_STATSDRAM];
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){
			for(unsigned int k=0; k<_KVSTATSSZ; k++){
				statsptrVec[k].data[u].key = tempstats[u][k].key;
				statsptrVec[k].data[u].value = 0;
				// statsptrVec[k].data[u].value = tempstats[u][k].value; // CRITICAL REMOVEME.
			}
		}
		
		uint512_vec_dt * edgesstatsptrVec = (uint512_vec_dt *)&stats[i][_BASEOFFSET_EDGESSTATSDRAM];
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER+1; u++){
			edgesstatsptrVec[u].data[0].key = PARTITION_CHKPT[u];
			edgesstatsptrVec[u].data[0].value = 0;
		}
		
		unsigned int totalnumpb4llop = 0;
		#ifdef ENABLERECURSIVEPARTITIONING
			for(unsigned int k=0; k<TREE_DEPTH; k++){ totalnumpb4llop += (unsigned int)pow(NUM_PARTITIONS, k); } 
			#else 
			for(unsigned int k=0; k<TREE_DEPTH+1; k++){ totalnumpb4llop += (unsigned int)pow(NUM_PARTITIONS, k); } 	// NOTE: because 
			#endif
		for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){
			if(maxdramsz < (tempstats[u][totalnumpb4llop-1].key + tempstats[u][totalnumpb4llop-1].value)){ maxdramsz = tempstats[u][totalnumpb4llop-1].key + tempstats[u][totalnumpb4llop-1].value; }
		}
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadoffsetmarkers:: maxdramsz: "<<maxdramsz<<endl;
		#endif 
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats [after]", tempstats[i], totalnumpb4llop);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing statsptrVec [after]", (keyvalue_t *)statsptrVec, totalnumpb4llop * VECTOR_SIZE, VECTOR_SIZE);
		cout<<"tempstats["<<i<<"]["<<totalnumpb4llop-1<<"].key: "<<tempstats[i][totalnumpb4llop-1].key<<", tempstats["<<i<<"]["<<totalnumpb4llop-1<<"].value: "<<tempstats[i][totalnumpb4llop-1].value<<endl;
		#endif
		#ifdef _DEBUGMODE_HOSTPRINTS3
		if(i==0){ for(unsigned int u=0; u<_NUM_EDGECHUNKS_IN_A_BUFFER; u++){ cout<<"loadoffsetmarkers: u:"<<u<<endl; utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats[u] [after]", tempstats[u], 17); } }
		#endif
	}
	// exit(EXIT_SUCCESS); ////
	
	globalparams.globalparamsK.SIZE_KVDRAM = maxdramsz + 64; // CHECKME. FIXME.
	globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE = globalparams.globalparamsK.SIZE_KVDRAM; // maxdramsz;
	globalparams.globalparamsK.SIZE_OTHERINFOS = 0; // 64 * VECTOR_SIZE;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.SIZE_KVDRAM = 0; 
	globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE = 0;
	globalparams.globalparamsE.SIZE_OTHERINFOS = 0;
	#endif  
	
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM = globalparams.globalparamsK.BASEOFFSETKVS_EDGESSTATSDRAM + globalparams.globalparamsK.SIZE_EDGESSTATSDRAM; 
	globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM + (globalparams.globalparamsK.SIZE_KVDRAM / VECTOR_SIZE); 
	globalparams.globalparamsK.BASEOFFSETKVS_OTHERINFOS = globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.globalparamsK.SIZE_KVDRAMWORKSPACE / VECTOR_SIZE); 
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM = globalparams.globalparamsE.BASEOFFSETKVS_EDGESSTATSDRAM + globalparams.globalparamsE.SIZE_EDGESSTATSDRAM; 
	globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE = globalparams.globalparamsE.BASEOFFSETKVS_KVDRAM + (globalparams.globalparamsE.SIZE_KVDRAM / VECTOR_SIZE);
	globalparams.globalparamsE.BASEOFFSETKVS_OTHERINFOS = globalparams.globalparamsE.BASEOFFSETKVS_KVDRAMWORKSPACE + (globalparams.globalparamsE.SIZE_KVDRAMWORKSPACE / VECTOR_SIZE); 
	#endif
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_KVDRAM", globalparams.globalparamsK.BASEOFFSETKVS_KVDRAM, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	utilityobj->checkoutofbounds("loadoffsetmarkers.BASEOFFSETKVS_KVDRAMWORKSPACE", globalparams.globalparamsK.BASEOFFSETKVS_KVDRAMWORKSPACE, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS3
	for(unsigned int i=0; i<1; i++){
		utilityobj->printkeyvalues("loadoffsetmarkers: printing stats[i][_BASEOFFSET_STATSDRAM]", (keyvalue_t *)&stats[i][_BASEOFFSET_STATSDRAM], (1+16) * VECTOR_SIZE, VECTOR_SIZE);
		// utilityobj->printkeyvalues("loadoffsetmarkers: printing stats[i][_BASEOFFSET_STATSDRAM]", (keyvalue_t *)&stats[i][_BASEOFFSET_STATSDRAM], totalnumpartitions * VECTOR_SIZE, VECTOR_SIZE);
	}
	#endif
	// exit(EXIT_SUCCESS); //
	return globalparams;
}

globalparams_TWOt loadedges_splitdstvxs::generatevmaskdata(vector<vertex_t> &activevertices, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], uint512_vec_dt * vdram, globalparams_TWOt globalparams){ 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadedges_splitdstvxs::generatevmaskdata:: generating vmask... "<<endl;
	#endif
	
	globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.globalparamsK.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.globalparamsK.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsK.SIZE_VERTICESDATAMASK = VERTICESDATAMASKSZ;
	
	globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.globalparamsK.SIZE_VERTICESDATAMASK / VECTOR2_SIZE);
	globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK = (NUMPROCESSEDGESPARTITIONS * 2) * VECTOR_SIZE; // '*2' is padding value. AUTOMATEME.
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.globalparamsE.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.globalparamsE.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsE.SIZE_VERTICESDATAMASK = 0;
	
	globalparams.globalparamsE.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsE.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.globalparamsE.SIZE_VERTICESDATAMASK / VECTOR2_SIZE);
	globalparams.globalparamsE.SIZE_VERTICESPARTITIONMASK = 0;
	#endif 
	
	globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK = globalparams.globalparamsV.BASEOFFSETKVS_ACTIVEVERTICES + ((globalparams.globalparamsV.SIZE_ACTIVEVERTICES/NUMINTSINKEYVALUETYPE) / VECTOR_SIZE);
	globalparams.globalparamsV.SIZE_VERTICESDATAMASK = VERTICESDATAMASKSZ;
	
	globalparams.globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK = globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK + (globalparams.globalparamsV.SIZE_VERTICESDATAMASK / VECTOR2_SIZE);
	globalparams.globalparamsV.SIZE_VERTICESPARTITIONMASK = (NUMPROCESSEDGESPARTITIONS * 2) * VECTOR_SIZE;
	
	unsigned int _BASEOFFSET_VERTICESDATAMASK_KVS = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK;
	unsigned int _BASEOFFSET_VERTICESPARTITIONMASK_KVS = globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK;
	unsigned int _BASEOFFSETV_VERTICESDATAMASK_KVS = globalparams.globalparamsV.BASEOFFSETKVS_VERTICESDATAMASK;
	unsigned int _BASEOFFSETV_VERTICESPARTITIONMASK_KVS = globalparams.globalparamsV.BASEOFFSETKVS_VERTICESPARTITIONMASK; // NEWCHANGE.
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("generatevmaskdata.BASEOFFSETKVS_VERTICESDATAMASK", globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	utilityobj->checkoutofbounds("generatevmaskdata.BASEOFFSETKVS_VERTICESPARTITIONMASK", globalparams.globalparamsK.BASEOFFSETKVS_VERTICESPARTITIONMASK, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);				
	#endif 
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"[globalparams.globalparamsK.BASEOFFSET_VERTICESDATAMASK: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK * VECTOR_SIZE<<"]"<<endl;
	cout<<"[globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK: "<<globalparams.globalparamsK.BASEOFFSETKVS_VERTICESDATAMASK<<"]"<<endl;
	#endif
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		for(unsigned int partition=0; partition<NUMPROCESSEDGESPARTITIONS; partition++){
			for(unsigned int k=0; k<VMASKBUFFERSZ_KVS; k++){
				#ifdef _DEBUGMODE_CHECKS3
				utilityobj->checkoutofbounds("loadgraph::generatevmaskdata 2._BASEOFFSET_VERTICESDATAMASK_KVS", _BASEOFFSET_VERTICESDATAMASK_KVS + k, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
				#endif
				for(unsigned int v=0; v<VECTOR_SIZE; v++){
					kvbuffer[i][_BASEOFFSET_VERTICESDATAMASK_KVS + (partition * VMASKBUFFERSZ_KVS) + k].data[v].key = 0;
					kvbuffer[i][_BASEOFFSET_VERTICESDATAMASK_KVS + (partition * VMASKBUFFERSZ_KVS) + k].data[v].value = 0;
				}
			}
		}
	}
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		for(unsigned int k=0; k<globalparams.globalparamsK.SIZE_VERTICESPARTITIONMASK; k++){
			#ifdef _DEBUGMODE_CHECKS3
			utilityobj->checkoutofbounds("loadedges::generatevmaskdata 2._BASEOFFSET_VERTICESPARTITIONMASK_KVS", _BASEOFFSET_VERTICESPARTITIONMASK_KVS + k, PADDEDKVSOURCEDRAMSZ_KVS, NAp, NAp, NAp);
			#endif
			kvbuffer[i][_BASEOFFSET_VERTICESPARTITIONMASK_KVS + k].data[0].key = 0;
		}
		kvbuffer[i][_BASEOFFSET_VERTICESPARTITIONMASK_KVS].data[0].key = 1; 
	}
	return globalparams;
}

void loadedges_splitdstvxs::savevdata(uint512_vec_dt * kvdram, unsigned int buffer[VDATA_PACKINGSIZE][REDUCEPARTITIONSZ_KVS2], batch_type vbaseoffset_kvs, batch_type voffset_kvs){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadedges_splitdstvxs::savemasks:: saving vdata: voffset_kvs: "<<voffset_kvs<<endl;
	#endif 
	for(unsigned int i=0; i<REDUCEPARTITIONSZ_KVS2; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram[vbaseoffset_kvs + voffset_kvs + i].data[v].key = buffer[2*v][i];
			kvdram[vbaseoffset_kvs + voffset_kvs + i].data[v].value = buffer[2*v+1][i];
		}
	}
	return;
}
void loadedges_splitdstvxs::savemasks(uint512_vec_dt * kvdram, unit1_type vmaskBITS[VMASK_PACKINGSIZE][REDUCEPARTITIONSZ_KVS2], batch_type vmask_baseoffset_kvs, batch_type vmask_offset_kvs){			
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadedges_splitdstvxs::savemasks:: saving vmasks: vmask_offset_kvs: "<<vmask_offset_kvs<<endl;
	#endif
	/** WRITETO_UINT(&data, 28, 4, 1) => 010000000000000000000000000000
	WRITETO_UINT(&data, 28, 4, 2) => 100000000000000000000000000000
	WRITETO_UINT(&data, 28, 4, 3) => 110000000000000000000000000000 */
	
	vmask_offset_kvs = vmask_offset_kvs / FACTOR_REDUCEPARTITIONSZ_VMASKSZ; // convert
	
	uint32_type tempbuffer[REDUCEPARTITIONSZ_KVS2];
	unsigned int indexx = 0;
	
	for(buffer_type k=0; k<REDUCEPARTITIONSZ_KVS2; k+=2){
		unsigned int n_i = k;
		unsigned int n_iplus1 = k + 1;
		
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			utilityobj->WRITETO_UINT(&tempbuffer[indexx], v, 1, vmaskBITS[v][n_i]);
		}
		for(unsigned int v=0; v<VECTOR2_SIZE; v++){
			utilityobj->WRITETO_UINT(&tempbuffer[indexx], 16+v, 1, vmaskBITS[v][n_iplus1]);
		}
		indexx += 1;
	}
	
	indexx = 0;
	for(buffer_type k=0; k<VMASKBUFFERSZ_KVS; k++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvdram[vmask_baseoffset_kvs + vmask_offset_kvs + k].data[v].key = tempbuffer[indexx];
			kvdram[vmask_baseoffset_kvs + vmask_offset_kvs + k].data[v].value = tempbuffer[indexx + 1];
			indexx += 2;
		}
	}
	
	#ifdef _DEBUGMODE_CHECKS3
	utilityobj->checkoutofbounds("loadedges_splitdstvxs::savemasks", vmask_offset_kvs + VMASKBUFFERSZ_KVS - 1, VERTICESDATAMASKSZ_KVS2, vmask_baseoffset_kvs, vmask_offset_kvs, VMASKBUFFERSZ_KVS);
	#endif
	return;
}
void loadedges_splitdstvxs::setrootvid(uint512_vec_dt * kvbuffer, vector<vertex_t> &activevertices, globalparams_t globalparams){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::setrootvid:: setting root vid(s)... "<<endl;
	#endif 

	unsigned int vbuffer[VDATA_PACKINGSIZE][REDUCEPARTITIONSZ_KVS2];
	unit1_type vmask[VDATA_PACKINGSIZE][REDUCEPARTITIONSZ_KVS2];
	for(unsigned int i=0; i<NUM_PEs; i++){
		for(unsigned int partition=0; partition<NUMREDUCEPARTITIONS; partition++){
			for(unsigned int k=0; k<REDUCEPARTITIONSZ_KVS2; k++){
				for(unsigned int v=0; v<VDATA_PACKINGSIZE; v++){
					unsigned int lvid = (partition * REDUCEPARTITIONSZ_KVS2 * VDATA_PACKINGSIZE) + (k * VDATA_PACKINGSIZE) + v;
					unsigned int vid = utilityobj->UTIL_GETREALVID(lvid, i);
					
					vbuffer[v][k] = 0;
					if(vid == 1){
						#ifdef _DEBUGMODE_HOSTPRINTS
						cout<<"loadedges_splitdstvxs::setrootvid::(1) vid == 1 seen. i: "<<i<<", partition: "<<partition<<", k: "<<k<<", v: "<<v<<", lvid: "<<lvid<<", vid: "<<vid<<endl;
						#endif 
						
						#ifdef CONFIG_MERGEVMASKSWITHVBUFFERDATA
						utilityobj->WRITETO_UINT((unsigned int *)&vbuffer[v][k], SIZEOF_VDATA0, SIZEOF_VMASK0, 1);//1);
						utilityobj->WRITETO_UINT((unsigned int *)&vbuffer[v][k], 0, SIZEOF_VDATA0, 0);
						#else 
						vbuffer[v][k] = 0;
						#endif 
						vmask[v][k] = 1;
						
						// vbuffer[v][k] = 0;
						// vmask[v][k] = 1;
					} else {
						#ifdef CONFIG_MERGEVMASKSWITHVBUFFERDATA
						utilityobj->WRITETO_UINT((unsigned int *)&vbuffer[v][k], SIZEOF_VDATA0, SIZEOF_VMASK0, 0);
						utilityobj->WRITETO_UINT((unsigned int *)&vbuffer[v][k], 0, SIZEOF_VDATA0, MAXVDATA); //algorithmobj->vertex_initdata()); //32767
						#else 
						vbuffer[v][k] = algorithmobj->vertex_initdata();
						#endif 
						vmask[v][k] = 0;
						
						// vbuffer[v][k] = algorithmobj->vertex_initdata();
						// vmask[v][k] = 0;
					}
				}
			}
			savevdata(kvbuffer, vbuffer, globalparams.BASEOFFSETKVS_SRCVERTICESDATA, (i * NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2) + (partition * REDUCEPARTITIONSZ_KVS2));
			savemasks(kvbuffer, vmask, globalparams.BASEOFFSETKVS_VERTICESDATAMASK, (i * NUMREDUCEPARTITIONS * REDUCEPARTITIONSZ_KVS2) + (partition * REDUCEPARTITIONSZ_KVS2));
		}
	}
	return;
}



