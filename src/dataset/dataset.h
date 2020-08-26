#ifndef DATASET_H
#define DATASET_H
#include <string.h>
#include <mutex>
#include "../../include/host_common.h"
#include "../../include/common.h"
using namespace std;

class dataset {
public:
	dataset();
	~dataset();
	
	unsigned int getdatasetid();
	void loadalldatasets();

private:
	dataset_t datasets[32];
};
#endif







