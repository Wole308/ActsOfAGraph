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
	loadgraph();
	~loadgraph();
	
	unsigned int GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	
	bool isbufferused(unsigned int id);
	
	globalparams_t loadvertexdata(std::string algorithm, keyvalue_t * kvbuffer, vertex_t kvbufferoffset_xxx, vertex_t size_xxx, globalparams_t globalparams, unsigned int edgesorkv, unsigned int srcordest);
	
	globalparams_TWOt loadactvvertices(vector<vertex_t> &activevertices, keyy_t * kvbuffer, container_t * container, globalparams_TWOt globalparams);
	
	unsigned int getglobalpartition(keyvalue_t keyvalue, vertex_t upperlimit, unsigned int batch_range_pow, unsigned int treedepth);
	globalparams_TWOt loadoffsetmarkers(keyvalue_t * stats[MAXNUMSUBCPUTHREADS], vector<edge3_type> (&edges_temp)[MAXNUMSUBCPUTHREADS], container_t * container, globalparams_TWOt globalparams);
	void accumstats(uint512_vec_dt * kvbuffer[MAXNUMSUBCPUTHREADS], uint512_vec_dt * stats[MAXNUMSUBCPUTHREADS], globalparams_TWOt globalparams);
	
	globalparams_TWOt generatevmaskdata(vector<vertex_t> &activevertices, uint512_vec_dt * kvbuffer[MAXNUMSUBCPUTHREADS], uint512_vec_dt * vdram, globalparams_TWOt globalparams);
	
	void savevdata(uint512_vec_dt * kvdram, unsigned int buffer[VDATA_PACKINGSIZE][DOUBLE_BLOCKRAM_SIZE], batch_type vbaseoffset_kvs, batch_type voffset_kvs);
	void setrootvid(uint512_vec_dt * kvbuffer, vector<vertex_t> &activevertices, globalparams_t globalparams);
	
	globalparams_TWOt loadmessages(uint512_vec_dt * mdram, uint512_vec_dt * vdram, uint512_vec_dt * edges[MAXNUMSUBCPUTHREADS], uint512_vec_dt * kvbuffer[MAXNUMSUBCPUTHREADS], container_t * container, unsigned int GraphIter, globalparams_TWOt globalparams);				
	globalparams_t createmessages(
			unsigned int id,
			uint512_vec_dt * kvstats,
			unsigned int srcvoffset,
			unsigned int srcvsize,
			unsigned int edgessize,
			unsigned int destvoffset,
			unsigned int actvvsize,
			unsigned int GraphIter,
			unsigned int runsize,
			unsigned int numprocesspartitions, 
			unsigned int numreducepartitions, 
			globalparams_t globalparams,
			universalparams_t universalparams);	

	void setcustomeval(uint512_vec_dt * vdram, uint512_vec_dt * kvbuffer[MAXNUMSUBCPUTHREADS], unsigned int evalid);
	
	globalparams_t finishglobaparamsV(globalparams_t globalparams);
	
	globalparams_t finishglobaparamsM(globalparams_t globalparams);

private:
	utility * utilityobj;
	evalparams * evalparamsobj;
	universalparams_t universalparams;
};
#endif







