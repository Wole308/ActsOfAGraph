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

void loadgraph::loadvertexdata(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], vertex_t offset, vertex_t size){
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		memcpy(&kvbuffer[i][BASEOFFSET_VERTICESDATA], &vertexdatabuffer[offset], size * sizeof(value_t));
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printkeyvalues("loadgraph::loadvertexdata", &kvbuffer[i][BASEOFFSET_VERTICESDATA], 16);
		#endif
	}
	return;
}
void loadgraph::loadvertexptrs(unsigned int col, edge_t * vertexptrbuffer, value_t * vertexdatabuffer, vptr_type * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(col);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		for(unsigned int k=0; k<KVDATA_RANGE; k++){
			#ifdef SINELEVALUEVPTRTYPE
			kvbuffer[i][2*BASEOFFSET_VERTEXPTR + k].key = vertexptrbuffer[k];
			#else 
			kvbuffer[i][BASEOFFSET_VERTEXPTR + k].key = vertexptrbuffer[k];
			kvbuffer[i][BASEOFFSET_VERTEXPTR + k].value = vertexdatabuffer[k];
			#endif
		}
		
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printkeyvalues("loadgraph::loadvertexptrs::first", &(keyvalue_t *)kvbuffer[i][BASEOFFSET_VERTEXPTR], 16);
		utilityobj->printkeyvalues("loadgraph::loadvertexptrs::last", &(keyvalue_t *)kvbuffer[i][BASEOFFSET_VERTEXPTR+KVDATA_RANGE-16], 16);
		#endif 
	}
	return;
}
vertex_t loadgraph::loadedges_columnwise(unsigned int col, vertex_t srcvoffset, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, edge_type * edges[NUMSUBCPUTHREADS], edge_t baseoffset_edgedata, container_t * container, unsigned int GraphAlgo){
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadedges_columnwise:: col: "<<col<<", srcvoffset: "<<srcvoffset<<endl;
	#endif 
	
	unsigned int beginsrcvoffset = srcvoffset;
	graphobj->loadvertexptrsfromfile(col, beginsrcvoffset, container->tempvertexptrs[0], 0, 2); 
	unsigned int beginvptr = container->tempvertexptrs[0][0];
	
	edge_t approxnumedges = utilityobj->hmin(graphobj->getedgessize(col) - beginvptr, EDGES_BATCHSIZE);
	
	graphobj->loadedgesfromfile(col, beginvptr + approxnumedges - 1, container->tempedges[0], 0, 2);
	unsigned int endsrcvoffset = utilityobj->allignhigher_KV(container->tempedges[0][0].srcvid);
	graphobj->loadvertexptrsfromfile(col, endsrcvoffset, container->tempvertexptrs[0], 0, 2); 
	unsigned int endvptr = container->tempvertexptrs[0][0];
	
	edge_t actualnumedges = endvptr - beginvptr;
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(col);
	graphobj->loadedgesfromfile(col, beginvptr, edgedatabuffer, 0, actualnumedges);
	
	unsigned int edgesszpersubthread = actualnumedges / NUMSUBCPUTHREADS;
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadedges_columnwise:: beginsrcvoffset: "<<beginsrcvoffset<<", endsrcvoffset: "<<endsrcvoffset<<endl;
	cout<<"loadgraph::loadedges_columnwise:: beginvptr: "<<beginvptr<<", endvptr: "<<endvptr<<", approxnumedges: "<<approxnumedges<<", actualnumedges: "<<actualnumedges<<", edgesszpersubthread: "<<edgesszpersubthread<<endl;
	#endif 
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		unsigned int srcvoffset; // set srcvoffset first
		unsigned int nextsrcvoffset;
		
		if(actualnumedges > 0){
			srcvoffset = utilityobj->allignlower_KV(edgedatabuffer[i*edgesszpersubthread].srcvid);
			if(i < NUMSUBCPUTHREADS-1){ nextsrcvoffset = utilityobj->allignlower_KV(edgedatabuffer[(i+1)*edgesszpersubthread].srcvid); }
			else { nextsrcvoffset = utilityobj->allignlower_KV(edgedatabuffer[actualnumedges-1].srcvid); }
		} else {
			srcvoffset = 0;
			nextsrcvoffset = srcvoffset;
		}
		
		unsigned int srcvsize = nextsrcvoffset - srcvoffset;
		unsigned int localbeginptr = vertexptrbuffer[srcvoffset] - beginvptr;
		unsigned int localendptr =  vertexptrbuffer[srcvoffset + srcvsize] - beginvptr;
		unsigned int edgessize = localendptr - localbeginptr;
		#ifdef _DEBUGMODE_HOSTPRINTS3
		cout<<"loadgraph::loadedges_columnwise:: i: "<<i<<", srcvoffset: "<<srcvoffset<<", nextsrcvoffset: "<<nextsrcvoffset<<", srcvsize: "<<srcvsize<<", edgessize: "<<edgessize<<endl;
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS2
		utilityobj->checkforlessthanthan("loadgraph::loadedges_columnwise.nextsrcvoffset & srcvoffset 33", nextsrcvoffset, srcvoffset);
		utilityobj->checkoutofbounds("loadgraph::loadedges_columnwise.edges 34", edgessize, MAXKVDATA_BATCHSIZE, baseoffset_edgedata, edgessize, NAp); // EDGESSZ
		utilityobj->checkoutofbounds("loadgraph::loadedges_columnwise.edgedatabuffer 35", localbeginptr + edgessize, PADDEDEDGES_BATCHSIZE, localbeginptr, edgessize, NAp);
		#endif 
		for(unsigned int k=0; k<edgessize; k++){
			#ifdef SINELEVALUEEDGETYPE
			edges[i][2*baseoffset_edgedata + k].dstvid = edgedatabuffer[localbeginptr + k].dstvid; // baseoffset_edgedata // BASEOFFSET_EDGESDATA
			#else 
			edges[i][baseoffset_edgedata + k].srcvid = edgedatabuffer[localbeginptr + k].dstvid;
			edges[i][baseoffset_edgedata + k].dstvid = edgedatabuffer[localbeginptr + k].srcvid;
			#endif
		}
		
		container->srcvoffset[i] = srcvoffset;
		container->srcvsize[i] = srcvsize;
		container->edgessize[i] = edgessize;
		if(GraphAlgo == PAGERANK){ container->runsize[i] = edgessize; } else { container->runsize[i] = 1; }
		container->destvoffset[i] = col * KVDATA_RANGE_PERSSDPARTITION;
		container->actvvsize[i] = 0;
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printedges("loadgraph::loadedges_columnwise::first", edges[i], 16);
		utilityobj->printedges("loadgraph::loadedges_columnwise::last", &edges[i][baseoffset_edgedata+edgessize-16], 16);
		#endif
	}
	return endsrcvoffset;
}
void loadgraph::loadedges_rowwise(unsigned int col, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadedges_rowwise:: col: "<<col<<endl;
	#endif 
	
	graphobj->loadedgesfromfile(col, 0, edgedatabuffer, 0, graphobj->getedgessize(col));
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(col);
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printvalues("loadedges_rowwise.vertexptrbuffer", (value_t *)vertexptrbuffer, 16);
	utilityobj->printedges("loadedges_rowwise.edgedatabuffer", edgedatabuffer, 16);
	#endif
	
	unsigned int counts[NUMSUBCPUTHREADS];
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ counts[i] = 0; }
	unsigned int index = 0;
	
	for(unsigned int vid=0; vid<KVDATA_RANGE; vid++){
		if(vid % 100000 == 0){ cout<<"### loadgraph::loadedges_rowwise:: vid: "<<vid<<", vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end;
		if(vid < KVDATA_RANGE-1){ vptr_end = vertexptrbuffer[vid+1]; } 
		else { vptr_end = graphobj->getedgessize(col); }
		edge_t edges_size = vptr_end - vptr_begin;
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"loadgraph::loadedges_rowwise:: vptr_begin: "<<vptr_begin<<endl;
		utilityobj->printvalues("loadedges_rowwise.counts", (value_t *)counts, NUMSUBCPUTHREADS);
		cout<<"loadgraph::loadedges_rowwise:: vptr_end: "<<vptr_end<<endl;
		cout<<"loadgraph::loadedges_rowwise:: edges_size: "<<edges_size<<endl;
		#endif 
		
		for(unsigned int k=0; k<edges_size; k++){
			unsigned int subthread = (index / VECTOR2_SIZE) % NUMSUBCPUTHREADS;
			#ifdef _DEBUGMODE_CHECKS2
			utilityobj->checkoutofbounds("loadgraph::loadedges_rowwise. 2*BASEOFFSET_EDGESDATA + counts[subthread]", 2*BASEOFFSET_EDGESDATA + counts[subthread], PADDEDKVSOURCEDRAMSZ*2, 2*BASEOFFSET_EDGESDATA, counts[subthread], PADDEDKVSOURCEDRAMSZ*2);
			#endif 
		
			edges[subthread][2*BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = edgedatabuffer[index].dstvid;
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"k: "<<k<<", subthread: "<<subthread<<", counts["<<subthread<<"]: "<<counts[subthread]<<", index: "<<index<<", edgedatabuffer["<<index<<"].dstvid: "<<edgedatabuffer[index].dstvid<<endl;
			#endif
			
			counts[subthread] += 1;
			index += 1;
		}
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ if(vid < KVDATA_RANGE-1){ vptrs[i][2*BASEOFFSET_VERTEXPTR + vid + 1].key = counts[i]; }}
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"loadgraph::loadedges_rowwise:: index: "<<index<<endl;
		utilityobj->printvalues("loadedges_rowwise.counts", (value_t *)counts, NUMSUBCPUTHREADS);
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadedges_rowwise.edges["+std::to_string(i)+"]["+std::to_string(2*BASEOFFSET_EDGESDATA)+"]", (value_t *)&edges[i][2*BASEOFFSET_EDGESDATA], counts[i]); }
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadedges_rowwise.vptrs["+std::to_string(i)+"]["+std::to_string(2*BASEOFFSET_VERTEXPTR)+"]", (value_t *)&vptrs[i][2*BASEOFFSET_VERTEXPTR], 5); } // vid+1
		#endif 
	}
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = KVDATA_RANGE; 
		container->edgessize[i] = counts[i]; 
		container->runsize[i] = counts[i];
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printedges("loadgraph::loadedges_rowwise::first", edges[i], 16);
		utilityobj->printedges("loadgraph::loadedges_rowwise::last", &edges[i][BASEOFFSET_EDGESDATA+container->edgessize[i]-16], 16);
		#endif
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	utilityobj->printvalues("loadedges_rowwise.counts", (value_t *)counts, NUMSUBCPUTHREADS);
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadedges_rowwise.vptrs["+std::to_string(i)+"]["+std::to_string(2*BASEOFFSET_VERTEXPTR)+"]", (value_t *)&vptrs[i][2*BASEOFFSET_VERTEXPTR], 8); }
	for(unsigned int i=0; i<0; i++){ utilityobj->printvalues("loadedges_rowwise.edges["+std::to_string(i)+"]["+std::to_string(2*BASEOFFSET_EDGESDATA)+"]", (value_t *)&edges[i][2*BASEOFFSET_EDGESDATA], 8); }
	#endif
}
void loadgraph::loadactvvertices(vector<vertex_t> &srcvids, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		for(unsigned int k=0; k<srcvids.size(); k++){
			kvbuffer[i][BASEOFFSET_ACTIVEVERTICES + k].key = srcvids[k];
			kvbuffer[i][BASEOFFSET_ACTIVEVERTICES + k].value = 777;//NAp;
		}
		container->actvvsize[i] = srcvids.size();
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printkeyvalues("loadgraph::loadvertexptrs::first", &kvbuffer[i][BASEOFFSET_ACTIVEVERTICES], utilityobj->hmin(16, srcvids.size()));
		#endif
	}
	return;
}

edge_t loadgraph::countedges(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int * srcvidsoffset, edge_t maxnumedgestoload, container_t * container){
	edge_t edgessz = 0;
	unsigned int k = 0;
	unsigned int beginoffset = *srcvidsoffset;
	for(k=beginoffset; k<srcvids.size(); k++){
		graphobj->loadvertexptrsfromfile(col, srcvids[k], container->tempvertexptrs[0], 0, 2); 
		edgessz += container->tempvertexptrs[0][1] - container->tempvertexptrs[0][0];
		if(edgessz >= maxnumedgestoload){ break; }
	}
	*srcvidsoffset = k;
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::countedges(B):: total number of edges: "<<edgessz<<", srcvidsoffset: "<<*srcvidsoffset<<", srcvids.size(): "<<srcvids.size()<<endl;
	#endif 
	return edgessz;
}
void loadgraph::loadactivesubgraph(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], value_t * vertexdatabuffer, unsigned int balancededgesizes[NUMSUBCPUTHREADS], container_t * container){ // in-memory	 	
	unsigned int srcvidsnxtoffset = 0;
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		unsigned int edgeoffset = 0;
		edge_t edgessz = 0;
		vertex_t srcvsz = 0;
		unsigned int hsrcvid = 0;
		unsigned int vpindex = 0;
		kvbuffer[i][BASEOFFSET_VERTEXPTR + vpindex].key = 0;
		for(unsigned int k=srcvidsnxtoffset; k<srcvids.size(); k++){
			#ifdef _DEBUGMODE_HOSTPRINTS2
			cout<<"loadactivesubgraph:: k: "<<k<<", edgeoffset: "<<edgeoffset<<", edgessz: "<<edgessz<<", srcvids["<<k<<"]: "<<srcvids[k]<<endl;
			#endif
			
			graphobj->loadvertexptrsfromfile(col, srcvids[k], container->tempvertexptrs[i], 0, 2); 
			
			edge_t edgesoffset = container->tempvertexptrs[i][0];
			edge_t numedgestoload = container->tempvertexptrs[i][1] - container->tempvertexptrs[i][0]; //NEWCHANGE.
			if(numedgestoload > 0){ graphobj->loadedgesfromfile(col, edgesoffset, (edge2_type *)&kvbuffer[i][BASEOFFSET_EDGESDATA], edgeoffset, numedgestoload); }
			
			// re-assign vertex pointers
			kvbuffer[i][BASEOFFSET_VERTEXPTR + vpindex + 1].key = kvbuffer[i][BASEOFFSET_VERTEXPTR + vpindex].key + numedgestoload;
			kvbuffer[i][BASEOFFSET_VERTEXPTR + vpindex].value = NAp; // vertexdatabuffer[srcvids[k]]; // REMOVEME. FIXME.
			
			// re-assign edges' srcvids
			for(unsigned int n = 0; n < numedgestoload; n++){ kvbuffer[i][BASEOFFSET_EDGESDATA + edgeoffset + n].key = hsrcvid; }
			
			// switch (kv.key = edge.dstvid && kv.value = edge.srcvid)
			for(unsigned int n = 0; n < numedgestoload; n++){ 
				keyy_t key = kvbuffer[i][BASEOFFSET_EDGESDATA + edgeoffset + n].key;
				value_t value = kvbuffer[i][BASEOFFSET_EDGESDATA + edgeoffset + n].value;
				kvbuffer[i][BASEOFFSET_EDGESDATA + edgeoffset + n].key = value;
				kvbuffer[i][BASEOFFSET_EDGESDATA + edgeoffset + n].value = key;
			}
			
			edgeoffset += numedgestoload;
			edgessz += numedgestoload;
			if(numedgestoload > 0){
				vpindex += 1;
				srcvsz += 1;
				hsrcvid += 1;
			}
			
			if(edgessz >= balancededgesizes[i]){
				#ifdef _DEBUGMODE_HOSTPRINTS2
				cout<<"loadactivesubgraph::NOTE: edgessz ("<<edgessz<<") has reached max. srcvidsnxtoffset: ("<<srcvidsnxtoffset<<"). load in next subthread. breaking out..."<<endl; 
				#endif 
				break; }
		}

		srcvidsnxtoffset += srcvsz;
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = srcvsz + 1;
		// container->srcvsize[i] = srcvsz; // NEWCHANGE.
		container->edgessize[i] = edgessz;
		container->runsize[i] = edgessz;
		container->destvoffset[i] = col * KVDATA_RANGE_PERSSDPARTITION;
		container->actvvsize[i] = 0;
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printkeyvalues("loadgraph::loadactivesubgraph.vertexptrs[i]", (keyvalue_t *)&kvbuffer[i][BASEOFFSET_VERTEXPTR], 16);
		utilityobj->printedges("loadgraph::loadactivesubgraph.edgesdata[i]", (edge2_type *)&kvbuffer[i][BASEOFFSET_EDGESDATA], 16); // edgessz
		#endif
	}
	return;
}
void loadgraph::loadactivesubgraph(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int srcvidsoffset, value_t * vertexdatabuffer, 
										edge_t * vertexptrs[NUMSUBCPUTHREADS], value_t * verticesdata[NUMSUBCPUTHREADS], edge2_type * edges[NUMSUBCPUTHREADS], unsigned int balancededgesizes[NUMSUBCPUTHREADS], container_t * container){ // not in-memory			
	unsigned int srcvidsnxtoffset = srcvidsoffset;
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		unsigned int edgeoffset = 0;
		edge_t edgessz = 0;
		vertex_t srcvsz = 0;
		unsigned int hsrcvid = 0;
		unsigned int vpindex = 0;
		vertexptrs[i][vpindex] = 0;
		for(unsigned int k=srcvidsnxtoffset; k<srcvids.size(); k++){
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"loadactivesubgraph:: k: "<<k<<", edgeoffset: "<<edgeoffset<<", edgessz: "<<edgessz<<", srcvids["<<k<<"]: "<<srcvids[k]<<endl;
			#endif
			
			graphobj->loadvertexptrsfromfile(col, srcvids[k], container->tempvertexptrs[i], 0, 2); 
			
			edge_t edgesoffset = container->tempvertexptrs[i][0];
			edge_t numedgestoload = container->tempvertexptrs[i][1] - container->tempvertexptrs[i][0]; //NEWCHANGE.
			if(numedgestoload > 0){ graphobj->loadedgesfromfile(col, edgesoffset, edges[i], edgeoffset, numedgestoload); }
			
			// re-assign vertex pointers
			vertexptrs[i][vpindex + 1] = vertexptrs[i][vpindex] + numedgestoload;
			
			// re-assign vertices data 
			verticesdata[i][vpindex] = vertexdatabuffer[srcvids[k]];
			
			// re-assign edges' srcvids
			for(unsigned int n = 0; n < numedgestoload; n++){ edges[i][edgeoffset + n].srcvid = hsrcvid; }
			
			edgeoffset += numedgestoload;
			edgessz += numedgestoload;
			if(numedgestoload > 0){
				vpindex += 1;
				srcvsz += 1;
				hsrcvid += 1;
			}
			
			if(edgessz >= balancededgesizes[i]){
				#ifdef _DEBUGMODE_HOSTPRINTS2
				cout<<"loadactivesubgraph::NOTE: edgessz ("<<edgessz<<") has reached max. srcvidsnxtoffset: ("<<srcvidsnxtoffset<<"). load in next subthread. breaking out..."<<endl; 
				#endif 
				break; }
		}
		
		srcvidsnxtoffset += srcvsz;
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = srcvsz;
		container->edgessize[i] = edgessz;
		container->runsize[i] = edgessz;
		container->destvoffset[i] = col * KVDATA_RANGE_PERSSDPARTITION;
		container->actvvsize[i] = 0;
		#ifdef _DEBUGMODE_HOSTPRINTS3
		utilityobj->printvalues("loadgraph::loadactivesubgraph.vertexptrs[i]", vertexptrs[i], 16);
		utilityobj->printedges("loadgraph::loadactivesubgraph.edges[i]", edges[i], 16); // edgessz 16
		#endif
	}
	return;
}
unsigned int loadgraph::getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth){
	partition_type partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * treedepth)));
	
	#ifdef _DEBUGMODE_CHECKS//2 // FIXME. too slow
	utilityobj->checkoutofbounds("loadgraph::getglobalpartition", partition, (1 << (NUM_PARTITIONS_POW * treedepth)), keyvalue.key, upperlimit, NAp);
	#endif
	return partition;
}
void loadgraph::loadkvstats(keyvalue_t * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	cout<<"loadgraph::loadkvstats"<<endl;
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		for(unsigned int k=0; k<KVSTATSDRAMSZ; k++){
			kvbuffer[i][BASEOFFSET_STATSDRAM + k].key = 0;
			kvbuffer[i][BASEOFFSET_STATSDRAM + k].value = 0;
		}
	}
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		cout<<"loadgraph::loadkvstats:: runsize[0]["<<i<<"]: "<<container->runsize[i]<<endl;
		keyvalue_t * EEE = (keyvalue_t *)&kvbuffer[i][BASEOFFSET_EDGESDATA];
		keyvalue_t * SSS = (keyvalue_t *)&kvbuffer[i][BASEOFFSET_STATSDRAM];
		for(unsigned int k=0; k<container->runsize[i]; k++){
			keyvalue_t edge = EEE[k];
			
			keyvalue_t keyvalue;
			keyvalue.key = edge.key;
			keyvalue.value = 0;
	
			for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
				
				unsigned int offset = 0;
				for(unsigned int k=0; k<CLOP; k++){ offset += (unsigned int)pow(NUM_PARTITIONS, k); } 
				
				unsigned int partitionCLOP = getglobalpartition(keyvalue, 0, BATCH_RANGE_POW, CLOP);
				#ifdef _DEBUGMODE_CHECKS//2 // FIXME. too slow
				utilityobj->checkoutofbounds("loadgraph::loadkvstats.partitionCLOP", partitionCLOP, pow(NUM_PARTITIONS, TREE_DEPTH), keyvalue.key, NAp, NAp);
				utilityobj->checkoutofbounds("loadgraph::loadkvstats.partitionCLOP", offset + partitionCLOP, KVSTATSDRAMSZ, keyvalue.key, NAp, NAp);
				#endif
				SSS[((offset + partitionCLOP) * VECTOR_SIZE)].value += 1;
			}
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS3
	utilityobj->printkeyvalues("loadkvstats: printing kvbuffer[1][BASEOFFSET_EDGESDATA]", (keyvalue_t *)&kvbuffer[1][BASEOFFSET_EDGESDATA], 16);
	utilityobj->printkeyvalues("loadkvstats: printing kvbuffer[1][BASEOFFSET_EDGESDATA][last]", (keyvalue_t *)&kvbuffer[1][BASEOFFSET_EDGESDATA+container->runsize[0]-32], 16);
	utilityobj->printkeyvalues("loadkvstats: printing kvbuffer[1][BASEOFFSET_STATSDRAM]", (keyvalue_t *)&kvbuffer[1][BASEOFFSET_STATSDRAM], (1+16) * VECTOR_SIZE, VECTOR_SIZE);
	// utilityobj->printkeyvalues("loadkvstats: printing kvbuffer[0][BASEOFFSET_STATSDRAM]", (keyvalue_t *)&kvbuffer[0][BASEOFFSET_STATSDRAM], (1+16+256+4096) * VECTOR_SIZE, VECTOR_SIZE);
	#endif
	return;
}

void loadgraph::loadmessages(uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, unsigned int GraphAlgo){	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printcontainer(container); 
	#endif
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		createmessages(
			kvbuffer[i], // uint512_vec_dt * kvstats,
			container->srcvoffset[i], // unsigned int srcvoffset,
			container->srcvsize[i], // unsigned int srcvsize,
			container->edgessize[i], // unsigned int edgessize,
			container->destvoffset[i], // unsigned int destvoffset,
			container->actvvsize[i], // unsigned int actvvsize,
			NAp, // container->firstvid[i], // unsigned int firstvid,
			NAp, // container->firstvid[i],// unsigned int firstkey,
			NAp, // unsigned int firstvalue,
			TREE_DEPTH, // unsigned int treedepth,
			GraphIter, // unsigned int GraphIter,
			GraphAlgo, // unsigned int GraphAlgo,
			container->runsize[i], // unsigned int runsize,
			BATCH_RANGE, // unsigned int batch_range,
			BATCH_RANGE_POW, // unsigned int batch_range_pow,
			APPLYVERTEXBUFFERSZ, // unsigned int applyvertexbuffersz,
			NUMLASTLEVELPARTITIONS); // unsigned int numlastlevelpartitions
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"loadmessages:: running Acts... size: "<<container->edgessize[i]<<endl; 
		#endif 
	}
	#ifdef _DEBUGMODE_HOSTPRINTS3
	unsigned int totalsize = 0;
	cout<<"loadmessages:: loading Acts... sizes: ["; 
	cout<<"["; for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ totalsize += container->runsize[i]; cout<<container->runsize[i]; if(i<NUMSUBCPUTHREADS-1){ cout<<", "; }} cout<<"]";
	cout<<"]"<<endl;
	cout<<"loadmessages:: total sizes: "<<totalsize<<endl; 
	#endif 
	// exit(EXIT_SUCCESS);
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
		#if defined(INMEMORYGP) && not defined(MERGEPROCESSEDGESANDPARTITIONSTAGE)
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 2;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 0;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		#else 
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = treedepth + 1;
		kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
		
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_BEGINLOP].data[0].key = 1;
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_NUMLOPS].data[0].key = 1;
		// kvstats[BASEOFFSET_MESSAGESDRAM_KVS + MESSAGES_ENDLOP].data[0].key = NAp;
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









