#ifndef LOADGRAPH_H
#define LOADGRAPH_H
#include <mutex>
#include <thread>
#include <math.h> 
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
#include "utility.h"
#include "algorithm.h"
#include "evalparams.h"
#include "../include/common.h"
using namespace std;

#define EDGESTATUS_VALIDEDGE 5
#define EDGESTATUS_INVALIDEDGE 6
#define EDGESTATUS_BITMAP 7
#define EDGESTATUS_SOURCEV 8
#define EDGESTATUS_DUMMYEDGE 9

class loadgraph {
public:
	loadgraph(universalparams_t _universalparams);
	~loadgraph();
	
	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth, universalparams_t universalparams);
	
	globalparams_t loadvertexdata(std::string algorithm, keyvalue_t * kvbuffer, vertex_t kvbufferoffset_xxx, vertex_t size_xxx, globalparams_t globalparams, unsigned int edgesorkv, unsigned int srcordest, universalparams_t universalparams);
	
	globalparams_TWOt loadactvvertices(vector<vertex_t> &activevertices, globalparams_TWOt globalparams, universalparams_t universalparams);
	
	globalparams_TWOt loadoffsetmarkers(keyvalue_t * stats[MAXNUM_PEs], vector<edge2_vec_dt> (&edges_final)[MAXNUM_PEs], container_t * container, globalparams_TWOt globalparams, universalparams_t universalparams);
	void accumstats(uint512_vec_dt * kvbuffer[MAXNUM_PEs], uint512_vec_dt * stats[MAXNUM_PEs], globalparams_TWOt globalparams, universalparams_t universalparams);
	
	globalparams_TWOt loadmaps(vector<vertex_t> &activevertices, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUM_PEs], map_t * edges_map[MAXNUM_PEs][MAXNUM_VPs], keyvalue_t * actvedges_map[MAXNUM_PEs][MAXNUM_VPs], globalparams_TWOt globalparams, universalparams_t universalparams);
	
	void savevdata(uint512_vec_dt * kvdram, unsigned int buffer[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs, universalparams_t universalparams);
	void setrootvid(uint512_vec_dt * kvbuffer, vector<vertex_t> &activevertices, globalparams_t globalparams, universalparams_t universalparams);
	
	globalparams_TWOt loadmessages(uint512_vec_dt * mdram, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUM_PEs], uint512_vec_dt * kvbuffer[MAXNUM_PEs], container_t * container, globalparams_TWOt globalparams, universalparams_t universalparams);				
	globalparams_t createmessages(
			unsigned int id,
			uint512_vec_dt * kvstats,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int edgessize,
			unsigned int destvoffset,
			unsigned int actvvsize,
			unsigned int runsize,
			unsigned int numprocesspartitions, 
			unsigned int numreducepartitions, 
			globalparams_t globalparams,
			universalparams_t universalparams);	

	void setcustomeval(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[MAXNUM_PEs], unsigned int evalid);
	
	globalparams_t finishglobaparamsV(globalparams_t globalparams, universalparams_t universalparams);
	
	globalparams_t finishglobaparamsM(globalparams_t globalparams, universalparams_t universalparams);

private:
	utility * utilityobj;
	evalparams * evalparamsobj;
	universalparams_t myuniversalparams;
};
#endif







