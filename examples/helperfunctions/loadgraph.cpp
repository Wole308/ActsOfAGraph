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
#include "../../examples/helperfunctions/postprocess.h"
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
	statsobj = _statsobj;
	postprocessobj = new postprocess(graphobj, statsobj); 
}
loadgraph::loadgraph(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
	kernelobj = new kernel();
}
loadgraph::~loadgraph(){} 

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
void loadgraph::loadedgedata(unsigned int col, edge_t * vertexptrbuffer, edge_type * edgedatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo){
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
		if(GraphAlgo == PAGERANK){ container->runsize[0][j] = edgessize; } else { container->runsize[0][j] = 1; }
		container->destvoffset[0][j] = col * KVDATA_RANGE_PERSSDPARTITION;
		#ifdef _DEBUGMODE_HOSTPRINTS2X
		utilityobj->printkeyvalues("loadgraph::loadedgedata::first", &kvbuffer[j][BASEOFFSET_EDGESDATA], 16);
		utilityobj->printkeyvalues("loadgraph::loadedgedata::last", &kvbuffer[j][BASEOFFSET_EDGESDATA+edgessize-16], 16);
		#endif
	}
	return;
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
			container->srcvoffset[i][j] = 0;
			container->srcvsize[i][j] = srcvsz + 1;
			container->edgessize[i][j] = edgessz;
			container->runsize[i][j] = edgessz;
			container->destvoffset[i][j] = col * KVDATA_RANGE_PERSSDPARTITION;
			#ifdef _DEBUGMODE_HOSTPRINTS2
			utilityobj->printkeyvalues("loadgraph::loadgraphdata.vertexptrs[i][j]", (keyvalue_t *)&kvbuffer[i][j][BASEOFFSET_VERTEXPTR], 16);
			utilityobj->printedges("loadgraph::loadgraphdata.edgesdata[i][j]", (edge_type *)&kvbuffer[i][j][BASEOFFSET_EDGESDATA], 16); // edgessz
			#endif
		}
	}
	return;
}
void loadgraph::loadactvvertices(vector<vertex_t> &srcvids, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container){
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		for(unsigned int k=0; k<srcvids.size(); k++){
			kvbuffer[0][j][BASEOFFSET_ACTIVEVERTICES + k].key = srcvids[k];
			kvbuffer[0][j][BASEOFFSET_ACTIVEVERTICES + k].value = NAp;
		}
		container->actvvsize[0][j] = srcvids.size();
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printkeyvalues("loadgraph::loadvertexptrs::first", &kvbuffer[0][j][BASEOFFSET_ACTIVEVERTICES], utilityobj->hmin(16, srcvids.size()));
		#endif
	}
	return;
}
void loadgraph::loadmessages(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container){	
	#ifdef _DEBUGMODE_HOSTPRINTS2
	utilityobj->printcontainer(container); 
	#endif
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ 
		if(container->srcvsize[0][j] >= KVDRAMSZ){ cout<<"loadgraph::run::ERROR: fix this. srcvsize is greater than allowed. srcvsize["<<j<<"]: "<<container->srcvsize[0][j]<<", KVDRAMSZ: "<<KVDRAMSZ<<endl; exit(EXIT_FAILURE); }
		createmessages(
			kvbuffer[j], // uint512_vec_dt * kvstats,
			container->srcvoffset[0][j], // unsigned int srcvoffset,
			container->srcvsize[0][j], // unsigned int srcvsize,
			container->edgessize[0][j], // unsigned int edgessize,
			container->destvoffset[0][j], // unsigned int destvoffset,
			container->actvvsize[0][j], // unsigned int actvvsize,
			NAp, // container->firstvid[0][j], // unsigned int firstvid,
			NAp, // container->firstvid[0][j],// unsigned int firstkey,
			NAp, // unsigned int firstvalue,
			TREE_DEPTH, // unsigned int treedepth,
			0, // unsigned int GraphIter,
			PAGERANK, // unsigned int GraphAlgo,
			container->runsize[0][j], // unsigned int runsize,
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
	cout<<"["; for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ cout<<container->runsize[0][j]; if(j<NUMSUBCPUTHREADS-1){ cout<<", "; }} cout<<"]";
	cout<<"]"<<endl;
	#endif 
	return;
}
void loadgraph::createmessages(
			uint512_vec_dt * kvstats,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int edgessize,
			unsigned int destvoffset,
			unsigned int actvvsize,
			unsigned int firstvid,
			unsigned int firstkey,
			unsigned int firstvalue,
			unsigned int treedepth,
			unsigned int GraphIter,
			unsigned int GraphAlgo,
			unsigned int runsize,
			unsigned int batch_range,
			unsigned int batch_range_pow,
			unsigned int applyvertexbuffersz,
			unsigned int numlastlevelpartitions){
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNKERNELCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PROCESSCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_COLLECTSTATSCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_PARTITIONCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYUPDATESCOMMANDID].data[0].key = ON;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVOFFSET].data[0].key = srcvoffset;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE].data[0].key = srcvsize;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_SRCVSIZE_KVS].data[0].key = (srcvsize + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE].data[0].key = edgessize;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_EDGESSIZE_KVS].data[0].key = (edgessize + (VECTOR_SIZE - 1)) / VECTOR_SIZE;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_DESTVOFFSET].data[0].key = destvoffset;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ACTVVSIZE].data[0].key = actvvsize;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTVID].data[0].key = firstvid;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTKEY].data[0].key = firstkey;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FIRSTVALUE].data[0].key = firstvalue;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_TREEDEPTH].data[0].key = treedepth;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHITERATIONID].data[0].key = GraphIter;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_GRAPHALGORITHMID].data[0].key = GraphAlgo;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NEXTBATCHOFFSET].data[0].key = 0; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHSIZE].data[0].key = runsize; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE].data[0].key = runsize; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_RUNSIZE_KVS].data[0].key = (runsize + (VECTOR_SIZE - 1)) / VECTOR_SIZE; 
	
	if(runsize == 0 || runsize >= KVSOURCEDRAMSZ){
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = 0;
	} else {
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 2;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1; // REMOVEME
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 1;
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0; // REMOVEME
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1;
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
	}
	
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE].data[0].key = batch_range; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_KVS].data[0].key = batch_range / VECTOR_SIZE; // batch_range_kvs;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_POW].data[0].key = batch_range_pow;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ].data[0].key = applyvertexbuffersz; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].data[0].key = applyvertexbuffersz / VECTOR_SIZE; // applyvertexbuffersz_kvs;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key = numlastlevelpartitions;
	return;
}









