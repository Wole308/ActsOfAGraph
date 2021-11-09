#ifndef ACTSSYNC_H
#define ACTSSYNC_H
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
#include "../../acts/actsutility/actsutility.h"
using namespace std;

class actsmerge {
public:
	actsmerge();
	~actsmerge();
	
	unsigned int SYNC_amin(unsigned int val1, unsigned int val2);
	value_t mergefunc(value_t value1, value_t value2, unsigned int GraphAlgo);
	globalparams_t SYNC_getglobalparams(uint512_dt * kvdram);

	void topkernelmerge(uint512_dt * kvdram0, uint512_dt * kvdram1, uint512_dt * kvdram2, uint512_dt * kvdram3);

private:
	#ifndef FPGA_IMPL
	actsutility * actsutilityobj;
	#endif
};
#endif 






