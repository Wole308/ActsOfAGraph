#ifndef ACTSMERGE_H
#define ACTSMERGE_H
#ifdef SW 
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
#endif 
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
#ifndef HW
#include "../../src/utility/utility.h"
#endif
#ifndef HW
#include "../../acts/actsutility/actsutility.h"
#endif 
using namespace std;

class actsmerge {
public:
	actsmerge();
	~actsmerge();
	
	// functions (bit manipulation)
	unsigned int SYNC_GETMASK_UINT(unsigned int index, unsigned int size);
	void SYNC_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	uint32_type CONVERTVMASKTOUINT32(unitBRAMwidth_type vmask[BLOCKRAM_SIZE], unsigned int index);
	
	unsigned int SYNC_amin(unsigned int val1, unsigned int val2);
	value_t mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	value_t applyfunc(value_t vtemp, value_t res, unsigned int GraphIter, unsigned int GraphAlgo);
	globalparams_t getglobalparams(uint512_dt * kvdram);
	batch_type get_num_source_partitions(step_type currentLOP);

	void topkernelsync(uint512_dt * kvdram0, uint512_dt * kvdram1, uint512_dt * kvdram2, uint512_dt * kvdram3);

private:
	#ifndef HW
	actsutility * actsutilityobj;
	#endif
};
#endif 






