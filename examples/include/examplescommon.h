#ifndef EXAMPLESCOMMON_H
#define EXAMPLESCOMMON_H
#include "../../include/common.h"

typedef struct {
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
} container_t;
#endif

