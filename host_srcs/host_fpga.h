#ifndef HOST_FPGA_H_
#define HOST_FPGA_H_
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

class host_fpga {
public:
	host_fpga(universalparams_t _universalparams);
	~host_fpga();
	
	long double runapp(action_t action, std::string binaryFile[2], HBM_channelAXISW_t * HBM_axichannel[2][NUM_PEs], HBM_channelAXISW_t * HBM_axicenter[2], unsigned int globalparams[1024], universalparams_t universalparams);					
	
private:
	utility * utilityobj;
	universalparams_t myuniversalparams;
};
#endif







