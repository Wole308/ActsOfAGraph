#ifndef LOADGRAPH_RANDOM_H
#define LOADGRAPH_RANDOM_H
#include <mutex>
#include <thread>
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
#include "evalparams.h"
#include "utility.h"
#include "algorithm.h"
#include "../include/common.h"
#include "loadedges.h"
using namespace std;

#define EDGESTATUS_VALIDEDGE 5
#define EDGESTATUS_INVALIDEDGE 6
#define EDGESTATUS_BITMAP 7
#define EDGESTATUS_SOURCEV 8
#define EDGESTATUS_DUMMYEDGE 9

class loadedges {
public:
	loadedges(universalparams_t _universalparams);
	~loadedges();
	
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	
	unsigned int allignlower(unsigned int val, unsigned int V_SIZE);
	unsigned int allignhigher(unsigned int val, unsigned int V_SIZE);
	
	unsigned int gethash(unsigned int vid);
	unsigned int getlocalvid(unsigned int vid);

	bool isbufferused(unsigned int id);
	
	unsigned int getpartition(unsigned int data, vertex_t upperlimit);
	void calculateoffsets(keyvalue_t * buffer, unsigned int size);
	void getXYLayoutV(unsigned int s, unsigned int depths[VECTOR_SIZE]);
	edge2_vec_dt rearrangeLayoutV(unsigned int s, edge2_vec_dt edge_vec);
	
	globalparams_TWOt start(unsigned int col, vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, vptr_type * vptrs[MAXNUM_PEs], uint512_vec_dt * edges[MAXNUM_PEs], vector<edge2_vec_dt> (&edges_final)[MAXNUM_PEs], map_t * edges_map[MAXNUM_PEs][MAXNUM_VPs], container_t * container, globalparams_TWOt globalparams);

private:
	utility * utilityobj;
	evalparams * evalparamsobj;
	universalparams_t universalparams;
};
#endif







