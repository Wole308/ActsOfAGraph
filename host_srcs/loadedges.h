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
	
	unsigned int gethash(unsigned int vid);
	unsigned int getlocalvid(unsigned int vid);

	void getXYLayoutVx8(unsigned int s, unsigned int depths[EDGEDATA_PACKINGSIZE]);
	edge2_vec_dt rearrangeLayoutVx8(unsigned int s, edge2_vec_dt edge_vec);
	
	void getXYLayoutVx16(unsigned int s, unsigned int depths[EDGEDATA_PACKINGSIZE]);
	edge2_vec_dt rearrangeLayoutVx16F(unsigned int s, edge2_vec_dt edge_vec);
	
	edge2_vec_dt rearrangeLayoutVx16B(unsigned int s, edge2_vec_dt edge_vec);
	
	void getXYLayoutV(unsigned int s, unsigned int depths[EDGEDATA_PACKINGSIZE]);
	edge2_vec_dt rearrangeLayoutV(unsigned int s, edge2_vec_dt edge_vec);
	
	globalparams_TWOt start(unsigned int col, vector<edge_t> &vertexptrbuffer, vector<edge2_type> &edgedatabuffer, vptr_type * vptrs[MAXNUM_PEs], uint512_vec_dt * edges[MAXNUM_PEs], vector<edge2_vec_dt> (&edges_final)[MAXNUM_PEs], map_t * edges_map[MAXNUM_PEs][MAXNUM_VPs], uint512_vec_dt * edgeblock_map[MAXNUM_PEs][MAXNUM_VPs], unsigned int * edgeblock_stats[MAXNUM_PEs], container_t * container, globalparams_TWOt globalparams);								

private:
	utility * utilityobj;
	evalparams * evalparamsobj;
	universalparams_t universalparams;
};
#endif







