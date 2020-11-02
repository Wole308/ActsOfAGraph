#ifndef BFS_H
#define BFS_H
#include "../../src/utility/utility.h"
#include "../../src/algorithm/algorithm.h"
#include "../../src/graphs/graph.h"
#include "../../src/dataset/dataset.h"
#include "../../examples/helperfunctions/helperfunctions.h"
#include "../../examples/helperfunctions/helperfunctions2.h"
#include "../../src/stats/stats.h"
#include "../../include/common.h"
#include "../include/examplescommon.h"
#include "bfs.h"

/* typedef struct {
	unsigned int srcvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int srcvsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int destvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int firstvid[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int edgeoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int edgesize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int runsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]; 
	unsigned int numedgesretrieved[NUMSSDPARTITIONS];
	unsigned int totalnumedgesinfile[NUMSSDPARTITIONS];
	edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	edge_t * tempvertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS];
} container_t; */

class bfs {
public:
	bfs(unsigned int algorithmid, unsigned int datasetid, std::string binaryFile);
	~bfs();
	void finish();
	
	runsummary_t run();
	void WorkerThread(unsigned int col, hostglobalparams_t globalparams, vector<vertex_t> &activevertices, container_t * container);
	
	void loadgraphdata(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, container_t * container);
	void loadbalancedgraphdata(unsigned int col, graph * graphobj, vector<vertex_t> &srcvids, unsigned int balancededgesizes[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);
	// void trim(container_t * container);
	// void loadsourcevertices(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], vector<vertex_t> &srcvids, container_t * container);
	// void loaddestvertices(value_t * vertexdatabuffer, keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS]);
	// void loadedges(keyvalue_t * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);
	// void loadmessages(uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS], container_t * container);
	
private:
	graph * graphobj;
	parameters * parametersobj[NUMSUPERCPUTHREADS];
	utility * utilityobj[NUMSUPERCPUTHREADS];
	helperfunctions2 * helperfunctionsobj[NUMSUPERCPUTHREADS];
	stats * statsobj;
	hostglobalparams_t globalparams;
	
	vertexprop_t * vertexpropertybuffer;
	value_t * vertexdatabuffer;
	container_t * container;
	uint512_vec_dt * kvbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	// vector<keyvalue_t> activevertices;
	// vector<value_t> * activevertices;
};
#endif








