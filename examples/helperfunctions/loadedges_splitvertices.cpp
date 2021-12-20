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
#include "loadedges_splitvertices.h"
using namespace std;

// order of base addresses
// messages area {messages}
// edges area {edges, vertex ptrs} 
// vertices area {src vertices data, dest vertices data}
// actvvs area {active vertices, active vertices mask}
// stats area {stats, edge stats}
// workspace area {kvdram, kvdram workspace}

#define TWOO 2
#define NUMWS 16//NUM_PARTITIONS
#define _VECTOR2_SIZE VECTOR2_SIZE
#define MAX_NUM_UNIQ_EDGES_PER_VEC 8//4 // FIXME.
#define BITMAPCODE 0b10000000000000000000000000000000
#define BITMAPCODE_WITHHEADSRCV 0b11000000000000000000000000000000

#define FILLEMPTYVERTEXSLOTS
#define CHECK1
#define CHECK2
#define CHECK3
#define INSERTBITMAP

loadedges_splitvertices::loadedges_splitvertices(graph * _graphobj, stats * _statsobj){
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	statsobj = _statsobj;
	evalparamsobj = new evalparams();
}
loadedges_splitvertices::loadedges_splitvertices(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
loadedges_splitvertices::~loadedges_splitvertices(){} 

unsigned int loadedges_splitvertices::ACTSUTIL_GETMASK_UINT(unsigned int index, unsigned int size){
	unsigned int A = ((1 << (size)) - 1);
	unsigned int B = A << index;
	return B;
}
unsigned int loadedges_splitvertices::ACTSUTIL_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size){ 
	return (((data) & ACTSUTIL_GETMASK_UINT((index), (size))) >> (index)); 
}
void loadedges_splitvertices::ACTSUTIL_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value){
	unsigned int tempdata = *data;
	unsigned int A = ((value) << (index));
	unsigned int B = (~ACTSUTIL_GETMASK_UINT((index), (size)));
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

unsigned int loadedges_splitvertices::gethash(unsigned int vid){
	return vid % NUMWS;
}
unsigned int loadedges_splitvertices::allignlower(unsigned int val, unsigned int V_SIZE){
	unsigned int fac = val / V_SIZE;
	return (fac * V_SIZE);
}

globalparams_TWOt loadedges_splitvertices::loadedges(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams){			
	cout<<"loadedges_splitvertices::loadedges:: loading edges (rowwise)... "<<endl;
	
	///////
	globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsK.SIZE_MESSAGESDRAM;
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_MESSAGESDATA + globalparams.globalparamsE.SIZE_MESSAGESDRAM;
	#endif  
	
	// CRITICAL REMOVEME.
	globalparams.globalparamsK.BASEOFFSETKVS_VERTEXPTR = 0;//globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA;
	globalparams.globalparamsK.SIZE_VERTEXPTRS = 0; 
	////////
	
	// vector edge2_type * edgedatabuffers_temp[NUMWS];
	// vector<edge_type> edgedatabuffers[NUMWS];
	vector<edge2_type> edgedatabuffers_temp[NUMWS];
	// for(unsigned int i=0; i<NUMWS; i++){ edgedatabuffers_temp[i] = new edge2_type[(graphobj->getedgessize(0) / NUMWS)]; }
	// unsigned int total_num_edges = graphobj->getedgessize(0);
	
	#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsE.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	globalparams_t globalparamsVPTRS = globalparams.globalparamsE;
	#else 
	unsigned int _BASEOFFSET_EDGESDATA = globalparams.globalparamsK.BASEOFFSETKVS_EDGESDATA * VECTOR_SIZE;
	globalparams_t globalparamsVPTRS = globalparams.globalparamsK;
	#endif 
	unsigned int _BASEOFFSET_VERTEXPTR = globalparamsVPTRS.BASEOFFSETKVS_VERTEXPTR * VECTOR_SIZE;
	
	unsigned int * counts_validedges[NUMSUBCPUTHREADS];
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		counts_validedges[i] = new unsigned int[KVDATA_RANGE];
		for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){ counts_validedges[i][vid] = 0; }
	}
	unsigned int isFirst = 0;
	unsigned int isLast = 0;
	
	// calculate counts_validedges 
	unsigned int total_counts_fori[NUMWS]; for(unsigned int i=0; i<NUMWS; i++){ total_counts_fori[i] = 0; }
	unsigned int total_counts = 0;
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS3
		if(vid % 700000 == 0){ cout<<"### loadedges_splitvertices::loadedges:: vid: "<<vid<<", vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
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
	
	#ifdef FILLEMPTYVERTEXSLOTS
	vector<edge2_type> edgedatabuffers_temp2[NUMWS];
	edge2_type dummyedge; dummyedge.srcvid = INVALIDDATA; dummyedge.dstvid = INVALIDDATA;
	unsigned int gap = 0;
	unsigned int totalgap = 0;
	for(unsigned int i=0; i<NUMWS; i++){
		gap = 0;
		edgedatabuffers_temp2[i].push_back(edgedatabuffers_temp[i][0]);
		for(unsigned int k=1; k<edgedatabuffers_temp[i].size(); k++){
			unsigned int currsrcvid = edgedatabuffers_temp[i][k].srcvid;
			unsigned int prevsrcvid = edgedatabuffers_temp[i][k-1].srcvid;
			if(currsrcvid >= prevsrcvid){
				if(currsrcvid > prevsrcvid + 1){
					for(unsigned int m=prevsrcvid+1; m<currsrcvid; m++){
						if(i==0 && gap < 8){ cout<<"loadedges_splitvertices::loadedges:: prevsrcvid: "<<prevsrcvid<<" currsrcvid: "<<currsrcvid<<", m: "<<m<<endl; }
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
				cout<<"### loadedges_splitvertices::loadedges:: ERROR: currsrcvid("<<currsrcvid<<") < prevsrcvid("<<prevsrcvid<<") "<<endl;
				exit(EXIT_FAILURE);
			}
		}
		cout<<"### loadedges_splitvertices::loadedges:: number of empty vertices filled for memory channel "<<i<<": "<<gap<<endl;
	}
	cout<<"### loadedges_splitvertices::loadedges:: total number of empty vertices filled for all memory channels: "<<totalgap<<endl;
	for(unsigned int i=0; i<NUMWS; i++){ cout<<"edgedatabuffers_temp2["<<i<<"].size(): "<<edgedatabuffers_temp2[i].size()<<endl; }
	for(unsigned int i=0; i<NUMWS; i++){ cout<<"edgedatabuffers_temp["<<i<<"].size(): "<<edgedatabuffers_temp[i].size()<<endl; }
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// CHECK 1: ensure all vertices are represented
	#ifdef CHECK1
	for(unsigned int i=0; i<NUMWS; i++){
		for(unsigned int k=1; k<edgedatabuffers_temp2[i].size(); k++){
			unsigned int currsrcvid = edgedatabuffers_temp2[i][k].srcvid;
			unsigned int prevsrcvid = edgedatabuffers_temp2[i][k-1].srcvid;
			if(currsrcvid > prevsrcvid + 1 || currsrcvid < prevsrcvid){
				cout<<"### loadedges_splitvertices::loadedges:: CHECK 1: ERROR CHECKING FAILED: k:"<<k<<", edgedatabuffers_temp2["<<i<<"].size(): "<<edgedatabuffers_temp2[i].size()<<", (currsrcvid("<<currsrcvid<<") > prevsrcvid("<<prevsrcvid<<") + 1 || currsrcvid("<<currsrcvid<<") < prevsrcvid("<<prevsrcvid<<")). EXITING..."<<endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	cout<<"### loadedges_splitvertices::loadedges:: CHECK 1: ERROR CHECKING SUCCESSFUL."<<endl;
	#endif 
	// exit(EXIT_SUCCESS); //

	// partition edges into memory channels according to edge representation format
	cout<<"### loadedges_splitvertices::loadedges:: partitioning edges into memory channels according to edge representation format..."<<endl;
	unsigned int tempe_index = 0; 
	unsigned int index = 0;
	unsigned int counts_alldatas[NUMWS]; for(unsigned int i=0; i<NUMWS; i++){ counts_alldatas[i] = 0; }
	unsigned int counts_alldata = 0;
	unsigned int srcvid_lastvechead = 0xFFFFFFFF;
	unsigned int srcvid_lastseen;
	bool debug = false; // true;
	vector<edge2_type> edges_temp[NUMSUBCPUTHREADS]; for(unsigned int j=0; j<NUMSUBCPUTHREADS; j++){ edges_temp[j].clear(); }
	for(unsigned int i=0; i<NUMWS; i++){ // NUMWS // FIXME
		tempe_index = 0; 
		index = 0;
		while(tempe_index < edgedatabuffers_temp2[i].size()){
			if(tempe_index % 1000000 == 0 && false){ cout<<"loadedges_splitvertices::loadedges:: filling edges... tempe_index: "<<tempe_index<<endl; }
			edge2_type edge = edgedatabuffers_temp2[i][tempe_index];
			edge2_type last_edge; if(tempe_index==0){ last_edge = edge; } else { last_edge = edgedatabuffers_temp2[i][tempe_index-1]; }
			if(debug==true){ cout<<">>> edge.srcvid: "<<edge.srcvid<<", edge.dstvid: "<<edge.dstvid<<" [-]"<<endl; }
			
			if(index%VECTOR2_SIZE == 0 || index%VECTOR2_SIZE == 1){
				if(index%VECTOR2_SIZE == 0){
					// insert bitmap
					edge2_type edge_temp; edge_temp.srcvid = BITMAPCODE; edge_temp.dstvid = BITMAPCODE; edges_temp[i].push_back(edge_temp);
					index += 1;
				} else {
					if(edge.srcvid == srcvid_lastvechead){
						// insert edge at head (for optimal space efficiency)
						edge2_type edge_temp; edge_temp.srcvid = edge.srcvid; edge_temp.dstvid = edge.dstvid; edges_temp[i].push_back(edge_temp);
						if(debug==true){ cout<<">>> edge_temp.srcvid: "<<edge_temp.srcvid<<", edge_temp.dstvid: "<<edge_temp.dstvid<<" [1]"<<endl; }
						srcvid_lastseen = edge_temp.srcvid;
						index += 1;
						tempe_index += 1;
						counts_validedges[i][edge.srcvid] += 1;
					} else {
						// insert source at head
						edges_temp[i][edges_temp[i].size()-1].srcvid = BITMAPCODE_WITHHEADSRCV; edges_temp[i][edges_temp[i].size()-1].dstvid = BITMAPCODE_WITHHEADSRCV; // UPDATE BITMAP. DON"T REMOVE FROM HERE.
						edge2_type edge_temp; edge_temp.srcvid = edge.srcvid; edge_temp.dstvid = edge.srcvid; edges_temp[i].push_back(edge_temp);
						if(debug==true){ cout<<">>> edge_temp.srcvid: "<<edge_temp.srcvid<<", edge_temp.dstvid: "<<edge_temp.dstvid<<" [2]"<<endl; }
						srcvid_lastvechead = edge.srcvid;
						srcvid_lastseen = edge_temp.srcvid;
						index += 1;
					}
				}
			} else {
				unsigned int numuniqsrcvssincehead = 0;
				for(unsigned int k=0; k<index-allignlower(index, VECTOR2_SIZE); k++){ if(edges_temp[i][index-k].srcvid != edges_temp[i][index-k-1].srcvid){ numuniqsrcvssincehead += 1; }}
				// for(unsigned int k=0; k<index-allignlower(index, VECTOR2_SIZE); k++){ if(edgedatabuffers_temp[i][tempe_index-k].srcvid != edgedatabuffers_temp[i][tempe_index-k-1].srcvid){ numuniqsrcvssincehead += 1; }} // REMOVEME
				if(numuniqsrcvssincehead <= MAX_NUM_UNIQ_EDGES_PER_VEC+1){ //'+1 to account for bitmask head'
					// insert edge
					edge2_type edge_temp; edge_temp.srcvid = edge.srcvid; edge_temp.dstvid = edge.dstvid; edges_temp[i].push_back(edge_temp);
					if(debug==true){ cout<<">>> edge_temp.srcvid: "<<edge_temp.srcvid<<", edge_temp.dstvid: "<<edge_temp.dstvid<<" [3]"<<endl; }
					srcvid_lastseen = edge_temp.srcvid;
					tempe_index += 1;
					index += 1;
					counts_validedges[i][edge.srcvid] += 1;
				} else {
					// fill with dummy edges
					for(unsigned int k=index%VECTOR2_SIZE; k<VECTOR2_SIZE; k++){
						edge2_type edge_temp; edge_temp.srcvid = srcvid_lastseen; edge_temp.dstvid = INVALIDDATA; edges_temp[i].push_back(edge_temp);
						if(debug==true){ cout<<">>> edge_temp.srcvid: "<<edge_temp.srcvid<<", edge_temp.dstvid: "<<edge_temp.dstvid<<" [4]"<<endl; }
						index += 1;
					}
				}
			}
		}
		
		counts_alldatas[i] += index;
		counts_alldata += index;
		
		utilityobj->printkeyvalues("loadedges_splitvertices[after]::loadedges: printing edges_temp["+std::to_string(i)+"][~]", (keyvalue_t *)&edges_temp[i][0], 4);
		cout<<"### loadedges_splitvertices::loadedges:: memory channel "<<i<<": tempe_index: "<<tempe_index<<", index: "<<index<<endl;
	}
	utilityobj->printvalues(">>> loadedges_splitvertices[after]::loadedges: total number of edges in channels[0-N]", (value_t *)&counts_alldatas[0], NUMWS);
	cout<<">>> loadedges_splitvertices::loadedges:: total number of edges in all memory channels: counts_alldata: "<<counts_alldata<<", NAp: "<<NAp<<endl;
	// exit(EXIT_SUCCESS); //
	
	// CHECK 2: ensure all vertices are represented
	#ifdef CHECK2
	for(unsigned int i=0; i<NUMWS; i++){
		for(unsigned int j=0; j<edges_temp[i].size(); j+=VECTOR2_SIZE){
			for(unsigned int v=2; v<VECTOR2_SIZE; v++){
				if(j+v >= edges_temp[i].size()){ continue; }
				unsigned int currsrcvid = edges_temp[i][j+v].srcvid;
				unsigned int prevsrcvid = edges_temp[i][j+v-1].srcvid;
				if(currsrcvid > prevsrcvid + 1 || currsrcvid < prevsrcvid){
					cout<<"### loadedges_splitvertices::loadedges:: CHECK 2: ERROR CHECKING FAILED: ind:"<<j+v<<", (currsrcvid("<<currsrcvid<<") > prevsrcvid("<<prevsrcvid<<") + 1 || currsrcvid("<<currsrcvid<<") < prevsrcvid("<<prevsrcvid<<")). EXITING..."<<endl;
					for(unsigned int v1=0; v1<VECTOR2_SIZE; v1++){
						cout<<v1<<": edges_temp["<<i<<"]["<<j+v1<<"].srcvid: "<<edges_temp[i][j+v1].srcvid<<", edges_temp["<<i<<"]["<<j+v1<<"].dstvid: "<<edges_temp[i][j+v1].dstvid<<endl;
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	cout<<"### loadedges_splitvertices::loadedges:: CHECK 2: ERROR CHECKING SUCCESSFUL."<<endl;
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// CHECK 3: check to make sure no vector has more than 4 distinct srcvids
	#ifdef CHECK3
	edge2_type temp[VECTOR2_SIZE];
	for(unsigned int i=0; i<NUMWS; i++){
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
				if(num_uniq_scrvids > MAX_NUM_UNIQ_EDGES_PER_VEC+1){ // MAX_NUM_UNIQ_EDGES_PER_VEC // FIXME.
					cout<<"### loadedges_splitvertices::loadedges:: CHECK 3: ERROR CHECKING FAILED: num_uniq_scrvids("<<num_uniq_scrvids<<") > MAX_NUM_UNIQ_EDGES_PER_VEC("<<MAX_NUM_UNIQ_EDGES_PER_VEC<<"). index:"<<j+v<<". EXITING..."<<endl;
					for(unsigned int v=0; v<VECTOR2_SIZE; v++){
						cout<<""<<v<<": edges_temp["<<i<<"]["<<j+v<<"].srcvid: "<<edges_temp[i][j+v].srcvid<<", edges_temp["<<i<<"]["<<j+v<<"].dstvid: "<<edges_temp[i][j+v].dstvid<<endl;
					}
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	cout<<"### loadedges_splitvertices::loadedges:: CHECK 3: ERROR CHECKING SUCCESSFUL."<<endl;
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// insert bitmap 
	#ifdef INSERTBITMAP
	cout<<"### loadedges_splitvertices::loadedges:: inserting bitmap..."<<endl;
	for(unsigned int i=0; i<NUMWS; i++){
		for(unsigned int j=0; j<edges_temp[i].size(); j+=VECTOR2_SIZE){
			unsigned int bitmapp = edges_temp[i][j].dstvid;
			unsigned int srcvid_head = edges_temp[i][j+1].srcvid;
			for(unsigned int v=1; v<VECTOR2_SIZE; v++){
				if(j+v >= edges_temp[i].size()){ continue; }
				if(v==1){
					// DO NOTHING.
				} else {
					if(edges_temp[i][j+v].dstvid != INVALIDDATA){
						// edge was inserted here
						unsigned int diff = edges_temp[i][j+v].srcvid - edges_temp[i][j+1].srcvid; // srcvid_head;
						unsigned int loc = v;
						if(diff > MAX_NUM_UNIQ_EDGES_PER_VEC){ //3,4 // FIXME.
							cout<<"ERROR 3. diff("<<diff<<") > 3. [edges_temp["<<i<<"]["<<j+v<<"].srcvid: "<<edges_temp[i][j+v].srcvid<<", edges_temp["<<i<<"]["<<j+1<<"].srcvid: "<<edges_temp[i][j+1].srcvid<<"]. exiting..."<<endl; 
							for(unsigned int v=0; v<VECTOR2_SIZE; v++){
								cout<<""<<v<<": edges_temp["<<i<<"]["<<j+v<<"].srcvid: "<<edges_temp[i][j+v].srcvid<<", edges_temp["<<i<<"]["<<j+v<<"].dstvid: "<<edges_temp[i][j+v].dstvid<<endl;
							}
							exit(EXIT_FAILURE); }
						if(loc > 15){ cout<<"ERROR 3. loc("<<loc<<") > 31. [edges_temp["<<i<<"]["<<j+v<<"].srcvid: "<<edges_temp[i][j+v].srcvid<<", edges_temp["<<i<<"]["<<j+1<<"].srcvid: "<<edges_temp[i][j+1].srcvid<<"]. exiting..."<<endl; exit(EXIT_FAILURE); }
						ACTSUTIL_WRITETO_UINT(&bitmapp, loc*2, 2, diff);
					} else {
						// dummy was inserted here
						unsigned int diff = 0;
						unsigned int loc = v;
						if(diff > 3){ cout<<"ERROR 4. diff("<<diff<<") > 3. exiting..."<<endl; exit(EXIT_FAILURE); }
						if(loc > 15){ cout<<"ERROR 4. loc("<<loc<<") > 31. exiting..."<<endl; exit(EXIT_FAILURE); }
						ACTSUTIL_WRITETO_UINT(&bitmapp, loc*2, 2, diff);
					}
				}
			}
		}
	}
	cout<<"### loadedges_splitvertices::loadedges:: bitmap inserted successfully"<<endl;
	#endif 
	// exit(EXIT_SUCCESS); //
	
	// load edges 
	for(unsigned int i=0; i<NUMWS; i++){
		for(unsigned int k=0; k<edges_temp[i].size(); k++){
			edges[i][TWOO*_BASEOFFSET_EDGESDATA + k].dstvid = edges_temp[i][k].dstvid;
		}
		utilityobj->printvalues("loadedges_splitvertices[after]::loadedges: printing edges["+std::to_string(i)+"][~]", (value_t *)&edges[i][TWOO*_BASEOFFSET_EDGESDATA], 4);
	}
		
	for(unsigned int i=0; i<NUMWS; i++){ for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){ total_counts_fori[i] += counts_validedges[i][vid]; total_counts += counts_validedges[i][vid]; }}
	utilityobj->printvalues(">>> loadedges_splitvertices[after]::loadedges: printing edge counts_validedges for each memory channel", (value_t *)&total_counts_fori[0], NUMWS);
	cout<<"loadedges_splitvertices[after]::loadedges: total_counts: "<<total_counts<<endl;
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_splitvertices[after]::loadedges: printing counts_validedges["+std::to_string(i)+"][~]", (value_t *)&counts_validedges[i][0], 4); } // NUMWS
	for(unsigned int i=0; i<0; i++){ utilityobj->printkeyvalues("loadedges_splitvertices[after]::loadedges: printing edgedatabuffers_temp["+std::to_string(i)+"][~]", (keyvalue_t *)&edgedatabuffers_temp[i][0], 4); }
	utilityobj->printvalues(">>> loadedges_splitvertices[after]::loadedges: total number of edges in channels[0-N]", (value_t *)&counts_alldatas[0], NUMWS);
	cout<<">>> loadedges_splitvertices::loadedges:: total number of edges in all memory channels: counts_alldata: "<<counts_alldata<<", NAp: "<<NAp<<endl;
	exit(EXIT_SUCCESS); //
	
	// calculate offsets
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		vptrs[i][TWOO*_BASEOFFSET_VERTEXPTR + 0].key = 0;
		for(unsigned int vid=1; vid<KVDATA_RANGE; vid++){	
			vptrs[i][TWOO*_BASEOFFSET_VERTEXPTR + vid].key = vptrs[i][TWOO*_BASEOFFSET_VERTEXPTR + vid-1].key + counts_validedges[i][vid-1]; 
		}
	}
	return globalparams;
}


