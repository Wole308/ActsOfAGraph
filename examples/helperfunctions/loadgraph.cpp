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
#include "../../examples/helperfunctions/helperfunctions2.h"
#include "../../src/parameters/parameters.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/stats/stats.h"
#include "../../kernels/kernel.h"
#include "../../acts/sortreduce/sr.h" // change to sr
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "loadgraph.h"
using namespace std;

loadgraph::loadgraph(graph * _graphobj, stats * _statsobj){
	parametersobj = new parameters();
	utilityobj = new utility();
	graphobj = _graphobj;
	algorithmobj = new algorithm();
	kernelobj = new kernel();
	#ifdef GRAFBOOST_SETUP
	srkernelobj = new sr();
	#endif
	statsobj = _statsobj;
	helperfunctionsobj = new helperfunctions2(graphobj, statsobj); 
}
loadgraph::loadgraph(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
	kernelobj = new kernel();
	#ifdef GRAFBOOST_SETUP
	srkernelobj = new sr();
	#endif 
}
loadgraph::~loadgraph(){} 

void loadgraph::loadvertexdata(edge_t * vertexptrbuffer, value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS]){
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		vertexptrbuffer = graphobj->loadvertexptrsfromfile(j);
		for(unsigned int k=0; k<KVDATA_RANGE; k++){
			kvbuffer[j][BASEOFFSET_VERTICESDATA + k].key = vertexptrbuffer[k];
			kvbuffer[j][BASEOFFSET_VERTICESDATA + k].value = vertexdatabuffer[k];
		}
		for(unsigned int m=0; m<SRCBUFFER_SIZE*VECTOR_SIZE; m++){ // safe padding to avoid error in ACTS
			kvbuffer[j][BASEOFFSET_VERTICESDATA + KVDATA_RANGE - 1 + m].key = kvbuffer[j][BASEOFFSET_VERTICESDATA + KVDATA_RANGE - 1].key;
			kvbuffer[j][BASEOFFSET_VERTICESDATA + KVDATA_RANGE - 1 + m].value = kvbuffer[j][BASEOFFSET_VERTICESDATA + KVDATA_RANGE - 1].value;
		}
		
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printkeyvalues("loadgraph::loadvertexdata", &kvbuffer[j][BASEOFFSET_VERTICESDATA], 16);
		utilityobj->printkeyvalues("loadgraph::loadvertexdata", &kvbuffer[j][BASEOFFSET_VERTICESDATA+KVDATA_RANGE-16], 16);
		#endif 
	}
	return;
}
void loadgraph::loadedgedata(edge_type * edgedatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS]){
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		edgedatabuffer = graphobj->loadedgesfromfile(j);
		unsigned int edgessz = graphobj->getedgessize(j);
		for(unsigned int k=0; k<edgessz; k++){
			kvbuffer[j][BASEOFFSET_EDGESDATA + k].key = edgedatabuffer[k].srcvid;
			kvbuffer[j][BASEOFFSET_EDGESDATA + k].value = edgedatabuffer[k].dstvid;
		}
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printkeyvalues("loadgraph::loadedgedata", &kvbuffer[j][BASEOFFSET_EDGESDATA], 16);
		utilityobj->printkeyvalues("loadgraph::loadedgedata", &kvbuffer[j][BASEOFFSET_EDGESDATA+edgessz-16], 16);
		#endif 
	}
	return;
}

void loadgraph::loadvertexdata_even(edge_t * vertexptrbuffer, value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		for(unsigned int k=0; k<KVDATA_RANGE; k++){
			kvbuffer[j][BASEOFFSET_VERTICESDATA + k].key = vertexptrbuffer[k];
			kvbuffer[j][BASEOFFSET_VERTICESDATA + k].value = vertexdatabuffer[k];
		}
		for(unsigned int m=0; m<SRCBUFFER_SIZE*VECTOR_SIZE; m++){ // safe padding to avoid error in ACTS
			kvbuffer[j][BASEOFFSET_VERTICESDATA + KVDATA_RANGE - 1 + m].key = kvbuffer[j][BASEOFFSET_VERTICESDATA + KVDATA_RANGE - 1].key;
			kvbuffer[j][BASEOFFSET_VERTICESDATA + KVDATA_RANGE - 1 + m].value = kvbuffer[j][BASEOFFSET_VERTICESDATA + KVDATA_RANGE - 1].value;
		}
		
		#ifdef _DEBUGMODE_HOSTPRINTS2X
		utilityobj->printkeyvalues("loadgraph::loadvertexdata::first", &kvbuffer[j][BASEOFFSET_VERTICESDATA], 16);
		utilityobj->printkeyvalues("loadgraph::loadvertexdata::last", &kvbuffer[j][BASEOFFSET_VERTICESDATA+KVDATA_RANGE-16], 16);
		#endif 
	}
	return;
}
/* void loadgraph::loadedgedata_even(edge_type * edgedatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	edgedatabuffer = graphobj->loadedgesfromfile(0);
	// unsigned int edgessz = utilityobj->allignhigher_KV(graphobj->getedgessize(0));
	unsigned int edgessz = graphobj->getedgessize(0);
	unsigned int edgesszpersubthread = edgessz / NUMSUBCPUTHREADS;
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		for(unsigned int k=0; k<edgesszpersubthread; k++){
			kvbuffer[j][BASEOFFSET_EDGESDATA + k].key = edgedatabuffer[j*edgesszpersubthread + k].srcvid;
			kvbuffer[j][BASEOFFSET_EDGESDATA + k].value = edgedatabuffer[j*edgesszpersubthread + k].dstvid;
		}
		
		container->srcvoffset[0][j] = utilityobj->allignlower_KV(kvbuffer[j][BASEOFFSET_EDGESDATA].key);
		container->srcvsize[0][j] = kvbuffer[j][BASEOFFSET_EDGESDATA + edgesszpersubthread - 1].key - kvbuffer[j][BASEOFFSET_EDGESDATA].key;
		container->edgessize[0][j] = edgesszpersubthread;
		container->destvoffset[0][j] = 0; // REMOVEME
		#ifdef _DEBUGMODE_HOSTPRINTS2X
		utilityobj->printkeyvalues("loadgraph::loadedgedata::first", &kvbuffer[j][BASEOFFSET_EDGESDATA], 16);
		utilityobj->printkeyvalues("loadgraph::loadedgedata::last", &kvbuffer[j][BASEOFFSET_EDGESDATA+edgesszpersubthread-16], 16);
		#endif 
		if(j==1){utilityobj->printkeyvalues("loadgraph::loadedgedata::first", &kvbuffer[j][BASEOFFSET_EDGESDATA], 16);}
	}
	return;
} */
/* void loadgraph::loadedgedata_even(edge_t * vertexptrbuffer, edge_type * edgedatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	edgedatabuffer = graphobj->loadedgesfromfile(0);
	unsigned int edgessz = graphobj->getedgessize(0);
	unsigned int edgesszpersubthread = edgessz / NUMSUBCPUTHREADS;
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		
		unsigned int srcvoffset = 64; // REMOVEME. // NB: this MUST be picked first!
		unsigned int srcvsize = 128 * 64 * 8;
		unsigned int beginptr = vertexptrbuffer[srcvoffset];
		unsigned int endptr =  vertexptrbuffer[srcvoffset + srcvsize];
		unsigned int edgessize = endptr - beginptr;
		
		for(unsigned int k=0; k<edgessize; k++){
			kvbuffer[j][BASEOFFSET_EDGESDATA + k].key = edgedatabuffer[beginptr + k].srcvid;
			kvbuffer[j][BASEOFFSET_EDGESDATA + k].value = edgedatabuffer[beginptr + k].dstvid;
		}
		
		container->srcvoffset[0][j] = srcvoffset;
		container->srcvsize[0][j] = srcvsize;
		container->edgessize[0][j] = edgessize;
		container->destvoffset[0][j] = 0; // REMOVEME
		#ifdef _DEBUGMODE_HOSTPRINTS2X
		utilityobj->printkeyvalues("loadgraph::loadedgedata::first", &kvbuffer[j][BASEOFFSET_EDGESDATA], 16);
		utilityobj->printkeyvalues("loadgraph::loadedgedata::last", &kvbuffer[j][BASEOFFSET_EDGESDATA+edgesszpersubthread-16], 16);
		#endif 
		if(j==1){utilityobj->printkeyvalues("loadgraph::loadedgedata::first", &kvbuffer[j][BASEOFFSET_EDGESDATA], 16);}
	}
	return;
} */
void loadgraph::loadedgedata_even(edge_t * vertexptrbuffer, edge_type * edgedatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(0);
	edgedatabuffer = graphobj->loadedgesfromfile(0);
	unsigned int totaledgessz = graphobj->getedgessize(0);
	unsigned int edgesszpersubthread = totaledgessz / NUMSUBCPUTHREADS;
	
	
	
	
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		
		
		
		unsigned int srcvoffset = utilityobj->allignlower_KV(edgedatabuffer[j*edgesszpersubthread].srcvid);
		unsigned int nextsrcvoffset;
		if(j < NUMSUBCPUTHREADS-1){ nextsrcvoffset = utilityobj->allignlower_KV(edgedatabuffer[(j+1)*edgesszpersubthread].srcvid); }
		else { nextsrcvoffset = utilityobj->allignlower_KV(edgedatabuffer[totaledgessz-1].srcvid); }
		unsigned int srcvsize = nextsrcvoffset - srcvoffset;
		
		// unsigned int srcvoffset = 64; // REMOVEME. // NB: this MUST be picked first!
		// unsigned int srcvsize = 128 * 64 * 8;
		unsigned int beginptr = vertexptrbuffer[srcvoffset];
		unsigned int endptr =  vertexptrbuffer[srcvoffset + srcvsize];
		unsigned int edgessize = endptr - beginptr;
		cout<<"loadgraph::loadedgedata_even:: srcvoffset: "<<srcvoffset<<", srcvsize: "<<srcvsize<<", edgessize: "<<edgessize<<endl;
		
		for(unsigned int k=0; k<edgessize; k++){
			kvbuffer[j][BASEOFFSET_EDGESDATA + k].key = edgedatabuffer[beginptr + k].srcvid;
			kvbuffer[j][BASEOFFSET_EDGESDATA + k].value = edgedatabuffer[beginptr + k].dstvid;
		}
		
		container->srcvoffset[0][j] = srcvoffset;
		container->srcvsize[0][j] = srcvsize;
		container->edgessize[0][j] = edgessize;
		container->destvoffset[0][j] = 0; // REMOVEME
		#ifdef _DEBUGMODE_HOSTPRINTS2X
		utilityobj->printkeyvalues("loadgraph::loadedgedata::first", &kvbuffer[j][BASEOFFSET_EDGESDATA], 16);
		utilityobj->printkeyvalues("loadgraph::loadedgedata::last", &kvbuffer[j][BASEOFFSET_EDGESDATA+edgessize-16], 16);
		#endif 
		if(j==1){utilityobj->printkeyvalues("loadgraph::loadedgedata::first", &kvbuffer[j][BASEOFFSET_EDGESDATA], 16);}
	}
	// exit(EXIT_SUCCESS);
	return;
}
void loadgraph::loadmessages(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container){	
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ 
		if(container->srcvsize[0][j] >= KVDRAMSZ){ cout<<"loadgraph::run::ERROR: fix this. srcvsize is greater than allowed. srcvsize["<<j<<"]: "<<container->srcvsize[0][j]<<", KVDRAMSZ: "<<KVDRAMSZ<<endl; exit(EXIT_FAILURE); }
			helperfunctionsobj->createmessages(
				kvbuffer[j], // uint512_vec_dt * kvstats,
				container->srcvoffset[0][j], // unsigned int srcvoffset,
				container->srcvsize[0][j], // unsigned int srcvsize,
				container->edgessize[0][j], // unsigned int edgessize,
				container->destvoffset[0][j], // unsigned int destvoffset,
				NAp, // container->firstvid[0][j], // unsigned int firstvid,
				NAp, // container->firstvid[0][j],// unsigned int firstkey,
				NAp, // unsigned int firstvalue,
				TREE_DEPTH, // unsigned int treedepth,
				0, // unsigned int GraphIter,
				PAGERANK, // unsigned int GraphAlgo,
				container->edgessize[0][j], // unsigned int runsize,
				BATCH_RANGE, // unsigned int batch_range,
				BATCH_RANGE_POW, // unsigned int batch_range_pow,
				APPLYVERTEXBUFFERSZ, // unsigned int applyvertexbuffersz,
				NUMLASTLEVELPARTITIONS); // unsigned int numlastlevelpartitions
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"loadmessages:: running Acts... size: "<<container->edgessize[0][j]<<endl; 
		#endif 
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadmessages:: running Acts... sizes: ["; 
	cout<<"["; for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ cout<<container->edgessize[0][j]; if(j<NUMSUBCPUTHREADS-1){ cout<<", "; }} cout<<"]";
	cout<<"]"<<endl;
	#endif 
	return;
}




