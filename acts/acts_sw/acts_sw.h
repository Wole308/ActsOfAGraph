#ifndef ACTS_SW_H
#define ACTS_SW_H
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <ctime>
#include <functional>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#include "../../acts/actsutility/actsutility.h"
using namespace std;

class acts_sw {
public:
	acts_sw();
	~acts_sw();
	
	/* void printstats(vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS]);
	
	unsigned int amin(unsigned int val1, unsigned int val2);
	
	unsigned int get_num_source_partitions(unsigned int currentLOP);
	
	unsigned int getskipsize(unsigned int currentLOP, bool_type sourceORdest);
	
	sweepparams_t getsweepparams(unsigned int currentLOP, unsigned int source_partition);
	
	value_t processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);
	
	value_t reducefunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	
	partition_type getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit, unsigned int batch_range_pow); */
	
	void processit(edge_type * edges, edge_t * vptrs, unsigned int * vdatas, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int currentLOP, unsigned int GraphAlgo, unsigned int GraphIter, unsigned int * sourcestatsmarker, unsigned int * deststatsmarker);					
	
	void partitionit(vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], value_t * vdatas, vector<vertex_t> &actvvs_nextit, unsigned int currentLOP, unsigned int GraphAlgo, unsigned int * sourcestatsmarker, unsigned int * deststatsmarker);
	
	void reduceit(vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], value_t * vdatas, vector<vertex_t> &actvvs_nextit, unsigned int currentLOP, unsigned int GraphAlgo, unsigned int GraphIter, unsigned int * sourcestatsmarker, unsigned int * deststatsmarker);
	
	void topkernelproc(edge_type * edges, edge_t * vptrs, unsigned int * vdatas, vector<vertex_t> &actvvs, vector<vertex_t> &actvvs_nextit, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int GraphAlgo, unsigned int GraphIter);
	
private:
	actsutility * actsutilityobj;
};
#endif 






