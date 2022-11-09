#ifndef GOCLKERNEL_H_
#define GOCLKERNEL_H_
#include <chrono>
#include <stdlib.h>
#include <ctime>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <mutex>
#include "algorithm.h"
#include "utility.h"
#include "../include/common.h"
#ifdef FPGA_IMPL
#include "xcl2.hpp"
#include <CL/cl2.hpp>
#include <iostream>
#include <fstream>
#include <CL/cl_ext_xilinx.h>
#endif 
#include "utility.h"
#include "algorithm.h"
#include "../include/common.h"

class app_hw {
public:
	app_hw(universalparams_t _universalparams);
	~app_hw();
	
	#ifdef FPGA_IMPL
	long double app_hw::runapp(std::string binaryFile[2], HBM_channelTHIS_t * HBM_axichannel[2][NUM_PEs], HBM_channelTHIS_t * HBM_axicenter[2], universalparams_t universalparams);					
	#endif
	
private:
	utility * utilityobj;
	universalparams_t myuniversalparams;
};
#endif







