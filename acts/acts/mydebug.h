#ifndef DEBUG_H
#define DEBUG_H
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
#include "mydebug.h"
#include "../../include/config_params.h"
#include "../../include/common.h"
#include "../include/actscommon.h"
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

class mydebug {
public:
	mydebug();
	~mydebug();
	
	void set(unsigned int index1, unsigned int index2, unsigned int data);
	unsigned int get(unsigned int index1, unsigned int index2);
	void increment(unsigned int index1, unsigned int index2, unsigned int incr);
	
	// functions (bit manipulation)
	unsigned int DEBUG_GETMASK_UINT(unsigned int index, unsigned int size);
	unsigned int DEBUG_READFROM_UINT(unsigned int data, unsigned int index, unsigned int size);
	void DEBUG_WRITETO_UINT(unsigned int * data, unsigned int index, unsigned int size, unsigned int value);
	unsigned int DEBUG_READBITSFROM_UINTV(uint32_type data, unsigned int index, unsigned int size);
	void DEBUG_WRITEBITSTO_UINTV(uint32_type * data, unsigned int index, unsigned int size, unsigned int value);
	
	void printvdata(value_t vdata[VECTOR2_SIZE], unsigned int data1, unsigned int data2);
	void printvdatas(keyvalue_vbuffer_t vdatas[VDATA_PACKINGSIZE][BLOCKRAM_SIZE], unsigned int data1, unsigned int data2, unsigned int limit);
	void printvdatawithvmask(value_t vdata[VECTOR2_SIZE], unsigned int data1, unsigned int data2);
	
private:
	unsigned int global_datas[8][128];
};
#endif








