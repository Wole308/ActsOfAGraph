#ifndef DATASET_H
#define DATASET_H
#include <string.h>
#include <mutex>
#include "../../include/host_common.h"
#include "../../include/common.h"
using namespace std;

class dataset {
public:
	dataset(unsigned int id);
	~dataset();
	
	unsigned int getdatasetid();

private:
	dataset_t datasets[32];
	unsigned int mydatasetid;
};
#endif







