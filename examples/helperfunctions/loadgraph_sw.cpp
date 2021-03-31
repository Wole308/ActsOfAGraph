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
#include "loadgraph_sw.h"
using namespace std;

loadgraph_sw::loadgraph_sw(graph * _graphobj, stats * _statsobj){
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	statsobj = _statsobj;
	evalparamsobj = new evalparams();
}
loadgraph_sw::loadgraph_sw(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
loadgraph_sw::~loadgraph_sw(){} 

void loadgraph_sw::loadedges_rowwise(graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_t * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS]){			
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph_sw::loadedges_rowwise:: loading edges (rowwise)... "<<endl;
	#endif 
	
	unsigned int counts[NUMSUBCPUTHREADS];
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ counts[i] = 0; }
	unsigned int index1 = 0;
	unsigned int index2 = 0;
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		vptrs[i][0] = 0; 
	}
	
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS
		if(vid % 100000 == 0){ cout<<"### loadgraph_sw::loadedges_rowwise:: vid: "<<vid<<", vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		#endif 
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		unsigned int subthread = 0;
		unsigned int k=0;
		while(k<edges_size){
			subthread = (index1 / VECTOR2_SIZE) % NUMSUBCPUTHREADS;
			
			#ifdef _DEBUGMODE_CHECKS2
			utilityobj->checkoutofbounds("loadgraph_sw::loadedges_rowwise 23. ", counts[subthread], graphobj->get_num_edges(), NAp, NAp, NAp);				
			#endif 
		
			edges[subthread][counts[subthread]].dstvid = edgedatabuffer[index2].dstvid;
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"k: "<<k<<", subthread: "<<subthread<<", counts["<<subthread<<"]: "<<counts[subthread]<<", index2: "<<index2<<", edgedatabuffer["<<index2<<"].dstvid: "<<edgedatabuffer[index2].dstvid<<endl;
			#endif
			
			counts[subthread] += 1;
			index1 += 1;
			index2 += 1;
			k += 1;
		}
		
		// load temp vertex ptrs 
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
			if(vid < KVDATA_RANGE-1){
				vptrs[i][vid + 1] = counts[i]; 
			}
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	unsigned int totalsize = 0;
	cout<<"loadedges_rowwise:: loading Acts... sizes: ["; 
	cout<<"["; for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ totalsize += counts[i]; cout<<counts[i]; if(i<NUMSUBCPUTHREADS-1){ cout<<", "; }} cout<<"]";
	cout<<"]"<<endl;
	cout<<"loadedges_rowwise:: total sizes: "<<totalsize<<endl; 
	#endif
	return;
}






