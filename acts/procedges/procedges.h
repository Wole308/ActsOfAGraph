#ifndef PEDGES_H
#define PEDGES_H
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
#include "../../include/common.h"
#include "../include/actscommon.h"
#include "../../src/utility/utility.h"
#include "../../acts/actsutility/actsutility.h"
using namespace std;

class procedges {
public:
	procedges();
	~procedges();
	
	unsigned int processedgefunc(unsigned int Uprop, unsigned int edgeweight);
	void run(keyvalue_t * vertexptrs, keyvalue_t * vertexdatas, keyvalue_t * edges, keyvalue_t * keyvalues, unsigned int srcvoffset, unsigned int srcvsize);											

private:
	actsutility * actsutilityobj;
};
#endif 







