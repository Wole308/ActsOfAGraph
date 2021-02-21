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
#include "../../examples/helperfunctions/evalparams.h"
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
	evalparamsobj = new evalparams();
}
loadgraph::loadgraph(){
	utilityobj = new utility();
	algorithmobj = new algorithm();
}
loadgraph::~loadgraph(){} 

unsigned int loadgraph::getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth){
	unsigned int partition = ((keyvalue.key - upperlimit) >> (BATCH_RANGE_POW - (NUM_PARTITIONS_POW * treedepth)));
	
	#ifdef _DEBUGMODE_CHECKS
	utilityobj->checkoutofbounds("loadgraph::getglobalpartition", partition, (1 << (NUM_PARTITIONS_POW * treedepth)), keyvalue.key, upperlimit, NAp);
	#endif
	return partition;
}

void loadgraph::loadedges_columnwise(unsigned int col, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, value_t * vertexdatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadedges_columnwise:: loading edges (columnwise)... "<<endl;
	#endif 
	unsigned int beginsrcvoffset = 0;
	graphobj->loadvertexptrsfromfile(col, beginsrcvoffset, container->tempvertexptrs[0], 0, 2); 
	unsigned int beginvptr = container->tempvertexptrs[0][0];
	
	edge_t totalnumedges = graphobj->getedgessize(col);
	
	graphobj->loadedgesfromfile(col, beginvptr + totalnumedges - 1, container->tempedges[0], 0, 2);
	unsigned int endsrcvoffset = utilityobj->allignhigher_KV(container->tempedges[0][0].srcvid);
	graphobj->loadvertexptrsfromfile(col, endsrcvoffset, container->tempvertexptrs[0], 0, 2); 
	unsigned int endvptr = container->tempvertexptrs[0][0];
	
	edge_t actualnumedges = endvptr - beginvptr;
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(col);
	graphobj->loadedgesfromfile(col, beginvptr, edgedatabuffer, 0, actualnumedges);
	
	unsigned int edgesszpersubthread = actualnumedges / NUMSUBCPUTHREADS;
	#ifdef _DEBUGMODE_HOSTPRINTS2
	cout<<"loadgraph::loadedges_columnwise:: beginsrcvoffset: "<<beginsrcvoffset<<", endsrcvoffset: "<<endsrcvoffset<<endl;
	cout<<"loadgraph::loadedges_columnwise:: beginvptr: "<<beginvptr<<", endvptr: "<<endvptr<<", totalnumedges: "<<totalnumedges<<", actualnumedges: "<<actualnumedges<<", edgesszpersubthread: "<<edgesszpersubthread<<endl;
	#endif 
	
	// load edges 
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
		#ifdef _DEBUGMODE_HOSTPRINTS2
		cout<<"loadgraph::loadedges_columnwise:: i: "<<i<<", srcvoffset: "<<srcvoffset<<", nextsrcvoffset: "<<nextsrcvoffset<<", srcvsize: "<<srcvsize<<", edgessize: "<<edgessize<<endl;
		#endif 
		
		#ifdef _DEBUGMODE_CHECKS2
		utilityobj->checkforlessthanthan("loadgraph::loadedges_columnwise.nextsrcvoffset & srcvoffset 33", nextsrcvoffset, srcvoffset);
		utilityobj->checkoutofbounds("loadgraph::loadedges_columnwise.edges 34a", edgessize, MAXKVDATA_BATCHSIZE, BASEOFFSET_EDGESDATA, edgessize, NAp);
		utilityobj->checkoutofbounds("loadgraph::loadedges_columnwise.edgedatabuffer 35", localbeginptr + edgessize, PADDEDEDGES_BATCHSIZE, localbeginptr, edgessize, NAp);
		#endif 
		for(unsigned int k=0; k<edgessize; k++){
			edges[i][2*BASEOFFSET_EDGESDATA + k].dstvid = edgedatabuffer[localbeginptr + k].dstvid; 
		}
		
		container->srcvoffset[i] = srcvoffset;
		container->srcvsize[i] = srcvsize;
		container->edgessize[i] = edgessize;
		if(GraphAlgo == PAGERANK){ container->runsize[i] = edgessize; } else { container->runsize[i] = 1; }
		container->destvoffset[i] = col * KVDATA_RANGE_PERSSDPARTITION;
		container->actvvsize[i] = 0;
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printkeyvalues("loadgraph::loadedges_columnwise::first", (keyvalue_t *)&edges[i][BASEOFFSET_EDGESDATA], 16);
		utilityobj->printkeyvalues("loadgraph::loadedges_columnwise::last", (keyvalue_t *)&edges[i][BASEOFFSET_EDGESDATA+edgessize-16], 16);
		#endif
	}
	
	// load vertex ptrs
	for(unsigned int k=0; k<KVDATA_RANGE; k++){
		vptrs[0][2*BASEOFFSET_VERTEXPTR + k].key = vertexptrbuffer[k];
	}
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printkeyvalues("loadgraph::loadedges_columnwise::first", &(keyvalue_t *)vptrs[0][BASEOFFSET_VERTEXPTR], 16);
	utilityobj->printkeyvalues("loadgraph::loadedges_columnwise::last", &(keyvalue_t *)vptrs[0][BASEOFFSET_VERTEXPTR+KVDATA_RANGE-16], 16);
	#endif 
	return;
}

void loadgraph::loadedges_rowwise(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadedges_rowwise:: loading edges (rowwise)... "<<endl;
	#endif 
	unsigned int counts[NUMSUBCPUTHREADS];
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ counts[i] = 0; }
	unsigned int index = 0;
	
	unsigned int packingfactor = 2;
	
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS
		if(vid % 100000 == 0){ cout<<"### loadgraph::loadedges_rowwise:: vid: "<<vid<<", vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		#endif 
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
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
			utilityobj->checkoutofbounds("loadgraph::loadedges_rowwise 23. packingfactor*BASEOFFSET_EDGESDATA + counts[subthread]", packingfactor*BASEOFFSET_EDGESDATA + counts[subthread], PADDEDKVSOURCEDRAMSZ*packingfactor, packingfactor*BASEOFFSET_EDGESDATA, counts[subthread], PADDEDKVSOURCEDRAMSZ*packingfactor);				
			#endif 
		
			edges[subthread][2*BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = edgedatabuffer[index].dstvid;
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"k: "<<k<<", subthread: "<<subthread<<", counts["<<subthread<<"]: "<<counts[subthread]<<", index: "<<index<<", edgedatabuffer["<<index<<"].dstvid: "<<edgedatabuffer[index].dstvid<<endl;
			#endif
			
			counts[subthread] += 1;
			index += 1;
		}
		// load vertex ptrs 
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
			if(vid < KVDATA_RANGE-1){ 
				vptrs[i][2*BASEOFFSET_VERTEXPTR + vid + 1].key = counts[i]; 
			}
		}
	}
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = utilityobj->allignhigherto16_KV(graphobj->get_num_vertices());
		container->edgessize[i] = counts[i]; 
		container->runsize[i] = counts[i]; // FIXME. 1
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::loadedges_rowwise:: index: "<<index<<endl;
	utilityobj->printvalues("loadedges_rowwise.counts", (value_t *)counts, NUMSUBCPUTHREADS);
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadedges_rowwise.vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][2*BASEOFFSET_VERTEXPTR], 8); }
	#endif
}

void loadgraph::loadedges_rowblockwise(unsigned int col, graph * graphobj, edge_t * vertexptrbuffer, edge2_type * edgedatabuffer, vptr_type * vptrs[NUMSUBCPUTHREADS], edge_type * edges[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphAlgo){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadedges_rowblockwise:: loading edges (rowwise)... "<<endl;
	#endif 
	unsigned int counts[NUMSUBCPUTHREADS];
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ counts[i] = 0; }
	unsigned int index1 = 0;
	unsigned int index2 = 0;
	
	unsigned int packingfactor = 2;
	
	for(unsigned int vid=0; vid<KVDATA_RANGE-1; vid++){
		#ifdef _DEBUGMODE_HOSTPRINTS
		if(vid % 100000 == 0){ cout<<"### loadgraph::loadedges_rowblockwise:: vid: "<<vid<<", vptr_begin: "<<vertexptrbuffer[vid]<<endl; }
		#endif 
		
		edge_t vptr_begin = vertexptrbuffer[vid];
		edge_t vptr_end = vertexptrbuffer[vid+1];
		edge_t edges_size = vptr_end - vptr_begin;
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"loadgraph::loadedges_rowblockwise:: vptr_begin: "<<vptr_begin<<endl;
		utilityobj->printvalues("loadedges_rowblockwise.counts", (value_t *)counts, NUMSUBCPUTHREADS);
		cout<<"loadgraph::loadedges_rowblockwise:: vptr_end: "<<vptr_end<<endl;
		cout<<"loadgraph::loadedges_rowblockwise:: edges_size: "<<edges_size<<endl;
		#endif
		
		unsigned int subthread = 0;
		unsigned int k=0;
		while(k<edges_size){
			subthread = (index1 / VECTOR2_SIZE) % NUMSUBCPUTHREADS;
			
			// >>> setting header metadata
			if(counts[subthread] % VECTOR2_SIZE == 0){
				edges[subthread][2*BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = edgedatabuffer[index2].srcvid; // INVALIDDATA, 777
				counts[subthread] += 1;
				index1 += 1;
				continue;
			}
			
			#ifdef _DEBUGMODE_CHECKS2
			utilityobj->checkoutofbounds("loadgraph::loadedges_rowblockwise 23. packingfactor*BASEOFFSET_EDGESDATA + counts[subthread]", packingfactor*BASEOFFSET_EDGESDATA + counts[subthread], PADDEDKVSOURCEDRAMSZ*packingfactor, packingfactor*BASEOFFSET_EDGESDATA, counts[subthread], PADDEDKVSOURCEDRAMSZ*packingfactor);				
			#endif 
		
			edges[subthread][2*BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = edgedatabuffer[index2].dstvid;
			#ifdef _DEBUGMODE_HOSTPRINTS
			cout<<"k: "<<k<<", subthread: "<<subthread<<", counts["<<subthread<<"]: "<<counts[subthread]<<", index2: "<<index2<<", edgedatabuffer["<<index2<<"].dstvid: "<<edgedatabuffer[index2].dstvid<<endl;
			#endif
			
			counts[subthread] += 1;
			index1 += 1;
			index2 += 1;
			k += 1;
		}
		
		// >>> fill in the blanks
		if(counts[subthread] % VECTOR2_SIZE != 0 && edges_size > 0){
			unsigned int sz = VECTOR2_SIZE - (counts[subthread] % VECTOR2_SIZE);
			for(unsigned int v=0; v<sz; v++){
				edges[subthread][2*BASEOFFSET_EDGESDATA + counts[subthread]].dstvid = INVALIDDATA;
				counts[subthread] += 1;
				index1 += 1;
			}
		}
		
		// load vertex ptrs 
		for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
			if(vid < KVDATA_RANGE-1){ 
				vptrs[i][2*BASEOFFSET_VERTEXPTR + vid + 1].key = counts[i]; 
				
				#ifdef _DEBUGMODE_CHECKS2
				if(counts[i] % 16 != 0){ cout<<"loadedges_rowblockwise:: ERROR. counts["<<i<<"] % 16 != 0. counts["<<i<<"]: "<<counts[i]<<". exiting..."<<endl; exit(EXIT_FAILURE); }
				#endif 
			}
		}
	}
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		container->srcvoffset[i] = 0;
		container->srcvsize[i] = utilityobj->allignhigherto16_KV(graphobj->get_num_vertices());
		container->edgessize[i] = counts[i]; 
		container->runsize[i] = counts[i]; // FIXME. 1
		container->destvoffset[i] = 0;
		container->actvvsize[i] = 0;
	}
	
	// >>> dummy overflows
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int k=counts[i]; k<counts[i] + (512*2*8); k++){
			edges[i][2*BASEOFFSET_EDGESDATA + k].dstvid = UNUSEDDATA;
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::loadedges_rowblockwise:: index1: "<<index1<<", index2: "<<index2<<endl;
	utilityobj->printvalues("loadgraph::loadedges_rowblockwise.counts", (value_t *)counts, NUMSUBCPUTHREADS);
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadgraph::loadedges_rowblockwise.edges["+std::to_string(i)+"][~]", (value_t *)&edges[i][2*BASEOFFSET_EDGESDATA], 8); }
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ utilityobj->printvalues("loadedges_rowblockwise.vptrs["+std::to_string(i)+"][~]", (value_t *)&vptrs[i][2*BASEOFFSET_VERTEXPTR], 8); }
	#endif
	#ifdef _DEBUGMODE_HOSTPRINTS
	unsigned int numvaliditems = 0;
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){ 
		for(unsigned int k=0; k<counts[i]; k++){ 
			unsigned int value = edges[i][2*BASEOFFSET_EDGESDATA + k].dstvid;
			if(value == INVALIDDATA || value == 777){ }
			else { numvaliditems += 1; }
		}
	}
	cout<<"loadgraph::loadedges_rowblockwise:: number of valid items: "<<numvaliditems<<endl;
	#endif
}

void loadgraph::loadoffsetmarkers(edge_type * edges[NUMSUBCPUTHREADS], keyvalue_t * stats[NUMSUBCPUTHREADS], container_t * container){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadoffsetmarkers:: loading offset markers... "<<endl;
	#endif 
	unsigned int baseoffset_edgedata = 2*BASEOFFSET_EDGESDATA;
	
	unsigned int totalnumpartitions = 0;
	for(unsigned int k=0; k<=TREE_DEPTH; k++){ totalnumpartitions += (unsigned int)pow(NUM_PARTITIONS, k); } 
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][BASEOFFSET_STATSDRAM];
		for(unsigned int k=0; k<KVSTATSSZ; k++){
			statsptrVec[k].data[0].key = 0;
			statsptrVec[k].data[0].value = 0;
		}
	}
	
	keyvalue_t * tempstats = new keyvalue_t[KVSTATSDRAMSZ];
	for(unsigned int k=0; k<KVSTATSDRAMSZ; k++){ tempstats[k].key = 0; tempstats[k].value = 0; }
	
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		#ifdef _DEBUGMODE_HOSTPRINTS
		cout<<"loadgraph::loadoffsetmarkers:: edgessize[0]["<<i<<"]: "<<container->edgessize[i]<<endl;
		#endif 
		edge_type * edgesptr = (edge_type *)&edges[i][baseoffset_edgedata];	
		keyvalue_t * statsptr = (keyvalue_t *)&stats[i][BASEOFFSET_STATSDRAM];
		for(unsigned int k=0; k<KVSTATSDRAMSZ; k++){ tempstats[k].key = 0; tempstats[k].value = 0; }
		
		for(unsigned int k=0; k<container->edgessize[i]; k++){
			#ifdef _DEBUGMODE_HOSTPRINTS
			if(k % 100000 == 0){ cout<<"### loadgraph::loadoffsetmarkers:: k: "<<k<<endl; }
			#endif 
			
			keyvalue_t keyvalue;
			keyvalue.key = edgesptr[k].dstvid;
			keyvalue.value = 0;
			
			if(keyvalue.key == INVALIDDATA){ keyvalue.key = 0; } // >>> for loadedges_rowblockwise
			
			for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
				
				unsigned int offset = 0;
				for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); } 
				
				unsigned int partitionCLOP = getglobalpartition(keyvalue, 0, BATCH_RANGE_POW, CLOP);
				
				#ifdef _DEBUGMODE_CHECKS
				utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.partitionCLOP", partitionCLOP, pow(NUM_PARTITIONS, TREE_DEPTH), keyvalue.key, NAp, NAp);
				utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.partitionCLOP", offset + partitionCLOP, KVSTATSDRAMSZ, keyvalue.key, NAp, NAp);
				#endif
				tempstats[offset + partitionCLOP].value += 1;
			}
		}
		
		for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
			unsigned int offset = 0;
			for(unsigned int k=0; k<CLOP; k++){ offset += (1 << (NUM_PARTITIONS_POW * k)); }
			
			utilityobj->checkoutofbounds("loadgraph::loadoffsetmarkers.offset", offset, KVSTATSDRAMSZ, NAp, NAp, NAp);
			utilityobj->getmarkerpositions((keyvalue_t *)&tempstats[offset], (unsigned int)pow(NUM_PARTITIONS, CLOP));
		}
		
		uint512_vec_dt * statsptrVec = (uint512_vec_dt *)&stats[i][BASEOFFSET_STATSDRAM];
		for(unsigned int k=0; k<KVSTATSSZ; k++){
			statsptrVec[k].data[0].key = tempstats[k].key;
			statsptrVec[k].data[0].value = 0;
			// statsptrVec[k].data[0].value = tempstats[k].value;
		}
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats [after]", tempstats, totalnumpartitions);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing statsptr [after]", (keyvalue_t *)statsptrVec, totalnumpartitions * VECTOR_SIZE, VECTOR_SIZE);
		#endif
		
		unsigned int totalnumpartitionsb4last = 0;
		for(unsigned int k=0; k<TREE_DEPTH; k++){ totalnumpartitionsb4last += (unsigned int)pow(NUM_PARTITIONS, k); } 
		for(unsigned int k=0; k<totalnumpartitionsb4last; k++){
			if(tempstats[k].key >= KVDRAMSZ){ 
				cout<<"loadoffsetmarkers::tempstats 33. ERROR. out of bounds. (tempstats["<<k<<"].key("<<tempstats[k].key<<") >= KVDRAMSZ("<<KVDRAMSZ<<")) exiting..."<<endl; 
				utilityobj->printkeyvalues("loadoffsetmarkers: printing tempstats", tempstats, k);
				exit(EXIT_FAILURE); 
			}
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	for(unsigned int i=0; i<1; i++){ // NUMSUBCPUTHREADS
		utilityobj->printkeyvalues("loadoffsetmarkers: printing edges[i][BASEOFFSET_EDGESDATA]", (keyvalue_t *)&edges[i][baseoffset_edgedata], 16);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing edges[i][BASEOFFSET_EDGESDATA][last]", (keyvalue_t *)&edges[i][baseoffset_edgedata+container->runsize[i]-32], 16);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing stats[i][BASEOFFSET_STATSDRAM]", (keyvalue_t *)&stats[i][BASEOFFSET_STATSDRAM], (1+16) * VECTOR_SIZE, VECTOR_SIZE);
		utilityobj->printkeyvalues("loadoffsetmarkers: printing stats[i][BASEOFFSET_STATSDRAM]", (keyvalue_t *)&stats[i][BASEOFFSET_STATSDRAM], totalnumpartitions * VECTOR_SIZE, VECTOR_SIZE);
	}
	#endif
	// exit(EXIT_SUCCESS);
	return;
}

void loadgraph::loadvertexptrs(unsigned int col, edge_t * vertexptrbuffer, value_t * vertexdatabuffer, vptr_type * kvbuffer[NUMSUBCPUTHREADS], container_t * container){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadvertexptrs:: loading vertex pointers... "<<endl;
	#endif 
	vertexptrbuffer = graphobj->loadvertexptrsfromfile(col);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){
		for(unsigned int k=0; k<KVDATA_RANGE; k++){
			kvbuffer[i][2*BASEOFFSET_VERTEXPTR + k].key = vertexptrbuffer[k];
		}
		
		#ifdef _DEBUGMODE_HOSTPRINTS
		utilityobj->printkeyvalues("loadgraph::loadvertexptrs::first", (keyvalue_t *)&kvbuffer[i][BASEOFFSET_VERTEXPTR], 16);
		utilityobj->printkeyvalues("loadgraph::loadvertexptrs::last", (keyvalue_t *)&kvbuffer[i][BASEOFFSET_VERTEXPTR+KVDATA_RANGE-16], 16);
		#endif 
	}
	return;
}

void loadgraph::loadvertexdata(value_t * vertexdatabuffer, keyvalue_t * kvbuffer, vertex_t offset, vertex_t size){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadvertexdata:: loading vertex datas... "<<endl;
	#endif 
	for(unsigned int i = 0; i < 1; i++){
		memcpy(&kvbuffer[BASEOFFSET_VERTICESDATA], &vertexdatabuffer[offset], size * sizeof(value_t));
		#ifdef _DEBUGMODE_HOSTPRINTS2
		utilityobj->printkeyvalues("loadgraph::loadvertexdata", &kvbuffer[BASEOFFSET_VERTICESDATA], 16);
		#endif
	}
	return;
}

void loadgraph::loadactvvertices(vector<vertex_t> &activevertices, keyy_t * kvbuffer, container_t * container){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::loadactvvertices:: loading active vertices... "<<endl;
	#endif
	
	for(unsigned int k=0; k<activevertices.size(); k++){
		kvbuffer[2*BASEOFFSET_ACTIVEVERTICES + k] = activevertices[k];
	}
	container->actvvsize[0] = activevertices.size();
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printkeyvalues("loadgraph::loadactvvertices", (keyvalue_t *)&kvbuffer[2*BASEOFFSET_ACTIVEVERTICES], utilityobj->hmin(16, activevertices.size()));
	#endif
	return;
}

void loadgraph::savevmasks(bool_type enable, uint512_vec_dt * kvbuffer, keyvalue_vec_bittype vmask[NUM_PARTITIONS][REDUCEBUFFERSZ], batch_type offset_kvs, buffer_type size_kvs){
	#ifdef _DEBUGMODE_HOSTPRINTS
	cout<<"loadgraph::savevmasks:: saving vmask... "<<endl;
	#endif
	unsigned int bitsbuffer[REDUCEBUFFERSZ];
	keyvalue_t tempbuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	
	for (buffer_type i=0; i<REDUCEBUFFERSZ; i++){
		for(unsigned int k=0; k<NUM_PARTITIONS; k++){
			utilityobj->WRITETO_UINT(&bitsbuffer[i], 2*k, 1, vmask[k][i].key);
			utilityobj->WRITETO_UINT(&bitsbuffer[i], 2*k+1, 1, vmask[k][i].value);
		}
	}
	
	unsigned int index = 0;
	for (buffer_type i=0; i<VMASKBUFFERSZ_KVS; i++){
		for (vector_type v=0; v<VECTOR_SIZE; v++){
			#ifdef _DEBUGMODE_CHECKS2
			utilityobj->checkoutofbounds("loadgraph::savevmasks_h 1", index, REDUCEBUFFERSZ, NAp, NAp, NAp);
			utilityobj->checkoutofbounds("loadgraph::savevmasks_h 1", i, VMASKBUFFERSZ_KVS, NAp, NAp, NAp);
			#endif
			tempbuffer[v][i].key = bitsbuffer[index]; 
			tempbuffer[v][i].value = bitsbuffer[index+1]; 
			index += 2;
		}
	}
	
	for (buffer_type i=0; i<size_kvs; i++){
		for(unsigned int v=0; v<VECTOR_SIZE; v++){
			kvbuffer[offset_kvs + i].data[v] = tempbuffer[v][i];
		}
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printvalues("savevmasks.bitsbuffer", bitsbuffer, 4);
	utilityobj->printkeyvalues("savevmasks.vmask", vmask, NUM_PARTITIONS, 4);
	utilityobj->printkeyvalues("savevmasks.tempbuffer", tempbuffer, 8, 4);
	utilityobj->printkeyvalues("savevmasks.kvbuffer[0]", (keyvalue_t *)&kvbuffer[offset_kvs], 4);
	#endif 
	// exit(EXIT_SUCCESS);
	return;
}
void loadgraph::generatevmaskdata(vector<vertex_t> &activevertices, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS]){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::generatevmaskdata:: generating vmask... "<<endl;
	#endif
	
	keyvalue_vec_bittype vmask[NUM_PARTITIONS][REDUCEBUFFERSZ];
	// keyvalue_t kvdrambuffer[VECTOR_SIZE][BLOCKRAM_SIZE];
	
	for(unsigned int i=0; i<NUMSUBCPUTHREADS; i++){
		for(unsigned int k=0; k<VERTICESDATAMASKSZ_KVS; k++){ 
			for(unsigned int v=0; v<VECTOR_SIZE; v++){
				kvbuffer[i][BASEOFFSET_VERTICESDATAMASK_KVS + k].data[v].key = 0;
				kvbuffer[i][BASEOFFSET_VERTICESDATAMASK_KVS + k].data[v].value = 0;
			}
		}
	}
	
	unsigned int vmaskoffset_kvs = 0;
	for(unsigned int offset=0; offset<BATCH_RANGE; offset+=REDUCESZ * NUM_PARTITIONS){
	// for(unsigned int offset=0; offset<8 * REDUCESZ * NUM_PARTITIONS; offset+=REDUCESZ * NUM_PARTITIONS){
		// cout<<"offset: "<<offset<<endl;
		for(unsigned int i=0; i<NUM_PARTITIONS; i++){
			unsigned int * V = (unsigned int *)vmask[i]; 
			for(unsigned int k=0; k<REDUCESZ; k++){
				unsigned int vid = offset + (i*REDUCESZ) + k;
				if(vid==1){ V[k] = 1; }
				else{ V[k] = 0; }
			}
		}
		
		for(unsigned int i=0; i<1; i++){ savevmasks(ON, kvbuffer[i], vmask, BASEOFFSET_VERTICESDATAMASK_KVS + vmaskoffset_kvs, VMASKBUFFERSZ_KVS); }
		vmaskoffset_kvs += VMASKBUFFERSZ_KVS;
	}
	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printkeyvalues("generatevmaskdata.vmask", vmask, NUM_PARTITIONS, 4);
	utilityobj->printkeyvalues("generatevmaskdata.kvbuffer[0]", (keyvalue_t *)&kvbuffer[0][BASEOFFSET_VERTICESDATAMASK_KVS], 4);
	cout<<"loadgraph::generatevmaskdata:: end. "<<endl;
	#endif 
	// exit(EXIT_SUCCESS);
	return;
}

void loadgraph::setrootvid(value_t * kvbuffer, vector<vertex_t> &activevertices){
	#ifdef _DEBUGMODE_HOSTPRINTS3
	cout<<"loadgraph::setrootvid:: setting root vid(s)... "<<endl;
	#endif 
	
	for(unsigned int i = 0; i < activevertices.size(); i++){
		unsigned int vid = activevertices[i];
		kvbuffer[2*BASEOFFSET_VERTICESDATA + vid] = 0;
	}
	return;
}

void loadgraph::loadmessages(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, unsigned int GraphAlgo){	
	#ifdef _DEBUGMODE_HOSTPRINTS
	utilityobj->printcontainer(container); 
	#endif
	unsigned int totalkvstatssize = 0;
	for(unsigned int CLOP=1; CLOP<=TREE_DEPTH; CLOP++){
		for(unsigned int k=0; k<CLOP; k++){ totalkvstatssize += (1 << (NUM_PARTITIONS_POW * k)); }
	}
	
	createmessages(
			vdram, // uint512_vec_dt * kvstats,
			container->srcvoffset[0], // unsigned int srcvoffset,
			container->srcvsize[0], // unsigned int srcvsize,
			container->edgessize[0], // unsigned int edgessize,
			container->destvoffset[0], // unsigned int destvoffset,
			container->actvvsize[0], // unsigned int actvvsize,
			NAp, // container->firstvid[i], // unsigned int firstvid,
			NAp, // container->firstvid[i],// unsigned int firstkey,
			NAp, // unsigned int firstvalue,
			TREE_DEPTH, // unsigned int treedepth,
			GraphIter, // unsigned int GraphIter,
			GraphAlgo, // unsigned int GraphAlgo,
			container->runsize[0]); // unsigned int runsize
			
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
			container->runsize[i]); // unsigned int runsize
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
	return;
}
void loadgraph::createmessages(
			uint512_vec_dt * kvbuffer,
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
			unsigned int runsize){
	#ifdef _DEBUGMODE_CHECKS2
	if(runsize > MAXKVDATA_BATCHSIZE){ cout<<"loadgraph::createmessages::ERROR. runsize too large!. runsize: "<<runsize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	if(edgessize > MAXKVDATA_BATCHSIZE){ cout<<"loadgraph::createmessages::ERROR. edgessize too large!. edgessize: "<<edgessize<<", MAXKVDATA_BATCHSIZE: "<<MAXKVDATA_BATCHSIZE<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	// if(actvvsize > ACTIVEVERTICESSZ){ cout<<"loadgraph::createmessages::ERROR. actvvsize too large!. actvvsize: "<<actvvsize<<", ACTIVEVERTICESSZ: "<<ACTIVEVERTICESSZ<<". EXITING"<<endl; exit(EXIT_FAILURE); }
	#endif
	
	unsigned int kvstatssz = 0;
	for(unsigned int CLOP=0; CLOP<=TREE_DEPTH; CLOP++){ kvstatssz += (1 << (NUM_PARTITIONS_POW * CLOP)); }
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_RUNKERNELCOMMAND].data[0].key = ON;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PROCESSCOMMAND].data[0].key = ON; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_PARTITIONCOMMAND].data[0].key = ON;  
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ENABLE_APPLYUPDATESCOMMAND].data[0].key = ON;  

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_MESSAGESDATA].data[0].key  = BASEOFFSET_MESSAGESDATA_KVS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_EDGESDATA].data[0].key = BASEOFFSET_EDGESDATA_KVS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTEXPTR].data[0].key = BASEOFFSET_VERTEXPTR_KVS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATA].data[0].key = BASEOFFSET_VERTICESDATA_KVS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_ACTIVEVERTICES].data[0].key = BASEOFFSET_ACTIVEVERTICES_KVS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_VERTICESDATAMASK].data[0].key = BASEOFFSET_VERTICESDATAMASK_KVS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_STATSDRAM].data[0].key = BASEOFFSET_STATSDRAM_KVS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAM].data[0].key = BASEOFFSET_KVDRAM_KVS;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_BASEOFFSETKVS_KVDRAMWORKSPACE].data[0].key = BASEOFFSET_KVDRAMWORKSPACE_KVS;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_MESSAGESDATA].data[0].key = MESSAGESDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_EDGES].data[0].key = edgessize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTEXPTRS].data[0].key = srcvsize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATA].data[0].key = VERTICESDATASZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_ACTIVEVERTICES].data[0].key = actvvsize; // 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_VERTICESDATAMASK].data[0].key = VERTICESDATAMASKSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVSTATSDRAM].data[0].key = kvstatssz;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAM].data[0].key = KVDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_KVDRAMWORKSPACE].data[0].key = KVSTATSDRAMSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_APPLYVERTEXBUFFER].data[0].key = APPLYVERTEXBUFFERSZ;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_BATCHRANGE].data[0].key = BATCH_RANGE; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_SIZE_RUN].data[0].key = runsize; //

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_MESSAGESDRAM].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_EDGES].data[0].key = NAp; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTEXPTRS].data[0].key = NAp; 
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATA].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_ACTIVEVERTICES].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_VERTICESDATAMASK].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVSTATSDRAM].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAM].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_KVDRAMWORKSPACE].data[0].key = NAp;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_APPLYVERTEXBUFFER].data[0].key = APPLYVERTEXBUFFERSZ_POW; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_POW_BATCHRANGE].data[0].key = BATCH_RANGE_POW; //
	
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHITERATIONID].data[0].key = GraphIter;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ALGORITHMINFO_GRAPHALGORITHMID].data[0].key = GraphAlgo;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_BEGINLOP].data[0].key = 1;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_NUMLOPS].data[0].key = TREE_DEPTH;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_TREEDEPTH].data[0].key = TREE_DEPTH;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_FINALNUMPARTITIONS].data[0].key = NUMLASTLEVELPARTITIONS; //
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVOFFSET].data[0].key = srcvoffset;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_SRCVSIZE].data[0].key = srcvsize;
	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_ACTSPARAMS_DESTVOFFSET].data[0].key = destvoffset;

	kvbuffer[BASEOFFSET_MESSAGESDATA_KVS + MESSAGES_RETURN_RETURNVALUES].data[0].key = MESSAGES_RETURN_RETURNVALUES;
	return;
}

void loadgraph::setcustomeval(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[NUMSUBCPUTHREADS], unsigned int evalid){
	evalparamsobj->loadevalparams(evalid, vdram);
	for(unsigned int i = 0; i < NUMSUBCPUTHREADS; i++){ 
		evalparamsobj->loadevalparams(evalid, kvbuffer[i]);
	}
	return;
}






