#ifndef ACTS_H
#define ACTS_H
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

class acts {
public:
	acts();
	~acts();
	
	void loadsrcvs( uint512_dt * edges0, uint512_dt * kvdram0,  uint512_dt * edges1, uint512_dt * kvdram1,  uint512_dt * edges2, uint512_dt * kvdram2,  uint512_dt * edges3, uint512_dt * kvdram3,  uint512_dt * vdram);

	void topkernel(
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		uint512_dt * edges0,
		#endif 
		uint512_dt * kvdram0,
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		uint512_dt * edges1,
		#endif 
		uint512_dt * kvdram1,
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		uint512_dt * edges2,
		#endif 
		uint512_dt * kvdram2,
		#ifdef EDGES_IN_SEPERATE_BUFFER_FROM_KVDRAM
		uint512_dt * edges3,
		#endif 
		uint512_dt * kvdram3,
 
		uint512_dt * vdram);
private:
	
};
#endif 






