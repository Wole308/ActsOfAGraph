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

void loadgraph::loadvertexptrs(unsigned int col, edge_t * vertexptrbuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(col);
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		for(unsigned int k=0; k<KVDATA_RANGE; k++){ // KVDATA_RANGE_PERSSDPARTITION
			kvbuffer[j][BASEOFFSET_VERTEXPTR + k].key = vertexptrbuffer[k];
			kvbuffer[j][BASEOFFSET_VERTEXPTR + k].value = NAp; // vertexdatabuffer[k];
		}
		
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printkeyvalues("loadgraph::loadvertexptrs::first", &kvbuffer[j][BASEOFFSET_VERTEXPTR], 16);
		utilityobj->printkeyvalues("loadgraph::loadvertexptrs::last", &kvbuffer[j][BASEOFFSET_VERTEXPTR+KVDATA_RANGE-16], 16);
		#endif 
	}
	return;
}
void loadgraph::loadvertexdata(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vertex_t offset, vertex_t size){
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			memcpy(&kvbuffer[i][j][BASEOFFSET_VERTICESDATA], &vertexdatabuffer[offset], size * sizeof(value_t));
			#ifdef _DEBUGMODE_HOSTPRINTS2
			utilityobj->printkeyvalues("loadgraph::loadvertexdata", &kvbuffer[i][j][BASEOFFSET_VERTICESDATA], 16);
			#endif
		}
	}
	return;
}
void loadgraph::loadedgedata(unsigned int col, edge_t * vertexptrbuffer, edge_type * edgedatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(col);
	edgedatabuffer = graphobj->loadedgesfromfile(col);
	unsigned int totaledgessz = graphobj->getedgessize(col);
	unsigned int edgesszpersubthread = totaledgessz / NUMSUBCPUTHREADS;
	
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		unsigned int srcvoffset = utilityobj->allignlower_KV(edgedatabuffer[j*edgesszpersubthread].srcvid); // set first
		unsigned int nextsrcvoffset;
		if(j < NUMSUBCPUTHREADS-1){ nextsrcvoffset = utilityobj->allignlower_KV(edgedatabuffer[(j+1)*edgesszpersubthread].srcvid); }
		else { nextsrcvoffset = utilityobj->allignlower_KV(edgedatabuffer[totaledgessz-1].srcvid); }
		unsigned int srcvsize = nextsrcvoffset - srcvoffset;
		unsigned int beginptr = vertexptrbuffer[srcvoffset];
		unsigned int endptr =  vertexptrbuffer[srcvoffset + srcvsize];
		unsigned int edgessize = endptr - beginptr;
		cout<<"loadgraph::loadedgedata:: srcvoffset: "<<srcvoffset<<", srcvsize: "<<srcvsize<<", edgessize: "<<edgessize<<endl;
		
		for(unsigned int k=0; k<edgessize; k++){
			kvbuffer[j][BASEOFFSET_EDGESDATA + k].key = edgedatabuffer[beginptr + k].srcvid;
			kvbuffer[j][BASEOFFSET_EDGESDATA + k].value = edgedatabuffer[beginptr + k].dstvid;
		}
		
		container->srcvoffset[0][j] = srcvoffset;
		container->srcvsize[0][j] = srcvsize;
		container->edgessize[0][j] = edgessize;
		container->destvoffset[0][j] = col * KVDATA_RANGE_PERSSDPARTITION;
		#ifdef _DEBUGMODE_HOSTPRINTS2X
		utilityobj->printkeyvalues("loadgraph::loadedgedata::first", &kvbuffer[j][BASEOFFSET_EDGESDATA], 16);
		utilityobj->printkeyvalues("loadgraph::loadedgedata::last", &kvbuffer[j][BASEOFFSET_EDGESDATA+edgessize-16], 16);
		#endif
	}
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

edge_t loadgraph::countedges(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, container_t * container){
	edge_t edgessz = 0;
	for(unsigned int k=0; k<srcvids.size(); k++){
		graphobj->loadvertexptrsfromfile(col, srcvids[k], container->tempvertexptrs[0][0], 0, 2); 
		edgessz += container->tempvertexptrs[0][0][1] - container->tempvertexptrs[0][0][0];
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::countedges:: total number of edges: "<<edgessz<<endl;
	#endif 
	return edgessz;
}

void loadgraph::loadgraphdata(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], value_t * vertexdatabuffer, unsigned int balancededgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container){			
	unsigned int srcvidsoffset = 0;
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			unsigned int edgeoffset = 0;
			edge_t edgessz = 0;
			vertex_t srcvsz = 0;
			unsigned int hsrcvid = 0;
			unsigned int vpindex = 0;
			kvbuffer[i][j][BASEOFFSET_VERTEXPTR + vpindex].key = 0;
			for(unsigned int k=srcvidsoffset; k<srcvids.size(); k++){
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"loadgraphdata:: k: "<<k<<", edgeoffset: "<<edgeoffset<<", edgessz: "<<edgessz<<", srcvids["<<k<<"]: "<<srcvids[k]<<endl;
				#endif
				
				graphobj->loadvertexptrsfromfile(col, srcvids[k], container->tempvertexptrs[i][j], 0, 2); 
				
				edge_t edgesoffset = container->tempvertexptrs[i][j][0];
				edge_t numedgestoload = container->tempvertexptrs[i][j][1] - container->tempvertexptrs[i][j][0]; //NEWCHANGE.
				if(numedgestoload > 0){ graphobj->loadedgesfromfile(col, edgesoffset, (edge_type *)&kvbuffer[i][j][BASEOFFSET_EDGESDATA], edgeoffset, numedgestoload); }
				
				// re-assign vertex pointers
				kvbuffer[i][j][BASEOFFSET_VERTEXPTR + vpindex + 1].key = kvbuffer[i][j][BASEOFFSET_VERTEXPTR + vpindex].key + numedgestoload;
				kvbuffer[i][j][BASEOFFSET_VERTEXPTR + vpindex].value = NAp; // vertexdatabuffer[srcvids[k]]; // REMOVEME. FIXME.
				
				// re-assign edges' srcvids
				for(unsigned int n = 0; n < numedgestoload; n++){ kvbuffer[i][j][BASEOFFSET_EDGESDATA + edgeoffset + n].key = hsrcvid; }
				
				edgeoffset += numedgestoload;
				edgessz += numedgestoload;
				if(numedgestoload > 0){
					vpindex += 1;
					srcvsz += 1;
					hsrcvid += 1;
				}
				
				if(edgessz >= balancededgesizes[i][j]){
					#ifdef _DEBUGMODE_HOSTPRINTS2
					cout<<"loadgraphdata::NOTE: edgessz ("<<edgessz<<") has reached max. srcvidsoffset: ("<<srcvidsoffset<<"). load in next subthread. breaking out..."<<endl; 
					#endif 
					break; }
			}

			srcvidsoffset += srcvsz;
			container->srcvoffset[i][j] = 0; // utilityobj->allignlower_KV(kvbuffer[i][j][BASEOFFSET_EDGESDATA].key);
			container->srcvsize[i][j] = srcvsz + 1;
			container->edgessize[i][j] = edgessz;
			container->destvoffset[i][j] = col * KVDATA_RANGE_PERSSDPARTITION;
			#ifdef _DEBUGMODE_HOSTPRINTS2
			utilityobj->printkeyvalues("loadgraph::loadgraphdata.vertexptrs[i][j]", (keyvalue_t *)&kvbuffer[i][j][BASEOFFSET_VERTEXPTR], 16);
			utilityobj->printedges("loadgraph::loadgraphdata.edgesdata[i][j]", (edge_type *)&kvbuffer[i][j][BASEOFFSET_EDGESDATA], 16); // edgessz
			#endif
		}
	}
	return;
}









