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
#ifndef FPGA_IMPL
#include "../../src/utility/utility.h"
#endif 
#ifndef HW
#include "../../acts/actsutility/actsutility.h"
#endif 
#include "procedges.h"
using namespace std;

procedges::procedges(){ actsutilityobj = new actsutility(); }
procedges::~procedges(){}

unsigned int procedgesfunc(unsigned int Uprop, unsigned int edgeweight){
	value_t ret = 0;
	#ifdef PR_ALGORITHM
	ret = Uprop;
	#elif defined(BFS_ALGORITHM)
	ret = Uprop + edgeweight;
	#elif defined(BC_ALGORITHM)
	ret = Uprop + edgeweight;
	#endif
	return ret;
}

void procedges::run(keyvalue_t * vertexptrs, value_t * vertexdatas, keyvalue_t * edges, keyvalue_t * keyvalues, unsigned int srcvoffset, unsigned int srcvsize){							
	unsigned int firstvptr = vertexptrs[srcvoffset].key;
	unsigned int k = 0;
	
	for(unsigned int i=srcvoffset; i<srcvsize; i++){
		unsigned int srcvid = i; // actvvs[i];
		
		cout<<"--------------- procedges::run:: srcvid: "<<srcvid<<endl;
		continue;
		
		unsigned int sourcedata = vertexdatas[srcvid];
		
		keyy_t beginvptr = vertexptrs[srcvid] - firstvptr;
		keyy_t endvptr = vertexptrs[srcvid + 1] - firstvptr;
		edge_t numedges = endvptr - beginvptr;
		
		for(unsigned int j=beginvptr; j<(beginvptr + numedges); j++){
			keyvalue_t edge = edges[j];
			
			keyvalue_t vertexupdate;
			vertexupdate.key = edge.value;
			vertexupdate.value = procedgesfunc(sourcedata, 1);
			
			keyvalues[k] = vertexupdate; 
			k += 1;
		}
	}
	return;
}







