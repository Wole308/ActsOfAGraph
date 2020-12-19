#ifndef EXAMPLESCOMMON_H
#define EXAMPLESCOMMON_H
#include "../../include/common.h"

typedef struct {
	unsigned int srcvoffset[NUMSUBCPUTHREADS];
	unsigned int srcvsize[NUMSUBCPUTHREADS];
	unsigned int destvoffset[NUMSUBCPUTHREADS];
	unsigned int actvvsize[NUMSUBCPUTHREADS];
	unsigned int firstvid[NUMSUBCPUTHREADS];
	unsigned int edgeoffset[NUMSUBCPUTHREADS];
	unsigned int edgessize[NUMSUBCPUTHREADS];
	unsigned int batchoffset[NUMSUBCPUTHREADS];
	unsigned int batchsize[NUMSUBCPUTHREADS];
	unsigned int runsize[NUMSUBCPUTHREADS]; 
	unsigned int numedgesretrieved[NUMSSDPARTITIONS];
	unsigned int totalnumedgesinfile[NUMSSDPARTITIONS];
	edge_t * vertexptrs[NUMSUBCPUTHREADS];
	edge2_type * edgesbuffer[NUMSUBCPUTHREADS];
	edge_t tempvertexptrs[NUMSUBCPUTHREADS][2];
	edge2_type tempedges[NUMSUBCPUTHREADS][2];
} container_t;
#endif

