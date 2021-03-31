#ifndef ACTSPROC_SW_H
#define ACTSPROC_SW_H
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

class actsproc_sw {
public:
	actsproc_sw();
	~actsproc_sw();
	
	unsigned int get_num_source_partitions(unsigned int currentLOP);
	
	unsigned int getskipsize(unsigned int currentLOP, bool_type sourceORdest);
	
	sweepparams_t getsweepparams(unsigned int currentLOP, unsigned int source_partition);
	
	value_t processfunc(value_t udata, value_t edgew, unsigned int GraphAlgo);
	
	partition_type getpartition(keyvalue_t keyvalue, unsigned int currentLOP, vertex_t upperlimit, unsigned int batch_range_pow);
	
	void processit(edge_type * edges, edge_t * vptrs, unsigned int * vdatas, vector<vertex_t> &actvvs, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int currentLOP, unsigned int GraphAlgo);					
	
	void partitionit(vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int currentLOP, unsigned int GraphAlgo);
	
	void topkernelproc(edge_type * edges, edge_t * vptrs, unsigned int * vdatas, vector<vertex_t> &actvvs, vector<keyvalue_t> (&kvdram)[TOTALNUMPARTITIONS], unsigned int GraphAlgo);
	
private:
	actsutility * actsutilityobj;
};
#endif 






