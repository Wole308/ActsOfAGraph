#ifndef EXAMPLESCOMMON_H
#define EXAMPLESCOMMON_H
#include "../../include/common.h"

typedef struct {
	unsigned int srcvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int srcvsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int destvoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int actvvsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int firstvid[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int edgeoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int edgessize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchoffset[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int batchsize[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	unsigned int runsize[NUMCPUTHREADS][NUMSUBCPUTHREADS]; 
	unsigned int numedgesretrieved[NUMSSDPARTITIONS];
	unsigned int totalnumedgesinfile[NUMSSDPARTITIONS];
	edge_t * vertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	edge_type * edgesbuffer[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	// edge_t * tempvertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS];
	edge_t tempvertexptrs[NUMCPUTHREADS][NUMSUBCPUTHREADS][2];
	edge_type tempedges[NUMCPUTHREADS][NUMSUBCPUTHREADS][2];
} container_t;
#endif

