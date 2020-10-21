#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../../src/parameters/parameters.h"
#include "../../src/graphs/graph.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "edge_process.h"
using namespace std;

edge_process::edge_process(graph * _graphobj, stats * _statsobj){		
	cout<<"edge_process::edge_process:: constructor called. "<<endl;
	graphobj = _graphobj;
	parametersobj = new parameters();
	utilityobj = new utility();
	algorithmobj = new algorithm();
	statsobj = _statsobj;
	
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ edgebuffer[i] = new edgeprop1_t[KVDATA_BATCHSIZE]; }
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ vertexpointerbuffer[i] = new prvertexoffset_t[KVDATA_BATCHSIZE]; }
	vertexpropertybuffer = graphobj->getvertexpropertybuffer();
	vertexdatabuffer = graphobj->getvertexdatabuffer();
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ currentvid[i] = 0; }
	
	m_idx_buffer_offset = 0;
	m_idx_buffer_bytes = 0;
	mp_idx_buffer = aligned_alloc(512, m_buffer_alloc_bytes);
	m_edge_buffer_offset = 0;
	m_edge_buffer_bytes = 0;
	mp_edge_buffer = aligned_alloc(512, m_buffer_alloc_bytes);
	m_index_blocks_read = 0;
	m_edge_blocks_read = 0;
}
edge_process::~edge_process(){} 

void edge_process::generateupdates(unsigned int groupid, unsigned int col, unsigned int fdoffset[NUMCPUTHREADS], keyvalue_t * batch[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int datasize[NUMCPUTHREADS], unsigned int voffset){							
	#ifdef LOCKE
	for (int i = 0; i < NUMCPUTHREADS; i++){ generateupdates_stream(i, groupid, col, fdoffset[i], batch[i], batchoffset[i], batchsize[i], datasize[i], voffset); }
	#else
	for (int i = 0; i < NUMCPUTHREADS; i++){ mythread[i] = std::thread(&edge_process::generateupdates_stream, this, i, groupid, col, fdoffset[i], batch[i], batchoffset[i], batchsize[i], datasize[i], voffset); }
	for (int i = 0; i < NUMCPUTHREADS; i++){ mythread[i].join(); }
	#endif
	for(unsigned int i = 0; i < NUMCPUTHREADS; i++){ statsobj->appendkeyvaluecount(col, datasize[i]); }
	return;
}
unsigned int edge_process::generateupdates_stream(int ithreadidx, unsigned int groupid, unsigned int col, unsigned int fdoffset, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], vertex_t datasize, unsigned int voffset){			
	graphobj->loadedgesfromfile(col, fdoffset, edgebuffer[ithreadidx], 0, datasize); 
	
	graphobj->loadvertexpointersfromfile(col, fdoffset, vertexpointerbuffer[ithreadidx], 0, datasize); 
	
	unsigned int kvcount = generatekeyvalues_stream(ithreadidx, groupid, batch, batchoffset, batchsize, datasize, voffset); 
	return kvcount;
}
unsigned int edge_process::generatekeyvalues_stream(int ithreadidx, unsigned int groupid, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], vertex_t datasize, unsigned int voffset){
	unsigned int tempbatchoffset[NUMSUBCPUTHREADS]; // LEARNFROMME. avoids significant bottleneck
	unsigned int tempbatchsize[NUMSUBCPUTHREADS];
	utilityobj->copy(tempbatchoffset, batchoffset, NUMSUBCPUTHREADS);
	utilityobj->copy(tempbatchsize, batchsize, NUMSUBCPUTHREADS);
	
	vertexprop_t vertexprop; vertexprop.outdegree = 1;
	vertex_t tempcurrentvid = currentvid[ithreadidx];
	for(unsigned int i=0; i<datasize; i++){
		edgeprop1_t edge = edgebuffer[ithreadidx][i];
		
		unsigned int edgeoffsetbit = utilityobj->RetrieveBit((unsigned int *)vertexpointerbuffer[ithreadidx], i); 
		if(edgeoffsetbit==1){
			utilityobj->checkoutofbounds("edge_process::generatekeyvalues_stream 1", tempcurrentvid, KVDATA_RANGE, i, tempcurrentvid, NAp);
			vertexprop = vertexpropertybuffer[tempcurrentvid]; 
			tempcurrentvid+=1;
		}

		unsigned int outdegree = vertexprop.outdegree;
		unsigned int vdata = 1; // FIXME.
		
		#ifdef PR_ALGORITHM
		if(outdegree == 0){ outdegree = 1; } // REMOVEME.
		value_t edgeval = 0.1 + ((1 - 0.1) * (vdata / outdegree));
		#elif defined(BFS_ALGORITHM)
		value_t edgeval = tempcurrentvid;
		#elif defined(BC_ALGORITHM)
		value_t edgeval = tempcurrentvid;
		#endif
		
		keyvalue_t data;
		data.key = edge.dstvid;
		data.value = edgeval;
		utilityobj->checkoutofbounds("edge_process::generatekeyvalues_stream 2", i, KVDATA_BATCHSIZE, NAp, NAp, NAp);
		
		insertkeyvaluetobuffer(batch, tempbatchoffset, tempbatchsize, data, voffset, groupid);
	}
	currentvid[ithreadidx] = tempcurrentvid;
	utilityobj->copy(batchsize, tempbatchsize, NUMSUBCPUTHREADS);
	return datasize;
}

unsigned int edge_process::insertkeyvaluetobuffer(keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], keyvalue_t keyvalue, unsigned int voffset, unsigned int groupid){
	#if NUMSUBCPUTHREADS == 1
	unsigned int partition = 0;
	#else 
	unsigned int partition = (keyvalue.key - voffset) / parametersobj->GET_BATCH_RANGE(groupid);
	#endif 
	
	utilityobj->checkoutofbounds("edge_process::insertkeyvaluetobuffer 2", partition, NUMSUBCPUTHREADS, keyvalue.key, parametersobj->GET_BATCH_RANGE(groupid), voffset); //  (MESSAGESDRAMSZ + KVDRAMBUFFERSZ + KVDRAMSZ)

	batch[partition][batchoffset[partition] + batchsize[partition]] = keyvalue;
	batchsize[partition] += 1;
	return partition;
}

runsummary_t edge_process::timingandsummary(unsigned int graph_iterationidx, long double totaltime_ms){
	return statsobj->timingandsummary(graph_iterationidx, totaltime_ms);
}
void edge_process::resetstatevariables(){
	for(unsigned int i=0; i<NUMCPUTHREADS; i++){ currentvid[i] = 0; }
	return;
}



