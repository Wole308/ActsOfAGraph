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
	statsobj = _statsobj;
	postprocessobj = new postprocess(graphobj, statsobj); 
}
loadgraph::loadgraph(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
loadgraph::~loadgraph(){} 

edge_t loadgraph::countedges(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int * srcvidsoffset, edge_t maxnumedgestoload, container_t * container){
	edge_t edgessz = 0;
	unsigned int k = 0;
	unsigned int beginoffset = *srcvidsoffset;
	for(k=beginoffset; k<srcvids.size(); k++){
		graphobj->loadvertexptrsfromfile(col, srcvids[k], container->tempvertexptrs[0][0], 0, 2); 
		edgessz += container->tempvertexptrs[0][0][1] - container->tempvertexptrs[0][0][0];
		if(edgessz >= maxnumedgestoload){ break; }
	}
	*srcvidsoffset = k;
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::countedges(B):: total number of edges: "<<edgessz<<", srcvidsoffset: "<<*srcvidsoffset<<", srcvids.size(): "<<srcvids.size()<<endl;
	#endif 
	return edgessz;
}
void loadgraph::loadvertexptrs(unsigned int col, edge_t * vertexptrbuffer, value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(col);
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		for(unsigned int k=0; k<KVDATA_RANGE; k++){
			kvbuffer[j][BASEOFFSET_VERTEXPTR + k].key = vertexptrbuffer[k];
			kvbuffer[j][BASEOFFSET_VERTEXPTR + k].value = vertexdatabuffer[k];
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
vertex_t loadgraph::loadedges(unsigned int col, vertex_t srcvoffset, edge_t * vertexptrbuffer, edge_type * edgedatabuffer, edge_type * edges[NUMSUBCPUTHREADS], unsigned int edgesbaseoffset, container_t * container, unsigned int GraphAlgo){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadedges:: col: "<<col<<", srcvoffset: "<<srcvoffset<<endl;
	#endif 
	
	unsigned int beginsrcvoffset = srcvoffset;
	graphobj->loadvertexptrsfromfile(col, beginsrcvoffset, container->tempvertexptrs[0][0], 0, 2); 
	unsigned int beginvptr = container->tempvertexptrs[0][0][0];
	
	edge_t approxnumedges = utilityobj->hmin(graphobj->getedgessize(col) - beginvptr, EDGES_BATCHSIZE);
	
	graphobj->loadedgesfromfile(col, beginvptr + approxnumedges - 1, container->tempedges[0][0], 0, 2);
	unsigned int endsrcvoffset = utilityobj->allignhigher_KV(container->tempedges[0][0][0].srcvid);
	graphobj->loadvertexptrsfromfile(col, endsrcvoffset, container->tempvertexptrs[0][0], 0, 2); 
	unsigned int endvptr = container->tempvertexptrs[0][0][0];
	
	// edge_t actualnumedges = endvptr - beginvptr + 1;
	edge_t actualnumedges = endvptr - beginvptr;
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(col);
	graphobj->loadedgesfromfile(col, beginvptr, edgedatabuffer, 0, actualnumedges);
	
	unsigned int edgesszpersubthread = actualnumedges / NUMSUBCPUTHREADS;
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadedges:: beginsrcvoffset: "<<beginsrcvoffset<<", endsrcvoffset: "<<endsrcvoffset<<endl;
	cout<<"loadgraph::loadedges:: beginvptr: "<<beginvptr<<", endvptr: "<<endvptr<<", approxnumedges: "<<approxnumedges<<", actualnumedges: "<<actualnumedges<<", edgesszpersubthread: "<<edgesszpersubthread<<endl;
	#endif 
	
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
		unsigned int srcvoffset; // set srcvoffset first
		unsigned int nextsrcvoffset;
		
		if(actualnumedges > 0){
			srcvoffset = utilityobj->allignlower_KV(edgedatabuffer[j*edgesszpersubthread].srcvid);
			if(j < NUMSUBCPUTHREADS-1){ nextsrcvoffset = utilityobj->allignlower_KV(edgedatabuffer[(j+1)*edgesszpersubthread].srcvid); }
			else { nextsrcvoffset = utilityobj->allignlower_KV(edgedatabuffer[actualnumedges-1].srcvid); }
		} else {
			srcvoffset = 0;
			nextsrcvoffset = srcvoffset;
		}
		
		unsigned int srcvsize = nextsrcvoffset - srcvoffset;
		unsigned int localbeginptr = vertexptrbuffer[srcvoffset] - beginvptr;
		unsigned int localendptr =  vertexptrbuffer[srcvoffset + srcvsize] - beginvptr;
		unsigned int edgessize = localendptr - localbeginptr;
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"loadgraph::loadedges:: j: "<<j<<", srcvoffset: "<<srcvoffset<<", nextsrcvoffset: "<<nextsrcvoffset<<", srcvsize: "<<srcvsize<<", edgessize: "<<edgessize<<endl;
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS2
		utilityobj->checkforlessthanthan("loadgraph::loadedges.nextsrcvoffset & srcvoffset", nextsrcvoffset, srcvoffset);
		utilityobj->checkoutofbounds("loadgraph::loadedges.edges", edgessize, MAXKVDATA_BATCHSIZE, edgesbaseoffset, edgessize, NAp); // EDGESSZ
		utilityobj->checkoutofbounds("loadgraph::loadedges.edgedatabuffer", localbeginptr + edgessize, PADDEDEDGES_BATCHSIZE, localbeginptr, edgessize, NAp);
		#endif 
		for(unsigned int k=0; k<edgessize; k++){
			edges[j][edgesbaseoffset + k].srcvid = edgedatabuffer[localbeginptr + k].srcvid;
			edges[j][edgesbaseoffset + k].dstvid = edgedatabuffer[localbeginptr + k].dstvid;
		}
		
		container->srcvoffset[0][j] = srcvoffset;
		container->srcvsize[0][j] = srcvsize;
		container->edgessize[0][j] = edgessize;
		if(GraphAlgo == PAGERANK){ container->runsize[0][j] = edgessize; } else { container->runsize[0][j] = 1; }
		container->destvoffset[0][j] = col * KVDATA_RANGE_PERSSDPARTITION;
		container->actvvsize[0][j] = 0;
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printedges("loadgraph::loadedges::first", edges[j], 16);
		utilityobj->printedges("loadgraph::loadedges::last", &edges[j][edgesbaseoffset+edgessize-16], 16);
		#endif
	}
	return endsrcvoffset;
}
void loadgraph::loadactivesubgraph(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], value_t * vertexdatabuffer, unsigned int balancededgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container){ // in-memory	 	
	unsigned int srcvidsnxtoffset = 0;
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			unsigned int edgeoffset = 0;
			edge_t edgessz = 0;
			vertex_t srcvsz = 0;
			unsigned int hsrcvid = 0;
			unsigned int vpindex = 0;
			kvbuffer[i][j][BASEOFFSET_VERTEXPTR + vpindex].key = 0;
			for(unsigned int k=srcvidsnxtoffset; k<srcvids.size(); k++){
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"loadactivesubgraph:: k: "<<k<<", edgeoffset: "<<edgeoffset<<", edgessz: "<<edgessz<<", srcvids["<<k<<"]: "<<srcvids[k]<<endl;
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
					cout<<"loadactivesubgraph::NOTE: edgessz ("<<edgessz<<") has reached max. srcvidsnxtoffset: ("<<srcvidsnxtoffset<<"). load in next subthread. breaking out..."<<endl; 
					#endif 
					break; }
			}

			srcvidsnxtoffset += srcvsz;
			container->srcvoffset[i][j] = 0;
			container->srcvsize[i][j] = srcvsz + 1;
			container->edgessize[i][j] = edgessz;
			container->runsize[i][j] = edgessz;
			container->destvoffset[i][j] = col * KVDATA_RANGE_PERSSDPARTITION;
			container->actvvsize[i][j] = 0;
			#ifdef _DEBUGMODE_HOSTPRINTS2
			utilityobj->printkeyvalues("loadgraph::loadactivesubgraph.vertexptrs[i][j]", (keyvalue_t *)&kvbuffer[i][j][BASEOFFSET_VERTEXPTR], 16);
			utilityobj->printedges("loadgraph::loadactivesubgraph.edgesdata[i][j]", (edge_type *)&kvbuffer[i][j][BASEOFFSET_EDGESDATA], 16); // edgessz
			#endif
		}
	}
	return;
}
void loadgraph::loadactivesubgraph(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int srcvidsoffset, value_t * vertexdatabuffer, 
										edge_t * vertexptrs[NUMSUBCPUTHREADS], value_t * verticesdata[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], unsigned int balancededgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container){ // not in-memory			
	unsigned int srcvidsnxtoffset = srcvidsoffset;
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){
		for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){
			unsigned int edgeoffset = 0;
			edge_t edgessz = 0;
			vertex_t srcvsz = 0;
			unsigned int hsrcvid = 0;
			unsigned int vpindex = 0;
			vertexptrs[j][vpindex] = 0;
			for(unsigned int k=srcvidsnxtoffset; k<srcvids.size(); k++){
				#ifdef _DEBUGMODE_HOSTPRINTS
				cout<<"loadactivesubgraph:: k: "<<k<<", edgeoffset: "<<edgeoffset<<", edgessz: "<<edgessz<<", srcvids["<<k<<"]: "<<srcvids[k]<<endl;
				#endif
				
				graphobj->loadvertexptrsfromfile(col, srcvids[k], container->tempvertexptrs[i][j], 0, 2); 
				
				edge_t edgesoffset = container->tempvertexptrs[i][j][0];
				edge_t numedgestoload = container->tempvertexptrs[i][j][1] - container->tempvertexptrs[i][j][0]; //NEWCHANGE.
				if(numedgestoload > 0){ graphobj->loadedgesfromfile(col, edgesoffset, edges[j], edgeoffset, numedgestoload); }
				
				// re-assign vertex pointers
				vertexptrs[j][vpindex + 1] = vertexptrs[j][vpindex] + numedgestoload;
				
				// re-assign vertices data 
				verticesdata[j][vpindex] = vertexdatabuffer[srcvids[k]];
				
				// re-assign edges' srcvids
				for(unsigned int n = 0; n < numedgestoload; n++){ edges[j][edgeoffset + n].srcvid = hsrcvid; }
				
				edgeoffset += numedgestoload;
				edgessz += numedgestoload;
				if(numedgestoload > 0){
					vpindex += 1;
					srcvsz += 1;
					hsrcvid += 1;
				}
				
				if(edgessz >= balancededgesizes[i][j]){
					#ifdef _DEBUGMODE_HOSTPRINTS2
					cout<<"loadactivesubgraph::NOTE: edgessz ("<<edgessz<<") has reached max. srcvidsnxtoffset: ("<<srcvidsnxtoffset<<"). load in next subthread. breaking out..."<<endl; 
					#endif 
					break; }
			}
			
			srcvidsnxtoffset += srcvsz;
			container->srcvoffset[i][j] = 0;
			// container->srcvsize[i][j] = srcvsz + 1;
			container->srcvsize[i][j] = srcvsz;
			container->edgessize[i][j] = edgessz;
			container->runsize[i][j] = edgessz;
			container->destvoffset[i][j] = col * KVDATA_RANGE_PERSSDPARTITION;
			container->actvvsize[i][j] = 0;
			#ifdef _DEBUGMODE_HOSTPRINTS
			utilityobj->printvalues("loadgraph::loadactivesubgraph.vertexptrs[j]", vertexptrs[j], 16);
			utilityobj->printedges("loadgraph::loadactivesubgraph.edges[j]", edges[j], 16); // edgessz
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
void loadgraph::loadmessages(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, unsigned int GraphAlgo){	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printcontainer(container); 
	#endif
	for(unsigned int j = 0; j < NUMSUBCPUTHREADS; j++){ 
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
			GraphIter, // unsigned int GraphIter,
			GraphAlgo, // unsigned int GraphAlgo,
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
	cout<<"loadmessages:: loading Acts... sizes: ["; 
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
	#ifdef _DEBUGMODE_CHECKS2
	if(runsize > MAXKVDATA_BATCHSIZE){ cout<<"loadgraph::createmessages::ERROR. runsize too large!. runsize: "<<runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	if(edgessize > MAXKVDATA_BATCHSIZE){ cout<<"loadgraph::createmessages::ERROR. edgessize too large!. edgessize: "<<edgessize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	if(actvvsize > ACTIVEVERTICESSZ){ cout<<"loadgraph::createmessages::ERROR. actvvsize too large!. actvvsize: "<<actvvsize<<", ACTIVEVERTICESSZ: "<<ACTIVEVERTICESSZ<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	
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
		#ifdef INMEMORYGP
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 2;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1; // REMOVEME
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 1;
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
			#ifdef ACTSFAST
			kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0; // REMOVEME
			kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 2;
			kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
			#endif 
		#else 
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 2;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 2;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		#endif
	}
	
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE].data[0].key = batch_range; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_KVS].data[0].key = batch_range / VECTOR_SIZE; // batch_range_kvs;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BATCHRANGE_POW].data[0].key = batch_range_pow;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ].data[0].key = applyvertexbuffersz; 
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_APPLYVERTEXBUFFERSZ_KVS].data[0].key = applyvertexbuffersz / VECTOR_SIZE; // applyvertexbuffersz_kvs;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_FINALNUMPARTITIONS].data[0].key = numlastlevelpartitions;

	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_MESSAGESDRAM_KVS].data[0].key = BASEOFFSET_MESSAGESDRAM_KVS;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAMBUFFER_KVS].data[0].key = BASEOFFSET_KVDRAMBUFFER_KVS;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAM_KVS].data[0].key = BASEOFFSET_KVDRAM_KVS;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_KVDRAMWORKSPACE_KVS].data[0].key = BASEOFFSET_KVDRAMWORKSPACE_KVS;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_STATSDRAM_KVS].data[0].key = BASEOFFSET_STATSDRAM_KVS;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_ACTIVEVERTICES_KVS].data[0].key = BASEOFFSET_ACTIVEVERTICES_KVS;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_EDGESDATA_KVS].data[0].key = BASEOFFSET_EDGESDATA_KVS;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTEXPTR_KVS].data[0].key = BASEOFFSET_VERTEXPTR_KVS;
	kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BASEOFFSET_VERTICESDATA_KVS].data[0].key = BASEOFFSET_VERTICESDATA_KVS;
	return;
}









