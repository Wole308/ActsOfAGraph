#ifndef EDGE_PROCESS_H
#define EDGE_PROCESS_H
#include <thread>
#include "EdgeProcess.h" 
#include "VertexValues.h" 
#include "sortreduce.h" 
#include "filekvreader.h" 
#include "../../src/graphs/graph.h"
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/dataaccess/dataaccess.h"
#include "../../src/stats/stats.h"
#include "../../src/graphs/create2Dgraph.h" // REMOVEME. just for debugging.
#include "../../include/common.h"
using namespace std;

class edge_process {
public:
	edge_process(graph * _graphobj);
	~edge_process();
	
	void generateupdates(unsigned int bank, unsigned int col, unsigned int fdoffset[NUMCPUTHREADS], keyvalue_t * batch[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int datasize[NUMCPUTHREADS], unsigned int voffset);			
	unsigned int generateupdates_stream(int ithreadidx, unsigned int bank, unsigned int col, unsigned int fdoffset, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], vertex_t datasize, unsigned int voffset);			
	unsigned int generatekeyvalues_stream(int ithreadidx, unsigned int bank, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], vertex_t datasize, unsigned int voffset);
	
	void generateupdates(unsigned int readerbank, unsigned int bank, unsigned int col, keyvalue_t * batch[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS], unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS], kvresults_t * results);
	void generateupdates_random(int resultbank, unsigned int readerbank, unsigned int bank, unsigned int col, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], kvresults_t * results);
	void generatekeyvalues_random(vertex_t key, value_t val, unsigned int bank, unsigned int col, keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], unsigned int * keyvaluesread);
	
	void generateupdates(unsigned int readerbank, unsigned int bank, unsigned int col, keyvalue_t * batch[NUMCPUTHREADS], vertex_t batchoffset, kvresults_t * results);
	void generateupdates_random(int resultbank, unsigned int readerbank, unsigned int bank, unsigned int col, keyvalue_t * batch, vertex_t batchoffset, kvresults_t * results);
	void generatekeyvalues_random(vertex_t key, value_t val, unsigned int bank, unsigned int col, keyvalue_t * batch, vertex_t batchoffset, unsigned int * keyvaluesread);

	unsigned int insertkeyvaluetobuffer(keyvalue_t * batch[NUMSUBCPUTHREADS], unsigned int batchoffset[NUMSUBCPUTHREADS], unsigned int batchsize[NUMSUBCPUTHREADS], keyvalue_t keyvalue, unsigned int voffset);
	
	runsummary_t timingandsummary(unsigned int graph_iterationidx, long double totaltime_ms);
	void resetstatevariables();
	
private:
	graph * graphobj;
	utility * utilityobj;
	algorithm * algorithmobj;
	dataaccess * dataaccessobj;
	stats * statsobj;
	
	edgeprop1_t * edgebuffer[NUMCPUTHREADS];
	prvertexoffset_t * vertexpointerbuffer[NUMCPUTHREADS];
	value_t * vertexdatabuffer[MAXNUMVERTEXBANKS];
	vertexprop_t * vertexpropertybuffer[MAXNUMVERTEXBANKS];
	std::thread mythread[NUMCPUTHREADS];
	unsigned int currentvid[NUMCPUTHREADS];
	
	size_t m_idx_buffer_offset;
	size_t m_idx_buffer_bytes;
	void * mp_idx_buffer;
	uint64_t m_edge_buffer_offset;
	size_t m_edge_buffer_bytes;
	void * mp_edge_buffer;
	static const size_t m_buffer_alloc_bytes = (1024*2);
	std::atomic<size_t> m_index_blocks_read;
	std::atomic<size_t> m_edge_blocks_read;
	
	// #ifdef _DEBUGMODE_CHECKS // REMOVEME. just for debugging.
	create2Dgraph * create2Dgraphobj;
	// #endif 
};
#endif





